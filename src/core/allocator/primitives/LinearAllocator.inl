#pragma once
#if __RESHARPER__
#include "LinearAllocator.h"
#endif

namespace Onca::Alloc
{
	template<usize Size, usize BaseAlignment>
	LinearAllocator<Size, BaseAlignment>::LinearAllocator(IAllocator* pBackingAlloc)
		: IMemBackedAllocator(pBackingAlloc->Allocate<u8>(Size, BaseAlignment, true))
	{
		m_head = m_mem.Ptr();
	}
	
	template<usize Size, usize BaseAlignment>
	void LinearAllocator<Size, BaseAlignment>::Reset() noexcept
	{
		m_head = m_mem.Ptr();

#if ENABLE_ALLOC_STATS
		m_stats.ResetCur();
#endif
	}

	template<usize Size, usize BaseAlignment>
	auto LinearAllocator<Size, BaseAlignment>::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		ASSERT(Math::IsPowOf2(align), "Alignment needs to be a power of 2");

		const usize mask = align - 1;
		const usize diff = m_head & mask;
		const usize padding = (align - diff) & align;
		const usize paddedSize = size + padding;

		if (m_head + paddedSize > m_mem.Ptr() + m_mem.Size()) UNLIKELY
			return nullptr;
		
		u8* ptr = m_head;
		m_head += paddedSize;

#if ENABLE_ALLOC_STATS
		m_stats.AddAlloc(size, padding, isBacking);
#endif

		return { ptr, this, Math::Log2(align), size, isBacking };
	}

	template<usize Size, usize BaseAlignment>
	void LinearAllocator<Size, BaseAlignment>::DeallocateRaw(MemRef<u8>&& mem) noexcept
	{
		UNUSED(mem);
	}
}
