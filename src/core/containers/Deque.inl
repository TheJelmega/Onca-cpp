#pragma once
#if __RESHARPER__
#include "Deque.h"
#endif

namespace Onca
{
	template <typename T, usize BlockSize>
	Deque<T, BlockSize>::Iterator::Iterator()
		: m_blockIdx(0)
		, m_idx(0)
	{
	}

	template <typename T, usize BlockSize>
	Deque<T, BlockSize>::Iterator::Iterator(const Iterator& other) noexcept
		: m_blocks(other.m_blocks)
		, m_blockIdx(other.m_blockIdx)
		, m_idx(other.m_idx)
	{
	}

	template <typename T, usize BlockSize>
	Deque<T, BlockSize>::Iterator::Iterator(Iterator&& other) noexcept
		: m_blocks(Move(other.m_blocks))
		, m_blockIdx(other.m_blockIdx)
		, m_idx(other.m_idx)
	{
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator=(const Iterator& other) noexcept
	{
		m_blocks = other.m_blocks;
		m_blockIdx = other.m_blockIdx;
		m_idx = other.m_idx;
		return *this;
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator=(Iterator&& other) noexcept -> Iterator&
	{
		m_blocks = Move(other.m_blocks);
		m_blockIdx = other.m_blockIdx;
		m_idx = other.m_idx;
		return *this;
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator->() const noexcept -> T*
	{
		return (m_blocks.Ptr() + m_blockIdx)->Ptr() + m_idx;
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator*() const noexcept -> T&
	{
		return *((m_blocks.Ptr() + m_blockIdx)->Ptr() + m_idx);
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator++() noexcept -> Iterator
	{
		++m_idx;
		if (m_idx == BlockSize)
		{
			m_idx = 0;
			++m_blockIdx;
		}
		return *this;
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator++(int) noexcept -> Iterator
	{
		Iterator it{ *this };
		operator++();
		return it;
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator--() noexcept -> Iterator
	{
		if (m_idx == 0)
		{
			m_idx = BlockSize - 1;
			--m_blockIdx;
		}
		else
		{
			--m_idx;
		}
		return *this;
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator--(int) noexcept -> Iterator
	{
		Iterator it{ *this };
		operator--();
		return it;
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator+(usize count) const noexcept -> Iterator
	{
		usize idx = m_idx + count;
		usize offset = m_blockIdx + idx / BlockSize;
		idx &= Mask;
		return Iterator{ m_blocks, offset, idx };
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator-(usize count) const noexcept -> Iterator
	{
		usize actIdx = m_blockIdx * BlockSize + m_idx;
		if (actIdx < count)
			return Iterator{ m_blocks, 0, 0 };
		actIdx -= count;
		usize idx = actIdx & Mask;
		usize offset = actIdx / BlockSize;
		return Iterator{ m_blocks, offset, idx };
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator-(const Iterator& it) const noexcept -> isize
	{
		usize actIdx = m_blockIdx * BlockSize + m_idx;
		usize otherIdx = it.m_blockIdx * BlockSize + it.m_idx;
		ASSERT(actIdx > otherIdx, "Iterator subtraction is in the wrong order");
		return actIdx - otherIdx;
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator+=(usize count) noexcept -> Iterator&
	{
		m_idx = m_idx + count;
		m_blockIdx = m_blockIdx + m_idx / BlockSize;
		m_idx &= Mask;
		return *this;
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator-=(usize count) noexcept -> Iterator&
	{
		usize actIdx = m_blockIdx * BlockSize + m_idx;
		if (actIdx < count)
		{
			m_blockIdx = m_idx = 0;
		}
		actIdx -= count;
		m_idx = actIdx & Mask;
		m_blockIdx = actIdx / BlockSize;
		return *this;
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator==(const Iterator& other) const noexcept -> bool
	{
		return m_blockIdx == other.m_blockIdx && m_idx == other.m_idx && m_blocks == other.m_blocks;
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator!=(const Iterator& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator<=>(const Iterator& other) const noexcept -> std::partial_ordering
	{
		if (m_blocks != other.m_blocks)
			return std::partial_ordering::unordered;

		usize actIdx = m_blockIdx * BlockSize + m_idx;
		usize otherIdx = other.m_blockIdx * BlockSize + other.m_idx;

		if (actIdx == otherIdx) return std::strong_ordering::equal;
		if (actIdx < otherIdx) return std::strong_ordering::less;
		return std::strong_ordering::greater;
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator[](usize idx) noexcept -> T&
	{
		usize actIdx = m_blockIdx * BlockSize + m_idx + idx;
		usize offset = actIdx / BlockSize;
		usize blockIdx = actIdx & Mask;
		return (m_blocks.Ptr() + offset)->Ptr() + blockIdx;
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Iterator::operator[](usize idx) const noexcept -> const T&
	{
		usize actIdx = m_blockIdx * BlockSize + m_idx + idx;
		usize offset = actIdx / BlockSize;
		usize blockIdx = actIdx & Mask;
		return (m_blocks.Ptr() + offset)->Ptr() + blockIdx;
	}

	template <typename T, usize BlockSize>
	Deque<T, BlockSize>::Iterator::Iterator(const MemRef<MemRef<T>>& blocks, usize blockOffset, usize idx) noexcept
		: m_blocks(blocks)
		, m_blockIdx(blockOffset)
		, m_idx(idx)
	{
	}

	template <typename T, usize BlockSize>
	Deque<T, BlockSize>::Deque(Alloc::IAllocator& alloc) noexcept
		: m_blocks(&alloc)
		, m_initialIdx(0)
		, m_size(0)
	{
	}

	template <typename T, usize BlockSize>
	Deque<T, BlockSize>::Deque(usize count, Alloc::IAllocator& alloc) noexcept requires DefaultConstructible<T>
		: m_blocks(&alloc)
		, m_initialIdx(0)
		, m_size(0)
	{
		FillDefault(count);
	}

	template <typename T, usize BlockSize>
	Deque<T, BlockSize>::Deque(usize count, const T& val, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>
		: m_blocks(&alloc)
		, m_initialIdx(0)
		, m_size(0)
	{
		Fill(count, val);
	}

	template <typename T, usize BlockSize>
	Deque<T, BlockSize>::Deque(const InitializerList<T>& il, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>
		: m_blocks(&alloc)
		, m_initialIdx(0)
		, m_size(0)
	{
		Assign(il);
	}

	template <typename T, usize BlockSize>
	template <ForwardIterator It>
	Deque<T, BlockSize>::Deque(const It& begin, const It& end, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>
		: m_blocks(&alloc)
		, m_initialIdx(0)
		, m_size(0)
	{
		Assign(begin, end);
	}

	template <typename T, usize BlockSize>
	Deque<T, BlockSize>::Deque(const Deque& other) noexcept requires CopyConstructible<T>
		: m_blocks(other.GetAllocator())
		, m_initialIdx(0)
		, m_size(0)
	{
		Assign(other.Begin(), other.End());
	}

	template <typename T, usize BlockSize>
	template <usize B>
	Deque<T, BlockSize>::Deque(const Deque<T, B>& other) noexcept requires CopyConstructible<T>
		: m_blocks(other.GetAllocator())
		, m_initialIdx(0)
		, m_size(0)
	{
		Assign(other.Begin(), other.End());
	}

	template <typename T, usize BlockSize>
	template <usize B>
	Deque<T, BlockSize>::Deque(const Deque<T, B>& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>
		: m_blocks(&alloc)
		, m_initialIdx(0)
		, m_size(0)
	{
		Assign(other.Begin(), other.End());
	}

	template <typename T, usize BlockSize>
	Deque<T, BlockSize>::Deque(Deque&& other) noexcept
		: m_blocks(Move(other.m_blocks))
		, m_initialIdx(other.m_initialIdx)
		, m_size(other.m_size)
	{
		other.m_size = other.m_initialIdx = 0;
	}

	template <typename T, usize BlockSize>
	template <usize B>
	Deque<T, BlockSize>::Deque(Deque<T, B>&& other, Alloc::IAllocator& alloc) noexcept
		: m_blocks(&alloc)
		, m_initialIdx(other.m_initialIdx)
		, m_size(other.m_size)
	{
		usize size = m_size + m_initialIdx;
		usize blocksNeeded = (size + Mask) / BlockSize;
		MemRef<T>* pBlocks = AddBackBlocks(blocksNeeded);
		MemRef<T>* pOtherBlocks = other.m_blocks.Ptr();
		for (usize i = 0; i < blocksNeeded; ++i)
		{
			MemRef<T>& otherBlock = *(pOtherBlocks + i);
			MemRefCpy(*(pBlocks + i), otherBlock, BlockSize);
			otherBlock.Dealloc();
		}
		other.m_blocks.Dealloc();
		other.m_initialIdx = other.m_size = 0;
	}

	template <typename T, usize BlockSize>
	Deque<T, BlockSize>::~Deque() noexcept
	{
		Clear(true);
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::operator=(const InitializerList<T>& il) noexcept -> Deque<T>& requires CopyConstructible<T>
	{
		Assign(il);
		return *this;
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::operator=(const Deque& other) noexcept -> Deque<T>& requires CopyConstructible<T>
	{
		Assign(other.Begin(), other.End());
		return *this;
	}

	template <typename T, usize BlockSize>
	template <usize B>
	auto Deque<T, BlockSize>::operator=(const Deque<T, B>& other) noexcept -> Deque<T>& requires CopyConstructible<T>
	{
		Assign(other.Begin(), other.End()); 
		return *this;
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::operator=(Deque&& other) noexcept -> Deque<T>&
	{
		m_blocks = Move(other.m_blocks);
		m_initialIdx = other.m_initialIdx;
		m_size = other.m_size;
		other.m_initialIdx = other.m_size = 0;
		return *this;
	}

	template <typename T, usize BlockSize>
	template <ForwardIterator It>
	void Deque<T, BlockSize>::Assign(const It& begin, const It& end) noexcept requires CopyConstructible<T>
	{
		Clear(false);
		if constexpr (ContiguousIterator<T> && MemCopyable<T>)
		{
			ASSERT(begin < end, "'begin' iterator must be smaller than 'end' iterator");
			usize size = end - begin;
			usize blocksNeeded = (size + Mask) / Mask;
			MemRef<T>* pBlocks = AddBackBlocks(blocksNeeded);
			const T* pSrc = &*begin;
			for (usize i = 0; i < blocksNeeded; ++i)
			{
				usize count = Min(size, blocksNeeded);
				MemCpy(pBlocks + i, pSrc + i, count + 1);
			}
		}
		else
		{
			for (It it = begin; it != end; ++it)
				Push(T{ *it });
		}
	}

	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::Assign(const InitializerList<T>& il) noexcept requires CopyConstructible<T>
	{
		Clear(false);
		if constexpr (ContiguousIterator<T> && MemCopyable<T>)
		{
			usize size = end - begin;
			usize blockNeeded = (size + Mask) / Mask;
			MemRef<T>* pBlocks = AddBackBlocks((il.size() + Mask) / BlockSize);
			const T* pSrc = &*begin;
			for (usize i = 0; i < blockNeeded; ++i)
			{
				usize count = Min(size, blockNeeded);
				MemCpy(pBlocks + i, pSrc + i, count + 1);
			}
		}
		else
		{
			for (const T* it = il.begin(), *end = il.end(); it != end; ++it)
				Push(T{ *it });
		}
	}

	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::Fill(usize count, const T& val) noexcept requires CopyConstructible<T>
	{
		Clear();
		for (usize i = 0; i < count; ++i)
			Push(T{ val });
	}

	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::FillDefault(usize count) noexcept requires NoThrowDefaultConstructible<T>
	{
		Clear();
		for (usize i = 0; i < count; ++i)
			EmplaceBack();
	}

	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::Resize(usize newSize, const T& val) noexcept requires CopyConstructible<T>
	{
		if (newSize < m_size)
		{
			MemRef<T>* pBlocks = m_blocks.Ptr();
			for (usize i = newSize; i < m_size; ++i)
				GetElemAddr(pBlocks, i)->~T();
		}
		else
		{
			for (usize i = m_size; i < newSize; ++i)
				Push(T{ val });
		}
	}

	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::Resize(usize newSize) noexcept requires NoThrowDefaultConstructible<T>
	{
		if (newSize < m_size)
		{
			MemRef<T>* pBlocks = m_blocks.Ptr();
			for (usize i = newSize; i < m_size; ++i)
				GetElemAddr(pBlocks, i)->~T();
		}
		else
		{
			for (usize i = m_size; i < newSize; ++i)
				EmplaceBack();
		}
		m_size = newSize;
	}

	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::PushFront(const T& val) noexcept requires CopyConstructible<T>
	{
		PushFront(T{ val });
	}

	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::PushFront(T&& val) noexcept
	{
		if (m_initialIdx == 0)
		{
			AddFrontBlocks(1);
			m_initialIdx = BlockSize;
		}

		--m_initialIdx;
		new (m_blocks.Ptr()->Ptr() + m_initialIdx) T{ Move(val) };
		++m_size;
	}

	template <typename T, usize BlockSize>
	template <usize B>
	void Deque<T, BlockSize>::PushFront(const Deque<T, B>& other) requires CopyConstructible<T>
	{
		for (usize i = 0; i < other.m_size; ++i)
			PushFront(other[other.m_size - i - 1]);
	}

	template <typename T, usize BlockSize>
	template <usize B>
	void Deque<T, BlockSize>::PushFront(Deque<T, B>&& other)
	{
		for (usize i = 0; i < other.m_size; ++i)
			PushFront(*other.GetElemAddr(other.m_size - i - 1));
		other.m_blocks.Dealloc();
		other.m_initialIdx = other.m_size = 0;
	}

	template <typename T, usize BlockSize>
	template <typename... Args> requires ConstructableFrom<T, Args...>
	void Deque<T, BlockSize>::EmplaceFront(Args&&... args) noexcept
	{
		PushFront(T{ args... });
	}

	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::Push(const T& val) noexcept requires CopyConstructible<T>
	{
		Push(T{ val });
	}

	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::Push(T&& val) noexcept
	{
		auto [offset, idx] = GetElemOffsetIdx(m_size);
		if (idx == 0) 
			AddBackBlocks(1); 
		
		new ((m_blocks.Ptr() + offset)->Ptr() + idx) T{ Move(val) };
		++m_size;
	}

	template <typename T, usize BlockSize>
	template <usize B>
	void Deque<T, BlockSize>::Push(const Deque<T, B>& other) requires CopyConstructible<T>
	{
		for (Iterator it = other.Begin(), end = other.End(); it != end; ++it)
			Push(T{ *it });
	}

	template <typename T, usize BlockSize>
	template <usize B>
	void Deque<T, BlockSize>::Push(Deque<T, B>&& other)
	{
		for (usize i = 0; i < other.m_size; ++i)
			Push(Move(*other.GetElemAddr(i)));
		other.m_blocks.Dealloc();
		other.m_initialIdx = other.m_size = 0;
	}

	template <typename T, usize BlockSize>
	template <typename ... Args> requires ConstructableFrom<T, Args...>
	void Deque<T, BlockSize>::EmplaceBack(Args&&... args) noexcept
	{
		Push(T{ args... });
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Insert(ConstIterator& it, const T& val) noexcept -> Iterator requires CopyConstructible<T>
	{
		return Insert(it, T{ val });
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Insert(ConstIterator& it, T&& val) noexcept -> Iterator
	{
		usize itIdx = it.m_blockIdx * BlockSize + it.m_idx - m_initialIdx;
		PrepareInsert(itIdx, 1);
		new (GetElemAddr(itIdx)) T{ Move(val) };
		++m_size;
		return IteratorAt(itIdx);
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Insert(ConstIterator& it, usize count, const T& val) noexcept -> Iterator requires CopyConstructible<T>
	{
		usize itIdx = it.m_blockIdx * BlockSize + it.m_idx - m_initialIdx;
		PrepareInsert(itIdx, count);
		for (usize i = 0; i < count; ++i)
			new (GetElemAddr(itIdx + i)) T{ Move(val) };
		m_size += count;
		return IteratorAt(itIdx);
	}

	template <typename T, usize BlockSize>
	template <ForwardIterator It>
	auto Deque<T, BlockSize>::Insert(ConstIterator& it, const It& begin, const It& end) noexcept -> Iterator requires CopyConstructible<T>
	{
		usize count = CountElems(begin, end);
		return InsertIts(it, count, begin, end);
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Insert(ConstIterator& it, const InitializerList<T>& il) noexcept -> Iterator requires CopyConstructible<T>
	{
		return InsertIts(it, il.size(), il.begin(), il.end());
	}

	template <typename T, usize BlockSize>
	template <usize B>
	auto Deque<T, BlockSize>::Insert(ConstIterator& it, const Deque<T, B>& other) noexcept -> Iterator requires CopyConstructible<T>
	{
		return InsertIts(it, other.m_size, other.Begin(), other.End());
	}

	template <typename T, usize BlockSize>
	template <usize B>
	auto Deque<T, BlockSize>::Insert(ConstIterator& it, Deque<T, B>&& other) noexcept -> Iterator
	{
		usize itIdx = it.m_blockIdx * BlockSize + it.m_idx - m_initialIdx;
		PrepareInsert(itIdx, other.m_size);
		usize i = itIdx;
		for (Iterator valIt = other.Begin(), end = other.End(); valIt != end; ++valIt, ++i)
			new (GetElemAddr(i)) T{ Move(*valIt) };
		m_size += other.m_size;

		usize otherBlocks = (other.m_initialIdx + other.m_size + B - 1) / B;
		MemRef<T>* pOtherBegin = other.m_blocks.Ptr();
		for (i = 0; i < otherBlocks; ++i)
			(pOtherBegin + i)->Dealloc();
		other.m_blocks.Dealloc();
		other.m_initialIdx = other.m_size = 0;

		return IteratorAt(itIdx);
	}

	template <typename T, usize BlockSize>
	template <typename ... Args> requires ConstructableFrom<T, Args...>
	auto Deque<T, BlockSize>::Emplace(ConstIterator& it, Args&&... args) noexcept -> Iterator
	{
		return Insert(it, T{ args... });
	}

	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::Clear(bool clearMemory) noexcept
	{
		for (usize i = 0; i < m_size; ++i)
			GetElemAddr(i)->~T();

		if (!clearMemory)
		{
			m_size = 0;
			return;
		}

		usize usedBlocks = (m_initialIdx + m_size + Mask) / BlockSize;
		MemRef<T>* pBegin = m_blocks.Ptr();
		for (usize i = 0; i < usedBlocks; ++i)
			(pBegin + i)->Dealloc();
		Alloc::IAllocator* pAlloc = m_blocks.GetAlloc();
		m_blocks.Dealloc();
		m_blocks = MemRef<MemRef<T>>{ pAlloc };
		m_size = 0;
	}

	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::PopFront() noexcept
	{
		GetElemAddr(0)->~T();
		if (m_initialIdx == BlockSize - 1)
		{
			m_blocks->Dealloc();
			const usize numBlocks = (m_initialIdx + m_size + Mask) / BlockSize - 1;
			MemMove(m_blocks, 0, 1, numBlocks * sizeof(MemRef<T>));
			m_initialIdx = 0;
		}
		else
		{
			++m_initialIdx;
		}
		--m_size;
	}

	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::Pop() noexcept
	{
		GetElemAddr(m_size - 1)->~T();
		usize endIdx = (m_initialIdx + m_size) & Mask;
		if (endIdx == 0)
		{
			usize endBlock = (m_initialIdx + m_size) / BlockSize;
			(m_blocks.Ptr() + endBlock)->Dealloc();
		}
		--m_size;
	}

	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::Erase(const Iterator& it) noexcept
	{
		Erase(it, 1);
	}

	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::Erase(const Iterator& it, usize count) noexcept
	{
		usize itIdx = it.m_blockIdx * BlockSize + it.m_idx - m_initialIdx;
		usize center = itIdx + count / 2;
		usize halfSize = m_size / 2;

		for (usize i = itIdx, end = itIdx + count; i < end; ++i)
			GetElemAddr(i)->~T();

		if (center >= halfSize)
		{
			usize numElems = m_size - itIdx - count + 1;
			Algo::Move(it + count, it, numElems);

			auto [offset, index] = GetElemOffsetIdx(m_size - count);
			if (offset)
			{
				usize usedBlocks = (m_initialIdx + m_size + Mask) / BlockSize;
				MemRef<T>* pEnd = m_blocks.Ptr() - usedBlocks;
				for (usize i = 0; i < offset; ++i)
					(pEnd - i - 1)->Dealloc();
				MemMove(pEnd, pEnd - offset, usedBlocks - offset);
			}
		}
		else
		{
			Iterator begin = Begin();
			Algo::Move(begin, begin + count, itIdx);
			auto [offset, index] = GetElemOffsetIdx(count);

			if (offset)
			{
				MemRef<T>* pBegin = m_blocks.Ptr();
				for (usize i = 0; i < offset; ++i)
					(pBegin + i)->Dealloc();
				usize usedBlocks = (m_initialIdx + m_size + Mask) / BlockSize;
				MemMove(pBegin, pBegin + offset, usedBlocks - offset);
			}

			m_initialIdx = index;
		}
		m_size -= count;
	}

	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::Erase(const Iterator& begin, const Iterator& end) noexcept
	{
		Erase(begin, end - begin);
	}

	template <typename T, usize BlockSize>
	template <EqualComparable<T> U>
	auto Deque<T, BlockSize>::Find(const U& value) noexcept -> Iterator
	{
		Iterator end = End();
		for (Iterator it = Begin(); it != end; ++it)
		{
			if (value == *it)
				return it;
		}
		return end;
	}

	template <typename T, usize BlockSize>
	template <EqualComparable<T> U>
	auto Deque<T, BlockSize>::Find(const U& value) const noexcept -> ConstIterator
	{
		Iterator end = End();
		for (Iterator it = Begin(); it != end; ++it)
		{
			if (value == *it)
				return it;
		}
		return end;
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::At(usize idx) const noexcept -> Optional<T>
	{
		if (idx >= m_size)
			return NullOpt;
		return *GetElemAddr(idx);
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::IteratorAt(usize idx) noexcept -> Iterator
	{
		ASSERT(idx < m_size, "Index out of range");
		return IteratorAtInternal(idx);
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::IteratorAt(usize idx) const noexcept -> ConstIterator
	{
		ASSERT(idx < m_size, "Index out of range");
		return IteratorAtInternal(idx);
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::operator[](usize idx) noexcept -> T&
	{
		ASSERT(idx < m_size, "Index out of range");
		return *GetElemAddr(idx);
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::operator[](usize idx) const noexcept -> const T&
	{
		ASSERT(idx < m_size, "Index out of range");
		return *GetElemAddr(idx);
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Size() const noexcept -> usize
	{
		return m_size;
	}
	
	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::IsEmpty() const noexcept -> bool
	{
		return m_size == 0;
	}
	
	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::GetAllocator() const noexcept -> Alloc::IAllocator*
	{
		return m_blocks.GetAlloc();
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Front() noexcept -> T&
	{
		ASSERT(m_size, "Invalid when Deque is empty");
		return *(*m_blocks.Ptr() + m_initialIdx);
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Front() const noexcept -> const T&
	{
		ASSERT(m_size, "Invalid when Deque is empty");
		return *(*m_blocks.Ptr() + m_initialIdx);
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Back() noexcept -> T&
	{
		ASSERT(m_size, "Invalid when Deque is empty");
		return GetElemAddr(m_size - 1);
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Back() const noexcept -> const T&
	{
		ASSERT(m_size, "Invalid when Deque is empty");
		return GetElemAddr(m_size - 1);
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Begin() noexcept -> Iterator
	{
		return Iterator{ m_blocks, 0, m_initialIdx };
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::Begin() const noexcept -> ConstIterator
	{
		return Iterator{ m_blocks, 0, m_initialIdx };
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::End() noexcept -> Iterator
	{
		auto [offset, idx] = GetElemOffsetIdx(m_size);
		return Iterator{ m_blocks, offset, idx };
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::End() const noexcept -> ConstIterator
	{
		auto [offset, idx] = GetElemOffsetIdx(m_size);
		return Iterator{ m_blocks, offset, idx };
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::RBegin() noexcept -> Iterator
	{
		return End();
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::RBegin() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::REnd() noexcept -> Iterator
	{
		return Begin();
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::REnd() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::begin() noexcept -> Iterator
	{
		return Begin();
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::begin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::cbegin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::end() noexcept -> Iterator
	{
		return End();
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::end() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::cend() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::GetElemOffsetIdx(usize idx) const noexcept -> Pair<usize, usize>
	{
		usize actIdx = m_initialIdx + idx;
		return Pair{ actIdx / BlockSize, actIdx & Mask };
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::GetElemAddr(usize idx) const noexcept -> T*
	{
		return GetElemAddr(m_blocks.Ptr(), idx);
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::GetElemAddr(MemRef<T>* pBegin, usize idx) const noexcept -> T*
	{
		auto [offset, blockIdx] = GetElemOffsetIdx(idx);
		return (pBegin + offset)->Ptr() + blockIdx;
	}
	
	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::AddBackBlocks(usize numBlocks) noexcept
	{
		ReserveBase<true>(numBlocks);
		Alloc::IAllocator* pAlloc = m_blocks.GetAlloc();
		usize usedBlocks = (m_initialIdx + m_size + Mask) / BlockSize;
		MemRef<T>* pEnd = m_blocks.Ptr() + usedBlocks;
		for (usize i = 0; i < numBlocks; ++i)
			*(pEnd + i) = pAlloc->Allocate<T>(BlockByteSize);
	}

	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::AddFrontBlocks(usize numBlocks) noexcept
	{
		ReserveBase<false>(numBlocks);
		Alloc::IAllocator* pAlloc = m_blocks.GetAlloc();
		MemRef<T>* pBegin = m_blocks.Ptr();
		for (usize i = 0; i < numBlocks; ++i)
			*(pBegin + i) = pAlloc->Allocate<T>(BlockByteSize);
	}
	
	template <typename T, usize BlockSize>
	template <bool AtBack>
	auto Deque<T, BlockSize>::ReserveBase(usize numAdditionalBlocks) noexcept -> MemRef<T>*
	{
		usize curBlocks = m_blocks.Size() / sizeof(MemRef<T>);
 		usize usedBlocks = (m_initialIdx + m_size + Mask) / BlockSize;
		usize neededBlocks = usedBlocks + numAdditionalBlocks;

		if (neededBlocks <= curBlocks)
		{
			if constexpr (AtBack)
			{
				return m_blocks.Ptr() + usedBlocks;
			}
			else
			{
				MemMove(m_blocks, numAdditionalBlocks, 0, usedBlocks * sizeof(MemRef<T>));
				return m_blocks.Ptr();
			}
		}

		usize cap = Math::Max(curBlocks, usize(1));
		while (cap < neededBlocks)
			cap = (cap << 1) - (cap >> 1);

		Alloc::IAllocator* pAlloc = m_blocks.GetAlloc();
		MemRef<MemRef<T>> oldBlock = Move(m_blocks);

		m_blocks = pAlloc->Allocate<MemRef<T>>(cap * sizeof(MemRef<T>));
		if (curBlocks)
		{
			if constexpr (AtBack)
				MemRefCpy(m_blocks, oldBlock, curBlocks);
			else
				MemRefCpy(m_blocks, numAdditionalBlocks, oldBlock, 0, curBlocks);
		}

		oldBlock.Dealloc();

		return m_blocks.Ptr();
	}

	// TODO: optimizations for types that allow memcpy/memmove in containers
	template <typename T, usize BlockSize>
	void Deque<T, BlockSize>::PrepareInsert(usize idx, usize moveOffset) noexcept
	{
		usize halfSize = m_size / 2;
		if (idx > halfSize)
		{
			const usize actEndIdx = m_initialIdx + m_size;
			const usize lastBlock = (actEndIdx + Mask) / BlockSize;
			const usize usedBlocks = (actEndIdx + moveOffset + Mask) / BlockSize;
			const usize neededBlocks = usedBlocks - lastBlock;
			AddBackBlocks(neededBlocks);

			const usize copySize = m_size - idx;
			Algo::Move(IteratorAtInternal(idx), IteratorAtInternal(idx + moveOffset), copySize);
		}
		else
		{
			const usize actEndIdx = m_initialIdx + m_size;
			const usize lastBlock = (actEndIdx + Mask) / BlockSize;
			const usize usedBlocks = (actEndIdx + moveOffset + Mask) / BlockSize;
			const usize neededBlocks = usedBlocks - lastBlock;
			AddFrontBlocks(neededBlocks);

			const usize oldOffset = neededBlocks * BlockSize + m_initialIdx;
			const usize offset = moveOffset & Mask;
			if (offset > m_initialIdx)
				m_initialIdx = (BlockSize + m_initialIdx - offset);
			else
				m_initialIdx -= offset;
			
			const usize copySize = idx;
			Algo::Move(IteratorAtInternal(oldOffset + idx + moveOffset), IteratorAtInternal(idx), copySize);
		}
	}

	template <typename T, usize BlockSize>
	template <ForwardIterator It>
	auto Deque<T, BlockSize>::InsertIts(ConstIterator& it, usize count, const It& begin, const It& end) noexcept -> Iterator
	{
		usize idx = it.m_blockIdx * BlockSize + it.m_idx - m_initialIdx;
		PrepareInsert(idx, count);
		usize tmp = idx;
		for (It valIt = begin; valIt != end; ++valIt, ++tmp)
			new (GetElemAddr(tmp)) T{ *valIt };
		m_size += count;
		return IteratorAt(idx);
	}

	template <typename T, usize BlockSize>
	auto Deque<T, BlockSize>::IteratorAtInternal(usize idx) const noexcept -> Iterator
	{
		auto [offset, blockIdx] = GetElemOffsetIdx(idx);
		return Iterator{ m_blocks, offset, blockIdx };
	}
}
