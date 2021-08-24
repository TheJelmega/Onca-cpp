#include "gtest/gtest.h"
#include "core/Core.h"

TEST(DListTest, DefaultInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ mallocator };
	ASSERT_EQ(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 0);
	EXPECT_TRUE(list.IsEmpty());
}

TEST(DListTest, DefaultValueInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ 20, mallocator };
	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 20);
	EXPECT_FALSE(list.IsEmpty());
}

TEST(DListTest, ValueInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ 20, 42, mallocator };
	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 20);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
	ASSERT_EQ(list.Back(), 42);
}

TEST(DListTest, IteratorInit)
{
	Core::Alloc::Mallocator mallocator;

	u32 src[7] = { 0, 1, 2, 3, 4, 5, 6 };

	Core::DList<u32> list{ (u32*)src, src + 7, mallocator };
	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, InitializerDListInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };
	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, FromOtherInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> src{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };

	Core::DList<u32> list(src);
	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, FromOtherWithAllocInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::Alloc::Mallocator mallocator2;
	Core::DList<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);

	Core::DList<u32> list{ src, mallocator2 };
	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_NE(list.GetAllocator(), src.GetAllocator());
	ASSERT_EQ(list.GetAllocator(), &mallocator2);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, MovedFromOtherInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> src{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };

	Core::DList<u32> list{ Move(src) };
	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);

	ASSERT_EQ(src.Begin(), Core::DList<u32>::Iterator{});
	EXPECT_TRUE(src.IsEmpty());
}

TEST(DListTest, InitializerDListAssignOp)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ mallocator };
	list = { 0, 1, 2, 3, 4, 5, 6 };

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, FromOtherAssignOp)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);

	Core::DList<u32> list{ mallocator };
	list = src;
	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, MovedFromOtherAssignOp)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);

	Core::DList<u32> list{ mallocator };
	list = Move(src);
	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);

	ASSERT_EQ(src.Begin(), Core::DList<u32>::Iterator{});
	EXPECT_TRUE(src.IsEmpty());
}

TEST(DListTest, InitializerDListAssign)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ mallocator };
	list.Assign({ 0, 1, 2, 3, 4, 5, 6 });

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, IteratorAssign)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ mallocator };

	u32 src[7] = { 0, 1, 2, 3, 4, 5, 6 };
	list.Assign((u32*)src, src + 7);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, Fill)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ mallocator };
	list.Fill(20, 42);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 20);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
	ASSERT_EQ(list.Back(), 42);
}

TEST(DListTest, FillDefault)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ mallocator };
	list.FillDefault(20);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 20);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 0);
}

TEST(DListTest, ResizeLarger)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ 10, 42, mallocator };
	list.Resize(20);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 20);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
	ASSERT_EQ(list.Back(), 0);
}

TEST(DListTest, ResizeLargerWithVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ 10, 42, mallocator };
	list.Resize(20);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 20);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
	ASSERT_EQ(list.Back(), 0);
}

TEST(DListTest, ResizeSmaller)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ 20, 42, mallocator };
	list.Resize(10);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 10);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
	ASSERT_EQ(list.Back(), 42);
}

TEST(DListTest, AddVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ mallocator };
	usize val = 42;
	list.Add(static_cast<const u32&>(val));

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 1);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
}

TEST(DListTest, AddMovedVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ mallocator };
	list.Add(42);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 1);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
}

TEST(DListTest, AddDynArr)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::DList<u32> list{ mallocator };
	list.Add(src);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, AddMovedDynArr)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::DList<u32> list{ mallocator };
	list.Add(Move(src));

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(src.Begin(), Core::DList<u32>::Iterator{});
	EXPECT_TRUE(src.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, EmplaceBack)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ mallocator };
	list.EmplaceBack(42u);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 1);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
}

