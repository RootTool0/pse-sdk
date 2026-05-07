#include "PSE.h"

#include <Windows.h>

static inline PseBuffers* gPseBuffers = nullptr;
static inline PseCommandBuffer* gPseCommandBuffer = nullptr;
static inline PseEventBuffer* gPseEventBuffer = nullptr;
static inline void* gPseSharedMemoryHandle = nullptr;

// PseBufferData / Create
void pseBufferDataCreateCommand(PseBufferData* pBufferData, PseCommand command) { if(pBufferData){ std::memset(pBufferData, 0, sizeof(PseBufferData)); pBufferData->command = command; }}
void pseBufferDataCreateResult (PseBufferData* pBufferData, PseResult  result ) { if(pBufferData){ std::memset(pBufferData, 0, sizeof(PseBufferData)); pBufferData->result  = result;  }}
void pseBufferDataCreateEvent  (PseBufferData* pBufferData, PseEvent   event  ) { if(pBufferData){ std::memset(pBufferData, 0, sizeof(PseBufferData)); pBufferData->event   = event;   }}

// PseBufferData / Read
PseCommand pseBufferDataReadCommand(const PseBufferData* pBufferData) { return pBufferData ? pBufferData->command : PSE_COMMAND_NONE; }
PseResult  pseBufferDataReadResult (const PseBufferData* pBufferData) { return pBufferData ? pBufferData->result  : PSE_RESULT_NONE;  }
PseEvent   pseBufferDataReadEvent  (const PseBufferData* pBufferData) { return pBufferData ? pBufferData->event   : PSE_EVENT_NONE;   }

// PseBufferData / Setters
void pseBufferDataSetInt8 (PseBufferData* pBufferData, int32_t offset, int8_t  value) { if(pBufferData) pBufferData->Set<int8_t >(offset, value); }
void pseBufferDataSetInt16(PseBufferData* pBufferData, int32_t offset, int16_t value) { if(pBufferData) pBufferData->Set<int16_t>(offset, value); }
void pseBufferDataSetInt32(PseBufferData* pBufferData, int32_t offset, int32_t value) { if(pBufferData) pBufferData->Set<int32_t>(offset, value); }
void pseBufferDataSetInt64(PseBufferData* pBufferData, int32_t offset, int64_t value) { if(pBufferData) pBufferData->Set<int64_t>(offset, value); }
void pseBufferDataSetGUID (PseBufferData* pBufferData, int32_t offset, PseGUID value) { if(pBufferData) pBufferData->Set<PseGUID>(offset, value); }

void pseBufferDataSetVector3  (PseBufferData* pBufferData, int32_t offset, PseVector3 value)          { if(pBufferData) pBufferData->Set<PseVector3  >(offset, value); }
void pseBufferDataSetQuat     (PseBufferData* pBufferData, int32_t offset, PseQuat value)             { if(pBufferData) pBufferData->Set<PseQuat     >(offset, value); }
void pseBufferDataSetTransform(PseBufferData* pBufferData, int32_t offset, const PseTransform& value) { if(pBufferData) pBufferData->Set<PseTransform>(offset, value); }
void pseBufferDataSetColor    (PseBufferData* pBufferData, int32_t offset, PseColor value)            { if(pBufferData) pBufferData->Set<PseColor    >(offset, value); }

// PseBufferData / Getters
int8_t  pseBufferDataGetInt8 (const PseBufferData* pBufferData, int32_t offset) { return pBufferData ? pBufferData->Get<int8_t >(offset) : int8_t {}; }
int16_t pseBufferDataGetInt16(const PseBufferData* pBufferData, int32_t offset) { return pBufferData ? pBufferData->Get<int16_t>(offset) : int16_t{}; }
int32_t pseBufferDataGetInt32(const PseBufferData* pBufferData, int32_t offset) { return pBufferData ? pBufferData->Get<int32_t>(offset) : int32_t{}; }
int64_t pseBufferDataGetInt64(const PseBufferData* pBufferData, int32_t offset) { return pBufferData ? pBufferData->Get<int64_t>(offset) : int64_t{}; }
PseGUID pseBufferDataGetGUID (const PseBufferData* pBufferData, int32_t offset) { return pBufferData ? pBufferData->Get<PseGUID>(offset) : PseGUID{}; }

