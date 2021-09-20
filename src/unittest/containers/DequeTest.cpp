#include "gtest/gtest.h"
#include "core/Core.h"

TEST(DequeTest, DefaultInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ mallocator };
	ASSERT_EQ(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 0);
	EXPECT_TRUE(deque.IsEmpty());
}

TEST(DequeTest, DefValueInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ 20, mallocator };
	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 20);
	EXPECT_FALSE(deque.IsEmpty());
}

TEST(DequeTest, ValueInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ 20, 42, mallocator };
	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 20);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 42);
	ASSERT_EQ(deque[19], 42);
}

TEST(DequeTest, IteratorInit)
{
	Core::Alloc::Mallocator mallocator;

	u32 src[7] = { 0, 1, 2, 3, 4, 5, 6 };

	Core::Deque<u32> deque{ (u32*)src, src + 7, mallocator };
	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 7);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[6], 6);
}

TEST(DequeTest, InitializerListInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };
	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 7);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[6], 6);
}

TEST(DequeTest, FromOtherInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> src{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };

	Core::Deque<u32> deque(src);
	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 7);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[6], 6);
}

TEST(DequeTest, FromOtherWithAllocInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::Alloc::Mallocator mallocator2;
	Core::Deque<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);

	Core::Deque<u32> deque{ src, mallocator2 };
	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_NE(deque.GetAllocator(), src.GetAllocator());
	ASSERT_EQ(deque.GetAllocator(), &mallocator2);
	ASSERT_EQ(deque.Size(), 7);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[6], 6);
}

TEST(DequeTest, MovedFromOtherInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> src{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };

	Core::Deque<u32> deque{ Move(src) };
	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 7);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[6], 6);

	ASSERT_EQ(src.Begin(), src.End());
	EXPECT_TRUE(src.IsEmpty());
}

TEST(DequeTest, InitializerListAssignOp)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ mallocator };
	deque = { 0, 1, 2, 3, 4, 5, 6 };

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 7);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[6], 6);
}

TEST(DequeTest, FromOtherAssignOp)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);

	Core::Deque<u32> deque{ mallocator };
	deque = src;
	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 7);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[6], 6);
}

TEST(DequeTest, MovedFromOtherAssignOp)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);

	Core::Deque<u32> deque{ mallocator };
	deque = Move(src);
	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 7);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[6], 6);

	ASSERT_EQ(src.Begin(), src.End());
	EXPECT_TRUE(src.IsEmpty());
}

TEST(DequeTest, InitializerListAssign)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ mallocator };
	deque.Assign({ 0, 1, 2, 3, 4, 5, 6 });

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 7);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[6], 6);
}

TEST(DequeTest, IteratorAssign)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ mallocator };

	u32 src[7] = { 0, 1, 2, 3, 4, 5, 6 };
	deque.Assign((u32*)src, src + 7);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 7);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[6], 6);
}

TEST(DequeTest, Fill)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ mallocator };
	deque.Fill(20, 42);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 20);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 42);
	ASSERT_EQ(deque[19], 42);
}

TEST(DequeTest, FillDefault)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ mallocator };
	deque.FillDefault(20);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 20);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[19], 0);
}

TEST(DequeTest, ResizeLarger)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ 10, 42, mallocator };
	deque.Resize(20);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 20);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 42);
	ASSERT_EQ(deque[19], 0);
}

TEST(DequeTest, ResizeLargerWithVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ 10, 42, mallocator };
	deque.Resize(20, 4);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 20);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 42);
	ASSERT_EQ(deque[19], 4);
}

TEST(DequeTest, ResizeSmaller)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ 20, 42, mallocator };
	deque.Resize(10);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 10);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 42);
	ASSERT_EQ(deque[9], 42);
}

TEST(DequeTest, PushFrontVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ mallocator };
	usize val = 42;
	deque.PushFront(static_cast<const u32&>(val));

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 1);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 42);
}

TEST(DequeTest, PushFrontMovedVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ mallocator };
	deque.PushFront(42);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 1);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 42);
}

TEST(DequeTest, PushFrontDynArr)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::Deque<u32> deque{ mallocator };
	deque.PushFront(src);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 7);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[6], 6);
}

TEST(DequeTest, PushFrontMovedDynArr)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::Deque<u32> deque{ mallocator };
	deque.PushFront(Move(src));

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 7);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(src.Begin(), src.End());
	EXPECT_TRUE(src.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[6], 6);
}

TEST(DequeTest, EmplaceFront)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ mallocator };
	deque.EmplaceFront(42u);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 1);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 42);
}

TEST(DequeTest, PushVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ mallocator };
	usize val = 42;
	deque.Push(static_cast<const u32&>(val));

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 1);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 42);
}

TEST(DequeTest, PushMovedVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ mallocator };
	deque.Push(42);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 1);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 42);
}

TEST(DequeTest, PushDynArr)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::Deque<u32> deque{ mallocator };
	deque.Push(src);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 7);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[6], 6);
}

