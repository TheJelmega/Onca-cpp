#include "gtest/gtest.h"
#include "core/Core.h"

namespace Math = Core::Math;

TEST(Vec3, DefaultInit)
{
	f32v3 v;
	ASSERT_EQ(v.x, 0);
	ASSERT_EQ(v.y, 0);
	ASSERT_EQ(v.z, 0);
}

TEST(Vec3, ComponentInit)
{
	f32v3 v{ 1, 2, 3};
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
}

TEST(Vec3, Vec2AndComponentInit)
{
	f32v2 v2{ 1, 2 };
	f32v3 v{ v2, 3 };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
}

TEST(Vec3, CopyInit)
{
	f32v3 src{ 1, 2, 3 };
	f32v3 v{ src };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
}

TEST(Vec3, CopyDiffTypeInit)
{
	i32v3 src{ 1, 2, 3 };
	f32v3 v{ src };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
}

TEST(Vec3, MoveInit)
{
	f32v3 src{ 1, 2, 3 };
	f32v3 v{ Core::Move(src) };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
}

TEST(Vec3, Assign)
{
	f32v3 v = f32v3{ 1, 2, 3 };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
}

TEST(Vec3, DiffTypeAssign)
{
	f32v3 v = i32v3{ 1, 2, 3 };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
}

TEST(Vec3, MoveAssign)
{
	f32v3 v = Core::Move(f32v3{ 1, 2, 3 });
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
}

TEST(Vec3, Comparison)
{
	ASSERT_EQ((f32v3{ 1, 2, 3 }), (f32v3{ 1, 2, 3 }));
	ASSERT_NE((f32v3{ 1, 2, 3 }), (f32v3{ 3, 4, 5 }));
	ASSERT_TRUE((f32v3{ 1, 2, 3 }.Compare(f32v3{ 1.1f, 1.9f, 3.05f }, 0.2f)));
}

TEST(Vec3, ArithOps)
{
	ASSERT_EQ((f32v3{ 1, 2, 3 } + f32v3{ 2, 4, 6 }), (f32v3{ 3   ,  6    ,  9   }));
	ASSERT_EQ((f32v3{ 1, 2, 3 } - f32v3{ 2, 4, 6 }), (f32v3{ -1  , -2    , -3   }));
	ASSERT_EQ((f32v3{ 1, 2, 3 } * f32v3{ 2, 4, 6 }), (f32v3{ 2   ,  8    ,  18  }));
	ASSERT_EQ((f32v3{ 1, 2, 3 } / f32v3{ 2, 8, 3 }), (f32v3{ 0.5f,  0.25f,  1.f }));
	ASSERT_EQ((i32v3{ 1, 5, 9 } % i32v3{ 2, 3, 4 }), (i32v3{ 1   ,      2,  1   }));
}

TEST(Vec3, ArithOpsScalar)
{
	ASSERT_EQ((f32v3{ 1, 2, 3 } + 2), (f32v3{  3   , 4, 5    }));
	ASSERT_EQ((f32v3{ 1, 2, 3 } - 2), (f32v3{ -1   , 0, 1    }));
	ASSERT_EQ((f32v3{ 1, 2, 3 } * 2), (f32v3{  2   , 4, 6    }));
	ASSERT_EQ((f32v3{ 1, 2, 3 } / 2), (f32v3{  0.5f, 1, 1.5f }));
	ASSERT_EQ((i32v3{ 1, 2, 3 } % 2), (i32v3{  1   , 0, 1    }));
}

TEST(Vec3, LogicOps)
{
	ASSERT_EQ((i32v3{ 1, 4, 8 }   |  i32v3{ 2, 4, 12 }), (i32v3{ 3, 4, 12  }));
	ASSERT_EQ((i32v3{ 1, 4, 8 }   ^  i32v3{ 2, 4, 12 }), (i32v3{ 3, 0, 4   }));
	ASSERT_EQ((i32v3{ 1, 4, 8 }   &  i32v3{ 2, 4, 12 }), (i32v3{ 0, 4, 8   }));
	ASSERT_EQ((i32v3{ 1, 2, 3 }   << i32v3{ 2, 3, 4 }) , (i32v3{ 4, 16, 48 }));
	ASSERT_EQ((i32v3{ 4, 16, 48 } >> i32v3{ 2, 3, 4 }) , (i32v3{ 1, 2, 3   }));
}

