#pragma once
#include "core/MinInclude.h"
#include "Array.h"

namespace Core
{
	/**
	 * Dynamically sized bitset
	 */
	template<usize Bits>
	class InplaceBitSet
	{
	public:
		/**
		 * Create a InplaceBitSet
		 */
		constexpr InplaceBitSet() noexcept;
		/**
		 * Create a InplaceBitSet with the content of another InplaceBitSet
		 * \param[in] other Other bitset
		 */
		constexpr InplaceBitSet(const InplaceBitSet& other) noexcept;
		/**
		 * Move a InplaceBitSet into a new InplaceBitSet
		 * \param[in] other Other bitset
		 */
		constexpr InplaceBitSet(InplaceBitSet&& other) noexcept;

		constexpr auto operator~() const noexcept -> InplaceBitSet;

		constexpr auto operator|(const InplaceBitSet& other) const noexcept -> InplaceBitSet;
		constexpr auto operator^(const InplaceBitSet& other) const noexcept -> InplaceBitSet;
		constexpr auto operator&(const InplaceBitSet& other) const noexcept -> InplaceBitSet;

		constexpr auto operator=(const InplaceBitSet& other) noexcept -> InplaceBitSet&;
		constexpr auto operator=(InplaceBitSet&& other) noexcept -> InplaceBitSet&;

		constexpr auto operator|=(const InplaceBitSet& other) noexcept -> InplaceBitSet&;
		constexpr auto operator^=(const InplaceBitSet& other) noexcept -> InplaceBitSet&;
		constexpr auto operator&=(const InplaceBitSet& other) noexcept -> InplaceBitSet&;

		constexpr auto operator[](usize idx) const noexcept -> bool;

		constexpr auto operator==(const InplaceBitSet& other) const noexcept -> bool;

		/**
		 * Check if the common bits are the same
		 * \param[in] other Other bitset
		 * \return Whether the bitsets match
		 */
		constexpr auto Match(const InplaceBitSet& other) const noexcept -> bool;

		/**
		 * Set a bit to a certain value
		 * \param[in] idx Index of the bit to set
		 * \param[in] val Value
		 */
		constexpr auto Set(usize idx, bool val) noexcept -> void;
		/**
		 * Set a bit to true
		 * \param[in] idx Index of the bit to set
		 */
		constexpr auto Set(usize idx) noexcept -> void;
		/**
		 * Set a bit to false
		 * \param[in] idx Index of the bit to set
		 */
		constexpr auto Unset(usize idx) noexcept -> void;
		/**
		 * Clear all bits
		 */
		constexpr auto Clear() noexcept -> void;

		/**
		 * Flip all bits in the bitset
		 */
		constexpr auto Flip() noexcept -> void;

		/**
		 * Count the numbers of bits that are set
		 * \return Number of bits that are set
		 */
		constexpr auto Count() const noexcept -> usize;
		/**
		 * Check if not bits are set
		 * \return Whether no bits are set
		 */
		constexpr auto None() const noexcept -> bool;
		/**
		 * Check if any bit is set
		 * \return Whether any bit is set
		 */
		constexpr auto Any() const noexcept -> bool;
		/**
		 * Check if all bits are set
		 * \return Whether all bits are set
		 */
		constexpr auto All() const noexcept -> bool;

		/**
		 * Get the number of bits in the InplaceBitSet
		 * \return Number of bits
		 */
		constexpr auto NumBits() const noexcept -> usize;
		/**
		 * Get a pointer to the DynArray's data
		 * \return Pointer to the DynArray's data
		 */
		constexpr auto Data() noexcept -> usize*;
		/**
		 * Get a pointer to the DynArray's data
		 * \return Pointer to the DynArray's data
		 */
		constexpr auto Data() const noexcept -> const usize*;
		/**
		 * Get the size of the data (in bytes)
		 * \return Size of the data
		 */
		constexpr auto DataSize() const noexcept -> usize;

	private:
		static constexpr usize BitsPerElem = sizeof(usize) * 8;
		static constexpr usize BitIdxMask = BitsPerElem - 1;
		static constexpr usize NumElems = (Bits + BitIdxMask) / BitsPerElem;

		Array<usize, NumElems> m_data;    ///< Data
	};

	
}

#include "InplaceBitSet.inl"