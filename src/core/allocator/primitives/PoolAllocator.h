#pragma once
#include "core/allocator/IAllocator.h"
#include "core/utils/Atomic.h"

namespace Core::Alloc
{
	/**
	 * \brief An allocator that allocates memory from fixed-size block in the allocator
	 *
	 * A pool allocator manages a chunk of memory as a pool of blocks, which can be given out or returned at any moment.
	 * Each block's size is required to be a power of 2, as the size of the blocks also defines the maximum alignment of an allocation.
	 * Each allocation is limited to the size of a single block, this help avoid the need for defragmentation and avoids additional overhead to manage the allocations.
	 *
	 *     used     used     free     used     used     free     free
	 *     v        v        v        v        v        v        v
	 * +--------+--------+--------+--------+--------+--------+--------+
	 * |506F6F6C|29416C6C|????????|6F636174|6F720000|????????|????????|
	 * +--------+--------+--------+--------+--------+--------+--------+
	 *
	 * \tparam BlockSize Size of the blocks
	 * \tparam NumBlocks Number of blocks
	 *
	 * \note This allocator needs no defragmentation as all blocks can always be used
	 */
	template<usize BlockSize, usize NumBlocks>
	class PoolAllocator final : public IAllocator
	{
	public:
		/**
		 * \brief Create a pool allocator
		 * \param[in] pBackingAlloc Allocator to create the underlying memory block
		 */
		explicit PoolAllocator(IAllocator* pBackingAlloc) noexcept;
		PoolAllocator(PoolAllocator&& other) noexcept; // = default; is deleted
		~PoolAllocator() noexcept override;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		void DeallocateRaw(MemRef<u8>&& mem) noexcept override;
		auto OwnsInternal(const MemRef<u8>& mem) noexcept -> bool override;

	private:
		MemRef<u8>   m_mem;          ///< Managed memory
		Atomic<u8*>  m_head;         ///< Offset to first empty block
	};
}

#include "PoolAllocator.inl"