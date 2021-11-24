#pragma once

#if __RESHARPER__
#include "Mat2.h"
#endif

#include "Constants.h"
#include "Mat2.h"
#include "core/Assert.h"
#include "utils/Algo.h"

namespace Core::Math
{
	template <Numeric T>
	constexpr Mat2<T>::Mat2() noexcept
		: m00(T(1)) , m01(T(0))
		, m10(T(0)) , m11(T(1))
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr Mat2<T>::Mat2(A diag) noexcept
		: m00(T(diag)        ), m01(T(0))
		, m10(T(0)), m11(T(diag)        )
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> U0, ConvertableTo<T> U1, ConvertableTo<T> U2, ConvertableTo<T> U3>
	constexpr Mat2<T>::Mat2(U0 m00, U1 m01, U2 m10, U3 m11) noexcept
		: m00(T(m00)), m01(T(m01))
		, m10(T(m10)), m11(T(m11))
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> U0, ConvertableTo<T> U1>
	constexpr Mat2<T>::Mat2(const Vec2<U0>& row0, const Vec2<U1>& row1)
		: row0(row0)
		, row1(row1)
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> U0, ConvertableTo<T> U1>
	constexpr Mat2<T>::Mat2(ColumnInitTag, const Vec2<U0>& column0, const Vec2<U1>& column1)
		: m00(T(column0.x)), m01(T(column1.x))
		, m10(T(column0.y)), m11(T(column1.y))
	{
	}

	template <Numeric T>
	constexpr Mat2<T>::Mat2(const Mat2& other) noexcept
		: row0(other.row0)
		, row1(other.row1)
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr Mat2<T>::Mat2(const Mat2<A>& other) noexcept
		: row0(other.row0)
		, row1(other.row1)
	{
	}

	template <Numeric T>
	constexpr Mat2<T>::Mat2(Mat2&& other) noexcept
		: row0(Move(other.row0))
		, row1(Move(other.row1))
	{
	}

