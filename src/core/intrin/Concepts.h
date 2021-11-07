#pragma once
#include "Base.h"

namespace Core::Intrin
{
	template<typename T>
	concept SimdBaseType = Integral<T> || FloatingPoint<T>;
}
