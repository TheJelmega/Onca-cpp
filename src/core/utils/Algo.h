#pragma once
#include "core/MinInclude.h"

namespace Core::Algo
{
	/**
	 * Copy a range of elements from an iterator to another iterator
	 * \tparam InIt Forward iterator
	 * \tparam OutIt Forward iterator
	 * \param[in] from Iterator to copy from
	 * \param[in] to Iterator to copy to
	 * \param[in] size Number of elements to copy
	 */
	template<ForwardIterator InIt, ForwardIterator OutIt>
	auto Copy(InIt from, OutIt to, usize size) noexcept -> void;
	/**
	 * Copy a range of elements from an iterator to another iterator
	 * \tparam InIt Forward iterator
	 * \tparam OutIt Forward iterator
	 * \param[in] from Iterator to copy from
	 * \param[in] to Iterator to copy to
	 * \param[in] size Number of elements to copy
	 * \note This version supports backwards copying
	 */
	template<RandomAccessIterator InIt, RandomAccessIterator OutIt>
	auto Copy(InIt from, OutIt to, usize size) noexcept -> void;

	/**
	 * Move a range elements from an iterator to another iterator
	 * \tparam InIt Random access iterator
	 * \tparam OutIt Random access iterator
	 * \param[in] from Iterator to start moving from
	 * \param[in] to Iterator to move to
	 * \param[in] size Number of elements to move
	 */
	template<ForwardIterator InIt, ForwardIterator OutIt>
	auto Move(InIt from, OutIt to, usize size) noexcept -> void;
	/**
	 * Move a range elements from an iterator to another iterator
	 * \tparam InIt Random access iterator
	 * \tparam OutIt Random access iterator
	 * \param[in] from Iterator to start moving from
	 * \param[in] to Iterator to move to
	 * \param[in] size Number of elements to move
	 */
	template<RandomAccessIterator InIt, RandomAccessIterator OutIt>
	auto Move(InIt from, OutIt to, usize size) noexcept -> void;

}

#include "Algo.inl"