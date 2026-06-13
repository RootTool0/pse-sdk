// Portal Solver Editor SDK

#pragma once

#ifdef __cplusplus

#include "pse.h" 
#include <cstring>
#include <stdint.h>

namespace PseUtilities
{
    namespace Register 
    {
        template<uint8_t offset>
        void setBit(PseRegister& outRegister, const bool bInValue)
        {
            outRegister = (outRegister & ~(1U << offset)) | (static_cast<uint32_t>(bInValue) << offset);
        }
        
        template<uint8_t offset>
        bool getBit(const PseRegister inRegister)
        {
            return (inRegister & (1U << offset)) != 0;
        }
        
        template<uint8_t offset, typename T>
        void set(PseRegister& outRegister, const T InValue)
        {
            static_assert(sizeof(T) * 8 + offset <= 32, "T or offset is too big for 32-bit register!");
            constexpr uint8_t BitSize = sizeof(T) * 8;
            constexpr uint32_t Mask = (BitSize == 32) ? 0xFFFFFFFF : ((1U << BitSize) - 1);
            outRegister = (outRegister & ~(Mask << offset)) | ((static_cast<uint32_t>(InValue) & Mask) << offset);
        }
        
        template<uint8_t offset, typename T>
        T get(const PseRegister inRegister)
        {
            static_assert(sizeof(T) * 8 + offset <= 32, "T or offset is too big for 32-bit register!");
            constexpr uint8_t BitSize = sizeof(T) * 8;
            constexpr uint32_t Mask = (BitSize == 32) ? 0xFFFFFFFF : ((1U << BitSize) - 1);
            return static_cast<T>((inRegister >> offset) & Mask);
        }
        
        template<uint8_t offset, typename T, uint8_t bitCount>
        void setAs(PseRegister& outRegister, const T inValue)
        {
            static_assert(sizeof(T) * 8 + offset <= 32, "T or offset is too big for 32-bit register!");
            constexpr uint32_t mask = (bitCount == 32) ? 0xFFFFFFFF : ((1U << bitCount) - 1);
            outRegister = (outRegister & ~(mask << offset)) | ((static_cast<uint32_t>(inValue) & mask) << offset);
        }
        
        template<uint8_t offset, typename T, uint8_t bitCount>
        T getAs(const PseRegister inRegister)
        {
            static_assert(sizeof(T) * 8 + offset <= 32, "T or offset is too big for 32-bit register!");
            constexpr uint32_t mask = (bitCount == 32) ? 0xFFFFFFFF : ((1U << bitCount) - 1);
            return static_cast<T>((inRegister >> offset) & mask);
        }
    }
    
    namespace BufferData
    {
        template<uint8_t offset, typename T>
        void set(PseBufferData& outBufferData, const T& inValue) 
        {
            std::memcpy(&outBufferData.data[offset], &inValue, sizeof(T));
        }
        
        template<uint8_t offset, typename T>
        T get(const PseBufferData& inBufferData) 
        {
            T result;
            std::memcpy(&result, &inBufferData.data[offset], sizeof(T));
            return result;
        }
    }
}

#endif
