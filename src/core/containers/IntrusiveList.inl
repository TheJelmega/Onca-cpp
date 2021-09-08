#pragma once
#include "IntrusiveList.h"

namespace Core
{
	template <typename Owner>
	IntrusiveListNode<Owner>::IntrusiveListNode(usize memberOffset) noexcept
		: m_pNext(nullptr)
		, m_offset(memberOffset)
	{
		// Offset sanity check
		ASSERT(memberOffset < sizeof(Owner), "Member offset is larger than the size of its Owner");
	}


	template <typename Owner>
	auto IntrusiveListNode<Owner>::operator=(const IntrusiveListNode& other) noexcept -> IntrusiveListNode&
	{
		m_pNext = other.m_pNext;
		m_offset = other.m_offset;
		return *this;
	}

	template <typename Owner>
	auto IntrusiveListNode<Owner>::operator=(IntrusiveListNode&& other) noexcept -> IntrusiveListNode&
	{
		m_pNext = other.m_pNext;
		m_offset = other.m_offset;
		return *this;
	}

	template <typename Owner>
	auto IntrusiveListNode<Owner>::Get() noexcept -> Owner*
	{
		return reinterpret_cast<Owner*>((u8*)this - m_offset);
	}

	template <typename Owner>
	auto IntrusiveListNode<Owner>::Get() const noexcept -> const Owner*
	{
		return reinterpret_cast<Owner*>((u8*)this - m_offset);
	}

	template <typename T>
	IntrusiveList<T>::Iterator::Iterator() noexcept
		: m_pNode(nullptr)
	{
	}

	template <typename T>
	auto IntrusiveList<T>::Iterator::operator=(const Iterator& other) noexcept
	{
		m_pNode = other.m_pNode;
		return *this;
	}

	template <typename T>
	auto IntrusiveList<T>::Iterator::operator=(Iterator&& other) noexcept
	{
		m_pNode = other.m_pNode;
		return *this;
	}

	template <typename T>
	auto IntrusiveList<T>::Iterator::operator++(int) noexcept -> Iterator
	{
		Iterator it{ m_pNode };
		operator++();
		return it;
	}

	template <typename T>
	auto IntrusiveList<T>::Iterator::operator->() const noexcept -> T*
	{
		return m_pNode->Get();
	}

	template <typename T>
	auto IntrusiveList<T>::Iterator::operator++() noexcept -> Iterator
	{
		if (m_pNode)
			m_pNode = m_pNode->m_pNext;
		return *this;
	}

	template <typename T>
	auto IntrusiveList<T>::Iterator::operator*() const noexcept -> T&
	{
		return *m_pNode->Get();
	}

	template <typename T>
	auto IntrusiveList<T>::Iterator::operator+(usize count) const noexcept -> Iterator
	{
		Iterator it{ m_pNode };
		for (usize i = 0; i < count; ++i)
			++it;
		return it;
	}

	template <typename T>
	auto IntrusiveList<T>::Iterator::operator+=(usize count) noexcept -> Iterator&
	{
		for (usize i = 0; i < count; ++i)
			operator++();
		return *this;
	}

	template <typename T>
	auto IntrusiveList<T>::Iterator::operator==(const Iterator& other) const noexcept -> bool
	{
		return m_pNode == other.m_pNode;
	}

	template <typename T>
	auto IntrusiveList<T>::Iterator::operator!=(const Iterator& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <typename T>
	IntrusiveList<T>::Iterator::Iterator(IntrusiveListNode<T>* pNode)
		: m_pNode(pNode)
	{
	}

	template <typename T>
	IntrusiveList<T>::IntrusiveList() noexcept
		: m_pHead(nullptr)
		, m_pTail(nullptr)
	{
	}
	
	template <typename T>
	IntrusiveList<T>::IntrusiveList(IntrusiveList&& other) noexcept
		: m_pHead(other.m_pHead)
		, m_pTail(other.m_pTail)
	{
		other.m_pHead == other.m_pTail;
	}
	
	template <typename T>
	auto IntrusiveList<T>::operator=(IntrusiveList&& other) noexcept -> IntrusiveList<T>&
	{
		m_pHead = other.m_pHead;
		m_pTail = other.m_pTail;
		other.m_pHead = other.m_pTail = nullptr;
		return *this;
	}

	template <typename T>
	auto IntrusiveList<T>::Add(IntrusiveListNode<T>& val) noexcept -> void
	{
		m_pTail->m_pNext = &val;
		m_pTail = &val;
	}

	template <typename T>
	auto IntrusiveList<T>::Add(IntrusiveList&& other) -> void
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
		m_pTail = other.m_pHead;
		other.m_pHead = other.m_pTail = nullptr;
	}

	template <typename T>
	auto IntrusiveList<T>::InsertAfter(Iterator& it, IntrusiveListNode<T>& val) noexcept -> Iterator
	{
		ASSERT(it, "Iterator out of bounds");
		val.m_pNext = it.m_pNode->m_pNext;
		it.m_pNode->m_pNext = val.m_pNext;

		if (!val.m_pNext)
			m_pTail = val;

		return Iterator{ val };
	}

