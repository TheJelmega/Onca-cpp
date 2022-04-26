#pragma once
#include "core/MinInclude.h"
#include "core/allocator/GlobalAlloc.h"
#include "core/memory/MemRef.h"

namespace Core
{

	/**
	 * Linked List
	 * \tparam T Stored type (needs to conform to Core::Movable)
	 */
	template<typename T>
	class List
	{
		// static assert to get around incomplete type issues when a class can return a List of itself
		STATIC_ASSERT(Movable<T>, "Type needs to be movable to be used in a List");
	private:

		/**
		 * List node
		 */
		struct Node
		{
			MemRef<Node> next; ///< Reference to next node
			T            val;  ///< Val
		};
		using NodeRef = MemRef<Node>;

	public:

		/**
		 * List iterator
		 */
		class Iterator
		{
		public:
			Iterator() noexcept;

			auto operator->() const noexcept -> T*;
			auto operator*() const noexcept -> T&;

			auto operator++() noexcept -> Iterator;
			auto operator++(int) noexcept -> Iterator;

			auto operator+(usize count) const noexcept -> Iterator;
			auto operator+=(usize count) noexcept -> Iterator&;

			auto operator==(const Iterator& other) const noexcept -> bool;
			auto operator!=(const Iterator& other) const noexcept -> bool;

		private:
			explicit Iterator(const MemRef<Node>& node) noexcept;

			NodeRef m_node; ///< Current node

			friend class List;
		};
		using ConstIterator = const Iterator;

