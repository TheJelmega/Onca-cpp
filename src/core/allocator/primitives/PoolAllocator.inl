#pragma once
#include "PoolAllocator.h"

namespace Core::Alloc
{
	template<usize BlockSize, usize NumBlocks>
	PoolAllocator<BlockSize, NumBlocks>::PoolAllocator(IAllocator* pBackingAlloc) noexcept
		: m_mem(nullptr)
		, m_head(0)
	{
		STATIC_ASSERT(IsPowOf2(BlockSize), "Block size needs to be a power of 2");
		STATIC_ASSERT(BlockSize > sizeof(usize), "Block size needs to be larger than the size of usize");
		STATIC_ASSERT(NumBlocks != 0, "Needs to have at least 1 block");
		
		constexpr usize memSize = BlockSize * NumBlocks;
		m_mem = pBackingAlloc->Allocate<u8>(memSize, true);

		// Setup free blocks
		u8* pBegin = m_mem.Ptr();

		for (usize i = 0; i < memSize;)
		{
			usize next = i + BlockSize;
			*reinterpret_cast<usize*>(pBegin + i) = next;
			i = next;
		}
		*reinterpret_cast<usize*>(pBegin + memSize - BlockSize) = usize(-1);

#if ENABLE_ALLOC_STATS
		m_blockPadding = u16(blockPadding);
#endif
	}

	template<usize BlockSize, usize NumBlocks>
	PoolAllocator<BlockSize, NumBlocks>::PoolAllocator(PoolAllocator&& other) noexcept
		: m_mem(StdMove(other.m_mem))
		, m_head(StdMove(other.m_head))
#if ENABLE_ALLOC_STATS
		, m_blockPadding(other.m_blockPadding)
#endif
	{
	}

	template<usize BlockSize, usize NumBlocks>
	PoolAllocator<BlockSize, NumBlocks>::~PoolAllocator() noexcept
	{
		m_mem.Dealloc();
	}

	template<usize BlockSize, usize NumBlocks>
	auto PoolAllocator<BlockSize, NumBlocks>::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		ASSERT(align <= BlockSize, "Cannot have a greater aligment than the blocksize");
		ASSERT(size <= BlockSize, "Cannot allocate more than the blocksize");

		u8* pMem = m_mem.Ptr();
		usize handle, next;
		do
		{
			handle = m_head;

			// In case we run out of space when updating the head
			if (handle >= m_mem.Size()) UNLIKELY
				return MemRef<u8>{ nullptr };

			next = *reinterpret_cast<usize*>(pMem + handle);
		}
		while (!m_head.CompareExchangeWeak(handle, next));

#if ENABLE_ALLOC_STATS
		const usize overhead = m_blockSize - size;
		m_stats.AddAlloc(size, overhead, isBacking);
#endif
		
		return { handle, this, Log2(align), size, isBacking };
	}

	template<usize BlockSize, usize NumBlocks>
	auto PoolAllocator<BlockSize, NumBlocks>::DeallocateRaw(MemRef<u8>&& mem) noexcept -> void
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
		const usize overhead = m_blockSize - size;
		m_stats.RemoveAlloc(size, overhead, mem.IsBackingMem());
#endif
	}

	template<usize BlockSize, usize NumBlocks>
	auto PoolAllocator<BlockSize, NumBlocks>::TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8*
	{
		return m_mem.Ptr() + mem.GetRawHandle();
	}
}
