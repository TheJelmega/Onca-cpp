#pragma once
#include "IntrusiveDList.h"
#include "core/Assert.h"

namespace Core
{
	template <typename Owner>
	IntrusiveDListNode<Owner>::IntrusiveDListNode(usize memberOffset) noexcept
		: m_pPrev(nullptr)
		, m_pNext(nullptr)
		, m_offset(memberOffset)
	{
	}

	template <typename Owner>
	auto IntrusiveDListNode<Owner>::operator=(const IntrusiveDListNode& other) noexcept -> IntrusiveDListNode&
	{
		m_pPrev = other.m_pPrev;
		m_pNext = other.m_pNext;
		m_offset = other.m_offset;
		return *this;
	}

	template <typename Owner>
	auto IntrusiveDListNode<Owner>::operator=(IntrusiveDListNode&& other) noexcept -> IntrusiveDListNode&
	{
		m_pPrev = other.m_pPrev;
		m_pNext = other.m_pNext;
		m_offset = other.m_offset;
		return *this;
	}

	template <typename Owner>
	auto IntrusiveDListNode<Owner>::Get() noexcept -> Owner*
	{
		return reinterpret_cast<Owner*>((u8*)this - m_offset);
	}

	template <typename Owner>
	auto IntrusiveDListNode<Owner>::Get() const noexcept -> const Owner*
	{
		return reinterpret_cast<Owner*>((u8*)this - m_offset);
	}

	template <typename T>
	IntrusiveDList<T>::Iterator::Iterator() noexcept
		: m_pNode(nullptr)
	{
	}

	template <typename T>
	auto IntrusiveDList<T>::Iterator::operator=(const Iterator& other) noexcept
	{
		m_pNode = other.m_pNode;
		return *this;
	}

	template <typename T>
	auto IntrusiveDList<T>::Iterator::operator=(Iterator&& other) noexcept
	{
		m_pNode = other.m_pNode;
		return *this;
	}

	template <typename T>
	auto IntrusiveDList<T>::Iterator::operator++() noexcept -> Iterator
	{
		if (m_pNode)
			m_pNode = m_pNode->m_pNext;
		return *this;
	}

	template <typename T>
	auto IntrusiveDList<T>::Iterator::operator++(int) noexcept -> Iterator
	{
		Iterator it{ m_pNode };
		operator++();
		return it;
	}

	template <typename T>
	auto IntrusiveDList<T>::Iterator::operator--() noexcept -> Iterator
	{
		if (m_pNode)
			m_pNode = m_pNode->m_pPrev;
		return *this;
	}

	template <typename T>
	auto IntrusiveDList<T>::Iterator::operator--(int) noexcept -> Iterator
	{
		Iterator it{ m_pNode };
		operator--();
		return it;
	}

	template <typename T>
	auto IntrusiveDList<T>::Iterator::operator->() const noexcept -> T*
	{
		return &m_pNode->Get();
	}

	template <typename T>
	auto IntrusiveDList<T>::Iterator::operator*() const noexcept -> T&
	{
		return m_pNode->Get();
	}

	template <typename T>
	auto IntrusiveDList<T>::Iterator::operator+(usize count) const noexcept -> Iterator
	{
		Iterator it{ m_pNode };
		for (usize i = 0; i < count; ++i)
			++it;
		return it;
	}

	template <typename T>
	auto IntrusiveDList<T>::Iterator::operator-(usize count) const noexcept -> Iterator
	{
		Iterator it{ m_pNode };
		for (usize i = 0; i < count; ++i)
			--it;
		return it;
	}

	template <typename T>
	auto IntrusiveDList<T>::Iterator::operator+=(usize count) noexcept -> Iterator&
	{
		for (usize i = 0; i < count; ++i)
			operator++();
		return *this;
	}

	template <typename T>
	auto IntrusiveDList<T>::Iterator::operator-=(usize count) noexcept -> Iterator&
	{
		for (usize i = 0; i < count; ++i)
			operator--();
		return *this;
	}

	template <typename T>
	auto IntrusiveDList<T>::Iterator::operator==(const Iterator& other) const noexcept -> bool
	{
		return m_pNode == other.m_pNode;
	}

