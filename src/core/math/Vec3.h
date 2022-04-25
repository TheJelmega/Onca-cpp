#pragma once
#include "core/MinInclude.h"
#include "Concepts.h"
#include "FwdDecl.h"
#include "MathUtils.h"

namespace Core::Math
{
	/**
	 * 3D vector
	 * \tparam Ty Component type
	 */
	template<Numeric Ty>
	struct Vec3
	{
		constexpr static u8 X = 0;
		constexpr static u8 Y = 1;
		constexpr static u8 Z = 2;
		constexpr static u8 U = X;
		constexpr static u8 V = Y;
		constexpr static u8 W = Z;
		constexpr static u8 S = X;
		constexpr static u8 T = Y;
		constexpr static u8 Q = Z;

		union
		{
			struct { Ty x, y, z; };
			struct { Ty u, v, w; };
			struct { Ty s, t, q; };
			Ty data[3];
		};

		/**
		* Create a default Vec3 (0, 0, 0)
		*/
		constexpr Vec3() noexcept;
		/**
		* Create a Vec3 with all components set to a value
		* \tparam A Type of value
		* \param[in] val Value
		*/
		template<ConvertableTo<Ty> A>
		explicit constexpr Vec3(A val) noexcept;
		/**
		* Create a Vec3 with the given components
		* \tparam A Type of x component
		* \tparam B Type of y component
		* \tparam C Type of z component
		* \param[in] x X component
		* \param[in] y Y component
		* \param[in] z Z component
		*/
		template<ConvertableTo<Ty> A, ConvertableTo<Ty> B, ConvertableTo<Ty> C>
		constexpr Vec3(A x, B y, C z) noexcept;
		/**
		 * Create a Vec3 from a Vec2 and z component
		 * \tparam A Type of Vec2's components
		 * \tparam B Type of z component
		 * \param[in] vec Vec2 with x and y components
		 * \param[in] z Z component
		 */
		template<ConvertableTo<Ty> A, ConvertableTo<Ty> B>
		constexpr Vec3(const Vec2<A>& vec, B z);
		/** 
		* Create a Vec3 from another Vec3
		* \param[in] other Other Vec3
		*/
		constexpr Vec3(const Vec3& other) noexcept;
		/**
		* Create a Vec3 from another Vec3
		* \tparam A Type of other Vec3's components
		* \param[in] other Other Vec3
		*/
		template<ConvertableTo<Ty> A>
		constexpr Vec3(const Vec3<A>& other) noexcept;
		/**
		* Create a Vec3 from a moved Vec3
		* \param[in] other Other Vec3
		*/
		constexpr Vec3(Vec3&& other) noexcept;

		constexpr auto operator=(const Vec3& other) noexcept -> Vec3&;
		template <ConvertableTo<Ty> A>
		constexpr auto operator=(const Vec3<A>& other) noexcept -> Vec3&;
		constexpr auto operator=(Vec3&& other) noexcept -> Vec3&;

		constexpr auto operator++() noexcept -> Vec3&;
		constexpr auto operator++(int) noexcept -> Vec3;

		constexpr auto operator--() noexcept -> Vec3&;
		constexpr auto operator--(int) noexcept -> Vec3;

		constexpr auto operator+() const noexcept -> Vec3;
		constexpr auto operator-() const noexcept -> Vec3;
		constexpr auto operator~() const noexcept -> Vec3 requires Invertable<Ty>;

		constexpr auto operator+ (const Vec3& other) const noexcept -> Vec3;
		constexpr auto operator- (const Vec3& other) const noexcept -> Vec3;
		constexpr auto operator* (const Vec3& other) const noexcept -> Vec3;
		constexpr auto operator/ (const Vec3& other) const noexcept -> Vec3;
		constexpr auto operator% (const Vec3& other) const noexcept -> Vec3 requires HasModulo<Ty>;
		constexpr auto operator| (const Vec3& other) const noexcept -> Vec3 requires Orable<Ty>;
		constexpr auto operator^ (const Vec3& other) const noexcept -> Vec3 requires Xorable<Ty>;
		constexpr auto operator& (const Vec3& other) const noexcept -> Vec3 requires Andable<Ty>;
		constexpr auto operator<<(const Vec3& other) const noexcept -> Vec3 requires BitShiftable<Ty>;
		constexpr auto operator>>(const Vec3& other) const noexcept -> Vec3 requires BitShiftable<Ty>;

