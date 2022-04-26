#pragma once
#if __RESHARPER__
#include "DynArray.h"
#endif

#include "core/utils/Algo.h"

namespace Core
{
	template <typename T>
	DynArray<T>::DynArray(Alloc::IAllocator& alloc) noexcept
		: m_mem(&alloc)
		, m_size(0)
	{
	}

	template <typename T>
	DynArray<T>::DynArray(usize capacity, Alloc::IAllocator& alloc) noexcept
		: m_mem(&alloc)
		, m_size(0)
	{
		Reserve(capacity);
	}

	template <typename T>
	DynArray<T>::DynArray(usize count, const T& val, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>
		: m_mem(&alloc)
		, m_size(0)
	{
		Resize(count, val);
	}

	template <typename T>
	DynArray<T>::DynArray(const InitializerList<T>& il, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>
		: m_mem(&alloc)
		, m_size(0)
	{
		Assign(il);
	}

	template <typename T>
	template <ForwardIterator It>
	DynArray<T>::DynArray(const It& begin, const It& end, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>
		: m_mem(&alloc)
		, m_size(0)
	{
		Assign(begin, end);
	}

	template <typename T>
	DynArray<T>::DynArray(const DynArray& other) noexcept requires CopyConstructible<T>
		: m_mem(other.GetAllocator())
		, m_size(0)
	{
		Reserve(other.m_size);
		Assign(other.Begin(), other.End());
	}

	template <typename T>
	DynArray<T>::DynArray(const DynArray& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructible<T>
		: m_mem(&alloc)
		, m_size(0)
	{
		Reserve(other.m_size);
		Assign(other.Begin(), other.End());
	}

	template <typename T>
	DynArray<T>::DynArray(DynArray&& other) noexcept
		: m_mem(Move(other.m_mem))
		, m_size(Move(other.m_size))
	{
		other.m_size = 0;
	}

	template <typename T>
	DynArray<T>::DynArray(DynArray&& other, Alloc::IAllocator& alloc) noexcept
		: m_mem(&alloc)
		, m_size(other.m_size)
	{
		Reserve(m_size);
		if constexpr (MemCopyable<T>)
			MemCpy(m_mem.Ptr(), other.m_mem.Ptr(), m_size);
		else
			Algo::Move(other.m_mem.Ptr(), m_mem.Ptr(), m_size);
		other.m_mem.Dealloc();
		other.m_mem = nullptr;
	}

	template <typename T>
	DynArray<T>::~DynArray() noexcept
	{
		Clear(true);
	}

	template <typename T>
	auto DynArray<T>::operator=(const InitializerList<T>& il) noexcept -> DynArray<T>& requires CopyConstructible<T>
	{
		Assign(il);
		return *this;
	}

	template <typename T>
	auto DynArray<T>::operator=(const DynArray<T>& other) noexcept -> DynArray<T>& requires CopyConstructible<T>
	{
		Assign(other.Begin(), other.End());
		return *this;
	}

	template <typename T>
	auto DynArray<T>::operator=(DynArray&& other) noexcept -> DynArray<T>&
	{
		Clear(true);
		m_mem = Move(other.m_mem);
		m_size = other.m_size;
		other.m_size = 0;
		return *this;
	}

	template <typename T>
	template <ForwardIterator It>
	void DynArray<T>::Assign(const It& begin, const It& end) noexcept requires CopyConstructible<T>
	{
		Clear();
		if (!(begin != end))
			return;

		if constexpr (RandomAccessIterator<It>)
		{
			ASSERT(begin < end, "'begin' iterator must be smaller than 'end' iterator");
			Reserve(usize(end - begin));
		}

		if constexpr (ContiguousIterator<It> && MemCopyable<T>)
		{
			const usize size = usize(end - begin);
			T* pBegin = m_mem.Ptr();
			MemCpy(pBegin, &*begin, size);
			m_size = size; 
		}
		else
		{
			for(It it = begin; it != end; ++it)
				InsertEnd(Move(T{ *it }));
		}
	}
	

	template <typename T>
	void DynArray<T>::Assign(const InitializerList<T>& il) noexcept requires CopyConstructible<T>
	{
		Clear();
		const usize size = il.size();
		Reserve(size);
		if constexpr (MemCopyable<T>)
		{
			T* pBegin = m_mem.Ptr();
			Algo::Copy(il.begin(), pBegin, size);
			m_size = size;
		}
		else
		{
			for (const T* it = il.begin(), *end = il.end(); it != end; ++it)
				InsertEnd(Move(T{ *it }));
		}
	}

	template <typename T>
	void DynArray<T>::Fill(usize count, const T& val) noexcept requires CopyConstructible<T>
	{
		STATIC_ASSERT(CopyConstructible<T>, "T needs to be copy constructable");
		Clear();
		Resize(count, val);
	}

	template <typename T>
	void DynArray<T>::FillDefault(usize count) noexcept requires NoThrowDefaultConstructible<T>
	{
		Clear();
		Resize(count);
	}

	template <typename T>
	void DynArray<T>::Reserve(usize newCap) noexcept
	{
		const usize curCap = Capacity();
		if (curCap >= newCap)
			return;

		// Capacity increases in 1.5x steps to balance memory used vs number of reserves
		// Actual results of capacity calculation may differ from other because calls to ShrinkToFit
		usize cap = curCap == 0 ? 2 : curCap;
		while (cap < newCap)
			cap = (cap << 1) - (cap >> 1);

		// TODO: Allocators, align to min 8 bytes boundaries
		MemRef<T> mem = m_mem.GetAlloc()->template Allocate<T>(cap * sizeof(T), Math::Max(8, alignof(T)));
		if (m_mem.IsValid())
		{
			Algo::Move(m_mem.Ptr(), mem.Ptr(), m_size);
			m_mem.Dealloc();
		}
		m_mem = Move(mem);
		ASSERT(m_mem, "Failed to allocate memory");
	}

	template <typename T>
	void DynArray<T>::Resize(usize newSize, const T& val) noexcept requires CopyConstructible<T>
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
			T* pBegin = m_mem.Ptr();
			for (usize i = m_size; i < newSize; ++i)
				new (pBegin + i) T{ val };
		}
		m_size = newSize;
	}

	template <typename T>
	void DynArray<T>::Resize(usize newSize) noexcept requires NoThrowDefaultConstructible<T>
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
			T* pBegin = m_mem.Ptr();
			if constexpr (IsPrimitive<T>)
			{
				MemClear(pBegin + m_size, (newSize - m_size) * sizeof(T));
			}
			else
			{
				for (T* it = pBegin + m_size, *end = pBegin + newSize; it != end; ++it)
					new (it) T{};
			}
		}
		m_size = newSize;
	}

