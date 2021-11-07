#pragma once
#if __RESHARPER__
#include "MemUtils.h"
#endif

#include "core/Assert.h"
#include "core/math/MathUtils.h"

namespace Core
{
	inline auto MemCpy(void* dst, const void* src, usize numBytes) noexcept -> void
	{
		memcpy(dst, src, numBytes);
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

	inline auto MemMove(void* dst, void* src, usize numBytes) noexcept -> void
	{
		memmove(dst, src, numBytes);
	}

	inline auto MemSet(void* ptr, u8 val, usize numBytes) noexcept -> void
	{
		memset(ptr, val, numBytes);
	}

	inline auto MemClear(void* ptr, usize numBytes) noexcept -> void
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
