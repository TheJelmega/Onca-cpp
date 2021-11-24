#include "gtest/gtest.h"
#include "core/Core.h"

namespace Math = Core::Math;

TEST(Mat3, DefaultInit)
{
	f32m3 mat;
	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 0);
	ASSERT_EQ(mat.m02, 0);
	ASSERT_EQ(mat.m10, 0);
	ASSERT_EQ(mat.m11, 1);
	ASSERT_EQ(mat.m12, 0);
	ASSERT_EQ(mat.m20, 0);
	ASSERT_EQ(mat.m21, 0);
	ASSERT_EQ(mat.m22, 1);
}

TEST(Mat3, DiagInit)
{
	f32m3 mat{ 2 };
	ASSERT_EQ(mat.m00, 2);
	ASSERT_EQ(mat.m01, 0);
	ASSERT_EQ(mat.m02, 0);
	ASSERT_EQ(mat.m10, 0);
	ASSERT_EQ(mat.m11, 2);
	ASSERT_EQ(mat.m12, 0);
	ASSERT_EQ(mat.m20, 0);
	ASSERT_EQ(mat.m21, 0);
	ASSERT_EQ(mat.m22, 2);
}

TEST(Mat3, ComponentInit)
{
	f32m3 mat{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m10, 4);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 6);
	ASSERT_EQ(mat.m20, 7);
	ASSERT_EQ(mat.m21, 8);
	ASSERT_EQ(mat.m22, 9);
}

TEST(Mat3, RowInit)
{
	f32m3 mat{ f32v3{ 1, 2, 3 }, f32v3{ 4, 5, 6 }, f32v3{ 7, 8, 9 } };
	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m10, 4);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 6);
	ASSERT_EQ(mat.m20, 7);
	ASSERT_EQ(mat.m21, 8);
	ASSERT_EQ(mat.m22, 9);
}

TEST(Mat3, ColumnInit)
{
	f32m3 mat{ Math::ColumnInit, f32v3{ 1, 2, 3 }, f32v3{ 4, 5, 6 }, f32v3{ 7, 8, 9 } };
	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 4);
	ASSERT_EQ(mat.m02, 7);
	ASSERT_EQ(mat.m10, 2);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 8);
	ASSERT_EQ(mat.m20, 3);
	ASSERT_EQ(mat.m21, 6);
	ASSERT_EQ(mat.m22, 9);
}

TEST(Mat3, CopyInit)
{
	f32m3 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	f32m3 mat{ src };

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m10, 4);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 6);
	ASSERT_EQ(mat.m20, 7);
	ASSERT_EQ(mat.m21, 8);
	ASSERT_EQ(mat.m22, 9);
}

TEST(Mat3, CopyDiffTypeInit)
{
	i32m3 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	f32m3 mat{ src };

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m10, 4);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 6);
	ASSERT_EQ(mat.m20, 7);
	ASSERT_EQ(mat.m21, 8);
	ASSERT_EQ(mat.m22, 9);
}

TEST(Mat3, MoveInit)
{
	f32m3 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	f32m3 mat{ Core::Move(src) };

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m10, 4);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 6);
	ASSERT_EQ(mat.m20, 7);
	ASSERT_EQ(mat.m21, 8);
	ASSERT_EQ(mat.m22, 9);
}

TEST(Mat3, CopyAssign)
{
	f32m3 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	f32m3 mat = src;

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m10, 4);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 6);
	ASSERT_EQ(mat.m20, 7);
	ASSERT_EQ(mat.m21, 8);
	ASSERT_EQ(mat.m22, 9);
}

TEST(Mat3, CopyDiffTypeAssign)
{
	i32m3 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	f32m3 mat = src;

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m10, 4);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 6);
	ASSERT_EQ(mat.m20, 7);
	ASSERT_EQ(mat.m21, 8);
	ASSERT_EQ(mat.m22, 9);
}

TEST(Mat3, MoveAssign)
{
	f32m3 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	f32m3 mat = Core::Move(src);

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m10, 4);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 6);
	ASSERT_EQ(mat.m20, 7);
	ASSERT_EQ(mat.m21, 8);
	ASSERT_EQ(mat.m22, 9);
}

