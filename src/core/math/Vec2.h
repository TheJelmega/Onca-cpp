#pragma once
#include "core/MinInclude.h"
#include "Concepts.h"
#include "FwdDecl.h"
#include "MathUtils.h"

namespace Onca::Math
{
	/**
	 * 2D vector
	 * \tparam Ty Component type
	 */
	template<Numeric Ty>
	struct Vec2
	{
		constexpr static u8 X = 0;
		constexpr static u8 Y = 1;
		constexpr static u8 U = X;
		constexpr static u8 V = Y;
		constexpr static u8 S = X;
		constexpr static u8 T = Y;

		union
		{
			struct { Ty x, y; };
			struct { Ty u, v; };
			struct { Ty s, t; };
			Ty data[2];
		};

		/**
		* Create a default Vec2 (0, 0)
		*/
		constexpr Vec2() noexcept;
		/**
		* Create a Vec2 with all components set to a value
		* \tparam A Type of value
		* \param[in] val Value
		*/
		template<ConvertableTo<Ty> A>
		explicit constexpr Vec2(A val) noexcept;
		/**
		* Create a Vec2 with the given components
		* \tparam A Type of x component
		* \tparam B Type of y component
		* \param[in] x X component
		* \param[in] y Y component
		*/
		template<ConvertableTo<Ty> A, ConvertableTo<Ty> B>
		constexpr Vec2(A x, B y) noexcept
			: x(Ty(x)), y(Ty(y))
		{
		}
		/**
		* Create a Vec2 from another Vec2
		* \param[in] other Other Vec2
		*/

		constexpr Vec2(const Vec2& other) noexcept;
		/**
		* Create a Vec2 from another Vec2
		* \tparam A Type of other Vec2's components
		* \param[in] other Other Vec2
		*/
		template<ConvertableTo<Ty> A>
		constexpr Vec2(const Vec2<A>& other) noexcept;
		/**
		* Create a Vec2 from a moved Vec2
		* \param[in] other Other Vec2
		*/
		constexpr Vec2(Vec2&& other) noexcept;

		constexpr auto operator=(const Vec2& other) noexcept -> Vec2&;
		template <ConvertableTo<Ty> A>
		constexpr auto operator=(const Vec2<A>& other) noexcept -> Vec2&;
		constexpr auto operator=(Vec2&& other) noexcept -> Vec2&;

		constexpr auto operator++() noexcept -> Vec2&;
		constexpr auto operator++(int) noexcept -> Vec2;

		constexpr auto operator--() noexcept -> Vec2&;
		constexpr auto operator--(int) noexcept -> Vec2;

		constexpr auto operator+() const noexcept -> Vec2;
		constexpr auto operator-() const noexcept -> Vec2;
		constexpr auto operator~() const noexcept -> Vec2 requires Invertable<Ty>;

		constexpr auto operator+ (const Vec2& other) const noexcept -> Vec2;
		constexpr auto operator- (const Vec2& other) const noexcept -> Vec2;
		constexpr auto operator* (const Vec2& other) const noexcept -> Vec2;
		constexpr auto operator/ (const Vec2& other) const noexcept -> Vec2;
		constexpr auto operator% (const Vec2& other) const noexcept -> Vec2 requires HasModulo<Ty>;
		constexpr auto operator| (const Vec2& other) const noexcept -> Vec2 requires Orable<Ty>;
		constexpr auto operator^ (const Vec2& other) const noexcept -> Vec2 requires Xorable<Ty>;
		constexpr auto operator& (const Vec2& other) const noexcept -> Vec2 requires Andable<Ty>;
		constexpr auto operator<<(const Vec2& other) const noexcept -> Vec2 requires BitShiftable<Ty>;
		constexpr auto operator>>(const Vec2& other) const noexcept -> Vec2 requires BitShiftable<Ty>;

