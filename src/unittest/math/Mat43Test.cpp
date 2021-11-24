#include "gtest/gtest.h"
#include "core/Core.h"

namespace Math = Core::Math;

TEST(Mat43, DefaultInit)
{
	f32m43 mat;
	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 0);
	ASSERT_EQ(mat.m02, 0);
	ASSERT_EQ(mat.m10, 0);
	ASSERT_EQ(mat.m11, 1);
	ASSERT_EQ(mat.m12, 0);
	ASSERT_EQ(mat.m20, 0);
	ASSERT_EQ(mat.m21, 0);
	ASSERT_EQ(mat.m22, 1);
	ASSERT_EQ(mat.m30, 0);
	ASSERT_EQ(mat.m31, 0);
	ASSERT_EQ(mat.m32, 0);
}

TEST(Mat43, DiagInit)
{
	f32m43 mat{ 2 };
	ASSERT_EQ(mat.m00, 2);
	ASSERT_EQ(mat.m01, 0);
	ASSERT_EQ(mat.m02, 0);
	ASSERT_EQ(mat.m10, 0);
	ASSERT_EQ(mat.m11, 2);
	ASSERT_EQ(mat.m12, 0);
	ASSERT_EQ(mat.m20, 0);
	ASSERT_EQ(mat.m21, 0);
	ASSERT_EQ(mat.m22, 2);
	ASSERT_EQ(mat.m30, 0);
	ASSERT_EQ(mat.m31, 0);
	ASSERT_EQ(mat.m32, 0);
}

TEST(Mat43, ComponentInit)
{
	f32m43 mat{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m10, 4);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 6);
	ASSERT_EQ(mat.m20, 7);
	ASSERT_EQ(mat.m21, 8);
	ASSERT_EQ(mat.m22, 9);
	ASSERT_EQ(mat.m30, 10);
	ASSERT_EQ(mat.m31, 11);
	ASSERT_EQ(mat.m32, 12);
}

TEST(Mat43, RowInit)
{
	f32m43 mat{ f32v3{ 1, 2, 3 }, f32v3{ 4, 5, 6 }, f32v3{ 7, 8, 9 }, f32v3{ 10, 11, 12 } };
	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m10, 4);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 6);
	ASSERT_EQ(mat.m20, 7);
	ASSERT_EQ(mat.m21, 8);
	ASSERT_EQ(mat.m22, 9);
	ASSERT_EQ(mat.m30, 10);
	ASSERT_EQ(mat.m31, 11);
	ASSERT_EQ(mat.m32, 12);
}

TEST(Mat43, ColumnInit)
{
	f32m43 mat{ Math::ColumnInit, f32v4{ 1, 2, 3, 4 }, f32v4{ 5, 6, 7, 8 }, f32v4{ 9, 10, 11, 12 } };
	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 5);
	ASSERT_EQ(mat.m02, 9);
	ASSERT_EQ(mat.m10, 2);
	ASSERT_EQ(mat.m11, 6);
	ASSERT_EQ(mat.m12, 10);
	ASSERT_EQ(mat.m20, 3);
	ASSERT_EQ(mat.m21, 7);
	ASSERT_EQ(mat.m22, 11);
	ASSERT_EQ(mat.m30, 4);
	ASSERT_EQ(mat.m31, 8);
	ASSERT_EQ(mat.m32, 12);
}

TEST(Mat43, CopyInit)
{
	f32m43 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	f32m43 mat{ src };

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m10, 4);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 6);
	ASSERT_EQ(mat.m20, 7);
	ASSERT_EQ(mat.m21, 8);
	ASSERT_EQ(mat.m22, 9);
	ASSERT_EQ(mat.m30, 10);
	ASSERT_EQ(mat.m31, 11);
	ASSERT_EQ(mat.m32, 12);
}

