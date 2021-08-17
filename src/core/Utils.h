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

	template<ForwardIterator T>
	auto CountElems(const T& begin, const T& end) noexcept -> usize;

#define ConcatTuple(...) std::tuple_cat(__VA_ARGS__)
#define ConstructFromTuple std::make_from_tuple
	
}

#include "Utils.inl"