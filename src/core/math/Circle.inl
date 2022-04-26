#pragma once

#if __RESHARPER__
#include "Circle.h"
#endif

#include "MathUtils.h"

namespace Onca::Math
{
	template <Numeric T>
	constexpr Circle<T>::Circle() noexcept
		: center()
		, radius(T(1))
	{
	}

	template <Numeric T>
	constexpr Circle<T>::Circle(const Vec2<T>& center, T radius) noexcept
		: center(center)
		, radius(radius)
	{
	}

	template <Numeric T>
	constexpr auto Circle<T>::operator==(const Circle& other) const noexcept -> bool
	{
		return center == other.center && radius == other.radius;
	}

	template <Numeric T>
	constexpr auto Circle<T>::operator!=(const Circle& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <Numeric T>
	constexpr auto Circle<T>::Compare(const Circle& other, T e) const noexcept -> bool
	{
		return center.Compare(other.center, e) && EpsilonCompare(radius, other.radius, e);
	}

	template <Numeric T>
	constexpr auto Circle<T>::Area() const noexcept -> T
	{
		return Consts::Pi<T> * radius * radius;
	}

	template <Numeric T>
	constexpr auto Circle<T>::ContainsPoint(const Vec2<T>& point) const noexcept -> bool
	{
		return (point - center).LenSq() <= radius * radius;
	}

	template <Numeric T>
	constexpr auto Circle<T>::Overlaps(const Circle& circle) const noexcept -> bool
	{
		T totalRad = radius + circle.radius;
		totalRad *= totalRad;

		return (circle.center - center).LenSq() < totalRad;
	}

	template <Numeric T>
	constexpr auto Circle<T>::OverlapsInclusive(const Circle& circle) const noexcept -> bool
	{
		T totalRad = radius + circle.radius;
		totalRad *= totalRad;

		return (circle.center - center).LenSq() <= totalRad;
	}

	template <Numeric T>
	constexpr auto Circle<T>::Encloses(const Circle& circle) const noexcept -> bool
	{
		if (radius < circle.radius)
			return false;

		T maxRad = circle.radius - radius;
		maxRad *= maxRad;

		return (circle.center - center).LenSq() < maxRad;
	}

	template <Numeric T>
	constexpr auto Circle<T>::DistanceSq(const Vec2<T>& point) const noexcept -> T
	{
		return (point - center).LenSq() - radius * radius;
	}

	template <Numeric T>
	constexpr auto Circle<T>::DistanceSq(const Circle& circle) const noexcept -> T
	{
		T totalRad = radius + circle.radius;
		return (circle.center - center).LenSq() - totalRad * totalRad;
	}

	template <Numeric T>
	constexpr auto Circle<T>::Merge(const Circle& circle) noexcept -> Circle&
	{
		T dist = (circle.center - center).Len();
		if (dist + circle.radius <= radius)
			return *this;
		if (dist + radius <= circle.radius)
		{
			*this = circle;
			return *this;
		}
		
		T theta = T(1) / T(2) + (circle.radius - radius) / (T(2) * dist);

		center = center.Lerp(circle.center, theta);
		radius += dist + circle.radius;
		radius /= T(2);
		return *this;
	}

	template <Numeric T>
	constexpr auto Circle<T>::Merged(const Circle& circle) const noexcept -> Circle
	{
		T dist = (circle.center - center).Len();
		if (dist + circle.radius <= radius)
			return *this;
		if (dist + radius <= circle.radius)
			return circle;
		
		T theta = T(1) / T(2) + (circle.radius - radius) / (T(2) * dist);

		T cen = center.Lerp(circle.center, theta);
		T rad = (dist + radius + circle.radius) / T(2);
		return { cen, rad };
	}
}
