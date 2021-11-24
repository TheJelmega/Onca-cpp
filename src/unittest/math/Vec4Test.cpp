#include "gtest/gtest.h"
#include "core/Core.h"

namespace Math = Core::Math;

TEST(Vec4, DefaultInit)
{
	f32v4 v;
	ASSERT_EQ(v.x, 0);
	ASSERT_EQ(v.y, 0);
	ASSERT_EQ(v.z, 0);
	ASSERT_EQ(v.w, 0);
}

TEST(Vec4, ComponentInit)
{
	f32v4 v{ 1, 2, 3, 4 };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
	ASSERT_EQ(v.w, 4);
}

TEST(Vec4, Vec2AndComponentInit)
{
	f32v2 v2{ 1, 2 };
	f32v4 v{ v2, 3, 4 };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
	ASSERT_EQ(v.w, 4);
}

TEST(Vec4, Vec3AndComponentInit)
{
	f32v3 v3{ 1, 2, 3 };
	f32v4 v{ v3, 4 };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
	ASSERT_EQ(v.w, 4);
}

TEST(Vec4, CopyInit)
{
	f32v4 src{ 1, 2, 3, 4 };
	f32v4 v{ src };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
	ASSERT_EQ(v.w, 4);
}

TEST(Vec4, CopyDiffTypeInit)
{
	i32v4 src{ 1, 2, 3, 4 };
	f32v4 v{ src };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
	ASSERT_EQ(v.w, 4);
}

TEST(Vec4, MoveInit)
{
	f32v4 src{ 1, 2, 3, 4 };
	f32v4 v{ Core::Move(src) };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
	ASSERT_EQ(v.w, 4);
}

TEST(Vec4, Assign)
{
	f32v4 v = f32v4{ 1, 2, 3, 4 };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
	ASSERT_EQ(v.w, 4);
}

TEST(Vec4, DiffTypeAssign)
{
	f32v4 v = i32v4{ 1, 2, 3, 4 };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
	ASSERT_EQ(v.w, 4);
}

TEST(Vec4, MoveAssign)
{
	f32v4 v = Core::Move(f32v4{ 1, 2, 3, 4 });
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
	ASSERT_EQ(v.z, 3);
	ASSERT_EQ(v.w, 4);
}

TEST(Vec4, Comparison)
{
	ASSERT_EQ((f32v4{ 1, 2, 3, 4 }), (f32v4{ 1, 2, 3, 4 }));
	ASSERT_NE((f32v4{ 1, 2, 3, 4 }), (f32v4{ 3, 4, 5, 6}));
	ASSERT_TRUE((f32v4{ 1, 2, 3, 4 }.Compare(f32v4{ 1.1f, 1.9f, 3.05f, 3.95f }, 0.2f)));
}

TEST(Vec4, ArithOps)
{
	ASSERT_EQ((f32v4{ 1, 2, 3, 4  } + f32v4{ 2, 4, 6, 8  }), (f32v4{ 3   ,  6    ,  9   ,  12     }));
	ASSERT_EQ((f32v4{ 1, 2, 3, 4  } - f32v4{ 2, 4, 6, 8  }), (f32v4{ -1  , -2    , -3   , -4      }));
	ASSERT_EQ((f32v4{ 1, 2, 3, 4  } * f32v4{ 2, 4, 6, 8  }), (f32v4{ 2   ,  8    ,  18  ,  32     }));
	ASSERT_EQ((f32v4{ 1, 2, 3, 4  } / f32v4{ 2, 8, 3, 32 }), (f32v4{ 0.5f,  0.25f,  1.f ,  0.125f }));
	ASSERT_EQ((i32v4{ 1, 5, 9, 13 } % i32v4{ 2, 3, 4, 5  }), (i32v4{ 1   ,  2    ,  1   ,  3      }));
}

