#pragma once

#if __RESHARPER__
#include "ByteBuffer.h"
#endif

namespace Core
{
	inline ByteBuffer::ByteBuffer(Alloc::IAllocator& alloc) noexcept
		: m_data(alloc)
		, m_cursor(0)
	{
	}

	inline ByteBuffer::ByteBuffer(usize capacity, Alloc::IAllocator& alloc)
		: m_data(capacity, alloc)
		, m_cursor(0)
	{
	}

	inline ByteBuffer::ByteBuffer(const u8* pData, usize size) noexcept
		: m_data(pData, pData + size)
		, m_cursor(0)
	{
	}

	inline ByteBuffer::ByteBuffer(const DynArray<u8>& data) noexcept
		: m_data(data)
		, m_cursor(0)
	{
	}

	inline ByteBuffer::ByteBuffer(DynArray<u8>&& data) noexcept
		: m_data(Move(data))
		, m_cursor(0)
	{
	}

	inline ByteBuffer::ByteBuffer(const ByteBuffer& other) noexcept
		: m_data(other.m_data)
		, m_cursor(other.m_cursor)
	{
	}

	inline ByteBuffer::ByteBuffer(const ByteBuffer& other, Alloc::IAllocator& alloc) noexcept
		: m_data(other.m_data, alloc)
		, m_cursor(other.m_cursor)
	{
	}

	inline ByteBuffer::ByteBuffer(ByteBuffer&& other) noexcept
		: m_data(Move(other.m_data))
		, m_cursor(other.m_cursor)
	{
	}

	inline ByteBuffer::ByteBuffer(ByteBuffer&& other, Alloc::IAllocator& alloc) noexcept
		: m_data(Move(other.m_data), alloc)
		, m_cursor(other.m_cursor)
	{
	}

	inline auto ByteBuffer::operator=(const ByteBuffer& other) noexcept -> ByteBuffer&
	{
		m_data = other.m_data;
		m_cursor = other.m_cursor;
		return *this;
	}

	inline auto ByteBuffer::operator=(ByteBuffer&& other) noexcept -> ByteBuffer&
	{
		m_data = Move(other.m_data);
		m_cursor = other.m_cursor;
		return *this;
	}

	inline void ByteBuffer::Reserve(usize newCap) noexcept
	{
		m_data.Reserve(newCap);
	}

	inline void ByteBuffer::Resize(usize newSize, u8 val) noexcept
	{
		m_data.Resize(newSize, val);
	}

	inline void ByteBuffer::Resize(usize newSize) noexcept
	{
		m_data.Resize(newSize);
	}

	inline void ByteBuffer::ShrinkToFit() noexcept
	{
		m_data.ShrinkToFit();
	}

	inline void ByteBuffer::Seek(usize pos) noexcept
	{
		const usize size = Size();
		if (pos >= size)
			pos = size;
		m_cursor = pos;
	}

	template <typename T>
	auto ByteBuffer::Peek(usize offset) noexcept -> const T&
	{
		ASSERT(m_cursor + offset < Size(), "Peek out of range");
		return *reinterpret_cast<T*>(&m_data[m_cursor + offset]);
	}

	template <typename T>
	auto ByteBuffer::Read(usize offset) noexcept -> const T&
	{
		usize readPos = m_cursor + offset;
		ASSERT(readPos < Size(), "Read out of range");
		m_cursor += offset;
		return *reinterpret_cast<T*>(&m_data[readPos]);
	}

	template <typename T>
	void ByteBuffer::Write(const T& val) noexcept
	{
		if (m_cursor + sizeof(T) >= Size())
			m_data.Resize(m_cursor + sizeof(T));
		new (&m_data[m_cursor]) T{ val };
		m_cursor += sizeof(T);
	}

	template <typename T>
	void ByteBuffer::Insert(const T& val, usize numBytes) noexcept
	{
		m_data.Insert(m_data.IteratorAt(m_cursor), numBytes, 0);
		Write(val);
	}

	inline auto ByteBuffer::GetContainer() noexcept -> DynArray<u8>&
	{
		return m_data;
	}

	inline auto ByteBuffer::GetContainer() const noexcept -> const DynArray<u8>&
	{
		return m_data;
	}

	inline auto ByteBuffer::Size() const noexcept -> usize
	{
		return m_data.Size();
	}

	inline auto ByteBuffer::Capacity() const noexcept -> usize
	{
		return m_data.Capacity();
	}

	inline auto ByteBuffer::IsEmpty() const noexcept -> bool
	{
		return m_data.IsEmpty();
	}

	inline auto ByteBuffer::Data() noexcept -> u8*
	{
		return m_data.Data();
	}

	inline auto ByteBuffer::Data() const noexcept -> const u8*
	{
		return m_data.Data();
	}
}
