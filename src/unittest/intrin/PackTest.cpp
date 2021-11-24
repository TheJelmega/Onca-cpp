
/**
 * File needs to be recompiled for test using different intrinsics. It was tried to make a unittest per intrinsic,
 * but seemingly because of a quirk with MSVC, DISABLE_... did not work, as the preprocessed headers were seemingly cached
 */

#define SSE_VER 3

#if 1
#define DISABLE_SSE_SUPPORT (SSE_VER < 1)
#define DISABLE_AVX         (SSE_VER < 2)
#define DISABLE_AVX2        (SSE_VER < 3)
#endif

#include "gtest/gtest.h"
#include "core/Core.h"

namespace Intrin = Core::Intrin;

template<Intrin::SimdBaseType T, usize Width>
auto ZeroAndStore() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	PackT pack = PackT::Zero();
	T res[Width];
	pack.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], 0);
}

TEST(IntrinPack, ZeroAndStore)
{
	ZeroAndStore<f64, 2>();
	ZeroAndStore<f64, 4>();
	//ZeroAndStore<f64, 8>();

	ZeroAndStore<f32, 4>();
	ZeroAndStore<f32, 8>();
	//ZeroAndStore<f32, 16>();

	ZeroAndStore<u64, 2>();
	ZeroAndStore<u64, 4>();
	//ZeroAndStore<u64, 8>();

	ZeroAndStore<u32, 4>();
	ZeroAndStore<u32, 8>();
	//ZeroAndStore<u32, 16>();

	ZeroAndStore<u16, 8>();
	ZeroAndStore<u16, 16>();
	//ZeroAndStore<u16, 32>();

	ZeroAndStore<u8, 16>();
	ZeroAndStore<u8, 32>();
	//ZeroAndStore<u8, 64>();

	ZeroAndStore<i64, 2>();
	ZeroAndStore<i64, 4>();
	//ZeroAndStore<i64, 8>();
	
	ZeroAndStore<i32, 4>();
	ZeroAndStore<i32, 8>();
	//ZeroAndStore<i32, 16>();

	ZeroAndStore<i16, 8>();
	ZeroAndStore<i16, 16>();
	//ZeroAndStore<i16, 32>();

	ZeroAndStore<i8, 16>();
	ZeroAndStore<i8, 32>();
	//ZeroAndStore<i8, 64>();
}

template<Intrin::SimdBaseType T, usize Width>
auto OnesAndStore() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	PackT pack = PackT::Ones();
	T res[Width];
	pack.Store(res);

	u64 ones = 0xFFFF'FFFF'FFFF'FFFF;

	using Unsigned = Core::UnsignedOfSameSize<T>;
	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(*reinterpret_cast<Unsigned*>(&res[i]), Unsigned(ones));
}

TEST(IntrinPack, OnesAndStore)
{
	OnesAndStore<f64, 2>();
	OnesAndStore<f64, 4>();
	//OnesAndStore<f64, 8>();

	OnesAndStore<f32, 4>();
	OnesAndStore<f32, 8>();
	//OnesAndStore<f32, 16>();

	OnesAndStore<u64, 2>();
	OnesAndStore<u64, 4>();
	//OnesAndStore<u64, 8>();

	OnesAndStore<u32, 4>();
	OnesAndStore<u32, 8>();
	//OnesAndStore<u32, 16>();

	OnesAndStore<u16, 8>();
	OnesAndStore<u16, 16>();
	//OnesAndStore<u16, 32>();

	OnesAndStore<u8, 16>();
	OnesAndStore<u8, 32>();
	//OnesAndStore<u8, 64>();

	OnesAndStore<i64, 2>();
	OnesAndStore<i64, 4>();
	//OnesAndStore<i64, 8>();

	OnesAndStore<i32, 4>();
	OnesAndStore<i32, 8>();
	//OnesAndStore<i32, 16>();

	OnesAndStore<i16, 8>();
	OnesAndStore<i16, 16>();
	//OnesAndStore<i16, 32>();

	OnesAndStore<i8, 16>();
	OnesAndStore<i8, 32>();
	//OnesAndStore<i8, 64>();
}

template<Intrin::SimdBaseType T, usize Width>
auto Set1AndStore()
{
	using PackT = Intrin::Pack<T, Width>;
	PackT pack = PackT::Set(42);
	T res[Width];
	pack.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], 42);
}

TEST(IntrinPack, Set1AndStore)
{
	Set1AndStore<f64, 2>();
	Set1AndStore<f64, 4>();
	//Set1AndStore<f64, 8>();

	Set1AndStore<f32, 4>();
	Set1AndStore<f32, 8>();
	//Set1AndStore<f32, 16>();

	Set1AndStore<u64, 2>();
	Set1AndStore<u64, 4>();
	//Set1AndStore<u64, 8>();

	Set1AndStore<u32, 4>();
	Set1AndStore<u32, 8>();
	//Set1AndStore<u32, 16>();

	Set1AndStore<u16, 8>();
	Set1AndStore<u16, 16>();
	//Set1AndStore<u16, 32>();

	Set1AndStore<u8, 16>();
	Set1AndStore<u8, 32>();
	//Set1AndStore<u8, 64>();

	Set1AndStore<i64, 2>();
	Set1AndStore<i64, 4>();
	//Set1AndStore<i64, 8>();

	Set1AndStore<i32, 4>();
	Set1AndStore<i32, 8>();
	//Set1AndStore<i32, 16>();

	Set1AndStore<i16, 8>();
	Set1AndStore<i16, 16>();
	//Set1AndStore<i16, 32>();

	Set1AndStore<i8, 16>();
	Set1AndStore<i8, 32>();
	//Set1AndStore<i8, 64>();
}

template<Intrin::SimdBaseType T, usize Width, Core::ConvertableTo<T>... Args>
auto SetAndStore(Args... args)
{
	using PackT = Intrin::Pack<T, Width>;
	PackT pack = PackT::Set(T(args)...);
	T res[Width];
	pack.Store(res);

	T expected[Width] = { T(args)... };
	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], expected[i]);
}

TEST(IntrinPack, SetAndStore)
{
	SetAndStore<f64, 2>(1.0, 2.0);
	SetAndStore<f64, 4>(1.0, 2.0, 3.0, 4.0);
	//SetAndStore<f64, 8>(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0);

	SetAndStore<f32, 4>(1.0f, 2.0f, 3.0f, 4.0f);
	SetAndStore<f32, 8>(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
	//SetAndStore<f32, 16>(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f);

	SetAndStore<u64, 2>(1, 2);
	SetAndStore<u64, 4>(1, 2, 3, 4);
	//SetAndStore<u64, 8>(1, 2, 3, 4, 5, 6, 7, 8);

	SetAndStore<u32, 4>(1, 2, 3, 4);
	SetAndStore<u32, 8>(1, 2, 3, 4, 5, 6, 7, 8);
	//SetAndStore<u32, 16>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

	SetAndStore<u16, 8>(1, 2, 3, 4, 5, 6, 7, 8);
	SetAndStore<u16, 16>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	//SetAndStore<u16, 32>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);

	SetAndStore<u8, 16>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	SetAndStore<u8, 32>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
	//SetAndStore<u8, 64>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	//                    33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64);

	SetAndStore<i64, 2>(1, 2);
	SetAndStore<i64, 4>(1, 2, 3, 4);
	//SetAndStore<i64, 8>(1, 2, 3, 4, 5, 6, 7, 8);

	SetAndStore<i32, 4>(1, 2, 3, 4);
	SetAndStore<i32, 8>(1, 2, 3, 4, 5, 6, 7, 8);
	//SetAndStore<i32, 16>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

	SetAndStore<i16, 8>(1, 2, 3, 4, 5, 6, 7, 8);
	SetAndStore<i16, 16>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	//SetAndStore<i16, 32>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);

	SetAndStore<i8, 16>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	SetAndStore<i8, 32>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
	//SetAndStore<i8, 64>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	//                    33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64);
}

template<Intrin::SimdBaseType T, usize Width, Core::ConvertableTo<T>... Args>
auto SetRAndStore(Args... args)
{
	using PackT = Intrin::Pack<T, Width>;
	PackT pack = PackT::SetR(T(args)...);
	T res[Width];
	pack.Store(res);

	T expected[Width] = { T(args)... };
	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], expected[Width - i - 1]);
}

TEST(IntrinPack, SetRAndStore)
{
	SetRAndStore<f64, 2>(1.0, 2.0);
	SetRAndStore<f64, 4>(1.0, 2.0, 3.0, 4.0);
	//SetRAndStore<f64, 8>(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0);

	SetRAndStore<f32, 4>(1.0f, 2.0f, 3.0f, 4.0f);
	SetRAndStore<f32, 8>(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
	//SetRAndStore<f32, 16>(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f);

	SetRAndStore<u64, 2>(1, 2);
	SetRAndStore<u64, 4>(1, 2, 3, 4);
	//SetRAndStore<u64, 8>(1, 2, 3, 4, 5, 6, 7, 8);

	SetRAndStore<u32, 4>(1, 2, 3, 4);
	SetRAndStore<u32, 8>(1, 2, 3, 4, 5, 6, 7, 8);
	//SetRAndStore<u32, 16>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

	SetRAndStore<u16, 8>(1, 2, 3, 4, 5, 6, 7, 8);
	SetRAndStore<u16, 16>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	//SetRAndStore<u16, 32>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);

	SetRAndStore<u8, 16>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	SetRAndStore<u8, 32>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
	//SetRAndStore<u8, 64>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	//                    33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64);

	SetRAndStore<i64, 2>(1, 2);
	SetRAndStore<i64, 4>(1, 2, 3, 4);
	//SetRAndStore<i64, 8>(1, 2, 3, 4, 5, 6, 7, 8);

	SetRAndStore<i32, 4>(1, 2, 3, 4);
	SetRAndStore<i32, 8>(1, 2, 3, 4, 5, 6, 7, 8);
	//SetRAndStore<i32, 16>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

	SetRAndStore<i16, 8>(1, 2, 3, 4, 5, 6, 7, 8);
	SetRAndStore<i16, 16>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	//SetRAndStore<i16, 32>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);

	SetRAndStore<i8, 16>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	SetRAndStore<i8, 32>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
	//SetRAndStore<i8, 64>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	//                    33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64);
}

template<Intrin::SimdBaseType T, usize Width>
auto LoadAndStore()
{
	using PackT = Intrin::Pack<T, Width>;
	T src[Width];
	for (usize i = 0; i < Width; ++i)
		src[i] = T(i);

	PackT pack = PackT::Load(src);
	T res[Width];
	pack.Store(res);
	
	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], src[i]);
}

TEST(IntrinPack, LoadAndStoreTest)
{
	LoadAndStore<f64, 2>();
	LoadAndStore<f64, 4>();
	//LoadAndStore<f64, 8>();

	LoadAndStore<f32, 4>();
	LoadAndStore<f32, 8>();
	//LoadAndStore<f32, 16>();
	
	LoadAndStore<u64, 2>();
	LoadAndStore<u64, 4>();
	//LoadAndStore<u64, 8>();
	LoadAndStore<u32, 4>();
	LoadAndStore<u32, 8>();
	//LoadAndStore<u32, 16>();
	LoadAndStore<u16, 8>();
	LoadAndStore<u16, 16>();
	//LoadAndStore<u16, 32>();
	LoadAndStore<u8, 16>();
	LoadAndStore<u8, 32>();
	//LoadAndStore<u8, 64>();

	LoadAndStore<i64, 2>();
	LoadAndStore<i64, 4>();
	//LoadAndStore<i64, 8>();
	LoadAndStore<i32, 4>();
	LoadAndStore<i32, 8>();
	//LoadAndStore<i32, 16>();
	LoadAndStore<i16, 8>();
	LoadAndStore<i16, 16>();
	//LoadAndStore<i16, 32>();
	LoadAndStore<i8, 16>();
	LoadAndStore<i8, 32>();
	//LoadAndStore<i8, 64>();
}

template<Intrin::SimdBaseType T, usize Width>
auto AlignedLoadAndStore()
{
	using PackT = Intrin::Pack<T, Width>;
	alignas(PackT::Align) T src[Width];
	for (usize i = 0; i < Width; ++i)
		src[i] = T(i);

	PackT pack = PackT::AlignedLoad(src);
	alignas(PackT::Align) T res[Width];
	pack.AlignedStore(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], src[i]);
}

TEST(IntrinPack, AlignedLoadAndStore)
{
	AlignedLoadAndStore<f64, 2>();
	AlignedLoadAndStore<f64, 4>();
	//AlignedLoadAndStore<f64, 8>();

	AlignedLoadAndStore<f32, 4>();
	AlignedLoadAndStore<f32, 8>();
	//AlignedLoadAndStore<f32, 16>();

	AlignedLoadAndStore<u64, 2>();
	AlignedLoadAndStore<u64, 4>();
	//AlignedLoadAndStore<u64, 8>();
	AlignedLoadAndStore<u32, 4>();
	AlignedLoadAndStore<u32, 8>();
	//AlignedLoadAndStore<u32, 16>();
	AlignedLoadAndStore<u16, 8>();
	AlignedLoadAndStore<u16, 16>();
	//AlignedLoadAndStore<u16, 32>();
	AlignedLoadAndStore<u8, 16>();
	AlignedLoadAndStore<u8, 32>();
	//AlignedLoadAndStore<u8, 64>();

	AlignedLoadAndStore<i64, 2>();
	AlignedLoadAndStore<i64, 4>();
	//AlignedLoadAndStore<i64, 8>();
	AlignedLoadAndStore<i32, 4>();
	AlignedLoadAndStore<i32, 8>();
	//AlignedLoadAndStore<i32, 16>();
	AlignedLoadAndStore<i16, 8>();
	AlignedLoadAndStore<i16, 16>();
	//AlignedLoadAndStore<i16, 32>();
	AlignedLoadAndStore<i8, 16>();
	AlignedLoadAndStore<i8, 32>();
	//AlignedLoadAndStore<i8, 64>();
}

template<Intrin::SimdBaseType T, usize Width>
auto Insert()
{
	using PackT = Intrin::Pack<T, Width>;
	alignas(PackT::Align) T src[Width];
	for (usize i = 0; i < Width; ++i)
		src[i] = T(i);
	PackT pack = PackT::AlignedLoad(src);

	constexpr usize Idx = Width / 2 - 1;
	PackT packRes = pack.template Insert<Idx>(T(137));

	alignas(PackT::Align) T res[Width];
	packRes.AlignedStore(res);

	ASSERT_EQ(res[Idx], T(137));
}

TEST(IntrinPack, Insert)
{
	Insert<f64, 2>();
	Insert<f64, 4>();
	//Insert<f64, 8>();

	Insert<f32, 4>();
	Insert<f32, 8>();
	//Insert<f32, 16>();

	Insert<u64, 2>();
	Insert<u64, 4>();
	//Insert<u64, 8>();
	Insert<u32, 4>();
	Insert<u32, 8>();
	//Insert<u32, 16>();
	Insert<u16, 8>();
	Insert<u16, 16>();
	//Insert<u16, 32>();
	Insert<u8, 16>();
	Insert<u8, 32>();
	//Insert<u8, 64>();

	Insert<i64, 2>();
	Insert<i64, 4>();
	//Insert<i64, 8>();
	Insert<i32, 4>();
	Insert<i32, 8>();
	//Insert<i32, 16>();
	Insert<i16, 8>();
	Insert<i16, 16>();
	//Insert<i16, 32>();
	Insert<i8, 16>();
	Insert<i8, 32>();
	//Insert<i8, 64>();
}