TEST(DequeTest, PushMovedDynArr)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::Deque<u32> deque{ mallocator };
	deque.Push(Move(src));

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 7);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(src.Begin(), src.End());
	EXPECT_TRUE(src.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[6], 6);
}

TEST(DequeTest, EmplaceBack)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ mallocator };
	deque.EmplaceBack(42u);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 1);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 42);
}

TEST(DequeTest, InsertVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	usize val = 42;
	deque.Insert(deque.IteratorAt(4), static_cast<const u32&>(val));

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 8);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[4], 42);
	ASSERT_EQ(deque[7], 6);
}

TEST(DequeTest, InsertMovedVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	deque.Insert(deque.IteratorAt(4), 42);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 8);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[4], 42);
	ASSERT_EQ(deque[7], 6);
}

TEST(DequeTest, InsertValWithCount)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	deque.Insert(deque.IteratorAt(4), 5, 42);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 12);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[4], 42);
	ASSERT_EQ(deque[8], 42);
	ASSERT_EQ(deque[11], 6);
}

TEST(DequeTest, InsertIterators)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	u32 src[] = { 40, 41, 42, 43, 44 };
	deque.Insert(deque.IteratorAt(4), static_cast<u32*>(src), src + 5);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 12);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[4], 40);
	ASSERT_EQ(deque[8], 44);
	ASSERT_EQ(deque[11], 6);
}

TEST(DequeTest, InsertInitializerList)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	deque.Insert(deque.IteratorAt(4), { 40, 41, 42, 43, 44 });

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 12);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[4], 40);
	ASSERT_EQ(deque[8], 44);
	ASSERT_EQ(deque[11], 6);
}

TEST(DequeTest, InsertOther)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> src({ 40, 41, 42, 43, 44 }, mallocator);
	Core::Deque<u32> deque({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	deque.Insert(deque.IteratorAt(4), src);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 12);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[4], 40);
	ASSERT_EQ(deque[8], 44);
	ASSERT_EQ(deque[11], 6);
}

TEST(DequeTest, InsertMovedOther)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> src({ 40, 41, 42, 43, 44 }, mallocator);
	Core::Deque<u32> deque({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	deque.Insert(deque.IteratorAt(4), Move(src));

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 12);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(src.Begin(), src.End());
	EXPECT_TRUE(src.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[4], 40);
	ASSERT_EQ(deque[8], 44);
	ASSERT_EQ(deque[11], 6);
}

TEST(DequeTest, Emplace)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	usize val = 42;
	deque.Emplace(deque.IteratorAt(4), static_cast<const u32&>(val));

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 8);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[4], 42);
	ASSERT_EQ(deque[7], 6);
}

TEST(DequeTest, Clear)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ 20, 42, mallocator };
	deque.Clear(false);

	ASSERT_EQ(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 0);
	EXPECT_TRUE(deque.IsEmpty());
}

TEST(DequeTest, ClearWithDealloc)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ 20, 42, mallocator };
	deque.Clear(true);

	ASSERT_EQ(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 0);
	EXPECT_TRUE(deque.IsEmpty());
}

TEST(DequeTest, Pop)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ { 0, 1, 2, 3, 4, 5 }, mallocator };
	deque.Pop();

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 5);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[4], 4);
}

TEST(DequeTest, PopFront)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque{ { 0, 1, 2, 3, 4, 5 }, mallocator };
	deque.PopFront();

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 5);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 1);
	ASSERT_EQ(deque[4], 5);
}

TEST(DequeTest, Erase)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, mallocator);
	deque.Erase(deque.IteratorAt(4));

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 9);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[3], 3);
	ASSERT_EQ(deque[4], 5);
	ASSERT_EQ(deque[8], 9);
}

TEST(DequeTest, EraseCount)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, mallocator);
	deque.Erase(deque.IteratorAt(4), 3);

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 7);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[3], 3);
	ASSERT_EQ(deque[5], 8);
	ASSERT_EQ(deque[6], 9);
}

TEST(DequeTest, EraseIterators)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, mallocator);
	deque.Erase(deque.IteratorAt(4), deque.IteratorAt(7));

	ASSERT_NE(deque.Begin(), deque.End());
	ASSERT_EQ(deque.GetAllocator(), &mallocator);
	ASSERT_EQ(deque.Size(), 7);
	EXPECT_FALSE(deque.IsEmpty());

	ASSERT_EQ(deque[0], 0);
	ASSERT_EQ(deque[3], 3);
	ASSERT_EQ(deque[5], 8);
	ASSERT_EQ(deque[6], 9);
}

TEST(DequeTest, At)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);

	ASSERT_EQ(deque.At(4), 4);
}

TEST(DequeTest, AtOutOfBounds)
{
	Core::Alloc::Mallocator mallocator;
	Core::Deque<u32> deque({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);

	ASSERT_EQ(deque.At(10), std::nullopt);
}