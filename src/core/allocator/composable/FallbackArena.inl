#pragma once
#if __RESHARPER__
#include "FallbackArena.h"
#endif

namespace Onca::Alloc
{
	template <ImplementsIAllocator MainAlloc, ImplementsIAllocator Fallback>
	FallbackArena<MainAlloc, Fallback>::FallbackArena(MainAlloc&& main, Fallback&& fallback)
		: m_main(Move(main))
		, m_fallback(Move(fallback))
	{
	}

	template <ImplementsIAllocator MainAlloc, ImplementsIAllocator Fallback>
	auto FallbackArena<MainAlloc, Fallback>::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		
#if ENABLE_ALLOC_STATS
			usize _, oldMemUse, oldOverhead;
			m_main.GetAllocStats().GetCurStats(oldMemUse, _, oldOverhead, _);
#endif

		MemRef<u8> mem = m_main.template Allocate<u8>(size, align, isBacking);
		if (mem)
		{
#if ENABLE_ALLOC_STATS
			usize newMemUse, newOverhead;
			m_main.GetAllocStats().GetCurStats(newMemUse, _, newOverhead, _);
			m_stats.AddAlloc(newMemUse - oldMemUse, newOverhead - oldOverhead, isBacking);
#endif

			return mem;
		}
			
			
#if ENABLE_ALLOC_STATS
		m_fallback.GetAllocStats().GetCurStats(oldMemUse, _, oldOverhead, _);
#endif		
			
		mem = m_fallback.template Allocate<u8>(size, align, isBacking);

#if ENABLE_ALLOC_STATS
		usize newMemUse, newOverhead;
		m_fallback.GetAllocStats().GetCurStats(newMemUse, _, newOverhead, _);
		m_stats.AddAlloc(newMemUse - oldMemUse, newOverhead - oldOverhead, isBacking);
#endif

		return mem;
	}

	template <ImplementsIAllocator MainAlloc, ImplementsIAllocator Fallback>
	void FallbackArena<MainAlloc, Fallback>::DeallocateRaw(MemRef<u8>&& mem) noexcept
	{
		if (m_main.Owns(mem))
		{
#if ENABLE_ALLOC_STATS
			usize _, oldMemUse, oldOverhead;
			m_main.GetAllocStats().GetCurStats(oldMemUse, _, oldOverhead, _);
#endif

			m_main.Deallocate(Move(mem));

#if ENABLE_ALLOC_STATS
			usize newMemUse, newOverhead;
			m_main.GetAllocStats().GetCurStats(newMemUse, _, newOverhead, _);
			m_stats.AddAlloc(oldMemUse - newMemUse, oldOverhead - newOverhead, mem.IsBackingMem());
#endif
		}
		else
		{
#if ENABLE_ALLOC_STATS
			usize _, oldMemUse, oldOverhead;
			m_fallback.GetAllocStats().GetCurStats(oldMemUse, _, oldOverhead, _);
#endif

			m_fallback.Deallocate(Move(mem));

#if ENABLE_ALLOC_STATS
			usize newMemUse, newOverhead;
			m_fallback.GetAllocStats().GetCurStats(newMemUse, _, newOverhead, _);
			m_stats.AddAlloc(oldMemUse - newMemUse, oldOverhead - newOverhead, mem.IsBackingMem());
#endif
		}
	}
}
