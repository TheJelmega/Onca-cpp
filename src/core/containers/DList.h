#pragma once
#include "core/MinInclude.h"
#include "core/allocator/IAllocator.h"
#include "core/allocator/GlobalAlloc.h"

namespace Core
{
	/**
	 * Doubly linked DList
	 * \tparam T Stored type
	 */
	template<MoveConstructable T>
	class DList
	{
	private:

		struct Node
		{
			MemRef<Node> next;
			MemRef<Node> prev;
			T val;
		};
		using NodeRef = MemRef<Node>;

	public:

		class Iterator
		{
		public:
			Iterator() noexcept;

			auto operator++() noexcept -> Iterator;
			auto operator++(int) noexcept -> Iterator;
			auto operator--() noexcept -> Iterator;
			auto operator--(int) noexcept -> Iterator;

			auto operator->() const noexcept -> T*;
			auto operator*() const noexcept -> T&;

			auto operator+(usize count) const noexcept -> Iterator;
			auto operator-(usize count) const noexcept -> Iterator;

			auto operator+=(usize count) noexcept -> Iterator&;
			auto operator-=(usize count) noexcept -> Iterator&;

			auto operator==(const Iterator& other) const noexcept -> bool;
			auto operator!=(const Iterator& other) const noexcept -> bool;

		private:
			explicit Iterator(const MemRef<Node>& node) noexcept;

			NodeRef m_node;

			friend class DList;
		};
		using ConstIterator = const Iterator;

