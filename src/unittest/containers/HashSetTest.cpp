#include "gtest/gtest.h"
#include "core/Core.h"

TEST(HashSetTest, DefaultInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> hashset{ mallocator };

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_EQ(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 0);
	ASSERT_EQ(hashset.BucketCount(), 0);
	ASSERT_TRUE(hashset.IsEmpty());
}

TEST(HashSetTest, DefaultInitMinBuckets)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> hashset{ 12, mallocator };

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_EQ(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 0);
	ASSERT_EQ(hashset.BucketCount(), 16);
	ASSERT_TRUE(hashset.IsEmpty());
}

TEST(HashSetTest, InitializerListInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> hashset{ { 0u, 1u, 2u, 3u, 4u }, mallocator };

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_NE(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 5);
	ASSERT_EQ(hashset.BucketCount(), 8);
	ASSERT_FALSE(hashset.IsEmpty());
}

TEST(HashSetTest, InitializerListMinBuckets)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> hashset{
		{ 0u,  1u, 2u, 3u,  4u },
		12,
		mallocator };

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_NE(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 5);
	ASSERT_EQ(hashset.BucketCount(), 16);
	ASSERT_FALSE(hashset.IsEmpty());
}

TEST(HashSetTest, IteratorInit)
{
	u32 src[5] = { 0u, 1u, 2u, 3u, 4u };

	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> hashset{ reinterpret_cast<u32*>(src), src + 5, mallocator };

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_NE(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 5);
	ASSERT_EQ(hashset.BucketCount(), 8);
	ASSERT_FALSE(hashset.IsEmpty());
}

TEST(HashSetTest, IteratorInitMinBuckets)
{
	u32 src[5] = { 0u, 1u, 2u, 3u, 4u };

	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> hashset{ reinterpret_cast<u32*>(src), src + 5, 12, mallocator };

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_NE(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 5);
	ASSERT_EQ(hashset.BucketCount(), 16);
	ASSERT_FALSE(hashset.IsEmpty());
}


TEST(HashSetTest, Copy)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> src{ { 0u, 1u, 2u, 3u, 4u }, mallocator };

	Core::HashSet<u32> hashset{ mallocator };
	hashset = src;

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_NE(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 5);
	ASSERT_EQ(hashset.BucketCount(), 8);
	ASSERT_FALSE(hashset.IsEmpty());

	ASSERT_NE(src.Begin(), nullIt);
	ASSERT_EQ(src.Size(), 5);
	ASSERT_EQ(src.BucketCount(), 8);
	ASSERT_FALSE(src.IsEmpty());
}

TEST(HashSetTest, CopyMove)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> src{ { 0u, 1u, 2u, 3u, 4u }, mallocator };

	Core::HashSet<u32> hashset{ mallocator };
	hashset = Move(src);

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_NE(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 5);
	ASSERT_EQ(hashset.BucketCount(), 8);
	ASSERT_FALSE(hashset.IsEmpty());

	ASSERT_EQ(src.Begin(), nullIt);
	ASSERT_EQ(src.Size(), 0);
	ASSERT_EQ(src.BucketCount(), 0);
	ASSERT_TRUE(src.IsEmpty());
}

TEST(HashSetTest, Rehash)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> hashset{ { 0u, 1u, 2u, 3u, 4u }, mallocator };

	hashset.Rehash(12);

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_NE(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 5);
	ASSERT_EQ(hashset.BucketCount(), 16);
	ASSERT_FALSE(hashset.IsEmpty());
}

TEST(HashSetTest, InsertKeyValue)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> hashset{ mallocator };

	auto it = hashset.Insert(0);

	ASSERT_EQ(*it.first, 0);
	ASSERT_TRUE(it.second);

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_NE(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 1);
	ASSERT_EQ(hashset.BucketCount(), 2);
	ASSERT_FALSE(hashset.IsEmpty());
}

TEST(HashSetTest, InsertMoveKeyValue)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> hashset{ mallocator };

	u32 key = 0;
	auto it = hashset.Insert(key);

	ASSERT_EQ(*it.first, 0);
	ASSERT_TRUE(it.second);

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_NE(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 1);
	ASSERT_EQ(hashset.BucketCount(), 2);
	ASSERT_FALSE(hashset.IsEmpty());
}

TEST(HashSetTest, Clear)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> hashset{ { 0u, 1u, 2u, 3u, 4u }, mallocator };

	hashset.Clear();

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_EQ(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 0);
	ASSERT_EQ(hashset.BucketCount(), 8);
	ASSERT_TRUE(hashset.IsEmpty());
}

TEST(HashSetTest, Find)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> hashset{ { 0u, 1u, 2u, 3u, 4u }, mallocator };

	auto it = hashset.Find(3);
	ASSERT_EQ(*it, 3);

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_NE(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 5);
	ASSERT_EQ(hashset.BucketCount(), 8);
	ASSERT_FALSE(hashset.IsEmpty());
}

TEST(HashSetTest, FindRange)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> hashset{ { 0u, 1u, 2u, 3u, 4u }, mallocator };

	auto it = hashset.FindRange(3);
	ASSERT_NE(it.first, it.second);
	ASSERT_EQ(it.first + 1, it.second);
	ASSERT_EQ(*it.first, 3);

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_NE(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 5);
	ASSERT_EQ(hashset.BucketCount(), 8);
	ASSERT_FALSE(hashset.IsEmpty());
}

TEST(HashSetTest, Contains)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> hashset{ { 0u, 1u, 2u, 3u, 4u }, mallocator };

	ASSERT_TRUE(hashset.Contains(3));
	ASSERT_FALSE(hashset.Contains(9));

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_NE(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 5);
	ASSERT_EQ(hashset.BucketCount(), 8);
	ASSERT_FALSE(hashset.IsEmpty());
}

TEST(HashSetTest, Count)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> hashset{ { 0u, 1u, 2u, 3u, 4u }, mallocator };

	ASSERT_EQ(hashset.Count(3), 1);

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_NE(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 5);
	ASSERT_EQ(hashset.BucketCount(), 8);
	ASSERT_FALSE(hashset.IsEmpty());
}

TEST(HashSetTest, BucketSize)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> hashset{ { 0u, 1u, 2u, 3u, 4u, 8u }, mallocator };

	ASSERT_EQ(hashset.BucketSize(0), 2);
	ASSERT_EQ(hashset.BucketSize(3), 1);

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_NE(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.Size(), 6);
	ASSERT_EQ(hashset.BucketCount(), 8);
	ASSERT_FALSE(hashset.IsEmpty());
}

TEST(HashSetTest, FontLastBeginEnd)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashSet<u32> hashset{ { 0u, 1u, 2u, 3u, 4u }, mallocator };

	Core::HashSet<u32>::Iterator nullIt{};
	ASSERT_NE(hashset.Begin(), nullIt);
	ASSERT_EQ(hashset.End(), nullIt);

	ASSERT_LT(hashset.Front(), 5u);
	ASSERT_LT(hashset.Back(), 5u);
	ASSERT_NE(hashset.Front(), hashset.Back());

	ASSERT_EQ(hashset.Size(), 5);
	ASSERT_EQ(hashset.BucketCount(), 8);
	ASSERT_FALSE(hashset.IsEmpty());
}
