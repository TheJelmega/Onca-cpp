#pragma once
#include "core/allocator/IAllocator.h"

namespace Core::Alloc
{
	/**
	 * \brief A linear allocator (can not deallocate, only can reset) (NOT threadsafe)
	 *
	 * A linear allocator holds a chunk of memory, from which it can allocate memory from,
	 * but it can only allocate, as it has no knowledge on how to deallocate anything.
	 * Any new allocation made on the allocator, will be appended to the end and this will happen until the entire allocator is reset,
	 * allowing new memory to start at the beginning, but invalidating all former allocations made.
	 * Alignment is done using padding between allocations.
	 *
	 * begin                    head
	 * v                        v
	 * +------------------------+-----------------
	 * |4C696E65617229416C6C6F63|?????????????????
	 * +------------------------+-----------------
	 *
	 * \note A linear allocator cannot be defragmented
	 */
	class LinearAllocator final : public IAllocator
	{
	public:
		/**
		 * Create a linear allcoator
		 * \param pBackingAlloc Allocator to create the underlying memory block
		 * \param size Size of the allocator
		 * \param startAlign Start alignment for the allocator
		 */
		LinearAllocator(IAllocator* pBackingAlloc, usize size, u16 startAlign);
		LinearAllocator(LinearAllocator&&) = default;

		/**
		 * Reset the head of the allocator
		 * \note This will invalidate all memory owned by this allocator
		 */
		auto Reset() noexcept -> void;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		auto DeallocateRaw(MemRef<u8>&& mem) noexcept -> void override;
		auto TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8* override;

	private:
		MemRef<u8>  m_mem;    ///< Managed memory
		usize       m_offset; ///< Current location in the linear allocator
	};

}