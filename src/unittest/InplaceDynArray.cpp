#include "gtest/gtest.h"
#include "core/Core.h"

TEST(InplaceDynArrayTest, DefaultInit)
{
	Core::InplaceDynArray<u32, 24> dynArr;
	ASSERT_EQ(dynArr.Size(), 0);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_TRUE(dynArr.IsEmpty());
}

TEST(InplaceDynArrayTest, DefValueInit)
{
	Core::InplaceDynArray<u32, 24> dynArr(20);
	ASSERT_EQ(dynArr.Size(), 20);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[19], 0);
}

TEST(InplaceDynArrayTest, ValueInit)
{
	Core::InplaceDynArray<u32, 24> dynArr(20, 42);
	ASSERT_EQ(dynArr.Size(), 20);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
	ASSERT_EQ(dynArr[19], 42);
}

TEST(InplaceDynArrayTest, IteratorInit)
{
	u32 src[7] = { 0, 1, 2, 3, 4, 5, 6 };

	Core::InplaceDynArray<u32, 24> dynArr{ (u32*)src, src + 7 };
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(InplaceDynArrayTest, InitializerListInit)
{
	Core::InplaceDynArray<u32, 24> dynArr{ { 0, 1, 2, 3, 4, 5, 6 } };
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(InplaceDynArrayTest, FromOtherInit)
{
	Core::InplaceDynArray<u32, 24> src{ { 0, 1, 2, 3, 4, 5, 6 } };

	Core::InplaceDynArray<u32, 24> dynArr(src);
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(InplaceDynArrayTest, FromOtherWithAllocInit)
{
	Core::InplaceDynArray<u32, 24> src({ 0, 1, 2, 3, 4, 5, 6 });

	Core::InplaceDynArray<u32, 24> dynArr{ src };
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(InplaceDynArrayTest, MovedFromOtherInit)
{
	Core::InplaceDynArray<u32, 24> src{ { 0, 1, 2, 3, 4, 5, 6 } };

	Core::InplaceDynArray<u32, 24> dynArr{ Move(src) };
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
	
	EXPECT_TRUE(src.IsEmpty());
}

TEST(InplaceDynArrayTest, InitializerListAssignOp)
{
	Core::InplaceDynArray<u32, 24> dynArr;
	dynArr = { 0, 1, 2, 3, 4, 5, 6 };
	
	ASSERT_EQ(dynArr.Size(), 7);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(InplaceDynArrayTest, FromOtherAssignOp)
{
	Core::InplaceDynArray<u32, 24> src({ 0, 1, 2, 3, 4, 5, 6 });

	Core::InplaceDynArray<u32, 24> dynArr;
	dynArr = src;
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(InplaceDynArrayTest, MovedFromOtherAssignOp)
{
	Core::InplaceDynArray<u32, 24> src({ 0, 1, 2, 3, 4, 5, 6 });

	Core::InplaceDynArray<u32, 24> dynArr;
	dynArr = Move(src);
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
	
	EXPECT_TRUE(src.IsEmpty());
}

TEST(InplaceDynArrayTest, InitializerListAssign)
{
	Core::Alloc::Mallocator mallocator;
	Core::InplaceDynArray<u32, 24> dynArr;
	dynArr.Assign({ 0, 1, 2, 3, 4, 5, 6 });
	
	ASSERT_EQ(dynArr.Size(), 7);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(InplaceDynArrayTest, IteratorAssign)
{
	Core::InplaceDynArray<u32, 24> dynArr;

	u32 src[7] = { 0, 1, 2, 3, 4, 5, 6 };
	dynArr.Assign((u32*)src, src + 7);
	
	ASSERT_EQ(dynArr.Size(), 7);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(InplaceDynArrayTest, Fill)
{
	Core::InplaceDynArray<u32, 24> dynArr;
	dynArr.Fill(20, 42);
	
	ASSERT_EQ(dynArr.Size(), 20);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
	ASSERT_EQ(dynArr[19], 42);
}

TEST(InplaceDynArrayTest, FillDefault)
{
	Core::InplaceDynArray<u32, 24> dynArr;
	dynArr.FillDefault(20);
	
	ASSERT_EQ(dynArr.Size(), 20);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[19], 0);
}

TEST(InplaceDynArrayTest, ResizeLarger)
{
	Core::InplaceDynArray<u32, 24> dynArr(10, 42);
	dynArr.Resize(20);
	
	ASSERT_EQ(dynArr.Size(), 20);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
	ASSERT_EQ(dynArr[19], 0);
}

TEST(InplaceDynArrayTest, ResizeLargerWithVal)
{
	Core::InplaceDynArray<u32, 24> dynArr(10, 42);
	dynArr.Resize(20, 4);
	
	ASSERT_EQ(dynArr.Size(), 20);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
	ASSERT_EQ(dynArr[19], 4);
}

TEST(InplaceDynArrayTest, ResizeSmaller)
{
	Core::InplaceDynArray<u32, 24> dynArr(20, 42);
	dynArr.Resize(10);
	
	ASSERT_EQ(dynArr.Size(), 10);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
	ASSERT_EQ(dynArr[9], 42);
}

TEST(InplaceDynArrayTest, AddVal)
{
	Core::InplaceDynArray<u32, 24> dynArr;
	usize val = 42;
	dynArr.Add(static_cast<const u32&>(val));
	
	ASSERT_EQ(dynArr.Size(), 1);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
}

TEST(InplaceDynArrayTest, AddMovedVal)
{
	Core::InplaceDynArray<u32, 24> dynArr;
	dynArr.Add(42);
	
	ASSERT_EQ(dynArr.Size(), 1);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
}

TEST(InplaceDynArrayTest, AddDynArr)
{
	Core::InplaceDynArray<u32, 24> src({ 0, 1, 2, 3, 4, 5, 6 });
	Core::InplaceDynArray<u32, 24> dynArr;
	dynArr.Add(src);
	
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(InplaceDynArrayTest, AddMovedDynArr)
{
	Core::InplaceDynArray<u32, 24> src({ 0, 1, 2, 3, 4, 5, 6 });
	Core::InplaceDynArray<u32, 24> dynArr;
	dynArr.Add(Move(src));
	
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());
	
	EXPECT_TRUE(src.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[6], 6);
}

TEST(InplaceDynArrayTest, EmplaceBack)
{
	Core::InplaceDynArray<u32, 24> dynArr;
	dynArr.EmplaceBack(42u);
	
	ASSERT_EQ(dynArr.Size(), 1);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 42);
}

TEST(InplaceDynArrayTest, InsertVal)
{
	Core::InplaceDynArray<u32, 24> dynArr({ 0, 1, 2, 3, 4, 5, 6 });
	usize val = 42;
	dynArr.Insert(dynArr.IteratorAt(4), static_cast<const u32&>(val));
	
	ASSERT_EQ(dynArr.Size(), 8);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[4], 42);
	ASSERT_EQ(dynArr[7], 6);
}

TEST(InplaceDynArrayTest, InsertMovedVal)
{
	Core::InplaceDynArray<u32, 24> dynArr({ 0, 1, 2, 3, 4, 5, 6 });
	dynArr.Insert(dynArr.IteratorAt(4), 42);
	
	ASSERT_EQ(dynArr.Size(), 8);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[4], 42);
	ASSERT_EQ(dynArr[7], 6);
}

TEST(InplaceDynArrayTest, InsertValWithCount)
{
	Core::InplaceDynArray<u32, 24> dynArr({ 0, 1, 2, 3, 4, 5, 6 });
	dynArr.Insert(dynArr.IteratorAt(4), 5, 42);
	
	ASSERT_EQ(dynArr.Size(), 12);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[4], 42);
	ASSERT_EQ(dynArr[8], 42);
	ASSERT_EQ(dynArr[11], 6);
}

TEST(InplaceDynArrayTest, InsertIterators)
{
	Core::InplaceDynArray<u32, 24> dynArr({ 0, 1, 2, 3, 4, 5, 6 });
	u32 src[] = { 40, 41, 42, 43, 44 };
	dynArr.Insert(dynArr.IteratorAt(4), static_cast<u32*>(src), src + 5);
	
	ASSERT_EQ(dynArr.Size(), 12);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[4], 40);
	ASSERT_EQ(dynArr[8], 44);
	ASSERT_EQ(dynArr[11], 6);
}

TEST(InplaceDynArrayTest, InsertInitializerList)
{
	Core::InplaceDynArray<u32, 24> dynArr({ 0, 1, 2, 3, 4, 5, 6 });
	dynArr.Insert(dynArr.IteratorAt(4), { 40, 41, 42, 43, 44 });
	
	ASSERT_EQ(dynArr.Size(), 12);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[4], 40);
	ASSERT_EQ(dynArr[8], 44);
	ASSERT_EQ(dynArr[11], 6);
}

TEST(InplaceDynArrayTest, InsertOther)
{
	Core::InplaceDynArray<u32, 24> src({ 40, 41, 42, 43, 44 });
	Core::InplaceDynArray<u32, 24> dynArr({ 0, 1, 2, 3, 4, 5, 6 });
	dynArr.Insert(dynArr.IteratorAt(4), src);
	
	ASSERT_EQ(dynArr.Size(), 12);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[4], 40);
	ASSERT_EQ(dynArr[8], 44);
	ASSERT_EQ(dynArr[11], 6);
}

