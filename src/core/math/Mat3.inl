#pragma once

#if __RESHARPER__
#include "Mat3.h"
#endif

#include "Constants.h"
#include "Mat2.h"
#include "core/Assert.h"
#include "core/utils/Algo.h"

namespace Core::Math
{
	template <Numeric T>
	constexpr Mat3<T>::Mat3() noexcept
		: m00(T(1)), m01(T(0)), m02(T(0))
		, m10(T(0)), m11(T(1)), m12(T(0))
		, m20(T(0)), m21(T(0)), m22(T(1))
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr Mat3<T>::Mat3(A diag) noexcept
		: m00(T(diag)), m01(T(0))   , m02(T(0))
		, m10(T(0))   , m11(T(diag)), m12(T(0))
		, m20(T(0))   , m21(T(0))   , m22(T(diag))
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> U0, ConvertableTo<T> U1, ConvertableTo<T> U2, ConvertableTo<T> U3, ConvertableTo<T> U4, ConvertableTo<T> U5, ConvertableTo<T> U6,
		ConvertableTo<T> U7, ConvertableTo<T> U8>
	constexpr Mat3<T>::Mat3(U0 m00, U1 m01, U2 m02, U3 m10, U4 m11, U5 m12, U6 m20, U7 m21, U8 m22) noexcept
		: m00(T(m00)), m01(T(m01)), m02(T(m02))
		, m10(T(m10)), m11(T(m11)), m12(T(m12))
		, m20(T(m20)), m21(T(m21)), m22(T(m22))
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> U0, ConvertableTo<T> U1, ConvertableTo<T> U2>
	constexpr Mat3<T>::Mat3(const Vec3<U0>& row0, const Vec3<U1>& row1, const Vec3<U2>& row2)
		: row0(row0)
		, row1(row1)
		, row2(row2)
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> U0, ConvertableTo<T> U1, ConvertableTo<T> U2>
	constexpr Mat3<T>::Mat3(ColumnInitTag, const Vec3<U0>& column0, const Vec3<U1>& column1, const Vec3<U2>& column2)
		: m00(T(column0.x)), m01(T(column1.x)), m02(T(column2.x))
		, m10(T(column0.y)), m11(T(column1.y)), m12(T(column2.y))
		, m20(T(column0.z)), m21(T(column1.z)), m22(T(column2.z))
	{
	}

	template <Numeric T>
	constexpr Mat3<T>::Mat3(const Mat3& other) noexcept
		: row0(other.row0)
		, row1(other.row1)
		, row2(other.row2)
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr Mat3<T>::Mat3(const Mat3<A>& other) noexcept
		: row0(other.row0)
		, row1(other.row1)
		, row2(other.row2)
	{
	}

