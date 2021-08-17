#include "StackAllocator.h"

#include "core/Assert.h"

namespace Core::Alloc
{
	StackAllocator::StackAllocator(IAllocator* pBackingAlloc, usize size, u16 maxAlign) noexcept
		: m_mem(pBackingAlloc->Allocate<u8>(size, maxAlign, true))
		, m_offset(0)
		, m_maxAlign(maxAlign)
	{
		ASSERT(IsPowOf2(maxAlign), "Max alignment needs to be a power of 2");
		ASSERT(size & ~(maxAlign - 1), "Memory size needs to be a multiple of maxAlignment");
	}

	StackAllocator::~StackAllocator() noexcept
	{
		m_mem.Dealloc();
	}

	auto StackAllocator::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		ASSERT(align <= m_maxAlign, "Alignment cannot be larger than is allowed by the allocator");
		UNUSED(align);

		if (m_offset + size > m_mem.Size()) UNLIKELY
			return MemRef<u8>{ nullptr };

		const usize mask = align - 1;
		const usize diff = size & mask;
		const usize padding = (diff == 0 ? 0 : m_maxAlign - diff);
		const usize paddedSize = size + padding;
		 
		usize handle = m_offset;
		m_offset += paddedSize;

#if ENABLE_ALLOC_STATS
		m_stats.AddAlloc(size, padding, isBacking);
#endif
		
		return { handle, this, Log2(align), size, isBacking };
	}

	auto StackAllocator::DeallocateRaw(MemRef<u8>&& mem) noexcept -> void
	{
#if ENABLE_ASSERT || ENABLE_ALLOC_STATS
		const usize align = mem.Align();
		const usize size = mem.Size();
		const usize mask = align - 1;
		const usize diff = size & mask;
		const usize padding = (diff == 0 ? 0 : m_maxAlign - diff);
		const usize paddedSize = size + padding;
#endif

		const usize handle = mem.GetRawHandle();
		ASSERT(handle == m_offset - paddedSize, "Deallocations can only happen from the top of the stack");
		m_offset = handle;

#if ENABLE_ALLOC_STATS
		m_stats.RemoveAlloc(size, padding, mem.IsBackingMem());
#endif
	}

	auto StackAllocator::TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8*
	{
		return m_mem.Ptr() + mem.GetRawHandle();
	}
}
