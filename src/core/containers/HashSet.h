#pragma once
#include "core/MinInclude.h"
#include "core/memory/MemRef.h"
#include "core/allocator/GlobalAlloc.h"
#include "core/utils/Utils.h"
#include "core/containers/HashMap.h"
#include "core/utils/Pair.h"

namespace Core
{

	/**
	 * A bucketed hash map
	 * \tparam K Key type (needs to conform to Core::Movable)
	 * \tparam H Hasher type
	 * \tparam C Comparator type
	 * \tparam IsMultiMap Whether the HashSet is a MultiMap or not
	 * \note Hash function are expected to have a high amount of randomness, especially in lower bits, since the Hashmap relies on a power of 2 to distribute values
	 */
	template<typename K, Hasher<K> H = Hash<K>, EqualsComparator<K> C = DefaultEqualComparator<K>, bool IsMultiMap = false>
	class HashSet
	{
		// static assert to get around incomplete type issues when a class can return a HashSet of itself
		STATIC_ASSERT(Movable<K>, "Type needs to be movable to be used in n HashSet");
	private:

		using Map = HashMap<K, Empty, H, C, IsMultiMap>;

	public:
		/**
		 * HashSet iterator
		 */
		class Iterator
		{
		public:
			Iterator() noexcept = default;

			auto operator->() const noexcept -> const K*;
			auto operator*() const noexcept -> const K&;

			auto operator++() noexcept -> Iterator;
			auto operator++(int) noexcept -> Iterator;

			auto operator+(usize count) const noexcept -> Iterator;

			auto operator+=(usize count) noexcept -> Iterator&;

			auto operator==(const Iterator& other) const noexcept -> bool;
			auto operator!=(const Iterator& other) const noexcept -> bool;

		private:
			Iterator(const typename Map::Iterator& it) noexcept;

			typename Map::Iterator m_it; ///< Underlying iterator

			friend class HashSet;
		};
		using ConstIterator = const Iterator;

