#pragma once
#if __RESHARPER__
#include "Flags.h"
#endif

namespace Core
{
	template <EnumType E>
	constexpr Flags<E>::Flags() noexcept
		: m_data(0)
	{
	}

	template <EnumType E>
	constexpr Flags<E>::Flags(E val) noexcept
		: m_data(I(val))
	{
	}

	template <EnumType E>
	constexpr Flags<E>::Flags(I val) noexcept
		: m_data(val)
	{
	}

	template <EnumType E>
	constexpr Flags<E>::Flags(const InitializerList<E>& enums) noexcept
		: m_data(0)
	{
		for (const E* it = enums.begin(), *end = enums.end(); it != end; ++it)
			m_data |= I(*it);
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator!() const noexcept -> bool
	{
		return !m_data;
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator~() const noexcept -> Flags
	{
		return { I(~m_data) };
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator|(Flags other) const noexcept -> Flags
	{
		return { I(m_data | other.m_data) };
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator|(E other) const noexcept -> Flags
	{
		return { I(m_data | I(other)) };
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator|(I mask) const noexcept -> Flags
	{
		return { I(m_data | mask) };
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator&(Flags other) const noexcept -> Flags
	{
		return { I(m_data & other.m_data) };
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator&(E other) const noexcept -> Flags
	{
		return { I(m_data & I(other)) };
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator&(I mask) const noexcept -> Flags
	{
		return { I(m_data & mask) };
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator^(Flags other) const noexcept -> Flags
	{
		return { I(m_data ^ other.m_data) };
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator^(E other) const noexcept -> Flags
	{
		return { I(m_data ^ I(other)) };
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator^(I mask) const noexcept -> Flags
	{
		return { I(m_data ^ mask) };
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator=(const Flags& other) noexcept -> Flags&
	{
		m_data = other.m_data;
		return *this;
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator=(Flags&& other) noexcept -> Flags&
	{
		m_data = other.m_data;
		return *this;
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator|=(Flags other) noexcept -> Flags&
	{
		m_data |= other.m_data;
		return *this;
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator|=(E other) noexcept -> Flags&
	{
		m_data |= I(other);
		return *this;
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator|=(I mask) noexcept -> Flags&
	{
		m_data |= mask;
		return *this;
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator&=(Flags other) noexcept -> Flags&
	{
		m_data &= other.m_data;
		return *this;
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator&=(E other) noexcept -> Flags&
	{
		m_data &= I(other);
		return *this;
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator&=(I mask) noexcept -> Flags&
	{
		m_data &= mask;
		return *this;
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator^=(Flags other) noexcept -> Flags&
	{
		m_data ^= other.m_data;
		return *this;
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator^=(E other) noexcept -> Flags&
	{
		m_data ^= I(other);
		return *this;
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator^=(I mask) noexcept -> Flags&
	{
		m_data ^= mask;
		return *this;
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator==(Flags other) const noexcept -> bool
	{
		return m_data == other.m_data;
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator==(E other) const noexcept -> bool
	{
		return m_data == I(other);
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator!=(Flags other) const noexcept -> bool
	{
		return m_data != other.m_data;
	}

	template <EnumType E>
	constexpr auto Flags<E>::operator!=(E other) const noexcept -> bool
	{
		return m_data != I(other);
	}

	template <EnumType E>
	constexpr Flags<E>::operator E() const noexcept
	{
		return E(m_data);
	}

	template <EnumType E>
	constexpr Flags<E>::operator I() const noexcept
	{
		return m_data;
	}

	template <EnumType E>
	constexpr Flags<E>::operator bool() const noexcept
	{
		return m_data != 0;
	}

	template <EnumType E>
	constexpr auto Flags<E>::Set(E val, bool set) noexcept -> void
	{
		if (set)
			m_data |= I(val);
		else
			m_data &= ~I(val);
	}

	template <EnumType E>
	constexpr auto Flags<E>::IsSet(E val) const noexcept -> bool
	{
		return m_data & I(val);
	}

	template <EnumType E>
	constexpr auto Flags<E>::IsSet(Flags flags) const noexcept -> bool
	{
		return m_data & flags.m_data;
	}

	template <EnumType E>
	constexpr auto Flags<E>::None() const noexcept -> bool
	{
		return !m_data;
	}

	template <EnumType E>
	constexpr auto Flags<E>::Any() const noexcept -> bool
	{
		return !!m_data;
	}

	template <EnumType E>
	constexpr auto Flags<E>::All() const noexcept -> bool
	{
		if constexpr (Detail::EnumFlagHasAllVal<E>)
			return m_data == I(E::All);
		else
			return m_data == ~I(0);
	}

	template <typename E>
	constexpr auto Hash<Flags<E>>::operator()(const Flags<E>& flags) const noexcept -> u64
	{
		using I = UnderlyingType<E>;
		return Hash<I>{}(I(flags));
	}
}
