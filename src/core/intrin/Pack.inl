#pragma once
#if __RESHARPER__
#include "Pack.h"
#endif

namespace Core::Intrin
{
	template <SimdBaseType T, usize Width>
	constexpr Pack<T, Width>::Pack() noexcept
	{
		*this = Zero();
	}


	template <SimdBaseType T, usize Width>
	constexpr Pack<T, Width>::Pack(UnInitTag) noexcept
	{
	}

	template <SimdBaseType T, usize Width>
	constexpr Pack<T, Width>::Pack(T val) noexcept
	{
		*this = Set(val);
	}

	template <SimdBaseType T, usize Width>
	template <SameAs<T> ... Args>
	constexpr Pack<T, Width>::Pack(Args... vals) noexcept
	{
		*this = Set(vals...);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator+() const noexcept -> Pack
	{
		return Pack(data);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator-() const noexcept -> Pack
	{
		return Neg();
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator~() const noexcept -> Pack
	{
		return Not();
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator+(const Pack& other) const noexcept -> Pack
	{
		return Add(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator-(const Pack& other) const noexcept -> Pack
	{
		return Sub(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator*(const Pack& other) const noexcept -> Pack
	{
		return Mul(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator/(const Pack& other) const noexcept -> Pack
	{
		return Div(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator&(const Pack& other) const noexcept -> Pack
	{
		return And(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator^(const Pack& other) const noexcept -> Pack
	{
		return Xor(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator|(const Pack& other) const noexcept -> Pack
	{
		return Or(other);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator>>(const Pack& count) const noexcept -> Pack
	{
		return ShiftL(count);
	}

	template <SimdBaseType T, usize Width>
	template <UnsignedIntegral U>
	constexpr auto Pack<T, Width>::operator>>(U count) const noexcept -> Pack
	{
		return ShiftL(count);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator<<(const Pack& count) const noexcept -> Pack
	{
		if constexpr (UnsignedIntegral<T>)
			return ShiftRL(count);
		else
			return ShiftRA(count);
	}

	template <SimdBaseType T, usize Width>
	template <UnsignedIntegral U>
	constexpr auto Pack<T, Width>::operator<<(U count) const noexcept -> Pack
	{
		if constexpr (UnsignedIntegral<T>)
			return ShiftRL(count);
		else
			return ShiftRA(count);
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator+=(const Pack& other) noexcept -> Pack&
	{
		*this = *this + other;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator-=(const Pack& other) noexcept -> Pack&
	{
		*this = *this - other;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator*=(const Pack& other) noexcept -> Pack&
	{
		*this = *this * other;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator/=(const Pack& other) noexcept -> Pack&
	{
		*this = *this / other;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator&=(const Pack& other) noexcept -> Pack&
	{
		*this = *this & other;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator^=(const Pack& other) noexcept -> Pack&
	{
		*this = *this ^ other;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator|=(const Pack& other) noexcept -> Pack&
	{
		*this = *this | other;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator>>=(const Pack& count) const noexcept -> Pack&
	{
		*this = *this >> count;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	template <UnsignedIntegral U>
	constexpr auto Pack<T, Width>::operator>>=(U count) const noexcept -> Pack&
	{
		*this = *this >> count;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	constexpr auto Pack<T, Width>::operator<<=(const Pack& count) const noexcept -> Pack&
	{
		*this = *this << count;
		return *this;
	}

	template <SimdBaseType T, usize Width>
	template <UnsignedIntegral U>
	constexpr auto Pack<T, Width>::operator<<=(U count) const noexcept -> Pack&
	{
		*this = *this << count;
		return *this;
	}
}
