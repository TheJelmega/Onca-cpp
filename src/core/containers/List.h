#pragma once
#include "core/MinInclude.h"
#include "core/allocator/GlobalAlloc.h"
#include "core/memory/MemRef.h"

namespace Core
{

	/**
	 * Linked List
	 * \tparam T Stored type
	 */
	template<MoveConstructable T>
	class List
	{
	private:

		struct Node
		{
			MemRef<Node> next;
			T val;
		};
		using NodeRef = MemRef<Node>;

	public:

		class Iterator
		{
		public:
			Iterator() noexcept;
			Iterator(const Iterator&) noexcept = default;
			Iterator(Iterator&&) noexcept = default;

			auto operator=(const Iterator& other) noexcept;
			auto operator=(Iterator&& other) noexcept;

			auto operator++() noexcept -> Iterator;
			auto operator++(int) noexcept -> Iterator;

			auto operator->() const noexcept -> T*;
			auto operator*() const noexcept -> T&;

			auto operator+(usize count) const noexcept -> Iterator;

			auto operator==(const Iterator& other) const noexcept -> bool;
			auto operator!=(const Iterator& other) const noexcept -> bool;

		private:
			explicit Iterator(const MemRef<Node>& node) noexcept;

			NodeRef m_node;

			friend class List;
		};
		using ConstIterator = const Iterator;

