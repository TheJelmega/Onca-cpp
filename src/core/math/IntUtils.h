#pragma once
#include "core/MinInclude.h"

// Currently only has implementations needed for DragonBox
namespace Core
{
	// TODO: Signed 128-bit int

	/**
	 * Unsigned 128-bit integer
	 */
	struct U128
	{
		u64 low;
		u64 high;

		/**
		 * Create a 128-bit integer initialized to 0
		 */
		constexpr U128() noexcept;
		/**
		 * Create a 128-bit integer from low and high bits
		 * \param[in] low Low bits
		 * \param[in] high High bits
		 */
		constexpr U128(u64 low, u64 high);

		constexpr auto operator+=(u64 val) noexcept -> U128&;

	};

	/**
	 * Calculate the 128-bit product of 2 64-bit integers
	 * \param[in] x First value
	 * \param[in] y Second value
	 * \return Product
	 */
	constexpr auto Umul128(u64 x, u64 y) noexcept -> U128;
	/**
	 * Calculate the upper 64 bits of the 128-bit product
	 * \param[in] x First value
	 * \param[in] y Second value
	 * \return Upper 64 bits of the 128-bit product
	 */
	constexpr auto Umul128Upper64(u64 x, u64 y) noexcept -> u64;
	
	/**
	 * Calculate the upper 64 bits of the 192-bit product
	 * \param[in] x First value
	 * \param[in] y Second value
	 * \return Upper 64 bits of the 192-bit product
	 */
	constexpr auto Umul192Upper64(u64 x, U128 y) noexcept -> u64;
	/**
	 * Calculate the upper 32 bits of the 96-bit product
	 * \param[in] x First value
	 * \param[in] y Second value
	 * \return Upper 32 bits of the 96-bit product
	 */
	constexpr auto Umul96Upper32(u32 x, u64 y) noexcept -> u32;

	/**
	 * Calculate the middle 64 bits of the 96-bit product
	 * \param[in] x First value
	 * \param[in] y Second value
	 * \return Middle 64 bits of the 96-bit product
	 */
	constexpr auto Umul192Middle64(u64 x, U128 y) noexcept -> u64;

	/**
	 * Calculate the lower 64 bits of the 96-bit product
	 * \param[in] x First value
	 * \param[in] y Second value
	 * \return Lower 64 bits of the 96-bit product
	 */
	constexpr auto Umul96Lower64(u32 x, u64 y) noexcept -> u64;

}

#include "IntUtils.inl"
