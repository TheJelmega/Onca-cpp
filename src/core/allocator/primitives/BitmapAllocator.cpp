#include "BitmapAllocator.h"

#include "core/memory/MemUtils.h"

namespace Core::Alloc
{
	BitmapAllocator::BitmapAllocator(IAllocator* pBackingAlloc, usize blockSize, usize numBlocks) noexcept
		: m_mem(pBackingAlloc->Allocate<u8>(CalcReqMemSize(blockSize, numBlocks), u16(blockSize > 0xFFFF ? 0xFFFF : blockSize), true))
		, m_blockSize(blockSize)
		, m_numBlocks(numBlocks)
	{
		ASSERT(IsPowOf2(blockSize), "Blocksize needs to be a power of 2");
		
		m_numManagementBlocks = m_mem.Size() / blockSize - numBlocks;
		MemClear(m_mem.Ptr(), m_numManagementBlocks * blockSize);
	}

	BitmapAllocator::~BitmapAllocator() noexcept
	{
		m_mem.Dealloc();
	}

	auto BitmapAllocator::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		ASSERT(align < m_blockSize, "Alignment cannot be greater than blocksize");
		Threading::Lock lock{m_mutex};
		
		const u8* pManagmentInfo = m_mem.Ptr();
		const usize blocksNeeded = (size + m_blockSize - 1) / m_blockSize;
		
		for (usize i = 0; i < m_numBlocks; ++i)
		{
			u8 startBit = u8(i & 0x07);
			usize byteIdx = i / 8;
			usize numBlocks = blocksNeeded;

			while (numBlocks > 0)
			{
				const u8 curBlockShift = u8(numBlocks >= 8 ? 0 : 8 - numBlocks);
				u8 searchMask = u8(0xFF << curBlockShift) >> startBit;

				if (pManagmentInfo[byteIdx] & searchMask)
					break;

				++byteIdx;

				const u8 blocksWritten = 8 - startBit;
				numBlocks = blocksWritten >= numBlocks ? 0 : numBlocks - blocksWritten;
				startBit = 0;
			}

			if (numBlocks == 0)
			{
				MarkBits(i, blocksNeeded, true);

#if ENABLE_ALLOC_STATS
				const usize overhead = blocksNeeded * m_blockSize - size;
				m_stats.RemoveAlloc(size, overhead, isBacking);
#endif		
				return { i, this, Log2(align), size, isBacking };
			}
		}

		return MemRef<u8>{ nullptr };
	}

	auto BitmapAllocator::DeallocateRaw(MemRef<u8>&& mem) noexcept -> void
	{
		Threading::Lock lock{ m_mutex };
		
		const usize startIdx = mem.GetRawHandle();
		const usize size = mem.Size();
		const usize numBlocks = (size + m_blockSize - 1) / m_blockSize;
		MarkBits(startIdx, numBlocks, false);

#if ENABLE_ALLOC_STATS
		const usize overhead = numBlocks * m_blockSize - size;
		m_stats.RemoveAlloc(size, overhead, mem.IsBackingMem());
#endif
	}

	auto BitmapAllocator::TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8*
	{
		return m_mem.Ptr() + (m_numManagementBlocks + mem.GetRawHandle()) * m_blockSize;
	}

	auto BitmapAllocator::CalcReqMemSize(usize blockSize, usize numBlocks) noexcept -> usize
	{
		const usize numManagmentBytes = numBlocks / 8;
		const usize numManagmentBlocks = (numManagmentBytes + blockSize - 1) / blockSize;
		return blockSize * (numBlocks + numManagmentBlocks);
	}

	auto BitmapAllocator::MarkBits(usize startIdx, usize numBlocks, bool set) noexcept -> void
	{
		u8* pManagmentInfo = m_mem.Ptr();

		const u8 firstByteOffset = startIdx & 0x07;
		if (firstByteOffset)
		{
			const u8 curBlockShift = u8(numBlocks >= 8 ? 0 : 8 - numBlocks);
			const u8 blocksWritten = 8 - firstByteOffset;
			numBlocks = blocksWritten >= numBlocks ? 0 : numBlocks - blocksWritten;

			u8 mask = u8(0xFF) << curBlockShift;
			mask >>= firstByteOffset;

			const usize byteIdx = startIdx / 8;
			u8 curVal = pManagmentInfo[byteIdx];
			pManagmentInfo[byteIdx] = set ? curVal | mask : curVal & ~mask;
			startIdx += 8 - firstByteOffset;
		}

		for (usize i = startIdx, endIdx = startIdx + numBlocks; i < endIdx; i += 8)
		{
			u8 curBlockShift = u8(numBlocks >= 8 ? 0 : 8 - numBlocks);
			numBlocks -= 8;
			u8 mask = u8(0xFF) << curBlockShift;

			const usize byteIdx = i / 8;
			u8 curVal = pManagmentInfo[byteIdx];
			pManagmentInfo[byteIdx] = set ? curVal | mask : curVal & ~mask;
		}
	}
}
