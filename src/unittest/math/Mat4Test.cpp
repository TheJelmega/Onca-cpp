#include "gtest/gtest.h"
#include "core/Core.h"

namespace Math = Core::Math;

TEST(Mat4, DefaultInit)
{
	f32m4 mat;
	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 0);
	ASSERT_EQ(mat.m02, 0);
	ASSERT_EQ(mat.m03, 0);
	ASSERT_EQ(mat.m10, 0);
	ASSERT_EQ(mat.m11, 1);
	ASSERT_EQ(mat.m12, 0);
	ASSERT_EQ(mat.m13, 0);
	ASSERT_EQ(mat.m20, 0);
	ASSERT_EQ(mat.m21, 0);
	ASSERT_EQ(mat.m22, 1);
	ASSERT_EQ(mat.m23, 0);
	ASSERT_EQ(mat.m30, 0);
	ASSERT_EQ(mat.m31, 0);
	ASSERT_EQ(mat.m32, 0);
	ASSERT_EQ(mat.m33, 1);
}

TEST(Mat4, DiagInit)
{
	f32m4 mat{ 2 };
	ASSERT_EQ(mat.m00, 2);
	ASSERT_EQ(mat.m01, 0);
	ASSERT_EQ(mat.m02, 0);
	ASSERT_EQ(mat.m03, 0);
	ASSERT_EQ(mat.m10, 0);
	ASSERT_EQ(mat.m11, 2);
	ASSERT_EQ(mat.m12, 0);
	ASSERT_EQ(mat.m13, 0);
	ASSERT_EQ(mat.m20, 0);
	ASSERT_EQ(mat.m21, 0);
	ASSERT_EQ(mat.m22, 2);
	ASSERT_EQ(mat.m23, 0);
	ASSERT_EQ(mat.m30, 0);
	ASSERT_EQ(mat.m31, 0);
	ASSERT_EQ(mat.m32, 0);
	ASSERT_EQ(mat.m33, 2);
}

TEST(Mat4, ComponentInit)
{
	f32m4 mat{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m03, 4);
	ASSERT_EQ(mat.m10, 5);
	ASSERT_EQ(mat.m11, 6);
	ASSERT_EQ(mat.m12, 7);
	ASSERT_EQ(mat.m13, 8);
	ASSERT_EQ(mat.m20, 9);
	ASSERT_EQ(mat.m21, 10);
	ASSERT_EQ(mat.m22, 11);
	ASSERT_EQ(mat.m23, 12);
	ASSERT_EQ(mat.m30, 13);
	ASSERT_EQ(mat.m31, 14);
	ASSERT_EQ(mat.m32, 15);
	ASSERT_EQ(mat.m33, 16);
}

TEST(Mat4, RowInit)
{
	f32m4 mat{ f32v4{ 1, 2, 3, 4 }, f32v4{ 5, 6, 7, 8 }, f32v4{ 9, 10, 11, 12 }, f32v4{ 13, 14, 15, 16 } };
	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m03, 4);
	ASSERT_EQ(mat.m10, 5);
	ASSERT_EQ(mat.m11, 6);
	ASSERT_EQ(mat.m12, 7);
	ASSERT_EQ(mat.m13, 8);
	ASSERT_EQ(mat.m20, 9);
	ASSERT_EQ(mat.m21, 10);
	ASSERT_EQ(mat.m22, 11);
	ASSERT_EQ(mat.m23, 12);
	ASSERT_EQ(mat.m30, 13);
	ASSERT_EQ(mat.m31, 14);
	ASSERT_EQ(mat.m32, 15);
	ASSERT_EQ(mat.m33, 16);
}

TEST(Mat4, ColumnInit)
{
	f32m4 mat{ Math::ColumnInit, f32v4{ 1, 2, 3, 4 }, f32v4{ 5, 6, 7, 8 }, f32v4{ 9, 10, 11, 12 }, f32v4{ 13, 14, 15, 16 } };
	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 5);
	ASSERT_EQ(mat.m02, 9);
	ASSERT_EQ(mat.m03, 13);
	ASSERT_EQ(mat.m10, 2);
	ASSERT_EQ(mat.m11, 6);
	ASSERT_EQ(mat.m12, 10);
	ASSERT_EQ(mat.m13, 14);
	ASSERT_EQ(mat.m20, 3);
	ASSERT_EQ(mat.m21, 7);
	ASSERT_EQ(mat.m22, 11);
	ASSERT_EQ(mat.m23, 15);
	ASSERT_EQ(mat.m30, 4);
	ASSERT_EQ(mat.m31, 8);
	ASSERT_EQ(mat.m32, 12);
	ASSERT_EQ(mat.m33, 16);
}

TEST(Mat4, CopyInit)
{
	f32m4 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	f32m4 mat{ src };

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m03, 4);
	ASSERT_EQ(mat.m10, 5);
	ASSERT_EQ(mat.m11, 6);
	ASSERT_EQ(mat.m12, 7);
	ASSERT_EQ(mat.m13, 8);
	ASSERT_EQ(mat.m20, 9);
	ASSERT_EQ(mat.m21, 10);
	ASSERT_EQ(mat.m22, 11);
	ASSERT_EQ(mat.m23, 12);
	ASSERT_EQ(mat.m30, 13);
	ASSERT_EQ(mat.m31, 14);
	ASSERT_EQ(mat.m32, 15);
	ASSERT_EQ(mat.m33, 16);
}

