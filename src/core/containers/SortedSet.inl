#pragma once
#if __RESHARPER__
#include "SortedSet.h"
#endif


namespace Core
{
	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Iterator::operator->() const noexcept -> const K*
	{
		return m_it.operator->();
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Iterator::operator*() const noexcept -> const K&
	{
		return *m_it;
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Iterator::operator++() noexcept -> Iterator
	{
		return { ++m_it };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Iterator::operator++(int) noexcept -> Iterator
	{
		return { m_it++ };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Iterator::operator--() noexcept -> Iterator&
	{
		return { --m_it };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Iterator::operator--(int) noexcept -> Iterator
	{
		return { m_it-- };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Iterator::operator+(usize count) const noexcept -> Iterator
	{
		return { m_it + count };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Iterator::operator-(usize count) const noexcept -> Iterator
	{
		return { m_it - count };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Iterator::operator+=(usize count) noexcept -> Iterator&
	{
		return { m_it += count };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Iterator::operator-=(usize count) noexcept -> Iterator&
	{
		return { m_it -= count };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Iterator::operator==(const Iterator& other) const noexcept -> bool
	{
		return m_it == other.m_it;
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Iterator::operator!=(const Iterator& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	SortedSet<K, C, IsMultiSet>::Iterator::Iterator(const typename RBTree::Iterator& it) noexcept
		: m_it(it)
	{
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	SortedSet<K, C, IsMultiSet>::SortedSet(Alloc::IAllocator& alloc) noexcept
		: m_tree(alloc)
	{
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	SortedSet<K, C, IsMultiSet>::SortedSet(C comp, Alloc::IAllocator& alloc) noexcept
		: m_tree(Move(comp), alloc)
	{
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	SortedSet<K, C, IsMultiSet>::SortedSet(const InitializerList<K>& il, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K>
		: m_tree(il, alloc)
	{
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	SortedSet<K, C, IsMultiSet>::SortedSet(const InitializerList<K>& il, C comp, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K>
		: m_tree(il, Move(comp), alloc)
	{
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	template <ForwardIterator It>
	SortedSet<K, C, IsMultiSet>::SortedSet(const It& begin, const It& end, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K>
		: m_tree(begin, end, alloc)
	{
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	template <ForwardIterator It>
	SortedSet<K, C, IsMultiSet>::SortedSet(const It& begin, const It& end, C comp, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K>
		: m_tree(begin, end, Move(comp), alloc)
	{
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	SortedSet<K, C, IsMultiSet>::SortedSet(const SortedSet& other) noexcept requires CopyConstructible<K>
		: m_tree(other.m_tree)
	{
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	SortedSet<K, C, IsMultiSet>::SortedSet(const SortedSet& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<K>
		: m_tree(other.m_tree, alloc)
	{
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	SortedSet<K, C, IsMultiSet>::SortedSet(SortedSet&& other) noexcept
		: m_tree(Move(other.m_tree))
	{
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	SortedSet<K, C, IsMultiSet>::SortedSet(SortedSet&& other, Alloc::IAllocator& alloc) noexcept
		: m_tree(Move(other.m_tree), alloc)
	{
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::operator=(const InitializerList<K>& il) noexcept -> SortedSet requires CopyConstructible<K>
	{
		m_tree = il;
		return *this;
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::operator=(const SortedSet& other) noexcept -> SortedSet requires CopyConstructible<K>
	{
		m_tree = other.m_tree;
		return *this;
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::operator=(SortedSet&& other) noexcept -> SortedSet
	{
		m_tree = Move(other.m_tree);
		return *this;
	}
	
	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Insert(const K& key) noexcept -> Pair<ConstIterator, bool> requires CopyConstructible<K>
	{
		auto [it, res] = m_tree.Insert(key);
		return { { it }, res };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Insert(K&& key) noexcept -> Pair<ConstIterator, bool>
	{
		auto [it, res] = m_tree.Insert(Move(key));
		return { { it }, res };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	template <typename ... Args> requires ConstructableFrom<K, Args...>
	auto SortedSet<K, C, IsMultiSet>::Emplace(Args&&... args) noexcept -> Pair<ConstIterator, bool>
	{
		auto [it, res] = m_tree.Insert(K{ args });
		return { { it }, res };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	template <Comparator C2>
	auto SortedSet<K, C, IsMultiSet>::Merge(SortedSet<K, C2>& other) noexcept -> void
	{
		m_tree.Merge(other.m_tree);
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Clear() noexcept -> void
	{
		m_tree.Clear();
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Erase(ConstIterator& it) noexcept -> Iterator
	{
		return m_tree.Erase(it.m_it);
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Erase(const K& key) noexcept -> usize
	{
		return m_tree.Erase(key);
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Find(const K& key) const noexcept -> ConstIterator
	{
		return { m_tree.Find(key) };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	template <EqualComparable<K> K2>
	auto SortedSet<K, C, IsMultiSet>::Find(const K2& key) const noexcept -> ConstIterator
	{
		return { m_tree.Find(key) };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::FindRange(const K& key) const noexcept -> Pair<ConstIterator, ConstIterator>
	{
		auto [begin, end] = m_tree.FindRange(key);
		return { { begin }, { end } };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	template <EqualComparable<K> K2>
	auto SortedSet<K, C, IsMultiSet>::FindRange(const K2& key) const noexcept -> Pair<ConstIterator, ConstIterator>
	{
		auto [begin, end] = m_tree.FindRange(key);
		return { { begin }, { end } };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Contains(const K& key) const noexcept -> bool
	{
		return m_tree.Contains(key);
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	template <EqualComparable<K> K2>
	auto SortedSet<K, C, IsMultiSet>::Contains(const K2& key) const noexcept -> bool
	{
		return m_tree.Contains(key);
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Count(const K& key) const noexcept -> usize
	{
		return m_tree.Count(key);
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	template <EqualComparable<K> K2>
	auto SortedSet<K, C, IsMultiSet>::Count(const K2& key) const noexcept -> usize
	{
		return m_tree.Count(key);
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Size() const noexcept -> usize
	{
		return m_tree.Size();
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::IsEmpty() const noexcept -> bool
	{
		return m_tree.IsEmpty();
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::GetAllocator() const noexcept -> Alloc::IAllocator*
	{
		return m_tree.GetAllocator();
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Front() const noexcept -> const K&
	{
		return m_tree.Front();
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Back() const noexcept -> const K&
	{
		return m_tree.Back();
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::Begin() const noexcept -> ConstIterator
	{
		return { m_tree.Begin() };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::End() const noexcept -> ConstIterator
	{
		return { m_tree.End() };
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::begin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::cbegin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::end() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename K, Comparator<K, K> C, bool IsMultiSet>
	auto SortedSet<K, C, IsMultiSet>::cend() const noexcept -> ConstIterator
	{
		return End();
	}
}
