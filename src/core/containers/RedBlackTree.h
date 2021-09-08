#pragma once
#include "core/MinInclude.h"
#include "core/allocator/IAllocator.h"
#include "core/utils/Pair.h"

namespace Core
{
	template<Movable T, Comparator<T> C = DefaultComparator<T>, bool AllowMultiple = false>
	class RedBlackTree;

	namespace Detail
	{
		/**
		 * Color of a red-black tree node
		 */
		enum class RedBlackTreeColor : u8
		{
			Black,
			Red,
		};

		template<Movable T, Comparator<T> C, bool AllowMultiple>
		class RedBlackTreeIterator;

		/**
		 * Node of a red black tree
		 * \tparam T Underlying type
		 * \tparam C Comparator
		 * \tparam StoreMultiple Whether to allow multiple values to be store
		 */
		template<Movable T, Comparator<T> C, bool StoreMultiple>
		struct RedBlackTreeNode
		{
		private:
			T value;
			RedBlackTreeColor color;
			MemRef<RedBlackTreeNode> parent;

			union
			{
				struct
				{
					MemRef<RedBlackTreeNode> left;
					MemRef<RedBlackTreeNode> right;
				};
				MemRef<RedBlackTreeNode> children[2];
			};
			
			friend class RedBlackTree<T, C, StoreMultiple>;
			friend class RedBlackTreeIterator<T, C, StoreMultiple>;
		};

		template<Movable T, Comparator<T> C>
		struct RedBlackTreeNode<T, C, true>
		{
		private:
			DynArray<T> value;
			RedBlackTreeColor color;
			MemRef<RedBlackTreeNode> parent;

			union
			{
				struct
				{
					MemRef<RedBlackTreeNode> left;
					MemRef<RedBlackTreeNode> right;
				};
				MemRef<RedBlackTreeNode> children[2];
			};
			
			friend class RedBlackTree<T, C, true>;
			friend class RedBlackTreeIterator<T, C, true>;
		};

		/**
		* An iterator for a red-black tree
		* \tparam T Underlying type
		* \tparam C Comparator
		* \tparam AllowMultiple Whether to allow multiple values per node
		*/
		template<Movable T, Comparator<T> C, bool AllowMultiple>
		class RedBlackTreeIterator
		{
		private:
			using NodeRef = MemRef<RedBlackTreeNode<T, C, AllowMultiple>>;

		public:
			RedBlackTreeIterator() noexcept;
			RedBlackTreeIterator(const RedBlackTreeIterator& other) noexcept;
			RedBlackTreeIterator(RedBlackTreeIterator&& other) noexcept;

			auto operator=(const RedBlackTreeIterator& other) noexcept;
			auto operator=(RedBlackTreeIterator&& other) noexcept;

			auto operator*() const noexcept -> T&;
			auto operator->() const noexcept -> T*;

			auto operator++() noexcept -> RedBlackTreeIterator&;
			auto operator++(int) noexcept -> RedBlackTreeIterator;

			auto operator--() noexcept -> RedBlackTreeIterator&;
			auto operator--(int) noexcept -> RedBlackTreeIterator;

			auto operator+(usize count) const noexcept -> RedBlackTreeIterator;
			auto operator-(usize count) const noexcept -> RedBlackTreeIterator;

			auto operator+=(usize count) noexcept -> RedBlackTreeIterator&;
			auto operator-=(usize count) noexcept -> RedBlackTreeIterator&;

			auto operator==(const RedBlackTreeIterator& other) const noexcept -> bool;
			auto operator!=(const RedBlackTreeIterator& other) const noexcept -> bool;

		private:
			explicit RedBlackTreeIterator(const NodeRef& node, usize idx = 0);

			NodeRef m_node;
			usize m_Idx;

			friend class Core::RedBlackTree<T, C, AllowMultiple>;
		};
	}

	/**
	 * A red-black tree
	 * \tparam T Underlying type
	 * \tparam C Comparator
	 * \tparam AllowMultiple Whether to allow multiple values per node
	 */
	template<Movable T, Comparator<T> C, bool AllowMultiple>
	class RedBlackTree
	{
	private:
		enum class RotateDir : u8
		{
			Left,
			Right,
		};

		using Node = Detail::RedBlackTreeNode<T, C, AllowMultiple>;
		using NodeRef = MemRef<Node>;

	public:
		using Iterator = Detail::RedBlackTreeIterator<T, C, AllowMultiple>;
		using ConstIterator = const Iterator;

