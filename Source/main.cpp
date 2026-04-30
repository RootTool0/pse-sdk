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

#define PSE_BUFFER_SIZE 2048

struct PseBuffer
{

public:
    void synchronize() const
    {
        uint32_t pse = PseIndex.load(std::memory_order_relaxed);
        while (true)
        {
            uint32_t game = GameIndex.load(std::memory_order_acquire);
            if((int32_t)(game - pse) >= 0) break;
            _mm_pause();
        }
    }
    
    /* */ PseData& operator[](uint32_t absoluteIndex) /* */ { return (/* */ PseData&)Buffer[absoluteIndex & (PSE_BUFFER_SIZE - 1)]; }
    const PseData& operator[](uint32_t absoluteIndex) const { return (const PseData&)Buffer[absoluteIndex & (PSE_BUFFER_SIZE - 1)]; }
    
#pragma region Immediate Access (One-by-one)
    uint32_t push(const PseData& data)
    {
        while (true)
        {
            uint32_t pse = PseIndex.load(std::memory_order_relaxed);
            uint32_t game = GameIndex.load(std::memory_order_acquire);

            if(pse - game < PSE_BUFFER_SIZE)
            {
                (*this)[pse] = data;
                PseIndex.store(pse + 1, std::memory_order_release);
                return pse;
            }
            _mm_pause();
        }
    }
    
    PseData wait(uint32_t index) const
    {
        while (true)
        {
            uint32_t game = GameIndex.load(std::memory_order_acquire);

            if((int32_t)(game - index) > 0)
            {
                return (*this)[index];
            }
            _mm_pause();
        }
    }
    
    PseData pushAndWait(const PseData& data) { return wait(push(data)); }
#pragma endregion
    
#pragma region Batch Access (Copying)
    uint32_t pushBatch(const PseData* pData, uint32_t count)
    {
        static_assert(sizeof(PseData) == 64, "Structure must be exactly 64 bytes!");
        if(count == 0) return 0;

        while(true)
        {
            uint32_t pse = PseIndex.load(std::memory_order_relaxed);
            uint32_t game = GameIndex.load(std::memory_order_acquire);
            
            if(pse - game + count <= PSE_BUFFER_SIZE)
            {
                this->rawCopy((void*)pData, pse, count, true);
                PseIndex.store(pse + count, std::memory_order_release);
                return pse;
            }
            _mm_pause();
        }
    }
    
    void waitBatch(uint32_t index, PseData* pData, uint32_t count)
    {
        static_assert(sizeof(PseData) == 64, "Structure must be exactly 64 bytes!");
        if(count == 0) return;

        while(true)
        {
            uint32_t game = GameIndex.load(std::memory_order_acquire);
            if((int32_t)(game - (index + count)) >= 0)
            {
                this->rawCopy(pData, index, count, false);
                return;
            }
            _mm_pause();
        }
    }
    
    void pushAndWaitBatch(PseData* pData, uint32_t count)
    {
        static_assert(sizeof(PseData) == 64, "Structure must be exactly 64 bytes!");
        waitBatch(pushBatch(pData, count), pData, count);
    }
#pragma endregion

#pragma region In-Place Access (Zero-Copy)
    uint32_t reserveRaw(uint32_t count) const
    {
        while(true)
        {
            uint32_t pse = PseIndex.load(std::memory_order_relaxed);
            uint32_t game = GameIndex.load(std::memory_order_acquire);
            if(pse - game + count <= PSE_BUFFER_SIZE) return pse;
            _mm_pause();
        }
    }
    
    void commitRaw(uint32_t startIndex, uint32_t count) { PseIndex.store(startIndex + count, std::memory_order_release); }
    
    void waitRaw(uint32_t index, uint32_t count) const
    {
        if(count == 0) return;
        
        while(true)
        {
            uint32_t game = GameIndex.load(std::memory_order_acquire);
            if((int32_t)(game - (index + count)) >= 0) return;
            _mm_pause();
        }
    }
#pragma endregion

private:
    alignas(64) PseData Buffer[PSE_BUFFER_SIZE] = {};
    alignas(64) std::atomic<uint32_t> PseIndex{0}; // (Writer/Producer)
    alignas(64) std::atomic<uint32_t> GameIndex{0}; // (Reader/Consumer)
    
