#pragma once
#include "core/MinInclude.h"
#include "core/memory/MemRef.h"
#include "core/allocator/GlobalAlloc.h"
#include "core/utils/Utils.h"

namespace Core
{
	/**
	 * A bucketed hash map
	 * \tparam K Key type (needs to conform to Core::Movable)
	 * \tparam V Value type (needs to conform to Core::Movable)
	 * \tparam H Hasher type
	 * \tparam C Comparator type
	 * \tparam IsMultiMap Whether the HashMap is a MultiMap or not
	 * \note Hash function are expected to have a high amount of randomness, especially in lower bits, since the Hashmap relies on a power of 2 to distribute values
	 */
	template<typename K, typename V, Hasher<K> H = Hash<K>, EqualsComparator<K> C = DefaultEqualComparator<K>, bool IsMultiMap = false>
	class HashMap
	{
		// static assert to get around incomplete type issues when a class can return a HashMap of itself
		STATIC_ASSERT(Movable<K>, "Key type needs to be movable to be used in a HashMap");
		STATIC_ASSERT(Movable<V>, "Value type needs to be movable to be used in a HashMap");
	private:
		/**
		 * HashMap node
		 */
		struct Node
		{
			MemRef<Node> next; ///< Reference to next node in the bucket
			u64          hash; ///< Hash of the key
			Pair<K, V>   pair; ///< Key-value pair
		};
		using NodeRef = MemRef<Node>;

	public:

		/**
		 * HashMap iterator
		 */
		class Iterator
		{
		public:
			Iterator() noexcept = default;

			auto operator->() const noexcept -> Pair<const K, V>*;
			auto operator*() const noexcept -> Pair<const K, V>&;

			auto operator++() noexcept -> Iterator;
			auto operator++(int) noexcept -> Iterator;

			auto operator+(usize count) const noexcept -> Iterator;

			auto operator+=(usize count) noexcept -> Iterator&;

			auto operator==(const Iterator & other) const noexcept -> bool;
			auto operator!=(const Iterator & other) const noexcept -> bool;

		private:
			Iterator(const MemRef<NodeRef>& buckets, usize bucketIdx, const NodeRef& node) noexcept;

			MemRef<NodeRef> m_buckets;
			usize           m_bucketIdx;
			NodeRef         m_node;

			friend class HashMap;
		};
		using ConstIterator = const Iterator;

