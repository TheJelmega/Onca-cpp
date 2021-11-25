#pragma once
#include "core/MinInclude.h"
#include "Concepts.h"
#include "FwdDecl.h"
#include "MathUtils.h"

namespace Core::Math
{
	namespace Detail
	{
		template<Numeric Ty>
		struct Vec4PackSelector
		{
			using Type = Empty;
		};

#if !MATH_DISABLE_SIMD
		template<Intrin::SimdBaseType Ty>
			requires (sizeof(Ty) == 4) || (sizeof(Ty) == 8)
		struct Vec4PackSelector<Ty>
		{
			using Type = Intrin::Pack<Ty, 4>;
		};
#endif

		template<Numeric Ty>
		using Vec4Pack = typename Vec4PackSelector<Ty>::Type;
	}

#define REFLECT(...)

	/**
	 * 4D vector
	 * \tparam Ty Component type
	 */
	REFLECT(0, Value)
	template<Numeric Ty>
	struct Vec4
	{
	private:
		using PackT = Detail::Vec4Pack<Ty>;

		/**
		 * Create a Vec4 from a pack
		 * \param[in] pack SIMD pack
		 */
		explicit constexpr Vec4(const PackT& pack) noexcept;

	public:

		constexpr static u8 X = 0;
		constexpr static u8 Y = 1;
		constexpr static u8 Z = 2;
		constexpr static u8 W = 3;

		union
		{
			struct { Ty x, y, z, w; };
			Ty data[4];
			PackT pack;
		};

		static constexpr auto HasNativeRegister() noexcept -> bool { return !SameAs<PackT, Empty>; }

		/**
		* Create a default Vec4 (0, 0, 0, 0)
		*/
		constexpr Vec4() noexcept;
		/**
		* Create a Vec4 with all components set to a value
		* \tparam A Type of value
		* \param[in] val Value
		*/
		template<ConvertableTo<Ty> A>
		explicit constexpr Vec4(A val) noexcept;
		/**
		* Create a Vec4 with the given components
		* \tparam A Type of x component
		* \tparam B Type of y component
		* \tparam C Type of z component
		* \tparam D Type of w component
		* \param[in] x X component
		* \param[in] y Y component
		* \param[in] z Z component
		* \param[in] w W component
		*/
		template<ConvertableTo<Ty> A, ConvertableTo<Ty> B, ConvertableTo<Ty> C, ConvertableTo<Ty> D>
		constexpr Vec4(A x, B y, C z, D w) noexcept;
		/**
		 * Create a Vec4 from a Vec2 and w + z components
		 * \tparam A Type of Vec2's components
		 * \tparam B Type of z component
		 * \tparam C Type of w component
		 * \param[in] vec Vec2 with x and y components
		 * \param[in] z Z component
		 * \param[in] w W component
		 */
		template<ConvertableTo<Ty> A, ConvertableTo<Ty> B, ConvertableTo<Ty> C>
		constexpr Vec4(const Vec2<A>& vec, B z, C w);
		/**
		 * Create a Vec4 from a Vec3 and w component
		 * \tparam A Type of Vec3's components
		 * \tparam B Type of w component
		 * \param[in] vec Vec2 with x, y, and z components
		 * \param[in] w W component
		 */
		template<ConvertableTo<Ty> A, ConvertableTo<Ty> B>
		constexpr Vec4(const Vec3<A>& vec, B w);
		/** 
		* Create a Vec4 from another Vec4
		* \param[in] other Other Vec4
		*/
		constexpr Vec4(const Vec4& other) noexcept;
		/**
		* Create a Vec4 from another Vec4
		* \tparam A Type of other Vec4's components
		* \param[in] other Other Vec4
		*/
		template<ConvertableTo<Ty> A>
		constexpr Vec4(const Vec4<A>& other) noexcept;
		/**
		* Create a Vec4 from a moved Vec4
		* \param[in] other Other Vec4
		*/
		constexpr Vec4(Vec4&& other) noexcept;

