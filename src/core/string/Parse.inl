#pragma once
#if __RESHARPER__
#endif
#include <charconv>

#include "Parse.h"

#include "core/math/IntUtils.h"

namespace Core
{
	namespace Detail::Parse
	{
		inline auto ComputeF64(u64 i, i64 power, bool isNegative) -> Pair<f64, bool>
		{
			ASSERT(power >= SmallestPower, "Power is too small");
			ASSERT(power <= LargestPower, "Power is too large");

			if (power >= -22 && power <= 22 && i <= 9007199254740991)
			{
				// Convert the u64 to i64, lossless since 0 <= i <= 2^53 - 1
				f64 d = f64(i);

				// If 0 <= s< 2^53 and 0 <= p <= 10^22 then
				// 1) both can be represented as exact 64-bit floating point values
				// 2) since both can be represented, s * p and s / p, should give accurate results
				if (power < 0)
					d /= PowerOfTen64[-power];
				else
					d *= PowerOfTen64[power];

				if (isNegative)
					d = -d;

				return { d, true };
			}

			// A second fast path is defined by David M. Gay in  "Correctly rounded binary-decimal and decimal-binary conversions." (1990)
			// to calculate values for i * 10^(22 + x), where x < 16
			// 1) Compute i * 10^x, result should be exact
			// 2) Proceed to compute (i * 10 ^ x) * 10 ^ 22
			// This is the fastest path for numbers with power of 22 < power < 22 + 16
			// But this optimization path is very rare:
			// http://www.exploringbinary.com/fast-path-decimal-to-floating-point-conversion/
			// https://rapidjson.org/strtod_8h_source.html


			// The fast path failed, so use a slower path
			// We need to adjust i, so that it is > 1 << 63, this is not possible for 0, so it handled separately
			if (i == 0)
				return { isNegative ? -0.0 : 0.0, true };

			u64 factorMantissa = mantissa64[power - SmallestPower];

			// The exponent is 1024 + 63 + power + floor(log(5^power)/log(2))
			// 1024 comes from the ieee64 standard
			// 63 comes from using a 64-bit value
			//
			// A fast way to compute floor(log(5^power)/log(2)) is the following:
			// For a power in (400,-350), we have that
			// ((152170 + 65536) * power) >> 16
			// is
			// floor(log(5^power)/log(2)) + power when power >= 0
			// and
			// floor(log(5^-power)/log(2)) - power when power < 0
			//
			// 65536 is 1<<16, so corresponds to (65536 * power) >> 16
			// floor(log(5^-power)/log(2)) is (152170 * power) >> 16
			//
			// This is because 152170/ (1<<16) is approximately log(5)/log(2)

			i64 exp = (((152170 + 65536) * power) >> 16) + 1024 + 63;
			u32 lz = Intrin::ZeroCountLSB(i);
			i <<= lz;

			U128 product = Umul128(i, factorMantissa);
			u64 lower = product.low;
			u64 upper = product.high;

			// As long as the first 9 bits of the "upper" are not "1", we know we have an exact computed value for the 55 bits,
			// because any imprecision would play out as a +1, in the worst case
			// 55 bits are needed:
			// - 53 bits for the mantissa
			// - 1 rounding bit
			// - 1 bit to wast if the most significant bit of the product is 0
			//
			// It's expected that this branch is rarely taken (probably 1% of the time)
			if ((product.high & 0x1FF) == 0x1FF && (product.low + i < product.low)) UNLIKELY
			{
				u64 factorMantissaLow = mantissa64_128[power - SmallestPower];


				product = Umul128(i, factorMantissaLow);

				u64 prodLow = product.low;
				u64 prodMid = product.high + lower;
				u64 prodHi = upper;

				if (prodMid < lower)
					++prodHi;

				// Check whether the mantissa * i + i would affect the result, this can happen with 7.3177701707893310e+15
				if (prodMid + 1 == 0 && (prodHi & 0x1FF) == 0x1FF && prodLow + 1 < prodLow)
					return { 0, false };

				upper = prodHi;
				lower = prodMid;
			}
				
			u64 upperBit = upper >> 63;
			u64 mantissa = upper >> (upperBit + 9);
			lz += u32(1 ^ upperBit);

			if (lower == 0 && (upper & 0x1FF) == 0 && (mantissa & 3) == 1)
				return { 0, false };

			mantissa += mantissa & 1;
			mantissa >>= 1;

			if (mantissa >= (1ull << 53))
			{
				mantissa = 1ull << 52;
				--lz;
			}

			mantissa &= ~(1ull << 52);
			u64 realExp = exp - lz;
			if (realExp < 0 || realExp > 2046) UNLIKELY
				return { 0, false };

			mantissa |= realExp << 52;
			mantissa |= u64(isNegative) << 63;
			return { Math::UIntBitsToFloat(mantissa), true };
		}
			
	}

	template<typename T>
	auto Parse(const String& str) noexcept -> T
	{
		STATIC_ASSERT(false, "Cannot parse type");
		return T{};
	}

	template <typename T>
	auto TryParse(const String& str) noexcept -> Optional<T>
	{
		return NullOpt;
	}

	template <UnsignedIntegral U>
	auto Parse(const String& str) noexcept -> U
	{
		U val = 0;
		for (UCodepoint cp : str)
		{
			val *= 10;
			val += cp - '0';
		}
		return  val;
	}

