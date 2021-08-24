#include "FreeListAllocator.h"

namespace Core::Alloc
{
	template<usize Size>
	FreeListAllocator<Size>::FreeListAllocator(IAllocator* pBackingAllocator) noexcept
		: m_mem(pBackingAllocator->Allocate<u8>(Size, sizeof(usize), true))
		, m_head(0)
	{
		STATIC_ASSERT(Size > sizeof(FreeHeader), "Size needs to be larger than 16 bytes");
		FreeHeader* pFreeHeader = reinterpret_cast<FreeHeader*>(m_mem.Ptr());
		pFreeHeader->next = usize(-1);
		pFreeHeader->size = Size;
	}

	template <usize Size>
	FreeListAllocator<Size>::~FreeListAllocator() noexcept
	{
		m_mem.Dealloc();
	}

	template<usize Size>
	auto FreeListAllocator<Size>::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		const usize loc = AllocFirst(size, align, isBacking);
		if (loc == usize(-1))
			return nullptr;
		return { loc, this, Log2(align), size, isBacking };
	}

	template<usize Size>
	auto FreeListAllocator<Size>::DeallocateRaw(MemRef<u8>&& mem) noexcept -> void
	{
		const usize loc = mem.GetRawHandle();
		u8* pBegin = m_mem.Ptr();

		AllocHeader* pAllocHeader = reinterpret_cast<AllocHeader*>(pBegin + loc - sizeof(AllocHeader));
		const usize padding = pAllocHeader->frontPadding;
		const usize overhead = padding + pAllocHeader->backPadding;
		const usize allocSize = overhead + mem.Size();

		const usize curLoc = loc - padding;
		FreeHeader* pCurHeader = reinterpret_cast<FreeHeader*>(pBegin + curLoc);
		pCurHeader->size = allocSize;

		usize prevLoc = usize(-1);
		usize nextLoc = m_head;
		while (nextLoc < curLoc)
		{
			prevLoc = nextLoc;
			FreeHeader* pNextHeader = reinterpret_cast<FreeHeader*>(pBegin + nextLoc);
			nextLoc = pNextHeader->next;
		}

		pCurHeader->next = nextLoc;

		if (prevLoc != usize(-1))
		{
			FreeHeader* pPrevHeader = reinterpret_cast<FreeHeader*>(pBegin + prevLoc);
			pPrevHeader->next = curLoc;
		}
		else
		{
			m_head = curLoc;
		}

		Coalesce(pBegin, prevLoc, curLoc, nextLoc);

#if ENABLE_ALLOC_STATS
		m_stats.RemoveAlloc(mem.Size(), overhead, mem.IsBackingMem());
#endif
	}

	template<usize Size>
	auto FreeListAllocator<Size>::TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8*
	{
		return m_mem.Ptr() + mem.GetRawHandle();
	}

	template<usize Size>
	auto FreeListAllocator<Size>::AllocFirst(usize size, u16 align, bool isBacking) noexcept -> usize
	{
		usize head = m_head;
		usize prevHead = usize(-1);
		u8* pBegin = m_mem.Ptr();

		const usize mask = align - 1;

		while (head != usize(-1))
		{
			FreeHeader* pHeader = reinterpret_cast<FreeHeader*>(pBegin + head);

			const usize allocStart = head + sizeof(AllocHeader);
			const usize offset = allocStart & mask;
			usize padding = sizeof(AllocHeader) + (offset == 0 ? 0 : align - offset);
			const usize paddedSize = size + padding;
			const usize allocLoc = head + padding;

			if  (paddedSize < pHeader->size)
			{
				const usize newHeaderSize = pHeader->size - paddedSize;

				usize newLoc;
				if (newHeaderSize > sizeof(FreeHeader))
				{
					usize next = pHeader->next;
					newLoc = head + paddedSize;

					FreeHeader* pNewHeader = reinterpret_cast<FreeHeader*>(pBegin + newLoc);
					pNewHeader->next = next;
					pNewHeader->size = newHeaderSize;

					AllocHeader* pAllocHeader = reinterpret_cast<AllocHeader*>(pBegin + allocLoc - sizeof(AllocHeader));
					pAllocHeader->frontPadding = u16(padding);
					pAllocHeader->backPadding = 0;
				}
				else
				{
					newLoc = pHeader->next;

					AllocHeader* pAllocHeader = reinterpret_cast<AllocHeader*>(pBegin + allocLoc - sizeof(AllocHeader));
					pAllocHeader->frontPadding = u16(padding);
					pAllocHeader->backPadding = u16(newHeaderSize);

#if ENABLE_ALLOC_STATS
					padding += newHeaderSize;
#endif
				}

				if (prevHead == usize(-1))
				{
					m_head = newLoc;
				}
				else
				{
					FreeHeader* pPrevHeader = reinterpret_cast<FreeHeader*>(pBegin + prevHead);
					pPrevHeader->next = newLoc;
				}

#if ENABLE_ALLOC_STATS
				m_stats.AddAlloc(size, padding, isBacking);
#endif

				return allocLoc;
			}

			prevHead = head;
			head = pHeader->next;
		}
		return usize(-1);
	}

	template<usize Size>
	auto FreeListAllocator<Size>::Coalesce(u8* pBegin, usize prevLoc, usize curLoc, usize nextLoc) noexcept -> void
	{
		FreeHeader* pPrevHeader = prevLoc == usize(-1) ? nullptr : reinterpret_cast<FreeHeader*>(pBegin + prevLoc);
		FreeHeader* pCurHeader = reinterpret_cast<FreeHeader*>(pBegin + curLoc);

		if (curLoc + pCurHeader->size == nextLoc)
		{
			FreeHeader* pNextHeader = reinterpret_cast<FreeHeader*>(pBegin + nextLoc);
			pCurHeader->size += pNextHeader->size;
			pCurHeader->next = pNextHeader->next;
		}

		if (pPrevHeader && prevLoc + pPrevHeader->size == curLoc)
		{
			pPrevHeader->size += pCurHeader->size;
			pPrevHeader->next = pCurHeader->next;
		}
	}
}
