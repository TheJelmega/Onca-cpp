#pragma once
#include "Defines.h"
#include "Config.h"

namespace Core
{
	constexpr auto SwitchEndianess(u16 val) noexcept -> u16;
	constexpr auto SwitchEndianess(u32 val) noexcept -> u32;
	constexpr auto SwitchEndianess(u64 val) noexcept -> u64;

	constexpr auto ToLittleEndian(u16 val) noexcept -> u16;
	constexpr auto ToLittleEndian(u32 val) noexcept -> u32;
	constexpr auto ToLittleEndian(u64 val) noexcept -> u64;

	constexpr auto ToBigEndian(u16 val) noexcept -> u16;
	constexpr auto ToBigEndian(u32 val) noexcept -> u32;
	constexpr auto ToBigEndian(u64 val) noexcept -> u64;
}

#include "Endianess.inl"