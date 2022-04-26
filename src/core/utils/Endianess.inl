#pragma once
#if __RESHARPER__
#include "Endianess.h"
#endif

namespace Onca
{
	constexpr auto SwitchEndianess(u16 val) noexcept -> u16
	{
		return val << 8 | val >> 8;
	}

	constexpr auto SwitchEndianess(u32 val) noexcept -> u32
	{
		return val               << 24 |
			  (val & 0x0000'FF00) << 8  |
			  (val & 0x00FF'0000) >> 8  |
			  (val & 0xFF00'0000) >> 24 ;
	}

	constexpr auto SwitchEndianess(u64 val) noexcept -> u64
	{
		return val               << 56 |
			  (val & 0x0000'0000'0000'FF00) << 40  |
			  (val & 0x0000'0000'00FF'0000) << 24  |
			  (val & 0x0000'0000'FF00'0000) << 8  |
			  (val & 0x0000'00FF'0000'0000) >> 8  |
			  (val & 0x0000'FF00'0000'0000) >> 24  |
			  (val & 0x00FF'0000'0000'0000) >> 40  |
			  (val & 0xFF00'0000'0000'0000) >> 56;
	}

	constexpr auto ToLittleEndian(u16 val) noexcept -> u16
	{
		if constexpr (IS_LITTLE_ENDIAN)
			return val;
		else
			return SwitchEndianess(val);
	}

	constexpr auto ToLittleEndian(u32 val) noexcept -> u32
	{
		if constexpr (IS_LITTLE_ENDIAN)
			return val;
		else
			return SwitchEndianess(val);
	}

	constexpr auto ToLittleEndian(u64 val) noexcept -> u64
	{
		if constexpr (IS_LITTLE_ENDIAN)
			return val;
		else
			return SwitchEndianess(val);
	}

	constexpr auto ToBigEndian(u16 val) noexcept -> u16
	{
		if constexpr (IS_LITTLE_ENDIAN)
			return SwitchEndianess(val);
		else
			return val;
	}

	constexpr auto ToBigEndian(u32 val) noexcept -> u32
	{
		if constexpr (IS_LITTLE_ENDIAN)
			return SwitchEndianess(val);
		else
			return val;
	}

	constexpr auto ToBigEndian(u64 val) noexcept -> u64
	{
		if constexpr (IS_LITTLE_ENDIAN)
			return SwitchEndianess(val);
		else
			return val;
	}
}