	public:
		/**
		 * Create a List with an allocator
		 * \param alloc Allocator the container should use
		 */
		explicit List(Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a List with a number of elements with a default value (via placement new)
		 * \param count Number of elements
		 * \param alloc Allcoator the container shoud use
		 */
		explicit List(usize count, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires NoThrowDefaultConstructable<T>;
		/**
		 * Create a List filled with a number of elements
		 * \param count Number of elements
		 * \param val Value of elements
		 * \param alloc Allcoator the container shoud use
		 */
		explicit List(usize count, const T& val, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructable<T>;;

		/**
		 * Create a List from an initializer list
		 * \param[in] il Initializer list with elements
		 * \param[in] alloc Allocator the containter should use
		 */
		explicit List(const InitializerList<T>& il, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructable<T>;
		/**
		 * Create a List from an iterable range
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit List(const It& begin, const It& end, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructable<T>;
		/**
		 * \brief Create a List with the contents of another List
		 * \param[in] other List to copy
		 */
		List(const List& other) noexcept requires CopyConstructable<T>;
		/**
		 * \brief Create a List with the contents of another List, but with a different allocator
		 * \param[in] other List to copy
		 * \param[in] alloc Allocator the container should use
		 */
		explicit List(const List& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructable<T>;
		/**
		 * Move another List into a new List
		 * \param[in] other List to move from
		 */
		List(List&& other) noexcept;
		~List() noexcept;

		auto operator=(const InitializerList<T>& il) noexcept -> List<T>& requires CopyConstructable<T>;
		auto operator=(const List& other) noexcept -> List<T>& requires CopyConstructable<T>;
		auto operator=(List&& other) noexcept -> List<T>&;

		/**
		 * \brief Assign an iterable range to the List
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 */
		template<ForwardIterator It>
		auto Assign(const It& begin, const It& end) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Assign a linked list to the List
		 * \param[in] il Initializer list with elements
		 */
		auto Assign(const InitializerList<T>& il) noexcept -> void requires CopyConstructable<T>;

		/**
		 * Fill the List with a number of elements
		 * \param[in] count Number of elements to fill
		 * \param[in] val Value to fill elements with
		 */
		auto Fill(usize count, const T& val) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Fill the List with a number of elements with a default value (via placement new)
		 * \param[in] count Number of elements to fill
		 */
		auto FillDefault(usize count) noexcept -> void requires NoThrowDefaultConstructable<T>;

		/**
		 * Resize the List and fill missing elements if needed
		 * \param[in] newSize New size of the DynArray
		 * \param[in] val Value to fill missing elements with
		 */
		auto Resize(usize newSize, const T& val) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Resize the List and fill missing elements with a default value (via placement new) if needed
		 * \param[in] newSize New size of the DynArray
		 */
		auto Resize(usize newSize) noexcept -> void requires NoThrowDefaultConstructable<T>;

		/**
		 * Add an element to the List
		 * \param[in] val Element to add
		 */
		auto Add(const T& val) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Add an element to the List
		 * \param[in] val Element to add
		 */
		auto Add(T&& val) noexcept -> void;
		/**
		 * Add the contents of a List to the List
		 * \param[in] other List to add
		 */
		auto Add(const List& other) -> void requires CopyConstructable<T>;
		/**
		 * Add the contents of a List to the List
		 * \param[in] other List to add
		 */
		auto Add(List&& other) -> void;

		/**
		 * Emplace an element at the back of the List
		 * \tparam Args Type of arguments
		 * \param[in] args Arguments
		 */
		template<typename ...Args>
			requires IsConstructableWith<T, Args...>
		auto EmplaceBack(Args&&... args) noexcept -> void;

		/**
		 * Insert an element after a certain location
		 * \param[in] it Iterator to position before the element to insert the element at
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto InsertAfter(const ConstIterator& it, const T& val) noexcept -> Iterator requires CopyConstructable<T>;
		/**
		 * Insert an element after a certain location
		 * \param[in] it Iterator to position before the element to insert the element at
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto InsertAfter(const ConstIterator& it, T&& val) noexcept -> Iterator;
		/**
		 * Insert a number of elements in the List after a certain location
		 * \param[in] it Iterator to position before the element to insert elements at
		 * \param[in] count Number of elements to insert
		 * \param[in] val Value of elements to insert
		 * \return Iterator to the first element that was inserter
		 */
		auto InsertAfter(const ConstIterator& it, usize count, const T& val) noexcept -> Iterator requires CopyConstructable<T>;
		/**
		 * Insert an iterable range into the List after a certain location
		 * \tparam It Iterator type
		 * \param[in] it Iterator to position before the element to insert elements at
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \return Iterator to the first element that was inserted
		 */
		template<ForwardIterator It>
		auto InsertAfter(const ConstIterator& it, const It& begin, const It& end) noexcept -> Iterator requires CopyConstructable<T>;
		/**
		 * Insert an initializer list into the List after a certain location
		 * \param[in] it Iterator to position before the element to insert elements at
		 * \param[in] il Initializer list to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto InsertAfter(const ConstIterator& it, const InitializerList<T>& il) noexcept -> Iterator requires CopyConstructable<T>;
		/**
		 * Insert a List into the List after a certain location
		 * \param[in] it Iterator to position before the element to insert elements at
		 * \param[in] other List to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto InsertAfter(const ConstIterator& it, const List& other) noexcept -> Iterator requires CopyConstructable<T>;
		/**
		 * Insert a List into the List after a certain location
		 * \param[in] it Iterator to position before the element to insert elements at
		 * \param[in] other List to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto InsertAfter(const ConstIterator& it, List&& other) noexcept -> Iterator;

		/**
		 * Emplace an element into the List after a certain location
		 * \tparam Args Type of arguments
		 * \param[in] it Iterator to position before the element to insert elements at
		 * \param[in] args Arguments
		 */
		template<typename ...Args>
			requires IsConstructableWith<T, Args...>
		auto EmplaceAfter(const ConstIterator& it, Args&&... args) noexcept -> Iterator;

		/**
		 * Add an element at the front of the List
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto AddFront(const T& val) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Add an element at the front of the List
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto AddFront(T&& val) noexcept -> void;
		/**
		 * Add a number of elements at the front of the List
		 * \param[in] count Number of elements to insert
		 * \param[in] val Value of elements to insert
		 * \return Iterator to the first element that was inserter
		 */
		auto AddFront(usize count, const T& val) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Add an iterable range at the front of the List
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \return Iterator to the first element that was inserted
		 */
		template<ForwardIterator It>
		auto AddFront(const It& begin, const It& end) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Add an initializer list at the front of the List
		 * \param[in] il Initializer list to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto AddFront(const InitializerList<T>& il) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Add a List into the List at the front of the List
		 * \param[in] other List to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto AddFront(const List& other) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Add a List into the List at the front of the List
		 * \param[in] other List to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto AddFront(List&& other) noexcept -> void;

		/**
		 * Emplace an element at the front of the List
		 * \tparam Args Type of arguments
		 * \param[in] args Arguments
		 */
		template<typename ...Args>
			requires IsConstructableWith<T, Args...>
		auto EmplaceFront(Args&&... args) noexcept -> void;

		/**
		 * Clear the contents of the List
		 */
		auto Clear() noexcept -> void;
		/**
		 * Remove the last element from the List
		 */
		auto Pop() noexcept -> void;
		/**
		 * Remove the first element from the List
		 */
		auto PopFront() noexcept -> void;
		/**
		 * Erase an element from the List
		 * \param[in] it Iterator to element before the element to erase
		 */
		auto EraseAfter(const Iterator& it) noexcept -> void;
		/**
		 * Erase a number of elements from the List
		 * \param[in] it Iterator to first element before the element to erase
		 * \param[in] count Number of elements to erase
		 */
		auto EraseAfter(const Iterator& it, usize count) noexcept -> void;
		/**
		 * Erase a range of elements from the List
		 * \param[in] begin Iterator to first element before the element to erase
		 * \param[in] end Iterator to last element to erase
		 */
		auto EraseAfter(const Iterator& begin, const Iterator& end) noexcept -> void;

		/**
		 * Reverse the elements in the list
		 * \note This does not invalidate any iterators
		 */
		auto Reverse() noexcept -> void;

		/**
		 * Get the size of the List
		 * \return Size of the List
		 */
		auto Size() const noexcept -> usize;
		/**
		 * Check if the List is empty
		 * \return Whether the List is empty
		 */
		auto IsEmpty() const noexcept ->  bool;
		/**
		 * Get the allocator used by the List
		 * \return Allocator used by the List
		 */
		auto GetAllocator() const noexcept -> Alloc::IAllocator*;

		/**
		 * Get the first element in the List
		 * \return First element in the List
		 * \note Only use when the List is not empty
		 */
		auto Front() noexcept -> T&;
		/**
		 * Get the first element in the List
		 * \return First element in the List
		 * \note Only use when the List is not empty
		 */
		auto Front() const noexcept -> const T&;
		/**
		 * Get the last element in the List
		 * \return Last element in the List
		 * \note Only use when the List is not empty
		 */
		auto Back() noexcept -> T&;
		/**
		 * Get the last element in the List
		 * \return Last element in the List
		 * \note Only use when the List is not empty
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

		// Overloads for 'for ( ... : ... )'
		auto begin() noexcept -> Iterator;
		auto begin() const noexcept -> ConstIterator;
		auto cbegin() const noexcept -> ConstIterator;
		auto end() noexcept -> Iterator;
		auto end() const noexcept -> ConstIterator;
		auto cend() const noexcept -> ConstIterator;

	private:

		/**
		 * Create an empty node
		 */
		auto CreateNode(T&& val) noexcept -> NodeRef;

		MemRef<Node> m_head;
		MemRef<Node> m_tail;
	};
}

#include "List.inl"