		template<ConvertableTo<Ty> A>
		constexpr auto operator+ (A val) const noexcept -> Vec3;
		template<ConvertableTo<Ty> A>
		constexpr auto operator- (A val) const noexcept -> Vec3;
		template<ConvertableTo<Ty> A>
		constexpr auto operator* (A val) const noexcept -> Vec3;
		template<ConvertableTo<Ty> A>
		constexpr auto operator/ (A val) const noexcept -> Vec3;
		template<ConvertableTo<Ty> A>
		constexpr auto operator% (A val) const noexcept -> Vec3 requires HasModulo<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator| (A val) const noexcept -> Vec3 requires Orable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator^ (A val) const noexcept -> Vec3 requires Xorable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator& (A val) const noexcept -> Vec3 requires Andable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator<<(A val) const noexcept -> Vec3 requires BitShiftable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator>>(A val) const noexcept -> Vec3 requires BitShiftable<Ty>;

		template<ConvertableTo<Ty> A>
		constexpr auto operator*(const Quaternion<A>& quat) const noexcept -> Vec3;

		constexpr auto operator+= (const Vec3& other) noexcept -> Vec3&;
		constexpr auto operator-= (const Vec3& other) noexcept -> Vec3&;
		constexpr auto operator*= (const Vec3& other) noexcept -> Vec3&;
		constexpr auto operator/= (const Vec3& other) noexcept -> Vec3&;
		constexpr auto operator%= (const Vec3& other) noexcept -> Vec3& requires HasModulo<Ty>;
		constexpr auto operator|= (const Vec3& other) noexcept -> Vec3& requires Orable<Ty>;
		constexpr auto operator^= (const Vec3& other) noexcept -> Vec3& requires Xorable<Ty>;
		constexpr auto operator&= (const Vec3& other) noexcept -> Vec3& requires Andable<Ty>;
		constexpr auto operator<<=(const Vec3& other) noexcept -> Vec3& requires BitShiftable<Ty>;
		constexpr auto operator>>=(const Vec3& other) noexcept -> Vec3& requires BitShiftable<Ty>;

		template<ConvertableTo<Ty> A>
		constexpr auto operator+= (A val) noexcept -> Vec3&;
		template<ConvertableTo<Ty> A>
		constexpr auto operator-= (A val) noexcept -> Vec3&;
		template<ConvertableTo<Ty> A>
		constexpr auto operator*= (A val) noexcept -> Vec3&;
		template<ConvertableTo<Ty> A>
		constexpr auto operator/= (A val) noexcept -> Vec3&;
		template<ConvertableTo<Ty> A>
		constexpr auto operator%= (A val) noexcept -> Vec3& requires HasModulo<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator|= (A val) noexcept -> Vec3& requires Orable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator^= (A val) noexcept -> Vec3& requires Xorable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator&= (A val) noexcept -> Vec3& requires Andable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator<<=(A val) noexcept -> Vec3& requires BitShiftable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator>>=(A val) noexcept -> Vec3& requires BitShiftable<Ty>;

		template<ConvertableTo<Ty> A>
		constexpr auto operator*=(const Quaternion<A>& quat) noexcept -> Vec3&;

		constexpr auto operator[](usize idx) noexcept -> Ty&;
		constexpr auto operator[](usize idx) const noexcept -> Ty;

		constexpr auto operator==(const Vec3& other) const noexcept -> bool;
		constexpr auto operator!=(const Vec3& other) const noexcept -> bool;

		/**
		 * Compare 2 vectors using an epsilon
		 * \param[in] other Vector to compare with
		 * \param[in] e Epsilon
		 * \return Whether the vectors are equal
		 */
		constexpr auto Compare(const Vec3& other, Ty e = Math::Consts::MathEpsilon<Ty>) const noexcept -> bool;

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
		constexpr auto Min(const Vec3& other) const noexcept -> Vec3;
		/**
		 * Get the component-wise maximum
		 * \param[in] other Other vector
		 * \return Vector with component-wise maximum
		 */
		constexpr auto Max(const Vec3& other) const noexcept -> Vec3;

		/**
		 * Clamp the components between 2 values
		 * \tparam A Numeric type
		 * \tparam B Numeric type
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Reference to the Vector
		 */
		template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
		constexpr auto Clamp(A min, B max) noexcept -> Vec3&;
		/**
		 * Clamp the components between the corresponding components of the given vectors
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Reference to the Vector
		 */
		constexpr auto Clamp(const Vec3& min, const Vec3& max) noexcept -> Vec3&;
		/**
		 * Get a vector with its components clamped between 2 values
		 * \tparam A Numeric type
		 * \tparam B Numeric type
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Clamped vector
		 */
		template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
		constexpr auto Clamped(A min, B max) const noexcept -> Vec3;
		/**
		 * Get a vector with its components clamped between the corresponding components of the given vectors
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Clamped vector
		 */
		constexpr auto Clamped(const Vec3& min, const Vec3& max) const noexcept -> Vec3;

