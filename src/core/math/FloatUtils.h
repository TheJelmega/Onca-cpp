#pragma once
#include "core/MinInclude.h"
#include "Concepts.h"
#include "Constants.h"

namespace Onca::FloatUtils
{
	template<FloatingPoint F>
	using Carrier = UnsignedOfSameSize<F>;
	template<typename T>
		requires FloatingPoint<T> || Integral<T>
	constexpr i32 CarrierBits = sizeof(T) * 8;
	template<Integral I>
	constexpr i32 ExponentBias = 1 - (1 << (CarrierBits<I> - Math::Consts::SignificandBits<I> -2));

	// TODO: put somewhere in FloatingPointUtils (Math???)
	/**
	 * Extract the exponent bits from a floating point representation
	 * \tparam I Integer type
	 * \param[in] bits Bits representing a floating point
	 * \return Exponent bits in lower bits of the result
	 */
	template<Integral I>
	constexpr auto ExtractExpBits(I bits) noexcept -> u32;
	/**
	 * Extract the significand bits from a floating point representation
	 * \tparam I Integral type
	 * \param[in] bits Bits representing a floating point
	 * \return Significand bits in lower bits
	 */
	template<Integral I>
	constexpr auto ExtractSignificandBits(I bits) noexcept -> I;
	/**
	 * Remove exponent bits from a floating point representation
	 * \tparam I Integral type
	 * \param[in] bits Bits representing a floating point
	 * \param[in] exp Exponent bits
	 * \return Bits with exponent removed
	 */
	template<Integral I>
	constexpr auto RemoveExpBits(I bits, i32 exp) noexcept -> I;
	/**
	 * Remove the sign bit
	 * \tparam I Integral type
	 * \param[in] bits Bits representing a floating point
	 * \return Result bits
	 */
	template<Integral I>
	constexpr auto RemoveSign(I bits) noexcept -> I;
	/**
	 * Remove the sign bit and move 1 bit to the left
	 * \tparam I Integral type
	 * \param[in] bits Bits representing a floating point
	 * \return Result bits
	 */
	template<Integral I>
	constexpr auto RemoveSignAndShift(I bits) noexcept -> I;
	/**
	 * Convert an extracted exponent to the actual exponent
	 * \tparam I Integral type
	 * \param[in] exp Exponent
	 * \return Actual exponent
	 */
	template<Integral I>
	constexpr auto BinaryExponent(u32 exp) noexcept -> u32;
	/**
	 * Convert an extracted significand to the actual significand
	 * \tparam I Integral type
	 * \param[in] significand Significand
	 * \param[in] exp Exponent
	 * \return Actual significand
	 */
	template<Integral I>
	constexpr auto BinarySignificand(I significand, u32 exp) noexcept -> I;
	/**
	 * Check if the floating point representation is not zero
	 * \tparam I Integral type
	 * \param[in] val Floating point representation
	 * \return Whether the floating point representation is not zero
	 */
	template<Integral I>
	constexpr auto IsNonZero(I val) noexcept -> bool;
	/**
	 * Check if the floating point representation is positive
	 * \tparam I Integral type
	 * \param[in] val Floating point representation
	 * \return Whether the floating point representation is positive
	 */
	template<Integral I>
	constexpr auto IsPositive(I val) noexcept -> bool;
	/**
	 * Check if the floating point representation is negative
	 * \tparam I Integral type
	 * \param[in] val Floating point representation
	 * \return Whether the floating point representation is negative
	 */
	template<Integral I>
	constexpr auto IsNegative(I val) noexcept -> bool;
	/**
	 * Check if the floating point representation is finite
	 * \tparam I Integral type
	 * \param[in] exp Exponent
	 * \return Whether the floating point representation is finite
	 */
	template<Integral I>
	constexpr auto IsFinite(I exp) noexcept -> bool;
	/**
	 * Check if all of the floating point representation's significand bits are zero
	 * \tparam I Integral type
	 * \param[in] val Floating point representation
	 * \return Whether all of the floating point representation's significand bits are zero
	 */
	template<Integral I>
	constexpr auto HasAllZeroSignificandBits(I val) noexcept -> bool;
	/**
	 * Check if all of the floating point representation's significand is even
	 * \tparam I Integral type
	 * \param[in] val Floating point representation
	 * \return Whether all of the floating point representation's significand is even
	 */
	template<Integral I>
	constexpr auto HasEvenSignificandBits(I val) noexcept -> bool;
	
}

#include "FloatUtils.inl"