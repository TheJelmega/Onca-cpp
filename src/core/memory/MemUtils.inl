#pragma once
#include "MemUtils.h"
#include "core/Assert.h"

namespace Core
{
	INL auto MemCpy(void* dst, const void* src, usize numBytes) noexcept -> void
	{
		ASSERT(!(dst <= src && usize(dst) + numBytes > usize(src)) || !(dst >= src && usize(dst) < usize(src) + numBytes), "Regions overlap");
		memcpy(dst, src, numBytes);
	}

	template <typename T>
	auto MemCpy(T& dst, const T& src) noexcept -> void
	{
		MemCpy(&dst, &src, sizeof(T));
	}

	INL auto MemMove(void* dst, void* src, usize numBytes) noexcept -> void
	{
		memmove(dst, src, numBytes);
	}

	INL auto MemSet(void* ptr, u8 val, usize numBytes) noexcept -> void
	{
		memset(ptr, val, numBytes);
	}

	INL auto MemClear(void* ptr, usize numBytes) noexcept -> void
	{
		MemSet(ptr, 0, numBytes);
	}

	template<typename T>
	auto MemClear(T& val) noexcept -> void
	{
		MemSet(&val, 0, sizeof(T));
	}
}
