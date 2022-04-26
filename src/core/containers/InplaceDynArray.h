#pragma once
#include "core/MinInclude.h"
#include "core/Assert.h"

namespace Core
{

	/**
	 * Inplace dynamically sized array (Memory is on stack)
	 * \tparam T Stored type (needs to conform to Core::Movable)
	 * \tparam Cap Capacity (max number of elements)
	 * \note Iterators are invalidated after certain container modifications
	 */
	template<typename T, usize Cap>
	class InplaceDynArray
	{
		// static assert to get around incomplete type issues when a class can return a InplaceDynArray of itself
		STATIC_ASSERT(Movable<T>, "Type needs to be movable to be used in an InplaceDynArray");
	public:
		using Iterator = T*;
		using ConstIterator = const T*;

		/**
		 * Create a InplaceDynArray
		 */
		constexpr InplaceDynArray() noexcept;
		/**
		 * Create a InplaceDynArray with a number of elements with a default value (via placement new)
		 * \param[in] count Number of elements to create
		 * \param[in] val Value of elements
		 */
		constexpr explicit InplaceDynArray(usize count) noexcept requires CopyConstructible<T>;
		/**
		 * Create a InplaceDynArray filled with a number of elements
		 * \param[in] count Number of elements to create
		 * \param[in] val Value of elements
		 */
		constexpr explicit InplaceDynArray(usize count, const T& val) noexcept requires CopyConstructible<T>;
		/**
		 * Create a InplaceDynArray from an initializer list
		 * \param[in] il Initializer list with elements
		 */
		constexpr explicit InplaceDynArray(const InitializerList<T>& il) noexcept requires CopyConstructible<T>;
		/**
		 * Create a InplaceDynArray from an iterable range
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 */
		template<ForwardIterator It>
		constexpr explicit InplaceDynArray(const It& begin, const It& end) noexcept requires CopyConstructible<T>;
		/**
		 * Create a InplaceDynArray with the contents of another InplaceDynArray
		 * \param[in] other InplaceDynArray to copy
		 */
		constexpr InplaceDynArray(const InplaceDynArray& other) noexcept requires CopyConstructible<T>;
		/**
		 * Move another InplaceDynArray into a new InplaceDynArray
		 * \param[in] other InplaceDynArray to move from
		 */
		constexpr InplaceDynArray(InplaceDynArray&& other) noexcept;
		constexpr ~InplaceDynArray() noexcept;

		constexpr auto operator=(const InitializerList<T>& il) noexcept -> InplaceDynArray& requires CopyConstructible<T>;
		constexpr auto operator=(const InplaceDynArray& other) noexcept -> InplaceDynArray& requires CopyConstructible<T>;
		constexpr auto operator=(InplaceDynArray&& other) noexcept -> InplaceDynArray&;

		/**
		 * Assign an iterable range to the InplaceDynArray
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 */
		template<ForwardIterator It>
		constexpr void Assign(const It& begin, const It& end) noexcept requires CopyConstructible<T>;
		/**
		 * Assign a linked list to the InplaceDynArray
		 * \param[in] il Initializer list with elements
		 */
		constexpr void Assign(const InitializerList<T>& il) noexcept requires CopyConstructible<T>;

		/**
		 * Fill the InplaceDynArray with a number of elements
		 * \param[in] count Number of elements to fill
		 * \param[in] val Value to fill elements with
		 */
		constexpr void Fill(usize count, const T& val) noexcept requires CopyConstructible<T>;
		/**
		 * Fill the InplaceDynArray with a number of elements with a default value (via placement new)
		 * \param[in] count Number of elements to fill
		 */
		constexpr void FillDefault(usize count) noexcept requires NoThrowDefaultConstructible<T>;
		
		/**
		 * Resize the InplaceDynArray and fill missing elements if needed
		 * \param[in] newSize New size of the InplaceDynArray
		 * \param[in] val Value to fill missing elements with
		 */
		constexpr void Resize(usize newSize, const T& val) noexcept requires CopyConstructible<T>;
		/**
		 * Resize the InplaceDynArray and fill missing elements with a default value (via placement new) if needed
		 * \param[in] newSize New size of the InplaceDynArray
		 */
		constexpr void Resize(usize newSize) noexcept requires NoThrowDefaultConstructible<T>;

