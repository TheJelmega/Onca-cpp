#pragma once
#if __RESHARPER__
#include "FreeListAllocator.h"
#endif

namespace Core::Alloc
{
	template<usize Size>
	FreeListAllocator<Size>::FreeListAllocator(IAllocator* pBackingAllocator) noexcept
		: m_mem(pBackingAllocator->Allocate<u8>(Size, sizeof(usize), true))
	{
		STATIC_ASSERT(Size > sizeof(FreeHeader), "Size needs to be larger than 16 bytes");
		FreeHeader* pFreeHeader = reinterpret_cast<FreeHeader*>(m_mem.Ptr());
		pFreeHeader->next = nullptr;
		pFreeHeader->size = Size;
		m_head = m_mem.Ptr();
	}

	template <usize Size>
	FreeListAllocator<Size>::~FreeListAllocator() noexcept
	{
		m_mem.Dealloc();
	}

	template<usize Size>
	auto FreeListAllocator<Size>::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		u8* ptr = AllocFirst(size, align, isBacking);
		if (!ptr)
			return nullptr;
		return { ptr, this, Math::Log2(align), size, isBacking };
	}

	template<usize Size>
	auto FreeListAllocator<Size>::DeallocateRaw(MemRef<u8>&& mem) noexcept -> void
	{
		u8* ptr = mem.Ptr();

		AllocHeader* pAllocHeader = reinterpret_cast<AllocHeader*>(ptr - sizeof(AllocHeader));
		const usize padding = pAllocHeader->frontPadding;
		const usize overhead = padding + pAllocHeader->backPadding;
		const usize allocSize = overhead + mem.Size();

		u8* pCur = ptr - padding;
		FreeHeader* pCurHeader = reinterpret_cast<FreeHeader*>(pCur);
		pCurHeader->size = allocSize;

		u8* pPrev = nullptr;
		u8* pNext = m_head;
		while (pNext && pNext < pCur)
		{
			pPrev = pNext;
			FreeHeader* pNextHeader = reinterpret_cast<FreeHeader*>(pNext);
			pNext = pNextHeader->next;
		}

		pCurHeader->next = pNext;

		if (pPrev)
		{
			FreeHeader* pPrevHeader = reinterpret_cast<FreeHeader*>(pPrev);
			pPrevHeader->next = pCur;
		}
		else
		{
			m_head = pCur;
		}

		Coalesce(pPrev, pCur, pNext);

#if ENABLE_ALLOC_STATS
		m_stats.RemoveAlloc(mem.Size(), overhead, mem.IsBackingMem());
#endif
	}

	template<usize Size>
	auto FreeListAllocator<Size>::AllocFirst(usize size, u16 align, bool isBacking) noexcept -> u8*
	{
		u8* prevHead = nullptr;

		const usize mask = align - 1;

		while (m_head)
		{
			FreeHeader* pHeader = reinterpret_cast<FreeHeader*>(m_head);

			u8* pAllocStart = m_head + sizeof(AllocHeader);
			const usize offset = usize(pAllocStart) & mask;
			usize padding = sizeof(AllocHeader) + (offset == 0 ? 0 : align - offset);
			const usize paddedSize = size + padding;
			u8* ptr = m_head + padding;

			if  (paddedSize < pHeader->size)
			{
				const usize newHeaderSize = pHeader->size - paddedSize;

				u8* newLoc;
				if (newHeaderSize > sizeof(FreeHeader))
				{
					u8* next = pHeader->next;
					newLoc = m_head + paddedSize;

					FreeHeader* pNewHeader = reinterpret_cast<FreeHeader*>(newLoc);
					pNewHeader->next = next;
					pNewHeader->size = newHeaderSize;

					AllocHeader* pAllocHeader = reinterpret_cast<AllocHeader*>(ptr - sizeof(AllocHeader));
					pAllocHeader->frontPadding = u16(padding);
					pAllocHeader->backPadding = 0;
				}
				else
				{
					newLoc = pHeader->next;

					AllocHeader* pAllocHeader = reinterpret_cast<AllocHeader*>(ptr - sizeof(AllocHeader));
					pAllocHeader->frontPadding = u16(padding);
					pAllocHeader->backPadding = u16(newHeaderSize);

#if ENABLE_ALLOC_STATS
					padding += newHeaderSize;
#endif
				}

				if (!prevHead)
				{
					m_head = newLoc;
				}
				else
				{
					FreeHeader* pPrevHeader = reinterpret_cast<FreeHeader*>(prevHead);
					pPrevHeader->next = newLoc;
				}

#if ENABLE_ALLOC_STATS
				m_stats.AddAlloc(size, padding, isBacking);
#endif

				return ptr;
			}

			prevHead = m_head;
			m_head = pHeader->next;
		}
		return nullptr;
	}

	template<usize Size>
	auto FreeListAllocator<Size>::Coalesce(u8* pPrev, u8* pCur, u8* pNext) noexcept -> void
	{
		FreeHeader* pPrevHeader = reinterpret_cast<FreeHeader*>(pPrev);
		FreeHeader* pCurHeader = reinterpret_cast<FreeHeader*>(pCur);

		if (pCur + pCurHeader->size == pNext)
		{
			FreeHeader* pNextHeader = reinterpret_cast<FreeHeader*>(pNext);
			pCurHeader->size += pNextHeader->size;
			pCurHeader->next = pNextHeader->next;
		}

		if (pPrevHeader && pPrev + pPrevHeader->size == pCur)
		{
			pPrevHeader->size += pCurHeader->size;
			pPrevHeader->next = pCurHeader->next;
		}
	}
}
