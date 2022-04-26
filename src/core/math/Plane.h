#pragma once
#include "core/MinInclude.h"
#include "FwdDecl.h"
#include "Concepts.h"
#include "Vec3.h"

namespace Onca::Math
{
	template<Numeric T>
	struct Plane
	{
		Vec3<T> normal;    ///< Plane normal
		T       planeDist; ///< Plane's distance from origin

		/**
		 * Create a plane at the origin with it's normal pointing up
		 */
		constexpr Plane() noexcept;
		/**
		 * Create a plane as a given distance from the center
		 * \param[in] normal Normal
		 * \param[in] planeDist Distance from center
		 */
		constexpr Plane(const Vec3<T>& normal, T planeDist) noexcept;
		/**
		 * Create a plane passing through a given point
		 * \param[in] normal Normal
		 * \param[in] point Point
		 */
		constexpr Plane(const Vec3<T>& normal, const Vec3<T>& point) noexcept;

		constexpr auto operator==(const Plane& other) const noexcept -> bool;
		constexpr auto operator!=(const Plane& other) const noexcept -> bool;

		/**
		 * Compare 2 planes using an epsilon
		 * \param[in] other Plane to compare with
		 * \param[in] e Epsilon
		 * \return Whether the planes are equal
		 */
		constexpr auto Compare(const Plane& other, T e = Consts::MathEpsilon<T>) const noexcept -> bool;

		/**
		 * Calculate the distance to the point
		 * \param[in] point Point
		 * \return Distance to the point
		 */
		constexpr auto Distance(const Vec3<T>& point) const noexcept -> T;
		/**
		 * Check if the point is above the plane
		 * \param[in] point Point
		 * \return Whether the point is above the plane
		 */
		constexpr auto IsAbove(const Vec3<T>& point) const noexcept -> T;
	};

}