	template <typename T>
	void DynArray<T>::ShrinkToFit() noexcept
	{
		usize cap = Capacity();
		if (cap > m_size)
		{
			MemRef<T> mem = m_mem.GetAlloc()->template Allocate<T>(m_size * sizeof(T));
			if (m_mem.IsValid())
			{
				if (MemCopyable<T>)
					MemCpy(mem.Ptr(), m_mem.Ptr(), m_size);
				else
					Algo::Copy(m_mem.Ptr(), mem.Ptr(), m_size);
				m_mem.Dealloc();
			}
			m_mem = Move(mem);
		}
	}

	template <typename T>
	void DynArray<T>::Add(const T& val) noexcept requires CopyConstructible<T>
	{
		InsertEnd(Move(T{ val }));
	}

	template <typename T>
	void DynArray<T>::Add(T&& val) noexcept
	{
		InsertEnd(Move(val));
	}

	template <typename T>
	void DynArray<T>::Add(const DynArray& other) requires CopyConstructible<T>
	{
		Reserve(m_size + other.m_size);
		if constexpr (MemCopyable<T>)
		{
			Algo::Move(other.m_mem.Ptr(), m_mem.Ptr() + m_size, other.m_size);
			m_size += other.m_size;
		}
		else
		{
			for (const T& val : other)
				InsertEnd(Move(T{ val }));
		}
	}

	template <typename T>
	void DynArray<T>::Add(DynArray&& other)
	{
		const usize idx = m_size;
		m_size += other.m_size;
		Reserve(m_size);

		Algo::Move(other.m_mem.Ptr(), m_mem.Ptr() + idx, other.Size());
		other.m_mem.Dealloc();
		other.m_size = 0;
	}

	template <typename T>
	auto DynArray<T>::AddUnique(const T& val) noexcept -> bool requires CopyConstructible<T>
	{
		if (!Contains(val))
		{
			Add(val);
			return true;
		}
		return false;
	}

	template <typename T>
	auto DynArray<T>::AddUnique(T&& val) noexcept -> bool
	{
		if (!Contains(val))
		{
			Add(Move(val));
			return  true;
		}
		return false;
	}

