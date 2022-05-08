#pragma once
#include "core/MinInclude.h"
#include "core/allocator/GlobalAlloc.h"
#include "core/allocator/IAllocator.h"

namespace Onca
{

	/**
	 * Dynamically sized array
	 * \tparam T Stored type (needs to conform to Onca::Movable)
	 * \note Iterators are invalidated after certain container modifications
	 */
	template<typename T>
	class DynArray
	{
		// static assert to get around incomplete type issues when a class can return a DynArray of itself
		STATIC_ASSERT(Movable<T>, "Type needs to be moveable to be used in a DynArray");
	public:
		using Iterator = T*;
		using ConstIterator = const T* const;
		
		/**
		 * Create a DynArray with an allocator
		 * \param[in] alloc Allocator the container should use
		 */
		explicit DynArray(Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a DynArray with a capacity and an allocator
		 * \param[in] capacity Capacity of the array
		 * \param[in] alloc Allocator the container should use
		 */
		explicit DynArray(usize capacity, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a DynArray filled with a number of elements
		 * \param[in] count Number of elements to create
		 * \param[in] val Value of elements
		 * \param[in] alloc Allocator the container should use
		 */
		explicit DynArray(usize count, const T& val, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<T>;
		/**
		 * Create a DynArray from an initializer list
		 * \param[in] il Initializer list with elements
		 * \param[in] alloc Allocator the containter should use
		 */
		explicit DynArray(const InitializerList<T>& il, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<T>;
		/**
		 * Create a DynArray from an iterable range 
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit DynArray(const It& begin, const It& end, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<T>;
		/**
		 * Create a DynArray with the contents of another DynArray
		 * \param[in] other DynArray to copy
		 */
		DynArray(const DynArray& other) noexcept requires CopyConstructible<T>;
		/**
		 * Create a DynArray with the contents of another DynArray, but with a different allocator
		 * \param[in] other DynArray to copy
		 * \param[in] alloc Allocator the container should use
		 */
		explicit DynArray(const DynArray& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>;
		/**
		 * Move another DynArray into a new DynArray
		 * \param[in] other DynArray to move from
		 */
		DynArray(DynArray&& other) noexcept;
		/**
		 * Move another DynArray into a new DynArray, but with a different allocator
		 * \param[in] other DynArray to move from
		 * \param[in] alloc Allocator the container should use
		 */
		DynArray(DynArray&& other, Alloc::IAllocator& alloc) noexcept;
		~DynArray() noexcept;

		auto operator=(const InitializerList<T>& il) noexcept -> DynArray<T>& requires CopyConstructible<T>;
		auto operator=(const DynArray& other) noexcept -> DynArray<T>& requires CopyConstructible<T>;
		auto operator=(DynArray&& other) noexcept -> DynArray<T>&;

		/**
		 * Assign an iterable range to the DynArray
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 */
		template<ForwardIterator It>
		void Assign(const It& begin, const It& end) noexcept requires CopyConstructible<T>;
		/**
		 * Assign a linked list to the DynArray
		 * \param[in] il Initializer list with elements
		 */
		void Assign(const InitializerList<T>& il) noexcept requires CopyConstructible<T>;

		/**
		 * Fill the DynArray with a number of elements
		 * \param[in] count Number of elements to fill
		 * \param[in] val Value to fill elements with
		 */
		void Fill(usize count, const T& val) noexcept requires CopyConstructible<T>;
		/**
		 * Fill the DynArray with a number of elements with a default value (via placement new)
		 * \param[in] count Number of elements to fill
		 */
		void FillDefault(usize count) noexcept requires NoThrowDefaultConstructible<T>;

		/**
		 * Reserve additional space in the DynArray
		 * \param[in] newCap New capacity
		 */
		void Reserve(usize newCap) noexcept;
		/**
		 * Resize the DynArray and fill missing elements if needed
		 * \param[in] newSize New size of the DynArray
		 * \param[in] val Value to fill missing elements with
		 */
		void Resize(usize newSize, const T& val) noexcept requires CopyConstructible<T>;
		/**
		 * Resize the DynArray and fill missing elements with a default value (via placement new) if needed
		 * \param[in] newSize New size of the DynArray
		 */
		void Resize(usize newSize) noexcept requires NoThrowDefaultConstructible<T>;
		/**
		 * Shrink the memory used by the DynArray to the minimum needed
		 */
		void ShrinkToFit() noexcept;

		/**
		 * Add an element to the DynArray
		 * \param[in] val Element to add
		 */
		void Add(const T& val) noexcept requires CopyConstructible<T>;
		/**
		 * Add an element to the DynArray
		 * \param[in] val Element to add
		 */
		void Add(T&& val) noexcept;
		/**
		 * Add the contents of a DynArray to the DynArray
		 * \param[in] other DynArray to add
		 */
		void Add(const DynArray& other) requires CopyConstructible<T>;
		/**
		 * Add the contents of a DynArray to the DynArray
		 * \param[in] other DynArray to add
		 */
		void Add(DynArray&& other);

		/**
		 * Add a unique element to the DynArray (only add when not in the DynArray)
		 * \param[in] val Element to add
		 * \return Whether the value was added
		 */
		auto AddUnique(const T& val) noexcept -> bool requires CopyConstructible<T>;
		/**
		 * Add a unique element to the DynArray (only add when not in the DynArray)
		 * \param[in] val Element to add
		 * \return Whether the value was added
		 */
		auto AddUnique(T&& val) noexcept -> bool;

		/**
		 * Emplace an element at the back of the DynArray
		 * \tparam Args Type of arguments
		 * \param[in] args Arguments
		 */
		template<typename ...Args>
			requires ConstructableFrom<T, Args...>
		void EmplaceBack(Args&&... args) noexcept;

		/**
		 * Insert an element in a certain location
		 * \param[in] it Iterator to position to insert the element at
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto Insert(ConstIterator& it, const T& val) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert an element in a certain location
		 * \param[in] it Iterator to position to insert the element at
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto Insert(ConstIterator& it, T&& val) noexcept -> Iterator;
		/**
		 * Insert a number of elements in the DynArray
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] count Number of elements to insert
		 * \param[in] val Value of elements to insert
		 * \return Iterator to the first element that was inserter
		 */
		auto Insert(ConstIterator& it, usize count, const T& val) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert an iterable range into the DynArray
		 * \tparam It Iterator type
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \return Iterator to the first element that was inserted
		 */
		template<ForwardIterator It>
		auto Insert(ConstIterator& it, const It& begin, const It& end) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert an initializer list into the DynArray
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] il Initializer list to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto Insert(ConstIterator& it, const InitializerList<T>& il) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert a DynArray into the DynArray
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] other DynArray to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto Insert(ConstIterator& it, const DynArray& other) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert a DynArray into the DynArray
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] other DynArray to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto Insert(ConstIterator& it, DynArray&& other) noexcept -> Iterator;

		/**
		 * Insert an element in a certain location
		 * \param[in] idx Index to insert the element at
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto Insert(usize idx, const T& val) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert an element in a certain location
		 * \param[in] idx Index to insert the element at
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto Insert(usize idx, T&& val) noexcept -> Iterator;
		/**
		 * Insert a number of elements in the DynArray
		 * \param[in] idx Index to insert elements at
		 * \param[in] count Number of elements to insert
		 * \param[in] val Value of elements to insert
		 * \return Iterator to the first element that was inserter
		 */
		auto Insert(usize idx, usize count, const T& val) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert an iterable range into the DynArray
		 * \tparam It Iterator type
		 * \param[in] idx Index to insert elements at
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \return Iterator to the first element that was inserted
		 */
		template<ForwardIterator It>
		auto Insert(usize idx, const It& begin, const It& end) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert an initializer list into the DynArray
		 * \param[in] idx Index to insert elements at
		 * \param[in] il Initializer list to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto Insert(usize idx, const InitializerList<T>& il) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert a DynArray into the DynArray
		 * \param[in] idx Index to insert elements at
		 * \param[in] other DynArray to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto Insert(usize idx, const DynArray& other) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert a DynArray into the DynArray
		 * \param[in] idx Index to insert elements at
		 * \param[in] other DynArray to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto Insert(usize idx, DynArray&& other) noexcept -> Iterator;

		/**
		 * Emplace an element into the DynArray
		 * \tparam Args Type of arguments
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] args Arguments
		 */
		template<typename ...Args>
			requires ConstructableFrom<T, Args...>
		auto Emplace(ConstIterator& it, Args&&... args) noexcept -> Iterator;
		
		/**
		 * Clear the contents of the DynArray, possibly also deallocate the memory
		 * \param[in] clearMemory Whether to deallocate the memory
		 */
		void Clear(bool clearMemory = false) noexcept;
		/**
		 * Remove the last element from the DynArray
		 */
		void Pop() noexcept;
		/**
		 * Erase an element from the DynArray
		 * \param[in] it Iterator to element to erase
		 */
		void Erase(ConstIterator& it) noexcept;
		/**
		 * Erase a number of elements from the DynArray
		 * \param[in] it Iterator to first element to erase
		 * \param[in] count Number of elements to erase
		 */
		void Erase(ConstIterator& it, usize count) noexcept;
		/**
		 * Erase a range of elements from the DynArray
		 * \param[in] begin Iterator to first element to erase
		 * \param[in] end Iterator to last element to erase
		 */
		void Erase(ConstIterator& begin, ConstIterator& end) noexcept;
		/**
		 * Remove all occurrences that are equal to a given value from the DynArray
		 * \tparam U Type to compare with
		 * \param[in] val Value to remove
		 * \param[in] onlyFirst Whether to only remove the first occurrence of the value
		 */
		template<EqualComparable<T> U>
		void Erase(const U& val, bool onlyFirst = false) noexcept;
		/**
		 * Erase all elements for which the functor returns true
		 * \tparam F Functor type
		 * \param[in] fun Functor
		 */
		template<Callable<bool, const T&> F>
		void EraseIf(F fun) noexcept;
		/**
		 * Erase an element at a given index from the DynArray
		 * \param[in] idx Index
		 */
		void EraseAt(usize idx) noexcept;
		/**
		 * Erase a number of element at a given index from the DynArray
		 * \param[in] idx Index
		 * \param[in] count Number of elements to erase
		 */
		void EraseAt(usize idx, usize count) noexcept;

		/**
		 * Move the element out of the DynArray and erase the invalid data at its iterator
		 * \param[in] it Iterator to element
		 * \return Extracted element
		 */
		auto Extract(ConstIterator& it) noexcept -> T;
		/**
		 * Move the element out of the DynArray and erase the invalid data at its iterator
		 * \param[in] idx index of the element
		 * \return Extracted element
		 */
		auto Extract(usize idx) noexcept -> T;

		/**
		 * Find the first element that matches the looked for value
		 * \tparam U Type to compare with
		 * \param[in] value Value to find
		 * \return Iterator to the found element
		 */
		template<EqualComparable<T> U>
		auto Find(const U& value) noexcept -> Iterator;
		/**
		 * Find the first element that matches the looked for value
		 * \tparam U Type to compare with
		 * \param[in] value Value to find
		 * \return Iterator to the found element
		 */
		template<EqualComparable<T> U>
		auto Find(const U& value) const noexcept -> ConstIterator;
		/**
		 * Find the first element where the functor returns true
		 * \tparam F Functor type
		 * \param[in] fun Functor
		 * \return Iterator to the found element
		 */
		template<Callable<bool, const T&> F>
		auto FindIf(F fun) noexcept -> Iterator;
		/**
		 * Find the first element where the functor returns true
		 * \tparam F Functor type
		 * \param[in] fun Functor
		 * \return Iterator to the found element
		 */
		template<Callable<bool, const T&> F>
		auto FindIf(F fun) const noexcept -> ConstIterator;

		/**
		 * Check if the DynArray contains a value
		 * \tparam U Type to compare with
		 * \param[in] value Value to find
		 * \return Whether the DynArray contains the value
		 */
		template<EqualComparable<T> U>
		auto Contains(const U& value) const noexcept -> bool;
		/**
		 * Check if the DynArray contains a value where the functor returns true
		 * \tparam F Functor type
		 * \param[in] fun Functor
		 * \return Whether the DynArray contains the value
		 */
		template<Callable<bool, const T&> F>
		auto ContainsIf(F fun) const noexcept -> bool;
		
		/**
		 * Get the element at an index
		 * \param[in] idx Index of element
		 * \return Optional with value
		 * \note Will return an empty optional when index is out of bounds
		 */
		auto At(usize idx) const noexcept -> Optional<T>;
		/**
		 * Get an iterator to the element at an index
		 * \param[in] idx Index to get iterator to
		 * \return Iterator to element at idx
		 */
		auto IteratorAt(usize idx) noexcept -> Iterator;
		/**
		 * Get an iterator to the element at an index
		 * \param[in] idx Index to get iterator to
		 * \return Iterator to element at idx
		 */
		auto IteratorAt(usize idx) const noexcept -> ConstIterator;
		
		/**
		 * Get the element at an index
		 * \param[in] idx Index of element
		 * \return Element at index
		 * \note Only use when certain the index is not out of bounds
		 */
		auto operator[](usize idx) noexcept -> T&;
		/**
		 * Get the element at an index
		 * \param[in] idx Index of element
		 * \return Element at index
		 * \note Only use when certain the index is not out of bounds
		 */
		auto operator[](usize idx) const noexcept -> const T&;

		/**
		 * Get the size of the DynArray
		 * \return Size of the DynArray
		 */
		auto Size() const noexcept -> usize;
		/**
		 * Get the capacity of the DynArray
		 * \return Capacity of the DynArray
		 */
		auto Capacity() const noexcept -> usize;
		/**
		 * Check if the DynArray is empty
		 * \return Whether the DynArray is empty
		 */
		auto IsEmpty() const noexcept ->  bool;
		/**
		 * Get a pointer to the DynArray's data
		 * \return Pointer to the DynArray's data
		 */
		auto Data() noexcept -> T*;
		/**
		 * Get a pointer to the DynArray's data
		 * \return Pointer to the DynArray's data
		 */
		auto Data() const noexcept -> const T*;

		/**
		 * Get the allocator used by the DynArray
		 * \return Allocator used by the DynArray
		 */
		auto GetAllocator() const noexcept -> Alloc::IAllocator*;

		/**
		 * Get the first element in the DynArray
		 * \return First element in the DynArray
		 * \note Only use when the DynArray is not empty
		 */
		auto Front() noexcept -> T&;
		/**
		 * Get the first element in the DynArray
		 * \return First element in the DynArray
		 * \note Only use when the DynArray is not empty
		 */
		auto Front() const noexcept -> const T&;
		/**
		 * Get the last element in the DynArray
		 * \return Last element in the DynArray
		 * \note Only use when the DynArray is not empty
		 */
		auto Back() noexcept -> T&;
		/**
		 * Get the last element in the DynArray
		 * \return Last element in the DynArray
		 * \note Only use when the DynArray is not empty
		 */
		auto Back() const noexcept -> const T&;

		/**
		 * Get an iterator to the first element
		 * \return Iterator to the first element
		 */
		auto Begin() noexcept -> Iterator;
		/**
		 * Get an iterator to the first element
		 * \return Iterator to the first element
		 */
		auto Begin() const noexcept -> ConstIterator;

		/**
		 * Get an iterator to the end of the elements
		 * \return Iterator to the end of the elements
		 */
		auto End() noexcept -> Iterator;
		/**
		 * Get an iterator to the end of the elements
		 * \return Iterator to the end of the elements
		 */
		auto End() const noexcept -> ConstIterator;

		/**
		 * Get a reverse Reverse iterator to the first element
		 * \return Reverse iterator to the first element
		 */
		auto RBegin() noexcept -> Iterator;
		/**
		 * Get a reverse Reverse iterator to the first element
		 * \return Reverse iterator to the first element
		 */
		auto RBegin() const noexcept -> ConstIterator;

		/**
		 * Get a reverse Reverse iterator to the end of the elements
		 * \return Reverse iterator to the end of the elements
		 */
		auto REnd() noexcept -> Iterator;
		/**
		 * Get a reverse Reverse iterator to the end of the elements
		 * \return Reverse iterator to the end of the elements
		 */
		auto REnd() const noexcept -> ConstIterator;

		// Overloads for 'for ( ... : ... )'
		auto begin() noexcept -> Iterator;
		auto begin() const noexcept -> ConstIterator;
		auto cbegin() const noexcept -> ConstIterator;
		auto end() noexcept -> Iterator;
		auto end() const noexcept -> ConstIterator;
		auto cend() const noexcept -> ConstIterator;
		
	private:

		auto InsertEnd(T&& val) noexcept -> Iterator;
		auto PrepareInsert(usize offset, usize count) noexcept -> Iterator;
		
		MemRef<T> m_mem;  ///< Managed memory
		usize     m_size; ///< Size of the DynArray
	};

}

#include "DynArray.inl"
