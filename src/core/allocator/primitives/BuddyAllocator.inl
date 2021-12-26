#pragma once
#if __RESHARPER__
#include "BuddyAllocator.h"
#endif

#include "core/utils/Endianess.h"

namespace Core::Alloc
{
	template<usize Size, u8 MaxSubDivisions>
	BuddyAllocator<Size, MaxSubDivisions>::BuddyAllocator(IAllocator* pBackingAlloc) noexcept
		: m_mem(nullptr)
	{
		STATIC_ASSERT(Math::IsPowOf2(Size), "A buddy allocator requires a size that is a power of 2");
		STATIC_ASSERT(SmallestBlockSize, "MaxSubDivision is too high");
		const u16 align = SmallestBlockSize > 0x8000 ? 0x8000 : u16(SmallestBlockSize);
		m_mem = pBackingAlloc->Allocate<u8>(Size + ManagementSize, align, true);
		MemClear(m_mem.Ptr(), ManagementSize);
	}

	template <usize Size, u8 MaxSubDivisions>
	BuddyAllocator<Size, MaxSubDivisions>::~BuddyAllocator() noexcept
	{
		m_mem.Dealloc();
	}

	template<usize Size, u8 MaxSubDivisions>
	auto BuddyAllocator<Size, MaxSubDivisions>::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		Threading::Lock lock{ m_mutex };

		u8* pManagementInfo = m_mem.Ptr();
		if (pManagementInfo[0] & 0x80) UNLIKELY
			return nullptr;

		const auto [sizeClass, sizeClassBlockSize] = CalculateSizeClassAndBlockSize(size);

		const usize divIdx = GetIdx(pManagementInfo, sizeClass);
		if (divIdx == usize(-1))
			return nullptr;

		const usize offset = divIdx + 1 - (1ull << Math::Log2(divIdx + 1));
		const usize memOffset = offset * sizeClassBlockSize;

		Mark(pManagementInfo, divIdx);

#if ENABLE_ALLOC_STATS
		const usize overHead = sizeClassBlockSize - size;
		m_stats.AddAlloc(size, overHead, isBacking);
#endif

