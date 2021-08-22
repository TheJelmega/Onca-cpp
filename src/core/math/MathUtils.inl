#pragma once
#include "MathUtils.h"

#ifdef __INTELLISENSE__
#pragma diag_suppress 438 // supress bpgus "expected a ''" due to concepts
#endif

namespace Core
{
	template<LessComparable T>
	constexpr auto Min(T a, T b) noexcept -> T
	{
		return a < b ? a : b;
	}

	template<GreaterComparable T>
	constexpr auto Max(T a, T b) noexcept -> T
	{
		return a > b ? a : b;
	}

	constexpr auto IsPowOf2(u64 val) noexcept -> bool
	{
		return !(val & (val - 1));
	}

#define LOG2_HELPER(n) if (val >= 1ull << (n)) { res += n; val >>= n; } 

	constexpr auto Log2(u64 val) noexcept -> u8
	{
		u8 res = -(val == 0);
		if (val >= 1ull << 32) { res += 32; val >>= 32; };
		if (val >= 1ull << 16) { res += 16; val >>= 16; };
		if (val >= 1ull << 8) { res += 8; val >>= 8; };
		if (val >= 1ull << 4) { res += 4; val >>= 4; };
		if (val >= 1ull << 2) { res += 2; val >>= 2; };
		if (val >= 1ull << 1) { res += 1; };
		return res;
	}

	constexpr auto Log2(u32 val) noexcept -> u8
	{
		u8 res = -(val == 0);
		if (val >= 1ull << 16) { res += 16; val >>= 16; };
		if (val >= 1ull << 8) { res += 8; val >>= 8; };
		if (val >= 1ull << 4) { res += 4; val >>= 4; };
		if (val >= 1ull << 2) { res += 2; val >>= 2; };
		if (val >= 1ull << 1) { res += 1; };
		return res;
	}

	constexpr auto Log2(u16 val) noexcept -> u8
	{
		u8 res = -(val == 0);
		if (val >= 1ull << 8) { res += 8; val >>= 8; };
		if (val >= 1ull << 4) { res += 4; val >>= 4; };
		if (val >= 1ull << 2) { res += 2; val >>= 2; };
		if (val >= 1ull << 1) { res += 1; };
		return res;
	}

	constexpr auto Log2(u8 val) noexcept -> u8
	{
		u8 res = -(val == 0);
		if (val >= 1ull << 4) { res += 4; val >>= 4; };
		if (val >= 1ull << 2) { res += 2; val >>= 2; };
		if (val >= 1ull << 1) { res += 1; };
		return res;
	}

}