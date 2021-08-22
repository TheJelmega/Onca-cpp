#pragma once
#include "core/MinInclude.h"

#ifdef __INTELLISENSE__
#pragma diag_suppress 438 // supress bpgus "expected a ''" due to concepts
#endif

namespace Core
{
	/**
	 * \brief Get the minimum value
	 * \tparam T Value type
	 * \param[in] a First value
	 * \param[in] b Second value
	 * \return Minimum value
	 */
	template<LessComparable T>
	constexpr auto Min(T a, T b) noexcept -> T;

	/**
	 * \brief Get the maximum value
	 * \tparam T Value type
	 * \param[in] a First value
	 * \param[in] b Second value
	 * \return Maximum value
	 */
	template<GreaterComparable T>
	constexpr auto Max(T a, T b) noexcept -> T;

	constexpr auto IsPowOf2(u64 val) noexcept -> bool;
	constexpr auto Log2(u64 val) noexcept -> u8;
	constexpr auto Log2(u32 val) noexcept -> u8;
	constexpr auto Log2(u16 val) noexcept -> u8;
	constexpr auto Log2(u8 val) noexcept -> u8;
}

#include "MathUtils.inl"