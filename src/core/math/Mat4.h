#pragma once
#include "core/MinInclude.h"
#include "Concepts.h"
#include "FwdDecl.h"
#include "MathUtils.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Quaternion.h"
#include "Mat3.h"

namespace Core::Math
{

	/**
	 * 4x4 matrix
	 * \tparam T Numeric type
	 * \note column matrix with row-major storage
	 */
	template<Numeric T>
	struct Mat4
	{
		union
		{
			struct { T m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33; };
			struct { Vec4<T> row0, row1, row2, row3; };
			Vec4<T> rows[4];
			T data[16];
		};

		/**
		 * Create an identity matrix
		 */
		constexpr Mat4() noexcept;
		/**
		 * Create a matrix with the diagonal set to the given value
		 * \tparam A Type of value
		 * \param[in] diag Diagonal value
		 */
		template<ConvertableTo<T> A>
		explicit constexpr Mat4(A diag) noexcept;
		/**
		 * Create a matrix with the given values
		 * \tparam U0 Type of element [0,0]
		 * \tparam U1 Type of element [0,1]
		 * \tparam U2 Type of element [0,2]
		 * \tparam U3 Type of element [0,3]
		 * \tparam U4 Type of element [1,0]
		 * \tparam U5 Type of element [1,1]
		 * \tparam U6 Type of element [1,2]
		 * \tparam U7 Type of element [1,3]
		 * \tparam U8 Type of element [2,0]
		 * \tparam U9 Type of element [2,1]
		 * \tparam U10 Type of element [2,2]
		 * \tparam U11 Type of element [2,3]
		 * \tparam U12 Type of element [3,0]
		 * \tparam U13 Type of element [3,1]
		 * \tparam U14 Type of element [3,2]
		 * \tparam U15 Type of element [3,3]
		 * \param[in] m00 Element [0,0]
		 * \param[in] m01 Element [0,1]
		 * \param[in] m02 Element [0,2]
		 * \param[in] m03 Element [0,3]
		 * \param[in] m10 Element [1,0]
		 * \param[in] m11 Element [1,1]
		 * \param[in] m12 Element [1,2]
		 * \param[in] m13 Element [1,3]
		 * \param[in] m20 Element [2,0]
		 * \param[in] m21 Element [2,1]
		 * \param[in] m22 Element [2,2]
		 * \param[in] m23 Element [2,3]
		 * \param[in] m30 Element [3,0]
		 * \param[in] m31 Element [3,1]
		 * \param[in] m32 Element [3,2]
		 * \param[in] m33 Element [3,3]
		 */
		template<ConvertableTo<T> U0, ConvertableTo<T> U1, ConvertableTo<T> U2, ConvertableTo<T> U3,
			ConvertableTo<T> U4, ConvertableTo<T> U5, ConvertableTo<T> U6, ConvertableTo<T> U7,
			ConvertableTo<T> U8, ConvertableTo<T> U9, ConvertableTo<T> U10, ConvertableTo<T> U11,
			ConvertableTo<T> U12, ConvertableTo<T> U13, ConvertableTo<T> U14, ConvertableTo<T> U15>
			constexpr Mat4(U0 m00, U1 m01, U2 m02, U3 m03, U4 m10, U5 m11, U6 m12, U7 m13, 
						   U8 m20, U9 m21, U10 m22, U11 m23, U12 m30, U13 m31, U14 m32, U15 m33) noexcept;
		/**
		 * Create a matrix with the given rows
		 * \tparam U0 Type of row 0's components
		 * \tparam U1 Type of row 1's components
		 * \tparam U2 Type of row 2's components
		 * \tparam U3 Type of row 3's components
		 * \param[in] row0 Row 0
		 * \param[in] row1 Row 1
		 * \param[in] row2 Row 2
		 * \param[in] row3 Row 3
		 */
		template<ConvertableTo<T> U0, ConvertableTo<T> U1, ConvertableTo<T> U2, ConvertableTo<T> U3>
		constexpr Mat4(const Vec4<U0>& row0, const Vec4<U1>& row1, const Vec4<U2>& row2, const Vec4<U3>& row3);
		/**
		 * Create a matrix with the given rows
		 * \tparam U0 Type of column 0's components
		 * \tparam U1 Type of column 1's components
		 * \tparam U2 Type of column 2's components
		 * \tparam U3 Type of column 3's components
		 * \param[in] column0 Column 0
		 * \param[in] column1 Column 1
		 * \param[in] column2 Column 2
		 * \param[in] column3 Column 3
		 */
		template<ConvertableTo<T> U0, ConvertableTo<T> U1, ConvertableTo<T> U2, ConvertableTo<T> U3>
		constexpr Mat4(ColumnInitTag, const Vec4<U0>& column0, const Vec4<U1>& column1, const Vec4<U2>& column2, const Vec4<U3>& column3);
		/**
		 * Create a matrix from 3x4 matrix
		 * \tparam A Type of other matrix' components
		 * \param[in] other Other matrix
		 */
		template<ConvertableTo<T> A>
		explicit constexpr Mat4(const Mat43<A>& other) noexcept;
		/**
		 * Create a matrix from another matrix
		 * \param[in] other Other matrix
		 */
		constexpr Mat4(const Mat4& other) noexcept;
		/**
		 * Create a matrix from another matrix
		 * \tparam A Type of other matrix' components
		 * \param[in] other Other matrix
		 */
		template<ConvertableTo<T> A>
		constexpr Mat4(const Mat4<A>& other) noexcept;
		/**
		 * Create a matrix from a moved matrix
		 * \param[in] other Other matrix
		 */
		constexpr Mat4(Mat4&& other) noexcept;

