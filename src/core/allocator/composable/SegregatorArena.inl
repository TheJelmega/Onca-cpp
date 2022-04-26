#pragma once
#if __RESHARPER__
#include "SegregatorArena.h"
#endif

namespace Onca::Alloc
{
	template <ImplementsIAllocator GtAlloc, ImplementsIAllocator LeAlloc>
	SegregatorAllocator<GtAlloc, LeAlloc>::SegregatorAllocator(usize bound, GtAlloc&& gtAlloc, LeAlloc&& leAlloc)
		: m_bound(bound)
		, m_gtAlloc(Move(gtAlloc))
		, m_leAlloc(Move(leAlloc))
	{
	}

	template <ImplementsIAllocator GtAlloc, ImplementsIAllocator LeAlloc>
	auto SegregatorAllocator<GtAlloc, LeAlloc>::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		// TODO: Alloc stats

		MemRef<u8> mem;
		if (size > m_bound)
			mem = m_gtAlloc.template Allocate<u8>(size, align, isBacking);
		else
			mem = m_leAlloc.template Allocate<u8>(size, align, isBacking);
		mem.SetAlloc(this);
		return mem;
	}

	template <ImplementsIAllocator GtAlloc, ImplementsIAllocator LeAlloc>
	void SegregatorAllocator<GtAlloc, LeAlloc>::DeallocateRaw(MemRef<u8>&& mem) noexcept
	{
		// TODO: Alloc stats

		if (mem.Size() > m_bound)
			m_gtAlloc.Deallocate(Move(mem));
		else
			m_leAlloc.Deallocate(Move(mem));
	}
}