template<Intrin::SimdBaseType T, usize Width>
auto Extract()
{
	using PackT = Intrin::Pack<T, Width>;
	alignas(PackT::Align) T src[Width];
	for (usize i = 0; i < Width; ++i)
		src[i] = T(i);
	constexpr usize Idx = Width / 2 - 1;
	src[Idx] = T(137);
	PackT pack = PackT::AlignedLoad(src);

	T val = pack.template Extract<Idx>();

	ASSERT_EQ(val, T(137));
}

TEST(IntrinPack, Extract)
{
	Extract<f64, 2>();
	Extract<f64, 4>();
	//Extract<f64, 8>();

	Extract<f32, 4>();
	Extract<f32, 8>();
	//Extract<f32, 16>();

	Extract<u64, 2>();
	Extract<u64, 4>();
	//Extract<u64, 8>();
	Extract<u32, 4>();
	Extract<u32, 8>();
	//Extract<u32, 16>();
	Extract<u16, 8>();
	Extract<u16, 16>();
	//Extract<u16, 32>();
	Extract<u8, 16>();
	Extract<u8, 32>();
	//Extract<u8, 64>();

	Extract<i64, 2>();
	Extract<i64, 4>();
	//Extract<i64, 8>();
	Extract<i32, 4>();
	Extract<i32, 8>();
	//Extract<i32, 16>();
	Extract<i16, 8>();
	Extract<i16, 16>();
	//Extract<i16, 32>();
	Extract<i8, 16>();
	Extract<i8, 32>();
	//Extract<i8, 64>();
}

template<Intrin::ComparisonOp Op, Intrin::SimdBaseType T, usize Width>
auto Compare(InitializerList<T> a, InitializerList<T> b, InitializerList<Core::UnsignedOfSameSize<T>> r) -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T data[Width];
	Core::MemCpy(data, a.begin(), a.size() * sizeof(T));
	PackT packA = PackT::Load(data);
	Core::MemCpy(data, b.begin(), b.size() * sizeof(T));
	PackT packB = PackT::Load(data);

	PackT packRes = packA.template Compare<Op>(packB);
	T res[Width];
	packRes.Store(res);

	Core::UnsignedOfSameSize<T> expected[Width];
	Core::MemCpy(expected, r.begin(), r.size() * sizeof(T));

	using USizeT = Core::UnsignedOfSameSize<T>;
	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(*reinterpret_cast<USizeT*>(&res[i]), expected[i]);
}

TEST(IntrinPack, CmpEqTest)
{
	u64 Ones64 = 0xFFFF'FFFF'FFFF'FFFF;
	u32 Ones32 = 0xFFFF'FFFF;

	Compare<Intrin::ComparisonOp::Eq, f64, 2>({ 1, 2 }, { 1, 0 }, { Ones64, 0 });
	Compare<Intrin::ComparisonOp::Eq, f64, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { Ones64, 0, 0, Ones64 });
	//Compare<Intrin::ComparisonOp::Eq, f64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { Ones64, 0, 0, Ones64, Ones64, Ones64, 0, 0 });

	Compare<Intrin::ComparisonOp::Eq, f32, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { Ones32, 0, 0, Ones32 });
	Compare<Intrin::ComparisonOp::Eq, f32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0 });
	//Compare<Intrin::ComparisonOp::Eq, f32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 }, 
	//										   { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0, 0, 0, Ones32, Ones32, 0, Ones32, Ones32, 0 });

	Compare<Intrin::ComparisonOp::Eq, u64, 2>({ 1, 2 }, { 1, 0 }, { Ones64, 0 });
	Compare<Intrin::ComparisonOp::Eq, u64, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { Ones64, 0, 0, Ones64 });
	//Compare<Intrin::ComparisonOp::Eq, u64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { Ones64, 0, 0, Ones64, Ones64, Ones64, 0, 0 });

	Compare<Intrin::ComparisonOp::Eq, u32, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { Ones32, 0, 0, Ones32 });
	Compare<Intrin::ComparisonOp::Eq, u32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0 });
	//Compare<Intrin::ComparisonOp::Eq, u32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 }, 
	//										   { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0, 0, 0, Ones32, Ones32, 0, Ones32, Ones32, 0 });



	Compare<Intrin::ComparisonOp::Eq, u16, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0 });
	Compare<Intrin::ComparisonOp::Eq, u16, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 },
											   { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF, 0, 0xFFFF, 0xFFFF, 0 });
	//Compare<Intrin::ComparisonOp::Eq, u16, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0, 0 , 18, 19, 0 , 21, 22, 0 , 0 , 0 , 0 , 27, 28, 29, 0 , 0 , 32 },
	//										   { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF, 0, 0xFFFF, 0xFFFF, 0,
	//										   	 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0,  0xFFFF, 0, 0, 0xFFFF });

	Compare<Intrin::ComparisonOp::Eq, u8, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											  { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 },
											  { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0 });
	Compare<Intrin::ComparisonOp::Eq, u8, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
											  { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0, 0 , 18, 19, 0 , 21, 22, 0 , 0 , 0 , 0 , 27, 28, 29, 0 , 0 , 32 },
											  { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
												 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF });
	//Compare<Intrin::ComparisonOp::Eq, u8, 64>({ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	//                                            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64 },
	//										    { 1 , 0 , 0 , 4 , 5 , 6 , 0 , 0 , 0 , 0 , 11, 12, 0 , 14, 15 , 0, 17, 0 , 0 , 20, 21, 22, 0 , 0 , 0 , 0 , 27, 28, 0 , 30, 31, 0 ,
	//										      0 , 34, 35, 0 , 0 , 0 , 39, 40, 41, 42, 0 , 0 , 45, 0 , 0 , 48, 0 , 50, 51, 0 , 0 , 0 , 55, 56, 57, 58, 0 , 0 , 61, 0 , 0 , 64 },
	//										    { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
	//											  0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
	//										   	  0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0,  0xFF, 0, 0, 0xFF,
	//										      0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0,  0xFF, 0, 0, 0xFF });

	Compare<Intrin::ComparisonOp::Eq, i64, 2>({ 1, 2 }, { 1, 0 }, { Ones64, 0 });
	Compare<Intrin::ComparisonOp::Eq, i64, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { Ones64, 0, 0, Ones64 });
	//Compare<Intrin::ComparisonOp::Eq, i64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { Ones64, 0, 0, Ones64, Ones64, Ones64, 0, 0 });

	Compare<Intrin::ComparisonOp::Eq, i32, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { Ones32, 0, 0, Ones32 });
	Compare<Intrin::ComparisonOp::Eq, i32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0 });
	//Compare<Intrin::ComparisonOp::Eq, i32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 }, 
	//										   { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0, 0, 0, Ones32, Ones32, 0, Ones32, Ones32, 0 });



	Compare<Intrin::ComparisonOp::Eq, i16, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0 });
	Compare<Intrin::ComparisonOp::Eq, i16, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 },
											   { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF, 0, 0xFFFF, 0xFFFF, 0 });
	//Compare<Intrin::ComparisonOp::Eq, i16, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
	//										     { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0, 0 , 18, 19, 0 , 21, 22, 0 , 0 , 0 , 0 , 27, 28, 29, 0 , 0 , 32 },
	//										     { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF, 0, 0xFFFF, 0xFFFF, 0,
	//										       0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0,  0xFFFF, 0, 0, 0xFFFF });

	Compare<Intrin::ComparisonOp::Eq, i8, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											  { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 },
											  { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0 });
	Compare<Intrin::ComparisonOp::Eq, i8, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
											  { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0, 0 , 18, 19, 0 , 21, 22, 0 , 0 , 0 , 0 , 27, 28, 29, 0 , 0 , 32 },
											  { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
												 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF });
	//Compare<Intrin::ComparisonOp::Eq, i8, 64>({ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	//                                            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64 },
	//										    { 1 , 0 , 0 , 4 , 5 , 6 , 0 , 0 , 0 , 0 , 11, 12, 0 , 14, 15 , 0, 17, 0 , 0 , 20, 21, 22, 0 , 0 , 0 , 0 , 27, 28, 0 , 30, 31, 0 ,
	//										      0 , 34, 35, 0 , 0 , 0 , 39, 40, 41, 42, 0 , 0 , 45, 0 , 0 , 48, 0 , 50, 51, 0 , 0 , 0 , 55, 56, 57, 58, 0 , 0 , 61, 0 , 0 , 64 },
	//										    { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
	//											  0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
	//										   	  0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0,  0xFF, 0, 0, 0xFF,
	//										      0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0,  0xFF, 0, 0, 0xFF });
}

TEST(IntrinPack, CmpNEqTest)
{
	u64 Ones64 = 0xFFFF'FFFF'FFFF'FFFF;
	u32 Ones32 = 0xFFFF'FFFF;

	Compare<Intrin::ComparisonOp::NEq, f64, 2>({ 1, 2 }, { 1, 0 }, { 0, Ones64 });
	Compare<Intrin::ComparisonOp::NEq, f64, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { 0, Ones64, Ones64, 0 });
	//Compare<Intrin::ComparisonOp::NEq, f64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0, Ones64, Ones64, 0, 0, 0, Ones64, Ones64 });

	Compare<Intrin::ComparisonOp::NEq, f32, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { 0, Ones32, Ones32, 0 });
	Compare<Intrin::ComparisonOp::NEq, f32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32 });
	//Compare<Intrin::ComparisonOp::NEq, f32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 }, 
	//										   {  0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32, Ones32, Ones32, 0, 0, Ones32, 0, 0, Ones32 });
	
	Compare<Intrin::ComparisonOp::NEq, u64, 2>({ 1, 2 }, { 1, 0 }, { 0, Ones64 });
	Compare<Intrin::ComparisonOp::NEq, u64, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { 0, Ones64, Ones64, 0 });
	//Compare<Intrin::ComparisonOp::NEq, u64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0, Ones64, Ones64, 0, 0, 0, Ones64, Ones64 });
	
	Compare<Intrin::ComparisonOp::NEq, u32, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { 0, Ones32, Ones32, 0 });
	Compare<Intrin::ComparisonOp::NEq, u32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32 });
	//Compare<Intrin::ComparisonOp::NEq, u32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 }, 
	//										   { 0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32, Ones32, Ones32, 0, 0, Ones32, 0, 0, Ones32 });
	

	Compare<Intrin::ComparisonOp::NEq, u16, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF });
	Compare<Intrin::ComparisonOp::NEq, u16, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 },
											   { 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0xFFFF, 0, 0, 0xFFFF });
	//Compare<Intrin::ComparisonOp::NEq, u16, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0, 0 , 18, 19, 0 , 21, 22, 0 , 0 , 0 , 0 , 27, 28, 29, 0 , 0 , 32 },
	//										   { 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0xFFFF, 0, 0, 0xFFFF,
	//										   	 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF,  0, 0xFFFF, 0xFFFF, 0 });
	
	Compare<Intrin::ComparisonOp::NEq, u8, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											  { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 },
											  { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF });
	Compare<Intrin::ComparisonOp::NEq, u8, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
											  { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0, 0 , 18, 19, 0 , 21, 22, 0 , 0 , 0 , 0 , 27, 28, 29, 0 , 0 , 32 },
											  { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
												 0xFF, 0, 0, 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0  });
	//Compare<Intrin::ComparisonOp::NEq, u8, 64>({ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	//                                            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64 },
	//										    { 1 , 0 , 0 , 4 , 5 , 6 , 0 , 0 , 0 , 0 , 11, 12, 0 , 14, 15 , 0, 17, 0 , 0 , 20, 21, 22, 0 , 0 , 0 , 0 , 27, 28, 0 , 30, 31, 0 ,
	//										      0 , 34, 35, 0 , 0 , 0 , 39, 40, 41, 42, 0 , 0 , 45, 0 , 0 , 48, 0 , 50, 51, 0 , 0 , 0 , 55, 56, 57, 58, 0 , 0 , 61, 0 , 0 , 64 },
	//										    { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
	//											  0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
	//										   	  0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF,  0, 0xFF, 0xFF, 0,
	//										      0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF,  0, 0xFF, 0xFF, 0 });
	
	Compare<Intrin::ComparisonOp::NEq, i64, 2>({ 1, 2 }, { 1, 0 }, { 0, Ones64 });
	Compare<Intrin::ComparisonOp::NEq, i64, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { 0, Ones64, Ones64, 0 });
	//Compare<Intrin::ComparisonOp::NEq, i64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0, Ones64, Ones64, 0, 0, 0, Ones64, Ones64 });
	
	Compare<Intrin::ComparisonOp::NEq, i32, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { 0, Ones32, Ones32, 0 });
	Compare<Intrin::ComparisonOp::NEq, i32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32 });
	//Compare<Intrin::ComparisonOp::NEq, i32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 }, 
	//										   { 0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32, Ones32, Ones32, 0, 0, Ones32, 0, 0, Ones32 });
	
	
	
	Compare<Intrin::ComparisonOp::NEq, i16, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF });
	Compare<Intrin::ComparisonOp::NEq, i16, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
												{ 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 },
												{ 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0xFFFF, 0, 0, 0xFFFF });
	//Compare<Intrin::ComparisonOp::NEq, i16, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0, 0 , 18, 19, 0 , 21, 22, 0 , 0 , 0 , 0 , 27, 28, 29, 0 , 0 , 32 },
	//										   { 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0xFFFF, 0, 0, 0xFFFF,
	//										   	 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF,  0, 0xFFFF, 0xFFFF, 0 });
	
	Compare<Intrin::ComparisonOp::NEq, i8, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 },
											   { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF });
	Compare<Intrin::ComparisonOp::NEq, i8, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
											   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0, 0 , 18, 19, 0 , 21, 22, 0 , 0 , 0 , 0 , 27, 28, 29, 0 , 0 , 32 },
											   { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
												  0xFF, 0, 0, 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0 });
	//Compare<Intrin::ComparisonOp::NEq, i8, 64>({ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	//                                            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64 },
	//										    { 1 , 0 , 0 , 4 , 5 , 6 , 0 , 0 , 0 , 0 , 11, 12, 0 , 14, 15 , 0, 17, 0 , 0 , 20, 21, 22, 0 , 0 , 0 , 0 , 27, 28, 0 , 30, 31, 0 ,
	//										      0 , 34, 35, 0 , 0 , 0 , 39, 40, 41, 42, 0 , 0 , 45, 0 , 0 , 48, 0 , 50, 51, 0 , 0 , 0 , 55, 56, 57, 58, 0 , 0 , 61, 0 , 0 , 64 },
	//										    { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
	//											  0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
	//										   	  0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF,  0, 0xFF, 0xFF, 0,
	//										      0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF,  0, 0xFF, 0xFF, 0 });
}

