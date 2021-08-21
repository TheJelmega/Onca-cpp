#pragma once
#include "core/MinInclude.h"

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
	 * Copy a region of memory from one location to another
	 * \param[in] dst Destination MemRef
	 * \param[in] dstOffset Offset in destination MemRef
	 * \param[in] src Source MemRef
	 * \param[in] srcOffset Offset in source MemRef
	 * \param[in] numBytes Number of bytes to copy
	 * \note Regions cannot overlap
	 */
	auto MemCpy(MemRef<u8>& dst, usize dstOffset, const MemRef<u8>& src, usize srcOffset, usize numBytes) noexcept -> void;
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
	 * \param[in] numBytes Number of bytes to copy
	 */
	template<typename T>
	auto MemMove(MemRef<T>& mem, usize dst, usize src, usize numBytes) noexcept -> void;
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
	 * \param[in] numBytes Number of bytes to clear
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
	
}

#include "MemUtils.inl"