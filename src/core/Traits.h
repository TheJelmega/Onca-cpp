#pragma once
#include <type_traits>
#include "Config.h"

namespace Core
{
	template<typename T>
	INL constexpr bool IsNothrowDefaultConstructable = std::is_nothrow_default_constructible_v<T>;

	template<typename T>
	INL constexpr bool IsCopyConstructable = std::is_copy_constructible_v<T>;
	template<typename T>
	INL constexpr bool IsMoveConstructable = std::is_move_constructible_v<T>;
}