	template <typename T>
	template <typename ...Args>
		requires ConstructableFrom<T, Args...>
	void DynArray<T>::EmplaceBack(Args&&... args) noexcept
	{
		InsertEnd(Move(T{ Forward<Args>(args)... }));
	}

	template <typename T>
	auto DynArray<T>::Insert(ConstIterator& it, const T& val) noexcept -> Iterator requires CopyConstructible<T>
	{
		return Emplace(it, val);
	}

	template <typename T>
	auto DynArray<T>::Insert(ConstIterator& it, T&& val) noexcept -> Iterator
	{
		return Emplace(it, Move(val));
	}

	template <typename T>
	auto DynArray<T>::Insert(ConstIterator& it, usize count, const T& val) noexcept -> Iterator requires CopyConstructible<T>
	{
		const usize offset = usize(it - m_mem.Ptr());
		ASSERT(offset <= m_size, "Iterator out of range");

		Iterator loc = PrepareInsert(offset, count);

		for (T* curLoc = loc, *end = loc + count; curLoc != end; ++curLoc)
			new (curLoc) T{ Move(val) };
		return loc;
	}

	template <typename T>
	template <ForwardIterator It>
	auto DynArray<T>::Insert(ConstIterator& it, const It& begin, const It& end) noexcept -> Iterator requires CopyConstructible<T>
	{
		const usize offset = usize(it - m_mem.Ptr());
		ASSERT(offset <= m_size, "Iterator out of range");
		
		const usize count = CountElems(begin, end);
		Iterator loc = PrepareInsert(offset, count);
		
		for (It curIt = begin; curIt != end; ++curIt, ++loc)
			new (loc) T{ *curIt };
		return loc;
	}

	template <typename T>
	auto DynArray<T>::Insert(ConstIterator& it, const InitializerList<T>& il) noexcept -> Iterator requires CopyConstructible<T>
	{
		const usize offset = usize(it - m_mem.Ptr());
		ASSERT(offset <= m_size, "Iterator out of range");
		
		Iterator loc = PrepareInsert(offset, il.size());
		
		for (const T* curIt = il.begin(); curIt != il.end(); ++curIt, ++loc)
			new (loc) T{ *curIt };
		return loc;
	}

	template <typename T>
	auto DynArray<T>::Insert(ConstIterator& it, const DynArray& other) noexcept -> Iterator requires CopyConstructible<T>
	{
		const usize offset = usize(it - m_mem.Ptr());
		ASSERT(offset <= m_size, "Iterator out of range");

		Iterator loc = PrepareInsert(offset, other.m_size);

		for (const T* curIt = other.Begin(); curIt != other.End(); ++curIt, ++loc)
			new (loc) T{ *curIt };
		return loc;
	}

	template <typename T>
	auto DynArray<T>::Insert(ConstIterator& it, DynArray&& other) noexcept -> Iterator
	{
		const usize offset = usize(it - m_mem.Ptr());
		ASSERT(offset <= m_size, "Iterator out of range");

		Iterator loc = PrepareInsert(offset, other.m_size);

		for (T* curIt = other.Begin(); curIt != other.End(); ++curIt, ++loc)
			new (loc) T{ Move(*curIt) };

		other.m_mem.Dealloc();
		other.m_size = 0;
		return loc;
	}

	template <typename T>
	template <typename ... Args>
		requires ConstructableFrom<T, Args...>
	auto DynArray<T>::Emplace(ConstIterator& it, Args&&... args) noexcept -> Iterator
	{
		const usize offset = usize(it - m_mem.Ptr());
		ASSERT(offset <= m_size, "Iterator out of range");

		Iterator loc = PrepareInsert(offset, 1);
		
		new (loc) T{ Forward<Args>(args)... };
		return loc;
	}

	template <typename T>
	void DynArray<T>::Clear(bool clearMemory) noexcept
	{
		m_size = 0;
		if (clearMemory && m_mem)
		{
			Alloc::IAllocator* pAlloc = m_mem.GetAlloc();
			m_mem.Dealloc();
			m_mem = MemRef<T>(pAlloc);
		}
	}

	template <typename T>
	void DynArray<T>::Pop() noexcept
	{
		ASSERT(m_size, "Cannot pop from an empty DynArray");
		--m_size;
		(m_mem.Ptr() + m_size)->~T();
	}

	template <typename T>
	void DynArray<T>::Erase(ConstIterator& it) noexcept
	{
		Erase(it, 1);
	}