		return MemRef<u8>{ m_mem.Ptr() + ManagementSize + memOffset, this, Math::Log2(align), size, isBacking };
	}

	template<usize Size, u8 MaxSubDivisions>
	auto BuddyAllocator<Size, MaxSubDivisions>::DeallocateRaw(MemRef<u8>&& mem) noexcept -> void
	{
		const auto [sizeClass, sizeClassBlockSize] = CalculateSizeClassAndBlockSize(mem.Size());

		const usize offset = usize(mem.Ptr() - ManagementSize - m_mem.Ptr()) / sizeClassBlockSize;
		const usize divIdx = sizeClass == 0 ? 0 : (1ull << sizeClass) - 1 + offset;

		Threading::Lock lock{ m_mutex };

		u8* pManagementInfo = m_mem.Ptr();
		Unmark(pManagementInfo, divIdx);

#if ENABLE_ALLOC_STATS
		const usize overHead = sizeClassBlockSize - mem.Size();
		m_stats.AddAlloc(mem.Size(), overHead, mem.IsBackingMem());
#endif
	}
	
	template<usize Size, u8 MaxSubDivisions>
	auto BuddyAllocator<Size, MaxSubDivisions>::CalculateSizeClassAndBlockSize(usize size) noexcept -> Tuple<usize, usize>
	{
		const usize sizeClass = MaxSubDivisions - Math::Log2((size + SmallestBlockSize - 1) / SmallestBlockSize);
		const usize sizeClassBlockSize = Size >> sizeClass;
		return { sizeClass, sizeClassBlockSize };
	}

	template<usize Size, u8 MaxSubDivisions>
	auto BuddyAllocator<Size, MaxSubDivisions>::GetIdx(u8* pManagementInfo, usize neededClass) noexcept -> usize
	{
		u8 flag = pManagementInfo[0] >> 6;
		if (neededClass == 0)
		{
			return flag == FreeFlag ? 0 : usize(-1);
		}
		if (flag & UsedFlag)
			return usize(-1);

		auto [lFlag, rflag] = GetDivFlags(pManagementInfo, 1);
		if (neededClass == 1)
		{
			if (lFlag == FreeFlag)
				return 1;
			if (rflag == FreeFlag)
				return 2;
		}

		if (!(lFlag & UsedFlag))
			return GetSubIdx(pManagementInfo, neededClass, 1, 1);
		return GetSubIdx(pManagementInfo, neededClass, 1, 2);
	}

	template<usize Size, u8 MaxSubDivisions>
	auto BuddyAllocator<Size, MaxSubDivisions>::GetSubIdx(u8* pManagementInfo, usize neededClass, usize curClass, usize curDivIdx) noexcept -> usize
	{
		usize newDivIdx = curDivIdx * 2 + 1;
		auto [lFlag, rflag] = GetDivFlags(pManagementInfo, newDivIdx);

		if (neededClass == curClass + 1)
		{
			if (lFlag == FreeFlag)
				return newDivIdx;
			if (rflag == FreeFlag)
				return newDivIdx + 1;
		}

		if (!(lFlag & UsedFlag))
			return GetSubIdx(pManagementInfo, neededClass, curClass + 1, newDivIdx);
		return GetSubIdx(pManagementInfo, neededClass, curClass + 1, newDivIdx + 1);
	}

	template<usize Size, u8 MaxSubDivisions>
	auto BuddyAllocator<Size, MaxSubDivisions>::GetDivFlag(u8* pManagementInfo, usize divIdx) noexcept -> u8
	{
		usize bitIdx = (3 - (divIdx & 0x3)) * 2;
		usize byteIdx = divIdx / 4;
		return (pManagementInfo[byteIdx] >> bitIdx) & 0x3;
	}

	template<usize Size, u8 MaxSubDivisions>
	auto BuddyAllocator<Size, MaxSubDivisions>::GetDivFlags(u8* pManagementInfo, usize divIdx) noexcept -> Tuple<u8, u8>
	{
		usize bitIdx = (6 - (divIdx & 0x3)) * 2;
		usize byteIdx = divIdx / 4;

		u16 data = ToBigEndian(*reinterpret_cast<u16*>(pManagementInfo + byteIdx));
		u16 mask = 0x0F << bitIdx;
		u16 flags = (data & mask);
		flags >>= bitIdx;

		return { u8(flags >> 2), u8(flags & 0x03) };
	}

	template<usize Size, u8 MaxSubDivisions>
	auto BuddyAllocator<Size, MaxSubDivisions>::SetDivFlag(u8* pManagementInfo, usize divIdx, u8 flag) noexcept -> void
	{
		usize bitIdx = (3 - (divIdx & 0x3)) * 2;
		usize byteIdx = divIdx / 4;

		pManagementInfo[byteIdx] &= ~(0x3 << bitIdx);
		pManagementInfo[byteIdx] |= flag << bitIdx;
	}

	template<usize Size, u8 MaxSubDivisions>
	auto BuddyAllocator<Size, MaxSubDivisions>::Mark(u8* pManagementInfo, usize divIdx) noexcept -> void
	{
		SetDivFlag(pManagementInfo, divIdx, UsedFlag);
		u8 ownFlag = UsedFlag;
		while (divIdx)
		{
			u8 buddyFlag = GetDivFlag(pManagementInfo, divIdx & 0x1 ? divIdx + 1 : divIdx - 1);
			ownFlag = SplitFlag | ((ownFlag & buddyFlag) & UsedFlag);

			const usize layerStart2 = (1ull << Math::Log2(divIdx + 1));
			const usize layerStart = (layerStart2 >> 1) - 1;
			const usize offset = divIdx - (layerStart2 - 1);
			divIdx = layerStart + offset / 2;
			SetDivFlag(pManagementInfo, divIdx, ownFlag);
		}
	}

	template<usize Size, u8 MaxSubDivisions>
	auto BuddyAllocator<Size, MaxSubDivisions>::Unmark(u8* pManagementInfo, usize divIdx) noexcept -> void
	{
		SetDivFlag(pManagementInfo, divIdx, FreeFlag);
		u8 ownFlag = FreeFlag;
		while (divIdx)
		{
			u8 buddyFlag = GetDivFlag(pManagementInfo, divIdx & 0x1 ? divIdx + 1 : divIdx - 1);
			ownFlag = ownFlag | !!(buddyFlag & (SplitFlag | UsedFlag)) * SplitFlag;

			const usize layerStart2 = (1ull << Math::Log2(divIdx + 1));
			const usize layerStart = (layerStart2 >> 1) - 1;
			const usize offset = divIdx - (layerStart2 - 1);
			divIdx = layerStart + offset / 2;
			SetDivFlag(pManagementInfo, divIdx, ownFlag);
		}
	}
}