TEST(Vec3, LogicOpsScalar)
{
	ASSERT_EQ((i32v3{ 1, 4, 12 } | 2) , (i32v3{ 3, 6, 14 }));
	ASSERT_EQ((i32v3{ 1, 4, 12 } ^ 4) , (i32v3{ 5, 0, 8  }));
	ASSERT_EQ((i32v3{ 1, 4, 12 } & 4) , (i32v3{ 0, 4, 4  }));
	ASSERT_EQ((i32v3{ 1, 2, 3  } << 2), (i32v3{ 4, 8, 12 }));
	ASSERT_EQ((i32v3{ 4, 8, 12 } >> 2), (i32v3{ 1, 2, 3  }));
}

TEST(Vec3, ArithAssignOps)
{
	{
		f32v3 vec{ 1, 2, 3 };
		vec += f32v3{ 2, 4, 6 };
		ASSERT_EQ(vec, (f32v3{ 3, 6, 9 }));
	}
	{
		f32v3 vec{ 1, 2, 3 };
		vec -= f32v3{ 2, 4, 6 };
		ASSERT_EQ(vec, (f32v3{ -1, -2, -3 }));
	}
	{
		f32v3 vec{ 1, 2, 3 };
		vec *= f32v3{ 2, 4, 6 };
		ASSERT_EQ(vec, (f32v3{ 2, 8, 18 }));
	}
	{
		f32v3 vec{ 1, 2, 3 };
		vec /= f32v3{ 2, 8, 3 };
		ASSERT_EQ(vec, (f32v3{ 0.5f, 0.25f, 1.f }));
	}
	{
		i32v3 vec{ 1, 5, 9 };
		vec %= i32v3{ 2, 3, 4 };
		ASSERT_EQ(vec, (i32v3{ 1, 2, 1 }));
	}
}

TEST(Vec3, ArithAssignOpsScalar)
{
	{
		f32v3 vec{ 1, 2, 3 };
		vec += 2;
		ASSERT_EQ(vec, (f32v3{ 3, 4, 5 }));
	}
	{
		f32v3 vec{ 1, 2, 3 };
		vec -= 2;
		ASSERT_EQ(vec, (f32v3{ -1, 0, 1 }));
	}
	{
		f32v3 vec{ 1, 2, 3 };
		vec *= 2;
		ASSERT_EQ(vec, (f32v3{ 2, 4, 6 }));
	}
	{
		f32v3 vec{ 1, 2, 3 };
		vec /= 2;
		ASSERT_EQ(vec, (f32v3{ 0.5f, 1, 1.5f }));
	}
	{
		i32v3 vec{ 1, 2, 3 };
		vec %= 2;
		ASSERT_EQ(vec, (i32v3{ 1, 0, 1 }));
	}
}

TEST(Vec3, LogicAssignOps)
{
	{
		i32v3 vec{ 1, 4, 8 };
		vec |= i32v3{ 2, 4, 12 };
		ASSERT_EQ(vec, (i32v3{ 3, 4, 12 }));
	}
	{
		i32v3 vec{ 1, 4, 8 };
		vec ^= i32v3{ 2, 4, 12 };
		ASSERT_EQ(vec, (i32v3{ 3, 0, 4 }));
	}
	{
		i32v3 vec{ 1, 4, 8 };
		vec &= i32v3{ 2, 4, 12 };
		ASSERT_EQ(vec, (i32v3{ 0, 4, 8 }));
	}
	{
		i32v3 vec{ 1, 2, 3 };
		vec <<= i32v3{ 2, 3, 4 };
		ASSERT_EQ(vec, (i32v3{ 4, 16, 48 }));
	}
	{
		i32v3 vec{ 4, 16, 48 };
		vec >>= i32v3{ 2, 3, 4 };
		ASSERT_EQ(vec, (i32v3{ 1, 2, 3 }));
	}
}

