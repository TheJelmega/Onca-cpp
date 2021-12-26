#pragma once
#include "core/MinInclude.h"
#include "core/memory/MemRef.h"

namespace Core
{
	struct Empty {};

	struct UnInitTag {};
	constexpr UnInitTag UnInit{};

	template<typename T>
	struct DefaultDeleter
	{
		auto operator()(MemRef<T>&& ref) noexcept -> void;

		template<typename U>
		auto operator=(DefaultDeleter<U>&&) noexcept -> DefaultDeleter& { return *this; }
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

	/**
	 * Count the number of elements in an iterator range
	 * \tparam T Forward iterator
	 * \param begin Iterator to first element
	 * \param end Iterator to element after the last element
	 * \return Number of elements in the iterator range
	 */
	template<ForwardIterator T>
	auto CountElems(const T& begin, const T& end) noexcept -> usize;

	/**
	 * Bitcast a value from one type to another
	 * \tparam To Type to cast to
	 * \tparam From Type to cast from
	 * \param from Value to cast
	 * \return Bitcasted value
	 */
	template<typename To, typename From>
	constexpr auto Bitcast(From& from) noexcept -> To;
}

#include "Utils.inl"