#pragma once
#include "FallbackArena.h"

namespace Core::Alloc
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

		MemRef<u8> mem = m_main.Allocate(size, align, isBacking);
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
			
		mem = m_fallback.Allocate(size, align, isBacking);

#if ENABLE_ALLOC_STATS
		usize newMemUse, newOverhead;
		m_fallback.GetAllocStats().GetCurStats(newMemUse, _, newOverhead, _);
		m_stats.AddAlloc(newMemUse - oldMemUse, newOverhead - oldOverhead, isBacking);
#endif

		return mem;
	}

	template <ImplementsIAllocator MainAlloc, ImplementsIAllocator Fallback>
	auto FallbackArena<MainAlloc, Fallback>::DeallocateRaw(MemRef<u8>&& mem) noexcept -> void
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

	template <ImplementsIAllocator MainAlloc, ImplementsIAllocator FallbackAlloc>
	auto FallbackArena<MainAlloc, FallbackAlloc>::TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8*
	{
		ASSERT(false, "MemRef's allocator should never reference a FallbackArena");
		return nullptr;
	}
}
