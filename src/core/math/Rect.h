#pragma once
#include "core/MinInclude.h"
#include "FwdDecl.h"
#include "Concepts.h"
#include "Vec2.h"

namespace Onca::Math
{
	/**
	 * 2D axis-aligned rectangle
	 * \tparam T Numeric type
	 */
	template<Numeric T>
	struct Rect
	{
		Vec2<T> min; ///< Minimum values
		Vec2<T> max; ///< Maximum values

		/**
		 * Create a default rect ((-min, -min), (max, max))
		 */
		constexpr Rect() noexcept;
		/**
		 * Create a Rect with minimum and maximum values
		 * \param[in] min Minimum values
		 * \param[in] max Maximum values
		 */
		constexpr Rect(const Vec2<T>& min, const Vec2<T>& max) noexcept;
		/**
		 * Create a rect with the given values
		 * \param[in] top Top value
		 * \param[in] right Right
		 * \param[in] bottom Bottom
		 * \param[in] left Left
		 */
		constexpr Rect(T top, T right, T bottom, T left) noexcept;

		constexpr auto operator==(const Rect& other) const noexcept -> bool;
		constexpr auto operator!=(const Rect& other) const noexcept -> bool;

		/**
		 * Compare 2 rects using an epsilon
		 * \param[in] other Rect to compare with
		 * \param[in] e Epsilon
		 * \return Whether the rects are equal
		 */
		constexpr auto Compare(const Rect& other, T e = Consts::MathEpsilon<T>) const noexcept -> bool;

		/**
		 * Get the size of the rect
		 * \return Size of the rect
		 */
		constexpr auto Size() const noexcept -> Vec2<T>;
		/**
		 * Set the size of the Rect
		 * \param[in] size Size
		 */
		constexpr void SetSize(const Vec2<T>& size) noexcept;
		/**
		 * Get the center of the rect
		 * \return Center of the rect
		 */
		constexpr auto Center() const noexcept -> Vec2<T>;
		/**
		 * Set the center of the Rect
		 * \param[in] center Center
		 */
		constexpr void SetCenter(const Vec2<T>& center) noexcept;
		/**
		 * Get the area of the Rect
		 * \return Area of the Rect
		 */
		constexpr auto Area() const noexcept -> T;

		/**
		 * Check if the point is in the rect
		 * \param[in] point Point
		 * \return Whether the point is in the rect
		 */
		constexpr auto ContainsPoint(const Vec2<T>& point) const noexcept -> bool;
		/**
		 * Check if the rects overlap
		 * \param[in] rect Other rect
		 * \return whether the rects overlap
		 * \note Touching sides is NOT counted as overlapping
		 */
		constexpr auto Overlaps(const Rect& rect) const noexcept -> bool;
		/**
		 * Check if the rects overlap
		 * \param[in] rect Other rect
		 * \return whether the rects overlap
		 * \note Touching sides is also counted as overlapping
		 */
		constexpr auto OverlapsInclusive(const Rect& rect) const noexcept -> bool;
		/**
		 * Check if the Rect encloses the other Rect (completely contains the other Rect)
		 * \param[in] rect Other Rect
		 * \return Whether the Rect encloses the other Rect
		 */
		constexpr auto Encloses(const Rect& rect) const noexcept -> bool;

		/**
		 * Calculate the square distance to a point
		 * \param[in] point Point
		 * \return Square distance to point
		 */
		constexpr auto DistanceSq(const Vec2<T>& point) const noexcept -> T;
		/**
		 * Calculate the square distance to another Rect
		 * \param[in] rect Other rect
		 * \return Square distance to another rect
		 */
		constexpr auto DistanceSq(const Rect& rect) const noexcept -> T;


		/**
		 * Expand the size of the Rect in both directions by the given half-extend
		 * \param[in] halfExtend Half of the total extend
		 * \return Reference to the Rect
		 */
		constexpr auto Expand(const Vec2<T>& halfExtend) noexcept -> Rect&;
		/**
		 * Get a Rect expanded in both directions by the given half-extend
		 * \param[in] halfExtend Half of the total extend
		 * \return Expanded rect
		 */
		constexpr auto Expanded(const Vec2<T>& halfExtend) const noexcept -> Rect;

		/**
		 * Merge with another Rect into a Rect with a size that encompasses the area of both rectangles
		 * \param[in] rect Other rect
		 * \return Reference to the Rect
		 */
		constexpr auto Merge(const Rect& rect) noexcept -> Rect&;
		/**
		 * Get a rect with a size that encompasses the area of both rectangles
		 * \param[in] rect Other rect
		 * \return Merged rect
		 */
		constexpr auto Merged(const Rect& rect) const noexcept -> Rect;

		/**
		 * Move the Rect by a delta
		 * \param[in] delta Move delta
		 * \return Reference to the Rect
		 */
		constexpr auto Move(const Vec2<T>& delta) noexcept -> Rect&;
		/**
		 * Get a Rect moved by a delta
		 * \param[in] delta Move delta
		 * \return Moved rect
		 */
		constexpr auto Moved(const Vec2<T>& delta) const noexcept -> Rect;
	};
}