		template<ConvertableTo<Ty> A>
		constexpr auto operator+ (A val) const noexcept -> Vec2;
		template<ConvertableTo<Ty> A>
		constexpr auto operator- (A val) const noexcept -> Vec2;
		template<ConvertableTo<Ty> A>
		constexpr auto operator* (A val) const noexcept -> Vec2;
		template<ConvertableTo<Ty> A>
		constexpr auto operator/ (A val) const noexcept -> Vec2;
		template<ConvertableTo<Ty> A>
		constexpr auto operator% (A val) const noexcept -> Vec2 requires HasModulo<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator| (A val) const noexcept -> Vec2 requires Orable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator^ (A val) const noexcept -> Vec2 requires Xorable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator& (A val) const noexcept -> Vec2 requires Andable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator<<(A val) const noexcept -> Vec2 requires BitShiftable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator>>(A val) const noexcept -> Vec2 requires BitShiftable<Ty>;

		constexpr auto operator+= (const Vec2& other) noexcept -> Vec2&;
		constexpr auto operator-= (const Vec2& other) noexcept -> Vec2&;
		constexpr auto operator*= (const Vec2& other) noexcept -> Vec2&;
		constexpr auto operator/= (const Vec2& other) noexcept -> Vec2&;
		constexpr auto operator%= (const Vec2& other) noexcept -> Vec2& requires HasModulo<Ty>;
		constexpr auto operator|= (const Vec2& other) noexcept -> Vec2& requires Orable<Ty>;
		constexpr auto operator^= (const Vec2& other) noexcept -> Vec2& requires Xorable<Ty>;
		constexpr auto operator&= (const Vec2& other) noexcept -> Vec2& requires Andable<Ty>;
		constexpr auto operator<<=(const Vec2& other) noexcept -> Vec2& requires BitShiftable<Ty>;
		constexpr auto operator>>=(const Vec2& other) noexcept -> Vec2& requires BitShiftable<Ty>;

		template<ConvertableTo<Ty> A>
		constexpr auto operator+= (A val) noexcept -> Vec2&;
		template<ConvertableTo<Ty> A>
		constexpr auto operator-= (A val) noexcept -> Vec2&;
		template<ConvertableTo<Ty> A>
		constexpr auto operator*= (A val) noexcept -> Vec2&;
		template<ConvertableTo<Ty> A>
		constexpr auto operator/= (A val) noexcept -> Vec2&;
		template<ConvertableTo<Ty> A>
		constexpr auto operator%= (A val) noexcept -> Vec2& requires HasModulo<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator|= (A val) noexcept -> Vec2& requires Orable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator^= (A val) noexcept -> Vec2& requires Xorable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator&= (A val) noexcept -> Vec2& requires Andable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator<<=(A val) noexcept -> Vec2& requires BitShiftable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator>>=(A val) noexcept -> Vec2& requires BitShiftable<Ty>;

		constexpr auto operator[](usize idx) noexcept -> Ty&;
		constexpr auto operator[](usize idx) const noexcept -> Ty;

		constexpr auto operator==(const Vec2& other) const noexcept -> bool;
		constexpr auto operator!=(const Vec2& other) const noexcept -> bool;

		/**
		 * Compare 2 vectors using an epsilon
		 * \param[in] other Vector to compare with
		 * \param[in] e Epsilon
		 * \return Whether the vectors are equal
		 */
		constexpr auto Compare(const Vec2& other, Ty e = Math::Consts::MathEpsilon<Ty>) const noexcept -> bool;

		/**
		 * Check if all components are exactly 0
		 * \return Whether all components are exactly 0
		 */
		constexpr auto IsZero() const noexcept -> bool;
		/**
		 * Check if all components are nearly 0 using an epsilon
		 * \param[in] e Epsilon
		 * \return Whether all components are nearly 0 using an epsilon
		 */
		constexpr auto IsNearlyZero(Ty e = Math::Consts::MathEpsilon<Ty>) const noexcept -> bool;
		/**
		 * Check if the vector is uniform (all elements are the same)
		 * \param[in] e Epsilon
		 * \return Whether the vector is uniform
		 */
		constexpr auto IsUniform(Ty e = Math::Consts::MathEpsilon<Ty>) const noexcept -> bool;

