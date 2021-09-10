#pragma once
#include "core/MinInclude.h"

namespace Core
{
	/**
	 * A pair of 2 values
	 * \tparam T0 First type
	 * \tparam T1 Second type
	 * \note Has no_unique_address optimizations for empty types
	 */
	template<typename T0, typename T1>
	struct Pair
	{
		/**
		 * Create an empty Pair
		 */
		constexpr Pair() noexcept requires NoThrowDefaultConstructible<T0>&& NoThrowDefaultConstructible<T1>;
		/**
		 * Create a Pair with 2 values
		 * \param first First value
		 * \param second Second value
		 */
		constexpr Pair(const T0& first, const T1& second) noexcept requires CopyConstructible<T0>&& CopyConstructible<T1>;

		/**
		 * Create a Pair with 2 moved values
		 * \tparam U0 First type
		 * \tparam U1 Second type
		 * \param first Init value for first
		 * \param second Init value for second
		 */
		template<typename U0, typename U1>
			requires ConstructableFrom<T0, U0> && ConstructableFrom<T1, U1>
		constexpr explicit Pair(U0&& first, const U1&& second) noexcept;

		/**
		 * Convert a Pair to this Pair
		 * \tparam U0 First type
		 * \tparam U1 Second type
		 * \param other Pair to convert
		 */
		template<typename U0, typename U1>
			requires ConstructableFrom<T0, U0>&& ConstructableFrom<T1, U1>
		constexpr Pair(const Pair<U0, U1>& other) noexcept;
		/**
		 * Convert a Pair to this Pair
		 * \tparam U0 First type
		 * \tparam U1 Second type
		 * \param other Pair to convert
		 */
		template<typename U0, typename U1>
			requires ConstructableFrom<T0, U0>&& ConstructableFrom<T1, U1>
		constexpr Pair(Pair<U0, U1>&& other) noexcept;

		/**
		 * Create a pair with each value constructed from a tuple
		 * \tparam Args0 First arguments
		 * \tparam Args1 Second arguments
		 * \param args0 Arguments for the first value
		 * \param args1 Arguments for the second value
		 */
		template<typename... Args0, typename... Args1>
			requires ConstructableFrom<T0, Args0...> && ConstructableFrom<T1, Args1...>
		constexpr explicit Pair(Tuple<Args0...> args0, Tuple<Args1...> args1) noexcept;

		template<typename... Args0, typename... Args1, usize... Idx0, usize... Idx1>
			requires ConstructableFrom<T0, Args0...> && ConstructableFrom<T1, Args1...>
		constexpr explicit Pair(Tuple<Args0...>& args0, Tuple<Args1...>& args1, IndexSequence<Idx0...>, IndexSequence<Idx1...>) noexcept;

		constexpr Pair(const Pair&) noexcept = default;
		constexpr Pair(Pair&&) noexcept = default;


		auto operator=(const Pair& other) noexcept -> Pair&;
		template<typename U0, typename U1>
			requires ConstructableFrom<T0, U0>&& ConstructableFrom<T1, U1>
		auto operator=(const Pair<U0, U1>& other) noexcept -> Pair&;

		auto operator=(Pair&& other) noexcept -> Pair&;
		template<typename U0, typename U1>
			requires ConstructableFrom<T0, U0>&& ConstructableFrom<T1, U1>
		auto operator=(Pair<U0, U1>&& other) noexcept -> Pair&;

		NO_UNIQUE_ADDRESS T0 first;
		NO_UNIQUE_ADDRESS T1 second;
	};

	template<usize Idx, typename T0, typename T1>
	struct TupleElementHelper<Idx, Pair<T0, T1>>
	{
		STATIC_ASSERT(Idx < 2, "Index out of range");

		using Type = Conditional<Idx == 0, T0, T1>;
	};
}

namespace std
{
	template<usize Idx, typename T0, typename T1>
	constexpr auto get(Core::Pair<T0, T1>& pair) noexcept -> Core::TupleElement<Idx, Core::Pair<T0, T1>>&;
	template<usize Idx, typename T0, typename T1>
	constexpr auto get(const Core::Pair<T0, T1>& pair) noexcept -> const Core::TupleElement<Idx, Core::Pair<T0, T1>>&;
	template<usize Idx, typename T0, typename T1>
	constexpr auto get(Core::Pair<T0, T1>&& pair) noexcept -> Core::TupleElement<Idx, Core::Pair<T0, T1>>&&;
	template<usize Idx, typename T0, typename T1>
	constexpr auto get(const Core::Pair<T0, T1>&& pair) noexcept -> const Core::TupleElement<Idx, Core::Pair<T0, T1>>&&;
}

#include "Pair.inl"