		constexpr auto operator=(const Mat4& other) noexcept -> Mat4&;
		template<ConvertableTo<T> A>
		constexpr auto operator=(const Mat4<A>& other) noexcept -> Mat4&;
		constexpr auto operator=(Mat4&& other) noexcept -> Mat4&;

		constexpr auto operator+() const noexcept -> Mat4;
		constexpr auto operator-() const noexcept -> Mat4;

		constexpr auto operator+(const Mat4& other) const noexcept -> Mat4;
		constexpr auto operator-(const Mat4& other) const noexcept -> Mat4;
		constexpr auto operator*(const Mat4& other) const noexcept -> Mat4;

		template<ConvertableTo<T> A>
		constexpr auto operator*(A val) const noexcept -> Mat4;
		template<ConvertableTo<T> A>
		constexpr auto operator/(A val) const noexcept -> Mat4;

		constexpr auto operator+=(const Mat4& other) noexcept -> Mat4&;
		constexpr auto operator-=(const Mat4& other) noexcept -> Mat4&;
		constexpr auto operator*=(const Mat4& other) noexcept -> Mat4&;

		template<ConvertableTo<T> A>
		constexpr auto operator*=(A val) noexcept -> Mat4&;
		template<ConvertableTo<T> A>
		constexpr auto operator/=(A val) noexcept -> Mat4&;

		constexpr auto operator[](usize idx) noexcept -> T&;
		constexpr auto operator[](usize idx) const noexcept -> T;

		constexpr auto operator()(usize row, usize column) noexcept -> T&;
		constexpr auto operator()(usize row, usize column) const noexcept -> T;

		constexpr auto operator==(const Mat4& other) const noexcept -> bool;
		constexpr auto operator!=(const Mat4& other) const noexcept -> bool;

		/**
		 * Compare 2 matrices using an epsilon
		 * \param[in] other Matrix to compare with
		 * \param[in] e Epsilon
		 * \return Whether the matrices are equal
		 */
		constexpr auto Compare(const Mat4& other, T e = Consts::MathEpsilon<T>) const noexcept -> bool;

