#pragma once
#include <type_traits>
#include "Config.h"

namespace Core
{
	/**
	 * Is a type nothrow default constructable
	 * \tparam T Type to check
	 */
	template<typename T>
	INL constexpr bool IsNothrowDefaultConstructable = std::is_nothrow_default_constructible_v<T>;

	/**
	 * Is a type default constructable
	 * \tparam T Type to check
	 */
	template<typename T>
	INL constexpr bool IsCopyConstructable = std::is_copy_constructible_v<T>;
	/**
	 * Is a type default Movable
	 * \tparam T Type to check
	 */
	template<typename T>
	INL constexpr bool IsMoveConstructable = std::is_move_constructible_v<T>;

	/**
	 * Check if 2 types are the same
	 * \tparam A First type
	 * \tparam B Second type
	 */
	template<typename A, typename B>
	INL constexpr bool IsSameAs = std::is_same_v<A, B>;

	/**
	 * Is a type an integral
	 * \tparam T Type to check
	 */
	template<typename T>
	INL constexpr  bool IsIntegral = std::is_integral_v<T>;
	/**
	 * Is a type an floating point
	 * \tparam T Type to check
	 */
	template<typename T>
	INL constexpr  bool IsFloastingPoint = std::is_floating_point_v<T>;
	/**
	 * Is a type a boolean
	 * \tparam T Type to check
	 */
	template<typename T>
	INL constexpr bool IsBoolean = IsSameAs<T, bool>;
	/**
	 * Is a type an pointer
	 * \tparam T Type to check
	 */
	template<typename T>
	INL constexpr bool IsPointer = std::is_pointer_v<T>;
	/**
	 * Is a type void
	 * \tparam T Type to check
	 */
	template<typename T>
	INL constexpr bool IsVoid = std::is_void_v<T>;
	/**
	 * Is a type an nullptr_t
	 * \tparam T Type to check
	 */
	template<typename T>
	INL constexpr bool IsNullptrT = std::is_null_pointer_v<T>;
	/**
	 * Is a type a primitive type
	 * \tparam T Type to check
	 */
	template<typename T>
	INL constexpr bool IsPrimitive = std::is_fundamental_v<T>;
	/**
	 * Is a type an lvalue reference
	 * \tparam T Type to check
	 */
	template<typename T>
	INL constexpr bool IsLValueReference = std::is_lvalue_reference_v<T>;

	/**
	 * Does an iterator have contiguous data
	 * \tparam T Type to check
	 * \note This should be overloaded to allow for specific optimizations
	 */
	template<typename T>
	INL constexpr bool IteratorHasContiguousData = IsPointer<T>;

	/**
	 * Removes the reference of a type
	 * \tparam T Type to remove reference from
	 */
	template<typename T>
	using RemoveReference = std::remove_reference_t<T>;


	template<bool C, typename T, typename U>
	using Conditional = std::conditional_t<C, T, U>;

}
