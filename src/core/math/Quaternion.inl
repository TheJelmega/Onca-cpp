#pragma once

#if __RESHARPER__
#include "Quaternion.h"
#endif

#include "Constants.h"
#include "Angle.h"

namespace Onca::Math
{
	template <Numeric T>
	constexpr Quaternion<T>::Quaternion() noexcept
		: w(T(1))
		, x(T(0))
		, y(T(0))
		, z(T(0))
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> W, ConvertableTo<T> X, ConvertableTo<T> Y, ConvertableTo<T> Z>
	constexpr Quaternion<T>::Quaternion(W w, X x, Y y, Z z) noexcept
		: w(T(w))
		, x(T(x))
		, y(T(y))
		, z(T(z))
	{
	}

	template <Numeric T>
	template <ConvertableTo<T> R, ConvertableTo<T> I>
	constexpr Quaternion<T>::Quaternion(R real, const Vec3<I>& imaginary)
		: real(T(real) )
		, imaginary(imaginary)
	{
	}

	template <Numeric T>
	constexpr Quaternion<T>::Quaternion(const Mat3<T>& mat)
	{
		T trace = mat.m00 + mat.m11 + mat.m22;
		if (trace > 0)
		{
			T s = Sqrt(T(1) + trace) * T(2); // 4 * w
			w = s / T(4);
			x = (mat.m21 - mat.m12) / s;
			y = (mat.m02 - mat.m20) / s;
			z = (mat.m10 - mat.m01) / s;
		}
		else if (mat.m00 > mat.m11 && mat.m00 > mat.m22)
		{
			T s = Sqrt(T(1) + mat.m00 - mat.m11 - mat.m22) * T(2);
			w = (mat.m21 - mat.m12) / s;
			x = s / T(4);
			y = (mat.m01 + mat.m10) / s;
			z = (mat.m02 + mat.m20) / s;
		}
		else if (mat.m11 > mat.m22)
		{
			T s = Sqrt(T(1) + mat.m11 - mat.m00 - mat.m22);
			w = (mat.m02 - mat.m20) / s;
			x = (mat.m01 + mat.m10) / s;
			y = s / T(4);
			z = (mat.m12 + mat.m21) / s;
		}
		else
		{
			T s = Sqrt(T(1) + mat.m22 - mat.m00 - mat.m11);
			w = (mat.m10 - mat.m01) / s;
			x = (mat.m02 + mat.m20) / s;
			y = (mat.m12 + mat.m21) / s;
			z = s / T(4);
		}
	}

	template <Numeric T>
	constexpr Quaternion<T>::Quaternion(const Quaternion& other) noexcept
		: w(other.w)
		, x(other.x)
		, y(other.y)
		, z(other.z)
	{
	}

	template <Numeric T>
	template <Numeric U>
	constexpr Quaternion<T>::Quaternion(const Quaternion<U>& other) noexcept
		: x(T(other.x))
		, y(T(other.y))
		, z(T(other.z))
		, w(T(other.w))
	{
	}

