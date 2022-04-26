#pragma once
#if __RESHARPER__
#include "LinearAllocator.h"
#endif

namespace Onca::Alloc
{
	template<usize Size, usize BaseAlignment>
	LinearAllocator<Size, BaseAlignment>::LinearAllocator(IAllocator* pBackingAlloc)
		: m_mem(pBackingAlloc->Allocate<u8>(Size, BaseAlignment, true))
	{
		m_head = m_mem.Ptr();
	}

	template<usize Size, usize BaseAlignment>
	LinearAllocator<Size, BaseAlignment>::~LinearAllocator() noexcept
	{
		m_mem.Dealloc();
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
		const usize mask = ~(align - 1);
		const usize diff = m_head & mask;
		const usize padding = diff == 0 ? 0 : align - diff;

		if (m_head + padding + size > m_mem.Ptr() + m_mem.Size()) UNLIKELY
			return nullptr;
		
		u8* ptr = m_head;
		m_head += size;

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

	template <usize Size, usize BaseAlignment>
	bool LinearAllocator<Size, BaseAlignment>::OwnsInternal(const MemRef<u8>& mem) noexcept
	{
		u8* ptr = mem.Ptr();
		u8* buffer = m_mem.Ptr();
		return ptr >= buffer && ptr < buffer + m_mem.Size();
	}
}