	public:
		/**
		 * Create a List with an allocator
		 * \param[in]alloc Allocator the container should use
		 */
		explicit List(Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a List with a number of elements with a default value (via placement new)
		 * \param[in]  count Number of elements
		 * \param[in]  alloc Allcoator the container shoud use
		 */
		explicit List(usize count, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires NoThrowDefaultConstructible<T>;
		/**
		 * Create a List filled with a number of elements
		 * \param[in]  count Number of elements
		 * \param[in]  val Value of elements
		 * \param[in]  alloc Allcoator the container shoud use
		 */
		explicit List(usize count, const T& val, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<T>;;

		/**
		 * Create a List from an initializer list
		 * \param[in] il Initializer list with elements
		 * \param[in] alloc Allocator the containter should use
		 */
		explicit List(const InitializerList<T>& il, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<T>;
		/**
		 * Create a List from an iterable range
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit List(const It& begin, const It& end, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<T>;
		/**
		 * \brief Create a List with the contents of another List
		 * \param[in] other List to copy
		 */
		List(const List& other) noexcept requires CopyConstructible<T>;
		/**
		 * \brief Create a List with the contents of another List, but with a different allocator
		 * \param[in] other List to copy
		 * \param[in] alloc Allocator the container should use
		 */
		explicit List(const List& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>;
		/**
		 * Move another List into a new List
		 * \param[in] other List to move from
		 */
		List(List&& other) noexcept;
		~List() noexcept;

		auto operator=(const InitializerList<T>& il) noexcept -> List<T>& requires CopyConstructible<T>;
		auto operator=(const List& other) noexcept -> List<T>& requires CopyConstructible<T>;
		auto operator=(List&& other) noexcept -> List<T>&;

		/**
		 * \brief Assign an iterable range to the List
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 */
		template<ForwardIterator It>
		void Assign(const It& begin, const It& end) noexcept requires CopyConstructible<T>;
		/**
		 * Assign a linked list to the List
		 * \param[in] il Initializer list with elements
		 */
		void Assign(const InitializerList<T>& il) noexcept requires CopyConstructible<T>;

		/**
		 * Fill the List with a number of elements
		 * \param[in] count Number of elements to fill
		 * \param[in] val Value to fill elements with
		 */
		void Fill(usize count, const T& val) noexcept requires CopyConstructible<T>;
		/**
		 * Fill the List with a number of elements with a default value (via placement new)
		 * \param[in] count Number of elements to fill
		 */
		void FillDefault(usize count) noexcept requires NoThrowDefaultConstructible<T>;

		/**
		 * Resize the List and fill missing elements if needed
		 * \param[in] newSize New size of the DynArray
		 * \param[in] val Value to fill missing elements with
		 */
		void Resize(usize newSize, const T& val) noexcept requires CopyConstructible<T>;
		/**
		 * Resize the List and fill missing elements with a default value (via placement new) if needed
		 * \param[in] newSize New size of the DynArray
		 */
		void Resize(usize newSize) noexcept requires NoThrowDefaultConstructible<T>;

		/**
		 * Add an element to the List
		 * \param[in] val Element to add
		 */
		void Add(const T& val) noexcept requires CopyConstructible<T>;
		/**
		 * Add an element to the List
		 * \param[in] val Element to add
		 */
		void Add(T&& val) noexcept;
		/**
		 * Add the contents of a List to the List
		 * \param[in] other List to add
		 */
		void Add(const List& other) requires CopyConstructible<T>;
		/**
		 * Add the contents of a List to the List
		 * \param[in] other List to add
		 */
		void Add(List&& other);

		/**
		 * Emplace an element at the back of the List
		 * \tparam Args Type of arguments
		 * \param[in] args Arguments
		 */
		template<typename ...Args>
			requires ConstructableFrom<T, Args...>
		void EmplaceBack(Args&&... args) noexcept;

		/**
		 * Insert an element after a certain location
		 * \param[in] it Iterator to position before the element to insert the element at
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto InsertAfter(const ConstIterator& it, const T& val) noexcept -> Iterator requires CopyConstructible<T>;
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
		auto InsertAfter(const ConstIterator& it, usize count, const T& val) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert an iterable range into the List after a certain location
		 * \tparam It Iterator type
		 * \param[in] it Iterator to position before the element to insert elements at
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \return Iterator to the first element that was inserted
		 */
		template<ForwardIterator It>
		auto InsertAfter(const ConstIterator& it, const It& begin, const It& end) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert an initializer list into the List after a certain location
		 * \param[in] it Iterator to position before the element to insert elements at
		 * \param[in] il Initializer list to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto InsertAfter(const ConstIterator& it, const InitializerList<T>& il) noexcept -> Iterator requires CopyConstructible<T>;
		/**
		 * Insert a List into the List after a certain location
		 * \param[in] it Iterator to position before the element to insert elements at
		 * \param[in] other List to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto InsertAfter(const ConstIterator& it, const List& other) noexcept -> Iterator requires CopyConstructible<T>;
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
			requires ConstructableFrom<T, Args...>
		auto EmplaceAfter(const ConstIterator& it, Args&&... args) noexcept -> Iterator;

		/**
		 * Add an element at the front of the List
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		void AddFront(const T& val) noexcept requires CopyConstructible<T>;
		/**
		 * Add an element at the front of the List
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		void AddFront(T&& val) noexcept;
		/**
		 * Add a number of elements at the front of the List
		 * \param[in] count Number of elements to insert
		 * \param[in] val Value of elements to insert
		 * \return Iterator to the first element that was inserter
		 */
		void AddFront(usize count, const T& val) noexcept requires CopyConstructible<T>;
		/**
		 * Add an iterable range at the front of the List
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \return Iterator to the first element that was inserted
		 */
		template<ForwardIterator It>
		void AddFront(const It& begin, const It& end) noexcept requires CopyConstructible<T>;
		/**
		 * Add an initializer list at the front of the List
		 * \param[in] il Initializer list to insert
		 * \return Iterator to the first element that was inserted
		 */
		void AddFront(const InitializerList<T>& il) noexcept requires CopyConstructible<T>;
		/**
		 * Add a List into the List at the front of the List
		 * \param[in] other List to insert
		 * \return Iterator to the first element that was inserted
		 */
		void AddFront(const List& other) noexcept requires CopyConstructible<T>;
		/**
		 * Add a List into the List at the front of the List
		 * \param[in] other List to insert
		 * \return Iterator to the first element that was inserted
		 */
		void AddFront(List&& other) noexcept;

		/**
		 * Emplace an element at the front of the List
		 * \tparam Args Type of arguments
		 * \param[in] args Arguments
		 */
		template<typename ...Args>
			requires ConstructableFrom<T, Args...>
		void EmplaceFront(Args&&... args) noexcept;

		/**
		 * Clear the contents of the List
		 */
		void Clear() noexcept;
		/**
		 * Remove the last element from the List
		 */
		void Pop() noexcept;
		/**
		 * Remove the first element from the List
		 */
		void PopFront() noexcept;
		/**
		 * Erase an element from the List
		 * \param[in] it Iterator to element before the element to erase
		 */
		void EraseAfter(const Iterator& it) noexcept;
		/**
		 * Erase a number of elements from the List
		 * \param[in] it Iterator to first element before the element to erase
		 * \param[in] count Number of elements to erase
		 */
		void EraseAfter(const Iterator& it, usize count) noexcept;
		/**
		 * Erase a range of elements from the List
		 * \param[in] begin Iterator to first element before the element to erase
		 * \param[in] end Iterator to last element to erase
		 */
		void EraseAfter(const Iterator& begin, const Iterator& end) noexcept;

		/**
		 * Reverse the elements in the list
		 * \note This does not invalidate any iterators
		 */
		void Reverse() noexcept;

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

		MemRef<Node> m_head; ///< Head of the list
		MemRef<Node> m_tail; ///< Tail of the list
	};
}

#include "List.inl"