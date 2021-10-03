#pragma once
#include "core/MinInclude.h"

namespace Core::Algo
{
	/**
	 * Swap the value in 2 references
	 * \tparam T Movable type
	 * \param a First value
	 * \param b Second value
	 */
	template<Movable T>
	constexpr auto Swap(T& a, T& b) noexcept -> void;

	/**
	 * Swap the value in 2 iterators
	 * \tparam It Forward Iterator
	 * \param a First value
	 * \param b Second value
	 */
	template<ForwardIterator It>
	constexpr auto SwapIter(It a, It b) noexcept -> void;

	/**
	 * Copy a range of elements from an iterator to another iterator
	 * \tparam InIt Forward iterator
	 * \tparam OutIt Forward iterator
	 * \param[in] from Iterator to copy from
	 * \param[in] to Iterator to copy to
	 * \param[in] size Number of elements to copy
	 * \note Ranges should not overlap
	 */
	template<ForwardIterator InIt, ForwardIterator OutIt>
	constexpr auto Copy(InIt from, OutIt to, usize size) noexcept -> void;
	/**
	 * Copy a range of elements from an iterator to another iterator
	 * \tparam InIt Random access iterator
	 * \tparam OutIt Random access iterator
	 * \param[in] from Iterator to copy from
	 * \param[in] to Iterator to copy to
	 * \param[in] size Number of elements to copy
	 * \note This version supports backwards copying
	 */
	template<RandomAccessIterator InIt, RandomAccessIterator OutIt>
	constexpr auto Copy(InIt from, OutIt to, usize size) noexcept -> void;

	/**
	 * Move a range elements from an iterator to another iterator
	 * \tparam InIt Random access iterator
	 * \tparam OutIt Random access iterator
	 * \param[in] from Iterator to start moving from
	 * \param[in] to Iterator to move to
	 * \param[in] size Number of elements to move
	 * \note Ranges should not overlap
	 */
	template<ForwardIterator InIt, ForwardIterator OutIt>
	constexpr auto Move(InIt from, OutIt to, usize size) noexcept -> void;
	/**
	 * Move a range elements from an iterator to another iterator
	 * \tparam InIt Random access iterator
	 * \tparam OutIt Random access iterator
	 * \param[in] from Iterator to start moving from
	 * \param[in] to Iterator to move to
	 * \param[in] size Number of elements to move
	 */
	template<RandomAccessIterator InIt, RandomAccessIterator OutIt>
	constexpr auto Move(InIt from, OutIt to, usize size) noexcept -> void;

	/**
	 * Reverse a range of values
	 * \tparam It Bidirectional Iterator
	 * \param first Iterator to first element
	 * \param last Iterator to last element
	 */
	template<BidirectionalIterator It>
	constexpr auto Reverse(It first, It last) noexcept -> void;

	/**
	 * Reverse a range of values
	 * \tparam It Random access Iterator
	 * \param first Iterator to first element
	 * \param last Iterator to last element
	 */
	template<RandomAccessIterator It>
	constexpr auto Reverse(It first, It last) noexcept -> void;

}

#include "Algo.inl"