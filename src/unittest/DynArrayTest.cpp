#include "gtest/gtest.h"
#include "core/Core.h"

TEST(DynArrayTest, DefaultInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ mallocator };
	ASSERT_EQ(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 0);
	ASSERT_EQ(dynArr.Capacity(), 0);
	EXPECT_TRUE(dynArr.IsEmpty());
}

TEST(DynArrayTest, CapacityInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ 20, mallocator };
	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 0);
	ASSERT_GE(dynArr.Capacity(), 20);
	EXPECT_TRUE(dynArr.IsEmpty());
}

TEST(DynArrayTest, ValueInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ 20, 42, mallocator };
	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 20);
	ASSERT_GE(dynArr.Capacity(), 20);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
	ASSERT_EQ(dynArr[19], 42);
}

TEST(DynArrayTest, IteratorInit)
{
	Core::Alloc::Mallocator mallocator;

	u32 src[7] = { 0, 1, 2, 3, 4, 5, 6 };

	Core::DynArray<u32> dynArr{ (u32*)src, src + 7, mallocator };
	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_GE(dynArr.Capacity(), 7);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(DynArrayTest, InitializerListInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };
	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_GE(dynArr.Capacity(), 7);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(DynArrayTest, FromOtherInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> src{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };

	Core::DynArray<u32> dynArr(src);
	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_GE(dynArr.Capacity(), 7);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(DynArrayTest, FromOtherWithAllocInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::Alloc::Mallocator mallocator2;
	Core::DynArray<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);

	Core::DynArray<u32> dynArr{ src, mallocator2 };
	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_NE(dynArr.GetAllocator(), src.GetAllocator());
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator2);
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_GE(dynArr.Capacity(), 7);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(DynArrayTest, MovedFromOtherInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> src{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };

	Core::DynArray<u32> dynArr{ StdMove(src) };
	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_GE(dynArr.Capacity(), 7);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);

	ASSERT_EQ(src.Data(), nullptr);
	EXPECT_TRUE(src.IsEmpty());
}

TEST(DynArrayTest, InitializerListAssignOp)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ mallocator };
	dynArr = { 0, 1, 2, 3, 4, 5, 6 };

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 7);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(DynArrayTest, FromOtherAssignOp)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);

	Core::DynArray<u32> dynArr{ mallocator };
	dynArr = src;
	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_GE(dynArr.Capacity(), 7);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(DynArrayTest, MovedFromOtherAssignOp)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	
	Core::DynArray<u32> dynArr{ mallocator };
	dynArr = StdMove(src);
	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_GE(dynArr.Capacity(), 7);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);

	ASSERT_EQ(src.Data(), nullptr);
	EXPECT_TRUE(src.IsEmpty());
}

TEST(DynArrayTest, InitializerListAssign)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ mallocator };
	dynArr.Assign({ 0, 1, 2, 3, 4, 5, 6 });

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 7);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(DynArrayTest, IteratorAssign)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ mallocator };

	u32 src[7] = { 0, 1, 2, 3, 4, 5, 6 };
	dynArr.Assign((u32*)src, src + 7);

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 7);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(DynArrayTest, Fill)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ mallocator };
	dynArr.Fill(20, 42);

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 20);
	ASSERT_GE(dynArr.Capacity(), 20);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
	ASSERT_EQ(dynArr[19], 42);
}

TEST(DynArrayTest, FillDefault)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ mallocator };
	dynArr.FillDefault(20);

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 20);
	ASSERT_GE(dynArr.Capacity(), 20);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[19], 0);
}

TEST(DynArrayTest, Reserve)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ mallocator };
	dynArr.Reserve(20);

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 0);
	ASSERT_GE(dynArr.Capacity(), 20);
	EXPECT_TRUE(dynArr.IsEmpty());
}

TEST(DynArrayTest, ResizeLarger)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ 10, 42, mallocator };
	dynArr.Resize(20);

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 20);
	ASSERT_GE(dynArr.Capacity(), 20);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
	ASSERT_EQ(dynArr[19], 0);
}

TEST(DynArrayTest, ResizeLargerWithVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ 10, 42, mallocator };
	dynArr.Resize(20, 4);

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 20);
	ASSERT_GE(dynArr.Capacity(), 20);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
	ASSERT_EQ(dynArr[19], 4);
}

TEST(DynArrayTest, ResizeSmaller)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ 20, 42, mallocator };
	dynArr.Resize(10);

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 10);
	ASSERT_GE(dynArr.Capacity(), 10);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
	ASSERT_EQ(dynArr[9], 42);
}

TEST(DynArrayTest, ShrinkToFit)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ 20, mallocator };
	dynArr.Resize(10, 42);
	dynArr.ShrinkToFit();

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 10);
	ASSERT_EQ(dynArr.Capacity(), 10);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
	ASSERT_EQ(dynArr[9], 42);
}

TEST(DynArrayTest, AddVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ mallocator };
	usize val = 42;
	dynArr.Add(static_cast<const u32&>(val));

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 1);
	ASSERT_GE(dynArr.Capacity(), 1);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
}

TEST(DynArrayTest, AddMovedVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ mallocator };
	dynArr.Add(42);

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 1);
	ASSERT_GE(dynArr.Capacity(), 1);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
}

TEST(DynArrayTest, AddDynArr)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::DynArray<u32> dynArr{ mallocator };
	dynArr.Add(src);

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_GE(dynArr.Capacity(), 7);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(DynArrayTest, AddMovedDynArr)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::DynArray<u32> dynArr{ mallocator };
	dynArr.Add(StdMove(src));

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_GE(dynArr.Capacity(), 7);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(src.Data(), nullptr);
	EXPECT_TRUE(src.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(DynArrayTest, EmplaceBack)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ mallocator };
	dynArr.EmplaceBack(42u);

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 1);
	ASSERT_GE(dynArr.Capacity(), 1);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
}