TEST(Vec3, LogicAssignOpsScalar)
{
	{
		i32v3 vec{ 1, 4, 12 };
		vec |= 2;
		ASSERT_EQ(vec, (i32v3{ 3, 6, 14 }));
	}
	{
		i32v3 vec{ 1, 4, 12 };
		vec ^= 4;
		ASSERT_EQ(vec, (i32v3{ 5, 0, 8 }));
	}
	{
		i32v3 vec{ 1, 4, 12 };
		vec &= 4;
		ASSERT_EQ(vec, (i32v3{ 0, 4, 4 }));
	}
	{
		i32v3 vec{ 1, 2, 3 };
		vec <<= 2;
		ASSERT_EQ(vec, (i32v3{ 4, 8, 12 }));
	}
	{
		i32v3 vec{ 4, 8, 12 };
		vec >>= 2;
		ASSERT_EQ(vec, (i32v3{ 1, 2, 3 }));
	}
}

TEST(Vec3, Index)
{
	f32v3 vec{ 1, 3, 5 };
	ASSERT_EQ(vec[0], 1);
	ASSERT_EQ(vec[1], 3);
	ASSERT_EQ(vec[2], 5);
}

TEST(Vec3, IsZero)
{
	ASSERT_TRUE ((f32v3{ 0, 0, 0 }.IsZero()));
	ASSERT_FALSE((f32v3{ 0, 0, 1 }.IsZero()));
	ASSERT_TRUE( (f32v3{ 0.1f, 0.f, -0.1f }.IsNearlyZero(0.11f)));
	ASSERT_FALSE((f32v3{ 0.1f, 0.f, -0.2f }.IsNearlyZero(0.11f)));
}

TEST(Vec3, MinMaxComponent)
{
	f32v3 vec{ 1, -3, -5 };
	ASSERT_EQ(vec.MinComponent(), -5);
	ASSERT_EQ(vec.MinAbsComponent(), 1);
	ASSERT_EQ(vec.MaxComponent(), 1);
	ASSERT_EQ(vec.MaxAbsComponent(), 5);
}

TEST(Vec3, MinMax)
{
	f32v3 vec0{ 1, 3, 5 };
	f32v3 vec1{ 4, 2, 6 };
	ASSERT_EQ(vec0.Min(vec1), (f32v3{ 1, 2, 5 }));
	ASSERT_EQ(vec0.Max(vec1), (f32v3{ 4, 3, 6 }));
}

TEST(Vec3, ClampAndSaturate)
{
	ASSERT_EQ((f32v3{ -1.f, 1.f, 2.f }.Clamped(0.f, 1.f)), (f32v3{ 0.f, 1.f, 1.f }));
	ASSERT_EQ((f32v3{ -1.f, 1.f, 2.f }.Clamped(f32v3{ 0.f, 0.f, -2.f }, f32v3{ 1.f, .5f, 1.5f })), (f32v3{ 0.f, .5f, 1.5f }));
	ASSERT_EQ((f32v3{ -2.f, 1.f, 2.f }.ClampedLen(1.f, 2.f)), (f32v3{ -2.f, 1.f, 2.f }.Normalized() * 2.f));
	ASSERT_EQ((f32v3{ -1.f, 1.f, 2.f }.Saturated()), (f32v3{ 0.f, 1.f, 1.f }));
}

TEST(Vec3, Len)
{
	f32v3 vec{ 3, 4, 5 };
	ASSERT_EQ(vec.LenSq(), 50);
	ASSERT_TRUE((Math::EpsilonCompare(vec.Len(), 7.0710f, 0.0001f)));
}