    void rawCopy(void* pExternalData, uint32_t absoluteIndex, uint32_t count, bool bToBuffer)
    {
        uint32_t spaceAtEnd = PSE_BUFFER_SIZE - (absoluteIndex & (PSE_BUFFER_SIZE - 1));
        void* pInternalStart = &(*this)[absoluteIndex]; 
        
        if(count <= spaceAtEnd)
        {
            std::memcpy
            (
                bToBuffer ? pInternalStart : pExternalData,
                bToBuffer ? pExternalData : pInternalStart,
                count * 64
            );
        }
        else
        {
            uint32_t BytesAtEnd = spaceAtEnd * 64;
            
            std::memcpy
            (
                bToBuffer ? pInternalStart : pExternalData,
                bToBuffer ? pExternalData : pInternalStart,
                BytesAtEnd
            );
            
            std::memcpy
            (
                bToBuffer ? (uint8_t*)&Buffer[0] : (uint8_t*)pExternalData + BytesAtEnd,
                bToBuffer ? (uint8_t*)pExternalData + BytesAtEnd : (uint8_t*)&Buffer[0],
                (count - spaceAtEnd) * 64
            );
        }
    }
};

static PseBuffer* gPseBuffer = nullptr;
static PseID IDs[64];

int main()
{
    HANDLE hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "Global\\PortalSolverEditorSDK");
    if(!hMapFile)
    {
        std::cout << "Could not open file mapping object: " << GetLastError() << '\n';
        return -1;
    }
    
    void* pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(PseBuffer));
    if (pBuf == nullptr)
    {
        std::cout << "Could not map view of file: " << GetLastError() << '\n';
        CloseHandle(hMapFile);
        return -2;
    }
    
    std::random_device rd;
    std::mt19937 Generator(rd());
    std::bernoulli_distribution Random(0.5);
    
    gPseBuffer = static_cast<PseBuffer*>(pBuf);
    
    /*
    for(uint8_t i = 0; i < 64; ++i)
    {
        PseData Data = PseData::CreateCommand(PSE_COMMAND_DYNAMIC_MESH_CREATE)
            .Set<0, PseMesh>(PSE_MESH_FACE)
            .Set<4, PseMaterial>(PSE_MATERIAL_FLOOR_BLACK)
            .Set<8, PseTransform>(PseTransform(PseVector((i % 8) * 200.0f, (i / 8) * 200.0f, 0)));
        
        IDs[i] = gBuffer->PushAndWait<PseData>(Data).Get<0, PseID>();
    }
    */
    
    /*
    PseData TempDataArray[64];
    for(uint8_t i = 0; i < 64; ++i)
    {
        TempDataArray[i] = PseData::CreateCommand(PSE_COMMAND_DYNAMIC_MESH_CREATE)
            .Set<0, PseMesh>(PSE_MESH_FACE)
            .Set<4, PseMaterial>(PSE_MATERIAL_FLOOR_BLACK)
            .Set<8, PseTransform>(PseTransform(PseVector((i % 8) * 200.0f, (i / 8) * 200.0f, 0)));
    }
    
    gBuffer->PushAndWaitArray(TempDataArray, 64);
    
    for(int i = 0; i < 64; ++i)
        IDs[i] = TempDataArray[i].Get<0, PseID>();
    */

    {
        uint32_t startIndex = gPseBuffer->reserveRaw(64);
        for(uint8_t i = 0; i < 64; ++i)
        {
            gPseBuffer->operator[](startIndex + i) = PseData::CreateCommand(PSE_COMMAND_DYNAMIC_MESH_CREATE)
                .Set<0, PseMesh>(PSE_MESH_FACE)
                .Set<4, PseMaterial>(PSE_MATERIAL_FLOOR_BLACK)
                .Set<8, PseTransform>(PseTransform(PseVector((i % 8) * 200.0f, (i / 8) * 200.0f, 0)));
        }
        gPseBuffer->commitRaw(startIndex, 64);
    
        gPseBuffer->waitRaw(startIndex, 64);
        for(int i = 0; i < 64; ++i)
            IDs[i] = gPseBuffer->operator[](startIndex + i).Get<0, PseID>();
    }
    
    while(true)
    {
        
        /*
        for(uint8_t ID : IDs)
        {
            PseData Data = PseData::CreateCommand(PSE_COMMAND_DYNAMIC_MESH_SET_MATERIAL)
                .Set<0, PseID>(IDs[ID])
                .Set<8, PseMaterial>(Random(Generator) ? PSE_MATERIAL_FLOOR_WHITE : PSE_MATERIAL_FLOOR_BLACK);
            
            gPseBuffer->push(Data);
        }
        */
        
        uint32_t startIndex = gPseBuffer->reserveRaw(64);
        for(uint8_t i = 0; i < 64; ++i)
        {
            gPseBuffer->operator[](startIndex + i) = PseData::CreateCommand(PSE_COMMAND_DYNAMIC_MESH_SET_MATERIAL)
                .Set<0, PseID>(IDs[i])
                .Set<8, PseMaterial>(Random(Generator) ? PSE_MATERIAL_FLOOR_WHITE : PSE_MATERIAL_FLOOR_BLACK);
        }
        gPseBuffer->commitRaw(startIndex, 64);
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    if(gPseBuffer) UnmapViewOfFile(gPseBuffer);
    if(hMapFile) CloseHandle(hMapFile);
    
    return 0;
}