TEST(IntrinPack, CmpLtTest)
{
	u64 Ones64 = 0xFFFF'FFFF'FFFF'FFFF;
	u32 Ones32 = 0xFFFF'FFFF;

	Compare<Intrin::ComparisonOp::Lt, f64, 2>({ 1, 2 }, { 1, 99 }, { 0, Ones64 });
	Compare<Intrin::ComparisonOp::Lt, f64, 4>({ 1, 2, 3, 4 }, { 1, 99, 99, 4 }, { 0, Ones64, Ones64, 0 });
	//Compare<Intrin::ComparisonOp::Lt, f64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 99, 99, 4, 5, 6, 99, 99 }, { 0, Ones64, Ones64, 0, 0, 0, Ones64, Ones64 });

	Compare<Intrin::ComparisonOp::Lt, f32, 4>({ 1, 2, 3, 4 }, { 1, 99, 99, 4 }, { 0, Ones32, Ones32, 0 });
	Compare<Intrin::ComparisonOp::Lt, f32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 99, 99, 4, 5, 6, 99, 99 }, { 0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32 });
	//Compare<Intrin::ComparisonOp::Lt, f32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99 }, 
	//										   {  0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32, Ones32, Ones32, 0, 0, Ones32, 0, 0, Ones32 });

	Compare<Intrin::ComparisonOp::Lt, u64, 2>({ 1, 2 }, { 1, 99 }, { 0, Ones64 });
	Compare<Intrin::ComparisonOp::Lt, u64, 4>({ 1, 2, 3, 4 }, { 1, 99, 99, 4 }, { 0, Ones64, Ones64, 0 });
	//Compare<Intrin::ComparisonOp::Lt, u64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 99, 99, 4, 5, 6, 99, 99 }, { 0, Ones64, Ones64, 0, 0, 0, Ones64, Ones64 });
	
	Compare<Intrin::ComparisonOp::Lt, u32, 4>({ 1, 2, 3, 4 }, { 1, 99, 99, 4 }, { 0, Ones32, Ones32, 0 });
	Compare<Intrin::ComparisonOp::Lt, u32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 99, 99, 4, 5, 6, 99, 99 }, { 0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32 });
	//Compare<Intrin::ComparisonOp::Lt, u32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99 }, 
	//										   { 0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32, Ones32, Ones32, 0, 0, Ones32, 0, 0, Ones32 });
	
	Compare<Intrin::ComparisonOp::Lt, u16, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 99, 99, 4, 5, 6, 99, 99 }, { 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF });
	Compare<Intrin::ComparisonOp::Lt, u16, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
												{ 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99 },
												{ 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0xFFFF, 0, 0, 0xFFFF });
	//Compare<Intrin::ComparisonOp::Lt, u16, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
	//										   { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99, 99 , 18, 19, 99 , 21, 22, 99 , 99 , 99 , 99 , 27, 28, 29, 99 , 99 , 32 },
	//										   { 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0xFFFF, 0, 0, 0xFFFF,
	//										   	 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF,  0, 0xFFFF, 0xFFFF, 0 });
	
	Compare<Intrin::ComparisonOp::Lt, u8, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											   { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99 },
											   { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF });
	Compare<Intrin::ComparisonOp::Lt, u8, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
											   { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99, 99 , 18, 19, 99 , 21, 22, 99 , 99 , 99 , 99 , 27, 28, 29, 99 , 99 , 32 },
											   { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
												  0xFF, 0, 0, 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0 });
	//Compare<Intrin::ComparisonOp::Lt, u8, 64>({ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	//                                            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64 },
	//										    { 1 , 99 , 99 , 4 , 5 , 6 , 99 , 99 , 99 , 99 , 11, 12, 99 , 14, 15 , 99, 17, 99 , 99 , 299, 21, 22, 99 , 99 , 99 , 99 , 27, 28, 99 ,399, /31, /99 ,
	//										      99 , 34, 35, 99 , 99 , 99 , 39, 499, 41, 42, 99 , 99 , 45, 99 , 99 , 48, 99 , 599, 51, 99 , 99 , 99 , 55, 56, 57, 58, 99 , 99 , 61,99 , /99 ,/ 64 },
	//										    { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
	//											  0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
	//										   	  0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF,  0, 0xFF, 0xFF, 0,
	//										      0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF,  0, 0xFF, 0xFF, 0 });

	Compare<Intrin::ComparisonOp::Lt, i64, 2>({ 1, 2 }, { 1, 99 }, { 0, Ones64 });
	Compare<Intrin::ComparisonOp::Lt, i64, 4>({ 1, 2, 3, 4 }, { 1, 99, 99, 4 }, { 0, Ones64, Ones64, 0 });
	//Compare<Intrin::ComparisonOp::Lt, i64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 99, 99, 4, 5, 6, 99, 99 }, { 0, Ones64, Ones64, 0, 0, 0, Ones64, Ones64 });
	
	Compare<Intrin::ComparisonOp::Lt, i32, 4>({ 1, 2, 3, 4 }, { 1, 99, 99, 4 }, { 0, Ones32, Ones32, 0 });
	Compare<Intrin::ComparisonOp::Lt, i32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 99, 99, 4, 5, 6, 99, 99 }, { 0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32 });
	//Compare<Intrin::ComparisonOp::Lt, i32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99 }, 
	//										   { 0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32, Ones32, Ones32, 0, 0, Ones32, 0, 0, Ones32 });
	
	
	
	Compare<Intrin::ComparisonOp::Lt, i16, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 99, 99, 4, 5, 6, 99, 99 }, { 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF });
	Compare<Intrin::ComparisonOp::Lt, i16, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
												{ 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99 },
												{ 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0xFFFF, 0, 0, 0xFFFF });
	//Compare<Intrin::ComparisonOp::Lt, i16, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
	//										   { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99, 99 , 18, 19, 99 , 21, 22, 99 , 99 , 99 , 99 , 27, 28, 29, 99 , 99 , 32 },
	//										   { 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0xFFFF, 0, 0, 0xFFFF,
	//										   	 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF,  0, 0xFFFF, 0xFFFF, 0 });
	
	Compare<Intrin::ComparisonOp::Lt, i8, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											   { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99 },
											   { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF });
	Compare<Intrin::ComparisonOp::Lt, i8, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
											   { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99, 99 , 18, 19, 99 , 21, 22, 99 , 99 , 99 , 99 , 27, 28, 29, 99 , 99 , 32 },
											   { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
												  0xFF, 0, 0, 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0 });
	//Compare<Intrin::ComparisonOp::Lt, i8, 64>({ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	//                                            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64 },
	//										    { 1 , 99 , 99 , 4 , 5 , 6 , 99 , 99 , 99 , 99 , 11, 12, 99 , 14, 15 , 99, 17, 99 , 99 , 299, 21, 22, 99 , 99 , 99 , 99 , 27, 28, 99 , 399, 31, 99 ,
	//										      99 , 34, 35, 99 , 99 , 99 , 39, 499, 41, 42, 99 , 99 , 45, 99 , 99 , 48, 99 , 599, 51, 99 , 99 , 99 , 55, 56, 57, 58, 99 , 99 , 61, 99 , 99 , 64 },
	//										    { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
	//											  0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
	//										   	  0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF,  0, 0xFF, 0xFF, 0,
	//										      0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF,  0, 0xFF, 0xFF, 0 });
}

TEST(IntrinPack, CmpLeTest)
{
	u64 Ones64 = 0xFFFF'FFFF'FFFF'FFFF;
	u32 Ones32 = 0xFFFF'FFFF;

	Compare<Intrin::ComparisonOp::Le, f64, 2>({ 1, 2 }, { 1, 0 }, { Ones64, 0 });
	Compare<Intrin::ComparisonOp::Le, f64, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { Ones64, 0, 0, Ones64 });
	//Compare<Intrin::ComparisonOp::Le, f64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { Ones64, 0, 0, Ones64, Ones64, Ones64, 0, 0 });

	Compare<Intrin::ComparisonOp::Le, f32, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { Ones32, 0, 0, Ones32 });
	Compare<Intrin::ComparisonOp::Le, f32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0 });
	//Compare<Intrin::ComparisonOp::Le, f32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 }, 
	//										   { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0, 0, 0, Ones32, Ones32, 0, Ones32, Ones32, 0 });

	Compare<Intrin::ComparisonOp::Le, u64, 2>({ 1, 2 }, { 1, 0 }, { Ones64, 0 });
	Compare<Intrin::ComparisonOp::Le, u64, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { Ones64, 0, 0, Ones64 });
	//Compare<Intrin::ComparisonOp::Le, u64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { Ones64, 0, 0, Ones64, Ones64, Ones64, 0, 0 });

	Compare<Intrin::ComparisonOp::Le, u32, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { Ones32, 0, 0, Ones32 });
	Compare<Intrin::ComparisonOp::Le, u32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0 });
	//Compare<Intrin::ComparisonOp::Le, u32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 }, 
	//										   { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0, 0, 0, Ones32, Ones32, 0, Ones32, Ones32, 0 });



	Compare<Intrin::ComparisonOp::Le, u16, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0 });
	Compare<Intrin::ComparisonOp::Le, u16, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 },
											   { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF, 0, 0xFFFF, 0xFFFF, 0 });
	//Compare<Intrin::ComparisonOp::Le, u16, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0, 0 , 18, 19, 0 , 21, 22, 0 , 0 , 0 , 0 , 27, 28, 29, 0 , 0 , 32 },
	//										   { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF, 0, 0xFFFF, 0xFFFF, 0,
	//										   	 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0,  0xFFFF, 0, 0, 0xFFFF });

	Compare<Intrin::ComparisonOp::Le, u8, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											  { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 },
											  { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0 });
	Compare<Intrin::ComparisonOp::Le, u8, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
											  { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0, 0 , 18, 19, 0 , 21, 22, 0 , 0 , 0 , 0 , 27, 28, 29, 0 , 0 , 32 },
											  { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
												 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF });
	//Compare<Intrin::ComparisonOp::Le, u8, 64>({ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	//                                            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64 },
	//										    { 1 , 0 , 0 , 4 , 5 , 6 , 0 , 0 , 0 , 0 , 11, 12, 0 , 14, 15 , 0, 17, 0 , 0 , 20, 21, 22, 0 , 0 , 0 , 0 , 27, 28, 0 , 30, 31, 0 ,
	//										      0 , 34, 35, 0 , 0 , 0 , 39, 40, 41, 42, 0 , 0 , 45, 0 , 0 , 48, 0 , 50, 51, 0 , 0 , 0 , 55, 56, 57, 58, 0 , 0 , 61, 0 , 0 , 64 },
	//										    { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
	//											  0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
	//										   	  0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0,  0xFF, 0, 0, 0xFF,
	//										      0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0,  0xFF, 0, 0, 0xFF });

	Compare<Intrin::ComparisonOp::Le, i64, 2>({ 1, 2 }, { 1, 0 }, { Ones64, 0 });
	Compare<Intrin::ComparisonOp::Le, i64, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { Ones64, 0, 0, Ones64 });
	//Compare<Intrin::ComparisonOp::Le, i64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { Ones64, 0, 0, Ones64, Ones64, Ones64, 0, 0 });

	Compare<Intrin::ComparisonOp::Le, i32, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { Ones32, 0, 0, Ones32 });
	Compare<Intrin::ComparisonOp::Le, i32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0 });
	//Compare<Intrin::ComparisonOp::Le, i32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 }, 
	//										   { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0, 0, 0, Ones32, Ones32, 0, Ones32, Ones32, 0 });



	Compare<Intrin::ComparisonOp::Le, i16, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0 });
	Compare<Intrin::ComparisonOp::Le, i16, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 },
											   { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF, 0, 0xFFFF, 0xFFFF, 0 });
	//Compare<Intrin::ComparisonOp::Le, i16, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
	//										     { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0, 0 , 18, 19, 0 , 21, 22, 0 , 0 , 0 , 0 , 27, 28, 29, 0 , 0 , 32 },
	//										     { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF, 0, 0xFFFF, 0xFFFF, 0,
	//										       0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0,  0xFFFF, 0, 0, 0xFFFF });

	Compare<Intrin::ComparisonOp::Le, i8, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											  { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 },
											  { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0 });
	Compare<Intrin::ComparisonOp::Le, i8, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
											  { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0, 0 , 18, 19, 0 , 21, 22, 0 , 0 , 0 , 0 , 27, 28, 29, 0 , 0 , 32 },
											  { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
												 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF });
	//Compare<Intrin::ComparisonOp::Le, i8, 64>({ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	//                                            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64 },
	//										    { 1 , 0 , 0 , 4 , 5 , 6 , 0 , 0 , 0 , 0 , 11, 12, 0 , 14, 15 , 0, 17, 0 , 0 , 20, 21, 22, 0 , 0 , 0 , 0 , 27, 28, 0 , 30, 31, 0 ,
	//										      0 , 34, 35, 0 , 0 , 0 , 39, 40, 41, 42, 0 , 0 , 45, 0 , 0 , 48, 0 , 50, 51, 0 , 0 , 0 , 55, 56, 57, 58, 0 , 0 , 61, 0 , 0 , 64 },
	//										    { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
	//											  0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
	//										   	  0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0,  0xFF, 0, 0, 0xFF,
	//										      0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0,  0xFF, 0, 0, 0xFF });
}