	public:
		/**
		 * Create a DList with an allocator
		 * \param alloc Allocator the container should use
		 */
		explicit DList(Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a DList with a number of elements with a default value (via placement new)
		 * \param count Number of elements
		 * \param alloc Allcoator the container shoud use
		 */
		explicit DList(usize count, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires NoThrowDefaultConstructable<T>;
		/**
		 * Create a DList filled with a number of elements
		 * \param count Number of elements
		 * \param val Value of elements
		 * \param alloc Allcoator the container shoud use
		 */
		explicit DList(usize count, const T& val, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructable<T>;

		/**
		 * Create a DList from an initializer list
		 * \param[in] il Initializer list with elements
		 * \param[in] alloc Allocator the containter should use
		 */
		explicit DList(const InitializerList<T>& il, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructable<T>;
		/**
		 * Create a DList from an iterable range
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit DList(const It& begin, const It& end, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructable<T>;
		/**
		 * \brief Create a DList with the contents of another DList
		 * \param[in] other DList to copy
		 */
		DList(const DList& other) noexcept requires CopyConstructable<T>;
		/**
		 * \brief Create a DList with the contents of another DList, but with a different allocator
		 * \param[in] other DList to copy
		 * \param[in] alloc Allocator the container should use
		 */
		explicit DList(const DList& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructable<T>;
		/**
		 * Move another DList into a new DList
		 * \param[in] other DList to move from
		 */
		DList(DList<T>&& other) noexcept;
		~DList() noexcept;

		auto operator=(const InitializerList<T>& il) noexcept -> DList<T>& requires CopyConstructable<T>;
		auto operator=(const DList<T>& other) noexcept -> DList<T>& requires CopyConstructable<T>;
		auto operator=(DList<T>&& other) noexcept -> DList<T>&;

		/**
		 * \brief Assign an iterable range to the DList
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 */
		template<ForwardIterator It>
		auto Assign(const It& begin, const It& end) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Assign a linked list to the DList
		 * \param[in] il Initializer list with elements
		 */
		auto Assign(const InitializerList<T>& il) noexcept -> void requires CopyConstructable<T>;

		/**
		 * Fill the DList with a number of elements
		 * \param[in] count Number of elements to fill
		 * \param[in] val Value to fill elements with
		 */
		auto Fill(usize count, const T& val) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Fill the DList with a number of elements with a default value (via placement new)
		 * \param[in] count Number of elements to fill
		 */
		auto FillDefault(usize count) noexcept -> void requires NoThrowDefaultConstructable<T>;

		/**
		 * Resize the DList and fill missing elements if needed
		 * \param[in] newSize New size of the DynArray
		 * \param[in] val Value to fill missing elements with
		 */
		auto Resize(usize newSize, const T& val) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Resize the DList and fill missing elements with a default value (via placement new) if needed
		 * \param[in] newSize New size of the DynArray
		 */
		auto Resize(usize newSize) noexcept -> void requires NoThrowDefaultConstructable<T>;

		/**
		 * Add an element to the DList
		 * \param[in] val Element to add
		 */
		auto Add(const T& val) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Add an element to the DList
		 * \param[in] val Element to add
		 */
		auto Add(T&& val) noexcept -> void;
		/**
		 * Add the contents of a DList to the DList
		 * \param[in] other DList to add
		 */
		auto Add(const DList& other) -> void requires CopyConstructable<T>;
		/**
		 * Add the contents of a DList to the DList
		 * \param[in] other DList to add
		 */
		auto Add(DList&& other) -> void;

		/**
		 * Emplace an element at the back of the DList
		 * \tparam Args Type of arguments
		 * \param[in] args Arguments
		 */
		template<typename ...Args>
			requires ConstructableFrom<T, Args...>
		auto EmplaceBack(Args&&... args) noexcept -> void;

		/**
		 * Insert an element before a certain location
		 * \param[in] it Iterator to position to insert the element at
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto Insert(const ConstIterator& it, const T& val) noexcept -> Iterator requires CopyConstructable<T>;
		/**
		 * Insert an element before a certain location
		 * \param[in] it Iterator to position to insert the element at
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto Insert(const ConstIterator& it, T&& val) noexcept -> Iterator;
		/**
		 * Insert a number of elements in the DList before a certain location
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] count Number of elements to insert
		 * \param[in] val Value of elements to insert
		 * \return Iterator to the first element that was inserter
		 */
		auto Insert(const ConstIterator& it, usize count, const T& val) noexcept -> Iterator requires CopyConstructable<T>;
		/**
		 * Insert an iterable range into the DList before a certain location
		 * \tparam It Iterator type
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \return Iterator to the first element that was inserted
		 */
		template<ForwardIterator It>
		auto Insert(const ConstIterator& it, const It& begin, const It& end) noexcept -> Iterator requires CopyConstructable<T>;
		/**
		 * Insert an initializer list into the DList before a certain location
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] il Initializer list to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto Insert(const ConstIterator& it, const InitializerList<T>& il) noexcept -> Iterator requires CopyConstructable<T>;
		/**
		 * Insert a DList into the DList before a certain location
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] other DList to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto Insert(const ConstIterator& it, const DList& other) noexcept -> Iterator requires CopyConstructable<T>;
		/**
		 * Insert a DList into the DList before a certain location
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] other DList to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto Insert(const ConstIterator& it, DList&& other) noexcept -> Iterator;

		/**
		 * Emplace an element into the DList before a certain location
		 * \tparam Args Type of arguments
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] args Arguments
		 */
		template<typename ...Args>
			requires ConstructableFrom<T, Args...>
		auto Emplace(const ConstIterator& it, Args&&... args) noexcept -> Iterator;

		/**
		 * Add an element at the front of the DList
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto AddFront(const T& val) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Add an element at the front of the DList
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto AddFront(T&& val) noexcept -> void;
		/**
		 * Add a number of elements at the front of the DList
		 * \param[in] count Number of elements to insert
		 * \param[in] val Value of elements to insert
		 * \return Iterator to the first element that was inserter
		 */
		auto AddFront(usize count, const T& val) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Add an iterable range at the front of the DList
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \return Iterator to the first element that was inserted
		 */
		template<ForwardIterator It>
		auto AddFront(const It& begin, const It& end) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Add an initializer list at the front of the DList
		 * \param[in] il Initializer list to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto AddFront(const InitializerList<T>& il) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Add a DList into the DList at the front of the DList
		 * \param[in] other DList to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto AddFront(const DList& other) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Add a DList into the DList at the front of the DList
		 * \param[in] other DList to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto AddFront(DList&& other) noexcept -> void;

		/**
		 * Emplace an element at the front of the DList
		 * \tparam Args Type of arguments
		 * \param[in] args Arguments
		 */
		template<typename ...Args>
			requires ConstructableFrom<T, Args...>
		auto EmplaceFront(Args&&... args) noexcept -> void;

		/**
		 * Clear the contents of the DList
		 */
		auto Clear() noexcept -> void;
		/**
		 * Remove the last element from the DList
		 */
		auto Pop() noexcept -> void;
		/**
		 * Remove the first element from the DList
		 */
		auto PopFront() noexcept -> void;
		/**
		 * Erase an element from the DList
		 * \param[in] it Iterator to element before the element to erase
		 */
		auto Erase(const Iterator& it) noexcept -> void;
		/**
		 * Erase a number of elements from the DList
		 * \param[in] it Iterator to first element before the element to erase
		 * \param[in] count Number of elements to erase
		 */
		auto Erase(const Iterator& it, usize count) noexcept -> void;
		/**
		 * Erase a range of elements from the DList
		 * \param[in] begin Iterator to first element before the element to erase
		 * \param[in] end Iterator to last element to erase
		 */
		auto Erase(const Iterator& begin, const Iterator& end) noexcept -> void;

		/**
		 * Reverse the elements in the list
		 * \note This does not invalidate any iterators
		 */
		auto Reverse() noexcept -> void;

		/**
		 * Get the size of the DList
		 * \return Size of the DList
		 */
		auto Size() const noexcept -> usize;
		/**
		 * Check if the DList is empty
		 * \return Whether the DList is empty
		 */
		auto IsEmpty() const noexcept ->  bool;
		/**
		 * Get the allocator used by the DList
		 * \return Allocator used by the DList
		 */
		auto GetAllocator() const noexcept -> Alloc::IAllocator*;

		/**
		 * Get the first element in the DList
		 * \return First element in the DList
		 * \note Only use when the DList is not empty
		 */
		auto Front() noexcept -> T&;
		/**
		 * Get the first element in the DList
		 * \return First element in the DList
		 * \note Only use when the DList is not empty
		 */
		auto Front() const noexcept -> const T&;
		/**
		 * Get the last element in the DList
		 * \return Last element in the DList
		 * \note Only use when the DList is not empty
		 */
		auto Back() noexcept -> T&;
		/**
		 * Get the last element in the DList
		 * \return Last element in the DList
		 * \note Only use when the DList is not empty
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

#include "Dlist.inl"
