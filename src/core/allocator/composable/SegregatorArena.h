#pragma once

#include "core/allocator/IAllocator.h"

namespace Core::Alloc
{
	/**
	 * An allocator that selects 1 of 2 underlying allocators, depending on the size of the allocation
	 * \tparam GtAlloc Allocator to use for sizes greater than Bound
	 * \tparam LeAlloc Allocator to use for sizes less or equal than Bound
	 */
	template<ImplementsIAllocator GtAlloc, ImplementsIAllocator LeAlloc>
	class SegregatorAllocator final : public IAllocator
	{
	public:
		SegregatorAllocator(usize bound, GtAlloc&& gtAlloc, LeAlloc&& leAlloc);
		SegregatorAllocator(SegregatorAllocator&&) = default;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		auto DeallocateRaw(MemRef<u8>&& mem) noexcept -> void override;
		auto TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8* override;

	private:
		usize   m_bound;   ///< Boundary between allocators
		GtAlloc m_gtAlloc; ///< Allocator for allocations with sizes greater than Bound
		LeAlloc m_leAlloc; ///< Allocator for allocations with sizes less or equal than Bound
	};
}

#include "SegregatorArena.inl"