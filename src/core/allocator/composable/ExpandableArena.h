#pragma once
#include "core/allocator/IAllocator.h"
#include "core/collections/DynArray.h"
#include "core/memory/Unique.h"

namespace Core::Alloc
{
	template<typename T, typename ...Args>
	concept ExtendableAlloc = requires(IAllocator* alloc, Args... args)
	{
		MoveConstructable<T>;
		DerivesFrom<T, IAllocator>;
		{ T{ alloc, args... } } noexcept;
	};

	/**
	 * An arena encapsulating a number of dynamic allocators to satisfy a certain amount of space
	 * \tparam Alloc Allocator to use
	 * \tparam AllocArgs Types of allocation arguments
	 */
	template<typename Alloc, typename ...AllocArgs>
		requires ExtendableAlloc<Alloc, AllocArgs...>
	class ExpandableArena final : public IAllocator
	{
	public:
		ExpandableArena(IAllocator* expandAlloc, AllocArgs&&... allocArgs);

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		auto DeallocateRaw(MemRef<u8>&& mem) noexcept -> void override;
		auto OwnsInternal(IAllocator* pAlloc) noexcept -> bool override;
		auto TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8* override;

	private:
		Tuple<IAllocator*>      m_backingAlloc; ///< Allocator to back sub-allocators
		Tuple<AllocArgs...>     m_allocArgs;    ///< Additional arguments for the allocator
		DynArray<Unique<Alloc>> m_allocs;       ///< Allocators
	};
}

#include "ExpandableArena.inl"