	template <typename T>
	auto IntrusiveList<T>::InsertAfter(Iterator& it, IntrusiveList&& other) noexcept -> Iterator
	{
		ASSERT(it, "Iterator out of bounds");
		other.m_pTail->m_pNext = it->m_pNext;
		it->m_pNext = other.m_pHead;

		if (!other.m_pTail->m_pNext)
			m_pTail = other.m_pTail;

		Iterator retIt{ other.m_pHead };
		other.m_pHead = other.m_pTail = nullptr;
		return retIt;
	}

	template <typename T>
	auto IntrusiveList<T>::AddFront(IntrusiveListNode<T>& val) noexcept -> void
	{
		val.m_pNext = m_pHead;
		m_pHead = &val;
	}

	template <typename T>
	auto IntrusiveList<T>::AddFront(IntrusiveList&& other) noexcept -> void
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
		m_pTail = other.m_pHead;

		other.m_pHead = other.m_pTail = nullptr;
	}

	template <typename T>
	auto IntrusiveList<T>::Clear() noexcept -> void
	{
		IntrusiveListNode<T>* pNode = m_pHead;
		while (pNode)
		{
			IntrusiveListNode<T>* pNext = pNode->m_pNext;
			pNode->m_pNext = nullptr;
			pNode = pNext;
		}

		m_pHead = m_pTail = nullptr;
	}

	template <typename T>
	auto IntrusiveList<T>::Pop() noexcept -> void
	{
		if (!m_pHead)
			return;

		if (!m_pHead->m_pNext)
		{
			m_pHead = m_pTail = nullptr;
			return;
		}

		IntrusiveListNode<T>* pNewTail = m_pHead;
		while (pNewTail->m_pNext != m_pTail)
		{
			pNewTail = pNewTail->m_pNext;
		}
		pNewTail->m_pNext = nullptr;
		m_pTail = pNewTail;
	}

	template <typename T>
	auto IntrusiveList<T>::PopFront() noexcept -> void
	{
		if (!m_pHead)
			return;

		if (!m_pHead->m_pNext)
		{
			m_pHead = m_pTail = nullptr;
			return;
		}

		IntrusiveListNode<T>* pHead = m_pHead;
		m_pHead = pHead->m_pNext;
		pHead->m_pNext = nullptr;
	}

	template <typename T>
	auto IntrusiveList<T>::EraseAfter(Iterator& it) noexcept -> void
	{
		IntrusiveListNode<T>* pNext = it.m_pNode->m_pNext;
		it.m_pNode->m_pNext = pNext->m_pNext;
		pNext->m_pNext = nullptr;
	}

	template <typename T>
	auto IntrusiveList<T>::Reverse() noexcept -> void
	{
		IntrusiveListNode<T>* pNode = m_pHead;
		IntrusiveListNode<T>* pNext = pNode->m_pNext;

		while (pNext)
		{
			IntrusiveListNode<T>* pTmp = pNext->m_pNext;
			pNext->m_pNext = pNode;
			pNode = pNext;
			pNext = pTmp;
		}
		
		m_pTail = m_pHead;
		m_pHead = pNode;
	}

	template <typename T>
	auto IntrusiveList<T>::Size() const noexcept -> usize
	{
		usize count = 0;
		for (IntrusiveListNode<T>* pNode = m_pHead; pNode; pNode = pNode->m_pNext)
			++count;
		return count;
	}

	template <typename T>
	auto IntrusiveList<T>::IsEmpty() const noexcept -> bool
	{
		return !m_pHead;
	}

	template <typename T>
	auto IntrusiveList<T>::Front() noexcept -> T&
	{
		ASSERT(m_pHead, "Invalid when IntrusiveList is empty");
		return m_pHead->Get();
	}

	template <typename T>
	auto IntrusiveList<T>::Front() const noexcept -> const T&
	{
		ASSERT(m_pHead, "Invalid when IntrusiveList is empty");
		return m_pHead->Get();
	}

	template <typename T>
	auto IntrusiveList<T>::Back() noexcept -> T&
	{
		ASSERT(m_pHead, "Invalid when IntrusiveList is empty");
		return m_pTail->Get();
	}

	template <typename T>
	auto IntrusiveList<T>::Back() const noexcept -> const T&
	{
		ASSERT(m_pHead, "Invalid when IntrusiveList is empty");
		return m_pTail->Get();
	}

	template <typename T>
	auto IntrusiveList<T>::Begin() noexcept -> Iterator
	{
		return Iterator{ m_pHead };
	}

	template <typename T>
	auto IntrusiveList<T>::Begin() const noexcept -> ConstIterator
	{
		return Iterator{ m_pHead };
	}

	template <typename T>
	auto IntrusiveList<T>::End() noexcept -> Iterator
	{
		return Iterator{};
	}

	template <typename T>
	auto IntrusiveList<T>::End() const noexcept -> ConstIterator
	{
		return Iterator{};
	}

	template <typename T>
	auto IntrusiveList<T>::begin() noexcept -> Iterator
	{
		return Begin();
	}

	template <typename T>
	auto IntrusiveList<T>::begin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T>
	auto IntrusiveList<T>::cbegin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <typename T>
	auto IntrusiveList<T>::end() noexcept -> Iterator
	{
		return End();
	}

	template <typename T>
	auto IntrusiveList<T>::end() const noexcept -> ConstIterator
	{
		return End();
	}

	template <typename T>
	auto IntrusiveList<T>::cend() const noexcept -> ConstIterator
	{
		return End();
	}
}
