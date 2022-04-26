#pragma once
#include "core/MinInclude.h"
#include <tuple>

namespace Onca
{
	template<typename... Args>
	using Tuple = std::tuple<Args...>;

	template<usize Idx, typename T>
	struct TupleElementHelper;

	template<usize Idx, typename T>
	using TupleElement = typename TupleElementHelper<Idx, T>::Type;
}
