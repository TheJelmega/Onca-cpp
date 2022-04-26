#pragma once

#if __RESHARPER__
#include "Vec4.h"
#endif

#include "Constants.h"
#include "core/Assert.h"

namespace Onca::Math
{
	template <Numeric Ty>
	constexpr Vec4<Ty>::Vec4(const PackT& pack) noexcept
		: pack(pack)
	{
	}

	template <Numeric Ty>
	constexpr Vec4<Ty>::Vec4() noexcept
	{
		if constexpr (HasNativeRegister())
		{
			pack = PackT::Zero();
		}
		else
		{
			x = Ty(0);
			y = Ty(0);
			z = Ty(0);
			w = Ty(0);
		}
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr Vec4<Ty>::Vec4(A val) noexcept
	{
		if constexpr (HasNativeRegister())
		{
			pack = PackT::Set(Ty(val));
		}
		else
		{
			x = Ty(val);
			y = Ty(val);
			z = Ty(val);
			w = Ty(val);
		}
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, ConvertableTo<Ty> B, ConvertableTo<Ty> C, ConvertableTo<Ty> D>
	constexpr Vec4<Ty>::Vec4(A x, B y, C z, D w) noexcept
	{
		if constexpr (HasNativeRegister())
		{
			pack = PackT::Set(Ty(x), Ty(y), Ty(z), Ty(w));
		}
		else
		{
			x = Ty(x);
			y = Ty(y);
			z = Ty(z);
			w = Ty(w);
		}
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, ConvertableTo<Ty> B, ConvertableTo<Ty> C>
	constexpr Vec4<Ty>::Vec4(const Vec2<A>& vec, B z, C w)
	{
		if constexpr (HasNativeRegister())
		{
			pack = PackT::Set(Ty(vec.x), Ty(vec.y), Ty(z), Ty(w));
		}
		else
		{
			x = Ty(vec.x);
			y = Ty(vec.y);
			z = Ty(z);
			w = Ty(w);
		}
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
	constexpr Vec4<Ty>::Vec4(const Vec3<A>& vec, B w)
	{
		if constexpr (HasNativeRegister())
		{
			pack = PackT::Set(Ty(vec.x), Ty(vec.y), Ty(vec.z), Ty(w));
		}
		else
		{
			x = Ty(vec.x);
			y = Ty(vec.y);
			z = Ty(vec.z);
			w = Ty(w);
		}
	}

	template <Numeric Ty>
	constexpr Vec4<Ty>::Vec4(const Vec4& other) noexcept
	{
		if constexpr (HasNativeRegister())
		{
			pack = other.pack;
		}
		else
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr Vec4<Ty>::Vec4(const Vec4<A>& other) noexcept
	{
		if constexpr (HasNativeRegister())
		{
			pack = other.pack.template Convert<Ty>();
		}
		else
		{
			x = Ty(other.x);
			y = Ty(other.y);
			z = Ty(other.z);
			w = Ty(other.w);
		}
	}

	template <Numeric Ty>
	constexpr Vec4<Ty>::Vec4(Vec4&& other) noexcept
	{
		if constexpr (HasNativeRegister())
		{
			pack = other.pack;
		}
		else
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator=(const Vec4& other) noexcept -> Vec4&
	{
		if constexpr (HasNativeRegister())
		{
			pack = other.pack;
		}
		else
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator=(const Vec4<A>& other) noexcept -> Vec4&
	{
		if constexpr (HasNativeRegister())
		{
			pack = other.pack;
		}
		else
		{
			x = Ty(other.x);
			y = Ty(other.y);
			z = Ty(other.z);
			w = Ty(other.w);
		}
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator=(Vec4&& other) noexcept -> Vec4&
	{
		if constexpr (HasNativeRegister())
		{
			pack = other.pack;
		}
		else
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator++() noexcept -> Vec4&
	{
		if constexpr (HasNativeRegister())
		{
			++pack;
		}
		else
		{
			++x;
			++y;
			++z;
			++w;
		}
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator++(int) noexcept -> Vec4
	{
		Vec4 tmp = *this;
		operator++();
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator--() noexcept -> Vec4&
	{
		if constexpr (HasNativeRegister())
		{
			--pack;
		}
		else
		{
			--x;
			--y;
			--z;
			--w;
		}
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator--(int) noexcept -> Vec4
	{
		Vec4 tmp = *this;
		operator--();
		return tmp;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator+() const noexcept -> Vec4
	{
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator-() const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ -pack };
		else
			return { -x, -y, -z, -w };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator~() const noexcept -> Vec4 requires Invertable<Ty>
	{

		if constexpr (HasNativeRegister())
			return Vec4{ ~pack };
		else
			return { ~x, ~y, ~z, ~w };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator+(const Vec4& other) const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack + other.pack };
		else
			return { x + other.x, y + other.y, z + other.z, w + other.w };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator-(const Vec4& other) const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack - other.pack };
		else
			return { x - other.x, y - other.y, z - other.z, w - other.w };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator*(const Vec4& other) const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack * other.pack };
		else
			return { x * other.x, y * other.y, z * other.z, w * other.w };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator/(const Vec4& other) const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack / other.pack };
		else
			return { x / other.x, y / other.y, z / other.z, w / other.w };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator%(const Vec4& other) const noexcept -> Vec4 requires HasModulo<Ty>
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack % other.pack };
		else
			return { x % other.x, y % other.y, z % other.z, w % other.w };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator|(const Vec4& other) const noexcept -> Vec4 requires Orable<Ty>
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack | other.pack };
		else
			return { x | other.x, y | other.y, z | other.z, w | other.w };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator^(const Vec4& other) const noexcept -> Vec4 requires Xorable<Ty>
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack ^ other.pack };
		else
			return { x ^ other.x, y ^ other.y, z ^ other.z, w ^ other.w };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator&(const Vec4& other) const noexcept -> Vec4 requires Andable<Ty>
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack & other.pack };
		else
			return { x & other.x, y & other.y, z & other.z, w & other.w };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator<<(const Vec4& other) const noexcept -> Vec4 requires BitShiftable<Ty>
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack << other.pack };
		else
			return { x << other.x, y << other.y, z << other.z, w << other.w };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator>>(const Vec4& other) const noexcept -> Vec4 requires BitShiftable<Ty>
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack >> other.pack };
		else
			return { x >> other.x, y >> other.y, z >> other.z, w >> other.w };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator+(A val) const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack + PackT::Set(Ty(val)) };
		else
			return { x + Ty(val), y + Ty(val), z + Ty(val), w + Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator-(A val) const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack - PackT::Set(Ty(val)) };
		else
			return { x - Ty(val), y - Ty(val), z - Ty(val), w - Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator*(A val) const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack * PackT::Set(Ty(val)) };
		else
			return { x * Ty(val), y * Ty(val), z * Ty(val), w * Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator/(A val) const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack / PackT::Set(Ty(val)) };
		else
			return { x / Ty(val), y / Ty(val), z / Ty(val), w / Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator%(A val) const noexcept -> Vec4 requires HasModulo<Ty>
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack % PackT::Set(Ty(val)) };
		else
			return { x % Ty(val), y % Ty(val), z % Ty(val), w % Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator|(A val) const noexcept -> Vec4 requires Orable<Ty>
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack | PackT::Set(Ty(val)) };
		else
			return { x | Ty(val), y | Ty(val), z | Ty(val), w | Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator^(A val) const noexcept -> Vec4 requires Xorable<Ty>
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack ^ PackT::Set(Ty(val)) };
		else
			return { x ^ Ty(val), y ^ Ty(val), z ^ Ty(val), w ^ Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator&(A val) const noexcept -> Vec4 requires Andable<Ty>
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack & PackT::Set(Ty(val)) };
		else
			return { x & Ty(val), y & Ty(val), z & Ty(val), w & Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator<<(A val) const noexcept -> Vec4 requires BitShiftable<Ty>
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack << PackT::Set(Ty(val)) };
		else
			return { x << Ty(val), y << Ty(val), z << Ty(val), w << Ty(val) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator>>(A val) const noexcept -> Vec4 requires BitShiftable<Ty>
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack >> PackT::Set(Ty(val)) };
		else
			return { x >> Ty(val), y >> Ty(val), z >> Ty(val), w >> Ty(val) };
	}

	template <Numeric Ty>
	template<ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator*(const Quaternion<A>& quat) const noexcept -> Vec4
	{
		// TODO SIMD
		{
			return {
				x * quat.w + y * quat.z - z * quat.y,
				y * quat.w + z * quat.x - x * quat.z,
				z * quat.w + x * quat.y - y * quat.x,
				0
			};
		}
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator+=(const Vec4& other) noexcept -> Vec4&
	{
		if constexpr (HasNativeRegister())
		{
			pack += other.pack;
		}
		else
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
		}
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator-=(const Vec4& other) noexcept -> Vec4&
	{
		if constexpr (HasNativeRegister())
		{
			pack -= other.pack;
		}
		else
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
		}
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator*=(const Vec4& other) noexcept -> Vec4&
	{
		if constexpr (HasNativeRegister())
		{
			pack *= other.pack;
		}
		else
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
		}
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator/=(const Vec4& other) noexcept -> Vec4&
	{
		if constexpr (HasNativeRegister())
		{
			pack /= other.pack;
		}
		else
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
		}
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator%=(const Vec4& other) noexcept -> Vec4& requires HasModulo<Ty>
	{
		if constexpr (HasNativeRegister())
		{
			pack %= other.pack;
		}
		else
		{
			x %= other.x;
			y %= other.y;
			z %= other.z;
			w %= other.w;
		}
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator|=(const Vec4& other) noexcept -> Vec4& requires Orable<Ty>
	{
		if constexpr (HasNativeRegister())
		{
			pack |= other.pack;
		}
		else
		{
			x |= other.x;
			y |= other.y;
			z |= other.z;
			w |= other.w;
		}
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator^=(const Vec4& other) noexcept -> Vec4& requires Xorable<Ty>
	{
		if constexpr (HasNativeRegister())
		{
			pack ^= other.pack;
		}
		else
		{
			x ^= other.x;
			y ^= other.y;
			z ^= other.z;
			w ^= other.w;
		}
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator&=(const Vec4& other) noexcept -> Vec4& requires Andable<Ty>
	{
		if constexpr (HasNativeRegister())
		{
			pack &= other.pack;
		}
		else
		{
			x &= other.x;
			y &= other.y;
			z &= other.z;
			w &= other.w;
		}
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator<<=(const Vec4& other) noexcept -> Vec4& requires BitShiftable<Ty>
	{
		if constexpr (HasNativeRegister())
		{
			pack <<= other.pack;
		}
		else
		{
			x <<= other.x;
			y <<= other.y;
			z <<= other.z;
			w <<= other.w;
		}
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator>>=(const Vec4& other) noexcept -> Vec4& requires BitShiftable<Ty>
	{
		if constexpr (HasNativeRegister())
		{
			pack >>= other.pack;
		}
		else
		{
			x >>= other.x;
			y >>= other.y;
			z >>= other.z;
			w >>= other.w;
		}
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator+=(A val) noexcept -> Vec4&
	{
		if constexpr (HasNativeRegister())
		{
			pack += PackT::Set(Ty(val));
		}
		else
		{
			x += Ty(val);
			y += Ty(val);
			z += Ty(val);
			w += Ty(val);
		}
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator-=(A val) noexcept -> Vec4&
	{
		if constexpr (HasNativeRegister())
		{
			pack -= PackT::Set(Ty(val));
		}
		else
		{
			x -= Ty(val);
			y -= Ty(val);
			z -= Ty(val);
			w -= Ty(val);
		}
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator*=(A val) noexcept -> Vec4&
	{
		if constexpr (HasNativeRegister())
		{
			pack *= PackT::Set(Ty(val));
		}
		else
		{
			x *= Ty(val);
			y *= Ty(val);
			z *= Ty(val);
			w *= Ty(val);
		}
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator/=(A val) noexcept -> Vec4&
	{
		if constexpr (HasNativeRegister())
		{
			pack /= PackT::Set(Ty(val));
		}
		else
		{
			x /= Ty(val);
			y /= Ty(val);
			z /= Ty(val);
			w /= Ty(val);
		}
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator%=(A val) noexcept -> Vec4& requires HasModulo<Ty>
	{
		if constexpr (HasNativeRegister())
		{
			pack %= PackT::Set(Ty(val));
		}
		else
		{
			x %= Ty(val);
			y %= Ty(val);
			z %= Ty(val);
			w %= Ty(val);
		}
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator|=(A val) noexcept -> Vec4& requires Orable<Ty>
	{
		if constexpr (HasNativeRegister())
		{
			pack |= PackT::Set(Ty(val));
		}
		else
		{
			x |= Ty(val);
			y |= Ty(val);
			z |= Ty(val);
			w |= Ty(val);
		}
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator^=(A val) noexcept -> Vec4& requires Xorable<Ty>
	{
		if constexpr (HasNativeRegister())
		{
			pack ^= PackT::Set(Ty(val));
		}
		else
		{
			x ^= Ty(val);
			y ^= Ty(val);
			z ^= Ty(val);
			w ^= Ty(val);
		}
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator&=(A val) noexcept -> Vec4& requires Andable<Ty>
	{
		if constexpr (HasNativeRegister())
		{
			pack &= PackT::Set(Ty(val));
		}
		else
		{
			x &= Ty(val);
			y &= Ty(val);
			z &= Ty(val);
			w &= Ty(val);
		}
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator<<=(A val) noexcept -> Vec4& requires BitShiftable<Ty>
	{
		if constexpr (HasNativeRegister())
		{
			pack <<= PackT::Set(Ty(val));
		}
		else
		{
			x <<= Ty(val);
			y <<= Ty(val);
			z <<= Ty(val);
			w <<= Ty(val);
		}
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator>>=(A val) noexcept -> Vec4& requires BitShiftable<Ty>
	{
		if constexpr (HasNativeRegister())
		{
			pack >>= PackT::Set(Ty(val));
		}
		else
		{
			x >>= Ty(val);
			y >>= Ty(val);
			z >>= Ty(val);
			w >>= Ty(val);
		}
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::operator*=(const Quaternion<A>& quat) noexcept -> Vec4&
	{
		// TODO SIMD
		{
			Ty tmpX = x * quat.w + y * quat.z - z * quat.y;
			Ty tmpY = y * quat.w + z * quat.x - x * quat.z;
			Ty tmpZ = z * quat.w + x * quat.y - y * quat.x;
			x = tmpX;
			y = tmpY;
			z = tmpZ;
		}
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator[](usize idx) noexcept -> Ty&
	{
		MATH_ASSERT(idx < 4, "Index out of range");
		return data[idx];
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator[](usize idx) const noexcept -> Ty
	{
		MATH_ASSERT(idx < 4, "Index out of range");
		return data[idx];
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator==(const Vec4& other) const noexcept -> bool
	{
		if (HasNativeRegister())
			return (pack == other.pack).All();
		else
			return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::operator!=(const Vec4& other) const noexcept -> bool
	{
		if (HasNativeRegister())
			return (pack == other.pack).None();
		else
			return !(*this == other);
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Compare(const Vec4& other, Ty e) const noexcept -> bool
	{
		if constexpr (HasNativeRegister())
			return Math::EpsilonCompare(pack, other.pack, e).All();
		else
			return Math::EpsilonCompare(x, other.x, e) && Math::EpsilonCompare(y, other.y, e), Math::EpsilonCompare(z, other.z, e) && Math::EpsilonCompare(w, other.w, e);
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::IsZero() const noexcept -> bool
	{
		return x == Ty(0) && y == Ty(0) && z == Ty(0) && w == Ty(0);
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::IsNearlyZero(Ty e) const noexcept -> bool
	{
		return Math::EpsilonCompare(x, Ty(0), e) && Math::EpsilonCompare(y, Ty(0), e) && Math::EpsilonCompare(z, Ty(0), e) && Math::EpsilonCompare(w, Ty(0), e);
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::IsUniform(Ty e) const noexcept -> bool
	{
		return Math::EpsilonCompare(x, y, e) && Math::EpsilonCompare(x, z, e) && Math::EpsilonCompare(x, w, e);
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::MinComponent() const noexcept -> Ty
	{
		return Math::Min(x, y, z, w);
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::MinAbsComponent() const noexcept -> Ty
	{
		return Abs().MinComponent();
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::MaxComponent() const noexcept -> Ty
	{
		return Math::Max(x, y, z, w);
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::MaxAbsComponent() const noexcept -> Ty
	{
		return Abs().MaxComponent();
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Min(const Vec4& other) const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack.Min(other.pack) };
		else
			return { Math::Min(x, other.x), Math::Min(y, other.y), Math::Min(z, other.z), Math::Min(w, other.w) };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Max(const Vec4& other) const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack.Max(other.pack) };
		else
			return { Math::Max(x, other.x), Math::Max(y, other.y), Math::Max(z, other.z), Math::Max(w, other.w) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
	constexpr auto Vec4<Ty>::Clamp(A min, B max) noexcept -> Vec4&
	{
		if (HasNativeRegister())
		{
			pack = Math::Clamp(pack, PackT::Set(min), PackT::Set(max));
		}
		else
		{
			x = Math::Clamp(x, min, max);
			y = Math::Clamp(y, min, max);
			z = Math::Clamp(z, min, max);
			w = Math::Clamp(w, min, max);
		}
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Clamp(const Vec4& min, const Vec4& max) noexcept -> Vec4&
	{
		if (HasNativeRegister())
		{
			pack = Math::Clamp(pack, min.pack, max.pack);
		}
		else
		{
			x = Math::Clamp(x, min.x, max.x);
			y = Math::Clamp(y, min.y, max.y);
			z = Math::Clamp(z, min.z, max.z);
			w = Math::Clamp(w, min.w, max.w);
		}
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
	constexpr auto Vec4<Ty>::Clamped(A min, B max) const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ Math::Clamp(pack, PackT::Set(Ty(min)), PackT::Set(Ty(max))) };
		else
			return { Math::Clamp(x, min, max), Math::Clamp(y, min, max), Math::Clamp(z, min, max), Math::Clamp(w, min, max) };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Clamped(const Vec4& min, const Vec4& max) const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ Math::Clamp(pack, min.pack, max.pack) };
		else
			return { Math::Clamp(x, min.x, max.x), Math::Clamp(y, min.y, max.y), Math::Clamp(z, min.z, max.z), Math::Clamp(w, min.w, max.w) };
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
	constexpr auto Vec4<Ty>::ClampLen(A min, B max) noexcept -> Vec4&
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
	constexpr auto Vec4<Ty>::ClampedLen(A min, B max) const noexcept -> Vec4
	{
		Ty minSq = min * min;
		Ty maxSq = max * max;
		Ty lenSq = LenSq();
		if (lenSq >= minSq && lenSq <= maxSq)
			return *this;

		Ty len = lenSq < minSq ? min : max;
		return *this * (Math::RSqrt(lenSq) * len);
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Saturate() noexcept -> Vec4&
	{
		return Clamp(0, 1);
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Saturated() const noexcept -> Vec4
	{
		return Clamped(0, 1);
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Snap(Ty stepSize) noexcept -> Vec4&
	{
		*this = Snapped(stepSize);
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Snapped(Ty stepSize) const noexcept -> Vec4
	{
		return (*this / stepSize).Round() * stepSize;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::Lerp(const Vec4& other, A i) const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ Math::Lerp(pack, other.pack, PackT::Set(i)) };
		else
			return *this * (Ty(1) - Ty(i)) + other * Ty(i);
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::LenSq() const noexcept -> Ty
	{
		if constexpr (HasNativeRegister())
			return pack.Dot(pack).template Extract<0>();
		else
			return x * x + y * y + z * z + w * w;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Len() const noexcept -> Ty
	{
		return Math::Sqrt(LenSq());
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::DistanceSq(const Vec4& other) const noexcept -> Ty
	{
		return (*this - other).LenSq();
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Distance(const Vec4& other) const noexcept -> Ty
	{
		return (*this - other).Len();
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Dot(const Vec4& other) const noexcept -> Ty
	{
		if constexpr (HasNativeRegister())
			return pack.Dot(other.pack).template Extract<0>();
		else
			return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Cross3D(const Vec4& other) const noexcept -> Vec4
	{
		// TODO: SIMD version
		MATH_ASSERT(w == 0, "Can only calculate the dot product of vectors with a w of 0");
		return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x, Ty(0) };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Normalize() noexcept -> Vec4&
	{
		if constexpr (HasNativeRegister())
			pack = pack * pack.Dot(pack).RSqrt();
		else
			*this *= Math::RSqrt(LenSq());
		return *this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Normalized() const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack * pack.Dot(pack).RSqrt() };
		else
			return *this * Math::RSqrt(LenSq());
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::IsNormalized(Ty e) const noexcept -> bool
	{
		return Math::EpsilonCompare(LenSq(), Ty(1), e);
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::Rotate(const Quaternion<A>& quat) noexcept -> Vec4&
	{
		*this = Rotated(quat);
		return *this;
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::Rotated(const Quaternion<A>& quat) const noexcept -> Vec4
	{
		MATH_ASSERT(quat.IsNormalized(), "Can only rotate with a normalized quaternion");
		MATH_ASSERT(Math::EpsilonCompare(w, 0, Consts::MachineEpsilon<Ty>), "Can only rotate a 4D vector with w=0");

		Vec3<Ty> vec{ x, y, z };
		Vec3<Ty> a = quat.imaginary.Cross(vec) + quat.real * vec;
		Vec4 b = { quat.imaginary.Cross(a), 0 };
		return Ty(2) * b + *this;
	}
	
	template <Numeric Ty>
	constexpr auto Vec4<Ty>::FaceForward3D(const Vec4& incidence, const Vec4& normal) const noexcept -> Vec4
	{
		MATH_ASSERT(w == 0, "Can only calculate the face forward of a vector with a w of 0");
		MATH_ASSERT(incidence.w == 0, "Can only calculate the face forward of an incidence vector with a w of 0");
		MATH_ASSERT(normal.w == 0, "Can only calculate the face forward of an normal vector with a w of 0");
		return incidence.Dot(normal) < 0 ? *this : -*this;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Reflect3D(const Vec4& normal) const noexcept -> Vec4
	{
		MATH_ASSERT(w == 0, "Can only calculate the reflection of a vector with a w of 0");
		MATH_ASSERT(normal.w == 0, "Can only calculate the reflection of a normal with a w of 0");
		MATH_ASSERT(normal.IsNormalized(), "normal needs to be normalized");
		if constexpr (HasNativeRegister())
			return Vec4{ pack - normal.pack * pack.Dot(normal.pack) * PackT::Set(2) };
		else
			return *this - normal * Dot(normal) * Ty(2);
	}

	template <Numeric Ty>
	template <ConvertableTo<Ty> A>
	constexpr auto Vec4<Ty>::Refract3D(const Vec4& normal, A eta) const noexcept -> Vec4
	{
		MATH_ASSERT(w == 0, "Can only calculate the refraction of a vector with a w of 0");
		MATH_ASSERT(normal.w == 0, "Can only calculate the refraction of a normal vector with a w of 0");
		MATH_ASSERT(normal.IsNormalized(), "normal needs to be normalized");

		Ty cosi = Dot(normal);
		MATH_ASSERT(cosi < 0, "vector should move into the surface (this vector should be in the opposite direction of the normal)");

		Ty k = Ty(1) - eta * eta * (Ty(1) - cosi * cosi);
		if (k < 0)
			return {};

		if constexpr (HasNativeRegister())
			return Vec4{ pack * PackT::Set(eta) - normal.pack * PackT::Set(eta * cosi + Math::Sqrt(k)) };
		else
			return (*this * eta - normal * (eta * cosi + Math::Sqrt(k)));
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::DirectionAndLength() const noexcept -> Pair<Vec4, Ty>
	{
		Ty len = Len();
		Vec4 dir = len >= Consts::MathEpsilon<Ty> ? *this / len : Vec4{};
		return { dir, len };
	}

	template <Numeric Ty>
	template <usize Idx0, usize Idx1, usize Idx2, usize Idx3>
	constexpr auto Vec4<Ty>::Swizzle() const noexcept -> Vec4
	{
		// TODO: SIMD version
		Vec4 res;
		switch (Idx0)
		{
		case X: res.x = x; break;
		case Y: res.x = y; break;
		case Z: res.x = z; break;
		case W: res.x = w; break;
		default: break;
		}
		switch (Idx1)
		{
		case X: res.y = x; break;
		case Y: res.y = y; break;
		case Z: res.y = z; break;
		case W: res.y = w; break;
		default: break;
		}
		switch (Idx2)
		{
		case X: res.z = x; break;
		case Y: res.z = y; break;
		case Z: res.z = z; break;
		case W: res.z = w; break;
		default: break;
		}
		switch (Idx3)
		{
		case X: res.w = x; break;
		case Y: res.w = y; break;
		case Z: res.w = z; break;
		case W: res.w = w; break;
		default: break;
		}
		return res;
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Swizzle(u8 idx0, u8 idx1, u8 idx2, u8 idx3) const noexcept -> Vec4
	{
		Vec4 res;
		switch (idx0)
		{
		case X: res.x = x; break;
		case Y: res.x = y; break;
		case Z: res.x = z; break;
		case W: res.x = w; break;
		default: break;
		}
		switch (idx1)
		{
		case X: res.y = x; break;
		case Y: res.y = y; break;
		case Z: res.y = z; break;
		case W: res.y = w; break;
		default: break;
		}
		switch (idx2)
		{
		case X: res.z = x; break;
		case Y: res.z = y; break;
		case Z: res.z = z; break;
		case W: res.z = w; break;
		default: break;
		}
		switch (idx3)
		{
		case X: res.w = x; break;
		case Y: res.w = y; break;
		case Z: res.w = z; break;
		case W: res.w = w; break;
		default: break;
		}
		return res;
	}

#define CONCAT_SWIZZLE_(x,y,z,w) x ## y ## z ## w
#define CONCAT_SWIZZLE(x,y,z,w) CONCAT_SWIZZLE_(x,y,z,w)

#define SWIZZLE_IMPL(x, y, z, w) \
	template<Numeric Ty> constexpr auto Vec4<Ty>::CONCAT_SWIZZLE(x, y, z, w)() const noexcept -> Vec4<Ty> { return { x, y, z, w }; }

#define SWIZZLE_IMPL4(x_, y_, z_) \
	SWIZZLE_IMPL(x_, y_, z_, x) \
	SWIZZLE_IMPL(x_, y_, z_, y) \
	SWIZZLE_IMPL(x_, y_, z_, z) \
	SWIZZLE_IMPL(x_, y_, z_, w)

#define SWIZZLE_IMPL16(x_, y_) \
	SWIZZLE_IMPL4(x_, y_, x) \
	SWIZZLE_IMPL4(x_, y_, y) \
	SWIZZLE_IMPL4(x_, y_, z) \
	SWIZZLE_IMPL4(x_, y_, w)

	// TODO: SIMD version
	SWIZZLE_IMPL16(x, x)
	SWIZZLE_IMPL16(x, y)
	SWIZZLE_IMPL16(x, z)
	SWIZZLE_IMPL16(x, w)

	SWIZZLE_IMPL16(y, x)
	SWIZZLE_IMPL16(y, y)
	SWIZZLE_IMPL16(y, z)
	SWIZZLE_IMPL16(y, w)

	SWIZZLE_IMPL16(z, x)
	SWIZZLE_IMPL16(z, y)
	SWIZZLE_IMPL16(z, z)
	SWIZZLE_IMPL16(z, w)

	SWIZZLE_IMPL16(w, x)
	SWIZZLE_IMPL16(w, y)
	SWIZZLE_IMPL16(w, z)
	SWIZZLE_IMPL16(w, w)

#undef SWIZZLE_IMPL16
#undef SWIZZLE_IMPL4
#undef SWIZZLE_IMPL
#undef CONCAT_SWIZZLE
#undef CONCAT_SWIZZLE_

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Abs() const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack.Abs() };
		else
			return { Math::Abs(x), Math::Abs(y), Math::Abs(z), Math::Abs(w) };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Ceil() const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack.Ceil() };
		else
			return { Math::Ceil(x), Math::Ceil(y), Math::Ceil(z), Math::Ceil(w) };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Floor() const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack.Floor() };
		else
			return { Math::Floor(x), Math::Floor(y), Math::Floor(z), Math::Floor(w) };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Trunc() const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack.Trunc() };
		else
			return { Math::Trunc(x), Math::Trunc(y), Math::Trunc(z), Math::Trunc(w) };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Round() const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack.Round() };
		else
			return { Math::Round(x), Math::Round(y), Math::Round(z), Math::Round(w) };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::RoundEven() const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack.RoundEven() };
		else
			return { Math::RoundEven(x), Math::RoundEven(y), Math::RoundEven(z), Math::RoundEven(w) };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Sign() const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ pack.Sign() };
		else
			return { Math::Sign(x), Math::Sign(y), Math::Sign(z), Math::Sign(w) };
	}

	template <Numeric Ty>
	constexpr auto Vec4<Ty>::Fract() const noexcept -> Vec4
	{
		if constexpr (HasNativeRegister())
			return Vec4{ Math::Fract(pack) };
		else
			return { Math::Fract(x), Math::Fract(y), Math::Fract(z), Math::Fract(w) };
	}

	template <Numeric T, ConvertableTo<T> A>
	constexpr auto operator*(A t, const Vec4<T>& vec) noexcept -> Vec4<T>
	{
		return vec * t;
	}

	template <Numeric T, ConvertableTo<T> A>
	constexpr auto operator*(const Quaternion<A>& quat, const Vec4<T>& vec) noexcept -> Vec4<T>
	{
		return {
				quat.w * vec.x + quat.y * vec.z - quat.z * vec.y,
				quat.w * vec.y + quat.z * vec.x - quat.x * vec.z,
				quat.w * vec.z + quat.x * vec.y - quat.y * vec.x,
				0
		};
	}
}