	template <typename T>
	auto IntrusiveDList<T>::Iterator::operator!=(const Iterator& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <typename T>
	IntrusiveDList<T>::Iterator::Iterator(IntrusiveDListNode<T>* pNode)
		: m_pNode(pNode)
	{
	}

	template <typename T>
	IntrusiveDList<T>::IntrusiveDList() noexcept
		: m_pHead(nullptr)
		, m_pTail(nullptr)
	{
	}
	
	template <typename T>
	IntrusiveDList<T>::IntrusiveDList(IntrusiveDList&& other) noexcept
		: m_pHead(other.m_pHead)
		, m_pTail(other.m_pTail)
	{
		other.m_pHead = other.m_pTail = nullptr;
	}
	
	template <typename T>
	auto IntrusiveDList<T>::operator=(IntrusiveDList&& other) noexcept -> IntrusiveDList<T>&
	{
		m_pHead = other.m_pHead;
		m_pTail = other.m_pTail;
		other.m_pHead = other.m_pTail = nullptr;
		return *this;
	}

	template <typename T>
	auto IntrusiveDList<T>::Add(IntrusiveDListNode<T>& val) noexcept -> void
	{
		val.m_pPrev = m_pTail;
		if (m_pTail)
			m_pTail->m_pNext = &val;
		else
			m_pHead = &val;
		m_pTail = &val;
	}

	template <typename T>
	auto IntrusiveDList<T>::Add(IntrusiveDList&& other) -> void
	{
		if (!other.m_pHead)
			return;

		if (!m_pHead)
		{
			m_pHead = other.m_pHead;
			m_pTail = other.m_pTail;
			return;
		}

		m_pTail->m_pNext = other.m_pHead;
		other.m_pHead->m_pPrev = m_pTail;
		m_pTail = other.m_pTail;
		other.m_pHead = other.m_pTail = nullptr;
	}

	template <typename T>
	auto IntrusiveDList<T>::Insert(Iterator& it, IntrusiveDListNode<T>& val) noexcept -> Iterator
	{
		ASSERT(it.m_pNode, "Iterator out of bounds");
		IntrusiveDListNode<T>* pNode = it.m_pNode;
		IntrusiveDListNode<T>* pPrev = pNode->m_pPrev;

		val.m_pNext = pNode;
		pNode->m_pNext = &val;
		&val.m_pPrev = pPrev;

		if (pPrev)
			pPrev->m_pNext = &val;
		else
			m_pHead = &val;

		return Iterator{ val };
	}

	template <typename T>
	auto IntrusiveDList<T>::Insert(Iterator& it, IntrusiveDList&& other) noexcept -> Iterator
	{
		ASSERT(it.m_pNode, "Iterator out of bounds");
		if (!other.m_pHead)
			return it;

		IntrusiveDListNode<T>* pNode = it.m_pNode;
		IntrusiveDListNode<T>* pPrev = pNode->m_pPrev;

		other.m_pTail->m_pNext = pNode;
		pNode->m_pPrev = other.m_pTail;
		other.m_pHead->m_pPrev = pPrev;

		if (pPrev)
			pPrev->m_pNext = &other.m_pHead;
		else
			m_pHead = &other.m_pHead;

		return Iterator{ other.m_pHead };
	}

	template <typename T>
	auto IntrusiveDList<T>::AddFront(IntrusiveDListNode<T>& val) noexcept -> void
	{
		val.m_pNext = m_pHead;

		if (m_pHead)
			m_pHead->m_pPrev = &val;

		m_pHead = &val;
	}

	template <typename T>
	auto IntrusiveDList<T>::AddFront(IntrusiveDList&& other) noexcept -> void
	{
		if (!other.m_pHead)
			return;

		if (!m_pHead)
		{
			m_pHead = other.m_pHead;
			m_pTail = other.m_pTail;
			return;
		}

		other.m_pTail->m_pNext = m_pHead;
		m_pHead->m_pPrev = other.m_pTail;
		m_pHead = other.m_pHead;
		other.m_pHead = other.m_pTail = nullptr;
	}

	template <typename T>
	auto IntrusiveDList<T>::Clear() noexcept -> void
	{
		IntrusiveDListNode<T>* pNode = m_pHead;
		while (pNode)
		{
			IntrusiveDListNode<T>* pNext = pNode->m_pNext;
			pNode->m_pPrev = pNode->m_pNext = nullptr;
			pNode = pNext;
		}

		m_pHead = m_pTail = nullptr;
	}

	template <typename T>
	auto IntrusiveDList<T>::Pop() noexcept -> void
	{
		if (!m_pHead->m_pNext)
		{
			m_pHead = m_pTail = nullptr;
			return;
		}

		IntrusiveDListNode<T>* pTail = m_pTail;
		m_pTail = pTail->m_pPrev;
		m_pTail->m_pNext = nullptr;
		pTail->m_pPrev = pTail->m_pNext = nullptr;
	}

	template <typename T>
	auto IntrusiveDList<T>::PopFront() noexcept -> void
	{
		if (!m_pHead->m_pNext)
		{
			m_pHead = m_pTail = nullptr;
			return;
		}

		IntrusiveDListNode<T>* pHead = m_pHead;
		m_pHead = pHead->m_pNext;
		m_pHead->m_pPrev = nullptr;
		m_pHead->m_pPrev = m_pHead->m_pNext = nullptr;
	}

	template <typename T>
	auto IntrusiveDList<T>::Erase(Iterator& it) noexcept -> void
	{
		ASSERT(it.m_pNode, "Iterator out of bounds");
		IntrusiveDListNode<T>* pPrev = it.m_pNode->m_pPrev;
		IntrusiveDListNode<T>* pNext = it.m_pNode->m_pNext;

		it.m_pNode->m_pPrev = it.m_pNode->m_pNext = nullptr;

		if (pPrev)
			pPrev->m_pNext = pNext;
		else
			m_pHead = pNext;

		if (pNext)
			pNext->m_pPrev = pPrev;
		else
			m_pTail = pPrev;
	}

	template <typename T>
	auto IntrusiveDList<T>::Reverse() noexcept -> void
	{
		IntrusiveDListNode<T>* pNode = m_pHead;

		while (pNode)
		{
			IntrusiveDListNode<T>* pNext = pNode->m_pNext;
			pNode->m_pNext = pNode->m_pPrev;
			pNode->m_pPrev = pNext;
			pNode = pNext;
		}

		m_pTail = m_pHead;
		m_pHead = pNode;
	}

	template <typename T>
	auto IntrusiveDList<T>::Size() const noexcept -> usize
	{
		usize count = 0;
		for (IntrusiveDListNode<T>* pNode = m_pHead; pNode; pNode = pNode->m_pNext)
			++count;
		return count;
	}

	template <typename T>
	auto IntrusiveDList<T>::IsEmpty() const noexcept -> bool
	{
		return !m_pHead;
	}

	template <typename T>
	auto IntrusiveDList<T>::Front() noexcept -> T&
	{
		ASSERT(m_pHead, "Invalid when IntrusiveList is empty");
		return m_pHead->Get();
	}

	template <typename T>
	auto IntrusiveDList<T>::Front() const noexcept -> const T&
	{
		ASSERT(m_pHead, "Invalid when IntrusiveList is empty");
		return m_pHead->Get();
	}

	template <typename T>
	auto IntrusiveDList<T>::Back() noexcept -> T&
	{
		ASSERT(m_pHead, "Invalid when IntrusiveList is empty");
		return m_pTail->Get();
	}

	template <typename T>
	auto IntrusiveDList<T>::Back() const noexcept -> const T&
	{
		ASSERT(m_pHead, "Invalid when IntrusiveList is empty");
		return m_pTail->Get();
	}

	template <typename T>
	auto IntrusiveDList<T>::Begin() noexcept -> Iterator
	{
		return Iterator{ m_pHead };
	}

	template <typename T>
	auto IntrusiveDList<T>::Begin() const noexcept -> ConstIterator
	{
		return Iterator{ m_pHead };
	}

	template <typename T>
	auto IntrusiveDList<T>::End() noexcept -> Iterator
	{
		return Iterator{};
	}

	template <typename T>
	auto IntrusiveDList<T>::End() const noexcept -> ConstIterator
	{
		return Iterator{};
	}

	template <typename T>
	auto IntrusiveDList<T>::begin() noexcept -> Iterator
	{
		return Begin();
	}

	template <typename T>
	auto IntrusiveDList<T>::begin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T>
	auto IntrusiveDList<T>::cbegin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T>
	auto IntrusiveDList<T>::end() noexcept -> Iterator
	{
		return End();
	}

	template <typename T>
	auto IntrusiveDList<T>::end() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename T>
	auto IntrusiveDList<T>::cend() const noexcept -> ConstIterator
	{
		return End();
	}
}
