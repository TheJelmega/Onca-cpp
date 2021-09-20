#include "gtest/gtest.h"
#include "core/Core.h"

TEST(HashMapTest, DefaultInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{ mallocator };

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_EQ(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 0);
	ASSERT_EQ(hashmap.BucketCount(), 0);
	ASSERT_TRUE(hashmap.IsEmpty());
}

TEST(HashMapTest, DefaultInitMinBuckets)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{ 12, mallocator };

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_EQ(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 0);
	ASSERT_EQ(hashmap.BucketCount(), 16);
	ASSERT_TRUE(hashmap.IsEmpty());
}

TEST(HashMapTest, InitializerListInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{
		{ Core::Pair{0u, 1u},
		  Core::Pair{1u, 2u},
		  Core::Pair{2u, 3u},
		  Core::Pair{3u, 4u},
		  Core::Pair{4u, 5u} },
		mallocator };

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 5);
	ASSERT_EQ(hashmap.BucketCount(), 8);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, InitializerListMinBuckets)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{
		{ Core::Pair{0u, 1u},
		  Core::Pair{1u, 2u},
		  Core::Pair{2u, 3u},
		  Core::Pair{3u, 4u},
		  Core::Pair{4u, 5u} },
		12,
		mallocator };

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 5);
	ASSERT_EQ(hashmap.BucketCount(), 16);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, IteratorInit)
{
	Core::Pair<u32, u32> src[5] = {
		Core::Pair{0u, 1u},
		Core::Pair{1u, 2u},
		Core::Pair{2u, 3u},
		Core::Pair{3u, 4u},
		Core::Pair{4u, 5u}
	};

	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{ reinterpret_cast<Core::Pair<u32, u32>*>(src), src + 5, mallocator };

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 5);
	ASSERT_EQ(hashmap.BucketCount(), 8);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, IteratorInitMinBuckets)
{
	Core::Pair<u32, u32> src[5] = {
		Core::Pair{0u, 1u},
		Core::Pair{1u, 2u},
		Core::Pair{2u, 3u},
		Core::Pair{3u, 4u},
		Core::Pair{4u, 5u}
	};

	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{ reinterpret_cast<Core::Pair<u32, u32>*>(src), src + 5, 12, mallocator };

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 5);
	ASSERT_EQ(hashmap.BucketCount(), 16);
	ASSERT_FALSE(hashmap.IsEmpty());
}


TEST(HashMapTest, Copy)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> src{
		{ Core::Pair{0u, 1u},
		  Core::Pair{1u, 2u},
		  Core::Pair{2u, 3u},
		  Core::Pair{3u, 4u},
		  Core::Pair{4u, 5u} },
		mallocator };

	Core::HashMap<u32, u32> hashmap{ mallocator };
	hashmap = src;

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 5);
	ASSERT_EQ(hashmap.BucketCount(), 8);
	ASSERT_FALSE(hashmap.IsEmpty());
	
	ASSERT_NE(src.Begin(), nullIt);
	ASSERT_EQ(src.Size(), 5);
	ASSERT_EQ(src.BucketCount(), 8);
	ASSERT_FALSE(src.IsEmpty());
}

TEST(HashMapTest, CopyMove)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> src{
		{ Core::Pair{0u, 1u},
		  Core::Pair{1u, 2u},
		  Core::Pair{2u, 3u},
		  Core::Pair{3u, 4u},
		  Core::Pair{4u, 5u} },
		mallocator };

	Core::HashMap<u32, u32> hashmap{ mallocator };
	hashmap = Move(src);

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 5);
	ASSERT_EQ(hashmap.BucketCount(), 8);
	ASSERT_FALSE(hashmap.IsEmpty());

	ASSERT_EQ(src.Begin(), nullIt);
	ASSERT_EQ(src.Size(), 0);
	ASSERT_EQ(src.BucketCount(), 0);
	ASSERT_TRUE(src.IsEmpty());
}