TEST(Vec3, Distance)
{
	f32v3 vec0{ 1, 2, 3 };
	f32v3 vec1{ 4, 6, 8 };
	ASSERT_EQ(vec0.DistanceSq(vec1), 50);
	ASSERT_TRUE((Math::EpsilonCompare(vec0.Distance(vec1), 7.0710f, 0.0001f)));
}

TEST(Vec3, Dot)
{
	f32v3 vec0{ 1, 2, 3 };
	f32v3 vec1{ 4, 6, 8 };
	ASSERT_EQ(vec0.Dot(vec1), 40);
}

TEST(Vec3, Cross)
{
	f32v3 vec0{ 1, 2, 3 };
	f32v3 vec1{ 4, 6, 8 };
	ASSERT_EQ(vec0.Cross(vec1), (f32v3{ -2, 4, -2 }));
}

TEST(Vec3, Normalization)
{
	f32v3 vec{ 3, 4, 5 };
	f32v3 norm = vec;
	norm.Normalize();

	ASSERT_TRUE(vec.Normalized().Compare(f32v3{ 0.4243, 0.5657, 0.7071 }, 0.0001f));
	ASSERT_TRUE(norm.Compare(f32v3{ 0.4243, 0.5657, 0.7071 }, 0.0001f));

	ASSERT_FALSE(vec.IsNormalized());
	ASSERT_TRUE(norm.IsNormalized());
}

TEST(Vec3, FaceForward)
{
	f32v3 vec{ -1, -1, -1 };
	f32v3 inc{ 2, -3, -2 };
	f32v3 normal{ 0, 1, 0 };

	ASSERT_EQ(vec.FaceForward(inc, normal), vec);
	ASSERT_EQ(vec.FaceForward(-inc, normal), -vec);
}

TEST(Vec3, Reflect)
{
	f32v3 vec{ 0, -1, 0 };
	f32v3 normal{ .5f, .5f, .5f };
	normal.Normalize();

	ASSERT_TRUE(vec.Reflect(normal).Compare(f32v3{ .6666f, -.3333f, .6666f }, 0.0001f));
}

TEST(Vec3, Refract)
{
	f32v3 vec{ -1, -1, -1 };
	vec.Normalize();
	f32v3 normal{ 0, 1, 0 };

	ASSERT_TRUE((vec.Refract(normal, 1.0f).Compare(vec)));

	f32 eta = 1.0f / 1.5808f;
	ASSERT_TRUE((vec.Refract(normal, eta).Compare(f32v3{ -0.3652, -0.8563, -0.3652 }, 0.0001f)));
}

TEST(Vec3, DistAndLen)
{
	f32v3 vec{ 3, 4, 5 };
	auto [dir, len] = vec.DirectionAndLength();
	ASSERT_TRUE((Math::EpsilonCompare(len, 7.0710f, 0.0001f)));
	ASSERT_EQ(vec.Normalized(), (f32v3{ 3, 4, 5 }.Normalized()));
}

