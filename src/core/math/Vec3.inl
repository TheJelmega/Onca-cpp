#pragma once

#if __RESHARPER__
#include "Vec3.h"
#endif

#include "Constants.h"
#include "core/Assert.h"

namespace Core::Math
{
	template <Numeric Ty>
	constexpr Vec3<Ty>::Vec3() noexcept
		: x(Ty(0))
		, y(Ty(0))
		, z(Ty(0))
	{
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr Vec3<Ty>::Vec3(A val) noexcept
		: x(Ty(val))
		, y(Ty(val))
		, z(Ty(val))
	{
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, ConvertableTo<Ty> B, ConvertableTo<Ty> C>
	constexpr Vec3<Ty>::Vec3(A x, B y, C z) noexcept
		: x(Ty(x))
		, y(Ty(y))
		, z(Ty(z))
	{
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
	constexpr Vec3<Ty>::Vec3(const Vec2<A>& vec, B z)
		: x(Ty(vec.x))
		, y(Ty(vec.y))
		, z(Ty(z))
	{
	}

	template <Numeric Ty>
	constexpr Vec3<Ty>::Vec3(const Vec3& other) noexcept
		: x(other.x)
		, y(other.y)
		, z(other.z)
	{
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr Vec3<Ty>::Vec3(const Vec3<A>& other) noexcept
		: x(Ty(other.x))
		, y(Ty(other.y))
		, z(Ty(other.z))
	{
	}

	template <Numeric Ty>
	constexpr Vec3<Ty>::Vec3(Vec3&& other) noexcept
		: x(other.x)
		, y(other.y)
		, z(other.z)
	{
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator=(const Vec3& other) noexcept -> Vec3&
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator=(const Vec3<A>& other) noexcept -> Vec3&
	{
		x = Ty(other.x);
		y = Ty(other.y);
		z = Ty(other.z);
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator=(Vec3&& other) noexcept -> Vec3&
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator++() noexcept -> Vec3&
	{
		++x;
		++y;
		++z;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator++(int) noexcept -> Vec3
	{
		Vec3 tmp = *this;
		operator++();
		return tmp;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator--() noexcept -> Vec3&
	{
		--x;
		--y;
		--z;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator--(int) noexcept -> Vec3
	{
		Vec3 tmp = *this;
		operator--();
		return tmp;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator+() const noexcept -> Vec3
	{
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator-() const noexcept -> Vec3
	{
		return { -x, -y, -z };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator~() const noexcept -> Vec3 requires Invertable<Ty>
	{
		return { ~x, ~y, ~z };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator+(const Vec3& other) const noexcept -> Vec3
	{
		return { x + other.x, y + other.y, z + other.z };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator-(const Vec3& other) const noexcept -> Vec3
	{
		return { x - other.x, y - other.y, z - other.z };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator*(const Vec3& other) const noexcept -> Vec3
	{
		return { x * other.x, y * other.y, z * other.z };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator/(const Vec3& other) const noexcept -> Vec3
	{
		return { x / other.x, y / other.y, z / other.z };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator%(const Vec3& other) const noexcept -> Vec3 requires HasModulo<Ty>
	{
		return { x % other.x, y % other.y, z % other.z };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator|(const Vec3& other) const noexcept -> Vec3 requires Orable<Ty>
	{
		return { x | other.x, y | other.y, z | other.z };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator^(const Vec3& other) const noexcept -> Vec3 requires Xorable<Ty>
	{
		return { x ^ other.x, y ^ other.y, z ^ other.z };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator&(const Vec3& other) const noexcept -> Vec3 requires Andable<Ty>
	{
		return { x & other.x, y & other.y, z & other.z };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator<<(const Vec3& other) const noexcept -> Vec3 requires BitShiftable<Ty>
	{
		return { x << other.x, y << other.y, z << other.z };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator>>(const Vec3& other) const noexcept -> Vec3 requires BitShiftable<Ty>
	{
		return { x >> other.x, y >> other.y, z >> other.z };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator+(A val) const noexcept -> Vec3
	{
		return { x + Ty(val), y + Ty(val), z + Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator-(A val) const noexcept -> Vec3
	{
		return { x - Ty(val), y - Ty(val), z - Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator*(A val) const noexcept -> Vec3
	{
		return { x * Ty(val), y * Ty(val), z * Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator/(A val) const noexcept -> Vec3
	{
		return { x / Ty(val), y / Ty(val), z / Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator%(A val) const noexcept -> Vec3 requires HasModulo<Ty>
	{
		return { x % Ty(val), y % Ty(val), z % Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator|(A val) const noexcept -> Vec3 requires Orable<Ty>
	{
		return { x | Ty(val), y | Ty(val), z | Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator^(A val) const noexcept -> Vec3 requires Xorable<Ty>
	{
		return { x ^ Ty(val), y ^ Ty(val), z ^ Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator&(A val) const noexcept -> Vec3 requires Andable<Ty>
	{
		return { x & Ty(val), y & Ty(val), z & Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator<<(A val) const noexcept -> Vec3 requires BitShiftable<Ty>
	{
		return { x << Ty(val), y << Ty(val), z << Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator>>(A val) const noexcept -> Vec3 requires BitShiftable<Ty>
	{
		return { x >> Ty(val), y >> Ty(val), z >> Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator*(const Quaternion<A>& quat) const noexcept -> Vec3
	{
		return {
			x * quat.w + y * quat.z - z * quat.y,
			y * quat.w + z * quat.x - x * quat.z,
			z * quat.w + x * quat.y - y * quat.x
		};
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator+=(const Vec3& other) noexcept -> Vec3&
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator-=(const Vec3& other) noexcept -> Vec3&
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator*=(const Vec3& other) noexcept -> Vec3&
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator/=(const Vec3& other) noexcept -> Vec3&
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator%=(const Vec3& other) noexcept -> Vec3& requires HasModulo<Ty>
	{
		x %= other.x;
		y %= other.y;
		z %= other.z;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator|=(const Vec3& other) noexcept -> Vec3& requires Orable<Ty>
	{
		x |= other.x;
		y |= other.y;
		z |= other.z;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator^=(const Vec3& other) noexcept -> Vec3& requires Xorable<Ty>
	{
		x ^= other.x;
		y ^= other.y;
		z ^= other.z;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator&=(const Vec3& other) noexcept -> Vec3& requires Andable<Ty>
	{
		x &= other.x;
		y &= other.y;
		z &= other.z;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator<<=(const Vec3& other) noexcept -> Vec3& requires BitShiftable<Ty>
	{
		x <<= other.x;
		y <<= other.y;
		z <<= other.z;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator>>=(const Vec3& other) noexcept -> Vec3& requires BitShiftable<Ty>
	{
		x >>= other.x;
		y >>= other.y;
		z >>= other.z;
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator+=(A val) noexcept -> Vec3&
	{
		x += Ty(val);
		y += Ty(val);
		z += Ty(val);
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator-=(A val) noexcept -> Vec3&
	{
		x -= Ty(val);
		y -= Ty(val);
		z -= Ty(val);
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator*=(A val) noexcept -> Vec3&
	{
		x *= Ty(val);
		y *= Ty(val);
		z *= Ty(val);
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator/=(A val) noexcept -> Vec3&
	{
		x /= Ty(val);
		y /= Ty(val);
		z /= Ty(val);
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator%=(A val) noexcept -> Vec3& requires HasModulo<Ty>
	{
		x %= Ty(val);
		y %= Ty(val);
		z %= Ty(val);
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator|=(A val) noexcept -> Vec3& requires Orable<Ty>
	{
		x |= Ty(val);
		y |= Ty(val);
		z |= Ty(val);
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator^=(A val) noexcept -> Vec3& requires Xorable<Ty>
	{
		x ^= Ty(val);
		y ^= Ty(val);
		z ^= Ty(val);
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator&=(A val) noexcept -> Vec3& requires Andable<Ty>
	{
		x &= Ty(val);
		y &= Ty(val);
		z &= Ty(val);
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator<<=(A val) noexcept -> Vec3& requires BitShiftable<Ty>
	{
		x <<= Ty(val);
		y <<= Ty(val);
		z <<= Ty(val);
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator>>=(A val) noexcept -> Vec3& requires BitShiftable<Ty>
	{
		x >>= Ty(val);
		y >>= Ty(val);
		z >>= Ty(val);
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::operator*=(const Quaternion<A>& quat) noexcept -> Vec3&
	{
		*this = *this * quat;
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator[](usize idx) noexcept -> Ty&
	{
		MATH_ASSERT(idx < 3, "Index out of range");
		return data[idx];
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator[](usize idx) const noexcept -> Ty
	{
		MATH_ASSERT(idx < 3, "Index out of range");
		return data[idx];
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator==(const Vec3& other) const noexcept -> bool
	{
		return x == other.x && y == other.y && z == other.z;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::operator!=(const Vec3& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Compare(const Vec3& other, Ty e) const noexcept -> bool
	{
		return Math::EpsilonCompare(x, other.x, e) && Math::EpsilonCompare(y, other.y, e) && Math::EpsilonCompare(z, other.z, e);
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::IsZero() const noexcept -> bool
	{
		return x == Ty(0) && y == Ty(0) && z == Ty(0);
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::IsNearlyZero(Ty e) const noexcept -> bool
	{
		return Math::EpsilonCompare(x, Ty(0), e) && Math::EpsilonCompare(y, Ty(0), e) && Math::EpsilonCompare(z, Ty(0), e);
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::IsUniform(Ty e) const noexcept -> bool
	{
		return Math::EpsilonCompare(x, y, e), Math::EpsilonCompare(x, z, e);
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::MinComponent() const noexcept -> Ty
	{
		return Math::Min(x, y, z);
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::MinAbsComponent() const noexcept -> Ty
	{
		return Abs().MinComponent();
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::MaxComponent() const noexcept -> Ty
	{
		return Math::Max(x, y, z);
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::MaxAbsComponent() const noexcept -> Ty
	{
		return Abs().MaxComponent();
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Min(const Vec3& other) const noexcept -> Vec3
	{
		return { Math::Min(x, other.x), Math::Min(y, other.y), Math::Min(z, other.z) };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Max(const Vec3& other) const noexcept -> Vec3
	{
		return { Math::Max(x, other.x), Math::Max(y, other.y), Math::Max(z, other.z) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
	constexpr auto Vec3<Ty>::Clamp(A min, B max) noexcept -> Vec3&
	{
		x = Math::Clamp(x, min, max);
		y = Math::Clamp(y, min, max);
		z = Math::Clamp(z, min, max);
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Clamp(const Vec3& min, const Vec3& max) noexcept -> Vec3&
	{
		x = Math::Clamp(x, min.x, max.x);
		y = Math::Clamp(y, min.y, max.y);
		z = Math::Clamp(z, min.z, max.z);
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
	constexpr auto Vec3<Ty>::Clamped(A min, B max) const noexcept -> Vec3
	{
		return { Math::Clamp(x, min, max), Math::Clamp(y, min, max), Math::Clamp(z, min, max) };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Clamped(const Vec3& min, const Vec3& max) const noexcept -> Vec3
	{
		return { Math::Clamp(x, min.x, max.x), Math::Clamp(y, min.y, max.y), Math::Clamp(z, min.z, max.z) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
	constexpr auto Vec3<Ty>::ClampLen(A min, B max) noexcept -> Vec3&
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
	constexpr auto Vec3<Ty>::ClampedLen(A min, B max) const noexcept -> Vec3
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
	constexpr auto Vec3<Ty>::Saturate() noexcept -> Vec3&
	{
		return Clamp(0, 1);
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Saturated() const noexcept -> Vec3
	{
		return Clamped(0, 1);
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Snap(Ty stepSize) noexcept -> Vec3&
	{
		*this = Snapped(stepSize);
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Snapped(Ty stepSize) const noexcept -> Vec3
	{
		return (*this / stepSize).Round() * stepSize;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::Lerp(const Vec3& other, A i) const noexcept -> Vec3
	{
		return *this * (Ty(1) - Ty(i)) + other * Ty(i);
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::LenSq() const noexcept -> Ty
	{
		return x * x + y * y + z * z;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Len() const noexcept -> Ty
	{
		return Math::Sqrt(LenSq());
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::DistanceSq(const Vec3& other) const noexcept -> Ty
	{
		return (*this - other).LenSq();
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Distance(const Vec3& other) const noexcept -> Ty
	{
		return (*this - other).Len();
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Dot(const Vec3& other) const noexcept -> Ty
	{
		return x * other.x + y * other.y + z * other.z;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Cross(const Vec3& other) const noexcept -> Vec3
	{
		return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Normalize() noexcept -> Vec3&
	{
		*this *= Math::RSqrt(LenSq());
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Normalized() const noexcept -> Vec3
	{
		return *this * Math::RSqrt(LenSq());
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::IsNormalized(Ty e) const noexcept -> bool
	{
		return Math::EpsilonCompare(LenSq(), Ty(1), e);
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::Rotate(const Quaternion<A>& quat) noexcept -> Vec3&
	{
		*this = Rotated(quat);
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::Rotated(const Quaternion<A>& quat) const noexcept -> Vec3
	{
		MATH_ASSERT(quat.IsNormalized(), "Can only rotate with a normalized quaternion");

		Vec3 a = quat.imaginary.Cross(*this) + quat.real * *this;
		Vec3 b = quat.imaginary.Cross(a);
		return Ty(2) * b + *this;
	}
	
	template <Numeric Ty>
	constexpr auto Vec3<Ty>::FaceForward(const Vec3& incidence, const Vec3& normal) const noexcept -> Vec3
	{
		return incidence.Dot(normal) < 0 ? *this : -*this;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Reflect(const Vec3& normal) const noexcept -> Vec3
	{
		MATH_ASSERT(normal.IsNormalized(), "normal needs to be normalized");
		return *this - normal * Dot(normal) * Ty(2);
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec3<Ty>::Refract(const Vec3& normal, A eta) const noexcept -> Vec3
	{
		MATH_ASSERT(normal.IsNormalized(), "normal needs to be normalized");

		Ty cosi = Dot(normal);
		MATH_ASSERT(cosi < 0, "vector should move into the surface (this vector should be in the opposite direction of the normal)");

		Ty k = Ty(1) - eta * eta * (Ty(1) - cosi * cosi);
		return k >= 0 ? (*this * eta - normal * (eta * cosi + Math::Sqrt(k))) : Vec3{};
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::DirectionAndLength() const noexcept -> Pair<Vec3, Ty>
	{
		Ty len = Len();
		Vec3 dir = len >= Consts::MathEpsilon<Ty> ? *this / len : Vec3{};
		return { dir, len };
	}

	template <Numeric Ty>
	template <usize Idx0, usize Idx1, usize Idx2>
	constexpr auto Vec3<Ty>::Swizzle() const noexcept -> Vec3
	{
		Vec3 res;
		switch (Idx0)
		{
		case X: res.x = x; break;
		case Y: res.x = y; break;
		case Z: res.x = z; break;
		default: break;
		}
		switch (Idx1)
		{
		case X: res.y = x; break;
		case Y: res.y = y; break;
		case Z: res.y = z; break;
		default: break;
		}
		switch (Idx2)
		{
		case X: res.z = x; break;
		case Y: res.z = y; break;
		case Z: res.z = z; break;
		default: break;
		}
		return res;
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Swizzle(u8 idx0, u8 idx1, u8 idx2) const noexcept -> Vec3
	{
		Vec3 res;
		switch (idx0)
		{
		case X: res.x = x; break;
		case Y: res.x = y; break;
		case Z: res.x = z; break;
		default: break;
		}
		switch (idx1)
		{
		case X: res.y = x; break;
		case Y: res.y = y; break;
		case Z: res.y = z; break;
		default: break;
		}
		switch (idx2)
		{
		case X: res.z = x; break;
		case Y: res.z = y; break;
		case Z: res.z = z; break;
		default: break;
		}
		return res;
	}

#define SWIZZLE_IMPL(xy, uv, st, x, y, z) \
	template<Numeric Ty> constexpr auto Vec3<Ty>::xy() const noexcept -> Vec3<Ty> { return { x, y, z }; } \
	template<Numeric Ty> constexpr auto Vec3<Ty>::uv() const noexcept -> Vec3<Ty> { return { x, y, z }; } \
	template<Numeric Ty> constexpr auto Vec3<Ty>::st() const noexcept -> Vec3<Ty> { return { x, y, z }; }

	SWIZZLE_IMPL(xxx, uuu, sss, x, x, x)
	SWIZZLE_IMPL(xxy, uuv, sst, x, x, y)
	SWIZZLE_IMPL(xxz, uuw, ssq, x, x, z)
	SWIZZLE_IMPL(xyx, uvu, sts, x, y, x)
	SWIZZLE_IMPL(xyy, uvv, stt, x, y, y)
	SWIZZLE_IMPL(xyz, uvw, stq, x, y, z)
	SWIZZLE_IMPL(xzx, uwu, sqs, x, z, x)
	SWIZZLE_IMPL(xzy, uwv, sqt, x, z, y)
	SWIZZLE_IMPL(xzz, uww, sqq, x, z, z)
	SWIZZLE_IMPL(yxx, vuu, tss, y, x, x)
	SWIZZLE_IMPL(yxy, vuv, tst, y, x, y)
	SWIZZLE_IMPL(yxz, vuw, tsq, y, x, z)
	SWIZZLE_IMPL(yyx, vvu, tts, y, y, x)
	SWIZZLE_IMPL(yyy, vvv, ttt, y, y, y)
	SWIZZLE_IMPL(yyz, vvw, ttq, y, y, z)
	SWIZZLE_IMPL(yzx, vwu, tqs, y, z, x)
	SWIZZLE_IMPL(yzy, vwv, tqt, y, z, y)
	SWIZZLE_IMPL(yzz, vww, tqq, y, z, z)
	SWIZZLE_IMPL(zxx, wuu, qss, z, x, x)
	SWIZZLE_IMPL(zxy, wuv, qst, z, x, y)
	SWIZZLE_IMPL(zxz, wuw, qsq, z, x, z)
	SWIZZLE_IMPL(zyx, wvu, qts, z, y, x)
	SWIZZLE_IMPL(zyy, wvv, qtt, z, y, y)
	SWIZZLE_IMPL(zyz, wvw, qtq, z, y, z)
	SWIZZLE_IMPL(zzx, wwu, qqs, z, z, x)
	SWIZZLE_IMPL(zzy, wwv, qqt, z, z, y)
	SWIZZLE_IMPL(zzz, www, qqq, z, z, z)

#undef SWIZZLE_IMPL

	template<Numeric Ty>
	constexpr auto Vec3<Ty>::Abs() const noexcept -> Vec3
	{
		return { Math::Abs(x), Math::Abs(y), Math::Abs(z) };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Ceil() const noexcept -> Vec3
	{
		return { Math::Ceil(x), Math::Ceil(y), Math::Ceil(z) };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Floor() const noexcept -> Vec3
	{
		return { Math::Floor(x), Math::Floor(y), Math::Floor(z) };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Trunc() const noexcept -> Vec3
	{
		return { Math::Trunc(x), Math::Trunc(y), Math::Trunc(z) };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Round() const noexcept -> Vec3
	{
		return { Math::Round(x), Math::Round(y), Math::Round(z) };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::RoundEven() const noexcept -> Vec3
	{
		return { Math::RoundEven(x), Math::RoundEven(y), Math::RoundEven(z) };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Sign() const noexcept -> Vec3
	{
		return { Math::Sign(x), Math::Sign(y), Math::Sign(z) };
	}

	template <Numeric Ty>
	constexpr auto Vec3<Ty>::Fract() const noexcept -> Vec3
	{
		return { Math::Fract(x), Math::Fract(y), Math::Fract(z) };
	}

	template<Numeric T, ConvertableTo<T> A>
	constexpr auto operator*(A t, const Vec3<T>& vec) noexcept -> Vec3<T>
	{
		return vec * t;
	}

	template <Numeric T, ConvertableTo<T> A>
	constexpr auto operator*(const Quaternion<A>& quat, const Vec3<T>& vec) noexcept -> Vec3<T>
	{
		return {
				quat.w * vec.x + quat.y * vec.z - quat.z * vec.y,
				quat.w * vec.y + quat.z * vec.x - quat.x * vec.z,
				quat.w * vec.z + quat.x * vec.y - quat.y * vec.x
		};
	}
}
