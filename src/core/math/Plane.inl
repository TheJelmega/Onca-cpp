#pragma once

#if __RESHARPER__
#include "Plane.h"
#endif

#include "Constants.h"
#include "MathUtils.h"
#include "VecConsts.h"

namespace Core::Math
{

	template <Numeric T>
	constexpr Plane<T>::Plane() noexcept
		: normal(Consts::Up<Vec3<T>>)
		, planeDist(T(0))
	{
	}

	template <Numeric T>
	constexpr Plane<T>::Plane(const Vec3<T>& normal, T planeDist) noexcept
		: normal(normal.Normalized())
		, planeDist(planeDist)
	{
	}

	template <Numeric T>
	constexpr Plane<T>::Plane(const Vec3<T>& normal, const Vec3<T>& point) noexcept
		: normal(normal.Normalized())
	{
		planeDist = normal.Dot(point);
	}

	template <Numeric T>
	constexpr auto Plane<T>::operator==(const Plane& other) const noexcept -> bool
	{
		return normal == other.normal && planeDist == other.planeDist;
	}

	template <Numeric T>
	constexpr auto Plane<T>::operator!=(const Plane& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <Numeric T>
	constexpr auto Plane<T>::Compare(const Plane& other, T e) const noexcept -> bool
	{
		return normal.Compare(other.normal, e) && EpsilonCompare(planeDist, other.planeDist, e);
	}

	template <Numeric T>
	constexpr auto Plane<T>::Distance(const Vec3<T>& point) const noexcept -> T
	{
		return normal.Dot(point) - planeDist;
	}

	template <Numeric T>
	constexpr auto Plane<T>::IsAbove(const Vec3<T>& point) const noexcept -> T
	{
		return Distance(point) > 0;
	}
}
