// Portal Solver Editor SDK

#pragma once

#ifdef __cplusplus

#include "pse.h" 
#include <cstring>
#include <stdint.h>

namespace pse
{
    namespace Register 
    {
		// Bit
		void setBit(PseRegister& outRegister, const uint8_t inOffset, const bool inValue)
		{
			outRegister = (outRegister & ~(1 << inOffset)) | (static_cast<PseRegister>(inValue) << inOffset);
		}
		
		bool getBit(const PseRegister InRegister, const uint8_t inOffset)
		{
			return (InRegister & (1 << inOffset)) != 0;
		}
		
		void updateBit(const bool& toRegister, PseRegister& outRegister, const uint8_t inOffset, bool& outValue)
		{
			if(toRegister) setBit(outRegister, inOffset, outValue);
			else outValue = getBit(outRegister, inOffset);
		}
		
		// Int
		template<typename T>
		T getInt(const PseRegister InRegister, const uint8_t inOffset)
		{
	        constexpr uint8_t BitSize = sizeof(T) * 8;
			static_assert(sizeof(T) * 8 <= 32, "T is big!");
	        constexpr uint32_t Mask = (BitSize == 32) ? 0xFFFFFFFF : ((1U << BitSize) - 1);
			return static_cast<T>((InRegister >> inOffset) & Mask);
		}
		
		template<typename T>
		void setInt(PseRegister& outRegister, const uint8_t inOffset, const T inValue)
		{
	        constexpr uint8_t BitSize = sizeof(T) * 8;
			static_assert(sizeof(T) * 8 <= 32, "T is big!");
	        constexpr uint32_t Mask = (BitSize == 32) ? 0xFFFFFFFF : ((1U << BitSize) - 1);
			outRegister = (outRegister & ~(Mask << inOffset)) | ((static_cast<PseRegister>(inValue) & Mask) << inOffset);
		}
		
		template<typename T, typename Q>
		void updateInt(const bool& toRegister, PseRegister& outRegister, const uint8_t inOffset, Q& outValue)
		{
			if(toRegister) setInt<T>(outRegister, inOffset, static_cast<T>(outValue));
			else outValue = static_cast<Q>(getInt<T>(outRegister, inOffset));
		}
		
		// BitAs
		template<typename T, uint8_t bitCount>
		T getBitsAs(const PseRegister InRegister, const uint8_t inOffset)
		{
			static_assert(bitCount > 0 && bitCount <= 32, "bitCount must be between 1 and 32!");
	        constexpr uint32_t Mask = (bitCount == 32) ? 0xFFFFFFFF : ((1U << bitCount) - 1);
			return static_cast<T>((static_cast<PseRegister>(InRegister) >> inOffset) & Mask);
		}
		
		template<typename T, uint8_t bitCount>
		void setBitsAs(PseRegister& outRegister, const uint8_t inOffset, const T inValue)
		{
			static_assert(bitCount > 0 && bitCount <= 32, "bitCount must be between 1 and 32!");
	        constexpr uint32_t Mask = (bitCount == 32) ? 0xFFFFFFFF : ((1U << bitCount) - 1);
			outRegister = static_cast<PseRegister>((static_cast<PseRegister>(outRegister) & ~(Mask << inOffset)) | ((static_cast<PseRegister>(inValue) & Mask) << inOffset));
		}
		
		template<typename T, uint8_t bitCount, typename Q>
		void updateBitsAs(const bool& toRegister, PseRegister& outRegister, const uint8_t inOffset, Q& outValue)
		{
			if(toRegister) setBitsAs<T, bitCount>(outRegister, inOffset, static_cast<T>(outValue));
			else outValue = static_cast<Q>(getBitsAs<T, bitCount>(outRegister, inOffset));
		}
    }
	
/*
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
*/
}

#endif