TEST(DynArrayTest, InsertVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	usize val = 42;
	dynArr.Insert(dynArr.IteratorAt(4), static_cast<const u32&>(val));

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 8);
	ASSERT_GE(dynArr.Capacity(), 8);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[4], 42);
	ASSERT_EQ(dynArr[7], 6);
}

TEST(DynArrayTest, InsertMovedVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	dynArr.Insert(dynArr.IteratorAt(4), 42);

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 8);
	ASSERT_GE(dynArr.Capacity(), 8);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[4], 42);
	ASSERT_EQ(dynArr[7], 6);
}

TEST(DynArrayTest, InsertValWithCount)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	dynArr.Insert(dynArr.IteratorAt(4), 5, 42);

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 12);
	ASSERT_GE(dynArr.Capacity(), 12);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[4], 42);
	ASSERT_EQ(dynArr[8], 42);
	ASSERT_EQ(dynArr[11], 6);
}

TEST(DynArrayTest, InsertIterators)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	u32 src[] = { 40, 41, 42, 43, 44 };
	dynArr.Insert(dynArr.IteratorAt(4), static_cast<u32*>(src), src + 5);

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 12);
	ASSERT_GE(dynArr.Capacity(), 12);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[4], 40);
	ASSERT_EQ(dynArr[8], 44);
	ASSERT_EQ(dynArr[11], 6);
}

TEST(DynArrayTest, InsertInitializerList)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	dynArr.Insert(dynArr.IteratorAt(4), { 40, 41, 42, 43, 44 });

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 12);
	ASSERT_GE(dynArr.Capacity(), 12);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[4], 40);
	ASSERT_EQ(dynArr[8], 44);
	ASSERT_EQ(dynArr[11], 6);
}

TEST(DynArrayTest, InsertOther)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> src({ 40, 41, 42, 43, 44 }, mallocator);
	Core::DynArray<u32> dynArr({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	dynArr.Insert(dynArr.IteratorAt(4), src);

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 12);
	ASSERT_GE(dynArr.Capacity(), 12);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[4], 40);
	ASSERT_EQ(dynArr[8], 44);
	ASSERT_EQ(dynArr[11], 6);
}

TEST(DynArrayTest, InsertMovedOther)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> src({ 40, 41, 42, 43, 44 }, mallocator);
	Core::DynArray<u32> dynArr({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	dynArr.Insert(dynArr.IteratorAt(4), StdMove(src));

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 12);
	ASSERT_GE(dynArr.Capacity(), 12);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(src.Data(), nullptr);
	EXPECT_TRUE(src.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[4], 40);
	ASSERT_EQ(dynArr[8], 44);
	ASSERT_EQ(dynArr[11], 6);
}

TEST(DynArrayTest, Emplace)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	usize val = 42;
	dynArr.Emplace(dynArr.IteratorAt(4), static_cast<const u32&>(val));

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 8);
	ASSERT_GE(dynArr.Capacity(), 8);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[4], 42);
	ASSERT_EQ(dynArr[7], 6);
}

TEST(DynArrayTest, Clear)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ 20, 42, mallocator };
	dynArr.Clear(false);

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 0);
	ASSERT_GE(dynArr.Capacity(), 20);
	EXPECT_TRUE(dynArr.IsEmpty());
}

TEST(DynArrayTest, ClearWithDealloc)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ 20, 42, mallocator };
	dynArr.Clear(true);

	ASSERT_EQ(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 0);
	ASSERT_GE(dynArr.Capacity(), 0);
	EXPECT_TRUE(dynArr.IsEmpty());
}

TEST(DynArrayTest, Pop)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr{ 20, 42, mallocator };
	dynArr.Pop();

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 19);
	ASSERT_GE(dynArr.Capacity(), 20);
	EXPECT_FALSE(dynArr.IsEmpty());
}

TEST(DynArrayTest, Erase)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, mallocator);
	dynArr.Erase(dynArr.IteratorAt(4));

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 9);
	ASSERT_GE(dynArr.Capacity(), 10);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[3], 3);
	ASSERT_EQ(dynArr[4], 5);
	ASSERT_EQ(dynArr[8], 9);
}

TEST(DynArrayTest, EraseCount)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, mallocator);
	dynArr.Erase(dynArr.IteratorAt(4), 3);

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_GE(dynArr.Capacity(), 10);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[3], 3);
	ASSERT_EQ(dynArr[5], 8);
	ASSERT_EQ(dynArr[6], 9);
}

TEST(DynArrayTest, EraseIterators)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, mallocator);
	dynArr.Erase(dynArr.IteratorAt(4), dynArr.IteratorAt(7));

	ASSERT_NE(dynArr.Data(), nullptr);
	ASSERT_EQ(dynArr.GetAllocator(), &mallocator);
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_GE(dynArr.Capacity(), 10);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[3], 3);
	ASSERT_EQ(dynArr[5], 8);
	ASSERT_EQ(dynArr[6], 9);
}

TEST(DynArrayTest, At)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);

	ASSERT_EQ(dynArr.At(4), 4);
}

TEST(DynArrayTest, AtOutOfBounds)
{
	Core::Alloc::Mallocator mallocator;
	Core::DynArray<u32> dynArr({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	
	ASSERT_EQ(dynArr.At(10), std::nullopt);
}