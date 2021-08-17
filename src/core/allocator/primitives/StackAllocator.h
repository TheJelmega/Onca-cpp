#pragma once
#include "core/allocator/IAllocator.h"

namespace Core::Alloc
{
	/**
	 * An allocator that allows the allocation of items on a stack (NOT threadsafe)
	 */
	class CORE_API StackAllocator final : public IAllocator
	{
	public:
		/**
		 * \brief Create a stack allocator
		 * \param[in] pBackingAlloc Allocator to create the underlying memory block
		 * \param[in] size Size of the internal memory
		 * \param[in] maxAlign Maximum alignment for an element
		 */
		explicit StackAllocator(IAllocator* pBackingAlloc, usize size, u16 maxAlign) noexcept;
		StackAllocator(StackAllocator&&) = default;
		~StackAllocator() noexcept override;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		auto DeallocateRaw(MemRef<u8>&& mem) noexcept -> void override;
		auto TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8* override;

	private:
		MemRef<u8>  m_mem;      ///< Managed memory
		usize       m_offset;   ///< Current location on the stack
		const usize m_maxAlign; ///< Maximum alignment possible
	};
}
