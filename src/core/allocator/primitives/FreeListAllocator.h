#pragma once
#include "core/allocator/IAllocator.h"

namespace Core::Alloc
{
	/**
	 * \brief An allocator that keeps a list of free memory
	 *
	 * A freelist allocator keeps a list of free blocks of memory. This list is stored in the free memory itself.
	 * Whenever an allocation is done, the allocator looks for a free block in the list that fits the allocation (first-fit),
	 * when a free block that fits is found, it either gets fully used (if not enough space is available to put a new free block at the end),
	 * or part of the block is used, and the freelist is allocated.
	 * The freelist allocated a bit more space then required, to add a small header in front of the allocation to store additional info about the padding for that memory.
	 *
	 * \tparam Size Size of the managed memory
	 */
	template<usize Size>
	class FreeListAllocator final : public IAllocator
	{
	private:

		/**
		 * Header to manage free memory
		 */
		struct FreeHeader
		{
			usize size; ///< Size of free block
			usize next; ///< Offset to the next free block
		};

		/**
		 * Header to keep track of padding
		 */
		struct AllocHeader
		{
			u16 frontPadding; ///< Padding in the front of the allocation
			u16 backPadding;  ///< Padding at the back of the allocation
		};

	public:
		/**
		 * Create a freelist allocator
		 * \param pBackingAllocator Allocator to create the underlying memory block
		 */
		FreeListAllocator(IAllocator* pBackingAllocator) noexcept;
		FreeListAllocator(FreeListAllocator&&) = default;
		~FreeListAllocator() noexcept override;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		auto DeallocateRaw(MemRef<u8>&& mem) noexcept -> void override;
		auto TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8* override;

	private:

		/**
		 * Find space for the allocation (first-fit) and create space for the allcoation
		 * \param size Size of the allocation
		 * \param align Align of the allocation
		 * \param isBacking Whether the memory backs another allocator
		 * \return Offset to allocation
		 */
		auto AllocFirst(usize size, u16 align, bool isBacking) noexcept -> usize;
		/**
		 * Coalesce multiple blocks when possible
		 * \param pBegin Pointer to memory
		 * \param prevLoc Index of previous block
		 * \param curLoc Index of current block
		 * \param nextLoc Index of next block
		 */
		auto Coalesce(u8* pBegin, usize prevLoc, usize curLoc, usize nextLoc) noexcept -> void;

		MemRef<u8> m_mem;  ///< Managed memory
		usize      m_head; ///< Offset to free head
	};

}

#include "FreeListAllocator.inl"
