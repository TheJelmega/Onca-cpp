#pragma once
#if __RESHARPER__
#include "FloatUtils.h"
#endif

namespace Core::FloatUtils
{
	template <Integral I>
	constexpr auto ExtractExpBits(I bits) noexcept -> u32
	{
		constexpr i32 expBits = Math::Consts::ExpDigits<FloatOfSameSize<I>>;
		constexpr u32 expBitMask = (1u << expBits) - 1;
		return (bits >> Math::Consts::SignificandBits<FloatOfSameSize<I>>) & expBitMask;
	}

	template <Integral I>
	constexpr auto ExtractSignificandBits(I bits) noexcept -> I
	{
		constexpr I mask = I((I(1) << Math::Consts::SignificandBits<FloatOfSameSize<I>>) - 1);
		return I(bits & mask);
	}

	template <Integral I>
	constexpr auto RemoveExpBits(I bits, i32 exp) noexcept -> I
	{
		return bits ^ (I(exp) << Math::Consts::SignificandBits<FloatOfSameSize<I>>);
	}

	template <Integral I>
	constexpr auto RemoveSign(I bits) noexcept -> I
	{
		return bits & ~(1ull << (sizeof(I) * 8 - 1));
	}

	template <Integral I>
	constexpr auto RemoveSignAndShift(I bits) noexcept -> I
	{
		return I(bits << 1);
	}

	template<Integral I>
	constexpr auto BinaryExponent(u32 exp) noexcept -> u32
	{
		return exp == 0 ? Math::Consts::MinExp<FloatOfSameSize<I>> : u32(exp + ExponentBias<FloatOfSameSize<I>>);
	}

	template <Integral I>
	constexpr auto BinarySignificand(I significand, u32 exp) noexcept -> I
	{
		return exp == 0 ? significand : significand | (I(1) << Math::Consts::SignificandBits<FloatOfSameSize<I>>);
	}

	template <Integral I>
	constexpr auto IsNonZero(I val) noexcept -> bool
	{
		return (val << 1) != 0;
	}

	template <Integral I>
	constexpr auto IsPositive(I val) noexcept -> bool
	{
		constexpr u64 signBit = I(1) << (Math::Consts::SignificandBits<FloatOfSameSize<I>> + Math::Consts::ExpDigits<FloatOfSameSize<I>>);
		return u64(val) < signBit;
	}

	template <Integral I>
	constexpr auto IsNegative(I val) noexcept -> bool
	{
		return !IsPositive(val);
	}

	template <Integral I>
	constexpr auto IsFinite(I exp) noexcept -> bool
	{
		constexpr I ExpBitsAllSet = (1u << Math::Consts::ExpDigits<FloatOfSameSize<I>>) - 1;
		return exp != ExpBitsAllSet;
	}

	template <Integral I>
	constexpr auto HasAllZeroSignificandBits(I val) noexcept -> bool
	{
		return (val << 1) == 0;
	}

	template <Integral I>
	constexpr auto HasEvenSignificandBits(I val) noexcept -> bool
	{
		return (val & 1) == 0;
	}
}
