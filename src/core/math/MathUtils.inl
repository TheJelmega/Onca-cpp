#pragma once
#include "MathUtils.h"

#ifdef __INTELLISENSE__
#pragma diag_suppress 438 // supress bogus "expected a ''" due to concepts
#endif

namespace Core::Math
{
	namespace Detail
	{
		template<FloatingPoint T>
		constexpr auto SqrtHelperF(T x, T curr, T prev) noexcept -> T
		{
			if (curr == prev)
				return curr;

			return SqrtHelperF(x, T(0.5 * (curr + x / curr)), curr);
		}

		template<Integral T>
		constexpr auto SqrtHelper(T x, T lo, T hi) noexcept -> T
		{
			if (lo == hi)
				return lo;

			const T mid = (lo + hi + 1) / 2;
			if (x / mid < mid)
				return SqrtHelper<T>(x, lo, mid - 1);
			return SqrtHelper<T>(x, mid, hi);
		}
	}

	template<typename T0, LessComparable<T0> T1>
	requires ConvertableFrom<T0, T1>
	constexpr auto Min(T0 a, T1 b) noexcept -> T0
	{
		return a < b ? a : T0(b);
	}

	template<typename T0, LessComparable<T0> T1>
		requires ConvertableFrom<T0, T1>
	constexpr auto Max(T0 a, T1 b) noexcept -> T0
	{
		return a > b ? a : T0(b);
	}

	template <Numeric T, LessComparable<T> A, GreaterComparable<T> B> requires ConvertableFrom<T, A> && ConvertableFrom<T, B>
	constexpr auto Clamp(T val, A min, B max) noexcept -> T
	{
		if (static_cast<T>(min) > val)
			return static_cast<T>(min);
		if (static_cast<T>(max) < val)
			return static_cast<T>(max);
		return val;
	}

	template <Numeric T, Numeric U> requires Multiplyable<T, U>
	constexpr auto Lerp(T a, T b, U i) noexcept -> T
	{
		return a * (static_cast<T>(1) - i) + b * i;
	}

	template <Numeric T>
	constexpr auto SmoothStep(T min, T max, T i) noexcept -> T
	{
		T tmp = Clamp((i - min) / (max - min), static_cast<T>(0), static_cast<T>(1));

		return tmp * tmp * (static_cast<T>(3) - static_cast<T>(2) * tmp);
	}

	template <Numeric T>
	constexpr auto Step(T edge, T i) noexcept -> T
	{
		T res[2] = { static_cast<T>(0), static_cast<T>(1) };
		return res[i >= edge];
	}

	template <Numeric T>
	constexpr auto Abs(T t) noexcept -> T
	{
		if constexpr (UnsignedIntegral<T>)
		{
			return t;
		}
		else
		{
			const T values[2] = { t, -t };
			return values[t < 0];
		}
	}

	template <FloatingPoint F>
	constexpr auto Ceil(F f) noexcept -> FloatIntType<F>
	{
		FloatIntType<F> trunc = Trunc(f);
		return trunc + (f >= 0 && f != trunc);
	}

	template <FloatingPoint F>
	constexpr auto Floor(F f) noexcept -> FloatIntType<F>
	{
		FloatIntType<F> trunc = Trunc(f);
		return trunc - (f < 0 && f != trunc);
	}

	template <FloatingPoint F>
	constexpr auto Trunc(F f) noexcept -> FloatIntType<F>
	{
		return static_cast<FloatIntType<F>>(f);
	}

	template <FloatingPoint F>
	constexpr auto Round(F f) noexcept -> FloatIntType<F>
	{
		constexpr double offset[2] = { 0.5f, -0.5f };
		return static_cast<FloatIntType<F>>(f + offset[f < 0]);
	}

	template <FloatingPoint F>
	constexpr auto RoundEven(F f) noexcept -> FloatIntType<F>
	{
		using Int = FloatIntType<F>;
		FloatIntType<F> round = Round(f);
		F diff = Abs(f - round);
		Int possibleVals[2] = { round, static_cast<Int>(f - diff) };
		return possibleVals[(diff == 0.5f) && (round & 1)];
	}

	template <Numeric T>
	constexpr auto Sign(T t) noexcept -> i8
	{
		constexpr i8 signs[3] = { 1, 0, -1 };
		return signs[(t <= 0) + (t < 0)];
	}

