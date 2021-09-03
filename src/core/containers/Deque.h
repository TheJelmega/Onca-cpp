#pragma once
#include "core/MinInclude.h"
#include "core/allocator/GlobalAlloc.h"
#include "core/memory/MemRef.h"

namespace Core
{

	/**
	 * A double-ended queue
	 * \tparam T Underlying types
	 * \tparam BlockSize Size of a block allocated by the Deque
	 */
	// TODO: Balance MemRefs in base array to be centered (less moving on PushFront)
	template<typename T, usize BlockSize = 8>
	class Deque
	{
		STATIC_ASSERT(IsPowOf2(BlockSize), "BlockSize needs to be a power of 2");
	public:
		class Iterator
		{
		public:
			Iterator();
			Iterator(const Iterator& other) noexcept;
			Iterator(Iterator&&) noexcept;

			auto operator=(const Iterator& other) noexcept;
			auto operator=(Iterator&& other) noexcept -> Iterator&;

			auto operator++() noexcept -> Iterator;
			auto operator++(int) noexcept -> Iterator;
			auto operator--() noexcept -> Iterator;
			auto operator--(int) noexcept -> Iterator;

			auto operator->() const noexcept -> T*;
			auto operator*() const noexcept -> T&;

			auto operator+(usize count) const noexcept -> Iterator;
			auto operator-(usize count) const noexcept -> Iterator;
			auto operator-(const Iterator& it) const noexcept -> isize;

			auto operator+=(usize count) noexcept -> Iterator&;
			auto operator-=(usize count) noexcept -> Iterator&;

			auto operator==(const Iterator& other) const noexcept -> bool;
			auto operator!=(const Iterator& other) const noexcept -> bool;
			auto operator<=>(const Iterator& other) const noexcept -> std::partial_ordering;

			auto operator[](usize idx) noexcept -> T&;
			auto operator[](usize idx) const noexcept -> const T&;

		private:
			Iterator(const MemRef<MemRef<T>>& blocks, usize blockOffset, usize idx) noexcept;

			MemRef<MemRef<T>> m_blocks;
			usize m_blockOffset;
			usize m_idx;

			friend class Deque;
		};
		using ConstIterator = const Iterator;

