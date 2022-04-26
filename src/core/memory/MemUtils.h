#pragma once
#include "core/MinInclude.h"
#include "MemRef.h"

namespace Onca
{
	/**
	 * Copy a region of memory from one location to another
	 * \param[in] dst Pointer to destination
	 * \param[in] src Pointer to source
	 * \param[in] numBytes Number of bytes to copy
	 * \note Regions cannot overlap
	 */
	template<typename T>
	constexpr void MemCpy(T* dst, const T* src, usize numBytes) noexcept;
	/**
	 * Copy a region of memory from one location to another
	 * \param[in] dst Pointer to destination
	 * \param[in] src Pointer to source
	 * \param[in] numBytes Number of bytes to copy
	 * \note Regions cannot overlap
	 */
	void MemCpy(void* dst, const void* src, usize numBytes) noexcept;
	/**
	 * Copy the contents from one element to another
	 * \tparam T Type to copy
	 * \param[in] dst Destination
	 * \param[in] src Source
	 */
	template<typename T>
	void MemCpy(T& dst, const T& src) noexcept;
	/**
	 * Move a region of memory from one location to another
	 * \param[in] dst Pointer to destination
	 * \param[in] src Pointer to source
	 * \param[in] numBytes Number of bytes to copy
	 */
	void MemMove(void* dst, void* src, usize numBytes) noexcept;
	/**
	 * Move a region of memory from one location to another in a MemRef
	 * \param mem MemRef to move memory in
	 * \param[in] dst Destination offset
	 * \param[in] src Source offset
	 * \param[in] numElems Number of elements to copy
	 */
	template<typename T>
	void MemMove(MemRef<T>& mem, usize dst, usize src, usize numElems) noexcept;
	/**
	 * Set the value of a region of memory to 'val'
	 * \param[in] ptr Pointer to memory
	 * \param[in] val Value to set memory to
	 * \param[in] numBytes Number of bytes to set
	 */
	void MemSet(void* ptr, u8 val, usize numBytes) noexcept;
	/**
	 * Set the content of a MeMRef to 'val'
	 * \param[in] mem MemRef to set content for
	 * \param[in] val Value to set memory to
	 */
	template<typename T>
	void MemSet(MemRef<T>& mem, u8 val) noexcept;
	/**
	 * Clear a region of memory (set to 0)
	 * \param[in] ptr Pointer to memory
	 * \param[in] numBytes Number of numElems to clear
	 */
	void MemClear(void* ptr, usize numBytes) noexcept;
	/**
	 * Clears the content of a MemRef
	 * \tparam T Type of value to clear
	 * \param[in] mem MemRef to clear content for
	 */
	template<typename T>
	void MemClear(MemRef<T>& mem) noexcept;
	/**
	 * Clears an object
	 * \tparam T Type of value to clear
	 * \param[in] val Value to clear
	 */
	template<typename T>
	void MemClearData(T& val) noexcept;

	/**
	 * Compare 2 blocks of memory
	 * \param pA Pointer to first memory block
	 * \param pB Pointer to second memory block
	 * \param numBytes Number of bytes to compare
	 * \return 0 if blocks match, -1 if the first difference has a smaller byte in a, 1 otherwise
	 */
	auto MemCmp(const void* pA, const void* pB, usize numBytes) noexcept -> i8;
	/**
	 * Compare 2 blocks of memory
	 * \param a First MemRef
	 * \param b Second MemRef
	 * \return 0 if blocks match, -1 if the first difference has a smaller byte in a, 1 otherwise
	 */
	template<typename T>
	auto MemCmp(MemRef<T>& a, MemRef<T>& b) noexcept -> i8;
	
}

#include "MemUtils.inl"