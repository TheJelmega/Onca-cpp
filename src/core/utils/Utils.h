#pragma once
#include "core/MinInclude.h"
#include "memory/MemRef.h"

namespace Core
{
	struct Empty
	{};

	template<usize Idx, typename T>
	struct TupleElementHelper;

	template<usize Idx, typename T>
	using TupleElement = typename TupleElementHelper<Idx, T>::Type;

	template<typename T>
	struct DefaultDeleter
	{
		auto operator()(MemRef<T>&& ref) noexcept -> void;
	};

	template<typename T>
		requires EqualComparable<T, T>
	struct DefaultEqualComparator
	{
		auto operator()(const T& t0, const T& t1) const noexcept -> bool;
	};

	template<typename T>
		requires OrderedComparable<T>
	struct DefaultComparator
	{
		auto operator()(const T& t0, const T& t1) const noexcept -> i8;
	};

	template<typename T>
	struct Hash
	{
		auto operator()(const T& t) const noexcept -> u64;
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

	template<MoveAssignable T>
	auto Swap(T& a, T& b) noexcept -> void;
}

#include "Utils.inl"