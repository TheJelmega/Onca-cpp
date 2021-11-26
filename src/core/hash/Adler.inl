#pragma once
#if __RESHARPER__
#include "Adler.h"
#endif

namespace Core::Hashing
{
	constexpr auto Adler32::operator()(const u8* pData, usize size) const noexcept -> u32
	{
		u32 a = 1;
		u32 b = 0;
		while (size--)
		{
			a = (a + *pData++) % Detail::Adler32Mod;
			b = (b + a) % Detail::Adler32Mod;
		}
		return (b << 16) | a;
	}
}