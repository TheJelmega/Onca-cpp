#include "gtest/gtest.h"
#include "core/Core.h"

namespace Math = Core::Math;

TEST(Vec2, DefaultInit)
{
	f32v2 v;
	ASSERT_EQ(v.x, 0);
	ASSERT_EQ(v.y, 0);
}

TEST(Vec2, ComponentInit)
{
	f32v2 v{ 1, 2 };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
}

TEST(Vec2, CopyInit)
{
	f32v2 src{ 1, 2 };
	f32v2 v{ src };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
}

TEST(Vec2, CopyDiffTypeInit)
{
	i32v2 src{ 1, 2 };
	f32v2 v{ src };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
}

TEST(Vec2, MoveInit)
{
	f32v2 src{ 1, 2 };
	f32v2 v{ Core::Move(src) };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
}

TEST(Vec2, Assign)
{
	f32v2 v = f32v2{ 1, 2 };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
}

TEST(Vec2, DiffTypeAssign)
{
	f32v2 v = i32v2{ 1, 2 };
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
}

TEST(Vec2, MoveAssign)
{
	f32v2 v = Core::Move(f32v2{ 1, 2 });
	ASSERT_EQ(v.x, 1);
	ASSERT_EQ(v.y, 2);
}

TEST(Vec2, Comparison)
{
	ASSERT_EQ((f32v2{ 1, 2 }), (f32v2{ 1, 2 }));
	ASSERT_NE((f32v2{ 1, 2 }), (f32v2{ 3, 4 }));
	ASSERT_TRUE((f32v2{ 1, 2 }.Compare(f32v2{ 1.1f, 1.9f }, 0.2f)));
}

TEST(Vec2, ArithOps)
{
	ASSERT_EQ((f32v2{ 1, 2 } + f32v2{ 2, 4 }), (f32v2{  3   ,  6     }));
	ASSERT_EQ((f32v2{ 1, 2 } - f32v2{ 2, 4 }), (f32v2{ -1   , -2     }));
	ASSERT_EQ((f32v2{ 1, 2 } * f32v2{ 2, 4 }), (f32v2{  2   ,  8     }));
	ASSERT_EQ((f32v2{ 1, 2 } / f32v2{ 2, 8 }), (f32v2{  0.5f,  0.25f }));
	ASSERT_EQ((i32v2{ 1, 5 } % i32v2{ 2, 3 }), (i32v2{  1,     2     }));
}

TEST(Vec2, ArithOpsScalar)
{
	ASSERT_EQ((f32v2{ 1, 2 } + 2), (f32v2{  3   ,  4 }));
	ASSERT_EQ((f32v2{ 1, 2 } - 2), (f32v2{ -1   ,  0 }));
	ASSERT_EQ((f32v2{ 1, 2 } * 2), (f32v2{  2   ,  4 }));
	ASSERT_EQ((f32v2{ 1, 2 } / 2), (f32v2{  0.5f,  1 }));
	ASSERT_EQ((i32v2{ 1, 2 } % 2), (i32v2{  1   ,  0 }));
}

TEST(Vec2, LogicOps)
{
	ASSERT_EQ((i32v2{ 1, 4  } |  i32v2{ 2, 4 }), (i32v2{ 3, 4  }));
	ASSERT_EQ((i32v2{ 1, 4  } ^  i32v2{ 2, 4 }), (i32v2{ 3, 0  }));
	ASSERT_EQ((i32v2{ 1, 4  } &  i32v2{ 2, 4 }), (i32v2{ 0, 4  }));
	ASSERT_EQ((i32v2{ 1, 2  } << i32v2{ 2, 3 }), (i32v2{ 4, 16 }));
	ASSERT_EQ((i32v2{ 4, 16 } >> i32v2{ 2, 3 }), (i32v2{ 1, 2  }));
}

