#pragma once

#if __RESHARPER__
#include "BitIntrin.h"
#endif

namespace Core::Intrin
{
	// TODO: non-MSVC intrinsics
	template<Integral T>
	constexpr auto BitScan(T t) noexcept -> u8
	{
		IF_NOT_CONSTEVAL
		{
			if constexpr (sizeof(T) == 8)
			{
#if COMPILER_MSVC
				unsigned long idx;
				u8 found = _BitScanForward64(&idx, t);
				return found ? u8(idx) : 255;
#elif COMPILER_CLANG || COMPILER_GCCC
				return u8(t == 0 ? 255 : __builtin_clzll(u32(t)));
#endif
			}
			else
			{
#if COMPILER_MSVC
				unsigned long idx;
				u8 found = _BitScanForward(&idx, u32(t));
				return found && idx < (sizeof(T) * 8) ? u8(idx) : u8(-1);
#elif COMPILER_CLANG || COMPILER_GCC
				return u8(t == 0 ? 255 : __builtin_clz(u32(t)));
#endif
			}
		}
			
		u8 idx = 0;
		constexpr u8 max = sizeof(T) * 8;
		constexpr u64 bit = 0x8000'0000'0000'0000 >> ((8 - sizeof(T)) * 8);
		for (; idx < max; ++idx)
		{
			if ((u64(t) << idx) & bit)
				return (sizeof(T) * 8 - 1) - idx;
		}
		return u8(-1);
	}

	template <Integral T>
	constexpr auto BitScan0(T t) noexcept -> u8
	{
		return BitScan(~t);
	}

	// TODO: non-MSVC intrinsics
	template <Integral T>
	constexpr auto BitScanR(T t) noexcept -> u8
	{
		IF_NOT_CONSTEVAL
		{
#if COMPILER_MSVC
			if constexpr (sizeof(T) == 8)
			{
				unsigned long idx;
				u8 found = _BitScanReverse64(&idx, t);
				return found ? u8(idx) : 255;
			}
			else
			{
				unsigned long idx;
				u8 found = _BitScanReverse(&idx, u32(t));
				return found && idx < (sizeof(T) * 8) ? u8(idx) : u8(-1);
			}
#elif COMPILER_GCC
			if constexpr (sizeof(T) == 8)
				return u8(t == 0 ? 255 : __builtin_ctzll(u32(t)));
			else
				return u8(t == 0 ? 255 : __builtin_ctz(u32(t)));
#endif
			
		}

		u8 idx = 0;
		constexpr u8 max = sizeof(T) * 8;
		for (; idx < max; ++idx)
		{
			if ((t >> idx) & 1)
				return idx;
		}
		return u8(-1);
	}

	template <Integral T>
	constexpr auto BitScanR0(T t) noexcept -> u8
	{
		return BitScanR(~t);
	}

	template <Integral T>
	constexpr auto PopCnt(T t) noexcept -> u8
	{
		{
#if HAS_POPCNT
#	if COMPILER_MSVC
			if constexpr (sizeof(T) == 8)
				return u8(__popcnt64(u64(t)));
			else if constexpr (sizeof(T) == 4)
				return u8(__popcnt(u32(t)));
			else
				return u8(__popcnt16(u16(t)));
#	elif COMPILER_CLANG || COMPILER_GCC
			if constexpr (sizeof(T) == 8)
				return u8(__builtin_popcountll(u64(t)));
			else if constexpr (sizeof(T) == 4)
				return u8(__builtin_popcount(u32(t)));
#	endif
#endif
		}

		// Calculate popcnt using hamming weigths: https://en.wikipedia.org/wiki/Hamming_weight#Efficient_implementation
		constexpr u64 m1  = 0x5555'5555'5555'5555;
		constexpr u64 m2  = 0x3333'3333'3333'3333;
		constexpr u64 m4  = 0x0F0F'0F0F'0F0F'0F0F;
		constexpr u64 h01 = 0x0101'0101'0101'0101;

		t -= (t >> 1) & m1;             // put count of each 2 bits into 2 bits
		t = (t & m2) + ((t >> 2) & m2); // put count of each 4 bits into 4 bits
		t = (t + (t >> 4)) & m4;        // put count of each 8 bits into 8 bits
		return (t * h01) >> 56;         // left 8 bits of the sum: x + (x << 8) + (x << 16) + ...
	}

	template <Integral T>
	constexpr auto PopCnt0(T t) noexcept -> u8
	{
		return u8(sizeof(T) * 8 - PopCnt(t));
	}
}
