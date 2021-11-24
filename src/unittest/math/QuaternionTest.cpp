#include "gtest/gtest.h"
#include "core/Core.h"

namespace Math = Core::Math;

TEST(Quaternion, IdentityInit)
{
	f32q quat;
	ASSERT_EQ(quat.w, 1.f);
	ASSERT_EQ(quat.x, 0.f);
	ASSERT_EQ(quat.y, 0.f);
	ASSERT_EQ(quat.z, 0.f);
}

TEST(Quaternion, ComponentInit)
{
	f32q quat{ 1, 2, 3, 4 };
	ASSERT_EQ(quat.w, 1.f);
	ASSERT_EQ(quat.x, 2.f);
	ASSERT_EQ(quat.y, 3.f);
	ASSERT_EQ(quat.z, 4.f);
}

TEST(Quaternion, RealImaginaryInit)
{
	f32v3 imaginary{ 2, 3, 4 };
	f32q quat{ 1, imaginary };
	ASSERT_EQ(quat.w, 1.f);
	ASSERT_EQ(quat.x, 2.f);
	ASSERT_EQ(quat.y, 3.f);
	ASSERT_EQ(quat.z, 4.f);
}

TEST(Quaternion, CopyInit)
{
	f32q src{ 1, 2, 3, 4 };
	f32q quat{ src };
	ASSERT_EQ(quat.w, 1.f);
	ASSERT_EQ(quat.x, 2.f);
	ASSERT_EQ(quat.y, 3.f);
	ASSERT_EQ(quat.z, 4.f);
}

TEST(Quaternion, CopyDiffTypeInit)
{
	f64q src{ 1, 2, 3, 4 };
	f32q quat{ src };
	ASSERT_EQ(quat.w, 1.f);
	ASSERT_EQ(quat.x, 2.f);
	ASSERT_EQ(quat.y, 3.f);
	ASSERT_EQ(quat.z, 4.f);
}

TEST(Quaternion, MoveInit)
{
	f32q src{ 1, 2, 3, 4 };
	f32q quat{ Core::Move(src) };
	ASSERT_EQ(quat.w, 1.f);
	ASSERT_EQ(quat.x, 2.f);
	ASSERT_EQ(quat.y, 3.f);
	ASSERT_EQ(quat.z, 4.f);
}

TEST(Quaternion, CopyAssign)
{
	f32q src{ 1, 2, 3, 4 };
	f32q quat = src;
	ASSERT_EQ(quat.w, 1.f);
	ASSERT_EQ(quat.x, 2.f);
	ASSERT_EQ(quat.y, 3.f);
	ASSERT_EQ(quat.z, 4.f);
}

TEST(Quaternion, DiffTypeAssign)
{
	f64q src{ 1, 2, 3, 4 };
	f32q quat = src;
	ASSERT_EQ(quat.w, 1.f);
	ASSERT_EQ(quat.x, 2.f);
	ASSERT_EQ(quat.y, 3.f);
	ASSERT_EQ(quat.z, 4.f);
}

TEST(Quaternion, MoveAssign)
{
	f32q src{ 1, 2, 3, 4 };
	f32q quat = Core::Move(src);
	ASSERT_EQ(quat.w, 1.f);
	ASSERT_EQ(quat.x, 2.f);
	ASSERT_EQ(quat.y, 3.f);
	ASSERT_EQ(quat.z, 4.f);
}

TEST(Quaternion, AddSub)
{
	ASSERT_EQ((f32q{ 1, 2, 3, 4 } + f32q{ 2, 4, 6, 8 }), (f32q{ 3, 6, 9, 12 }));
	ASSERT_EQ((f32q{ 3, 6, 9, 12 } - f32q{ 2, 4, 6, 8 }), (f32q{ 1, 2, 3, 4 }));
}