TEST(HashMapTest, Rehash)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{
		{ Core::Pair{0u, 1u},
		  Core::Pair{1u, 2u},
		  Core::Pair{2u, 3u},
		  Core::Pair{3u, 4u},
		  Core::Pair{4u, 5u} },
		mallocator };

	hashmap.Rehash(12);

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 5);
	ASSERT_EQ(hashmap.BucketCount(), 16);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, InsertPair)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{ mallocator };

	Core::Pair p{ 0u, 324u };
	auto it = hashmap.Insert(p);

	ASSERT_EQ(it.first->first, 0);
	ASSERT_EQ(it.first->second, 324);
	ASSERT_TRUE(it.second);

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 1);
	ASSERT_EQ(hashmap.BucketCount(), 2);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, InsertMovePair)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{ mallocator };

	auto it = hashmap.Insert(Core::Pair{ 0u, 324u });

	ASSERT_EQ(it.first->first, 0);
	ASSERT_EQ(it.first->second, 324);
	ASSERT_TRUE(it.second);

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 1);
	ASSERT_EQ(hashmap.BucketCount(), 2);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, InsertKeyValue)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{ mallocator };

	auto it = hashmap.Insert(0, 324);

	ASSERT_EQ(it.first->first, 0);
	ASSERT_EQ(it.first->second, 324);
	ASSERT_TRUE(it.second);

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 1);
	ASSERT_EQ(hashmap.BucketCount(), 2);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, InsertMoveKeyValue)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{ mallocator };

	u32 key = 0, val = 324;
	auto it = hashmap.Insert(key, val);

	ASSERT_EQ(it.first->first, 0);
	ASSERT_EQ(it.first->second, 324);
	ASSERT_TRUE(it.second);

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 1);
	ASSERT_EQ(hashmap.BucketCount(), 2);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, TryInsertPair)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{
		{ Core::Pair{0u, 1u},
		  Core::Pair{1u, 2u},
		  Core::Pair{2u, 3u},
		  Core::Pair{3u, 4u},
		  Core::Pair{4u, 5u} },
		mallocator };

	Core::Pair p{ 0u, 324u };
	auto it = hashmap.TryInsert(p);

	ASSERT_EQ(it.first->first, 0);
	ASSERT_EQ(it.first->second, 1);
	ASSERT_FALSE(it.second);

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 5);
	ASSERT_EQ(hashmap.BucketCount(), 8);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, TryInsertMovePair)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{
		{ Core::Pair{0u, 1u},
		  Core::Pair{1u, 2u},
		  Core::Pair{2u, 3u},
		  Core::Pair{3u, 4u},
		  Core::Pair{4u, 5u} },
		mallocator };

	auto it = hashmap.TryInsert(Core::Pair{ 0u, 324u });

	ASSERT_EQ(it.first->first, 0);
	ASSERT_EQ(it.first->second, 1);
	ASSERT_FALSE(it.second);

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 5);
	ASSERT_EQ(hashmap.BucketCount(), 8);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, TryInsertKeyValue)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{
		{ Core::Pair{0u, 1u},
		  Core::Pair{1u, 2u},
		  Core::Pair{2u, 3u},
		  Core::Pair{3u, 4u},
		  Core::Pair{4u, 5u} },
		mallocator };

	auto it = hashmap.TryInsert(0, 324);

	ASSERT_EQ(it.first->first, 0);
	ASSERT_EQ(it.first->second, 1);
	ASSERT_FALSE(it.second);

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 5);
	ASSERT_EQ(hashmap.BucketCount(), 8);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, TryInsertMoveKeyValue)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{
		{ Core::Pair{0u, 1u},
		  Core::Pair{1u, 2u},
		  Core::Pair{2u, 3u},
		  Core::Pair{3u, 4u},
		  Core::Pair{4u, 5u} },
		mallocator };

	u32 key = 0, val = 324;
	auto it = hashmap.TryInsert(key, val);

	ASSERT_EQ(it.first->first, 0);
	ASSERT_EQ(it.first->second, 1);
	ASSERT_FALSE(it.second);

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 5);
	ASSERT_EQ(hashmap.BucketCount(), 8);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, Clear)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{
		{ Core::Pair{0u, 1u},
		  Core::Pair{1u, 2u},
		  Core::Pair{2u, 3u},
		  Core::Pair{3u, 4u},
		  Core::Pair{4u, 5u} },
		mallocator };

	hashmap.Clear();

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_EQ(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 0);
	ASSERT_EQ(hashmap.BucketCount(), 8);
	ASSERT_TRUE(hashmap.IsEmpty());
}