	template <typename T>
	void DynArray<T>::Erase(ConstIterator& it, usize count) noexcept
	{
		const usize offset = usize(it - m_mem.Ptr());
		ASSERT(offset < m_size, "Iterator out of range");

		const usize maxCount = m_size - offset;
		count = maxCount < count ? maxCount : count;
		const usize moveSize = maxCount - count;

		T* pIt = const_cast<T*>(it);
		T* eraseIt = pIt;
		for (usize i = 0; i < count; ++i, ++eraseIt)
			eraseIt->~T();

		if (moveSize)
			MemMove(pIt, pIt + count, moveSize * sizeof(T));

		m_size -= count;
	}

	template <typename T>
	void DynArray<T>::Erase(ConstIterator& begin, ConstIterator& end) noexcept
	{
		Erase(begin, usize(end - begin));
	}

	template <typename T>
	template <EqualComparable<T> U>
	void DynArray<T>::Erase(const U& val, bool onlyFirst) noexcept
	{
		if (onlyFirst)
		{
			auto it = Find(val);
			if (it != End())
				Erase(it);
		}
		else
		{
			auto it = Find(val);
			while (it != End())
			{
				Erase(it);
				it = Find(val);
			}
		}
	}

	template <typename T>
	template <Callable<bool, const T&> F>
	void DynArray<T>::EraseIf(F fun) noexcept
	{
		for (auto it = Begin(); it != End();)
		{
			if (fun(*it))
				Erase(it);
			else
				++it;
		}
	}

	template <typename T>
	auto DynArray<T>::Extract(ConstIterator& it) noexcept -> T
	{
		const usize offset = usize(it - m_mem.Ptr());
		ASSERT(offset < m_size, "Iterator out of range");

		T* pIt = const_cast<T*>(it);
		T tmp = Move(*pIt);

		const usize moveSize = m_size - offset - 1;
		if (moveSize)
			MemMove(pIt, pIt + 1, moveSize * sizeof(T));
		--m_size;

		return tmp;
	}

	template <typename T>
	auto DynArray<T>::Extract(usize idx) noexcept -> T
	{
		ASSERT(idx < m_size, "Index out of range");

		T* pIt = m_mem.Ptr() + idx;
		T tmp = Move(*pIt);

		const usize moveSize = m_size - idx - 1;
		if (moveSize)
			MemMove(pIt, pIt + 1, moveSize * sizeof(T));
		--m_size;

		return tmp;
	}

	template <typename T>
	template <EqualComparable<T> U>
	auto DynArray<T>::Find(const U& value) noexcept -> Iterator
	{
		for (auto it = Begin(), end = End(); it != end; ++it)
		{
			if (*it == value)
				return it;
		}
		return End();
	}

	template <typename T>
	template <EqualComparable<T> U>
	auto DynArray<T>::Find(const U& value) const noexcept -> ConstIterator
	{
		for (auto it = Begin(), end = End(); it != end; ++it)
		{
			if (*it == value)
				return it;
		}
		return End();
	}

	template <typename T>
	template <Callable<bool, const T&> F>
	auto DynArray<T>::FindIf(F fun) noexcept -> Iterator
	{
		for (auto it = Begin(), end = End(); it != end; ++it)
		{
			if (fun(*it))
				return it;
		}
		return End();
	}

	template <typename T>
	template <Callable<bool, const T&> F>
	auto DynArray<T>::FindIf(F fun) const noexcept -> ConstIterator
	{
		for (auto it = Begin(), end = End(); it != end; ++it)
		{
			if (fun(*it))
				return it;
		}
		return End();
	}

	template <typename T>
	template<EqualComparable<T> U>
	auto DynArray<T>::Contains(const U& value) const noexcept -> bool
	{
		T* pBegin = m_mem.Ptr();
		for (auto it = pBegin, end = pBegin + m_size; it != end; ++it)
		{
			if (*it == value)
				return true;
		}
		return false;
	}

	template <typename T>
	template <Callable<bool, const T&> F>
	auto DynArray<T>::ContainsIf(F fun) const noexcept -> bool
	{
		for (auto it = Begin(), end = End(); it != end; ++it)
		{
			if (fun(*it))
				return true;
		}
		return false;
	}

	template <typename T>
	auto DynArray<T>::At(usize idx) const noexcept -> Optional<T>
	{
		if (idx < m_size) LIKELY
			return *(m_mem.Ptr() + idx);
		return NullOpt;
	}

	template <typename T>
	auto DynArray<T>::IteratorAt(usize idx) noexcept -> Iterator
	{
		FREQ_ASSERT(idx <= m_size, "Index out of range");
		return m_mem.Ptr() + idx;
	}

