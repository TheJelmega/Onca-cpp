#pragma once
#include "core/MinInclude.h"
#include "String.h"
#include "Interfaces.h"

namespace Onca
{
	/**
	 * Convert a string to a string
	 * \param[in] str String
	 * \return String
	 */
	auto ToString(const String& str) noexcept -> String;
	/**
	 * Convert a const string to a string
	 * \tparam N Size of const string
	 * \param[in] str Const string
	 * \return String
	 */
	template<usize N>
	auto ToString(const ConstString<N>& str) noexcept -> String;
	/**
	 * Convert any type that has a ToString() method to a string via a free function
	 * \tparam T Type
	 * \param[in] t Value
	 * \return String
	 */
	template<ToStringMethod T>
	auto ToString(const T& t) noexcept -> String;
	/**
	 * Convert a c-string to a string
	 * \tparam C Character type
	 * \param[in] cstr c-string
	 * \return String
	 */
	template<CharacterType C>
	auto ToString(const C* cstr) noexcept -> String;
	/**
	 * Convert an integer to a string
	 * \tparam I Integral type
	 * \param[in] val value
	 * \return String
	 */
	template<Integral I>
	auto ToString(I val) noexcept -> String;
	/**
	 * Convert a floating point to a string
	 * \tparam F Floating point type
	 * \param[in] val value
	 * \return String
	 */
	template<FloatingPoint F>
	auto ToString(F val) noexcept -> String;
	/**
	 * Convert a floating point to a string
	 * \tparam F Floating point type
	 * \param[in] val value
	 * \return String
	 */
	auto ToString(bool val) noexcept -> String;

	////////////////////////////////////////////////////////////////

	/**
	 * Convert a string to a string
	 * \param[in] str String
	 * \return String
	 */
	auto ToDebugString(const String& str) noexcept -> String;
	/**
	 * Convert a const string to a string
	 * \tparam N Size of const string
	 * \param[in] str Const string
	 * \return String
	 */
	template<usize N>
	auto ToDebugString(const ConstString<N>& str) noexcept -> String;
	/**
	 * Convert any type that has a ToDebugString() method to a string via a free function
	 * \tparam T Type
	 * \param[in] t Value
	 * \return String
	 */
	template<ToDebugStringMethod T>
	auto ToDebugString(const T& t) noexcept -> String;
	/**
	 * Convert a c-string to a string
	 * \tparam C Character type
	 * \param[in] cstr c-string
	 * \return String
	 */
	template<CharacterType C>
	auto ToDebugString(const C* cstr) noexcept -> String;
	/**
	 * Convert an integer to a string
	 * \tparam I Integral type
	 * \param[in] val value
	 * \return String
	 */
	template<Integral I>
	auto ToDebugString(I val) noexcept -> String;
	/**
	 * Convert a floating point to a string
	 * \tparam F Floating point type
	 * \param[in] val value
	 * \return String
	 */
	template<FloatingPoint F>
	auto ToDebugString(F val) noexcept -> String;
	/**
	 * Convert a floating point to a string
	 * \tparam F Floating point type
	 * \param[in] val value
	 * \return String
	 */
	auto ToDebugString(bool val) noexcept -> String;
}

#include "Stringify.inl"