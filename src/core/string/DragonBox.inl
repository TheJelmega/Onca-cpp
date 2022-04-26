#pragma once
#if __RESHARPER__
#include "DragonBox.h"
#endif

#include "core/Assert.h"

namespace Onca::DragonBox
{
	namespace Log
	{
		constexpr auto FloorShift(u32 intPart, u64 fractPart, usize shift) noexcept -> i32
		{
			ASSERT(shift < 32, "Shift too large");
			ASSERT(shift == 0 || intPart < (1u << (32 - shift)), "Overflow will occur");

			return i32(shift == 0 ? intPart : (u64(intPart) << shift) | (fractPart >> (64 - shift)));
		}

		template <u32 CIntPart, u64 CFractDigits, usize Shift, i32 MaxExp, u32 SIntPart, u64 SFractPart>
		constexpr auto Compute(i32 exp) noexcept -> i32
		{
			ASSERT(exp <= MaxExp && exp >= -MaxExp, "Exponent out of range");
			constexpr i32 c = FloorShift(CIntPart, CFractDigits, Shift);
			constexpr i32 s = FloorShift(SIntPart, SFractPart, Shift);
			return i32((exp * c - s) >> Shift);
		}

		constexpr auto FloorLog10Pow2(i32 exp) noexcept -> i32
		{
			return Compute<0, Log10_2FractDigits, FloorLog10Pow2Shift, FloorLog10Pow2InputLimit>(exp);
		}

		constexpr auto FloorLog2Pow10(i32 exp) noexcept -> i32
		{
			return Compute<3, Log2_10FractDigits, FloorLog2Pow10Shift, FloorLog2Pow10InputLimit>(exp);
		}

		constexpr auto FloorLog5Pow2(i32 exp) noexcept -> i32
		{
			return Compute<0, Log5_2FractDigits, FloorLog5Pow2Shift, FloorLog5Pow2InputLimit>(exp);
		}

		constexpr auto FloorLog5Pow2MinusLog5_3(i32 exp) noexcept -> i32
		{
			return Compute<0, Log5_2FractDigits, FloorLog5Pow2Shift, FloorLog5Pow2MinusLog5_3InputLimit, 0, Log5_3FractDigits>(exp);
		}

		constexpr auto FloorLog10Pow2MinusLog10_4Over3(i32 exp) noexcept -> i32
		{
			return Compute<0, Log10_2FractDigits, FloorLog10Pow2Shift, FloorLog10Pow2MinusLog10_4Over3InputLimit, 0, Log10_4Over3FractDigits>(exp);
		}
	}

	template<UnsignedIntegral U, U A>
	constexpr auto ModularInverse(u32 bitWidth) noexcept -> u32
	{
		// By Euler's theorem, a^phi(2^n) == 1 (mod 2^n), where phi(2^n) = 2^(n-1), so the
		// modular inverse of a is a^(2^(n-1) - 1) = a^(1 + 2 + 2^2 + ... + 2^(n-2)).
		u64 modInv = 1;
		for (usize i = 1; i < bitWidth; ++i)
			modInv *= modInv * A;

		if (bitWidth >= Math::Consts::Digits<U>)
			return u32(modInv);

		u32 mask = (1 << bitWidth) - 1;
		return u32(modInv & mask);
	}

	template <usize TableSize, UnsignedIntegral U>
	constexpr auto DivisibleByPowerOf5(U x, u32 exp) noexcept -> bool
	{
		constexpr const auto& divTable = DivisibilityCheckTable<U, 5, TableSize>;
		ASSERT(exp < divTable.size, "DragonBox: Exponent is too large");
		return x * divTable.table[exp].modInv <= divTable.table[exp].maxQuotient;
	}

	template <UnsignedIntegral U>
	constexpr auto DivisibleByPowerOf2(U x, u32 exp) noexcept -> bool
	{
		ASSERT(exp >= 1, "DragonBox: Exponent needs to be larger than 0");
		ASSERT(x != 0, "DragonBox: x cannot be 0");
		return Intrin::ZeroCountMSB(x) >= exp;
	}

	template <i32 N>
	constexpr auto CheckDivisibilityAndDivideByPow10(u32& n) noexcept -> bool
	{
		// Make sure the computation for max_n does not overflow.
		STATIC_ASSERT(N + 1 < Log::FloorLog10Pow2(31), "DragonBox: Overflow");
		ASSERT((n <= Math::Pow(10u, N + 1)), "DragonBox: n needs to be smaller than 10^N");

		using Info = CheckDivisibilityAndDivideByPow10Info<N>;
		n *= Info::MagicNumber;

		// Mask for the lowest (N + bits_for_comparison)-bits.
		STATIC_ASSERT(Info::BitsForComp + N < 32, "DragonBox: N is too large");
		constexpr u32 compMask = u32(1u << (N + Info::BitsForComp)) - 1u;

		// The lowest N bits of n must be zero, and
		// (n & comparison_mask) >> N must be at most threshold.
		u32 c = Intrin::RotateR(n & compMask, N);

		n >>= Info::ShiftAmount;
		return c <= Info::Threshold;
	}

	template <i32 N>
	constexpr auto SmallDivisionByPow10(u32 n) noexcept -> u32
	{
		ASSERT(n <= Math::Pow(10u, N + 1), "DragonBox: n needs to be smaller than 10^N");
		using Info = SmallDivisionByPow10Info<N>;
		return (n * Info::MagicNumber) >> Info::ShiftAmount;
	}

