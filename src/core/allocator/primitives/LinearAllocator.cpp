#include "LinearAllocator.h"

namespace Core::Alloc
{
	LinearAllocator::LinearAllocator(IAllocator* pBackingAlloc, usize size, u16 startAlign)
		: m_mem(pBackingAlloc->Allocate<u8>(size, startAlign, true))
		, m_offset(0)
	{
	}

	LinearAllocator::~LinearAllocator() noexcept
	{
		m_mem.Dealloc();
	}

	auto LinearAllocator::Reset() noexcept -> void
	{
		m_offset = 0;

#if ENABLE_ALLOC_STATS
		m_stats.ResetCur();
#endif
	}

	auto LinearAllocator::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		const usize mask = ~(align - 1);
		const usize diff = m_offset & mask;
		const usize padding = diff == 0 ? 0 : align - diff;

		if (m_offset + padding + size > m_mem.Size()) UNLIKELY
			return MemRef<u8>{ nullptr };
		
		usize handle = m_offset;
		m_offset += size;

#if ENABLE_ALLOC_STATS
		m_stats.AddAlloc(size, padding, isBacking);
#endif

		return { handle, this, Log2(align), size, isBacking };
	}

	auto LinearAllocator::DeallocateRaw(MemRef<u8>&& mem) noexcept -> void
	{
		UNUSED(mem);
	}

	auto LinearAllocator::TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8*
	{
		return m_mem.Ptr() + mem.GetRawHandle();
	}
}