TEST(HashMapTest, Find)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{
		{ Core::Pair{0u, 1u},
		  Core::Pair{1u, 2u},
		  Core::Pair{2u, 3u},
		  Core::Pair{3u, 4u},
		  Core::Pair{4u, 5u} },
		mallocator };

	auto it = hashmap.Find(3);
	ASSERT_EQ(it->first, 3);
	ASSERT_EQ(it->second, 4);

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 5);
	ASSERT_EQ(hashmap.BucketCount(), 8);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, FindRange)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{
		{ Core::Pair{0u, 1u},
		  Core::Pair{1u, 2u},
		  Core::Pair{2u, 3u},
		  Core::Pair{3u, 4u},
		  Core::Pair{4u, 5u} },
		mallocator };

	auto it = hashmap.FindRange(3);
	ASSERT_NE(it.first, it.second);
	ASSERT_EQ(it.first + 1, it.second);
	ASSERT_EQ(it.first->first, 3);
	ASSERT_EQ(it.first->second, 4);

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 5);
	ASSERT_EQ(hashmap.BucketCount(), 8);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, Contains)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{
		{ Core::Pair{0u, 1u},
		  Core::Pair{1u, 2u},
		  Core::Pair{2u, 3u},
		  Core::Pair{3u, 4u},
		  Core::Pair{4u, 5u} },
		mallocator };

	ASSERT_TRUE(hashmap.Contains(3));
	ASSERT_FALSE(hashmap.Contains(9));

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 5);
	ASSERT_EQ(hashmap.BucketCount(), 8);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, AtIndexCount)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{
		{ Core::Pair{0u, 1u},
		  Core::Pair{1u, 2u},
		  Core::Pair{2u, 3u},
		  Core::Pair{3u, 4u},
		  Core::Pair{4u, 5u} },
		mallocator };

	ASSERT_EQ(hashmap.At(2), 3);
	ASSERT_EQ(hashmap.At(9), NullOpt);

	ASSERT_EQ(hashmap[4], 5);

	ASSERT_EQ(hashmap.Count(3), 1);

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 5);
	ASSERT_EQ(hashmap.BucketCount(), 8);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, BucketSize)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{
		{ Core::Pair{0u, 1u},
		  Core::Pair{1u, 2u},
		  Core::Pair{2u, 3u},
		  Core::Pair{3u, 4u},
		  Core::Pair{4u, 5u},
		  Core::Pair{8u, 16u} },
		mallocator };

	ASSERT_EQ(hashmap.BucketSize(0), 2);
	ASSERT_EQ(hashmap.BucketSize(3), 1);

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.Size(), 6);
	ASSERT_EQ(hashmap.BucketCount(), 8);
	ASSERT_FALSE(hashmap.IsEmpty());
}

TEST(HashMapTest, FontLastBeginEnd)
{
	Core::Alloc::Mallocator mallocator;
	Core::HashMap<u32, u32> hashmap{
		{ Core::Pair{0u, 1u},
		  Core::Pair{1u, 2u},
		  Core::Pair{2u, 3u},
		  Core::Pair{3u, 4u},
		  Core::Pair{4u, 5u} },
		mallocator };

	ASSERT_EQ(hashmap.At(2), 3);
	ASSERT_EQ(hashmap.At(9), NullOpt);

	ASSERT_EQ(hashmap[4], 5);

	Core::HashMap<u32, u32>::Iterator nullIt{};
	ASSERT_NE(hashmap.Begin(), nullIt);
	ASSERT_EQ(hashmap.End(), nullIt);

	ASSERT_LT(hashmap.Front().first, 5u);
	ASSERT_LT(hashmap.Back().first, 5u);
	ASSERT_NE(hashmap.Front().first, hashmap.Back().first);

	ASSERT_EQ(hashmap.Size(), 5);
	ASSERT_EQ(hashmap.BucketCount(), 8);
	ASSERT_FALSE(hashmap.IsEmpty());
}
