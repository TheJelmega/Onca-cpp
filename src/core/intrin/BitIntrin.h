#pragma once
#include "Base.h"
#include "Concepts.h"

namespace Core::Intrin
{

	/**
	 * Find the index of the first bit that is set to 1 in a 
	 * \tparam T Integral type
	 * \param t Value to find bit in
	 * \return Index of the first bit that is set to 1, 255 if no bit set to 1 was found
	 */
	template<Integral T>
	constexpr auto BitScanLSB(T t) noexcept -> u8;
	/**
	 * Find the index of the first bit that is set to 0 in a
	 * \tparam T Integral type
	 * \param t Value to find bit in
	 * \return Index of the first bit that is set to 0, 255 if no bit set to 0 was found
	 */
	template<Integral T>
	constexpr auto BitScanLSB0(T t) noexcept -> u8;

	/**
	 * Count the number of trailing 0's
	 * \tparam T Integral type
	 * \param t Value to find bit in
	 * \return Number of trailing 0's
	 */
	template<Integral T>
	constexpr auto ZeroCountLSB(T t) noexcept -> u8;
	/**
	 * Count the number of trailing 1's
	 * \tparam T Integral type
	 * \param t Value to find bit in
	 * \return Number of trailing 1's
	 */
	template<Integral T>
	constexpr auto OneCountLSB(T t) noexcept -> u8;

	/**
	 * Find the index of the last bit that is set to 1 in a
	 * \tparam T Integral type
	 * \param t Value to find bit in
	 * \return Index of the last bit that is set to 1, 255 if no bit set to 1 was found
	 */
	template<Integral T>
	constexpr auto BitScanMSB(T t) noexcept -> u8;
	/**
	 * Find the index of the last bit that is set to 0 in a
	 * \tparam T Integral type
	 * \param t Value to find bit in
	 * \return Index of the last bit that is set to 0, 255 if no bit set to 0 was found
	 */
	template<Integral T>
	constexpr auto BitScanMSB0(T t) noexcept -> u8;

	/**
	 * Count the number of trailing 0's
	 * \tparam T Integral type
	 * \param t Value to find bit in
	 * \return Number of trailing 0's
	 */
	template<Integral T>
	constexpr auto ZeroCountMSB(T t) noexcept -> u8;
	/**
	 * Count the number of trailing 1's
	 * \tparam T Integral type
	 * \param t Value to find bit in
	 * \return Number of trailing 1's
	 */
	template<Integral T>
	constexpr auto OneCountMSB(T t) noexcept -> u8;

	/**
	 * Count the number of bits that are set to 1
	 * \tparam T Integral type
	 * \param t value to count
	 * \return Number of bits that are set to 1
	 */
	template<Integral T>
	constexpr auto PopCnt(T t) noexcept -> u8;
	/**
	 * Count the number of bits that are set to 0
	 * \tparam T Integral type
	 * \param t value to count
	 * \return Number of bits that are set to 0
	 */
	template<Integral T>
	constexpr auto PopCnt0(T t) noexcept -> u8;

	/**
	 * Rotate a value to the right by a given number of bits
	 * \tparam T Integral type
	 * \param[in] t Value to rotate
	 * \param[in] bits Bits ot rotate
	 * \return Rotated value
	 */
	template<Integral T>
	constexpr auto RotateR(T t, u8 bits) noexcept -> T;
	/**
	 * Rotate a value to the right by a given number of bits
	 * \tparam T Integral type
	 * \param[in] t Value to rotate
	 * \param[in] bits Bits ot rotate
	 * \return Rotated value
	 */
	template<Integral T>
	constexpr auto RotateL(T t, u8 bits) noexcept -> T;

}

#include "BitIntrin.inl"