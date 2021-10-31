#pragma once
#include "Base.h"

namespace Core::Intrin
{
	template<typename T>
	concept SimdBaseType = Integral<T> || FloatingPoint<T>;

	namespace Detail
	{

		// Helpers to make simd code clearer
		template<usize Size>
		concept IsSIMD128 = (Size == 16);
		template<usize Size>
		concept IsSIMD256 = (Size == 32);
	}
}
