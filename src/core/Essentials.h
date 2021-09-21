#pragma once
#include "Config.h"
#include "Traits.h"

namespace Core
{
	// Reimplementation since we cannot do function aliases
	/**
	 * Move a value
	 * \tparam T Type of value
	 * \param moved Value to move
	 * \return Moved value
	 */
	template<typename T>
	NO_DISCARD("Cannot discard result of Move")
	constexpr auto Move(T&& moved) noexcept -> RemoveReference<T>&&;

	// Reimplementation since we cannot do function aliases
	/**
	 * Forward a value
	 * \tparam T Type of value
	 * \param arg Value to forward
	 * \return Forward value
	 */
	template<typename T>
	NO_DISCARD("Cannot discard result of Forward")
	constexpr auto Forward(RemoveReference<T>& arg) noexcept -> T&&;
	// Reimplementation since we cannot do function aliases
	/**
	 * Forward a value
	 * \tparam T Type of value
	 * \param arg Value to forward
	 * \return Forward value
	 */
	template<typename T>
	NO_DISCARD("Cannot discard result of Forward")
	constexpr auto Forward(RemoveReference<T>&& arg) noexcept -> T&&;
}
