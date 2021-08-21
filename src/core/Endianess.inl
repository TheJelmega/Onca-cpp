#pragma once
#include "Endianess.h"

namespace Core
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
}
