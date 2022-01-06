#pragma once

#if __RESHARPER__
#include "Mat43.h"
#endif

#include "Constants.h"
#include "Mat3.h"
#include "core/Assert.h"
#include "core/utils/Algo.h"

namespace Core::Math
{
	template <Numeric T>
	constexpr Mat43<T>::Mat43() noexcept
		: row0(1, 0, 0)
		, row1(0, 1, 0)
		, row2(0, 0, 1)
		, row3(0, 0, 0)
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr Mat43<T>::Mat43(A diag) noexcept
		: row0(diag, 0   , 0   )
		, row1(0   , diag, 0   )
		, row2(0   , 0   , diag)
		, row3(0   , 0   , 0   )
	{
	}

	template <Numeric T>
	template<ConvertableTo<T> U0, ConvertableTo<T> U1, ConvertableTo<T> U2, ConvertableTo<T> U3,
		ConvertableTo<T> U4, ConvertableTo<T> U5, ConvertableTo<T> U6, ConvertableTo<T> U7,
		ConvertableTo<T> U8, ConvertableTo<T> U9, ConvertableTo<T> U10, ConvertableTo<T> U11>
	constexpr Mat43<T>::Mat43(U0 m00, U1 m01, U2 m02, U3 m10, U4 m11, U5 m12, U6 m20, U7 m21, U8 m22, U9 m30, U10 m31, U11 m32) noexcept
		: row0(m00, m01, m02)
		, row1(m10, m11, m12)
		, row2(m20, m21, m22)
		, row3(m30, m31, m32)
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> U0, ConvertableTo<T> U1, ConvertableTo<T> U2, ConvertableTo<T> U3>
	constexpr Mat43<T>::Mat43(const Vec3<U0>& row0, const Vec3<U1>& row1, const Vec3<U2>& row2, const Vec3<U3>& row3)
		: row0(row0)
		, row1(row1)
		, row2(row2)
		, row3(row3)
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> U0, ConvertableTo<T> U1, ConvertableTo<T> U2>
	constexpr Mat43<T>::Mat43(ColumnInitTag, const Vec4<U0>& column0, const Vec4<U1>& column1, const Vec4<U2>& column2)
		: row0(column0.x, column1.x, column2.x)
		, row1(column0.y, column1.y, column2.y)
		, row2(column0.z, column1.z, column2.z)
		, row3(column0.w, column1.w, column2.w)
	{
	}

	template <Numeric T>
	constexpr Mat43<T>::Mat43(const Mat43& other) noexcept
		: row0(other.row0)
		, row1(other.row1)
		, row2(other.row2)
		, row3(other.row3)
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr Mat43<T>::Mat43(const Mat43<A>& other) noexcept
		: row0(other.row0)
		, row1(other.row1)
		, row2(other.row2)
		, row3(other.row3)
	{
	}

	template <Numeric T>
	constexpr Mat43<T>::Mat43(Mat43&& other) noexcept
		: row0(other.row0)
		, row1(other.row1)
		, row2(other.row2)
		, row3(other.row3)
	{
	}

