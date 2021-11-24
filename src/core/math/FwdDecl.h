#pragma once
#include "core/MinInclude.h"
#include "Concepts.h"

namespace Core::Math
{
	enum class AngleKind
	{
		Radians,
		Degrees,
	};

	enum class EulerOrder
	{
		XYZ,
		XZY,
		YXZ,
		YZX,
		ZXY,
		ZYX
	};

	template<Numeric Ty, AngleKind A>
	struct Angle;

	template<Numeric Ty>
	struct Vec2;

	template<Numeric Ty>
	struct Vec3;

	template<Numeric Ty>
	struct Vec4;

	template<Numeric T>
	struct Quaternion;

	template<Numeric T>
	struct Mat2;

	template<Numeric T>
	struct Mat3;

	template<Numeric T>
	struct Mat43;

	template<Numeric T>
	struct Mat4;

	struct ColumnInitTag {};
	constexpr ColumnInitTag ColumnInit{};
}