		constexpr auto operator=(const Vec4& other) noexcept -> Vec4&;
		template <ConvertableTo<Ty> A>
		constexpr auto operator=(const Vec4<A>& other) noexcept -> Vec4&;
		constexpr auto operator=(Vec4&& other) noexcept -> Vec4&;

		constexpr auto operator++() noexcept -> Vec4&;
		constexpr auto operator++(int) noexcept -> Vec4;

		constexpr auto operator--() noexcept -> Vec4&;
		constexpr auto operator--(int) noexcept -> Vec4;

		constexpr auto operator+() const noexcept -> Vec4;
		constexpr auto operator-() const noexcept -> Vec4;
		constexpr auto operator~() const noexcept -> Vec4 requires Invertable<Ty>;

		constexpr auto operator+ (const Vec4& other) const noexcept -> Vec4;
		constexpr auto operator- (const Vec4& other) const noexcept -> Vec4;
		constexpr auto operator* (const Vec4& other) const noexcept -> Vec4;
		constexpr auto operator/ (const Vec4& other) const noexcept -> Vec4;
		constexpr auto operator% (const Vec4& other) const noexcept -> Vec4 requires HasModulo<Ty>;
		constexpr auto operator| (const Vec4& other) const noexcept -> Vec4 requires Orable<Ty>;
		constexpr auto operator^ (const Vec4& other) const noexcept -> Vec4 requires Xorable<Ty>;
		constexpr auto operator& (const Vec4& other) const noexcept -> Vec4 requires Andable<Ty>;
		constexpr auto operator<<(const Vec4& other) const noexcept -> Vec4 requires BitShiftable<Ty>;
		constexpr auto operator>>(const Vec4& other) const noexcept -> Vec4 requires BitShiftable<Ty>;

		template<ConvertableTo<Ty> A>
		constexpr auto operator+ (A val) const noexcept -> Vec4;
		template<ConvertableTo<Ty> A>
		constexpr auto operator- (A val) const noexcept -> Vec4;
		template<ConvertableTo<Ty> A>
		constexpr auto operator* (A val) const noexcept -> Vec4;
		template<ConvertableTo<Ty> A>
		constexpr auto operator/ (A val) const noexcept -> Vec4;
		template<ConvertableTo<Ty> A>
		constexpr auto operator% (A val) const noexcept -> Vec4 requires HasModulo<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator| (A val) const noexcept -> Vec4 requires Orable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator^ (A val) const noexcept -> Vec4 requires Xorable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator& (A val) const noexcept -> Vec4 requires Andable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator<<(A val) const noexcept -> Vec4 requires BitShiftable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator>>(A val) const noexcept -> Vec4 requires BitShiftable<Ty>;

		template<ConvertableTo<Ty> A>
		constexpr auto operator*(const Quaternion<A>& quat) const noexcept -> Vec4;

		constexpr auto operator+= (const Vec4& other) noexcept -> Vec4&;
		constexpr auto operator-= (const Vec4& other) noexcept -> Vec4&;
		constexpr auto operator*= (const Vec4& other) noexcept -> Vec4&;
		constexpr auto operator/= (const Vec4& other) noexcept -> Vec4&;
		constexpr auto operator%= (const Vec4& other) noexcept -> Vec4& requires HasModulo<Ty>;
		constexpr auto operator|= (const Vec4& other) noexcept -> Vec4& requires Orable<Ty>;
		constexpr auto operator^= (const Vec4& other) noexcept -> Vec4& requires Xorable<Ty>;
		constexpr auto operator&= (const Vec4& other) noexcept -> Vec4& requires Andable<Ty>;
		constexpr auto operator<<=(const Vec4& other) noexcept -> Vec4& requires BitShiftable<Ty>;
		constexpr auto operator>>=(const Vec4& other) noexcept -> Vec4& requires BitShiftable<Ty>;