	template <Numeric T>
	constexpr auto Mat43<T>::operator=(const Mat43& other) noexcept -> Mat43&
	{
		row0 = other.row0;
		row1 = other.row1;
		row2 = other.row2;
		row3 = other.row3;
		return *this;
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat43<T>::operator=(const Mat43<A>& other) noexcept -> Mat43&
	{
		row0 = other.row0;
		row1 = other.row1;
		row2 = other.row2;
		row3 = other.row3;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat43<T>::operator=(Mat43&& other) noexcept -> Mat43&
	{
		row0 = other.row0;
		row1 = other.row1;
		row2 = other.row2;
		row3 = other.row3;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat43<T>::operator+() const noexcept -> Mat43
	{
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat43<T>::operator-() const noexcept -> Mat43
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
	constexpr auto Mat43<T>::operator+(const Mat43& other) const noexcept -> Mat43
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
	constexpr auto Mat43<T>::operator-(const Mat43& other) const noexcept -> Mat43
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
	constexpr auto Mat43<T>::operator*(const Mat43& other) const noexcept -> Mat43
	{
		Vec4<T> column0 = other.Column(0);
		Vec4<T> column1 = other.Column(1);
		Vec4<T> column2 = other.Column(2);

		return
		{
			Vec4<T>{ row0, 0 }.Dot(column0),
			Vec4<T>{ row0, 0 }.Dot(column1),
			Vec4<T>{ row0, 0 }.Dot(column2),

			Vec4<T>{ row1, 0 }.Dot(column0),
			Vec4<T>{ row1, 0 }.Dot(column1),
			Vec4<T>{ row1, 0 }.Dot(column2),

			Vec4<T>{ row2, 0 }.Dot(column0),
			Vec4<T>{ row2, 0 }.Dot(column1),
			Vec4<T>{ row2, 0 }.Dot(column2),

			Vec4<T>{ row3, 1 }.Dot(column0),
			Vec4<T>{ row3, 1 }.Dot(column1),
			Vec4<T>{ row3, 1 }.Dot(column2)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat43<T>::operator*(A val) const noexcept -> Mat43
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
	constexpr auto Mat43<T>::operator/(A val) const noexcept -> Mat43
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
	constexpr auto Mat43<T>::operator*(const Vec4<T>& vec) const noexcept -> Vec4<T>
	{
		return vec.x * Vec4<T>{ row0, 0 } + vec.x * Vec4<T>{ row1, 0 } + vec.x * Vec4<T>{ row2, 0 } + vec.x * Vec4<T>{ row3, 1 };
	}

	template <Numeric T>
	constexpr auto Mat43<T>::operator+=(const Mat43& other) noexcept -> Mat43&
	{
		row0 += other.row0;
		row1 += other.row1;
		row2 += other.row2;
		row3 += other.row3;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat43<T>::operator-=(const Mat43& other) noexcept -> Mat43&
	{
		row0 -= other.row0;
		row1 -= other.row1;
		row2 -= other.row2;
		row3 -= other.row3;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat43<T>::operator*=(const Mat43& other) noexcept -> Mat43&
	{
		Vec4<T> column0 = other.Column(0);
		Vec4<T> column1 = other.Column(1);
		Vec4<T> column2 = other.Column(2);

		T tmp00 = Vec4<T>{ row0, 0 }.Dot(column0);
		T tmp01 = Vec4<T>{ row0, 0 }.Dot(column1);
		T tmp02 = Vec4<T>{ row0, 0 }.Dot(column2);

		T tmp10 = Vec4<T>{ row1, 0 }.Dot(column0);
		T tmp11 = Vec4<T>{ row1, 0 }.Dot(column1);
		T tmp12 = Vec4<T>{ row1, 0 }.Dot(column2);

		T tmp20 = Vec4<T>{ row2, 0 }.Dot(column0);
		T tmp21 = Vec4<T>{ row2, 0 }.Dot(column1);
		T tmp22 = Vec4<T>{ row2, 0 }.Dot(column2);

		T tmp30 = Vec4<T>{ row3, 1 }.Dot(column0);
		T tmp31 = Vec4<T>{ row3, 1 }.Dot(column1);
		T tmp32 = Vec4<T>{ row3, 1 }.Dot(column2);

		m00 = tmp00; m01 = tmp01; m02 = tmp02;
		m10 = tmp10; m11 = tmp11; m12 = tmp12;
		m20 = tmp20; m21 = tmp21; m22 = tmp22;
		m30 = tmp30; m31 = tmp31; m32 = tmp32;
		return *this;
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat43<T>::operator*=(A val) noexcept -> Mat43&
	{
		row0 *= val;
		row1 *= val;
		row2 *= val;
		row3 *= val;
		return *this;
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat43<T>::operator/=(A val) noexcept -> Mat43&
	{
		row0 /= val;
		row1 /= val;
		row2 /= val;
		row3 /= val;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat43<T>::operator[](usize idx) noexcept -> T&
	{
		MATH_ASSERT(idx < 12, "Index out of range");
		return data[idx];
	}

	template <Numeric T>
	constexpr auto Mat43<T>::operator[](usize idx) const noexcept -> T
	{
		MATH_ASSERT(idx < 12, "Index out of range");
		return data[idx];
	}

	template <Numeric T>
	constexpr auto Mat43<T>::operator()(usize row, usize column) noexcept -> T&
	{
		MATH_ASSERT(row    < 4, "Row index out of range");
		MATH_ASSERT(column < 3, "Column index out of range");
		return rows[row][column];
	}

	template <Numeric T>
	constexpr auto Mat43<T>::operator()(usize row, usize column) const noexcept -> T
	{
		MATH_ASSERT(row    < 3, "Row index out of range");
		MATH_ASSERT(column < 4, "Column index out of range");
		return rows[column][row];
	}

	template <Numeric T>
	constexpr auto Mat43<T>::operator==(const Mat43& other) const noexcept -> bool
	{
		return row0 == other.row0 && row1 == other.row1 && row2 == other.row2 && row3 == other.row3;
	}

	template <Numeric T>
	constexpr auto Mat43<T>::operator!=(const Mat43& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <Numeric T>
	constexpr auto Mat43<T>::Compare(const Mat43& other, T e) const noexcept -> bool
	{
		return row0.Compare(other.row0) && row1.Compare(other.row1) && row2.Compare(other.row2) && row3.Compare(other.row3);
	}
	
	template <Numeric T>
	constexpr auto Mat43<T>::Row(usize idx) const noexcept -> Vec4<T>
	{
		MATH_ASSERT(idx < 4, "Row index out of range");
		return { rows[idx], T(0) };
	}

	template <Numeric T>
	constexpr auto Mat43<T>::SetRow(usize idx, const Vec3<T>& row) noexcept -> void
	{
		MATH_ASSERT(idx < 4, "Row index out of range");
		rows[0] = row;
	}

	template <Numeric T>
	constexpr auto Mat43<T>::Column(usize idx) const noexcept -> Vec4<T>
	{
		MATH_ASSERT(idx < 3, "Column index out of range");
		return { row0[idx], row1[idx], row2[idx], row3[idx] };
	}

	template <Numeric T>
	constexpr auto Mat43<T>::SetColumn(usize idx, const Vec4<T>& column) noexcept -> void
	{
		MATH_ASSERT(idx < 3, "Column index out of range");
		row0[idx] = column.x;
		row1[idx] = column.y;
		row2[idx] = column.z;
		row3[idx] = column.w;
	}

	template <Numeric T>
	constexpr auto Mat43<T>::Diagonal() const noexcept -> Vec4<T>
	{
		return { m00, m11, m22, T(1) };
	}

	template <Numeric T>
	constexpr auto Mat43<T>::Minor(usize row, usize column) const noexcept -> Mat3<T>
	{
		switch (row)
		{
		case 0:
		{
			switch (column)
			{
			case 0: return { m11, m12, T(0), m21, m22, T(0), m31, m32, T(1) };
			case 1: return { m10, m12, T(0), m20, m22, T(0), m30, m32, T(1) };
			case 2:	return { m10, m11, T(0), m20, m21, T(0), m30, m31, T(1) };
			case 3:	return { m10, m11, m12 , m20, m21, m22 , m30, m31, m32 };
			default: ASSERT(false, "Invalid Mat3::Minor case"); break;
			}
		}
		case 1:
		{
			switch (column)
			{
			case 0:	return { m01, m02, T(0), m21, m22, T(0), m31, m32, T(1) };
			case 1:	return { m00, m02, T(0), m20, m22, T(0), m30, m32, T(1) };
			case 2:	return { m00, m01, T(0), m20, m21, T(0), m30, m31, T(1) };
			case 3:	return { m00, m01, m02 , m20, m21, m22 , m30, m31, m32 };
			default: ASSERT(false, "Invalid Mat3::Minor case"); break;
			}
		}
		case 2:
		{
			switch (column)
			{
			case 0:	return { m01, m02, T(0), m11, m12, T(0), m31, m32, T(1) };
			case 1:	return { m00, m02, T(0), m10, m12, T(0), m30, m32, T(1) };
			case 2:	return { m00, m01, T(0), m10, m11, T(0), m30, m31, T(1) };
			case 3:	return { m00, m01, m02 , m10, m11, m12 , m30, m31, m32 };
			default: ASSERT(false, "Invalid Mat3::Minor case"); break;
			}
		}
		case 3:
		{
			switch (column)
			{
			case 0:	return { m01, m02, T(0), m11, m12, T(0), m21, m22, T(0) };
			case 1:	return { m00, m02, T(0), m10, m12, T(0), m20, m22, T(0) };
			case 2:	return { m00, m01, T(0), m10, m11, T(0), m20, m21, T(0) };
			case 3:	return { m00, m01, m02 , m10, m11, m12 , m20, m21, m22  };
			default: ASSERT(false, "Invalid Mat3::Minor case"); break;
			}
		}
		default: ASSERT(false, "Invalid Mat3::Minor case"); break;
		}
		return {};
	}

	template <Numeric T>
	constexpr auto Mat43<T>::Transpose() noexcept -> Mat43&
	{
		Algo::Swap(m01, m10);
		Algo::Swap(m02, m20);
		Algo::Swap(m12, m21);
		m30 = T(0);
		m31 = T(0);
		m32 = T(0);

		return *this;
	}

	template <Numeric T>
	constexpr auto Mat43<T>::Transposed() const noexcept -> Mat43
	{
		return
		{
			m00 , m10 , m20,
			m01 , m11 , m21,
			m02 , m12 , m22,
			T(0), T(0), T(0)
		};
	}

	template <Numeric T>
	constexpr auto Mat43<T>::Determinant() const noexcept -> T
	{
		T minor00Det = m11 * m22 - m12 * m21;
		T minor01Det = m10 * m22 - m12 * m20;
		T minor02Det = m10 * m21 - m11 * m20;
		return m00 * minor00Det - m01 * minor01Det + m02 * minor02Det;
	}

	template <Numeric T>
	constexpr auto Mat43<T>::Trace() const noexcept -> T
	{
		return m00 + m11 + m22 + T(1);
	}

	template <Numeric T>
	constexpr auto Mat43<T>::Adjugate() noexcept -> Mat43&
	{
		T tmp00 = m11 * m22 - m12 * m21 * 1;
		T tmp01 = m10 * m22 - m12 * m20 * 1;
		T tmp02 = m10 * m21 - m11 * m20 * 1;
		T tmp03 = m10 * (m21 * m32 - m22 * m31) - m11 * (m20 * m32 - m22 * m30) + m12 * (m20 * m31 - m21 * m30);

		T tmp10 = m01 * m22 - m02 * m21;
		T tmp11 = m00 * m22 - m02 * m20;
		T tmp12 = m00 * m21 - m01 * m20;
		T tmp13 = m00 * (m21 * m32 - m22 * m31) - m01 * (m20 * m32 - m22 * m30) + m02 * (m20 * m31 - m21 * m30);

		T tmp20 = m01 * m12 - m02 * m11;
		T tmp21 = m00 * m12 - m02 * m10;
		T tmp22 = m00 * m11 - m01 * m10;
		T tmp23 = m00 * (m11 * m32 - m12 * m31) - m01 * (m10 * m32 - m12 * m30) + m02 * (m10 * m31 - m11 * m30);

		m00 =  tmp00; m01 = -tmp10; m02 =  tmp20;
		m10 = -tmp01; m11 =  tmp11; m12 = -tmp21;
		m20 =  tmp02; m21 = -tmp12; m22 =  tmp22;
		m30 = -tmp03; m31 =  tmp13; m32 = -tmp23;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat43<T>::Adjoint() const noexcept -> Mat43
	{
		return
		{
			 (m11 * m22 - m12 * m21),
			-(m01 * m22 - m02 * m21),
			 (m01 * m12 - m02 * m11),

			-(m10 * m22 - m12 * m20),
			 (m00 * m22 - m02 * m20),
			-(m00 * m12 - m02 * m10),

			 (m10 * m21 - m11 * m20),
			-(m00 * m21 - m01 * m20),
			 (m00 * m11 - m01 * m10),

			-(m10 * (m21 * m32 - m22 * m31) - m11 * (m20 * m32 - m22 * m30) + m12 * (m20 * m31 - m21 * m30)),
			 (m00 * (m21 * m32 - m22 * m31) - m01 * (m20 * m32 - m22 * m30) + m02 * (m20 * m31 - m21 * m30)),
			-(m00 * (m11 * m32 - m12 * m31) - m01 * (m10 * m32 - m12 * m30) + m02 * (m10 * m31 - m11 * m30))

		};
	}

	template <Numeric T>
	constexpr auto Mat43<T>::ToCofactor() noexcept -> Mat43&
	{
		T tmp00 = m11 * m22 - m12 * m21;
		T tmp01 = m10 * m22 - m12 * m20;
		T tmp02 = m10 * m21 - m11 * m20;

		T tmp10 = m01 * m22 - m02 * m21;
		T tmp11 = m00 * m22 - m02 * m20;
		T tmp12 = m00 * m21 - m01 * m20;

		T tmp20 = m01 * m12 - m02 * m11;
		T tmp21 = m00 * m12 - m02 * m10;
		T tmp22 = m00 * m11 - m01 * m10; 

		m00 =  tmp00; m01 = -tmp01; m02 =  tmp02;
		m10 = -tmp10; m11 =  tmp11; m12 = -tmp12;
		m20 =  tmp20; m21 = -tmp21; m22 =  tmp22;
		m30 =  T(0) ; m31 =  T(0) ; m32 =  T(0) ;
		return *this;

	}

	template <Numeric T>
	constexpr auto Mat43<T>::Cofactor() const noexcept -> Mat43
	{
		return
		{
			 (m11 * m22 - m12 * m21),
			-(m10 * m22 - m12 * m20),
			 (m10 * m21 - m11 * m20),

			-(m01 * m22 - m02 * m21),
			 (m00 * m22 - m02 * m20),
			-(m00 * m21 - m01 * m20),

			 (m01 * m12 - m02 * m11),
			-(m00 * m12 - m02 * m10),
			 (m00 * m11 - m01 * m10),

			 T(0),
			 T(0),
			 T(0)
		};
	}

	template <Numeric T>
	constexpr auto Mat43<T>::Invert() noexcept -> Mat43&
	{
		T det = Determinant();
		if (EpsilonCompare(det, 0))
		{
			*this = Mat43{};
			return *this;
		}

		Adjugate();
		*this *= Rcp(det);
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat43<T>::Inverse() const noexcept -> Mat43
	{
		T det = Determinant();
		if (EpsilonCompare(det, 0))
			return Mat43{ 0 };
		return Adjoint() * Rcp(det);
	}

	template <Numeric T>
	constexpr auto Mat43<T>::TransformVector(const Vec4<T>& vec) const noexcept -> Vec4<T>
	{
		return vec.x * Vec4<T>{ row0, 0 } + vec.x * Vec4<T>{ row1, 0 } + vec.x * Vec4<T>{ row2, 0 } + vec.x * Vec4<T>{ row3, 1 };
	}

	template <Numeric T>
	constexpr auto Mat43<T>::TransformVector(const Vec3<T>& vec) const noexcept -> Vec3<T>
	{
		return vec.x * row0 + vec.y * row1 + vec.x * row2;
	}

	template <Numeric T>
	constexpr auto Mat43<T>::TransformVector(const Vec2<T>& vec) const noexcept -> Vec2<T>
	{
		return
		{
			m00 * vec.x + m01 * vec.y,
			m10 * vec.x + m11 * vec.y,
		};
	}

	template <Numeric T>
	constexpr auto Mat43<T>::TransformPoint(const Vec3<T>& vec) const noexcept -> Vec3<T>
	{
		return vec.x * row0 + vec.y * row1 + vec.x * row2 + row3;
	}

	template <Numeric T>
	constexpr auto Mat43<T>::TransformPoint(const Vec2<T>& vec) const noexcept -> Vec2<T>
	{
		return vec.x * Vec2<T>{ m00, m01 } + vec.y * Vec2<T>{ m10, m11 } + Vec2<T>{ m30, m31 };
	}

	template <Numeric T>
	constexpr auto Mat43<T>::Decompose() const noexcept -> Tuple<Vec3<T>, Quaternion<T>, Vec3<T>>
	{
		Vec3<T> scale;
		scale.x = Column(0).Len();
		scale.y = Column(1).Len();
		scale.z = Column(2).Len();

		Mat3 qMat;
		qMat.row0 = row0 / scale;
		qMat.row1 = row1 / scale;
		qMat.row2 = row2 / scale;
		Quaternion<T> quat{ qMat };

		Vec3<T> trans{ m30, m31, m32 };

		return { scale, quat, trans };
	}

	template <Numeric T>
	constexpr auto Mat43<T>::Decompose2D() const noexcept -> Tuple<Vec2<T>, Angle<T, AngleKind::Radians>, Vec2<T>>
	{
		MATH_ASSERT((row2 == Vec3<T>{ 0, 0, 1 }), "Matrix is not a 2D tansformation");

		Vec2<T> scale;
		scale.x = Column(0).Len();
		scale.y = Column(1).Len();

		Angle<T, AngleKind::Radians> angle = ACos(m00 / scale.x);
		Vec2<T> trans{ m02, m12 };

		return { scale, angle, trans };
	}

	template <Numeric T>
	constexpr auto Mat43<T>::GetScaleRotationMatrix() const noexcept -> Mat3<T>
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
	constexpr auto Mat43<T>::CreateScale2D(A scale) noexcept -> Mat43
	{
		return
		{
			T(scale), T(0)    , T(0),
			T(0)    , T(scale), T(0),
			T(0)    , T(0)    , T(1),
			T(0)    , T(0)    , T(0)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat43<T>::CreateScale2D(const Vec2<A>& scale) noexcept -> Mat43
	{
		return
		{
			T(scale.x), T(0)      , T(0),
			T(0)      , T(scale.y), T(0),
			T(0)      , T(0)      , T(1),
			T(0)      , T(0)      , T(0)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat43<T>::CreateShear2D(const Vec2<A>& shear) noexcept -> Mat43
	{
		return
		{
			T(1)      , T(shear.y), T(0),
			T(shear.x), T(1)      , T(0),
			T(0)      , T(0)      , T(1),
			T(0)      , T(0)      , T(0)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, AngleKind Kind>
	constexpr auto Mat43<T>::CreateRotation2D(const Angle<A, Kind>& angle) noexcept -> Mat43
	{
		T cs = T(Cos(angle));
		T sn = T(Sin(angle));
		return
		{
			cs  , -sn , T(0),
			sn  ,  cs , T(0),
			T(0), T(0), T(1),
			T(0), T(0), T(0)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat43<T>::CreateTranslation2D(const Vec2<A>& trans) noexcept -> Mat43
	{
		return
		{
			T(1)      , T(0)      , T(0),
			T(0)      , T(1)      , T(0),
			T(0)      , T(0)      , T(0),
			T(trans.x), T(trans.y), T(1),
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, AngleKind Kind, ConvertableTo<T> C>
	constexpr auto Mat43<T>::CreateTransform2D(A scale, const Angle<B, Kind>& angle, const Vec2<T>& trans) noexcept -> Mat43
	{
		T cs = T(Cos(angle));
		T sn = T(Sin(angle));
		return
		{
			T(scale) * cs, T(scale) * -sn, T(0),
			T(scale) * sn, T(scale) *  cs, T(0),
			T(0)         , T(0)          , T(0),
			T(trans.x)   , T(trans.y)    , T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, AngleKind Kind, ConvertableTo<T> C>
	constexpr auto Mat43<T>::CreateTransform2D(const Vec2<A>& scale, const Angle<B, Kind>& angle, const Vec2<C>& trans) noexcept -> Mat43
	{
		T cs = T(Cos(angle));
		T sn = T(Sin(angle));
		return
		{
			T(scale.x) * cs, T(scale.y) * -sn, T(0),
			T(scale.x) * sn, T(scale.y) *  cs, T(0),
			T(0)           , T(0)            , T(0),
			T(trans.x)     , T(trans.y)      , T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat43<T>::CreateScale(A scale) noexcept -> Mat43
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
	constexpr auto Mat43<T>::CreateScale(const Vec3<A>& scale) noexcept -> Mat43
	{
		return
		{
			T(scale.x), T(0)      , T(0)      ,
			T(0)      , T(scale.y), T(0)      ,
			T(0)      , T(0)      , T(scale.z),
			T(0)      , T(0)      , T(0)      
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat43<T>::CreateRotation(const Quaternion<A>& quat) noexcept -> Mat43
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
			T(1) - T(2) * (yy + zz),        T(2) * (xy - zw),        T(2) * (xz + yw),
				   T(2) * (xy + zw), T(1) - T(2) * (xx + zz),        T(2) * (yz - zw),
				   T(2) * (xz - yw),        T(2) * (yz + xw), T(1) - T(2) * (xx + yy),
			T(0)                   , T(0)                   , T(0)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat43<T>::CreateTranslation(const Vec3<A>& trans) noexcept -> Mat43
	{
		return
		{
			T(1)      , T(0)      , T(0)      ,
			T(0)      , T(1)      , T(0)      ,
			T(0)      , T(0)      , T(1)      ,
			T(trans.x), T(trans.y), T(trans.z)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C>
	constexpr auto Mat43<T>::CreateTransform(A scale, const Quaternion<B>& quat, const Vec3<C>& trans) noexcept -> Mat43
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
			T(scale) * (T(1) - T(2) * (yy + zz)), T(scale) * (       T(2) * (xy - zw)), T(scale) * (       T(2) * (xz + yw)),
			T(scale) * (       T(2) * (xy + zw)), T(scale) * (T(1) - T(2) * (xx + zz)), T(scale) * (       T(2) * (yz - zw)),
			T(scale) * (       T(2) * (xz - yw)), T(scale) * (       T(2) * (yz + xw)), T(scale) * (T(1) - T(2) * (xx + yy)),
			T(trans.x)                          , T(trans.y)                          , T(trans.z)                          
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, ConvertableTo<T> C>
	constexpr auto Mat43<T>::CreateTransform(const Vec3<A>& scale, const Quaternion<B>& quat, const Vec3<C>& trans) noexcept -> Mat43
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
			T(scale.x) * (T(1) - T(2) * (yy + zz)), T(scale.y) * (       T(2) * (xy - zw)), T(scale.z) * (       T(2) * (xz + yw)),
			T(scale.x) * (       T(2) * (xy + zw)), T(scale.y) * (T(1) - T(2) * (xx + zz)), T(scale.z) * (       T(2) * (yz - zw)),
			T(scale.x) * (       T(2) * (xz - yw)), T(scale.y) * (       T(2) * (yz + xw)), T(scale.z) * (T(1) - T(2) * (xx + yy)),
			T(trans.x)                            , T(trans.y)                            , T(trans.z)                            
		};
	}

	template <Numeric T, ConvertableTo<T> U>
	constexpr auto operator*(const Vec4<T>& vec, const Mat43<U>& mat) noexcept -> Vec4<T>
	{
		return vec.x * Vec4<T>{ mat.row0, 0 } + vec.x * Vec4<T>{ mat.row1, 0 } + vec.x * Vec4<T>{ mat.row2, 0 } + vec.x * Vec4<T>{ mat.row3, 1 };
	}

	template <Numeric T, ConvertableTo<T> U>
	constexpr auto operator*=(Vec4<T>& vec, const Mat43<U>& mat) noexcept -> Vec4<T>&
	{
		return vec.x * Vec4<T>{ mat.row0, 0 } + vec.x * Vec4<T>{ mat.row1, 0 } + vec.x * Vec4<T>{ mat.row2, 0 } + vec.x * Vec4<T>{ mat.row3, 1 };
	}
}
