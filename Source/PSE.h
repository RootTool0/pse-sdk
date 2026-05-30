#pragma once

#if defined(_WIN32)
    #ifdef PSE_EXPORTS
        #define PSE_API __declspec(dllexport)
    #else
        #define PSE_API __declspec(dllimport)
    #endif
#else
    #define PSE_API __attribute__((visibility("default")))
#endif

#include "Enums.h"
#include "Structures.h"

#ifdef __cplusplus
extern "C"
{
#endif
    
    // PseBufferData / Create
    PSE_API void pseBufferDataCreateCommand(PseBufferData* pBufferData, PseCommand command);
    PSE_API void pseBufferDataCreateResult (PseBufferData* pBufferData, PseResult  result );
    PSE_API void pseBufferDataCreateEvent  (PseBufferData* pBufferData, PseEvent   event  );
    
    // PseBufferData / Read
    PSE_API PseCommand pseBufferDataReadCommand(const PseBufferData* pBufferData);
    PSE_API PseResult  pseBufferDataReadResult (const PseBufferData* pBufferData);
    PSE_API PseEvent   pseBufferDataReadEvent  (const PseBufferData* pBufferData);
    
    // PseBufferData / Setters
    PSE_API void pseBufferDataSetInt8     (PseBufferData* pBufferData, int32_t offset, int8_t  value);
    PSE_API void pseBufferDataSetInt16    (PseBufferData* pBufferData, int32_t offset, int16_t value);
    PSE_API void pseBufferDataSetInt32    (PseBufferData* pBufferData, int32_t offset, int32_t value);
    PSE_API void pseBufferDataSetInt64    (PseBufferData* pBufferData, int32_t offset, int64_t value);
    PSE_API void pseBufferDataSetFloat    (PseBufferData* pBufferData, int32_t offset, float value);
    PSE_API void pseBufferDataSetGUID     (PseBufferData* pBufferData, int32_t offset, PseGUID value);
    PSE_API void pseBufferDataSetVector2  (PseBufferData* pBufferData, int32_t offset, PseVector2 value);
    PSE_API void pseBufferDataSetVector3  (PseBufferData* pBufferData, int32_t offset, PseVector3 value);
    PSE_API void pseBufferDataSetQuat     (PseBufferData* pBufferData, int32_t offset, PseQuat value);
    PSE_API void pseBufferDataSetTransform(PseBufferData* pBufferData, int32_t offset, const PseTransform* value);
    PSE_API void pseBufferDataSetColor    (PseBufferData* pBufferData, int32_t offset, PseColor value);
    
    // PseBufferData / Getters
    PSE_API int8_t       pseBufferDataGetInt8     (const PseBufferData* pBufferData, int32_t offset);
    PSE_API int16_t      pseBufferDataGetInt16    (const PseBufferData* pBufferData, int32_t offset);
    PSE_API int32_t      pseBufferDataGetInt32    (const PseBufferData* pBufferData, int32_t offset);
    PSE_API int64_t      pseBufferDataGetInt64    (const PseBufferData* pBufferData, int32_t offset);
    PSE_API float        pseBufferDataGetFloat    (const PseBufferData* pBufferData, int32_t offset);
    PSE_API PseGUID      pseBufferDataGetGUID     (const PseBufferData* pBufferData, int32_t offset);
    PSE_API PseVector2   pseBufferDataGetVector2  (const PseBufferData* pBufferData, int32_t offset);
    PSE_API PseVector3   pseBufferDataGetVector3  (const PseBufferData* pBufferData, int32_t offset);
    PSE_API PseQuat      pseBufferDataGetQuat     (const PseBufferData* pBufferData, int32_t offset);
    PSE_API PseTransform pseBufferDataGetTransform(const PseBufferData* pBufferData, int32_t offset);
    PSE_API PseColor     pseBufferDataGetColor    (const PseBufferData* pBufferData, int32_t offset);
    
    // Buffers / Basic
    PSE_API int32_t pseInitializeBuffers();
    PSE_API void    pseDeinitializeBuffers();
    
    // Buffers / Address Getters
    PSE_API PseBuffers*       pseGetBuffersAddress();
    PSE_API PseCommandBuffer* pseGetCommandBufferAddress();
    PSE_API PseEventBuffer*   pseGetEventBufferAddress();
    PSE_API void*             pseGetSharedMemoryHandle();
    
    // Command Buffer / Methods
    PSE_API void     pseCommandBufferSynchronize();
    
    PSE_API uint32_t pseCommandBufferSinglePush(const PseBufferData* pBufferData);
    PSE_API void     pseCommandBufferSingleWait(uint32_t index, PseBufferData* pOutBufferData);
    PSE_API void     pseCommandBufferSinglePushAndWait(const PseBufferData* pBufferData);
    
    PSE_API uint32_t pseCommandBufferBatchPush(const PseBufferData* pBufferData, uint32_t count);
    PSE_API void     pseCommandBufferBatchWait(uint32_t index, const PseBufferData* pBufferData, uint32_t count);
    PSE_API void     pseCommandBufferBatchPushAndWait(const PseBufferData* pBufferData, uint32_t count);
    
    PSE_API uint32_t pseCommandBufferRawReserve(uint32_t count);
    PSE_API void     pseCommandBufferRawCommit(uint32_t startIndex, uint32_t count);
    PSE_API void     pseCommandBufferRawWait(uint32_t index, uint32_t count);
    PSE_API void     pseCommandBufferRawCommitAndWait(uint32_t startIndex, uint32_t count);
    
    // Event Buffer / Methods
    PSE_API int32_t pseEventBufferTryGet(PseBufferData* pOutBufferData);
    
#ifdef __cplusplus
}
#endif