		template<ConvertableTo<Ty> A>
		constexpr auto operator+= (A val) noexcept -> Vec4&;
		template<ConvertableTo<Ty> A>
		constexpr auto operator-= (A val) noexcept -> Vec4&;
		template<ConvertableTo<Ty> A>
		constexpr auto operator*= (A val) noexcept -> Vec4&;
		template<ConvertableTo<Ty> A>
		constexpr auto operator/= (A val) noexcept -> Vec4&;
		template<ConvertableTo<Ty> A>
		constexpr auto operator%= (A val) noexcept -> Vec4& requires HasModulo<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator|= (A val) noexcept -> Vec4& requires Orable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator^= (A val) noexcept -> Vec4& requires Xorable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator&= (A val) noexcept -> Vec4& requires Andable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator<<=(A val) noexcept -> Vec4& requires BitShiftable<Ty>;
		template<ConvertableTo<Ty> A>
		constexpr auto operator>>=(A val) noexcept -> Vec4& requires BitShiftable<Ty>;

		template<ConvertableTo<Ty> A>
		constexpr auto operator*=(const Quaternion<A>& quat) noexcept -> Vec4&;

		constexpr auto operator[](usize idx) noexcept -> Ty&;
		constexpr auto operator[](usize idx) const noexcept -> Ty;

		constexpr auto operator==(const Vec4& other) const noexcept -> bool;
		constexpr auto operator!=(const Vec4& other) const noexcept -> bool;

		/**
		 * Compare 2 vectors using an epsilon
		 * \param[in] other Vector to compare with
		 * \param[in] e Epsilon
		 * \return Whether the vectors are equal
		 */
		constexpr auto Compare(const Vec4& other, Ty e = Math::Consts::MathEpsilon<Ty>) const noexcept -> bool;

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
		constexpr auto Min(const Vec4& other) const noexcept -> Vec4;
		/**
		 * Get the component-wise maximum
		 * \param[in] other Other vector
		 * \return Vector with component-wise maximum
		 */
		constexpr auto Max(const Vec4& other) const noexcept -> Vec4;

		/**
		 * Clamp the components between 2 values
		 * \tparam A Numeric type
		 * \tparam B Numeric type
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Reference to the Vector
		 */
		template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
		constexpr auto Clamp(A min, B max) noexcept -> Vec4&;
		/**
		 * Clamp the components between the corresponding components of the given vectors
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Reference to the Vector
		 */
		constexpr auto Clamp(const Vec4& min, const Vec4& max) noexcept -> Vec4&;
		/**
		 * Get a vector with its components clamped between 2 values
		 * \tparam A Numeric type
		 * \tparam B Numeric type
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Clamped vector
		 */
		template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
		constexpr auto Clamped(A min, B max) const noexcept -> Vec4;
		/**
		 * Get a vector with its components clamped between the corresponding components of the given vectors
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Clamped vector
		 */
		constexpr auto Clamped(const Vec4& min, const Vec4& max) const noexcept -> Vec4;

		/**
		 * Clamp the length between 2 values
		 * \tparam A Numeric type
		 * \tparam B Numeric type
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Reference to the Vector
		 */
		template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
		constexpr auto ClampLen(A min, B max) noexcept -> Vec4&;
		/**
		 * Get a vector with its length clamped between 2 values
		 * \tparam A Numeric type
		 * \tparam B Numeric type
		 * \param[in] min Minimum value
		 * \param[in] max Maximum value
		 * \return Clamped vector
		 */
		template <ConvertableTo<Ty> A, ConvertableTo<Ty> B>
		constexpr auto ClampedLen(A min, B max) const noexcept -> Vec4;

		/**
		 * Saturate the components (clamp between 0 and 1)
		 * \return Reference to the Vector
		 */
		constexpr auto Saturate() noexcept -> Vec4&;
		/**
		 * Get a vector with saturated components (between 0 and 1)
		 * \return Vector with saturated components
		 */
		constexpr auto Saturated() const noexcept -> Vec4;

