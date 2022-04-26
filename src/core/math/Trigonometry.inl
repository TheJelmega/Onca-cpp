#pragma once
#include "Trigonometry.h"
#include "MathUtils.h"

namespace Onca::Math
{
	namespace Detail
	{
		// tan could also be calculated using the taylor series, making the expression simpler by first converting tan to its cotangent
		// tan(x - pi/2) = -cot(x)
		// expanded taylor series for -cot(x)
		// -( 1/x - x/3 - x^3/45 - x^5/945 - x^5/4725 )
		// =  1/x + x/3 + x^3/45 + x^5/945 + x^5/4725 )
		// This expansion should be good enough for very small values
		// Each element can be calculates as:
		//  (-1)^n * 2^2n * B(2n)
		// ----------------------- x^(2n-1)
		//          (2n)!
		template<Numeric T>
		constexpr auto CompTanHelper(T val) noexcept -> T
		{
			// tan(x) == tan(x + pi)
			if (val > Consts::Pi<T>)
				val -= Consts::Pi<T>;

			/// HalfPi return the value that would be returned by std::tan(HalfPi)
			if (EpsilonCompare(val, Consts::HalfPi<T>))
				return T(16331239353195370.);

			// Recurse 11 times, smallest values have no impact on the value (double precision)
			// Calculate using continued fraction: tan(x) = val / (3 - x^2 / (5 - x^2 / (...)))
			usize maxDepth = 11;
			T val2 = val * val;
			T res = T(2 * maxDepth - 1);
			for (usize i = maxDepth; i > 0; --i)
				res = T(2 * i - 1) - val2 / res;
			return val / res;
		}

		template<Numeric T>
		constexpr auto CompTan(T val) noexcept -> T
		{
			if (val < 0)
				return -CompTanHelper(-val);
			return CompTanHelper(val);
		}

		template<Numeric T>
		constexpr auto CompSin(T val) noexcept -> T
		{
			if (EpsilonCompare(val, 0) || EpsilonCompare(val, Consts::HalfPi<T>) || EpsilonCompare(val, Consts::TwoPi<T>))
				return T(0);
			if (EpsilonCompare(val, Consts::HalfPi<T>))
				return T(1);
			if (EpsilonCompare(val, Consts::ThreeOverTwoPi<T>))
				return T(-1);

			// https://en.wikipedia.org/wiki/Tangent_half-angle_formula
			// sin(x) = 2*tan(x/2) / (1 - tan(x/2)^2)
			T tanHalfAngle = CompTan(val / T(2));
			return T(2) * tanHalfAngle / (T(1) + tanHalfAngle * tanHalfAngle);
		}

		template<Numeric T>
		constexpr auto CompCos(T val) noexcept -> T
		{
			// special cases (0, pi/2 (90), pi (180), 3pi/2 (270), 2pi (360))
			if (EpsilonCompare(val, 0) || EpsilonCompare(val, Consts::TwoPi<T>))
				return T(1);
			if (EpsilonCompare(val, Consts::Pi<T>))
				return T(-1);
			if (EpsilonCompare(val, Consts::HalfPi<T>) || EpsilonCompare(val, Consts::ThreeOverTwoPi<T>))
				return T(0);

			// https://en.wikipedia.org/wiki/Tangent_half-angle_formula
			// cos(x) = (1 - tan(x/2)^2) / (1 + tan(x/2)^2)
			T tanHalfAngle = CompTan(val / T(2));
			return (1 - tanHalfAngle * tanHalfAngle) / (T(1) + tanHalfAngle * tanHalfAngle);
		}