TEST(Quaternion, MulDivScalar)
{
	ASSERT_EQ((f32q{ 1, 2, 3, 4 } * 2), (f32q{ 2, 4, 6, 8 }));
	ASSERT_EQ((f32q{ 2, 4, 6, 8 } / 2), (f32q{ 1, 2, 3, 4 }));
}

TEST(Quaternion, Mul)
{
	ASSERT_EQ((f32q{ 1, 2, 3, 4 } * f32q{ 2, 4, 6, 8 }), (f32q{ -56, 8, 12, 16 }));
}

TEST(Quaternion, AddSubAssign)
{
	{
		f32q quat{ 1, 2, 3, 4 };
		quat += f32q{ 2, 4, 6, 8 };
		ASSERT_EQ((quat), (f32q{ 3, 6, 9, 12 }));
	} 
	{
		f32q quat{ 3, 6, 9, 12 };
		quat -= f32q{ 2, 4, 6, 8 };
		ASSERT_EQ((quat), (f32q{ 1, 2, 3, 4 }));
	}
}

TEST(Quaternion, MulDivAssignScalar)
{
	{
		f32q quat{ 1, 2, 3, 4 };
		quat *= 2;
		ASSERT_EQ((quat), (f32q{ 2, 4, 6, 8 }));
	}
	{
		f32q quat{ 2, 4, 6, 8 };
		quat /= 2;
		ASSERT_EQ((quat), (f32q{ 1, 2, 3, 4 }));
	}
}

TEST(Quaternion, MulAssign)
{
	f32q quat{ 1, 2, 3, 4 };
	quat *= f32q{ 2, 4, 6, 8 };
	ASSERT_EQ((quat), (f32q{ -56, 8, 12, 16 }));
}

TEST(Quaternion, Compare)
{
	ASSERT_TRUE((f32q{ 1, 2, 3, 4 } == f32q{ 1, 2, 3, 4 }));
	ASSERT_TRUE((f32q{ 1, 2, 3, 4 } != f32q{ 4, 3, 2, 1 }));
	ASSERT_TRUE((f32q{ 1, 2, 3, 4 }.Compare(f32q{ 1, 2, 3, 4 })));
}

TEST(Quaternion, Dot)
{
	ASSERT_EQ((f32q{ 1, 2, 3, 4 }.Dot(f32q{ 2, 4, 6, 8 })), 60);
}

TEST(Quaternion, Len)
{
	ASSERT_EQ((f32q{ 1, 2, 3, 4 }.LenSq()), 30);
	ASSERT_TRUE((Math::EpsilonCompare(f32q{ 1, 2, 3, 4 }.Len(), Math::Sqrt(30.f))));
}

TEST(Quaternion, Normalize)
{
	ASSERT_TRUE((f32q{ 1, 2, 3, 4 }.Normalized()).Compare(f32q{ 0.1825, 0.3651, 0.5477, 0.7302 }, 0.0001f));

	f32q quat{ 1, 2, 3, 4 };
	quat.Normalize();
	ASSERT_TRUE((quat).Compare(f32q{ 0.1825, 0.3651, 0.5477, 0.7302 }, 0.0001f));

	ASSERT_FALSE((f32q{ 1, 2, 3, 4 }.IsNormalized(0.0001f)));
	ASSERT_TRUE((quat.IsNormalized(0.0001f)));
}

TEST(Quaternion, Conjugate)
{
	ASSERT_EQ((f32q{ 1, 2, 3, 4 }.Conjugated()), (f32q{ 1, -2, -3, -4 }));

	f32q quat{ 1, 2, 3, 4 };
	quat.Conjugate();
	ASSERT_EQ((quat), (f32q{ 1, -2, -3, -4 }));
}

TEST(Quaternion, Inverse)
{
	ASSERT_TRUE((f32q{ 1, 2, 3, 4 }.Inverse()).Compare(f32q{ 0.0333, -0.0666, -0.1, -0.1333 }, 0.0001f));

	f32q quat{ 1, 2, 3, 4 };
	quat.Invert();
	ASSERT_TRUE((quat).Compare(f32q{ 0.0333, -0.0666, -0.1, -0.1333 }, 0.0001f));
}

