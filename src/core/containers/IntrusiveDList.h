#pragma once
#include "core/MinInclude.h"

namespace Core
{
	template<typename T>
	class IntrusiveDList;

	/**
	 * Node in a intrusive linked list
	 * \tparam Owner Type that owns the node
	 * \note This should not be used by any struct/class that can be relocated by memory defragmentation
	 */
	template<typename Owner>
	class IntrusiveDListNode
	{
	public:
		/**
		 * Create an intrusive list node
		 * \param memberOffset Offset to Owner type (use OffsetOf<>());
		 */
		explicit IntrusiveDListNode(usize memberOffset) noexcept;
		IntrusiveDListNode(const IntrusiveDListNode&) noexcept = default;
		IntrusiveDListNode(IntrusiveDListNode&&) noexcept = default;

		auto operator=(const IntrusiveDListNode& other) noexcept -> IntrusiveDListNode&;
		auto operator=(IntrusiveDListNode&& other) noexcept -> IntrusiveDListNode&;

		auto Get() noexcept -> Owner*;
		auto Get() const noexcept -> const Owner*;

	private:
		IntrusiveDListNode* m_pPrev; ///< Pointer to prev node
		IntrusiveDListNode* m_pNext;  ///< Pointer to next node
		usize  m_offset; ///< Offset to Owner

		friend class IntrusiveDList<Owner>;
		friend class IntrusiveDList<Owner>::Iterator;
	};


	/**
	 * An intrusive linked list
	 * \tparam T Underlying type
	 * \note This should not be used by any struct/class that can be relocated by memory defragmentation
	 */
	template<typename T>
	class IntrusiveDList
	{
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
			Iterator(IntrusiveDListNode<T>* pNode);

			IntrusiveDListNode<T>* m_pNode;
		};
		using ConstIterator = const Iterator;

	public:
		IntrusiveDList() noexcept;
		
		/**
		 * Move another IntrusiveDList into a new IntrusiveDList
		 * \param[in] other IntrusiveDList to move from
		 */
		IntrusiveDList(IntrusiveDList&& other) noexcept;
		
		auto operator=(IntrusiveDList&& other) noexcept -> IntrusiveDList<T>&;

		/**
		 * Add an element to the IntrusiveDList
		 * \param[in] val Element to add
		 */
		auto Add(IntrusiveDListNode<T>& val) noexcept -> void;
		/**
		 * Add the contents of a IntrusiveDList to the IntrusiveDList
		 * \param[in] other IntrusiveDList to add
		 */
		auto Add(IntrusiveDList&& other) -> void;

		/**
		 * Insert an element after a certain location
		 * \param[in] it Iterator to position before the element to insert the element at
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto Insert(Iterator& it, IntrusiveDListNode<T>& val) noexcept -> Iterator;
		/**
		 * Insert a IntrusiveDList into the IntrusiveDList after a certain location
		 * \param[in] it Iterator to position before the element to insert elements at
		 * \param[in] other IntrusiveDList to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto Insert(Iterator& it, IntrusiveDList&& other) noexcept -> Iterator;

		/**
		 * Add an element at the front of the IntrusiveDList
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto AddFront(IntrusiveDListNode<T>& val) noexcept -> void;
		/**
		 * Add a IntrusiveDList into the IntrusiveDList at the front of the IntrusiveDList
		 * \param[in] other IntrusiveDList to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto AddFront(IntrusiveDList&& other) noexcept -> void;

		/**
		 * Clear the contents of the IntrusiveDList
		 */
		auto Clear() noexcept -> void;
		/**
		 * Remove the last element from the IntrusiveDList
		 */
		auto Pop() noexcept -> void;
		/**
		 * Remove the first element from the IntrusiveDList
		 */
		auto PopFront() noexcept -> void;
		/**
		 * Erase an element from the IntrusiveDList
		 * \param[in] it Iterator to element before the element to erase
		 */
		auto Erase(Iterator& it) noexcept -> void;

		/**
		 * Reverse the elements in the list
		 * \note This does not invalidate any iterators
		 */
		auto Reverse() noexcept -> void;

		/**
		 * Get the size of the IntrusiveDList
		 * \return Size of the IntrusiveDList
		 */
		auto Size() const noexcept -> usize;
		/**
		 * Check if the IntrusiveDList is empty
		 * \return Whether the IntrusiveDList is empty
		 */
		auto IsEmpty() const noexcept ->  bool;

		/**
		 * Get the first element in the IntrusiveDList
		 * \return First element in the IntrusiveDList
		 * \note Only use when the IntrusiveDList is not empty
		 */
		auto Front() noexcept -> T&;
		/**
		 * Get the first element in the IntrusiveDList
		 * \return First element in the IntrusiveDList
		 * \note Only use when the IntrusiveDList is not empty
		 */
		auto Front() const noexcept -> const T&;
		/**
		 * Get the last element in the IntrusiveDList
		 * \return Last element in the IntrusiveDList
		 * \note Only use when the IntrusiveDList is not empty
		 */
		auto Back() noexcept -> T&;
		/**
		 * Get the last element in the IntrusiveDList
		 * \return Last element in the IntrusiveDList
		 * \note Only use when the IntrusiveDList is not empty
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
		IntrusiveDListNode<T>* m_pHead;
		IntrusiveDListNode<T>* m_pTail;
	};


}

#include "IntrusiveDList.inl"