// ReSharper disable CppClangTidyCppcoreguidelinesNarrowingConversions
// ReSharper disable CppClangTidyClangDiagnosticImplicitIntFloatConversion
#include <atomic>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <Windows.h>

#include "CoreTypes.h"
#include "Types.h"

#define BUFFER_SIZE 2048

struct FBuffer
{

public:
    template<typename T>
    uint32_t Push(const T& Value)
    {
        static_assert(sizeof(T) == 64, "Structure must be exactly 64 bytes!");

        while (true)
        {
            uint32_t Pse = PseIndex.load(std::memory_order_relaxed);
            uint32_t Game = GameIndex.load(std::memory_order_acquire);

            if (Pse - Game < BUFFER_SIZE)
            {
                std::memcpy(&Buffer[Pse & (BUFFER_SIZE - 1)], &Value, 64);
                PseIndex.store(Pse + 1, std::memory_order_release);
                return Pse;
            }
            _mm_pause();
        }
    }
    
    template<typename T>
    T Wait(uint32_t SlotIndex)
    {
        static_assert(sizeof(T) == 64, "Structure must be exactly 64 bytes!");

        while (true)
        {
            uint32_t Game = GameIndex.load(std::memory_order_acquire);

            if ((int32_t)(Game - SlotIndex) > 0)
            {
                T Result;
                std::memcpy(&Result, &Buffer[SlotIndex & (BUFFER_SIZE - 1)], 64);
                return Result;
            }
            _mm_pause();
        }
    }
    
    template<typename T>
    T PushAndWait(const T& Value)
    {
        static_assert(sizeof(T) == 64, "Structure must be exactly 64 bytes!");
        return Wait<T>(Push<T>(Value));
    }
    
    void Synchronize() const
    {
        uint32_t Pse = PseIndex.load(std::memory_order_relaxed);
        while (true)
        {
            uint32_t Game = GameIndex.load(std::memory_order_acquire);
            if ((int32_t)(Game - Pse) >= 0) break;
            _mm_pause();
        }
    }
    
private:
    struct alignas(64) FEmptySlot{};
    alignas(64) FEmptySlot Buffer[BUFFER_SIZE];
    alignas(64) std::atomic<uint32_t> PseIndex{0}; // (Writer/Producer)
    alignas(64) std::atomic<uint32_t> GameIndex{0}; // (Reader/Consumer)
};

static FBuffer* GBuffer = nullptr;
static uint8_t IDs[8][8];

int main()
{
    HANDLE hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "Global\\PortalSolverEditorSDK");
    if(!hMapFile)
    {
        std::cout << "Could not open file mapping object: " << GetLastError() << '\n';
        return -1;
    }
    
    void* pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(FBuffer));
    if (pBuf == nullptr)
    {
        std::cout << "Could not map view of file: " << GetLastError() << '\n';
        CloseHandle(hMapFile);
        return -2;
    }
    
    GBuffer = static_cast<FBuffer*>(pBuf);
    
    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
        {
            PseData Data = PseData::CreateCommand(PSE_COMMAND_DYNAMIC_MESH_CREATE);
            Data.Set<0, PseMesh>(PSE_MESH_FACE);
            Data.Set<4, PseMaterial>(PSE_MATERIAL_FLOOR_BLACK);
            Data.Set<8, PseTransform>(PseTransform
            (
                PseVector(x * 200.0f, y * 200.0f, 0),
                PseQuat  (0, 0, 0, 1),
                PseVector(1.0f)
            ));
        
            IDs[x][y] = GBuffer->PushAndWait<PseData>(Data).Get<0, PseID>();
        }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution d(0.5);
    
    while(true)
    {
        // GBuffer->Synchronize();
        
        for (int x = 0; x < 8; ++x)
            for (int y = 0; y < 8; ++y)
                GBuffer->Push<PseData>(
                    PseData::CreateCommand(PSE_COMMAND_DYNAMIC_MESH_SET_MATERIAL)
                        .Set<0, PseID>(IDs[x][y])
                        .Set<8, PseMaterial>(d(gen) ? PSE_MATERIAL_FLOOR_WHITE : PSE_MATERIAL_FLOOR_BLACK)
                );
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    if(GBuffer) UnmapViewOfFile(GBuffer);
    if(hMapFile) CloseHandle(hMapFile);
    
    return 0;
}
