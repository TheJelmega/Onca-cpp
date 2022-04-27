#pragma once
#if __RESHARPER__
#include "IAllocator.h"
#endif

#include "core/Assert.h"
#include "core/math/MathUtils.h"

namespace Onca::Alloc
{	
	template <typename T>
	auto IAllocator::Allocate(usize size, u16 align, bool isBacking) noexcept -> MemRef<T>
	{
		ASSERT(align > 0 && Math::IsPowOf2(align), "Alignment needs to be a power of 2");
		return AllocateRaw(size, align, isBacking).As<T>();
	}

	template <typename T>
	void IAllocator::Deallocate(MemRef<T>&& ref) noexcept
	{
		DeallocateRaw(ref.template As<u8>());
		MemClearData(ref);
	}

	template <typename T>
	auto IAllocator::Owns(const MemRef<T>& ref) noexcept -> bool
	{
		return OwnsInternal(ref.template As<u8>());
	}

	INL void AllocatorStats::AddAlloc(usize memUse, usize overhead, bool isBacking) noexcept
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

		maxMemoryUsed = Math::Max(curMemoryUse, maxMemoryUsed);
		maxAllocs = Math::Max(curAllocs, maxAllocs);
		maxOverhead = Math::Max(curOverhead, maxOverhead);
		maxBackingMemory = Math::Max(curBackingMemory, maxBackingMemory);
	}

	INL void AllocatorStats::RemoveAlloc(usize memUse, usize overhead, bool isBacking) noexcept
	{
		Threading::Lock lock(m_statMutex);
		
		curMemoryUse -= memUse;
		curBackingMemory -= memUse * usize(isBacking);
		curOverhead -= overhead;
		++curAllocs;
	}

	INL void AllocatorStats::ResetCur() noexcept
	{
		Threading::Lock lock(m_statMutex);

		curMemoryUse = 0;
		curBackingMemory = 0;
		curOverhead = 0;
		curAllocs = 0;
	}

	INL void AllocatorStats::GetCurStats(usize& memUse, usize& numAllocs, usize& overhead,
	                                     usize& backingMem) noexcept
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

	INL auto IAllocator::OwnsInternal(const MemRef<u8>& mem) noexcept -> bool
	{
		return mem.GetAlloc() == this;
	}

	inline IMemBackedAllocator::IMemBackedAllocator(MemRef<u8>&& mem) noexcept
		: m_mem(Move(mem))
	{
	}

	inline IMemBackedAllocator::~IMemBackedAllocator() noexcept
	{
		m_mem.Dealloc();
	}

	inline auto IMemBackedAllocator::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		return {};
	}

	inline void IMemBackedAllocator::DeallocateRaw(MemRef<u8>&& mem) noexcept
	{
	}

	inline bool IMemBackedAllocator::OwnsInternal(const MemRef<u8>& mem) noexcept
	{
		u8* ptr = mem.Ptr();
		u8* buffer = m_mem.Ptr();
		return ptr >= buffer && ptr < buffer + m_mem.Size();
	}
}