		/**
		 * Get the row at the given index
		 * \param[in] idx Index
		 * \return Row
		 */
		constexpr auto Row(usize idx) noexcept -> Vec4<T>&;
		/**
		 * Get the row at the given index
		 * \param[in] idx Index
		 * \return Row
		 */
		constexpr auto Row(usize idx) const noexcept -> Vec4<T>;
		/**
		 * Set the row at the given index
		 * \param[in] idx Index
		 * \param[in] row Row
		 * \return Row
		 */
		constexpr void SetRow(usize idx, const Vec4<T>& row) noexcept;
		/**
		 * Get the column at the given index
		 * \param[in] idx Index
		 * \return Column
		 */
		constexpr auto Column(usize idx) const noexcept -> Vec4<T>;
		/**
		 * Get the column at the given index
		 * \param[in] idx Index
		 * \param[in] column Column
		 * \return Column
		 */
		constexpr void SetColumn(usize idx, const Vec4<T>& column) noexcept;
		/**
		 * Get the diagonal of the matrix
		 * \return Diagonal
		 */
		constexpr auto Diagonal() const noexcept -> Vec4<T>;

		/**
		 * Get the minor matrix for
		 * \param[in] column Column index
		 * \param[in] row Row index
		 * \return Minor matrix
		 */
		constexpr auto Minor(usize row, usize column) const noexcept -> Mat3<T>;

		/**
		 * Transpose the matrix
		 * \return Reference to the matrix
		 */
		constexpr auto Transpose() noexcept -> Mat4&;
		/**
		 * Get the transposed matrix
		 * \return Transposed matrix
		 */
		constexpr auto Transposed() const noexcept -> Mat4;

		/**
		 * Get the determinant of the matrix
		 * \return Determinant of the matrix
		 */
		constexpr auto Determinant() const noexcept -> T;
		/**
		 * Get the trace of the matrix (sum of diagonal components)
		 * \return Trace of the matrix
		 */
		constexpr auto Trace() const noexcept -> T;

		/**
		 * Adjugate the matrix
		 * \return Reference to the matrix
		 */
		constexpr auto Adjugate() noexcept -> Mat4&;
		/**
		 * Get the adjunct matrix (adjugated)
		 * \return Adjunct matrix
		 */
		constexpr auto Adjoint() const noexcept -> Mat4;
		/**
		 * Convert to the cofactor of the matrix
		 * \return Reference to the matrix
		 */
		constexpr auto ToCofactor() noexcept -> Mat4&;
		/**
		 * Get the cofactor of the matrix
		 * \return Cofactor of the matrix
		 */
		constexpr auto Cofactor() const noexcept -> Mat4;

		/**
		 * Invert the matrix
		 * \return Reference to the matrix
		 */
		constexpr auto Invert() noexcept -> Mat4&;
		/**
		 * Get the inverse of the matrix
		 * \return Inverse of the matrix
		 */
		constexpr auto Inverse() const noexcept -> Mat4;

		/**
		 * Transform a 4D vector by the matrix (same as multiplication)
		 * \param[in] vec Vector
		 * \return Transformed vector
		 */
		constexpr auto TransformVector(const Vec4<T>& vec) const noexcept -> Vec4<T>;
		/**
		 * Transform a 3D vector by the matrix (no translation)
		 * \param[in] vec Vector
		 * \return Transformed vector
		 */
		constexpr auto TransformVector(const Vec3<T>& vec) const noexcept -> Vec3<T>;
		/**
		 * Transform a 2D vector by the matrix (no translation)
		 * \param[in] vec Vector
		 * \return Transformed vector
		 */
		constexpr auto TransformVector(const Vec2<T>& vec) const noexcept -> Vec2<T>;
		/**
		 * Transform a 3D point by the matrix
		 * \param[in] vec Vector
		 * \return Transformed vector
		 */
		constexpr auto TransformPoint(const Vec3<T>& vec) const noexcept -> Vec3<T>;
		/**
		 * Transform a 2D point by the matrix
		 * \param[in] vec Vector
		 * \return Transformed vector
		 */
		constexpr auto TransformPoint(const Vec2<T>& vec) const noexcept -> Vec2<T>;