TEST(Vec4, ArithOpsScalar)
{
	ASSERT_EQ((f32v4{ 1, 2, 3, 4 } + 2), (f32v4{ 3   , 4, 5   , 6}));
	ASSERT_EQ((f32v4{ 1, 2, 3, 4 } - 2), (f32v4{ -1  , 0, 1   , 2}));
	ASSERT_EQ((f32v4{ 1, 2, 3, 4 } * 2), (f32v4{ 2   , 4, 6   , 8}));
	ASSERT_EQ((f32v4{ 1, 2, 3, 4 } / 2), (f32v4{ 0.5f, 1, 1.5f, 2}));
	ASSERT_EQ((i32v4{ 1, 2, 3, 4 } % 2), (i32v4{ 1   , 0, 1   , 0}));
}

TEST(Vec4, LogicOps)
{
	ASSERT_EQ((i32v4{ 1, 4, 8, 12    } |  i32v4{ 2, 4, 12, 16 }), (i32v4{ 3, 4 , 12, 28  }));
	ASSERT_EQ((i32v4{ 1, 4, 8, 12    } ^  i32v4{ 2, 4, 12, 16 }), (i32v4{ 3, 0 , 4 , 28  }));
	ASSERT_EQ((i32v4{ 1, 4, 8, 12    } &  i32v4{ 2, 4, 12, 16 }), (i32v4{ 0, 4 , 8 , 0   }));
	ASSERT_EQ((i32v4{ 1, 2, 3, 4     } << i32v4{ 2, 3, 4, 5   }), (i32v4{ 4, 16, 48, 128 }));
	ASSERT_EQ((i32v4{ 4, 16, 48, 128 } >> i32v4{ 2, 3, 4, 5   }), (i32v4{ 1, 2 , 3 , 4   }));
}

TEST(Vec4, LogicOpsScalar)
{
	ASSERT_EQ((i32v4{ 1, 4, 12, 18 } |  2 ), (i32v4{ 3, 6, 14, 18 }));
	ASSERT_EQ((i32v4{ 1, 4, 12, 18 } ^  4 ), (i32v4{ 5, 0, 8 , 22 }));
	ASSERT_EQ((i32v4{ 1, 4, 12, 18 } &  20), (i32v4{ 0, 4, 4 , 16  }));
	ASSERT_EQ((i32v4{ 1, 2, 3, 4   } << 2 ), (i32v4{ 4, 8, 12, 16 }));
	ASSERT_EQ((i32v4{ 4, 8, 12, 16 } >> 2 ), (i32v4{ 1, 2, 3 , 4  }));
}

TEST(Vec4, ArithAssignOps)
{
	{
		f32v4 vec{ 1, 2, 3, 4 };
		vec += f32v4{ 2, 4, 6, 8 };
		ASSERT_EQ(vec, (f32v4{ 3, 6, 9, 12 }));
	}
	{
		f32v4 vec{ 1, 2, 3, 4 };
		vec -= f32v4{ 2, 4, 6, 8 };
		ASSERT_EQ(vec, (f32v4{ -1, -2, -3, -4 }));
	}
	{
		f32v4 vec{ 1, 2, 3, 4 };
		vec *= f32v4{ 2, 4, 6, 8 };
		ASSERT_EQ(vec, (f32v4{ 2, 8, 18, 32 }));
	}
	{
		f32v4 vec{ 1, 2, 3, 4 };
		vec /= f32v4{ 2, 8, 3, 32 };
		ASSERT_EQ(vec, (f32v4{ 0.5f, 0.25f, 1.f, 0.125f }));
	}
	{
		i32v4 vec{ 1, 5, 9, 13 };
		vec %= i32v4{ 2, 3, 4, 5 };
		ASSERT_EQ(vec, (i32v4{ 1, 2, 1, 3 }));
	}
}

TEST(Vec4, ArithAssignOpsScalar)
{
	{
		f32v4 vec{ 1, 2, 3, 4 };
		vec += 2;
		ASSERT_EQ(vec, (f32v4{ 3, 4, 5, 6 }));
	}
	{
		f32v4 vec{ 1, 2, 3, 4 };
		vec -= 2;
		ASSERT_EQ(vec, (f32v4{ -1, 0, 1, 2 }));
	}
	{
		f32v4 vec{ 1, 2, 3, 4 };
		vec *= 2;
		ASSERT_EQ(vec, (f32v4{ 2, 4, 6, 8 }));
	}
	{
		f32v4 vec{ 1, 2, 3, 4 };
		vec /= 2;
		ASSERT_EQ(vec, (f32v4{ 0.5f, 1, 1.5f, 2 }));
	}
	{
		i32v4 vec{ 1, 2, 3, 4 };
		vec %= 2;
		ASSERT_EQ(vec, (i32v4{ 1, 0, 1, 0 }));
	}
}

