#pragma once
#include <concepts>
#include "Traits.h"

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
	concept PrimitiveType = std::is_fundamental_v<T>;
	template<typename T>
	concept EnumType = std::is_enum_v<T>;

	template<typename T>
	concept TriviallyCopyable = std::is_trivially_copyable_v<T>;

	template<typename T>
	concept DefaultConstructible = std::default_initializable<T>;
	template<typename T>
	concept MoveConstructible = std::move_constructible<T>;
	template<typename T>
	concept CopyConstructible = std::copy_constructible<T>;
	template<typename T>
	concept NoThrowDefaultConstructible = IsNothrowDefaultConstructable<T>;

	template<typename T>
	concept MoveAssignable = std::is_move_assignable_v<T>;

	template<typename T>
	concept Movable = MoveConstructible<T> && MoveAssignable<T>;

	template<typename A, typename B>
	concept SameAs = std::same_as<A, B>;

	template<typename Derived, typename Base>
	concept DerivesFrom = std::derived_from<Derived, Base>;

	template<typename T>
	concept MemCopyable =
		PrimitiveType<T> ||
		TriviallyCopyable<T>;

	template<typename T, typename... Args>
	concept ConstructableFrom = requires (Args... args)
	{
		{ T{ args... } } noexcept;
	};

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

	template<typename A, typename B = A>
	concept OrderedComparable = requires(A a, B b)
	{
		{ a < b  } noexcept;
		{ a <= b } noexcept;
		{ a > b  } noexcept;
		{ a >= b } noexcept;
	};

	template<typename It>
	concept ForwardIterator = requires(It a, It b, usize n)
	{
		{ ++a    } noexcept;
		{ a += n } noexcept;
		{ a + n  } noexcept;
		{ *a     } noexcept;
		{ a != b } noexcept;
	};

	template<typename It>
	concept ReverseIterator = requires(It a, It b, usize n)
	{
		{ --a    } noexcept;
		{ a -= n } noexcept;
		{ a - n  } noexcept;
		{ *a     } noexcept;
		{ a != b } noexcept;
	};

	template<typename T>
	concept BidirectionalIterator =
		ForwardIterator<T> &&
		ReverseIterator<T>;

	template<typename T>
	concept RandomAccessIterator =
		BidirectionalIterator<T> &&
		requires(T a, T b, usize n)
	{
		{ a -  b } noexcept -> SameAs<isize>;
		{ a <  b } noexcept;
		{ a <= b } noexcept;
		{ a >  b } noexcept;
		{ a >= b } noexcept;
		{ a[n]   } noexcept;
	};

	// TODO: expand ContiguousIterator
	template<typename T>
	concept ContiguousIterator = 
		RandomAccessIterator<T> &&
		IteratorHasContiguousData<T>;

	template<typename T, typename U>
	concept Hasher =
		DefaultConstructible<T> &&
		requires(U u)
	{
		{ static_cast<const T>(T{})(u) } noexcept -> SameAs<u64>;
	};

	template<typename T, typename A, typename B = A>
	concept EqualsComparator =
		DefaultConstructible<T> &&
		requires(A a, B b)
	{
		{ static_cast<const T>(T{})(a, b) } noexcept -> SameAs<bool>;
	};

	template<typename T, typename A, typename B = A>
	concept Comparator =
		DefaultConstructible<T> &&
		requires(A a, B b)
	{
		{ static_cast<const T>(T{})(a, b) } noexcept -> SameAs<i8>;
	};
}
