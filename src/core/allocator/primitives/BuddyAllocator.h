#pragma once
#include "core/allocator/IAllocator.h"

namespace Core::Alloc
{
	class BuddyAllocator : public IAllocator
	{
	public:
		BuddyAllocator(IAllocator* pBackingAlloc, usize size, u8 maxSubDivisions);
		BuddyAllocator(BuddyAllocator&&) = default;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		auto DeallocateRaw(MemRef<u8>&& mem) noexcept -> void override;
		auto TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8* override;

	private:
		MemRef<u8> m_mem;
		usize m_size;
		usize m_managmentSize;
		Threading::Mutex m_mutex;
	};
}