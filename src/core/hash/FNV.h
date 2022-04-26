#pragma once
#include "core/MinInclude.h"

namespace Onca::Hashing
{
	namespace Detail
	{
		constexpr u32 FNV1_32Offset = 0x811c9dc5;
		constexpr u32 FNV1_32Prime = 0x01000193;

		constexpr u64 FNV1_64Offset = 0xcbf29ce484222325;
		constexpr u64 FNV1_64Prime = 0x00000100000001B3;
	}

	/**
	 * 32-bit Fowler-Noll-Vo 1 hash
	 */
	struct FVN1_32
	{
		constexpr auto operator()(const u8* pData, usize size) const noexcept -> u32;
	};

	/**
	 * 32-bit Fowler-Noll-Vo 1a hash
	 */
	struct FVN1A_32
	{
		constexpr auto operator()(const u8* pData, usize size) const noexcept -> u32;
	};

	/**
	 * 64-bit Fowler-Noll-Vo 1 hash
	 */
	struct FVN1_64
	{
		constexpr auto operator()(const u8* pData, usize size) const noexcept -> u64;
	};

	/**
	 * 64-bit Fowler-Noll-Vo 1a hash
	 */
	struct FVN1A_64
	{
		constexpr auto operator()(const u8* pData, usize size) const noexcept -> u64;
	};
}

#include "FNV.inl"