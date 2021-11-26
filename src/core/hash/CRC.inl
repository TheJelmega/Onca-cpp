#pragma once
#if __RESHARPER__
#include "CRC.h"
#endif

namespace Core::Hashing
{
	constexpr auto Crc32::operator()(const u8* pData, usize size) const noexcept -> u32
	{
		u32 crc32 = 0xFFFF'FFFF;
		while (size--)
		{
			const u8 lutIdx = (crc32 ^ *pData++) & 0xFF;
			crc32 = (crc32 >> 8) ^ Detail::Crc32Lut[lutIdx];
		}
		return crc32 ^ 0xFFFF'FFFF;
	}
}
