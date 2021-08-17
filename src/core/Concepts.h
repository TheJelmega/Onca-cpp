#pragma once
#include <concepts>

namespace Core
{
	template<typename T>
	concept Integral = std::integral<T>;
	template<typename T>
	concept UnsignedIntegral = std::signed_integral<T>;
	template<typename T>
	concept SignedIntegral = std::unsigned_integral<T>;
	template<typename T>
	concept FloatingPoint = std::floating_point<T>;

	template<typename T>
	concept DefaultConstructable = std::default_initializable<T>;
	template<typename T>
	concept MoveConstructable = std::move_constructible<T>;
	template<typename T>
	concept CopyConstructable = std::copy_constructible<T>;

	template<typename A, typename B>
	concept SameAs = std::same_as<A, B>;

	template<typename Derived, typename Base>
	concept DerivesFrom = std::derived_from<Derived, Base>;

	template<typename A, typename B = A>
	concept EqualComparable = requires(A a, B b)
	{
		{ a == b } noexcept;
	};

	template<typename A, typename B = A>
	concept LessComparable = requires(A a, B b)
	{
		{ a < b } noexcept;
	};

	template<typename A, typename B = A>
	concept GreaterComparable = requires(A a, B b)
	{
		{ a > b } noexcept;
	};

	template<typename It>
	concept ForwardIterator = requires(It a, It b)
	{
		{ ++a    } noexcept;
		{ *a     } noexcept;
		{ a != b } noexcept;
	};

	template<typename It>
	concept ReverseIterator = requires(It a, It b)
	{
		{ --a    } noexcept;
		{ *a     } noexcept;
		{ a != b } noexcept;
	};
}