TEST(IntrinPack, CmpGtTest)
{
	u64 Ones64 = 0xFFFF'FFFF'FFFF'FFFF;
	u32 Ones32 = 0xFFFF'FFFF;

	Compare<Intrin::ComparisonOp::Gt, f64, 2>({ 1, 2 }, { 1, 0 }, { 0, Ones64 });
	Compare<Intrin::ComparisonOp::Gt, f64, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { 0, Ones64, Ones64, 0 });
	//Compare<Intrin::ComparisonOp::Gt, f64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0, Ones64, Ones64, 0, 0, 0, Ones64, Ones64 });

	Compare<Intrin::ComparisonOp::Gt, f32, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { 0, Ones32, Ones32, 0 });
	Compare<Intrin::ComparisonOp::Gt, f32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32 });
	//Compare<Intrin::ComparisonOp::Gt, f32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 }, 
	//										   {  0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32, Ones32, Ones32, 0, 0, Ones32, 0, 0, Ones32 });

	Compare<Intrin::ComparisonOp::Gt, u64, 2>({ 1, 2 }, { 1, 0 }, { 0, Ones64 });
	Compare<Intrin::ComparisonOp::Gt, u64, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { 0, Ones64, Ones64, 0 });
	//Compare<Intrin::ComparisonOp::Gt, u64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0, Ones64, Ones64, 0, 0, 0, Ones64, Ones64 });

	Compare<Intrin::ComparisonOp::Gt, u32, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { 0, Ones32, Ones32, 0 });
	Compare<Intrin::ComparisonOp::Gt, u32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32 });
	//Compare<Intrin::ComparisonOp::Gt, u32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 }, 
	//										   { 0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32, Ones32, Ones32, 0, 0, Ones32, 0, 0, Ones32 });


	Compare<Intrin::ComparisonOp::Gt, u16, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF });
	Compare<Intrin::ComparisonOp::Gt, u16, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
												{ 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 },
												{ 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0xFFFF, 0, 0, 0xFFFF });
	//Compare<Intrin::ComparisonOp::Gt, u16, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0, 0 , 18, 19, 0 , 21, 22, 0 , 0 , 0 , 0 , 27, 28, 29, 0 , 0 , 32 },
	//										   { 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0xFFFF, 0, 0, 0xFFFF,
	//										   	 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF,  0, 0xFFFF, 0xFFFF, 0 });

	Compare<Intrin::ComparisonOp::Gt, u8, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 },
											   { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF });
	Compare<Intrin::ComparisonOp::Gt, u8, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
											   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0, 0 , 18, 19, 0 , 21, 22, 0 , 0 , 0 , 0 , 27, 28, 29, 0 , 0 , 32 },
											   { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
												  0xFF, 0, 0, 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0 });
	//Compare<Intrin::ComparisonOp::Gt, u8, 64>({ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	//                                            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64 },
	//										    { 1 , 0 , 0 , 4 , 5 , 6 , 0 , 0 , 0 , 0 , 11, 12, 0 , 14, 15 , 0, 17, 0 , 0 , 20, 21, 22, 0 , 0 , 0 , 0 , 27, 28, 0 , 30, 31, 0 ,
	//										      0 , 34, 35, 0 , 0 , 0 , 39, 40, 41, 42, 0 , 0 , 45, 0 , 0 , 48, 0 , 50, 51, 0 , 0 , 0 , 55, 56, 57, 58, 0 , 0 , 61, 0 , 0 , 64 },
	//										    { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
	//											  0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
	//										   	  0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF,  0, 0xFF, 0xFF, 0,
	//										      0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF,  0, 0xFF, 0xFF, 0 });

	Compare<Intrin::ComparisonOp::Gt, i64, 2>({ 1, 2 }, { 1, 0 }, { 0, Ones64 });
	Compare<Intrin::ComparisonOp::Gt, i64, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { 0, Ones64, Ones64, 0 });
	//Compare<Intrin::ComparisonOp::Gt, i64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0, Ones64, Ones64, 0, 0, 0, Ones64, Ones64 });

	Compare<Intrin::ComparisonOp::Gt, i32, 4>({ 1, 2, 3, 4 }, { 1, 0, 0, 4 }, { 0, Ones32, Ones32, 0 });
	Compare<Intrin::ComparisonOp::Gt, i32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32 });
	//Compare<Intrin::ComparisonOp::Gt, i32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 }, 
	//										   { 0, Ones32, Ones32, 0, 0, 0, Ones32, Ones32, Ones32, Ones32, 0, 0, Ones32, 0, 0, Ones32 });



	Compare<Intrin::ComparisonOp::Gt, i16, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 0, 0, 4, 5, 6, 0, 0 }, { 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF });
	Compare<Intrin::ComparisonOp::Gt, i16, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
												{ 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 },
												{ 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0xFFFF, 0, 0, 0xFFFF });
	//Compare<Intrin::ComparisonOp::Gt, i16, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
	//										   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0, 0 , 18, 19, 0 , 21, 22, 0 , 0 , 0 , 0 , 27, 28, 29, 0 , 0 , 32 },
	//										   { 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0xFFFF, 0, 0, 0xFFFF,
	//										   	 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF,  0, 0xFFFF, 0xFFFF, 0 });

	Compare<Intrin::ComparisonOp::Gt, i8, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0 },
											   { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF });
	Compare<Intrin::ComparisonOp::Gt, i8, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
											   { 1, 0, 0, 4, 5, 6, 0, 0, 0, 0 , 11, 12, 0 , 14, 15 , 0, 0 , 18, 19, 0 , 21, 22, 0 , 0 , 0 , 0 , 27, 28, 29, 0 , 0 , 32 },
											   { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
												  0xFF, 0, 0, 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0 });
	//Compare<Intrin::ComparisonOp::Gt, i8, 64>({ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	//                                            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64 },
	//										    { 1 , 0 , 0 , 4 , 5 , 6 , 0 , 0 , 0 , 0 , 11, 12, 0 , 14, 15 , 0, 17, 0 , 0 , 20, 21, 22, 0 , 0 , 0 , 0 , 27, 28, 0 , 30, 31, 0 ,
	//										      0 , 34, 35, 0 , 0 , 0 , 39, 40, 41, 42, 0 , 0 , 45, 0 , 0 , 48, 0 , 50, 51, 0 , 0 , 0 , 55, 56, 57, 58, 0 , 0 , 61, 0 , 0 , 64 },
	//										    { 0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
	//											  0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF,
	//										   	  0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF,  0, 0xFF, 0xFF, 0,
	//										      0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF,  0, 0xFF, 0xFF, 0 });
}

TEST(IntrinPack, CmpGeTest)
{
	u64 Ones64 = 0xFFFF'FFFF'FFFF'FFFF;
	u32 Ones32 = 0xFFFF'FFFF;

	Compare<Intrin::ComparisonOp::Ge, f64, 2>({ 1, 2 }, { 1, 99 }, { Ones64, 0 });
	Compare<Intrin::ComparisonOp::Ge, f64, 4>({ 1, 2, 3, 4 }, { 1, 99, 99, 4 }, { Ones64, 0, 0, Ones64 });
	//Compare<Intrin::ComparisonOp::Ge, f64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 99, 99, 4, 5, 6, 99, 99 }, { Ones64, 0, 0, Ones64, Ones64, Ones64, 0, 0 });

	Compare<Intrin::ComparisonOp::Ge, f32, 4>({ 1, 2, 3, 4 }, { 1, 99, 99, 4 }, { Ones32, 0, 0, Ones32 });
	Compare<Intrin::ComparisonOp::Ge, f32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 99, 99, 4, 5, 6, 99, 99 }, { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0 });
	//Compare<Intrin::ComparisonOp::Ge, f32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99 }, 
	//										   { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0, 0, 0, Ones32, Ones32, 0, Ones32, Ones32, 0 });

	Compare<Intrin::ComparisonOp::Ge, u64, 2>({ 1, 2 }, { 1, 99 }, { Ones64, 0 });
	Compare<Intrin::ComparisonOp::Ge, u64, 4>({ 1, 2, 3, 4 }, { 1, 99, 99, 4 }, { Ones64, 0, 0, Ones64 });
	//Compare<Intrin::ComparisonOp::Ge, u64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 99, 99, 4, 5, 6, 99, 99 }, { Ones64, 0, 0, Ones64, Ones64, Ones64, 0, 0 });

	Compare<Intrin::ComparisonOp::Ge, u32, 4>({ 1, 2, 3, 4 }, { 1, 99, 99, 4 }, { Ones32, 0, 0, Ones32 });
	Compare<Intrin::ComparisonOp::Ge, u32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 99, 99, 4, 5, 6, 99, 99 }, { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0 });
	//Compare<Intrin::ComparisonOp::Ge, u32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99 }, 
	//										   { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0, 0, 0, Ones32, Ones32, 0, Ones32, Ones32, 0 });



	Compare<Intrin::ComparisonOp::Ge, u16, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 99, 99, 4, 5, 6, 99, 99 }, { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0 });
	Compare<Intrin::ComparisonOp::Ge, u16, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											   { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99 },
											   { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF, 0, 0xFFFF, 0xFFFF, 0 });
	//Compare<Intrin::ComparisonOp::Ge, u16, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
	//										   { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99, 99 , 18, 19, 99 , 21, 22, 99 , 99 , 99 , 99 , 27, 28, 29, 99 , 99 , 32 },
	//										   { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF, 0, 0xFFFF, 0xFFFF, 0,
	//										   	 0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0,  0xFFFF, 0, 0, 0xFFFF });

	Compare<Intrin::ComparisonOp::Ge, u8, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											  { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99 },
											  { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0 });
	Compare<Intrin::ComparisonOp::Ge, u8, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
											  { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99, 99 , 18, 19, 99 , 21, 22, 99 , 99 , 99 , 99 , 27, 28, 29, 99 , 99 , 32 },
											  { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
												 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF });
	//Compare<Intrin::ComparisonOp::Ge, u8, 64>({ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	//                                            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64 },
	//										    { 1 , 99 , 99 , 4 , 5 , 6 , 99 , 99 , 99 , 99 , 11, 12, 99 , 14, 15 , 99, 17, 99 , 99 , 299, 21, 22, 99 , 99 , 99 , 99 , 27, 28, 99 , 399, 31, 99 ,
	//										      99 , 34, 35, 99 , 99 , 99 , 39, 499, 41, 42, 99 , 99 , 45, 99 , 99 , 48, 99 , 599, 51, 99 , 99 , 99 , 55, 56, 57, 58, 99 , 99 , 61, 99 , 99 , 64 },
	//										    { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
	//											  0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
	//										   	  0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0,  0xFF, 0, 0, 0xFF,
	//										      0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0,  0xFF, 0, 0, 0xFF });

	Compare<Intrin::ComparisonOp::Ge, i64, 2>({ 1, 2 }, { 1, 99 }, { Ones64, 0 });
	Compare<Intrin::ComparisonOp::Ge, i64, 4>({ 1, 2, 3, 4 }, { 1, 99, 99, 4 }, { Ones64, 0, 0, Ones64 });
	//Compare<Intrin::ComparisonOp::Ge, i64, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 99, 99, 4, 5, 6, 99, 99 }, { Ones64, 0, 0, Ones64, Ones64, Ones64, 0, 0 });

	Compare<Intrin::ComparisonOp::Ge, i32, 4>({ 1, 2, 3, 4 }, { 1, 99, 99, 4 }, { Ones32, 0, 0, Ones32 });
	Compare<Intrin::ComparisonOp::Ge, i32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 99, 99, 4, 5, 6, 99, 99 }, { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0 });
	//Compare<Intrin::ComparisonOp::Ge, i32, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, 
	//										   { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99 }, 
	//										   { Ones32, 0, 0, Ones32, Ones32, Ones32, 0, 0, 0, 0, Ones32, Ones32, 0, Ones32, Ones32, 0 });



	Compare<Intrin::ComparisonOp::Ge, i16, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 }, { 1, 99, 99, 4, 5, 6, 99, 99 }, { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0 });
	Compare<Intrin::ComparisonOp::Ge, i16, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											   { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99 },
											   { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF, 0, 0xFFFF, 0xFFFF, 0 });
	//Compare<Intrin::ComparisonOp::Ge, i16, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
	//										     { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99, 99 , 18, 19, 99 , 21, 22, 99 , 99 , 99 , 99 , 27, 28, 29, 99 , 99 , 32 },
	//										     { 0xFFFF, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0xFFFF, 0xFFFF, 0, 0xFFFF, 0xFFFF, 0,
	//										       0, 0xFFFF, 0xFFFF, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0,  0xFFFF, 0, 0, 0xFFFF });

	Compare<Intrin::ComparisonOp::Ge, i8, 16>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
											  { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99 },
											  { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0 });
	Compare<Intrin::ComparisonOp::Ge, i8, 32>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
											  { 1, 99, 99, 4, 5, 6, 99, 99, 99, 99 , 11, 12, 99 , 14, 15 , 99, 99 , 18, 19, 99 , 21, 22, 99 , 99 , 99 , 99 , 27, 28, 29, 99 , 99 , 32 },
											  { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
												 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF });
	//Compare<Intrin::ComparisonOp::Ge, i8, 64>({ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	//                                            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64 },
	//										    { 1 , 99 , 99 , 4 , 5 , 6 , 99 , 99 , 99 , 99 , 11, 12, 99 , 14, 15 , 99, 17, 99 , 99 , 299, 21, 22, 99 , 99 , 99 , 99 , 27, 28, 99 , 399, 31, 99 ,
	//										      99 , 34, 35, 99 , 99 , 99 , 39, 499, 41, 42, 99 , 99 , 45, 99 , 99 , 48, 99 , 599, 51, 99 , 99 , 99 , 55, 56, 57, 58, 99 , 99 , 61, 99 , 99 , 64 },
	//										    { 0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
	//											  0xFF, 0, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0,
	//										   	  0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0,  0xFF, 0, 0, 0xFF,
	//										      0, 0xFF, 0xFF, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0,  0xFF, 0, 0, 0xFF });
}

template<Intrin::SimdBaseType T, usize Width>
auto Convert()
{
	using PackT = Intrin::Pack<T, Width>;
	alignas(PackT::Align) T src[Width];
	for (usize i = 0; i < Width; ++i)
	{
		if (Core::SignedIntegral<T>)
			src[i] = T(i * (1 - 2 * (i & 1)));

		src[i] = T(i);
	}
	PackT pack = PackT::AlignedLoad(src);

	if constexpr (!Core::IsF64<T>)
	{
		constexpr usize NewWidth = PackT::DataSize / sizeof(f64);
		Intrin::Pack<f64, NewWidth> packRes = pack.template Convert<f64>();
		f64 res[NewWidth];
		packRes.Store(res);

		for (usize i = 0; i < Core::Math::Min(Width, NewWidth); ++i)
			ASSERT_EQ(res[i], src[i]);

		for (usize i = Width; i < NewWidth; ++i)
			ASSERT_EQ(res[i], 0);
	}
	if constexpr (!Core::IsF32<T>)
	{
		constexpr usize NewWidth = PackT::DataSize / sizeof(f32);
		Intrin::Pack<f32, NewWidth> packRes = pack.template Convert<f32>();
		f32 res[NewWidth];
		packRes.Store(res);

		for (usize i = 0; i < Core::Math::Min(Width, NewWidth); ++i)
			ASSERT_EQ(res[i], src[i]);

		for (usize i = Width; i < NewWidth; ++i)
			ASSERT_EQ(res[i], 0);
	}
	if constexpr (!Core::IsU64<T>)
	{
		constexpr usize NewWidth = PackT::DataSize / sizeof(u64);
		Intrin::Pack<u64, NewWidth> packRes = pack.template Convert<u64>();
		u64 res[NewWidth];
		packRes.Store(res);

		for (usize i = 0; i < Core::Math::Min(Width, NewWidth); ++i)
			ASSERT_EQ(res[i], u64(src[i]));

		for (usize i = Width; i < NewWidth; ++i)
			ASSERT_EQ(res[i], 0);
	}
	if constexpr (!Core::IsU32<T>)
	{
		constexpr usize NewWidth = PackT::DataSize / sizeof(u32);
		Intrin::Pack<u32, NewWidth> packRes = pack.template Convert<u32>();
		u32 res[NewWidth];
		packRes.Store(res);

		for (usize i = 0; i < Core::Math::Min(Width, NewWidth); ++i)
			ASSERT_EQ(res[i], u32(src[i]));

		for (usize i = Width; i < NewWidth; ++i)
			ASSERT_EQ(res[i], 0);
	}
	if constexpr (!Core::IsU16<T>)
	{
		constexpr usize NewWidth = PackT::DataSize / sizeof(u16);
		Intrin::Pack<u16, NewWidth> packRes = pack.template Convert<u16>();
		u16 res[NewWidth];
		packRes.Store(res);

		for (usize i = 0; i < Core::Math::Min(Width, NewWidth); ++i)
			ASSERT_EQ(res[i], u16(src[i]));

		for (usize i = Width; i < NewWidth; ++i)
			ASSERT_EQ(res[i], 0);
	}
	if constexpr (!Core::IsU8<T>)
	{
		constexpr usize NewWidth = PackT::DataSize / sizeof(u8);
		Intrin::Pack<u8, NewWidth> packRes = pack.template Convert<u8>();
		u8 res[NewWidth];
		packRes.Store(res);

		for (usize i = 0; i < Core::Math::Min(Width, NewWidth); ++i)
			ASSERT_EQ(res[i], u8(src[i]));

		for (usize i = Width; i < NewWidth; ++i)
			ASSERT_EQ(res[i], 0);
	}
	if constexpr (!Core::IsI64<T>)
	{
		constexpr usize NewWidth = PackT::DataSize / sizeof(i64);
		Intrin::Pack<i64, NewWidth> packRes = pack.template Convert<i64>();
		i64 res[NewWidth];
		packRes.Store(res);

		for (usize i = 0; i < Core::Math::Min(Width, NewWidth); ++i)
			ASSERT_EQ(res[i], src[i]);

		for (usize i = Width; i < NewWidth; ++i)
			ASSERT_EQ(res[i], 0);
	}
	if constexpr (!Core::IsI32<T>)
	{
		constexpr usize NewWidth = PackT::DataSize / sizeof(i32);
		Intrin::Pack<i32, NewWidth> packRes = pack.template Convert<i32>();
		i32 res[NewWidth];
		packRes.Store(res);

		for (usize i = 0; i < Core::Math::Min(Width, NewWidth); ++i)
			ASSERT_EQ(res[i], src[i]);

		for (usize i = Width; i < NewWidth; ++i)
			ASSERT_EQ(res[i], 0);
	}
	if constexpr (!Core::IsI16<T>)
	{
		constexpr usize NewWidth = PackT::DataSize / sizeof(i16);
		Intrin::Pack<i16, NewWidth> packRes = pack.template Convert<i16>();
		i16 res[NewWidth];
		packRes.Store(res);

		for (usize i = 0; i < Core::Math::Min(Width, NewWidth); ++i)
			ASSERT_EQ(res[i], src[i]);

		for (usize i = Width; i < NewWidth; ++i)
			ASSERT_EQ(res[i], 0);
	}
	if constexpr (!Core::IsI8<T>)
	{
		constexpr usize NewWidth = PackT::DataSize / sizeof(i8);
		Intrin::Pack<i8, NewWidth> packRes = pack.template Convert<i8>();
		i8 res[NewWidth];
		 packRes.Store(res);

		for (usize i = 0; i < Core::Math::Min(Width, NewWidth); ++i)
			ASSERT_EQ(res[i], src[i]);

		for (usize i = Width; i < NewWidth; ++i)
			ASSERT_EQ(res[i], 0);
	}
}

