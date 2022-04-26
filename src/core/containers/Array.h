#pragma once
#include "core/MinInclude.h"

namespace Onca
{
	/**
	 * A fixed size array
	 * \tparam T Underlying type (needs to conform to Onca::Movable)
	 * \tparam N Size of the array
	 */
	template<typename T, usize N>
	class Array
	{
		// static assert to get around incomplete type issues when a class can return an Array of itself
		STATIC_ASSERT(Movable<T>, "Type needs to be movable to be used in an Array");
	public:

		auto At(usize idx) const noexcept -> Optional<T>;

		constexpr auto operator[](usize idx) noexcept -> T&;
		constexpr auto operator[](usize idx) const noexcept -> const T&;

		/**
		 * Get the size of the Array
		 * \return Size of the Array
		 */
		constexpr auto Size() const noexcept -> usize;
		/**
		 * Check if the Array is empty
		 * \return Whether the Array is empty
		 */
		constexpr auto IsEmpty() const noexcept ->  bool;

		/**
		 * Get the first element in the Array
		 * \return First element in the Array
		 */
		constexpr auto Front() noexcept -> T&;
		/**
		 * Get the first element in the Array
		 * \return First element in the Array
		 */
		constexpr auto Front() const noexcept -> const T&;
		/**
		 * Get the last element in the Array
		 * \return Last element in the Array
		 */
		constexpr auto Back() noexcept -> T&;
		/**
		 * Get the last element in the Array
		 * \return Last element in the Array
		 */
		constexpr auto Back() const noexcept -> const T&;

		/**
		 * Get a pointer to the Array's data
		 * \return Pointer to the Array's data
		 */
		constexpr auto Data() noexcept -> T*;
		/**
		 * Get a pointer to the Array's data
		 * \return Pointer to the Array's data
		 */
		constexpr auto Data() const noexcept -> const T*;

		/**
		 * Get an iterator to the first element
		 * \return Iterator to the first element
		 */
		constexpr auto Begin() noexcept -> T*;
		/**
		 * Get an iterator to the first element
		 * \return Iterator to the first element
		 */
		constexpr auto Begin() const noexcept -> const T*;

		/**
		 * Get an iterator to the end of the elements
		 * \return Iterator to the end of the elements
		 */
		constexpr auto End() noexcept -> T*;
		/**
		 * Get an iterator to the end of the elements
		 * \return Iterator to the end of the elements
		 */
		constexpr auto End() const noexcept -> const T*;

	private:
		T m_data[N]; ///< Array data
	};

	
}

#include "Array.inl"