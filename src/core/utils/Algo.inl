#pragma once
#include "Algo.h"

namespace Core::Algo
{
	template <ForwardIterator InIt, ForwardIterator OutIt>
	auto Copy(InIt from, OutIt to, usize size) noexcept -> void
	{
		STATIC_ASSERT((IsSame<Decay<decltype(*from)>, Decay<decltype(*to)>>), "Iterators should have the same underlying type");
		using UnderlyingType = Decay<decltype(*to)>;

		for (usize i = 0; i < size; ++i)
			new (&*to++) UnderlyingType{ *from++ };
	}

	template <RandomAccessIterator InIt, RandomAccessIterator OutIt>
	auto Copy(InIt from, OutIt to, usize size) noexcept -> void
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
	auto Move(InIt from, OutIt to, usize size) noexcept -> void
	{
		STATIC_ASSERT((IsSame<Decay<decltype(*from)>, Decay<decltype(*to)>>), "Iterators should have the same underlying type");
		using UnderlyingType = Decay<decltype(*to)>;

		for (usize i = 0; i < size; ++i)
			new (&*to++) UnderlyingType{ Core::Move(*from++) };
	}

	template <RandomAccessIterator InIt, RandomAccessIterator OutIt>
	auto Move(InIt from, OutIt to, usize size) noexcept -> void
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
}