TEST(Mat3, Compare)
{
	ASSERT_EQ((f32m3{ 1, 2, 3, 4, 5, 6, 7, 8, 9 }), (f32m3{ 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
	ASSERT_NE((f32m3{ 1, 2, 3, 4, 5, 6, 7, 8, 9 }), (f32m3{ 1, 2, 3, 4, 5, 6, 4, 8, 9 }));
	ASSERT_TRUE((f32m3{ 1, 2, 3, 4, 5, 6, 7, 8, 9 }).Compare(f32m3{ 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
}

TEST(Mat3, IndexGet)
{
	f32m3 mat{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	ASSERT_EQ(mat.Row(1), (f32v3{ 4, 5, 6 }));
	ASSERT_EQ(mat.Column(0), (f32v3{ 1, 4, 7 }));
	ASSERT_EQ(mat.Diagonal(), (f32v3{ 1, 5, 9 }));
	ASSERT_EQ(mat(0, 1), 2);
}

TEST(Mat3, IndexSet)
{
	f32m3 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	f32m3 mat = src;
	mat.SetRow(0, f32v3{ 10, 11, 12 });
	ASSERT_EQ(mat, (f32m3{ 10, 11, 12, 4, 5, 6, 7, 8, 9 }));

	mat = src;
	mat.SetColumn(1, f32v3{ 10, 11, 12 });
	ASSERT_EQ(mat, (f32m3{ 1, 10, 3, 4, 11, 6, 7, 12, 9 }));

	mat = src;
	mat(1, 1) = 42;
}

TEST(Mat3, AddSub)
{
	f32m3 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	f32m3 b{ 2, 4, 6, 8, 10, 12, 14, 16, 18 };

	ASSERT_EQ(a + b, (f32m3{ 3,  6,  9,  12, 15, 18, 21, 24, 27 }));
	ASSERT_EQ(a - b, (f32m3{ -1, -2, -3, -4, -5, -6, -7, -8, -9}));
}

TEST(Mat3, MulDivScalar)
{
	f32m3 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	ASSERT_EQ(a * 2, (f32m3{ 2  , 4, 6  , 8, 10 , 12, 14 , 16, 18 }));
	ASSERT_EQ(a / 2, (f32m3{  .5, 1, 1.5, 2, 2.5, 3 , 3.5, 4 , 4.5 }));
}

TEST(Mat3, Mul)
{
	f32m3 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	f32m3 b{ 2, 4, 6, 8, 10, 12, 14, 16, 18 };

	ASSERT_EQ(a * b, (f32m3{ 60, 72, 84, 132, 162, 192, 204, 252, 300 }));
}

TEST(Mat3, AddSubAssign)
{
	f32m3 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	f32m3 b{ 2, 4, 6, 8, 10, 12, 14, 16, 18 };

	f32m3 tmp = a;
	tmp += b;
	ASSERT_EQ(tmp, (f32m3{ 3,  6,  9,  12, 15, 18, 21, 24, 27 }));

	tmp = a;
	tmp -= b;
	ASSERT_EQ(a - b, (f32m3{ -1, -2, -3, -4, -5, -6, -7, -8, -9 }));
}

TEST(Mat3, MulDivAssignScalar)
{
	f32m3 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	f32m3 tmp = a;
	tmp *= 2;
	ASSERT_EQ(tmp, (f32m3{ 2, 4, 6, 8, 10, 12, 14, 16, 18 }));

	tmp = a;
	tmp /= 2;
	ASSERT_EQ(tmp, (f32m3{ .5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5 }));
}

TEST(Mat3, MulAssign)
{
	f32m3 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	f32m3 b{ 2, 4, 6, 8, 10, 12, 14, 16, 18 };

	a *= b;
	ASSERT_EQ(a, (f32m3{ 60, 72, 84, 132, 162, 192, 204, 252, 300 }));
}

TEST(Mat3, Determinant)
{
	f32m3 a{ 1, 2, 3, 4, 5, 6, 7, 8, 10 };
	ASSERT_EQ(a.Determinant(), -3);
}

TEST(Mat3, Transpose)
{
	f32m3 a{ 1, 2, 3, 4, 5, 6, 7, 8, 10 };

	ASSERT_EQ(a.Transposed(), (f32m3{ 1, 4, 7, 2, 5, 8, 3, 6, 10 }));
	a.Transpose();
	ASSERT_EQ(a, (f32m3{ 1, 4, 7, 2, 5, 8, 3, 6, 10 }));
}

TEST(Mat3, Adjugate)
{
	f32m3 a{ 1, 2, 3, 4, 5, 6, 7, 8, 10 };

	ASSERT_EQ(a.Adjoint(), (f32m3{ 2, 4, -3, 2, -11, 6, -3, 6, -3 }));
	a.Adjugate();
	ASSERT_EQ(a, (f32m3{ 2, 4, -3, 2, -11, 6, -3, 6, -3 }));
}

TEST(Mat3, Cofactor)
{
	f32m3 a{ 1, 2, 3, 4, 5, 6, 7, 8, 10 };

	ASSERT_EQ(a.Cofactor(), (f32m3{ 2, 2, -3, 4, -11, 6, -3, 6, -3 }));
	a.ToCofactor();
	ASSERT_EQ(a, (f32m3{ 2, 2, -3, 4, -11, 6, -3, 6, -3 }));
}

TEST(Mat3, Inverse)
{
	f32m3 orig{ 1, 2, 3, 4, 5, 6, 7, 8, 10 };
	f32m3 a = orig;

	ASSERT_TRUE(a.Inverse().Compare(f32m3{ -2.f/3.f, -4.f/3.f, 1.f, -2.f/3.f, 11.f/3.f, -2, 1, -2, 1 }));
	a.Invert();
	ASSERT_TRUE(a.Compare(f32m3{ -2.f / 3.f, -4.f / 3.f, 1.f, -2.f / 3.f, 11.f / 3.f, -2, 1, -2, 1 }));

	ASSERT_TRUE((orig * a             ).Compare(f32m3{}));
	ASSERT_TRUE((orig * orig.Inverse()).Compare(f32m3{}));
}

TEST(Mat3, Decompose)
{
	f32q srcQuat = f32q::AxisAngle(f32v3{ 1, 1, 1 }, 50_deg);
	f32m3 mat = f32m3::CreateTransform(f32v3{ 2, 3, 4 }, srcQuat);

	auto [scale, quat] = mat.Decompose();

	ASSERT_TRUE(scale.Compare(f32v3{ 2, 3, 4 }));
	ASSERT_TRUE(quat.Compare(srcQuat));
}

TEST(Mat3, Decompose2D)
{
	f32m3 mat = f32m3::CreateTransform2D(f32v2{ 2, 3 }, 50_deg, f32v2{ 5, 6 });

	auto [scale, angle, trans] = mat.Decompose2D();

	ASSERT_TRUE(scale.Compare(f32v2{ 2, 3 }));
	ASSERT_TRUE(angle.Compare(50_deg));
	ASSERT_EQ(trans, (f32v2{ 5, 6 }));
}