TEST(IntrinPack, Convert)
{
	Convert<f64, 2>();
	Convert<f64, 4>();
	Convert<f32, 4>();
	Convert<f32, 8>();
	
	Convert<u64, 2>();
	Convert<u64, 4>();
	Convert<u32, 4>();
	Convert<u32, 8>();
	Convert<u16, 8>();
	Convert<u16, 16>();
	Convert<u8, 16>();
	Convert<u8, 32>();
	
	Convert<i64, 2>();
	Convert<i64, 4>();
	Convert<i32, 4>();
	Convert<i32, 8>();
	Convert<i16, 8>();
	Convert<i16, 16>();
	Convert<i8, 16>();
	Convert<i8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto Add() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	for (usize i = 0; i < Width; ++i)
		dataA[i] = T(i);
	PackT packA = PackT::Load(dataA);

	T dataB[Width];
	for (usize i = 0; i < Width; ++i)
		dataB[i] = T(2 * i);
	PackT packB = PackT::Load(dataB);

	PackT packRes = packA.Add(packB);
	T res[Width];
	packRes.Store(res);
	
	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], dataA[i] + dataB[i]);
}

TEST(IntrinPack, Add)
{
	Add<f64, 2>();
	Add<f64, 4>();
	Add<f32, 4>();
	Add<f32, 8>();

	Add<u64, 2>();
	Add<u64, 4>();
	Add<u32, 4>();
	Add<u32, 8>();
	Add<u16, 8>();
	Add<u16, 16>();
	Add<u8, 16>();
	Add<u8, 32>();

	Add<i64, 2>();
	Add<i64, 4>();
	Add<i32, 4>();
	Add<i32, 8>();
	Add<i16, 8>();
	Add<i16, 16>();
	Add<i8, 16>();
	Add<i8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto Sub() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	for (usize i = 0; i < Width; ++i)
		dataA[i] = T(3 * i);
	PackT packA = PackT::Load(dataA);

	T dataB[Width];
	for (usize i = 0; i < Width; ++i)
		dataB[i] = T(2 * i);
	PackT packB = PackT::Load(dataB);

	PackT packRes = packA.Sub(packB);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], dataA[i] - dataB[i]);
}

TEST(IntrinPack, Sub)
{
	Sub<f64, 2>();
	Sub<f64, 4>();
	Sub<f32, 4>();
	Sub<f32, 8>();

	Sub<u64, 2>();
	Sub<u64, 4>();
	Sub<u32, 4>();
	Sub<u32, 8>();
	Sub<u16, 8>();
	Sub<u16, 16>();
	Sub<u8, 16>();
	Sub<u8, 32>();

	Sub<i64, 2>();
	Sub<i64, 4>();
	Sub<i32, 4>();
	Sub<i32, 8>();
	Sub<i16, 8>();
	Sub<i16, 16>();
	Sub<i8, 16>();
	Sub<i8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto Mul() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	for (usize i = 0; i < Width; ++i)
		dataA[i] = T(i);
	PackT packA = PackT::Load(dataA);

	T dataB[Width];
	for (usize i = 0; i < Width; ++i)
		dataB[i] = T(2 * i);
	PackT packB = PackT::Load(dataB);

	PackT packRes = packA.Mul(packB);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], T(dataA[i] * dataB[i]));
}

TEST(IntrinPack, Mul)
{
	Mul<f64, 2>();
	Mul<f64, 4>();
	Mul<f32, 4>();
	Mul<f32, 8>();

	Mul<u64, 2>();
	Mul<u64, 4>();
	Mul<u32, 4>();
	Mul<u32, 8>();
	Mul<u16, 8>();
	Mul<u16, 16>();
	Mul<u8, 16>();
	Mul<u8, 32>();

	Mul<i64, 2>();
	Mul<i64, 4>();
	Mul<i32, 4>();
	Mul<i32, 8>();
	Mul<i16, 8>();
	Mul<i16, 16>();
	Mul<i8, 16>();
	Mul<i8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto Div(InitializerList<T> a, InitializerList<T> b) -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	Core::MemCpy(dataA, a.begin(), a.size() * sizeof(T));
	PackT packA = PackT::Load(dataA);
	T dataB[Width];
	Core::MemCpy(dataB, b.begin(), b.size() * sizeof(T));
	PackT packB = PackT::Load(dataB);

	PackT packRes = packA.Div(packB);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], dataA[i] / dataB[i]);
}

TEST(IntrinPack, Div)
{
	Div<f64, 2>({ 2, 8 },
				{ 2, 4 });
	Div<f64, 4>({ 2, 8, 18, 32 },
				{ 2, 4, 6 , 8 });

	Div<f32, 4>({ 2, 8, 18, 32 },
				{ 2, 4, 6 , 8 });
	Div<f32, 8>({ 2, 8, 18, 32, 50, 72, 98, 128 },
				{ 2, 4, 6 , 8 , 10, 12, 14, 16 });

	Div<u64, 2>({ 2, 8 },
				{ 2, 4 });
	Div<u64, 4>({ 2, 8, 18, 32 },
				{ 2, 4, 6 , 8 });

	Div<u32, 4>({ 2, 8, 18, 32 },
				{ 2, 4, 6 , 8 });
	Div<u32, 8>({ 2, 8, 18, 32, 50, 72, 98, 128 },
				{ 2, 4, 6 , 8 , 10, 12, 14, 16 });

	Div<u16, 8>({ 2, 8, 18, 32, 50, 72, 98, 128 },
				{ 2, 4, 6 , 8 , 10, 12, 14, 16 });
	Div<u16, 16>({ 2, 8, 18, 32, 50, 72, 98, 128, 162, 200, 242, 288, 338, 392, 450, 512 },
				 { 2, 4, 6 , 8 , 10, 12, 14, 16 , 18 , 20 , 22 , 24 , 26 , 28 , 30 , 32 });

	Div<u8, 16>({ 2, 8, 18, 32, 50, 72, 98, 128, 162, 200, 242, 200, 162, 128, 98, 72 },
				{ 2, 4, 6 , 8 , 10, 12, 14, 16 , 18 , 20 , 22 , 20 , 18 , 16 , 14, 12 });
	Div<u8, 32>({ 2, 8, 18, 32, 50, 72, 98, 128, 162, 200, 242, 200, 162, 128, 98, 72, 50, 32, 18, 8, 2, 8, 18, 32, 50, 72, 98, 128, 162, 200, 242, 200 },
				{ 2, 4, 6 , 8 , 10, 12, 14, 16 , 18 , 20 , 22 , 20 , 18 , 16 , 14, 12, 10, 8 , 6 , 4, 2, 4, 6 , 8 , 10, 12, 14, 16 , 18 , 20 , 22 , 20 });

	Div<i64, 2>({ 2, 8 },
				{ 2, 4 });
	Div<i64, 4>({ 2, 8, 18, 32 },
				{ 2, 4, 6 , 8 });

	Div<i32, 4>({ 2, 8, 18, 32 },
				{ 2, 4, 6 , 8 });
	Div<i32, 8>({ 2, 8, 18, 32, 50, 72, 98, 128 },
				{ 2, 4, 6 , 8 , 10, 12, 14, 16 });

	Div<i16, 8>({ 2, 8, 18, 32, 50, 72, 98, 128 },
				{ 2, 4, 6 , 8 , 10, 12, 14, 16 });
	Div<i16, 16>({ 2, 8, 18, 32, 50, 72, 98, 128, 162, 200, 242, 288, 338, 392, 450, 512 },
				 { 2, 4, 6 , 8 , 10, 12, 14, 16 , 18 , 20 , 22 , 24 , 26 , 28 , 30 , 32 });

	Div<i8, 16>({ 2, 8, 18, 32, 50, 72, 98, 72, 50, 32, 18, 8, 2, 8, 18, 32 },
				{ 2, 4, 6 , 8 , 10, 12, 14, 12, 10, 8 , 6 , 4, 2, 4, 6 , 8 });
	Div<i8, 32>({ 2, 8, 18, 32, 50, 72, 98, 72, 50, 32, 18, 8, 2, 8, 18, 32, 50, 72, 98, 72, 50, 32, 18, 8, 2, 8, 18, 32, 50, 72, 98, 72 },
				{ 2, 4, 6 , 8 , 10, 12, 14, 12, 10, 8 , 6 , 4, 2, 4, 6 , 8 , 10, 12, 14, 12, 10, 8 , 6 , 4, 2, 4, 6 , 8 , 10, 12, 14, 12 });
}

template<Intrin::SimdBaseType T, usize Width>
auto AddSaturated(InitializerList<T> a, InitializerList<T> b) -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	Core::MemCpy(dataA, a.begin(), a.size() * sizeof(T));
	PackT packA = PackT::Load(dataA);
	T dataB[Width];
	Core::MemCpy(dataB, b.begin(), b.size() * sizeof(T));
	PackT packB = PackT::Load(dataB);

	PackT packRes = packA.AddSaturated(packB);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
	{
		if (Core::Math::Consts::MaxVal<T> - dataA[i] < dataB[i])
			ASSERT_EQ(res[i], Core::Math::Consts::MaxVal<T>);
		else
			ASSERT_EQ(res[i], dataA[i] + dataB[i]);
	}
}

TEST(IntrinPack, AddSaturated)
{
	AddSaturated<f64, 2>({ 1, 2 },
				{ 2, 4 });
	AddSaturated<f64, 4>({ 1, 2, 3, 4 },
				{ 2, 4, 6, 8 });

	AddSaturated<f32, 4>({ 1, 2, 3, 4 },
				{ 2, 4, 6, 8 });
	AddSaturated<f32, 8>({ 1, 2, 3, 4 , 5 , 6 , 7 , 8 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16 });

	u64 maxU64 = Core::Math::Consts::MaxVal<u64>;
	AddSaturated<u64, 2>({ 1, maxU64 },
				{ 2, 4 });
	AddSaturated<u64, 4>({ 1, maxU64, 3, maxU64 - 5 },
				{ 2, 4, 6, 8 });

	AddSaturated<u32, 4>({ 1, 4294967295, 3, 4294967290 },
				{ 2, 4, 6, 8 });
	AddSaturated<u32, 8>({ 1, 4294967295, 3, 4294967290, 5 , 6 , 7 , 4294967280 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16 });

	AddSaturated<u16, 8>({ 1, 65535, 3, 65534 , 5 , 6 , 7 , 65520 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16 });
	AddSaturated<u16, 16>({ 1, 65535, 3, 65534 , 5 , 6 , 7 , 65520 , 9 , 10, 11, 12, 13, 14, 65510, 16 },
				 { 2, 4, 6, 8 , 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 });

	AddSaturated<u8, 16>({ 1, 255, 3, 253 , 5 , 6 , 7 , 240 , 9 , 10, 11, 12, 13, 14, 15, 230 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 });
	AddSaturated<u8, 32>({ 1, 255, 3, 253 , 5 , 6 , 7 , 240 , 9 , 10, 11, 12, 13, 14, 15, 230, 17, 18, 19, 20, 21, 22, 23, 250, 25, 26, 27, 28, 29, 30, 222, 32 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64 });

	i64 maxI64 = Core::Math::Consts::MaxVal<i64>;
	AddSaturated<i64, 2>({ 1, maxI64 },
				{ 2, 4 });
	AddSaturated<i64, 4>({ 1, maxI64, 3, maxI64 - 5 },
				{ 2, 4, 6, 8 });

	AddSaturated<i32, 4>({ 1, 2147483647, 3, 2147483640 },
				{ 2, 4, 6, 8 });
	AddSaturated<i32, 8>({ 1, 2147483647, 2147483640, 4 , 5 , 6 , 7 , 2147483630 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16 });

	AddSaturated<i16, 8>({ 1, 32767, 3, 32760 , 5 , 6 , 7 , 32750 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16 });
	AddSaturated<i16, 16>({ 1, 32767, 3, 32760 , 5 , 6 , 7 , 32750 , 9 , 10, 11, 12, 13, 14, 32740, 16 },
				 { 2, 4, 6, 8 , 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 });

	AddSaturated<i8, 16>({ 1, 127, 3, 120 , 5 , 6 , 7 , 110 , 9 , 10, 11, 12, 13, 14, 100, 16 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 });
	AddSaturated<i8, 32>({ 1, 127, 3, 120 , 5 , 6 , 7 , 110 , 9 , 10, 11, 12, 13, 14, 100, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 90, 28, 29, 30, 31, 32 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64 });
}

template<Intrin::SimdBaseType T, usize Width>
auto SubSaturated(InitializerList<T> a, InitializerList<T> b) -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	Core::MemCpy(dataA, a.begin(), a.size() * sizeof(T));
	PackT packA = PackT::Load(dataA);
	T dataB[Width];
	Core::MemCpy(dataB, b.begin(), b.size() * sizeof(T));
	PackT packB = PackT::Load(dataB);

	PackT packRes = packA.SubSaturated(packB);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
	{
		if (Core::Math::Consts::LowestVal<T> + dataB[i] > dataA[i])
			ASSERT_EQ(res[i], Core::Math::Consts::LowestVal<T>);
		else
			ASSERT_EQ(res[i], dataA[i] - dataB[i]);
	}
}

