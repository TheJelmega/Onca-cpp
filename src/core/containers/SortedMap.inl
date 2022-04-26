#pragma once
#if __RESHARPER__
#include "SortedMap.h"
#endif


namespace Core
{
	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::KeyValueComparator::operator()(const Pair<K, V>& p0, const Pair<K, V>& p1) noexcept -> i8
	{
		return comp(p0.first, p1.first);
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Iterator::operator->() const noexcept -> const K*
	{
		return m_it.operator->();
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Iterator::operator*() const noexcept -> const K&
	{
		return *m_it;
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Iterator::operator++() noexcept -> Iterator
	{
		return { ++m_it };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Iterator::operator++(int) noexcept -> Iterator
	{
		return { m_it++ };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Iterator::operator--() noexcept -> Iterator&
	{
		return { --m_it };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Iterator::operator--(int) noexcept -> Iterator
	{
		return { m_it-- };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Iterator::operator+(usize count) const noexcept -> Iterator
	{
		return { m_it + 1 };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Iterator::operator-(usize count) const noexcept -> Iterator
	{
		return { m_it - 1 };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Iterator::operator+=(usize count) noexcept -> Iterator&
	{
		return { m_it += 1 };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Iterator::operator-=(usize count) noexcept -> Iterator&
	{
		return { m_it -= 1 };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Iterator::operator==(const Iterator& other) const noexcept -> bool
	{
		return m_it == other.m_it;
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Iterator::operator!=(const Iterator& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	SortedMap<K, V, C, IsMultiMap>::Iterator::Iterator(const typename RBTree::Iterator& it) noexcept
		: m_it(it)
	{
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	SortedMap<K, V, C, IsMultiMap>::SortedMap(Alloc::IAllocator& alloc) noexcept
		: m_tree(alloc)
	{
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	SortedMap<K, V, C, IsMultiMap>::SortedMap(C comp, Alloc::IAllocator& alloc) noexcept
		: m_tree(KeyValueComparator{ Move(comp) }, alloc)
	{
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	SortedMap<K, V, C, IsMultiMap>::SortedMap(const InitializerList<Pair<K, V>>& il, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K> &&
		CopyConstructible<V>
		: m_tree(il, alloc)
	{
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	SortedMap<K, V, C, IsMultiMap>::SortedMap(const InitializerList<Pair<K, V>>& il, C comp, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K> &&
		CopyConstructible<V>
		: m_tree(il, KeyValueComparator{ Move(comp) }, alloc)
	{
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	template <ForwardIterator It>
	SortedMap<K, V, C, IsMultiMap>::SortedMap(const It& begin, const It& end, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K> &&
		CopyConstructible<V>
		: m_tree(begin, end, alloc)
	{
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	template <ForwardIterator It>
	SortedMap<K, V, C, IsMultiMap>::SortedMap(const It& begin, const It& end, C comp, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K> &&
		CopyConstructible<V>
		: m_tree(begin, end, KeyValueComparator{ Move(comp) }, alloc)
	{
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	SortedMap<K, V, C, IsMultiMap>::SortedMap(const SortedMap& other) noexcept requires CopyConstructible<K> && CopyConstructible<V>
		: m_tree(other.m_tree)
	{
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	SortedMap<K, V, C, IsMultiMap>::SortedMap(const SortedMap& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K> && CopyConstructible<V>
		: m_tree(other.m_tree, alloc)
	{
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	SortedMap<K, V, C, IsMultiMap>::SortedMap(SortedMap&& other) noexcept
		: m_tree(Move(other.m_tree))
	{
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	SortedMap<K, V, C, IsMultiMap>::SortedMap(SortedMap&& other, Alloc::IAllocator& alloc) noexcept
		: m_tree(Move(other.m_tree), alloc)
	{
	}
	
	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::operator=(const InitializerList<Pair<K, V>>& il) noexcept -> SortedMap requires CopyConstructible<K> &&
		CopyConstructible<V>
	{
		m_tree = il;
		return *this;
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::operator=(const SortedMap& other) noexcept -> SortedMap requires CopyConstructible<K> && CopyConstructible<V>
	{
		m_tree = other.m_tree;
		return *this;
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::operator=(SortedMap&& other) noexcept -> SortedMap
	{
		m_tree = Move(other.m_tree);
		return *this;
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Insert(const Pair<K, V>& pair) noexcept -> Pair<Iterator, bool> requires CopyConstructible<K> && CopyConstructible<V>
	{
		typename RBTree::Iterator it = m_tree.Find(pair);
		if (it != m_tree.End())
		{
			it->second = pair.second;
			return { it };
		}
		return Insert(pair);
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Insert(Pair<K, V>&& pair) noexcept -> Pair<Iterator, bool>
	{
		typename RBTree::Iterator it = m_tree.Find(pair);
		if (it != m_tree.End())
		{
			it->second = Move(pair.second);
			return { it };
		}
		return Insert(Move(pair));
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Insert(const K& key, const V& val) noexcept -> Pair<Iterator, bool> requires CopyConstructible<K> && CopyConstructible<V>
	{
		Pair pair{ key, val };
		typename RBTree::Iterator it = m_tree.Find(pair);
		if (it != m_tree.End())
		{
			it->second = pair.second;
			return { it };
		}
		return Insert(pair);
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Insert(K&& key, V&& val) noexcept -> Pair<Iterator, bool>
	{
		Pair pair{ Move(key), Move(val) };
		typename RBTree::Iterator it = m_tree.Find(pair);
		if (it != m_tree.End())
		{
			it->second = Move(pair.second);
			return { it };
		}
		return Insert(Move(pair));
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::TryInsert(const Pair<K, V>& pair) noexcept -> Pair<Iterator, bool> requires CopyConstructible<K> && CopyConstructible<V>
	{
		return { Insert(pair) };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::TryInsert(Pair<K, V>&& pair) noexcept -> Pair<Iterator, bool>
	{
		return { Insert(Move(pair)) };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::TryInsert(const K& key, const V& val) noexcept -> Pair<Iterator, bool> requires CopyConstructible<K> &&
		CopyConstructible<V>
	{
		return { Insert({ key, val }) };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::TryInsert(K&& key, V&& val) noexcept -> Pair<Iterator, bool>
	{
		return { Insert({ Move(key), (val) }) };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	template <typename ... Args> requires ConstructableFrom<Pair<K, V>, Args...>
	auto SortedMap<K, V, C, IsMultiMap>::Emplace(Args&&... args) noexcept -> Pair<Iterator, bool>
	{
		Pair<K, V> pair{ args... };
		typename RBTree::Iterator it = m_tree.Find(pair);
		if (it != m_tree.End())
		{
			it->second = Move(pair.second);
			return { it };
		}
		return Insert(Move(pair));
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	template <typename ... Args> requires ConstructableFrom<V, Args...>
	auto SortedMap<K, V, C, IsMultiMap>::TryEmplace(const K& key, Args&&... args) noexcept -> Pair<Iterator, bool>
	{
		return { Insert(Pair<K, V>{ args... }) };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	template <Comparator C2>
	void SortedMap<K, V, C, IsMultiMap>::Merge(SortedMap<K, V, C2>& other) noexcept
	{
		m_tree.Merge(other.m_tree);
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	void SortedMap<K, V, C, IsMultiMap>::Clear() noexcept
	{
		m_tree.Clear();
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Erase(ConstIterator& it) noexcept -> Iterator
	{
		return m_tree.Erase(it.m_it);
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Erase(const K& key) noexcept -> usize
	{
		return m_tree.Erase(key);
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Find(const K& key) noexcept -> Iterator
	{
		return { m_tree.Find(key) };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Find(const K& key) const noexcept -> ConstIterator
	{
		return { m_tree.Find(key) };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto SortedMap<K, V, C, IsMultiMap>::Find(const K2& key) noexcept -> Iterator
	{
		return { m_tree.Find(key) };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto SortedMap<K, V, C, IsMultiMap>::Find(const K2& key) const noexcept -> ConstIterator
	{
		return { m_tree.Find(key) };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::FindRange(const K& key) noexcept -> Pair<Iterator, Iterator>
	{
		auto [begin, end] = m_tree.FindRange(key);
		return { { begin }, { end } };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::FindRange(const K& key) const noexcept -> Pair<ConstIterator, ConstIterator>
	{
		auto [begin, end] = m_tree.FindRange(key);
		return { { begin }, { end } };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto SortedMap<K, V, C, IsMultiMap>::FindRange(const K2& key) noexcept -> Pair<Iterator, Iterator>
	{
		auto [begin, end] = m_tree.FindRange(key);
		return { { begin }, { end } };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto SortedMap<K, V, C, IsMultiMap>::FindRange(const K2& key) const noexcept -> Pair<ConstIterator, ConstIterator>
	{
		auto [begin, end] = m_tree.FindRange(key);
		return { { begin }, { end } };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Contains(const K& key) const noexcept -> bool
	{
		return m_tree.Contains(key);
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto SortedMap<K, V, C, IsMultiMap>::Contains(const K2& key) const noexcept -> bool
	{
		return m_tree.Contains(key);
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::At(const K& key) const noexcept -> Optional<V>
	{
		typename RBTree::Iterator it = m_tree.Find(key);
		if (it == m_tree.End())
			return NullOpt;
		return it->second;
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::operator[](const K& key) noexcept -> V&
	{
		typename RBTree::Iterator it = m_tree.Find(key);
		return it->second;
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::operator[](const K& key) const noexcept -> const V&
	{
		typename RBTree::Iterator it = m_tree.Find(key);
		return it->second;
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Count(const K& key) const noexcept -> usize
	{
		return m_tree.Count(key);
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto SortedMap<K, V, C, IsMultiMap>::Count(const K2& key) const noexcept -> usize
	{
		return m_tree.Count(key);
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Size() const noexcept -> usize
	{
		return m_tree.Size();
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::IsEmpty() const noexcept -> bool
	{
		return m_tree.IsEmpty();
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::GetAllocator() const noexcept -> Alloc::IAllocator*
	{
		return m_tree.GetAllocator();
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Front() noexcept -> Pair<K, V>&
	{
		return m_tree.Front();
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Front() const noexcept -> const Pair<K, V>&
	{
		return m_tree.Front();
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Back() noexcept -> Pair<K, V>&
	{
		return m_tree.Back();
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Back() const noexcept -> const Pair<K, V>&
	{
		return m_tree.Back();
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Begin() noexcept -> Iterator
	{
		return { m_tree.Begin() };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::Begin() const noexcept -> ConstIterator
	{
		return { m_tree.Begin() };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::End() noexcept -> Iterator
	{
		return { m_tree.End() };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::End() const noexcept -> ConstIterator
	{
		return { m_tree.End() };
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::begin() noexcept -> Iterator
	{
		return Begin();
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::begin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::cbegin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::end() noexcept -> Iterator
	{
		return End();
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::end() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename K, typename V, Comparator<K> C, bool IsMultiMap>
	auto SortedMap<K, V, C, IsMultiMap>::cend() const noexcept -> ConstIterator
	{
		return End();
	}
}
