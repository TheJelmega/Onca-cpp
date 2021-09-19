#pragma once
#include "MemUtils.h"
#include "core/Assert.h"
#include "core/math/MathUtils.h"

namespace Core
{
	INL auto MemCpy(void* dst, const void* src, usize numBytes) noexcept -> void
	{
		ASSERT(!(dst <= src && usize(dst) + numBytes > usize(src)) || !(dst >= src && usize(dst) < usize(src) + numBytes), "Regions overlap");
		memcpy(dst, src, numBytes);
	}

	template <typename T>
	auto MemCpy(MemRef<T>& dst, const MemRef<T>& src, usize numElems) noexcept -> void
	{
		ASSERT(dst != src, "Destination and source need to be different, use MemMove instead");
		numElems = Math::Min(numElems, src.Size());
		numElems = Math::Min(numElems, dst.Size());
		MemCpy(dst.Ptr(), src.Ptr(), numElems * sizeof(T));
	}

	template <typename T>
	auto MemCpy(MemRef<T>& dst, usize dstOffset, const MemRef<T>& src, usize srcOffset, usize numElems) noexcept -> void
	{
		ASSERT(dst != src, "Destination and source need to be different, use MemMove instead");
		ASSERT(dstOffset < dst.Size(), "Destination offset out of bounds");
		ASSERT(srcOffset < src.Size(), "Destination offset out of bounds");

		const usize maxDstLen = dst.Size() - dstOffset;
		numElems = Math::Min(numElems, maxDstLen);
		const usize maxSrcLen = src.Size() - srcOffset;
		numElems = Math::Min(numElems, maxSrcLen);

		MemCpy(dst.Ptr() + dstOffset, src.Ptr() + srcOffset, numElems * sizeof(T));
	}

	template <typename T>
	auto MemCpy(T& dst, const T& src) noexcept -> void
	{
		MemCpy(&dst, &src, sizeof(T));
	}

	template <typename T>
	auto MemMove(MemRef<T>& mem, usize dst, usize src, usize numBytes) noexcept -> void
	{
		const usize maxOffset = Math::Max(dst, src);
		const usize maxBytes = mem.Size() - maxOffset;
		numBytes = numBytes > maxBytes ? maxBytes : numBytes;
		u8* pBegin = reinterpret_cast<u8*>(mem.Ptr());
		MemMove(pBegin + dst, pBegin + src, numBytes);
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

	template <typename T>
	auto MemSet(MemRef<T>& mem, u8 val) noexcept -> void
	{
		MemSet(mem.Ptr(), val, mem.Size());
	}

	template <typename T>
	auto MemClear(MemRef<T>& mem) noexcept -> void
	{
		MemClear(mem.Ptr(), mem.Size());
	}

	template<typename T>
	auto MemClearData(T& val) noexcept -> void
	{
		MemClear(&val, sizeof(T));
	}

	inline auto MemCmp(const void* pA, const void* pB, usize numBytes) noexcept -> i8
	{
		return memcmp(pA, pB, numBytes);
	}

	template <typename T>
	auto MemCmp(MemRef<T>& a, MemRef<T>& b) noexcept -> i8
	{
		const usize sizeA = a.Size();
		const usize sizeB = b.Size();

		i8 res = MemCmp(a.Ptr(), b.Ptr(), Math::Min(sizeA, sizeB));
		if (res != 0)
			return res;

		return a.Size() < b.Size() ? -1 : 1;
	}

}
