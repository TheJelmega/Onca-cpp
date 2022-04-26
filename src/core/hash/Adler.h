#pragma once
#include "core/MinInclude.h"

namespace Onca::Hashing
{
	namespace Detail
	{
		constexpr u32 Adler32Mod = 65521;
	}

	/**
	 * 32-bit adler
	 */
	struct Adler32
	{
		constexpr auto operator()(const u8* pData, usize size) const noexcept -> u32;
	};
}

#include "Adler.inl"