TEST(Quaternion, Lerp)
{
	f32q quatA{ 0.9998, 0,  0.0174, 0 };
	f32q quatB{ 0.0087, 0,  0.9999, 0 };
	f32q quatC{ 0.0087, 0, -0.9999, 0 };

	ASSERT_TRUE(quatA.LerpFullPath(quatB, 0.75).Compare(f32q{ 0.2564, 0, 0.7542, 0 }, 0.0001f));
	ASSERT_TRUE(quatA.LerpFullPath(quatC, 0.75).Compare(f32q{ 0.2564, 0, -0.7455, 0 }, 0.0001f));
	ASSERT_TRUE(quatA.Lerp(quatB, 0.75).Compare(f32q{ 0.2564, 0, 0.7542, 0 }, 0.0001f));
	ASSERT_TRUE(quatA.Lerp(quatC, 0.75).Compare(f32q{ -0.2434, 0, -0.7542, 0 }, 0.0001f));
}

TEST(Quaternion, SLerp)
{
	f32q quatA{ 0.9998, 0,  0.0174, 0 };
	quatA.Normalize();
	f32q quatB{ 0.0087, 0,  0.9999, 0 };
	quatB.Normalize();
	f32q quatC{ 0.0087, 0, -0.9999, 0 };
	quatC.Normalize();

	ASSERT_TRUE(quatA.SlerpFullPath(quatB, 0.75).Compare(f32q{ 0.3847, 0, 0.9230, 0 }, 0.0001f));
	ASSERT_TRUE(quatA.SlerpFullPath(quatC, 0.75).Compare(f32q{ 0.3927, 0, -0.9196, 0 }, 0.0001f));
	ASSERT_TRUE(quatA.Slerp(quatB, 0.75).Compare(f32q{ 0.3847, 0, 0.9230, 0 }, 0.0001f));
	ASSERT_TRUE(quatA.Slerp(quatC, 0.75).Compare(f32q{ -0.3766, 0, -0.9331, 0 }, 0.0001f));
}

TEST(Quaternion, AxisAngle)
{
	f32v3 axis{ 0.5, 0.5, -0.5 };
	axis.Normalize();
	f32rad angle = 60_deg;

	f32q quat = f32q::AxisAngle(axis, angle);

	ASSERT_TRUE(quat.Compare(f32q{ 0.8660, 0.2886, 0.2886, -0.2886 }, 0.0001f));

	auto [a, b] = quat.ToAxisAngle();
	ASSERT_TRUE(a.Compare(f32v3{ 0.5, 0.5, -0.5 }.Normalized()));
	ASSERT_TRUE(Math::EpsilonCompare(b.val, angle.ToRadians().val));
}

