#pragma once
#include "core/MinInclude.h"
#include "Concepts.h"
#include "Constants.h"
#include "core/utils/Pair.h"

#ifdef __INTELLISENSE__
#pragma diag_suppress 438 // supress bogus "expected a ''" due to concepts
#endif

namespace Core::Math
{
	template<FloatingPoint F>
	using FloatIntType = Conditional<SameAs<F, f32>, i32, i64>;
	template<FloatingPoint F>
	using FloatUIntType = Conditional<SameAs<F, f32>, u32, u64>;
	template<SignedIntegral I>
	using IntFloatType = Conditional<SameAs<I, i32>, f32, f64>;
	template<UnsignedIntegral U>
	using UIntFloatType = Conditional<SameAs<U, u32>, f32, f64>;

	/**
	 * \brief Get the minimum value
	 * \tparam T0 Value type
	 * \tparam T1 Value type
	 * \param[in] a First value
	 * \param[in] b Second value
	 * \return Minimum value
	 */
	template<typename T0, LessComparable<T0> T1>
		requires ConvertableFrom<T0, T1>
	constexpr auto Min(T0 a, T1 b) noexcept -> T0;
	/**
	 * \brief Get the maximum value
	 * \tparam T0 Value type
	 * \tparam T1 Value type
	 * \param[in] a First value
	 * \param[in] b Second value
	 * \return Maximum value
	 */
	template<typename T0, LessComparable<T0> T1>
		requires ConvertableFrom<T0, T1>
	constexpr auto Max(T0 a, T1 b) noexcept -> T0;

	/**
	 * Clamp a value between a min and max
	 * \tparam T Type to clamp
	 * \tparam A Type of min value
	 * \tparam B Type of max value
	 * \param val Value to clamp
	 * \param min Min value
	 * \param max Max value
	 * \return Clamped value
	 */
	template<Numeric T, LessComparable<T> A, GreaterComparable<T> B>
		requires ConvertableFrom<T, A> && ConvertableFrom<T, B>
	constexpr auto Clamp(T val, A min, B max) noexcept -> T;

	/**
	 * Linearly interpolate between 2 values
	 * \tparam T Type
	 * \tparam U Interpolant type
	 * \param a First value
	 * \param b Second value
	 * \param i Interpolant
	 * \return Interpolated value
	 * \note The interpolant is not clamped
	 */
	template<Numeric T, Numeric U>
		requires Multiplyable<T, U>
	constexpr auto Lerp(T a, T b, U i) noexcept -> T;
	/**
	 * Smooth hermite interpolation between 0 and 1, if x is in range [min, max]
	 * \tparam T Numeric type
	 * \param min Minimum value
	 * \param max Maximum value
	 * \param i Interpolant
	 * \return Interpolated value in the range [0, 1]
	 */
	template<Numeric T>
	constexpr auto SmoothStep(T min, T max, T i) noexcept -> T;
	/**
	 * Return 0 if i < edge, 1 otherwise
	 * \tparam T Numeric type
	 * \param edge Edge
	 * \param i Interpolant
	 * \return 0 if i < edge, 1 otherwise
	 */
	template<Numeric T>
	constexpr auto Step(T edge, T i) noexcept -> T;

	/**
	 * Get the absolute value of a value
	 * \tparam T Floating point type
	 * \param t Value
	 * \return Absolute value of the value
	 */
	template<Numeric T>
	constexpr auto Abs(T t) noexcept -> T;
	/**
	 * Get a value equal to the nearest integer that is greater or equal to x
	 * \tparam F Floating point type
	 * \param f Value
	 * \return Ceiling of the value
	 */
	template<FloatingPoint F>
	constexpr auto Ceil(F f) noexcept -> F;
	/**
	 * Get a value equal to the nearest integer that is less or equal to x
	 * \tparam F Floating point type
	 * \param f Value
	 * \return Floor of the value
	 */
	template<FloatingPoint F>
	constexpr auto Floor(F f) noexcept -> F;
	/**
	 * Get a value equal to the nearest integer to x whose absolute value is not larger than the absolute value of f
	 * \tparam F Floating point type
	 * \param f Value
	 * \return Floor of the value
	 */
	template<FloatingPoint F>
	constexpr auto Trunc(F f) noexcept -> F;
	/**
	 * Round a value
	 * \tparam F Floating point type
	 * \param f Value
	 * \return Rounded value
	 */
	template<FloatingPoint F>
	constexpr auto Round(F f) noexcept -> F;
	/**
	 * Round a value, if the value ends on .5, round towards the nearest even integer value
	 * \tparam F Floating point type
	 * \param f Value
	 * \return Rounded value
	 */
	template<FloatingPoint F>
	constexpr auto RoundEven(F f) noexcept -> F;
	/**
	 * Get the sign of a value
	 * \tparam T Numeric type
	 * \param t Value
	 * \return Sign of the value
	 */
	template<Numeric T>
	constexpr auto Sign(T t) noexcept -> i8;
	/**
	 * Get the fractional part of a float
	 * \tparam F Floating point type
	 * \param t Value
	 * \return Fractional part
	 */
	template<FloatingPoint F>
	constexpr auto Fract(F t) noexcept -> F;
	/**
	 * Get the remainder of division
	 * \tparam T Numeric type
	 * \param a Dividend
	 * \param b Divisor
	 * \return Remainder
	 */
	template<Numeric T>
	constexpr auto Mod(T a, T b) noexcept -> T;
	/**
	 * Split a float into its integral component and fraction
	 * \tparam F Floating point type
	 * \param f Value
	 * \return Pair with its integral component and fraction
	 */
	template<FloatingPoint F>
	constexpr auto ModF(F f) noexcept -> Pair<F, F>;