TEST(Mat43, CopyDiffTypeInit)
{
	i32m43 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	f32m43 mat{ src };

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m10, 4);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 6);
	ASSERT_EQ(mat.m20, 7);
	ASSERT_EQ(mat.m21, 8);
	ASSERT_EQ(mat.m22, 9);
	ASSERT_EQ(mat.m30, 10);
	ASSERT_EQ(mat.m31, 11);
	ASSERT_EQ(mat.m32, 12);
}

TEST(Mat43, MoveInit)
{
	f32m43 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	f32m43 mat{ Core::Move(src) };

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m10, 4);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 6);
	ASSERT_EQ(mat.m20, 7);
	ASSERT_EQ(mat.m21, 8);
	ASSERT_EQ(mat.m22, 9);
	ASSERT_EQ(mat.m30, 10);
	ASSERT_EQ(mat.m31, 11);
	ASSERT_EQ(mat.m32, 12);
}

TEST(Mat43, CopyAssign)
{
	f32m43 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	f32m43 mat = src;

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m10, 4);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 6);
	ASSERT_EQ(mat.m20, 7);
	ASSERT_EQ(mat.m21, 8);
	ASSERT_EQ(mat.m22, 9);
	ASSERT_EQ(mat.m30, 10);
	ASSERT_EQ(mat.m31, 11);
	ASSERT_EQ(mat.m32, 12);
}

TEST(Mat43, CopyDiffTypeAssign)
{
	i32m43 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	f32m43 mat = src;

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m10, 4);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 6);
	ASSERT_EQ(mat.m20, 7);
	ASSERT_EQ(mat.m21, 8);
	ASSERT_EQ(mat.m22, 9);
	ASSERT_EQ(mat.m30, 10);
	ASSERT_EQ(mat.m31, 11);
	ASSERT_EQ(mat.m32, 12);
}

TEST(Mat43, MoveAssign)
{
	f32m43 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	f32m43 mat = Core::Move(src);

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m10, 4);
	ASSERT_EQ(mat.m11, 5);
	ASSERT_EQ(mat.m12, 6);
	ASSERT_EQ(mat.m20, 7);
	ASSERT_EQ(mat.m21, 8);
	ASSERT_EQ(mat.m22, 9);
	ASSERT_EQ(mat.m30, 10);
	ASSERT_EQ(mat.m31, 11);
	ASSERT_EQ(mat.m32, 12);
}

TEST(Mat43, Compare)
{
	ASSERT_EQ((f32m43{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }), (f32m43{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }));
	ASSERT_NE((f32m43{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }), (f32m43{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0 }));
	ASSERT_TRUE((f32m43{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }).Compare(f32m43{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }));
}