		/**
		 * Decompose the translation matrix into a scale and quaternion
		 * \return Tuple with the scale and quaternion
		 */
		constexpr auto Decompose() const noexcept -> Tuple<Vec3<T>, Quaternion<T>, Vec3<T>>;
		/**
		 * Decompose the 2D translation matrix into a scale, angle, and transformation
		 * \return Tuple with the scale and quaternion
		 */
		constexpr auto Decompose2D() const noexcept -> Tuple<Vec2<T>, Angle<T, AngleKind::Radians>, Vec2<T>>;

		/**
		 * Get the scale and rotation matrix our of this matrix
		 * \return Scale & rotation matrix
		 */
		constexpr auto GetScaleRotationMatrix() const noexcept -> Mat3<T>;

		/**
		 * Create a 2D uniform scale matrix
		 * \tparam A Type of scale
		 * \param[in] scale Scale
		 * \return Scale matrix
		 */
		template<ConvertableTo<T> A>
		constexpr static auto CreateScale2D(A scale) noexcept -> Mat4;
		/**
		 * Create a 2D scale matrix
		 * \tparam A Type of scale's components
		 * \param[in] scale Scale
		 * \return Scale matrix
		 */
		template<ConvertableTo<T> A>
		constexpr static auto CreateScale2D(const Vec2<A>& scale) noexcept -> Mat4;

		/**
		 * Create a 2D shearing matrix
		 * \tparam A Type of shear's components
		 * \param[in] shear Shear
		 * \return Shearing matrix
		 */
		template<ConvertableTo<T> A>
		constexpr static auto CreateShear2D(const Vec2<A>& shear) noexcept -> Mat4;

		/**
		 * Create a 2D rotation matrix
		 * \tparam A Type of angle
		 * \tparam Kind Kind of angle
		 * \param[in] angle Angle
		 * \return Rotation matrix
		 */
		template<ConvertableTo<T> A, AngleKind Kind>
		constexpr static auto CreateRotation2D(const Angle<A, Kind>& angle) noexcept -> Mat4;

		/**
		 * Create a 2D translation matrix
		 * \tparam A Type of trans' components
		 * \param[in] trans Translation
		 * \return Translation matrix
		 */
		template<ConvertableTo<T> A>
		constexpr static auto CreateTranslation2D(const Vec2<A>& trans) noexcept -> Mat4;

		/**
		 * Create a 2D transformation matrix with a scale and an angle
		 * \tparam A Type of scale
		 * \tparam B Type of angle
		 * \tparam Kind Kind of angle
		 * \tparam C Type of translation
		 * \param[in] scale Scale
		 * \param[in] angle Angle
		 * \param[in] trans Translation
		 * \return Transform matrix
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, AngleKind Kind, ConvertableTo<T> C>
		constexpr static auto CreateTransform2D(A scale, const Angle<B, Kind>& angle, const Vec2<T>& trans) noexcept -> Mat4;
		/**
		 * Create a 2D transformation matrix with a uniform scale and an angle
		 * \tparam A Type of scale's components
		 * \tparam B Type of angle
		 * \tparam Kind Kind of angle
		 * \tparam C Type of translation
		 * \param[in] scale Scale
		 * \param[in] angle Angle
		 * \param[in] trans Translation
		 * \return Transform matrix
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, AngleKind Kind, ConvertableTo<T> C>
		constexpr static auto CreateTransform2D(const Vec2<A>& scale, const Angle<B, Kind>& angle, const Vec2<C>& trans) noexcept -> Mat4;

		/**
		 * Create a uniform scale matrix
		 * \tparam A Type of scale
		 * \param[in] scale Scale
		 * \return Scale matrix
		 */
		template<ConvertableTo<T> A>
		constexpr static auto CreateScale(A scale) noexcept -> Mat4;
		/**
		 * Create a scale matrix
		 * \tparam A Type of scale's components
		 * \param[in] scale Scale
		 * \return Scale matrix
		 */
		template<ConvertableTo<T> A>
		constexpr static auto CreateScale(const Vec3<A>& scale) noexcept -> Mat4;

