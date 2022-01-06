#pragma once

#if __RESHARPER__
#include "Mat4.h"
#endif

#include "Constants.h"
#include "Mat3.h"
#include "core/Assert.h"
#include "core/utils/Algo.h"

namespace Core::Math
{
	template <Numeric T>
	constexpr Mat4<T>::Mat4() noexcept
		: row0(1, 0, 0, 0)
		, row1(0, 1, 0, 0)
		, row2(0, 0, 1, 0)
		, row3(0, 0, 0, 1)
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr Mat4<T>::Mat4(A diag) noexcept
		: row0(diag, 0, 0, 0)
		, row1(0, diag, 0, 0)
		, row2(0, 0, diag, 0)
		, row3(0, 0, 0, diag)
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> U0, ConvertableTo<T> U1, ConvertableTo<T> U2, ConvertableTo<T> U3,
		ConvertableTo<T> U4, ConvertableTo<T> U5, ConvertableTo<T> U6, ConvertableTo<T> U7,
		ConvertableTo<T> U8, ConvertableTo<T> U9, ConvertableTo<T> U10, ConvertableTo<T> U11,
		ConvertableTo<T> U12, ConvertableTo<T> U13, ConvertableTo<T> U14, ConvertableTo<T> U15>
	constexpr Mat4<T>::Mat4(U0 m00, U1 m01, U2 m02, U3 m03, U4 m10, U5 m11, U6 m12, U7 m13, U8 m20, U9 m21, U10 m22, U11 m23, U12 m30, U13 m31, U14 m32,
		U15 m33) noexcept
		: row0(m00, m01, m02, m03)
		, row1(m10, m11, m12, m13)
		, row2(m20, m21, m22, m23)
		, row3(m30, m31, m32, m33)
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> U0, ConvertableTo<T> U1, ConvertableTo<T> U2, ConvertableTo<T> U3>
	constexpr Mat4<T>::Mat4(const Vec4<U0>& row0, const Vec4<U1>& row1, const Vec4<U2>& row2, const Vec4<U3>& row3)
		: row0(row0)
		, row1(row1)
		, row2(row2)
		, row3(row3)
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> U0, ConvertableTo<T> U1, ConvertableTo<T> U2, ConvertableTo<T> U3>
	constexpr Mat4<T>::Mat4(ColumnInitTag, const Vec4<U0>& column0, const Vec4<U1>& column1, const Vec4<U2>& column2, const Vec4<U3>& column3)
		: row0(column0.x, column1.x, column2.x, column3.x)
		, row1(column0.y, column1.y, column2.y, column3.y)
		, row2(column0.z, column1.z, column2.z, column3.z)
		, row3(column0.w, column1.w, column2.w, column3.w)
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr Mat4<T>::Mat4(const Mat43<A>& other) noexcept
		: row0(other.row0, 0)
		, row1(other.row1, 0)
		, row2(other.row2, 0)
		, row3(other.row3, 1)
	{
	}

	template <Numeric T>
	constexpr Mat4<T>::Mat4(const Mat4& other) noexcept
		: row0(other.row0)
		, row1(other.row1)
		, row2(other.row2)
		, row3(other.row3)
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr Mat4<T>::Mat4(const Mat4<A>& other) noexcept
		: row0(other.row0)
		, row1(other.row1)
		, row2(other.row2)
		, row3(other.row3)
	{
	}

	template <Numeric T>
	constexpr Mat4<T>::Mat4(Mat4&& other) noexcept
		: row0(other.row0)
		, row1(other.row1)
		, row2(other.row2)
		, row3(other.row3)
	{
	}

