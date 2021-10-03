#pragma once
#if __RESHARPER__
#include "InPlaceStackAllocator.h"
#endif

#include "core/Assert.h"

namespace Core::Alloc
{
	template<usize Size, usize MaxAlignment>
	InPlaceStackAllocator<Size, MaxAlignment>::InPlaceStackAllocator() noexcept
		: m_mem()
		, m_offset(0)
	{
		STATIC_ASSERT(IsPowOf2(MaxAlignment), "Max alignment needs to be a power of 2");
		STATIC_ASSERT(Size & ~(MaxAlignment - 1), "Memory size needs to be a multiple of maxAlignment");
	}

	template<usize Size, usize MaxAlignment>
	auto InPlaceStackAllocator<Size, MaxAlignment>::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		ASSERT(align <= m_maxAlign, "Alignment cannot be larger than is allowed by the allocator");
		UNUSED(align);

		if (m_offset + size > Size) UNLIKELY
			return nullptr;

		const usize mask = align - 1;
		const usize diff = size & mask;
		const usize padding = (diff == 0 ? 0 : MaxAlignment - diff);
		const usize paddedSize = size + padding;

		usize handle = m_offset;
		m_offset += paddedSize;

#if ENABLE_ALLOC_STATS
		m_stats.AddAlloc(size, padding, isBacking);
#endif

		return { handle, this, Log2(align), size, isBacking };
	}

	template<usize Size, usize MaxAlignment>
	auto InPlaceStackAllocator<Size, MaxAlignment>::DeallocateRaw(MemRef<u8>&& mem) noexcept -> void
	{
		const usize handle = mem.GetRawHandle();

#if ENABLE_ASSERT || ENABLE_ALLOC_STATS
		const usize align = mem.Align();
		const usize size = mem.Size();
		const usize mask = align - 1;
		const usize diff = size & mask;
		const usize padding = (diff == 0 ? 0 : MaxAlignment - diff);
		const usize paddedSize = size + padding;

		ASSERT(handle == m_offset - paddedSize, "Deallocations can only happen from the top of the stack");
#endif
		
		m_offset = handle;

#if ENABLE_ALLOC_STATS
		m_stats.RemoveAlloc(size, padding, mem.IsBackingMem());
#endif
	}

	template<usize Size, usize MaxAlignment>
	auto InPlaceStackAllocator<Size, MaxAlignment>::TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8*
	{
		return m_mem + mem.GetRawHandle();
	}
}
