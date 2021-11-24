#pragma once

#if __RESHARPER__
#include "Sphere.h"
#endif

#include "MathUtils.h"

namespace Core::Math
{
	template <Numeric T>
	constexpr Sphere<T>::Sphere() noexcept
		: center()
		, radius(T(1))
	{
	}

	template <Numeric T>
	constexpr Sphere<T>::Sphere(const Vec3<T>& center, T radius) noexcept
		: center(center)
		, radius(radius)
	{
	}

	template <Numeric T>
	constexpr auto Sphere<T>::operator==(const Sphere& other) const noexcept -> bool
	{
		return center == other.center && radius == other.radius;
	}

	template <Numeric T>
	constexpr auto Sphere<T>::operator!=(const Sphere& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <Numeric T>
	constexpr auto Sphere<T>::Compare(const Sphere& other, T e) const noexcept -> bool
	{
		return center.Compare(other.center, e) && EpsilonCompare(radius, other.radius, e);
	}

	template <Numeric T>
	constexpr auto Sphere<T>::Volume() const noexcept -> T
	{
		return (T(4) / T(3)) * Consts::Pi<T> * radius * radius * radius;
	}

	template <Numeric T>
	constexpr auto Sphere<T>::ContainsPoint(const Vec3<T>& point) const noexcept -> bool
	{
		return (point - center).LenSq() <= radius * radius;
	}

	template <Numeric T>
	constexpr auto Sphere<T>::Overlaps(const Sphere& sphere) const noexcept -> bool
	{
		T totalRad = radius + sphere.radius;
		totalRad *= totalRad;

		return (sphere.center - center).LenSq() < totalRad;
	}

	template <Numeric T>
	constexpr auto Sphere<T>::OverlapsInclusive(const Sphere& sphere) const noexcept -> bool
	{
		T totalRad = radius + sphere.radius;
		totalRad *= totalRad;

		return (sphere.center - center).LenSq() <= totalRad;
	}

	template <Numeric T>
	constexpr auto Sphere<T>::Encloses(const Sphere& sphere) const noexcept -> bool
	{
		if (radius < sphere.radius)
			return false;

		T maxRad = sphere.radius - radius;
		maxRad *= maxRad;

		return (sphere.center - center).LenSq() < maxRad;
	}

	template <Numeric T>
	constexpr auto Sphere<T>::DistanceSq(const Vec3<T>& point) const noexcept -> T
	{
		return (point - center).LenSq() - radius * radius;
	}

	template <Numeric T>
	constexpr auto Sphere<T>::DistanceSq(const Sphere& sphere) const noexcept -> T
	{
		T totalRad = radius + sphere.radius;
		return (sphere.center - center).LenSq() - totalRad * totalRad;
	}

	template <Numeric T>
	constexpr auto Sphere<T>::Merge(const Sphere& sphere) noexcept -> Sphere&
	{
		T dist = (sphere.center - center).Len();
		if (dist + sphere.radius <= radius)
			return *this;
		if (dist + radius <= sphere.radius)
		{
			*this = sphere;
			return *this;
		}

		T theta = T(1) / T(2) + (sphere.radius - radius) / (T(2) * dist);

		center = center.Lerp(sphere.center, theta);
		radius += dist + sphere.radius;
		radius /= T(2);
		return *this;
	}

	template <Numeric T>
	constexpr auto Sphere<T>::Merged(const Sphere& sphere) const noexcept -> Sphere
	{
		T dist = (sphere.center - center).Len();
		if (dist + sphere.radius <= radius)
			return *this;
		if (dist + radius <= sphere.radius)
			return sphere;

		T theta = T(1) / T(2) + (sphere.radius - radius) / (T(2) * dist);

		T cen = center.Lerp(sphere.center, theta);
		T rad = (dist + radius + sphere.radius) / T(2);
		return { cen, rad };
	}
}