	template <i32 N, i32 MaxPow2, i32 MaxPow5, UnsignedIntegral U>
	constexpr auto DivideByPow10(U n) noexcept -> U
	{
		STATIC_ASSERT(N > 0, "DragonBox: N needs to be larger than 0");
		// Ensure no overflow
		STATIC_ASSERT(((MaxPow2 + Log::FloorLog2Pow10(MaxPow5) - MaxPow5) < Math::Consts::Digits<U>), "DragonBox: Overflow");

		// Specialize for 64-bit division by 1000.
		// Ensure that the correctness condition is met.
		if constexpr (IsU64<U> &&
					  N == 3 &&
					  MaxPow2 + Log::FloorLog2Pow10(N + MaxPow5) - (N + MaxPow5) < 70)
		{
			return Umul128Upper64(n, 0x8312'6E97'8D4F'DF3C) >> 9;
		}
		else
		{
			constexpr U divisor = Math::Pow(U(10), N);
			return n / divisor;
		}
	}

	namespace Detail
	{
		template <FloatingPoint F>
		template <typename Ret, typename IntervalType, typename TrailingZeroPolicy, typename BinaryToDecimalRoundingPolicy, typename CachePolicy, typename ... Args>
		auto Impl<F>::ComputeNearestNormal(Carrier twoFc, i32 exp, Args... args) noexcept -> Ret
		{
			Ret res;
			IntervalType interval{ args... };

			// Step 1: Schubfach multiplier calculation

			// Compute k and beta.
			const i32 minusK = Log::FloorLog10Pow2(exp) - Kappa;
			const CacheEntryType cache = CachePolicy::template GetCache<F>(-minusK);
			const i32 betaMinus1 = exp + Log::FloorLog2Pow10(-minusK);

			// Compute zi and deltai.
			// 10^kappa <= deltai < 10^(kappa + 1)
			const u32 deltaI = ComputeDelta(cache, betaMinus1);
			const Carrier twoFr = twoFc | 1;
			const Carrier zi = ComputeMul(twoFr << betaMinus1, cache);

			// Step 2: Try large divisor, remove trailing zeros if necessary
			constexpr u32 BigDivisor = Math::Pow(10u, Kappa + 1);
			constexpr u32 SmallDivisor = Math::Pow(10u, Kappa);

			// Using an upper bound on zi, we might be able to optimize the division
			// better than the compiler; we are computing zi / big_divisor here.

			res.significand = DivideByPow10<Kappa + 1, Math::Consts::SignificandBits<F> + Kappa + 2, Kappa + 1>(zi);
			u32 r = u32(zi - BigDivisor * res.significand);

			if (r > deltaI)
			{
				goto SmallDivisorCaseLabel_;
			}
			else if (r < deltaI)
			{
				// Exclude the right endpoint
				if (r == 0 && !interval.IncludeRightEndpoint() &&
					IsProductInteger<IntegerCheckCaseId::FcPmHalf>(twoFr, exp, minusK))
				{
					if constexpr (BinaryToDecimalRoundingPolicy::Tag == Policies::Detail::BinaryToDecimalRounding::TagT::DoNotCare)
					{
						res.significand *= 10;
						res.exponent = minusK + Kappa;
						--res.significand;
						return res;
					}
					else
					{
						--res.significand;
						r = BigDivisor;
						goto SmallDivisorCaseLabel_;
					}
				}
			}
			else
			{
				// r == deltai; compare fractional parts.
				// Check conditions in the order different from the paper
				// to take advantage of short-circuiting.

				const Carrier twoFl = twoFc - 1;
				if ((!interval.IncludeLeftEndpoint() ||
					 !IsProductInteger<IntegerCheckCaseId::FcPmHalf>(twoFl, exp, minusK)) &&
					!ComputeMulParity(twoFl, cache, betaMinus1))
					goto SmallDivisorCaseLabel_;
			}
			res.exponent = minusK + Kappa + 1;

			// We may beed to remove tailing zeros
			TrailingZeroPolicy::template OnTrailingZeros<Impl>(res);
			return res;

			// Step 3: Find the significand with the smaller divisor
		SmallDivisorCaseLabel_:
			TrailingZeroPolicy::template NoTrailingZeros<Impl>(res);
			res.significand *= 10;
			res.exponent = minusK + Kappa;

			if constexpr (BinaryToDecimalRoundingPolicy::Tag == Policies::Detail::BinaryToDecimalRounding::TagT::DoNotCare)
			{
				// Normally, we want to compute
				// ret_value.significand += r / small_divisor
				// and return, but we need to take care of the case that the resulting
				// value is exactly the right endpoint, while that is not included in the
				// interval.
				if (interval.IncludeRightEndpoint())
				{
					// Is r divisible by 10^kappa?
					if (CheckDivisibilityAndDivideByPow10<Kappa>(r) &&
						IsProductInteger<IntegerCheckCaseId::FcPmHalf>(twoFr, exp, minusK))
						// This should be in the interval
						res.significand += r - 1;
					else
						res.significand += r;
				}
				else
				{
					res.significand += SmallDivisionByPow10<Kappa>(r);
				}
			}
			else
			{
				u32 dist = r - deltaI / 2 + SmallDivisor / 2;
				const bool approxYParity = ((dist ^ (SmallDivisor / 2)) & 1) != 0;

				// Is dist divisible by 10^kappa
				bool divisibleBy10ToTheKappa = CheckDivisibilityAndDivideByPow10<Kappa>(dist);

				// Add dist / 10^kappa to the significand
				res.significand += dist;

				if (divisibleBy10ToTheKappa)
				{
					// Check z^(f) >= epsilon^(f)
					// We have either yi == zi - epsiloni or yi == (zi - epsiloni) - 1,
					// where yi == zi - epsiloni if and only if z^(f) >= epsilon^(f).
					// Since there are only 2 possibilities, we only need to care about the
					// parity. Also, zi and r should have the same parity since the divisor is
					// an even number.
					if (ComputeMulParity(twoFc, cache, betaMinus1) != approxYParity)
					{
						--res.significand;
					}
					else
					{
						// If z^(f) >= epsilon^(f), we might have a tie
						// when z^(f) == epsilon^(f), or equivalently, when y is an integer.
						// For tie-to-up case, we can just choose the upper one.
						if (BinaryToDecimalRoundingPolicy::PreferRoundDown(res) &&
							IsProductInteger<IntegerCheckCaseId::Fc>(twoFc, exp, minusK))
							--res.significand;
					}
				}
			}
			return res;
		}

		template <FloatingPoint F>
		template <typename Ret, typename IntervalType, typename TrailingZeroPolicy, typename BinaryToDecimalRoundingPolicy, typename CachePolicy, typename ...
			Args>
			auto Impl<F>::ComputeNearestShorter(i32 exp, Args... args) noexcept -> Ret
		{
			Ret res;
			IntervalType interval{ args... };

			// compute k and beta
			const i32 minusK = Log::FloorLog10Pow2MinusLog10_4Over3(exp);
			const i32 betaMinus1 = exp + Log::FloorLog2Pow10(-minusK);

			// Compute xi and zi
			const CacheEntryType cache = CachePolicy::template GetCache<F>(-minusK);
			Carrier xi = ComputeLeftEndpointForShorterIntervalCase(cache, betaMinus1);
			Carrier zi = ComputeRightEndpointForShorterIntervalCase(cache, betaMinus1);

			// If we don't accept the right endpoint and
			// if the right endpoint is an integer, decrease it
			if (!interval.IncludeRightEndpoint() &&
				IsRightEndpointIntergerShorterInterval(exp))
				--zi;

			// If we don't accept the left endpoint or
			// if the left endpoint is not an integer, increase it
			if (!interval.IncludeLeftEndpoint() ||
				!IsLeftEndpointIntegerShorterInterval(exp))
				++xi;

			// Try bigger divisor
			res.significand = zi / 10;

			// If succeed, remove trailing zeros if necessary and return
			if (res.significand * 10 >= xi)
			{
				res.exponent = minusK + 1;
				TrailingZeroPolicy::template OnTrailingZeros<Impl>(res);
				return res;
			}

			// Otherwise, compute the round-up of y
			TrailingZeroPolicy::template NoTrailingZeros<Impl>(res);
			res.significand = ComputeRoundUpForShorterIntervalCase(cache, betaMinus1);
			res.exponent = minusK;

			// When tie occurs, choose one of them according to the rule
			if (BinaryToDecimalRoundingPolicy::PreferRoundDown(res) &&
				exp >= ShorterIntervalTieLowerThreshold &&
				exp <= ShorterIntervalTieUpperThreshold)
				--res.significand;
			else if (res.significand < xi)
				++res.significand;

			return res;
		}

		template <FloatingPoint F>
		template <typename Ret, typename TrailingZeroPolicy, typename CachePolicy>
		auto Impl<F>::ComputeLeftClosedDirected(Carrier twoFc, i32 exp) noexcept -> Ret
		{
			// Step 1: Schubfach multiplier calculation

			Ret res;

			const i32 minusK = Log::FloorLog10Pow2(exp) - Kappa;
			const CacheEntryType cache = CachePolicy::template GetCache<F>(-minusK);
			const i32 betaMinus1 = exp + Log::FloorLog2Pow10(-minusK);

			// Compute xi and deltai
			// 10^kappa <= deltai < 10^(kappa + 1)
			const u32 deltaI = ComputeDelta(cache, betaMinus1);
			Carrier xi = ComputeMul(twoFc << betaMinus1, cache);

			if (!IsProductInteger<IntegerCheckCaseId::Fc>(twoFc, exp, minusK))
				++xi;

			// Step 2 : Try larger divisor; remove trailing zeros if necessary
			constexpr u32 BigDivisor = Math::Pow(10u, Kappa + 1);

			// Using an upper bound on xi, we might be able to optimize the division
			// better than the compiler; we are computing xi / bigDivisor here
			res.significand = DivideByPow10<Kappa + 1, Math::Consts::SignificandBits<F> +Kappa + 2, Kappa + 1>(xi);
			u32 r = u32(xi - BigDivisor * res.significand);

			if (r != 0)
			{
				++res.significand;
				r = BigDivisor - r;
			}

			if (r > deltaI)
			{
				goto SmallDivisorCaseLabel_;
			}
			else if (r == deltaI)
			{
				// Compare the fractional parts
				if (ComputeMulParity(twoFc + 2, cache, betaMinus1) ||
					IsProductInteger<IntegerCheckCaseId::Fc>(twoFc + 2, exp, minusK))
					goto SmallDivisorCaseLabel_;
			}

			// The ceiling is inside, so we are done
			res.exponent = minusK + Kappa + 1;
			TrailingZeroPolicy::template OnTrailingZeros<Impl>(res);
			return res;

		SmallDivisorCaseLabel_:

			res.significand *= 10;
			res.significand -= SmallDivisionByPow10<Kappa>(r);
			res.exponent = minusK + Kappa;
			TrailingZeroPolicy::template NoTrailingZeros<Impl>(res);
			return res;
		}

		template <FloatingPoint F>
		template <typename Ret, typename TrailingZeroPolicy, typename CachePolicy>
		auto Impl<F>::ComputeRightClosedDirected(Carrier twoFc, i32 exp, bool shorterInterval) noexcept -> Ret
		{
			// Step 1: Schubfach multiplier calculation

			Ret res;

			const i32 minusK = Log::FloorLog10Pow2(exp - shorterInterval) - Kappa;
			const CacheEntryType cache = CachePolicy::template GetCache<F>(-minusK);
			const i32 betaMinus1 = exp + Log::FloorLog2Pow10(-minusK);

			// Compute xi and deltai
			// 10^kappa <= deltai < 10^(kappa + 1)
			const u32 deltaI = shorterInterval ? ComputeDelta(cache, betaMinus1 - 1) : ComputeDelta(cache, betaMinus1);
			Carrier zi = ComputeMul(twoFc << betaMinus1, cache);

			// Step 2 : Try larger divisor; remove trailing zeros if necessary
			constexpr u32 BigDivisor = Math::Pow(10u, Kappa + 1);

			// Using an upper bound on xi, we might be able to optimize the division
			// better than the compiler; we are computing xi / bigDivisor here
			res.significand = DivideByPow10<Kappa + 1, Math::Consts::SignificandBits<F> +Kappa + 2, Kappa + 1>(zi);
			u32 r = u32(zi - BigDivisor * res.significand);

			if (r > deltaI)
			{
				goto SmallDivisorCaseLabel_;
			}
			else if (r == deltaI)
			{
				// Compare the fractional parts
				if (!ComputeMulParity(twoFc - 1 - shorterInterval, cache, betaMinus1))
					goto SmallDivisorCaseLabel_;
			}

			// The ceiling is inside, so we are done
			res.exponent = minusK + Kappa + 1;
			TrailingZeroPolicy::template OnTrailingZeros<Impl>(res);
			return res;

		SmallDivisorCaseLabel_:

			res.significand *= 10;
			res.significand += SmallDivisionByPow10<Kappa>(r);
			res.exponent = minusK + Kappa;
			TrailingZeroPolicy::template NoTrailingZeros<Impl>(res);
			return res;
		}

		template <FloatingPoint F>
		auto Impl<F>::RemoveTrailingZeros(Carrier& n) noexcept -> i32
		{
			constexpr i32 maxPower = []
			{
				constexpr Carrier MaxPossibleSignificand = Math::Consts::MaxVal<Carrier> / Math::Pow(10u, Kappa + 1);

				i32 k = 0;
				Carrier p = 1;
				while (p < MaxPossibleSignificand / 10)
				{
					p *= 10;
					++k;
				}
				return k;
			}();

			if constexpr (IsF32<F>)
			{
				// Perform binary search
				i32 s = 0;

#if !COMPILER_MSVC
				if (n % 10'000 == 0)
				{
					n /= 10'000;
					s |= 0x4;
				}
				if (n % 100 == 0)
				{
					n /= 100;
					s |= 0x2;
				}
				if (n % 10 == 0)
				{
					n /= 10;
					s |= 0x1;
				}
#else
				// Up to the currently tested version of MSVC (1926), it seems that MSVC is
				// not aware of the Granlund-Montgomery style divisibility test. Thus we
				// implement it ourselves here.

				constexpr const auto& divtable = DivisibilityCheckTable<Carrier, 5, Math::Consts::Digits10<F>>;
				u32 quotient;

				// is n divisible by 10^4
				quotient = Intrin::RotateR(n * divtable.table[4].modInv, 4);
				if (quotient <= (divtable.table[4].maxQuotient >> 4))
				{
					n = quotient;
					s |= 0x4;
				}

				// is n divisible by 10^2
				quotient = Intrin::RotateR(n * divtable.table[2].modInv, 2);
				if (quotient <= (divtable.table[2].maxQuotient >> 2))
				{
					n = quotient;
					s |= 0x2;
				}

				// is n divisible by 10^1
				quotient = Intrin::RotateR(n * divtable.table[1].modInv, 1);
				if (quotient <= (divtable.table[1].maxQuotient >> 1))
				{
					n = quotient;
					s |= 0x1;
				}
#endif

				return s;
			}
			else
			{
				STATIC_ASSERT(maxPower == 16, "DragonBox: Invalid Kappa");

				// Divide by 10^8 and reduce to 32-bits.
				// Since ret_value.significand <= (2^64 - 1) / 1000 < 10^17, both of the
				// quotient and the r should fit in 32-bits.

				constexpr const auto& divtable32 = DivisibilityCheckTable<u32, 5, 9>;

				// If the number is divisible by 100'000'000, work with the quotient
				u32 quotientByPow10_8 = u32(DivideByPow10<8, 54, 0>(n));
				u32 remainder = u32(n - 100'000'000 * quotientByPow10_8);

				if (remainder == 0)
				{
					u32 n32 = quotientByPow10_8;

#if !COMPILER_MSVC
					// Is n divisible by 10^8?
					// This branch is extremely unlikely.
					// Dragonbox' creator suspects it is impossible to get into this branch.
					if (n32 % 100'000'000 == 0) {
						n = n32 / 100'000'000;
						return 16;
					}

					// Otherwise perform binary search
					i32 s = 8;

					if (n32 % 10'000 == 0)
					{
						n32 /= 10'000;
						s |= 0x4;
					}
					if (n32 % 100 == 0)
					{
						n32 /= 100;
						s |= 0x2;
					}
					if (n32 % 10 == 0)
					{
						n32 /= 10;
						s |= 0x1;
					}
#else
					// Up to the currently tested version of MSVC (1926), it seems that MSVC is
					// not aware of the Granlund-Montgomery style divisibility test. Thus we
					// implement it ourselves here.

					u32 quotient;

					// Is n divisible by 10^8?
					// This branch is extremely unlikely.
					// Dragonbox' creator suspects it is impossible to get into this branch.
					quotient = Intrin::RotateR(n32 * divtable32.table[8].modInv, 8);
					if (quotient <= (divtable32.table[8].maxQuotient >> 8))
					{
						n32 = quotient;
						return 16;
					}

					// Otherwise perform binary search
					i32 s = 8;

					// is n divisible by 10^4
					quotient = Intrin::RotateR(n32 * divtable32.table[4].modInv, 4);
					if (quotient <= (divtable32.table[4].maxQuotient >> 4))
					{
						n32 = quotient;
						s |= 0x4;
					}

					// is n divisible by 10^2
					quotient = Intrin::RotateR(n32 * divtable32.table[2].modInv, 2);
					if (quotient <= (divtable32.table[2].maxQuotient >> 2))
					{
						n32 = quotient;
						s |= 0x2;
					}

					// is n divisible by 10^1
					quotient = Intrin::RotateR(n32 * divtable32.table[1].modInv, 1);
					if (quotient <= (divtable32.table[1].maxQuotient >> 1))
					{
						n32 = quotient;
						s |= 0x1;
					}
#endif

					n = n32;
					return s;
				}

				// Perform binary search
				u32 multiplier = 100'000'000;
				i32 s = 0;

#if !COMPILER_MSVC
				if (n % 10'000 == 0)
				{
					n /= 10'000;
					multiplier = 10'000;
					s |= 0x4;
				}
				if (n % 100 == 0)
				{
					n /= 100;
					multiplier = (multiplier >> 2) * divtable32.table[2].modInv;
					s |= 0x2;
				}
				if (n % 10 == 0)
				{
					n /= 10;
					multiplier = (multiplier >> 1) * divtable32.table[1].modInv;
					s |= 0x1;
				}
#else
				// Up to the currently tested version of MSVC (1926), it seems that MSVC is
				// not aware of the Granlund-Montgomery style divisibility test. Thus we
				// implement it ourselves here.

				constexpr const auto& divtable = DivisibilityCheckTable<Carrier, 5, Math::Consts::Digits10<F>>;
				u32 quotient;

				// is n divisible by 10^4
				quotient = Intrin::RotateR(n * divtable.table[4].modInv, 4);
				if (quotient <= (divtable.table[4].maxQuotient >> 4))
				{
					n = quotient;
					multiplier = 10'000;
					s |= 0x4;
				}

				// is n divisible by 10^2
				quotient = Intrin::RotateR(n * divtable.table[2].modInv, 2);
				if (quotient <= (divtable.table[2].maxQuotient >> 2))
				{
					n = quotient;
					multiplier = (s == 4 ? 100 : 1'000'000);
					s |= 0x2;
				}

				// is n divisible by 10^1
				quotient = Intrin::RotateR(n * divtable.table[1].modInv, 1);
				if (quotient <= (divtable.table[1].maxQuotient >> 1))
				{
					n = quotient;
					multiplier = (multiplier >> 1) * divtable32.table[1].modInv;
					s |= 0x1;
				}
#endif

				return s;
			}
		}

		template <FloatingPoint F>
		constexpr auto Impl<F>::ComputeMul(Carrier u, const CacheEntryType& cache) noexcept -> Carrier
		{
			if constexpr (IsF32<F>)
				return Umul96Upper32(u, cache);
			else
				return Umul192Upper64(u, cache);
		}

		template <FloatingPoint F>
		constexpr auto Impl<F>::ComputeDelta(const CacheEntryType& cache, i32 betaMinus1) noexcept -> u32
		{
			if constexpr (IsF32<F>)
				return u32(cache >> (CacheBits - 1 - betaMinus1));
			else
				return u32(cache.high >> (CarrierBits - 1 - betaMinus1));
		}

		template <FloatingPoint F>
		constexpr auto Impl<F>::ComputeMulParity(Carrier twoF, const CacheEntryType& cache, i32 betaMinus1) noexcept -> bool
		{
			ASSERT(betaMinus1 >= 1, "DragonBox: beta needs to be equal or larger to 1");
			ASSERT(betaMinus1 < 64, "DragonBox: beta needs to be smaller than 64");

			if constexpr (IsF32<F>)
				return ((Umul96Lower64(twoF, cache) >> (64 - betaMinus1)) & 1) != 0;
			else
				return ((Umul192Middle64(twoF, cache) >> (64 - betaMinus1)) & 1) != 0;
		}

		template <FloatingPoint F>
		constexpr auto Impl<F>::ComputeLeftEndpointForShorterIntervalCase(const CacheEntryType& cache, i32 betaMinus1) noexcept -> Carrier
		{
			if constexpr (IsF32<F>)
				return Carrier((cache - (cache >> (Math::Consts::SignificandBits<F> + 2))) >> (CacheBits - Math::Consts::SignificandBits<F> - 1 - betaMinus1));
			else
				return (cache.high - (cache.high >> (Math::Consts::SignificandBits<F> + 2))) >> (CarrierBits - Math::Consts::SignificandBits<F> - 1 - betaMinus1);
		}

		template <FloatingPoint F>
		constexpr auto Impl<F>::ComputeRightEndpointForShorterIntervalCase(const CacheEntryType& cache, i32 betaMinus1) noexcept -> Carrier
		{
			if constexpr (IsF32<F>)
				return Carrier((cache + (cache << (Math::Consts::SignificandBits<F> + 1))) >> (CacheBits - Math::Consts::SignificandBits<F> - 1 - betaMinus1));
			else
				return (cache.high + (cache.high >> (Math::Consts::SignificandBits<F> + 1))) >> (CarrierBits - Math::Consts::SignificandBits<F> - 1 - betaMinus1);
		}

		template <FloatingPoint F>
		constexpr auto Impl<F>::ComputeRoundUpForShorterIntervalCase(const CacheEntryType& cache, i32 betaMinus1) noexcept -> Carrier
		{
			if constexpr (IsF32<F>)
				return (Carrier(cache >> (CacheBits - Math::Consts::SignificandBits<F> - 2 - betaMinus1)) + 1) / 2;
			else
				return ((cache.high >> (CarrierBits - Math::Consts::SignificandBits<F> - 2 - betaMinus1)) + 1) / 2;
		}

		template <FloatingPoint F>
		constexpr auto Impl<F>::IsRightEndpointIntergerShorterInterval(i32 exp) noexcept -> bool
		{
			return exp >= CaseShorterIntervalRightEndpointLowerThreshold &&
				   exp <= CaseShorterIntervalRightEndpointUpperThreshold;
		}

		template <FloatingPoint F>
		constexpr auto Impl<F>::IsLeftEndpointIntegerShorterInterval(i32 exp) noexcept -> bool
		{
			return exp >= CaseShorterIntervalLeftEndpointLowerThreshold &&
				   exp <= CaseShorterIntervalLeftEndpointUpperThreshold;
		}

		template <FloatingPoint F>
		template <typename Detail::Impl<F>::IntegerCheckCaseId CaseId>
		constexpr auto Impl<F>::IsProductInteger(Carrier twoF, i32 exp, i32 minusK) noexcept -> bool
		{
			// Case I: f = fc +- 1/2
			if constexpr (CaseId == IntegerCheckCaseId::FcPmHalf)
			{
				if (exp < CaseFcPmHalfLowerThreshold)
					return false;
				// For k >= 0
				if (exp <= CaseFcPmHalfUpperThreshold)
					return true;
				// For k < 0
				if (exp > DivisibilityCheckBy5Treshold)
					return false;
				return DivisibleByPowerOf5<MaxPowOfFactorOf5 + 1>(twoF, minusK);
			}
			// Case II: f = fc + 1
			// Case III: f = fc
			else
			{
				// Exponent for 5 is negative
				if (exp > DivisibilityCheckBy5Treshold)
					return false;
				if (exp > CaseFcUpperThreshold)
					return DivisibleByPowerOf5<MaxPowOfFactorOf5 + 1>(twoF, minusK);
				// Both exponents are nonnegative
				if (exp >= CaseFcLowerThreshold)
					return true;
				// Exponent for 2 is negative
				return DivisibleByPowerOf2(twoF, minusK - exp + 1);
			}
		}
	}

	namespace Policies::Detail
	{
		template <typename Base, typename DefaultPolicy>
		template <typename FoundPolicyInfo>
		constexpr auto BaseDefaultPair<Base, DefaultPolicy>::GetPolicyImpl(FoundPolicyInfo) noexcept -> FoundPolicyInfo
		{
			return {};
		}

		template <typename Base, typename DefaultPolicy>
		template <typename FoundPolicyInfo, typename FirstPolicy, typename ... Args>
		constexpr auto BaseDefaultPair<Base, DefaultPolicy>::GetPolicyImpl(FoundPolicyInfo, FirstPolicy, Args... args) noexcept
		{
			if constexpr (DerivesFrom<FirstPolicy, Base>)
			{
				if constexpr (FoundPolicyInfo::FoundInfo == PolicyFoundInfo::NotFound)
					return GetPolicyImpl(FoundPolicyPair<FirstPolicy, PolicyFoundInfo::Unique>{}, args...);
				else
					return GetPolicyImpl(FoundPolicyPair<FirstPolicy, PolicyFoundInfo::Repeated>{}, args...);
			}
			else
			{
				return GetPolicyImpl(FoundPolicyInfo{}, args...);
			}
		}

		template <typename Policy>
		constexpr auto CheckPolicyValidity(Policy, BaseDefaultPairList<>) noexcept -> bool
		{
			return false;
		}

		template <typename Policy, typename FirstBaseDefaultPolicy, typename ... RemainingBaseDefaultPairs>
		constexpr auto CheckPolicyValidity(Policy, BaseDefaultPairList<FirstBaseDefaultPolicy, RemainingBaseDefaultPairs...>) noexcept -> bool
		{
			return DerivesFrom<Policy, typename FirstBaseDefaultPolicy::BaseT> || CheckPolicyValidity(Policy{}, BaseDefaultPairList<RemainingBaseDefaultPairs...>{});
		}

		template <typename BaseDefaultPairList>
		constexpr auto CheckPolicyListValidity(BaseDefaultPairList) noexcept -> bool
		{
			return true;
		}

		template <typename BaseDefaultPairList, typename FistPolicy, typename ... RemainingPolicies>
		constexpr auto CheckPolicyListValidity(BaseDefaultPairList, FistPolicy, RemainingPolicies... remainingPolicies) noexcept -> bool
		{
			return CheckPolicyValidity(FistPolicy{}, BaseDefaultPairList{}) &&
				CheckPolicyListValidity(BaseDefaultPairList{}, remainingPolicies...);
		}

		template <bool Repeated, typename ... FoundPolicyPairs, typename ... Policies>
		constexpr auto MakePolicyHolderImpl(BaseDefaultPairList<>, FoundPolicyPairList<Repeated, FoundPolicyPairs...>, Policies...)
		{
			return FoundPolicyPairList<Repeated, FoundPolicyPairs...>{};
		}

		template <typename FirstBaseDefaultPair, typename... RemainingBaseDefaultPairs, bool Repeated, typename... FoundPolicyPairs, typename... Policies>
		constexpr auto MakePolicyHolderImpl(BaseDefaultPairList<FirstBaseDefaultPair, RemainingBaseDefaultPairs...>,
											FoundPolicyPairList<Repeated, FoundPolicyPairs...>, Policies... policies) noexcept
		{
			using NewFoundPolicyPair = decltype(FirstBaseDefaultPair::GetPolicy(policies...));
			return MakePolicyHolderImpl(BaseDefaultPairList<RemainingBaseDefaultPairs...>{},
										FoundPolicyPairList<Repeated || NewFoundPolicyPair::FoundInfo == PolicyFoundInfo::Repeated, NewFoundPolicyPair, FoundPolicyPairs...>{},
										policies...);
		}

		template <bool Repeated, typename ... RawPolicies>
		constexpr auto ConvertToPolicyHolder(FoundPolicyPairList<Repeated>, RawPolicies...)
		{
			return PolicyHolder<RawPolicies...>{};
		}

		template <bool Repeated, typename FirstFoundPolicyPair, typename ... RemaaingFoundPolicyPairs, typename ... RawPolicies>
		constexpr auto ConvertToPolicyHolder(FoundPolicyPairList<Repeated, FirstFoundPolicyPair, RemaaingFoundPolicyPairs...>, RawPolicies... policies)
		{
			return ConvertToPolicyHolder(FoundPolicyPairList<Repeated, RemaaingFoundPolicyPairs...>{}, typename FirstFoundPolicyPair::PolicyT{}, policies...);
		}

		template <typename BaseDefaultPairList, typename ... Policies>
		constexpr auto MakePolicyHolder(BaseDefaultPairList, Policies... policies)
		{
			STATIC_ASSERT(CheckPolicyListValidity(BaseDefaultPairList{}, Policies{}...), "DragonBox: An invalid policy is specified");
			using PolicyPairList = decltype(MakePolicyHolderImpl(BaseDefaultPairList{}, FoundPolicyPairList<false>{}, policies...));
			STATIC_ASSERT(!PolicyPairList::Repeated, "DragonBox: Each policy should be specified at most once");
			return ConvertToPolicyHolder(PolicyPairList{});
		}
	}

	template<FloatingPoint F, typename... Policies_>
	auto ToDecimal(UnsignedOfSameSize<F> signedSignificandBits, u32 expBits, Policies_... policies) noexcept -> DecimalFP<FloatUtils::Carrier<F>>
	{
		// Build policy holder type
		using namespace Policies::Detail;
		using PolicyHolder = decltype(MakePolicyHolder(
			BaseDefaultPairList<BaseDefaultPair<Sign::Base, Sign::ReturnSign>,
				                BaseDefaultPair<TrailingZeros::Base, TrailingZeros::Remove>,
				                BaseDefaultPair<DecimalToBinaryRounding::Base, DecimalToBinaryRounding::NearestToEven>,
				                BaseDefaultPair<BinaryToDecimalRounding::Base, BinaryToDecimalRounding::ToEven>,
				                BaseDefaultPair<Cache::Base, Cache::Full>>{},
			policies...
		));

		using Carrier = FloatUtils::Carrier<F>;
		using ReturnType = DecimalFP<Carrier>;

		ReturnType res = PolicyHolder::Delegate(
			signedSignificandBits,
			[signedSignificandBits, expBits]<typename Interval>(Interval intervalProvider)
			{
				constexpr auto tag = Interval::Tag;

				Carrier twoFc = FloatUtils::RemoveSignAndShift(signedSignificandBits);
				i32 exponent = i32(expBits);

				if constexpr (tag == DecimalToBinaryRounding::TagT::ToNearest)
				{
					// Is the input a normal number?
					if (exponent != 0)
					{
						exponent += Math::Consts::ExpBias<F> - Math::Consts::SignificandBits<F>;

						// Shorter interval case; proceed like Schubfach.
						// One might think this condition is wrong, since when exponent_bits == 1
						// and two_fc == 0, the interval is actually regular. However, it turns out
						// that this seemingly wrong condition is actually fine, because the end
						// result is anyway the same.
						//
						// [binary32]
						// (fc-1/2) * 2^e = 1.175'494'28... * 10^-38
						// (fc-1/4) * 2^e = 1.175'494'31... * 10^-38
						//    fc    * 2^e = 1.175'494'35... * 10^-38
						// (fc+1/2) * 2^e = 1.175'494'42... * 10^-38
						//
						// Hence, shorter_interval_case will return 1.175'494'4 * 10^-38.
						// 1.175'494'3 * 10^-38 is also a correct shortest representation that will
						// be rejected if we assume shorter interval, but 1.175'494'4 * 10^-38 is
						// closer to the true value so it doesn't matter.
						//
						// [binary64]
						// (fc-1/2) * 2^e = 2.225'073'858'507'201'13... * 10^-308
						// (fc-1/4) * 2^e = 2.225'073'858'507'201'25... * 10^-308
						//    fc    * 2^e = 2.225'073'858'507'201'38... * 10^-308
						// (fc+1/2) * 2^e = 2.225'073'858'507'201'63... * 10^-308
						//
						// Hence, shorter_interval_case will return 2.225'073'858'507'201'4 *
						// 10^-308. This is indeed of the shortest length, and it is the unique one
						// closest to the true value among valid representations of the same length.

						if (twoFc == 0)
						{
							return Interval::InvokeShorterIntervalCase(signedSignificandBits,
																	   [exponent]<typename... Args>(Args... args)
							{
								return Detail::Impl<F>::template ComputeNearestShorter<ReturnType,
									Interval::ShorterIntervalType,
									PolicyHolder::TrailingZeroPolicy,
									PolicyHolder::BinaryToDecimalRoundingPolicy,
									PolicyHolder::CachePolicy>(exponent, args...);
							});
						}

						twoFc |= Carrier(1) << (Math::Consts::SignificandBits<F> + 1);
					}
					// Is the input a subnormal number?
					else
					{
						exponent = Math::Consts::MinExp<F> - Math::Consts::SignificandBits<F>;
					}

					return Interval::InvokeNormalIntervalCase(signedSignificandBits,
															  [twoFc, exponent]<typename... Args>(Args... args)
					{
						return Detail::Impl<F>::template ComputeNearestNormal<ReturnType,
							Interval::NormalIntervalType,
							PolicyHolder::TrailingZeroPolicy,
							PolicyHolder::BinaryToDecimalRoundingPolicy,
							PolicyHolder::CachePolicy>(twoFc, exponent, args...);
					});
				}
				else if constexpr (tag == DecimalToBinaryRounding::TagT::LeftClosedDirected)
				{
					// Is the input a normal number?
					if (exponent != 0)
					{
						exponent += Math::Consts::ExpBias<F> - Math::Consts::SignificandBits<F>;
						twoFc |= Carrier(1) << (Math::Consts::SignificandBits<F> + 1);
					}
					// Is the input a subnormal number?
					else
					{
						exponent = Math::Consts::ExpBias<F> - Math::Consts::SignificandBits<F>;
					}

					return Detail::Impl<F>::template ComputeLeftClosedDirected<ReturnType, PolicyHolder::TrailingZeroPolicy, PolicyHolder::CachePolicy>(twoFc, exponent);
				}
				else
				{
					ASSERT(tag == DecimalToBinaryRounding::TagT::RightClosedDirected, "DragonBox, Invalid DecimalToBinaryRounding tag");

					bool shorterInterval = false;
					// Is the input a normal number?
					if (exponent != 0)
					{
						shorterInterval = twoFc == 0 && exponent != -1;
						exponent += Math::Consts::ExpBias<F> -Math::Consts::SignificandBits<F>;
						twoFc |= Carrier(1) << (Math::Consts::SignificandBits<F> + 1);
					}
					// Is the input a subnormal number?
					else
					{
						exponent = Math::Consts::ExpBias<F> - Math::Consts::SignificandBits<F>;
					}
					return Detail::Impl<F>::template ComputeRightClosedDirected<ReturnType, PolicyHolder::TrailingZeroPolicy, PolicyHolder::CachePolicy>(twoFc, exponent, shorterInterval);
				}
			}
		);

		PolicyHolder::HandleSign(signedSignificandBits, res);
		return res;
	}

	template<FloatingPoint F, typename... Policies>
	auto ToDecimal(F f, Policies... policies) noexcept -> DecimalFP<FloatUtils::Carrier<F>>
	{
		const SignedOfSameSize<F> br = Math::FloatBitsToInt(f);
		const u32 exp = FloatUtils::ExtractExpBits(br);
		const SignedOfSameSize<F> s = FloatUtils::RemoveExpBits(br, exp);
		ASSERT(FloatUtils::IsFinite(exp), "DragonBox: f should be finite");

		return ToDecimal<F>(s, exp, policies...);
	}
	
}
