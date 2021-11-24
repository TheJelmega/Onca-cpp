#pragma once

#ifdef __RESHARPER__
#include "Ray.h"
#endif

#include "Constants.h"

namespace Core::Math
{
	template <Numeric T>
	constexpr Ray<T>::Ray() noexcept
		: orig()
		, min(Consts::LowestVal<T>)
		, dir(Consts::Forward<Vec3<T>>)
		, max(Consts::MaxVal<T>)
	{
	}

	template <Numeric T>
	constexpr Ray<T>::Ray(const Vec3<T>& orig, const Vec3<T>& dir) noexcept
		: orig(orig)
		, min(Consts::LowestVal<T>)
		, dir(dir.Normalized())
		, max(Consts::MaxVal<T>)
	{
	}

	template <Numeric T>
	constexpr Ray<T>::Ray(const Vec3<T>& orig, const Vec3<T>& dir, T min, T max) noexcept
		: orig(orig)
		, min(min)
		, dir(dir)
		, max(max)
	{
	}

	template <Numeric T>
	constexpr auto Ray<T>::PointAt(T rayParam) const noexcept -> Vec3<T>
	{
		return orig + dir * rayParam;
	}

	template <Numeric T>
	constexpr auto Ray<T>::GetRayParam(const Vec3<T>& point) const noexcept -> T
	{
		T rayParam = orig.Dot(point - orig);
		return Clamp(rayParam, min, max);
	}

	template <Numeric T>
	constexpr auto Ray<T>::ClosestPoint(const Vec3<T>& point) const noexcept -> Vec3<T>
	{
		T rayParam = GetRayParam(point);
		return orig + dir * rayParam;
	}

	template <Numeric T>
	constexpr auto Ray<T>::DistSq(const Vec3<T> point) const noexcept -> T
	{
		Vec3<T> closestPoint = ClosestPoint(point);
		return (point - closestPoint).LenSq();
	}

	template <Numeric T>
	constexpr auto Ray<T>::ClampRayParam(T rayParam) const noexcept -> T
	{
		return Clamp(rayParam, min, max);
	}
}
