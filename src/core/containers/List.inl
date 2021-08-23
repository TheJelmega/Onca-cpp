#pragma once
#include "List.h"

namespace Core
{
	template <MoveConstructable T>
	List<T>::Iterator::Iterator() noexcept
		: m_node(nullptr)
	{
	}

	template <MoveConstructable T>
	List<T>::Iterator::Iterator(const NodeRef& node) noexcept
		: m_node(node)
	{
	}

	template <MoveConstructable T>
	auto List<T>::Iterator::operator=(const Iterator& other) noexcept
	{
		m_node = other.m_node;
		return *this;
	}

	template <MoveConstructable T>
	auto List<T>::Iterator::operator=(Iterator&& other) noexcept
	{
		m_node = StdMove(other.m_node);
		return *this;
	}

	template <MoveConstructable T>
	auto List<T>::Iterator::operator++() noexcept -> Iterator
	{
		if (m_node)
			m_node = m_node->next;
		return *this;
	}

	template <MoveConstructable T>
	auto List<T>::Iterator::operator++(int) noexcept -> Iterator
	{
		Iterator tmp{ *this };
		operator++();
		return tmp;
	}

	template <MoveConstructable T>
	auto List<T>::Iterator::operator->() const noexcept -> T*
	{
		return &m_node->val;
	}

	template <MoveConstructable T>
	auto List<T>::Iterator::operator*() const noexcept -> T&
	{
		return m_node->val;
	}

	template <MoveConstructable T>
	auto List<T>::Iterator::operator+(usize count) const noexcept -> Iterator
	{
		Iterator it = *this;
		for (usize i = 0; i < count; ++i)
			++it;
		return it;
	}

	template <MoveConstructable T>
	auto List<T>::Iterator::operator==(const Iterator& other) const noexcept -> bool
	{
		return m_node == other.m_node;
	}

