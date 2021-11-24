#include "gtest/gtest.h"
#include "core/Core.h"

namespace Math = Core::Math;

TEST(Mat2, DefaultInit)
{
	f32m2 mat;
	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 0);
	ASSERT_EQ(mat.m10, 0);
	ASSERT_EQ(mat.m11, 1);
}

TEST(Mat2, DiagInit)
{
	f32m2 mat{ 2 };
	ASSERT_EQ(mat.m00, 2);
	ASSERT_EQ(mat.m01, 0);
	ASSERT_EQ(mat.m10, 0);
	ASSERT_EQ(mat.m11, 2);
}

TEST(Mat2, ComponentInit)
{
	f32m2 mat{ 1, 2, 3, 4 };
	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m10, 3);
	ASSERT_EQ(mat.m11, 4);
}

TEST(Mat2, RowInit)
{
	f32m2 mat{ f32v2{ 1, 2 }, f32v2{ 3, 4 } };
	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m10, 3);
	ASSERT_EQ(mat.m11, 4);
}

TEST(Mat2, ColumnInit)
{
	f32m2 mat{ Math::ColumnInit, f32v2{ 1, 2 }, f32v2{ 3, 4 } };
	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 3);
	ASSERT_EQ(mat.m10, 2);
	ASSERT_EQ(mat.m11, 4);
}

TEST(Mat2, CopyInit)
{
	f32m2 src{ 1, 2, 3, 4 };
	f32m2 mat{ src };

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m10, 3);
	ASSERT_EQ(mat.m11, 4);
}

TEST(Mat2, CopyDiffTypeInit)
{
	i32m2 src{ 1, 2, 3, 4 };
	f32m2 mat{ src };

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m10, 3);
	ASSERT_EQ(mat.m11, 4);
}

TEST(Mat2, MoveInit)
{
	f32m2 src{ 1, 2, 3, 4 };
	f32m2 mat{ Core::Move(src) };

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m10, 3);
	ASSERT_EQ(mat.m11, 4);
}

TEST(Mat2, CopyAssign)
{
	f32m2 src{ 1, 2, 3, 4 };
	f32m2 mat = src;

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m10, 3);
	ASSERT_EQ(mat.m11, 4);
}

TEST(Mat2, CopyDiffTypeAssign)
{
	i32m2 src{ 1, 2, 3, 4 };
	f32m2 mat = src;

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m10, 3);
	ASSERT_EQ(mat.m11, 4);
}

TEST(Mat2, MoveAssign)
{
	f32m2 src{ 1, 2, 3, 4 };
	f32m2 mat = Core::Move(src);

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m10, 3);
	ASSERT_EQ(mat.m11, 4);
}

TEST(Mat2, Compare)
{
	ASSERT_EQ((f32m2{ 1, 2, 3 ,4 }), (f32m2{ 1, 2, 3, 4 }));
	ASSERT_NE((f32m2{ 1, 2, 3 ,4 }), (f32m2{ 1, 2, 0, 4 }));
	ASSERT_TRUE((f32m2{ 1, 2, 3, 4 }).Compare(f32m2{ 1, 2, 3, 4 }));
}

TEST(Mat2, IndexGet)
{
	f32m2 mat{ 1, 2, 3, 4 };

	ASSERT_EQ(mat.Row(1), (f32v2{ 3, 4 }));
	ASSERT_EQ(mat.Column(0), (f32v2{ 1, 3 }));
	ASSERT_EQ(mat.Diagonal(), (f32v2{ 1, 4 }));
	ASSERT_EQ(mat(0, 1), 2);
}

TEST(Mat2, IndexSet)
{
	f32m2 src{ 1, 2, 3, 4 };

	f32m2 mat = src;
	mat.SetRow(0, f32v2{ 5, 6 });
	ASSERT_EQ(mat, (f32m2{ 5, 6, 3, 4 }));

	mat = src;
	mat.SetColumn(1, f32v2{ 5, 6 });
	ASSERT_EQ(mat, (f32m2{ 1, 5, 3, 6 }));

	mat = src;
	mat(1, 1) = 42;
}

TEST(Mat2, AddSub)
{
	f32m2 a{ 1, 2, 3, 4 };
	f32m2 b{ 2, 4, 6, 8 };

	ASSERT_EQ(a + b, (f32m2{  3,  6,  9,  12 }));
	ASSERT_EQ(a - b, (f32m2{ -1, -2, -3, -4  }));
}

TEST(Mat2, MulDivScalar)
{
	f32m2 a{ 1, 2, 3, 4 };

	ASSERT_EQ(a * 2, (f32m2{ 2  , 4, 6  , 8 }));
	ASSERT_EQ(a / 2, (f32m2{  .5, 1, 1.5, 2 }));
}

TEST(Mat2, Mul)
{
	f32m2 a{ 1, 2, 3, 4 };
	f32m2 b{ 2, 4, 6, 8 };

	ASSERT_EQ(a * b, (f32m2{ 14, 20, 30, 44 }));
}