		/**
		 * Get the maximum component in the vector
		 * \return Maximum component
		 */
		constexpr auto MinComponent() const noexcept -> Ty;
		/**
		 * Get the maximum component in the vector
		 * \return Maximum component
		 */
		constexpr auto MinAbsComponent() const noexcept -> Ty;
		/**
		 * Get the minimum absolute component in the vector
		 * \return Minimum absolute component
		 */
		constexpr auto MaxComponent() const noexcept -> Ty;
		/**
		 * Get the minimum absolute component in the vector
		 * \return Minimum absolute component
		 */
		constexpr auto MaxAbsComponent() const noexcept -> Ty;
		/**
		 * Get the component-wise minimum
		 * \param[in] other Other vector
		 * \return Vector with component-wise minimum
		 */
		constexpr auto Min(const Vec2& other) const noexcept -> Vec2;
		/**
		 * Get the component-wise maximum
		 * \param[in] other Other vector
		 * \return Vector with component-wise maximum
		 */
		constexpr auto Max(const Vec2& other) const noexcept -> Vec2;

		/**
		 * Clamp the components between 2 values
		 * \tparam A Numeric type
		 * \tparam B Numeric type
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Reference to the Vector
		 */
		template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
		constexpr auto Clamp(A min, B max) noexcept -> Vec2&;
		/**
		 * Clamp the components between the corresponding components of the given vectors
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Reference to the Vector
		 */
		constexpr auto Clamp(const Vec2& min, const Vec2& max) noexcept -> Vec2&;
		/**
		 * Get a vector with its components clamped between 2 values
		 * \tparam A Numeric type
		 * \tparam B Numeric type
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Clamped vector
		 */
		template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
		constexpr auto Clamped(A min, B max) const noexcept -> Vec2;
		/**
		 * Get a vector with its components clamped between the corresponding components of the given vectors
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Clamped vector
		 */
		constexpr auto Clamped(const Vec2& min, const Vec2& max) const noexcept -> Vec2;

		/**
		 * Clamp the length between 2 values
		 * \tparam A Numeric type
		 * \tparam B Numeric type
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Reference to the Vector
		 */
		template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
		constexpr auto ClampLen(A min, B max) noexcept -> Vec2&;
		/**
		 * Get a vector with its length clamped between 2 values
		 * \tparam A Numeric type
		 * \tparam B Numeric type
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Clamped vector
		 */
		template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
		constexpr auto ClampedLen(A min, B max) const noexcept -> Vec2;

		/**
		 * Saturate the components (clamp between 0 and 1)
		 * \return Reference to the Vector
		 */
		constexpr auto Saturate() noexcept -> Vec2&;
		/**
		 * Get a vector with saturated components (between 0 and 1)
		 * \return Vector with saturated components
		 */
		constexpr auto Saturated() const noexcept -> Vec2;

		/**
		 * Snap each component to the nearest multiple of the given step size
		 * \param[in] stepSize Size on to which elements should be snapped
		 * \return Reference to the Vector
		 */
		constexpr auto Snap(Ty stepSize) noexcept -> Vec2&;
		/**
		 * Get a vector with each component snapped to the nearest multiple of the given step size
		 * \param[in] stepSize Size on to which elements should be snapped
		 * \return Snapped vector
		 */
		constexpr auto Snapped(Ty stepSize) const noexcept -> Vec2;

		/**
		 * Linearly interpolate between 2 vectors
		 * \tparam A Interpolant type
		 * \param[in] other Other vector
		 * \param[in] i Interpolant
		 * \return Interpolated vector
		 */
		template <ConvertableTo<Ty> A>
		constexpr auto Lerp(const Vec2& other, A i) const noexcept -> Vec2;
		