TEST(InplaceDynArrayTest, InsertMovedOther)
{
	Core::InplaceDynArray<u32, 24> src({ 40, 41, 42, 43, 44 });
	Core::InplaceDynArray<u32, 24> dynArr({ 0, 1, 2, 3, 4, 5, 6 });
	dynArr.Insert(dynArr.IteratorAt(4), Move(src));
	
	ASSERT_EQ(dynArr.Size(), 12);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());
	
	EXPECT_TRUE(src.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[4], 40);
	ASSERT_EQ(dynArr[8], 44);
	ASSERT_EQ(dynArr[11], 6);
}

TEST(InplaceDynArrayTest, Emplace)
{
	Core::InplaceDynArray<u32, 24> dynArr({ 0, 1, 2, 3, 4, 5, 6 });
	usize val = 42;
	dynArr.Emplace(dynArr.IteratorAt(4), static_cast<const u32&>(val));
	
	ASSERT_EQ(dynArr.Size(), 8);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[4], 42);
	ASSERT_EQ(dynArr[7], 6);
}

TEST(InplaceDynArrayTest, Clear)
{
	Core::InplaceDynArray<u32, 24> dynArr{ 20, 42 };
	dynArr.Clear();
	
	ASSERT_EQ(dynArr.Size(), 0);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_TRUE(dynArr.IsEmpty());
}

