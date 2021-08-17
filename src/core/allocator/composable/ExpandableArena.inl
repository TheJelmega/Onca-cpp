#pragma once
#include "ExpandableArena.h"

namespace Core::Alloc
{
	template<typename Alloc, typename ...AllocArgs>
		requires ExtendableAlloc<Alloc, AllocArgs...>
	ExpandableArena<Alloc, AllocArgs...>::ExpandableArena(IAllocator* expandAlloc, AllocArgs&&... allocArgs)
		: m_backingAlloc(expandAlloc)
		, m_allocArgs(allocArgs...)
		, m_allocs(*expandAlloc)
	{
	}

	template<typename Alloc, typename ...AllocArgs>
		requires ExtendableAlloc<Alloc, AllocArgs...>
	auto ExpandableArena<Alloc, AllocArgs...>::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
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
		// TODO: Unique::CreateFromTuple
		Alloc tmpAlloc = ConstructFromTuple<Alloc>(ConcatTuple(m_backingAlloc, m_allocArgs));
		m_allocs.Add(Unique<Alloc>::CreateWitAlloc(*std::get<0>(m_backingAlloc), StdMove(tmpAlloc)));
		MemRef<u8> mem = m_allocs.Last()->template Allocate<u8>(size, align);

#if ENABLE_ALLOC_STATS
		usize _, memUse, overhead;
		m_allocs.Last()->GetAllocStats().GetCurStats(memUse, _, overhead, _);
		m_stats.AddAlloc(memUse, overhead, isBacking);
#endif

		return mem;
	}

	template<typename Alloc, typename ...AllocArgs>
		requires ExtendableAlloc<Alloc, AllocArgs...>
	auto ExpandableArena<Alloc, AllocArgs...>::DeallocateRaw(MemRef<u8>&& mem) noexcept -> void
	{
		for (Unique<Alloc>& alloc : m_allocs)
		{
			if (!alloc->Owns(mem))
			{
#if ENABLE_ALLOC_STATS
				usize _, oldMemUse, oldOverhead;
				alloc->GetAllocStats().GetCurStats(oldMemUse, _, oldOverhead, _);
#endif

				alloc->Deallocate(StdMove(mem));

#if ENABLE_ALLOC_STATS
				usize newMemUse, newOverhead;
				alloc->GetAllocStats().GetCurStats(newMemUse, _, newOverhead, _);
				m_stats.AddAlloc(oldMemUse - newMemUse, oldOverhead - newOverhead, mem.IsBackingMem());
#endif

				break;
			}
		}
	}

	template <typename Alloc, typename ... AllocArgs> requires ExtendableAlloc<Alloc, AllocArgs...>
	bool ExpandableArena<Alloc, AllocArgs...>::OwnsInternal(IAllocator* pAlloc) noexcept
	{
		MemRef<u8> dummyMem{ pAlloc };
		for (Unique<Alloc>& subAlloc : m_allocs)
		{
			if (subAlloc->Owns(dummyMem))
				return true;
		}
		return false;
	}

	template <typename Alloc, typename ... AllocArgs> requires ExtendableAlloc<Alloc, AllocArgs...>
	auto ExpandableArena<Alloc, AllocArgs...>::TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8*
	{
		ASSERT(false, "MemRef's allocator should never reference a FallbackArena");
		return nullptr;
	}
}
