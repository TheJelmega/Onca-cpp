#pragma once
#include "core/MinInclude.h"

#ifdef __INTELLISENSE__
#pragma diag_suppress 438 // supress bogus "expected a ''" due to concepts
#endif

namespace Core::Math
{
	/**
	 * \brief Get the minimum value
	 * \tparam T0 Value type
	 * \tparam T1 Value type
	 * \param[in] a First value
	 * \param[in] b Second value
	 * \return Minimum value
	 */
	template<typename T0, LessComparable<T0> T1>
		requires ConstructableFrom<T0, T1>
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
		requires ConstructableFrom<T0, T1>
	constexpr auto Max(T0 a, T1 b) noexcept -> T0;

	// TODO, return type based on fp type
	template<FloatingPoint T>
	constexpr auto Ceil(T t) noexcept -> u64;

	constexpr auto IsPowOf2(u64 val) noexcept -> bool;
	constexpr auto Log2(u64 val) noexcept -> u8;
	constexpr auto Log2(u32 val) noexcept -> u8;
	constexpr auto Log2(u16 val) noexcept -> u8;
	constexpr auto Log2(u8 val) noexcept -> u8;
}

#include "MathUtils.inl"