TEST(Vec4, LogicAssignOps)
{
	{
		i32v4 vec{ 1, 4, 8, 12 };
		vec |= i32v4{ 2, 4, 12, 16 };
		ASSERT_EQ(vec, (i32v4{ 3, 4 , 12, 28 }));
	}
	{
		i32v4 vec{ 1, 4, 8, 12 };
		vec ^= i32v4{ 2, 4, 12, 16 };
		ASSERT_EQ(vec, (i32v4{ 3, 0 , 4 , 28 }));
	}
	{
		i32v4 vec{ 1, 4, 8, 12 };
		vec &= i32v4{ 2, 4, 12, 16 };
		ASSERT_EQ(vec, (i32v4{ 0, 4 , 8 , 0 }));
	}
	{
		i32v4 vec{ 1, 2, 3, 4 };
		vec <<= i32v4{ 2, 3, 4, 5 };
		ASSERT_EQ(vec, (i32v4{ 4, 16, 48, 128 }));
	}
	{
		i32v4 vec{ 4, 16, 48, 128 };
		vec >>= i32v4{ 2, 3, 4, 5 };
		ASSERT_EQ(vec, (i32v4{ 1, 2, 3, 4 }));
	}
}

TEST(Vec4, LogicAssignOpsScalar)
{
	{
		i32v4 vec{ 1, 4, 12, 18 };
		vec |= 2;
		ASSERT_EQ(vec, (i32v4{ 3, 6, 14, 18 }));
	}
	{
		i32v4 vec{ 1, 4, 12, 18 };
		vec ^= 4;
		ASSERT_EQ(vec, (i32v4{ 5, 0, 8, 22 }));
	}
	{
		i32v4 vec{ 1, 4, 12, 18 };
		vec &= 20;
		ASSERT_EQ(vec, (i32v4{ 0, 4, 4, 16 }));
	}
	{
		i32v4 vec{ 1, 2, 3, 4 };
		vec <<= 2;
		ASSERT_EQ(vec, (i32v4{ 4, 8, 12, 16 }));
	}
	{
		i32v4 vec{ 4, 8, 12, 16 };
		vec >>= 2;
		ASSERT_EQ(vec, (i32v4{ 1, 2, 3, 4 }));
	}
}

TEST(Vec4, Index)
{
	f32v4 vec{ 1, 3, 5, 7 };
	ASSERT_EQ(vec[0], 1);
	ASSERT_EQ(vec[1], 3);
	ASSERT_EQ(vec[2], 5);
	ASSERT_EQ(vec[3], 7);
}

TEST(Vec4, IsZero)
{
	ASSERT_TRUE ((f32v4{ 0, 0, 0, 0 }.IsZero()));
	ASSERT_FALSE((f32v4{ 0, 2, 0, 1 }.IsZero()));
	ASSERT_TRUE( (f32v4{ 0.1f, 0.1f, 0.f, -0.1f }.IsNearlyZero(0.11f)));
	ASSERT_FALSE((f32v4{ 0.1f, 0.3f, 0.f, -0.2f }.IsNearlyZero(0.11f)));
}

TEST(Vec4, MinMaxComponent)
{
	f32v4 vec{ 1, -3, -5, 7 };
	ASSERT_EQ(vec.MinComponent(), -5);
	ASSERT_EQ(vec.MinAbsComponent(), 1);
	ASSERT_EQ(vec.MaxComponent(), 7);
	ASSERT_EQ(vec.MaxAbsComponent(), 7);
}

