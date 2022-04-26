#pragma once
#include "core/MinInclude.h"
#include "Concepts.h"
#include "FwdDecl.h"
#include "MathUtils.h"
#include "Vec2.h"

namespace Core::Math
{

	/**
	 * 2x2 matrix
	 * \tparam T Numeric type
	 * \note row-major
	 */
	template<Numeric T>
	struct Mat2
	{
		union 
		{
			struct { T m00, m01, m10, m11; };
			struct { Vec2<T> row0, row1; };
			Vec2<T> rows[2];
			T data[4];
		};

		/**
		 * Create an identity matrix
		 */
		constexpr Mat2() noexcept;
		/**
		 * Create a matrix with the diagonal set to the given value
		 * \tparam A Type of value
		 * \param diag Diagonal value
		 */
		template<ConvertableTo<T> A>
		explicit constexpr Mat2(A diag) noexcept;
		/**
		 * Create a matrix with the given values
		 * \tparam U0 Type of element [0,0]
		 * \tparam U1 Type of element [0,1]
		 * \tparam U2 Type of element [1,0]
		 * \tparam U3 Type of element [1,1]
		 * \param m00 Element [0,0]
		 * \param m01 Element [0,1]
		 * \param m10 Element [1,0]
		 * \param m11 Element [1,1]
		 */
		template<ConvertableTo<T> U0, ConvertableTo<T> U1, ConvertableTo<T> U2, ConvertableTo<T> U3>
		constexpr Mat2(U0 m00, U1 m01, U2 m10, U3 m11) noexcept;
		/**
		 * Create a matrix with the given rows
		 * \tparam U0 Type of row 0's components
		 * \tparam U1 Type of row 1's components
		 * \param row0 Row 0
		 * \param row1 Row 1
		 */
		template<ConvertableTo<T> U0, ConvertableTo<T> U1>
		constexpr Mat2(const Vec2<U0>& row0, const Vec2<U1>& row1);
		/**
		 * Create a matrix with the given rows
		 * \tparam U0 Type of column 0's components
		 * \tparam U1 Type of column 1's components
		 * \param column0 Column 0
		 * \param column1 Column 1
		 */
		template<ConvertableTo<T> U0, ConvertableTo<T> U1>
		constexpr Mat2(ColumnInitTag, const Vec2<U0>& column0, const Vec2<U1>& column1);
		/**
		 * Create a matrix from another matrix
		 * \param other Other matrix
		 */
		constexpr Mat2(const Mat2& other) noexcept;
		/**
		 * Create a matrix from another matrix
		 * \tparam A Type of other matrix' components
		 * \param other Other matrix
		 */
		template<ConvertableTo<T> A>
		constexpr Mat2(const Mat2<A>& other) noexcept;
		/**
		 * Create a matrix from a moved matrix
		 * \param other Other matrix
		 */
		constexpr Mat2(Mat2&& other) noexcept;

		constexpr auto operator=(const Mat2& other) noexcept -> Mat2&;
		template<ConvertableTo<T> A>
		constexpr auto operator=(const Mat2<A>& other) noexcept -> Mat2&;
		constexpr auto operator=(Mat2&& other) noexcept -> Mat2&;

		constexpr auto operator+() const noexcept -> Mat2;
		constexpr auto operator-() const noexcept -> Mat2;

		constexpr auto operator+(const Mat2& other) const noexcept -> Mat2;
		constexpr auto operator-(const Mat2& other) const noexcept -> Mat2;
		constexpr auto operator*(const Mat2& other) const noexcept -> Mat2;

		template<ConvertableTo<T> A>
		constexpr auto operator*(A val) const noexcept -> Mat2;
		template<ConvertableTo<T> A>
		constexpr auto operator/(A val) const noexcept -> Mat2;

		constexpr auto operator+=(const Mat2& other) noexcept -> Mat2&;
		constexpr auto operator-=(const Mat2& other) noexcept -> Mat2&;
		constexpr auto operator*=(const Mat2& other) noexcept -> Mat2&;

		template<ConvertableTo<T> A>
		constexpr auto operator*=(A val) noexcept -> Mat2&;
		template<ConvertableTo<T> A>
		constexpr auto operator/=(A val) noexcept -> Mat2&;

		constexpr auto operator[](usize idx) noexcept -> T&;
		constexpr auto operator[](usize idx) const noexcept -> T;

		constexpr auto operator()(usize row, usize column) noexcept -> T&;
		constexpr auto operator()(usize row, usize column) const noexcept -> T;

		constexpr auto operator==(const Mat2& other) const noexcept -> bool;
		constexpr auto operator!=(const Mat2& other) const noexcept -> bool;

		/**
		 * Compare 2 matrices using an epsilon
		 * \param[in] other Matrix to compare with
		 * \param[in] e Epsilon
		 * \return Whether the matrices are equal
		 */
		constexpr auto Compare(const Mat2& other, T e = Consts::MathEpsilon<T>) const noexcept -> bool;