	template <FloatingPoint F>
	constexpr auto Fract(F t) noexcept -> F
	{
		return t - Floor(t);
	}

	template <Numeric T>
	constexpr auto Mod(T a, T b) noexcept -> T
	{
		return a - b * Floor(a / b);
	}

	template <FloatingPoint F>
	constexpr auto ModF(F f) noexcept -> Pair<FloatIntType<F>, F>
	{
		auto i = Trunc(f);
		return { i, f - static_cast<F>(i) };
	}

	template <Numeric T>
	constexpr auto Rcp(T t) noexcept -> T
	{
		return T(1) / t;
	}

	template <Numeric T>
	constexpr auto Sqrt(T t) noexcept -> T
	{
		IF_CONSTEVAL
		{
			if constexpr (Integral<T>)
				return Detail::SqrtHelper(t, T(0), T(t / 2 - 1));
			else if constexpr (FloatingPoint<T>)
				return Detail::SqrtHelperF(t, t, T(0));
		}

		// TODO: sqrt is not constexpr
		return T(sqrt(t));
	}

	template <Numeric T>
	constexpr auto RSqrt(T t) noexcept -> T
	{
		return Rcp(Sqrt(t));
	}

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

	constexpr auto IsPowOf2(u64 val) noexcept -> bool
	{
		return !(val & (val - 1));
	}

	template <FloatingPoint F>
	constexpr auto IsInf(F f) noexcept -> bool
	{
		if constexpr (SameAs<F, f64>)
		{
			u64 bits = FloatBitsToUInt(f);
			return (bits & 0x7FF8'0000'0000'0000) == 0x7FF8'0000'0000'0000;
		}
		else
		{
			u32 bits = FloatBitsToUInt(f);
			return (bits & 0x7F80'0000) == 0x7F80'0000;
		}
	}

	template <FloatingPoint F>
	constexpr auto IsNaN(F f) noexcept -> bool
	{
		if constexpr (SameAs<F, f64>)
		{
			u64 bits = FloatBitsToUInt(f);
			return (bits & 0x7FF8'0000'0000'0000) == 0x7FF8'0000'0000'0000 &&
				 !!(bits & 0x0007'FFFF'FFFF'FFFF);
		}
		else
		{
			u32 bits = FloatBitsToUInt(f);
			return (bits & 0x7F80'0000) == 0x7F80'0000 &&
				 !!(bits & 0x007F'FFFF);
		}
	}

	template <Numeric T>
	constexpr auto EpsilonCompare(T a, T b, T e) noexcept -> bool
	{
		return Abs(a - b) <= T(e);
	}

	template <FloatingPoint F>
	constexpr auto FloatBitsToInt(F f) noexcept -> FloatIntType<F>
	{
		union
		{
			F f;
			FloatIntType<F> i;
		} u;
		u.f = f;
		return u.i;
	}

	template <FloatingPoint F>
	constexpr auto FloatBitsToUInt(F f) noexcept -> FloatUIntType<F>
	{
		union
		{
			F f;
			FloatUIntType<F> i;
		} u;
		u.f = f;
		return u.i;
	}

	template <SignedIntegral I>
	constexpr auto IntBitsToFloat(I i) noexcept -> IntFloatType<I>
	{
		union
		{
			I i;
			IntFloatType<I> f;
		} u;
		u.i = i;
		return u.f;
	}

	template <UnsignedIntegral I>
	constexpr auto UIntBitsToFloat(I i) noexcept -> UIntFloatType<I>
	{
		union
		{
			I i;
			UIntFloatType<I> f;
		} u;
		u.i = i;
		return u.f;
	}

	template<FloatingPoint F>
	auto Frexp(F f) noexcept -> Pair<F, i32>
	{
		if constexpr (SameAs<F, f32>)
		{
			i32 exp;
			f32 mantissa = frexpf(f, &exp);
			return { mantissa, exp };
		}
		else
		{
			i32 exp;
			f64 mantissa = frexp(f, &exp);
			return { mantissa, exp };
		}
	}

	template <FloatingPoint F>
	auto Ldexp(F mantissa, i32 exponent) noexcept -> F
	{
		if constexpr (SameAs<F, f32>)
		{
			return ldexpf(mantissa, exponent);
		}
		else
		{
			return ldexp(mantissa, exponent);
		}
	}
}
