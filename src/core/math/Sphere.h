#pragma once
#include "core/MinInclude.h"
#include "FwdDecl.h"
#include "Concepts.h"
#include "Vec3.h"

namespace Onca::Math
{
	template<Numeric T>
	struct Sphere
	{
		Vec3<T> center; ///< Sphere center
		T       radius; ///< Sphere radius

		/**
		 * Create a unit sphere at the origin
		 */
		constexpr Sphere() noexcept;
		/**
		 * Create a sphere
		 * \param[in] center Center
		 * \param[in] radius Radius
		 */
		constexpr Sphere(const Vec3<T>& center, T radius) noexcept;

		constexpr auto operator==(const Sphere& other) const noexcept -> bool;
		constexpr auto operator!=(const Sphere& other) const noexcept -> bool;

		/**
		 * Compare 2 spheres using an epsilon
		 * \param[in] other Sphere to compare with
		 * \param[in] e Epsilon
		 * \return Whether the spheres are equal
		 */
		constexpr auto Compare(const Sphere& other, T e = Consts::MathEpsilon<T>) const noexcept -> bool;

		/**
		 * Get the volume of the AABB
		 * \return Volume of the AABB
		 */
		constexpr auto Volume() const noexcept -> T;

		/**
		 * Check if the point is in the Sphere
		 * \param[in] point Point
		 * \return Whether the point is in the Sphere
		 */
		constexpr auto ContainsPoint(const Vec3<T>& point) const noexcept -> bool;
		/**
		 * Check if the spheres overlap
		 * \param[in] sphere Other Sphere
		 * \return whether the spheres overlap
		 * \note Touching is NOT counted as overlapping
		 */
		constexpr auto Overlaps(const Sphere& sphere) const noexcept -> bool;
		/**
		 * Check if the spheres overlap
		 * \param[in] sphere Other Sphere
		 * \return whether the spheres overlap
		 * \note Touching is also counted as overlapping
		 */
		constexpr auto OverlapsInclusive(const Sphere& sphere) const noexcept -> bool;
		/**
		 * Check if the Sphere encloses the other Sphere (completely contains the other Sphere)
		 * \param[in] sphere Other Sphere
		 * \return Whether the Sphere encloses the other Sphere
		 */
		constexpr auto Encloses(const Sphere& sphere) const noexcept -> bool;

		/**
		 * Calculate the square distance to a point
		 * \param[in] point Point
		 * \return Square distance to point
		 */
		constexpr auto DistanceSq(const Vec3<T>& point) const noexcept -> T;
		/**
		 * Calculate the square distance to another Sphere
		 * \param[in] sphere Other Sphere
		 * \return Square distance to another Sphere
		 */
		constexpr auto DistanceSq(const Sphere& sphere) const noexcept -> T;

		/**
		 * Merge with another Sphere into a Sphere with a size that encompasses the area of both Sphereangles
		 * \param[in] sphere Other Sphere
		 * \return Reference to the Sphere
		 */
		constexpr auto Merge(const Sphere& sphere) noexcept -> Sphere&;
		/**
		 * Get a Sphere with a size that encompasses the area of both Sphereangles
		 * \param[in] sphere Other Sphere
		 * \return Merged Sphere
		 */
		constexpr auto Merged(const Sphere& sphere) const noexcept -> Sphere;
	};
}
