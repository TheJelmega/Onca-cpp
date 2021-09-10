#pragma once
#include "core/MinInclude.h"
#include "core/memory/MemRef.h"
#include "core/allocator/GlobalAlloc.h"
#include "core/utils/Utils.h"
#include "core/containers/RedBlackTree.h"
#include "core/utils/Pair.h"

namespace Core
{

	/**
	 * A sorted hash set
	 * \tparam K Key type
	 * \tparam C Comparator type
	 */
	template<Movable K, Comparator<K, K> C = DefaultComparator<K>, bool IsMultiSet = false>
	class SortedSet
	{
	private:
		using RBTree = RedBlackTree<K, C>;

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

			friend class SortedSet;
		};
		using ConstIterator = const Iterator;

	public:
		/**
		 * Create a SortedSet
		 * \param[in]alloc Allocator the container should use
		 */
		explicit SortedSet(Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a SortedSet
		 * \param[in] comp Comparator to compare keys with
		 * \param[in] alloc Allocator the container should use
		 */
		explicit SortedSet(C comp, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;

		/**
		 * Create a SortedSet
		 * param[in] il Initializer list with elements
		 * \param[in] alloc Allocator the container should use
		 */
		explicit SortedSet(const InitializerList<K>& il, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>;
		/**
		 * Create a SortedSet
		 * \param[in] il Initializer list with elements
		 * \param[in] minBuckets Minimum number of buckets to create
		 * \param[in] il Initializer list with elements
		 * \param[in] hasher Hasher to hash keys with
		 * \param[in] comp Comparator to compare keys with
		 * \param[in] alloc Allocator the container should use
		 */
		explicit SortedSet(const InitializerList<K>& il, C comp, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>;

		/**
		 * Create a SortedSet
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit SortedSet(const It& begin, const It& end, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>;
		/**
		 * Create a SortedSet
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] comp Comparator to compare keys with
		 * \param[in] alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit SortedSet(const It& begin, const It& end, C comp, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructible<K>;

		/**
		 * \brief Create a SortedSet with the contents of another SortedSet
		 * \param[in] other SortedSet to copy
		 */
		SortedSet(const SortedSet& other) noexcept requires CopyConstructible<K>;
		/**
		 * \brief Create a SortedSet with the contents of another SortedSet, but with a different allocator
		 * \param[in] other SortedSet to copy
		 * \param[in] alloc Allocator the container should use
		 */
		SortedSet(const SortedSet& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K>;
		/**
		 * Move another SortedSet into a new SortedSet
		 * \param[in] other SortedSet to move from
		 */
		SortedSet(SortedSet&& other) noexcept;
		/**
		 * Move another SortedSet into a new SortedSet, but with a different allocator
		 * \param[in] other SortedSet to move from
		 * \param[in] alloc Allocator the container should use
		 */
		SortedSet(SortedSet&& other, Alloc::IAllocator& alloc) noexcept;

		auto operator=(const InitializerList<K>& il) noexcept -> SortedSet requires CopyConstructible<K>;
		auto operator=(const SortedSet& other) noexcept -> SortedSet requires CopyConstructible<K>;
		auto operator=(SortedSet&& other) noexcept -> SortedSet;

		/**
		 * Insert a key-value pair into the SortedSet, override value if it already exists
		 * \param[in] key Key to insert
		 * \return A pair with the iterator to the inserted element and a bool, telling if the insertion was successful (i.e. if the key didn't exist yet
		 */
		auto Insert(const K& key) noexcept -> Pair<ConstIterator, bool> requires CopyConstructible<K>;
		/**
		 * Insert a key-value pair into the SortedSet, override value if it already exists
		 * \param[in] key Key to insert
		 * \return A pair with the iterator to the inserted element and a bool, telling if the insertion was successful
		 */
		auto Insert(K&& key) noexcept -> Pair<ConstIterator, bool>;

		/**
		 * Emplace a key-value pair into the SortedSet
		 * \tparam Args Type of arguments
		 * \param[in] args Arguments
		 * \return A pair with the iterator to the inserted element and a bool telling if the insertion was successful
		 */
		template<typename ...Args>
		requires ConstructableFrom<K, Args...>
			auto Emplace(Args&&... args) noexcept -> Pair<ConstIterator, bool>;

		/**
		 * \brief Merge another SortedSet into this SortedSet
		 * Merging 2 SortedSets will move all key-value pairs, where the key does not exist in the SortedSet, all other values will remain in the other SortedSet
		 * \tparam H2 Hasher type of other
		 * \tparam C2 Comparator type of other
		 * \param[in] other DynArray to merge
		 */
		template<Comparator C2>
		auto Merge(SortedSet<K, C2>& other) noexcept -> void;

		/**
		 * Clear the contents of the SortedSet, possibly also deallocate the memory
		 * \param[in] clearMemory Whether to deallocate the memory
		 */
		auto Clear() noexcept -> void;

		/**
		 * Erase an element from the SortedSet
		 * \param[in] it Iterator to element to erase
		 * \return Iterator after erased element
		 */
		auto Erase(ConstIterator& it) noexcept -> Iterator;
		/**
		 * Erase an element from the SortedSet
		 * \param[in] key Key to value to remove
		 * \return Number of elements removed
		 */
		auto Erase(const K& key) noexcept -> usize;

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
		 * Check if the SortedSet contains a key
		 * \param[in] key Key to find
		 * \return Whether the SortedSet contains the key
		 */
		auto Contains(const K& key) const noexcept -> bool;
		/**
		 * Check if the SortedSet contains a key
		 * \tparam K2 Type of a value that can be compared to K
		 * \param[in] key Key to find
		 * \return Whether the SortedSet contains the key
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
		 * Get the size of the SortedSet
		 * \return Size of the SortedSet
		 */
		auto Size() const noexcept -> usize;
		/**
		 * Check if the SortedSet is empty
		 * \return Whether the SortedSet is empty
		 */
		auto IsEmpty() const noexcept -> bool;
			
		/**
		 * Get the allocator used by the SortedSet
		 * \return Allocator used by the SortedSet
		 */
		auto GetAllocator() const noexcept -> Alloc::IAllocator*;

		/**
		 * Get the first element in the SortedSet
		 * \return First element in the SortedSet
		 * \note Only use when the SortedSet is not empty
		 */
		auto Front() const noexcept -> const K&;
		/**
		 * Get the last element in the SortedSet
		 * \return Last element in the SortedSet
		 * \note Only use when the SortedSet is not empty
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

		RBTree m_tree; ///< Underlying RedBlackTree
	};

	template<Movable K, Comparator<K, K> C>
	using SortedMultiSet = SortedSet<K, C, true>;
}

#include "SortedSet.inl"