		/**
		 * Clamp the length between 2 values
		 * \tparam A Numeric type
		 * \tparam B Numeric type
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Reference to the Vector
		 */
		template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
		constexpr auto ClampLen(A min, B max) noexcept -> Vec3&;
		/**
		 * Get a vector with its length clamped between 2 values
		 * \tparam A Numeric type
		 * \tparam B Numeric type
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Clamped vector
		 */
		template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
		constexpr auto ClampedLen(A min, B max) const noexcept -> Vec3;

		/**
		 * Saturate the components (clamp between 0 and 1)
		 * \return Reference to the Vector
		 */
		constexpr auto Saturate() noexcept -> Vec3&;
		/**
		 * Get a vector with saturated components (between 0 and 1)
		 * \return Vector with saturated components
		 */
		constexpr auto Saturated() const noexcept -> Vec3;

		/**
		 * Snap each component to the nearest multiple of the given step size
		 * \param[in] stepSize Size on to which elements should be snapped
		 * \return Reference to the Vector
		 */
		constexpr auto Snap(Ty stepSize) noexcept -> Vec3&;
		/**
		 * Get a vector with each component snapped to the nearest multiple of the given step size
		 * \param[in] stepSize Size on to which elements should be snapped
		 * \return Snapped vector
		 */
		constexpr auto Snapped(Ty stepSize) const noexcept -> Vec3;

		/**
		 * Linearly interpolate between 2 vectors
		 * \tparam A Interpolant type
		 * \param[in] other Other vector
		 * \param[in] i Interpolant
		 * \return Interpolated vector
		 */
		template <ConvertableTo<Ty> A>
		constexpr auto Lerp(const Vec3& other, A i) const noexcept -> Vec3;

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
		constexpr auto DistanceSq(const Vec3& other) const noexcept -> Ty;
		/**
		 * Get the distance between this vector and another
		 * \param[in] other Vector to calculate distance to
		 * \return Distance between this vector and another
		 */
		constexpr auto Distance(const Vec3& other) const noexcept -> Ty;

		/**
		 * Get the dot product of this vector and another
		 * \param[in] other Other vector
		 * \return Dot product of this vector and another
		 */
		constexpr auto Dot(const Vec3& other) const noexcept -> Ty;
		/**
		 * Get the cross product of this vector and another
		 * \param[in] other Other vector
		 * \return Cross product of this vector and another
		 */
		constexpr auto Cross(const Vec3& other) const noexcept -> Vec3;

		/**
		 * Normalize the vector
		 * \return Reference to the vector
		 */
		constexpr auto Normalize() noexcept -> Vec3&;
		/**
		 * Get the normalized vector
		 * \return Normalized vector
		 */
		constexpr auto Normalized() const noexcept -> Vec3;
		/**
		 * Check if the vector is normalized
		 * \return Whether the vector is normalized
		 */
		constexpr auto IsNormalized(Ty e = Math::Consts::MathEpsilon<Ty>) const noexcept -> bool;

		/**
		 * Get the vector rotated by a quaternion
		 * \tparam A Type of quaternion components
		 * \param[in] quat Quaternion
		 * \return Rotated vector
		 */
		template<ConvertableTo<Ty> A>
		constexpr auto Rotate(const Quaternion<A>& quat) noexcept -> Vec3&;
		/**
		 * Get the vector rotated by a quaternion
		 * \tparam A Type of quaternion components
		 * \param[in] quat Quaternion
		 * \return Rotated vector
		 */
		template<ConvertableTo<Ty> A>
		constexpr auto Rotated(const Quaternion<A>& quat) const noexcept -> Vec3;

		/**
		 * Get or flip this vector, so it's pointing in the opposite direction of the incidence vector, relative to the normal
		 * \param[in] incidence Incidence factor
		 * \param[in] normal Normal
		 * \return Forward facing vector
		 */
		constexpr auto FaceForward(const Vec3& incidence, const Vec3& normal) const noexcept -> Vec3;
		/**
		 * Reflect a vector on a 'surface' with a normal
		 * \param[in] normal Normal of 'surface' to reflect on
		 * \return Reflected vector
		 */
		constexpr auto Reflect(const Vec3& normal) const noexcept -> Vec3;
		/**
		 * Refract the vector through a 'surface' with a given normal and eta
		 * \tparam A Eta type
		 * \param[in] normal Normal of the 'surface' to refract through
		 * \param[in] eta Ratio of indices of refraction at the surface interface (outgoing / incoming)
		 * \return Refracted vector
		 */
		template<ConvertableTo<Ty> A>
		constexpr auto Refract(const Vec3& normal, A eta) const noexcept -> Vec3;