TEST(IntrinPack, SubSaturated)
{
	SubSaturated<f64, 2>({ 3, 2 },
				{ 2, 4 });
	SubSaturated<f64, 4>({ 3, 2, 9, 12 },
				{ 2, 4, 6, 8 });

	SubSaturated<f32, 4>({ 3, 2, 9, 12 },
				{ 2, 4, 6, 8 });
	SubSaturated<f32, 8>({ 3, 2, 9, 12, 15, 18, 21, 24 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16 });

	SubSaturated<u64, 2>({ 3, 6 },
				{ 2, 4 });
	SubSaturated<u64, 4>({ 3, 6, 9, 12 },
				{ 2, 4, 6, 8 });

	SubSaturated<u32, 4>({ 3, 2, 9, 6 },
				{ 2, 4, 6, 8 });
	SubSaturated<u32, 8>({ 3, 2, 9, 6, 15, 18, 11, 24 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16 });
	
	SubSaturated<u16, 8>({ 3, 2, 9, 6, 15, 18, 21, 24 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16 });
	SubSaturated<u16, 16>({ 3, 2, 9, 6, 15, 18, 11, 24, 27, 15, 33, 36, 39, 42, 45, 48 },
				 { 2, 4, 6, 8 , 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 });
	
	SubSaturated<u8, 16>({ 3, 2, 9, 6, 15, 18, 11, 24, 27, 15, 33, 36, 39, 42, 45, 48 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 });
	SubSaturated<u8, 32>({ 3, 2, 9, 6, 15, 18, 21, 24, 27, 15, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60, 63, 66, 69, 72, 40, 78, 81, 84, 87, 90, 93, 96 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64 });
	
	SubSaturated<i64, 2>({ 3, 6 },
				{ 2, 4 });
	SubSaturated<i64, 4>({ 3, 6, 9, 12 },
				{ 2, 4, 6, 8 });
	
	SubSaturated<i32, 4>({ 3, -2147483647, 9, -2147483642 },
				{ 2, 4, 6, 8 });
	SubSaturated<i32, 8>({ 3, -2147483647, 9, -2147483642, 15, 18, 21, -2147483640 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16 });
	
	SubSaturated<i16, 8>({ 3, -32767, 9, -32761, 15, 18, 21, -32760 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16 });
	SubSaturated<i16, 16>({ 3, -32767, 9, -32761, 15, 18, 21, -32760, 27, 30, 33, 36, 39, 42, 45, -32750 },
				 { 2, 4, 6, 8 , 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 });
	
	SubSaturated<i8, 16>({ 3, -128, 9, -123, 15, 18, 21, -120, 27, 30, 33, 36, 39, 42, 45, -110 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 });
	SubSaturated<i8, 32>({ 3, -128, 9, -123, 15, 18, 21, -120, 27, 30, 33, 36, 39, 42, 45, -110, 51, 54, 57, 60, 63, 66, 69, 72, -100, 78, 81, 84, 87, 90, 93, 96 },
				{ 2, 4, 6, 8 , 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64 });
}

template<Intrin::SimdBaseType T, usize Width>
auto And(InitializerList<T> a, InitializerList<T> b) -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	Core::MemCpy(dataA, a.begin(), a.size() * sizeof(T));
	PackT packA = PackT::Load(dataA);
	T dataB[Width];
	Core::MemCpy(dataB, b.begin(), b.size() * sizeof(T));
	PackT packB = PackT::Load(dataB);

	PackT packRes = packA.And(packB);
	T res[Width];
	packRes.Store(res);

	using Unsigned = Core::UnsignedOfSameSize<T>;
	for (usize i = 0; i < Width; ++i)
	{
		Unsigned tmpA = *reinterpret_cast<Unsigned*>(dataA + i);
		Unsigned tmpB = *reinterpret_cast<Unsigned*>(dataB + i);
		Unsigned tmp = tmpA & tmpB;
		ASSERT_EQ(res[i], *reinterpret_cast<T*>(&tmp));
	}
}

template<Intrin::SimdBaseType T, usize Width>
auto And() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	T dataB[Width];
	T expected[Width];

	usize iMask = (sizeof(T) * 8) - 1;
	for (usize i = 0; i < Width; ++i)
	{
		dataA[i] = ~T(0);
		dataB[i] = T(T(1) << (i & iMask));
		expected[i] = dataA[i] & dataB[i];
	}
	
	PackT packA = PackT::Load(dataA);
	PackT packB = PackT::Load(dataB);
	PackT packRes = packA.And(packB);
	T res[Width];
	packRes.Store(res);
	
	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], expected[i]);
}

TEST(IntrinPack, And)
{
	u64 signF64 = 0x7FFF'FFFF'FFFF'FFFF;
	f64 maskF64 = *reinterpret_cast<f64*>(&signF64);
	And<f64, 2>({ -1, -2 },
				{ maskF64 , maskF64 });
	And<f64, 4>({ -1, -2, -3, -4 },
				{ maskF64 , maskF64, maskF64, maskF64 });

	u32 signF32 = 0x7FFF'FFFF;
	f32 maskF32 = *reinterpret_cast<f32*>(&signF32);
	And<f32, 4>({ -1, -2, -3, -4 },
				{ maskF32 , maskF32, maskF32, maskF32 });
	And<f32, 8>({ -1, -2, -3, -4, -5, -6, -7, -8 },
				{ maskF32 , maskF32, maskF32, maskF32, maskF32 , maskF32, maskF32, maskF32 });

	And<u64, 2>();
	And<u64, 4>();
	And<u32, 4>();
	And<u32, 8>();
	And<u16, 8>();
	And<u16, 16>();
	And<u8 , 16>();
	And<u8 , 32>();

	And<i64, 2>();
	And<i64, 4>();
	And<i32, 4>();
	And<i32, 8>();
	And<i16, 8>();
	And<i16, 16>();
	And<i8 , 16>();
	And<i8 , 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto AndNot(InitializerList<T> a, InitializerList<T> b) -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	Core::MemCpy(dataA, a.begin(), a.size() * sizeof(T));
	PackT packA = PackT::Load(dataA);
	T dataB[Width];
	Core::MemCpy(dataB, b.begin(), b.size() * sizeof(T));
	PackT packB = PackT::Load(dataB);

	PackT packRes = packA.AndNot(packB);
	T res[Width];
	packRes.Store(res);

	using Unsigned = Core::UnsignedOfSameSize<T>;
	for (usize i = 0; i < Width; ++i)
	{
		Unsigned tmpA = *reinterpret_cast<Unsigned*>(dataA + i);
		Unsigned tmpB = *reinterpret_cast<Unsigned*>(dataB + i);
		Unsigned tmp = tmpA &~ tmpB;
		ASSERT_EQ(res[i], *reinterpret_cast<T*>(&tmp));
	}
}

template<Intrin::SimdBaseType T, usize Width>
auto AndNot() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	T dataB[Width];
	T expected[Width];

	usize iMask = (sizeof(T) * 8) - 1;
	for (usize i = 0; i < Width; ++i)
	{
		dataA[i] = ~T(0);
		dataB[i] = T(T(1) << (i & iMask));
		expected[i] = dataA[i] & ~dataB[i];
	}

	PackT packA = PackT::Load(dataA);
	PackT packB = PackT::Load(dataB);
	PackT packRes = packA.AndNot(packB);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], expected[i]);
}

TEST(IntrinPack, AndNot)
{
	u64 signF64 = 0x8000'0000'0000'0000;
	f64 maskF64 = *reinterpret_cast<f64*>(&signF64);
	AndNot<f64, 2>({ -1, -2 },
				{ maskF64 , maskF64 });
	AndNot<f64, 4>({ -1, -2, -3, -4 },
				{ maskF64 , maskF64, maskF64, maskF64 });

	u32 signF32 = 0x8000'0000;
	f32 maskF32 = *reinterpret_cast<f32*>(&signF32);
	AndNot<f32, 4>({ -1, -2, -3, -4 },
				{ maskF32 , maskF32, maskF32, maskF32 });
	AndNot<f32, 8>({ -1, -2, -3, -4, -5, -6, -7, -8 },
				{ maskF32 , maskF32, maskF32, maskF32, maskF32 , maskF32, maskF32, maskF32 });

	AndNot<u64, 2>();
	AndNot<u64, 4>();
	AndNot<u32, 4>();
	AndNot<u32, 8>();
	AndNot<u16, 8>();
	AndNot<u16, 16>();
	AndNot<u8, 16>();
	AndNot<u8, 32>();

	AndNot<i64, 2>();
	AndNot<i64, 4>();
	AndNot<i32, 4>();
	AndNot<i32, 8>();
	AndNot<i16, 8>();
	AndNot<i16, 16>();
	AndNot<i8, 16>();
	AndNot<i8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto Xor(InitializerList<T> a, InitializerList<T> b) -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	Core::MemCpy(dataA, a.begin(), a.size() * sizeof(T));
	PackT packA = PackT::Load(dataA);
	T dataB[Width];
	Core::MemCpy(dataB, b.begin(), b.size() * sizeof(T));
	PackT packB = PackT::Load(dataB);

	PackT packRes = packA.Xor(packB);
	T res[Width];
	packRes.Store(res);

	using Unsigned = Core::UnsignedOfSameSize<T>;
	for (usize i = 0; i < Width; ++i)
	{
		Unsigned tmpA = *reinterpret_cast<Unsigned*>(dataA + i);
		Unsigned tmpB = *reinterpret_cast<Unsigned*>(dataB + i);
		Unsigned tmp = tmpA ^ tmpB;
		ASSERT_EQ(res[i], *reinterpret_cast<T*>(&tmp));
	}
}

template<Intrin::SimdBaseType T, usize Width>
auto Xor() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	T dataB[Width];
	T expected[Width];

	usize iMask = (sizeof(T) * 8) - 1;
	for (usize i = 0; i < Width; ++i)
	{
		dataA[i] = T(0x5555'5555'5555'5555);
		dataB[i] = T(T(1) << (i & iMask));
		expected[i] = dataA[i] ^ dataB[i];
	}

	PackT packA = PackT::Load(dataA);
	PackT packB = PackT::Load(dataB);
	PackT packRes = packA.Xor(packB);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], expected[i]);
}

TEST(IntrinPack, Xor)
{
	u64 signF64 = 0x8000'0000'0000'0000;
	f64 maskF64 = *reinterpret_cast<f64*>(&signF64);
	Xor<f64, 2>({ 1, -2 },
			   { maskF64 , maskF64 });
	Xor<f64, 4>({ 1, -2, 3, -4 },
			   { maskF64 , maskF64, maskF64, maskF64 });

	u32 signF32 = 0x8000'0000;
	f32 maskF32 = *reinterpret_cast<f32*>(&signF32);
	Xor<f32, 4>({ 1, -2, 3, -4 },
			   { maskF32 , maskF32, maskF32, maskF32 });
	Xor<f32, 8>({ 1, -2, 3, -4, 5, -6, 7, -8 },
			   { maskF32 , maskF32, maskF32, maskF32, maskF32 , maskF32, maskF32, maskF32 });

	Xor<u64, 2>();
	Xor<u64, 4>();
	Xor<u32, 4>();
	Xor<u32, 8>();
	Xor<u16, 8>();
	Xor<u16, 16>();
	Xor<u8, 16>();
	Xor<u8, 32>();

	Xor<i64, 2>();
	Xor<i64, 4>();
	Xor<i32, 4>();
	Xor<i32, 8>();
	Xor<i16, 8>();
	Xor<i16, 16>();
	Xor<i8, 16>();
	Xor<i8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto Or(InitializerList<T> a, InitializerList<T> b) -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	Core::MemCpy(dataA, a.begin(), a.size() * sizeof(T));
	PackT packA = PackT::Load(dataA);
	T dataB[Width];
	Core::MemCpy(dataB, b.begin(), b.size() * sizeof(T));
	PackT packB = PackT::Load(dataB);

	PackT packRes = packA.Or (packB);
	T res[Width];
	packRes.Store(res);

	using Unsigned = Core::UnsignedOfSameSize<T>;
	for (usize i = 0; i < Width; ++i)
	{
		Unsigned tmpA = *reinterpret_cast<Unsigned*>(dataA + i);
		Unsigned tmpB = *reinterpret_cast<Unsigned*>(dataB + i);
		Unsigned tmp = tmpA | tmpB;
		ASSERT_EQ(res[i], *reinterpret_cast<T*>(&tmp));
	}
}

template<Intrin::SimdBaseType T, usize Width>
auto Or() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	T dataB[Width];
	T expected[Width];

	usize iMask = (sizeof(T) * 8) - 1;
	for (usize i = 0; i < Width; ++i)
	{
		dataA[i] = ~T(0);
		dataB[i] = T(T(1) << (i & iMask));
		expected[i] = dataA[i] | dataB[i];
	}

	PackT packA = PackT::Load(dataA);
	PackT packB = PackT::Load(dataB);
	PackT packRes = packA.Or(packB);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], expected[i]);
}

TEST(IntrinPack, Or)
{
	u64 signF64 = 0x8000'0000'0000'0000;
	f64 maskF64 = *reinterpret_cast<f64*>(&signF64);
	Or<f64, 2>({ 1, 2 },
				{ maskF64 , maskF64 });
	Or<f64, 4>({ 1, 2, 3, 4 },
				{ maskF64 , maskF64, maskF64, maskF64 });

	u32 signF32 = 0x8000'0000;
	f32 maskF32 = *reinterpret_cast<f32*>(&signF32);
	Or<f32, 4>({ 1, 2, 3, 4 },
				{ maskF32 , maskF32, maskF32, maskF32 });
	Or<f32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 },
				{ maskF32 , maskF32, maskF32, maskF32, maskF32 , maskF32, maskF32, maskF32 });

	Or<u64, 2>();
	Or<u64, 4>();
	Or<u32, 4>();
	Or<u32, 8>();
	Or<u16, 8>();
	Or<u16, 16>();
	Or<u8, 16>();
	Or<u8, 32>();

	Or<i64, 2>();
	Or<i64, 4>();
	Or<i32, 4>();
	Or<i32, 8>();
	Or<i16, 8>();
	Or<i16, 16>();
	Or<i8, 16>();
	Or<i8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto Not(InitializerList<T> a) -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T data[Width];
	Core::MemCpy(data, a.begin(), a.size() * sizeof(T));
	PackT packA = PackT::Load(data);

	PackT packRes = packA.Not();
	T res[Width];
	packRes.Store(res);

	union FI { T f; Core::UnsignedOfSameSize<T> i; };
	for (usize i = 0; i < Width; ++i)
	{
		FI tmp;
		tmp.f = data[i];
		tmp.i = ~tmp.i;
		data[i] = tmp.f;
	}
	
	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], data[i]);
}

template<Intrin::SimdBaseType T, usize Width>
auto Not() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	T expected[Width];

	usize iMask = (sizeof(T) * 8) - 1;
	for (usize i = 0; i < Width; ++i)
	{
		dataA[i] = T(T(1) << (i & iMask));
		expected[i] = ~dataA[i];
	}

	PackT packA = PackT::Load(dataA);
	PackT packRes = packA.Not();
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], expected[i]);
}

TEST(IntrinPack, Not)
{
	Not<f64, 2>({ 1, 2 });
	Not<f64, 4>({ 1, 2, 3, 4 });
	
	Not<f32, 4>({ 1, 2, 3, 4 });
	Not<f32, 8>({ 1, 2, 3, 4, 5, 6, 7, 8 });

	Not<u64, 2>();
	Not<u64, 4>();
	Not<u32, 4>();
	Not<u32, 8>();
	Not<u16, 8>();
	Not<u16, 16>();
	Not<u8, 16>();
	Not<u8, 32>();

	Not<i64, 2>();
	Not<i64, 4>();
	Not<i32, 4>();
	Not<i32, 8>();
	Not<i16, 8>();
	Not<i16, 16>();
	Not<i8, 16>();
	Not<i8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto ShiftLPerElem() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	T dataB[Width];
	T expected[Width];

	usize iMask = (sizeof(T) * 8) - 1;
	for (usize i = 0; i < Width; ++i)
	{
		dataA[i] = ~T(0);
		dataB[i] = T(i) & iMask;
		expected[i] = dataA[i] << dataB[i];
	}

	PackT packA = PackT::Load(dataA);
	PackT packB = PackT::Load(dataB);
	PackT packRes = packA.ShiftL(packB);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], expected[i]);
}

