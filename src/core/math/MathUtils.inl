#pragma once
#if __RESHARPER__
#include "MathUtils.h"
#endif

#include "core/intrin/BitIntrin.h"

#ifdef __INTELLISENSE__
#pragma diag_suppress 438 // supress bogus "expected a ''" due to concepts
#endif

namespace Onca::Math
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

		template <Numeric T, Integral I>
		constexpr auto IntegralPow(T base, I exp) noexcept -> T
		{
			// https://en.wikipedia.org/wiki/Exponentiation_by_squaring
			if (exp == 0)
				return T(1);

			if constexpr (SignedIntegral<I>)
			{
				if (exp < 0)
				{
					base = T(1) / base;
					exp = -exp;
				}
			}

			if (exp == 1)
				return base;

			T y = T(1);
			while (exp > 1)
			{
				if (exp & 1)
				{
					y *= base;
					base *= base;
					exp /= 2;
				}
				else
				{
					base *= base;
					exp /= 2;
				}
			}
			return base * y;
		}

		template <Numeric T>
		constexpr auto ExpHelper(T val) noexcept -> T
		{
			if (val < Consts::MathEpsilon<T> && val > -Consts::MathEpsilon<T>)
				return T(1);

			// Euler's continued fraction for exponential
			// exp(x) = 1 / (1 - x / (1 + x - (1/2)x / (1 + (1/2)x - (1/3)x/(...))))
			T res = T(1);
			for (usize i = 25; i > 1; --i)
				res = T(1) + val / T(i - 1) - val / T(i) / res;
			return T(1) / (T(1) - val / res);
		}

		template <Numeric T>
		constexpr auto FindExp10I(T val) noexcept -> SignedOfSameSize<T>
		{
			using Signed = SignedOfSameSize<T>;

			if (val >= 1 && val <= 10)
				return Signed(0);

			Signed exp = 0;
			if (val < 1)
			{
				while (val < 1)
				{
					val *= 10;
					--exp;
				}
				return exp;
			}

			while (val > 10)
			{
				val /= 10;
				++exp;
			}
			return exp;
		}

		template <Numeric T>
		constexpr auto FindBase10Mantissa(T val) noexcept -> T
		{
			if (val >= 1 && val <= 10)
				return val;

			if (val < 1)
			{
				while (val < 1)
				{
					val *= 10;
				}
				return val;
			}

			while (val > 10)
			{
				val /= 10;
			}
			return val;
		}

		template <Numeric T>
		constexpr auto LnContinuedFraction(T val) noexcept -> T
		{
			// based on continued fraction: https://functions.wolfram.com/ElementaryFunctions/Log/10/0005/
			val = (val - T(1)) / (val + T(1));

			usize maxDepth = 25;
			T val2 = val * val;
			T res = T(2 * maxDepth - 1);
			for (usize i = maxDepth - 1; i > 0; --i)
				res = T(2 * i - 1) - T(i * i) * val2 / res;
			return T(2) * val / res;
		}
	}

	template<typename T0, LessComparable<T0> T1>
	requires ConvertableFrom<T0, T1>
	constexpr auto Min(T0 a, T1 b) noexcept -> T0
	{
		return a < b ? a : T0(b);
	}

	template<typename T0, ConvertableTo<T0> T1, ConvertableTo<T0> T2, ConvertableTo<T0>... Args>
	constexpr auto Min(T0 a, T1 b, T2 c, Args... args) noexcept -> T0
	{
		return Min(a, Min(b, c, args...));
	}

	template<typename T0, LessComparable<T0> T1>
		requires ConvertableFrom<T0, T1>
	constexpr auto Max(T0 a, T1 b) noexcept -> T0
	{
		return a > b ? a : T0(b);
	}

	template <typename T0, ConvertableTo<T0> T1, ConvertableTo<T0> T2, ConvertableTo<T0> ... Args>
	constexpr auto Max(T0 a, T1 b, T2 c, Args... args) noexcept -> T0
	{
		return Max(a, Max(b, c, args...));
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
			return t;
		
		const T values[2] = { t, -t };
		return values[t < 0];
		
	}

	template <Numeric T>
	constexpr auto Ceil(T t) noexcept -> T
	{
		if constexpr (!FloatingPoint<T>)
			return t;

		T trunc = Trunc(t);
		return trunc + (t >= 0 && t != trunc);
	}

	template <Numeric T>
	constexpr auto Floor(T t) noexcept -> T
	{
		if constexpr (!FloatingPoint<T>)
			return t;
		
		T trunc = Trunc(t);
		return trunc - (t < 0 && t != trunc);
	}

	template <Numeric T>
	constexpr auto Trunc(T t) noexcept -> T
	{
		if constexpr (FloatingPoint<T>)
			return T(static_cast<FloatIntType<T>>(t));
		else
			return t;
	}

	template <Numeric T>
	constexpr auto Round(T t) noexcept -> T
	{
		if constexpr (FloatingPoint<T>)
		{
			constexpr double offset[2] = { 0.5f, -0.5f };
			return T(static_cast<FloatIntType<T>>(t + offset[t < 0]));
			
		}
		else
		{
			return t;
		}
	}

	template <Numeric T>
	constexpr auto RoundEven(T t) noexcept -> T
	{
		if constexpr (FloatingPoint<T>)
		{
			using Int = FloatIntType<T>;
			T round = Round(t);
			T diff = Abs(t - round);
			Int possibleVals[2] = { Int(round), Int(t - diff) };
			return T(possibleVals[(diff == 0.5f) && (Int(round) & 1)]);
		}
		else
		{
			return t;
		}
	}

	template <Numeric T>
	constexpr auto Sign(T t) noexcept -> i8
	{
		constexpr i8 signs[3] = { 1, 0, -1 };
		return signs[(t <= 0) + (t < 0)];
	}

	template<Numeric T>
	constexpr auto Fract(T t) noexcept -> T
	{
		if constexpr (FloatingPoint<T>)
			return t - Trunc(t);
		else
			return T(0);
	}

	template <Numeric T>
	constexpr auto Mod(T a, T b) noexcept -> T
	{
		if constexpr (FloatingPoint<T>)
			return a - b * Floor(a / b);
		else
			return a % b;
	}

	template<Numeric T>
	constexpr auto ModF(T t) noexcept -> Pair<T, T>
	{
		if constexpr (FloatingPoint<T>)
		{
			T trunc = Trunc(t);
			return { trunc, t - trunc };
		}
		else
		{
			return { t, T(0) };
		}
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

	template <Numeric T>
	constexpr auto Exp(T power) noexcept -> T
	{
		IF_CONSTEVAL
		{
			if (EpsilonCompare(power, 0))
				return T(1);
			if (power < 2.0)
				Detail::ExpHelper(power);

			using Signed = SignedOfSameSize<T>;
			return Detail::IntegralPow(Consts::E<T>, Signed(Trunc(power))) * Detail::ExpHelper(Fract(power));
		}
		else
		{
			return T(exp(power));
		}
	}

	template <Numeric T>
	constexpr auto Ln(T val) noexcept -> T
	{
		//IF_CONSTEVAL
		{
			if (EpsilonCompare(val, 1))
				return T(0);

			if (val < 0.5 || val > 1.5)
			{
				T res = Consts::LnTen<T> * T(Detail::FindExp10I(val));

				constexpr T integerMantissaLn[10] =
				{
					T(0),
					T(0.6931471805599453094172321214581765680755L),
					T(1.0986122886681096913952452369225257046475L),
					T(1.3862943611198906188344642429163531361510L),
					T(1.6094379124341003746007593332261876395256L),
					T(1.7917594692280550008124773583807022727230L),
					T(1.9459101490553133051053527434431797296371L),
					T(2.0794415416798359282516963643745297042265L),
					T(2.1972245773362193827904904738450514092950L),
					T(2.3025850929940456840179914546843642076011L),
				};
				T mantissa = Detail::FindBase10Mantissa(val);

				res += integerMantissaLn[u8(mantissa) - 1];
				res += Detail::LnContinuedFraction(mantissa / T(u8(mantissa)));
				return res;
			}

			// Continued fractions is only accurate for small values around 1 (0.5-1.5)
			return Detail::LnContinuedFraction(val);
		}
		//else 
		//{
		//	return T(log(val));
		//}
	}

	template <Numeric T, Numeric U>
	constexpr auto Pow(T base, U exp) noexcept -> T
	{
		IF_CONSTEVAL
		{
			if constexpr (Integral<U>)
				return Detail::IntegralPow(base, exp);
			else
				return Exp(exp * Ln(base));
		}

		return T(pow(f64(base), f64(exp)));
	}

	template <Integral T, Integral U>
	constexpr auto CountFactors(T val, U divisor) noexcept -> T
	{
		if (divisor == 0)
			return T(-1);

		T count = 0;
		while (val % divisor == 0)
		{
			val /= divisor;
			++count;
		}
		return count;
	}

	template <Integral I>
	constexpr auto Log2(I val) noexcept -> u8
	{
		return Intrin::BitScanMSB(val);
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

	template <Numeric T, ConvertableTo<T> U>
	constexpr auto EpsilonCompare(T a, U b, T e) noexcept -> bool
	{
		return Abs(a - T(b)) <= T(e);
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
