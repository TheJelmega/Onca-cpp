#pragma once
#if __RESHARPER__
#include "MemUtils.h"
#endif

#include "core/Assert.h"
#include "core/math/MathUtils.h"

namespace Core
{
	constexpr auto MemCpy(void* dst, const void* src, usize numBytes) noexcept -> void
	{
		IF_CONSTEVAL
		{
			ASSERT(!(dst <= src && usize(dst) + numBytes > usize(src)) || !(dst >= src && usize(dst) < usize(src) + numBytes), "Regions overlap");
			u8* dst_ = static_cast<u8*>(dst);
			const u8* src_ = static_cast<const u8*>(src);
			for (usize i = 0; i < numBytes; ++i, ++dst_, ++src_)
				*dst_ = *src_;
		}
		else
		{
			memcpy(dst, src, numBytes);
		}
	}

	template <typename T>
	constexpr auto MemCpy(T& dst, const T& src) noexcept -> void
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

	constexpr auto MemMove(void* dst, void* src, usize numBytes) noexcept -> void
	{
		IF_CONSTEVAL
		{
			u8* dst_ = static_cast<u8*>(dst);
			const u8* src_ = static_cast<const u8*>(src);

			if (src_ < dst_)
			{
				for (usize i = 0; i < numBytes; ++i, ++dst_, ++src_)
					*dst_ = *src_;
			}
			else if (src_ > dst_)
			{
				dst_ += numBytes - 1;
				src_ += numBytes - 1;
				for (usize i = 0; i < numBytes; ++i, --dst_, --src_)
					*dst_ = *src_;
			}
		}
		else
		{
			memmove(dst, src, numBytes);
		}
	}

	constexpr auto MemSet(void* ptr, u8 val, usize numBytes) noexcept -> void
	{
		IF_CONSTEVAL
		{
			u8* ptr_ = static_cast<u8*>(ptr);
			for (usize i = 0; i < numBytes; ++i, ++ptr_)
				*ptr_ = val;
		}
		else
		{
			memset(ptr, val, numBytes);
		}
	}

	constexpr auto MemClear(void* ptr, usize numBytes) noexcept -> void
	{
		MemSet(ptr, 0, numBytes);
	}

	template <typename T>
	constexpr auto MemSet(MemRef<T>& mem, u8 val) noexcept -> void
	{
		MemSet(mem.Ptr(), val, mem.Size());
	}

	template <typename T>
	auto MemClear(MemRef<T>& mem) noexcept -> void
	{
		MemClear(mem.Ptr(), mem.Size());
	}

	template<typename T>
	constexpr auto MemClearData(T& val) noexcept -> void
	{
		MemClear(&val, sizeof(T));
	}

	constexpr auto MemCmp(const void* pA, const void* pB, usize numBytes) noexcept -> i8
	{
		IF_CONSTEVAL
		{
			const u8 * pA_ = static_cast<const u8*>(pA);
			const u8 * pB_ = static_cast<const u8*>(pB);

			for (usize i = 0; i < numBytes; ++i, ++pA_, ++pB_)
			{
				if (*pA_ != *pB_)
					return *pA_ < *pB_ ? -1 : 1;
			}
			return 0;
		}
		else
		{
			return memcmp(pA, pB, numBytes);
		}
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