		/**
		 * Get the square length of the vector
		 * \return Square length of the vector
		 */
		constexpr auto LenSq() const noexcept -> Ty;
		/**
		 * Get the length of the vector
		 * \return Length of the vector
		 */
		constexpr auto Len() const noexcept -> Ty;
		/**
		 * Get the square distance between this vector and another
		 * \param[in] other Vector to calculate distance to
		 * \return Square distance between this vector and another
		 */
		constexpr auto DistanceSq(const Vec2& other) const noexcept -> Ty;
		/**
		 * Get the distance between this vector and another
		 * \param[in] other Vector to calculate distance to
		 * \return Distance between this vector and another
		 */
		constexpr auto Distance(const Vec2& other) const noexcept -> Ty;

		/**
		 * Get the dot product of this vector and another
		 * \param[in] other Other vector
		 * \return Dot product of this vector and another
		 */
		constexpr auto Dot(const Vec2& other) const noexcept -> Ty;
		/**
		 * Get the cross product of this vector and another
		 * \param[in] other Other vector
		 * \return Cross product of this vector and another
		 */
		constexpr auto Cross(const Vec2& other) const noexcept -> Ty;
		/**
		 * Get a vector that is perpendicular to this vector (rotated clockwise)
		 * \return Vector that is perpendicular to this vector
		 */
		constexpr auto PerpendicularCW() const noexcept -> Vec2;
		/**
		 * Get a vector that is perpendicular to this vector (rotated counter-clockwise)
		 * \return Vector that is perpendicular to this vector
		 */
		constexpr auto PerpendicularCCW() const noexcept -> Vec2;

		/**
		 * Normalize the vector
		 * \return Reference to the Vector
		 */
		constexpr auto Normalize() noexcept -> Vec2&;
		/**
		 * Normalize the vector (no check for a 0 length is done)
		 * \return Reference to the Vector
		 */
		constexpr auto NormalizeUnsafe() noexcept -> Vec2&;
		/**
		 * Normalize the vector, or set it to 'safe' if the length is 0
		 * \return Reference to the Vector
		 */
		constexpr auto NormalizeSafe(const Vec2& safe) noexcept -> Vec2&;
		/**
		 * Get the normalized vector
		 * \return Normalized vector
		 */
		constexpr auto Normalized() const noexcept -> Vec2;
		/**
		 * Check if the vector is normalized
		 * \return Whether the vector is normalized
		 */
		constexpr auto IsNormalized(Ty e = Math::Consts::MathEpsilon<Ty>) const noexcept -> bool;

		/**
		 * Get the vector rotated by an angle
		 * \tparam A Type of quaternion components
		 * \tparam Kind Angle kind
		 * \param[in] angle Angle
		 * \return Rotated vector
		 */
		template<ConvertableTo<Ty> A, AngleKind Kind>
		constexpr auto Rotate(const Math::Angle<A, Kind>& angle) noexcept -> Vec2&;
		/**
		 * Get the vector rotated by an angle
		 * \tparam A Type of quaternion components
		 * \param[in] angle Angle
		 * \return Rotated vector
		 */
		template<ConvertableTo<Ty> A, AngleKind Kind>
		constexpr auto Rotated(const Math::Angle<A, Kind>& angle) const noexcept -> Vec2;

		/**
		 * Get the angle of the vector
		 * \return Angle of the Vector
		 */
		constexpr auto Angle() const noexcept -> Math::Angle<Ty, AngleKind::Radians>;
		/**
		 * Calculate the angle between 2 vectors (via shortest path)
		 * \param other Other vector
		 * \return Angle between vectors
		 */
		constexpr auto Angle(const Vec2& other) const noexcept -> Math::Angle<Ty, AngleKind::Radians>;
		/**
		 * Calculate the angle between 2 vectors (based on order)
		 * \param other Other vector
		 * \return Angle between vectors
		 */
		constexpr auto AngleFull(const Vec2& other) const noexcept -> Math::Angle<Ty, AngleKind::Radians>;