		/**
		 * Get the direction and length of the vector
		 * \return Pair with the direction and length of the vector
		 */
		constexpr auto DirectionAndLength() const noexcept -> Pair<Vec3, Ty>;

		/**
		 * Get a vector with swizzled components
		 * \tparam Idx0 Index of first component
		 * \tparam Idx1 Index of second component
		 * \tparam Idx2 Index of third component
		 * \return Swizzled vector
		 */
		template<usize Idx0, usize Idx1, usize Idx2>
		constexpr auto Swizzle() const noexcept -> Vec3;

		/**
		 * Get a vector with swizzled components
		 * \param[in] idx0 Index of first component
		 * \param[in] idx1 Index of second component
		 * \param[in] idx2 Index of third component
		 * \return Swizzled vector
		 */
		constexpr auto Swizzle(u8 idx0, u8 idx1, u8 idx2) const noexcept -> Vec3;
		
#define SWIZZLE_DECL(xy, uv, st) \
		constexpr auto xy() const noexcept -> Vec3; \
		constexpr auto uv() const noexcept -> Vec3; \
		constexpr auto st() const noexcept -> Vec3;

		SWIZZLE_DECL(xxx, uuu, sss)
		SWIZZLE_DECL(xxy, uuv, sst)
		SWIZZLE_DECL(xxz, uuw, ssq)
		SWIZZLE_DECL(xyx, uvu, sts)
		SWIZZLE_DECL(xyy, uvv, stt)
		SWIZZLE_DECL(xyz, uvw, stq)
		SWIZZLE_DECL(xzx, uwu, sqs)
		SWIZZLE_DECL(xzy, uwv, sqt)
		SWIZZLE_DECL(xzz, uww, sqq)
		SWIZZLE_DECL(yxx, vuu, tss)
		SWIZZLE_DECL(yxy, vuv, tst)
		SWIZZLE_DECL(yxz, vuw, tsq)
		SWIZZLE_DECL(yyx, vvu, tts)
		SWIZZLE_DECL(yyy, vvv, ttt)
		SWIZZLE_DECL(yyz, vvw, ttq)
		SWIZZLE_DECL(yzx, vwu, tqs)
		SWIZZLE_DECL(yzy, vwv, tqt)
		SWIZZLE_DECL(yzz, vww, tqq)
		SWIZZLE_DECL(zxx, wuu, qss)
		SWIZZLE_DECL(zxy, wuv, qst)
		SWIZZLE_DECL(zxz, wuw, qsq)
		SWIZZLE_DECL(zyx, wvu, qts)
		SWIZZLE_DECL(zyy, wvv, qtt)
		SWIZZLE_DECL(zyz, wvw, qtq)
		SWIZZLE_DECL(zzx, wwu, qqs)
		SWIZZLE_DECL(zzy, wwv, qqt)
		SWIZZLE_DECL(zzz, www, qqq)

#undef SWIZZLE_DECL

		/**
		 * Get a vector with the absolute values of the components
		 * \return Vector with absolute components
		 */
		constexpr auto Abs() const noexcept -> Vec3;
		/**
		 * Get a vector with the ceil of the components
		 * \return Vector with the ceil of the components
		 */
		constexpr auto Ceil() const noexcept -> Vec3;
		/**
		 * Get a vector with the floor of the components
		 * \return Vector with the floor of the components
		 */
		constexpr auto Floor() const noexcept -> Vec3;
		/**
		 * Get a vector with the truncated values of the components
		 * \return Vector with truncated components
		 */
		constexpr auto Trunc() const noexcept -> Vec3;
		/**
		 * Get a vector with the rounded values of the components
		 * \return Vector with rounded components
		 */
		constexpr auto Round() const noexcept -> Vec3;
		/**
		 * Get a vector with the rounded values of the components, if the component ends on .5, round towards the nearest even integer value
		 * \return Vector with rounded components
		 */
		constexpr auto RoundEven() const noexcept -> Vec3;
		/**
		 * Get a vector with the sign of the components
		 * \return Vector with the sign of the components
		 */
		constexpr auto Sign() const noexcept -> Vec3;
		/**
		 * Get a vector with the fractional part of the components
		 * \return Vector with the fractional part of the components
		 */
		constexpr auto Fract() const noexcept -> Vec3;
	};

	template<Numeric T, ConvertableTo<T> A>
	constexpr auto operator*(A t, const Vec3<T>& vec) noexcept -> Vec3<T>;

	template<Numeric T, ConvertableTo<T> A>
	constexpr auto operator*(const Quaternion<A>& quat, const Vec3<T>& vec) noexcept -> Vec3<T>;

}