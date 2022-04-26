#pragma once
#include "core/MinInclude.h"
#include "Concepts.h"
#include "FwdDecl.h"
#include "Vec3.h"

namespace Onca::Math
{
	/**
	 * Quaternion
	 * \tparam T Numeric type
	 * \note A quaternion cheat sheet for quaternions can be found at: http://www.neil.dantam.name/note/dantam-quaternion.pdf
	 */
	// TODO: SIMD
	template<Numeric T>
	struct Quaternion
	{
		union
		{
			struct { T w, x, y, z; };
			struct { T real; Vec3<T> imaginary; };
			T data[4];
		};

		/**
		 * Create an identity Quaternion
		 */
		constexpr Quaternion() noexcept;
		/**
		 * Create a Quaternion with the given components
		 * \tparam W Type of w component
		 * \tparam X Type of x component
		 * \tparam Y Type of y component
		 * \tparam Z Type of z component
		 * \param[in] w W component
		 * \param[in] x X component
		 * \param[in] y y Component
		 * \param[in] z Z compenent
		 */
		template<ConvertableTo<T> W, ConvertableTo<T> X, ConvertableTo<T> Y, ConvertableTo<T> Z>
		constexpr Quaternion(W w, X x, Y y, Z z) noexcept;
		/**
		 * Crate a Quaternion from a real and imaginary component
		 * \tparam R Type of the real component
		 * \tparam I Type of the imaginary component
		 * \param[in] real Real component
		 * \param[in] imaginary Imaginary component
		 */
		template<ConvertableTo<T> R, ConvertableTo<T> I>
		constexpr Quaternion(R real, const Vec3<I>& imaginary);
		/**
		 * Create a Quaternion from a matrix
		 * \param[in] mat Matrix
		 */
		explicit constexpr Quaternion(const Mat3<T>& mat);
		/**
		 * Create a Quaternion from another Quaternion
		 * \param[in] other Other Quaternion
		 */
		constexpr Quaternion(const Quaternion& other) noexcept;
		/**
		 * Create a Quaternion from another Quaternion
		 * \tparam U Type of other Quaternion
		 * \param[in] other Other Quaternion
		 */
		template<Numeric U>
		constexpr Quaternion(const Quaternion<U>& other) noexcept;
		/**
		 * Create a Quaternion from another Quaternion
		 * \param[in] other Other Quaternion
		 */
		constexpr Quaternion(Quaternion&& other) noexcept;

		constexpr auto operator=(const Quaternion& other) noexcept -> Quaternion;
		template<Numeric U>
		constexpr auto operator=(const Quaternion<U>& other) noexcept -> Quaternion;
		constexpr auto operator=(Quaternion&& other) noexcept -> Quaternion;

		constexpr auto operator+() const noexcept -> Quaternion;
		constexpr auto operator-() const noexcept -> Quaternion;

		constexpr auto operator+(const Quaternion& other) const noexcept -> Quaternion;
		constexpr auto operator-(const Quaternion& other) const noexcept -> Quaternion;
		constexpr auto operator*(const Quaternion& other) const noexcept -> Quaternion;

		template<ConvertableTo<T> U>
		constexpr auto operator*(U val) const noexcept -> Quaternion;
		template<ConvertableTo<T> U>
		constexpr auto operator/(U val) const noexcept -> Quaternion;

		constexpr auto operator+=(const Quaternion& other) noexcept -> Quaternion&;
		constexpr auto operator-=(const Quaternion& other) noexcept -> Quaternion&;
		constexpr auto operator*=(const Quaternion& other) noexcept -> Quaternion&;

		template<ConvertableTo<T> U>
		constexpr auto operator*=(U val) noexcept -> Quaternion&;
		template<ConvertableTo<T> U>
		constexpr auto operator/=(U val) noexcept -> Quaternion&;

		constexpr auto operator[](usize idx) const noexcept -> T;
		constexpr auto operator[](usize idx) noexcept -> T&;

		template<ConvertableTo<T> U>
		constexpr auto operator==(const Quaternion<U>& other) const noexcept -> bool;
		template<ConvertableTo<T> U>
		constexpr auto operator!=(const Quaternion<U>& other) const noexcept -> bool;

		/**
		 * Compare 2 quaternions using an epsilon
		 * \param[in] other Quaternion to compare with
		 * \param[in] e Epsilon
		 * \return Whether the quaternions are equal
		 */
		constexpr auto Compare(const Quaternion& other, T e = Consts::MathEpsilon<T>) const noexcept -> bool;

