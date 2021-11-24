#pragma once
#include "core/MinInclude.h"
#include "FwdDecl.h"
#include "Concepts.h"
#include "Vec3.h"

namespace Core::Math
{
	template<Numeric T>
	struct AABB
	{
		Vec3<T> min; ///< Minimum values
		Vec3<T> max; ///< Maximum values

		/**
		 * Create a default AABB ((-min, -min), (max, max))
		 */
		constexpr AABB() noexcept;
		/**
		 * Create a AABB with minimum and maximum values
		 * \param[in] min Minimum values
		 * \param[in] max Maximum values
		 */
		constexpr AABB(const Vec3<T>& min, const Vec3<T>& max) noexcept;
		/**
		 * Create a AABB with the given values
		 * \param[in] top Top value
		 * \param[in] right Right
		 * \param[in] back Back
		 * \param[in] bottom Bottom
		 * \param[in] left Left
		 * \param[in] front Front
		 */
		constexpr AABB(T top, T right, T back, T bottom, T left, T front) noexcept;

		constexpr auto operator==(const AABB& other) const noexcept -> bool;
		constexpr auto operator!=(const AABB& other) const noexcept -> bool;

		/**
		 * Compare 2 AABBs using an epsilon
		 * \param[in] other AABB to compare with
		 * \param[in] e Epsilon
		 * \return Whether the AABBs are equal
		 */
		constexpr auto Compare(const AABB& other, T e = Consts::MathEpsilon<T>) const noexcept -> bool;

		/**
		 * Get the size of the AABB
		 * \return Size of the AABB
		 */
		constexpr auto Size() const noexcept -> Vec3<T>;
		/**
		 * Set the size of the AABB
		 * \param[in] size Size
		 */
		constexpr auto SetSize(const Vec3<T>& size) noexcept -> void;
		/**
		 * Get the center of the AABB
		 * \return Center of the AABB
		 */
		constexpr auto Center() const noexcept -> Vec3<T>;
		/**
		 * Set the center of the AABB
		 * \param[in] center Center
		 */
		constexpr auto SetCenter(const Vec3<T>& center) noexcept -> void;
		/**
		 * Get the volume of the AABB
		 * \return Volume of the AABB
		 */
		constexpr auto Volume() const noexcept -> T;

		/**
		 * Check if the point is in the AABB
		 * \param[in] point Point
		 * \return Whether the point is in the AABB
		 */
		constexpr auto ContainsPoint(const Vec3<T>& point) const noexcept -> bool;
		/**
		 * Check if the AABBs overlap
		 * \param[in] aabb Other AABB
		 * \return whether the AABBs overlap
		 * \note Touching faces is NOT counted as overlapping
		 */
		constexpr auto Overlaps(const AABB& aabb) const noexcept -> bool;
		/**
		 * Check if the AABBs overlap
		 * \param[in] aabb Other AABB
		 * \return whether the AABBs overlap
		 * \note Touching faces is also counted as overlapping
		 */
		constexpr auto OverlapsInclusive(const AABB& aabb) const noexcept -> bool;
		/**
		 * Check if the AABB encloses the other AABB (completely contains the other Rect)
		 * \param[in] aabb Other AABB
		 * \return Whether the AABB encloses the other AABB
		 */
		constexpr auto Encloses(const AABB& aabb) const noexcept -> bool;

		/**
		 * Calculate the square distance to a point
		 * \param[in] point Point
		 * \return Square distance to point
		 */
		constexpr auto DistanceSq(const Vec3<T>& point) const noexcept -> T;
		/**
		 * Calculate the square distance to another AABB
		 * \param[in] aabb Other AABB
		 * \return Square distance to another AABB
		 */
		constexpr auto DistanceSq(const AABB& aabb) const noexcept -> T;


		/**
		 * Expand the size of the AABB in both diAABBions by the given half-extend
		 * \param[in] halfExtend Half of the total extend
		 * \return Reference to the AABB
		 */
		constexpr auto Expand(const Vec3<T>& halfExtend) noexcept -> AABB&;
		/**
		 * Get a AABB expanded in both diAABBions by the given half-extend
		 * \param[in] halfExtend Half of the total extend
		 * \return Expanded AABB
		 */
		constexpr auto Expanded(const Vec3<T>& halfExtend) const noexcept -> AABB;

		/**
		 * Merge with another AABB into a AABB with a size that encompasses the area of both AABBangles
		 * \param[in] aabb Other AABB
		 * \return Reference to the AABB
		 */
		constexpr auto Merge(const AABB& aabb) noexcept -> AABB&;
		/**
		 * Get a AABB with a size that encompasses the area of both AABBangles
		 * \param[in] aabb Other AABB
		 * \return Merged AABB
		 */
		constexpr auto Merged(const AABB& aabb) const noexcept -> AABB;

		/**
		 * Move the AABB by a delta
		 * \param[in] delta Move delta
		 * \return Reference to the AABB
		 */
		constexpr auto Move(const Vec3<T>& delta) noexcept -> AABB&;
		/**
		 * Get a AABB moved by a delta
		 * \param[in] delta Move delta
		 * \return Moved AABB
		 */
		constexpr auto Moved(const Vec3<T>& delta) const noexcept -> AABB;
	};
}
