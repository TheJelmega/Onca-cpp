#pragma once
#include "DynArray.h"

namespace Core
{
	class ByteBuffer
	{
	public:
		/**
		 * Create a ByteBuffer with an allocator
		 * \param[in] alloc Allocator
		 */
		explicit ByteBuffer(Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a ByteBuffer with a capacity and an allocator
		 * \param[in] capacity Capacity
		 * \param[in] alloc Allocator
		 */
		explicit ByteBuffer(usize capacity, Alloc::IAllocator& alloc = g_GlobalAlloc);

		/**
		 * Create a ByteBuffer from a DynArray
		 * \param[in] pData Pointer to data
		 * \param[in] size Size of data
		 */
		ByteBuffer(const u8* pData, usize size) noexcept;
		/**
		 * Create a ByteBuffer from a DynArray
		 * \param[in] data DynArray with data
		 */
		explicit ByteBuffer(const DynArray<u8>& data) noexcept;
		/**
		 * Create a ByteBuffer from a moved DynArray
		 * \param[in] data DynArray with data
		 */
		explicit ByteBuffer(DynArray<u8>&& data) noexcept;

		/**
		 * Create a ByteBuffer with the contents another ByteBuffer
		 * \param[in] other ByteBuffer to copy
		 */
		ByteBuffer(const ByteBuffer& other) noexcept;
		/**
		 * Create a ByteBuffer with the contents another ByteBuffer, but with a different allocator
		 * \param[in] other ByteBuffer to copy
		 * \param[in] alloc Allocator
		 */
		ByteBuffer(const ByteBuffer& other, Alloc::IAllocator& alloc) noexcept;
		/**
		 * Move another ByteBuffer into this ByteBuffer
		 * \param[in] other ByteBuffer to move from
		 */
		ByteBuffer(ByteBuffer&& other) noexcept;
		/**
		 * Move another ByteBuffer into this ByteBuffer, but with a different allocator
		 * \param[in] other ByteBuffer to move from
		 */
		ByteBuffer(ByteBuffer&& other, Alloc::IAllocator& alloc) noexcept;

		auto operator=(const ByteBuffer& other) noexcept -> ByteBuffer&;
		auto operator=(ByteBuffer&& other) noexcept -> ByteBuffer&;

		/**
		 * Reserve additional space in the ByteBuffer
		 * \param[in] newCap New capacity
		 */
		auto Reserve(usize newCap) noexcept -> void;
		/**
		 * Resize the ByteBuffer and fill missing elements if needed
		 * \param[in] newSize New size of the DynArray
		 * \param[in] val Value to fill missing elements with
		 */
		auto Resize(usize newSize, u8 val) noexcept -> void;
		/**
		 * Resize the ByteBuffer and fill missing elements with a default value (via placement new) if needed
		 * \param[in] newSize New size of the DynArray
		 */
		auto Resize(usize newSize) noexcept -> void;
		/**
		 * Shrink the memory used by the ByteBuffer to the minimum needed
		 */
		auto ShrinkToFit() noexcept -> void;

		/**
		 * Move the cursor to a position
		 * \param[in] pos Position in bytes
		 */
		auto Seek(usize pos) noexcept -> void;

		/**
		 * Peek at a value in the ByteBuffer, without advancing the cursor
		 * \tparam T Type to peak
		 * \param[in] offset Offset from cursor
		 * \return Peeked value
		 */
		template<typename T>
		auto Peek(usize offset = 0) noexcept -> const T&;
		/**
		 * Read a value from the ByteBuffer and advanced the cursor
		 * \tparam T Type to read
		 * \param[in] offset Offset from cursor
		 * \return Read value
		 */
		template<typename T>
		auto Read(usize offset = 0) noexcept -> const T&;

		/**
		 * Write a value in the ByteBuffer
		 * \tparam T Type to write
		 * \param[in] val Value to write
		 * \note Read overwrites the data already present at the location being read, or adds data when the cursor is at the end
		 */
		template<typename T>
		auto Write(const T& val) noexcept -> void;
		/**
		 * Insert a value in the ByteBuffer
		 * \tparam T Type to write
		 * \param[in] val Value to write
		 * \param[in] numBytes How many bytes to insert before writing the value, numBytes needs to be smaller or equal to sizeof(T), if 0, the function works like a write
		 */
		template<typename T>
		auto Insert(const T& val, usize numBytes = sizeof(T)) noexcept -> void;

		/**
		 * Get the position of the cursor
		 * \return Position of the cursor
		 */
		auto GetCursor() const noexcept -> usize;
		/**
		 * Get the underlying container
		 * \return Underlying container
		 */
		auto GetContainer() noexcept -> DynArray<u8>&;
		/**
		 * Get the underlying container
		 * \return Underlying container
		 */
		auto GetContainer() const noexcept -> const DynArray<u8>&;
		/**
		 * Get the size of the DynArray
		 * \return Size of the DynArray
		 */
		auto Size() const noexcept -> usize;
		/**
		 * Get the capacity of the DynArray
		 * \return Capacity of the DynArray
		 */
		auto Capacity() const noexcept -> usize;
		/**
		 * Check if the DynArray is empty
		 * \return Whether the DynArray is empty
		 */
		auto IsEmpty() const noexcept ->  bool;
		/**
		 * Get a pointer to the DynArray's data
		 * \return Pointer to the DynArray's data
		 */
		auto Data() noexcept -> u8*;
		/**
		 * Get a pointer to the DynArray's data
		 * \return Pointer to the DynArray's data
		 */
		auto Data() const noexcept -> const u8*;

		
	private:
		DynArray<u8> m_data;   ///< Data
		usize        m_cursor; ///< Cursor into data (index)
	};
	
}

#include "ByteBuffer.inl"