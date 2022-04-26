#pragma once
#if __RESHARPER__
#include "FNV.h"
#endif

namespace Onca::Hashing
{
	constexpr auto FVN1_32::operator()(const u8* pData, usize size) const noexcept -> u32
	{
		u32 hash = Detail::FNV1_32Offset;
		while (size--)
		{
			hash *= Detail::FNV1_32Prime;
			hash ^= *pData++;
		}
		return hash;
	}

	constexpr auto FVN1A_32::operator()(const u8* pData, usize size) const noexcept -> u32
	{
		u32 hash = Detail::FNV1_32Offset;
		while (size--)
		{
			hash ^= *pData++;
			hash *= Detail::FNV1_32Prime;
		}
		return hash;
	}

	constexpr auto FVN1_64::operator()(const u8* pData, usize size) const noexcept -> u64
	{
		u64 hash = Detail::FNV1_64Offset;
		while (size--)
		{
			hash *= Detail::FNV1_64Prime;
			hash ^= *pData++;
		}
		return hash;
	}

	constexpr auto FVN1A_64::operator()(const u8* pData, usize size) const noexcept -> u64
	{
		u64 hash = Detail::FNV1_64Offset;
		while (size--)
		{
			hash ^= *pData++;
			hash *= Detail::FNV1_64Prime;
		}
		return hash;
	}
}
