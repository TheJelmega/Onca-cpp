#pragma once
#include "core/allocator/IAllocator.h"

namespace Core::Alloc
{
	/**
	 * \brief An allocator that uses malloc
	 */
	class CORE_API Mallocator final : public IAllocator
	{
	public:
		Mallocator() = default;
		Mallocator(Mallocator&&) = default;
		~Mallocator() noexcept override;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		auto DeallocateRaw(MemRef<u8>&& mem) noexcept -> void override;
		auto OwnsInternal(const MemRef<u8>& mem) noexcept -> bool override;
	};
}
