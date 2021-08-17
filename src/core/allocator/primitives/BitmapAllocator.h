#pragma once
#include "core/allocator/IAllocator.h"
#include "core/threading/Sync.h"

namespace Core::Alloc
{
	/**
	 * \brief An allocator that uses a bitmap to keep track of allocations
	 */
	class CORE_API BitmapAllocator final : public IAllocator
	{
	public:
		/**
		 * Create a bitmap allocator
		 * \param[in] pBackingAlloc Allocator to create the underlying memory block
		 * \param[in] blockSize Size of managed block
		 * \param[in] numBlocks Number of blocks
		 */
		BitmapAllocator(IAllocator* pBackingAlloc, usize blockSize, usize numBlocks) noexcept;
		BitmapAllocator(BitmapAllocator&&) = default;
		~BitmapAllocator() noexcept override;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		auto DeallocateRaw(MemRef<u8>&& mem) noexcept -> void override;
		auto TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8* override;

	private:
		/**
		 * Calculate the required size for a number of block with a specific size, with additional space for the bitmap
		 * \param[in] blockSize Size of a block
		 * \param[in] numBlocks Number of blocks
		 * \return Size required to store blocks and the bitmap
		 */
		auto CalcReqMemSize(usize blockSize, usize numBlocks) noexcept -> usize;

		/**
		 * Mark bits to show the block has been used
		 * \param[in] startIdx Index of first bit to mark
		 * \param[in] numBlocks Number of blocks to mark
		 * \param[in] set Whether to mark the bits as set or not
		 */
		auto MarkBits(usize startIdx, usize numBlocks, bool set) noexcept -> void;
	
		MemRef<u8>       m_mem;                 ///< Managed memory
		usize            m_blockSize;           ///< Size of a block
		usize            m_numBlocks;           ///< Number of blocks
		usize            m_numManagementBlocks; ///< number of block used to manage bitmap
		Threading::Mutex m_mutex;               ///< Mutex to guard bitmap modifications
	};
}
