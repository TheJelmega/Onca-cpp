#pragma once
#if __RESHARPER__
#include "InplaceBitSet.h"
#endif

namespace Core
{
	template <usize Bits>
	constexpr InplaceBitSet<Bits>::InplaceBitSet() noexcept
	{
		Clear();
	}

	template <usize Bits>
	constexpr InplaceBitSet<Bits>::InplaceBitSet(const InplaceBitSet& other) noexcept
		: m_data(other.m_data)
	{
	}

	template <usize Bits>
	constexpr InplaceBitSet<Bits>::InplaceBitSet(InplaceBitSet&& other) noexcept
		: m_data(Move(other.m_data))
	{
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::operator~() const noexcept -> InplaceBitSet
	{
		InplaceBitSet res;
		for (usize i = 0; i < NumElems; ++i)
			res.m_data[i] = ~m_data[i];
		return res;
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::operator|(const InplaceBitSet& other) const noexcept -> InplaceBitSet
	{
		InplaceBitSet res;
		for (usize i = 0; i < NumElems; ++i)
			res.m_data[i] = m_data[i] | other.m_data[i];
		return res;
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::operator^(const InplaceBitSet& other) const noexcept -> InplaceBitSet
	{
		InplaceBitSet res;
		for (usize i = 0; i < NumElems; ++i)
			res.m_data[i] = m_data[i] ^ other.m_data[i];
		return res;
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::operator&(const InplaceBitSet& other) const noexcept -> InplaceBitSet
	{
		InplaceBitSet res;
		for (usize i = 0; i < NumElems; ++i)
			res.m_data[i] = m_data[i] & other.m_data[i];
		return res;
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::operator=(const InplaceBitSet& other) noexcept -> InplaceBitSet&
	{
		m_data = other.m_data;
		return *this;
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::operator=(InplaceBitSet&& other) noexcept -> InplaceBitSet&
	{
		m_data = Move(other.m_data);
		return *this;
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::operator|=(const InplaceBitSet& other) noexcept -> InplaceBitSet&
	{
		for (usize i = 0; i < NumElems; ++i)
			m_data[i] |= other.m_data[i];
		return *this;
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::operator^=(const InplaceBitSet& other) noexcept -> InplaceBitSet&
	{
		for (usize i = 0; i < NumElems; ++i)
			m_data[i] ^= other.m_data[i];
		return *this;
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::operator&=(const InplaceBitSet& other) noexcept -> InplaceBitSet&
	{
		for (usize i = 0; i < NumElems; ++i)
			m_data[i] &= other.m_data[i];
		return *this;
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::operator[](usize idx) const noexcept -> bool
	{
		if (idx >= Bits)
			return false;

		const usize elemIdx = idx / BitsPerElem;
		const usize bitIdx = idx & BitIdxMask;
		return (m_data[elemIdx] >> (BitIdxMask - bitIdx)) & 0x1;
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::operator==(const InplaceBitSet& other) const noexcept -> bool
	{
		for (usize i = 0; i < NumElems; ++i)
		{
			if (m_data[i] != other.m_data[i])
				return false;
		}
		return true;
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::Match(const InplaceBitSet& other) const noexcept -> bool
	{
		return *this == other;
	}

	template <usize Bits>
	constexpr void InplaceBitSet<Bits>::Set(usize idx, bool val) noexcept
	{
		if (val)
			Set(idx);
		else
			Unset(idx);
	}

	template <usize Bits>
	constexpr void InplaceBitSet<Bits>::Set(usize idx) noexcept
	{
		if (idx >= Bits)
			return;

		const usize elemIdx = idx / BitsPerElem;
		const usize bitIdx = idx & BitIdxMask;
		m_data[elemIdx] |= usize(1) << (BitIdxMask - bitIdx);
	}

	template <usize Bits>
	constexpr void InplaceBitSet<Bits>::Unset(usize idx) noexcept
	{
		if (idx >= Bits)
			return;

		const usize elemIdx = idx / BitsPerElem;
		const usize bitIdx = idx & BitIdxMask;
		m_data[elemIdx] &= ~(usize(1) << (BitIdxMask - bitIdx));
	}

	template <usize Bits>
	constexpr void InplaceBitSet<Bits>::Clear() noexcept
	{
		MemClear(Data(), NumElems * sizeof(usize));
	}

	template <usize Bits>
	constexpr void InplaceBitSet<Bits>::Flip() noexcept
	{
		for (usize i = 0; i < NumElems; ++i)
			m_data[i] = ~m_data[i];
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::Count() const noexcept -> usize
	{
		usize cnt = 0;
		for (usize i = 0; i < NumElems; ++i)
			cnt += Intrin::PopCnt(m_data[i]);
		return cnt;
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::None() const noexcept -> bool
	{
		for (usize i = 0; i < NumElems; ++i)
		{
			if (m_data[i])
				return false;
		}
		return true;
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::Any() const noexcept -> bool
	{
		for (usize i = 0; i < NumElems; ++i)
		{
			if (m_data[i])
				return true;
		}
		return false;
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::All() const noexcept -> bool
	{
		for (usize i = 0; i < NumElems; ++i)
		{
			if (m_data[i] != 0xFF)
				return false;
		}
		return true;
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::NumBits() const noexcept -> usize
	{
		return Bits;
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::Data() noexcept -> usize*
	{
		return m_data.Data();
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::Data() const noexcept -> const usize*
	{
		return m_data.Data();
	}

	template <usize Bits>
	constexpr auto InplaceBitSet<Bits>::DataSize() const noexcept -> usize
	{
		return m_data.Size();
	}
}
