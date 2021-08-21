#include "BuddyAllocator.h"

namespace Core::Alloc
{
	BuddyAllocator::BuddyAllocator(IAllocator* pBackingAlloc, usize size, u8 maxSubDivisions)
		: m_mem(nullptr)
		, m_size(size)
		, m_maxDivisions(maxSubDivisions)
	{
		ASSERT(IsPowOf2(size), "A buddy allocator requires a size that is a power of 2");
		const usize minAlign = size >> maxSubDivisions;
		const u16 align = minAlign > 0x8000 ? 0x8000 : u16(minAlign);
		m_managmentSize = CalculateManagementSize(maxSubDivisions, size);
		m_mem = pBackingAlloc->Allocate<u8>(size + m_managmentSize, align, true);
		MemClear(m_mem.Ptr(), m_managmentSize);
	}

	auto BuddyAllocator::AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8>
	{
		u8* pManagementInfo = m_mem.Ptr();
		if (pManagementInfo[0] & 0x80) UNLIKELY
			return MemRef<u8>{ nullptr };

		const auto [sizeClass, sizeClassBlockSize] = CalculateSizeClassAndBlockSize(size);

		Threading::Lock lock{ m_mutex };

		const usize divIdx = GetIdx(pManagementInfo, sizeClass);
		if (divIdx == usize(-1))
			return MemRef<u8>{ nullptr };

		const usize offset = divIdx + 1 - (1ull << Log2(divIdx + 1));
		const usize memOffset = offset * sizeClassBlockSize;

		Mark(pManagementInfo, divIdx);

#if ENABLE_ALLOC_STATS
		const usize overHead = sizeClassBlockSize - size;
		m_stats.AddAlloc(size, overHead, isBacking);
#endif

		return MemRef<u8>{ memOffset, this, Log2(align), size, isBacking };
	}

	auto BuddyAllocator::DeallocateRaw(MemRef<u8>&& mem) noexcept -> void
	{
		u8* pManagementInfo = m_mem.Ptr();

		const auto [sizeClass, sizeClassBlockSize] = CalculateSizeClassAndBlockSize(mem.Size());

		const usize offset = mem.GetRawHandle() / sizeClassBlockSize;
		const usize divIdx = sizeClass == 0 ? 0 : (1ull << sizeClass) - 1 + offset;

		Threading::Lock lock{ m_mutex };

		Unmark(pManagementInfo, divIdx);

#if ENABLE_ALLOC_STATS
		const usize overHead = sizeClassBlockSize - mem.Size();
		m_stats.AddAlloc(mem.Size(), overHead, mem.IsBackingMem());
#endif
	}

	auto BuddyAllocator::TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8*
	{
		return m_mem.Ptr() + m_managmentSize + mem.GetRawHandle();
	}

	auto BuddyAllocator::CalculateManagementSize(u8 numDivisions, usize size) noexcept -> usize
	{
		const usize numFlags = (1ull << (numDivisions + 1)) - 1;
		const usize numManagmentBytes = (numFlags + 3) / 4;
		const usize smallestBlockSize = size << numDivisions;
		const usize numBlocks = (numManagmentBytes + smallestBlockSize - 1) / smallestBlockSize;
		return numBlocks * smallestBlockSize;
	}

	auto BuddyAllocator::CalculateSizeClassAndBlockSize(usize size) noexcept -> Tuple<usize, usize>
	{
		const usize smallestBlockSize = m_size >> m_maxDivisions;
		const usize sizeClass = m_maxDivisions - Log2((size + smallestBlockSize - 1) / smallestBlockSize);
		const usize sizeClassBlockSize = m_size >> sizeClass;
		return { sizeClass, sizeClassBlockSize };
	}

	auto BuddyAllocator::GetIdx(u8* pManagementInfo, usize neededClass) noexcept -> usize
	{
		u8 flag = GetDivFlag(pManagementInfo, 0);
		if (neededClass == 0)
		{
			return flag == FreeFlag ? 0 : usize(-1);
		}
		if (flag & UsedFlag)
			return usize(-1);
		
		usize idx = GetSubIdx(pManagementInfo, neededClass, 1, 1);
		if (idx != usize(-1))
			return idx;

		return GetSubIdx(pManagementInfo, neededClass, 1, 2);
	}

