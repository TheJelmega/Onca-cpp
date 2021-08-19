#pragma once
#include "core/allocator/IAllocator.h"
#include "core/Atomic.h"

namespace Core::Alloc
{
	/**
	 * An allocator that allocates memory from fixed-size block in the allocator
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