	template <MoveConstructable T>
	auto List<T>::Iterator::operator!=(const Iterator& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <MoveConstructable T>
	List<T>::List(Alloc::IAllocator& alloc) noexcept
		: m_head(&alloc)
		, m_tail(nullptr)
	{
	}

	template <MoveConstructable T>
	List<T>::List(usize count, Alloc::IAllocator& alloc) noexcept
		: m_head(&alloc)
		, m_tail(nullptr)
	{
		FillDefault(count);
	}

	template <MoveConstructable T>
	List<T>::List(usize count, const T& val, Alloc::IAllocator& alloc) noexcept
		: m_head(&alloc)
		, m_tail(nullptr)
	{
		Fill(count, val);
	}

	template <MoveConstructable T>
	List<T>::List(const InitializerList<T>& il, Alloc::IAllocator& alloc) noexcept
		: m_head(&alloc)
		, m_tail(nullptr)
	{
		Assign(il);
	}

	template <MoveConstructable T>
	template <ForwardIterator It>
	List<T>::List(const It& begin, const It& end, Alloc::IAllocator& alloc) noexcept
		: m_head(&alloc)
		, m_tail(nullptr)
	{
		Assign(begin, end);
	}

	template <MoveConstructable T>
	List<T>::List(const List<T>& other) noexcept
		: m_head(other.GetAllocator())
		, m_tail(nullptr)
	{
		Assign(other.Begin(), other.End());
	}

	template <MoveConstructable T>
	List<T>::List(const List<T>& other, Alloc::IAllocator& alloc) noexcept
		: m_head(&alloc)
		, m_tail(nullptr)
	{
		Assign(other.Begin(), other.End());
	}

	template <MoveConstructable T>
	List<T>::List(List<T>&& other) noexcept
		: m_head(StdMove(other.m_head))
		, m_tail(StdMove(other.m_tail))
	{
	}

	template <MoveConstructable T>
	List<T>::~List() noexcept
	{
		Clear();
	}

	template <MoveConstructable T>
	auto List<T>::operator=(const InitializerList<T>& il) noexcept -> List<T>&
	{
		Clear();
		Assign(il);
		return *this;
	}

	template <MoveConstructable T>
	auto List<T>::operator=(const List<T>& other) noexcept -> List<T>&
	{
		Clear();
		Assign(other.Begin(), other.End());
		return *this;
	}

	template <MoveConstructable T>
	auto List<T>::operator=(List<T>&& other) noexcept -> List<T>&
	{
		Clear();
		m_head = StdMove(other.m_head);
		m_tail = StdMove(other.m_tail);
		MemClearData(other);
		return *this;
	}

	template <MoveConstructable T>
	template <ForwardIterator It>
	auto List<T>::Assign(const It& begin, const It& end) noexcept -> void
	{
		Clear();
		for (It it = begin; it != end; ++it)
			Add(*it);
	}

	template <MoveConstructable T>
	auto List<T>::Assign(const InitializerList<T>& il) noexcept -> void
	{
		Clear();
		for (const T* it = il.begin(); it != il.end(); ++it)
			Add(*it);
	}

	template <MoveConstructable T>
	auto List<T>::Fill(usize count, const T& val) noexcept -> void
	{
		Clear();
		for (usize i = 0; i < count; ++i)
			Add(val);
	}

	template <MoveConstructable T>
	auto List<T>::FillDefault(usize count) noexcept -> void
	{
		STATIC_ASSERT(IsNothrowDefaultConstructable<T>, "T needs to be nothrow default constructable");
		Clear();
		for (usize i = 0; i < count; ++i)
			EmplaceBack();
	}

	template <MoveConstructable T>
	auto List<T>::Resize(usize newSize, const T& val) noexcept -> void
	{
		STATIC_ASSERT(CopyConstructable<T>, "T needs to be copy constructable");

		Iterator it{ m_head };
		Iterator end;
		Iterator last;
		usize i = 0;

		for (; i < newSize && it != end; ++i, ++it)
			last = it;

		// too large
		if (it != end)
		{
			m_tail = last.m_node;
			m_tail->next = {};
			NodeRef curNode = it.m_node;
			do
			{
				NodeRef next = curNode->next;
				curNode.Dealloc();
				curNode = StdMove(end);
			} while (curNode);
		}
		// too small
		else
		{
			for (; i < newSize; ++i)
				Add(val);
		}

	}

	template <MoveConstructable T>
	auto List<T>::Resize(usize newSize) noexcept -> void
	{
		STATIC_ASSERT(NoThrowDefaultConstructable<T>, "T needs to be copy constructable");

		Iterator it{ m_head };
		Iterator end;
		Iterator last;
		usize i = 0;

		for (; i < newSize && it != end; ++i, ++it)
			last = it;

		// too large
		if (it != end)
		{
			m_tail = last.m_node;
			m_tail->next = NodeRef{ nullptr };
			NodeRef curNode = it.m_node;
			do
			{
				NodeRef next = curNode->next;
				curNode.Dealloc();
				curNode = next;
			} while (curNode);
		}
		// too small
		else if (i < newSize)
		{
			for (; i < newSize; ++i)
				Add(StdMove(T{}));
		}
	}

	template <MoveConstructable T>
	auto List<T>::Add(const T& val) noexcept -> void
	{
		STATIC_ASSERT(CopyConstructable<T>, "T needs to be copy constructable");
		Add(StdMove(T{ val }));
	}

	template <MoveConstructable T>
	auto List<T>::Add(T&& val) noexcept -> void
	{
		NodeRef node = CreateNode(StdMove(val));

		if (m_tail)
		{
			m_tail->next = node;
			m_tail = StdMove(node);
		}
		else
		{
			m_tail = m_head = StdMove(node);
		}
	}

	template <MoveConstructable T>
	auto List<T>::Add(const List<T>& other) -> void
	{
		for (Iterator it = other.Begin(), end = other.End(); it != end; ++it)
			Add(StdMove(T{ it.m_node->val }));
	}

	template <MoveConstructable T>
	auto List<T>::Add(List<T>&& other) -> void
	{
		NodeRef node = other.m_head;
		while (node)
		{
			Add(StdMove(node->val));
			NodeRef next = node->next;
			node.Dealloc();
			node = next;
		}
		other.m_head = other.m_tail = NodeRef{ nullptr };
	}

	template <MoveConstructable T>
	template <typename ... Args>
	auto List<T>::EmplaceBack(Args&&... args) noexcept -> void
	{
		Add(StdMove(T{ args... }));
	}

	template <MoveConstructable T>
	auto List<T>::InsertAfter(ConstIterator& it, const T& val) noexcept -> Iterator
	{
		STATIC_ASSERT(CopyConstructable<T>, "T needs to be copy constructable");
		return InsertAfter(it, T{ val });
	}

	template <MoveConstructable T>
	auto List<T>::InsertAfter(ConstIterator& it, T&& val) noexcept -> Iterator
	{
		ASSERT(it.m_node, "Iterator out of bounds");

		NodeRef curNode = it.m_node;

		NodeRef node = CreateNode(StdMove(val));

		node->next = curNode->next;
		curNode->next = node;

		if (!node->next)
			m_tail = node;

		return Iterator{ node };
	}

	template <MoveConstructable T>
	auto List<T>::InsertAfter(ConstIterator& it, usize count, const T& val) noexcept -> Iterator
	{
		STATIC_ASSERT(CopyConstructable<T>, "T needs to be copy constructable");
		Iterator curIt = it;
		for (usize i = 0; i < count; ++i)
			curIt = InsertAfter(curIt, StdMove(T{ val }));
		return it + 1;
	}

	template <MoveConstructable T>
	template <ForwardIterator It>
	auto List<T>::InsertAfter(ConstIterator& it, const It& begin, const It& end) noexcept -> Iterator
	{
		STATIC_ASSERT(CopyConstructable<T>, "T needs to be copy constructable");
		Iterator curIt = it;
		for (It valIt = begin; valIt != end; ++valIt)
			curIt = InsertAfter(curIt, StdMove(T{ *valIt }));
		return it + 1;
	}

	template <MoveConstructable T>
	auto List<T>::InsertAfter(ConstIterator& it, const InitializerList<T>& il) noexcept -> Iterator
	{
		STATIC_ASSERT(CopyConstructable<T>, "T needs to be copy constructable");
		Iterator curIt = it;
		for (const T* valIt = il.begin(), *end = il.end(); valIt != end; ++valIt)
			curIt = InsertAfter(curIt, StdMove(T{ *valIt }));
		return it + 1;
	}

	template <MoveConstructable T>
	auto List<T>::InsertAfter(ConstIterator& it, const List<T>& other) noexcept -> Iterator
	{
		if (other.IsEmpty())
			return it;

		STATIC_ASSERT(CopyConstructable<T>, "T needs to be copy constructable");
		Iterator curIt = it;
		for (Iterator valIt = other.Begin(), end = other.End(); valIt != end; ++valIt)
		{

			curIt = InsertAfter(curIt, StdMove(T{ *valIt }));
		}
		return it + 1;
	}

	template <MoveConstructable T>
	auto List<T>::InsertAfter(ConstIterator& it, List<T>&& other) noexcept -> Iterator
	{
		if (other.IsEmpty())
			return it;

		STATIC_ASSERT(CopyConstructable<T>, "T needs to be copy constructable");
		Iterator curIt = it;

		NodeRef node = other.m_head;
		while (node)
		{
			curIt = InsertAfter(curIt, StdMove(node->val));
			NodeRef next = node->next;
			node.Dealloc();
			node = next;
		}
		other.m_head = other.m_tail = NodeRef{ nullptr };
		return it + 1;
	}

	template <MoveConstructable T>
	template <typename ... Args>
	auto List<T>::EmplaceAfter(ConstIterator& it, Args&&... args) noexcept -> Iterator
	{
		return InsertAfter(it, StdMove(T{ args... }));
	}

	template <MoveConstructable T>
	auto List<T>::AddFront(const T& val) noexcept -> void
	{
		AddFront(StdMove(T{ val }));
	}

	template <MoveConstructable T>
	auto List<T>::AddFront(T&& val) noexcept -> void
	{
		NodeRef node = CreateNode(StdMove(val));
		node->next = m_head;
		m_head = node;
	}

	template <MoveConstructable T>
	auto List<T>::AddFront(usize count, const T& val) noexcept -> void
	{
		for (usize i = 0; i < count; ++i)
			AddFront(StdMove(T{ val }));
	}

	template <MoveConstructable T>
	template <ForwardIterator It>
	auto List<T>::AddFront(const It& begin, const It& end) noexcept -> void
	{
		for (It it = begin; it < end; ++it)
			AddFront(StdMove(T{ *it }));
	}

	template <MoveConstructable T>
	auto List<T>::AddFront(const InitializerList<T>& il) noexcept -> void
	{
		for (T* it = il.begin(), end = il.end(); it < end; ++it)
			AddFront(StdMove(T{ *it }));
	}

	template <MoveConstructable T>
	auto List<T>::AddFront(const List<T>& other) noexcept -> void
	{
		if (other.IsEmpty())
			return;

		AddFront(StdMove(T{ *other.Begin() }));
		Iterator curIt{ m_head };
		for (Iterator it = other.Begin() + 1, end = other.End(); it != end; ++it)
			curIt = InsertAfter(curIt, StdMove(T{ *it }));
	}

	template <MoveConstructable T>
	auto List<T>::AddFront(List<T>&& other) noexcept -> void
	{
		if (other.IsEmpty())
			return;

		NodeRef node = other.m_head;
		AddFront(StdMove(node->val));
		NodeRef next = node->next;
		node.Dealloc();
		node = next;

		Iterator curIt{ m_head };
		while (node)
		{
			curIt = InsertAfter(curIt, StdMove(node->val));
			next = node->next;
			node.Dealloc();
			node = next;
		}
		other.m_tail = other.m_head = NodeRef{ nullptr };
	}

	template <MoveConstructable T>
	template <typename ... Args>
	auto List<T>::EmplaceFront(Args&&... args) noexcept -> void
	{
		AddFront(StdMove(T{ args... }));
	}

	template <MoveConstructable T>
	auto List<T>::Clear() noexcept -> void
	{
		if (!m_head)
			return;

		NodeRef node = m_head;
		Alloc::IAllocator* pAlloc = m_head.Alloc();
		while (node)
		{
			node->val.~T();
			NodeRef next = node->next;
			node.Dealloc();
			node = next;
		}
		MemClearData(*this);
		m_head = NodeRef{ pAlloc };
	}

	template <MoveConstructable T>
	auto List<T>::Pop() noexcept -> void
	{
		NodeRef node = m_head;
		NodeRef next = node->next;
		if (!next)
		{
			m_head->val.~T();
			Alloc::IAllocator* pAlloc = m_head.Alloc();
			m_head.Dealloc();
			MemClearData(m_tail);
			m_head = NodeRef{ pAlloc };
		}
		else 
		{
			while (next->next)
			{
				node = next;
				next = next->next;
			}

			next->val.~T();
			next.Dealloc();
			node->next = NodeRef{ nullptr };
			m_tail = node;
		}
	}

	template <MoveConstructable T>
	auto List<T>::PopFront() noexcept -> void
	{
		NodeRef newHead = m_head->next;
		if (newHead)
		{
			m_head->val.~T();
			m_head.Dealloc();
			m_head = newHead;
		}
		else
		{
			Alloc::IAllocator* pAlloc = m_head.Alloc();
			m_head->val.~T();
			m_head.Dealloc();
			m_head = NodeRef{ pAlloc };
			m_tail = NodeRef{ nullptr };
		}
	}

	template <MoveConstructable T>
	auto List<T>::EraseAfter(const Iterator& it) noexcept -> void
	{
		EraseAfter(it, 1);
	}

	template <MoveConstructable T>
	auto List<T>::EraseAfter(const Iterator& it, usize count) noexcept -> void
	{
		NodeRef begin = it.m_node;
		NodeRef next = begin->next;

		if (!next)
			return;

		usize i = 0;
		for (; i < count && next; ++i)
		{
			NodeRef tmp = next->next;
			next->val.~T();
			next.Dealloc();
			next = tmp;
		}
		ASSERT(i == count, "Count too large");

		begin->next = next;
		if (!next)
			m_tail = begin;
	}

	template <MoveConstructable T>
	auto List<T>::EraseAfter(const Iterator& begin, const Iterator& end) noexcept -> void
	{
		NodeRef beginNode = begin.m_node;
		NodeRef next = begin.m_node->next;

		if (!next || next == end.m_node)
			return;

		do
		{
			NodeRef tmp = next->next;
			next->val.~T();
			next.Dealloc();
			next = tmp;
		}
		while (next != end.m_node);

		beginNode->next = next;
		if (!next)
			m_tail = beginNode;
	}

	template <MoveConstructable T>
	auto List<T>::Reverse() noexcept -> void
	{
		NodeRef node = m_head;
		if (!node)
			return;

		NodeRef next = node->next;
		while (next)
		{
			NodeRef tmp = next->next;
			next->next = node;
			node = next;
			next = tmp;
		}

		m_head->next = NodeRef{ nullptr };
		NodeRef head = StdMove(m_head);
		m_head = StdMove(m_tail);
		m_tail = StdMove(head);
	}


	template <MoveConstructable T>
	auto List<T>::Size() const noexcept -> usize
	{
		usize size = 0;
		for (Iterator it{ m_head }, end{}; it != end; ++it)
			++size;
		return size;
	}

	template <MoveConstructable T>
	auto List<T>::IsEmpty() const noexcept -> bool
	{
		return !m_head;
	}

	template <MoveConstructable T>
	auto List<T>::GetAllocator() const noexcept -> Alloc::IAllocator*
	{
		return m_head.Alloc();
	}

	template <MoveConstructable T>
	auto List<T>::Front() noexcept -> T&
	{
		ASSERT(m_head, "Invalid when List is empty");
		return m_head->val;
	}

	template <MoveConstructable T>
	auto List<T>::Front() const noexcept -> const T&
	{
		ASSERT(m_head, "Invalid when List is empty");
		return m_head->val;
	}

	template <MoveConstructable T>
	auto List<T>::Last() noexcept -> T&
	{
		ASSERT(m_head, "Invalid when List is empty");
		return m_tail->val;
	}

	template <MoveConstructable T>
	auto List<T>::Last() const noexcept -> const T&
	{
		ASSERT(m_head, "Invalid when List is empty");
		return m_tail->val;
	}

	template <MoveConstructable T>
	auto List<T>::Begin() noexcept -> Iterator
	{
		return Iterator{ m_head };
	}

	template <MoveConstructable T>
	auto List<T>::Begin() const noexcept -> ConstIterator
	{
		return Iterator{ m_head };
	}

	template <MoveConstructable T>
	auto List<T>::End() noexcept -> Iterator
	{
		return Iterator{};
	}

	template <MoveConstructable T>
	auto List<T>::End() const noexcept -> ConstIterator
	{
		return Iterator{};
	}

	template <MoveConstructable T>
	auto List<T>::begin() noexcept -> Iterator
	{
		return Begin();
	}

	template <MoveConstructable T>
	auto List<T>::begin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <MoveConstructable T>
	auto List<T>::cbegin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template <MoveConstructable T>
	auto List<T>::end() noexcept -> Iterator
	{
		return End();
	}

	template <MoveConstructable T>
	auto List<T>::end() const noexcept -> ConstIterator
	{
		return End();
	}

	template <MoveConstructable T>
	auto List<T>::cend() const noexcept -> ConstIterator
	{
		return End();
	}

	template <MoveConstructable T>
	auto List<T>::CreateNode(T&& val) noexcept -> NodeRef
	{
		NodeRef node = m_head.Alloc()->template Allocate<Node>();
		Node* pNode = node.Ptr();
		new (&pNode->next) NodeRef{ nullptr };
		new (&pNode->val) T{ StdMove(val) };
		return node;
	}
}
