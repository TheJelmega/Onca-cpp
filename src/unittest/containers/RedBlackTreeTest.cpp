#include "gtest/gtest.h"
#include "core/Core.h"

TEST(RedBlackTreeTest, Init)
{
	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> rb{ alloc };

	ASSERT_EQ(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 0);
	ASSERT_TRUE(rb.IsEmpty());
}

TEST(RedBlackTreeTest, InitInitializerList)
{
	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> rb{ { 2, 5, 7, 10, 12, 15, 17 }, alloc };

	ASSERT_NE(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 7);
	ASSERT_FALSE(rb.IsEmpty());

	ASSERT_EQ(rb.Front(), 2);
	ASSERT_EQ(rb.Back(), 17);
}

TEST(RedBlackTreeTest, InitIterators)
{
	u32 src[] = { 2, 5, 7, 10, 12, 15, 17 };

	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> rb{ (u32*)src, src + 7, alloc };

	ASSERT_NE(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 7);
	ASSERT_FALSE(rb.IsEmpty());

	ASSERT_EQ(rb.Front(), 2);
	ASSERT_EQ(rb.Back(), 17);
}

TEST(RedBlackTreeTest, InitOther)
{
	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> src{ { 2, 5, 7, 10, 12, 15, 17 }, alloc };
	Core::RedBlackTree<u32> rb{ src };

	ASSERT_NE(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 7);
	ASSERT_FALSE(rb.IsEmpty());

	ASSERT_EQ(rb.Front(), 2);
	ASSERT_EQ(rb.Back(), 17);
}

TEST(RedBlackTreeTest, InitMovedOther)
{
	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> src{ { 2, 5, 7, 10, 12, 15, 17 }, alloc };
	Core::RedBlackTree<u32> rb{ Move(src) };

	ASSERT_NE(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 7);
	ASSERT_FALSE(rb.IsEmpty());

	ASSERT_EQ(rb.Front(), 2);
	ASSERT_EQ(rb.Back(), 17);

	ASSERT_EQ(src.Begin(), src.End());
	ASSERT_EQ(src.GetAllocator(), &alloc);
	ASSERT_EQ(src.Size(), 0);
	ASSERT_TRUE(src.IsEmpty());
}

TEST(RedBlackTreeTest, AssignInitializerList)
{
	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> rb{ alloc };

	rb.Assign({ 2, 5, 7, 10, 12, 15, 17 });

	ASSERT_NE(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 7);
	ASSERT_FALSE(rb.IsEmpty());

	ASSERT_EQ(rb.Front(), 2);
	ASSERT_EQ(rb.Back(), 17);
}

TEST(RedBlackTreeTest, AssignIterators)
{
	u32 src[] = { 2, 5, 7, 10, 12, 15, 17 };

	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> rb{ alloc };

	rb.Assign((u32*)src, src + 7);

	ASSERT_NE(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 7);
	ASSERT_FALSE(rb.IsEmpty());

	ASSERT_EQ(rb.Front(), 2);
	ASSERT_EQ(rb.Back(), 17);
}

TEST(RedBlackTreeTest, InsertRoot)
{
	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> rb{ alloc };

	rb.Insert(10);

	ASSERT_NE(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 1);
	ASSERT_FALSE(rb.IsEmpty());

	ASSERT_EQ(rb.Front(), 10);
	ASSERT_EQ(rb.Back(), 10);
}

TEST(RedBlackTreeTest, InsertBalanced)
{
	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> rb{ alloc };

	rb.Insert(10);
	rb.Insert(5);
	rb.Insert(15);
	rb.Insert(2);
	rb.Insert(12);
	rb.Insert(7);
	rb.Insert(17);

	ASSERT_NE(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 7);
	ASSERT_FALSE(rb.IsEmpty());

	ASSERT_EQ(rb.Front(), 2);
	ASSERT_EQ(rb.Back(), 17);

	auto it = rb.Begin();
	ASSERT_EQ(*it++, 2);
	ASSERT_EQ(*it++, 5);
	ASSERT_EQ(*it++, 7);
	ASSERT_EQ(*it++, 10);
	ASSERT_EQ(*it++, 12);
	ASSERT_EQ(*it++, 15);
	ASSERT_EQ(*it++, 17);
}

