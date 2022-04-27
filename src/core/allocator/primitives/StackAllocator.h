#pragma once
#include "core/allocator/IAllocator.h"

namespace Onca::Alloc
{
	 /**
	  * \brief An allocator that allows the allocation of items on a stack (NOT threadsafe)
	  *
	  * A stack allocator holds a chunk of memory, from which it can allocate memory from,
	  * memory can also be deallocated, but this is required to happen in the reverse order of allocation (FILO).
	  *	This limitation is what limits the allocator to just 1 thread, since otherwise no guarantee can be made about the order of deallocations.
	  *	Each new allocation is aligned to the allocator's max alignment, any allocation with a smaller or equal alignment are possible.
	  *
	  * begin        last        head
	  * v            v           v
	  * +------------+-----------+-----------------
	  * |537461636B20|4416C6C6F63|?????????????????
	  * +------------+-----------+-----------------
	  *
	  *	\tparam Size Size of the managed memory
	  *	\tparam MaxAlignment Maximum alignment of an allocation
	  */
	template<usize Size, usize MaxAlignment>
	class StackAllocator final : public IMemBackedAllocator
	{
	public:
		/**
		 * \brief Create a stack allocator
		 * \param[in] pBackingAlloc Allocator to create the underlying memory block
		 */
		explicit StackAllocator(IAllocator* pBackingAlloc) noexcept;
		StackAllocator(StackAllocator&&) = default;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		void DeallocateRaw(MemRef<u8>&& mem) noexcept override;

	private:
		u8*        m_head;   ///< Current location on the stack
	};
}

#include "StackAllocator.inl"
