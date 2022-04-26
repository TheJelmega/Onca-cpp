#pragma once
#include "Base.h"

namespace Onca::Intrin
{
	template<typename T>
	concept SimdBaseType = Integral<T> || FloatingPoint<T>;
}