	/**
	 * Calculate the reciprocal (1/x) of a value
	 * \tparam T Numeric type
	 * \param t Value
	 * \return Reciprocal of the value
	 */
	template<Numeric T>
	constexpr auto Rcp(T t) noexcept -> T;
	/**
	 * Calculate the square root of a value
	 * \tparam T Numeric type
	 * \param t Value
	 * \return Square root of the value
	 */
	template<Numeric T>
	constexpr auto Sqrt(T t) noexcept -> T;
	/**
	 * Calculate the reciprocal square root of a value
	 * \tparam T Numeric type
	 * \param t Value
	 * \return Reciprocal square root of the value
	 */
	template<Numeric T>
	constexpr auto RSqrt(T t) noexcept -> T;

	/**
	 * Get an integer log2 of a value
	 * \param val Value
	 * \return Integer log2 of the value
	 */
	constexpr auto Log2(u64 val) noexcept -> u8;
	/**
	 * Get an integer log2 of a value
	 * \param val Value
	 * \return Integer log2 of the value
	 */
	constexpr auto Log2(u32 val) noexcept -> u8;
	/**
	 * Get an integer log2 of a value
	 * \param val Value
	 * \return Integer log2 of the value
	 */
	constexpr auto Log2(u16 val) noexcept -> u8;
	/**
	 * Get an integer log2 of a value
	 * \param val Value
	 * \return Integer log2 of the value
	 */
	constexpr auto Log2(u8 val) noexcept -> u8;

	/**
	 * Check if a value is a power of 2
	 * \param val Value
	 * \return Whether the value is a power of 2
	 */
	constexpr auto IsPowOf2(u64 val) noexcept -> bool;
	/**
	 * Check if a value is infinity
	 * \tparam F Floating point type
	 * \param f Value
	 * \return Whether the value is infinity
	 */
	template<FloatingPoint F>
	constexpr auto IsInf(F f) noexcept -> bool;
	/**
	 * Check if a value is NaN
	 * \tparam F Floating point type
	 * \param f Value
	 * \return Whether the value is NaN
	 */
	template<FloatingPoint F>
	constexpr auto IsNaN(F f) noexcept -> bool;

	/**
	 * Compare 2 values with an epsilon
	 * \tparam T Numeric type
	 * \param a First value
	 * \param b Second value
	 * \param e Epsilon
	 * \return Whether the floating points are equal
	 */
	template<Numeric T>
	constexpr auto EpsilonCompare(T a, T b, T e = Consts::Epsilon<T>) noexcept -> bool;

	/**
	 * Get the signed integer representation of a float
	 * \tparam F Floating point Type
	 * \param f Fp value
	 * \return Signed integer representation
	 */
	template<FloatingPoint F>
	constexpr auto FloatBitsToInt(F f) noexcept -> FloatIntType<F>;
	/**
	 * Get the unsigned integer representation of a float
	 * \tparam F Floating point Type
	 * \param f Fp value
	 * \return Unsigned integer representation
	 */
	template<FloatingPoint F>
	constexpr auto FloatBitsToUInt(F f) noexcept -> FloatUIntType<F>;
	
	/**
	 * Get a floating point from it signed integer representation
	 * \tparam I Signed integer type
	 * \param i Signed integer representation
	 * \return Floating point
	 */
	template<SignedIntegral I>
	constexpr auto IntBitsToFloat(I i) noexcept -> IntFloatType<I>;
	/**
	 * Get a floating point from it unsigned integer representation
	 * \tparam I Signed integer type
	 * \param i Unsigned integer representation
	 * \return Floating point
	 */
	template<UnsignedIntegral I>
	constexpr auto UIntBitsToFloat(I i) noexcept -> UIntFloatType<I>;

	////////////////////////////////////////////////////////////////
	// TODO: functions below are not constexpr yet
	////////////////////////////////////////////////////////////////

	/**
	 * Decompose a floating point into its normalized mantissa [0.5, 1.0) and exponent
	 * \tparam F Floating point type
	 * \param f Value
	 * \return Pair with mantissa and exponent
	 * \note The float is decomposed so that f == mantissa * 2^exp
	 */
	template<FloatingPoint F>
	auto Frexp(F f) noexcept -> Pair<F, i32>;
	/**
	 * Construct a float from a normalized mantissa [0.5, 1.0) and an exponent
	 * \tparam F Floating point type
	 * \param mantissa Normalized mantissa
	 * \param exponent Exponent
	 * \return Constructed float (mantissa * 2^exponent)
	 */
	template<FloatingPoint F>
	auto Ldexp(F mantissa, i32 exponent) noexcept -> F;
}

#include "MathUtils.inl"