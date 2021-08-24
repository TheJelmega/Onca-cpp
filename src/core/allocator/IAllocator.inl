#pragma once
#include "IAllocator.h"
#include "core/Assert.h"
#include "core/Utils.h"
#include "core/math/MathUtils.h"

namespace Core::Alloc
{	
	template <typename T>
	auto IAllocator::Allocate(usize size, u16 align, bool isBacking) noexcept -> MemRef<T>
	{
		ASSERT(align > 0 && IsPowOf2(align), "Alignment needs to be a power of 2");
		return AllocateRaw(size, align, isBacking).As<T>();
	}

	template <typename T>
	auto IAllocator::Deallocate(MemRef<T>&& ref) noexcept -> void
	{
		ASSERT(Owns(ref), "MemRef is not owned by allocator");
		DeallocateRaw(ref.template As<u8>());
		MemClearData(ref);
	}

	template <typename T>
	auto IAllocator::Owns(const MemRef<T>& ref) noexcept -> bool
	{
		return OwnsInternal(ref.GetAlloc());
	}

	template <typename T>
	auto IAllocator::TranslateToPtr(const MemRef<T>& mem) noexcept -> T*
	{
		return reinterpret_cast<T*>(TranslateToPtrInternal(mem.template As<u8>()));
	}

	INL auto AllocatorStats::AddAlloc(usize memUse, usize overhead, bool isBacking) noexcept -> void
	{
		Threading::Lock lock(m_statMutex);
		
		curMemoryUse += memUse;
		curBackingMemory += memUse * usize(isBacking);
		curOverhead += overhead;
		++curAllocs;

		totalMemoryUsed += memUse;
		totalBackingMemory += memUse * usize(isBacking);
		totalOverhead += overhead;
		++totalAllocs;

		maxMemoryUsed = Max(curMemoryUse, maxMemoryUsed);
		maxAllocs = Max(curAllocs, maxAllocs);
		maxOverhead = Max(curOverhead, maxOverhead);
		maxBackingMemory = Max(curBackingMemory, maxBackingMemory);
	}

	INL auto AllocatorStats::RemoveAlloc(usize memUse, usize overhead, bool isBacking) noexcept -> void
	{
		Threading::Lock lock(m_statMutex);
		
		curMemoryUse -= memUse;
		curBackingMemory -= memUse * usize(isBacking);
		curOverhead -= overhead;
		++curAllocs;
	}

	INL auto AllocatorStats::ResetCur() noexcept -> void
	{
		Threading::Lock lock(m_statMutex);

		curMemoryUse = 0;
		curBackingMemory = 0;
		curOverhead = 0;
		curAllocs = 0;
	}

	INL auto AllocatorStats::GetCurStats(usize& memUse, usize& numAllocs, usize& overhead,
	                                     usize& backingMem) noexcept -> void
	{
		Threading::Lock lock(m_statMutex);

		memUse = curMemoryUse;
		numAllocs = curAllocs;
		overhead = curOverhead;
		backingMem = curBackingMemory;
	}

	INL auto IAllocator::GetAllocStats() noexcept -> AllocatorStats&
	{
#if ENABLE_ALLOC_STATS
		return m_stats;
#else
		static AllocatorStats stats;
		return stats;
#endif
	}

	INL auto IAllocator::OwnsInternal(IAllocator* pAlloc) noexcept -> bool
	{
		return pAlloc == this;
	}
}
