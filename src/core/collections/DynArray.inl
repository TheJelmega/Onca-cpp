#pragma once
#include "DynArray.h"

namespace Core
{
	template <MoveConstructable T>
	DynArray<T>::DynArray(Alloc::IAllocator& alloc) noexcept
		: m_mem(&alloc)
		, m_size(0)
	{
	}

	template <MoveConstructable T>
	DynArray<T>::DynArray(usize capacity, Alloc::IAllocator& alloc) noexcept
		: m_mem(&alloc)
		, m_size(0)
	{
		Reserve(capacity);
	}

	template <MoveConstructable T>
	DynArray<T>::DynArray(usize count, const T& val, Alloc::IAllocator& alloc) noexcept
		: m_mem(&alloc)
		, m_size(0)
	{
		Resize(count, val);
	}

	template <MoveConstructable T>
	DynArray<T>::DynArray(const InitializerList<T>& il, Alloc::IAllocator& alloc) noexcept
		: m_mem(&alloc)
		, m_size(0)
	{
		Assign(il);
	}

	template <MoveConstructable T>
	template <ForwardIterator It>
	DynArray<T>::DynArray(const It& begin, const It& end, Alloc::IAllocator& alloc) noexcept
		: m_mem(&alloc)
		, m_size(0)
	{
		Assign(begin, end);
	}

	template <MoveConstructable T>
	DynArray<T>::DynArray(const DynArray<T>& other) noexcept
		: m_mem(other.m_mem.Alloc())
		, m_size(0)
	{
		Reserve(other.m_size);
		Assign(other.Begin(), other.End());
	}

	template <MoveConstructable T>
	DynArray<T>::DynArray(const DynArray<T>& other, Alloc::IAllocator& alloc) noexcept
		: m_mem(&alloc)
		, m_size(0)
	{
		Reserve(other.m_size);
		Assign(other.Begin(), other.End());
	}

	template <MoveConstructable T>
	DynArray<T>::DynArray(DynArray<T>&& other) noexcept
		: m_mem(StdMove(other.m_mem))
		, m_size(StdMove(other.m_size))
	{
		other.m_size = 0;
	}

	template <MoveConstructable T>
	DynArray<T>::~DynArray() noexcept
	{
		Clear(true);
	}

	template <MoveConstructable T>
	auto DynArray<T>::operator=(const InitializerList<T>& il) noexcept -> DynArray<T>&
	{
		Assign(il);
		return *this;
	}

	template <MoveConstructable T>
	auto DynArray<T>::operator=(const DynArray<T>& other) noexcept -> DynArray<T>&
	{
		Reserve(other.m_size);
		Assign(other.Begin(), other.End());
		return *this;
	}

	template <MoveConstructable T>
	auto DynArray<T>::operator=(DynArray<T>&& other) noexcept -> DynArray<T>&
	{
		m_mem = StdMove(other.m_mem);
		m_size = other.m_size;
		other.m_size = 0;
		return *this;
	}

	template <MoveConstructable T>
	template <ForwardIterator It>
	auto DynArray<T>::Assign(const It& begin, const It& end) noexcept -> void
	{
		for (It it = begin; it != end; ++it)
			Add(*it);
	}

	template <MoveConstructable T>
	auto DynArray<T>::Assign(const InitializerList<T>& il) noexcept -> void
	{
		Reserve(il.size());
		for (const T* it = il.begin(), *end = il.end(); it != end; ++it)
			Add(*it);
	}

	template <MoveConstructable T>
	auto DynArray<T>::Fill(usize count, const T& val) noexcept -> void
	{
		Clear();
		Resize(count, val);
	}

	template <MoveConstructable T>
	auto DynArray<T>::FillDefault(usize count) noexcept -> void
	{
		STATIC_ASSERT(IsNothrowDefaultConstructable<T>, "T needs to be nothrow default constructable");
		Clear();
		Resize(count);
	}

	template <MoveConstructable T>
	auto DynArray<T>::Reserve(usize newCap) noexcept -> void
	{
		const usize curCap = Capacity();
		if (curCap >= newCap)
			return;

		usize cap = 2;
		while (cap < newCap)
			cap = (cap << 1) - (cap >> 1);

		MemRef<T> mem = m_mem.Alloc()->template Allocate<T>(newCap * sizeof(T));
		if (m_mem.IsValid())
		{
			MemCpy(mem.Ptr(), m_mem.Ptr(), m_size * sizeof(T));
			m_mem.Dealloc();
		}
		m_mem = StdMove(mem);
		ASSERT(m_mem, "Failed to allocate memory");
	}