		/**
		 * Snap each component to the nearest multiple of the given step size
		 * \param[in] stepSize Size on to which elements should be snapped
		 * \return Reference to the Vector
		 */
		constexpr auto Snap(Ty stepSize) noexcept -> Vec4&;
		/**
		 * Get a vector with each component snapped to the nearest multiple of the given step size
		 * \param[in] stepSize Size on to which elements should be snapped
		 * \return Snapped vector
		 */
		constexpr auto Snapped(Ty stepSize) const noexcept -> Vec4;

		/**
		 * Linearly interpolate between 2 vectors
		 * \tparam A Interpolant type
		 * \param[in] other Other vector
		 * \param[in] i Interpolant
		 * \return Interpolated vector
		 */
		template <ConvertableTo<Ty> A>
		constexpr auto Lerp(const Vec4& other, A i) const noexcept -> Vec4;

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
		constexpr auto DistanceSq(const Vec4& other) const noexcept -> Ty;
		/**
		 * Get the distance between this vector and another
		 * \param[in] other Vector to calculate distance to
		 * \return Distance between this vector and another
		 */
		constexpr auto Distance(const Vec4& other) const noexcept -> Ty;

		/**
		 * Get the dot product of this vector and another
		 * \param[in] other Other vector
		 * \return Dot product of this vector and another
		 */
		constexpr auto Dot(const Vec4& other) const noexcept -> Ty;
		/**
		 * Get the 3D cross product of this vector and another
		 * \param[in] other Other vector
		 * \return 3D cross product of this vector and another
		 */
		constexpr auto Cross3D(const Vec4& other) const noexcept -> Vec4;

		/**
		 * Normalize the vector
		 * \return Reference to the vector
		 */
		constexpr auto Normalize() noexcept -> Vec4&;
		/**
		 * Get the normalized vector
		 * \return Normalized vector
		 */
		constexpr auto Normalized() const noexcept -> Vec4;
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
		constexpr auto Rotate(const Quaternion<A>& quat) noexcept -> Vec4&;
		/**
		 * Get the vector rotated by a quaternion
		 * \tparam A Type of quaternion components
		 * \param[in] quat Quaternion
		 * \return Rotated vector
		 */
		template<ConvertableTo<Ty> A>
		constexpr auto Rotated(const Quaternion<A>& quat) const noexcept -> Vec4;

		/**
		 * Get or flip this vector, so it's pointing in the opposite direction of the incidence vector, relative to the normal
		 * \param[in] incidence Incidence factor
		 * \param[in] normal Normal
		 * \return Forward facing vector
		 */
		constexpr auto FaceForward3D(const Vec4& incidence, const Vec4& normal) const noexcept -> Vec4;
		/**
		 * Reflect a vector on a 'surface' with a normal
		 * \param[in] normal Normal of 'surface' to reflect on
		 * \return Reflected vector
		 */
		constexpr auto Reflect3D(const Vec4& normal) const noexcept -> Vec4;
		/**
		 * Refract the vector through a 'surface' with a given normal and eta
		 * \tparam A Eta type
		 * \param[in] normal Normal of the 'surface' to refract through
		 * \param[in] eta Ratio of indices of refraction at the surface interface (outgoing / incoming)
		 * \return Refracted vector
		 */
		template<ConvertableTo<Ty> A>
		constexpr auto Refract3D(const Vec4& normal, A eta) const noexcept -> Vec4;

		/**
		 * Get the direction and length of the vector
		 * \return Pair with the direction and length of the vector
		 */
		constexpr auto DirectionAndLength() const noexcept -> Pair<Vec4, Ty>;

