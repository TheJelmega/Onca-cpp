#include "LinearAllocator.h"

namespace Core::Alloc
{
	template<usize Size, usize BaseAlignment>
	LinearAllocator<Size, BaseAlignment>::LinearAllocator(IAllocator* pBackingAlloc)
		: m_mem(pBackingAlloc->Allocate<u8>(Size, BaseAlignment, true))
		, m_offset(0)
	{
	}

	template<usize Size, usize BaseAlignment>
	LinearAllocator<Size, BaseAlignment>::~LinearAllocator() noexcept
	{
		m_mem.Dealloc();
	}

	template<usize Size, usize BaseAlignment>
	auto LinearAllocator<Size, BaseAlignment>::Reset() noexcept -> void
	{
		m_offset = 0;

#if ENABLE_ALLOC_STATS
		m_stats.ResetCur();
#endif
	}

	template<usize Size, usize BaseAlignment>
	auto LinearAllocator<Size, BaseAlignment>::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		const usize mask = ~(align - 1);
		const usize diff = m_offset & mask;
		const usize padding = diff == 0 ? 0 : align - diff;

		if (m_offset + padding + size > m_mem.Size()) UNLIKELY
			return nullptr;
		
		usize handle = m_offset;
		m_offset += size;

#if ENABLE_ALLOC_STATS
		m_stats.AddAlloc(size, padding, isBacking);
#endif

		return { handle, this, Log2(align), size, isBacking };
	}

	template<usize Size, usize BaseAlignment>
	auto LinearAllocator<Size, BaseAlignment>::DeallocateRaw(MemRef<u8>&& mem) noexcept -> void
	{
		UNUSED(mem);
	}

	template<usize Size, usize BaseAlignment>
	auto LinearAllocator<Size, BaseAlignment>::TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8*
	{
		return m_mem.Ptr() + mem.GetRawHandle();
	}
}
