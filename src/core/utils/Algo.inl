#pragma once
#include "Algo.h"

namespace Core::Algo
{
	template <ForwardIterator InIt, ForwardIterator OutIt>
	auto Copy(InIt from, OutIt to, usize size) noexcept -> void
	{
		STATIC_ASSERT((IsSame<Decay<decltype(*from)>, Decay<decltype(*to)>>), "Iterators should have the same underlying type");

		for (usize i = 0; i < size; ++i)
			*to++ = *from++;
	}

	template <RandomAccessIterator InIt, RandomAccessIterator OutIt>
	auto Copy(InIt from, OutIt to, usize size) noexcept -> void
	{
		STATIC_ASSERT((IsSame<Decay<decltype(*from)>, Decay<decltype(*to)>>), "Iterators should have the same underlying type");

		if (from <= to)
		{
			from += size - 1;
			to += size - 1;
			for (usize i = 0; i < size; ++i)
				*to-- = *from--;
		}
		else
		{
			for (usize i = 0; i < size; ++i)
				*to++ = *from++;
		}
	}

	template <ForwardIterator InIt, ForwardIterator OutIt>
	auto Move(InIt from, OutIt to, usize size) noexcept -> void
	{
		STATIC_ASSERT((IsSame<Decay<decltype(*from)>, Decay<decltype(*to)>>), "Iterators should have the same underlying type");

		for (usize i = 0; i < size; ++i)
			*to++ = Core::Move(*from++);
	}

	template <RandomAccessIterator InIt, RandomAccessIterator OutIt>
	auto Move(InIt from, OutIt to, usize size) noexcept -> void
	{
		STATIC_ASSERT((IsSame<Decay<decltype(*from)>, Decay<decltype(*to)>>), "Iterators should have the same underlying type");

		if (from <= to)
		{
			from += size - 1;
			to += size - 1;
			for (usize i = 0; i < size; ++i)
				*to-- = Core::Move(*from--);
		}
		else
		{
			for (usize i = 0; i < size; ++i)
				*to++ = Core::Move(*from++);
		}
	}
}
