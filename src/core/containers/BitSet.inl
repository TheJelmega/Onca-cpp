#pragma once
#if __RESHARPER__
#include "BitSet.h"
#endif

namespace Core
{
	inline BitSet::BitSet(Alloc::IAllocator& alloc) noexcept
		: m_data(alloc)
		, m_numBits(0)
	{
	}

	inline BitSet::BitSet(usize numBits, Alloc::IAllocator& alloc) noexcept
		: m_data((numBits + BitIdxMask) / BitsPerElem, alloc)
		, m_numBits(numBits)
	{
	}

	inline BitSet::BitSet(const BitSet& other) noexcept
		: m_data(other.m_data)
		, m_numBits(other.m_numBits)
	{
	}

	inline BitSet::BitSet(const BitSet& other, Alloc::IAllocator& alloc) noexcept
		: m_data(other.m_data, alloc)
		, m_numBits(other.m_numBits)
	{
	}

	inline BitSet::BitSet(BitSet&& other) noexcept
		: m_data(Move(other.m_data))
		, m_numBits(other.m_numBits)
	{
		other.m_numBits = 0;
	}

	inline BitSet::BitSet(BitSet&& other, Alloc::IAllocator& alloc) noexcept
		: m_data(Move(other.m_data), alloc)
		, m_numBits(other.m_numBits)
	{
	}

	inline auto BitSet::operator~() const noexcept -> BitSet
	{
		BitSet res{ m_numBits, *m_data.GetAllocator() };
		for (usize i = 0; i < DataSize(); ++i)
			res.m_data[i] = ~m_data[i];
		return res;
	}

	inline auto BitSet::operator|(const BitSet& other) const noexcept -> BitSet
	{
		BitSet res{ Math::Max(m_numBits, other.m_numBits), *m_data.GetAllocator() };

		const usize minBytes = Math::Min(DataSize(), other.DataSize());
		for (usize i = 0; i < minBytes; ++i)
			res.m_data[i] = m_data[i] | other.m_data[i];

		if (DataSize() > minBytes)
			MemCpy(res.Data() + minBytes, Data() + minBytes, DataSize() - minBytes);
		else if (other.DataSize() > minBytes)
			MemCpy(res.Data() + minBytes, other.Data() + minBytes, other.DataSize() - minBytes);
		return res;
	}

	inline auto BitSet::operator^(const BitSet& other) const noexcept -> BitSet
	{
		BitSet res{ Math::Max(m_numBits, other.m_numBits), *m_data.GetAllocator() };

		const usize minBytes = Math::Min(DataSize(), other.DataSize());
		for (usize i = 0; i < minBytes; ++i)
			res.m_data[i] = m_data[i] ^ other.m_data[i];

		if (DataSize() > minBytes)
			MemCpy(res.Data() + minBytes, Data() + minBytes, DataSize() - minBytes);
		else if (other.DataSize() > minBytes)
			MemCpy(res.Data() + minBytes, other.Data() + minBytes, other.DataSize() - minBytes);
		return res;
	}

	inline auto BitSet::operator&(const BitSet& other) const noexcept -> BitSet
	{
		BitSet res{ Math::Max(m_numBits, other.m_numBits), *m_data.GetAllocator() };

		const usize minBytes = Math::Min(DataSize(), other.DataSize());
		for (usize i = 0; i < minBytes; ++i)
			res.m_data[i] = m_data[i] & other.m_data[i];
		return res;
	}

	inline auto BitSet::operator=(const BitSet& other) noexcept -> BitSet&
	{
		m_data = other.m_data;
		m_numBits = other.m_numBits;
		return *this;
	}

	inline auto BitSet::operator=(BitSet&& other) noexcept -> BitSet&
	{
		m_data = Move(other.m_data);
		m_numBits = other.m_numBits;
		other.m_numBits = 0;
		return *this;
	}

	inline auto BitSet::operator|=(const BitSet& other) noexcept -> BitSet&
	{
		const usize numBytes = DataSize();
		const usize numOtherBytes = other.DataSize();
		if (numOtherBytes > numBytes)
			m_data.Resize(other.DataSize());
		
		const usize minBytes = Math::Min(DataSize(), numOtherBytes);
		for (usize i = 0; i < minBytes; ++i)
			m_data[i] |= other.m_numBits;

		if (other.DataSize() > numBytes)
		{
			MemCpy(Data() + numBytes, other.Data() + numBytes, numOtherBytes - numBytes);
		}
		else
		{
			usize mask = usize(-1) << (BitIdxMask - (m_numBits & BitIdxMask));
			m_data.Back() &= mask;
		}
		return *this;
	}

	inline auto BitSet::operator^=(const BitSet& other) noexcept -> BitSet&
	{
		if (other.DataSize() > DataSize())
			m_data.Resize(other.DataSize());

		usize minSize = Math::Min(DataSize(), other.DataSize());
		for (usize i = 0; i < minSize; ++i)
			m_data[i] ^= other.m_numBits;

		if (other.DataSize() > DataSize())
		{
			for (usize i = DataSize(); i < other.DataSize(); ++i)
				m_data[i] = other.m_data[i];
		}
		return *this;
	}

