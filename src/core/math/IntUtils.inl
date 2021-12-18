#pragma once
#if __RESHARPER__
#include "IntUtils.h"
#endif
#include <intrin.h>

namespace Core
{
	constexpr U128::U128() noexcept
		: low(0)
		, high(0)
	{
	}

	constexpr U128::U128(u64 low, u64 high)
		: low(low)
		, high(high)
	{
	}

	constexpr auto U128::operator+=(u64 val) noexcept -> U128&
	{
		IF_NOT_CONSTEVAL
		{
#if COMPILER_MSVC
			u64 carry = _addcarry_u64(0, low, val, &low);
			_addcarry_u64(u8(carry), high, 0, &high);
#elif COMPILER_CLANG || COMPILER_GCC
			u64 carry;
			low = __builtin_addcll(low, val, 0, &carry);
			high = __builtin_addcll(high, 0, carry, &carry);
#endif
			return *this;
		}

		u64 sum = low + val;
		high += (sum < low);
		low = sum;

		return *this;
	}

	constexpr auto Umul128(u64 x, u64 y) noexcept -> U128
	{
		IF_NOT_CONSTEVAL
		{
#if COMPILER_MSVC
			U128 res;
			res.low = _umul128(x, y, &res.high);
			return res;
#endif
		}

		u32 a = u32(x >> 32);
		u32 b = u32(x);
		u32 c = u32(y >> 32);
		u32 d = u32(y);

		u64 ac = u64(a) * u64(c);
		u64 bc = u64(b) * u64(c);
		u64 ad = u64(a) * u64(d);
		u64 bd = u64(b) * u64(d);

		u64 intermediate = (bd >> 32) + u32(ad) + u32(bc);
		return { (intermediate << 32) + u32(bd), ac + (intermediate >> 32) + (ad >> 32) + (bc >> 32) };
	}

	constexpr auto Umul128Upper64(u64 x, u64 y) noexcept -> u64
	{
		IF_NOT_CONSTEVAL
		{
#if COMPILER_MSVC
			u64 res;
			_umul128(x, y, &res);
			return res;
#endif
		}

		u32 a = u32(x >> 32);
		u32 b = u32(x);
		u32 c = u32(y >> 32);
		u32 d = u32(y);

		u64 ac = u64(a) * u64(c);
		u64 bc = u64(b) * u64(c);
		u64 ad = u64(a) * u64(d);
		u64 bd = u64(b) * u64(d);

		u64 intermediate = (bd >> 32) + u32(ad) + u32(bc);
		return ac + (intermediate >> 32) + (ad >> 32) + (bc >> 32);
	}

	constexpr auto Umul192Upper64(u64 x, U128 y) noexcept -> u64
	{
		U128 val = Umul128(x, y.high);
		val += Umul128Upper64(x, y.low);
		return val.high;
	}

	constexpr auto Umul96Upper32(u32 x, u64 y) noexcept -> u32
	{
#if COMPILER_MSVC && 0
		return u32(Umul128Upper64(x, y));
#endif
		u32 b = x;
		u32 c = u32(y >> 32);
		u32 d = u32(y);

		u64 bc = u64(b) * u64(c);
		u64 bd = u64(b) * u64(d);

		u64 intermediate = (bd >> 32) + bc;
		return intermediate >> 32;
	}

	constexpr auto Umul192Middle64(u64 x, U128 y) noexcept -> u64
	{
		u64 upper = x * y.high;
		u64 lower = Umul128Upper64(x, y.low);
		return upper + lower;
	}

	constexpr auto Umul96Lower64(u32 x, u64 y) noexcept -> u64
	{
		return x * y;
	}
}
