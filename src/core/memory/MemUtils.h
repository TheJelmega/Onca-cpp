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
	 * Set the value of a region of memory to 'val'
	 * \param[in] ptr Pointer to memory
	 * \param[in] val Value to set memory to
	 * \param[in] numBytes Number of bytes to set
	 */
	auto MemSet(void* ptr, u8 val, usize numBytes) noexcept -> void;
	/**
	 * Clear a region of memory (set to 0)
	 * \param[in] ptr Pointer to memory
	 * \param[in] numBytes Number of bytes to clear
	 */
	auto MemClear(void* ptr, usize numBytes) noexcept -> void;
	/**
	 * Clears an object
	 * \tparam T Type of value to clear
	 * \param[in] val Value to clear
	 */
	template<typename T>
	auto MemClear(T& val) noexcept -> void;
	
}

#include "MemUtils.inl"