TEST(Mat43, IndexGet)
{
	f32m43 mat{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

	ASSERT_EQ(mat.Row(1), (f32v4{ 4, 5, 6, 0 }));
	ASSERT_EQ(mat.Column(0), (f32v4{ 1, 4, 7, 10 }));
	ASSERT_EQ(mat.Diagonal(), (f32v4{ 1, 5, 9, 1 }));
	ASSERT_EQ(mat(0, 1), 2);
}

TEST(Mat43, IndexSet)
{
	f32m43 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

	f32m43 mat = src;
	mat.SetRow(0, f32v3{ 40, 41, 42 });
	ASSERT_EQ(mat, (f32m43{ 40, 41, 42, 4, 5, 6, 7, 8, 9, 10, 11, 12 }));

	mat = src;
	mat.SetColumn(1, f32v4{ 40, 41, 42, 43 });
	ASSERT_EQ(mat, (f32m43{ 1, 40, 3, 4, 41, 6, 7, 42, 9, 10, 43, 12 }));

	mat = src;
	mat(1, 1) = 42;
}

TEST(Mat43, AddSub)
{
	f32m43 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	f32m43 b{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24 };

	ASSERT_EQ(a + b, (f32m43{ 3,  6,  9,  12, 15, 18, 21, 24, 27, 30, 33, 36 }));
	ASSERT_EQ(a - b, (f32m43{ -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 }));
}

TEST(Mat43, MulDivScalar)
{
	f32m43 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

	ASSERT_EQ(a * 2, (f32m43{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24 }));
	ASSERT_EQ(a / 2, (f32m43{ .5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6 }));
}

TEST(Mat43, Mul)
{
	f32m43 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	f32m43 b{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24 };

	ASSERT_EQ(a * b, (f32m43{ 60, 72, 84, 132, 162, 192, 204, 252, 300, 296, 364, 432 }));
}

TEST(Mat43, AddSubAssign)
{
	f32m43 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	f32m43 b{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24 };

	f32m43 tmp = a;
	tmp += b;
	ASSERT_EQ(tmp, (f32m43{ 3,  6,  9,  12, 15, 18, 21, 24, 27, 30, 33, 36 }));

	tmp = a;
	tmp -= b;
	ASSERT_EQ(a - b, (f32m43{ -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 }));
}

TEST(Mat43, MulDivAssignScalar)
{
	f32m43 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

	f32m43 tmp = a;
	tmp *= 2;
	ASSERT_EQ(tmp, (f32m43{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24 }));

	tmp = a;
	tmp /= 2;
	ASSERT_EQ(tmp, (f32m43{ .5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6 }));
}

TEST(Mat43, MulAssign)
{
	f32m43 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	f32m43 b{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24 };

	a *= b;
	ASSERT_EQ(a, (f32m43{ 60, 72, 84, 132, 162, 192, 204, 252, 300, 296, 364, 432 }));
} 

TEST(Mat43, Determinant)
{
	f32m43 a{ 1, 2, 5, 4, 4, 3, 1, 5, 2, 5, 3, 4 };
	ASSERT_EQ(a.Determinant(), 63);
}


TEST(Mat43, Transpose)
{
	f32m43 a{ 1, 2, 5, 4, 4, 3, 1, 5, 2, 5, 3, 4 };
	
	ASSERT_EQ(a.Transposed(), (f32m43{ 1, 4, 1, 2, 4, 5, 5, 3, 2, 0, 0, 0 }));
	a.Transpose();
	ASSERT_EQ(a, (f32m43{ 1, 4, 1, 2, 4, 5, 5, 3, 2, 0, 0, 0 }));
}

TEST(Mat43, Adjugate)
{
	f32m43 a{ 1, 2, 5, 4, 4, 3, 1, 5, 2, 5, 3, 4 };

	ASSERT_EQ(a.Adjoint(), (f32m43{ -7, 21, -14, -5, -3, 17, 16, -3, -4, -14, -84, 35 }));
	a.Adjugate();
	ASSERT_EQ(a, (f32m43{ -7, 21, -14, -5, -3, 17, 16, -3, -4, -14, -84, 35 }));
}

TEST(Mat43, Cofactor)
{
	f32m43 a{ 1, 2, 5, 4, 4, 3, 1, 5, 2, 5, 3, 4 };
	 
	ASSERT_EQ(a.Cofactor(), (f32m43{ -7, -5, 16, 21, -3, -3, -14, 17, -4, 0, 0, 0 }));
	a.ToCofactor();
	ASSERT_EQ(a, (f32m43{ -7, -5, 16, 21, -3, -3, -14, 17, -4, 0, 0, 0 }));
}

TEST(Mat43, Inverse)
{
	f32m43 orig{ 1, 2, 5, 4, 4, 3, 1, 5, 2, 5, 3, 4 };
	f32m43 a = orig;
	f32m43 res{ -7, 21, -14, -5, -3, 17, 16, -3, -4, -14, -84, 35 };
	res /= 63;

	ASSERT_TRUE(a.Inverse().Compare(res));
	a.Invert();
	ASSERT_TRUE(a.Compare(res));

	ASSERT_TRUE((orig * a).Compare(f32m43{}));
	ASSERT_TRUE((orig * orig.Inverse()).Compare(f32m43{}));
}
