#pragma once
#include "core/Concepts.h"
#include "core/Defines.h"

namespace Core
{
	/**
	 * Sequence of integers
	 * \tparam T Integral type
	 * \tparam Inds Indices
	 */
	template<Integral T, T... Inds>
	struct IntegerSequence
	{
		using ValType = T;
		using Type = IntegerSequence;

		NO_DISCARD("")
		static constexpr auto Size() noexcept -> usize
		{
			return sizeof...(Inds);
		}
	};

	/**
	 * Sequence of indices
	 * \tparam Inds Indices
	 */
	template<usize... Inds>
	using IndexSequence = IntegerSequence<usize, Inds...>;

	// Implementation should be O(log N)
	namespace Detail
	{
		template<typename Seq1, typename Seq2>
		struct MergeAndRenumberIntegerSequences;
		template<Integral T, usize... I1, usize... I2>
		struct MergeAndRenumberIntegerSequences<IntegerSequence<T, I1...>, IntegerSequence<T, I2...>>
			: IntegerSequence<T, T(I1)..., T(sizeof...(I1) + I2)...>
		{};

		template<typename Seq1, typename Seq2>
		struct ReverseMergeAndRenumberIntegerSequences;
		template<Integral T, usize... I1, usize... I2>
		struct ReverseMergeAndRenumberIntegerSequences<IntegerSequence<T, I1...>, IntegerSequence<T, I2...>>
			: IntegerSequence<T, T(sizeof...(I2) + I1)..., T(I2)...>
		{};

		template<Integral T, usize N>
		struct MakeIntegerSequence
			: MergeAndRenumberIntegerSequences<typename MakeIntegerSequence<T, N/2>::Type,
			                   typename MakeIntegerSequence<T, N - N/2>::Type>
		{};
		template<Integral T>
		struct MakeIntegerSequence<T, 0> : IntegerSequence<T>{};
		template<Integral T>
		struct MakeIntegerSequence<T, 1> : IntegerSequence<T, T(0)> {};

		template<Integral T, usize N>
		struct MakeReverseIntegerSequence
			: ReverseMergeAndRenumberIntegerSequences<typename MakeIntegerSequence<T, N - N / 2>::Type,
			                                          typename MakeIntegerSequence<T, N / 2>::Type>
		{};
		template<Integral T>
		struct MakeReverseIntegerSequence<T, 0> : IntegerSequence<T> {};
		template<Integral T>
		struct MakeReverseIntegerSequence<T, 1> : IntegerSequence<T, T(0)> {};

		template<Integral T, usize Begin, usize Size>
		struct MakeIntegerSequenceRange
			: MergeAndRenumberIntegerSequences<typename MakeIntegerSequenceRange<T, Begin, Size / 2>::Type,
			                                   typename MakeIntegerSequenceRange<T, Begin, Size - Size / 2>::Type>
		{};
		template<Integral T, usize N>
		struct MakeIntegerSequenceRange<T, N, 0> : IntegerSequence<T>{};
		template<Integral T, usize N>
		struct MakeIntegerSequenceRange<T, N, 1> : IntegerSequence<T, T(N)>{};

		template<Integral T, usize Begin, usize Size>
		struct MakeReverseIntegerSequenceRange
			: ReverseMergeAndRenumberIntegerSequences<typename MakeIntegerSequenceRange<T, Begin, Size - Size / 2>::Type,
			                                          typename MakeIntegerSequenceRange<T, Begin, Size / 2>::Type>
		{};
		template<Integral T, usize N>
		struct MakeReverseIntegerSequenceRange<T, N, 0> : IntegerSequence<T> {};
		template<Integral T, usize N>
		struct MakeReverseIntegerSequenceRange<T, N, 1> : IntegerSequence<T, T(N)> {};
	}

	/**
	 * Create an IntegerSequence
	 * \tparam T Integral type
	 * \tparam N Size of sequence
	 */
	template<Integral T, usize N>
	using MakeIntegerSequence = Detail::MakeIntegerSequence<T, N>;

	/**
	 * Creates an IndexSequence
	 * \tparam N Size of sequence
	 */
	template<usize N>
	using MakeIndexSequence = MakeIntegerSequence<usize, N>;

	/**
	 * Create a ranged IntegerSequence
	 * \tparam T Integral type
	 * \tparam Start First value of sequence
	 * \tparam End Last value of sequence
	 */
	template<typename T, usize Start, usize End>
	using MakeIntegerSequenceRange = Detail::MakeIntegerSequenceRange<T, Start, End - Start>;
	
	/**
	 * Create a ranged IndexSequence
	 * \tparam Start First value of sequence
	 * \tparam End Last value of sequence
	 */
	template<usize Start, usize End>
	using MakeIndexSequenceRange = MakeIntegerSequenceRange<usize, Start, End>;

	/**
	 * Create a reverse IntegerSequence
	 * \tparam T Integral type
	 * \tparam N Size of sequence
	 */
	template<Integral T, usize N>
	using MakeReverseIntegerSequence = Detail::MakeReverseIntegerSequence<T, N>;

	/**
	 * Creates a reverse IndexSequence
	 * \tparam N Size of sequence
	 */
	template<usize N>
	using MakeReverseIndexSequence = MakeReverseIntegerSequence<usize, N>;

	/**
	 * Create a reverse ranged IntegerSequence
	 * \tparam T Integral type
	 * \tparam Start First value of sequence
	 * \tparam End Last value of sequence
	 */
	template<typename T, usize Start, usize End>
	using MakeReverseIntegerSequenceRange = Detail::MakeReverseIntegerSequenceRange<T, Start, End - Start>;

	/**
	 * Create a reverse ranged IndexSequence
	 * \tparam Start First value of sequence
	 * \tparam End Last value of sequence
	 */
	template<usize Start, usize End>
	using MakeReverseIndexSequenceRange = MakeReverseIntegerSequenceRange<usize, Start, End>;
}