	template <typename T>
	auto DynArray<T>::IteratorAt(usize idx) const noexcept -> ConstIterator
	{
		FREQ_ASSERT(idx <= m_size, "Index out of range");
		return m_mem.Ptr() + idx;
	}

	template <typename T>
	auto DynArray<T>::operator[](usize idx) noexcept -> T&
	{
		FREQ_ASSERT(idx < m_size, "Index out of range");
		return *(m_mem.Ptr() + idx);
	}

	template <typename T>
	auto DynArray<T>::operator[](usize idx) const noexcept -> const T&
	{
		FREQ_ASSERT(idx < m_size, "Index out of range");
		return *(m_mem.Ptr() + idx);
	}

	template <typename T>
	auto DynArray<T>::Size() const noexcept -> usize
	{
		return m_size;
	}

	template <typename T>
	auto DynArray<T>::Capacity() const noexcept -> usize
	{
		return m_mem.Size() / sizeof(T);
	}

	template <typename T>
	auto DynArray<T>::IsEmpty() const noexcept -> bool
	{
		return m_size == 0;
	}

	template <typename T>
	auto DynArray<T>::Data() noexcept -> T*
	{
		return m_mem.Ptr();
	}

	template <typename T>
	auto DynArray<T>::Data() const noexcept -> const T*
	{
		return m_mem.Ptr();
	}

	template <typename T>
	auto DynArray<T>::GetAllocator() const noexcept -> Alloc::IAllocator*
	{
		return m_mem.GetAlloc();
	}

	template <typename T>
	auto DynArray<T>::Front() noexcept -> T&
	{
		ASSERT(m_size, "Invalid when DynArray is empty");
		return *m_mem.Ptr();
	}

	template <typename T>
	auto DynArray<T>::Front() const noexcept -> const T&
	{
		ASSERT(m_size, "Invalid when DynArray is empty");
		return *m_mem.Ptr();
	}

	template <typename T>
	auto DynArray<T>::Back() noexcept -> T&
	{
		ASSERT(m_size, "Invalid when DynArray is empty");
		return *(m_mem.Ptr() + m_size - 1);
	}

	template <typename T>
	auto DynArray<T>::Back() const noexcept -> const T&
	{
		ASSERT(m_size, "Invalid when DynArray is empty");
		return *(m_mem.Ptr() + m_size - 1);
	}

	template <typename T>
	auto DynArray<T>::Begin() noexcept -> Iterator
	{
		return m_mem.Ptr();
	}

	template <typename T>
	auto DynArray<T>::Begin() const noexcept -> ConstIterator
	{
		return m_mem.Ptr();
	}

	template <typename T>
	auto DynArray<T>::End() noexcept -> Iterator
	{
		return m_mem.Ptr() + m_size;
	}

	template <typename T>
	auto DynArray<T>::End() const noexcept -> ConstIterator
	{
		return m_mem.Ptr() + m_size;
	}

	template <typename T>
	auto DynArray<T>::RBegin() noexcept -> Iterator
	{
		return End();
	}

	template <typename T>
	auto DynArray<T>::RBegin() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename T>
	auto DynArray<T>::REnd() noexcept -> Iterator
	{
		return Begin();
	}

	template <typename T>
	auto DynArray<T>::REnd() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T>
	auto DynArray<T>::begin() noexcept -> Iterator
	{
		return Begin();
	}

	template <typename T>
	auto DynArray<T>::begin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T>
	auto DynArray<T>::cbegin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T>
	auto DynArray<T>::end() noexcept -> Iterator
	{
		return End();
	}

	template <typename T>
	auto DynArray<T>::end() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename T>
	auto DynArray<T>::cend() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename T>
	auto DynArray<T>::InsertEnd(T&& val) noexcept -> Iterator
	{
		Reserve(m_size + 1);
		const usize idx = m_size++;
		T* loc = m_mem.Ptr() + idx;
		new (loc) T{ Move(val) };
		return loc;
	}

	template <typename T>
	auto DynArray<T>::PrepareInsert(usize offset, usize count) noexcept -> Iterator
	{
		const usize endIdx = m_size;
		m_size += count;

		Reserve(m_size);
		Iterator from = m_mem.Ptr() + offset;

		if (offset == endIdx)
			return from;

		Iterator to = from + count;
		Algo::Move(from, to, endIdx - offset);
		return from;
	}
}
