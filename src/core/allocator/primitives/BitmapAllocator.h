#pragma once
#include "core/allocator/IAllocator.h"
#include "core/threading/Sync.h"

namespace Core::Alloc
{
	/**
	 * \brief An allocator that uses a bitmap to keep track of allocations
	 *
	 * A bitmap allocator manages a chunk of memory as a blocks, similar to a pool allocator, but a single allocation can consist of multiple blocks,
	 * The blocks that are used by allocations are managed by a bitmap.
	 *
	 * Each block's size is required to be a power of 2, as the size of the blocks also defines the maximum alignment of an allocation.
	 *
	 *     alloc 1  alloc 2  free     alloc 3  alloc 3  free     free
	 *     v        v        v        v        v        v        v
	 * +--------+--------+--------+--------+--------+--------+--------+
	 * |4269746D|61702041|????????|6C6C6F63|61746F72|????????|????????|
	 * +--------+--------+--------+--------+--------+--------+--------+
	 *
	 * \tparam BlockSize Size of a block
	 * \tparam NumBlocks Number of blocks
	 */
	template<usize BlockSize, usize NumBlocks>
	class BitmapAllocator final : public IAllocator
	{
	public:
		/**
		 * Create a bitmap allocator
		 * \param[in] pBackingAlloc Allocator to create the underlying memory block
		 */
		BitmapAllocator(IAllocator* pBackingAlloc) noexcept;
		BitmapAllocator(BitmapAllocator&&) = default;
		~BitmapAllocator() noexcept override;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		auto DeallocateRaw(MemRef<u8>&& mem) noexcept -> void override;

	private:
		static constexpr auto CalcNumManagementBlocks() noexcept -> usize
		{
			const usize numManagmentBytes = (NumBlocks + 7) / 8;
			return (numManagmentBytes + BlockSize - 1) / BlockSize;
		}
		/**
		 * Calculate the required size for a number of block with a specific size, with additional space for the bitmap
		 * \return Size required to store blocks and the bitmap
		 */
		static constexpr auto CalcReqMemSize() noexcept -> usize
		{
			return BlockSize * (NumBlocks + CalcNumManagementBlocks());
		}

		/**
		 * Mark bits to show the block has been used
		 * \param[in] startIdx Index of first bit to mark
		 * \param[in] numBlocks Number of blocks to mark
		 * \param[in] set Whether to mark the bits as set or not
		 */
		auto MarkBits(usize startIdx, usize numBlocks, bool set) noexcept -> void;

		static constexpr usize NumManagementBlocks = CalcNumManagementBlocks();
	
		MemRef<u8>       m_mem;                 ///< Managed memory
		Threading::Mutex m_mutex;               ///< Mutex to guard bitmap modifications
	};
}

#include "BitmapAllocator.inl"