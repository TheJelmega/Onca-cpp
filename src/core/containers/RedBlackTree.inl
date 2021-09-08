#pragma once
#include "RedBlackTree.h"

namespace Core
{
	namespace Detail
	{
		template <Movable T, Comparator<T> C, bool AllowMultiple>
		RedBlackTreeIterator<T, C, AllowMultiple>::RedBlackTreeIterator() noexcept
			: m_node()
		{
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		RedBlackTreeIterator<T, C, AllowMultiple>::RedBlackTreeIterator(const RedBlackTreeIterator& other) noexcept
			: m_node(other.m_node)
		{
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		RedBlackTreeIterator<T, C, AllowMultiple>::RedBlackTreeIterator(RedBlackTreeIterator&& other) noexcept
			: m_node(Move(other.m_node))
		{
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		auto RedBlackTreeIterator<T, C, AllowMultiple>::operator=(const RedBlackTreeIterator& other) noexcept
		{
			m_node = other.m_node;
			return *this;
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		auto RedBlackTreeIterator<T, C, AllowMultiple>::operator=(RedBlackTreeIterator&& other) noexcept
		{
			m_node = Move(other.m_node);
			return *this;
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		auto RedBlackTreeIterator<T, C, AllowMultiple>::operator*() const noexcept -> T&
		{
			if constexpr (AllowMultiple)
				return m_node->value[m_Idx];
			else
				return m_node->value;
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		auto RedBlackTreeIterator<T, C, AllowMultiple>::operator->() const noexcept -> T*
		{
			if constexpr (AllowMultiple)
				return &m_node->value[m_Idx];
			else
				return &m_node->value;
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		auto RedBlackTreeIterator<T, C, AllowMultiple>::operator++() noexcept -> RedBlackTreeIterator&
		{
			if constexpr (AllowMultiple)
			{
				++m_Idx;
				if (m_Idx < m_node->value.Size())
					return *this;
				m_Idx = 0;
			}

			NodeRef parent = m_node->parent;

			if (m_node->right)
			{
				m_node = m_node->right;
				while (m_node->left)
					m_node = m_node->left;

				return *this;
			}
			if (!parent)
			{
				m_node = NodeRef{};
				return *this;
			}
			if (m_node == parent->left)
			{
				m_node = parent;
				return *this;
			}

			while (parent->parent && m_node == parent->right)
			{
				m_node = parent;
				parent = m_node->parent;
			}

			if (m_node == parent->left)
			{
				m_node = parent;
				return *this;
			}
			if (!parent->parent && m_node == parent->right)
			{
				m_node = NodeRef{};
				return *this;
			}

			return *this;
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		auto RedBlackTreeIterator<T, C, AllowMultiple>::operator++(int) noexcept -> RedBlackTreeIterator
		{
			RedBlackTreeIterator it{ m_node };
			operator++();
			return it;
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		auto RedBlackTreeIterator<T, C, AllowMultiple>::operator--() noexcept -> RedBlackTreeIterator&
		{
			if constexpr (AllowMultiple)
			{
				if (m_Idx > 0)
				{
					--m_Idx;
					return *this;
				}
			}

			NodeRef parent = m_node->parent;

			if (m_node->left)
			{
				m_node = m_node->left;
				while (m_node->right)
					m_node = m_node->right;
			}
			else if (!parent)
			{
				m_node = NodeRef{};
			}
			else if (m_node == parent->right)
			{
				m_node = parent;
			}
			else
			{
				while (parent->parent && m_node == parent->left)
				{
					m_node = parent;
					parent = m_node->parent;
				}

				if (!parent->parent && m_node->left == parent)
					m_node = NodeRef{};
			}

			m_Idx = m_node->value.Size() - 1;
			return *this;
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		auto RedBlackTreeIterator<T, C, AllowMultiple>::operator--(int) noexcept -> RedBlackTreeIterator
		{
			RedBlackTreeIterator it{ m_node };
			operator--();
			return it;
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		auto RedBlackTreeIterator<T, C, AllowMultiple>::operator+(usize count) const noexcept -> RedBlackTreeIterator
		{
			RedBlackTreeIterator it{ m_node };
			for (usize i = 0; i < count; ++i)
				++it;
			return it;
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		auto RedBlackTreeIterator<T, C, AllowMultiple>::operator-(usize count) const noexcept -> RedBlackTreeIterator
		{
			RedBlackTreeIterator it{ m_node };
			for (usize i = 0; i < count; ++i)
				--it;
			return it;
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		auto RedBlackTreeIterator<T, C, AllowMultiple>::operator+=(usize count) noexcept -> RedBlackTreeIterator&
		{
			for (usize i = 0; i < count; ++i)
				operator++();
			return *this;
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		auto RedBlackTreeIterator<T, C, AllowMultiple>::operator-=(usize count) noexcept -> RedBlackTreeIterator&
		{
			for (usize i = 0; i < count; ++i)
				operator--();
			return *this;
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		auto RedBlackTreeIterator<T, C, AllowMultiple>::operator==(const RedBlackTreeIterator& other) const noexcept -> bool
		{
			return m_node == other.m_node;
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		auto RedBlackTreeIterator<T, C, AllowMultiple>::operator!=(const RedBlackTreeIterator& other) const noexcept -> bool
		{
			return !(*this == other);
		}

		template <Movable T, Comparator<T> C, bool AllowMultiple>
		RedBlackTreeIterator<T, C, AllowMultiple>::RedBlackTreeIterator(const NodeRef& node, usize idx)
			: m_node(node)
			, m_Idx(idx)
		{
		}
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	RedBlackTree<T, C, AllowMultiple>::RedBlackTree(Alloc::IAllocator& alloc) noexcept
		: m_root(&alloc)
		, m_size(0)
	{
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	RedBlackTree<T, C, AllowMultiple>::RedBlackTree(C comp, Alloc::IAllocator& alloc) noexcept
		: m_root(&alloc)
		, m_size(0)
		, m_comp(Move(comp))
	{
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	RedBlackTree<T, C, AllowMultiple>::RedBlackTree(const InitializerList<T>& il, Alloc::IAllocator& alloc) noexcept requires CopyConstructable<T>
		: m_root(&alloc)
		, m_size(0)
	{
		Assign(il.begin(), il.end());
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	RedBlackTree<T, C, AllowMultiple>::RedBlackTree(const InitializerList<T>& il, C comp, Alloc::IAllocator& alloc) noexcept requires CopyConstructable<T>
		: m_root(&alloc)
		, m_size(0)
		, m_comp(Move(comp))
	{
		Assign(il.begin(), il.end());
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	template <ForwardIterator It>
	RedBlackTree<T, C, AllowMultiple>::RedBlackTree(const It& begin, const It& end, Alloc::IAllocator& alloc) noexcept requires CopyConstructable<T>
		: m_root(&alloc)
		, m_size(0)
	{
		Assign(begin, end);
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	template <ForwardIterator It>
	RedBlackTree<T, C, AllowMultiple>::RedBlackTree(const It& begin, const It& end, C comp, Alloc::IAllocator& alloc) noexcept requires CopyConstructable<T>
		: m_root(&alloc)
		, m_size(0)
		, m_comp(Move(comp))
	{
		Assign(begin, end);
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	RedBlackTree<T, C, AllowMultiple>::RedBlackTree(const RedBlackTree& other) noexcept
		: m_root(other.GetAllocator())
		, m_size(0)
	{
		Copy<false>(other);
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	RedBlackTree<T, C, AllowMultiple>::RedBlackTree(const RedBlackTree& other, Alloc::IAllocator& alloc) noexcept
		: m_root(other.GetAllocator())
		, m_size(0)
	{
		Copy<false>(other);
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	RedBlackTree<T, C, AllowMultiple>::RedBlackTree(RedBlackTree&& other) noexcept
		: m_root(Move(other.m_root))
		, m_size(other.m_size)
	{
		other.m_root = NodeRef{ m_root.GetAlloc() };
		other.m_size = 0;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	RedBlackTree<T, C, AllowMultiple>::RedBlackTree(RedBlackTree&& other, Alloc::IAllocator& alloc) noexcept
		: m_root(&alloc)
		, m_size(0)
	{
		Copy<true>(other);
		other.m_size = 0;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::operator=(const RedBlackTree& other) noexcept -> RedBlackTree&
	{
		Copy<false>(other);
		return *this;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::operator=(RedBlackTree&& other) noexcept -> RedBlackTree&
	{
		m_root = Move(other.m_root);
		m_size = other.m_size;
		other.m_size = 0;
		return *this;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	template<ForwardIterator It>
	auto RedBlackTree<T, C, AllowMultiple>::Assign(const It& begin, const It& end) noexcept -> void requires CopyConstructable<T>
	{
		Clear();
		for (It it = begin; it != end; ++it)
			Insert(T{ *it });
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Assign(const InitializerList<T>& il) noexcept -> void requires CopyConstructable<T>
	{
		Assign(il.begin(), il.end());
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Insert(const T& value) noexcept -> Pair<Iterator, bool> requires CopyConstructable<T>
	{
		return Insert(T{ value });
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Insert(T&& value) noexcept -> Pair<Iterator, bool>
	{
		if (!m_root)
		{
			m_root = CreateNode(Move(value));
			++m_size;
			return { Iterator{ m_root }, true  };
		}

		// Find the predecessor
		NodeRef node = m_root;
		while (node)
		{
			i8 res = Compare(value, node);
			if (res == 0)
			{
				if constexpr (AllowMultiple)
				{
					usize idx = node->value.Size();
					node->value.Add(Move(value));
					return { Iterator{ node, idx }, false };
				}
				else
				{
					return { Iterator{ node }, false };
				}
			}

			if (res < 0)
			{
				NodeRef left = node->left;
				if (!left)
					break;
				node = left;
			}
			// res > 0
			else
			{
				NodeRef right = node->right;
				if (!right)
					break;
				node = right;
			}
		}

		i8 res = Compare(value, node);
		RotateDir dir;
		if (res < 0)
		{
			ASSERT(!node->left, "Invalid RedBackTree predecessor");
			NodeRef newNode = CreateNode(Move(value));
			newNode->parent = node;
			node->left = newNode;
			node = newNode;
			dir = RotateDir::Left;
		}
		else
		{
			ASSERT(!node->right, "Invalid RedBackTree predecessor");
			NodeRef newNode = CreateNode(Move(value));
			newNode->parent = node;
			node->right = newNode;
			node = newNode;
			dir = RotateDir::Right;
		}

		RebalanceInsert(node, dir);
		++m_size;

		return { Iterator{ node }, true };
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	template <Comparator C2>
	auto RedBlackTree<T, C, AllowMultiple>::Merge(RedBlackTree<T, C2>& other) noexcept -> void
	{
		if constexpr (AllowMultiple)
		{
			for (auto otherIt = other.Begin(); otherIt.m_node; ++otherIt)
			{
				Insert(*otherIt);
			}
			other.template ClearInternal<false>();
		}
		else
		{
			for (auto otherIt = other.Begin(); otherIt.m_node; ++otherIt)
			{
				if (!Contains(otherIt->value))
				{
					Insert(Move(otherIt->value));
					other.template EraseInternal<false>(otherIt);
				}
			}
		}
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Clear() noexcept -> void
	{
		ClearInternal<true>();
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Erase(ConstIterator& it) noexcept -> Iterator
	{
		return EraseInternal<true>(it);
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Erase(const T& value) noexcept -> Iterator
	{
		return EraseInternal<true>(Find(value));
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Find(const T& key) noexcept -> Iterator
	{
		NodeRef node = m_root;
		while (node)
		{
			i8 res = Compare(key, node);
			if (res == 0)
				return Iterator{ node };

			if (res < 0)
				node = node->left;
			else
				node = node->right;
		}
		return Iterator{};
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Find(const T& key) const noexcept -> ConstIterator
	{
		NodeRef node = m_root;
		while (node)
		{
			i8 res = Compare(key, node);
			if (res == 0)
				return Iterator{ node };

			if (res < 0)
				node = node->left;
			else
				node = node->right;
		}
		return Iterator{};
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	template <OrderedComparable<T> T2>
	auto RedBlackTree<T, C, AllowMultiple>::Find(const T2& key) noexcept -> Iterator
	{
		NodeRef node = m_root;
		while (node)
		{
			if constexpr (AllowMultiple)
			{
				if (key < node->value[0])
					node = node->left;
				else if (key > node->value[0])
					node = node->right;
				else
					return Iterator{ node };
			}
			else
			{
				if (key < node->value)
					node = node->left;
				else if (key > node->value)
					node = node->right;
				else
					return Iterator{ node };
			}
		}
		return Iterator{};
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	template <OrderedComparable<T> T2>
	auto RedBlackTree<T, C, AllowMultiple>::Find(const T2& key) const noexcept -> ConstIterator
	{
		NodeRef node = m_root;
		while (node)
		{
			if constexpr (AllowMultiple)
			{
				if (key < node->value[0])
					node = node->left;
				else if (key > node->value[0])
					node = node->right;
				else
					return Iterator{ node };
			}
			else
			{
				if (key < node->value)
					node = node->left;
				else if (key > node->value)
					node = node->right;
				else
					return Iterator{ node };
			}
		}
		return Iterator{};
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Contains(const T& key) const noexcept -> bool
	{
		return !!Find(key).m_node;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	template <OrderedComparable<T> T2>
	auto RedBlackTree<T, C, AllowMultiple>::Contains(const T2& key) const noexcept -> bool
	{
		return !!Find(key).m_node;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Size() const noexcept -> usize
	{
		return m_size;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::IsEmpty() const noexcept -> bool
	{
		return m_size == 0;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::GetAllocator() const noexcept -> Alloc::IAllocator*
	{
		return m_root.GetAlloc();
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Front() noexcept -> T&
	{
		ASSERT(m_size, "Invalid when RedBlackTree is empty");
		if constexpr (AllowMultiple)
			return GetFirstNode()->value.Front();
		else 
			return GetFirstNode()->value;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Front() const noexcept -> const T&
	{
		ASSERT(m_size, "Invalid when RedBlackTree is empty");
		if constexpr (AllowMultiple)
			return GetFirstNode()->value.Front();
		else
			return GetFirstNode()->value;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Back() noexcept -> T&
	{
		ASSERT(m_size, "Invalid when RedBlackTree is empty");
		if constexpr (AllowMultiple)
			return GetLastNode()->value.Back();
		else
			return GetLastNode()->value;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Back() const noexcept -> const T&
	{
		ASSERT(m_size, "Invalid when RedBlackTree is empty");
		if constexpr (AllowMultiple)
			return GetLastNode()->value.Back();
		else
			return GetLastNode()->value;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Begin() noexcept -> Iterator
	{
		return Iterator{ GetFirstNode() };
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Begin() const noexcept -> ConstIterator
	{
		return Iterator{ GetFirstNode() };
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::End() noexcept -> Iterator
	{
		return Iterator{};
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::End() const noexcept -> ConstIterator
	{
		return Iterator{};
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::CreateNode(T&& val) noexcept -> NodeRef
	{
		Alloc::IAllocator* pAlloc = m_root.GetAlloc();
		NodeRef node = pAlloc->Allocate<Node>();

		if constexpr (AllowMultiple)
			node->value.Add(Move(val));
		else
			node->value = Move(val);
		node->parent = node->left = node->right = NodeRef{};
		node->color = Detail::RedBlackTreeColor::Red;
		return node;
	}
	
	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Rotate(NodeRef& parent, RotateDir dir) noexcept -> NodeRef
	{
		if (!parent)
			return NodeRef{};

		NodeRef grandParent = parent->parent;
		NodeRef sibling = parent->children[1 - u8(dir)];

		if (!sibling)
			return NodeRef{};

		NodeRef child = sibling->children[u8(dir)];

		parent->children[1 - u8(dir)] = child;
		if (child)
			child->parent = parent;

		sibling->children[u8(dir)] = parent;
		parent->parent = sibling;
		  
		sibling->parent = grandParent;

		if (grandParent)
			grandParent->children[u8(parent == grandParent->right)] = sibling;
		else
			m_root = sibling;
		return sibling;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::RebalanceInsert(NodeRef node, RotateDir dir) noexcept -> void
	{
		do
		{
			NodeRef parent = node->parent;

			// Case 1 and 3: parent is black or no parent
			if (!parent || parent->color == Detail::RedBlackTreeColor::Black)
				return;

			NodeRef grandparent = parent->parent;
			if (!grandparent)
			{
				// Case 4: parent is root and red
				parent->color = Detail::RedBlackTreeColor::Black;
				return;
			}

			NodeRef uncle = grandparent->children[1 - u8(dir)];
			if (!uncle || uncle->color == Detail::RedBlackTreeColor::Black)
			{
				if (node == parent->children[1 - u8(dir)])
				{
					// Case 5: parent is red, uncle is black and node is inner grandchild
					Rotate(parent, dir);
					node = parent;
					parent = grandparent->children[u8(dir)];
					// Fallthrough to case 6
				}

				// Case 6: parent is red, uncle is black and node is outer grandchild
				Rotate(grandparent, RotateDir(1 - u8(dir)));
				parent->color = Detail::RedBlackTreeColor::Black;
				grandparent->color = Detail::RedBlackTreeColor::Red;
				return;	
			}

			// Case 2: parent and uncle are red
			parent->color = uncle->color = Detail::RedBlackTreeColor::Black;
			if (grandparent != m_root)
				grandparent->color = Detail::RedBlackTreeColor::Red;
			node = grandparent;
		}
		while (node);
	}
	
	template <Movable T, Comparator<T> C, bool AllowMultiple>
	template <bool Moved>
	auto RedBlackTree<T, C, AllowMultiple>::Copy(const RedBlackTree& other) noexcept -> void
	{
		for (Iterator it = other.Begin(); it.m_node; ++it)
		{
			if constexpr (Moved)
				Insert(Move(*it));
			else
				Insert(*it);
		}

		if constexpr (Moved)
			other.template ClearInternal<false>();
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	template <bool Destruct>
	auto RedBlackTree<T, C, AllowMultiple>::EraseInternal(Iterator it) noexcept -> Iterator
	{
		Iterator nextIt = it + 1;

		NodeRef node = it.m_node;
		node = EraseNode(node);
		if constexpr (Destruct)
			node->value.~T();
		node.Dealloc();

		--m_size;
		return nextIt;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::EraseNode(NodeRef node) noexcept -> NodeRef
	{
		// If only the root node exists
		if (m_size == 1)
		{
			m_root = NodeRef{ m_root.GetAlloc() };
			return node;
		}

		if (node->left && node->right)
		{
			NodeRef inOrderPredecessor = node->left;
			while (inOrderPredecessor->right)
				inOrderPredecessor->right;

			Swap(node->value, inOrderPredecessor->value);
			node = inOrderPredecessor;
		}
		else if (node->left || node->right)
		{
			ASSERT(node->color == Detail::RedBlackTreeColor::Black, "Invalid RedBlackTree");

			if (NodeRef left = node->left)
			{
				ASSERT(left->color == Detail::RedBlackTreeColor::Red, "Invalid RedBlackTree");

				left->color = Detail::RedBlackTreeColor::Black;
				NodeRef parent = node->parent;
				left->parent = parent;
				node->parent = left;
				if (parent)
					parent->children[node == parent->right] = left;
				else
					m_root = left;
			}
			else
			{
				NodeRef right = node->right;
				ASSERT(right->color == Detail::RedBlackTreeColor::Red, "Invalid RedBlackTree");

				right->color = Detail::RedBlackTreeColor::Black;
				NodeRef parent = node->parent;
				right->parent = parent;
				node->parent = right;
				if (parent)
					parent->children[node == parent->right] = right;
				else
					m_root = right;
			}
			return node;
		}

		// If leaf is red, we can just remove it (done in EraseInternal)
		if (node->color == Detail::RedBlackTreeColor::Red)
		{
			NodeRef parent = node->parent;
			parent->children[node == parent->right] = NodeRef{};
			return node;
		}

		EraseLeafNode(node);
		return node;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::EraseLeafNode(NodeRef node) noexcept -> void
	{
		NodeRef parent = node->parent;
		RotateDir dir = RotateDir(node == parent->right);
		parent->children[u8(dir)] = NodeRef{};

		do
		{
			NodeRef sibling = parent->children[1 - u8(dir)];
			NodeRef distantNephew = sibling ? sibling->children[1 - u8(dir)] : NodeRef{};
			NodeRef closestNephew = sibling ? sibling->children[u8(dir)] : NodeRef{};

			bool case3 = false;
			if (sibling->color == Detail::RedBlackTreeColor::Red)
			{
				// Case 3: sibling is red and parent, distant and closest nephew are black
				Rotate(parent, dir);
				parent->color = Detail::RedBlackTreeColor::Red;
				sibling->color = Detail::RedBlackTreeColor::Black;
				sibling = closestNephew;
				distantNephew = sibling->children[1 - u8(dir)];
				case3 = true;
			}

			if (distantNephew && distantNephew->color == Detail::RedBlackTreeColor::Red)
			{
				// Case 6: distant nephew is red and sibling is black
				Rotate(parent, dir);
				sibling->color = parent->color;
				parent->color = Detail::RedBlackTreeColor::Black;
				distantNephew->color = Detail::RedBlackTreeColor::Black;
				return;
			}

			closestNephew = sibling->children[u8(dir)];
			if (closestNephew && closestNephew->color == Detail::RedBlackTreeColor::Red)
			{
				// Case 5: closest nephew is red and sibling is black
				Rotate(sibling, RotateDir(1 - u8(dir)));
				sibling->color = Detail::RedBlackTreeColor::Red;
				closestNephew->color = Detail::RedBlackTreeColor::Black;
				distantNephew = sibling;
				sibling = closestNephew;

				// Fallthrough to
				// Case 6: distant nephew is red and sibling is black
				Rotate(parent, dir);
				sibling->color = parent->color;
				parent->color = Detail::RedBlackTreeColor::Black;
				distantNephew->color = Detail::RedBlackTreeColor::Black;
				return;
			}

			if (case3 || parent->color == Detail::RedBlackTreeColor::Red)
			{
				// Case 4: parent is red and sibling, distant and closest nephew are black
				sibling->color = Detail::RedBlackTreeColor::Red;
				parent->color = Detail::RedBlackTreeColor::Black;
				return;
			}

			// Case 1: parent, sibling, distant and closest nephew are black
			sibling->color = Detail::RedBlackTreeColor::Red;

			node = parent;
			parent = node->parent;

			if (parent)
				dir = RotateDir(node == parent->left);
		}
		// the 'while' also handles case 2: no parent
		while (parent);
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	template<bool Destruct>
	auto RedBlackTree<T, C, AllowMultiple>::ClearInternal() noexcept -> void
	{
		Alloc::IAllocator* pAlloc = m_root.GetAlloc();

		NodeRef node = m_root;
		while (node)
		{
			if (!node->left || !node->right)
			{
				NodeRef parent = node->parent;
				if (parent)
					parent->children[node == parent->right] = NodeRef{};

				if constexpr (Destruct)
					node->value.~T();
				node.Dealloc();
				node = parent;
				continue;
			}

			if (node->left)
				node = node->left;
			else
				node = node->right;
		}

		m_root = NodeRef{ pAlloc };
		m_size = 0;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::GetFirstNode() const noexcept -> NodeRef
	{
		if (!m_root)
			return NodeRef{};

		NodeRef node = m_root;
		while (node->left)
			node = node->left;
		return node;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::GetLastNode() const noexcept -> NodeRef
	{
		if (!m_root)
			return NodeRef{};

		NodeRef node = m_root;
		while (node->right)
			node = node->right;
		return node;
	}

	template <Movable T, Comparator<T> C, bool AllowMultiple>
	auto RedBlackTree<T, C, AllowMultiple>::Compare(const T& val, const NodeRef& node) const noexcept -> i8
	{
		if constexpr (AllowMultiple)
			return m_comp(val, node->value[0]);
		else
			return m_comp(val, node->value);
	}
}
