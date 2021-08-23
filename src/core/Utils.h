#pragma once
#include "core/MinInclude.h"
#include "memory/MemRef.h"

namespace Core
{
	template<typename T>
	struct DefaultDeleter
	{
		auto operator()(MemRef<T>&& ref) noexcept -> void;
	};

	// Reimplementation since we cannot do function aliases
	/**
	 * Move a value
	 * \tparam T Type of value
	 * \param moved Value to move
	 * \return Moved value
	 */
	template<typename T>
	NO_DISCARD("Cannot discard result of Move") constexpr auto Move(T&& moved) noexcept -> RemoveReference<T>&&;

	// Reimplementation since we cannot do function aliases
	/**
	 * Forward a value
	 * \tparam T Type of value
	 * \param arg Value to forward
	 * \return Forward value
	 */
	template<typename T>
	NO_DISCARD("Cannot discard result of Forward") constexpr auto Forward(RemoveReference<T>& arg) noexcept -> T&&;
	// Reimplementation since we cannot do function aliases
	/**
	 * Forward a value
	 * \tparam T Type of value
	 * \param arg Value to forward
	 * \return Forward value
	 */
	template<typename T>
	NO_DISCARD("Cannot discard result of Forward") constexpr auto Forward(RemoveReference<T>&& arg) noexcept -> T&&;

	template<ForwardIterator T>
	auto CountElems(const T& begin, const T& end) noexcept -> usize;

#define ConcatTuple(...) std::tuple_cat(__VA_ARGS__)
#define ConstructFromTuple std::make_from_tuple
	
}

#include "Utils.inl"