#pragma once
#include "core/allocator/IAllocator.h"
#include "core/collections/DynArray.h"
#include "core/memory/Unique.h"

namespace Core::Alloc
{
	/**
	 * An interface defining the requirements an allocator needs to be able to be used in an expandable allocator
	 * \tparam T Type to constrain
	 * \tparam Args Argument types for allocator ctor
	 */
	template<typename T, typename ...Args>
	concept ExtendableAlloc = requires(IAllocator* alloc, Args... args)
	{
		MoveConstructable<T>;
		DerivesFrom<T, IAllocator>;
		{ T{ alloc, args... } } noexcept;
	};

	/**
	 * An expandable arena managed a set of allocators and allows the creation of additional allocators when the current ones run out of space
	 *
	 * \tparam Alloc Allocator to use
	 * \tparam AllocArgs Types of allocation arguments
	 */
	template<typename Alloc, typename ...AllocArgs>
		requires ExtendableAlloc<Alloc, AllocArgs...>
	class ExpandableArena final : public IAllocator
	{
	public:
		/**
		 * Create an expandable allocator
		 * \param expandAlloc Backing allocator used to expand size
		 * \param allocArgs Arguments for sub allocators
		 */
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