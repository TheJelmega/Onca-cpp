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
	constexpr auto BitScan(T t) noexcept -> u8;
	/**
	 * Find the index of the first bit that is set to 0 in a
	 * \tparam T Integral type
	 * \param t Value to find bit in
	 * \return Index of the first bit that is set to 0, 255 if no bit set to 0 was found
	 */
	template<Integral T>
	constexpr auto BitScan0(T t) noexcept -> u8;

	/**
	 * Find the index of the last bit that is set to 1 in a
	 * \tparam T Integral type
	 * \param t Value to find bit in
	 * \return Index of the last bit that is set to 1, 255 if no bit set to 1 was found
	 */
	template<Integral T>
	constexpr auto BitScanR(T t) noexcept -> u8;
	/**
	 * Find the index of the last bit that is set to 0 in a
	 * \tparam T Integral type
	 * \param t Value to find bit in
	 * \return Index of the last bit that is set to 0, 255 if no bit set to 0 was found
	 */
	template<Integral T>
	constexpr auto BitScanR0(T t) noexcept -> u8;

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

}

#include "BitIntrin.inl"