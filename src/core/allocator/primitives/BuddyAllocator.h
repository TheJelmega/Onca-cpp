#pragma once
#include "core/allocator/IAllocator.h"

namespace Core::Alloc
{
	/**
	 * An allocator that allocates memory from subdivided blocks
	 */
	class CORE_API BuddyAllocator : public IAllocator
	{
	private:
		static constexpr u8 FreeFlag  = 0x00;
		static constexpr u8 SplitFlag = 0x01;
		static constexpr u8 UsedFlag  = 0b10;

	public:
		/**
		 * Create a buddy allocator
		 * \param pBackingAlloc Allocator to create the underlying memory block
		 * \param size Size of full memory block
		 * \param maxSubDivisions Maximum number of subdivision
		 */
		BuddyAllocator(IAllocator* pBackingAlloc, usize size, u8 maxSubDivisions);
		BuddyAllocator(BuddyAllocator&&) = default;

	protected:
		auto AllocateRaw(usize size, u16 align, bool isBacking) noexcept -> MemRef<u8> override;
		auto DeallocateRaw(MemRef<u8>&& mem) noexcept -> void override;
		auto TranslateToPtrInternal(const MemRef<u8>& mem) noexcept -> u8* override;

	private:
		auto CalculateManagementSize(u8 numDivisions) noexcept -> usize;

		auto CalculateSizeClassAndBlockSize(usize size) noexcept -> Tuple<usize, usize>;

		auto GetSubIdx(u8* pManagementInfo, usize neededClass, usize curClass, usize curDivIdx) noexcept -> usize;

		auto GetDivFlag(u8* pManagementInfo, usize divIdx) noexcept -> u8;
		auto SetDivFlag(u8* pManagementInfo, usize divIdx, u8 flag) noexcept -> void;

		auto Mark(u8* pManagementInfo, usize divIdx) noexcept -> void;
		auto Unmark(u8* pManagementInfo, usize divIdx) noexcept -> void;

		MemRef<u8> m_mem;
		usize m_size;
		usize m_maxDivisions;
		usize m_managmentSize;
		Threading::Mutex m_mutex;
	};
}