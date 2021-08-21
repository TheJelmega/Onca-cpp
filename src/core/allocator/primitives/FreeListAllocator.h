#pragma once
#include "core/allocator/IAllocator.h"

namespace Core::Alloc
{

	class CORE_API FreeListAllocator final : public IAllocator
	{
	private:

		struct FreeHeader
		{
			usize size;
			usize next;
		};

		struct AllocHeader
		{
			u16 frontPadding;
			u16 backPadding;
		};

	public:
		FreeListAllocator(IAllocator* pBackingAllocator, usize size) noexcept;
		FreeListAllocator(FreeListAllocator&&) = default;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		auto DeallocateRaw(MemRef<u8>&& mem) noexcept -> void override;
		auto TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8* override;

	private:

		auto AllocFirst(usize size, u16 align, bool isBacking) noexcept -> usize;
		auto Coalesce(u8* pBegin, usize prevLoc, usize curLoc, usize nextLoc) noexcept -> void;

		MemRef<u8> m_mem;  ///< Managed memory
		usize      m_head; ///< Offset to free head
	};

}