	auto BuddyAllocator::GetSubIdx(u8* pManagementInfo, usize neededClass, usize curClass, usize curDivIdx) noexcept -> usize
	{
		bool isLastStep = neededClass == curClass;
		u8 flag = GetDivFlag(pManagementInfo, curDivIdx);
		if (isLastStep && flag == FreeFlag)
			return curDivIdx;
		if (flag & UsedFlag)
			return usize(-1);

		usize newDivIdx = curDivIdx * 2 + 1;
		flag = GetDivFlag(pManagementInfo, curDivIdx);
		if (!isLastStep && (flag == FreeFlag || flag == SplitFlag))
		{
			usize idx = GetSubIdx(pManagementInfo, neededClass, curClass + 1, newDivIdx);
			if (idx != usize(-1))
				return idx;
		}

		++newDivIdx;
		flag = GetDivFlag(pManagementInfo, newDivIdx);
		if (isLastStep && flag == FreeFlag)
			return curDivIdx + 1;

		if (!isLastStep && (flag == FreeFlag || flag == SplitFlag))
		{
			usize idx = GetSubIdx(pManagementInfo, neededClass, curClass + 1, newDivIdx);
			if (idx != usize(-1))
				return idx;
		}

		return usize(-1);
	}

	auto BuddyAllocator::GetDivFlag(u8* pManagementInfo, usize divIdx) noexcept -> u8
	{
		usize bitIdx = (3 - (divIdx & 0x3)) * 2;
		usize byteIdx = divIdx / 4;
		return (pManagementInfo[byteIdx] >> bitIdx) & 0x3;
	}

	auto BuddyAllocator::SetDivFlag(u8* pManagementInfo, usize divIdx, u8 flag) noexcept -> void
	{
		usize bitIdx = (3 - (divIdx & 0x3)) * 2;
		usize byteIdx = divIdx / 4;

		pManagementInfo[byteIdx] &= ~(0x3 << bitIdx);
		pManagementInfo[byteIdx] |= flag << bitIdx;
	}

	auto BuddyAllocator::Mark(u8* pManagementInfo, usize divIdx) noexcept -> void
	{
		SetDivFlag(pManagementInfo, divIdx, UsedFlag);
		u8 ownFlag = UsedFlag;
		while (divIdx)
		{
			u8 buddyFlag = GetDivFlag(pManagementInfo, divIdx & 0x1 ? divIdx + 1 : divIdx - 1);
			ownFlag = SplitFlag | !!((ownFlag & buddyFlag) & UsedFlag) * UsedFlag;

			const usize layerStart2 = (1ull << Log2(divIdx + 1));
			const usize layerStart = (layerStart2 >> 1) - 1;
			const usize offset = divIdx - (layerStart2 - 1);
			divIdx = layerStart + offset / 2;
			SetDivFlag(pManagementInfo, divIdx, ownFlag);
		}
	}

	auto BuddyAllocator::Unmark(u8* pManagementInfo, usize divIdx) noexcept -> void
	{
		SetDivFlag(pManagementInfo, divIdx, FreeFlag);
		u8 ownFlag = FreeFlag;
		while (divIdx)
		{
			u8 buddyFlag = GetDivFlag(pManagementInfo, divIdx & 0x1 ? divIdx + 1 : divIdx - 1);
			ownFlag = ownFlag | !!(buddyFlag & (SplitFlag | UsedFlag)) * SplitFlag;

			const usize layerStart2 = (1ull << Log2(divIdx + 1));
			const usize layerStart = (layerStart2 >> 1) - 1;
			const usize offset = divIdx - (layerStart2 - 1);
			divIdx = layerStart + offset / 2;
			SetDivFlag(pManagementInfo, divIdx, ownFlag);
		}
	}
}