	public:
		/**
		 * Create a HashSet
		 * \param[in]alloc Allocator the container should use
		 */
		explicit HashSet(Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a HashSet
		 * \param[in] minBuckets Minimum number of buckets to create
		 * \param[in] alloc Allocator the container should use
		 */
		explicit HashSet(usize minBuckets, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a HashSet
		 * \param[in] minBuckets Minimum number of buckets to create
		 * \param[in] hasher Hasher to hash keys with
		 * \param[in] comp Comparator to compare keys with
		 * \param[in] alloc Allocator the container should use
		 */
		explicit HashSet(usize minBuckets, H hasher, C comp, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;

		/**
		 * Create a HashSet
		 * param[in] il Initializer list with elements
		 * \param[in] alloc Allocator the container should use
		 */
		explicit HashSet(const InitializerList<K>& il, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>;
		/**
		 * Create a HashSet
		 * \param[in] il Initializer list with elements
		 * \param[in] minBuckets Minimum number of buckets to create
		 * \param[in] alloc Allocator the container should use
		 */
		explicit HashSet(const InitializerList<K>& il, usize minBuckets, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>;
		/**
		 * Create a HashSet
		 * \param[in] il Initializer list with elements
		 * \param[in] minBuckets Minimum number of buckets to create
		 * \param[in] il Initializer list with elements
		 * \param[in] hasher Hasher to hash keys with
		 * \param[in] comp Comparator to compare keys with
		 * \param[in] alloc Allocator the container should use
		 */
		explicit HashSet(const InitializerList<K>& il, usize minBuckets, H hasher, C comp, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>;

		/**
		 * Create a HashSet
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit HashSet(const It& begin, const It& end, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>;
		/**
		 * Create a HashSet
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] minBuckets Minimum number of buckets to create
		 * \param[in]alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit HashSet(const It& begin, const It& end, usize minBuckets, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>;
		/**
		 * Create a HashSet
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] minBuckets Minimum number of buckets to create
		 * \param[in] hasher Hasher to hash keys with
		 * \param[in] comp Comparator to compare keys with
		 * \param[in] alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit HashSet(const It& begin, const It& end, usize minBuckets, H hasher, C comp, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>;

		/**
		 * \brief Create a HashSet with the contents of another HashSet
		 * \param[in] other HashSet to copy
		 */
		HashSet(const HashSet& other) noexcept requires CopyConstructible<K>;
		/**
		 * \brief Create a HashSet with the contents of another HashSet, but with a different allocator
		 * \param[in] other HashSet to copy
		 * \param[in] alloc Allocator the container should use
		 */
		HashSet(const HashSet& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K>;
		/**
		 * Move another HashSet into a new HashSet
		 * \param[in] other HashSet to move from
		 */
		HashSet(HashSet&& other) noexcept;
		/**
		 * Move another HashSet into a new HashSet, but with a different allocator
		 * \param[in] other HashSet to move from
		 * \param[in] alloc Allocator the container should use
		 */
		HashSet(HashSet&& other, Alloc::IAllocator& alloc) noexcept;

		auto operator=(const InitializerList<K>& il) noexcept -> HashSet requires CopyConstructible<K>;
		auto operator=(const HashSet& other) noexcept -> HashSet requires CopyConstructible<K>;
		auto operator=(HashSet&& other) noexcept -> HashSet;

		/**
		 * Rehash the HashSet to have a minimum number of buckets
		 * \param[in] count Minimum number of buckets to rehash to
		 */
		auto Rehash(usize count) noexcept -> void;
		/**
		 * Reserve space for a number of elements and rehashes if needed
		 * \param[in] count Number of elements to reserve
		 */
		auto Reserve(usize count) noexcept -> void;
		
		/**
		 * Insert a key-value pair into the HashSet, override value if it already exists
		 * \param[in] key Key to insert
		 * \return A pair with the iterator to the inserted element and a bool, telling if the insertion was successful (i.e. if the key didn't exist yet
		 */
		auto Insert(const K& key) noexcept -> Pair<ConstIterator, bool> requires CopyConstructible<K>;
		/**
		 * Insert a key-value pair into the HashSet, override value if it already exists
		 * \param[in] key Key to insert
		 * \return A pair with the iterator to the inserted element and a bool, telling if the insertion was successful
		 */
		auto Insert(K&& key) noexcept -> Pair<ConstIterator, bool>;

		/**
		 * Emplace a key-value pair into the HashSet
		 * \tparam Args Type of arguments
		 * \param[in] args Arguments
		 * \return A pair with the iterator to the inserted element and a bool telling if the insertion was successful
		 */
		template<typename ...Args>
		requires ConstructableFrom<K, Args...>
			auto Emplace(Args&&... args) noexcept -> Pair<ConstIterator, bool>;

		/**
		 * \brief Merge another HashSet into this HashSet
		 * Merging 2 HashSets will move all key-value pairs, where the key does not exist in the HashSet, all other values will remain in the other HashSet
		 * \tparam H2 Hasher type of other
		 * \tparam C2 Comparator type of other
		 * \param[in] other DynArray to merge
		 */
		template<Hasher H2, EqualsComparator C2>
		auto Merge(HashSet<K, H2, C2>& other) noexcept -> void;

		/**
		 * Clear the contents of the HashSet, possibly also deallocate the memory
		 * \param[in] clearMemory Whether to deallocate the memory
		 */
		auto Clear(bool clearMemory = false) noexcept -> void;

		/**
		 * Erase an element from the HashSet
		 * \param[in] it Iterator to element to erase
		 * \return Iterator after erased element
		 */
		auto Erase(ConstIterator& it) noexcept -> Iterator;
		/**
		 * Erase an element from the HashSet
		 * \param[in] key Key to value to remove
		 * \return Number of elements removed
		 */
		auto Erase(const K& key) noexcept -> usize;
		/**
		 * Erase all elements for which the functor return true
		 * \tparam F Functor type
		 * \param[in] fun Functor
		 */
		template<Callable<bool, const K&> F>
		auto EraseIf(F fun) noexcept -> void;
		
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
		auto Find(const K2& key) const noexcept -> ConstIterator;
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
		auto FindRange(const K2& key) const noexcept -> Pair<ConstIterator, ConstIterator>;

		/**
		 * Check if the HashSet contains a key
		 * \param[in] key Key to find
		 * \return Whether the HashSet contains the key
		 */
		auto Contains(const K& key) const noexcept -> bool;
		/**
		 * Check if the HashSet contains a key
		 * \tparam K2 Type of a value that can be compared to K
		 * \param[in] key Key to find
		 * \return Whether the HashSet contains the key
		 * \note This function is slower than using a key of the Key type, as a linear search needs to be done
		 */
		template<EqualComparable<K> K2>
		auto Contains(const K2& key) const noexcept -> bool;

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
		 * Get the size of the HashSet
		 * \return Size of the HashSet
		 */
		auto Size() const noexcept -> usize;
		/**
		 * Check if the HashSet is empty
		 * \return Whether the HashSet is empty
		 */
		auto IsEmpty() const noexcept -> bool;

		/**
		 * Get the number of buckets in the HashSet
		 * \return Number of buckets in the HashSet
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
		 * Get the current load factor of the HashSet
		 * \return Current load factor of the HashSet
		 */
		auto LoadFactor() const noexcept -> f32;
		/**
		 * Get the maximum load factor before rehashing
		 * \return Maximum load factor before rehashing
		 */
		auto MaxLoadFactor() const noexcept -> usize;
		/**
		 * Set the maximum load factor before rehashing
		 * \param[in] ml Maximum load factor before rehashing
		 */
		auto SetMaxLoadFactor(f32 ml) noexcept -> void;

		/**
		 * Get the allocator used by the HashSet
		 * \return Allocator used by the HashSet
		 */
		auto GetAllocator() const noexcept -> Alloc::IAllocator*;
		
		/**
		 * Get the first element in the HashSet
		 * \return First element in the HashSet
		 * \note Only use when the HashSet is not empty
		 */
		auto Front() const noexcept -> const K&;
		/**
		 * Get the last element in the HashSet
		 * \return Last element in the HashSet
		 * \note Only use when the HashSet is not empty
		 */
		auto Back() const noexcept -> const K&;
		
		/**
		 * Get an iterator to the first element
		 * \return Iterator to the first element
		 */
		auto Begin() const noexcept -> ConstIterator;
		
		/**
		 * Get an iterator to the end of the elements
		 * \return Iterator to the end of the elements
		 */
		auto End() const noexcept -> ConstIterator;

		// Overloads for 'for ( ... : ... )'
		auto begin() const noexcept -> ConstIterator;
		auto cbegin() const noexcept -> ConstIterator;
		auto end() const noexcept -> ConstIterator;
		auto cend() const noexcept -> ConstIterator;

	private:

		Map m_hashMap; ///< Underlying HashSet
	};

	template<Movable K, Hasher<K> H, EqualsComparator<K> C>
	using HashMultiSet = HashSet<K, H, C, true>;
}

#include "HashSet.inl"