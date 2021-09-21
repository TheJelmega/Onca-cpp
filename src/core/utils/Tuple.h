#pragma once
#include "core/MinInclude.h"
#include <tuple>

namespace Core
{
	template<typename... Args>
	using Tuple = std::tuple<Args...>;

	template<usize Idx, typename T>
	struct TupleElementHelper;

	template<usize Idx, typename T>
	using TupleElement = typename TupleElementHelper<Idx, T>::Type;
}
