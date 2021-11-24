#pragma once
#include "core/MinInclude.h"
#include "Concepts.h"
#include "FwdDecl.h"
#include "Vec3.h"
#include "VecConsts.h"

namespace Core::Math
{
	/**
	 * Ray
	 * \tparam T Numeric type
	 */
	template<Numeric T>
	struct Ray
	{
		Vec3<T> orig; ///< Origin of the ray
		T min;        ///< Minimum length on the ray
		Vec3<T> dir;  ///< Direction of the ray
		T max;        ///< Maximum length on the ray

		/**
		 * Create a ray at the origin facing forward
		 */
		constexpr Ray() noexcept;
		/**
		 * Create a ray with an origin and direction
		 * \param[in] orig Ray origin
		 * \param[in] dir Ray direction
		 */
		constexpr Ray(const Vec3<T>& orig, const Vec3<T>& dir = Consts::Forward<Vec3<T>>) noexcept;
		/**
		 * Create a ray with an origin, direction, and minimum and maximum values on the ray
		 * \param[in] orig Ray origin
		 * \param[in] dir Ray direction
		 * \param[in] min Minimum value on the ray
		 * \param[in] max Maximum value on the ray
		 */
		constexpr Ray(const Vec3<T>& orig, const Vec3<T>& dir, T min, T max) noexcept;

		/**
		 * Calculate the point on the ray at a certain value
		 * \param[in] rayParam Ray parameter
		 * \return Point
		 */
		constexpr auto PointAt(T rayParam) const noexcept -> Vec3<T>;
		/**
		 * Calculate the clamped ray parameter of the closest point to point, on the ray
		 * \param[in] point Point to calculate ray parameter for
		 * \return Ray parameter
		 */
		constexpr auto GetRayParam(const Vec3<T>& point) const noexcept -> T;
		/**
		 * Calculate the closest point on the ray to the given point
		 * \param[in] point Point
		 * \return Closest point
		 */
		constexpr auto ClosestPoint(const Vec3<T>& point) const noexcept -> Vec3<T>;
		/**
		 * Calculate the square distance to the closest point on the ray, to a given point
		 * \param[in] point Point
		 * \return Square distance to ray
		 */
		constexpr auto DistSq(const Vec3<T> point) const noexcept -> T;

		/**
		 * Clamp the ray parameter between the minimum and maximum value possible on the ray
		 * \param[in] rayParam Ray parameter to clamp
		 * \return Clamped Ray parameter
		 */
		constexpr auto ClampRayParam(T rayParam) const noexcept -> T;
	};
}