	template <MoveConstructable T>
	auto DynArray<T>::Resize(usize newSize, const T& val) noexcept -> void
	{
		if (newSize < m_size)
		{
			T* pBegin = m_mem.Ptr();
			for (T* it = pBegin + newSize, *end = pBegin + m_size; it != end; ++it)
				it->~T();
		}
		else
		{
			Reserve(newSize);
			for (usize i = m_size; i < newSize; ++i)
				Add(val);
		}
		m_size = newSize;
	}

	template <MoveConstructable T>
	auto DynArray<T>::Resize(usize newSize) noexcept -> void
	{
		STATIC_ASSERT(IsNothrowDefaultConstructable<T>, "T needs to be nothrow default constructable");
		T* pBegin = m_mem.Ptr();
		if (newSize < m_size)
		{
			for (T* it = pBegin + newSize, *end = pBegin + m_size; it != end; ++it)
				it->~T();
		}
		else
		{
			Reserve(newSize);
			for (T* it = pBegin + m_size, *end = pBegin + newSize; it != end; ++it)
				new (it) T{};
		}
		m_size = newSize;
	}

	template <MoveConstructable T>
	auto DynArray<T>::ShrinkToFit() noexcept -> void
	{
		usize cap = Capacity();
		if (cap > m_size)
		{
			MemRef<T> mem = m_mem.Alloc()->template Allocate<T>(m_size * sizeof(T));
			if (m_mem.IsValid())
			{
				MemCpy(mem.Ptr(), m_mem.Ptr(), m_size * sizeof(T));
				m_mem.Dealloc();
			}
			m_mem = StdMove(mem);
		}
	}

	template <MoveConstructable T>
	auto DynArray<T>::Add(const T& val) noexcept -> void
	{
		InsertEnd(StdMove(T{ val }));
	}

	template <MoveConstructable T>
	auto DynArray<T>::Add(T&& val) noexcept -> void
	{
		InsertEnd(StdMove(val));
	}

	template <MoveConstructable T>
	auto DynArray<T>::Add(const DynArray<T>& other) -> void
	{
		Reserve(m_size + other.m_size);
		for (const T& val : other)
			InsertEnd(StdMove(T{ val }));
	}

	template <MoveConstructable T>
	auto DynArray<T>::Add(DynArray<T>&& other) -> void
	{
		const usize idx = m_size;
		m_size += other.m_size;
		Reserve(m_size);

		MemCpy(m_mem.Ptr() + idx, other.m_mem.Ptr(), other.Size() * sizeof(T));
		other.m_mem.Dealloc();
		other.m_size = 0;
	}

	template <MoveConstructable T>
	template <typename ...Args>
	auto DynArray<T>::EmplaceBack(Args&&... args) noexcept -> void
	{
		InsertEnd(StdMove(T{ StdForward<Args>(args)... }));
	}

	template <MoveConstructable T>
	auto DynArray<T>::Insert(const ConstIterator& it, const T& val) noexcept -> Iterator
	{
		return Emplace(it, val);
	}

	template <MoveConstructable T>
	auto DynArray<T>::Insert(const ConstIterator& it, T&& val) noexcept -> Iterator
	{
		return Emplace(it, StdMove(val));
	}

	template <MoveConstructable T>
	auto DynArray<T>::Insert(const ConstIterator& it, usize count, const T& val) noexcept -> Iterator
	{
		const usize offset = usize(it - m_mem.Ptr());
		ASSERT(offset <= m_size, "Iterator out of range");

		const usize endIdx = m_size;
		m_size += count;

		Iterator loc = PrepareInsert(offset, count);

		for (T* curLoc = loc, *end = loc + count; curLoc != end; ++curLoc)
			new (curLoc) T{ StdMove(val) };
		return loc;
	}

	template <MoveConstructable T>
	template <ForwardIterator It>
	auto DynArray<T>::Insert(const ConstIterator& it, const It& begin, const It& end) noexcept -> Iterator
	{
		const usize offset = usize(it - m_mem.Ptr());
		ASSERT(offset <= m_size, "Iterator out of range");
		
		const usize count = CountElems(begin, end);
		Iterator loc = PrepareInsert(offset, count);
		
		for (It curIt = begin; curIt != end; ++curIt, ++loc)
			new (loc) T{ *curIt };
		return loc;
	}

	template <MoveConstructable T>
	auto DynArray<T>::Insert(const ConstIterator& it, const InitializerList<T>& il) noexcept -> Iterator
	{
		const usize offset = usize(it - m_mem.Ptr());
		ASSERT(offset <= m_size, "Iterator out of range");
		
		Iterator loc = PrepareInsert(offset, il.size());
		
		for (const T* curIt = il.begin(); curIt != il.end(); ++curIt, ++loc)
			new (loc) T{ *curIt };
		return loc;
	}

