#pragma once
#include "HashMap.h"
#include "math/MathUtils.h"

namespace Core
{
	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Iterator::operator->() const noexcept -> Pair<K, V>*
	{
		return &m_node->pair;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Iterator::operator*() const noexcept -> Pair<K, V>&
	{
		return m_node->pair;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Iterator::operator++() noexcept -> Iterator
	{
		if (m_node->next)
		{
			m_node = m_node->next;
			return *this;
		}

		NodeRef* pNodes = m_buckets.Ptr();
		usize bucketCount = m_buckets.Size() / sizeof(Node);
		do
		{
			++m_bucketIdx;
			m_node = *(pNodes + m_bucketIdx);
		}
		while (!m_node && m_bucketIdx < bucketCount);

		return *this;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Iterator::operator++(int) noexcept -> Iterator
	{
		Iterator tmp{ m_node };
		operator++();
		return tmp;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Iterator::operator+(usize count) const noexcept -> Iterator
	{
		Iterator it{ m_buckets, m_bucketIdx, m_node };
		for (usize i = 0; i < count; ++i)
			++it;
		return it;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Iterator::operator+=(usize count) noexcept -> Iterator&
	{
		for (usize i = 0; i < count; ++i)
			operator++();
		return *this;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Iterator::operator==(const Iterator& other) const noexcept -> bool
	{
		return m_node == other.m_node;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Iterator::operator!=(const Iterator& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	HashMap<K, V, H, C, IsMultiMap>::Iterator::Iterator(const MemRef<NodeRef>& buckets, usize bucketIdx, const NodeRef& node) noexcept
		: m_buckets(buckets)
		, m_bucketIdx(bucketIdx)
		, m_node(node)
	{
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	HashMap<K, V, H, C, IsMultiMap>::HashMap(Alloc::IAllocator& alloc) noexcept
		: HashMap(0, H{}, C{}, alloc)
	{
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	HashMap<K, V, H, C, IsMultiMap>::HashMap(usize minBuckets, Alloc::IAllocator& alloc) noexcept
		: HashMap(minBuckets, H{}, C{}, alloc)
	{
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	HashMap<K, V, H, C, IsMultiMap>::HashMap(usize minBuckets, H hasher, C comp,
		Alloc::IAllocator& alloc) noexcept
		: m_buckets(&alloc)
		, m_size(0)
		, m_maxLoadFactor(1.0f)
		, m_hash(Move(hasher))
		, m_comp(Move(comp))
	{
		ASSERT(&alloc, "No allocator supplied to a HashMap/HashMultiMap");
		Rehash(minBuckets);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	HashMap<K, V, H, C, IsMultiMap>::HashMap(const InitializerList<Pair<K, V>>& il, Alloc::IAllocator& alloc) noexcept
		requires CopyConstructible<K> && CopyConstructible<V>
		: HashMap(il, 0, H{}, C{}, alloc)
	{
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	HashMap<K, V, H, C, IsMultiMap>::HashMap(const InitializerList<Pair<K, V>>& il, usize minBuckets, Alloc::IAllocator& alloc) noexcept
		requires CopyConstructible<K> && CopyConstructible<V>
		: HashMap(il, minBuckets, H{}, C{}, alloc)
	{
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	HashMap<K, V, H, C, IsMultiMap>::HashMap(const InitializerList<Pair<K, V>>& il, usize minBuckets, H hasher,	C comp, Alloc::IAllocator& alloc) noexcept
		requires CopyConstructible<K> && CopyConstructible<V>
		: m_buckets(&alloc)
		, m_size(0)
		, m_maxLoadFactor(1.0f)
		, m_hash(Move(hasher))
		, m_comp(Move(comp))
	{
		ASSERT(&alloc, "No allocator supplied to a HashMap/HashMultiMap");
		minBuckets = Math::Max(minBuckets, Math::Ceil(il.size() / m_maxLoadFactor));
		Rehash(minBuckets);
		for (const Pair<K, V>* it = il.begin(); it < il.end(); ++it)
			Insert(Move(Pair{ *it }));
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	template <ForwardIterator It>
	HashMap<K, V, H, C, IsMultiMap>::HashMap(const It& begin, const It& end, Alloc::IAllocator& alloc) noexcept
		requires CopyConstructible<K> && CopyConstructible<V>
		: HashMap(begin, end, 0, H{}, C{}, alloc)
	{
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	template <ForwardIterator It>
	HashMap<K, V, H, C, IsMultiMap>::HashMap(const It& begin, const It& end, usize minBuckets, Alloc::IAllocator& alloc) noexcept
		requires CopyConstructible<K> && CopyConstructible<V>
		: HashMap(begin, end, minBuckets, H{}, C{}, alloc)
	{
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	template <ForwardIterator It>
	HashMap<K, V, H, C, IsMultiMap>::HashMap(const It& begin, const It& end, usize minBuckets, H hasher, C comp, Alloc::IAllocator& alloc) noexcept
		requires CopyConstructible<K> && CopyConstructible<V>
		: m_buckets(&alloc)
		, m_size(0)
		, m_maxLoadFactor(1.0f)
		, m_hash(Move(hasher))
		, m_comp(Move(comp))
	{
		ASSERT(&alloc, "No allocator supplied to a HashMap/HashMultiMap");
		if constexpr (ContiguousIterator<It>)
			minBuckets = Math::Max(minBuckets, Math::Ceil((end - begin) / m_maxLoadFactor));
		Rehash(minBuckets);

		for (It it = begin; it < end; ++it)
			Insert(Move(Pair{ *it }));
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	HashMap<K, V, H, C, IsMultiMap>::HashMap(const HashMap& other) noexcept requires CopyConstructible<K> &&
		CopyConstructible<V>
		: HashMap(other, *other.GetAllocator())
	{
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	HashMap<K, V, H, C, IsMultiMap>::HashMap(const HashMap& other, Alloc::IAllocator& alloc) noexcept requires
		CopyConstructible<K> && CopyConstructible<V>
		: m_buckets(&alloc)
		, m_size(other.m_size)
		, m_maxLoadFactor(other.m_maxLoadFactor)
		, m_hash(other.m_hash)
		, m_comp(other.m_comp)
	{
		Rehash(other.BucketCount());
		for (Iterator it = other.Begin(), end = other.End(); it != end; ++it)
			Insert(*it);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	HashMap<K, V, H, C, IsMultiMap>::HashMap(HashMap&& other) noexcept
		: m_buckets(Move(other.m_buckets))
		, m_size(other.m_size)
		, m_maxLoadFactor(other.m_maxLoadFactor)
		, m_hash(other.m_hash)
		, m_comp(other.m_comp)
	{
		other.m_size = 0;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	HashMap<K, V, H, C, IsMultiMap>::HashMap(HashMap&& other, Alloc::IAllocator& alloc) noexcept
		: m_buckets(&alloc)
		, m_size(other.m_size)
		, m_maxLoadFactor(other.m_maxLoadFactor)
		, m_hash(Move(other.m_hash))
		, m_comp(Move(other.m_comp))
	{
		Rehash(other.BucketCount());
		for (Iterator it = other.Begin(), end = other.End(); it != end; ++it)
			Insert(StdMove(it.m_node->pair));

		other.ClearInternal<false>(true);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	HashMap<K, V, H, C, IsMultiMap>::~HashMap()
	{
		ClearInternal<true>(true);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::operator=(const InitializerList<Pair<K, V>>& il) noexcept -> HashMap
		requires CopyConstructible<K> && CopyConstructible<V>
	{
		ClearInternal<true>(false);
		Reserve(il.size());
		for (const Pair<K, V>* it = il.begin(); it < il.end(); ++it)
			Insert(it);
		return *this;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::operator=(const HashMap& other) noexcept -> HashMap requires
		CopyConstructible<K> && CopyConstructible<V>
	{
		ClearInternal<true>(false);
		m_maxLoadFactor = other.m_maxLoadFactor;
		Rehash(other.BucketCount());
		for (Iterator it = other.Begin(), end = other.End(); it != end; ++it)
			Insert(*it);
		return *this;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::operator=(HashMap&& other) noexcept -> HashMap
	{
		if (GetAllocator() == other.GetAllocator())
		{
			ClearInternal<true>(true);

			m_buckets = Move(other.m_buckets);
			m_size = other.m_size;
			m_maxLoadFactor = other.m_maxLoadFactor;
			m_hash = Move(other.m_hash);
			m_comp = Move(other.m_comp);
			other.m_size = 0;
		}
		else
		{
			ClearInternal<true>(false);
			Rehash(other.BucketCount());
			for (Iterator it = other.Begin(), end = other.End(); it != end; ++it)
				Insert(Move(*it));
			other.ClearInternal<false>(true);
		}
		return *this;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Rehash(usize count) noexcept -> void
	{
		if (BucketCount() >= count)
			return;

		usize bucketCount = 2;
		while (bucketCount < count)
			bucketCount <<= 1;

		Alloc::IAllocator* pAlloc = m_buckets.GetAlloc();

		Iterator it = Begin(); // Still valid after the move below
		Iterator end;
		MemRef<NodeRef> oldData = Move(m_buckets);

		m_buckets = pAlloc->Allocate<NodeRef>(bucketCount * sizeof(NodeRef));
		NodeRef* pBegin = m_buckets.Ptr();
		for (usize i = 0; i < bucketCount; ++i)
			new (pBegin + i) NodeRef{};

		m_size = 0;
		for (; it != end; ++it)
		{
			InsertNode<true>(it.m_node);
		}

		oldData.Dealloc();
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Reserve(usize count) noexcept -> void
	{
		Rehash(usize(Math::Ceil(count / m_maxLoadFactor)));
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Insert(const Pair<K, V>& pair) noexcept -> Pair<Iterator, bool> requires
		CopyConstructible<K> && CopyConstructible<V>
	{
		Reserve(m_size + 1);
		u64 hash = m_hash(pair.first);
		return InsertNode<true>(CreateNode(hash, Pair<K, V>{ pair }));
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Insert(Pair<K, V>&& pair) noexcept -> Pair<Iterator, bool>
	{
		Reserve(m_size + 1);
		u64 hash = m_hash(pair.first);
		return InsertNode<true>(CreateNode(hash, Move(pair)));
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Insert(const K& key, const V& val) noexcept -> Pair<Iterator, bool>
		requires CopyConstructible<K> && CopyConstructible<V>
	{
		Reserve(m_size + 1);
		u64 hash = m_hash(key);
		return InsertNode<true>(CreateNode(hash, Pair<K, V>{ key, val }));
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Insert(K&& key, V&& val) noexcept -> Pair<Iterator, bool>
	{
		Reserve(m_size + 1);
		u64 hash = m_hash(key);
		return InsertNode<true>(CreateNode(hash, Pair<K, V>{ Move(key), Move(val) }));
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::TryInsert(const Pair<K, V>& pair) noexcept -> Pair<Iterator, bool>
		requires CopyConstructible<K> && CopyConstructible<V>
	{
		Reserve(m_size + 1);
		u64 hash = m_hash(pair.first);
		return InsertNode<false>(CreateNode(hash, Pair<K, V>{ pair }));
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::TryInsert(Pair<K, V>&& pair) noexcept -> Pair<Iterator, bool>
	{
		Reserve(m_size + 1);
		u64 hash = m_hash(pair.first);
		return InsertNode<false>(CreateNode(hash, Move(pair)));;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::TryInsert(const K& key, const V& val) noexcept -> Pair<Iterator, bool>
		requires CopyConstructible<K> && CopyConstructible<V>
	{
		Reserve(m_size + 1);
		u64 hash = m_hash(key);
		return InsertNode<false>(CreateNode(hash, Pair<K, V>{ key, val }));
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::TryInsert(K&& key, V&& val) noexcept -> Pair<Iterator, bool>
	{
		Reserve(m_size + 1);
		u64 hash = m_hash(key);
		return InsertNode<false>(CreateNode(hash, Pair<K, V>{ Move(key), Move(val) }));
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	template <typename ... Args> requires ConstructableFrom<Pair<K, V>, Args...>
	auto HashMap<K, V, H, C, IsMultiMap>::Emplace(Args&&... args) noexcept -> Pair<Iterator, bool>
	{
		Reserve(m_size + 1);
		Pair<K, V>pair{ args... };
		u64 hash = m_hash(pair.first); 
		return InsertNode<true>(CreateNode(hash, Move(pair)));
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	template <typename ... Args> requires ConstructableFrom<V, Args...>
	auto HashMap<K, V, H, C, IsMultiMap>::TryEmplace(const K& key, Args&&... args) noexcept -> Pair<Iterator, bool>
	{
		Reserve(m_size + 1);
		u64 hash = m_hash(key);
		return InsertNode<false>(CreateNode(hash, Pair<K, V>{ Tuple<K>{ key }, Tuple<Args...>{ args... } }));
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	template <Hasher H2, EqualsComparator C2>
	auto HashMap<K, V, H, C, IsMultiMap>::Merge(HashMap<K, V, H2, C2>& other) noexcept -> void
	{
		Iterator it = other.Begin();
		while (it.m_node)
		{
			Iterator nextIt = it + 1;
			NodeRef itNode = it.m_node;

			Iterator foundIt;
			auto [found, prev, next] = FindForInsertOrErase(itNode->hash, itNode->pair.first);
			if (IsMultiMap || !found)
			{
				if (GetAllocator() == other.GetAllocator())
				{
					InsertNode(prev, itNode, next);
				}
				else
				{
					NodeRef node = CreateNode(it.m_node->hash, Move(it.m_node->pair));
					InsertNode(prev, node, next);
				}
				if constexpr (!IsMultiMap)
					other.RemoveNode(itNode);
			}

			it = nextIt;
		}

		if constexpr (IsMultiMap)
			other.template ClearInternal<false>();
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Clear(bool clearMemory) noexcept -> void
	{
		ClearInternal<true>(clearMemory);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Erase(ConstIterator& it) noexcept -> Iterator
	{
		ASSERT(it.m_node, "Invalid iterator");

		Iterator nextIt = it + 1;
		it.m_node->pair.~Pair();
		RemoveNode(it.m_node);
		return nextIt;
	}
	
	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Erase(const K& key) noexcept -> usize
	{
		Iterator it = Find(key);
		usize count = 0;
		while (it.m_node && m_comp(it.m_node->pair.first, key))
		{
			it = Erase(it);
			++count;
		}
		return count;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Find(const K& key) noexcept -> Iterator
	{
		u64 hash = m_hash(key);
		return FindWithHash(hash, key);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Find(const K& key) const noexcept -> ConstIterator
	{
		u64 hash = m_hash(key);
		return FindWithHash(hash, key);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto HashMap<K, V, H, C, IsMultiMap>::Find(const K2& key) noexcept -> Iterator
	{
		return FindOther(key);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto HashMap<K, V, H, C, IsMultiMap>::Find(const K2& key) const noexcept -> ConstIterator
	{
		return FindOther(key);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::FindRange(const K& key) noexcept -> Pair<Iterator, Iterator>
	{
		u64 hash = m_hash(key);
		return FindRangeWithHash(hash, key);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::FindRange(const K& key) const noexcept -> Pair<ConstIterator, ConstIterator>
	{
		u64 hash = m_hash(key);
		return FindRangeWithHash(hash, key);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto HashMap<K, V, H, C, IsMultiMap>::FindRange(const K2& key) noexcept -> Pair<Iterator, Iterator>
	{
		return FindRangeOther(key);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto HashMap<K, V, H, C, IsMultiMap>::FindRange(const K2& key) const noexcept -> Pair<ConstIterator, ConstIterator>
	{
		return FindRangeOther(key);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Contains(const K& key) const noexcept -> bool
	{
		return !!Find(key).m_node;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto HashMap<K, V, H, C, IsMultiMap>::Contains(const K2& key) const noexcept -> bool
	{
		return !!Find(key).m_node;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::At(const K& key) const noexcept -> Optional<V>
	{
		Iterator it = Find(key);
		if (it.m_node)
			return it->second;
		return NullOpt;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::operator[](const K& key) noexcept -> V&
	{
		return Find(key)->second;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::operator[](const K& key) const noexcept -> const V&
	{
		return Find(key)->second;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Count(const K& key) const noexcept -> usize
	{
		if constexpr (IsMultiMap)
		{
			auto [it, end] = FindRange(key);
			usize count = 0;
			for (; it != end; ++it)
				++count;
			return count;
		}
		else
		{
			return usize(!!Find(key).m_node);
		}
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto HashMap<K, V, H, C, IsMultiMap>::Count(const K2& key) const noexcept -> usize
	{
		if constexpr (IsMultiMap)
		{
			auto [it, end] = FindRange(key);
			usize count = 0;
			for (; it != end; ++it)
				++count;
			return count;
		}
		else
		{
			return usize(!!Find(key).m_node);
		}
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Size() const noexcept -> usize
	{
		return m_size;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::IsEmpty() const noexcept -> bool
	{
		return m_size == 0;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::BucketCount() const noexcept -> usize
	{
		return m_buckets.Size() / sizeof(NodeRef);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::BucketSize(usize idx) const noexcept -> usize
	{
		const usize bucketCount = BucketCount();
		if (idx > bucketCount)
			return 0;

		NodeRef node = *(m_buckets.Ptr() + idx);
		if (!node)
			return 0;

		const u64 mask = bucketCount - 1;
		usize count = 0;
		do
		{
			if ((node->hash & mask) != idx)
				break;

			++count;
			node = node->next;
		}
		while (node);
		return count;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::BucketIdx(const K& key) const noexcept -> usize
	{
		Iterator it = Find(key);
		if (it.m_node)
		{
			const u64 mask = BucketCount() - 1;
			return it.m_node->hash & mask;
		}
		return ~usize(0);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::LoadFactor() const noexcept -> usize
	{
		return f32(m_size) / f32(BucketCount());
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::MaxLoadFactor() const noexcept -> f32
	{
		return m_maxLoadFactor;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::SetMaxLoadFactor(f32 ml) noexcept -> void
	{
		m_maxLoadFactor = ml;
		Rehash(m_size / m_maxLoadFactor);
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::GetAllocator() const noexcept -> Alloc::IAllocator*
	{
		return m_buckets.GetAlloc();
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Front() noexcept -> Pair<K, V>&
	{
		return GetFirstNode().second->pair;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Front() const noexcept -> const Pair<K, V>&
	{
		return GetFirstNode().second->pair;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Back() noexcept -> Pair<K, V>&
	{
		return GetLastNode()->pair;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Back() const noexcept -> const Pair<K, V>&
	{
		return GetLastNode()->pair;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Begin() noexcept -> Iterator
	{
		auto [idx, node] = GetFirstNode();
		if (!node)
			return Iterator{};
		return Iterator{ m_buckets, idx, node };
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::Begin() const noexcept -> ConstIterator
	{
		auto [idx, node] = GetFirstNode();
		if (!node)
			return Iterator{};
		return Iterator{ m_buckets, idx, node };
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::End() noexcept -> Iterator
	{
		return Iterator{};
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::End() const noexcept -> ConstIterator
	{
		return Iterator{};
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::begin() noexcept -> Iterator
	{
		return Begin();
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::begin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::cbegin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::end() noexcept -> Iterator
	{
		return End();
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::end() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::cend() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::CreateNode(u64 hash, Pair<K, V>&& pair)  noexcept -> NodeRef
	{
		NodeRef node = m_buckets.GetAlloc()->template Allocate<Node>();
		Node* pNode = node.Ptr();
		pNode->next = NodeRef{};
		pNode->hash = hash; 
		pNode->pair = Move(pair);
		return node;
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	template<bool AllowOverride>
	auto HashMap<K, V, H, C, IsMultiMap>::InsertNode(NodeRef node) noexcept -> Pair<Iterator, bool>
	{
		u64 hash = node->hash;
		const K& key = node->pair.first;

		u64 mask = u64(BucketCount()) - 1;
		usize bucketIdx = hash & mask;
		NodeRef* pBucket = m_buckets.Ptr() + bucketIdx;
		NodeRef bucket = *pBucket;

		if (!bucket)
		{
			new (pBucket) NodeRef{ Move(node) };
			++m_size;
			return { Iterator{ m_buckets, bucketIdx, *pBucket }, true };
		}

		if constexpr (IsMultiMap)
		{
			bool found = false;

			if (bucket->hash == hash && m_comp(node->pair.first, key))
				found = true;

			NodeRef next = bucket->next;
			while (next && (next->hash & mask) == bucketIdx)
			{
				if (found)
				{
					if (next->hash != hash || !m_comp(next->pair.first, key))
					{
						node->next = Move(bucket->next);
						bucket->next = Move(node);
						++m_size;
						return { Iterator{ m_buckets, bucketIdx, bucket }, true };
					}
				}
				else
				{
					if (next->hash == hash && m_comp(next->pair.first, key))
						found = true;
				}

				bucket = next;
				next = next->next;
			}

			bucket->next = Move(node);
			++m_size;
			return { Iterator{ m_buckets, bucketIdx, bucket }, true };
		}
		else
		{
			if (bucket->hash == hash && m_comp(node->pair.first, key))
			{
				if constexpr (AllowOverride)
				{
					node->next = Move(bucket->next);
					new (pBucket) NodeRef{ Move(node) };

					bucket->pair.~Pair();
					bucket.Dealloc();
				}
				return { Iterator{ m_buckets, bucketIdx, *pBucket }, false };
			}

			NodeRef next = bucket->next;
			while (next)
			{
				if (next->hash == hash && m_comp(next->pair.first, key))
				{
					if constexpr (AllowOverride)
					{
						bucket->pair.~Pair();
						bucket->pair = Move(node->pair);
						node.Dealloc();
					}
					return { Iterator{ m_buckets, bucketIdx, bucket }, false };
				}
				bucket = next;
				next = next->next;
			}

			bucket->next = Move(node);
			++m_size;
			return { Iterator{ m_buckets, bucketIdx, bucket->next }, false };
		}
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::RemoveNode(NodeRef node) noexcept -> void
	{
		auto [found, prev, next] = FindForInsertOrErase(node->hash, node->pair.first);
		if (prev)
		{
			prev->next = next;
			NodeRef tmp = node;

			u64 mask = u64(BucketCount()) - 1;
			usize bucketIdx = node->hash & mask;
			NodeRef bucketNode = *(m_buckets.Ptr() + bucketIdx);
			if (node == bucketNode && next && next->hash & mask == bucketIdx)
			{
				new (m_buckets.Ptr() + bucketIdx) NodeRef{ Move(next) };
			}

			tmp.Dealloc();
		}
		else
		{
			NodeRef tmp = node;

			u64 mask = u64(BucketCount()) - 1;
			usize bucketIdx = node->hash & mask;
			if (next && (next->hash & mask) == bucketIdx)
			{
				new (m_buckets.Ptr() + bucketIdx) NodeRef{ Move(next) };
			}

			tmp.Dealloc();
		}
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	template <bool Destruct>
	auto HashMap<K, V, H, C, IsMultiMap>::ClearInternal(bool clearMemory) noexcept -> void
	{
		NodeRef* pBuckets = m_buckets.Ptr();
		for (usize i = 0, bucketCount = BucketCount(); i < bucketCount; ++i)
		{
			NodeRef node = *(pBuckets + i);
			while (node)
			{
				NodeRef next = node->next;
				if constexpr (Destruct)
					node->pair.~Pair();
				node.Dealloc();
				node = next;
			}
			new (pBuckets + i) NodeRef{};
		}
		m_size = 0;

		if (clearMemory)
		{
			m_buckets.Dealloc();
		}
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::FindWithHash(u64 hash, const K& key) const noexcept -> Iterator
	{
		u64 mask = BucketCount() - 1;
		usize bucketIdx = hash & mask;
		NodeRef node = *(m_buckets.Ptr() + bucketIdx);
		if (!node)
			return Iterator{};
		
		while (node && (node->hash & mask) == bucketIdx)
		{
			if (node->hash == hash && m_comp(node->pair.first, key))
				return Iterator{ m_buckets, bucketIdx, node };
			node = node->next;
		}
		return Iterator{};
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto HashMap<K, V, H, C, IsMultiMap>::FindOther(const K2& key) const noexcept -> Iterator
	{
		if constexpr (ConstructableFrom<K, K2>)
		{
			return Find(K(key));
		}
		else
		{
			for (Iterator it = Begin(), end = End(); it != end; ++it)
			{
				if (key == it->first)
					return it;
			}
			return Iterator{};
		}
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::FindRangeWithHash(u64 hash, const K& key) const noexcept -> Pair<Iterator, Iterator>
	{
		u64 mask = BucketCount() - 1;
		usize bucketIdx = hash & mask;
		NodeRef node = *(m_buckets.Ptr() + bucketIdx);
		if (!node)
			return Pair{ Iterator{}, Iterator{} };
		
		Iterator startIt;
		while (node && (node->hash & mask) == bucketIdx)
		{
			if constexpr (IsMultiMap)
			{
				if (startIt.m_node)
				{
					if (node->hash != hash || m_comp(node->pair.first, key))
						break;
				}
				else
				{
					if (node->hash == hash && m_comp(node->pair.first, key))
						startIt = Iterator{ m_buckets, node->hash & mask, node };
				}
			}
			else
			{
				if (node->hash == hash && m_comp(node->pair.first, key))
				{
					startIt = Iterator{ m_buckets, node->hash & mask, node };
					break;
				}
			}
			node = node->next;
		}
		return Pair{ startIt, Iterator{ m_buckets, node->hash & mask, node } + 1 };
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	template <EqualComparable<K> K2>
	auto HashMap<K, V, H, C, IsMultiMap>::FindRangeOther(const K2& key) const noexcept -> Pair<Iterator, Iterator>
	{
		if constexpr (ConstructableFrom<K, K2>)
		{
			K tmp(key);
			u64 hash = m_hash(tmp);
			return FindRangeWithHash(hash, tmp);
		}
		else
		{
			Iterator startIt = FindOther(key);
			Iterator it = startIt;
			while (it.m_node && key == it->first)
				++it;
			return Pair{ startIt, it };
		}
	}
	
	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::GetFirstNode() const noexcept -> Pair<usize, NodeRef>
	{
		if (!m_buckets)
			return Pair{ usize(0), NodeRef{} };

		usize bucketCount = BucketCount();
		NodeRef* pBegin = m_buckets.Ptr();
		for (usize i = 0; i < bucketCount; ++i)
		{
			NodeRef node = *(pBegin + i);
			if (node)
				return Pair{ i, node };
		}
		return Pair{ usize(0), NodeRef{} };
	}

	template <typename K, typename V, Hasher<K> H, EqualsComparator<K> C, bool IsMultiMap>
	auto HashMap<K, V, H, C, IsMultiMap>::GetLastNode() const noexcept -> NodeRef
	{
		usize bucketCount = BucketCount();
		NodeRef* pEnd = m_buckets.Ptr() + bucketCount;
		for (usize i = 1; i <= bucketCount; ++i)
		{
			NodeRef node = *(pEnd - i);
			if (!node)
				continue;

			while (node->next)
				node = node->next;

			return node;
		}
		return NodeRef{};
	}
}