	template <Numeric T>
	constexpr Quaternion<T>::Quaternion(Quaternion&& other) noexcept
		: w(other.w)
		, x(other.x)
		, y(other.y)
		, z(other.z)
	{
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::operator=(const Quaternion& other) noexcept -> Quaternion
	{
		w = other.w;
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	template <Numeric T>
	template <Numeric U>
	constexpr auto Quaternion<T>::operator=(const Quaternion<U>& other) noexcept -> Quaternion
	{
		w = T(other.w);
		x = T(other.x);
		y = T(other.y);
		z = T(other.z);
		return *this;
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::operator=(Quaternion&& other) noexcept -> Quaternion
	{
		w = other.w;
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::operator+() const noexcept -> Quaternion
	{
		return *this;
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::operator-() const noexcept -> Quaternion
	{
		return { -w, -x, -y, -z };
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::operator+(const Quaternion& other) const noexcept -> Quaternion
	{
		return { w + other.w, x + other.x, y + other.y, z + other.z };
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::operator-(const Quaternion& other) const noexcept -> Quaternion
	{
		return { w - other.w, x - other.x, y - other.y, z - other.z };
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::operator*(const Quaternion& other) const noexcept -> Quaternion
	{
		return {
			w * other.w - x * other.x - y * other.y - z * other.z,
			w * other.x + x * other.w + y * other.z - z * other.y,
			w * other.y + y * other.w + z * other.x - x * other.z,
			w * other.z + z * other.w + x * other.y - y * other.x
		};
	}

	template <Numeric T>
	template<ConvertableTo<T> U>
	constexpr auto Quaternion<T>::operator*(U val) const noexcept -> Quaternion
	{
		return { w * T(val), x * T(val), y * T(val), z * T(val) };
	}

	template <Numeric T>
	template <ConvertableTo<T> U>
	constexpr auto Quaternion<T>::operator/(U val) const noexcept -> Quaternion
	{
		return { w / T(val), x / T(val), y / T(val), z / T(val) };
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::operator+=(const Quaternion& other) noexcept -> Quaternion&
	{
		w += other.w;
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::operator-=(const Quaternion& other) noexcept -> Quaternion&
	{
		w -= other.w;
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::operator*=(const Quaternion& other) noexcept -> Quaternion&
	{
		T tmpW = w * other.w - x * other.x - y * other.y - z * other.z;
		T tmpX = w * other.x + x * other.w + y * other.z - z * other.y;
		T tmpY = w * other.y + y * other.w + z * other.x - x * other.z;
		T tmpZ = w * other.z + z * other.w + x * other.y - y * other.x;
		w = tmpW;
		x = tmpX;
		y = tmpY;
		z = tmpZ;
		return *this;
	}

	template <Numeric T>
	template<ConvertableTo<T> U>
	constexpr auto Quaternion<T>::operator*=(U val) noexcept -> Quaternion&
	{
		w *= T(val);
		x *= T(val);
		y *= T(val);
		z *= T(val);
		return *this;
	}

	template <Numeric T>
	template <ConvertableTo<T> U>
	constexpr auto Quaternion<T>::operator/=(U val) noexcept -> Quaternion&
	{
		w /= T(val);
		x /= T(val);
		y /= T(val);
		z /= T(val);
		return *this;
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::operator[](usize idx) const noexcept -> T
	{
		MATH_ASSERT(idx < 4, "Index out of range");
		return data[idx];
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::operator[](usize idx) noexcept -> T&
	{
		MATH_ASSERT(idx < 4, "Index out of range");
		return data[idx];
	}

	template <Numeric T>
	template <ConvertableTo<T> U>
	constexpr auto Quaternion<T>::operator==(const Quaternion<U>& other) const noexcept -> bool
	{
		return w == other.w && x == other.x && y == other.y && z == other.z;
	}

	template <Numeric T>
	template <ConvertableTo<T> U>
	constexpr auto Quaternion<T>::operator!=(const Quaternion<U>& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::Compare(const Quaternion& other, T e) const noexcept -> bool
	{
		return Math::EpsilonCompare(w, other.w, e) && Math::EpsilonCompare(x, other.x, e) && Math::EpsilonCompare(y, other.y, e) && Math::EpsilonCompare(z, other.z, e);
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::IsIdentity(T e) const noexcept -> bool
	{
		return Math::EpsilonCompare(w, 1, e) && Math::EpsilonCompare(x, 0, e) && Math::EpsilonCompare(y, 0, e) && Math::EpsilonCompare(z, 0, e);
	}

	template <Numeric T>
	template <ConvertableTo<T> I>
	constexpr auto Quaternion<T>::LerpFullPath(const Quaternion& other, I interpolant) const noexcept -> Quaternion
	{
		MATH_ASSERT(interpolant >= 0 && interpolant <= 1, "interpolant needs to be in the range [0;1]");

		T i = T(interpolant);
		return *this * (T(1) - i) + other * i;
	}

	template <Numeric T>
	template <ConvertableTo<T> I>
	constexpr auto Quaternion<T>::Lerp(const Quaternion& other, I interpolant) const noexcept -> Quaternion
	{
		MATH_ASSERT(interpolant >= 0 && interpolant <= 1, "interpolant needs to be in the range [0;1]");

		T cos = Dot(other);
		return cos < 0 ? (-*this).LerpFullPath(other, interpolant) : LerpFullPath(other, interpolant);
	}

	template <Numeric T>
	template <ConvertableTo<T> I>
	constexpr auto Quaternion<T>::SlerpFullPath(const Quaternion& other, I interpolant) const noexcept -> Quaternion
	{
		MATH_ASSERT(IsNormalized(), "Quaternion needs to be normalized for SlerpFullPath");
		MATH_ASSERT(other.IsNormalized(), "Other quaternion needs to be normalized for SlerpFullPath");
		MATH_ASSERT(interpolant >= 0 && interpolant <= 1, "interpolant needs to be in the range [0;1]");

		T cos = Dot(other);

		// If angles are the same or the opposite, linearly interpolate in between them
		// Go via the shortest path to try and avoid the case there t = 0.5 results in a zero quaternion
		if (Math::EpsilonCompare(Abs(cos), 1))
			return cos < 0 ? (-*this).LerpFullPath(other, interpolant) : LerpFullPath(other, interpolant);

		auto angle = ACos(cos);
		T sin = Sqrt(T(1) - cos * cos);
		T sinI = Sin(T(interpolant) * angle);
		T sinI1 = Sin((1 - T(interpolant)) * angle);

		return (*this * sinI1 + other * sinI) * Rcp(sin);
	}

	template <Numeric T>
	template <ConvertableTo<T> I>
	constexpr auto Quaternion<T>::Slerp(const Quaternion& other, I interpolant) const noexcept -> Quaternion
	{
		MATH_ASSERT(IsNormalized(), "Quaternion needs to be normalized for Slerp");
		MATH_ASSERT(other.IsNormalized(), "Other quaternion needs to be normalized for Slerp");
		MATH_ASSERT(interpolant >= 0 && interpolant <= 1, "interpolant needs to be in the range [0;1]");

		T cos = Dot(other);

		// If angles are the same or the opposite, linearly interpolate in between them
		// Go via the shortest path to try and avoid the case there t = 0.5 results in a zero quaternion
		if (Math::EpsilonCompare(Abs(cos), 1))
			return cos < 0 ? (-*this).LerpFullPath(other, interpolant) : LerpFullPath(other, interpolant);

		Quaternion quatA = cos < 0 ? -*this : *this;
		Math::Angle<T, AngleKind::Radians> angle = ACos(cos);
		// TODO: Abs for angles
		T sin = Sin(Math::Angle<T, AngleKind::Radians>{ Abs(angle.val) });
		T sinI = Sin(T(interpolant) * angle);
		T sinI1 = Sin((1 - T(interpolant)) * angle);

		return (quatA * sinI1 + other * sinI) * Rcp(sin);
	}

	template <Numeric T>
	template <ConvertableTo<T> I>
	constexpr auto Quaternion<T>::SquadFullPath(const Quaternion& a, const Quaternion& q1, const Quaternion& b, I interpolant) const noexcept -> Quaternion
	{
		MATH_ASSERT(IsNormalized(), "Quaternion needs to be normalized for SquadFullPath");
		MATH_ASSERT(a.IsNormalized(), "Tangent of this quaternion needs to be normalized for SquadFullPath");
		MATH_ASSERT(q1.IsNormalized(), "Other quaternion needs to be normalized for SquadFullPath");
		MATH_ASSERT(b.IsNormalized(), "Tangent of other quaternion needs to be normalized for SquadFullPath");
		MATH_ASSERT(interpolant >= 0 && interpolant <= 1, "interpolant needs to be in the range [0;1]");

		T squadInterp = T(2) * T(interpolant) * (T(1) - T(interpolant));
		return SlerpFullPath(q1, interpolant).SlerpFullPath(a.SlerpFullPath(b, interpolant), squadInterp);
	}

	template <Numeric T>
	template <ConvertableTo<T> I>
	constexpr auto Quaternion<T>::Squad(const Quaternion& a, const Quaternion& q1, const Quaternion& b, I interpolant) const noexcept -> Quaternion
	{
		MATH_ASSERT(IsNormalized(), "Quaternion needs to be normalized for Squad");
		MATH_ASSERT(a.IsNormalized(), "Tangent of this quaternion needs to be normalized for Squad");
		MATH_ASSERT(q1.IsNormalized(), "Other quaternion needs to be normalized for Squad");
		MATH_ASSERT(b.IsNormalized(), "Tangent of other quaternion needs to be normalized for Squad");
		MATH_ASSERT(interpolant >= 0 && interpolant <= 1, "interpolant needs to be in the range [0;1]");

		T squadInterp = T(2) * T(interpolant) * (T(1) - T(interpolant));
		return Slerp(q1, interpolant).SlerpFullPath(a.SlerpFullPath(b, interpolant), squadInterp);
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::LenSq() const noexcept -> T
	{
		return w * w + x * x + y * y + z * z;
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::Len() const noexcept -> T
	{
		return Sqrt(LenSq());
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::Dot(const Quaternion& other) const noexcept -> T
	{
		return w * other.w + x * other.x + y * other.y + z * other.z;
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::Cross(const Quaternion& other) const noexcept -> Quaternion
	{
		return { T(0), imaginary.Cross(other.imaginary) };
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::Normalize() noexcept -> Quaternion&
	{
		*this *= RSqrt(LenSq());
		return *this;
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::Normalized() const noexcept -> Quaternion
	{
		return *this * RSqrt(LenSq());
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::IsNormalized(T e) const noexcept -> bool
	{
		return Math::EpsilonCompare(LenSq(), 1, e);
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::Invert() noexcept -> Quaternion&
	{
		T invLenSq = T(1) / LenSq();
		w = w * invLenSq;
		x = -x * invLenSq;
		y = -y * invLenSq;
		z = -z * invLenSq;
		return *this;
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::Inverse() const noexcept -> Quaternion
	{
		T invLenSq = T(1) / LenSq();
		return { w * invLenSq, -x * invLenSq, -y * invLenSq, -z * invLenSq };
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::Conjugate() noexcept -> Quaternion&
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::Conjugated() const noexcept -> Quaternion
	{
		return Quaternion{ w, -x, -y, -z };
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::Log() const noexcept -> Quaternion
	{
		Quaternion res = *this;
		res.w = 0.0f;

		Math::Angle<T, AngleKind::Radians> theta = ACos(w);
		T sinAngle = Sin(theta);

		if (Math::EpsilonCompare(sinAngle))
		{
			T scale = theta.val / sinAngle;
			res.x *= scale;
			res.y *= scale;
			res.z *= scale;
		}
		return res;
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::Exp() const noexcept -> Quaternion
	{
		Math::Angle<T, AngleKind::Radians> theta = imaginary.Len();
		return { Cos(theta), imaginary.Normalized() * Sin(theta) };
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::Angle() const noexcept -> Math::Angle<T, AngleKind::Radians>
	{
		return Math::Angle<T, AngleKind::Radians>{ ACos(w)* T(2) };
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::Angle(const Quaternion& other) const noexcept -> Math::Angle<T, AngleKind::Radians>
	{
		T angleY = (*this - other).Len();
		T angleX = (*this + other).Len();
		return T(2) * ATan2(angleY, angleX);
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::SwingTwist(const Vec3<T>& axis) const noexcept -> Pair<Quaternion, Quaternion>
	{
		Vec3<T> projection = axis.Dot(imaginary) * axis;

		Quaternion twist{ w, projection };

		if (Compare(twist.LenSq(), 0))
			twist = Quaternion{};
		else
			twist.Normalize();

		Quaternion swing = *this * twist.Invert();

		return { swing, twist };
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::TwistAngle(const Vec3<T>& axis) const noexcept -> Math::Angle<T, AngleKind::Radians>
	{
		T xyz = axis.Dot(imaginary);
		return Math::Angle<T, AngleKind::Radians>{ T(2)* ATan2(xyz, w) }.Mod();
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::ToEuler(EulerOrder order) const noexcept -> Math::EulerAngles<T, AngleKind::Radians>
	{
		Math::EulerAngles<T, AngleKind::Radians> euler;

		T ww = w * w;
		T xx = x * x;
		T yy = y * y;
		T zz = z * z;
		T test = x * w + y * z;

		// TODO: handle this for all cases
		if (test > T(0.5) - Consts::MathEpsilon<T>)
		{
			euler.yaw = T(2) * ATan2(y, x);
			euler.pitch = Consts::HalfPi<T>;
			euler.roll = 0;
			return euler;
		}
		else if (test < -T(0.5) + Consts::MathEpsilon<T>)
		{
			euler.yaw = T(2) * ATan2(y, x);
			euler.pitch = -Consts::HalfPi<T>;
			euler.roll = 0;
			return euler;
		}
		else
		{
			T yawY = T(2) * (x * w + y * z);
			T yawX = T(1) - T(2) * (z * z + w * w);
			euler.yaw = ATan2(yawY, yawX);
			euler.pitch = ASin(T(2) * (x * z - w * y));
			T rollY = T(2) * (x * y + z * w);
			T rollX = T(1) - T(2) * (y * y + z * z);
			euler.roll = ATan2(rollY, rollX);
		}

		switch (order)
		{
		case EulerOrder::XYZ:
		{
			euler.pitch = ATan2(-T(2) * (y * z - w * x), ww - xx - yy + zz);
			euler.yaw = ASin(T(2) * (x * z + w * y));
			euler.roll = ATan2(-T(2) * (x * y - w * z), ww + xx - yy - zz);
			break;
		}
		case EulerOrder::XZY:
		{
			euler.pitch = ATan2(T(2) * (y * z + w * x), ww - xx + yy - zz);
			euler.yaw = ATan2(T(2) * (x * z + w * y), ww + xx - yy - zz);
			euler.roll = ASin(-T(2) * (x * y - w * z));
			break;
		}
		case EulerOrder::YXZ:
		{
			euler.pitch = ASin(-T(2) * (y * z - w * x));
			euler.yaw = ATan2(T(2) * (x * z + w * y), ww - xx - yy + zz);
			euler.roll = ATan2(T(2) * (x * y + w * z), ww - xx + yy - zz);
			break;
		}
		case EulerOrder::YZX:
		{
			euler.pitch = ATan2(-T(2) * (y * z - w * x), ww - xx + yy - zz);
			euler.yaw = ATan2(-T(2) * (x * z - w * y), ww + xx - yy - zz);
			euler.roll = ASin(T(2) * (x * y + w * z));
			break;
		}
		case EulerOrder::ZXY:
		{
			euler.pitch = ASin(T(2) * (y * z + w * x));
			euler.yaw = ATan2(-T(2) * (x * z - w * y), ww - xx - yy + zz);
			euler.roll = ATan2(-T(2) * (x * y - w * z), ww - xx + yy - zz);
			break;
		}
		case EulerOrder::ZYX:
		{
			euler.pitch = ATan2(T(2) * (y * z + w * x), ww - xx - yy + zz);
			euler.yaw = ASin(-T(2) * (x * z - w * y));
			euler.roll = ATan2(T(2) * (x * y + w * z), ww + xx - yy - zz);
			break;
		}
		default: break;
		}

		return euler;
	}

	template <Numeric T>
	constexpr auto Quaternion<T>::ToAxisAngle() const noexcept -> Pair<Vec3<T>, Math::Angle<T, AngleKind::Radians>>
	{
		Vec3<T> axis = imaginary.Normalized();
		Math::Angle<T, AngleKind::Radians> angle = T(2) * ACos(w);
		return { axis, angle };
	}
	
	template <Numeric T>
	constexpr auto Quaternion<T>::CalculateInnerQuadrangle(const Quaternion& prev, const Quaternion& other, const Quaternion& next) const noexcept -> Pair<Quaternion, Quaternion>
	{
		Quaternion q0 = (prev  + *this).LenSq() < (prev  - *this).LenSq() ? -prev  : prev;
		Quaternion q2 = (*this + other).LenSq() < (*this - other).LenSq() ? -other : other;
		Quaternion q3 = (*this + next ).LenSq() < (*this - next ).LenSq() ? -next  : next;

		Quaternion q1Exp = Inverse();
		Quaternion q1q2 = (q1Exp * q2).Log();
		Quaternion q1q0 = (q1Exp * q0).Log();
		Quaternion a = *this * ( (q1q2 + q1q0) / -T(4) ).Exp();

		Quaternion q2Exp = q2.Inverse();
		Quaternion q2q3 = (q2Exp * q3   ).Log();
		Quaternion q2q1 = (q2Exp * *this).Log();
		Quaternion b = *this * ( (q2q3 + q2q1) / -T(4) ).Exp();

		return { a, b };
	}

	template <Numeric T>
	template<ConvertableTo<T> A, AngleKind Kind>
	constexpr auto Quaternion<T>::EulerAngles(const Math::EulerAngles<A, Kind>& euler, EulerOrder order) noexcept -> Quaternion
	{
		Math::Angle<T, AngleKind::Radians> pitch = euler.pitch / T(2);
		Math::Angle<T, AngleKind::Radians> yaw = euler.yaw / T(2);
		Math::Angle<T, AngleKind::Radians> roll = euler.roll / T(2);

		T sp = Sin(pitch);
		T cp = Cos(pitch);
		T sy = Sin(yaw);
		T cy = Cos(yaw);
		T sr = Sin(roll);
		T cr = Cos(roll);

		Quaternion quat;
		switch (order)
		{
		case EulerOrder::XYZ:
		{
			quat.w = cp * cy * cr - sp * sy * sr;
			quat.x = sp * cy * cr + cp * sy * sr;
			quat.y = cp * sy * cr - sp * cy * sr;
			quat.z = cp * cy * sr + sp * sy * cr;
			break;
		}
		case EulerOrder::XZY:
		{
			quat.w = cp * cy * cr + sp * sy * sr;
			quat.x = sp * cy * cr - cp * sy * sr;
			quat.y = cp * sy * cr - sp * cy * sr;
			quat.z = cp * cy * sr + sp * sy * cr;
			break;
		}
		case EulerOrder::YXZ:
		{
			quat.w = cp * cy * cr + sp * sy * sr;
			quat.x = sp * cy * cr + cp * sy * sr;
			quat.y = cp * sy * cr - sp * cy * sr;
			quat.z = cp * cy * sr - sp * sy * cr;
			break;
		}
		case EulerOrder::YZX:
		{
			quat.w = cp * cy * cr - sp * sy * sr;
			quat.x = sp * cy * cr + cp * sy * sr;
			quat.y = cp * sy * cr + sp * cy * sr;
			quat.z = cp * cy * sr - sp * sy * cr;
			break;
		}
		case EulerOrder::ZXY:
		{
			quat.w = cp * cy * cr - sp * sy * sr;
			quat.x = sp * cy * cr - cp * sy * sr;
			quat.y = cp * sy * cr + sp * cy * sr;
			quat.z = cp * cy * sr + sp * sy * cr;
			break;
		}
		case EulerOrder::ZYX:
		{
			quat.w = cp * cy * cr + sp * sy * sr;
			quat.x = sp * cy * cr - cp * sy * sr;
			quat.y = cp * sy * cr + sp * cy * sr;
			quat.z = cp * cy * sr - sp * sy * cr;
			break;
		}
		default: ;
		}
		
		return quat;
	}

	template<Numeric T>
	template<ConvertableTo<T> A, ConvertableTo<T> B, AngleKind Kind>
	constexpr auto Quaternion<T>::AxisAngle(const Vec3<A>& axis, const Math::Angle<B, Kind>& angle) noexcept -> Quaternion
	{
		Math::Angle<T, AngleKind::Radians> a = angle / 2;
		T sin = Sin(a);
		T cos = Cos(a);
		return Quaternion{ cos, axis.x * sin, axis.y * sin, axis.z * sin }.Normalize();
	}
}