		/**
		 * Check if the quaternion is an identity quaternion
		 * \param[in] e Epsilon
		 * \return Whether the quaternion is an identity quaternion
		 */
		constexpr auto IsIdentity(T e = Consts::MathEpsilon<T>) const noexcept -> bool;

		/**
		 * Linearly interpolate between 2 quaternions
		 * \tparam I Type of the interpolant
		 * \param[in] other Other Quaternion
		 * \param[in] interpolant Interpolant, in the range [0;1]
		 * \return Interpolated quaternion
		 */
		template<ConvertableTo<T> I>
		constexpr auto LerpFullPath(const Quaternion& other, I interpolant) const noexcept -> Quaternion;
		/**
		 * Linearly interpolate between 2 quaternions, via the shortest path between both quaternions
		 * \tparam I Type of the interpolant
		 * \param[in] other Other Quaternion
		 * \param[in] interpolant Interpolant, in the range [0;1]
		 * \return Interpolated quaternion
		 */
		template<ConvertableTo<T> I>
		constexpr auto Lerp(const Quaternion& other, I interpolant) const noexcept -> Quaternion;
		/**
		 * Spherical linear interpolation between 2 quaternions
		 * \tparam I Type of the interpolant
		 * \param[in] other Other Quaternion
		 * \param[in] interpolant Interpolant, in the range [0;1]
		 * \return Interpolated quaternion
		 * \note Both quaternions should be normalized
		 */
		template<ConvertableTo<T> I>
		constexpr auto SlerpFullPath(const Quaternion& other, I interpolant) const noexcept -> Quaternion;
		/**
		 * Spherical linear interpolation between 2 quaternions, via the shortest path between both quaternions
		 * \tparam I Type of the interpolant
		 * \param[in] other Other Quaternion
		 * \param[in] interpolant Interpolant, in the range [0;1]
		 * \return Interpolated quaternion
		 * \note Both quaternions should be normalized
		 */
		template<ConvertableTo<T> I>
		constexpr auto Slerp(const Quaternion& other, I interpolant) const noexcept -> Quaternion;

		/**
		 * \Spherical quadratic interpolation between 2 quaternions
		 * \tparam I Interpolant type
		 * \param[in] a Tangent of this Quaternion
		 * \param[in] q1 Second quaternion
		 * \param[in] b Tangent of second Quaternion
		 * \param[in] interpolant Interpolant, in the range [0;1]
		 * \return Interpolated quaternion
		 * \note Quaternions should be normalized
		 */
		template<ConvertableTo<T> I>
		constexpr auto SquadFullPath(const Quaternion& a, const Quaternion& q1, const Quaternion& b, I interpolant) const noexcept -> Quaternion;
		/**
		 * \Spherical quadrangle interpolation between 2 quaternions, via the shortest path between both quaternions
		 * \tparam I Interpolant type
		 * \param[in] a Tangent of this Quaternion
		 * \param[in] q1 Second quaternion
		 * \param[in] b Tangent of second Quaternion
		 * \param[in] interpolant Interpolant, in the range [0;1]
		 * \return Interpolated quaternion
		 * \note Quaternions should be normalized
		 */
		template<ConvertableTo<T> I>
		constexpr auto Squad(const Quaternion& a, const Quaternion& q1, const Quaternion& b, I interpolant) const noexcept -> Quaternion;

		/**
		 * Get the square length of the quaternion;
		 * \return Square length of the quaternion
		 */
		constexpr auto LenSq() const noexcept -> T;
		/**
		 * Get the length of the Quaternion
		 * \return Length of the Quaternion
		 */
		constexpr auto Len() const noexcept -> T;

		/**
		 * Get the dot product of the Quaternion
		 * \param[in] other Other Quaternion
		 * \return Dot product
		 */
		constexpr auto Dot(const Quaternion& other) const noexcept -> T;
		/**
		 * Get the cross product of the quaternion
		 * \param[in] other Other Quaternion
		 * \return Cross product
		 */
		constexpr auto Cross(const Quaternion& other) const noexcept -> Quaternion;

		/**
		 * Normalize the Quaternion
		 * \return Reference to the Quaternion
		 */
		constexpr auto Normalize() noexcept -> Quaternion&;
		/**
		 * Get the normalized Quaternion
		 * \return Normalized Quaternion
		 */
		constexpr auto Normalized() const noexcept -> Quaternion;
		/**
		 * Check if the Quaternion is normalized
		 * \param[in] e Epsilon
		 * \return Whether the quaternion is normalized
		 */
		constexpr auto IsNormalized(T e = Consts::MathEpsilon<T>) const noexcept -> bool;

