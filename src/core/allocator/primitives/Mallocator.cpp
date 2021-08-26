#include "Mallocator.h"
#include <cstdlib>

namespace Core::Alloc
{
	Mallocator::~Mallocator() noexcept
	{
	}

	auto Mallocator::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		const usize allocSize = size + align;
		const usize mask = align - 1;
		
		u8* ptr = static_cast<u8*>(malloc(allocSize));
		u8* alignedPtr = reinterpret_cast<u8*>(usize(ptr + mask) & ~mask);
		isize diff = alignedPtr - ptr;
		if (diff == 0)
		{
			alignedPtr += align;
			diff = align;
		}

		if (diff <= 0x7F)
		{
			alignedPtr[-1] = u8(diff);
		}
		else
		{
			alignedPtr[-1] = (u8(diff) & 0x7F) | 0x80;
			alignedPtr[-2] = u8(diff >> 7);
		}

#if ENABLE_ALLOC_STATS
		m_stats.AddAlloc(size, align, isBacking);
#endif
		
		return { reinterpret_cast<usize>(alignedPtr), this, Log2(align), size, isBacking };
	}

	auto Mallocator::DeallocateRaw(MemRef<u8>&& mem) noexcept -> void
	{
		u8* ptr = reinterpret_cast<u8*>(mem.GetRawHandle());
		usize offset = ptr[-1];
		if (offset & 0x80)
		{
			offset &= 0x7F;
			offset |= usize(ptr[-2]) << 7;
		}

		free(ptr - offset);

#if ENABLE_ALLOC_STATS
		m_stats.RemoveAlloc(mem.Size(), mem.Align(), mem.IsBackingMem());
#endif
	}

	auto Mallocator::TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8*
	{
		return reinterpret_cast<u8*>(mem.GetRawHandle());
	}
}