	public:
		/**
		 * Create a HashMap
		 * \param[in]alloc Allocator the container should use
		 */
		explicit HashMap(Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a HashMap
		 * \param[in] minBuckets Minimum number of buckets to create
		 * \param[in] alloc Allocator the container should use 
		 */
		explicit HashMap(usize minBuckets, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a HashMap
		 * \param[in] minBuckets Minimum number of buckets to create
		 * \param[in] hasher Hasher to hash keys with
		 * \param[in] comp Comparator to compare keys with
		 * \param[in] alloc Allocator the container should use
		 */
		explicit HashMap(usize minBuckets, H hasher, C comp, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;

		/**
		 * Create a HashMap
		 * param[in] il Initializer list with elements
		 * \param[in] alloc Allocator the container should use
		 */
		explicit HashMap(const InitializerList<Pair<K, V>>& il, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K> && CopyConstructible<V>;
		/**
		 * Create a HashMap
		 * \param[in] il Initializer list with elements
		 * \param[in] minBuckets Minimum number of buckets to create
		 * \param[in] alloc Allocator the container should use
		 */
		explicit HashMap(const InitializerList<Pair<K, V>>& il, usize minBuckets, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * Create a HashMap
		 * \param[in] il Initializer list with elements
		 * \param[in] minBuckets Minimum number of buckets to create
		 * \param[in] il Initializer list with elements
		 * \param[in] hasher Hasher to hash keys with
		 * \param[in] comp Comparator to compare keys with
		 * \param[in] alloc Allocator the container should use
		 */
		explicit HashMap(const InitializerList<Pair<K, V>>& il, usize minBuckets, H hasher, C comp, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>&& CopyConstructible<V>;

		/**
		 * Create a HashMap
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit HashMap(const It& begin, const It& end, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * Create a HashMap
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] minBuckets Minimum number of buckets to create
		 * \param[in]alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit HashMap(const It& begin, const It& end, usize minBuckets, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * Create a HashMap
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] minBuckets Minimum number of buckets to create
		 * \param[in] hasher Hasher to hash keys with
		 * \param[in] comp Comparator to compare keys with
		 * \param[in] alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit HashMap(const It& begin, const It& end, usize minBuckets, H hasher, C comp, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>&& CopyConstructible<V>;

		/**
		 * \brief Create a HashMap with the contents of another HashMap
		 * \param[in] other HashMap to copy
		 */
		HashMap(const HashMap& other) noexcept requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * \brief Create a HashMap with the contents of another HashMap, but with a different allocator
		 * \param[in] other HashMap to copy
		 * \param[in] alloc Allocator the container should use
		 */
		HashMap(const HashMap& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * Move another HashMap into a new HashMap
		 * \param[in] other HashMap to move from
		 */
		HashMap(HashMap&& other) noexcept;
		/**
		 * Move another HashMap into a new HashMap, but with a different allocator
		 * \param[in] other HashMap to move from
		 * \param[in] alloc Allocator the container should use
		 */
		HashMap(HashMap&& other, Alloc::IAllocator& alloc) noexcept;
		~HashMap();

		auto operator=(const InitializerList<Pair<K, V>>& il) noexcept -> HashMap requires CopyConstructible<K>&& CopyConstructible<V>;
		auto operator=(const HashMap& other) noexcept -> HashMap requires CopyConstructible<K>&& CopyConstructible<V>;
		auto operator=(HashMap&& other) noexcept -> HashMap;

		/**
		 * Rehash the HashMap to have a minimum number of buckets
		 * \param[in] count Minimum number of buckets to rehash to
		 */
		auto Rehash(usize count) noexcept -> void;
		/**
		 * Reserve space for a number of elements and rehashes if needed
		 * \param[in] count Number of elements to reserve
		 */
		auto Reserve(usize count) noexcept -> void;

		/**
		 * Insert a key-value pair into the HashMap, override value if it already exists
		 * \param[in] pair Key-value pair to insert
		 * \return A pair with the iterator to the inserted element and a bool, where true means the element was inserted and false if the element was overriden
		 */
		auto Insert(const Pair<K, V>& pair) noexcept -> Pair<Iterator, bool> requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * Insert a key-value pair into the HashMap, override value if it already exists
		 * \param[in] pair Key-value pair to insert
		 * \return A pair with the iterator to the inserted element and a bool, where true means the element was inserted and false if the element was overriden
		 */
		auto Insert(Pair<K, V>&& pair) noexcept -> Pair<Iterator, bool>;
		/**
		 * Insert a key-value pair into the HashMap, override value if it already exists
		 * \param[in] key Key to insert
		 * \param[in] val Value to insert
		 * \return A pair with the iterator to the inserted element and a bool, where true means the element was inserted and false if the element was overriden
		 */
		auto Insert(const K& key, const V& val) noexcept -> Pair<Iterator, bool> requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * Insert a key-value pair into the HashMap, override value if it already exists
		 * \param[in] key Key to insert
		 * \param[in] val Value to insert
		 * \return A pair with the iterator to the inserted element and a bool, where true means the element was inserted and false if the element was overriden
		 */
		auto Insert(K&& key, V&& val) noexcept -> Pair<Iterator, bool>;
		/**
		 * Try to insert a key-value pair into the HashMap
		 * \param[in] pair Key-value pair to insert
		 * \return A pair with the iterator to the inserted element and a bool if the insertion was successful
		 */
		auto TryInsert(const Pair<K, V>& pair) noexcept -> Pair<Iterator, bool> requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * Try to insert a key-value pair into the HashMap
		 * \param[in] pair Key-value pair to insert
		 * \return A pair with the iterator to the inserted element and a bool if the insertion was successful
		 */
		auto TryInsert(Pair<K, V>&& pair) noexcept -> Pair<Iterator, bool>;
		/**
		 * Try to insert a key-value pair into the HashMap
		 * \param[in] key Key to insert
		 * \param[in] val Value to insert
		 * \return A pair with the iterator to the inserted element and a bool if the insertion was successful
		 */
		auto TryInsert(const K& key, const V& val) noexcept -> Pair<Iterator, bool> requires CopyConstructible<K>&& CopyConstructible<V>;
		/**
		 * Try to insert a key-value pair into the HashMap
		 * \param[in] key Key to insert
		 * \param[in] val Value to insert
		 * \return A pair with the iterator to the inserted element and a bool if the insertion was successful
		 */
		auto TryInsert(K&& key, V&& val) noexcept -> Pair<Iterator, bool>;

		/**
		 * Emplace a key-value pair into the HashMap
		 * \tparam Args Type of arguments
		 * \param[in] args Arguments
		 * \return A pair with the iterator to the inserted element and a bool if the insertion was successful
		 */
		template<typename ...Args>
			requires ConstructableFrom<Pair<K, V>, Args...>
		auto Emplace(Args&&... args) noexcept -> Pair<Iterator, bool>;
		/**
		 * Emplace a key-value pair into the HashMap
		 * \tparam Args Type of arguments
		 * \param[in] key Key to insert
		 * \param[in] args Arguments
		 * \return A pair with the iterator to the inserted element and a bool if the insertion was successful
		 */
		template<typename ...Args>
			requires ConstructableFrom<V, Args...>
		auto TryEmplace(const K& key, Args&&... args) noexcept -> Pair<Iterator, bool>;

		/**
		 * \brief Merge another HashMap into this HashMap
		 * Merging 2 HashMaps will move all key-value pairs, where the key does not exist in the HashMap, all other values will remain in the other HashMap
		 * \tparam H2 Hasher type of other
		 * \tparam C2 Comparator type of other
		 * \param[in] other DynArray to merge
		 */
		template<Hasher H2, EqualsComparator C2>
		auto Merge(HashMap<K, V, H2, C2>& other) noexcept -> void;

		/**
		 * Clear the contents of the HashMap, possibly also deallocate the memory
		 * \param[in] clearMemory Whether to deallocate the memory
		 */
		auto Clear(bool clearMemory = false) noexcept -> void;

		/**
		 * Erase an element from the HashMap
		 * \param[in] it Iterator to element to erase
		 * \return Iterator after erased element
		 */
		auto Erase(ConstIterator& it) noexcept -> Iterator;
		/**
		 * Erase an element from the HashMap
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
		 * Check if the HashMap contains a key
		 * \param[in] key Key to find
		 * \return Whether the HashMap contains the key
		 */
		auto Contains(const K& key) const noexcept -> bool;
		/**
		 * Check if the HashMap contains a key
		 * \tparam K2 Type of a value that can be compared to K
		 * \param[in] key Key to find
		 * \return Whether the HashMap contains the key
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
		 * Get the size of the HashMap
		 * \return Size of the HashMap
		 */
		auto Size() const noexcept -> usize;
		/**
		 * Check if the HashMap is empty
		 * \return Whether the HashMap is empty
		 */
		auto IsEmpty() const noexcept -> bool;

		/**
		 * Get the number of buckets in the HashMap
		 * \return Number of buckets in the HashMap
		 */
		auto BucketCount() const noexcept -> usize;
		/**
		 * Get the size of a bucket in bytes
		 * \param[in] idx Index of the bucket
		 * \return Size of the bucket in bytes
		 */
		auto BucketSize(usize idx) const noexcept -> usize;
		/**
		 * Get the index of the bucket containing the key (first bucket in case of a MultiMap)
		 * \param[in] key Key to find
		 * \return Index of the bucket containing the key, ~usize(0) if the key does not exist
		 */
		auto BucketIdx(const K& key) const noexcept -> usize;

		/**
		 * Get the current load factor of the HashMap
		 * \return Current load factor of the HashMap
		 */
		auto LoadFactor() const noexcept -> usize;
		/**
		 * Get the maximum load factor before rehashing
		 * \return Maximum load factor before rehashing
		 */
		auto MaxLoadFactor() const noexcept -> f32;
		/**
		 * Set the maximum load factor before rehashing
		 * \param[in] ml Maximum load factor before rehashing
		 */
		auto SetMaxLoadFactor(f32 ml) noexcept -> void;

		/**
		 * Get the allocator used by the HashMap
		 * \return Allocator used by the HashMap
		 */
		auto GetAllocator() const noexcept -> Alloc::IAllocator*;

		/**
		 * Get the first element in the HashMap
		 * \return First element in the HashMap
		 * \note Only use when the HashMap is not empty
		 */
		auto Front() noexcept -> Pair<K, V>&;
		/**
		 * Get the first element in the HashMap
		 * \return First element in the HashMap
		 * \note Only use when the HashMap is not empty
		 */
		auto Front() const noexcept -> const Pair<K, V>&;
		/**
		 * Get the last element in the HashMap
		 * \return Last element in the HashMap
		 * \note Only use when the HashMap is not empty
		 */
		auto Back() noexcept -> Pair<K, V>&;
		/**
		 * Get the last element in the HashMap
		 * \return Last element in the HashMap
		 * \note Only use when the HashMap is not empty
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

		/**
		 * Create a new node
		 * \param[in]  hash Hash of the node
		 * \param[in]  pair Pair of the node
		 * \return New node
		 */
		auto CreateNode(u64 hash, Pair<K, V>&& pair) noexcept -> NodeRef;
		/**
		 * Insert a node into the HashMap
		 * \tparam AllowOverride Allow overriding of a value
		 * \param[in]  node Node to insert
		 * \return A pair with the iterator to the inserted element and a bool, where true means the element was inserted and false if the element was overriden
		 */
		template<bool AllowOverride>
		auto InsertNode(NodeRef node) noexcept -> Pair<Iterator, bool>;

		/**
		 * Remove a node out of the HashMap
		 * \param[in]  node Node to remove
		 */
		auto RemoveNode(NodeRef node) noexcept -> void;

		/**
		 * Internal clear
		 * \tparam Destruct Whether to destruct the elements
		 * \param[in] clearMemory Whether to deallocate the memory
		 */
		template<bool Destruct>
		auto ClearInternal(bool clearMemory) noexcept -> void;

		/**
		 * Get an iterator to the elements with a hash and a key
		 * \param[in] hash Hash of the key
		 * \param[in] key Key to find
		 * \return Iterator to the found element (first element in case of a MultiMap), or to end when the key wasn't found
		 */
		auto FindWithHash(u64 hash, const K& key) const noexcept -> Iterator;
		/**
		 * Get an iterator to the elements with a key
		 * \tparam K2 Type of a value that can be compared to K
		 * \param[in] key Key to find
		 * \return Iterator to the found element (first element in case of a MultiMap), or to end when the key wasn't found
		 * \note This function is slower when the the key isn't convertible to the key type, as a linear search needs to be done
		 */
		template<EqualComparable<K> K2>
		auto FindOther(const K2& key) const noexcept -> Iterator;
		/**
		 * Find a range of values where the keys match a given hash and key
		 * \param[in] hash Hash of the key
		 * \param[in] key Key to find
		 * \return Pair of iterator, representing the begin and end of the found range
		 */
		auto FindRangeWithHash(u64 hash, const K& key) const noexcept -> Pair<Iterator, Iterator>;
		/**
		 * Find a range of values where the keys match a given key
		 * \tparam K2 Type of a value that can be compared to K
		 * \param[in] key Key to find
		 * \return Pair of iterator, representing the begin and end of the found range
		 * \note This function is slower than using a key of the Key type, as a linear search needs to be done
		 */
		template<EqualComparable<K> K2>
		auto FindRangeOther(const K2& key) const noexcept -> Pair<Iterator, Iterator>;
		
		/**
		 * Get the first node in the HashMap
		 * \return First node in the HashMap
		 */
		auto GetFirstNode() const noexcept -> Pair<usize, NodeRef>;
		/**
		 * Get the last node in the HashMap
		 * \return Last node in the HashMap
		 */
		auto GetLastNode() const noexcept -> NodeRef;

		MemRef<NodeRef>     m_buckets;       ///< Managed memory with buckets
		usize               m_bucketCount;   ///< Number of buckets
		usize               m_size;          ///< Number of elements
		float               m_maxLoadFactor; ///< Maximum load factor
		NO_UNIQUE_ADDRESS H m_hash;          ///< Hasher for keys
		NO_UNIQUE_ADDRESS C m_comp;          ///< Comparator for keys
	};

	template<Movable K, Movable V, Hasher<K> H, EqualsComparator<K> C>
	using HashMultiMap = HashMap<K, V, H, C, true>;
}

#include "HashMap.inl"