TEST(Mat4, CopyDiffTypeInit)
{
	i32m4 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	f32m4 mat{ src };

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m03, 4);
	ASSERT_EQ(mat.m10, 5);
	ASSERT_EQ(mat.m11, 6);
	ASSERT_EQ(mat.m12, 7);
	ASSERT_EQ(mat.m13, 8);
	ASSERT_EQ(mat.m20, 9);
	ASSERT_EQ(mat.m21, 10);
	ASSERT_EQ(mat.m22, 11);
	ASSERT_EQ(mat.m23, 12);
	ASSERT_EQ(mat.m30, 13);
	ASSERT_EQ(mat.m31, 14);
	ASSERT_EQ(mat.m32, 15);
	ASSERT_EQ(mat.m33, 16);
}

TEST(Mat4, MoveInit)
{
	f32m4 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	f32m4 mat{ Core::Move(src) };

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m03, 4);
	ASSERT_EQ(mat.m10, 5);
	ASSERT_EQ(mat.m11, 6);
	ASSERT_EQ(mat.m12, 7);
	ASSERT_EQ(mat.m13, 8);
	ASSERT_EQ(mat.m20, 9);
	ASSERT_EQ(mat.m21, 10);
	ASSERT_EQ(mat.m22, 11);
	ASSERT_EQ(mat.m23, 12);
	ASSERT_EQ(mat.m30, 13);
	ASSERT_EQ(mat.m31, 14);
	ASSERT_EQ(mat.m32, 15);
	ASSERT_EQ(mat.m33, 16);
}

TEST(Mat4, CopyAssign)
{
	f32m4 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	f32m4 mat = src;

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m03, 4);
	ASSERT_EQ(mat.m10, 5);
	ASSERT_EQ(mat.m11, 6);
	ASSERT_EQ(mat.m12, 7);
	ASSERT_EQ(mat.m13, 8);
	ASSERT_EQ(mat.m20, 9);
	ASSERT_EQ(mat.m21, 10);
	ASSERT_EQ(mat.m22, 11);
	ASSERT_EQ(mat.m23, 12);
	ASSERT_EQ(mat.m30, 13);
	ASSERT_EQ(mat.m31, 14);
	ASSERT_EQ(mat.m32, 15);
	ASSERT_EQ(mat.m33, 16);
}

TEST(Mat4, CopyDiffTypeAssign)
{
	i32m4 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	f32m4 mat = src;

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m03, 4);
	ASSERT_EQ(mat.m10, 5);
	ASSERT_EQ(mat.m11, 6);
	ASSERT_EQ(mat.m12, 7);
	ASSERT_EQ(mat.m13, 8);
	ASSERT_EQ(mat.m20, 9);
	ASSERT_EQ(mat.m21, 10);
	ASSERT_EQ(mat.m22, 11);
	ASSERT_EQ(mat.m23, 12);
	ASSERT_EQ(mat.m30, 13);
	ASSERT_EQ(mat.m31, 14);
	ASSERT_EQ(mat.m32, 15);
	ASSERT_EQ(mat.m33, 16);
}

TEST(Mat4, MoveAssign)
{
	f32m4 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	f32m4 mat = Core::Move(src);

	ASSERT_EQ(mat.m00, 1);
	ASSERT_EQ(mat.m01, 2);
	ASSERT_EQ(mat.m02, 3);
	ASSERT_EQ(mat.m03, 4);
	ASSERT_EQ(mat.m10, 5);
	ASSERT_EQ(mat.m11, 6);
	ASSERT_EQ(mat.m12, 7);
	ASSERT_EQ(mat.m13, 8);
	ASSERT_EQ(mat.m20, 9);
	ASSERT_EQ(mat.m21, 10);
	ASSERT_EQ(mat.m22, 11);
	ASSERT_EQ(mat.m23, 12);
	ASSERT_EQ(mat.m30, 13);
	ASSERT_EQ(mat.m31, 14);
	ASSERT_EQ(mat.m32, 15);
	ASSERT_EQ(mat.m33, 16);
}

TEST(Mat4, Compare)
{
	ASSERT_EQ((f32m4{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }), (f32m4{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }));
	ASSERT_NE((f32m4{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }), (f32m4{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 13, 14, 15, 16 }));
	ASSERT_TRUE((f32m4{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }).Compare(f32m4{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }));
}

