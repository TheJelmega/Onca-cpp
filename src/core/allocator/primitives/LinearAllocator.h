#pragma once
#include "core/allocator/IAllocator.h"

namespace Core::Alloc
{

	/**
	 * A linear allocator (can not deallocate, only can reset) (NOT threadsafe)
	 */
	class LinearAllocator : public IAllocator
	{
	public:
		LinearAllocator(IAllocator* pBackingAlloc, usize size, u16 startAlign);
		LinearAllocator(LinearAllocator&&) = default;

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