	template <MoveConstructable T>
	auto DynArray<T>::Insert(const ConstIterator& it, const DynArray<T>& other) noexcept -> Iterator
	{
		const usize offset = usize(it - m_mem.Ptr());
		ASSERT(offset <= m_size, "Iterator out of range");

		Iterator loc = PrepareInsert(offset, other.m_size);

		for (const T* curIt = other.Begin(); curIt != other.End(); ++curIt, ++loc)
			new (loc) T{ *curIt };
		return loc;
	}

	template <MoveConstructable T>
	auto DynArray<T>::Insert(const ConstIterator& it, DynArray<T>&& other) noexcept -> Iterator
	{
		const usize offset = usize(it - m_mem.Ptr());
		ASSERT(offset <= m_size, "Iterator out of range");

		Iterator loc = PrepareInsert(offset, other.m_size);

		for (T* curIt = other.Begin(); curIt != other.End(); ++curIt, ++loc)
			new (loc) T{ StdMove(*curIt) };

		other.m_mem.Dealloc();
		other.m_size = 0;
		return loc;
	}

	template <MoveConstructable T>
	template <typename ... Args>
	auto DynArray<T>::Emplace(const ConstIterator& it, Args&&... args) noexcept -> Iterator
	{
		const usize offset = usize(it - m_mem.Ptr());
		ASSERT(offset <= m_size, "Iterator out of range");

		Iterator loc = PrepareInsert(offset, 1);
		
		new (loc) T{ StdForward<Args>(args)... };
		return loc;
	}

	template <MoveConstructable T>
	auto DynArray<T>::Clear(bool clearMemory) noexcept -> void
	{
		m_size = 0;
		if (clearMemory && m_mem)
		{
			Alloc::IAllocator* pAlloc = m_mem.Alloc();
			m_mem.Dealloc();
			m_mem = MemRef<T>(pAlloc);
		}
	}

	template <MoveConstructable T>
	auto DynArray<T>::Pop() noexcept -> void
	{
		--m_size;
	}

	template <MoveConstructable T>
	auto DynArray<T>::Erase(const Iterator& it) noexcept -> void
	{
		Erase(it, 1);
	}

	template <MoveConstructable T>
	auto DynArray<T>::Erase(const Iterator& it, usize count) noexcept -> void
	{
		const usize offset = usize(it - m_mem.Ptr());
		ASSERT(offset <= m_size, "Iterator out of range");

		const usize maxCount = m_size - offset;
		count = maxCount < count ? maxCount : count;
		const usize moveSize = maxCount - count;

		if (moveSize)
			MemMove(it, it + count, moveSize * sizeof(T));

		m_size -= count;
	}

	template <MoveConstructable T>
	auto DynArray<T>::Erase(const Iterator& begin, const Iterator& end) noexcept -> void
	{
		Erase(begin, usize(end - begin));
	}

	template <MoveConstructable T>
	template <EqualComparable<T> U>
	auto DynArray<T>::Find(const U& value) noexcept -> Iterator
	{
		T* pBegin = m_mem.Ptr();
		for (auto it = pBegin, end = pBegin + m_size; it != end; ++it)
		{
			if (*it == value)
				return it;
		}
		return pBegin + m_size;
	}

	template <MoveConstructable T>
	template <EqualComparable<T> U>
	auto DynArray<T>::Find(const U& value) const noexcept -> ConstIterator
	{
		T* pBegin = m_mem.Ptr();
		for (auto it = pBegin, end = pBegin + m_size; it != end; ++it)
		{
			if (*it == value)
				return it;
		}
		return pBegin + m_size;
	}

	template <MoveConstructable T>
	auto DynArray<T>::At(usize idx) const noexcept -> Optional<T>
	{
		if (idx < m_size) LIKELY
			return *(m_mem.Ptr() + idx);
		return {};
	}

	template <MoveConstructable T>
	auto DynArray<T>::IteratorAt(usize idx) noexcept -> Iterator
	{
		ASSERT(idx < m_size, "Index out of range");
		return m_mem.Ptr() + idx;
	}

	template <MoveConstructable T>
	auto DynArray<T>::IteratorAt(usize idx) const noexcept -> ConstIterator
	{
		ASSERT(idx < m_size, "Index out of range");
		return m_mem.Ptr() + idx;
	}

	template <MoveConstructable T>
	auto DynArray<T>::operator[](usize idx) noexcept -> T&
	{
		ASSERT(idx < m_size, "Index out of range");
		return *(m_mem.Ptr() + idx);
	}

	template <MoveConstructable T>
	auto DynArray<T>::operator[](usize idx) const noexcept -> const T&
	{
		ASSERT(idx < m_size, "Index out of range");
		return *(m_mem.Ptr() + idx);
	}

	template <MoveConstructable T>
	auto DynArray<T>::Size() const noexcept -> usize
	{
		return m_size;
	}

