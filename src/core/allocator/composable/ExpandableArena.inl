#pragma once
#if __RESHARPER__
#include "ExpandableArena.h"
#endif

namespace Onca::Alloc
{
	template<ExtendableAlloc Alloc>
	ExpandableArena<Alloc>::ExpandableArena(IAllocator* expandAlloc)
		: m_backingAlloc(expandAlloc)
		, m_allocs(*expandAlloc)
	{
	}

	template<ExtendableAlloc Alloc>
	auto ExpandableArena<Alloc>::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		Threading::Lock lock{ m_mutex };

		for (Unique<Alloc>& alloc : m_allocs)
		{
#if ENABLE_ALLOC_STATS
			usize _, oldMemUse, oldOverhead;
			alloc->GetAllocStats().GetCurStats(oldMemUse, _, oldOverhead, _);
#endif

			MemRef<u8> mem = alloc->template Allocate<u8>(size, align);

			if (!mem)
				continue;

#if ENABLE_ALLOC_STATS
			usize newMemUse, newOverhead;
			alloc->GetAllocStats().GetCurStats(newMemUse, _, newOverhead, _);
			m_stats.AddAlloc(newMemUse - oldMemUse, newOverhead - oldOverhead, isBacking);
#endif
			
			return mem;
		}

		// No place left, so expand the allocator
		m_allocs.Add(Unique<Alloc>::CreateWitAlloc(*m_backingAlloc, m_backingAlloc));
		MemRef<u8> mem = m_allocs.Back()->template Allocate<u8>(size, align);

#if ENABLE_ALLOC_STATS
		usize _, memUse, overhead;
		m_allocs.Back()->GetAllocStats().GetCurStats(memUse, _, overhead, _);
		m_stats.AddAlloc(memUse, overhead, isBacking);
#endif

		return mem;
	}

	template<ExtendableAlloc Alloc>
	void ExpandableArena<Alloc>::DeallocateRaw(MemRef<u8>&& mem) noexcept
	{
		Threading::Lock lock{ m_mutex };

		for (Unique<Alloc>& alloc : m_allocs)
		{
			if (alloc->Owns(mem))
			{
#if ENABLE_ALLOC_STATS
				usize _, oldMemUse, oldOverhead;
				alloc->GetAllocStats().GetCurStats(oldMemUse, _, oldOverhead, _);
#endif

				alloc->Deallocate(Move(mem));

#if ENABLE_ALLOC_STATS
				usize newMemUse, newOverhead;
				alloc->GetAllocStats().GetCurStats(newMemUse, _, newOverhead, _);
				m_stats.AddAlloc(oldMemUse - newMemUse, oldOverhead - newOverhead, mem.IsBackingMem());
#endif

				break;
			}
		}
	}

	template <ExtendableAlloc Alloc>
	bool ExpandableArena<Alloc>::OwnsInternal(const MemRef<u8>& mem) noexcept
	{
		Threading::Lock lock{ m_mutex };
		for (Unique<Alloc>& subAlloc : m_allocs)
		{
			if (subAlloc->Owns(mem))
				return true;
		}
		return false;
	}
}
