#pragma once

#if __RESHARPER__
#include "Vec2.h"
#endif

#include "Constants.h"
#include "core/Assert.h"

namespace Onca::Math
{
	template <Numeric Ty>
	constexpr Vec2<Ty>::Vec2() noexcept
		: x(Ty(0)), y(Ty(0))
	{
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr Vec2<Ty>::Vec2(A val) noexcept
		: x(Ty(val)), y(Ty(val))
	{
	}

	//template <Numeric Ty>
	//template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
	//constexpr Vec2<Ty>::Vec2(A x, B y) noexcept
	//	: x(Ty(x)), y(Ty(y))
	//{
	//}
	
	template <Numeric Ty>
	constexpr Vec2<Ty>::Vec2(const Vec2& other) noexcept
		: x(other.x), y(other.y)
	{
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr Vec2<Ty>::Vec2(const Vec2<A>& other) noexcept
		: x(static_cast<Ty>(other.x)), y(static_cast<Ty>(other.y))
	{
	}

	template <Numeric Ty>
	constexpr Vec2<Ty>::Vec2(Vec2&& other) noexcept
		: x(Move(other.x)), y(Move(other.y))
	{
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator=(const Vec2<A>& other) noexcept -> Vec2&
	{
		x = Ty(other.x);
		y = Ty(other.y);
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator=(Vec2&& other) noexcept -> Vec2&
	{
		x = Move(other.x);
		y = Move(other.y);
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator++() noexcept -> Vec2&
	{
		++x;
		++y;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator++(int) noexcept -> Vec2
	{
		Vec2 tmp = *this;
		operator++();
		return tmp;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator--() noexcept -> Vec2&
	{
		--x;
		--y;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator--(int) noexcept -> Vec2
	{
		Vec2 tmp = *this;
		operator--();
		return tmp;
	}
	
	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator+() const noexcept -> Vec2
	{
		return { +x, +y };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator-() const noexcept -> Vec2
	{
		return { -x, -y };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator~() const noexcept -> Vec2 requires Invertable<Ty>
	{
		return { ~x, ~y };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator=(const Vec2& other) noexcept -> Vec2&
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator+(const Vec2& other) const noexcept -> Vec2
	{
		return { x + other.x, y + other.y };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator-(const Vec2& other) const noexcept -> Vec2
	{
		return { x - other.x, y - other.y };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator*(const Vec2& other) const noexcept -> Vec2
	{
		return { x * other.x, y * other.y };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator/(const Vec2& other) const noexcept -> Vec2
	{
		return { x / other.x, y / other.y };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator%(const Vec2& other) const noexcept -> Vec2 requires HasModulo<Ty>
	{
		return { x % other.x, y % other.y };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator|(const Vec2& other) const noexcept -> Vec2 requires Orable<Ty>
	{
		return { x | other.x, y | other.y };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator^(const Vec2& other) const noexcept -> Vec2 requires Xorable<Ty>
	{
		return { x ^ other.x, y ^ other.y };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator&(const Vec2& other) const noexcept -> Vec2 requires Andable<Ty>
	{
		return { x & other.x, y & other.y };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator<<(const Vec2& other) const noexcept -> Vec2 requires BitShiftable<Ty>
	{
		return { x << other.x, y << other.y };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator>>(const Vec2& other) const noexcept -> Vec2 requires BitShiftable<Ty>
	{
		return { x >> other.x, y >> other.y };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator+(A val) const noexcept -> Vec2
	{
		Ty t = static_cast<Ty>(val);
		return { x + t, y + t };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator-(A val) const noexcept -> Vec2
	{
		Ty t = static_cast<Ty>(val);
		return { x - t, y - t };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator*(A val) const noexcept -> Vec2
	{
		Ty t = static_cast<Ty>(val);
		return { x * t, y * t };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator/(A val) const noexcept -> Vec2
	{
		Ty t = static_cast<Ty>(val);
		return { x / t, y / t };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator%(A val) const noexcept -> Vec2 requires HasModulo<Ty>
	{
		Ty t = static_cast<Ty>(val);
		return { x % t, y % t };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator|(A val) const noexcept -> Vec2 requires Orable<Ty>
	{
		Ty t = static_cast<Ty>(val);
		return { x | t, y | t };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator^(A val) const noexcept -> Vec2 requires Xorable<Ty>
	{
		Ty t = static_cast<Ty>(val);
		return { x ^ t, y ^ t };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator&(A val) const noexcept -> Vec2 requires Andable<Ty>
	{
		Ty t = static_cast<Ty>(val);
		return { x & t, y & t };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator<<(A val) const noexcept -> Vec2 requires BitShiftable<Ty>
	{
		Ty t = static_cast<Ty>(val);
		return { x << t, y << t };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator>>(A val) const noexcept -> Vec2 requires BitShiftable<Ty>
	{
		Ty t = static_cast<Ty>(val);
		return { x >> t, y >> t };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator+=(const Vec2& other) noexcept -> Vec2&
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator-=(const Vec2& other) noexcept -> Vec2&
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator*=(const Vec2& other) noexcept -> Vec2&
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator/=(const Vec2& other) noexcept -> Vec2&
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator%=(const Vec2& other) noexcept -> Vec2& requires HasModulo<Ty>
	{
		x %= other.x;
		y %= other.y;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator|=(const Vec2& other) noexcept -> Vec2& requires Orable<Ty>
	{
		x |= other.x;
		y |= other.y;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator^=(const Vec2& other) noexcept -> Vec2& requires Xorable<Ty>
	{
		x ^= other.x;
		y ^= other.y;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator&=(const Vec2& other) noexcept -> Vec2& requires Andable<Ty>
	{
		x &= other.x;
		y &= other.y;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator<<=(const Vec2& other) noexcept -> Vec2& requires BitShiftable<Ty>
	{
		x <<= other.x;
		y <<= other.y;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator>>=(const Vec2& other) noexcept -> Vec2& requires BitShiftable<Ty>
	{
		x >>= other.x;
		y >>= other.y;
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator+=(A val) noexcept -> Vec2&
	{
		Ty t = static_cast<Ty>(val);
		x += t;
		y += t;
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator-=(A val) noexcept -> Vec2&
	{
		Ty t = static_cast<Ty>(val);
		x -= t;
		y -= t;
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator*=(A val) noexcept -> Vec2&
	{
		Ty t = static_cast<Ty>(val);
		x *= t;
		y *= t;
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator/=(A val) noexcept -> Vec2&
	{
		Ty t = static_cast<Ty>(val);
		x /= t;
		y /= t;
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator%=(A val) noexcept -> Vec2& requires HasModulo<Ty>
	{
		Ty t = static_cast<Ty>(val);
		x %= t;
		y %= t;
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator|=(A val) noexcept -> Vec2& requires Orable<Ty>
	{
		Ty t = static_cast<Ty>(val);
		x |= t;
		y |= t;
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator^=(A val) noexcept -> Vec2& requires Xorable<Ty>
	{
		Ty t = static_cast<Ty>(val);
		x ^= t;
		y ^= t;
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator&=(A val) noexcept -> Vec2& requires Andable<Ty>
	{
		Ty t = static_cast<Ty>(val);
		x &= t;
		y &= t;
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator<<=(A val) noexcept -> Vec2& requires BitShiftable<Ty>
	{
		Ty t = static_cast<Ty>(val);
		x <<= t;
		y <<= t;
		return *this;
	} 

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::operator>>=(A val) noexcept -> Vec2& requires BitShiftable<Ty>
	{
		Ty t = static_cast<Ty>(val);
		x >>= t;
		y >>= t;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator[](usize idx) noexcept -> Ty&
	{
		MATH_ASSERT(idx < 2, "Index out of range");
		return data[idx];
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator[](usize idx) const noexcept -> Ty
	{
		MATH_ASSERT(idx < 2, "Index out of range");
		return data[idx];
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator==(const Vec2& other) const noexcept -> bool
	{
		return x == other.x && y == other.y;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::operator!=(const Vec2& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Compare(const Vec2& other, Ty e) const noexcept -> bool
	{
		return Math::EpsilonCompare(x, other.x, e) && Math::EpsilonCompare(y, other.y, e);
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::IsZero() const noexcept -> bool
	{
		return x == Ty(0) && y == Ty(0);
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::IsNearlyZero(Ty e) const noexcept -> bool
	{
		return Math::EpsilonCompare(x, Ty(0), e) && Math::EpsilonCompare(y, Ty(0), e);
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::IsUniform(Ty e) const noexcept -> bool
	{
		return Math::EpsilonCompare(x, y, e);
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::MinComponent() const noexcept -> Ty
	{
		return Math::Min(x, y);
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::MinAbsComponent() const noexcept -> Ty
	{
		return Abs().MinComponent();
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::MaxComponent() const noexcept -> Ty
	{
		return Math::Max(x, y);
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::MaxAbsComponent() const noexcept -> Ty
	{
		return Abs().MaxComponent();
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Min(const Vec2& other) const noexcept -> Vec2
	{
		return { Math::Min(x, other.x), Math::Min(y, other.y) };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Max(const Vec2& other) const noexcept -> Vec2
	{
		return { Math::Max(x, other.x), Math::Max(y, other.y) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
	constexpr auto Vec2<Ty>::Clamp(A min, B max) noexcept -> Vec2&
	{
		x = Math::Clamp(x, min, max);
		y = Math::Clamp(y, min, max);
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Clamp(const Vec2& min, const Vec2& max) noexcept -> Vec2&
	{
		x = Math::Clamp(x, min.x, max.x);
		y = Math::Clamp(y, min.y, max.y);
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
	constexpr auto Vec2<Ty>::Clamped(A min, B max) const noexcept -> Vec2
	{
		return { Math::Clamp(x, Ty(min), Ty(max)), Math::Clamp(y, Ty(min), Ty(max)) };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Clamped(const Vec2& min, const Vec2& max) const noexcept -> Vec2
	{
		return { Math::Clamp(x, min.x, max.x), Math::Clamp(y, min.y, max.y) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
	constexpr auto Vec2<Ty>::ClampLen(A min, B max) noexcept -> Vec2&
	{
		Ty minSq = min * min;
		Ty maxSq = max * max;
		Ty lenSq = LenSq();
		if (lenSq < minSq || lenSq > maxSq)
		{
			Ty len = lenSq < minSq ? min : max;
			*this *= Math::RSqrt(lenSq) * len;
		}
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
	constexpr auto Vec2<Ty>::ClampedLen(A min, B max) const noexcept -> Vec2
	{
		Ty minSq = min * min;
		Ty maxSq = max * max;
		Ty lenSq = LenSq();
		if (lenSq >= minSq && lenSq <= maxSq)
			return *this;

		Ty len = lenSq < minSq ? min : max;
		return *this * Math::RSqrt(lenSq) * len;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Saturate() noexcept -> Vec2&
	{
		return Clamp(0, 1);
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Saturated() const noexcept -> Vec2
	{
		return Clamped(0, 1);
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Snap(Ty stepSize) noexcept -> Vec2&
	{
		*this = Snapped(stepSize);
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Snapped(Ty stepSize) const noexcept -> Vec2
	{
		return (*this / stepSize).Round() * stepSize;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::Lerp(const Vec2& other, A i) const noexcept -> Vec2
	{
		return *this * (Ty(1) - Ty(i)) + other * Ty(i);
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::LenSq() const noexcept -> Ty
	{
		return x * x + y * y;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Len() const noexcept -> Ty
	{
		return Math::Sqrt(LenSq());
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::DistanceSq(const Vec2& other) const noexcept -> Ty
	{
		return (*this - other).LenSq();
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Distance(const Vec2& other) const noexcept -> Ty
	{
		return (*this - other).Len();
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Dot(const Vec2& other) const noexcept -> Ty
	{
		return x * other.x + y * other.y;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Cross(const Vec2& other) const noexcept -> Ty
	{
		return x * other.y - y * other.x;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::PerpendicularCW() const noexcept -> Vec2
	{
		return { y, -x };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::PerpendicularCCW() const noexcept -> Vec2
	{
		return { -y, x };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Normalize() noexcept -> Vec2&
	{
		Ty lenSq = LenSq();
		if (lenSq > Consts::MathEpsilon<Ty>)
			*this *= Math::RSqrt(LenSq());
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::NormalizeUnsafe() noexcept -> Vec2&
	{
		*this *= Math::RSqrt(LenSq());
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::NormalizeSafe(const Vec2& safe) noexcept -> Vec2&
	{
		Ty lenSq = LenSq();
		if (lenSq > Consts::MathEpsilon<Ty>)
			*this *= Math::RSqrt(LenSq());
		else
			*this = {};
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Normalized() const noexcept -> Vec2
	{
		return *this * Math::RSqrt(LenSq());
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::IsNormalized(Ty e) const noexcept -> bool
	{
		return Math::EpsilonCompare(LenSq(), Ty(1), e);
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, AngleKind Kind>
	constexpr auto Vec2<Ty>::Rotate(const Math::Angle<A, Kind>& angle) noexcept -> Vec2&
	{
		Ty sn = Ty(Sin(angle));
		Ty cs = Ty(Cos(angle));

		Ty tmpX = x * cs - y * sn;
		Ty tmpY = x * sn + y * cs;
		x = tmpX;
		y = tmpY;
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, AngleKind Kind>
	constexpr auto Vec2<Ty>::Rotated(const Math::Angle<A, Kind>& angle) const noexcept -> Vec2
	{
		Ty sn = Ty(Sin(angle));
		Ty cs = Ty(Cos(angle));

		return { x * cs - y * sn, x * sn + y * cs };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Angle() const noexcept -> Math::Angle<Ty, AngleKind::Radians>
	{
		if (Math::EpsilonCompare(x, 0) && Math::EpsilonCompare(y, 0))
			return { 0 };
		return ATan2(y, x);
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Angle(const Vec2& other) const noexcept -> Math::Angle<Ty, AngleKind::Radians>
	{
		Ty acos = Dot(other) / (Len() * other.Len());
		return ACos(acos);
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::AngleFull(const Vec2& other) const noexcept -> Math::Angle<Ty, AngleKind::Radians>
	{
		Ty acos = Dot(other) / (Len() * other.Len());
		Math::Angle<Ty, AngleKind::Radians> angle = ACos(acos);

		Ty cross = Cross(other);
		return cross < 0 ? Math::Angle<Ty, AngleKind::Radians>{ Consts::TwoPi<Ty> - angle.val } : angle;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::FaceForward(const Vec2& incidence, const Vec2& normal) const noexcept -> Vec2
	{
		return incidence.Dot(normal) < 0 ? *this : -*this;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Reflect(const Vec2& normal) const noexcept -> Vec2
	{
		MATH_ASSERT(normal.IsNormalized(), "normal needs to be normalized");
		return *this - normal * Dot(normal) * Ty(2);
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec2<Ty>::Refract(const Vec2& normal, A eta) const noexcept -> Vec2
	{
		MATH_ASSERT(normal.IsNormalized(), "normal needs to be normalized");

		Ty cosi = Dot(normal);
		MATH_ASSERT(cosi < 0, "vector should move into the surface (this vector should be in the opposite direction of the normal)");
		
		Ty k = Ty(1) - eta * eta * (Ty(1) - cosi * cosi);
		return k >= 0 ? (*this * eta - normal * (eta * cosi + Math::Sqrt(k))) : Vec2{};
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::DirectionAndLength() const noexcept -> Pair<Vec2, Ty>
	{
		Ty len = Len();
		Vec2 dir = len >= Consts::MathEpsilon<Ty> ? *this / len : Vec2{};
		return { dir, len };
	}

	template <Numeric Ty>
	template <usize Idx0, usize Idx1>
	constexpr auto Vec2<Ty>::Swizzle() const noexcept -> Vec2
	{
		Vec2 res;
		switch (Idx0)
		{
		case X: res.x = x; break;
		case Y: res.x = y; break;
		default: break;
		}
		switch (Idx1)
		{
		case X: res.y = x; break;
		case Y: res.y = y; break;
		default: break;
		}
		return res;
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Swizzle(u8 idx0, u8 idx1) const noexcept -> Vec2
	{
		Vec2 res;
		switch (idx0)
		{
		case X: res.x = x; break;
		case Y: res.x = y; break;
		default: break;
		}
		switch (idx1)
		{
		case X: res.y = x; break;
		case Y: res.y = y; break;
		default: break;
		}
		return res;
	}

#define SWIZZLE_IMPL(xy, uv, st, x, y) \
	template<Numeric Ty> constexpr auto Vec2<Ty>::xy() const noexcept -> Vec2<Ty> { return { x, y }; } \
	template<Numeric Ty> constexpr auto Vec2<Ty>::uv() const noexcept -> Vec2<Ty> { return { x, y }; } \
	template<Numeric Ty> constexpr auto Vec2<Ty>::st() const noexcept -> Vec2<Ty> { return { x, y }; }

	SWIZZLE_IMPL(xx, uu, ss, x, x)
	SWIZZLE_IMPL(xy, uv, st, x, y)
	SWIZZLE_IMPL(yx, vu, ts, y, x)
	SWIZZLE_IMPL(yy, vv, tt, y, y)

#undef SWIZZLE_IMPL

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Abs() const noexcept -> Vec2
	{
		return { Math::Abs(x), Math::Abs(y) };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Ceil() const noexcept -> Vec2
	{
		return { Math::Ceil(x), Math::Ceil(y) };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Floor() const noexcept -> Vec2
	{
		return { Math::Floor(x), Math::Floor(y) };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Trunc() const noexcept -> Vec2
	{
		return { Math::Trunc(x), Math::Trunc(y) };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Round() const noexcept -> Vec2
	{
		return { Math::Round(x), Math::Round(y) };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::RoundEven() const noexcept -> Vec2
	{
		return { Math::RoundEven(x), Math::RoundEven(y) };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Sign() const noexcept -> Vec2
	{
		return { Math::Sign(x), Math::Sign(y) };
	}

	template <Numeric Ty>
	constexpr auto Vec2<Ty>::Fract() const noexcept -> Vec2
	{
		return { Math::Fract(x), Math::Fract(y) };
	}

	template <Numeric T, ConvertableTo<T> A>
	constexpr auto operator*(A t, const Vec2<T>& vec) noexcept -> Vec2<T>
	{
		return vec * t;
	}
}