	template <Numeric T>
	constexpr auto Mat4<T>::operator=(const Mat4& other) noexcept -> Mat4&
	{
		row0 = other.row0;
		row1 = other.row1;
		row2 = other.row2;
		row3 = other.row3;
		return *this;
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat4<T>::operator=(const Mat4<A>& other) noexcept -> Mat4&
	{
		row0 = other.row0;
		row1 = other.row1;
		row2 = other.row2;
		row3 = other.row3;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat4<T>::operator=(Mat4&& other) noexcept -> Mat4&
	{
		row0 = other.row0;
		row1 = other.row1;
		row2 = other.row2;
		row3 = other.row3;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat4<T>::operator+() const noexcept -> Mat4
	{
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat4<T>::operator-() const noexcept -> Mat4
	{
		return
		{
			-row0,
			-row1,
			-row2,
			-row3
		};
	}

	template <Numeric T>
	constexpr auto Mat4<T>::operator+(const Mat4& other) const noexcept -> Mat4
	{
		return
		{
			row0 + other.row0,
			row1 + other.row1,
			row2 + other.row2,
			row3 + other.row3
		};
	}

	template <Numeric T>
	constexpr auto Mat4<T>::operator-(const Mat4& other) const noexcept -> Mat4
	{
		return
		{
			row0 - other.row0,
			row1 - other.row1,
			row2 - other.row2,
			row3 - other.row3
		};
	}

	template <Numeric T>
	constexpr auto Mat4<T>::operator*(const Mat4& other) const noexcept -> Mat4
	{
		Vec4<T> column0 = other.Column(0);
		Vec4<T> column1 = other.Column(1);
		Vec4<T> column2 = other.Column(2);
		Vec4<T> column3 = other.Column(3);

		return
		{
			row0.Dot(column0),
			row0.Dot(column1),
			row0.Dot(column2),
			row0.Dot(column3),

			row1.Dot(column0),
			row1.Dot(column1),
			row1.Dot(column2),
			row1.Dot(column3),

			row2.Dot(column0),
			row2.Dot(column1),
			row2.Dot(column2),
			row2.Dot(column3),

			row3.Dot(column0),
			row3.Dot(column1),
			row3.Dot(column2),
			row3.Dot(column3)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat4<T>::operator*(A val) const noexcept -> Mat4
	{
		return
		{
			row0 * val,
			row1 * val,
			row2 * val,
			row3 * val
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat4<T>::operator/(A val) const noexcept -> Mat4
	{
		return
		{
			row0 / val,
			row1 / val,
			row2 / val,
			row3 / val
		};
	}
	
	template <Numeric T>
	constexpr auto Mat4<T>::operator+=(const Mat4& other) noexcept -> Mat4&
	{
		row0 += other.row0;
		row1 += other.row1;
		row2 += other.row2;
		row3 += other.row3;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat4<T>::operator-=(const Mat4& other) noexcept -> Mat4&
	{
		row0 -= other.row0;
		row1 -= other.row1;
		row2 -= other.row2;
		row3 -= other.row3;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat4<T>::operator*=(const Mat4& other) noexcept -> Mat4&
	{
		Vec4<T> column0 = other.Column(0);
		Vec4<T> column1 = other.Column(1);
		Vec4<T> column2 = other.Column(2);
		Vec4<T> column3 = other.Column(3);

		T tmp00 = row0.Dot(column0);
		T tmp01 = row0.Dot(column1);
		T tmp02 = row0.Dot(column2);
		T tmp03 = row0.Dot(column3);

		T tmp10 = row1.Dot(column0);
		T tmp11 = row1.Dot(column1);
		T tmp12 = row1.Dot(column2);
		T tmp13 = row1.Dot(column3);

		T tmp20 = row2.Dot(column0);
		T tmp21 = row2.Dot(column1);
		T tmp22 = row2.Dot(column2);
		T tmp23 = row2.Dot(column3);

		T tmp30 = row3.Dot(column0);
		T tmp31 = row3.Dot(column1);
		T tmp32 = row3.Dot(column2);
		T tmp33 = row3.Dot(column3);

		m00 = tmp00; m01 = tmp01; m02 = tmp02; m03 = tmp03;
		m10 = tmp10; m11 = tmp11; m12 = tmp12; m13 = tmp13;
		m20 = tmp20; m21 = tmp21; m22 = tmp22; m23 = tmp23;
		m30 = tmp30; m31 = tmp31; m32 = tmp32; m33 = tmp33;
		return *this;
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat4<T>::operator*=(A val) noexcept -> Mat4&
	{
		row0 *= val;
		row1 *= val;
		row2 *= val;
		row3 *= val;
		return *this;
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat4<T>::operator/=(A val) noexcept -> Mat4&
	{
		row0 /= val;
		row1 /= val;
		row2 /= val;
		row3 /= val;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat4<T>::operator[](usize idx) noexcept -> T&
	{
		MATH_ASSERT(idx < 16, "Index out of range");
		return data[idx];
	}

	template <Numeric T>
	constexpr auto Mat4<T>::operator[](usize idx) const noexcept -> T
	{
		MATH_ASSERT(idx < 16, "Index out of range");
		return data[idx];
	}

	template <Numeric T>
	constexpr auto Mat4<T>::operator()(usize row, usize column) noexcept -> T&
	{
		MATH_ASSERT(row    < 4, "Row index out of range");
		MATH_ASSERT(column < 4, "Column index out of range");
		return rows[row][column];
	}

	template <Numeric T>
	constexpr auto Mat4<T>::operator()(usize row, usize column) const noexcept -> T
	{
		MATH_ASSERT(row    < 4, "Row index out of range");
		MATH_ASSERT(column < 4, "Column index out of range");
		return rows[column][row];
	}

	template <Numeric T>
	constexpr auto Mat4<T>::operator==(const Mat4& other) const noexcept -> bool
	{
		return row0 == other.row0 && row1 == other.row1 && row2 == other.row2 && row3 == other.row3;
	}

	template <Numeric T>
	constexpr auto Mat4<T>::operator!=(const Mat4& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Compare(const Mat4& other, T e) const noexcept -> bool
	{
		return row0.Compare(other.row0) && row1.Compare(other.row1) && row2.Compare(other.row2) && row3.Compare(other.row3);
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Row(usize idx) noexcept -> Vec4<T>&
	{
		MATH_ASSERT(idx < 4, "Row index out of range");
		return rows[idx];
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Row(usize idx) const noexcept -> Vec4<T>
	{
		MATH_ASSERT(idx < 4, "Row index out of range");
		return rows[idx];
	}

	template <Numeric T>
	constexpr auto Mat4<T>::SetRow(usize idx, const Vec4<T>& row) noexcept -> void
	{
		MATH_ASSERT(idx < 4, "Row index out of range");
		rows[0] = row;
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Column(usize idx) const noexcept -> Vec4<T>
	{
		MATH_ASSERT(idx < 4, "Column index out of range");
		return { row0[idx], row1[idx], row2[idx], row3[idx] };
	}

	template <Numeric T>
	constexpr auto Mat4<T>::SetColumn(usize idx, const Vec4<T>& column) noexcept -> void
	{
		MATH_ASSERT(idx < 4, "Column index out of range");
		row0[idx] = column.x;
		row1[idx] = column.y;
		row2[idx] = column.z;
		row3[idx] = column.w;
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Diagonal() const noexcept -> Vec4<T>
	{
		return { m00, m11, m22, m33 };
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Minor(usize row, usize column) const noexcept -> Mat3<T>
	{
		switch (row)
		{
		case 0:
		{
			switch (column)
			{
			case 0: return { m11, m12, m13, m21, m22, m23, m31, m32, m33 };
			case 1: return { m10, m12, m13, m20, m22, m23, m30, m32, m33 };
			case 2:	return { m10, m11, m13, m20, m21, m23, m30, m31, m33 };
			case 3:	return { m10, m11, m12, m20, m21, m22, m30, m31, m32 };
			default: ASSERT(false, "Invalid Mat3::Minor case"); break;
			}
		}
		case 1:
		{
			switch (column)
			{
			case 0:	return { m01, m02, m03, m21, m22, m23, m31, m32, m33 };
			case 1:	return { m00, m02, m03, m20, m22, m23, m30, m32, m33 };
			case 2:	return { m00, m01, m03, m20, m21, m23, m30, m31, m33 };
			case 3:	return { m00, m01, m02, m20, m21, m22, m30, m31, m32 };
			default: ASSERT(false, "Invalid Mat3::Minor case"); break;
			}
		}
		case 2:
		{
			switch (column)
			{
			case 0:	return { m01, m02, m03, m11, m12, m13, m31, m32, m33 };
			case 1:	return { m00, m02, m03, m10, m12, m13, m30, m32, m33 };
			case 2:	return { m00, m01, m03, m10, m11, m13, m30, m31, m33 };
			case 3:	return { m00, m01, m02, m10, m11, m12, m30, m31, m32 };
			default: ASSERT(false, "Invalid Mat3::Minor case"); break;
			}
		}
		case 3:
		{
			switch (column)
			{
			case 0:	return { m01, m02, m03, m11, m12, m13, m21, m22, m23 };
			case 1:	return { m00, m02, m03, m10, m12, m13, m20, m22, m23 };
			case 2:	return { m00, m01, m03, m10, m11, m13, m20, m21, m23 };
			case 3:	return { m00, m01, m02, m10, m11, m12, m20, m21, m22 };
			default: ASSERT(false, "Invalid Mat3::Minor case"); break;
			}
		}
		default: ASSERT(false, "Invalid Mat3::Minor case"); break;
		}
		return {};
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Transpose() noexcept -> Mat4&
	{
		Algo::Swap(m01, m10);
		Algo::Swap(m02, m20);
		Algo::Swap(m03, m30);
		Algo::Swap(m12, m21);
		Algo::Swap(m13, m31);
		Algo::Swap(m23, m32);
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Transposed() const noexcept -> Mat4
	{
		return
		{
			m00, m10, m20, m30,
			m01, m11, m21, m31,
			m02, m12, m22, m32,
			m03, m13, m23, m33
		};
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Determinant() const noexcept -> T
	{
		T minor00Det = m11 * (m22 * m33 - m23 * m32) - m12 * (m21 * m33 - m23 * m31) + m13 * (m21 * m32 - m22 * m31);
		T minor01Det = m10 * (m22 * m33 - m23 * m32) - m12 * (m20 * m33 - m23 * m30) + m13 * (m20 * m32 - m22 * m30);
		T minor02Det = m10 * (m21 * m33 - m23 * m31) - m11 * (m20 * m33 - m23 * m30) + m13 * (m20 * m31 - m21 * m30);
		T minor03Det = m10 * (m21 * m32 - m22 * m31) - m11 * (m20 * m32 - m22 * m30) + m12 * (m20 * m31 - m21 * m30);
		return m00 * minor00Det - m01 * minor01Det + m02 * minor02Det - m03 * minor03Det;
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Trace() const noexcept -> T
	{
		return m00 + m11 + m22 + m33;
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Adjugate() noexcept -> Mat4&
	{
		T tmp00 = m11 * (m22 * m33 - m23 * m32) - m12 * (m21 * m33 - m23 * m31) + m13 * (m21 * m32 - m22 * m31);
		T tmp01 = m10 * (m22 * m33 - m23 * m32) - m12 * (m20 * m33 - m23 * m30) + m13 * (m20 * m32 - m22 * m30);
		T tmp02 = m10 * (m21 * m33 - m23 * m31) - m11 * (m20 * m33 - m23 * m30) + m13 * (m20 * m31 - m21 * m30);
		T tmp03 = m10 * (m21 * m32 - m22 * m31) - m11 * (m20 * m32 - m22 * m30) + m12 * (m20 * m31 - m21 * m30);

		T tmp10 = m01 * (m22 * m33 - m23 * m32) - m02 * (m21 * m33 - m23 * m31) + m03 * (m21 * m32 - m22 * m31);
		T tmp11 = m00 * (m22 * m33 - m23 * m32) - m02 * (m20 * m33 - m23 * m30) + m03 * (m20 * m32 - m22 * m30);
		T tmp12 = m00 * (m21 * m33 - m23 * m31) - m01 * (m20 * m33 - m23 * m30) + m03 * (m20 * m31 - m21 * m30);
		T tmp13 = m00 * (m21 * m32 - m22 * m31) - m01 * (m20 * m32 - m22 * m30) + m02 * (m20 * m31 - m21 * m30);

		T tmp20 = m01 * (m12 * m33 - m13 * m32) - m02 * (m11 * m33 - m13 * m31) + m03 * (m11 * m32 - m12 * m31);
		T tmp21 = m00 * (m12 * m33 - m13 * m32) - m02 * (m10 * m33 - m13 * m30) + m03 * (m10 * m32 - m12 * m30);
		T tmp22 = m00 * (m11 * m33 - m13 * m31) - m01 * (m10 * m33 - m13 * m30) + m03 * (m10 * m31 - m11 * m30);
		T tmp23 = m00 * (m11 * m32 - m12 * m31) - m01 * (m10 * m32 - m12 * m30) + m02 * (m10 * m31 - m11 * m30);

		T tmp30 = m01 * (m12 * m23 - m13 * m22) - m02 * (m11 * m23 - m13 * m21) + m03 * (m11 * m22 - m12 * m21);
		T tmp31 = m00 * (m12 * m23 - m13 * m22) - m02 * (m10 * m23 - m13 * m20) + m03 * (m10 * m22 - m12 * m20);
		T tmp32 = m00 * (m11 * m23 - m13 * m21) - m01 * (m10 * m23 - m13 * m20) + m03 * (m10 * m21 - m11 * m20);
		T tmp33 = m00 * (m11 * m22 - m12 * m21) - m01 * (m10 * m22 - m12 * m20) + m02 * (m10 * m21 - m11 * m20);

		m00 = tmp00; m01 = -tmp10; m02 = tmp20; m03 = -tmp30;
		m10 = -tmp01; m11 = tmp11; m12 = -tmp21; m13 = tmp31;
		m20 = tmp02; m21 = -tmp12; m22 = tmp22; m23 = -tmp32;
		m30 = -tmp03; m31 = tmp13; m32 = -tmp23; m33 = tmp33;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Adjoint() const noexcept -> Mat4
	{
		return
		{
			 (m11 * (m22 * m33 - m23 * m32) - m12 * (m21 * m33 - m23 * m31) + m13 * (m21 * m32 - m22 * m31)),
			-(m01 * (m22 * m33 - m23 * m32) - m02 * (m21 * m33 - m23 * m31) + m03 * (m21 * m32 - m22 * m31)),
			 (m01 * (m12 * m33 - m13 * m32) - m02 * (m11 * m33 - m13 * m31) + m03 * (m11 * m32 - m12 * m31)),
			-(m01 * (m12 * m23 - m13 * m22) - m02 * (m11 * m23 - m13 * m21) + m03 * (m11 * m22 - m12 * m21)),

			-(m10 * (m22 * m33 - m23 * m32) - m12 * (m20 * m33 - m23 * m30) + m13 * (m20 * m32 - m22 * m30)),
			 (m00 * (m22 * m33 - m23 * m32) - m02 * (m20 * m33 - m23 * m30) + m03 * (m20 * m32 - m22 * m30)),
			-(m00 * (m12 * m33 - m13 * m32) - m02 * (m10 * m33 - m13 * m30) + m03 * (m10 * m32 - m12 * m30)),
			 (m00 * (m12 * m23 - m13 * m22) - m02 * (m10 * m23 - m13 * m20) + m03 * (m10 * m22 - m12 * m20)),

			 (m10 * (m21 * m33 - m23 * m31) - m11 * (m20 * m33 - m23 * m30) + m13 * (m20 * m31 - m21 * m30)),
			-(m00 * (m21 * m33 - m23 * m31) - m01 * (m20 * m33 - m23 * m30) + m03 * (m20 * m31 - m21 * m30)),
			 (m00 * (m11 * m33 - m13 * m31) - m01 * (m10 * m33 - m13 * m30) + m03 * (m10 * m31 - m11 * m30)),
			-(m00 * (m11 * m23 - m13 * m21) - m01 * (m10 * m23 - m13 * m20) + m03 * (m10 * m21 - m11 * m20)),

			-(m10 * (m21 * m32 - m22 * m31) - m11 * (m20 * m32 - m22 * m30) + m12 * (m20 * m31 - m21 * m30)),
			 (m00 * (m21 * m32 - m22 * m31) - m01 * (m20 * m32 - m22 * m30) + m02 * (m20 * m31 - m21 * m30)),
			-(m00 * (m11 * m32 - m12 * m31) - m01 * (m10 * m32 - m12 * m30) + m02 * (m10 * m31 - m11 * m30)),
			 (m00 * (m11 * m22 - m12 * m21) - m01 * (m10 * m22 - m12 * m20) + m02 * (m10 * m21 - m11 * m20))

		};
	}

	template <Numeric T>
	constexpr auto Mat4<T>::ToCofactor() noexcept -> Mat4&
	{
		T tmp00 = m11 * (m22 * m33 - m23 * m32) - m12 * (m21 * m33 - m23 * m31) + m13 * (m21 * m32 - m22 * m31);
		T tmp01 = m10 * (m22 * m33 - m23 * m32) - m12 * (m20 * m33 - m23 * m30) + m13 * (m20 * m32 - m22 * m30);
		T tmp02 = m10 * (m21 * m33 - m23 * m31) - m11 * (m20 * m33 - m23 * m30) + m13 * (m20 * m31 - m21 * m30);
		T tmp03 = m10 * (m21 * m32 - m22 * m31) - m11 * (m20 * m32 - m22 * m30) + m12 * (m20 * m31 - m21 * m30);

		T tmp10 = m01 * (m22 * m33 - m23 * m32) - m02 * (m21 * m33 - m23 * m31) + m03 * (m21 * m32 - m22 * m31);
		T tmp11 = m00 * (m22 * m33 - m23 * m32) - m02 * (m20 * m33 - m23 * m30) + m03 * (m20 * m32 - m22 * m30);
		T tmp12 = m00 * (m21 * m33 - m23 * m31) - m01 * (m20 * m33 - m23 * m30) + m03 * (m20 * m31 - m21 * m30);
		T tmp13 = m00 * (m21 * m32 - m22 * m31) - m01 * (m20 * m32 - m22 * m30) + m02 * (m20 * m31 - m21 * m30);

		T tmp20 = m01 * (m12 * m33 - m13 * m32) - m02 * (m11 * m33 - m13 * m31) + m03 * (m11 * m32 - m12 * m31);
		T tmp21 = m00 * (m12 * m33 - m13 * m32) - m02 * (m10 * m33 - m13 * m30) + m03 * (m10 * m32 - m12 * m30);
		T tmp22 = m00 * (m11 * m33 - m13 * m31) - m01 * (m10 * m33 - m13 * m30) + m03 * (m10 * m31 - m11 * m30);
		T tmp23 = m00 * (m11 * m32 - m12 * m31) - m01 * (m10 * m32 - m12 * m30) + m02 * (m10 * m31 - m11 * m30);

		T tmp30 = m01 * (m12 * m23 - m13 * m22) - m02 * (m11 * m23 - m13 * m21) + m03 * (m11 * m22 - m12 * m21);
		T tmp31 = m00 * (m12 * m23 - m13 * m22) - m02 * (m10 * m23 - m13 * m20) + m03 * (m10 * m22 - m12 * m20);
		T tmp32 = m00 * (m11 * m23 - m13 * m21) - m01 * (m10 * m23 - m13 * m20) + m03 * (m10 * m21 - m11 * m20);
		T tmp33 = m00 * (m11 * m22 - m12 * m21) - m01 * (m10 * m22 - m12 * m20) + m02 * (m10 * m21 - m11 * m20);

		m00 = tmp00; m01 = -tmp01; m02 = tmp02; m03 = -tmp03;
		m10 = -tmp10; m11 = tmp11; m12 = -tmp12; m13 = tmp13;
		m20 = tmp20; m21 = -tmp21; m22 = tmp22; m23 = -tmp23;
		m30 = -tmp30; m31 = tmp31; m32 = -tmp32; m33 = tmp33;
		return *this;
		
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Cofactor() const noexcept -> Mat4
	{
		return
		{
			 (m11 * (m22 * m33 - m23 * m32) - m12 * (m21 * m33 - m23 * m31) + m13 * (m21 * m32 - m22 * m31)),
			-(m10 * (m22 * m33 - m23 * m32) - m12 * (m20 * m33 - m23 * m30) + m13 * (m20 * m32 - m22 * m30)),
			 (m10 * (m21 * m33 - m23 * m31) - m11 * (m20 * m33 - m23 * m30) + m13 * (m20 * m31 - m21 * m30)),
			-(m10 * (m21 * m32 - m22 * m31) - m11 * (m20 * m32 - m22 * m30) + m12 * (m20 * m31 - m21 * m30)),

			-(m01 * (m22 * m33 - m23 * m32) - m02 * (m21 * m33 - m23 * m31) + m03 * (m21 * m32 - m22 * m31)),
			 (m00 * (m22 * m33 - m23 * m32) - m02 * (m20 * m33 - m23 * m30) + m03 * (m20 * m32 - m22 * m30)),
			-(m00 * (m21 * m33 - m23 * m31) - m01 * (m20 * m33 - m23 * m30) + m03 * (m20 * m31 - m21 * m30)),
			 (m00 * (m21 * m32 - m22 * m31) - m01 * (m20 * m32 - m22 * m30) + m02 * (m20 * m31 - m21 * m30)),

			 (m01 * (m12 * m33 - m13 * m32) - m02 * (m11 * m33 - m13 * m31) + m03 * (m11 * m32 - m12 * m31)),
			-(m00 * (m12 * m33 - m13 * m32) - m02 * (m10 * m33 - m13 * m30) + m03 * (m10 * m32 - m12 * m30)),
			 (m00 * (m11 * m33 - m13 * m31) - m01 * (m10 * m33 - m13 * m30) + m03 * (m10 * m31 - m11 * m30)),
			-(m00 * (m11 * m32 - m12 * m31) - m01 * (m10 * m32 - m12 * m30) + m02 * (m10 * m31 - m11 * m30)),

			-(m01 * (m12 * m23 - m13 * m22) - m02 * (m11 * m23 - m13 * m21) + m03 * (m11 * m22 - m12 * m21)),
			 (m00 * (m12 * m23 - m13 * m22) - m02 * (m10 * m23 - m13 * m20) + m03 * (m10 * m22 - m12 * m20)),
			-(m00 * (m11 * m23 - m13 * m21) - m01 * (m10 * m23 - m13 * m20) + m03 * (m10 * m21 - m11 * m20)),
			 (m00 * (m11 * m22 - m12 * m21) - m01 * (m10 * m22 - m12 * m20) + m02 * (m10 * m21 - m11 * m20))

		};
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Invert() noexcept -> Mat4&
	{
		T det = Determinant();
		if (EpsilonCompare(det, 0))
		{
			*this = Mat4{};
			return *this;
		}

		Adjugate();
		*this *= Rcp(det);
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Inverse() const noexcept -> Mat4
	{
		T det = Determinant();
		if (EpsilonCompare(det, 0))
			return Mat4{ 0 };
		return Adjoint() * Rcp(det);
	}

	template <Numeric T>
	constexpr auto Mat4<T>::TransformVector(const Vec4<T>& vec) const noexcept -> Vec4<T>
	{
		return vec.x * row0 + vec.y * row1 + vec.z * row2 + vec.w * row3;
	}

	template <Numeric T>
	constexpr auto Mat4<T>::TransformVector(const Vec3<T>& vec) const noexcept -> Vec3<T>
	{
		return
		{
			m00 * vec.x + m01 * vec.y + m02 * vec.z,
			m10 * vec.x + m11 * vec.y + m12 * vec.z,
			m20 * vec.x + m21 * vec.y + m22 * vec.z
		};
	}

	template <Numeric T>
	constexpr auto Mat4<T>::TransformVector(const Vec2<T>& vec) const noexcept -> Vec2<T>
	{
		return
		{
			m00 * vec.x + m01 * vec.y,
			m10 * vec.x + m11 * vec.y,
		};
	}

	template <Numeric T>
	constexpr auto Mat4<T>::TransformPoint(const Vec3<T>& vec) const noexcept -> Vec3<T>
	{
		return
		{
			m00 * vec.x + m01 * vec.y + m02 * vec.z + m03,
			m10 * vec.x + m11 * vec.y + m12 * vec.z + m13,
			m20 * vec.x + m21 * vec.y + m22 * vec.z + m23
		};
	}

	template <Numeric T>
	constexpr auto Mat4<T>::TransformPoint(const Vec2<T>& vec) const noexcept -> Vec2<T>
	{
		return
		{
			m00 * vec.x + m01 * vec.y + m02,
			m10 * vec.x + m11 * vec.y + m12,
		};
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Decompose() const noexcept -> Tuple<Vec3<T>, Quaternion<T>, Vec3<T>>
	{
		Vec3<T> scale;
		scale.x = Column(0).Len();
		scale.y = Column(1).Len();
		scale.z = Column(2).Len();
		
		Mat3 qMat;
		qMat.m00 = m00 / T(scale);
		qMat.m01 = m01 / T(scale);
		qMat.m02 = m02 / T(scale);
		qMat.m10 = m10 / T(scale);
		qMat.m11 = m11 / T(scale);
		qMat.m12 = m12 / T(scale);
		qMat.m20 = m20 / T(scale);
		qMat.m21 = m21 / T(scale);
		qMat.m22 = m22 / T(scale);
		Quaternion<T> quat{ qMat };

		Vec3<T> trans{ m30, m31, m32 };

		return { scale, quat, trans };
	}

	template <Numeric T>
	constexpr auto Mat4<T>::Decompose2D() const noexcept -> Tuple<Vec2<T>, Angle<T, AngleKind::Radians>, Vec2<T>>
	{
		MATH_ASSERT((row2 == Vec3<T>{ 0, 0, 1 }), "Matrix is not a 2D tansformation");

		Vec2<T> scale;
		scale.x = Column(0).Len();
		scale.y = Column(1).Len();

		Angle<T, AngleKind::Radians> angle = ACos(m00 / scale.x);
		Vec2<T> trans{ m30, m31 };

		return { scale, angle, trans };
	}

	template <Numeric T>
	constexpr auto Mat4<T>::GetScaleRotationMatrix() const noexcept -> Mat3<T>
	{
		return
		{
			m00, m01, m02,
			m10, m11, m12,
			m20, m21, m22
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat4<T>::CreateScale2D(A scale) noexcept -> Mat4
	{
		return
		{
			T(scale), T(0)    , T(0), T(0),
			T(0)    , T(scale), T(0), T(0),
			T(0)    , T(0)    , T(1), T(0),
			T(0)    , T(0)    , T(0), T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat4<T>::CreateScale2D(const Vec2<A>& scale) noexcept -> Mat4
	{
		return
		{
			T(scale.x), T(0)      , T(0), T(0),
			T(0)      , T(scale.y), T(0), T(0),
			T(0)      , T(0)      , T(1), T(0),
			T(0)      , T(0)      , T(0), T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat4<T>::CreateShear2D(const Vec2<A>& shear) noexcept -> Mat4
	{
		return
		{
			T(1)      , T(shear.y), T(0), T(0),
			T(shear.x), T(1)      , T(0), T(0),
			T(0)      , T(0)      , T(1), T(0),
			T(0)      , T(0)      , T(0), T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, AngleKind Kind>
	constexpr auto Mat4<T>::CreateRotation2D(const Angle<A, Kind>& angle) noexcept -> Mat4
	{
		T cs = T(Cos(angle));
		T sn = T(Sin(angle));
		return
		{
			cs  , -sn , T(0), T(0),
			sn  ,  cs , T(0), T(0),
			T(0), T(0), T(1), T(0),
			T(0), T(0), T(0), T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat4<T>::CreateTranslation2D(const Vec2<A>& trans) noexcept -> Mat4
	{
		return
		{
			T(1)      , T(0)      , T(0), T(0),
			T(0)      , T(1)      , T(0), T(0),
			T(0)      , T(0)      , T(1), T(0),
			T(trans.x), T(trans.y), T(0), T(0),
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, AngleKind Kind, ConvertableTo<T> C>
	constexpr auto Mat4<T>::CreateTransform2D(A scale, const Angle<B, Kind>& angle, const Vec2<T>& trans) noexcept -> Mat4
	{
		T cs = T(Cos(angle));
		T sn = T(Sin(angle));
		return
		{
			T(scale) * cs, T(scale) * -sn, T(0), T(0),
			T(scale) * sn, T(scale) *  cs, T(0), T(0),
			T(0)         , T(0)          , T(1), T(0),
			T(trans.x)   , T(trans.y)    , T(0), T(0)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, AngleKind Kind, ConvertableTo<T> C>
	constexpr auto Mat4<T>::CreateTransform2D(const Vec2<A>& scale, const Angle<B, Kind>& angle, const Vec2<C>& trans) noexcept -> Mat4
	{
		T cs = T(Cos(angle));
		T sn = T(Sin(angle));
		return
		{
			T(scale.x) * cs, T(scale.y) * -sn, T(0), T(0),
			T(scale.x) * sn, T(scale.y) *  cs, T(0), T(0),
			T(0)           , T(0)            , T(1), T(0),
			T(trans.x)     , T(trans.y)      , T(0), T(0)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat4<T>::CreateScale(A scale) noexcept -> Mat4
	{
		return
		{
			T(scale), T(0)    , T(0)    , T(0),
			T(0)    , T(scale), T(0)    , T(0),
			T(0)    , T(0)    , T(scale), T(0),
			T(0)    , T(0)    , T(0)    , T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat4<T>::CreateScale(const Vec3<A>& scale) noexcept -> Mat4
	{
		return
		{
			T(scale.x), T(0)      , T(0)      , T(0),
			T(0)      , T(scale.y), T(0)      , T(0),
			T(0)      , T(0)      , T(scale.z), T(0),
			T(0)      , T(0)      , T(0)      , T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat4<T>::CreateRotation(const Quaternion<A>& quat) noexcept -> Mat4
	{
		MATH_ASSERT(quat.IsNormalized(), "Quaternion needs to be normalized");

		T xx = quat.x * quat.x;
		T yy = quat.y * quat.y;
		T zz = quat.z * quat.z;

		T xw = quat.x * quat.w;
		T yw = quat.y * quat.w;
		T zw = quat.z * quat.w;

		T xy = quat.x * quat.y;
		T xz = quat.x * quat.z;
		T yz = quat.y * quat.z;

		return
		{
			T(1) - T(2) * (yy + zz),        T(2) * (xy - zw),        T(2) * (xz + yw), T(0),
				   T(2) * (xy + zw), T(1) - T(2) * (xx + zz),        T(2) * (yz - zw), T(0),
				   T(2) * (xz - yw),        T(2) * (yz + xw), T(1) - T(2) * (xx + yy), T(0),
			T(0)                   , T(0)                   , T(0)                   , T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat4<T>::CreateTranslation(const Vec3<A>& trans) noexcept -> Mat4
	{
		return
		{
			T(1)      , T(0)      , T(0)      , T(0),
			T(0)      , T(1)      , T(0)      , T(0),
			T(0)      , T(0)      , T(1)      , T(0),
			T(trans.x), T(trans.y), T(trans.z), T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C>
	constexpr auto Mat4<T>::CreateTransform(A scale, const Quaternion<B>& quat, const Vec3<C>& trans) noexcept -> Mat4
	{
		MATH_ASSERT(quat.IsNormalized(), "Quaternion needs to be normalized");

		T xx = quat.x * quat.x;
		T yy = quat.y * quat.y;
		T zz = quat.z * quat.z;

		T xw = quat.x * quat.w;
		T yw = quat.y * quat.w;
		T zw = quat.z * quat.w;

		T xy = quat.x * quat.y;
		T xz = quat.x * quat.z;
		T yz = quat.y * quat.z;

		return
		{
			T(scale) * (T(1) - T(2) * (yy + zz)), T(scale) * (       T(2) * (xy - zw)), T(scale) * (       T(2) * (xz + yw)), T(0),
			T(scale) * (       T(2) * (xy + zw)), T(scale) * (T(1) - T(2) * (xx + zz)), T(scale) * (       T(2) * (yz - zw)), T(0),
			T(scale) * (       T(2) * (xz - yw)), T(scale) * (       T(2) * (yz + xw)), T(scale) * (T(1) - T(2) * (xx + yy)), T(0),
			T(trans.x)                          , T(trans.y)                          , T(trans.z)                          , T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C>
	constexpr auto Mat4<T>::CreateTransform(const Vec3<A>& scale, const Quaternion<B>& quat, const Vec3<C>& trans) noexcept -> Mat4
	{
		MATH_ASSERT(quat.IsNormalized(), "Quaternion needs to be normalized");

		T xx = quat.x * quat.x;
		T yy = quat.y * quat.y;
		T zz = quat.z * quat.z;

		T xw = quat.x * quat.w;
		T yw = quat.y * quat.w;
		T zw = quat.z * quat.w;

		T xy = quat.x * quat.y;
		T xz = quat.x * quat.z;
		T yz = quat.y * quat.z;

		return
		{
			T(scale.x) * (T(1) - T(2) * (yy + zz)), T(scale.y) * (       T(2) * (xy - zw)), T(scale.z) * (       T(2) * (xz + yw)), T(0),
			T(scale.x) * (       T(2) * (xy + zw)), T(scale.y) * (T(1) - T(2) * (xx + zz)), T(scale.z) * (       T(2) * (yz - zw)), T(0),
			T(scale.x) * (       T(2) * (xz - yw)), T(scale.y) * (       T(2) * (yz + xw)), T(scale.z) * (T(1) - T(2) * (xx + yy)), T(0),
			T(trans.x)                            , T(trans.y)                            , T(trans.z)                            , T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C>
	constexpr auto Mat4<T>::CreateLookAt(const Vec3<A>& eyePos, const Vec3<B>& focusPos, const Vec3<T>& up) noexcept -> Mat4
	{
		Vec3<T> zAxis = (focusPos - eyePos).Normalized();
		Vec3<T> xAxis = up.Cross(zAxis).Normalized();
		Vec3<T> yAxis = zAxis.Cross(xAxis);

		return
		{
			 xAxis.x          ,  xAxis.y          ,  xAxis.z             , T(0),
			 yAxis.x          ,  yAxis.y          ,  yAxis.z             , T(0),
			 zAxis.x          ,  zAxis.y          ,  zAxis.z             , T(0),
			-xAxis.Dot(eyePos), -yAxis.Dot(eyePos), -zAxis.Dot(eyePos)   , T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C>
	constexpr auto Mat4<T>::CreateLookAtRH(const Vec3<A>& eyePos, const Vec3<B>& focusPos, const Vec3<T>& up) noexcept -> Mat4
	{
		Vec3<T> zAxis = (eyePos - focusPos).Normalized();
		Vec3<T> xAxis = up.Cross(zAxis).Normalized();
		Vec3<T> yAxis = zAxis.Cross(xAxis);

		return
		{
			 xAxis.x          ,  xAxis.y          ,  xAxis.z             , T(0),
			 yAxis.x          ,  yAxis.y          ,  yAxis.z             , T(0),
			 zAxis.x          ,  zAxis.y          ,  zAxis.z             , T(0),
			-xAxis.Dot(eyePos), -yAxis.Dot(eyePos), -zAxis.Dot(eyePos)   , T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C>
	constexpr auto Mat4<T>::CreateLookTo(const Vec3<A>& eyePos, const Vec3<B>& lookDir, const Vec3<T>& up) noexcept -> Mat4
	{
		Vec3<T> zAxis = lookDir.Normalized();
		Vec3<T> xAxis = up.Cross(zAxis).Normalized();
		Vec3<T> yAxis = zAxis.Cross(xAxis);

		return
		{
			 xAxis.x          ,  xAxis.y          ,  xAxis.z             , T(0),
			 yAxis.x          ,  yAxis.y          ,  yAxis.z             , T(0),
			 zAxis.x          ,  zAxis.y          ,  zAxis.z             , T(0),
			-xAxis.Dot(eyePos), -yAxis.Dot(eyePos), -zAxis.Dot(eyePos)   , T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C>
	constexpr auto Mat4<T>::CreateLookToRH(const Vec3<A>& eyePos, const Vec3<B>& lookDir, const Vec3<T>& up) noexcept -> Mat4
	{
		Vec3<T> zAxis = lookDir.Normalized();
		Vec3<T> xAxis = zAxis.Cross(up).Normalized();
		Vec3<T> yAxis = zAxis.Cross(xAxis);

		return
		{
			 xAxis.x          ,  xAxis.y          ,  xAxis.z             , T(0),
			 yAxis.x          ,  yAxis.y          ,  yAxis.z             , T(0),
			 zAxis.x          ,  zAxis.y          ,  zAxis.z             , T(0),
			-xAxis.Dot(eyePos), -yAxis.Dot(eyePos), -zAxis.Dot(eyePos)   , T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D>
	constexpr auto Mat4<T>::CreateOrtho(A width, B height, C zNear, D zFar) noexcept -> Mat4
	{
		return
		{
			T(2) / T(width), T(0)            ,  T(0)                           , T(0),
			T(0)           , T(2) / T(height),  T(0)                           , T(0),
			T(0)           , T(0)            ,  T(1)     / (T(zFar) - T(zNear)), T(0),
			T(0)           , T(0)            , -T(zNear) / (T(zFar) - T(zNear)), T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D>
	constexpr auto Mat4<T>::CreateOrthoRH(A width, B height, C zNear, D zFar) noexcept -> Mat4
	{
		return
		{
			T(2) / T(width), T(0)            , T(0)                           , T(0),
			T(0)           , T(2) / T(height), T(0)                           , T(0),
			T(0)           , T(0)            , T(1)     / (T(zFar) - T(zNear)), T(zNear) / (T(zFar) - T(zNear)),
			T(0)           , T(0)            , T(zNear) / (T(zFar) - T(zNear)), T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D, ConvertableTo<T> E, ConvertableTo<T> F>
	constexpr auto Mat4<T>::CreateOrtho(A left, B right, C top, D bottom, E zNear, F zFar) noexcept -> Mat4
	{
		return
		{
			T(2) / (T(right) - T(left))                , T(0)                                       , T(0)                            , T(0),
			T(0)                                       , T(2) / (T(top) - T(bottom))                , T(0)                            , T(0),
			T(0)                                       , T(0)                                       , T(1)      / (T(zFar) - T(zNear)), T(0),
			(T(left) + T(right)) / (T(left) - T(right)), (T(bottom) + T(top)) / (T(bottom) - T(top)), -T(zNear) / (T(zNear) - T(zFar)), T(1)
		};
	}  

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D, ConvertableTo<T> E, ConvertableTo<T> F>
	constexpr auto Mat4<T>::CreateOrthoRH(A left, B right, C top, D bottom, E zNear, F zFar) noexcept -> Mat4
	{
		return
		{
			T(2) / (T(right) - T(left))                , T(0)                                       , T(0)                            , T(0),
			T(0)                                       , T(2) / (T(top) - T(bottom))                , T(0)                            , T(0),
			T(0)                                       , T(0)                                       , T(1) / (T(zNear) - T(zFar)), T(0),
			(T(left) + T(right)) / (T(left) - T(right)), (T(bottom) + T(top)) / (T(bottom) - T(top)), -T(zNear) / (T(zNear) - T(zFar)), T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D>
	constexpr auto Mat4<T>::CreatePerspective(A width, B height, C zNear, D zFar) noexcept -> Mat4
	{
		return
		{
			T(2) * T(zNear) / T(width), T(0)                       , T(0)                                     , T(0),
			T(0)                      , T(2) * T(zNear) / T(height), T(0)                                     , T(0),
			T(0)                      , T(0)                       , T(zFar)            / (T(zFar) - T(zNear)), T(1),
			T(0)                      , T(0)                       , T(zFar) * T(zNear) / (T(zNear) - T(zFar)), T(0)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D>
	constexpr auto Mat4<T>::CreatePerspectiveRH(A width, B height, C zNear, D zFar) noexcept -> Mat4
	{
		return
		{
			T(2) * T(zNear) / T(width), T(0)                       , T(0)                                     , T(0),
			T(0)                      , T(2) * T(zNear) / T(height), T(0)                                     , T(0),
			T(0)                      , T(0)                       , T(zFar)            / (T(zNear) - T(zFar)), T(1),
			T(0)                      , T(0)                       , T(zFar) * T(zNear) / (T(zNear) - T(zFar)), T(0)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D, ConvertableTo<T> E, ConvertableTo<T> F>
	constexpr auto Mat4<T>::CreatePerspective(A left, B right, C top, D bottom, E zNear, F zFar) noexcept -> Mat4
	{
		return
		{
			T(2) * T(zNear) / (T(right) - T(left))     , T(0)                                       , T(0)                                     , T(0),
			T(0)                                       , T(2) * T(zNear) / (T(top) - T(bottom))     , T(0)                                     , T(0),
			(T(left) + T(right)) / (T(left) - T(right)), (T(top) + T(bottom)) / (T(bottom) - T(top)), T(zFar) / (T(zNear) - T(zFar))           , T(1),
			T(0)                                       , T(0)                                       , T(zFar) * T(zNear) / (T(zNear) - T(zFar)), T(0)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D, ConvertableTo<T> E, ConvertableTo<T> F>
	constexpr auto Mat4<T>::CreatePerspectiveRH(A left, B right, C top, D bottom, E zNear, F zFar) noexcept -> Mat4
	{
		return
		{
			T(2) * T(zNear) / (T(right) - T(left))     , T(0)                                       , T(0)                                     ,  T(0),
			T(0)                                       , T(2) * T(zNear) / (T(top) - T(bottom))     , T(0)                                     ,  T(0),
			(T(left) + T(right)) / (T(right) - T(left)), (T(top) + T(bottom)) / (T(top) - T(bottom)), T(zFar)            / (T(zNear) - T(zFar)), -T(1),
			T(0)                                       , T(0)                                       , T(zFar) * T(zNear) / (T(zNear) - T(zFar)),  T(0)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, AngleKind AK, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D>
	constexpr auto Mat4<T>::CreatePerspectiveFov(const Angle<A, AK>& fov, B aspect, C zNear, D zFar) noexcept -> Mat4
	{
		T xScale = Rcp(T(Tan(fov / 2)));
		T yScale = xScale / T(aspect);
		return
		{
			xScale, T(0)  ,  T(0)                                     , T(0),
			T(0)  , yScale,  T(0)                                     , T(0),
			T(0)  , T(0)  ,  T(zFar)            / (T(zFar) - T(zNear)), T(1),
			T(0)  , T(0)  , -T(zNear) * T(zFar) / (T(zFar) - T(zNear)), T(0)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, AngleKind AK, ConvertableTo<T> B, ConvertableTo<T> C, ConvertableTo<T> D>
	constexpr auto Mat4<T>::CreatePerspectiveFovRH(const Angle<A, AK>& fov, B aspect, C zNear, D zFar) noexcept -> Mat4
	{
		T xScale = Rcp(T(Tan(fov / 2)));
		T yScale = xScale / T(aspect);
		return
		{
			xScale, T(0)  ,  T(0)                                     , T(0),
			T(0)  , yScale,  T(0)                                     , T(0),
			T(0)  , T(0)  ,  T(zFar)            / (T(zNear) - T(zFar)), T(1),
			T(0)  , T(0)  , -T(zNear) * T(zFar) / (T(zNear) - T(zFar)), T(0)
		};
	}

	template <Numeric T, ConvertableTo<T> U>
	constexpr auto operator*(const Vec4<T>& vec, const Mat4<U>& mat) noexcept -> Vec4<T>
	{
		return vec.x * mat.row0 + vec.y * mat.row1 + vec.z * mat.row2 + vec.w * mat.row3;
	}

	template <Numeric T, ConvertableTo<T> U>
	constexpr auto operator*=(Vec4<T>& vec, const Mat4<U>& mat) noexcept -> Vec4<T>&
	{
		return vec.x * mat.row0 + vec.y * mat.row1 + vec.z * mat.row2 + vec.w * mat.row3;
	}
}