		template<Numeric T>
		constexpr auto CompATanHelper(T val) noexcept -> T
		{
			if (EpsilonCompare(val * val, 16331239353195370.))
				return Consts::HalfPi<T>;

			if (val > 3)
			{
				// Infinite series
				usize maxDepth = 10;
				T val2 = val * val;
				T basePow = val2 * val;
				T res = Consts::HalfPi<T> - T(1) / val;

				for (usize i = maxDepth; i > 0; --i)
				{
					res += T(1) / (T((maxDepth - i + 1) * 4 - 1) * basePow);
					basePow *= val2;
					res -= T(1) / (T((maxDepth - i + 1) * 4 + 1) * basePow);
					basePow *= val2;
				}
				return res;
			}

			// Calculate using continues fraction: atan(x) = x / (1 + (1x)^2 / (3 + (2x)^2 / (5 - (3x)^2 / (...))))
			// values are smallest values before changes in the results are noticeable (double precision)
			usize maxDepth = 56;
			if (val < 0.5) maxDepth = 13;
			else if (val < 1.0) maxDepth = 21;
			else if (val < 1.5) maxDepth = 30;
			else if (val < 2.0) maxDepth = 39;
			else if (val < 2.5) maxDepth = 47;

			T val2 = val * val;
			T res = T(2 * maxDepth - 1);
			for (usize i = maxDepth; i > 0; --i)
				res = (T(2 * i - 1)) + i * i * val2 / res;
			return val / res;
		}

		template<Numeric T>
		constexpr auto CompATan(T val) noexcept -> T
		{
			// special cases
			if (EpsilonCompare(val, 0.0))
				return T(0);

			if (val < 0)
				return -CompATanHelper(-val);
			return CompATanHelper(val);
		}

		template<Numeric T>
		constexpr auto CompASin(T val) noexcept -> T
		{
			// special cases
			if (EpsilonCompare(val, 0))
				return T(0);
			if (EpsilonCompare(val, 1))
				return Consts::HalfPi<T>;
			if (EpsilonCompare(val, -1))
				return Consts::ThreeOverTwoPi<T>;

			// arcsin(x) = arctan( x / sqrt(1 - x^2) )
			val = val / Sqrt(T(0) - val * val);

			if (val < 0)
				return -CompATan(-val);
			return CompATan(val);
		}

		template<Numeric T>
		constexpr auto CompACos(T val) noexcept -> T
		{
			// Special cases
			if (EpsilonCompare(val, -1.0))
				return T(0);
			if (EpsilonCompare(val, 0.0))
				return Consts::HalfPi<T>;
			if (EpsilonCompare(val, 1.0))
				return Consts::Pi<T>;

			// arccos(x) = arctan(sqrt(1 - x*x) / x)
			val = T(Sqrt(1.0 - val * val)) / val;

			if (val < 0)
				return Consts::Pi<T> - CompATan(-val);
			return CompATan(val);
		}

		template<Numeric T>
		constexpr auto CompATan2(T y, T x) noexcept -> T
		{
			// Invalid  case
			if (EpsilonCompare(y, 0) && EpsilonCompare(x, 0))
				return 0.0;

			// 90 or 270 degrees
			if (EpsilonCompare(x, 0))
				return y > 0 ? Consts::HalfPi<T> : -Consts::HalfPi<T>;

			T val = CompATan(y / x);

			// val is negative when the signs differ

			if (x >= 0 && y >= 0)
				return val;
			if (x >= 0 && y < 0)
				return -val;
			if (x < 0 && y >= 0)
				return val + Consts::Pi<T>;

			// (x < 0 && y < 0)
			return val  - Consts::Pi<T>;
		}
	}

