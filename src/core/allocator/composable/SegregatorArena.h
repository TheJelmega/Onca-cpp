#pragma once

#include "core/allocator/IAllocator.h"

namespace Onca::Alloc
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
		/**
		 * 
		 * \param[in] bound Maximum size of allocation to use the gtAlloc
		 * \param[in] gtAlloc Allocator to use when the size of the allocation is greater than the bound
		 * \param[in] leAlloc Allocator to use when the size of the allocation is less or equal than the bound
		 */
		SegregatorAllocator(usize bound, GtAlloc&& gtAlloc, LeAlloc&& leAlloc);
		SegregatorAllocator(SegregatorAllocator&&) = default;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		void DeallocateRaw(MemRef<u8>&& mem) noexcept override;

	private:
		usize   m_bound;   ///< Boundary between allocators
		GtAlloc m_gtAlloc; ///< Allocator for allocations with sizes greater than Bound
		LeAlloc m_leAlloc; ///< Allocator for allocations with sizes less or equal than Bound
	};
}

#include "SegregatorArena.inl"