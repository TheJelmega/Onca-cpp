#pragma once
#include "Pair.h"

namespace Core
{
	template <typename T0, typename T1>
	constexpr Pair<T0, T1>::Pair() noexcept requires NoThrowDefaultConstructable<T0>&& NoThrowDefaultConstructable<T1>
		: first()
		, second()
	{
	}

	template <typename T0, typename T1>
	constexpr Pair<T0, T1>::Pair(const T0& first, const T1& second) noexcept requires CopyConstructable<T0> &&
		CopyConstructable<T1>
		: first(first)
		, second(second)
	{
	}

	template <typename T0, typename T1>
	template <typename U0, typename U1> requires ConstructableFrom<T0, U0> && ConstructableFrom<T1, U1>
	constexpr Pair<T0, T1>::Pair(U0&& first, const U1&& second) noexcept
		: first(Move(first))
		, second(Move(second))
	{
	}

	template <typename T0, typename T1>
	template <typename U0, typename U1> requires ConstructableFrom<T0, U0> && ConstructableFrom<T1, U1>
	constexpr Pair<T0, T1>::Pair(const Pair<U0, U1>& other) noexcept
		: first(other.first)
		, second(other.second)
	{
	}

	template <typename T0, typename T1>
	template <typename U0, typename U1> requires ConstructableFrom<T0, U0> && ConstructableFrom<T1, U1>
	constexpr Pair<T0, T1>::Pair(Pair<U0, U1>&& other) noexcept
		: first(Move(other.first))
		, second(Move(other.second))
	{
	}

	template <typename T0, typename T1>
	template <typename ... Args0, typename ... Args1>
		requires ConstructableFrom<T0, Args0...> && ConstructableFrom<T1, Args1...>
	constexpr Pair<T0, T1>::Pair(Tuple<Args0...> args0, Tuple<Args1...> args1) noexcept
		: Pair(args0, args1, IndexSequenceFor<Args0...>{}, IndexSequenceFor<Args1...>{})
	{
		
	}

	template <typename T0, typename T1>
	template <typename ... Args0, typename ... Args1, usize... Idx0, usize... Idx1>
		requires ConstructableFrom<T0, Args0...> && ConstructableFrom<T1, Args1...>
	constexpr Pair<T0, T1>::Pair(Tuple<Args0...>& args0, Tuple<Args1...>& args1, IndexSequence<Idx0...>, IndexSequence<Idx1...>) noexcept
		: first(std::get<Idx0>(Move(args0))...)
		, second(std::get<Idx1>(Move(args1))...)
	{
	}

	template <typename T0, typename T1>
	auto Pair<T0, T1>::operator=(const Pair& other) noexcept -> Pair&
	{
		first = other.first;
		second = other.second;
		return *this;
	}

	template <typename T0, typename T1>
	template <typename U0, typename U1>
		requires ConstructableFrom<T0, U0> && ConstructableFrom<T1, U1>
	auto Pair<T0, T1>::operator=(const Pair<U0, U1>& other) noexcept -> Pair&
	{
		first = T0{ other.first };
		second = T1{ other.second };
		return *this;
	}

	template <typename T0, typename T1>
	auto Pair<T0, T1>::operator=(Pair&& other) noexcept -> Pair&
	{
		first = Move(other.first);
		second = Move(other.second);
		return *this;
	}

	template <typename T0, typename T1>
	template <typename U0, typename U1>
		requires ConstructableFrom<T0, U0> && ConstructableFrom<T1, U1>
	auto Pair<T0, T1>::operator=(Pair<U0, U1>&& other) noexcept -> Pair&
	{
		first = T0{ Move(other.first) };
		second = T1{ Move(other.second) };
		return *this;
	}
}

namespace std
{
	template<usize Idx, typename T0, typename T1>
	constexpr auto get(Core::Pair<T0, T1>& pair) noexcept -> Core::TupleElement<Idx, Core::Pair<T0, T1>>&
	{
		if constexpr (Idx == 0)
			return pair.first;
		else if constexpr (Idx == 1)
			return pair.second;
		else
			STATIC_ASSERT(false, "Index out of range");
	}

	template<usize Idx, typename T0, typename T1>
	constexpr auto get(const Core::Pair<T0, T1>& pair) noexcept -> const Core::TupleElement<Idx, Core::Pair<T0, T1>>&
	{
		if constexpr (Idx == 0)
			return pair.first;
		else if constexpr (Idx == 1)
			return pair.second;
		else
			STATIC_ASSERT(false, "Index out of range");
	}

	template<usize Idx, typename T0, typename T1>
	constexpr auto get(Core::Pair<T0, T1>&& pair) noexcept -> Core::TupleElement<Idx, Core::Pair<T0, T1>>&&
	{
		if constexpr (Idx == 0)
			return pair.first;
		else if constexpr (Idx == 1)
			return pair.second;
		else
			STATIC_ASSERT(false, "Index out of range");
	}

	template<usize Idx, typename T0, typename T1>
	constexpr auto get(const Core::Pair<T0, T1>&& pair) noexcept -> const Core::TupleElement<Idx, Core::Pair<T0, T1>>&&
	{
		if constexpr (Idx == 0)
			return pair.first;
		else if constexpr (Idx == 1)
			return pair.second;
		else
			STATIC_ASSERT(false, "Index out of range");
	}
}