TEST(InplaceDynArrayTest, Pop)
{
	Core::InplaceDynArray<u32, 24> dynArr(20, 42);
	dynArr.Pop();
	
	ASSERT_EQ(dynArr.Size(), 19);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());
}

TEST(InplaceDynArrayTest, Erase)
{
	Core::InplaceDynArray<u32, 24> dynArr({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
	dynArr.Erase(dynArr.IteratorAt(4));
	
	ASSERT_EQ(dynArr.Size(), 9);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[3], 3);
	ASSERT_EQ(dynArr[4], 5);
	ASSERT_EQ(dynArr[8], 9);
}

TEST(InplaceDynArrayTest, EraseCount)
{
	Core::InplaceDynArray<u32, 24> dynArr({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
	dynArr.Erase(dynArr.IteratorAt(4), 3);
	
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[3], 3);
	ASSERT_EQ(dynArr[5], 8);
	ASSERT_EQ(dynArr[6], 9);
}

TEST(InplaceDynArrayTest, EraseIterators)
{
	Core::InplaceDynArray<u32, 24> dynArr({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
	dynArr.Erase(dynArr.IteratorAt(4), dynArr.IteratorAt(7));
	
	ASSERT_EQ(dynArr.Size(), 7);
	ASSERT_EQ(dynArr.Capacity(), 24);
	EXPECT_FALSE(dynArr.IsEmpty());

	ASSERT_EQ(dynArr[0], 0);
	ASSERT_EQ(dynArr[3], 3);
	ASSERT_EQ(dynArr[5], 8);
	ASSERT_EQ(dynArr[6], 9);
}

TEST(InplaceDynArrayTest, At)
{
	Core::InplaceDynArray<u32, 24> dynArr({ 0, 1, 2, 3, 4, 5, 6 });

	ASSERT_EQ(dynArr.At(4), 4);
}

TEST(InplaceDynArrayTest, AtOutOfBounds)
{
	Core::InplaceDynArray<u32, 24> dynArr({ 0, 1, 2, 3, 4, 5, 6 });

	ASSERT_EQ(dynArr.At(10), std::nullopt);
}