#pragma once
#include "core/MinInclude.h"
#include "MemRef.h"

namespace Core
{
	/**
	 * Copy a region of memory from one location to another
	 * \param[in] dst Pointer to destination
	 * \param[in] src Pointer to source
	 * \param[in] numBytes Number of bytes to copy
	 * \note Regions cannot overlap
	 */
	auto MemCpy(void* dst, const void* src, usize numBytes) noexcept -> void;
	/**
	 * Copy the contents from one element to another
	 * \tparam T Type to copy
	 * \param[in] dst Destination
	 * \param[in] src Source
	 */
	template<typename T>
	auto MemCpy(T& dst, const T& src) noexcept -> void;
	/**
	 * Move a region of memory from one location to another
	 * \param[in] dst Pointer to destination
	 * \param[in] src Pointer to source
	 * \param[in] numBytes Number of bytes to copy
	 */
	auto MemMove(void* dst, void* src, usize numBytes) noexcept -> void;
	/**
	 * Move a region of memory from one location to another in a MemRef
	 * \param mem MemRef to move memory in
	 * \param[in] dst Destination offset
	 * \param[in] src Source offset
	 * \param[in] numElems Number of elements to copy
	 */
	template<typename T>
	auto MemMove(MemRef<T>& mem, usize dst, usize src, usize numElems) noexcept -> void;
	/**
	 * Set the value of a region of memory to 'val'
	 * \param[in] ptr Pointer to memory
	 * \param[in] val Value to set memory to
	 * \param[in] numBytes Number of bytes to set
	 */
	auto MemSet(void* ptr, u8 val, usize numBytes) noexcept -> void;
	/**
	 * Set the content of a MeMRef to 'val'
	 * \param[in] mem MemRef to set content for
	 * \param[in] val Value to set memory to
	 */
	template<typename T>
	auto MemSet(MemRef<T>& mem, u8 val) noexcept -> void;
	/**
	 * Clear a region of memory (set to 0)
	 * \param[in] ptr Pointer to memory
	 * \param[in] numBytes Number of numElems to clear
	 */
	auto MemClear(void* ptr, usize numBytes) noexcept -> void;
	/**
	 * Clears the content of a MemRef
	 * \tparam T Type of value to clear
	 * \param[in] mem MemRef to clear content for
	 */
	template<typename T>
	auto MemClear(MemRef<T>& mem) noexcept -> void;
	/**
	 * Clears an object
	 * \tparam T Type of value to clear
	 * \param[in] val Value to clear
	 */
	template<typename T>
	auto MemClearData(T& val) noexcept -> void;

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