	inline auto BitSet::operator&=(const BitSet& other) noexcept -> BitSet&
	{
		if (other.DataSize() > DataSize())
			m_data.Resize(other.DataSize());

		const usize minSize = Math::Min(DataSize(), other.DataSize());
		for (usize i = 0; i < minSize; ++i)
			m_data[i] &= other.m_numBits;
		return *this;
	}

	inline auto BitSet::operator[](usize idx) const noexcept -> bool
	{
		if (idx >= m_numBits)
			return false;

		const usize byteIdx = idx / BitsPerElem;
		const usize bitIdx = idx & BitIdxMask;
		return (m_data[byteIdx] >> (BitIdxMask - bitIdx)) & 0x1;
	}

	inline auto BitSet::operator==(const BitSet& other) const noexcept -> bool
	{
		if (m_numBits != other.m_numBits)
			return false;
		
		for (usize i = 0; i < m_data.Size(); ++i)
		{
			if (m_data[i] != other.m_data[i])
				return false;
		}
		return true;
	}

	inline auto BitSet::Match(const BitSet& other) const noexcept -> bool
	{
		const BitSet& expected = m_numBits < other.m_numBits ? *this : other;
		for (usize i = 0, size = expected.DataSize(); i < size; ++i)
		{
			if ((m_data[i] & other.m_data[i]) != expected.m_data[i])
				return false;
		}
		return true;
	}

	inline auto BitSet::Set(usize idx, bool val) noexcept -> void
	{
		if (val)
			Set(idx);
		else
			Unset(idx);
	}

	inline auto BitSet::Set(usize idx) noexcept -> void
	{
		if (idx >= m_numBits)
			return;

		const usize byteIdx = idx / BitsPerElem;
		const usize bitIdx = idx & BitIdxMask;
		m_data[byteIdx] |= usize(1) << (BitIdxMask - bitIdx);
	}

	inline auto BitSet::Unset(usize idx) noexcept -> void
	{
		if (idx >= m_numBits)
			return;

		const usize byteIdx = idx / BitsPerElem;
		const usize bitIdx = idx & BitIdxMask;
		m_data[byteIdx] &= ~(usize(1) << (BitIdxMask - bitIdx));
	}

	inline auto BitSet::Clear() noexcept -> void
	{
		MemClear(Data(), DataSize() * sizeof(usize));
	}

	inline auto BitSet::Flip() noexcept -> void
	{
		for (usize i = 0; i < DataSize(); ++i)
			m_data[i] = ~m_data[i];
	}

	inline auto BitSet::Count() const noexcept -> usize
	{
		usize cnt = 0;
		for (usize i = 0; i < DataSize(); ++i)
		{
			cnt += Intrin::PopCnt(m_data[i]);
		}
		return cnt;
	}

	inline auto BitSet::None() const noexcept -> bool
	{
		if (m_numBits == 0)
			return true;
		
		for (usize i = 0; i < m_data.Size(); ++i)
		{
			if (m_data[i])
				return false;
		}
		return true;
	}

	inline auto BitSet::Any() const noexcept -> bool
	{
		if (m_numBits == 0)
			return true;
		
		for (usize i = 0; i < m_data.Size(); ++i)
		{
			if (m_data[i])
				return true;
		}
		return false;
	}

	inline auto BitSet::All() const noexcept -> bool
	{
		if (m_numBits == 0)
			return true;

		const usize numFullBytes = m_numBits / BitsPerElem;
		for (usize i = 0; i < numFullBytes; ++i)
		{
			if (m_data[i] != 0xFF)
				return false;
		}

		const usize finalBits = m_numBits & BitIdxMask;
		const u8 bitMask = 0xFF << (BitIdxMask - finalBits);
		return (m_data.Back() & bitMask) == bitMask;
	}

	inline auto BitSet::Resize(usize numBits) noexcept -> void
	{
		m_data.Resize((numBits + BitIdxMask) / BitsPerElem);

		if (numBits < m_numBits)
		{
			usize byteIdx = numBits / BitsPerElem;
			usize bitIdx = numBits & BitsPerElem;
			usize mask = 0xFF >> (BitIdxMask - bitIdx);
			m_data[byteIdx] &= mask;
		}
		else
		{
			usize byteIdx = m_numBits / BitsPerElem;
			usize bitIdx = m_numBits & BitsPerElem;
			usize mask = 0xFF >> (BitIdxMask - bitIdx);
			m_data[byteIdx] &= mask;
		}
	}

	inline auto BitSet::NumBits() const noexcept -> usize
	{
		return m_numBits;
	}

	inline auto BitSet::Data() noexcept -> usize*
	{
		return m_data.Data();
	}

	inline auto BitSet::Data() const noexcept -> const usize*
	{
		return m_data.Data();
	}

	inline auto BitSet::DataSize() const noexcept -> usize
	{
		return m_data.Size();
	}

	inline auto BitSet::GetAllocator() const noexcept -> Alloc::IAllocator*
	{
		return m_data.GetAllocator();
	}
}
