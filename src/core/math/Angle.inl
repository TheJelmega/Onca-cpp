#pragma once
#include "Angle.h"

#include "Constants.h"

namespace Core::Math
{
	template <Numeric T, AngleKind Kind>
	constexpr Angle<T, Kind>::Angle() noexcept
		: val(T(0))
	{
	}

	template <Numeric T, AngleKind A>
	template<ConvertableTo<T> U>
	constexpr Angle<T, A>::Angle(U val) noexcept
		: val(T(val))
	{
	}

	template <Numeric T, AngleKind A>
	constexpr Angle<T, A>::Angle(const Angle& angle) noexcept
		: val(angle.val)
	{
	}

	template <Numeric T, AngleKind Kind>
	template <Numeric U, AngleKind A>
	constexpr Angle<T, Kind>::Angle(const Angle<U, A>& angle) noexcept
	{
		if constexpr (Kind == A)
			val = T(angle.val);
		else if constexpr (A == AngleKind::Degrees)
			val = T(angle.val * Consts::Deg2Rad<T>);
		else
			val = T(angle.val * Consts::Rad2Deg<T>);
	}

	template <Numeric T, AngleKind A>
	constexpr Angle<T, A>::Angle(Angle&& angle) noexcept
		: val(angle.val)
	{
	}

	template <Numeric T, AngleKind Kind>
	template <ConvertableTo<T> U>
	constexpr auto Angle<T, Kind>::operator=(U val) noexcept -> Angle&
	{
		this->val = T(val);
		return *this;
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Angle<T, Kind>::operator=(const Angle& other) noexcept -> Angle&
	{
		val = other.val;
		return *this;
	}

	template <Numeric T, AngleKind Kind>
	template <Numeric U, AngleKind A>
	constexpr auto Angle<T, Kind>::operator=(const Angle<U, A>& other) noexcept -> Angle&
	{
		if constexpr (Kind == A)
			val = T(other.val);
		else if constexpr (A == AngleKind::Degrees)
			val = T(other.val * Consts::Deg2Rad<T>);
		else
			val = T(other.val * Consts::Rad2Deg<T>);
		return *this;
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Angle<T, Kind>::operator=(Angle&& other) noexcept -> Angle&
	{
		val = other.val;
		return *this;
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Angle<T, Kind>::operator+() const noexcept -> Angle
	{
		return *this;
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Angle<T, Kind>::operator-() const noexcept -> Angle
	{
		return Angle{ -val };
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Angle<T, Kind>::operator+(const Angle& other) const noexcept -> Angle
	{
		return Angle{ val + other.val };
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Angle<T, Kind>::operator-(const Angle& other) const noexcept -> Angle
	{
		return Angle{ val - other.val };
	}

	template <Numeric T, AngleKind Kind>
	template <ConvertableTo<T> U>
	constexpr auto Angle<T, Kind>::operator*(U other) const noexcept -> Angle
	{
		return Angle{ val * other };
	}

	template <Numeric T, AngleKind Kind>
	template <ConvertableTo<T> U>
	constexpr auto Angle<T, Kind>::operator/(U other) const noexcept -> Angle
	{
		return Angle{ val / other };
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Angle<T, Kind>::operator+=(const Angle& other) noexcept -> Angle&
	{
		val += other.val;
		return *this;
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Angle<T, Kind>::operator-=(const Angle& other) noexcept -> Angle&
	{
		val -= other.val;
		return *this;
	}

	template <Numeric T, AngleKind Kind>
	template <ConvertableTo<T> U>
	constexpr auto Angle<T, Kind>::operator*=(U other) noexcept -> Angle&
	{
		val *= other;
		return *this;
	}

	template <Numeric T, AngleKind Kind>
	template <ConvertableTo<T> U>
	constexpr auto Angle<T, Kind>::operator/=(U other) noexcept -> Angle&
	{
		val /= other;
		return *this;
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Angle<T, Kind>::operator==(const Angle& other) const noexcept -> bool
	{
		return val == other.val;
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Angle<T, Kind>::operator!=(const Angle& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Angle<T, Kind>::Compare(const Angle& other, T e) const noexcept -> bool
	{
		return EpsilonCompare(val, other.val, e);
	}

	template <Numeric T, AngleKind A>
	constexpr auto Angle<T, A>::Mod() noexcept -> Angle&
	{
		constexpr T max = A == AngleKind::Degrees ? T(360) : Consts::TwoPi<T>;
		bool neg = val < 0;
		val = Math::Mod(val, max);
		if (neg && val > 0)
			val -= max;
		return *this;
	}

	template <Numeric T, AngleKind Kind>
	constexpr auto Angle<T, Kind>::GetBoundedVal() const noexcept -> T
	{
		constexpr T max = Kind == AngleKind::Degrees ? T(360) : Consts::TwoPi<T>;
		T tmp = Math::Mod(val, max);
		if (val < 0 && tmp > 0)
			tmp -= max;
		return tmp;
	}

	template <Numeric T, AngleKind A>
	constexpr auto Angle<T, A>::ToDegrees() const noexcept -> Angle<T, AngleKind::Degrees>
	{
		return Angle<T, AngleKind::Degrees>{ *this };
	}

	template <Numeric T, AngleKind A>
	constexpr auto Angle<T, A>::ToRadians() const noexcept -> Angle<T, AngleKind::Radians>
	{
		return Angle<T, AngleKind::Radians>{ *this };
	}

	template <Numeric T, ConvertableTo<T> U, AngleKind Kind>
	constexpr auto operator*(U val, const Angle<T, Kind>& angle) noexcept -> Angle<T, Kind>
	{
		return angle * val;
	}
}

inline namespace Literals
{
	constexpr auto operator""_deg(long double val) noexcept -> Core::Math::Angle<f64, Core::Math::AngleKind::Degrees>
	{
		return { val };
	}

	constexpr auto operator""_deg(unsigned long long int val) noexcept -> Core::Math::Angle<f64, Core::Math::AngleKind::Degrees>
	{
		return { val };
	}

	constexpr auto operator""_rad(long double val) noexcept -> Core::Math::Angle<f64, Core::Math::AngleKind::Radians>
	{
		return { val };
	}

	constexpr auto operator""_rad(unsigned long long int val) noexcept -> Core::Math::Angle<f64, Core::Math::AngleKind::Radians>
	{
		return { val };
	}
}