TEST(Vec2, LogicOpsScalar)
{
	ASSERT_EQ((i32v2{ 1, 4 } |  2), (i32v2{ 3, 6  }));
	ASSERT_EQ((i32v2{ 1, 4 } ^  4), (i32v2{ 5, 0  }));
	ASSERT_EQ((i32v2{ 1, 4 } &  4), (i32v2{ 0, 4  }));
	ASSERT_EQ((i32v2{ 1, 2 } << 2), (i32v2{ 4, 8  }));
	ASSERT_EQ((i32v2{ 4, 8 } >> 2), (i32v2{ 1, 2  }));
}

TEST(Vec2, ArithAssignOps)
{
	{
		f32v2 vec{ 1, 2 };
		vec += f32v2{ 2, 4 };
		ASSERT_EQ(vec, (f32v2{ 3, 6 }));
	}
	{
		f32v2 vec{ 1, 2 };
		vec -= f32v2{ 2, 4 };
		ASSERT_EQ(vec, (f32v2{ -1, -2 }));
	}
	{
		f32v2 vec{ 1, 2 };
		vec *= f32v2{ 2, 4 };
		ASSERT_EQ(vec, (f32v2{ 2, 8 }));
	}
	{
		f32v2 vec{ 1, 2 };
		vec /= f32v2{ 2, 8 };
		ASSERT_EQ(vec, (f32v2{ 0.5f, 0.25f }));
	}
	{
		i32v2 vec{ 1, 2 };
		vec %= i32v2{ 2, 4 };
		ASSERT_EQ(vec, (i32v2{ 1, 2 }));
	}
}

TEST(Vec2, ArithAssignOpsScalar)
{
	{
		f32v2 vec{ 1, 2 };
		vec += 2;
		ASSERT_EQ(vec, (f32v2{ 3, 4 }));
	}
	{
		f32v2 vec{ 1, 2 };
		vec -= 2;
		ASSERT_EQ(vec, (f32v2{ -1, 0 }));
	}
	{
		f32v2 vec{ 1, 2 };
		vec *= 2;
		ASSERT_EQ(vec, (f32v2{ 2, 4 }));
	}
	{
		f32v2 vec{ 1, 2 };
		vec /= 2;
		ASSERT_EQ(vec, (f32v2{ 0.5f, 1 }));
	}
	{
		i32v2 vec{ 1, 2 };
		vec %= 2;
		ASSERT_EQ(vec, (i32v2{ 1, 0 }));
	}
}

TEST(Vec2, LogicAssignOps)
{
	{
		i32v2 vec{ 1, 4 };
		vec |= i32v2{ 2, 4 };
		ASSERT_EQ(vec, (i32v2{ 3, 4 }));
	}
	{
		i32v2 vec{ 1, 4 };
		vec ^= i32v2{ 2, 4 };
		ASSERT_EQ(vec, (i32v2{ 3, 0 }));
	}
	{
		i32v2 vec{ 1, 4 };
		vec &= i32v2{ 2, 4 };
		ASSERT_EQ(vec, (i32v2{ 0, 4 }));
	}
	{
		i32v2 vec{ 1, 2 };
		vec <<= i32v2{ 2, 3 };
		ASSERT_EQ(vec, (i32v2{ 4, 16 }));
	}
	{
		i32v2 vec{ 4, 16 };
		vec >>= i32v2{ 2, 3 };
		ASSERT_EQ(vec, (i32v2{ 1, 2 }));
	}
}

TEST(Vec2, LogicAssignOpsScalar)
{
	{
		i32v2 vec{ 1, 4 };
		vec |= 2;
		ASSERT_EQ(vec, (i32v2{ 3, 6 }));
	}
	{
		i32v2 vec{ 1, 4 };
		vec ^= 4;
		ASSERT_EQ(vec, (i32v2{ 5, 0 }));
	}
	{
		i32v2 vec{ 1, 4 };
		vec &= 4;
		ASSERT_EQ(vec, (i32v2{ 0, 4 }));
	}
	{
		i32v2 vec{ 1, 2 };
		vec <<= 2;
		ASSERT_EQ(vec, (i32v2{ 4, 8 }));
	}
	{
		i32v2 vec{ 4, 8 };
		vec >>= 2;
		ASSERT_EQ(vec, (i32v2{ 1, 2 }));
	}
}

