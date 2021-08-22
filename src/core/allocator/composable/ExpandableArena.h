#pragma once
#include "core/allocator/IAllocator.h"
#include "core/collections/DynArray.h"
#include "core/memory/Unique.h"

namespace Core::Alloc
{
	/**
	 * An interface defining the requirements an allocator needs to be able to be used in an expandable allocator
	 * \tparam T Type to constrain
	 */
	template<typename T>
	concept ExtendableAlloc =
		ImplementsIAllocator<T> &&
		MoveConstructable<T> &&
		requires(IAllocator* pAlloc)
	{
		{ T{ pAlloc } } noexcept;
	};

	/**
	 * An expandable arena managed a set of allocators and allows the creation of additional allocators when the current ones run out of space
	 *
	 * \tparam Alloc Allocator to use
	 */
	template<ExtendableAlloc Alloc>
	class ExpandableArena final : public IAllocator
	{
	public:
		/**
		 * Create an expandable allocator
		 * \param expandAlloc Backing allocator used to expand size
		 */
		ExpandableArena(IAllocator* expandAlloc);

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		auto DeallocateRaw(MemRef<u8>&& mem) noexcept -> void override;
		auto OwnsInternal(IAllocator* pAlloc) noexcept -> bool override;
		auto TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8* override;

	private:
		IAllocator*             m_backingAlloc; ///< Allocator to back sub-allocators
		// TODO: replace with theadsafe container
		DynArray<Unique<Alloc>> m_allocs;       ///< Allocators
		Threading::Mutex        m_mutex;        ///< Mutex to guard DynArray
	};
}

#include "ExpandableArena.inl"