	public:
		/**
		 * Create a Deque with an allocator
		 * \param[in] alloc Allocator the container should use
		 * \note No allocation happens
		 */
		explicit Deque(Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a Deque filled with default values
		 * \param[in] count Number of elements to create
		 * \param[in] alloc Allocator the container should use
		 */
		explicit Deque(usize count, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires DefaultConstructable<T>;
		/**
		 * Create a Deque filled with a number of elements
		 * \param[in] count Number of elements to create
		 * \param[in] val Value of elements
		 * \param[in] alloc Allocator the container should use
		 */
		explicit Deque(usize count, const T& val, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructable<T>;
		/**
		 * Create a Deque from an initializer list
		 * \param[in] il Initializer list with elements
		 * \param[in] alloc Allocator the containter should use
		 */
		explicit Deque(const InitializerList<T>& il, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructable<T>;
		/**
		 * Create a Deque from an iterable range
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] alloc Allocator the container should use
		 */
		template<ForwardIterator It>
		explicit Deque(const It& begin, const It& end, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept requires CopyConstructable<T>;
		/**
		 * Create a Deque with the contents of another Deque
		 * \param[in] other Deque to copy
		 */
		Deque(const Deque& other) noexcept requires CopyConstructable<T>;
		/**
		 * Create a Deque with the contents of another Deque
		 * \tparam B Block size of other deque
		 * \param[in] other Deque to copy
		 */
		template<usize B>
		Deque(const Deque<T, B>& other) noexcept requires CopyConstructable<T>;
		/**
		 * Create a Deque with the contents of another Deque, but with a different allocator
		 * \tparam B Block size of other deque
		 * \param[in] other Deque to copy
		 * \param[in] alloc Allocator the container should use
		 */
		template<usize B>
		explicit Deque(const Deque<T, B>& other, Alloc::IAllocator& alloc) noexcept requires CopyConstructable<T>;
		/**
		 * Move another Deque into a new Deque
		 * \param[in] other Deque to move from
		 */
		Deque(Deque&& other) noexcept;
		/**
		 * Move another Deque into a new Deque, but with a different allocator
		 * \tparam B Block size of other deque
		 * \param[in] other Deque to move from
		 * \param[in] alloc Allocator the container should use
		 */
		template<usize B>
		Deque(Deque<T, B>&& other, Alloc::IAllocator& alloc) noexcept;
		~Deque() noexcept;

		auto operator=(const InitializerList<T>& il) noexcept -> Deque<T>& requires CopyConstructable<T>;
		auto operator=(const Deque& other) noexcept -> Deque<T>& requires CopyConstructable<T>;
		template<usize B>
		auto operator=(const Deque<T, B>& other) noexcept -> Deque<T>& requires CopyConstructable<T>;
		auto operator=(Deque&& other) noexcept -> Deque<T>&;

		/**
		 * Assign an iterable range to the Deque
		 * \tparam It Iterator type
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 */
		template<ForwardIterator It>
		auto Assign(const It& begin, const It& end) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Assign a linked list to the Deque
		 * \param[in] il Initializer list with elements
		 */
		auto Assign(const InitializerList<T>& il) noexcept -> void requires CopyConstructable<T>;

		/**
		 * Fill the Deque with a number of elements
		 * \param[in] count Number of elements to fill
		 * \param[in] val Value to fill elements with
		 */
		auto Fill(usize count, const T& val) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Fill the Deque with a number of elements with a default value (via placement new)
		 * \param[in] count Number of elements to fill
		 */
		auto FillDefault(usize count) noexcept -> void requires NoThrowDefaultConstructable<T>;
		
		/**
		 * Resize the Deque and fill missing elements if needed
		 * \param[in] newSize New size of the Deque
		 * \param[in] val Value to fill missing elements with
		 */
		auto Resize(usize newSize, const T& val) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Resize the Deque and fill missing elements with a default value (via placement new) if needed
		 * \param[in] newSize New size of the Deque
		 */
		auto Resize(usize newSize) noexcept -> void  requires NoThrowDefaultConstructable<T>;

		/**
		 * Add an element to the front of the Deque
		 * \param[in] val Element to add
		 */
		auto PushFront(const T& val) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Add an element to the front of the Deque
		 * \param[in] val Element to add
		 */
		auto PushFront(T&& val) noexcept -> void;
		/**
		 * Add the contents of a Deque to the front of the Deque
		 * \tparam B Block size of other deque
		 * \param[in] other Deque to add
		 */
		template<usize B>
		auto PushFront(const Deque<T, B>& other) -> void requires CopyConstructable<T>;
		/**
		 * Add the contents of a Deque to the front of the Deque
		 * \tparam B Block size of other deque
		 * \param[in] other Deque to add
		 */
		template<usize B>
		auto PushFront(Deque<T, B>&& other) -> void;

		/**
		 * Emplace an element at the front of the Deque
		 * \tparam Args Type of arguments
		 * \param[in] args Arguments
		 */
		template<typename ...Args>
		requires ConstructableFrom<T, Args...>
			auto EmplaceFront(Args&&... args) noexcept -> void;

		/**
		 * Add an element to the Deque
		 * \param[in] val Element to add
		 */
		auto Push(const T& val) noexcept -> void requires CopyConstructable<T>;
		/**
		 * Add an element to the Deque
		 * \param[in] val Element to add
		 */
		auto Push(T&& val) noexcept -> void;
		/**
		 * Add the contents of a Deque to the Deque
		 * \tparam B Block size of other deque
		 * \param[in] other Deque to add
		 */
		template<usize B>
		auto Push(const Deque<T, B>& other) -> void requires CopyConstructable<T>;
		/**
		 * Add the contents of a Deque to the Deque
		 * \tparam B Block size of other deque
		 * \param[in] other Deque to add
		 */
		template<usize B>
		auto Push(Deque<T, B>&& other) -> void;

		/**
		 * Emplace an element at the back of the Deque
		 * \tparam Args Type of arguments
		 * \param[in] args Arguments
		 */
		template<typename ...Args>
		requires ConstructableFrom<T, Args...>
			auto EmplaceBack(Args&&... args) noexcept -> void;

		/**
		 * Insert an element in a certain location
		 * \param[in] it Iterator to position to insert the element at
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto Insert(const ConstIterator& it, const T& val) noexcept -> Iterator requires CopyConstructable<T>;
		/**
		 * Insert an element in a certain location
		 * \param[in] it Iterator to position to insert the element at
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto Insert(const ConstIterator& it, T&& val) noexcept -> Iterator;
		/**
		 * Insert a number of elements in the Deque
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] count Number of elements to insert
		 * \param[in] val Value of elements to insert
		 * \return Iterator to the first element that was inserter
		 */
		auto Insert(const ConstIterator& it, usize count, const T& val) noexcept -> Iterator requires CopyConstructable<T>;
		/**
		 * Insert an iterable range into the Deque
		 * \tparam It Iterator type
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \return Iterator to the first element that was inserted
		 */
		template<ForwardIterator It>
		auto Insert(const ConstIterator& it, const It& begin, const It& end) noexcept -> Iterator requires CopyConstructable<T>;
		/**
		 * Insert an initializer list into the Deque
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] il Initializer list to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto Insert(const ConstIterator& it, const InitializerList<T>& il) noexcept -> Iterator requires CopyConstructable<T>;
		/**
		 * Insert a Deque into the Deque
		 * \tparam B Block size of other deque
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] other Deque to insert
		 * \return Iterator to the first element that was inserted
		 */
		template<usize B>
		auto Insert(const ConstIterator& it, const Deque<T, B>& other) noexcept -> Iterator requires CopyConstructable<T>;
		/**
		 * Insert a Deque into the Deque
		 * \tparam B Block size of other deque
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] other Deque to insert
		 * \return Iterator to the first element that was inserted
		 */
		template<usize B>
		auto Insert(const ConstIterator& it, Deque<T, B>&& other) noexcept -> Iterator;

		/**
		 * Emplace an element into the Deque
		 * \tparam Args Type of arguments
		 * \param[in] it Iterator to position to insert elements at
		 * \param[in] args Arguments
		 */
		template<typename ...Args>
		requires ConstructableFrom<T, Args...>
			auto Emplace(const ConstIterator& it, Args&&... args) noexcept -> Iterator;

		/**
		 * Clear the contents of the Deque, possibly also deallocate the memory
		 * \param[in] clearMemory Whether to deallocate the memory
		 */
		auto Clear(bool clearMemory = false) noexcept -> void;
		/**
		 * Remove the first element from the Deque
		 */
		auto PopFront() noexcept -> void;
		/**
		 * Remove the last element from the Deque
		 */
		auto Pop() noexcept -> void;
		/**
		 * Erase an element from the Deque
		 * \param[in] it Iterator to element to erase
		 */
		auto Erase(const Iterator& it) noexcept -> void;
		/**
		 * Erase a number of elements from the Deque
		 * \param[in] it Iterator to first element to erase
		 * \param[in] count Number of elements to erase
		 */
		auto Erase(const Iterator& it, usize count) noexcept -> void;
		/**
		 * Erase a range of elements from the Deque
		 * \param[in] begin Iterator to first element to erase
		 * \param[in] end Iterator to last element to erase
		 */
		auto Erase(const Iterator& begin, const Iterator& end) noexcept -> void;

		/**
		 * Find the first element that matches the looked for value
		 * \tparam U Type to compare with
		 * \param[in] value Value to find
		 * \return Iterator to found element
		 */
		template<EqualComparable<T> U>
		auto Find(const U& value) noexcept -> Iterator;
		/**
		 * Find the first element that matches the looked for value
		 * \tparam U Type to compare with
		 * \param[in] value Value to find
		 * \return Iterator to found element
		 */
		template<EqualComparable<T> U>
		auto Find(const U& value) const noexcept -> ConstIterator;

		/**
		 * Get the element at an index
		 * \param[in] idx Index of element
		 * \return Optional with value
		 * \note Will return an empty optional when index is out of bounds
		 */
		auto At(usize idx) const noexcept -> Optional<T>;
		/**
		 * Get an iterator to the element at an index
		 * \param[in] idx Index to get iterator to
		 * \return Iterator to element at idx
		 */
		auto IteratorAt(usize idx) noexcept -> Iterator;
		/**
		 * Get an iterator to the element at an index
		 * \param[in] idx Index to get iterator to
		 * \return Iterator to element at idx
		 */
		auto IteratorAt(usize idx) const noexcept -> ConstIterator;

		/**
		 * Get the element at an index
		 * \param[in] idx Index of element
		 * \return Element at index
		 * \note Only use when certain the index is not out of bounds
		 */
		auto operator[](usize idx) noexcept -> T&;
		/**
		 * Get the element at an index
		 * \param[in] idx Index of element
		 * \return Element at index
		 * \note Only use when certain the index is not out of bounds
		 */
		auto operator[](usize idx) const noexcept -> const T&;

		/**
		 * Get the size of the Deque
		 * \return Size of the Deque
		 */
		auto Size() const noexcept -> usize;
		/**
		 * Check if the Deque is empty
		 * \return Whether the Deque is empty
		 */
		auto IsEmpty() const noexcept ->  bool;

		/**
		 * Get the allocator used by the Deque
		 * \return Allocator used by the Deque
		 */
		auto GetAllocator() const noexcept -> Alloc::IAllocator*;

		/**
		 * Get the first element in the Deque
		 * \return First element in the Deque
		 * \note Only use when the Deque is not empty
		 */
		auto Front() noexcept -> T&;
		/**
		 * Get the first element in the Deque
		 * \return First element in the Deque
		 * \note Only use when the Deque is not empty
		 */
		auto Front() const noexcept -> const T&;
		/**
		 * Get the last element in the Deque
		 * \return Last element in the Deque
		 * \note Only use when the Deque is not empty
		 */
		auto Back() noexcept -> T&;
		/**
		 * Get the last element in the Deque
		 * \return Last element in the Deque
		 * \note Only use when the Deque is not empty
		 */
		auto Back() const noexcept -> const T&;

		/**
		 * Get an iterator to the first element
		 * \return Iterator to the first element
		 */
		auto Begin() noexcept -> Iterator;
		/**
		 * Get an iterator to the first element
		 * \return Iterator to the first element
		 */
		auto Begin() const noexcept -> ConstIterator;

		/**
		 * Get an iterator to the end of the elements
		 * \return Iterator to the end of the elements
		 */
		auto End() noexcept -> Iterator;
		/**
		 * Get an iterator to the end of the elements
		 * \return Iterator to the end of the elements
		 */
		auto End() const noexcept -> ConstIterator;

		/**
		 * Get a reverse Reverse iterator to the first element
		 * \return Reverse iterator to the first element
		 */
		auto RBegin() noexcept -> Iterator;
		/**
		 * Get a reverse Reverse iterator to the first element
		 * \return Reverse iterator to the first element
		 */
		auto RBegin() const noexcept -> ConstIterator;

		/**
		 * Get a reverse Reverse iterator to the end of the elements
		 * \return Reverse iterator to the end of the elements
		 */
		auto REnd() noexcept -> Iterator;
		/**
		 * Get a reverse Reverse iterator to the end of the elements
		 * \return Reverse iterator to the end of the elements
		 */
		auto REnd() const noexcept -> ConstIterator;

		// Overloads for 'for ( ... : ... )'
		auto begin() noexcept -> Iterator;
		auto begin() const noexcept -> ConstIterator;
		auto cbegin() const noexcept -> ConstIterator;
		auto end() noexcept -> Iterator;
		auto end() const noexcept -> ConstIterator;
		auto cend() const noexcept -> ConstIterator;

	private:
		static constexpr usize Mask = BlockSize - 1;                  ///< Mask to mask out index bits
		static constexpr usize BlockByteSize = BlockSize * sizeof(T); ///< Size of a block in bytes

		MemRef<MemRef<T>> m_blocks;        ///< Array of blocks
		usize             m_initialOffset; ///< Starting offset in the first block
		usize             m_size;          ///< Size of the deck

		/**
		 * Get the block offset and index of the element
		 * \param idx Index of the element
		 * \return Pair with the block offset and index of the element
		 */
		auto GetElemOffsetIdx(usize idx) const noexcept -> Pair<usize, usize>;
		/**
		 * Get the address of an element
		 * \param idx Index of the element
		 * \return Address of the element
		 */
		auto GetElemAddr(usize idx) const noexcept -> T*;
		/**
		 * Get the address of an element
		 * \param pBegin Address to first base elem containing the first block
		 * \param idx Index of the element
		 * \return Address of the element
		 */
		auto GetElemAddr(MemRef<T>* pBegin, usize idx) const noexcept -> T*;
		
		/**
		 * Add a number of blocks at the back of the current blocks
		 * \param numBlocks Number of block to add
		 */
		auto AddBackBlocks(usize numBlocks) noexcept -> void;
		/**
		 * Add a number of blocks on the front of the current blocks
		 * \param numBlocks Number of block to add
		 */
		auto AddFrontBlocks(usize numBlocks) noexcept -> void;

		/**
		 * Add space for a number of blocks in the base array
		 * \tparam AtBack Whether the allocation needs to be after the current block, false if it need to be in front
		 * \param numAdditionalBlocks Number of blocks to reserve space for
		 * \return Pointer to the first new element in the base array
		 */
		template<bool AtBack>
		auto ReserveBase(usize numAdditionalBlocks) noexcept -> MemRef<T>*;

		/**
		 * Prepare the Deque to insert a number of elements
		 * \param idx Index to insert element at
		 * \param moveOffset Number of elements to reserve space for
		 */
		auto PrepareInsert(usize idx, usize moveOffset) noexcept -> void;
		/**
		 * Insert a range of elements into the Deque
		 * \tparam It Forward iterator
		 * \param it Iterator to insert at
		 * \param count Number of elements to insert
		 * \param begin Iterator to the first elem tot insert
		 * \param end Iterator past the last element to insert
		 * \return Iterator to first reserved place
		 */
		template<ForwardIterator It>
		auto InsertIts(ConstIterator& it, usize count, const It& begin, const It& end) noexcept -> Iterator;

		/**
		 * Get an iterator to the element at an index
		 * \param[in] idx Index to get iterator to
		 * \return Iterator to element at idx
		 */
		auto IteratorAtInternal(usize idx) const noexcept -> Iterator;
	};

}

#include "Deque.inl"