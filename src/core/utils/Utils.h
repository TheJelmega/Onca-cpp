#pragma once
#include "core/MinInclude.h"
#include "core/memory/MemRef.h"

namespace Onca
{
	struct Empty {};

	struct UnInitTag {};
	constexpr UnInitTag UnInit{};

	template<typename T>
	struct DefaultDeleter
	{
		void operator()(MemRef<T>&& ref) noexcept;

		DefaultDeleter() noexcept {}
		template<typename U>
		DefaultDeleter(DefaultDeleter<U>&&) noexcept {}
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
	 * Get the size of an array
	 * \tparam T Array type
	 * \tparam N Array size
	 * \return Size of the array
	 */
	template<typename T, usize N>
	constexpr  auto ArraySize(const T (&)[N]) noexcept -> usize;
}

#include "Utils.inl"