PseVector3   pseBufferDataGetVector3  (const PseBufferData* pBufferData, int32_t offset) { return pBufferData ? pBufferData->Get<PseVector3  >(offset) : PseVector3  {}; }
PseQuat      pseBufferDataGetQuat     (const PseBufferData* pBufferData, int32_t offset) { return pBufferData ? pBufferData->Get<PseQuat     >(offset) : PseQuat     {}; }
PseTransform pseBufferDataGetTransform(const PseBufferData* pBufferData, int32_t offset) { return pBufferData ? pBufferData->Get<PseTransform>(offset) : PseTransform{}; }
PseColor     pseBufferDataGetColor    (const PseBufferData* pBufferData, int32_t offset) { return pBufferData ? pBufferData->Get<PseColor    >(offset) : PseColor    {}; }

// Buffers / Basic
int32_t pseInitializeBuffers()
{
    gPseSharedMemoryHandle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "Global\\PortalSolverEditorSDK");
    if(!gPseSharedMemoryHandle) return -1; // Could not open file mapping object
	
    void* pBuf = MapViewOfFile(gPseSharedMemoryHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(PseBuffers));
    if(!pBuf)
    {
        CloseHandle(gPseSharedMemoryHandle);
        gPseSharedMemoryHandle = nullptr;
        return -2; // Could not map view of file
    }
    
    gPseBuffers = new(pBuf) PseBuffers();
    gPseCommandBuffer = &gPseBuffers->commandBuffer;
    gPseEventBuffer = &gPseBuffers->eventBuffer;
    
    return 0;
}

void pseDeinitializeBuffers()
{
    if(gPseBuffers) UnmapViewOfFile(gPseBuffers);
    if(gPseSharedMemoryHandle) CloseHandle(gPseSharedMemoryHandle);

    gPseBuffers = nullptr;
    gPseCommandBuffer = nullptr;
    gPseEventBuffer = nullptr;
    gPseSharedMemoryHandle = nullptr;
}

// Buffers / Address Getters
PseBuffers*       pseGetBuffersAddress() { return gPseBuffers; }
PseCommandBuffer* pseGetCommandBufferAddress() { return gPseCommandBuffer; }
PseEventBuffer*   pseGetEventBufferAddress() { return gPseEventBuffer; }
void*             pseGetSharedMemoryHandle() { return gPseSharedMemoryHandle; }

// Command Buffer / Methods
void     pseCommandBufferSynchronize() { return gPseCommandBuffer->synchronize(); }

uint32_t pseCommandBufferSinglePush(const PseBufferData* pData) { return gPseCommandBuffer->singlePush(*pData); }
void     pseCommandBufferSingleWait(uint32_t index, PseBufferData* pOutData) { *pOutData = gPseCommandBuffer->singleWait(index); }
void     pseCommandBufferSinglePushAndWait(PseBufferData* pData) { *pData = gPseCommandBuffer->singlePushAndWait(*pData); }

uint32_t pseCommandBufferBatchPush(const PseBufferData* pData, uint32_t count) { return gPseCommandBuffer->batchPush(pData, count); }
void     pseCommandBufferBatchWait(uint32_t index, PseBufferData* pData, uint32_t count) { gPseCommandBuffer->batchWait(index, pData, count); }
void     pseCommandBufferBatchPushAndWait(PseBufferData* pData, uint32_t count) { gPseCommandBuffer->batchpushAndWait(pData, count); }

uint32_t pseCommandBufferRawReserve(uint32_t count) { return gPseCommandBuffer->rawReserve(count); }
void     pseCommandBufferRawCommit(uint32_t startIndex, uint32_t count) { gPseCommandBuffer->rawCommit(startIndex, count); }
void     pseCommandBufferRawWait(uint32_t index, uint32_t count) { gPseCommandBuffer->rawWait(index, count); }
void     pseCommandBufferRawCommitAndWait(uint32_t startIndex, uint32_t count) { gPseCommandBuffer->rawCommitAndWait(startIndex, count); }

// Event Buffer / Methods
int32_t pseEventBufferTryGet(PseBufferData* pOutData) { return gPseEventBuffer->tryGet(*pOutData) ? 1 : 0; }
