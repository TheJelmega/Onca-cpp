#include "gtest/gtest.h"
#include "core/Core.h"

namespace Math = Core::Math;

TEST(MathUtilsTest, MinMax)
{
	ASSERT_EQ(Math::Min(1, 2), 1);
	ASSERT_EQ(Math::Max(1, 2), 2);
}

TEST(MathUtilsTest, Clamp)
{
	ASSERT_EQ((Math::Clamp(-0.5, 0.0, 1.0)), 0.0);
	ASSERT_EQ((Math::Clamp( 0.5, 0.0, 1.0)), 0.5);
	ASSERT_EQ((Math::Clamp( 1.5, 0.0, 1.0)), 1.0);
}

TEST(MathUtilsTest, Lerp)
{
	ASSERT_TRUE(Math::EpsilonCompare(Math::Lerp(0.0, 2.0, -1.0), -2.0));
	ASSERT_TRUE(Math::EpsilonCompare(Math::Lerp(0.0, 2.0,  0.0),  0.0));
	ASSERT_TRUE(Math::EpsilonCompare(Math::Lerp(0.0, 2.0,  0.5),  1.0));
	ASSERT_TRUE(Math::EpsilonCompare(Math::Lerp(0.0, 2.0,  1.0),  2.0));
	ASSERT_TRUE(Math::EpsilonCompare(Math::Lerp(0.0, 2.0,  2.0),  4.0));
}

TEST(MathUtilsTest, SmoothStep)
{
	ASSERT_TRUE(Math::EpsilonCompare(Math::SmoothStep(1.0, 3.0, 0.0), 0.0    ));
	ASSERT_TRUE(Math::EpsilonCompare(Math::SmoothStep(1.0, 3.0, 1.0), 0.0    ));
	ASSERT_TRUE(Math::EpsilonCompare(Math::SmoothStep(1.0, 3.0, 1.5), 0.15625));
	ASSERT_TRUE(Math::EpsilonCompare(Math::SmoothStep(1.0, 3.0, 2.0), 0.5    ));
	ASSERT_TRUE(Math::EpsilonCompare(Math::SmoothStep(1.0, 3.0, 2.5), 0.84375));
	ASSERT_TRUE(Math::EpsilonCompare(Math::SmoothStep(1.0, 3.0, 3.0), 1.0    ));
	ASSERT_TRUE(Math::EpsilonCompare(Math::SmoothStep(1.0, 3.0, 4.0), 1.0    ));
}

TEST(MathUtilsTest, Step)
{
	ASSERT_EQ((Math::Step(0.5, 0.0)), 0.0);
	ASSERT_EQ((Math::Step(0.5, 0.5)), 1.0);
	ASSERT_EQ((Math::Step(0.5, 1.0)), 1.0);
}

TEST(MathUtilsTest, Abs)
{
	ASSERT_EQ(Math::Abs( 1.0), 1.0);
	ASSERT_EQ(Math::Abs( 0.0), 0.0);
	ASSERT_EQ(Math::Abs(-1.0), 1.0);
}

TEST(MathUtilsTest, Floor)
{
	ASSERT_EQ(Math::Floor(3.9), 3);
	ASSERT_EQ(Math::Floor(-3.9), -4);
}

TEST(MathUtilsTest, Ceil)
{
	ASSERT_EQ(Math::Ceil(3.9), 4);
	ASSERT_EQ(Math::Ceil(-3.9), -3);
}

TEST(MathUtilsTest, Trunc)
{
	ASSERT_EQ(Math::Trunc(3.9), 3);
	ASSERT_EQ(Math::Trunc(-3.9), -3);
}

TEST(MathUtilsTest, Round)
{
	ASSERT_EQ(Math::Round(1.0f), 1);
	ASSERT_EQ(Math::Round(1.3f), 1);
	ASSERT_EQ(Math::Round(1.5f), 2);
	ASSERT_EQ(Math::Round(1.7f), 2);
}

TEST(MathUtilsTest, RoundEven)
{
	ASSERT_EQ(Math::RoundEven(1.0f), 1);
	ASSERT_EQ(Math::RoundEven(1.3f), 1);
	ASSERT_EQ(Math::RoundEven(1.5f), 2);
	ASSERT_EQ(Math::RoundEven(1.7f), 2);
	ASSERT_EQ(Math::RoundEven(2.5f), 2);
}

TEST(MathUtilsTest, Sign)
{
	ASSERT_EQ(Math::Sign(-1.0), -1);
	ASSERT_EQ(Math::Sign( 0.0),  0);
	ASSERT_EQ(Math::Sign( 1.0),  1);
}

TEST(MathUtilsTest, Mod)
{
	ASSERT_TRUE(Math::EpsilonCompare(Math::Mod(2.0, 1.0), 0.0));
	ASSERT_TRUE(Math::EpsilonCompare(Math::Mod(2.5, 1.0), 0.5));
}

TEST(MathUtilsTest, ModF)
{
	Core::Pair<i64, f64> pair;

	pair = Math::ModF(1.9f);
	ASSERT_EQ(pair.first, 1);
	ASSERT_TRUE(Math::EpsilonCompare(pair.second, 0.9, 0.0001));
}

TEST(MathUtilsTest, EpsilonCompare)
{
	ASSERT_FALSE(Math::EpsilonCompare(0.0, 0.1));
	ASSERT_TRUE(Math::EpsilonCompare(0.0, 0.1, 0.15));
}