TEST(DListTest, InsertVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	usize val = 42;
	Core::DList<u32>::Iterator it = list.Begin() + 4;
	it = list.Insert(it, static_cast<const u32&>(val));

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 8);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(*(it - 1), 3);
	ASSERT_EQ(*it, 42);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, InsertMovedVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::DList<u32>::Iterator it = list.Begin() + 4;
	it = list.Insert(it, 42);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 8);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(*(it - 1), 3);
	ASSERT_EQ(*it, 42);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, InsertValWithCount)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::DList<u32>::Iterator it = list.Begin() + 4;
	it = list.Insert(it, 5, 42);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 12);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(*(it - 1), 3);
	ASSERT_EQ(*it, 42);
	ASSERT_EQ(*(it + 4), 42);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, InsertIterators)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	u32 src[] = { 40, 41, 42, 43, 44 };
	Core::DList<u32>::Iterator it = list.Begin() + 4;
	it = list.Insert(it, static_cast<u32*>(src), src + 5);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 12);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(*(it - 1), 3);
	ASSERT_EQ(*it, 40);
	ASSERT_EQ(*(it + 4), 44);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, InsertInitializerDList)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::DList<u32>::Iterator it = list.Begin() + 4;
	it = list.Insert(it, { 40, 41, 42, 43, 44 });

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 12);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(*(it - 1), 3);
	ASSERT_EQ(*it, 40);
	ASSERT_EQ(*(it + 4), 44);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, InsertOther)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> src({ 40, 41, 42, 43, 44 }, mallocator);
	Core::DList<u32> list({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::DList<u32>::Iterator it = list.Begin() + 4;
	it = list.Insert(it, src);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 12);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(*(it - 1), 3);
	ASSERT_EQ(*it, 40);
	ASSERT_EQ(*(it + 4), 44);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, InsertMovedOther)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> src({ 40, 41, 42, 43, 44 }, mallocator);
	Core::DList<u32> list({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::DList<u32>::Iterator it = list.Begin() + 4;
	it = list.Insert(it, Move(src));

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 12);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(src.Begin(), Core::DList<u32>::Iterator{});
	EXPECT_TRUE(src.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(*(it - 1), 3);
	ASSERT_EQ(*it, 40);
	ASSERT_EQ(*(it + 4), 44);
	ASSERT_EQ(list.Back(), 6);
}

TEST(DListTest, AddFrontVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ { 1 }, mallocator };
	usize val = 42;
	list.AddFront(static_cast<const u32&>(val));

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 2);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
	ASSERT_EQ(list.Back(), 1);
}

TEST(DListTest, AddFrontMovedVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ { 1 }, mallocator };
	list.AddFront(42);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 2);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
	ASSERT_EQ(list.Back(), 1);
}

TEST(DListTest, AddFrontDynArr)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::DList<u32> list{ { 42 }, mallocator };
	list.AddFront(src);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 8);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 42);
}

TEST(DListTest, AddFrontMovedDynArr)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::DList<u32> list{ { 42 }, mallocator };
	list.AddFront(Move(src));

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 8);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(src.Begin(), Core::DList<u32>::Iterator{});
	EXPECT_TRUE(src.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 42);
}

TEST(DListTest, EmplacFrontBack)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ { 1 }, mallocator };
	list.EmplaceFront(42u);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 2);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
	ASSERT_EQ(list.Back(), 1);
}

TEST(DListTest, Pop)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ 20, 42, mallocator };
	list.Pop();

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 19);
	EXPECT_FALSE(list.IsEmpty());
}

TEST(DListTest, PopBackElem)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ { 1 }, mallocator };
	list.Pop();

	ASSERT_EQ(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 0);
	EXPECT_TRUE(list.IsEmpty());
}

TEST(DListTest, PopFront)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ 20, 42, mallocator };
	list.PopFront();

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 19);
	EXPECT_FALSE(list.IsEmpty());
}

TEST(DListTest, PopFrontBackElem)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list{ { 1 }, mallocator };
	list.PopFront();

	ASSERT_EQ(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 0);
	EXPECT_TRUE(list.IsEmpty());
}

TEST(DListTest, Erase)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, mallocator);
	Core::DList<u32>::Iterator it = list.Begin();
	list.Erase(it + 4);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 9);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(*it, 0);
	ASSERT_EQ(*(it + 3), 3);
	ASSERT_EQ(*(it + 4), 5);
	ASSERT_EQ(*(it + 8), 9);
}

TEST(DListTest, EraseCount)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, mallocator);
	Core::DList<u32>::Iterator it = list.Begin();
	list.Erase(it + 4, 3);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(*it, 0);
	ASSERT_EQ(*(it + 3), 3);
	ASSERT_EQ(*(it + 5), 8);
	ASSERT_EQ(*(it + 6), 9);
}

TEST(DListTest, EraseIterators)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, mallocator);
	Core::DList<u32>::Iterator it = list.Begin();
	list.Erase(it + 4, it + 7);

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(*it, 0);
	ASSERT_EQ(*(it + 3), 3);
	ASSERT_EQ(*(it + 5), 8);
	ASSERT_EQ(*(it + 6), 9);
}

TEST(DListTest, Reverse)
{
	Core::Alloc::Mallocator mallocator;
	Core::DList<u32> list({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, mallocator);
	list.Reverse();

	ASSERT_NE(list.Begin(), Core::DList<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 10);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 9);
	ASSERT_EQ(list.Back(), 0);
}