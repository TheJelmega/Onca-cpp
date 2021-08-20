#pragma once
#include "core/allocator/IAllocator.h"

namespace Core::Alloc
{
	/**
	 * \brief An allocator that allocates memory from subdivided blocks
	 *
	 * A buddy allocator exist of a big block of memory that can be subdivided into separate smaller chunks.
	 * When memory is allocated, the smallest sized subdivision is found that fits the memory, which is then marked as used,
	 * after which the higher subdivision are set to split.
	 * Maximum alignment is determined by the smallest block size.
	 *
	 * Below is an example of a buddy allocator with 4 subdivisions and their indices
	 *
	 *                                                                                   div idx
	 * +-------------------------------------------------------------------------------+
	 * |                                       0                                       | 0
	 * +---------------------------------------+---------------------------------------+
	 * |                   1                   |                   2                   | 1
	 * +-------------------+-------------------+-------------------+-------------------+
	 * |         3         |         4         |         5         |         6         | 2
	 * +---------+---------+---------+---------+---------+---------=---------+---------+
	 * |    7    |    8    |    9    |   10    |   11    |   12    |   13    |   14    | 3
	 * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
	 * | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 4
	 * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
	 *
	 *
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
		/**
		 * Get the number of bytes needed to managed the divisions
		 * \param numDivisions Number of division to managed
		 * \param size Size of the managed memory
		 * \return Number of bytes needed to manage the divisions
		 */
		auto CalculateManagementSize(u8 numDivisions, usize size) noexcept -> usize;

		/**
		 * Calculate the size class and block size for an allocation of a certain size
		 * \param size Size of allocation
		 * \return Size class and block size for allocation
		 */
		auto CalculateSizeClassAndBlockSize(usize size) noexcept -> Tuple<usize, usize>;

		/**
		 * Get the index of a free division fitting the requested size class
		 * \param pManagementInfo Pointer to management info
		 * \param neededClass Size class needed for allocation
		 * \param curClass Current size class to work in
		 * \param curDivIdx CUrrent division index
		 * \return Index of a free division, or usize(-1) if no free division is found
		 */
		auto GetSubIdx(u8* pManagementInfo, usize neededClass, usize curClass, usize curDivIdx) noexcept -> usize;

		/**
		 * Get the flag for the division at a certain index
		 * \param pManagementInfo Pointer to management info
		 * \param divIdx Division index
		 * \return Flag of the division
		 */
		auto GetDivFlag(u8* pManagementInfo, usize divIdx) noexcept -> u8;
		/**
		 * Set the flag for the division at a certain index
		 * \param pManagementInfo Pointer to management info
		 * \param divIdx Division index
		 * \param flag Flag to se
		 */
		auto SetDivFlag(u8* pManagementInfo, usize divIdx, u8 flag) noexcept -> void;

		/**
		 * Mark a division as used and update the parent divisions
		 * \param pManagementInfo Pointer to management info
		 * \param divIdx Index to mark
		 */
		auto Mark(u8* pManagementInfo, usize divIdx) noexcept -> void;
		/**
		 * Unmark a division as used and update the parent divisions
		 * \param pManagementInfo Pointer to management info
		 * \param divIdx Index to unmark
		 */
		auto Unmark(u8* pManagementInfo, usize divIdx) noexcept -> void;

		MemRef<u8>       m_mem;           ///< Managed memory
		usize            m_size;          ///< Size of div 0
		usize            m_maxDivisions;  ///< Maximum levels of divisions
		usize            m_managmentSize; ///< Memory used for management
		Threading::Mutex m_mutex;         ///< Mutex to guard division flag modifications
	};
}