TEST(Mat2, AddSubAssign)
{
	f32m2 a{ 1, 2, 3, 4 };
	f32m2 b{ 2, 4, 6, 8 };

	f32m2 tmp = a;
	tmp += b;
	ASSERT_EQ(tmp, (f32m2{ 3,  6,  9,  12 }));

	tmp = a;
	tmp -= b;
	ASSERT_EQ(a - b, (f32m2{ -1, -2, -3, -4 }));
}

TEST(Mat2, MulDivAssignScalar)
{
	f32m2 a{ 1, 2, 3, 4 };

	f32m2 tmp = a;
	tmp *= 2;
	ASSERT_EQ(tmp, (f32m2{ 2, 4, 6, 8 }));

	tmp = a;
	tmp /= 2;
	ASSERT_EQ(tmp, (f32m2{ .5, 1, 1.5, 2 }));
}

TEST(Mat2, MulAssign)
{
	f32m2 a{ 1, 2, 3, 4 };
	f32m2 b{ 2, 4, 6, 8 };

	a *= b;
	ASSERT_EQ(a, (f32m2{ 14, 20, 30, 44 }));
}

TEST(Mat2, Determinant)
{
	f32m2 a{ 1, 2, 3, 4 };
	ASSERT_EQ(a.Determinant(), -2);
}

TEST(Mat2, Transpose)
{
	f32m2 a{ 1, 2, 3, 4 };

	ASSERT_EQ(a.Transposed(), (f32m2{ 1, 3, 2, 4 }));
	a.Transpose();
	ASSERT_EQ(a, (f32m2{ 1, 3, 2, 4 }));
}

TEST(Mat2, Adjugate)
{
	f32m2 a{ 1, 2, 3, 4 };

	ASSERT_EQ(a.Adjoint(), (f32m2{ 4, -2, -3, 1 }));
	a.Adjugate();
	ASSERT_EQ(a, (f32m2{ 4, -2, -3, 1 }));
}

TEST(Mat2, Cofactor)
{
	f32m2 a{ 1, 2, 3, 4 };

	ASSERT_EQ(a.Cofactor(), (f32m2{ 4, -3, -2, 1 }));
	a.ToCofactor();
	ASSERT_EQ(a, (f32m2{ 4, -3, -2, 1 }));
}

TEST(Mat2, Inverse)
{
	f32m2 orig{ 1, 2, 3, 4 };
	f32m2 a = orig;

	ASSERT_EQ(a.Inverse(), (f32m2{ -2, 1, 1.5, -0.5 }));
	a.Invert();
	ASSERT_EQ(a, (f32m2{ -2, 1, 1.5, -0.5 }));

	ASSERT_TRUE((orig * a             ).Compare(f32m2{}));
	ASSERT_TRUE((orig * orig.Inverse()).Compare(f32m2{}));
}

TEST(Mat2, TransformVector)
{
	f32m2 a;
	f32m2 b{ 0, 1, 1, 0 };
	f32m2 c{ 1, 1, 1, 1 };

	f32v2 v{ 1, 2 };

	ASSERT_EQ(a.TransformVector(v), (f32v2{ 1, 2 }));
	ASSERT_EQ(b.TransformVector(v), (f32v2{ 2, 1 }));
	ASSERT_EQ(c.TransformVector(v), (f32v2{ 3, 3 }));
}

TEST(Mat2, CreateScale)
{
	ASSERT_EQ(f32m2::CreateScale(2), (f32m2{ 2 }));
	ASSERT_EQ(f32m2::CreateScale(f32v2{ 2, 3 }), (f32m2{ 2, 0, 0, 3 }));
}

TEST(Mat2, CreateRotation)
{
	ASSERT_EQ(f32m2::CreateRotation(0_deg) , (f32m2{ 1,  0, 0, 1 }));
	ASSERT_TRUE(f32m2::CreateRotation(90_deg).Compare(f32m2{ 0, -1, 1, 0 }));
}

TEST(Mat2, CreateShear)
{
	ASSERT_EQ(f32m2::CreateShear(f32v2{ 2, 3 }), (f32m2{ 1, 3, 2, 1 }));
}

TEST(Mat2, CreateTransform)
{
	ASSERT_TRUE(f32m2::CreateTransform(2, 90_deg).Compare(f32m2{ 0, -2, 2, 0 }));
	ASSERT_TRUE(f32m2::CreateTransform(f32v2{ 2, 3 }, 90_deg).Compare(f32m2{ 0, -3, 2, 0 }));
}

TEST(Mat2, Decompose)
{
	f32m2 mat = f32m2::CreateTransform(f32v2{ 2, 3 }, 50_deg);

	auto [scale, angle] = mat.Decompose();

	ASSERT_EQ(scale, (f32v2{ 2, 3 }));
	ASSERT_TRUE(angle.Compare(50_deg));
}