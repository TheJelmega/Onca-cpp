#pragma once

#if __RESHARPER__
#include "Rect.h"
#endif

#include "Constants.h"
#include "MathUtils.h"

namespace Core::Math
{
	template <Numeric T>
	constexpr Rect<T>::Rect() noexcept
		: min(Consts::LowestVal<T>, Consts::LowestVal<T>)
		, max(Consts::MaxVal<T>, Consts::MaxVal<T>)
	{
	}

	template <Numeric T>
	constexpr Rect<T>::Rect(const Vec2<T>& min, const Vec2<T>& max) noexcept
		: min(min)
		, max(max)
	{
	}

	template <Numeric T>
	constexpr Rect<T>::Rect(T top, T right, T bottom, T left) noexcept
		: max(bottom, left)
		, min(top, right)
	{
	}

	template <Numeric T>
	constexpr auto Rect<T>::operator==(const Rect& other) const noexcept -> bool
	{
		return min == other.min && max == other.max;
	}

	template <Numeric T>
	constexpr auto Rect<T>::operator!=(const Rect& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <Numeric T>
	constexpr auto Rect<T>::Compare(const Rect& other, T e) const noexcept -> bool
	{
		return min.Compare(other.min, e) && max.Compare(other.max, e);
	}

	template <Numeric T>
	constexpr auto Rect<T>::Size() const noexcept -> Vec2<T>
	{
		return max - min;
	}

	template <Numeric T>
	constexpr void Rect<T>::SetSize(const Vec2<T>& size) noexcept
	{
		Vec2<T> center = Center();
		Vec2<T> halfExtend = size / T(2);
		min = center - halfExtend;
		max = center + halfExtend;
	}

	template <Numeric T>
	constexpr auto Rect<T>::Center() const noexcept -> Vec2<T>
	{
		return min + Size() / T(2);
	}

	template <Numeric T>
	constexpr void Rect<T>::SetCenter(const Vec2<T>& center) noexcept
	{
		Vec2<T> halfExtend = Size() / T(2);
		min = center - halfExtend;
		max = center + halfExtend;
	}

	template <Numeric T>
	constexpr auto Rect<T>::Area() const noexcept -> T
	{
		Vec2<T> size = Size();
		return size.x * size.y;
	}

	template <Numeric T>
	constexpr auto Rect<T>::ContainsPoint(const Vec2<T>& point) const noexcept -> bool
	{
		return point.x > min.x && point.x < max.x &&
			   point.y > min.y && point.y < max.y;
	}

	template <Numeric T>
	constexpr auto Rect<T>::Overlaps(const Rect& rect) const noexcept -> bool
	{
		return rect.max.x > min.x && rect.max.y > min.y &&
			   rect.min.x < max.x && rect.min.y < max.y;
	}

	template <Numeric T>
	constexpr auto Rect<T>::OverlapsInclusive(const Rect& rect) const noexcept -> bool
	{
		return rect.max.x >= min.x && rect.max.y >= min.y &&
			   rect.min.x <= max.x && rect.min.y <= max.y;
	}

	template <Numeric T>
	constexpr auto Rect<T>::Encloses(const Rect& rect) const noexcept -> bool
	{
		return rect.min.x >= max.x && rect.max.x <= min.x &&
			   rect.min.y >= max.y && rect.max.y <= min.y;
	}

	template <Numeric T>
	constexpr auto Rect<T>::DistanceSq(const Vec2<T>& point) const noexcept -> T
	{
		Vec2<T> minDiff = min - point; // only positive if point < min
		Vec2<T> maxDiff = point - max; // only positive if max < point
		Vec2<T> diff = minDiff.Max(maxDiff).Max({});
		return diff.LenSq();
	}

	template <Numeric T>
	constexpr auto Rect<T>::DistanceSq(const Rect& rect) const noexcept -> T
	{
		Vec2<T> diff0 = min - rect.max;
		Vec2<T> diff1 = rect.min - max;
		Vec2<T> diff = diff0.Max(diff1).Max({});
		return diff.LenSq();
	}

	template <Numeric T>
	constexpr auto Rect<T>::Expand(const Vec2<T>& halfExtend) noexcept -> Rect&
	{
		min -= halfExtend;
		max += halfExtend;
		return *this;
	}

	template <Numeric T>
	constexpr auto Rect<T>::Expanded(const Vec2<T>& halfExtend) const noexcept -> Rect
	{
		return Rect{ min - halfExtend, max + halfExtend };
	}

	template <Numeric T>
	constexpr auto Rect<T>::Merge(const Rect& rect) noexcept -> Rect&
	{
		min = Min(min, rect.min);
		max = Max(max, rect.max);
		return *this;
	}

	template <Numeric T>
	constexpr auto Rect<T>::Merged(const Rect& rect) const noexcept -> Rect
	{
		return { Min(min, rect.min), Max(max, rect.max) };
	}

	template <Numeric T>
	constexpr auto Rect<T>::Move(const Vec2<T>& delta) noexcept -> Rect&
	{
		min += delta;
		max += delta;
		return *this;
	}

	template <Numeric T>
	constexpr auto Rect<T>::Moved(const Vec2<T>& delta) const noexcept -> Rect
	{
		return { min + delta, max + delta };
	}
}
