#pragma once
#include "core/MinInclude.h"
#include "RedBlackTree.h"
#include "core/allocator/GlobalAlloc.h"
#include "core/utils/Utils.h"

namespace Core
{
	/**
	 * A sorted map
	 * \tparam K Key type (needs to conform to Core::Movable)
	 * \tparam V Value type (needs to conform to Core::Movable)
	 * \tparam C Comparator type
	 * \tparam IsMultiMap Whether the SortedMap is a MultiMap or not
	 *
	 * \note Hash function are expected to have a high amount of randomness, especially in lower bits, since the Hashmap relies on a power of 2 to distribute values
	 */
	template<typename K, typename V, Comparator<K> C = DefaultComparator<K>, bool IsMultiMap = false>
	class SortedMap
	{
		// static assert to get around incomplete type issues when a class can return a SortedMap of itself
		STATIC_ASSERT(Movable<K>, "Key type needs to be movable to be used in a SortedMap");
		STATIC_ASSERT(Movable<V>, "Value type needs to be movable to be used in a SortedMap");
	private:
		struct KeyValueComparator
		{
			auto operator(const Pair<K, V>& p0, const Pair<K, V>& p1) noexcept -> i8;

			NO_UNIQUE_ADDRESS C comp;
		};

		using RBTree = RedBlackTree<Pair<K, V>, KeyValueComparator, IsMultiMap>;

	public:
		class Iterator
		{
		public:
			Iterator() noexcept = default;

			auto operator->() const noexcept -> const K*;
			auto operator*() const noexcept -> const K&;

			auto operator++() noexcept -> Iterator;
			auto operator++(int) noexcept -> Iterator;

			auto operator--() noexcept -> Iterator&;
			auto operator--(int) noexcept -> Iterator;

			auto operator+(usize count) const noexcept -> Iterator;
			auto operator-(usize count) const noexcept -> Iterator;

			auto operator+=(usize count) noexcept -> Iterator&;
			auto operator-=(usize count) noexcept -> Iterator&;

			auto operator==(const Iterator& other) const noexcept -> bool;
			auto operator!=(const Iterator& other) const noexcept -> bool;

		private:
			Iterator(const typename RBTree::Iterator& it) noexcept;

			typename RBTree::Iterator m_it;

			friend class SortedMap;
		};
		using ConstIterator = const Iterator;

