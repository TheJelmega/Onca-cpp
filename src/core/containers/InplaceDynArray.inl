#pragma once
#if __RESHARPER__
#include "InplaceDynArray.h"
#endif

#include "core/Assert.h"
#include "core/utils/Utils.h"
#include "core/utils/Algo.h"

namespace Core
{
	template <typename T, usize Cap>
	constexpr InplaceDynArray<T, Cap>::InplaceDynArray() noexcept
		: m_data{}
		, m_size(0)
	{
	}

	template <typename T, usize Cap>
	constexpr InplaceDynArray<T, Cap>::InplaceDynArray(usize count) noexcept requires CopyConstructible<T>
		: m_data{}
		, m_size(0)
	{
		FillDefault(count);
	}

	template <typename T, usize Cap>
	constexpr InplaceDynArray<T, Cap>::InplaceDynArray(usize count, const T& val) noexcept requires CopyConstructible<T>
		: m_data{}
		, m_size(0)
	{
		Fill(count, val);
	}

	template <typename T, usize Cap>
	constexpr InplaceDynArray<T, Cap>::InplaceDynArray(const InitializerList<T>& il) noexcept requires CopyConstructible<T>
		: m_data{}
		, m_size(0)
	{
		Assign(il);
	}

	template <typename T, usize Cap>
	template <ForwardIterator It>
	constexpr InplaceDynArray<T, Cap>::InplaceDynArray(const It& begin, const It& end) noexcept requires CopyConstructible<T>
		: m_data{}
		, m_size(0)
	{
		Assign(begin, end);
	}

	template <typename T, usize Cap>
	constexpr InplaceDynArray<T, Cap>::InplaceDynArray(const InplaceDynArray& other) noexcept requires CopyConstructible<T>
		: m_data{}
		, m_size(0)
	{
		Assign(other.Begin(), other.End());
	}

	template <typename T, usize Cap>
	constexpr InplaceDynArray<T, Cap>::InplaceDynArray(InplaceDynArray&& other) noexcept
		: m_size(other.m_size)
	{
		Algo::Move(reinterpret_cast<T*>(other.m_data), reinterpret_cast<T*>(m_data), m_size);
		other.m_size = 0;
	}

