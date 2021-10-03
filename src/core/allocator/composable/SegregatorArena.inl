#pragma once
#if __RESHARPER__
#include "SegregatorArena.h"
#endif

namespace Core::Alloc
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

		if (size > m_bound)
			return m_gtAlloc.template Allocate<u8>(size, align, isBacking);
		return m_leAlloc.template Allocate<u8>(size, align, isBacking);
	}

	template <ImplementsIAllocator GtAlloc, ImplementsIAllocator LeAlloc>
	auto SegregatorAllocator<GtAlloc, LeAlloc>::DeallocateRaw(MemRef<u8>&& mem) noexcept -> void
	{
		// TODO: Alloc stats

		if (mem.Size() > m_bound)
			m_gtAlloc.Deallocate(Move(mem));
		else
			m_leAlloc.Deallocate(Move(mem));
	}

	template <ImplementsIAllocator GtAlloc, ImplementsIAllocator LeAlloc>
	auto SegregatorAllocator<GtAlloc, LeAlloc>::TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8*
	{
		if (mem.Size() > m_bound)
			return m_gtAlloc.TranslateToPtr(mem);
		return m_leAlloc.TranslateToPtr(mem);
	}
}