TEST(Vec4, MinMax)
{
	f32v4 vec0{ 1, 3, 5, 9 };
	f32v4 vec1{ 4, 2, 6, 8 };
	ASSERT_EQ(vec0.Min(vec1), (f32v4{ 1, 2, 5, 8 }));
	ASSERT_EQ(vec0.Max(vec1), (f32v4{ 4, 3, 6, 9 }));
}

TEST(Vec4, ClampAndSaturate)
{
	ASSERT_EQ((f32v4{ -1.f, 1.f, 2.f, -0.1f }.Clamped(0.f, 1.f)), (f32v4{ 0.f, 1.f, 1.f, 0.0f }));
	ASSERT_EQ((f32v4{ -1.f, 1.f, 2.f, -0.1f }.Clamped(f32v4{ 0.f, 0.f, -2.f, -.2f }, f32v4{ 1.f, .5f, 1.5f, 1.f })), (f32v4{ 0.f, .5f, 1.5f, -.1f }));
	ASSERT_EQ((f32v4{ -2.f, 1.f, 2.f, -0.1f }.ClampedLen(1.f, 2.f)), (f32v4{ -2.f, 1.f, 2.f, -.1f }.Normalized() * 2.f));
	ASSERT_EQ((f32v4{ -1.f, 1.f, 2.f, -0.1f }.Saturated()), (f32v4{ 0.f, 1.f, 1.f, 0.f }));
}

TEST(Vec4, Len)
{
	f32v4 vec{ 3, 4, 5, 6 };
	ASSERT_EQ(vec.LenSq(), 86);
	ASSERT_TRUE((Math::EpsilonCompare(vec.Len(), 9.2736f, 0.0001f)));
}

TEST(Vec4, Distance)
{
	f32v4 vec0{ 1, 2, 3, 4 };
	f32v4 vec1{ 4, 6, 8, 10 };
	ASSERT_EQ(vec0.DistanceSq(vec1), 86);
	ASSERT_TRUE((Math::EpsilonCompare(vec0.Distance(vec1), 9.2736f, 0.0001f)));
}

TEST(Vec4, Dot)
{
	f32v4 vec0{ 1, 2, 3, 4 };
	f32v4 vec1{ 4, 6, 8, 10 };
	ASSERT_EQ(vec0.Dot(vec1), 80);
}

TEST(Vec4, Cross3D)
{
	f32v4 vec0{ 1, 2, 3, 0 };
	f32v4 vec1{ 4, 6, 8, 0 };
	ASSERT_EQ(vec0.Cross3D(vec1), (f32v4{ -2, 4, -2, 0 }));
}

TEST(Vec4, Normalization)
{
	f32v4 vec{ 3, 4, 5, 6 };
	f32v4 norm = vec;
	norm.Normalize();

	ASSERT_TRUE(vec.Normalized().Compare(f32v4{ 0.3234, 0.4313, 0.5391, 0.6469 }, 0.0001f));
	ASSERT_TRUE(norm.Compare(f32v4{ 0.3234, 0.4313, 0.5391, 0.6469 }, 0.0001f));

	ASSERT_FALSE(vec.IsNormalized());
	ASSERT_TRUE(norm.IsNormalized());
}

TEST(Vec4, FaceForward)
{
	f32v4 vec{ -1, -1, -1, 0 };
	f32v4 inc{ 2, -3, -2, 0 };
	f32v4 normal{ 0, 1, 0, 0 };

	ASSERT_EQ(vec.FaceForward3D(inc, normal), vec);
	ASSERT_EQ(vec.FaceForward3D(-inc, normal), -vec);
}

TEST(Vec4, Reflect)
{
	f32v4 vec{ 0, -1, 0, 0 };
	f32v4 normal{ .5f, .5f, .5f, 0 };
	normal.Normalize();

	ASSERT_TRUE(vec.Reflect3D(normal).Compare(f32v4{ .6666f, -.3333f, .6666f, 0 }, 0.0001f));
}

TEST(Vec4, Refract)
{
	f32v4 vec{ -1, -1, -1, 0 };
	vec.Normalize();
	f32v4 normal{ 0, 1, 0, 0 };

	ASSERT_TRUE((vec.Refract3D(normal, 1.0f).Compare(vec)));

	f32 eta = 1.0f / 1.5808f;
	ASSERT_TRUE((vec.Refract3D(normal, eta).Compare(f32v4{ -0.3652, -0.8563, -0.3652, 0 }, 0.0001f)));
}