TEST(RedBlackTreeTest, InsertUnbalanced)
{
	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> rb{ alloc };

	rb.Insert(10);
	rb.Insert(5);
	rb.Insert(2);
	rb.Insert(7);
	rb.Insert(12);
	rb.Insert(15);
	rb.Insert(17);

	ASSERT_NE(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 7);
	ASSERT_FALSE(rb.IsEmpty());

	ASSERT_EQ(rb.Front(), 2);
	ASSERT_EQ(rb.Back(), 17);

	auto it = rb.Begin();
	ASSERT_EQ(*it++, 2);
	ASSERT_EQ(*it++, 5);
	ASSERT_EQ(*it++, 7);
	ASSERT_EQ(*it++, 10);
	ASSERT_EQ(*it++, 12);
	ASSERT_EQ(*it++, 15);
	ASSERT_EQ(*it++, 17);
}

TEST(RedBlackTreeTest, Clear)
{
	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> rb{ { 2, 5, 7, 10, 12, 15, 17 }, alloc };

	rb.Clear();

	ASSERT_EQ(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 0);
	ASSERT_TRUE(rb.IsEmpty());
}


TEST(RedBlackTreeTest, Erase)
{
	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> rb{ { 2, 5, 7, 10, 12, 15, 17 }, alloc };

	rb.Erase(10);

	ASSERT_NE(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 6);
	ASSERT_FALSE(rb.IsEmpty());

	ASSERT_EQ(rb.Front(), 2);
	ASSERT_EQ(rb.Back(), 17);

	auto it = rb.Begin();
	ASSERT_EQ(*it++, 2);
	ASSERT_EQ(*it++, 5);
	ASSERT_EQ(*it++, 7);
	ASSERT_EQ(*it++, 12);
	ASSERT_EQ(*it++, 15);
	ASSERT_EQ(*it++, 17);
}

TEST(RedBlackTreeTest, EraseReverseOrder)
{
	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> rb{ { 2, 5, 7, 10, 12, 15, 17 }, alloc };

	rb.Erase(17);
	rb.Erase(15);
	rb.Erase(12);
	rb.Erase(10);
	rb.Erase(7);
	rb.Erase(5);
	rb.Erase(2);

	ASSERT_EQ(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 0);
	ASSERT_TRUE(rb.IsEmpty());
}

TEST(RedBlackTreeTest, EraseSameOrder)
{
	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> rb{ { 2, 5, 7, 10, 12, 15, 17 }, alloc };

	rb.Erase(2);
	rb.Erase(5);
	rb.Erase(7);
	rb.Erase(10);
	rb.Erase(12);
	rb.Erase(15);
	rb.Erase(17);

	ASSERT_EQ(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 0);
	ASSERT_TRUE(rb.IsEmpty());
}

TEST(RedBlackTreeTest, RandomOrder)
{
	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> rb{ { 2, 5, 7, 10, 12, 15, 17 }, alloc };

	rb.Erase(10);
	rb.Erase(2);
	rb.Erase(17);
	rb.Erase(15);
	rb.Erase(5);
	rb.Erase(12);
	rb.Erase(7);

	ASSERT_EQ(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 0);
	ASSERT_TRUE(rb.IsEmpty());
}

TEST(RedBlackTreeTest, Find)
{
	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> rb{ { 2, 5, 7, 10, 12, 15, 17 }, alloc };

	auto it = rb.Find(12u);

	ASSERT_NE(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 7);
	ASSERT_FALSE(rb.IsEmpty());

	ASSERT_EQ(rb.Front(), 2);
	ASSERT_EQ(rb.Back(), 17);
	
	ASSERT_EQ(*it, 12);
}

TEST(RedBlackTreeTest, Contains)
{
	Core::Alloc::Mallocator alloc;
	Core::RedBlackTree<u32> rb{ { 2, 5, 7, 10, 12, 15, 17 }, alloc };
	
	ASSERT_TRUE(rb.Contains(12u));

	ASSERT_NE(rb.Begin(), rb.End());
	ASSERT_EQ(rb.GetAllocator(), &alloc);
	ASSERT_EQ(rb.Size(), 7);
	ASSERT_FALSE(rb.IsEmpty());

	ASSERT_EQ(rb.Front(), 2);
	ASSERT_EQ(rb.Back(), 17);
}