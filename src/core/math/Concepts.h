#pragma once
#include "core/MinInclude.h"

namespace Core::Math
{
	template<typename T>
	concept Incrementable = requires(T t)
	{
		{ ++t } noexcept;
		{ t++ } noexcept;
	};

	template<typename T>
	concept Decrementable = requires(T t)
	{
		{ --t } noexcept;
		{ t-- } noexcept;
	};

	template<typename T>
	concept HasPositive = requires(T t)
	{
		{ +t } noexcept;
	};

	template<typename T>
	concept HasNegative = requires(T t)
	{
		{ -t } noexcept;
	};

	template<typename T>
	concept Invertable = requires(T t)
	{
		{ ~t } noexcept;
	};

	template<typename T, typename U = T>
	concept Addable = requires(T a, U b)
	{
		{ a +  b } noexcept;
		{ a += b } noexcept;
	};

	template<typename T, typename U = T>
	concept Subtractable = requires(T a, U b)
	{
		{ a -  b } noexcept;
		{ a -= b } noexcept;
	};

	template<typename T, typename U = T>
	concept Multiplyable = requires(T a, U b)
	{
		{ a *  b } noexcept;
		{ a *= b } noexcept;
	};

	template<typename T, typename U = T>
	concept Divisible = requires(T a, U b)
	{
		{ a /  b } noexcept;
		{ a /= b } noexcept;
	};

	template<typename T, typename U = T>
	concept HasModulo = requires(T a, U b)
	{
		{ a %  b } noexcept;
		{ a %= b } noexcept;
	};

	template<typename T, typename U = T>
	concept Orable = requires(T a, U b)
	{
		{ a |  b } noexcept;
		{ a |= b } noexcept;
	};

	template<typename T, typename U = T>
	concept Xorable = requires(T a, U b)
	{
		{ a ^  b } noexcept;
		{ a ^= b } noexcept;
	};

	template<typename T, typename U = T>
	concept Andable = requires(T a, U b)
	{
		{ a &  b } noexcept;
		{ a &= b } noexcept;
	};

	template<typename T, typename U = T>
	concept BitShiftable = requires(T a, U b)
	{
		{ a >>  b } noexcept;
		{ a <<  b } noexcept;
		{ a >>= b } noexcept;
		{ a <<= b } noexcept;
	};

	template<typename T>
	concept Numeric =
		Integral<T> ||
		FloatingPoint<T> ||
		(Addable<T> &&
		 Subtractable<T> &&
		 Multiplyable<T> &&
		 Divisible<T> &&
		requires(T a, T b)
	{
		{ T{   } } noexcept;
		{ T{ a } } noexcept;
		{ a =  b } noexcept;
	});
}
