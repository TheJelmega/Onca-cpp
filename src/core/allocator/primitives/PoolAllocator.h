#pragma once
#include "core/allocator/IAllocator.h"
#include "core/Atomic.h"

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
	 * \note This allocator needs no defragmentation as all blocks can always be used
	 */
	class CORE_API PoolAllocator final : public IAllocator
	{
	public:
		/**
		 * \brief Create a pool allocator
		 * \param[in] pBackingAlloc Allocator to create the underlying memory block
		 * \param[in] blockAlign Alignment of the blocks
		 * \param[in] blockSize Size of the block
		 * \param[in] numBlocks Number of blocks
		 */
		explicit PoolAllocator(IAllocator* pBackingAlloc, u16 blockAlign, usize blockSize, usize numBlocks) noexcept;
		PoolAllocator(PoolAllocator&& other) noexcept; // = default; is deleted
		~PoolAllocator() noexcept override;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		auto DeallocateRaw(MemRef<u8>&& mem) noexcept -> void override;
		auto TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8* override;

	private:
		MemRef<u8>     m_mem;          ///< Managed memory
		Atomic<usize>  m_head;         ///< Offset to first empty block
		const usize    m_blockSize;    ///< Size of a block
#if ENABLE_ALLOC_STATS
		u16            m_blockPadding; ///< Padding per block
#endif
	};
}