	public:
		/**
		 * Create a SortedMap
		 * \param[in]alloc Allocator the container should use
		 */
		explicit SortedMap(Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a SortedMap
		 * \param[in] comp Comparator to compare keys with
		 * \param[in] alloc Allocator the container should use
		 */
		explicit SortedMap(C comp, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;

		/**
		 * Create a SortedMap
		 * param[in] il Initializer list with elements
		 * \param[in] alloc Allocator the container should use
		 */
		explicit SortedMap(const InitializerList<Pair<K, V>>& il, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * Create a SortedMap
		 * \param[in] il Initializer list with elements
		 * \param[in] comp Comparator to compare keys with
		 * \param[in] alloc Allocator the container should use
		 */
		explicit SortedMap(const InitializerList<Pair<K, V>>& il, C comp, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>&& CopyConstructible<V>;

		/**
		 * Create a SortedMap
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit SortedMap(const It& begin, const It& end, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * Create a SortedMap
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] comp Comparator to compare keys with
		 * \param[in] alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit SortedMap(const It& begin, const It& end, C comp, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>&& CopyConstructible<V>;

		/**
		 * \brief Create a SortedMap with the contents of another SortedMap
		 * \param[in] other SortedMap to copy
		 */
		SortedMap(const SortedMap& other) noexcept requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * \brief Create a SortedMap with the contents of another SortedMap, but with a different allocator
		 * \param[in] other SortedMap to copy
		 * \param[in] alloc Allocator the container should use
		 */
		SortedMap(const SortedMap& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * Move another SortedMap into a new SortedMap
		 * \param[in] other SortedMap to move from
		 */
		SortedMap(SortedMap&& other) noexcept;
		/**
		 * Move another SortedMap into a new SortedMap, but with a different allocator
		 * \param[in] other SortedMap to move from
		 * \param[in] alloc Allocator the container should use
		 */
		SortedMap(SortedMap&& other, Alloc::IAllocator& alloc) noexcept;

		auto operator=(const InitializerList<Pair<K, V>>& il) noexcept -> SortedMap requires CopyConstructible<K>&& CopyConstructible<V>;
		auto operator=(const SortedMap& other) noexcept -> SortedMap requires CopyConstructible<K>&& CopyConstructible<V>;
		auto operator=(SortedMap&& other) noexcept -> SortedMap;

		/**
		 * Insert a key-value pair into the SortedMap, override value if it already exists
		 * \param[in] pair Key-value pair to insert
		 * \return A pair with the iterator to the inserted element and a bool, where true means the element was inserted and false if the element was overriden
		 */
		auto Insert(const Pair<K, V>& pair) noexcept -> Pair<Iterator, bool> requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * Insert a key-value pair into the SortedMap, override value if it already exists
		 * \param[in] pair Key-value pair to insert
		 * \return A pair with the iterator to the inserted element and a bool, where true means the element was inserted and false if the element was overriden
		 */
		auto Insert(Pair<K, V>&& pair) noexcept -> Pair<Iterator, bool>;
		/**
		 * Insert a key-value pair into the SortedMap, override value if it already exists
		 * \param[in] key Key to insert
		 * \param[in] val Value to insert
		 * \return A pair with the iterator to the inserted element and a bool, where true means the element was inserted and false if the element was overriden
		 */
		auto Insert(const K& key, const V& val) noexcept -> Pair<Iterator, bool> requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * Insert a key-value pair into the SortedMap, override value if it already exists
		 * \param[in] key Key to insert
		 * \param[in] val Value to insert
		 * \return A pair with the iterator to the inserted element and a bool, where true means the element was inserted and false if the element was overriden
		 */
		auto Insert(K&& key, V&& val) noexcept -> Pair<Iterator, bool>;
		/**
		 * Try to insert a key-value pair into the SortedMap
		 * \param[in] pair Key-value pair to insert
		 * \return A pair with the iterator to the inserted element and a bool if the insertion was successful
		 */
		auto TryInsert(const Pair<K, V>& pair) noexcept -> Pair<Iterator, bool> requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * Try to insert a key-value pair into the SortedMap
		 * \param[in] pair Key-value pair to insert
		 * \return A pair with the iterator to the inserted element and a bool if the insertion was successful
		 */
		auto TryInsert(Pair<K, V>&& pair) noexcept -> Pair<Iterator, bool>;
		/**
		 * Try to insert a key-value pair into the SortedMap
		 * \param[in] key Key to insert
		 * \param[in] val Value to insert
		 * \return A pair with the iterator to the inserted element and a bool if the insertion was successful
		 */
		auto TryInsert(const K& key, const V& val) noexcept -> Pair<Iterator, bool> requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * Try to insert a key-value pair into the SortedMap
		 * \param[in] key Key to insert
		 * \param[in] val Value to insert
		 * \return A pair with the iterator to the inserted element and a bool if the insertion was successful
		 */
		auto TryInsert(K&& key, V&& val) noexcept -> Pair<Iterator, bool>;

		/**
		 * Emplace a key-value pair into the SortedMap
		 * \tparam Args Type of arguments
		 * \param[in] args Arguments
		 * \return A pair with the iterator to the inserted element and a bool if the insertion was successful
		 */
		template<typename ...Args>
		requires ConstructableFrom<Pair<K, V>, Args...>
			auto Emplace(Args&&... args) noexcept -> Pair<Iterator, bool>;
		/**
		 * Emplace a key-value pair into the SortedMap
		 * \tparam Args Type of arguments
		 * \param[in] key Key to insert
		 * \param[in] args Arguments
		 * \return A pair with the iterator to the inserted element and a bool if the insertion was successful
		 */
		template<typename ...Args>
		requires ConstructableFrom<V, Args...>
			auto TryEmplace(const K& key, Args&&... args) noexcept -> Pair<Iterator, bool>;

		/**
		 * \brief Merge another SortedMap into this SortedMap
		 * Merging 2 HashMaps will move all key-value pairs, where the key does not exist in the SortedMap, all other values will remain in the other SortedMap
		 * \tparam C2 Comparator type of other
		 * \param[in] other DynArray to merge
		 */
		template<Comparator C2>
		auto Merge(SortedMap<K, V, C2>& other) noexcept -> void;

		/**
		 * Clear the contents of the SortedMap, possibly also deallocate the memory
		 */
		auto Clear() noexcept -> void;

		/**
		 * Erase an element from the SortedMap
		 * \param[in] it Iterator to element to erase
		 * \return Iterator after erased element
		 */
		auto Erase(ConstIterator& it) noexcept -> Iterator;
		/**
		 * Erase an element from the SortedMap
		 * \param[in] key Key to value to remove
		 * \return Number of elements removed
		 */
		auto Erase(const K& key) noexcept -> usize;

		/**
		 * Get an iterator to the elements with a key
		 * \param[in] key Key to find
		 * \return Iterator to the found element (first element in case of a MultiMap), or to end when the key wasn't found
		 */
		auto Find(const K& key) noexcept -> Iterator;
		/**
		 * Get an iterator to the elements with a key
		 * \param[in] key Key to find
		 * \return Iterator to the found element (first element in case of a MultiMap), or to end when the key wasn't found
		 */
		auto Find(const K& key) const noexcept -> ConstIterator;

		/**
		 * Get an iterator to the elements with a key
		 * \tparam K2 Type of a value that can be compared to K
		 * \param[in] key Key to find
		 * \return Iterator to the found element (first element in case of a MultiMap), or to end when the key wasn't found
		 * \note This function is slower when the the key isn't convertible to the key type, as a linear search needs to be done
		 */
		template<EqualComparable<K> K2>
		auto Find(const K2& key) noexcept -> Iterator;
		/**
		 * Get an iterator to the elements with a key
		 * \tparam K2 Type of a value that can be compared to K
		 * \param[in] key Key to find
		 * \return Iterator to the found element (first element in case of a MultiMap), or to end when the key wasn't found
		 * \note This function is slower when the the key isn't convertible to the key type, as a linear search needs to be done
		 */
		template<EqualComparable<K> K2>
		auto Find(const K2& key) const noexcept -> ConstIterator;
		/**
		 * Find a range of values where the keys match a given key
		 * \param[in] key Key to find
		 * \return Pair of iterator, representing the begin and end of the found range
		 */
		auto FindRange(const K& key) noexcept -> Pair<Iterator, Iterator>;
		/**
		 * Find a range of values where the keys match a given key
		 * \param[in] key Key to find
		 * \return Pair of iterator, representing the begin and end of the found range
		 */
		auto FindRange(const K& key) const noexcept -> Pair<ConstIterator, ConstIterator>;
		/**
		 * Find a range of values where the keys match a given key
		 * \tparam K2 Type of a value that can be compared to K
		 * \param[in] key Key to find
		 * \return Pair of iterator, representing the begin and end of the found range
		 * \note This function is slower than using a key of the Key type, as a linear search needs to be done
		 */
		template<EqualComparable<K> K2>
		auto FindRange(const K2& key) noexcept -> Pair<Iterator, Iterator>;
		/**
		 * Find a range of values where the keys match a given key
		 * \tparam K2 Type of a value that can be compared to K
		 * \param[in] key Key to find
		 * \return Pair of iterator, representing the begin and end of the found range
		 * \note This function is slower than using a key of the Key type, as a linear search needs to be done
		 */
		template<EqualComparable<K> K2>
		auto FindRange(const K2& key) const noexcept -> Pair<ConstIterator, ConstIterator>;

		/**
		 * Check if the SortedMap contains a key
		 * \param[in] key Key to find
		 * \return Whether the SortedMap contains the key
		 */
		auto Contains(const K& key) const noexcept -> bool;
		/**
		 * Check if the SortedMap contains a key
		 * \tparam K2 Type of a value that can be compared to K
		 * \param[in] key Key to find
		 * \return Whether the SortedMap contains the key
		 * \note This function is slower than using a key of the Key type, as a linear search needs to be done
		 */
		template<EqualComparable<K> K2>
		auto Contains(const K2& key) const noexcept -> bool;

		/**
		 * \brief Get the element at a key
		 * \param[in] key Key of the element
		 * \return Optional with value
		 * \note Will return an empty optional when index is out of bounds
		 */
		auto At(const K& key) const noexcept -> Optional<V>;
		/**
		 * \brief Get the element at a key
		 * \param[in] key Key of the element
		 * \return Optional with value
		 * \note Will return an empty optional when index is out of bounds
		 */
		auto operator[](const K& key) noexcept -> V&;
		/**
		 * \brief Get the element at a key
		 * \param[in] key Key of the element
		 * \return Optional with value
		 * \note Will return an empty optional when index is out of bounds
		 */
		auto operator[](const K& key) const noexcept -> const V&;

		/**
		 * \brief Count the number of elements that use a certain key
		 * \param[in] key Key of the element
		 * \return Number of elements with the key
		 */
		auto Count(const K& key) const noexcept -> usize;
		/**
		 * \brief Count the number of elements that use a certain key
		 * \tparam K2 Type of a value that can be compared to K
		 * \param[in] key Key of the element
		 * \return Number of elements with the key
		 * \note This function is slower than using a key of the Key type, as a linear search needs to be done
		 */
		template<EqualComparable<K> K2>
		auto Count(const K2& key) const noexcept -> usize;

		/**
		 * Get the size of the SortedMap
		 * \return Size of the SortedMap
		 */
		auto Size() const noexcept -> usize;
		/**
		 * Check if the SortedMap is empty
		 * \return Whether the SortedMap is empty
		 */
		auto IsEmpty() const noexcept -> bool;

		/**
		 * Get the allocator used by the SortedMap
		 * \return Allocator used by the SortedMap
		 */
		auto GetAllocator() const noexcept -> Alloc::IAllocator*;

		/**
		 * Get the first element in the SortedMap
		 * \return First element in the SortedMap
		 * \note Only use when the SortedMap is not empty
		 */
		auto Front() noexcept -> Pair<K, V>&;
		/**
		 * Get the first element in the SortedMap
		 * \return First element in the SortedMap
		 * \note Only use when the SortedMap is not empty
		 */
		auto Front() const noexcept -> const Pair<K, V>&;
		/**
		 * Get the last element in the SortedMap
		 * \return Last element in the SortedMap
		 * \note Only use when the SortedMap is not empty
		 */
		auto Back() noexcept -> Pair<K, V>&;
		/**
		 * Get the last element in the SortedMap
		 * \return Last element in the SortedMap
		 * \note Only use when the SortedMap is not empty
		 */
		auto Back() const noexcept -> const Pair<K, V>&;

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

		RBTree m_tree; ///< Underlying RedBlackTree
	};

	template<Movable K, Movable V, Comparator<K, K> C>
	using SortedMultiMap = SortedMap<K, V, C, true>;
}

#include "SortedMap.inl"