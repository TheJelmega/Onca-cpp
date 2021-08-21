#pragma once
#include "Defines.h"
#include "Config.h"

namespace Core
{
	constexpr auto SwitchEndianess(u16 val) noexcept -> u16;
	constexpr auto SwitchEndianess(u32 val) noexcept -> u32;
	constexpr auto SwitchEndianess(u64 val) noexcept -> u64;
}

#if IS_LITTLE_ENDIAN
#	define TO_LITTLE_ENDIAN(x) (x)
#	define TO_BIG_ENDIAN(x) Core::SwitchEndianess(x)
#else
#	define TO_LITTLE_ENDIAN(x) Core::SwitchEndianess(x)
#	define TO_BIG_ENDIAN(x) (x)
#endif

#include "Endianess.inl"