TEST(Vec3, Swizzle)
{
	f32v3 v{ 1, 2, 3 };

	ASSERT_EQ((v.Swizzle<f32v3::X, f32v3::X, f32v3::X>()), (f32v3{ 1, 1, 1 }));
	ASSERT_EQ((v.Swizzle<f32v3::X, f32v3::X, f32v3::Y>()), (f32v3{ 1, 1, 2 }));
	ASSERT_EQ((v.Swizzle<f32v3::X, f32v3::X, f32v3::Z>()), (f32v3{ 1, 1, 3 }));
	ASSERT_EQ((v.Swizzle<f32v3::X, f32v3::Y, f32v3::X>()), (f32v3{ 1, 2, 1 }));
	ASSERT_EQ((v.Swizzle<f32v3::X, f32v3::Y, f32v3::Y>()), (f32v3{ 1, 2, 2 }));
	ASSERT_EQ((v.Swizzle<f32v3::X, f32v3::Y, f32v3::Z>()), (f32v3{ 1, 2, 3 }));
	ASSERT_EQ((v.Swizzle<f32v3::X, f32v3::Z, f32v3::X>()), (f32v3{ 1, 3, 1 }));
	ASSERT_EQ((v.Swizzle<f32v3::X, f32v3::Z, f32v3::Y>()), (f32v3{ 1, 3, 2 }));
	ASSERT_EQ((v.Swizzle<f32v3::X, f32v3::Z, f32v3::Z>()), (f32v3{ 1, 3, 3 }));
	ASSERT_EQ((v.Swizzle<f32v3::Y, f32v3::X, f32v3::X>()), (f32v3{ 2, 1, 1 }));
	ASSERT_EQ((v.Swizzle<f32v3::Y, f32v3::X, f32v3::Y>()), (f32v3{ 2, 1, 2 }));
	ASSERT_EQ((v.Swizzle<f32v3::Y, f32v3::X, f32v3::Z>()), (f32v3{ 2, 1, 3 }));
	ASSERT_EQ((v.Swizzle<f32v3::Y, f32v3::Y, f32v3::X>()), (f32v3{ 2, 2, 1 }));
	ASSERT_EQ((v.Swizzle<f32v3::Y, f32v3::Y, f32v3::Y>()), (f32v3{ 2, 2, 2 }));
	ASSERT_EQ((v.Swizzle<f32v3::Y, f32v3::Y, f32v3::Z>()), (f32v3{ 2, 2, 3 }));
	ASSERT_EQ((v.Swizzle<f32v3::Y, f32v3::Z, f32v3::X>()), (f32v3{ 2, 3, 1 }));
	ASSERT_EQ((v.Swizzle<f32v3::Y, f32v3::Z, f32v3::Y>()), (f32v3{ 2, 3, 2 }));
	ASSERT_EQ((v.Swizzle<f32v3::Y, f32v3::Z, f32v3::Z>()), (f32v3{ 2, 3, 3 }));
	ASSERT_EQ((v.Swizzle<f32v3::Z, f32v3::X, f32v3::X>()), (f32v3{ 3, 1, 1 }));
	ASSERT_EQ((v.Swizzle<f32v3::Z, f32v3::X, f32v3::Y>()), (f32v3{ 3, 1, 2 }));
	ASSERT_EQ((v.Swizzle<f32v3::Z, f32v3::X, f32v3::Z>()), (f32v3{ 3, 1, 3 }));
	ASSERT_EQ((v.Swizzle<f32v3::Z, f32v3::Y, f32v3::X>()), (f32v3{ 3, 2, 1 }));
	ASSERT_EQ((v.Swizzle<f32v3::Z, f32v3::Y, f32v3::Y>()), (f32v3{ 3, 2, 2 }));
	ASSERT_EQ((v.Swizzle<f32v3::Z, f32v3::Y, f32v3::Z>()), (f32v3{ 3, 2, 3 }));
	ASSERT_EQ((v.Swizzle<f32v3::Z, f32v3::Z, f32v3::X>()), (f32v3{ 3, 3, 1 }));
	ASSERT_EQ((v.Swizzle<f32v3::Z, f32v3::Z, f32v3::Y>()), (f32v3{ 3, 3, 2 }));
	ASSERT_EQ((v.Swizzle<f32v3::Z, f32v3::Z, f32v3::Z>()), (f32v3{ 3, 3, 3 }));

	ASSERT_EQ((v.xxx()), (f32v3{ 1, 1, 1 }));
	ASSERT_EQ((v.xxy()), (f32v3{ 1, 1, 2 }));
	ASSERT_EQ((v.xxz()), (f32v3{ 1, 1, 3 }));
	ASSERT_EQ((v.xyx()), (f32v3{ 1, 2, 1 }));
	ASSERT_EQ((v.xyy()), (f32v3{ 1, 2, 2 }));
	ASSERT_EQ((v.xyz()), (f32v3{ 1, 2, 3 }));
	ASSERT_EQ((v.xzx()), (f32v3{ 1, 3, 1 }));
	ASSERT_EQ((v.xzy()), (f32v3{ 1, 3, 2 }));
	ASSERT_EQ((v.xzz()), (f32v3{ 1, 3, 3 }));
	ASSERT_EQ((v.yxx()), (f32v3{ 2, 1, 1 }));
	ASSERT_EQ((v.yxy()), (f32v3{ 2, 1, 2 }));
	ASSERT_EQ((v.yxz()), (f32v3{ 2, 1, 3 }));
	ASSERT_EQ((v.yyx()), (f32v3{ 2, 2, 1 }));
	ASSERT_EQ((v.yyy()), (f32v3{ 2, 2, 2 }));
	ASSERT_EQ((v.yyz()), (f32v3{ 2, 2, 3 }));
	ASSERT_EQ((v.yzx()), (f32v3{ 2, 3, 1 }));
	ASSERT_EQ((v.yzy()), (f32v3{ 2, 3, 2 }));
	ASSERT_EQ((v.yzz()), (f32v3{ 2, 3, 3 }));
	ASSERT_EQ((v.zxx()), (f32v3{ 3, 1, 1 }));
	ASSERT_EQ((v.zxy()), (f32v3{ 3, 1, 2 }));
	ASSERT_EQ((v.zxz()), (f32v3{ 3, 1, 3 }));
	ASSERT_EQ((v.zyx()), (f32v3{ 3, 2, 1 }));
	ASSERT_EQ((v.zyy()), (f32v3{ 3, 2, 2 }));
	ASSERT_EQ((v.zyz()), (f32v3{ 3, 2, 3 }));
	ASSERT_EQ((v.zzx()), (f32v3{ 3, 3, 1 }));
	ASSERT_EQ((v.zzy()), (f32v3{ 3, 3, 2 }));
	ASSERT_EQ((v.zzz()), (f32v3{ 3, 3, 3 }));
}

