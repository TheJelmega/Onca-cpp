#include "StackAllocator.h"

#include "core/Assert.h"

namespace Core::Alloc
{
	template<usize Size, usize MaxAlignment>
	StackAllocator<Size, MaxAlignment>::StackAllocator(IAllocator* pBackingAlloc) noexcept
		: m_mem(pBackingAlloc->Allocate<u8>(Size, MaxAlignment, true))
		, m_offset(0)
	{
		STATIC_ASSERT(IsPowOf2(MaxAlignment), "Max alignment needs to be a power of 2");
		STATIC_ASSERT(Size & ~(MaxAlignment - 1), "Memory size needs to be a multiple of maxAlignment");
	}

	template<usize Size, usize MaxAlignment>
	StackAllocator<Size, MaxAlignment>::~StackAllocator() noexcept
	{
		m_mem.Dealloc();
	}

	template<usize Size, usize MaxAlignment>
	auto StackAllocator<Size, MaxAlignment>::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		ASSERT(align <= MaxAlignment, "Alignment cannot be larger than is allowed by the allocator");
		UNUSED(align);

		if (m_offset + size > m_mem.Size()) UNLIKELY
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
		
		return { handle, this, Math::Log2(align), size, isBacking };
	}

	template<usize Size, usize MaxAlignment>
	auto StackAllocator<Size, MaxAlignment>::DeallocateRaw(MemRef<u8>&& mem) noexcept -> void
	{
#if ENABLE_ASSERT || ENABLE_ALLOC_STATS
		const usize align = mem.Align();
		const usize size = mem.Size();
		const usize mask = align - 1;
		const usize diff = size & mask;
		const usize padding = (diff == 0 ? 0 : MaxAlignment - diff);
		const usize paddedSize = size + padding;
#endif

		const usize handle = mem.GetRawHandle();
		ASSERT(handle == m_offset - paddedSize, "Deallocations can only happen from the top of the stack");
		m_offset = handle;

#if ENABLE_ALLOC_STATS
		m_stats.RemoveAlloc(size, padding, mem.IsBackingMem());
#endif
	}

	template<usize Size, usize MaxAlignment>
	auto StackAllocator<Size, MaxAlignment>::TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8*
	{
		return m_mem.Ptr() + mem.GetRawHandle();
	}
}
