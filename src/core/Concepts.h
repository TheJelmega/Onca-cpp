#pragma once
#include <concepts>
#include "Traits.h"

namespace Core
{
	template<typename A, typename B>
	concept SameAs = std::same_as<A, B>;
	template<typename A, typename B>
	concept SameSizeAs = (sizeof(A) == sizeof(B));

	template<typename T>
	concept Integral = std::integral<T>;
	template<typename T>
	concept UnsignedIntegral = std::unsigned_integral<T>;
	template<typename T>
	concept SignedIntegral = std::signed_integral<T>;
	template<typename T>
	concept FloatingPoint = std::floating_point<T>;
	template<typename T>
	concept PrimitiveType = std::is_fundamental_v<T>;
	template<typename T>
	concept EnumType = std::is_enum_v<T>;

	template<typename A, typename B>
	concept IntegralOfSameSize = Integral<A> && SameSizeAs<A, B>;

	template<typename T>
	concept IsI8 = SameAs<T, i8>;
	template<typename T>
	concept IsI16 = SameAs<T, i16>;
	template<typename T>
	concept IsI32 = SameAs<T, i32>;
	template<typename T>
	concept IsI64 = SameAs<T, i64>;
	template<typename T>
	concept IsU8 = SameAs<T, u8>;
	template<typename T>
	concept IsU16 = SameAs<T, u16>;
	template<typename T>
	concept IsU32 = SameAs<T, u32>;
	template<typename T>
	concept IsU64 = SameAs<T, u64>;
	template<typename T>
	concept IsF32 = SameAs<T, f32>;
	template<typename T>
	concept IsF64 = SameAs<T, f64>;

	// wchar_t is not supported, as it is not portable
	template<typename T>
	concept CharacterType =
		SameAs<T, char> ||
		SameAs<T, char8_t> ||
		SameAs<T, char16_t> ||
		SameAs<T, char32_t>;

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


	template<typename T, typename U>
	concept ConvertableFrom = requires(U u)
	{
		{ static_cast<T>(u) };
	};
	template<typename T, typename U>
	concept ConvertableTo = ConvertableFrom<U, T>;

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

	template<typename T, typename R, typename... Args>
	concept Functor =
		DefaultConstructible<T> &&
		requires(T t, Args... args)
	{
		{ T{}(args...) } noexcept -> SameAs<R>;
	};

	// Context sensitive
	// TODO: better concept for this
	template<typename T>
	concept Lambda =
		SameAs<T, Decay<T>>;
}