		/**
		 * Get or flip this vector, so it's pointing in the opposite direction of the incidence vector, relative to the normal
		 * \param[in] incidence Incidence factor
		 * \param[in] normal Normal
		 * \return Forward facing vector
		 */
		constexpr auto FaceForward(const Vec2& incidence, const Vec2& normal) const noexcept -> Vec2;
		/**
		 * Reflect a vector on a 'surface' with a normal
		 * \param[in] normal Normal of 'surface' to reflect on
		 * \return Reflected vector
		 */
		constexpr auto Reflect(const Vec2& normal) const noexcept -> Vec2;
		/**
		 * Refract the vector through a 'surface' with a given normal and eta
		 * \tparam A Eta type
		 * \param[in] normal Normal of the 'surface' to refract through
		 * \param[in] eta Ratio of indices of refraction at the surface interface (outgoing / incoming)
		 * \return Refracted vector
		 */
		template<ConvertableTo<Ty> A>
		constexpr auto Refract(const Vec2& normal, A eta) const noexcept -> Vec2;

		/**
		 * Get the direction and length of the vector
		 * \return Pair with the direction and length of the vector
		 */
		constexpr auto DirectionAndLength() const noexcept -> Pair<Vec2, Ty>;

		/**
		 * Get a vector with swizzled components
		 * \tparam Idx0 Index of first component
		 * \tparam Idx1 Index of second component
		 * \return Swizzled vector
		 */
		template<usize Idx0, usize Idx1>
		constexpr auto Swizzle() const noexcept -> Vec2;

		/**
		 * Get a vector with swizzled components
		 * \param[in] idx0 Index of first component
		 * \param[in] idx1 Index of second component
		 * \return Swizzled vector
		 */
		constexpr auto Swizzle(u8 idx0, u8 idx1) const noexcept -> Vec2;

#define SWIZZLE_DECL(xy, uv, st) \
		constexpr auto xy() const noexcept -> Vec2; \
		constexpr auto uv() const noexcept -> Vec2; \
		constexpr auto st() const noexcept -> Vec2;

		SWIZZLE_DECL(xx, uu, ss)
		SWIZZLE_DECL(xy, uv, st)
		SWIZZLE_DECL(yx, vu, ts)
		SWIZZLE_DECL(yy, vv, tt)

#undef SWIZZLE_DECL

		/**
		 * Get a vector with the absolute values of the components
		 * \return Vector with absolute components
		 */
		constexpr auto Abs() const noexcept -> Vec2;
		/**
		 * Get a vector with the ceil of the components
		 * \return Vector with the ceil of the components
		 */
		constexpr auto Ceil() const noexcept -> Vec2;
		/**
		 * Get a vector with the floor of the components
		 * \return Vector with the floor of the components
		 */
		constexpr auto Floor() const noexcept -> Vec2;
		/**
		 * Get a vector with the truncated values of the components
		 * \return Vector with truncated components
		 */
		constexpr auto Trunc() const noexcept -> Vec2;
		/**
		 * Get a vector with the rounded values of the components
		 * \return Vector with rounded components
		 */
		constexpr auto Round() const noexcept -> Vec2;
		/**
		 * Get a vector with the rounded values of the components, if the component ends on .5, round towards the nearest even integer value
		 * \return Vector with rounded components
		 */
		constexpr auto RoundEven() const noexcept -> Vec2;
		/**
		 * Get a vector with the sign of the components
		 * \return Vector with the sign of the components
		 */
		constexpr auto Sign() const noexcept -> Vec2;
		/**
		 * Get a vector with the fractional part of the components
		 * \return Vector with the fractional part of the components
		 */
		constexpr auto Fract() const noexcept -> Vec2;
	};

	template<Numeric T, ConvertableTo<T> A>
	constexpr auto operator*(A t, const Vec2<T>& vec) noexcept -> Vec2<T>;
	
}