TEST(Vec2, Index)
{
	f32v2 vec{ 1, 3 };
	ASSERT_EQ(vec[0], 1);
	ASSERT_EQ(vec[1], 3);
}

TEST(Vec2, IsZero)
{
	ASSERT_TRUE( (f32v2{ 0, 0 }.IsZero()));
	ASSERT_FALSE((f32v2{ 0, 1 }.IsZero()));
	ASSERT_TRUE( (f32v2{ 0.1f, -0.1f }.IsNearlyZero(0.11f)));
	ASSERT_FALSE((f32v2{ 0.1f, -0.2f }.IsNearlyZero(0.11f)));
}

TEST(Vec2, MinMaxComponent)
{
	f32v2 vec{ 1, -3 };
	ASSERT_EQ(vec.MinComponent(), -3);
	ASSERT_EQ(vec.MinAbsComponent(), 1);
	ASSERT_EQ(vec.MaxComponent(), 1);
	ASSERT_EQ(vec.MaxAbsComponent(), 3);
}

TEST(Vec2, MinMax)
{
	f32v2 vec0{ 1, 3 };
	f32v2 vec1{ 4, 2 };
	ASSERT_EQ(vec0.Min(vec1), (f32v2{ 1, 2 }));
	ASSERT_EQ(vec0.Max(vec1), (f32v2{ 4, 3 }));
}

TEST(Vec2, ClampAndSaturate)
{
	ASSERT_EQ((f32v2{ -1.f, 1.f }.Clamped(0.f, 1.f)), (f32v2{ 0.f, 1.f }));
	ASSERT_EQ((f32v2{ -1.f, 1.f }.Clamped(f32v2{ 0.f, 0.f }, f32v2{ 1.f, .5f })), (f32v2{ 0.f, .5f }));
	ASSERT_EQ((f32v2{ -2.f, 1.f }.ClampedLen(1.f, 2.f)), (f32v2{ -2.f, 1.f }.Normalized() * 2.f));
	ASSERT_EQ((f32v2{ -1.f, 1.f }.Saturated()), (f32v2{ 0.f, 1.f }));
}

TEST(Vec2, Len)
{
	f32v2 vec{ 3, 4 };
	ASSERT_EQ(vec.LenSq(), 25);
	ASSERT_EQ(vec.Len(), 5);
}

TEST(Vec2, Distance)
{
	f32v2 vec0{ 1, 2 };
	f32v2 vec1{ 4, 6 };
	ASSERT_EQ(vec0.DistanceSq(vec1), 25);
	ASSERT_EQ(vec0.Distance(vec1), 5);
}

TEST(Vec2, Dot)
{
	f32v2 vec0{ 1, 2 };
	f32v2 vec1{ 4, 6 };
	ASSERT_EQ(vec0.Dot(vec1), 16);
}

TEST(Vec2, Cross)
{
	f32v2 vec0{ 1, 2 };
	f32v2 vec1{ 4, 6 };
	ASSERT_EQ(vec0.Cross(vec1), -2);
}

TEST(Vec2, Perpendicular)
{
	f32v2 vec{ 1, 2 };
	ASSERT_EQ(vec.PerpendicularCW(), (f32v2{ 2, -1 }));
	ASSERT_EQ(vec.PerpendicularCCW(), (f32v2{ -2, 1 }));
}