	template <UnsignedIntegral U>
	auto TryParse(const String& str) noexcept -> Optional<U>
	{
		if (str.IsEmpty())
			return NullOpt;

		U val = 0;
		for (UCodepoint cp : str)
		{
			if (cp < '0' || cp > '9')
				return NullOpt;

			val *= 10;
			val += cp - '0';
		}
		return  val;
	}

	template <SignedIntegral I>
	auto Parse(const String& str) noexcept -> I
	{
		bool neg = str[0] == '-';
		String::ConstIterator it = str.Begin() + neg;
		String::ConstIterator end = str.End();

		I val = 0;
		for (; it != end; ++it)
		{
			val *= 10;
			val += *it - '0';
		}
		return neg ? -val : val;
	}

	template <SignedIntegral I>
	auto TryParse(const String& str) noexcept -> Optional<I>
	{
		if (str.IsEmpty())
			return NullOpt;

		bool neg = str[0] == '-';
		String::ConstIterator it = str.Begin() + neg;
		String::ConstIterator end = str.End();

		I val = 0;
		for (; it != end; ++it)
		{
			UCodepoint cp = *it;

			if (cp < '0' || cp > '9')
				return NullOpt;

			val *= 10;
			val += cp - '0';
		}
		return neg ? -val : val;
	}

	template <FloatingPoint F>
	auto Parse(const String& str) noexcept -> F
	{
		Optional<F> res = TryParse<F>(str);
		return res.value_or(F(0));
	}

	template <FloatingPoint F>
	auto TryParse(const String& str) noexcept -> Optional<F>
	{
		if (str.IsEmpty())
			return NullOpt;

		// Parse both a double and float as if they doubles, then narrow if needed
		const char* it = reinterpret_cast<const char*>(str.Data());
		String::ConstIterator end = str.End();

		bool neg = false;
		if (*it == '-')
		{
			neg = true;
			++it;
			if (!Unicode::IsDigit(*it))
				return NullOpt;
		}

		const char* digitStart = it;
		u64 i;
		if (*it == '0')
		{
			// '0' cannot be followed by an integer
			++it;
			if (Unicode::IsDigit(*it))
				return NullOpt;
			i = 0;
		}
		else
		{
			if (!Unicode::IsDigit(*it))
				return NullOpt;

			i = *it - '0';
			++it;

			while (Unicode::IsDigit(*it))
			{
				i *= 10;
				i += *it - '0';
				++it;
			}
		}

		i64 exp = 0;
		if (*it == '.')
		{
			++it;
			if (!Unicode::IsDigit(*it))
				return NullOpt;

			const char* firstAfterPeriod = it;

			i *= 10;
			i += *it - '0';
			++it;

			while (Unicode::IsDigit(*it))
			{
				i *= 10;
				i += *it - '0';
				++it;
			}
			exp = firstAfterPeriod - it;
		}

		i32 digitCount = i32(it - digitStart) - 1;

		if (*it == 'e' || *it == 'E')
		{
			++it;
			bool negExp = *it == '-';
			if (negExp)
				++it;
			else if (*it == '+')
				++it;

			if (!Unicode::IsDigit(*it))
				return NullOpt;

			i64 expNum = *it - '0';
			++it;

			if (Unicode::IsDigit(*it))
			{
				expNum *= 10;
				expNum += *it - '0';
				++it;

				if (Unicode::IsDigit(*it))
				{
					expNum *= 10;
					expNum += *it - '0';
					++it;

					while (Unicode::IsDigit(*it))
					{
						if (expNum < 0x100000000)
						{
							expNum *= 10;
							expNum += *it - '0';
						}
						++it;
					}
				}
			}

			exp += (negExp ? -expNum : expNum);
		}

		const char* pInit = reinterpret_cast<const char*>(str.Data());
		if (digitCount >= 19) UNLIKELY
		{
			const char* start = digitStart;
			while (*start == '0' || (*start == '.'))
				++start;

			digitCount -= i32(start - digitStart);
			if (digitCount >= 19)
			{
				// Fallback on std implementation
				f64 res;
				bool success = u32(std::from_chars(pInit, it, res).ec) == 0;
				if (success)
					return NullOpt;
				return F(res);
			}
		}

		if (exp < Detail::Parse::SmallestPower || exp > Detail::Parse::LargestPower)
		{
			// Fallback on std implementation
			f64 res;
			bool success = u32(std::from_chars(pInit, it, res).ec) == 0;
			if (success)
				return NullOpt;
			return F(res);
		}

		auto [res, success] = Detail::Parse::ComputeF64(i, exp, neg);

		if (!success)
		{
			// Fallback on std implementation
			f64 res;
			bool success = u32(std::from_chars(pInit, it, res).ec) == 0;
			if (success)
				return NullOpt;
		}
		return F(res);
	}

	template <>
	inline auto Parse<bool>(const String& str) noexcept -> bool
	{
		return str.Length() <= 4 &&
			(str == "1"_s ||
			 str.AsLower() == "true"_s);
	}

	template <>
	inline auto TryParse<bool>(const String& str) noexcept -> Optional<bool>
	{
		if (str.Length() > 5)
			return NullOpt;

		if (str.Length() <= 4 &&
			(str == "1"_s ||
			 str.AsLower() == "true"_s))
			return true;

		if (str.Length() <= 4 &&
			(str == "0"_s ||
			 str.AsLower() == "false"_s))
			return false;

		return NullOpt;
	}
}