TEST(IntrinPack, ShiftLPerElem)
{
	// Only need to check for unsigned, as signed and floating point use the same underlying code
	ShiftLPerElem<u64, 2>();
	ShiftLPerElem<u64, 4>();
	ShiftLPerElem<u32, 4>();
	ShiftLPerElem<u32, 8>();
	ShiftLPerElem<u16, 8>();
	ShiftLPerElem<u16, 16>();
	ShiftLPerElem<u8, 16>();
	ShiftLPerElem<u8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto ShiftLScalar() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	
	for (usize i = 0; i < Width; ++i)
	{
		dataA[i] = ~T(0);
	}

	PackT packA = PackT::Load(dataA);
	PackT packRes = packA.ShiftL(3u);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i) 
		ASSERT_EQ(res[i], T(dataA[i] << 3));
}

TEST(IntrinPack, ShiftLScalar)
{
	// Only need to check for unsigned, as signed and floating point use the same underlying code
	ShiftLScalar<u64, 2>();
	ShiftLScalar<u64, 4>();
	ShiftLScalar<u32, 4>();
	ShiftLScalar<u32, 8>();
	ShiftLScalar<u16, 8>();
	ShiftLScalar<u16, 16>();
	ShiftLScalar<u8, 16>();
	ShiftLScalar<u8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto ShiftLConst() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];

	for (usize i = 0; i < Width; ++i)
	{
		dataA[i] = ~T(0);
	}

	PackT packA = PackT::Load(dataA);
	PackT packRes = packA.template ShiftL<3>();
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], T(dataA[i] << 3));
}

TEST(IntrinPack, ShiftLConst)
{
	// Only need to check for unsigned, as signed and floating point use the same underlying code
	ShiftLConst<u64, 2>();
	ShiftLConst<u64, 4>();
	ShiftLConst<u32, 4>();
	ShiftLConst<u32, 8>();
	ShiftLConst<u16, 8>();
	ShiftLConst<u16, 16>();
	ShiftLConst<u8, 16>();
	ShiftLConst<u8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto ShiftRAPerElem() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	T dataB[Width];
	T expected[Width];

	usize iMask = (sizeof(T) * 8) - 1;
	for (usize i = 0; i < Width; ++i)
	{
		dataB[i] = T(i) & iMask;
		T bit = (1ull << iMask) >> dataB[i];
		dataA[i] = i % 2 ? (~T(0) ^ bit) : bit;
		expected[i] = dataA[i] >> dataB[i];
	}

	PackT packA = PackT::Load(dataA);
	PackT packB = PackT::Load(dataB);
	PackT packRes = packA.ShiftRA(packB);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], expected[i]);
}

TEST(IntrinPack, ShiftRAPerElem)
{
	// Only need to check for signed, as unsigned and floating point use the same underlying code
	ShiftRAPerElem<i64, 2>();
	ShiftRAPerElem<i64, 4>();
	ShiftRAPerElem<i32, 4>();
	ShiftRAPerElem<i32, 8>();
	ShiftRAPerElem<i16, 8>();
	ShiftRAPerElem<i16, 16>();
	ShiftRAPerElem<i8, 16>();
	ShiftRAPerElem<i8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto ShiftRAScalar() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];

	for (usize i = 0; i < Width; ++i)
	{
		dataA[i] = ~T(0);
	}

	PackT packA = PackT::Load(dataA);
	PackT packRes = packA.ShiftRA(3u);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], T(dataA[i] >> 3));
}

TEST(IntrinPack, ShiftRAScalar)
{
	// Only need to check for unsigned, as signed and floating point use the same underlying code
	ShiftRAScalar<i64, 2>();
	ShiftRAScalar<i64, 4>();
	ShiftRAScalar<i32, 4>();
	ShiftRAScalar<i32, 8>();
	ShiftRAScalar<i16, 8>();
	ShiftRAScalar<i16, 16>();
	ShiftRAScalar<i8, 16>();
	ShiftRAScalar<i8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto ShiftRAConst() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];

	for (usize i = 0; i < Width; ++i)
	{
		dataA[i] = ~T(0);
	}

	PackT packA = PackT::Load(dataA);
	PackT packRes = packA.template ShiftRA<3>();
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], T(dataA[i] >> 3));
}

TEST(IntrinPack, ShiftRAConst)
{
	// Only need to check for unsigned, as signed and floating point use the same underlying code
	ShiftRAConst<i64, 2>();
	ShiftRAConst<i64, 4>();
	ShiftRAConst<i32, 4>();
	ShiftRAConst<i32, 8>();
	ShiftRAConst<i16, 8>();
	ShiftRAConst<i16, 16>();
	ShiftRAConst<i8, 16>();
	ShiftRAConst<i8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto ShiftRLPerElem() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	T dataB[Width];
	T expected[Width];

	usize iMask = (sizeof(T) * 8) - 1;
	for (usize i = 0; i < Width; ++i)
	{
		dataA[i] = ~T(0);
		dataB[i] = T(i) & iMask;
		expected[i] = dataA[i] >> dataB[i];
	}

	PackT packA = PackT::Load(dataA);
	PackT packB = PackT::Load(dataB);
	PackT packRes = packA.ShiftRL(packB);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], expected[i]);
}

TEST(IntrinPack, ShiftRLPerElem)
{
	// Only need to check for unsigned, as signed and floating point use the same underlying code
	ShiftRLPerElem<u64, 2>();
	ShiftRLPerElem<u64, 4>();
	ShiftRLPerElem<u32, 4>();
	ShiftRLPerElem<u32, 8>();
	ShiftRLPerElem<u16, 8>();
	ShiftRLPerElem<u16, 16>();
	ShiftRLPerElem<u8, 16>();
	ShiftRLPerElem<u8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto ShiftRLScalar() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];

	for (usize i = 0; i < Width; ++i)
	{
		dataA[i] = ~T(0);
	}

	PackT packA = PackT::Load(dataA);
	PackT packRes = packA.ShiftRL(3u);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], T(dataA[i] >> 3));
}

TEST(IntrinPack, ShiftRLScalar)
{
	// Only need to check for unsigned, as signed and floating point use the same underlying code
	ShiftRLScalar<u64, 2>();
	ShiftRLScalar<u64, 4>();
	ShiftRLScalar<u32, 4>();
	ShiftRLScalar<u32, 8>();
	ShiftRLScalar<u16, 8>();
	ShiftRLScalar<u16, 16>();
	ShiftRLScalar<u8, 16>();
	ShiftRLScalar<u8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto ShiftRLConst() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];

	for (usize i = 0; i < Width; ++i)
	{
		dataA[i] = ~T(0);
	}

	PackT packA = PackT::Load(dataA);
	PackT packRes = packA.template ShiftRL<3>();
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], T(dataA[i] >> 3));
}

TEST(IntrinPack, ShiftRLConst)
{
	// Only need to check for unsigned, as signed and floating point use the same underlying code
	ShiftRLConst<u64, 2>();
	ShiftRLConst<u64, 4>();
	ShiftRLConst<u32, 4>();
	ShiftRLConst<u32, 8>();
	ShiftRLConst<u16, 8>();
	ShiftRLConst<u16, 16>();
	ShiftRLConst<u8, 16>();
	ShiftRLConst<u8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto BlendPack() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	T dataB[Width];
	T dataM[Width];
	T expected[Width];

	for (usize i = 0; i < Width; ++i)
	{
		dataA[i] = T(i);
		dataB[i] = T(Width + i);
		u64 bits = ~0ull * (i & 1);
		dataM[i] = *reinterpret_cast<T*>(&bits);
		expected[i] = i & 1 ? dataB[i] : dataA[i];
	}

	PackT packA = PackT::Load(dataA);
	PackT packB = PackT::Load(dataB);
	PackT mask = PackT::Load(dataM);
	PackT packRes = packA.Blend(packB, mask);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], expected[i]);
}

TEST(IntrinPack, BlendPack)
{
	BlendPack<f64, 2>();
	BlendPack<f64, 4>();
	BlendPack<f32, 4>();
	BlendPack<f32, 8>();

	BlendPack<u64, 2>();
	BlendPack<u64, 4>();
	BlendPack<u32, 4>();
	BlendPack<u32, 8>();
	BlendPack<u16, 8>();
	BlendPack<u16, 16>();
	BlendPack<u8, 16>();
	BlendPack<u8, 32>();

	BlendPack<i64, 2>();
	BlendPack<i64, 4>();
	BlendPack<i32, 4>();
	BlendPack<i32, 8>();
	BlendPack<i16, 8>();
	BlendPack<i16, 16>();
	BlendPack<i8, 16>();
	BlendPack<i8, 32>();
}


template<Intrin::SimdBaseType T, usize Width>
auto Min(InitializerList<T> a, InitializerList<T> b)
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	Core::MemCpy(dataA, a.begin(), a.size() * sizeof(T));
	PackT packA = PackT::Load(dataA);
	T dataB[Width];
	Core::MemCpy(dataB, b.begin(), b.size() * sizeof(T));
	PackT packB = PackT::Load(dataB);

	PackT packRes = packA.Min(packB);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], Core::Math::Min(dataA[i], dataB[i]));
}

TEST(IntrinPack, Min)
{
	Min<f64, 2>({ 1, 3 },
				{ 3, 2 });
	Min<f64, 4>({ 1, 3, 2, 4 },
				{ 3, 2, 4, 1 });
	Min<f32, 4>({ 1, 3, 2, 4 },
				{ 3, 2, 4, 1 });
	Min<f32, 8>({ 1, 3, 2, 4, 5, 7, 6, 8 },
				{ 3, 2, 4, 1, 7, 6, 8, 5 });

	Min<u64, 2>({ 1, 3 },
				{ 3, 2 });
	Min<u64, 4>({ 1, 3, 2, 4 },
				{ 3, 2, 4, 1 });
	Min<u32, 4>({ 1, 3, 2, 4 },
				{ 3, 2, 4, 1 });
	Min<u32, 8>({ 1, 3, 2, 4, 5, 7, 6, 8 },
				{ 3, 2, 4, 1, 7, 6, 8, 5 });
	Min<u16, 8>({ 1, 3, 2, 4, 5, 7, 6, 8 },
				{ 3, 2, 4, 1, 7, 6, 8, 5 });
	Min<u16, 16>({ 1, 3, 2, 4, 5, 7, 6, 8, 9, 11, 10, 12, 13, 15, 14, 16 },
				 { 3, 2, 4, 1, 7, 6, 8, 5, 11, 10, 12, 9, 15, 14, 16, 13 });
	Min<u8, 16>({ 1, 3, 2, 4, 5, 7, 6, 8, 9, 11, 10, 12, 13, 15, 14, 16 },
				{ 3, 2, 4, 1, 7, 6, 8, 5, 11, 10, 12, 9, 15, 14, 16, 13 });
	Min<u8, 32>({ 1, 3, 2, 4, 5, 7, 6, 8, 9, 11, 10, 12, 13, 15, 14, 16, 17, 19, 18, 20, 21, 23, 22, 24, 25, 27, 26, 28, 29, 31, 30, 32 },
				{ 3, 2, 4, 1, 7, 6, 8, 5, 11, 10, 12, 9, 15, 14, 16, 13, 19, 18, 20, 17, 23, 22, 24, 21, 27, 26, 28, 25, 31, 30, 32, 29 });

	Min<i64, 2>({ 1, 3 },
				{ 3, 2 });
	Min<i64, 4>({ 1, 3, 2, 4 },
				{ 3, 2, 4, 1 });
	Min<i32, 4>({ 1, 3, 2, 4 },
				{ 3, 2, 4, 1 });
	Min<i32, 8>({ 1, 3, 2, 4, 5, 7, 6, 8 },
				{ 3, 2, 4, 1, 7, 6, 8, 5 });
	Min<i16, 8>({ 1, 3, 2, 4, 5, 7, 6, 8 },
				{ 3, 2, 4, 1, 7, 6, 8, 5 });
	Min<i16, 16>({ 1, 3, 2, 4, 5, 7, 6, 8, 9, 11, 10, 12, 13, 15, 14, 16 },
				 { 3, 2, 4, 1, 7, 6, 8, 5, 11, 10, 12, 9, 15, 14, 16, 13 });
	Min<i8, 16>({ 1, 3, 2, 4, 5, 7, 6, 8, 9, 11, 10, 12, 13, 15, 14, 16 },
				{ 3, 2, 4, 1, 7, 6, 8, 5, 11, 10, 12, 9, 15, 14, 16, 13 });
	Min<i8, 32>({ 1, 3, 2, 4, 5, 7, 6, 8, 9, 11, 10, 12, 13, 15, 14, 16, 17, 19, 18, 20, 21, 23, 22, 24, 25, 27, 26, 28, 29, 31, 30, 32 },
				{ 3, 2, 4, 1, 7, 6, 8, 5, 11, 10, 12, 9, 15, 14, 16, 13, 19, 18, 20, 17, 23, 22, 24, 21, 27, 26, 28, 25, 31, 30, 32, 29 });
}

template<Intrin::SimdBaseType T, usize Width>
auto Max(InitializerList<T> a, InitializerList<T> b)
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	Core::MemCpy(dataA, a.begin(), a.size() * sizeof(T));
	PackT packA = PackT::Load(dataA);
	T dataB[Width];
	Core::MemCpy(dataB, b.begin(), b.size() * sizeof(T));
	PackT packB = PackT::Load(dataB);

	PackT packRes = packA.Max(packB);
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], Core::Math::Max(dataA[i], dataB[i]));
}

