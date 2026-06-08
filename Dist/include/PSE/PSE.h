// Portal Solver Editor SDK

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

#include "enums.h"
#include "structures.h"

#ifdef __cplusplus
extern "C"
{
#endif
    
    // Buffers / Core
    PSE_API int32_t pseInitializeBuffers(void);
    PSE_API void    pseDeinitializeBuffers(void);
    
    PSE_API PseBuffers*       pseGetBuffersAddress(void);
    PSE_API PseCommandBuffer* pseGetCommandBufferAddress(void);
    PSE_API PseEventBuffer*   pseGetEventBufferAddress(void);
    PSE_API void*             pseGetSharedMemoryHandle(void);
    
    // Command Buffer / Methods
    PSE_API void     pseCommandBufferSynchronize(void);
    
    PSE_API uint32_t pseCommandBufferSinglePush(const PseBufferData* pBufferData);
    PSE_API void     pseCommandBufferSingleWait(uint32_t absoluteIndex, PseBufferData* pOutBufferData);
    PSE_API void     pseCommandBufferSinglePushAndWait(PseBufferData* pOutBufferData);
    
    PSE_API uint32_t pseCommandBufferBatchPush(const PseBufferData* pBufferData, uint32_t count);
    PSE_API void     pseCommandBufferBatchWait(uint32_t absoluteIndex, PseBufferData* pOutBufferData, uint32_t count);
    PSE_API void     pseCommandBufferBatchPushAndWait(PseBufferData* pOutBufferData, uint32_t count);
    
    PSE_API PseBufferData* pseCommandBufferRawAt(uint32_t absoluteIndex);
    PSE_API uint32_t       pseCommandBufferRawReserve(uint32_t count);
    PSE_API void           pseCommandBufferRawCommit(uint32_t startAbsoluteIndex, uint32_t count);
    PSE_API void           pseCommandBufferRawWait(uint32_t absoluteIndex, uint32_t count);
    PSE_API void           pseCommandBufferRawCommitAndWait(uint32_t startAbsoluteIndex, uint32_t count);
    
    // Event Buffer / Methods
    PSE_API int32_t pseEventBufferTryGet(PseBufferData* pOutBufferData);
    
#ifdef __cplusplus
}
#endif
