#pragma once
#include "core/MinInclude.h"
#include "core/memory/MemRef.h"

namespace Core
{
	struct Empty
	{};

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

	template<ForwardIterator T>
	auto CountElems(const T& begin, const T& end) noexcept -> usize;

	template<MoveAssignable T>
	auto Swap(T& a, T& b) noexcept -> void;
}

#include "Utils.inl"