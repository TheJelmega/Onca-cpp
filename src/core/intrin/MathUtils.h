#pragma once
#include "core/MinInclude.h"
#include "Concepts.h"
#include "Pack.h"
#include "core/Math/Constants.h"
#include "core/utils/Pair.h"

namespace Onca::Math
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
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param[in] a First value
	 * \param[in] b Second value
	 * \return Minimum value
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto Min(const Intrin::Pack<T, Width>& a, const Intrin::Pack<T, Width>& b) noexcept -> Intrin::Pack<T, Width>;
	/**
	 * \brief Get the maximum value
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param[in] a First value
	 * \param[in] b Second value
	 * \return Maximum value
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto Max(const Intrin::Pack<T, Width>& a, const Intrin::Pack<T, Width>& b) noexcept -> Intrin::Pack<T, Width>;

	/**
	 * Clamp a value between a min and max
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param val Value to clamp
	 * \param min Min value
	 * \param max Max value
	 * \return Clamped value
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto Clamp(const Intrin::Pack<T, Width>& val, const Intrin::Pack<T, Width>& min, const Intrin::Pack<T, Width>& max) noexcept -> Intrin::Pack<T, Width>;

	/**
	 * Linearly interpolate between 2 values
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param a First value
	 * \param b Second value
	 * \param i Interpolant
	 * \return Interpolated value
	 * \note The interpolant is not clamped
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto Lerp(const Intrin::Pack<T, Width>& a, const Intrin::Pack<T, Width>& b, const Intrin::Pack<T, Width>& i) noexcept -> Intrin::Pack<T, Width>;
	/**
	 * Smooth hermite interpolation between 0 and 1, if x is in range [min, max]
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param min Minimum value
	 * \param max Maximum value
	 * \param i Interpolant
	 * \return Interpolated value in the range [0, 1]
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto SmoothStep(const Intrin::Pack<T, Width>& min, const Intrin::Pack<T, Width>& max, const Intrin::Pack<T, Width>& i) noexcept -> Intrin::Pack<T, Width>;
	/**
	 * Return 0 if i < edge, 1 otherwise
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param edge Edge
	 * \param i Interpolant
	 * \return 0 if i < edge, 1 otherwise
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto Step(const Intrin::Pack<T, Width>& edge, const Intrin::Pack<T, Width>& i) noexcept -> Intrin::Pack<T, Width>;

	/**
	 * Get the absolute value of a value
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param pack Value
	 * \return Absolute value of the value
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto Abs(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>;
	/**
	 * Get a value equal to the nearest integer that is greater or equal to x
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param f Value
	 * \return Ceiling of the value
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto Ceil(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>;
	/**
	 * Get a value equal to the nearest integer that is less or equal to x
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param pack Value
	 * \return Floor of the value
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto Floor(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>;
	/**
	 * Get a value equal to the nearest integer to x whose absolute value is not larger than the absolute value of f
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param pack Value
	 * \return Floor of the value
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto Trunc(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>;
	/**
	 * Round a value
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param pack Value
	 * \return Rounded value
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto Round(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>;
	/**
	 * Round a value, if the value ends on .5, round towards the nearest even value
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param f Value
	 * \return Rounded value
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto RoundEven(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>;
	/**
	 * Get the sign of a value
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param pack Value
	 * \return Sign of the value
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto Sign(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>;

	/**
	 * Get the fractal part of a float
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param pack Value
	 * \return Fractal part
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto Fract(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>;
	
	/**
	 * Get the remainder of division
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param a Dividend
	 * \param b Divisor
	 * \return Remainder
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto Mod(const Intrin::Pack<T, Width>& a, const Intrin::Pack<T, Width>& b) noexcept -> Intrin::Pack<T, Width>;
	/**
	 * Split a float into its integral component and fraction
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param pack Value
	 * \return Pair with its integral component and fraction
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto ModF(const Intrin::Pack<T, Width>& pack) noexcept -> Pair<Intrin::Pack<T, Width>, Intrin::Pack<T, Width>>;

	/**
	 * Calculate the reciprocal (1/x) of a value
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param pack Value
	 * \return Reciprocal of the value
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto Rcp(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>;
	/**
	 * Calculate the square root of a value
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param pack Value
	 * \return Square root of the value
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto Sqrt(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>;
	/**
	 * Calculate the reciprocal square root of a value
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param pack Value
	 * \return Reciprocal square root of the value
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto RSqrt(const Intrin::Pack<T, Width>& pack) noexcept -> Intrin::Pack<T, Width>;

	/**
	 * Compare 2 values with an epsilon
	 * \tparam T Pack element type
	 * \tparam Width Pack width
	 * \param a First value
	 * \param b Second value
	 * \param e Epsilon
	 * \return Whether the floating points are equal
	 */
	template<Intrin::SimdBaseType T, usize Width>
	constexpr auto EpsilonCompare(const Intrin::Pack<T, Width>& a, const Intrin::Pack<T, Width>& b, T e = Consts::MathEpsilon<T>) noexcept -> Intrin::Pack<T, Width>;
}

#include "MathUtils.inl"