		/**
		 * Create a rotation matrix
		 * \tparam A Type of Quaternion's components
		 * \param[in] quat Quaternion
		 * \return Rotation matrix
		 */
		template<ConvertableTo<T> A>
		constexpr static auto CreateRotation(const Quaternion<A>& quat) noexcept -> Mat4;

		/**
		 * Create a translation matrix
		 * \tparam A Type of trans' components
		 * \param[in] trans Translation
		 * \return Translation matrix
		 */
		template<ConvertableTo<T> A>
		constexpr static auto CreateTranslation(const Vec3<A>& trans) noexcept -> Mat4;

		/**
		 * Create a transformation matrix with a scale and an angle
		 * \tparam A Type of scale
		 * \tparam B Type of Quaternion's components
		 * \param[in] scale Scale
		 * \param[in] quat Quaternion
		 * \param[in] trans Translation
		 * \return Transform matrix
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C>
		constexpr static auto CreateTransform(A scale, const Quaternion<B>& quat, const Vec3<C>& trans) noexcept -> Mat4;
		/**
		 * Create a transformation matrix with a uniform scale and an angle
		 * \tparam A Type of scale's components
		 * \tparam B Type of Quaternion's components
		 * \param[in] scale Scale
		 * \param[in] quat Quaternion
		 * \param[in] trans Translation
		 * \return Transform matrix
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C>
		constexpr static auto CreateTransform(const Vec3<A>& scale, const Quaternion<B>& quat, const Vec3<C>& trans) noexcept -> Mat4;

		/**
		 * Create a look at-matrix
		 * \tparam A Type of eyePos' components
		 * \tparam B Type of focus position's components
		 * \tparam C Type of up's components
		 * \param eyePos Eye position
		 * \param focusPos Focus position
		 * \param up Up position
		 * \return Look at-matrix
		 * \note Matrix is left handed
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C>
		constexpr static auto CreateLookAt(const Vec3<A>& eyePos, const Vec3<B>& focusPos, const Vec3<T>& up) noexcept -> Mat4;
		/**
		 * Create a look-at matrix (right-handed)
		 * \tparam A Type of eyePos' components
		 * \tparam B Type of focus position's components
		 * \tparam C Type of up's components
		 * \param eyePos Eye position
		 * \param focusPos Focus position
		 * \param up Up position
		 * \return Look-at matrix
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C>
		constexpr static auto CreateLookAtRH(const Vec3<A>& eyePos, const Vec3<B>& focusPos, const Vec3<T>& up) noexcept -> Mat4;

		/**
		 * Create a look-to matrix
		 * \tparam A Type of eyePos' components
		 * \tparam B Type of focus position's components
		 * \tparam C Type of up's components
		 * \param eyePos Eye position
		 * \param lookDir Look direction
		 * \param up Up position
		 * \return Look-to matrix
		 * \note Matrix is left handed
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C>
		constexpr static auto CreateLookTo(const Vec3<A>& eyePos, const Vec3<B>& lookDir, const Vec3<T>& up) noexcept -> Mat4;
		/**
		 * Create a look-to matrix (right-handed)
		 * \tparam A Type of eyePos' components
		 * \tparam B Type of focus position's components
		 * \tparam C Type of up's components
		 * \param eyePos Eye position
		 * \param lookDir Look direction
		 * \param up Up position
		 * \return Look-to matrix
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C>
		constexpr static auto CreateLookToRH(const Vec3<A>& eyePos, const Vec3<B>& lookDir, const Vec3<T>& up) noexcept -> Mat4;

		/**
		 * Create an orthographic projection matrix
		 * \tparam A Type of width
		 * \tparam B Type of height
		 * \tparam C Type of z-near
		 * \tparam D Type of z-far
		 * \param width Width
		 * \param height Height
		 * \param zNear Z-near
		 * \param zFar Z-far
		 * \return Orthographic projection
		 * \note Matrix is left handed
		 * \note Expects NDC coordinates with Z in range [0;1], if a range of [-1;1] is needed, m22 needs to be multiplied by -2
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D>
		constexpr static auto CreateOrtho(A width, B height, C zNear, D zFar) noexcept -> Mat4;
		/**
		 * Create an orthographic projection matrix (right-handed)
		 * \tparam A Type of width
		 * \tparam B Type of height
		 * \tparam C Type of z-near
		 * \tparam D Type of z-far
		 * \param width Width
		 * \param height Height
		 * \param zNear Z-near
		 * \param zFar Z-far
		 * \return Orthographic projection
		 * \note Matrix is left handed
		 * \note Expects NDC coordinates with Z in range [0;1], if a range of [-1;1] is needed, m22 needs to be multiplied by -2
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D>
		constexpr static auto CreateOrthoRH(A width, B height, C zNear, D zFar) noexcept -> Mat4;

		/**
		 * Create an off-center orthographic projection matrix
		 * \tparam A Type of left
		 * \tparam B Type of right
		 * \tparam C Type of top
		 * \tparam D Type of bottom
		 * \tparam E Type of z-near
		 * \tparam F Type of z-far
		 * \param left Left side of the view
		 * \param right Right side of the view
		 * \param top Top side of the view
		 * \param bottom Bottom side of the view
		 * \param zNear Z-near
		 * \param zFar Z-far
		 * \return Orthographic projection
		 * \note Matrix is left handed
		 * \note Expects NDC coordinates with Z in range [0;1], if a range of [-1;1] is needed, m22 needs to be multiplied by -2
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D, ConvertableTo<T> E, ConvertableTo<T> F>
		constexpr static auto CreateOrtho(A left, B right, C top, D bottom, E zNear, F zFar) noexcept -> Mat4;
		/**
		 * Create an off-center orthographic projection matrix (right-handed)
		 * \tparam A Type of left
		 * \tparam B Type of right
		 * \tparam C Type of top
		 * \tparam D Type of bottom
		 * \tparam E Type of z-near
		 * \tparam F Type of z-far
		 * \param left Left side of the view
		 * \param right Right side of the view
		 * \param top Top side of the view
		 * \param bottom Bottom side of the view
		 * \param zNear Z-near
		 * \param zFar Z-far
		 * \return Orthographic projection
		 * \note Matrix is left handed
		 * \note Expects NDC coordinates with Z in range [0;1], if a range of [-1;1] is needed, m22 needs to be multiplied by -2
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D, ConvertableTo<T> E, ConvertableTo<T> F>
		constexpr static auto CreateOrthoRH(A left, B right, C top, D bottom, E zNear, F zFar) noexcept -> Mat4;

		/**
		 * Create an perspective projection matrix
		 * \tparam A Type of width
		 * \tparam B Type of height
		 * \tparam C Type of z-near
		 * \tparam D Type of z-far
		 * \param width Width
		 * \param height Height
		 * \param zNear Z-near
		 * \param zFar Z-far
		 * \return Perspective projection
		 * \note Matrix is left handed
		 * \note Expects NDC coordinates with Z in range [0;1], if a range of [-1;1] is needed, m22 needs to be multiplied by -2
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D>
		constexpr static auto CreatePerspective(A width, B height, C zNear, D zFar) noexcept -> Mat4;
		/**
		 * Create an perspective projection matrix (right-handed)
		 * \tparam A Type of width
		 * \tparam B Type of height
		 * \tparam C Type of z-near
		 * \tparam D Type of z-far
		 * \param width Width
		 * \param height Height
		 * \param zNear Z-near
		 * \param zFar Z-far
		 * \return Perspective projection
		 * \note Matrix is left handed
		 * \note Expects NDC coordinates with Z in range [0;1], if a range of [-1;1] is needed, m22 needs to be multiplied by -2
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D>
		constexpr static auto CreatePerspectiveRH(A width, B height, C zNear, D zFar) noexcept -> Mat4;

		/**
		 * Create an off-center perspective projection matrix
		 * \tparam A Type of left
		 * \tparam B Type of right
		 * \tparam C Type of top
		 * \tparam D Type of bottom
		 * \tparam E Type of z-near
		 * \tparam F Type of z-far
		 * \param left Left side of the view
		 * \param right Right side of the view
		 * \param top Top side of the view
		 * \param bottom Bottom side of the view
		 * \param zNear Z-near
		 * \param zFar Z-far
		 * \return Perspective projection
		 * \note Matrix is left handed
		 * \note Expects NDC coordinates with Z in range [0;1], if a range of [-1;1] is needed, m22 needs to be multiplied by -2
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D, ConvertableTo<T> E, ConvertableTo<T> F>
		constexpr static auto CreatePerspective(A left, B right, C top, D bottom, E zNear, F zFar) noexcept -> Mat4;
		/**
		 * Create an off-center perspective projection matrix (right-handed)
		 * \tparam A Type of left
		 * \tparam B Type of right
		 * \tparam C Type of top
		 * \tparam D Type of bottom
		 * \tparam E Type of z-near
		 * \tparam F Type of z-far
		 * \param left Left side of the view
		 * \param right Right side of the view
		 * \param top Top side of the view
		 * \param bottom Bottom side of the view
		 * \param zNear Z-near
		 * \param zFar Z-far
		 * \return Perspective projection
		 * \note Matrix is left handed
		 * \note Expects NDC coordinates with Z in range [0;1], if a range of [-1;1] is needed, m22 needs to be multiplied by -2
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D, ConvertableTo<T> E, ConvertableTo<T> F>
		constexpr static auto CreatePerspectiveRH(A left, B right, C top, D bottom, E zNear, F zFar) noexcept -> Mat4;

		/**
		 * Create an perspective projection matrix
		 * \tparam A Type of fov
		 * \tparam AK Angle kind of fov
		 * \tparam B Type of aspect ratio
		 * \tparam C Type of z-near
		 * \tparam D Type of z-far
		 * \param fov Field of view angle (Y-direction)
		 * \param aspect Aspect ratio
		 * \param zNear Z-near
		 * \param zFar Z-far
		 * \return Perspective projection
		 * \note Matrix is left handed
		 * \note Expects NDC coordinates with Z in range [0;1], if a range of [-1;1] is needed, m22 needs to be multiplied by -2
		 */
		template<ConvertableTo<T> A, AngleKind AK, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D>
		constexpr static auto CreatePerspectiveFov(const Angle<A, AK>& fov, B aspect, C zNear, D zFar) noexcept -> Mat4;
		/**
		 * Create an perspective projection matrix (right-handed)
		 * \tparam A Type of fov
		 * \tparam AK Angle kind of fov
		 * \tparam B Type of aspect ratio
		 * \tparam C Type of z-near
		 * \tparam D Type of z-far
		 * \param fov Field of view angle (Y-direction)
		 * \param aspect Aspect ratio
		 * \param zNear Z-near
		 * \param zFar Z-far
		 * \return Perspective projection
		 * \note Matrix is left handed
		 * \note Expects NDC coordinates with Z in range [0;1], if a range of [-1;1] is needed, m22 needs to be multiplied by -2
		 */
		template<ConvertableTo<T> A, AngleKind AK, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D>
		constexpr static auto CreatePerspectiveFovRH(const Angle<A, AK>& fov, B aspect, C zNear, D zFar) noexcept -> Mat4;
	};

	template<Numeric T, ConvertableTo<T> U>
	constexpr auto operator*(const Vec4<T>& vec, const Mat4<U>& mat) noexcept -> Vec4<T>;
	template<Numeric T, ConvertableTo<T> U>
	constexpr auto operator*=(Vec4<T>& vec, const Mat4<U>& mat) noexcept -> Vec4<T>&;
}
