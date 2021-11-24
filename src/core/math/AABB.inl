#pragma once

#if __RESHARPER__
#include "AABB.h"
#endif

#include "Constants.h"
#include "MathUtils.h"

namespace Core::Math
{
	template <Numeric T>
	constexpr AABB<T>::AABB() noexcept
		: min(Consts::LowestVal<T>, Consts::LowestVal<T>, Consts::LowestVal<T>)
		, max(Consts::MaxVal<T>, Consts::MaxVal<T>, Consts::MaxVal<T>)
	{
	}

	template <Numeric T>
	constexpr AABB<T>::AABB(const Vec3<T>& min, const Vec3<T>& max) noexcept
		: min(min)
		, max(max)
	{
	}

	template <Numeric T>
	constexpr AABB<T>::AABB(T top, T right, T back, T bottom, T left, T front) noexcept
		: max(bottom, left, back)
		, min(top, right, front)
	{
	}

	template <Numeric T>
	constexpr auto AABB<T>::operator==(const AABB& other) const noexcept -> bool
	{
		return min == other.min && max == other.max;
	}

	template <Numeric T>
	constexpr auto AABB<T>::operator!=(const AABB& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <Numeric T>
	constexpr auto AABB<T>::Compare(const AABB& other, T e) const noexcept -> bool
	{
		return min.Compare(other.min, e) && max.Compare(other.max, e);
	}

	template <Numeric T>
	constexpr auto AABB<T>::Size() const noexcept -> Vec3<T>
	{
		return max - min;
	}

	template <Numeric T>
	constexpr auto AABB<T>::SetSize(const Vec3<T>& size) noexcept -> void
	{
		Vec3<T> center = Center();
		Vec3<T> halfExtend = size / T(2);
		min = center - halfExtend;
		max = center + halfExtend;
	}

	template <Numeric T>
	constexpr auto AABB<T>::Center() const noexcept -> Vec3<T>
	{
		return min + Size() / T(2);
	}

	template <Numeric T>
	constexpr auto AABB<T>::SetCenter(const Vec3<T>& center) noexcept -> void
	{
		Vec3<T> halfExtend = Size() / T(2);
		min = center - halfExtend;
		max = center + halfExtend;
	}

	template <Numeric T>
	constexpr auto AABB<T>::Volume() const noexcept -> T
	{
		Vec3<T> size = Size();
		return size.x * size.y * size.z;
	}

	template <Numeric T>
	constexpr auto AABB<T>::ContainsPoint(const Vec3<T>& point) const noexcept -> bool
	{
		return point.x > min.x && point.x < max.x &&
			   point.y > min.y && point.y < max.y &&
			   point.z > min.z && point.z < max.z;
	}

	template <Numeric T>
	constexpr auto AABB<T>::Overlaps(const AABB& aabb) const noexcept -> bool
	{
		return aabb.max.x > min.x && aabb.max.y > min.y && aabb.max.z > min.z &&
			   aabb.min.x < max.x && aabb.min.y < max.y && aabb.min.z < max.z;
	}

	template <Numeric T>
	constexpr auto AABB<T>::OverlapsInclusive(const AABB& aabb) const noexcept -> bool
	{
		return aabb.max.x >= min.x && aabb.max.y >= min.y && aabb.max.z >= min.z &&
			   aabb.min.x <= max.x && aabb.min.y <= max.y && aabb.min.z <= max.z;
	}

	template <Numeric T>
	constexpr auto AABB<T>::Encloses(const AABB& aabb) const noexcept -> bool
	{
		return aabb.min.x >= max.x && aabb.max.x <= min.x &&
			   aabb.min.y >= max.y && aabb.max.y <= min.y &&
			   aabb.min.z >= max.z && aabb.max.z <= min.z;
	}

	template <Numeric T>
	constexpr auto AABB<T>::DistanceSq(const Vec3<T>& point) const noexcept -> T
	{
		Vec3<T> minDiff = min - point; // only positive if point < min
		Vec3<T> maxDiff = point - max; // only positive if max < point
		Vec3<T> diff = minDiff.Max(maxDiff).Max({});
		return diff.LenSq();
	}

	template <Numeric T>
	constexpr auto AABB<T>::DistanceSq(const AABB& aabb) const noexcept -> T
	{
		Vec3<T> diff0 = min - aabb.max;
		Vec3<T> diff1 = aabb.min - max;
		Vec3<T> diff = diff0.Max(diff1).Max({});
		return diff.LenSq();
	}

	template <Numeric T>
	constexpr auto AABB<T>::Expand(const Vec3<T>& halfExtend) noexcept -> AABB&
	{
		min -= halfExtend;
		max += halfExtend;
		return *this;
	}

	template <Numeric T>
	constexpr auto AABB<T>::Expanded(const Vec3<T>& halfExtend) const noexcept -> AABB
	{
		return AABB{ min - halfExtend, max + halfExtend };
	}

	template <Numeric T>
	constexpr auto AABB<T>::Merge(const AABB& aabb) noexcept -> AABB&
	{
		min = Min(min, aabb.min);
		max = Max(max, aabb.max);
		return *this;
	}

	template <Numeric T>
	constexpr auto AABB<T>::Merged(const AABB& aabb) const noexcept -> AABB
	{
		return { Min(min, aabb.min), Max(max, aabb.max) };
	}

	template <Numeric T>
	constexpr auto AABB<T>::Move(const Vec3<T>& delta) noexcept -> AABB&
	{
		min += delta;
		max += delta;
		return *this;
	}

	template <Numeric T>
	constexpr auto AABB<T>::Moved(const Vec3<T>& delta) const noexcept -> AABB
	{
		return { min + delta, max + delta };
	}
}