TEST(Vec4, DistAndLen)
{
	f32v4 vec{ 3, 4, 5, 6 };
	auto [dir, len] = vec.DirectionAndLength();
	ASSERT_TRUE((Math::EpsilonCompare(len, 9.2736f, 0.0001f)));
	ASSERT_EQ(vec.Normalized(), (f32v4{ 3, 4, 5, 6 }.Normalized()));
}

TEST(Vec4, Swizzle)
{
	f32v4 v{ 1, 2, 3, 4 };

	ASSERT_EQ((v.Swizzle<f32v4::X, f32v4::X, f32v4::X, f32v4::X>()), (f32v4{ 1, 1, 1, 1 }));
	ASSERT_EQ((v.Swizzle<f32v4::Y, f32v4::Y, f32v4::Y, f32v4::Y>()), (f32v4{ 2, 2, 2, 2 }));
	ASSERT_EQ((v.Swizzle<f32v4::Z, f32v4::Z, f32v4::Z, f32v4::Z>()), (f32v4{ 3, 3, 3, 3 }));
	ASSERT_EQ((v.Swizzle<f32v4::W, f32v4::W, f32v4::W, f32v4::W>()), (f32v4{ 4, 4, 4, 4 }));

	ASSERT_EQ((v.xxxx()), (f32v4{ 1, 1, 1, 1 }));
	ASSERT_EQ((v.yyyy()), (f32v4{ 2, 2, 2, 2 }));
	ASSERT_EQ((v.zzzz()), (f32v4{ 3, 3, 3, 3 }));
	ASSERT_EQ((v.wwww()), (f32v4{ 4, 4, 4, 4 }));
}

TEST(Vec4, Misc)
{
	f32v4 v0{ 0.5f, -1.25f, 2.75f, -3.49f };

	ASSERT_EQ(v0.Abs(), (f32v4{ 0.5f,  1.25f, 2.75f, 3.49f }));
	ASSERT_EQ(v0.Ceil(), (f32v4{ 1.f , -1.f, 3.f, -3.f }));
	ASSERT_EQ(v0.Floor(), (f32v4{ 0.f , -2.f, 2.f, -4.f }));
	ASSERT_EQ(v0.Trunc(), (f32v4{ 0.f , -1.f, 2.f, -3.f }));
	ASSERT_EQ(v0.Round(), (f32v4{ 1.f , -1.f, 3.f, -3.f }));
	ASSERT_EQ(v0.RoundEven(), (f32v4{ 0.f , -1.f, 3.f, -3.f }));
	ASSERT_EQ(v0.Sign(), (f32v4{ 1.f , -1.f, 1.f, -1.f }));
	ASSERT_TRUE((v0.Fract().Compare(f32v4{ .5f, -.25f, .75f, -.49f }, 0.01f)));
}

TEST(Vec4, QuatMul)
{
	f32q quat = f32q::AxisAngle(f32v3{ 0, 0, 1 }, 90_deg);

	f32v4 v{ 1, 0, 0, 0 };

	f32v4 rot = quat * v * quat.Inverse();
	ASSERT_EQ(rot, (f32v4{ 0, 1, 0, 0 }));

	quat = quat.Invert();
	rot = quat * rot * quat.Inverse();
	ASSERT_EQ(rot, (f32v4{ 1, 0, 0, 0 }));
}

TEST(Vec4, Rotate)
{
	f32q quat = f32q::AxisAngle(f32v3{ 0, 0, 1 }, 90_deg);

	f32v4 v{ 1, 0, 0, 0 };

	f32v4 rot = v.Rotated(quat);
	ASSERT_TRUE(rot.Compare(f32v4{ 0, 1, 0, 0 }));

	rot.Rotate(quat.Inverse());
	ASSERT_TRUE(rot.Compare(f32v4{ 1, 0, 0, 0 }));
}