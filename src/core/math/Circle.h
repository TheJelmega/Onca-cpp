#pragma once
#include "core/MinInclude.h"
#include "FwdDecl.h"
#include "Concepts.h"
#include "Vec2.h"

namespace Onca::Math
{
	template<Numeric T>
	struct Circle
	{
		Vec2<T> center; ///< Circle center
		T       radius; ///< Circle radius

		/**z
		 * Create a unit circle at the origin
		 */
		constexpr Circle() noexcept;
		/**
		 * Create a circle
		 * \param[in] center Center
		 * \param[in] radius Radius
		 */
		constexpr Circle(const Vec2<T>& center, T radius) noexcept;

		constexpr auto operator==(const Circle& other) const noexcept -> bool;
		constexpr auto operator!=(const Circle& other) const noexcept -> bool;

		/**
		 * Compare 2 circles using an epsilon
		 * \param[in] other Circle to compare with
		 * \param[in] e Epsilon
		 * \return Whether the circles are equal
		 */
		constexpr auto Compare(const Circle& other, T e = Consts::MathEpsilon<T>) const noexcept -> bool;

		/**
		 * Get the area of the Circle
		 * \return Area of the rect
		 */
		constexpr auto Area() const noexcept -> T;

		/**
		 * Check if the point is in the Circle
		 * \param[in] point Point
		 * \return Whether the point is in the Circle
		 */
		constexpr auto ContainsPoint(const Vec2<T>& point) const noexcept -> bool;
		/**
		 * Check if the circles overlap
		 * \param[in] circle Other Circle
		 * \return whether the circles overlap
		 * \note Touching is NOT counted as overlapping
		 */
		constexpr auto Overlaps(const Circle& circle) const noexcept -> bool;
		/**
		 * Check if the circles overlap
		 * \param[in] circle Other Circle
		 * \return whether the circles overlap
		 * \note Touching is also counted as overlapping
		 */
		constexpr auto OverlapsInclusive(const Circle& circle) const noexcept -> bool;
		/**
		 * Check if the Circle encloses the other Circle (completely contains the other Circle)
		 * \param[in] circle Other Circle
		 * \return Whether the Circle encloses the other Circle
		 */
		constexpr auto Encloses(const Circle& circle) const noexcept -> bool;

		/**
		 * Calculate the square distance to a point
		 * \param[in] point Point
		 * \return Square distance to point
		 */
		constexpr auto DistanceSq(const Vec2<T>& point) const noexcept -> T;
		/**
		 * Calculate the square distance to another Circle
		 * \param[in] circle Other Circle
		 * \return Square distance to another Circle
		 */
		constexpr auto DistanceSq(const Circle& circle) const noexcept -> T;

		/**
		 * Merge with another Circle into a Circle with a size that encompasses the area of both Circleangles
		 * \param[in] circle Other Circle
		 * \return Reference to the Circle
		 */
		constexpr auto Merge(const Circle& circle) noexcept -> Circle&;
		/**
		 * Get a Circle with a size that encompasses the area of both Circleangles
		 * \param[in] circle Other Circle
		 * \return Merged Circle
		 */
		constexpr auto Merged(const Circle& circle) const noexcept -> Circle;
	};
}