	template <Numeric T>
	constexpr Mat3<T>::Mat3(Mat3&& other) noexcept
		: row0(Move(other.row0))
		, row1(Move(other.row1))
		, row2(Move(other.row2))
	{
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator=(const Mat3& other) noexcept -> Mat3&
	{
		row0 = other.row0;
		row1 = other.row1;
		row2 = other.row2;
		return *this;
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat3<T>::operator=(const Mat3<A>& other) noexcept -> Mat3&
	{
		row0 = other.row0;
		row1 = other.row1;
		row2 = other.row2;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator=(Mat3&& other) noexcept -> Mat3&
	{
		row0 = Move(other.row0);
		row1 = Move(other.row1);
		row2 = Move(other.row2);
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator+() const noexcept -> Mat3
	{
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator-() const noexcept -> Mat3
	{
		return
		{
			-row0,
			-row1,
			-row2
		};
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator+(const Mat3& other) const noexcept -> Mat3
	{
		return
		{
			row0 + other.row0,
			row1 + other.row1,
			row2 + other.row2
		};
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator-(const Mat3& other) const noexcept -> Mat3
	{
		return
		{
			row0 - other.row0,
			row1 - other.row1,
			row2 - other.row2
		};
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator*(const Mat3& other) const noexcept -> Mat3
	{
		return
		{
			m00 * other.m00 + m01 * other.m10 + m02 * other.m20, // row0 dot column0
			m00 * other.m01 + m01 * other.m11 + m02 * other.m21, // row0 dot column1
			m00 * other.m02 + m01 * other.m12 + m02 * other.m22, // row0 dot column2
			m10 * other.m00 + m11 * other.m10 + m12 * other.m20, // row1 dot column0
			m10 * other.m01 + m11 * other.m11 + m12 * other.m21, // row1 dot column1
			m10 * other.m02 + m11 * other.m12 + m12 * other.m22, // row1 dot column2
			m20 * other.m00 + m21 * other.m10 + m22 * other.m20, // row2 dot column0
			m20 * other.m01 + m21 * other.m11 + m22 * other.m21, // row2 dot column1
			m20 * other.m02 + m21 * other.m12 + m22 * other.m22, // row2 dot column2
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat3<T>::operator*(A val) const noexcept -> Mat3
	{
		return
		{
			row0 * val,
			row1 * val,
			row2 * val
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat3<T>::operator/(A val) const noexcept -> Mat3
	{
		return
		{
			row0 / val,
			row1 / val,
			row2 / val
		};
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator*(const Vec3<T>& vec) const noexcept -> Vec3<T>
	{
		return
		{
			m00 * vec.x + m01 * vec.y * m02 * vec.z,
			m10 * vec.x + m11 * vec.y + m12 * vec.z,
			m20 * vec.x + m21 * vec.y + m22 * vec.z
		};
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator+=(const Mat3& other) noexcept -> Mat3&
	{
		row0 += other.row0;
		row1 += other.row1;
		row2 += other.row2;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator-=(const Mat3& other) noexcept -> Mat3&
	{
		row0 -= other.row0;
		row1 -= other.row1;
		row2 -= other.row2;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator*=(const Mat3& other) noexcept -> Mat3&
	{
		T tmp00 = m00 * other.m00 + m01 * other.m10 + m02 * other.m20; // row0 dot column0
		T tmp01 = m00 * other.m01 + m01 * other.m11 + m02 * other.m21; // row0 dot column1
		T tmp02 = m00 * other.m02 + m01 * other.m12 + m02 * other.m22; // row0 dot column2
		T tmp10 = m10 * other.m00 + m11 * other.m10 + m12 * other.m20; // row1 dot column0
		T tmp11 = m10 * other.m01 + m11 * other.m11 + m12 * other.m21; // row1 dot column1
		T tmp12 = m10 * other.m02 + m11 * other.m12 + m12 * other.m22; // row1 dot column2
		T tmp20 = m20 * other.m00 + m21 * other.m10 + m22 * other.m20; // row2 dot column0
		T tmp21 = m20 * other.m01 + m21 * other.m11 + m22 * other.m21; // row2 dot column1
		T tmp22 = m20 * other.m02 + m21 * other.m12 + m22 * other.m22; // row2 dot column2

		m00 = tmp00; m01 = tmp01; m02 = tmp02;
		m10 = tmp10; m11 = tmp11; m12 = tmp12;
		m20 = tmp20; m21 = tmp21; m22 = tmp22;
		return *this;
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat3<T>::operator*=(A val) noexcept -> Mat3&
	{
		row0 *= val;
		row1 *= val;
		row2 *= val;
		return *this;
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat3<T>::operator/=(A val) noexcept -> Mat3&
	{
		row0 /= val;
		row1 /= val;
		row2 /= val;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator[](usize idx) noexcept -> T&
	{
		MATH_ASSERT(idx < 9, "Index out of range");
		return data[idx];
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator[](usize idx) const noexcept -> T
	{
		MATH_ASSERT(idx < 9, "Index out of range");
		return data[idx];
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator()(usize row, usize column) noexcept -> T&
	{
		MATH_ASSERT(row    < 3, "Row index out of range");
		MATH_ASSERT(column < 3, "Column index out of range");
		return rows[row][column];
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator()(usize row, usize column) const noexcept -> T
	{
		MATH_ASSERT(row    < 3, "Row index out of range");
		MATH_ASSERT(column < 3, "Column index out of range");
		return rows[row][column];
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator==(const Mat3& other) const noexcept -> bool
	{
		return row0 == other.row0 && row1 == other.row1 && row2 == other.row2;
	}

	template <Numeric T>
	constexpr auto Mat3<T>::operator!=(const Mat3& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Compare(const Mat3& other, T e) const noexcept -> bool
	{
		return row0.Compare(other.row0, e) && row1.Compare(other.row1, e) && row2.Compare(other.row2, e);
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Row(usize idx) noexcept -> Vec3<T>&
	{
		MATH_ASSERT(idx < 3, "Row index out of range");
		return rows[idx];
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Row(usize idx) const noexcept -> Vec3<T>
	{
		MATH_ASSERT(idx < 3, "Row index out of range");
		return rows[idx];
	}

	template <Numeric T>
	constexpr void Mat3<T>::SetRow(usize idx, const Vec3<T>& row) noexcept
	{
		MATH_ASSERT(idx < 3, "Row index out of range");
		rows[idx] = row;
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Column(usize idx) const noexcept -> Vec3<T>
	{
		MATH_ASSERT(idx < 3, "Column index out of range");
		return { row0[idx], row1[idx], row2[idx] };
	}

	template <Numeric T>
	constexpr void Mat3<T>::SetColumn(usize idx, const Vec3<T>& column) noexcept
	{
		MATH_ASSERT(idx < 3, "Column index out of range");
		row0[idx] = column.x;
		row1[idx] = column.y;
		row2[idx] = column.z;
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Diagonal() const noexcept -> Vec3<T>
	{
		return { m00, m11, m22 };
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Minor(usize row, usize column) const noexcept -> Mat2<T>
	{
		MATH_ASSERT(row    < 3, "Row index out of range");
		MATH_ASSERT(column < 3, "Column index out of range");

		switch (row)
		{
		case 0:
		{
			switch (column)
			{
			case 0: return { m11, m12, m21, m22 };
			case 1: return { m10, m12, m20, m22 };
			case 2:	return { m10, m11, m20, m21 };
			default: ASSERT(false, "Invalid Mat3::Minor case"); break;
			}
		}
		case 1:
		{
			switch (column)
			{
			case 0:	return { m01, m02, m21, m22 };
			case 1:	return { m00, m02, m20, m22 };
			case 2:	return { m00, m01, m20, m21 };
			default: ASSERT(false, "Invalid Mat3::Minor case"); break;
			}
		}
		case 2:
		{
			switch (column)
			{
			case 0:	return { m01, m02, m11, m12 };
			case 1:	return { m00, m02, m10, m12 };
			case 2:	return { m00, m01, m10, m11 };
			default: ASSERT(false, "Invalid Mat3::Minor case"); break;
			}
		}
		default: ASSERT(false, "Invalid Mat3::Minor case"); break;
		}
		return {};
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Determinant() const noexcept -> T
	{
		return m00 * (m11 * m22 - m21 * m12) - m01 * (m10 * m22 - m12 * m20) + m02 * (m10 * m21 - m11 * m20);
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Trace() const noexcept -> T
	{
		return m00 + m11 + m22;
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Transpose() noexcept -> Mat3&
	{
		Algo::Swap(m01, m10);
		Algo::Swap(m02, m20);
		Algo::Swap(m12, m21);
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Transposed() const noexcept -> Mat3
	{
		return
		{
			m00, m10, m20,
			m01, m11, m21,
			m02, m12, m22
		};
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Adjugate() noexcept -> Mat3&
	{
		T tmp00 = m11 * m22 - m12 * m21; //  det(m11, m12, m21, m22)
		T tmp01 = m02 * m21 - m01 * m22; // -det(m01, m02, m21, m22)
		T tmp02 = m01 * m12 - m02 * m11; //  det(m01, m02, m11, m12)

		T tmp10 = m12 * m20 - m10 * m22; // -det(m10, m12, m20, m22)
		T tmp11 = m00 * m22 - m02 * m20; //  det(m00, m02, m20, m22)
		T tmp12 = m02 * m10 - m00 * m12; // -det(m00, m02, m10, m12)

		T tmp20 = m10 * m21 - m11 * m20; //  det(m10, m11, m20, m21)
		T tmp21 = m01 * m20 - m00 * m21; // -det(m00, m01, m20, m21)
		T tmp22 = m00 * m11 - m01 * m10; //  det(m00, m01, m10, m11)

		m00 = tmp00; m01 = tmp01; m02 = tmp02;
		m10 = tmp10; m11 = tmp11; m12 = tmp12;
		m20 = tmp20; m21 = tmp21; m22 = tmp22;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Adjoint() const noexcept -> Mat3
	{
		return
		{
			m11 * m22 - m12 * m21, //  det(m11, m12, m21, m22)
			m02 * m21 - m01 * m22, // -det(m01, m02, m21, m22)
			m01 * m12 - m02 * m11, //  det(m01, m02, m11, m12)

			m12 * m20 - m10 * m22, // -det(m10, m12, m20, m22)
			m00 * m22 - m02 * m20, //  det(m00, m02, m20, m22)
			m02 * m10 - m00 * m12, // -det(m00, m02, m10, m12)

			m10 * m21 - m11 * m20, //  det(m10, m11, m20, m21)
			m01 * m20 - m00 * m21, // -det(m00, m01, m20, m21)
			m00 * m11 - m01 * m10  //  det(m00, m01, m10, m11)
		};
	}

	template <Numeric T>
	constexpr auto Mat3<T>::ToCofactor() noexcept -> Mat3&
	{
		T tmp00 = m11 * m22 - m12 * m21; //  det(m11, m12, m21, m22)
		T tmp01 = m12 * m20 - m10 * m22; // -det(m10, m12, m20, m22)
		T tmp02 = m10 * m21 - m11 * m20; //  det(m10, m11, m20, m21)

		T tmp10 = m02 * m21 - m01 * m22; // -det(m01, m02, m21, m22)
		T tmp11 = m00 * m22 - m02 * m20; //  det(m00, m02, m20, m22)
		T tmp12 = m01 * m20 - m00 * m21; // -det(m00, m01, m20, m21)

		T tmp20 = m01 * m12 - m02 * m11; //  det(m01, m02, m11, m12)
		T tmp21 = m02 * m10 - m00 * m12; // -det(m00, m02, m10, m12)
		T tmp22 = m00 * m11 - m01 * m10; //  det(m00, m01, m10, m11)

		m00 = tmp00; m01 = tmp01; m02 = tmp02;
		m10 = tmp10; m11 = tmp11; m12 = tmp12;
		m20 = tmp20; m21 = tmp21; m22 = tmp22;
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Cofactor() const noexcept -> Mat3
	{
		return
		{
			m11 * m22 - m12 * m21, //  det(m11, m12, m21, m22)
			m12 * m20 - m10 * m22, // -det(m10, m12, m20, m22)
			m10 * m21 - m11 * m20, //  det(m10, m11, m20, m21)

			m02 * m21 - m01 * m22, // -det(m01, m02, m21, m22)
			m00 * m22 - m02 * m20, //  det(m00, m02, m20, m22)
			m01 * m20 - m00 * m21, // -det(m00, m01, m20, m21)

			m01 * m12 - m02 * m11, //  det(m01, m02, m11, m12)
			m02 * m10 - m00 * m12, // -det(m00, m02, m10, m12)
			m00 * m11 - m01 * m10  //  det(m00, m01, m10, m11)
		};
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Invert() noexcept -> Mat3&
	{
		T det = Determinant();
		if (EpsilonCompare(det, 0))
		{
			*this = Mat3{};
			return *this;
		}

		Adjugate();
		*this *= Rcp(det);
		return *this;
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Inverse() const noexcept -> Mat3
	{
		T det = Determinant();
		if (EpsilonCompare(det, 0))
			return Mat3{ 0 };
		return Adjoint() * Rcp(det);
	}

	template <Numeric T>
	constexpr auto Mat3<T>::TransformVector(const Vec3<T>& vec) const noexcept -> Vec3<T>
	{
		return vec.x * row0 + vec.y * row1 + vec.z * row2;;
	}

	template <Numeric T>
	constexpr auto Mat3<T>::TransformVector(const Vec2<T>& vec) const noexcept -> Vec2<T>
	{
		return
		{
			vec.x * m00 + vec.y * m10,
			vec.x * m01 + vec.y * m11
		};
	}

	template <Numeric T>
	constexpr auto Mat3<T>::TransformPoint(const Vec2<T>& vec) const noexcept -> Vec2<T>
	{
		return
		{
			vec.x* m00 + vec.y * m10 + m20,
			vec.x* m01 + vec.y * m11 + m21
		};
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Decompose() const noexcept -> Pair<Vec3<T>, Quaternion<T>>
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

		return { scale, quat };
	}

	template <Numeric T>
	constexpr auto Mat3<T>::Decompose2D() const noexcept -> Tuple<Vec2<T>, Angle<T, AngleKind::Radians>, Vec2<T>>
	{
		MATH_ASSERT((row2 == Vec3<T>{ 0, 0, 1 }), "Matrix is not a 2D tansformation");

		Vec2<T> scale;
		scale.x = Column(0).Len();
		scale.y = Column(1).Len();

		Angle<T, AngleKind::Radians> angle = ACos(m00 / scale.x);
		Vec2<T> trans{ m20, m21 };

		return { scale, angle, trans };
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat3<T>::CreateScale2D(A scale) noexcept -> Mat3
	{
		return
		{
			T(scale), T(0)    , T(0),
			T(0)    , T(scale), T(0),
			T(0)    , T(0)    , T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat3<T>::CreateScale2D(const Vec2<A>& scale) noexcept -> Mat3
	{
		return
		{
			T(scale.x), T(0)      , T(0),
			T(0)      , T(scale.y), T(0),
			T(0)      , T(0)      , T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat3<T>::CreateShear2D(const Vec2<A>& shear) noexcept -> Mat3
	{
		return
		{
			T(1)      , T(shear.y), T(0),
			T(shear.x), T(1)      , T(0),
			T(0)      , T(0)      , T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, AngleKind Kind>
	constexpr auto Mat3<T>::CreateRotation2D(const Angle<A, Kind>& angle) noexcept -> Mat3
	{
		T cs = T(Cos(angle));
		T sn = T(Sin(angle));
		return
		{
			cs  , -sn , T(0),
			sn  ,  cs , T(0),
			T(0), T(0), T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat3<T>::CreateTranslation2D(const Vec2<A>& trans) noexcept -> Mat3
	{
		return
		{
			T(1)      , T(0)      , T(0),
			T(0)      , T(1)      , T(0),
			T(trans.x), T(trans.y), T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, AngleKind Kind, ConvertableTo<T> C>
	constexpr auto Mat3<T>::CreateTransform2D(A scale, const Angle<B, Kind>& angle, const Vec2<T>& trans) noexcept -> Mat3
	{
		T cs = T(Cos(angle));
		T sn = T(Sin(angle));
		return
		{
			T(scale) * cs, T(scale) * -sn, T(0),
			T(scale) * sn, T(scale) *  cs, T(0),
			T(trans.x)   , T(trans.y)    , T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B, AngleKind Kind, ConvertableTo<T> C>
	constexpr auto Mat3<T>::CreateTransform2D(const Vec2<A>& scale, const Angle<B, Kind>& angle, const Vec2<C>& trans) noexcept -> Mat3
	{
		T cs = T(Cos(angle));
		T sn = T(Sin(angle));
		return
		{
			T(scale.x) * cs, T(scale.y) * -sn, T(0),
			T(scale.x) * sn, T(scale.y) *  cs, T(0),
			T(trans.x)     , T(trans.y)      , T(1)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat3<T>::CreateScale(A scale) noexcept -> Mat3
	{
		return Mat3{ scale };
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat3<T>::CreateScale(const Vec3<A>& scale) noexcept -> Mat3
	{
		return
		{
			T(scale.x), T(0)      , T(0),
			T(0)      , T(scale.y), T(0),
			T(0)      , T(0)      , T(scale.z)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A>
	constexpr auto Mat3<T>::CreateRotation(const Quaternion<A>& quat) noexcept -> Mat3
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
				   T(2) * (xz - yw),        T(2) * (yz + xw), T(1) - T(2) * (xx + yy)
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B>
	constexpr auto Mat3<T>::CreateTransform(A scale, const Quaternion<B>& quat) noexcept -> Mat3
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
			T(scale) * (       T(2) * (xz - yw)), T(scale) * (       T(2) * (yz + xw)), T(scale) * (T(1) - T(2) * (xx + yy))
		};
	}

	template <Numeric T>
	template <ConvertableTo<T> A, ConvertableTo<T> B>
	constexpr auto Mat3<T>::CreateTransform(const Vec3<A>& scale, const Quaternion<B>& quat) noexcept -> Mat3
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
			T(scale.x) * (       T(2) * (xz - yw)), T(scale.y) * (       T(2) * (yz + xw)), T(scale.z) * (T(1) - T(2) * (xx + yy))
		};
	}

	template <Numeric T, ConvertableTo<T> U>
	constexpr auto operator*(const Vec3<T>& vec, const Mat3<U>& mat) noexcept -> Vec3<T>
	{
		return vec.x * mat.row0 + vec.y * mat.row1 + vec.z * mat.row2;
	}

	template <Numeric T, ConvertableTo<T> U>
	constexpr auto operator*=(Vec3<T>& vec, const Mat3<U>& mat) noexcept -> Vec3<T>&
	{
		vec = vec.x * mat.row0 + vec.y * mat.row1 + vec.z * mat.row2;
		return vec;
	}
}
