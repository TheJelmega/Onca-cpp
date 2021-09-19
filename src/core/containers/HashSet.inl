#pragma once
#include "HashSet.h"

namespace Core
{
	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Iterator::operator->() const noexcept -> const K*
	{
		return &m_it->first;
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Iterator::operator*() const noexcept -> const K&
	{
		return m_it->first;
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Iterator::operator++() noexcept -> Iterator
	{
		++m_it;
		return *this;
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Iterator::operator++(int) noexcept -> Iterator
	{
		Iterator it{ m_it };
		++m_it;
		return it;
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Iterator::operator+(usize count) const noexcept -> Iterator
	{
		Iterator it{ m_it };
		for (usize i = 0; i < count; ++i)
			++it;
		return it;
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Iterator::operator+=(usize count) noexcept -> Iterator&
	{
		for (usize i = 0; i < count; ++i)
			operator++();
		return *this;
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Iterator::operator==(const Iterator& other) const noexcept -> bool
	{
		return m_it == other.m_it;
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Iterator::operator!=(const Iterator& other) const noexcept -> bool
	{
		return m_it != other.m_it;
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::Iterator::Iterator(const typename Map::Iterator& it) noexcept
		: m_it(it)
	{
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(Alloc::IAllocator& alloc) noexcept
		: m_hashMap(alloc)
	{
		ASSERT(&alloc, "No allocator supplied to a HashSet/HashMultiSet");
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(usize minBuckets, Alloc::IAllocator& alloc) noexcept
		: m_hashMap(minBuckets, alloc)
	{
		ASSERT(&alloc, "No allocator supplied to a HashSet/HashMultiSet");
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(usize minBuckets, H hasher, C comp, Alloc::IAllocator& alloc) noexcept
		: m_hashMap(minBuckets, Move(hasher), Move(comp), alloc)
	{
		ASSERT(&alloc, "No allocator supplied to a HashSet/HashMultiSet");
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(const InitializerList<K>& il, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K>
		: HashSet(il, 0, H{}, C{}, alloc)
	{
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(const InitializerList<K>& il, usize minBuckets, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K>
		: HashSet(il, minBuckets, H{}, C{}, alloc)
	{
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(const InitializerList<K>& il, usize minBuckets, H hasher, C comp, Alloc::IAllocator& alloc) noexcept
		requires CopyConstructible<K>
		: m_hashMap(minBuckets, Move(hasher), Move(comp), alloc)
	{
		ASSERT(&alloc, "No allocator supplied to a HashMap/HashMultiMap");
		minBuckets = Math::Max(minBuckets, Math::Ceil(il.size() / m_hashMap.MaxLoadFactor()));
		Rehash(minBuckets);
		for (const K& key : il)
			Insert(key);
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	template <ForwardIterator It>
	HashSet<K, H, C, IsMultiMap>::HashSet(const It& begin, const It& end, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K>
		: HashSet(begin, end, 0, H{}, C{}, alloc)
	{
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	template <ForwardIterator It>
	HashSet<K, H, C, IsMultiMap>::HashSet(const It& begin, const It& end, usize minBuckets, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K>
		: HashSet(begin, end, minBuckets, H{}, C{}, alloc)
	{
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	template <ForwardIterator It>
	HashSet<K, H, C, IsMultiMap>::HashSet(const It& begin, const It& end, usize minBuckets, H hasher, C comp, Alloc::IAllocator& alloc) noexcept
		requires CopyConstructible<K>
		: m_hashMap(minBuckets, Move(hasher), Move(comp), alloc)
	{
		ASSERT(&alloc, "No allocator supplied to a HashSet/HashMultiSet");
		if constexpr (ContiguousIterator<It>)
			minBuckets = Math::Max(minBuckets, Math::Ceil((end - begin) / m_hashMap.MaxLoadFactor()));
		Rehash(minBuckets);
		for (It it = begin; it != end; ++it)
			Insert(*it);
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(const HashSet& other) noexcept requires CopyConstructible<K>
		: m_hashMap(other.m_hashMap)
	{
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(const HashSet& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K>
		: m_hashMap(other.m_hashMap, alloc)
	{
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(HashSet&& other) noexcept
		: m_hashMap(Move(other.m_hashMap))
	{
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	HashSet<K, H, C, IsMultiMap>::HashSet(HashSet&& other, Alloc::IAllocator& alloc) noexcept
		: m_hashMap(Move(other.m_hashMap), alloc)
	{
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::operator=(const InitializerList<K>& il) noexcept -> HashSet requires CopyConstructible<K>
	{
		m_hashMap.Clear();
		m_hashMap.Reserve(il.size());
		for (const K& key : il)
			Insert(key);
		return *this;
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::operator=(const HashSet& other) noexcept -> HashSet requires CopyConstructible<K>
	{
		m_hashMap = other.m_hashMap;
		return *this;
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::operator=(HashSet&& other) noexcept -> HashSet
	{
		m_hashMap = Move(other.m_hashMap);
		return *this;
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Rehash(usize count) noexcept -> void
	{
		m_hashMap.Rehash(count);
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Reserve(usize count) noexcept -> void
	{
		m_hashMap.Reserve(count);
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Insert(const K& key) noexcept -> Pair<ConstIterator, bool> requires CopyConstructible<K>
	{
		auto [it, success] = m_hashMap.TryInsert(key, Empty{});
		return Pair{ Iterator{ it }, success };
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Insert(K&& key) noexcept -> Pair<ConstIterator, bool>
	{
		auto [it, success] = m_hashMap.TryInsert(Move(key), Empty{});
		return Pair{ Iterator{ it }, success };
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	template <typename ... Args> requires ConstructableFrom<K, Args...>
	auto HashSet<K, H, C, IsMultiMap>::Emplace(Args&&... args) noexcept -> Pair<ConstIterator, bool>
	{
		auto [it, success] = m_hashMap.TryInsert(K{ args... }, Empty{});
		return Pair{ Iterator{ it }, success };
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	template <Hasher H2, EqualsComparator C2>
	auto HashSet<K, H, C, IsMultiMap>::Merge(HashSet<K, H2, C2>& other) noexcept -> void
	{
		m_hashMap.Merge(other.m_hashMap);
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Clear(bool clearMemory) noexcept -> void
	{
		m_hashMap.Clear(clearMemory);
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Erase(ConstIterator& it) noexcept -> Iterator
	{
		typename Map::Iterator retIt = m_hashMap.Erase(it.m_it);
		return Iterator{ retIt };
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Erase(const K& key) noexcept -> usize
	{
		typename Map::Iterator retIt = m_hashMap.Erase(key);
		return Iterator{ retIt };
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Find(const K& key) const noexcept -> ConstIterator
	{
		typename Map::Iterator retIt = m_hashMap.Find(key);
		return Iterator{ retIt };
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto HashSet<K, H, C, IsMultiMap>::Find(const K2& key) const noexcept -> ConstIterator
	{
		typename Map::Iterator retIt = m_hashMap.Find(key);
		return Iterator{ retIt };
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::FindRange(const K& key) const noexcept -> Pair<ConstIterator, ConstIterator>
	{
		auto [begin, end] = m_hashMap.FindRange(key);
		return Pair{ Iterator{ begin }, Iterator{ end } };
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto HashSet<K, H, C, IsMultiMap>::FindRange(const K2& key) const noexcept -> Pair<ConstIterator, ConstIterator>
	{
		auto [begin, end] = m_hashMap.FindRange(key);
		return Pair{ Iterator{ begin }, Iterator{ end } };
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Contains(const K& key) const noexcept -> bool
	{
		return m_hashMap.Contains(key);
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto HashSet<K, H, C, IsMultiMap>::Contains(const K2& key) const noexcept -> bool
	{
		return m_hashMap.Contains(key);
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Count(const K& key) const noexcept -> usize
	{
		return m_hashMap.Count(key);
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto HashSet<K, H, C, IsMultiMap>::Count(const K2& key) const noexcept -> usize
	{
		return m_hashMap.Count(key);
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Size() const noexcept -> usize
	{
		return m_hashMap.Size();
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::IsEmpty() const noexcept -> bool
	{
		return m_hashMap.IsEmpty();
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::BucketCount() const noexcept -> usize
	{
		return m_hashMap.BucketCount();
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::BucketSize(usize idx) const noexcept -> usize
	{
		return m_hashMap.BucketSize(idx);
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::BucketIdx(const K& key) const noexcept -> usize
	{
		return m_hashMap.BucketIdx(key);
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::LoadFactor() const noexcept -> f32
	{
		return m_hashMap.LoadFactor();
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::MaxLoadFactor() const noexcept -> usize
	{
		return m_hashMap.MaxLoadFactor();
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::SetMaxLoadFactor(f32 ml) noexcept -> void
	{
		m_hashMap.SetMaxLoadFactor(ml);
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::GetAllocator() const noexcept -> Alloc::IAllocator*
	{
		return m_hashMap.GetAllocator();
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Front() const noexcept -> const K&
	{
		return m_hashMap.Front().first;
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Back() const noexcept -> const K&
	{
		return m_hashMap.Back().first;
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::Begin() const noexcept -> ConstIterator
	{
		return Iterator{ m_hashMap.Begin() };
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::End() const noexcept -> ConstIterator
	{
		return Iterator{ m_hashMap.End() };
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::begin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::cbegin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::end() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename K, Hasher<K> H, EqualsComparator<K, K> C, bool IsMultiMap>
	auto HashSet<K, H, C, IsMultiMap>::cend() const noexcept -> ConstIterator
	{
		return End();
	}
}