		/**
		 * Get a vector with swizzled components
		 * \tparam Idx0 Index of first component
		 * \tparam Idx1 Index of second component
		 * \tparam Idx2 Index of third component
		 * \tparam Idx3 Index of fourth component
		 * \return Swizzled vector
		 */
		template<usize Idx0, usize Idx1, usize Idx2, usize Idx3>
		constexpr auto Swizzle() const noexcept -> Vec4;

#define CONCAT_SWIZZLE_(x,y,z,w) x ## y ## z ## w
#define CONCAT_SWIZZLE(x,y,z,w) CONCAT_SWIZZLE_(x,y,z,w)

#define SWIZZLE_DECL(x, y, z, w) constexpr auto CONCAT_SWIZZLE(x, y, z, w)() const noexcept -> Vec4;
#define SWIZZLE_DECL4(x_, y_, z_) \
	SWIZZLE_DECL(x_, y_, z_, x) \
	SWIZZLE_DECL(x_, y_, z_, y) \
	SWIZZLE_DECL(x_, y_, z_, z) \
	SWIZZLE_DECL(x_, y_, z_, w)

#define SWIZZLE_DECL16(x_, y_) \
	SWIZZLE_DECL4(x_, y_, x) \
	SWIZZLE_DECL4(x_, y_, y) \
	SWIZZLE_DECL4(x_, y_, z) \
	SWIZZLE_DECL4(x_, y_, w)

		SWIZZLE_DECL16(x, x)
		SWIZZLE_DECL16(x, y)
		SWIZZLE_DECL16(x, z)
		SWIZZLE_DECL16(x, w)

		SWIZZLE_DECL16(y, x)
		SWIZZLE_DECL16(y, y)
		SWIZZLE_DECL16(y, z)
		SWIZZLE_DECL16(y, w)

		SWIZZLE_DECL16(z, x)
		SWIZZLE_DECL16(z, y)
		SWIZZLE_DECL16(z, z)
		SWIZZLE_DECL16(z, w)

		SWIZZLE_DECL16(w, x)
		SWIZZLE_DECL16(w, y)
		SWIZZLE_DECL16(w, z)
		SWIZZLE_DECL16(w, w)

#undef SWIZZLE_DECL16
#undef SWIZZLE_DECL4
#undef SWIZZLE_DECL
#undef CONCAT_SWIZZLE
#undef CONCAT_SWIZZLE_

		/**
		 * Get a vector with the absolute values of the components
		 * \return Vector with absolute components
		 */
		constexpr auto Abs() const noexcept -> Vec4;
		/**
		 * Get a vector with the ceil of the components
		 * \return Vector with the ceil of the components
		 */
		constexpr auto Ceil() const noexcept -> Vec4;
		/**
		 * Get a vector with the floor of the components
		 * \return Vector with the floor of the components
		 */
		constexpr auto Floor() const noexcept -> Vec4;
		/**
		 * Get a vector with the truncated values of the components
		 * \return Vector with truncated components
		 */
		constexpr auto Trunc() const noexcept -> Vec4;
		/**
		 * Get a vector with the rounded values of the components
		 * \return Vector with rounded components
		 */
		constexpr auto Round() const noexcept -> Vec4;
		/**
		 * Get a vector with the rounded values of the components, if the component ends on .5, round towards the nearest even integer value
		 * \return Vector with rounded components
		 */
		constexpr auto RoundEven() const noexcept -> Vec4;
		/**
		 * Get a vector with the sign of the components
		 * \return Vector with the sign of the components
		 */
		constexpr auto Sign() const noexcept -> Vec4;
		/**
		 * Get a vector with the fractional part of the components
		 * \return Vector with the fractional part of the components
		 */
		constexpr auto Fract() const noexcept -> Vec4;
	};

	template<Numeric T, ConvertableTo<T> A>
	constexpr auto operator*(A t, const Vec4<T>& vec) noexcept -> Vec4<T>;

	template<Numeric T, ConvertableTo<T> A>
	constexpr auto operator*(const Quaternion<A>& t, const Vec4<T>& vec) noexcept -> Vec4<T>;

}