	template<Numeric T, AngleKind Kind>
	constexpr auto Sin(const Angle<T, Kind>& angle) noexcept -> T
	{
		T rads = angle.ToRadians().val;
		IF_CONSTEVAL
		{
			return Detail::CompSin(rads);
		}
		else
		{
			return T(sin(rads));
		}
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Cos(const Angle<T, Kind>& angle) noexcept -> T
	{
		T rads = angle.ToRadians().val;
		IF_CONSTEVAL
		{
			return Detail::CompCos(rads);
		}
		else
		{
			return T(cos(rads));
		}
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Tan(const Angle<T, Kind>& angle) noexcept -> T
	{
		T rads = angle.ToRadians().val;
		IF_CONSTEVAL
		{
			return Detail::CompTan(rads);
		}
		else
		{
			return T(tan(rads));
		}
	}

	template <Numeric T>
	constexpr auto ASin(T val) noexcept -> Angle<T, AngleKind::Radians>
	{
		IF_CONSTEVAL
		{
			return Angle<T, AngleKind::Radians>{ T(Detail::CompASin(val)) };
		}
		else
		{
			return Angle<T, AngleKind::Radians>{ T(asin(val)) };
		}
	}

	template <Numeric T>
	constexpr auto ACos(T val) noexcept -> Angle<T, AngleKind::Radians>
	{
		IF_CONSTEVAL
		{
			return Angle<T, AngleKind::Radians>{ T(Detail::CompACos(val)) };
		}
		else
		{
			return Angle<T, AngleKind::Radians>{ T(acos(val)) };
		}
	}

	template <Numeric T>
	constexpr auto ATan(T val) noexcept -> Angle<T, AngleKind::Radians>
	{
		IF_CONSTEVAL
		{
			return Angle<T, AngleKind::Radians>{ T(Detail::CompATan(val)) };
		}
		else
		{
			return Angle<T, AngleKind::Radians>{ T(atan(val)) };
		}
	}

	template <Numeric T>
	constexpr auto ATan2(T y, T x) noexcept -> Angle<T, AngleKind::Radians>
	{
		IF_CONSTEVAL
		{
			return Angle<T, AngleKind::Radians>{ T(Detail::CompATan2(y, x)) };
		}
		else
		{
			return Angle<T, AngleKind::Radians>{ T(atan2(y, x)) };
		}
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Sinh(const Angle<T, Kind>& angle) noexcept -> T
	{
		T rads = angle.ToRadians().val;
		IF_CONSTEVAL
		{
			if (EpsilonCompare(rads, 0))
				return T(0);
			return (Exp(rads) - Exp(-rads)) / T(2);
		}
		else
		{
			return T(sinh(rads));
		}
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Cosh(const Angle<T, Kind>& angle) noexcept -> T
	{
		T rads = angle.ToRadians().val;
		IF_CONSTEVAL
		{
			if (EpsilonCompare(rads, 0))
				return T(0);
			return (Exp(rads) + Exp(-rads)) / T(2);
		}
		else
		{
		return T(cosh(rads));
		}
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Tanh(const Angle<T, Kind>& angle) noexcept -> T
	{
		T rads = angle.ToRadians().val;
		IF_CONSTEVAL
		{
			if (EpsilonCompare(rads, 0))
				return T(0);

			T rads2 = T(2) * rads;
			return (Exp(rads2) - 1) / (Exp(rads2) + 1);
		}
		else
		{
		return T(tanh(rads));
		}
	}

	template <Numeric T>
	constexpr auto ASinh(T val) noexcept -> Angle<T, AngleKind::Radians>
	{
		IF_CONSTEVAL
		{
			T res = Ln(val + Sqrt(val * val + T(1)));
			return Angle<T, AngleKind::Radians>{ res };
		}
		else
		{
			return Angle<T, AngleKind::Radians>{ T(asinh(val)) };
		}
	}

	template <Numeric T>
	constexpr auto ACosh(T val) noexcept -> Angle<T, AngleKind::Radians>
	{
		IF_CONSTEVAL
		{
			if (val < 1.0)
				return Consts::QNaN<T>;

			T res = Ln(val + Sqrt(val * val - T(1)));
			return Angle<T, AngleKind::Radians>{ res };
		}
		else
		{
			return Angle<T, AngleKind::Radians>{ T(asinh(val)) };
		}
	}

	template <Numeric T>
	constexpr auto ATanh(T val) noexcept -> Angle<T, AngleKind::Radians>
	{
		IF_CONSTEVAL
		{
			if (val >= 1)
				return Consts::Infinity<T>;
			if (val <= -1)
				return -Consts::Infinity<T>;

			T res = Ln((1 + val) / T(1 - val)) / T(2);
			return Angle<T, AngleKind::Radians>{ res };
		}
		else
		{
			return Angle<T, AngleKind::Radians>{ T(asinh(val)) };
		}
	}
}
