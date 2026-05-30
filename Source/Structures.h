#pragma once

#include <atomic>
#include <cstring>
#include <cstdint>

#include "Enums.h"

#define PSE_COMMAND_BUFFER_SIZE 2048
#define PSE_EVENT_BUFFER_SIZE 256

typedef uint64_t PseGUID;

typedef struct { float x, y; } PseVector2;
typedef struct { float x, y, z; } PseVector3;
typedef struct { float x, y, z, w; } PseQuat;
typedef struct { PseQuat quat; PseVector3 location; PseVector3 scale; } PseTransform;
typedef struct { uint8_t r, g, b, a; } PseColor;

struct PseBufferData
{
    union
    {
        PseCommand command;
        PseEvent event;
        PseResult result;
    };
	
    uint8_t data[60];
	
#ifdef __cplusplus
    template<int32_t offset, typename T>
    T get() const { return *reinterpret_cast<const T*>(&data[offset]); }
	
    template<int32_t offset, typename T>
    PseBufferData& set(const T& inData) { std::memcpy(&data[offset], &inData, sizeof(T)); return *this; }
    
    template<typename T>
    T get(int32_t offset) const { return *reinterpret_cast<const T*>(&data[offset]); }
	
    template<typename T>
    PseBufferData& set(int32_t offset, const T& inData) { std::memcpy(&data[offset], &inData, sizeof(T)); return *this; }
#endif
};

struct PseCommandBuffer
{
    alignas(64) PseBufferData buffer[PSE_COMMAND_BUFFER_SIZE] = {};
    alignas(64) std::atomic<uint32_t> pseIndex{0};  // (Writer/Producer)
    alignas(64) std::atomic<uint32_t> gameIndex{0}; // (Reader/Consumer)
    
#ifdef __cplusplus
    /* */ PseBufferData& operator[](uint32_t absoluteIndex) /* */ { return (/* */ PseBufferData&)buffer[absoluteIndex & (PSE_COMMAND_BUFFER_SIZE - 1)]; }
    const PseBufferData& operator[](uint32_t absoluteIndex) const { return (const PseBufferData&)buffer[absoluteIndex & (PSE_COMMAND_BUFFER_SIZE - 1)]; }
    
	void synchronize() const
    {
        uint32_t pse = pseIndex.load(std::memory_order_relaxed);
        while(true)
        {
            uint32_t game = gameIndex.load(std::memory_order_acquire);
            if((int32_t)(game - pse) >= 0) break;
            _mm_pause();
        }
    }
    
#pragma region SINGLE - Immediate Access (One-by-one)
    uint32_t singlePush(const PseBufferData& data)
    {
        while(true)
        {
            uint32_t pse = pseIndex.load(std::memory_order_relaxed);
            uint32_t game = gameIndex.load(std::memory_order_acquire);

            if(pse - game < PSE_COMMAND_BUFFER_SIZE)
            {
                (*this)[pse] = data;
                pseIndex.store(pse + 1, std::memory_order_release);
                return pse;
            }
            _mm_pause();
        }
    }
    
    PseBufferData singleWait(uint32_t index) const
    {
        while(true)
        {
            uint32_t game = gameIndex.load(std::memory_order_acquire);
            
            if((int32_t)(game - index) > 0)
            {
                return (*this)[index];
            }
            _mm_pause();
        }
    }
    
    PseBufferData singlePushAndWait(const PseBufferData& data) { return singleWait(singlePush(data)); }
#pragma endregion
    
#pragma region BATCH - Batch Access (Copying)
    uint32_t batchPush(const PseBufferData* pData, uint32_t count)
    {
        if(count == 0) return 0;

        while(true)
        {
            uint32_t pse = pseIndex.load(std::memory_order_relaxed);
            uint32_t game = gameIndex.load(std::memory_order_acquire);
            
            if(pse - game + count <= PSE_COMMAND_BUFFER_SIZE)
            {
                rawCopy((void*)pData, pse, count, true);
                pseIndex.store(pse + count, std::memory_order_release);
                return pse;
            }
            _mm_pause();
        }
    }
    
    void batchWait(uint32_t index, PseBufferData* pData, uint32_t count)
    {
        if(count == 0) return;
        
        while(true)
        {
            uint32_t game = gameIndex.load(std::memory_order_acquire);
            if((int32_t)(game - (index + count)) >= 0)
            {
                rawCopy(pData, index, count, false);
                return;
            }
            _mm_pause();
        }
    }
    
    void batchPushAndWait(PseBufferData* pData, uint32_t count)
    {
        batchWait(batchPush(pData, count), pData, count);
    }
#pragma endregion
    
#pragma region RAW - In-Place Access (Zero-Copy)
    uint32_t rawReserve(uint32_t count) const
    {
        while(true)
        {
            uint32_t pse = pseIndex.load(std::memory_order_relaxed);
            uint32_t game = gameIndex.load(std::memory_order_acquire);
            if(pse - game + count <= PSE_COMMAND_BUFFER_SIZE) return pse;
            _mm_pause();
        }
    }
    
    void rawCommit(uint32_t startIndex, uint32_t count) { pseIndex.store(startIndex + count, std::memory_order_release); }
    
    void rawWait(uint32_t index, uint32_t count) const
    {
        if(count == 0) return;
        
        while(true)
        {
            uint32_t game = gameIndex.load(std::memory_order_acquire);
            if((int32_t)(game - (index + count)) >= 0) return;
            _mm_pause();
        }
    }
    
    void rawCommitAndWait(uint32_t startIndex, uint32_t count) { rawCommit(startIndex, count); rawWait(startIndex, count); }
#pragma endregion
    
private:
    void rawCopy(void* pExternalData, uint32_t absoluteIndex, uint32_t count, bool bToBuffer)
    {
        uint32_t spaceAtEnd = PSE_COMMAND_BUFFER_SIZE - (absoluteIndex & (PSE_COMMAND_BUFFER_SIZE - 1));
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
                bToBuffer ? (uint8_t*)&buffer[0] : (uint8_t*)pExternalData + BytesAtEnd,
                bToBuffer ? (uint8_t*)pExternalData + BytesAtEnd : (uint8_t*)&buffer[0],
                (count - spaceAtEnd) * 64
            );
        }
    }
#endif
};

struct PseEventBuffer
{
    alignas(64) PseBufferData buffer[PSE_EVENT_BUFFER_SIZE] = {};
    alignas(64) std::atomic<uint32_t> gameIndex{0};
    static inline uint32_t pseIndex = 0;
    
#ifdef __cplusplus
    /* */ PseBufferData& operator[](uint32_t absoluteIndex) /* */ { return (/* */ PseBufferData&)buffer[absoluteIndex & (PSE_EVENT_BUFFER_SIZE - 1)]; }
    const PseBufferData& operator[](uint32_t absoluteIndex) const { return (const PseBufferData&)buffer[absoluteIndex & (PSE_EVENT_BUFFER_SIZE - 1)]; }
    
    bool tryGet(PseBufferData& outPseBufferData)
    {
        uint32_t game = gameIndex.load(std::memory_order_acquire);
        if((int32_t)(game - pseIndex) <= 0) return false;
        
        outPseBufferData = (*this)[pseIndex];
        ++pseIndex;
        return true;
    }
#endif
};

#pragma pack(push, 1)
struct PseBuffers
{
    PseCommandBuffer commandBuffer;
    PseEventBuffer eventBuffer;
};
#pragma pack(pop)