TEST(Vec2, Normalization)
{
	f32v2 vec{ 3, 4 };
	f32v2 norm = vec;
	norm.Normalize();

	ASSERT_EQ(vec.Normalized(), (f32v2{ 0.6, 0.8 }));
	ASSERT_EQ(norm, (f32v2{ 0.6, 0.8 }));

	ASSERT_FALSE(vec.IsNormalized());
	ASSERT_TRUE(norm.IsNormalized());
}

TEST(Vec2, FaceForward)
{
	f32v2 vec{ -1, -1 };
	f32v2 inc{ 2, -3 };
	f32v2 normal{ 0, 1 };

	ASSERT_EQ(vec.FaceForward(inc, normal), vec);
	ASSERT_EQ(vec.FaceForward(-inc, normal), -vec);
}

TEST(Vec2, Reflect)
{
	f32v2 vec{ 0, -1 };
	f32v2 normal{ .5f, .5f };
	normal.Normalize();

	ASSERT_TRUE(vec.Reflect(normal).Compare(f32v2{ 1, 0 }));
}

TEST(Vec2, Refract)
{
	f32v2 vec{ -1, -1 };
	vec.Normalize();
	f32v2 normal{ 0, 1 };

	ASSERT_TRUE((vec.Refract(normal, 1.0f).Compare(vec)));

	f32 eta = 1.0f / 1.5808f;
	ASSERT_TRUE((vec.Refract(normal, eta).Compare(f32v2{ -1, -2 }.Normalized(), 0.0001f)));
}

TEST(Vec2, DistAndLen)
{
	f32v2 vec{ 3, 4 };
	auto [dir, len] = vec.DirectionAndLength();
	ASSERT_EQ(len, 5);
	ASSERT_EQ(vec.Normalized(), (f32v2{ 3, 4 }.Normalized()));
}

TEST(Vec2, Swizzle)
{
	f32v2 v{ 1, 2 };

	ASSERT_EQ((v.Swizzle<f32v2::X, f32v2::X>()), (f32v2{ 1, 1 }));
	ASSERT_EQ((v.Swizzle<f32v2::X, f32v2::Y>()), (f32v2{ 1, 2 }));
	ASSERT_EQ((v.Swizzle<f32v2::Y, f32v2::X>()), (f32v2{ 2, 1 }));
	ASSERT_EQ((v.Swizzle<f32v2::Y, f32v2::Y>()), (f32v2{ 2, 2 }));

	ASSERT_EQ(v.xx(), (f32v2{ 1, 1 }));
	ASSERT_EQ(v.xy(), (f32v2{ 1, 2 }));
	ASSERT_EQ(v.yx(), (f32v2{ 2, 1 }));
	ASSERT_EQ(v.yy(), (f32v2{ 2, 2 }));
}

TEST(Vec2, Misc)
{
	f32v2 v0{ 0.5f, -1.25f };

	ASSERT_EQ(v0.Abs()      , (f32v2{ 0.5f,  1.25f }));
	ASSERT_EQ(v0.Ceil()     , (f32v2{ 1.f , -1.f   }));
	ASSERT_EQ(v0.Floor()    , (f32v2{ 0.f , -2.f   }));
	ASSERT_EQ(v0.Trunc()    , (f32v2{ 0.f , -1.f   }));
	ASSERT_EQ(v0.Round()    , (f32v2{ 1.f , -1.f   }));
	ASSERT_EQ(v0.RoundEven(), (f32v2{ 0.f , -1.f   }));
	ASSERT_EQ(v0.Sign()     , (f32v2{ 1.f , -1.f   }));
	ASSERT_EQ(v0.Fract()    , (f32v2{  .5f, - .25f }));
}

TEST(Vec2, Rotate)
{
	f32v2 v{ 1, 0 };

	f32v2 rot = v.Rotated(90_deg);
	ASSERT_TRUE(rot.Compare(f32v2{ 0, 1 }));

	rot.Rotate(-90_deg);
	ASSERT_TRUE(rot.Compare(f32v2{ 1, 0 }));
}