TEST(Quaternion, EulerAngles)
{
	Math::EulerAngles<f32, Math::AngleKind::Radians> euler;
	f32q quat;

	euler.pitch = -2.712_rad;
	euler.yaw = 0.831_rad;
	euler.roll = 2.123_rad;
	quat = f32q::EulerAngles(euler, Math::EulerOrder::XYZ);
	ASSERT_TRUE(quat.Compare(f32q{ 0.4396, -0.3604, 0.8223, -0.0222 }, 0.001f));

	euler.pitch = -0.446_rad;
	euler.yaw = 2.017_rad;
	euler.roll = 0.610_rad;
	quat = f32q::EulerAngles(euler, Math::EulerOrder::XZY);
	ASSERT_TRUE(quat.Compare(f32q{ 0.4396, -0.3604, 0.8223, -0.0222 }, 0.001f));

	euler.pitch = -0.284_rad;
	euler.yaw = 2.262_rad;
	euler.roll = -0.691_rad;
	quat = f32q::EulerAngles(euler, Math::EulerOrder::YXZ);
	ASSERT_TRUE(quat.Compare(f32q{ 0.4396, -0.3604, 0.8223, -0.0222 }, 0.001f));

	euler.pitch = -0.362_rad;
	euler.yaw = 2.034_rad;
	euler.roll = -0.659_rad;
	quat = f32q::EulerAngles(euler, Math::EulerOrder::YZX);
	ASSERT_TRUE(quat.Compare(f32q{ 0.4396, -0.3604, 0.8223, -0.0222 }, 0.001f));

	euler.pitch = -0.361_rad;
	euler.yaw = 2.284_rad;
	euler.roll = 0.659_rad;
	quat = f32q::EulerAngles(euler, Math::EulerOrder::ZXY);
	ASSERT_TRUE(quat.Compare(f32q{ 0.4396, -0.3604, 0.8223, -0.0222 }, 0.001f));

	euler.pitch = 30_deg;
	euler.yaw = 135_deg;
	euler.roll = 60_deg;
	quat = f32q::EulerAngles(euler, Math::EulerOrder::ZYX);
	ASSERT_TRUE(quat.Compare(f32q{ 0.4396, -0.3604, 0.8223, -0.0222 }, 0.001f));

	euler = quat.ToEuler(Math::EulerOrder::XYZ);
	ASSERT_TRUE(Math::EpsilonCompare(euler.pitch.val, (-2.712_rad).val, 0.001f));
	ASSERT_TRUE(Math::EpsilonCompare(euler.yaw.val, (0.831_rad).val, 0.001f));
	ASSERT_TRUE(Math::EpsilonCompare(euler.roll.val, (2.123_rad).val, 0.001f));

	euler = quat.ToEuler(Math::EulerOrder::XZY);
	ASSERT_TRUE(Math::EpsilonCompare(euler.pitch.val, (-0.446_rad).val, 0.001f));
	ASSERT_TRUE(Math::EpsilonCompare(euler.yaw.val, (2.017_rad).val, 0.001f));
	ASSERT_TRUE(Math::EpsilonCompare(euler.roll.val, (0.610_rad).val, 0.001f));

	euler = quat.ToEuler(Math::EulerOrder::YXZ);
	ASSERT_TRUE(Math::EpsilonCompare(euler.pitch.val, (-0.284_rad).val, 0.001f));
	ASSERT_TRUE(Math::EpsilonCompare(euler.yaw.val, (2.262_rad).val, 0.001f));
	ASSERT_TRUE(Math::EpsilonCompare(euler.roll.val, (-0.691_rad).val, 0.001f));

	euler = quat.ToEuler(Math::EulerOrder::YZX);
	ASSERT_TRUE(Math::EpsilonCompare(euler.pitch.val, (-0.362_rad).val, 0.001f));
	ASSERT_TRUE(Math::EpsilonCompare(euler.yaw.val, (2.034_rad).val, 0.001f));
	ASSERT_TRUE(Math::EpsilonCompare(euler.roll.val, (-0.659_rad).val, 0.001f));

	euler = quat.ToEuler(Math::EulerOrder::ZXY);
	ASSERT_TRUE(Math::EpsilonCompare(euler.pitch.val, (-0.361_rad).val, 0.001f));
	ASSERT_TRUE(Math::EpsilonCompare(euler.yaw.val, (2.284_rad).val, 0.001f));
	ASSERT_TRUE(Math::EpsilonCompare(euler.roll.val, (0.659_rad).val, 0.001f));

	euler = quat.ToEuler(Math::EulerOrder::ZYX);
	ASSERT_TRUE(Math::EpsilonCompare(euler.pitch.val, (-150_deg).ToRadians().val, 0.0001f));
	ASSERT_TRUE(Math::EpsilonCompare(euler.yaw.val, (45_deg).ToRadians().val, 0.0001f));
	ASSERT_TRUE(Math::EpsilonCompare(euler.roll.val, (-120_deg).ToRadians().val, 0.0001f));
}