	template <Numeric T>
	constexpr auto Mat2<T>::operator=(const Mat2& other) noexcept -> Mat2&
	{
		row0 = other.row0;
		row1 = other.row1;
		return *this;
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat2<T>::operator=(const Mat2<A>& other) noexcept -> Mat2&
	{
		row0 = other.row0;
		row1 = other.row1;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat2<T>::operator=(Mat2&& other) noexcept -> Mat2&
	{
		row0 = Move(other.row0);
		row1 = Move(other.row1);
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat2<T>::operator+() const noexcept -> Mat2
	{
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat2<T>::operator-() const noexcept -> Mat2
	{
		return { -row0, -row1 };
	}

	template <Numeric T>
	constexpr auto Mat2<T>::operator+(const Mat2& other) const noexcept -> Mat2
	{
		return { row0 + other.row0, row1 + other.row1 };
	}

	template <Numeric T>
	constexpr auto Mat2<T>::operator-(const Mat2& other) const noexcept -> Mat2
	{
		return { row0 - other.row0, row1 - other.row1 };
	}

	template <Numeric T>
	constexpr auto Mat2<T>::operator*(const Mat2& other) const noexcept -> Mat2
	{
		return
		{
			m00 * other.m00 + m01 * other.m10, // row0 dot column0
			m00 * other.m01 + m01 * other.m11, // row1 dot column0
			m10 * other.m00 + m11 * other.m10, // row0 dot column1
			m10 * other.m01 + m11 * other.m11  // row1 dot column1
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat2<T>::operator*(A val) const noexcept -> Mat2
	{
		return
		{
			row0 * val,
			row1 * val
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat2<T>::operator/(A val) const noexcept -> Mat2
	{
		return
		{
			row0 / val,
			row1 / val
		};
	}
	
	template <Numeric T>
	constexpr auto Mat2<T>::operator+=(const Mat2& other) noexcept -> Mat2&
	{
		row0 += other.row0;
		row1 += other.row1;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat2<T>::operator-=(const Mat2& other) noexcept -> Mat2&
	{
		row0 -= other.row0;
		row1 -= other.row1;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat2<T>::operator*=(const Mat2& other) noexcept -> Mat2&
	{
		T tmp00 = m00 * other.m00 + m01 * other.m10; // row0 dot column0
		T tmp01 = m00 * other.m01 + m01 * other.m11; // row1 dot column0
		T tmp10 = m10 * other.m00 + m11 * other.m10; // row0 dot column1
		T tmp11 = m10 * other.m01 + m11 * other.m11; // row1 dot column1
		m00 = tmp00;
		m01 = tmp01;
		m10 = tmp10;
		m11 = tmp11;
		return *this;
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat2<T>::operator*=(A val) noexcept -> Mat2&
	{
		row0 *= val;
		row1 *= val;
		return *this;
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat2<T>::operator/=(A val) noexcept -> Mat2&
	{
		row0 /= val;
		row1 /= val;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat2<T>::operator[](usize idx) noexcept -> T&
	{
		MATH_ASSERT(idx < 4, "Index out of range");
		return data[idx];
	}

	template <Numeric T>
	constexpr auto Mat2<T>::operator[](usize idx) const noexcept -> T
	{
		MATH_ASSERT(idx < 4, "Index out of range");
		return data[idx];
	}

	template <Numeric T>
	constexpr auto Mat2<T>::operator()(usize row, usize column) noexcept -> T&
	{
		MATH_ASSERT(row    < 2, "Row index out of range");
		MATH_ASSERT(column < 2, "Column index out of range");
		return rows[row][column];
	}

	template <Numeric T>
	constexpr auto Mat2<T>::operator()(usize row, usize column) const noexcept -> T
	{
		MATH_ASSERT(column < 2, "Column index out of range");
		MATH_ASSERT(row    < 2, "Row index out of range");
		return rows[row][column];
	}

	template <Numeric T>
	constexpr auto Mat2<T>::operator==(const Mat2& other) const noexcept -> bool
	{
		return row0 == other.row0 && row1 == other.row1;
	}

	template <Numeric T>
	constexpr auto Mat2<T>::operator!=(const Mat2& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <Numeric T>
	constexpr auto Mat2<T>::Compare(const Mat2& other, T e) const noexcept -> bool
	{
		return row0.Compare(other.row0, e) && row1.Compare(other.row1, e);
	}

	template <Numeric T>
	constexpr auto Mat2<T>::Row(usize idx) noexcept -> Vec2<T>&
	{
		MATH_ASSERT(idx < 2, "Row index out of range");
		return rows[idx];
	}

	template <Numeric T>
	constexpr auto Mat2<T>::Row(usize idx) const noexcept -> Vec2<T>
	{
		MATH_ASSERT(idx < 2, "Row index out of range");
		return rows[idx];
	}

	template <Numeric T>
	constexpr auto Mat2<T>::SetRow(usize idx, const Vec2<T>& row) noexcept -> void
	{
		rows[idx] = row;
	}

	template <Numeric T>
	constexpr auto Mat2<T>::Column(usize idx) const noexcept -> Vec2<T>
	{
		MATH_ASSERT(idx < 2, "Column index out of range");
		return { row0[idx], row1[idx] };
	}

	template <Numeric T>
	constexpr auto Mat2<T>::SetColumn(usize idx, const Vec2<T>& column) noexcept -> void
	{
		MATH_ASSERT(idx < 2, "Column index out of range");
		row0[idx] = column.x;
		row1[idx] = column.y;
	}

	template <Numeric T>
	constexpr auto Mat2<T>::Diagonal() const noexcept -> Vec2<T>
	{
		return { m00, m11 };
	}

	template <Numeric T>
	constexpr auto Mat2<T>::Determinant() const noexcept -> T
	{
		return m00 * m11 - m01 * m10;
	}

	template <Numeric T>
	constexpr auto Mat2<T>::Trace() const noexcept -> T
	{
		return m00 + m11;
	}

	template <Numeric T>
	constexpr auto Mat2<T>::Transpose() noexcept -> Mat2&
	{
		Algo::Swap(m01, m10);
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat2<T>::Transposed() const noexcept -> Mat2
	{
		return
		{
			m00, m10,
			m01, m11
		};
	}

	template <Numeric T>
	constexpr auto Mat2<T>::Adjugate() noexcept -> Mat2&
	{
		m01 = -m01;
		m10 = -m10;
		Algo::Swap(m00, m11);
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat2<T>::Adjoint() const noexcept -> Mat2
	{
		return
		{
			 m11, -m01,
			-m10,  m00
		};
	}

	template <Numeric T>
	constexpr auto Mat2<T>::ToCofactor() noexcept -> Mat2&
	{
		T tmp = -m10;
		m10 = -m01;
		m01 = tmp;
		Algo::Swap(m00, m11);
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat2<T>::Cofactor() const noexcept -> Mat2
	{
		return
		{
			m11, -m10,
			-m01, m00
		};
	}

	template <Numeric T>
	constexpr auto Mat2<T>::Invert() noexcept -> Mat2&
	{
		T det = Determinant();
		if (EpsilonCompare(det, 0))
		{
			*this = Mat2{};
			return *this;
		}

		Adjugate();
		*this *= Rcp(det);
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat2<T>::Inverse() const noexcept -> Mat2
	{
		T det = Determinant();
		if (EpsilonCompare(det, 0))
			return Mat2{ 0 };
		return Adjoint() * Rcp(det);
	}

	template <Numeric T>
	constexpr auto Mat2<T>::TransformVector(const Vec2<T>& vec) const noexcept -> Vec2<T>
	{
		return vec.x * row0 + vec.y * row1;
	}

	template <Numeric T>
	constexpr auto Mat2<T>::Decompose() const noexcept -> Pair<Vec2<T>, Angle<T, AngleKind::Radians>>
	{
		Vec2<T> scale;
		scale.x = Column(0).Len();
		scale.y = Column(1).Len();

		Angle<T, AngleKind::Radians> angle = ACos(m00 / scale.x);

		return { scale, angle };
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat2<T>::CreateScale(A scale) noexcept -> Mat2
	{
		return Mat2{ scale };
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat2<T>::CreateScale(const Vec2<A>& scale) noexcept -> Mat2
	{
		return
		{
			T(scale.x), T(0)      ,
			T(0)      , T(scale.y)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat2<T>::CreateShear(const Vec2<A>& shear) noexcept -> Mat2
	{
		return
		{
			T(1)      , T(shear.y),
			T(shear.x), T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, AngleKind Kind>
	constexpr auto Mat2<T>::CreateRotation(const Angle<A, Kind>& angle) noexcept -> Mat2
	{
		T cs = T(Cos(angle));
		T sn = T(Sin(angle));
		return
		{
			cs, -sn,
			sn,  cs
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, AngleKind Kind>
	constexpr auto Mat2<T>::CreateTransform(A scale, const Angle<B, Kind>& angle) noexcept -> Mat2
	{
		T cs = T(Cos(angle));
		T sn = T(Sin(angle));
		return
		{
			T(scale) * cs, T(scale) * -sn,
			T(scale) * sn, T(scale) *  cs
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, AngleKind Kind>
	constexpr auto Mat2<T>::CreateTransform(const Vec2<A>& scale, const Angle<B, Kind>& angle) noexcept -> Mat2
	{
		T cs = T(Cos(angle));
		T sn = T(Sin(angle));
		return
		{
			T(scale.x) * cs, T(scale.y) * -sn,
			T(scale.x) * sn, T(scale.y) *  cs
		};
	}

	template <Numeric T, ConvertableTo<T> U>
	constexpr auto operator*(const Vec2<T>& vec, const Mat2<U>& mat) noexcept -> Vec2<T>
	{
		return vec.x * mat.row0 + vec.y * mat.row1;
	}

	template <Numeric T, ConvertableTo<T> U>
	constexpr auto operator*=(Vec2<T>& vec, const Mat2<U>& mat) noexcept -> Vec2<T>&
	{
		vec = vec.x * mat.row0 + vec.y * mat.row1;
		return vec;
	}
}