TEST(Vec3, Misc)
{
	f32v3 v0{ 0.5f, -1.25f, 2.75f };

	ASSERT_EQ(v0.Abs(), (f32v3{ 0.5f,  1.25f, 2.75f }));
	ASSERT_EQ(v0.Ceil(), (f32v3{ 1.f , -1.f, 3.f }));
	ASSERT_EQ(v0.Floor(), (f32v3{ 0.f , -2.f, 2.f }));
	ASSERT_EQ(v0.Trunc(), (f32v3{ 0.f , -1.f, 2.f }));
	ASSERT_EQ(v0.Round(), (f32v3{ 1.f , -1.f, 3.f }));
	ASSERT_EQ(v0.RoundEven(), (f32v3{ 0.f , -1.f, 3.f }));
	ASSERT_EQ(v0.Sign(), (f32v3{ 1.f , -1.f, 1.f }));
	ASSERT_EQ(v0.Fract(), (f32v3{ .5f, -.25f, .75f }));
}

TEST(Vec3, QuatMul)
{
	f32q quat = f32q::AxisAngle(f32v3{ 0, 0, 1 }, 90_deg);

	f32v3 v{ 1, 0, 0 };

	f32v3 rot = quat * v * quat.Inverse();
	ASSERT_EQ(rot, (f32v3{ 0, 1, 0 }));

	quat = quat.Invert();
	rot = quat * rot * quat.Inverse();
	ASSERT_EQ(rot, (f32v3{ 1, 0, 0 }));
}

TEST(Vec3, Rotate)
{
	f32q quat = f32q::AxisAngle(f32v3{ 0, 0, 1 }, 90_deg);

	f32v3 v{ 1, 0, 0 };

	f32v3 rot = v.Rotated(quat);
	ASSERT_TRUE(rot.Compare(f32v3{ 0, 1, 0 }));

	rot.Rotate(quat.Inverse());
	ASSERT_TRUE(rot.Compare(f32v3{ 1, 0, 0 }));
}