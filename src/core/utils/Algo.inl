#pragma once
#if __RESHARPER__
#include "Algo.h"
#endif

namespace Core::Algo
{
	template <Movable T>
	constexpr void Swap(T& a, T& b) noexcept
	{
		T tmp{ Core::Move(a) };
		a = Core::Move(b);
		b = Core::Move(tmp);
	}

	template <ForwardIterator It>
	constexpr void SwapIter(It a, It b) noexcept
	{
		STATIC_ASSERT(Movable<Decay<decltype(*a)>>, "Value contained in iterator should be movable");
		using UnderlyingType = Decay<decltype(*a)>;

		UnderlyingType tmp{ Core::Move(a) };
		a = Core::Move(b);
		b = Core::Move(tmp);
	}

	template <ForwardIterator InIt, ForwardIterator OutIt>
	constexpr void Copy(InIt from, OutIt to, usize size) noexcept
	{
		STATIC_ASSERT((IsSame<Decay<decltype(*from)>, Decay<decltype(*to)>>), "Iterators should have the same underlying type");
		using UnderlyingType = Decay<decltype(*to)>;

		for (usize i = 0; i < size; ++i)
			new (&*to++) UnderlyingType{ *from++ };
	}

	template <RandomAccessIterator InIt, RandomAccessIterator OutIt>
	constexpr void Copy(InIt from, OutIt to, usize size) noexcept
	{
		STATIC_ASSERT((IsSame<Decay<decltype(*from)>, Decay<decltype(*to)>>), "Iterators should have the same underlying type");
		using UnderlyingType = Decay<decltype(*to)>;

		if (from <= to)
		{
			from += size - 1;
			to += size - 1;
			for (usize i = 0; i < size; ++i)
				new (&*to--) UnderlyingType{ Core::Move(*from--) };
		}
		else
		{
			for (usize i = 0; i < size; ++i)
				new (&*to++) UnderlyingType{ *from++ };
		}
	}

	template <ForwardIterator InIt, ForwardIterator OutIt>
	constexpr void Move(InIt from, OutIt to, usize size) noexcept
	{
		STATIC_ASSERT((IsSame<Decay<decltype(*from)>, Decay<decltype(*to)>>), "Iterators should have the same underlying type");
		using UnderlyingType = Decay<decltype(*to)>;

		for (usize i = 0; i < size; ++i)
			new (&*to++) UnderlyingType{ Core::Move(*from++) };
	}

	template <RandomAccessIterator InIt, RandomAccessIterator OutIt>
	constexpr void Move(InIt from, OutIt to, usize size) noexcept
	{
		STATIC_ASSERT((IsSame<Decay<decltype(*from)>, Decay<decltype(*to)>>), "Iterators should have the same underlying type");
		using UnderlyingType = Decay<decltype(*to)>;

		if (from <= to)
		{
			from += size - 1;
			to += size - 1;
			for (usize i = 0; i < size; ++i)
				new (&*to--) UnderlyingType{ Core::Move(*from--) };
		}
		else
		{
			for (usize i = 0; i < size; ++i)
				new (&*to++) UnderlyingType{ Core::Move(*from++) };
		}
	}

	template<BidirectionalIterator It>
	constexpr void Reverse(It first, It last) noexcept
	{
		if (!(first != last))
			return;

		while ((first != last) && (first != ++last))
			SwapIter(first++, last);
	}

	template <RandomAccessIterator It>
	constexpr void Reverse(It first, It last) noexcept
	{
		if (!(first != last))
			return;

		for (; first < last; ++first, --last)
			SwapIter(first, last);
	}
}