		/**
		 * Add an element to the InplaceDynArray
		 * \param[in] val Element to add
		 */
		constexpr void Add(const T& val) noexcept requires CopyConstructible<T>;
		/**
		 * Add an element to the InplaceDynArray
		 * \param[in] val Element to add
		 */
		constexpr void Add(T&& val) noexcept;
		/**
		 * Add the contents of a InplaceDynArray to the InplaceDynArray
		 * \param[in] other InplaceDynArray to add
		 */
		constexpr void Add(const InplaceDynArray& other) requires CopyConstructible<T>;
		/**
		 * Add the contents of a InplaceDynArray to the InplaceDynArray
		 * \param[in] other InplaceDynArray to add
		 */
		constexpr void Add(InplaceDynArray&& other);

		/**
		 * Emplace an element at the back of the InplaceDynArray
		 * \tparam Args Type of arguments
		 * \param[in] args Arguments
		 */
		template<typename ...Args>
			requires ConstructableFrom<T, Args...>
		constexpr void EmplaceBack(Args&&... args) noexcept;

		/**
		 * Insert an element in a certain location
		 * \param[in] it Iterator to position to insert the element at
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		constexpr auto Insert(const ConstIterator& it, const T& val) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert an element in a certain location
		 * \param[in] it Iterator to position to insert the element at
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		constexpr auto Insert(const ConstIterator& it, T&& val) noexcept -> Iterator;
		/**
		 * Insert a number of elements in the InplaceDynArray
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] count Number of elements to insert
		 * \param[in] val Value of elements to insert
		 * \return Iterator to the first element that was inserter
		 */
		constexpr auto Insert(const ConstIterator& it, usize count, const T& val) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert an iterable range into the InplaceDynArray
		 * \tparam It Iterator type
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \return Iterator to the first element that was inserted
		 */
		template<ForwardIterator It>
		constexpr auto Insert(const ConstIterator& it, const It& begin, const It& end) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert an initializer list into the InplaceDynArray
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] il Initializer list to insert
		 * \return Iterator to the first element that was inserted
		 */
		constexpr auto Insert(const ConstIterator& it, const InitializerList<T>& il) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert a InplaceDynArray into the InplaceDynArray
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] other InplaceDynArray to insert
		 * \return Iterator to the first element that was inserted
		 */
		constexpr auto Insert(const ConstIterator& it, const InplaceDynArray& other) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert a InplaceDynArray into the InplaceDynArray
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] other InplaceDynArray to insert
		 * \return Iterator to the first element that was inserted
		 */
		constexpr auto Insert(const ConstIterator& it, InplaceDynArray&& other) noexcept -> Iterator;

		/**
		 * Emplace an element into the InplaceDynArray
		 * \tparam Args Type of arguments
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] args Arguments
		 */
		template<typename ...Args>
			requires ConstructableFrom<T, Args...>
		constexpr auto Emplace(const ConstIterator& it, Args&&... args) noexcept -> Iterator;

		/**
		 * Clear the contents of the InplaceDynArray, possibly also deallocate the memory
		 */
		constexpr void Clear() noexcept;
		/**
		 * Remove the last element from the InplaceDynArray
		 */
		constexpr void Pop() noexcept;
		/**
		 * Erase an element from the InplaceDynArray
		 * \param[in] it Iterator to element to erase
		 */
		constexpr void Erase(const Iterator& it) noexcept;
		/**
		 * Erase a number of elements from the InplaceDynArray
		 * \param[in] it Iterator to first element to erase
		 * \param[in] count Number of elements to erase
		 */
		constexpr void Erase(const Iterator& it, usize count) noexcept;
		/**
		 * Erase a range of elements from the InplaceDynArray
		 * \param[in] begin Iterator to first element to erase
		 * \param[in] end Iterator to last element to erase
		 */
		constexpr void Erase(const Iterator& begin, const Iterator& end) noexcept;

		/**
		 * Find the first element that matches the looked for value
		 * \tparam U Type to compare with
		 * \param[in] value Value to find
		 * \return Iterator to found element
		 */
		template<EqualComparable<T> U>
		constexpr auto Find(const U& value) noexcept -> Iterator;
		/**
		 * Find the first element that matches the looked for value
		 * \tparam U Type to compare with
		 * \param[in] value Value to find
		 * \return Iterator to found element
		 */
		template<EqualComparable<T> U>
		constexpr auto Find(const U& value) const noexcept -> ConstIterator;

		/**
		 * Get the element at an index
		 * \param[in] idx Index of element
		 * \return Optional with value
		 * \note Will return an empty optional when index is out of bounds
		 */
		constexpr auto At(usize idx) const noexcept -> Optional<T>;
		/**
		 * Get an iterator to the element at an index
		 * \param[in] idx Index to get iterator to
		 * \return Iterator to element at idx
		 */
		constexpr auto IteratorAt(usize idx) noexcept -> Iterator;
		/**
		 * Get an iterator to the element at an index
		 * \param[in] idx Index to get iterator to
		 * \return Iterator to element at idx
		 */
		constexpr auto IteratorAt(usize idx) const noexcept -> ConstIterator;

		/**
		 * Get the element at an index
		 * \param[in] idx Index of element
		 * \return Element at index
		 * \note Only use when certain the index is not out of bounds
		 */
		constexpr auto operator[](usize idx) noexcept -> T&;
		/**
		 * Get the element at an index
		 * \param[in] idx Index of element
		 * \return Element at index
		 * \note Only use when certain the index is not out of bounds
		 */
		constexpr auto operator[](usize idx) const noexcept -> const T&;

		/**
		 * Get the capacity of the InplaceDynArray
		 * \return Capacity of the InplaceDynArray
		 */
		constexpr auto Capacity() const noexcept -> usize;
		/**
		 * Get the size of the InplaceDynArray
		 * \return Size of the InplaceDynArray
		 */
		constexpr auto Size() const noexcept -> usize;
		/**
		 * Check if the InplaceDynArray is empty
		 * \return Whether the InplaceDynArray is empty
		 */
		constexpr auto IsEmpty() const noexcept ->  bool;
		/**
		 * Get a pointer to the InplaceDynArray's data
		 * \return Pointer to the InplaceDynArray's data
		 */
		constexpr auto Data() noexcept -> T*;
		/**
		 * Get a pointer to the InplaceDynArray's data
		 * \return Pointer to the InplaceDynArray's data
		 */
		constexpr auto Data() const noexcept -> const T*;

		/**
		 * Get the first element in the InplaceDynArray
		 * \return First element in the InplaceDynArray
		 * \note Only use when the InplaceDynArray is not empty
		 */
		constexpr auto Front() noexcept -> T&;
		/**
		 * Get the first element in the InplaceDynArray
		 * \return First element in the InplaceDynArray
		 * \note Only use when the InplaceDynArray is not empty
		 */
		constexpr auto Front() const noexcept -> const T&;
		/**
		 * Get the last element in the InplaceDynArray
		 * \return Last element in the InplaceDynArray
		 * \note Only use when the InplaceDynArray is not empty
		 */
		constexpr auto Back() noexcept -> T&;
		/**
		 * Get the last element in the InplaceDynArray
		 * \return Last element in the InplaceDynArray
		 * \note Only use when the InplaceDynArray is not empty
		 */
		constexpr auto Back() const noexcept -> const T&;

		/**
		 * Get an iterator to the first element
		 * \return Iterator to the first element
		 */
		constexpr auto Begin() noexcept -> Iterator;
		/**
		 * Get an iterator to the first element
		 * \return Iterator to the first element
		 */
		constexpr auto Begin() const noexcept -> ConstIterator;

		/**
		 * Get an iterator to the end of the elements
		 * \return Iterator to the end of the elements
		 */
		constexpr auto End() noexcept -> Iterator;
		/**
		 * Get an iterator to the end of the elements
		 * \return Iterator to the end of the elements
		 */
		constexpr auto End() const noexcept -> ConstIterator;

		/**
		 * Get a reverse Reverse iterator to the first element
		 * \return Reverse iterator to the first element
		 */
		constexpr auto RBegin() noexcept -> Iterator;
		/**
		 * Get a reverse Reverse iterator to the first element
		 * \return Reverse iterator to the first element
		 */
		constexpr auto RBegin() const noexcept -> ConstIterator;

		/**
		 * Get a reverse Reverse iterator to the end of the elements
		 * \return Reverse iterator to the end of the elements
		 */
		constexpr auto REnd() noexcept -> Iterator;
		/**
		 * Get a reverse Reverse iterator to the end of the elements
		 * \return Reverse iterator to the end of the elements
		 */
		constexpr auto REnd() const noexcept -> ConstIterator;

		// Overloads for 'for ( ... : ... )'
		constexpr auto begin() noexcept -> Iterator;
		constexpr auto begin() const noexcept -> ConstIterator;
		constexpr auto cbegin() const noexcept -> ConstIterator;
		constexpr auto end() noexcept -> Iterator;
		constexpr auto end() const noexcept -> ConstIterator;
		constexpr auto cend() const noexcept -> ConstIterator;

	private:

		T     m_data[Cap]; ///< Inplace memory
		usize m_size;      ///< Size of the InplaceDynArray
	};
}

#include "InplaceDynArray.inl"
