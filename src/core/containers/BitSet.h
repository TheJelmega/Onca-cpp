#pragma once
#include "core/MinInclude.h"
#include "allocator/IAllocator.h"
#include "allocator/GlobalAlloc.h"

namespace Core
{
	/**
	 * Dynamically sized bitset
	 */
	class BitSet
	{
	public:
		/**
		 * Create a BitSet
		 * \param[in] alloc Allocator
		 */
		explicit BitSet(Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a BitSet
		 * \param[in] numBits Number of bits
		 * \param[in] alloc Allocator
		 */
		BitSet(usize numBits, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a BitSet with the content of another BitSet
		 * \param[in] other Other bitset
		 */
		BitSet(const BitSet& other) noexcept;
		/**
		 * Create a BitSet with the content of another BitSet, but with a different allocator
		 * \param[in] other Other bitset
		 */
		BitSet(const BitSet& other, Alloc::IAllocator& alloc) noexcept;
		/**
		 * Move a BitSet into a new BitSet
		 * \param[in] other Other bitset
		 */
		BitSet(BitSet&& other) noexcept;
		/**
		 * Move a BitSet into a new BitSet, but with a different allocator
		 * \param[in] other Other bitset
		 */
		BitSet(BitSet&& other, Alloc::IAllocator& alloc) noexcept;

		auto operator~() const noexcept -> BitSet;

		auto operator|(const BitSet& other) const noexcept -> BitSet;
		auto operator^(const BitSet& other) const noexcept -> BitSet;
		auto operator&(const BitSet& other) const noexcept -> BitSet;

		auto operator=(const BitSet& other) noexcept -> BitSet&;
		auto operator=(BitSet&& other) noexcept -> BitSet&;

		auto operator|=(const BitSet& other) noexcept -> BitSet&;
		auto operator^=(const BitSet& other) noexcept -> BitSet&;
		auto operator&=(const BitSet& other) noexcept -> BitSet&;

		auto operator[](usize idx) const noexcept -> bool;

		auto operator==(const BitSet& other) const noexcept -> bool;

		/**
		 * Check if the common bits are the same
		 * \param[in] other Other bitset
		 * \return Whether the bitsets match
		 */
		auto Match(const BitSet& other) const noexcept -> bool;

		/**
		 * Set a bit to a certain value
		 * \param[in] idx Index of the bit to set
		 * \param[in] val Value
		 */
		auto Set(usize idx, bool val) noexcept -> void;
		/**
		 * Set a bit to true
		 * \param[in] idx Index of the bit to set
		 */
		auto Set(usize idx) noexcept -> void;
		/**
		 * Set a bit to false
		 * \param[in] idx Index of the bit to set
		 */
		auto Unset(usize idx) noexcept -> void;
		/**
		 * Clear all bits
		 */
		auto Clear() noexcept -> void;

		/**
		 * Flip all bits in the bitset
		 */
		auto Flip() noexcept -> void;

		/**
		 * Count the numbers of bits that are set
		 * \return Number of bits that are set
		 */
		auto Count() const noexcept -> usize;
		/**
		 * Check if not bits are set
		 * \return Whether no bits are set
		 */
		auto None() const noexcept -> bool;
		/**
		 * Check if any bit is set
		 * \return Whether any bit is set
		 */
		auto Any() const noexcept -> bool;
		/**
		 * Check if all bits are set
		 * \return Whether all bits are set
		 */
		auto All() const noexcept -> bool;

		/**
		 * Resize the bitset to a new number of bits
		 * \param[in] numBits Number of bits
		 */
		auto Resize(usize numBits) noexcept -> void;

		/**
		 * Get the number of bits in the BitSet
		 * \return Number of bits
		 */
		auto NumBits() const noexcept -> usize;
		/**
		 * Get a pointer to the DynArray's data
		 * \return Pointer to the DynArray's data
		 */
		auto Data() noexcept -> usize*;
		/**
		 * Get a pointer to the DynArray's data
		 * \return Pointer to the DynArray's data
		 */
		auto Data() const noexcept -> const usize*;
		/**
		 * Get the size of the data (in bytes)
		 * \return Size of the data
		 */
		auto DataSize() const noexcept -> usize;

		/**
		 * Get the allocator used by the DynArray
		 * \return Allocator used by the DynArray
		 */
		auto GetAllocator() const noexcept -> Alloc::IAllocator*;

	private:
		static constexpr usize BitsPerElem = sizeof(usize);
		static constexpr usize BitIdxMask = BitsPerElem - 1;

		DynArray<usize> m_data;    ///< Data
		usize           m_numBits; ///< Number of bits
	};
}

#include "BitSet.inl"