	public:
		/**
		 * Create a RedBlackTree with an allocator
		 * \param[in] alloc Allocator the container should use
		 */
		explicit RedBlackTree(Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a RedBlackTree with an allocator
		 * \param[in] comp Comparator to compare keys with
		 * \param[in] alloc Allocator the container should use
		 */
		explicit RedBlackTree(C comp, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;

		/**
		 * Create a RedBlackTree from an initializer list
		 * \param[in] il Initializer list with elements
		 * \param[in] alloc Allocator the container should use
		 */
		explicit RedBlackTree(const InitializerList<T>& il, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructable<T>;
		/**
		 * Create a RedBlackTree from an initializer list
		 * \param[in] il Initializer list with elements
		 * \param[in] alloc Allocator the container should use
		 */
		explicit RedBlackTree(const InitializerList<T>& il, C comp, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructable<T>;
		/**
		 * Create a RedBlackTree from an iterable range
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit RedBlackTree(const It& begin, const It& end, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructable<T>;
		/**
		 * Create a RedBlackTree from an iterable range
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit RedBlackTree(const It& begin, const It& end, C comp, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructable<T>;
		/**
		 * Create a RedBlackTree with the contents of another RedBlackTree
		 * \param[in] other RedBlackTree to copy
		 */
		RedBlackTree(const RedBlackTree& other) noexcept;
		/**
		 * Create a RedBlackTree with the contents of another RedBlackTree, but with a different allocator
		 * \param[in] other RedBlackTree to copy
		 * \param[in] alloc Allocator the container should use
		 */
		RedBlackTree(const RedBlackTree& other, Alloc::IAllocator& alloc) noexcept;
		/**
		 * Move another RedBlackTree into a new RedBlackTree
		 * \param[in] other RedBlackTree to move from
		 */
		RedBlackTree(RedBlackTree&& other) noexcept;
		/**
		 * Move another RedBlackTree into a new RedBlackTree, but with a different allocator
		 * \param[in] other RedBlackTree to move from
		 * \param[in] alloc Allocator the container should use
		 */
		RedBlackTree(RedBlackTree&& other, Alloc::IAllocator& alloc) noexcept;

		auto operator=(const RedBlackTree& other) noexcept -> RedBlackTree&;
		auto operator=(RedBlackTree&& other) noexcept -> RedBlackTree&;

		
		/**
		 * Assign an iterable range to the RedBlackTree
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 */
		template<ForwardIterator It>
		auto Assign(const It& begin, const It& end) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Assign a linked list to the RedBlackTree
		 * \param[in] il Initializer list with elements
		 */
		auto Assign(const InitializerList<T>& il) noexcept -> void requires CopyConstructable<T>;

		/**
		 * Insert a value in the RedBlackTree
		 * \param[in] value Value to insert
		 * \return A pair with the iterator to the inserted element and a bool, where true means the element was inserted
		 */
		auto Insert(const T& value) noexcept -> Pair<Iterator, bool> requires CopyConstructable<T>;
		/**
		 * Insert a value in the RedBlackTree
		 * \param[in] value Value to insert
		 * \return A pair with the iterator to the inserted element and a bool, where true means the element was inserted
		 */
		auto Insert(T&& value) noexcept -> Pair<Iterator, bool>;

		/**
		 * \brief Merge another RedBlackTree into this RedBlackTree
		 * Merging 2 RedBlackTrees will move all values, where the value does not exist in the RedBlackTree, all other values will remain in the other RedBlackTree
		 * \tparam C2 Comparator type of other
		 * \param[in] other RedBlackTree to merge
		 */
		template<Comparator C2>
		auto Merge(RedBlackTree<T, C2>& other) noexcept -> void;

		/**
		 * Clear the contents of the RedBlackTree
		 */
		auto Clear() noexcept -> void;
		/**
		 * Erase an element from the HashMap
		 * \param[in] it Iterator to element to erase
		 * \return Iterator after erased element
		 */
		auto Erase(ConstIterator& it) noexcept -> Iterator;
		/**
		 * Erase an element from the RedBlackTree
		 * \param[in] value Value to remove
		 * \return Iterator after erased value
		 */
		auto Erase(const T& value) noexcept -> Iterator;

		/**
		 * Get an iterator to the elements with a key
		 * \param[in] key Key to find
		 * \return Iterator to the found element (first element in case of a MultiMap), or to end when the key wasn't found
		 */
		auto Find(const T& key) noexcept -> Iterator;
		/**
		 * Get an iterator to the elements with a key
		 * \param[in] key Key to find
		 * \return Iterator to the found element (first element in case of a MultiMap), or to end when the key wasn't found
		 */
		auto Find(const T& key) const noexcept -> ConstIterator;
		/**
		 * Get an iterator to the elements with a key
		 * \tparam T2 Type of a value that can be compared to K
		 * \param[in] key Key to find
		 * \return Iterator to the found element (first element in case of a MultiMap), or to end when the key wasn't found
		 * \note This function is slower when the the key isn't convertible to the key type, as a linear search needs to be done
		 */
		template<OrderedComparable<T> T2>
		auto Find(const T2& key) noexcept -> Iterator;
		/**
		 * Get an iterator to the elements with a key
		 * \tparam T2 Type of a value that can be compared to K
		 * \param[in] key Key to find
		 * \return Iterator to the found element (first element in case of a MultiMap), or to end when the key wasn't found
		 * \note This function is slower when the the key isn't convertible to the key type, as a linear search needs to be done
		 */
		template<OrderedComparable<T> T2>
		auto Find(const T2& key) const noexcept -> ConstIterator;

		/**
		 * Check if the RedBlackTree contains a key
		 * \param[in] key Key to find
		 * \return Whether the RedBlackTree contains the key
		 */
		auto Contains(const T& key) const noexcept -> bool;
		/**
		 * Check if the RedBlackTree contains a key
		 * \tparam T2 Type of a value that can be compared to K
		 * \param[in] key Key to find
		 * \return Whether the RedBlackTree contains the key
		 * \note This function is slower than using a key of the Key type, as a linear search needs to be done
		 */
		template<OrderedComparable<T> T2>
		auto Contains(const T2& key) const noexcept -> bool;

		/**
		 * Get the size of the RedBlackTree
		 * \return Size of the RedBlackTree
		 */
		auto Size() const noexcept -> usize;
		/**
		 * Check if the RedBlackTree is empty
		 * \return Whether the RedBlackTree is empty
		 */
		auto IsEmpty() const noexcept -> bool;

		/**
		 * Get the allocator used by the RedBlackTree
		 * \return Allocator used by the RedBlackTree
		 */
		auto GetAllocator() const noexcept -> Alloc::IAllocator*;

		/**
		 * Get the first element in the RedBlackTree
		 * \return First element in the RedBlackTree
		 * \note Only use when the RedBlackTree is not empty
		 */
		auto Front() noexcept -> T&;
		/**
		 * Get the first element in the RedBlackTree
		 * \return First element in the RedBlackTree
		 * \note Only use when the RedBlackTree is not empty
		 */
		auto Front() const noexcept -> const T&;
		/**
		 * Get the last element in the RedBlackTree
		 * \return Last element in the RedBlackTree
		 * \note Only use when the RedBlackTree is not empty
		 */
		auto Back() noexcept -> T&;
		/**
		 * Get the last element in the RedBlackTree
		 * \return Last element in the RedBlackTree
		 * \note Only use when the RedBlackTree is not empty
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

	private:

		/**
		 * Create a new node with a value
		 * \param[in] val Node value
		 * \return New node
		 */
		auto CreateNode(T&& val) noexcept -> NodeRef;

		/**
		 * Rotate a subtree around a node
		 * \param[in] parent Node to rotate around
		 * \param[in] dir Direction to rotate
		 * \return New root of subtree
		 */
		auto Rotate(NodeRef& parent, RotateDir dir) noexcept -> NodeRef;

		/**
		 * Initial case of rebalancing: node is red
		 * \param[in] node Node to rebalance
		 */
		auto RebalanceInsert(NodeRef node, RotateDir dir) noexcept -> void;

		/**
		 * Copy from another red black tree
		 * \param[in] other Red black tree to copy
		 */
		template<bool Moved>
		auto Copy(const RedBlackTree& other) noexcept -> void;

		/**
		 * Erase a value from the RedBlackTree
		 * \tparam Destruct Whether to destruct the value
		 * \param[in] it Iterator to node to erase
		 * \return Iterator after erased value
		 */
		template<bool Destruct>
		auto EraseInternal(Iterator it) noexcept -> Iterator;

		/**
		 * Removed a node from the RedBlackTree
		 * \param[in] node Node to erase
		 * \return Erased node
		 */
		auto EraseNode(NodeRef node) noexcept -> NodeRef;

		/**
		 * Removed a leaf node from the RedBlackTree
		 * \param[in] node Leaf node to erase
		 */
		auto EraseLeafNode(NodeRef node) noexcept -> void;

		template<bool Destruct>
		auto ClearInternal() noexcept -> void;

		/**
		 * Get the first node in the RedBlackTree
		 * \return First node
		 */
		auto GetFirstNode() const noexcept -> NodeRef;
		/**
		 * Get the last node in the RedBlackTree
		 * \return Last node
		 */
		auto GetLastNode() const noexcept -> NodeRef;

		/**
		 * Compare the value in a NodeRef with a value
		 * \param node Node to compare
		 * \param val Value to compare with
		 * \return -1 if less, 1 if greater, and otherwise 0
		 */
		auto Compare(const T& val, const NodeRef& node) const noexcept -> i8;

		NodeRef             m_root; ///< Root node
		usize               m_size; ///< Size
		NO_UNIQUE_ADDRESS C m_comp; ///< Comparator
	};
}

#include "RedBlackTree.inl"