	template <typename T, usize Cap>
	constexpr InplaceDynArray<T, Cap>::~InplaceDynArray() noexcept
	{
		Clear();
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::operator=(const InitializerList<T>& il) noexcept -> InplaceDynArray& requires CopyConstructible<T>
	{
		Assign(il);
		return *this;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::operator=(const InplaceDynArray& other) noexcept -> InplaceDynArray& requires CopyConstructible<T>
	{
		Assign(other.Begin(), other.End());
		return *this;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::operator=(InplaceDynArray&& other) noexcept -> InplaceDynArray&
	{
		Clear();
		m_size = other.m_size;
		Algo::Move(reinterpret_cast<T*>(other.m_data), reinterpret_cast<T*>(m_data), m_size);
		other.m_size = 0;
		return *this;
	}

	template <typename T, usize Cap>
	template <ForwardIterator It>
	constexpr void InplaceDynArray<T, Cap>::Assign(const It& begin, const It& end) noexcept requires CopyConstructible<T>
	{
		Clear();
		for (It it = begin; it != end; ++it)
		{
			ASSERT(m_size != Cap, "InplaceDynArray has run out of space");
			IF_CONSTEVAL
				m_data[m_size] = T{ *it };
			else
				new (m_data + m_size) T{ *it };
			++m_size;
		}
	}

	template <typename T, usize Cap>
	constexpr void InplaceDynArray<T, Cap>::Assign(const InitializerList<T>& il) noexcept requires CopyConstructible<T>
	{
		Clear();
		ASSERT(il.size() <= Cap, "ImplaceDynArray has not enough capacity to store the initializer list");
		for (const T* it = il.begin(), *end = il.end(); it != end; ++it)
		{
			IF_CONSTEVAL
				m_data[m_size] = T{ *it };
			else
				new (m_data + m_size) T{ *it };
			++m_size;
		}
	}

	template <typename T, usize Cap>
	constexpr void InplaceDynArray<T, Cap>::Fill(usize count, const T& val) noexcept requires CopyConstructible<T>
	{
		Clear();
		ASSERT(count <= Cap, "ImplaceDynArray has not enough capacity to store the initializer list");
		for (usize i = 0; i < count; ++i)
		{
			IF_CONSTEVAL
				m_data[m_size] = T{ val };
			else
				new (m_data + m_size) T{ val };
			++m_size;
		}
	}

	template <typename T, usize Cap>
	constexpr void InplaceDynArray<T, Cap>::FillDefault(usize count) noexcept requires NoThrowDefaultConstructible<T>
	{
		Clear();
		ASSERT(count <= Cap, "ImplaceDynArray has not enough capacity to store the initializer list");
		for (usize i = 0; i < count; ++i)
		{
			IF_CONSTEVAL
				m_data[i] = T{};
			else
				new (m_data + i) T{};
		}
		m_size = count;
	}
	
	template <typename T, usize Cap>
	constexpr void InplaceDynArray<T, Cap>::Resize(usize newSize, const T& val) noexcept requires CopyConstructible<T>
	{
		ASSERT(newSize <= Cap, "Trying to resize InplaceDynArray to a size larger than its capacity");
		if (newSize < m_size)
		{
			for (usize i = m_size; i < newSize; ++i)
				m_data[m_size--].~T();
		}
		else
		{
			for (usize i = m_size; i < newSize; ++i)
			{
				IF_CONSTEVAL
					m_data[i] = T{ val };
				else
					new (m_data + i) T{ val };
			}
		}
		m_size = newSize;
	}

	template <typename T, usize Cap>
	constexpr void InplaceDynArray<T, Cap>::Resize(usize newSize) noexcept requires NoThrowDefaultConstructible<T>
	{
		ASSERT(newSize <= Cap, "Trying to resize InplaceDynArray to a size larger than its capacity");
		if (newSize < m_size)
		{
			for (usize i = m_size; i < newSize; ++i)
				m_data[m_size--].~T();
		}
		else
		{
			for (usize i = m_size; i < newSize; ++i)
			{
				IF_CONSTEVAL
					m_data[i] = T{};
				else
					new (m_data + i) T{};
			}
		}
		m_size = newSize;
	}
	
	template <typename T, usize Cap>
	constexpr void InplaceDynArray<T, Cap>::Add(const T& val) noexcept requires CopyConstructible<T>
	{
		Add(T{ val });
	}

	template <typename T, usize Cap>
	constexpr void InplaceDynArray<T, Cap>::Add(T&& val) noexcept
	{
		ASSERT(m_size < Cap, "ImplaceDynArray has not enough capacity to store another element");
		IF_CONSTEVAL
			m_data[m_size] = T{ Move(val) };
		else
			new (m_data + m_size) T{ Move(val) };
		++m_size;
	}

	template <typename T, usize Cap>
	constexpr void InplaceDynArray<T, Cap>::Add(const InplaceDynArray& other) requires CopyConstructible<T>
	{
		ASSERT(m_size + other.m_size <= Cap, "ImplaceDynArray has not enough capacity to add another InplaceDynArray");
		for (const T* it = other.Begin(), *end = other.End(); it != end; ++it)
			Add(T{ *it });
	}

	template <typename T, usize Cap>
	constexpr void InplaceDynArray<T, Cap>::Add(InplaceDynArray&& other)
	{
		ASSERT(m_size + other.m_size <= Cap, "ImplaceDynArray has not enough capacity to add another InplaceDynArray");
		for (const T* it = other.Begin(), *end = other.End(); it != end; ++it)
			Add(Move(*it));
		other.m_size = 0;
	}

	template <typename T, usize Cap>
	template <typename ... Args> requires ConstructableFrom<T, Args...>
	constexpr void InplaceDynArray<T, Cap>::EmplaceBack(Args&&... args) noexcept
	{
		Add(T{ args... });
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::Insert(const ConstIterator& it, const T& val) noexcept -> Iterator requires CopyConstructible<T>
	{
		return Insert(it, T{ val });
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::Insert(const ConstIterator& it, T&& val) noexcept -> Iterator
	{
		ASSERT(m_size < Cap, "ImplaceDynArray has not enough capacity to insert an element");
		usize idx = usize(it - m_data);
		Algo::Move(it, m_data + idx + 1, m_size - idx);
		new (m_data + idx) T{ Move(val) };
		++m_size;
		return m_data + idx;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::Insert(const ConstIterator& it, usize count, const T& val) noexcept -> Iterator requires CopyConstructible<T>
	{
		ASSERT(m_size + count <= Cap, "ImplaceDynArray has not enough capacity to insert an elements");
		usize idx = usize(it - m_data);
		Algo::Move(it, m_data + idx + count, m_size - idx);
		for (usize i = idx, end = idx + count; i < end; ++i)
			new (m_data + i) T{ val };
		m_size += count;
		return m_data + idx;

	}

	template <typename T, usize Cap>
	template <ForwardIterator It>
	constexpr auto InplaceDynArray<T, Cap>::Insert(const ConstIterator& it, const It& begin, const It& end) noexcept -> Iterator requires CopyConstructible<T>
	{
		usize count = CountElems(begin, end);
		ASSERT(m_size + count <= Cap, "ImplaceDynArray has not enough capacity to insert an elements");
		const usize idx = usize(it - m_data);
		ASSERT(idx <= m_size, "Iterator out of range");
		Algo::Move(it, m_data + idx + count, m_size - idx);
		usize i = 0;
		for (It valIt = begin; valIt != end; ++valIt, ++i)
			new (m_data + idx + i) T{ *valIt };
		m_size += count;
		return m_data + idx;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::Insert(const ConstIterator& it, const InitializerList<T>& il) noexcept -> Iterator requires CopyConstructible<T>
	{
		usize count = il.size();
		ASSERT(m_size + count <= Cap, "ImplaceDynArray has not enough capacity to insert an elements");
		const usize idx = usize(it - m_data);
		ASSERT(idx <= m_size, "Iterator out of range");
		Algo::Move(it, m_data + idx + count, m_size - idx);
		usize i = 0;
		for (const T* valIt = il.begin(), *end = il.end(); valIt != end; ++valIt, ++i)
			new (m_data + idx + i) T{ *valIt };
		m_size += count;
		return m_data + idx;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::Insert(const ConstIterator& it, const InplaceDynArray& other) noexcept -> Iterator requires CopyConstructible<T>
	{
		usize count = other.m_size;
		ASSERT(m_size + count <= Cap, "ImplaceDynArray has not enough capacity to insert an elements");
		const usize idx = usize(it - m_data);
		ASSERT(idx <= m_size, "Iterator out of range");
		Algo::Move(it, m_data + idx + count, m_size - idx);
		usize i = 0;
		for (const T* valIt = other.Begin(), *end = other.End(); valIt != end; ++valIt, ++i)
			new (m_data + idx + i) T{ *valIt };
		m_size += count;
		return m_data + idx;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::Insert(const ConstIterator& it, InplaceDynArray&& other) noexcept -> Iterator
	{
		usize count = other.m_size;
		ASSERT(m_size + count <= Cap, "ImplaceDynArray has not enough capacity to insert an elements");
		const usize idx = usize(it - m_data);
		ASSERT(idx <= m_size, "Iterator out of range");
		Algo::Move(it, m_data + idx + count, m_size - idx);
		usize i = 0;
		for (const T* valIt = other.Begin(), *end = other.End(); valIt != end; ++valIt, ++i)
			new (m_data + idx + i) T{ Move(*valIt) };
		m_size += count;
		other.m_size = 0;
		return m_data + idx;
	}

	template <typename T, usize Cap>
	template <typename ... Args> requires ConstructableFrom<T, Args...>
		constexpr auto InplaceDynArray<T, Cap>::Emplace(const ConstIterator& it, Args&&... args) noexcept -> Iterator
	{
		return Insert(it, T{ args... });
	}

	template <typename T, usize Cap>
	constexpr void InplaceDynArray<T, Cap>::Clear() noexcept
	{
		for (usize i = 0; i < m_size; ++i)
			m_data[i].~T();
		m_size = 0;
	}

	template <typename T, usize Cap>
	constexpr void InplaceDynArray<T, Cap>::Pop() noexcept
	{
		ASSERT(m_size, "Cannot pop from an empty DynArray");
		m_data[--m_size].~T();
	}

	template <typename T, usize Cap>
	constexpr void InplaceDynArray<T, Cap>::Erase(const Iterator& it) noexcept
	{
		Erase(it, 1);
	}

	template <typename T, usize Cap>
	constexpr void InplaceDynArray<T, Cap>::Erase(const Iterator& it, usize count) noexcept
	{
		const usize idx = usize(it - m_data);
		ASSERT(idx <= m_size, "Iterator out of range");
		ASSERT(count < m_size, "Cannot remove more elements than are in the InplaceDynArray");

		const usize moveCount = m_size - idx - count;
		if (moveCount)
			Algo::Move(it + count, m_data + idx, moveCount);
		m_size -= count;
	}

	template <typename T, usize Cap>
	constexpr void InplaceDynArray<T, Cap>::Erase(const Iterator& begin, const Iterator& end) noexcept
	{
		Erase(begin, usize(end - begin));
	}

	template <typename T, usize Cap>
	template <EqualComparable<T> U>
	constexpr auto InplaceDynArray<T, Cap>::Find(const U& value) noexcept -> Iterator
	{
		for (T* it = m_data, *end = m_data + m_size; it != end; ++it)
		{
			if (value == *it)
				return it;
		}
		return nullptr;
	}

	template <typename T, usize Cap>
	template <EqualComparable<T> U>
	constexpr auto InplaceDynArray<T, Cap>::Find(const U& value) const noexcept -> ConstIterator
	{
		for (const T* it = m_data, *end = m_data + m_size; it != end; ++it)
		{
			if (value == *it)
				return it;
		}
		return nullptr;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::At(usize idx) const noexcept -> Optional<T>
	{
		if (idx >= m_size)
			return NullOpt;
		return m_data[idx];
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::IteratorAt(usize idx) noexcept -> Iterator
	{
		FREQ_ASSERT(idx < m_size, "Index out of range");
		return m_data + idx;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::IteratorAt(usize idx) const noexcept -> ConstIterator
	{
		FREQ_ASSERT(idx < m_size, "Index out of range");
		return m_data + idx;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::operator[](usize idx) noexcept -> T&
	{
		FREQ_ASSERT(idx < m_size, "Index out of range");
		return m_data[idx];
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::operator[](usize idx) const noexcept -> const T&
	{
		FREQ_ASSERT(idx < m_size, "Index out of range");
		return m_data[idx];
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::Capacity() const noexcept -> usize
	{
		return Cap;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::Size() const noexcept -> usize
	{
		return m_size;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::IsEmpty() const noexcept -> bool
	{
		return m_size == 0;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::Data() noexcept -> T*
	{
		return m_data;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::Data() const noexcept -> const T*
	{
		return m_data;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::Front() noexcept -> T&
	{
		ASSERT(m_size, "Invalid when InplaceDynArray is empty");
		return m_data[0];
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::Front() const noexcept -> const T&
	{
		ASSERT(m_size, "Invalid when InplaceDynArray is empty");
		return m_data[0];
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::Back() noexcept -> T&
	{
		ASSERT(m_size, "Invalid when InplaceDynArray is empty");
		return m_data[m_size - 1];
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::Back() const noexcept -> const T&
	{
		ASSERT(m_size, "Invalid when InplaceDynArray is empty");
		return m_data[m_size - 1];
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::Begin() noexcept -> Iterator
	{
		return m_data;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::Begin() const noexcept -> ConstIterator
	{
		return m_data;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::End() noexcept -> Iterator
	{
		return m_data + m_size;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::End() const noexcept -> ConstIterator
	{
		return m_data + m_size;
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::RBegin() noexcept -> Iterator
	{
		return End();
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::RBegin() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::REnd() noexcept -> Iterator
	{
		return Begin();
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::REnd() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::begin() noexcept -> Iterator
	{
		return Begin();
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::begin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::cbegin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::end() noexcept -> Iterator
	{
		return End();
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::end() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename T, usize Cap>
	constexpr auto InplaceDynArray<T, Cap>::cend() const noexcept -> ConstIterator
	{
		return End();
	}
}