TEST(Mat4, IndexGet)
{
	f32m4 mat{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

	ASSERT_EQ(mat.Row(1), (f32v4{ 5, 6, 7, 8 }));
	ASSERT_EQ(mat.Column(0), (f32v4{ 1, 5, 9, 13 }));
	ASSERT_EQ(mat.Diagonal(), (f32v4{ 1, 6, 11, 16 }));
	ASSERT_EQ(mat(0, 1), 2);
}

TEST(Mat4, IndexSet)
{
	f32m4 src{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

	f32m4 mat = src;
	mat.SetRow(0, f32v4{ 40, 41, 42, 43 });
	ASSERT_EQ(mat, (f32m4{ 40, 41, 42, 43, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }));

	mat = src;
	mat.SetColumn(1, f32v4{ 40, 41, 42, 43 });
	ASSERT_EQ(mat, (f32m4{ 1, 40, 3, 4, 5, 41, 7, 8, 9, 42, 11, 12, 13, 43, 15, 16 }));

	mat = src;
	mat(1, 1) = 42;
}

TEST(Mat4, AddSub)
{
	f32m4 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	f32m4 b{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 };

	ASSERT_EQ(a + b, (f32m4{ 3,  6,  9,  12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48 }));
	ASSERT_EQ(a - b, (f32m4{ -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16}));
}

TEST(Mat4, MulDivScalar)
{
	f32m4 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

	ASSERT_EQ(a * 2, (f32m4{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 }));
	ASSERT_EQ(a / 2, (f32m4{ .5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8 }));
}

TEST(Mat4, Mul)
{
	f32m4 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	f32m4 b{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 };

	ASSERT_EQ(a * b, (f32m4{ 180, 200, 220, 240, 404, 456, 508, 560, 628, 712, 796, 880, 852, 968, 1084, 1200 }));
}

TEST(Mat4, AddSubAssign)
{
	f32m4 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	f32m4 b{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 };

	f32m4 tmp = a;
	tmp += b;
	ASSERT_EQ(tmp, (f32m4{ 3,  6,  9,  12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48 }));

	tmp = a;
	tmp -= b;
	ASSERT_EQ(a - b, (f32m4{ -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16 }));
}

TEST(Mat4, MulDivAssignScalar)
{
	f32m4 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

	f32m4 tmp = a;
	tmp *= 2;
	ASSERT_EQ(tmp, (f32m4{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 }));

	tmp = a;
	tmp /= 2;
	ASSERT_EQ(tmp, (f32m4{ .5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8 }));
}

TEST(Mat4, MulAssign)
{
	f32m4 a{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	f32m4 b{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 };

	a *= b;
	ASSERT_EQ(a, (f32m4{ 180, 200, 220, 240, 404, 456, 508, 560, 628, 712, 796, 880, 852, 968, 1084, 1200 }));
}

TEST(Mat4, Determinant)
{
	f32m4 a{ 1, 2, 5, 4, 4, 3, 1, 5, 2, 5, 3, 4, 4, 1, 3, 2 };
	ASSERT_EQ(a.Determinant(), -222);
}

TEST(Mat4, Transpose)
{
	f32m4 a{ 1, 2, 5, 4, 4, 3, 1, 5, 2, 5, 3, 4, 4, 1, 3, 2 };

	ASSERT_EQ(a.Transposed(), (f32m4{ 1, 4, 2, 4, 2, 3, 5, 1, 5, 1, 3, 3, 4, 5, 4, 2 }));
	a.Transpose();
	ASSERT_EQ(a, (f32m4{ 1, 4, 2, 4, 2, 3, 5, 1, 5, 1, 3, 3, 4, 5, 4, 2 }));
}

TEST(Mat4, Adjugate)
{
	f32m4 a{ 1, 2, 5, 4, 4, 3, 1, 5, 2, 5, 3, 4, 4, 1, 3, 2 };

	ASSERT_EQ(a.Adjoint(), (f32m4{ 36, -6, 1, -59, 42, 30, -79, -1, -30, 42, -7, -31, -48, -66, 48, 54 }));
	a.Adjugate();
	ASSERT_EQ(a, (f32m4{ 36, -6, 1, -59, 42, 30, -79, -1, -30, 42, -7, -31, -48, -66, 48, 54 }));
}

TEST(Mat4, Cofactor)
{
	f32m4 a{ 1, 2, 5, 4, 4, 3, 1, 5, 2, 5, 3, 4, 4, 1, 3, 2 };
	
	ASSERT_EQ(a.Cofactor(), (f32m4{ 36, 42, -30, -48, -6, 30, 42, -66, 1, -79, -7, 48, -59, -1, -31, 54 }));
	a.ToCofactor();
	ASSERT_EQ(a, (f32m4{ 36, 42, -30, -48, -6, 30, 42, -66, 1, -79, -7, 48, -59, -1, -31, 54 }));
}

TEST(Mat4, Inverse)
{
	f32m4 orig{ 1, 2, 5, 4, 4, 3, 1, 5, 2, 5, 3, 4, 4, 1, 3, 2 };
	f32m4 a = orig;
	f32m4 res{ 36, -6, 1, -59, 42, 30, -79, -1, -30, 42, -7, -31, -48, -66, 48, 54 };
	res /= -222;

	ASSERT_TRUE(a.Inverse().Compare(res));
	a.Invert();
	ASSERT_TRUE(a.Compare(res));

	ASSERT_TRUE((orig * a).Compare(f32m4{}));
	ASSERT_TRUE((orig * orig.Inverse()).Compare(f32m4{}));
}
