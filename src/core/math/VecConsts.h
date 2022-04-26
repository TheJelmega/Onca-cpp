#pragma once
#include "core/MinInclude.h"
#include "Concepts.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

namespace Onca::Math::Consts
{
	template <Numeric T>
	constexpr T XAxis = T(1);
	template <Numeric T>
	constexpr T YAxis = T(0);
	template <Numeric T>
	constexpr T ZAxis = T(0);
	template <Numeric T>
	constexpr T Right = T(1);
	template <Numeric T>
	constexpr T Left = T(-1);
	template <Numeric T>
	constexpr T Up = T(0);
	template <Numeric T>
	constexpr T Down = T(0);
	template <Numeric T>
	constexpr T Forward = T(0);
	template <Numeric T>
	constexpr T backward = T(0);

	template <Numeric T>
	constexpr Vec2<T> XAxis<Vec2<T>>    = {  1,  0 };
	template <Numeric T>
	constexpr Vec2<T> YAxis<Vec2<T>>    = {  0,  1 };
	template <Numeric T>
	constexpr Vec2<T> Right<Vec2<T>>    = {  1,  0 };
	template <Numeric T>
	constexpr Vec2<T> Left<Vec2<T>>     = { -1,  0 };
	template <Numeric T>
	constexpr Vec2<T> Up<Vec2<T>>       = {  0,  1 };
	template <Numeric T>
	constexpr Vec2<T> Down<Vec2<T>>     = {  0, -1 };
	template <Numeric T>
	constexpr Vec2<T> Forward<Vec2<T>>  = {  0,  0 };
	template <Numeric T>
	constexpr Vec2<T> backward<Vec2<T>> = {  0,  0 };

	template <Numeric T>
	constexpr Vec3<T> XAxis<Vec3<T>>    = {  1,  0,  0 };
	template <Numeric T>
	constexpr Vec3<T> YAxis<Vec3<T>>    = {  0,  1,  0 };
	template <Numeric T>
	constexpr Vec3<T> ZAxis<Vec3<T>>    = {  0,  0,  1 };
	template <Numeric T>
	constexpr Vec3<T> Right<Vec3<T>>    = {  1,  0,  0 };
	template <Numeric T>
	constexpr Vec3<T> Left<Vec3<T>>     = { -1,  0,  0 };
	template <Numeric T>
	constexpr Vec3<T> Up<Vec3<T>>       = {  0,  1,  0 };
	template <Numeric T>
	constexpr Vec3<T> Down<Vec3<T>>     = {  0, -1,  0 };
	template <Numeric T>
	constexpr Vec3<T> Forward<Vec3<T>>  = {  0,  0,  1 };
	template <Numeric T>
	constexpr Vec3<T> backward<Vec3<T>> = {  0,  0, -1 };

	template <Numeric T>
	constexpr Vec4<T> XAxis<Vec4<T>>    = {  1,  0,  0, 0};
	template <Numeric T>
	constexpr Vec4<T> YAxis<Vec4<T>>    = {  0,  1,  0, 0};
	template <Numeric T>
	constexpr Vec4<T> ZAxis<Vec4<T>>    = {  0,  0,  1, 0};
	template <Numeric T>
	constexpr Vec4<T> Right<Vec4<T>>    = {  1,  0,  0, 0 };
	template <Numeric T>
	constexpr Vec4<T> Left<Vec4<T>>     = { -1,  0,  0, 0 };
	template <Numeric T>
	constexpr Vec4<T> Up<Vec4<T>>       = {  0,  1,  0, 0 };
	template <Numeric T>
	constexpr Vec4<T> Down<Vec4<T>>     = {  0, -1,  0, 0 };
	template <Numeric T>
	constexpr Vec4<T> Forward<Vec4<T>>  = {  0,  0,  1, 0 };
	template <Numeric T>
	constexpr Vec4<T> backward<Vec4<T>> = {  0,  0, -1, 0 };
}
