#pragma once
#if __RESHARPER__
#include "BitmapAllocator.h"
#endif

#include "core/memory/MemUtils.h"

namespace Onca::Alloc
{
	template<usize BlockSize, usize NumBlocks>
	BitmapAllocator<BlockSize, NumBlocks>::BitmapAllocator(IAllocator* pBackingAlloc) noexcept
		: IMemBackedAllocator(pBackingAlloc->Allocate<u8>(CalcReqMemSize(), u16(BlockSize > 0x8000 ? 0x8000 : BlockSize), true))
	{
		STATIC_ASSERT(Math::IsPowOf2(BlockSize), "Blocksize needs to be a power of 2");
		MemClear(m_mem.Ptr(), NumManagementBlocks * BlockSize);
	}
	
	template<usize BlockSize, usize NumBlocks>
	auto BitmapAllocator<BlockSize, NumBlocks>::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		ASSERT(align <= BlockSize, "Alignment cannot be greater than blocksize");
		ASSERT(Math::IsPowOf2(align), "Alignment needs to be a power of 2");

		const usize blocksNeeded = (size + BlockSize - 1) / BlockSize;

		Threading::Lock lock{ m_mutex };
		
		const u8* pManagementInfo = m_mem.Ptr();
		
		for (usize i = 0; i < NumBlocks; ++i)
		{
			u8 startBit = u8(i & 0x07);
			usize byteIdx = i / 8;
			usize numBlocks = blocksNeeded;

			// Up to first 8 blocks
			const u8 firstBlockShift = u8(numBlocks >= 8 ? 0 : 8 - numBlocks);
			u8 searchMask = u8(0xFF << firstBlockShift) >> startBit;
			if (pManagementInfo[byteIdx] & searchMask)
				continue;;

			++byteIdx;

			u8 blocksWritten = 8 - startBit;
			numBlocks = blocksWritten >= numBlocks ? 0 : numBlocks - blocksWritten;

			// If we need more blocks
			while (numBlocks > 0)
			{
				const u8 curBlockShift = u8(numBlocks >= 8 ? 0 : 8 - numBlocks);
				searchMask = u8(0xFF << curBlockShift);
				if (pManagementInfo[byteIdx] & searchMask)
					break;

				++byteIdx;
				numBlocks = numBlocks <= 8 ? 0 : numBlocks - 8;
			}

			if (numBlocks == 0)
			{
				MarkBits(i, blocksNeeded, true);

#if ENABLE_ALLOC_STATS
				const usize overhead = blocksNeeded * BlockSize - size;
				m_stats.AddAlloc(size, overhead, isBacking);
#endif
				u8* ptr = m_mem.Ptr() + (i + NumManagementBlocks) * BlockSize;
				return { ptr, this, Math::Log2(align), size, isBacking };
			}
		}

		return nullptr;
	}

	template<usize BlockSize, usize NumBlocks>
	void BitmapAllocator<BlockSize, NumBlocks>::DeallocateRaw(MemRef<u8>&& mem) noexcept
	{
		// Threadsafe as memory pointers and total allocator memory size is consistent over threads
		const usize startIdx = (mem.Ptr() - m_mem.Ptr()) / BlockSize - NumManagementBlocks;
		const usize size = mem.Size();
		const usize numBlocks = (size + BlockSize - 1) / BlockSize;

		Threading::Lock lock{ m_mutex };
		MarkBits(startIdx, numBlocks, false);

#if ENABLE_ALLOC_STATS
		const usize overhead = numBlocks * BlockSize - size;
		m_stats.RemoveAlloc(size, overhead, mem.IsBackingMem());
#endif
	}
	
	template<usize BlockSize, usize NumBlocks>
	void BitmapAllocator<BlockSize, NumBlocks>::MarkBits(usize startIdx, usize numBlocks, bool set) noexcept
	{
		u8* pManagementInfo = m_mem.Ptr();

		const u8 firstByteOffset = startIdx & 0x07;
		if (firstByteOffset)
		{
			const u8 curBlockShift = u8(numBlocks >= 8 ? 0 : 8 - numBlocks);
			const u8 blocksWritten = 8 - firstByteOffset;
			numBlocks = blocksWritten >= numBlocks ? 0 : numBlocks - blocksWritten;

			u8 mask = u8(0xFF) << curBlockShift;
			mask >>= firstByteOffset;

			const usize byteIdx = startIdx / 8;
			u8 curVal = pManagementInfo[byteIdx];
			pManagementInfo[byteIdx] = set ? curVal | mask : curVal & ~mask;
			startIdx += 8 - firstByteOffset;
		}

		for (usize i = startIdx, endIdx = startIdx + numBlocks; i < endIdx; i += 8)
		{
			u8 curBlockShift = u8(numBlocks >= 8 ? 0 : 8 - numBlocks);
			numBlocks -= 8;
			u8 mask = u8(0xFF) << curBlockShift;

			const usize byteIdx = i / 8;
			u8 curVal = pManagementInfo[byteIdx];
			pManagementInfo[byteIdx] = set ? curVal | mask : curVal & ~mask;
		}
	}
}