	template <MoveConstructable T>
	auto DynArray<T>::Capacity() const noexcept -> usize
	{
		return m_mem.Size() / sizeof(T);
	}

	template <MoveConstructable T>
	auto DynArray<T>::IsEmpty() const noexcept -> bool
	{
		return m_size == 0;
	}

	template <MoveConstructable T>
	auto DynArray<T>::Data() noexcept -> T*
	{
		return m_mem.Ptr();
	}

	template <MoveConstructable T>
	auto DynArray<T>::Data() const noexcept -> const T*
	{
		return m_mem.Ptr();
	}

	template <MoveConstructable T>
	auto DynArray<T>::Front() noexcept -> T&
	{
		ASSERT(m_size, "Invalid when DynArray is empty");
		return *m_mem.Ptr();
	}

	template <MoveConstructable T>
	auto DynArray<T>::Front() const noexcept -> const T&
	{
		ASSERT(m_size, "Invalid when DynArray is empty");
		return *m_mem.Ptr();
	}

	template <MoveConstructable T>
	auto DynArray<T>::Last() noexcept -> T&
	{
		ASSERT(m_size, "Invalid when DynArray is empty");
		return *(m_mem.Ptr() + m_size - 1);
	}

	template <MoveConstructable T>
	auto DynArray<T>::Last() const noexcept -> const T&
	{
		ASSERT(m_size, "Invalid when DynArray is empty");
		return *(m_mem.Ptr() + m_size - 1);
	}

	template <MoveConstructable T>
	auto DynArray<T>::Begin() noexcept -> Iterator
	{
		return m_mem.Ptr();
	}

	template <MoveConstructable T>
	auto DynArray<T>::Begin() const noexcept -> ConstIterator
	{
		return m_mem.Ptr();
	}

	template <MoveConstructable T>
	auto DynArray<T>::End() noexcept -> Iterator
	{
		return m_mem.Ptr() + m_size;
	}

	template <MoveConstructable T>
	auto DynArray<T>::End() const noexcept -> ConstIterator
	{
		return m_mem.Ptr() + m_size;
	}

	template <MoveConstructable T>
	auto DynArray<T>::RBegin() noexcept -> Iterator
	{
		return End();
	}

	template <MoveConstructable T>
	auto DynArray<T>::RBegin() const noexcept -> ConstIterator
	{
		return End();
	}

	template <MoveConstructable T>
	auto DynArray<T>::REnd() noexcept -> Iterator
	{
		return Begin();
	}

	template <MoveConstructable T>
	auto DynArray<T>::REnd() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <MoveConstructable T>
	auto DynArray<T>::GetAllocator() noexcept -> Alloc::IAllocator*
	{
		return m_mem.Alloc();
	}

	template <MoveConstructable T>
	auto DynArray<T>::begin() noexcept -> Iterator
	{
		return Begin();
	}

	template <MoveConstructable T>
	auto DynArray<T>::begin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <MoveConstructable T>
	auto DynArray<T>::cbegin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <MoveConstructable T>
	auto DynArray<T>::end() noexcept -> Iterator
	{
		return End();
	}

	template <MoveConstructable T>
	auto DynArray<T>::end() const noexcept -> ConstIterator
	{
		return End();
	}

	template <MoveConstructable T>
	auto DynArray<T>::cend() const noexcept -> ConstIterator
	{
		return End();
	}

	template <MoveConstructable T>
	auto DynArray<T>::rbegin() noexcept -> Iterator
	{
		return End();
	}

	template <MoveConstructable T>
	auto DynArray<T>::rbegin() const noexcept -> ConstIterator
	{
		return End();
	}

	template <MoveConstructable T>
	auto DynArray<T>::crbegin() const noexcept -> ConstIterator
	{
		return End();
	}

	template <MoveConstructable T>
	auto DynArray<T>::rend() noexcept -> Iterator
	{
		return Begin();
	}

	template <MoveConstructable T>
	auto DynArray<T>::rend() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <MoveConstructable T>
	auto DynArray<T>::crend() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <MoveConstructable T>
	auto DynArray<T>::InsertEnd(T&& val) noexcept -> Iterator
	{
		const usize idx = m_size++;
		Reserve(m_size);
		T* loc = m_mem.Ptr() + idx;
		new (loc) T{ StdMove(val) };
		return loc;
	}

	template <MoveConstructable T>
	auto DynArray<T>::PrepareInsert(usize offset, usize count) noexcept -> Iterator
	{
		const usize endIdx = m_size;
		m_size += count;

		Reserve(m_size);
		Iterator loc = m_mem.Ptr() + offset;

		if (offset != endIdx)
			MemMove(loc + count, loc, (endIdx - offset) * sizeof(T));

		return loc;
	}
}
