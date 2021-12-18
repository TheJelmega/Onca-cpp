#pragma once
#include "core/MinInclude.h"

#ifndef MATH_DISABLE_SIMD
#	define MATH_DISABLE_SIMD 0
#endif

#include "Concepts.h"
#include "Constants.h"
#include "MathUtils.h"
#include "FloatUtils.h"
#include "IntUtils.h"
#include "core/intrin/MathUtils.h" // MathUtils overloads for Intrin::Pack<T, Width>

// headers
#include "Angle.h"
#include "Trigonometry.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Quaternion.h"
#include "Mat2.h"
#include "Mat3.h"
#include "Mat43.h"
#include "Mat4.h"

#include "Ray.h"
#include "Rect.h"
#include "AABB.h"
#include "Circle.h"
#include "Sphere.h"
#include "Plane.h"

// impls
#include "Angle.inl"
#include "Trigonometry.inl"
#include "Vec2.inl"
#include "Vec3.inl"
#include "Vec4.inl"

#include "VecConsts.h"

#include "Quaternion.inl"
#include "Mat2.inl"
#include "Mat3.inl"
#include "Mat43.inl"
#include "Mat4.inl"

#include "Ray.inl"
#include "Rect.inl"
#include "AABB.inl"
#include "Circle.inl"
#include "Sphere.inl"
#include "Plane.inl"

// Angle types
namespace Core::Math
{
	template<Numeric T>
	using Radians = Angle<T, AngleKind::Radians>;
	template<Numeric T>
	using Degrees = Angle<T, AngleKind::Degrees>;
}

using f32rad = Core::Math::Radians<f32>;
using f64rad = Core::Math::Radians<f64>;
using f32deg = Core::Math::Degrees<f32>;
using f64deg = Core::Math::Degrees<f64>;

// Vec2 types
using i8v2  = Core::Math::Vec2<i8>;
using i16v2 = Core::Math::Vec2<i16>;
using i32v2 = Core::Math::Vec2<i32>;
using i64v2 = Core::Math::Vec2<i64>;
using u8v2  = Core::Math::Vec2<u8>;
using u16v2 = Core::Math::Vec2<u16>;
using u32v2 = Core::Math::Vec2<u32>;
using u64v2 = Core::Math::Vec2<u64>;
using f32v2 = Core::Math::Vec2<f32>;
using f64v2 = Core::Math::Vec2<f64>;

// Vec3 types
using i8v3  = Core::Math::Vec3<i8>;
using i16v3 = Core::Math::Vec3<i16>;
using i32v3 = Core::Math::Vec3<i32>;
using i64v3 = Core::Math::Vec3<i64>;
using u8v3  = Core::Math::Vec3<u8>;
using u16v3 = Core::Math::Vec3<u16>;
using u32v3 = Core::Math::Vec3<u32>;
using u64v3 = Core::Math::Vec3<u64>;
using f32v3 = Core::Math::Vec3<f32>;
using f64v3 = Core::Math::Vec3<f64>;

// Vec4 types
using i8v4 = Core::Math::Vec4<i8>;
using i16v4 = Core::Math::Vec4<i16>;
using i32v4 = Core::Math::Vec4<i32>;
using i64v4 = Core::Math::Vec4<i64>;
using u8v4 = Core::Math::Vec4<u8>;
using u16v4 = Core::Math::Vec4<u16>;
using u32v4 = Core::Math::Vec4<u32>;
using u64v4 = Core::Math::Vec4<u64>;
using f32v4 = Core::Math::Vec4<f32>;
using f64v4 = Core::Math::Vec4<f64>;

// Point2 (Vec2) types
using i8p2  = i8v2;
using i16p2 = i16v2;
using i32p2 = i32v2;
using i64p2 = i64v2;
using u8p2  = u8v2;
using u16p2 = u16v2;
using u32p2 = u32v2;
using u64p2 = u64v2;
using f32p2 = f32v2;
using f64p2 = f64v2;

// Point3 (Vec3) types
using i8p3  = i8v3;
using i16p3 = i16v3;
using i32p3 = i32v3;
using i64p3 = i64v3;
using u8p3  = u8v3;
using u16p3 = u16v3;
using u32p3 = u32v3;
using u64p3 = u64v3;
using f32p3 = f32v3;
using f64p3 = f64v3;

// Point3 (Vec3) types
using i8p4 = i8v4;
using i16p4 = i16v4;
using i32p4 = i32v4;
using i64p4 = i64v4;
using u8p4 = u8v4;
using u16p4 = u16v4;
using u32p4 = u32v4;
using u64p4 = u64v4;
using f32p4 = f32v4;
using f64p4 = f64v4;

// Quaternion types
using f64q = Core::Math::Quaternion<f64>;
using f32q = Core::Math::Quaternion<f32>;

// Mat2 types
using i8m2  = Core::Math::Mat2<i8>;
using i16m2 = Core::Math::Mat2<i16>;
using i32m2 = Core::Math::Mat2<i32>;
using i64m2 = Core::Math::Mat2<i64>;
using u8m2  = Core::Math::Mat2<u8>;
using u16m2 = Core::Math::Mat2<u16>;
using u32m2 = Core::Math::Mat2<u32>;
using u64m2 = Core::Math::Mat2<u64>;
using f32m2 = Core::Math::Mat2<f32>;
using f64m2 = Core::Math::Mat2<f64>;

// Mat3 types
using i8m3  = Core::Math::Mat3<i8>;
using i16m3 = Core::Math::Mat3<i16>;
using i32m3 = Core::Math::Mat3<i32>;
using i64m3 = Core::Math::Mat3<i64>;
using u8m3  = Core::Math::Mat3<u8>;
using u16m3 = Core::Math::Mat3<u16>;
using u32m3 = Core::Math::Mat3<u32>;
using u64m3 = Core::Math::Mat3<u64>;
using f32m3 = Core::Math::Mat3<f32>;
using f64m3 = Core::Math::Mat3<f64>;

// Mat43 types
using i8m43 = Core::Math::Mat43<i8>;
using i16m43 = Core::Math::Mat43<i16>;
using i32m43 = Core::Math::Mat43<i32>;
using i64m43 = Core::Math::Mat43<i64>;
using u8m43 = Core::Math::Mat43<u8>;
using u16m43 = Core::Math::Mat43<u16>;
using u32m43 = Core::Math::Mat43<u32>;
using u64m43 = Core::Math::Mat43<u64>;
using f32m43 = Core::Math::Mat43<f32>;
using f64m43 = Core::Math::Mat43<f64>;

// Mat4 types
using i8m4  = Core::Math::Mat4<i8>;
using i16m4 = Core::Math::Mat4<i16>;
using i32m4 = Core::Math::Mat4<i32>;
using i64m4 = Core::Math::Mat4<i64>;
using u8m4  = Core::Math::Mat4<u8>;
using u16m4 = Core::Math::Mat4<u16>;
using u32m4 = Core::Math::Mat4<u32>;
using u64m4 = Core::Math::Mat4<u64>;
using f32m4 = Core::Math::Mat4<f32>;
using f64m4 = Core::Math::Mat4<f64>;