TEST(IntrinPack, Max)
{
	Max<f64, 2>({ 1, 3 },
				{ 3, 2 });
	Max<f64, 4>({ 1, 3, 2, 4 },
				{ 3, 2, 4, 1 });
	Max<f32, 4>({ 1, 3, 2, 4 },
				{ 3, 2, 4, 1 });
	Max<f32, 8>({ 1, 3, 2, 4, 5, 7, 6, 8 },
				{ 3, 2, 4, 1, 7, 6, 8, 5 });

	Max<u64, 2>({ 1, 3 },
				{ 3, 2 });
	Max<u64, 4>({ 1, 3, 2, 4 },
				{ 3, 2, 4, 1 });
	Max<u32, 4>({ 1, 3, 2, 4 },
				{ 3, 2, 4, 1 });
	Max<u32, 8>({ 1, 3, 2, 4, 5, 7, 6, 8 },
				{ 3, 2, 4, 1, 7, 6, 8, 5 });
	Max<u16, 8>({ 1, 3, 2, 4, 5, 7, 6, 8 },
				{ 3, 2, 4, 1, 7, 6, 8, 5 });
	Max<u16, 16>({ 1, 3, 2, 4, 5, 7, 6, 8, 9, 11, 10, 12, 13, 15, 14, 16 },
				 { 3, 2, 4, 1, 7, 6, 8, 5, 11, 10, 12, 9, 15, 14, 16, 13 });
	Max<u8, 16>({ 1, 3, 2, 4, 5, 7, 6, 8, 9, 11, 10, 12, 13, 15, 14, 16 },
				{ 3, 2, 4, 1, 7, 6, 8, 5, 11, 10, 12, 9, 15, 14, 16, 13 });
	Max<u8, 32>({ 1, 3, 2, 4, 5, 7, 6, 8, 9, 11, 10, 12, 13, 15, 14, 16, 17, 19, 18, 20, 21, 23, 22, 24, 25, 27, 26, 28, 29, 31, 30, 32 },
				{ 3, 2, 4, 1, 7, 6, 8, 5, 11, 10, 12, 9, 15, 14, 16, 13, 19, 18, 20, 17, 23, 22, 24, 21, 27, 26, 28, 25, 31, 30, 32, 29 });

	Max<i64, 2>({ 1, 3 },
				{ 3, 2 });
	Max<i64, 4>({ 1, 3, 2, 4 },
				{ 3, 2, 4, 1 });
	Max<i32, 4>({ 1, 3, 2, 4 },
				{ 3, 2, 4, 1 });
	Max<i32, 8>({ 1, 3, 2, 4, 5, 7, 6, 8 },
				{ 3, 2, 4, 1, 7, 6, 8, 5 });
	Max<i16, 8>({ 1, 3, 2, 4, 5, 7, 6, 8 },
				{ 3, 2, 4, 1, 7, 6, 8, 5 });
	Max<i16, 16>({ 1, 3, 2, 4, 5, 7, 6, 8, 9, 11, 10, 12, 13, 15, 14, 16 },
				 { 3, 2, 4, 1, 7, 6, 8, 5, 11, 10, 12, 9, 15, 14, 16, 13 });
	Max<i8, 16>({ 1, 3, 2, 4, 5, 7, 6, 8, 9, 11, 10, 12, 13, 15, 14, 16 },
				{ 3, 2, 4, 1, 7, 6, 8, 5, 11, 10, 12, 9, 15, 14, 16, 13 });
	Max<i8, 32>({ 1, 3, 2, 4, 5, 7, 6, 8, 9, 11, 10, 12, 13, 15, 14, 16, 17, 19, 18, 20, 21, 23, 22, 24, 25, 27, 26, 28, 29, 31, 30, 32 },
				{ 3, 2, 4, 1, 7, 6, 8, 5, 11, 10, 12, 9, 15, 14, 16, 13, 19, 18, 20, 17, 23, 22, 24, 21, 27, 26, 28, 25, 31, 30, 32, 29 });
}

template<Intrin::SimdBaseType T, usize Width>
auto Ceil()
{
	using PackT = Intrin::Pack<T, Width>;
	T data[Width];
	for (usize i = 0; i < Width; ++i)
	{
		T fract = T((i & 3) * 0.25);
		if (i & 1)
			data[i] = -(T(i) + fract);
		else
			data[i] = T(i) + fract;
	}
	PackT packA = PackT::Load(data);

	PackT packRes = packA.Ceil();
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_TRUE(Core::Math::EpsilonCompare(res[i], T(Core::Math::Ceil(data[i])), T(0.00001)));
}

TEST(IntrinPack, Ceil)
{
	Ceil<f64, 2>();
	Ceil<f64, 4>();
	Ceil<f32, 4>();
	Ceil<f32, 8>();
}

template<Intrin::SimdBaseType T, usize Width>
auto Floor()
{
	using PackT = Intrin::Pack<T, Width>;
	T data[Width];
	for (usize i = 0; i < Width; ++i)
	{
		T fract = T((i & 3) * 0.25);
		if (i & 1)
			data[i] = -(T(i) + fract);
		else
			data[i] = T(i) + fract;
	}
	PackT packA = PackT::Load(data);

	PackT packRes = packA.Floor();
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_TRUE(Core::Math::EpsilonCompare(res[i], T(Core::Math::Floor(data[i])), T(0.00001)));
}

TEST(IntrinPack, Floor)
{
	Floor<f64, 2>();
	Floor<f64, 4>();
	Floor<f32, 4>();
	Floor<f32, 8>();
}

template<Intrin::SimdBaseType T, usize Width>
auto Round()
{
	using PackT = Intrin::Pack<T, Width>;
	T data[Width];
	for (usize i = 0; i < Width; ++i)
	{
		T fract = T((i & 1) * 0.5);
		if (i & 1)
			data[i] = -T(i + fract);
		else
			data[i] = T(i) + fract;
	}
	PackT packA = PackT::Load(data);

	PackT packRes = packA.Round();
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_TRUE(Core::Math::EpsilonCompare(res[i], T(Core::Math::Round(data[i])), T(0.00001)));
}

TEST(IntrinPack, Round)
{
	Round<f64, 2>();
	Round<f64, 4>();
	Round<f32, 4>();
	Round<f32, 8>();
}

template<Intrin::SimdBaseType T, usize Width>
auto RoundEven()
{
	using PackT = Intrin::Pack<T, Width>;
	T data[Width];
	for (usize i = 0; i < Width; ++i)
	{
		T fract = T((i & 1) * 0.5);
		if (i & 1)
			data[i] = -T(i + fract);
		else
			data[i] = T(i) + fract;
	}
	PackT packA = PackT::Load(data);

	PackT packRes = packA.RoundEven();
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_TRUE(Core::Math::EpsilonCompare(res[i], T(Core::Math::RoundEven(data[i])), T(0.00001)));
}

TEST(IntrinPack, RoundEven)
{
	RoundEven<f64, 2>();
	RoundEven<f64, 4>();
	RoundEven<f32, 4>();
	RoundEven<f32, 8>();
}

template<Intrin::SimdBaseType T, usize Width>
auto Rcp()
{
	using PackT = Intrin::Pack<T, Width>;
	T data[Width];
	for (usize i = 0; i < Width; ++i)
		data[i] = T(i + 1);
	PackT packA = PackT::Load(data);

	PackT packRes = packA.Rcp();
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_TRUE(Core::Math::EpsilonCompare(res[i], Core::Math::Rcp(data[i]), T(0.00001)));
}

TEST(IntrinPack, Rcp)
{
	Rcp<f64, 2>();
	Rcp<f64, 4>();
	Rcp<f32, 4>();
	Rcp<f32, 8>();

	Rcp<u64, 2>();
	Rcp<u64, 4>();
	Rcp<u32, 4>();
	Rcp<u32, 8>();
	Rcp<u16, 8>();
	Rcp<u16, 16>();
	Rcp<u8, 16>();
	Rcp<u8, 32>();

	Rcp<i64, 2>();
	Rcp<i64, 4>();
	Rcp<i32, 4>();
	Rcp<i32, 8>();
	Rcp<i16, 8>();
	Rcp<i16, 16>();
	Rcp<i8, 16>();
	Rcp<i8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto Sqrt()
{
	using PackT = Intrin::Pack<T, Width>;
	T data[Width];
	for (usize i = 0; i < Width; ++i)
		data[i] = T(i);
	PackT packA = PackT::Load(data);

	PackT packRes = packA.Sqrt();
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_TRUE(Core::Math::EpsilonCompare(res[i], Core::Math::Sqrt(data[i]), T(0.00001)));
}

TEST(IntrinPack, Sqrt)
{
	Sqrt<f64, 2>();
	Sqrt<f64, 4>();
	Sqrt<f32, 4>();
	Sqrt<f32, 8>();

	Sqrt<u64, 2>();
	Sqrt<u64, 4>();
	Sqrt<u32, 4>();
	Sqrt<u32, 8>();
	Sqrt<u16, 8>();
	Sqrt<u16, 16>();
	Sqrt<u8, 16>();
	Sqrt<u8, 32>();

	Sqrt<i64, 2>();
	Sqrt<i64, 4>();
	Sqrt<i32, 4>();
	Sqrt<i32, 8>();
	Sqrt<i16, 8>();
	Sqrt<i16, 16>();
	Sqrt<i8, 16>();
	Sqrt<i8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto RSqrt()
{
	using PackT = Intrin::Pack<T, Width>;
	T data[Width];
	for (usize i = 0; i < Width; ++i)
		data[i] = T(i + 1);
	PackT packA = PackT::Load(data);

	PackT packRes = packA.RSqrt();
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_TRUE(Core::Math::EpsilonCompare(res[i], Core::Math::RSqrt(data[i]), T(0.00001)));
}

TEST(IntrinPack, RSqrt)
{
	RSqrt<f64, 2>();
	RSqrt<f64, 4>();
	RSqrt<f32, 4>();
	RSqrt<f32, 8>();

	RSqrt<u64, 2>();
	RSqrt<u64, 4>();
	RSqrt<u32, 4>();
	RSqrt<u32, 8>();
	RSqrt<u16, 8>();
	RSqrt<u16, 16>();
	RSqrt<u8, 16>();
	RSqrt<u8, 32>();

	RSqrt<i64, 2>();
	RSqrt<i64, 4>();
	RSqrt<i32, 4>();
	RSqrt<i32, 8>();
	RSqrt<i16, 8>();
	RSqrt<i16, 16>();
	RSqrt<i8, 16>();
	RSqrt<i8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto Abs() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];

	for (usize i = 0; i < Width; ++i)
	{
		if constexpr (Core::UnsignedIntegral<T>)
			dataA[i] = T(i);
		else
			dataA[i] = -T(i);
	}

	PackT packA = PackT::Load(dataA);
	PackT packRes = packA.Abs();
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], Core::Math::Abs(dataA[i]));
}

TEST(IntrinPack, Abs)
{
	Abs<f64, 2>();
	Abs<f64, 4>();
	Abs<f32, 4>();
	Abs<f32, 8>();

	Abs<u64, 2>();
	Abs<u64, 4>();
	Abs<u32, 4>();
	Abs<u32, 8>();
	Abs<u16, 8>();
	Abs<u16, 16>();
	Abs<u8, 16>();
	Abs<u8, 32>();

	Abs<i64, 2>();
	Abs<i64, 4>();
	Abs<i32, 4>();
	Abs<i32, 8>();
	Abs<i16, 8>();
	Abs<i16, 16>();
	Abs<i8, 16>();
	Abs<i8, 32>();
}

template<Intrin::SimdBaseType T, usize Width>
auto Sign() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];

	i8 vals[3] = { -23, 0, 42 };

	for (usize i = 0; i < Width; ++i)
	{
		dataA[i] = T(vals[i % 3]);
	}

	PackT packA = PackT::Load(dataA);
	PackT packRes = packA.Sign();
	T res[Width];
	packRes.Store(res);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], Core::Math::Sign(dataA[i]));
}

TEST(IntrinPack, Sign)
{
	Sign<f64, 2>();
	Sign<f64, 4>();
	Sign<f32, 4>();
	Sign<f32, 8>();

	Sign<u64, 2>();
	Sign<u64, 4>();
	Sign<u32, 4>();
	Sign<u32, 8>();
	Sign<u16, 8>();
	Sign<u16, 16>();
	Sign<u8, 16>();
	Sign<u8, 32>();

	Sign<i64, 2>();
	Sign<i64, 4>();
	Sign<i32, 4>();
	Sign<i32, 8>();
	Sign<i16, 8>();
	Sign<i16, 16>();
	Sign<i8, 16>();
	Sign<i8, 32>();
}

template<Intrin::SimdBaseType T, usize Width, usize NumElems = Width>
auto HAdd() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T data[Width];
	for (usize i = 0; i < Width; ++i)
		data[i] = T(i);
	PackT pack = PackT::Load(data);

	PackT packRes = pack.template HAdd<NumElems>();
	T res[Width];
	packRes.Store(res);

	T sum = T(0);
	for (usize i = 0; i < NumElems; ++i)
		sum += T(i);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], sum);
}

TEST(IntrinPack, HAdd)
{
	HAdd<f64, 2>();
	HAdd<f64, 4>();
	HAdd<f32, 4>();
	HAdd<f32, 8>();
	
	HAdd<u64, 2>();
	HAdd<u64, 4>();
	HAdd<u32, 4>();
	HAdd<u32, 8>();
	HAdd<u16, 8>();
	HAdd<u16, 16>();
	HAdd<u8, 16>();
	HAdd<u8, 32>();

	HAdd<i64, 2>();
	HAdd<i64, 4>();
	HAdd<i32, 4>();
	HAdd<i32, 8>();
	HAdd<i16, 8>();
	HAdd<i16, 16>();
	HAdd<i8, 16>();
	HAdd<i8, 32>();

	HAdd<f64, 2, 1>();
	HAdd<f64, 4, 3>();
	HAdd<f32, 4, 3>();
	HAdd<f32, 8, 7>();

	HAdd<u64, 2, 1>();
	HAdd<u64, 4, 2>();
	HAdd<u32, 4, 3>();
	HAdd<u32, 8, 7>();
	HAdd<u16, 8, 7>();
	HAdd<u16, 16, 15>();
	HAdd<u8, 16, 15>();
	HAdd<u8, 32, 31>();

	HAdd<i64, 2, 1>();
	HAdd<i64, 4, 2>();
	HAdd<i32, 4, 3>();
	HAdd<i32, 8, 7>();
	HAdd<i16, 8, 7>();
	HAdd<i16, 16, 15>();
	HAdd<i8, 16, 15>();
	HAdd<i8, 32, 31>();
}

template<Intrin::SimdBaseType T, usize Width, usize NumElems = Width>
auto Dot() -> void
{
	using PackT = Intrin::Pack<T, Width>;
	T dataA[Width];
	T dataB[Width];
	for (usize i = 0; i < Width; ++i)
	{
		dataA[i] = T(i);
		dataB[i] = T(i * 2);
	}
	PackT packA = PackT::Load(dataA);
	PackT packB = PackT::Load(dataB);

	PackT packRes = packA.template Dot<NumElems>(packB);
	T res[Width];
	packRes.Store(res);

	T sum = T(0);
	for (usize i = 0; i < NumElems; ++i)
		sum += T(i) * T(i * 2);

	for (usize i = 0; i < Width; ++i)
		ASSERT_EQ(res[i], sum);
}

TEST(IntrinPack, Dot)
{
	Dot<f64, 2>();
	Dot<f64, 4>();
	Dot<f32, 4>();
	Dot<f32, 8>();
	
	Dot<u64, 2>();
	Dot<u64, 4>();
	Dot<u32, 4>();
	Dot<u32, 8>();
	Dot<u16, 8>();
	Dot<u16, 16>();
	Dot<u8, 16>();
	Dot<u8, 32>();
	
	Dot<i64, 2>();
	Dot<i64, 4>();
	Dot<i32, 4>();
	Dot<i32, 8>();
	Dot<i16, 8>();
	Dot<i16, 16>();
	Dot<i8, 16>();
	Dot<i8, 32>();

	Dot<f64, 2, 1>();
	Dot<f64, 4, 3>();
	Dot<f32, 4, 3>();
	Dot<f32, 8, 7>();
	Dot<f32, 8, 3>();

	Dot<u64, 2, 1>();
	Dot<u64, 4, 2>();
	Dot<u32, 4, 3>();
	Dot<u32, 8, 7>();
	Dot<u16, 8, 7>();
	Dot<u16, 16, 15>();
	Dot<u8, 16, 15>();
	Dot<u8, 32, 31>();
	
	Dot<i64, 2, 1>();
	Dot<i64, 4, 2>();
	Dot<i32, 4, 3>();
	Dot<i32, 8, 7>();
	Dot<i16, 8, 7>();
	Dot<i16, 16, 15>();
	Dot<i8, 16, 15>();
	Dot<i8, 32, 31>();
}