		/**
		 * Invert the Quaternion
		 * \return Reference to the Quaternion
		 */
		constexpr auto Invert() noexcept -> Quaternion&;
		/**
		 * Get the inverse Quaternion
		 * \return Inverse Quaternion
		 */
		constexpr auto Inverse() const noexcept -> Quaternion;

		/**
		 * Conjugate the Quaternion
		 * \return Reference to the Quaternion
		 */
		constexpr auto Conjugate() noexcept -> Quaternion&;
		/**
		 * Get the conjugated Quaternion
		 * \return Conjugated Quaternion
		 */
		constexpr auto Conjugated() const noexcept -> Quaternion;

		/**
		 * Get the logarithm of the Quaternion, which is w=0 and V=theta*v (where |v| = 1)
		 * \return Logarithm of the quaternion
		 */
		constexpr auto Log() const noexcept -> Quaternion;
		/**
		 * Get the exponential of the quaternion, assuming a quaternion (0, theta*v), which is (cos(theta), sin(theta)*v)
		 * \return Exponential of the quaternion
		 * \note `w` is ignored
		 */
		constexpr auto Exp() const noexcept -> Quaternion;

		/**
		 * Get the angle represented by the Quaternion
		 * \return The angle represented by the Quaternion
		 */
		constexpr auto Angle() const noexcept -> Math::Angle<T, AngleKind::Radians>;
		/**
		 * Get the angle between quaternions
		 * \param[in] other Other quaternion
		 * \return Angle between quaternions
		 */
		constexpr auto Angle(const Quaternion& other) const noexcept -> Math::Angle<T, AngleKind::Radians>;

		/**
		 * Calculate the swing and twist rotations
		 * \param[in] axis Axis to twist around
		 * \return Pair with swing and twist rotations
		 */
		constexpr auto SwingTwist(const Vec3<T>& axis) const noexcept -> Pair<Quaternion, Quaternion>;

		/**
		 * Get the twist angle around an axis
		 * \param[in] axis Axis to twist around
		 * \return Twist angle
		 */
		constexpr auto TwistAngle(const Vec3<T>& axis) const noexcept -> Math::Angle<T, AngleKind::Radians>;

		/**
		 * Extract the euler angles from the Quaternion
		 * \param[in] order Order of rotations
		 * \return Euler angles
		 */
		constexpr auto ToEuler(EulerOrder order = EulerOrder::ZYX) const noexcept -> Math::EulerAngles<T, AngleKind::Radians>;
		/**
		 * Extract the axis of rotation and the angle from the Quaternion
		 * \return Pair with the axis and angle
		 */
		constexpr auto ToAxisAngle() const noexcept -> Pair<Vec3<T>, Math::Angle<T, AngleKind::Radians>>;

		/**
		 * Calculate the inner quadrangle points to be used in Squad (`a` and `b` parameters)
		 * \param[in] prev Quaternion at location N-1
		 * \param[in] other Quaternion at location N+1 (representing the end quaternion for Squad)
		 * \param[in] next Quaternion at location N+2
		 * \return Pair with the inner quadrangle points (a,b)
		 */
		constexpr auto CalculateInnerQuadrangle(const Quaternion& prev, const Quaternion& other, const Quaternion& next) const noexcept -> Pair<Quaternion, Quaternion>;

		/**
		 * Create a Quaternion from euler euler
		 * \tparam A Type of the pitch angle
		 * \tparam Kind Pitch angle kind
		 * \param[in] euler Angle (pitch, yaw, roll)
		 * \param[in] order Order of rotations
		 * \return Quaternion
		 * \note Rotation is done in a ZYX order
		 */
		template<ConvertableTo<T> A, AngleKind Kind>
		static constexpr auto EulerAngles(const Math::EulerAngles<A, Kind>& euler, EulerOrder order = EulerOrder::ZYX) noexcept -> Quaternion;
		/**
		 * Create a Quaternion from an axis and an angle
		 * \tparam A Type of axis components
		 * \tparam B Type of angle
		 * \tparam Kind Angle kind
		 * \param[in] axis Axis of rotation
		 * \param[in] angle Angle to rotate
		 * \return Quaternion
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, AngleKind Kind>
		static constexpr auto AxisAngle(const Vec3<A>& axis, const Math::Angle<B, Kind>& angle) noexcept -> Quaternion;
	};
}