		/**
		 * Get the row at the given index
		 * \param idx Index
		 * \return Row
		 */
		constexpr auto Row(usize idx) noexcept -> Vec2<T>&;
		/**
		 * Get the row at the given index
		 * \param idx Index
		 * \return Row
		 */
		constexpr auto Row(usize idx) const noexcept -> Vec2<T>;
		/**
		 * Set the row at the given index
		 * \param idx Index
		 * \param row Row
		 * \return Row
		 */
		constexpr void SetRow(usize idx, const Vec2<T>& row) noexcept;
		/**
		 * Get the column at the given index
		 * \param idx Index
		 * \return Column
		 */
		constexpr auto Column(usize idx) const noexcept -> Vec2<T>;
		/**
		 * Get the column at the given index
		 * \param idx Index
		 * \param column Column
		 * \return Column
		 */
		constexpr void SetColumn(usize idx, const Vec2<T>& column) noexcept;
		/**
		 * Get the diagonal of the matrix
		 * \return Diagonal
		 */
		constexpr auto Diagonal() const noexcept -> Vec2<T>;

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
		 * Transpose the matrix
		 * \return Reference to the matrix
		 */
		constexpr auto Transpose() noexcept -> Mat2&;
		/**
		 * Get the transposed matrix
		 * \return Transposed matrix
		 */
		constexpr auto Transposed() const noexcept -> Mat2;

		/**
		 * Adjugate the matrix
		 * \return Reference to the matrix
		 */
		constexpr auto Adjugate() noexcept -> Mat2&;
		/**
		 * Get the adjunct matrix (adjugated)
		 * \return Adjunct matrix
		 */
		constexpr auto Adjoint() const noexcept -> Mat2;
		/**
		 * Convert to the cofactor of the matrix
		 * \return Reference to the matrix
		 */
		constexpr auto ToCofactor() noexcept -> Mat2&;
		/**
		 * Get the cofactor of the matrix
		 * \return Cofactor of the matrix
		 */
		constexpr auto Cofactor() const noexcept -> Mat2;

		/**
		 * Invert the matrix
		 * \return Reference to the matrix
		 */
		constexpr auto Invert() noexcept -> Mat2&;
		/**
		 * Get the inverse of the matrix
		 * \return Inverse of the matrix
		 */
		constexpr auto Inverse() const noexcept -> Mat2;

		/**
		 * Transform a vector by the matrix
		 * \param vec Vector
		 * \return Transformed vector
		 */
		constexpr auto TransformVector(const Vec2<T>& vec) const noexcept -> Vec2<T>;

		/**
		 * Decompose a translation matrix into a scale and an angle
		 * \return Pair with scale and angle
		 */
		constexpr auto Decompose() const noexcept -> Pair<Vec2<T>, Angle<T, AngleKind::Radians>>;

		/**
		 * Create a 2D uniform scale matrix
		 * \tparam A Type of scale
		 * \param scale Scale
		 * \return Scale matrix
		 */
		template<ConvertableTo<T> A>
		constexpr static auto CreateScale(A scale) noexcept -> Mat2;
		/**
		 * Create a 2D scale matrix
		 * \tparam A Type of scale's components
		 * \param scale Scale
		 * \return Scale matrix
		 */
		template<ConvertableTo<T> A>
		constexpr static auto CreateScale(const Vec2<A>& scale) noexcept -> Mat2;

		/**
		 * Create a shearing matrix
		 * \tparam A Type of shear's components
		 * \param shear Shear
		 * \return Shearing matrix
		 */
		template<ConvertableTo<T> A>
		constexpr static auto CreateShear(const Vec2<A>& shear) noexcept -> Mat2;

		/**
		 * Create a 2D rotation matrix
		 * \tparam A Type of angle
		 * \tparam Kind Kind of angle
		 * \param angle Angle
		 * \return Rotation matrix
		 */
		template<ConvertableTo<T> A, AngleKind Kind>
		constexpr static auto CreateRotation(const Angle<A, Kind>& angle) noexcept -> Mat2;

		/**
		 * Create a 2D transformation matrix with a scale and an angle
		 * \tparam A Type of scale
		 * \tparam B Type of angle
		 * \tparam Kind Kind of angle
		 * \param scale Scale
		 * \param angle Angle
		 * \return Transform matrix
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, AngleKind Kind>
		constexpr static auto CreateTransform(A scale, const Angle<B, Kind>& angle) noexcept -> Mat2;
		/**
		 * Create a 2D transformation matrix with a uniform scale and an angle
		 * \tparam A Type of scale's components
		 * \tparam B Type of angle
		 * \tparam Kind Kind of angle
		 * \param scale Scale
		 * \param angle Angle
		 * \return Transform matrix
		 */
		template<ConvertableTo<T> A, ConvertableTo<T> B, AngleKind Kind>
		constexpr static auto CreateTransform(const Vec2<A>& scale, const Angle<B, Kind>& angle) noexcept -> Mat2;
	};

	template<Numeric T, ConvertableTo<T> U>
	constexpr auto operator*(const Vec2<T>& vec, const Mat2<U>& mat) noexcept -> Vec2<T>;
	template<Numeric T, ConvertableTo<T> U>
	constexpr auto operator*=(Vec2<T>& vec, const Mat2<U>& mat) noexcept -> Vec2<T>&;
}
