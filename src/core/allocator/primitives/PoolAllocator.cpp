#include "PoolAllocator.h"

namespace Core::Alloc
{
	PoolAllocator::PoolAllocator(IAllocator* pBackingAlloc, u16 blockAlign, usize blockSize, usize numBlocks) noexcept
		: m_mem(nullptr)
		, m_head(0)
		, m_blockSize(blockSize)
	{
		ASSERT(IsPowOf2(blockAlign), "Block alignment needs to be a power of 2");
		ASSERT(blockSize > sizeof(usize), "Block size needs to be larger than the size of usize");
		ASSERT(numBlocks != 0, "Needs to have at least 1 block");

		const usize mask = blockAlign - 1;
		const usize diff = blockSize & mask;
		const usize blockPadding = diff == 0 ? diff : blockAlign - diff;
		const usize totalBlockSize = blockSize + blockPadding;
		const usize memSize = totalBlockSize * numBlocks;

		m_mem = pBackingAlloc->Allocate<u8>(memSize, blockAlign, true);

		// Setup free blocks
		usize* pBegin = reinterpret_cast<usize*>(m_mem.Ptr());

		for (usize i = 0; i < memSize;)
		{
			usize next = i + m_blockSize;
			*(pBegin + i) = next;
			i = next;
		}

#if ENABLE_ALLOC_STATS
		m_blockPadding = u16(blockPadding);
#endif
	}

	PoolAllocator::PoolAllocator(PoolAllocator&& other) noexcept
		: m_mem(StdMove(other.m_mem))
		, m_head(StdMove(other.m_head))
		, m_blockSize(other.m_blockSize)
#if ENABLE_ALLOC_STATS
		, m_blockPadding(other.m_blockPadding)
#endif
	{
	}

	PoolAllocator::~PoolAllocator() noexcept
	{
		m_mem.Dealloc();
	}

	auto PoolAllocator::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		ASSERT(align <= m_blockSize, "Cannot have a greater aligment than the blocksize");
		ASSERT(size <= m_blockSize, "Cannot allocate more than the blocksize");

		u8* pMem = m_mem.Ptr();
		usize handle, next;
		do
		{
			handle = m_head;

			// In case we run out of space when updating the head
			if (handle >= m_mem.Size()) UNLIKELY
				return MemRef<u8>{ nullptr };

			next = *(pMem + handle);
		}
		while (!m_head.CompareExchangeWeak(handle, next));

#if ENABLE_ALLOC_STATS
		const usize overhead = m_blockSize - size;
		m_stats.AddAlloc(size, overhead, isBacking);
#endif
		
		return { handle, this, Log2(align), size, isBacking };
	}

	auto PoolAllocator::DeallocateRaw(MemRef<u8>&& mem) noexcept -> void
	{
		const usize handle = mem.GetRawHandle();
		usize* pBlock = reinterpret_cast<usize*>(m_mem.Ptr() + handle);
		usize cur;
		do
		{
			cur = m_head;
			*pBlock = cur;
		}
		while (!m_head.CompareExchangeWeak(cur, handle));

#if ENABLE_ALLOC_STATS
		const usize size = mem.Size();
		const usize overhead = m_blockSize - size + m_blockPadding;
		m_stats.RemoveAlloc(size, overhead, mem.IsBackingMem());
#endif
	}

	auto PoolAllocator::TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8*
	{
		return m_mem.Ptr() + mem.GetRawHandle();
	}
}
