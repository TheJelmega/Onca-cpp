#include "gtest/gtest.h"
#include "core/Core.h"

TEST(ListTest, DefaultInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ mallocator };
	ASSERT_EQ(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 0);
	EXPECT_TRUE(list.IsEmpty());
}

TEST(ListTest, DefaultValueInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ 20, mallocator };
	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 20);
	EXPECT_FALSE(list.IsEmpty());
}

TEST(ListTest, ValueInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ 20, 42, mallocator };
	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 20);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
	ASSERT_EQ(list.Back(), 42);
}

TEST(ListTest, IteratorInit)
{
	Core::Alloc::Mallocator mallocator;

	u32 src[7] = { 0, 1, 2, 3, 4, 5, 6 };

	Core::List<u32> list{ (u32*)src, src + 7, mallocator };
	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, InitializerListInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };
	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, FromOtherInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> src{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };

	Core::List<u32> list(src);
	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, FromOtherWithAllocInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::Alloc::Mallocator mallocator2;
	Core::List<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);

	Core::List<u32> list{ src, mallocator2 };
	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_NE(list.GetAllocator(), src.GetAllocator());
	ASSERT_EQ(list.GetAllocator(), &mallocator2);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, MovedFromOtherInit)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> src{ { 0, 1, 2, 3, 4, 5, 6 }, mallocator };

	Core::List<u32> list{ Move(src) };
	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);

	ASSERT_EQ(src.Begin(), Core::List<u32>::Iterator{});
	EXPECT_TRUE(src.IsEmpty());
}

TEST(ListTest, InitializerListAssignOp)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ mallocator };
	list = { 0, 1, 2, 3, 4, 5, 6 };

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, FromOtherAssignOp)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);

	Core::List<u32> list{ mallocator };
	list = src;
	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, MovedFromOtherAssignOp)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);

	Core::List<u32> list{ mallocator };
	list = Move(src);
	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);

	ASSERT_EQ(src.Begin(), Core::List<u32>::Iterator{});
	EXPECT_TRUE(src.IsEmpty());
}

TEST(ListTest, InitializerListAssign)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ mallocator };
	list.Assign({ 0, 1, 2, 3, 4, 5, 6 });

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, IteratorAssign)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ mallocator };

	u32 src[7] = { 0, 1, 2, 3, 4, 5, 6 };
	list.Assign((u32*)src, src + 7);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, Fill)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ mallocator };
	list.Fill(20, 42);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 20);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
	ASSERT_EQ(list.Back(), 42);
}

TEST(ListTest, FillDefault)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ mallocator };
	list.FillDefault(20);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 20);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 0);
}

TEST(ListTest, ResizeLarger)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ 10, 42, mallocator };
	list.Resize(20);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 20);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
	ASSERT_EQ(list.Back(), 0);
}

TEST(ListTest, ResizeLargerWithVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ 10, 42, mallocator };
	list.Resize(20);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 20);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
	ASSERT_EQ(list.Back(), 0);
}

TEST(ListTest, ResizeSmaller)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ 20, 42, mallocator };
	list.Resize(10);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 10);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
	ASSERT_EQ(list.Back(), 42);
}

TEST(ListTest, AddVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ mallocator };
	usize val = 42;
	list.Add(static_cast<const u32&>(val));

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 1);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
}

TEST(ListTest, AddMovedVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ mallocator };
	list.Add(42);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 1);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
}

TEST(ListTest, AddDynArr)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::List<u32> list{ mallocator };
	list.Add(src);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, AddMovedDynArr)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::List<u32> list{ mallocator };
	list.Add(Move(src));

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(src.Begin(), Core::List<u32>::Iterator{});
	EXPECT_TRUE(src.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, EmplaceBack)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ mallocator };
	list.EmplaceBack(42u);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 1);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
}

TEST(ListTest, InsertAfterVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	usize val = 42;
	Core::List<u32>::Iterator it = list.Begin() + 4;
	list.InsertAfter(it, static_cast<const u32&>(val));

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 8);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(*it++, 4);
	ASSERT_EQ(*it, 42);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, InsertAfterMovedVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::List<u32>::Iterator it = list.Begin() + 4;
	list.InsertAfter(it, 42);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 8);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(*it++, 4);
	ASSERT_EQ(*it, 42);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, InsertAfterValWithCount)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::List<u32>::Iterator it = list.Begin() + 4;
	list.InsertAfter(it, 5, 42);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 12);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(*it++, 4);
	ASSERT_EQ(*it, 42);
	ASSERT_EQ(*(it + 4), 42);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, InsertAfterIterators)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	u32 src[] = { 40, 41, 42, 43, 44 };
	Core::List<u32>::Iterator it = list.Begin() + 4;
	list.InsertAfter(it, static_cast<u32*>(src), src + 5);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 12);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(*it++, 4);
	ASSERT_EQ(*it, 40);
	ASSERT_EQ(*(it + 4), 44);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, InsertAfterInitializerList)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::List<u32>::Iterator it = list.Begin() + 4;
	list.InsertAfter(it, { 40, 41, 42, 43, 44 });

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 12);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(*it++, 4);
	ASSERT_EQ(*it, 40);
	ASSERT_EQ(*(it + 4), 44);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, InsertAfterOther)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> src({ 40, 41, 42, 43, 44 }, mallocator);
	Core::List<u32> list({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::List<u32>::Iterator it = list.Begin() + 4;
	list.InsertAfter(it, src);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 12);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(*it++, 4);
	ASSERT_EQ(*it, 40);
	ASSERT_EQ(*(it + 4), 44);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, InsertAfterMovedOther)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> src({ 40, 41, 42, 43, 44 }, mallocator);
	Core::List<u32> list({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::List<u32>::Iterator it = list.Begin() + 4;
	list.InsertAfter(it, Move(src));

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 12);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(src.Begin(), Core::List<u32>::Iterator{});
	EXPECT_TRUE(src.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(*it++, 4);
	ASSERT_EQ(*it, 40);
	ASSERT_EQ(*(it + 4), 44);
	ASSERT_EQ(list.Back(), 6);
}

TEST(ListTest, AddFrontVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ { 1 }, mallocator };
	usize val = 42;
	list.AddFront(static_cast<const u32&>(val));

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 2);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
	ASSERT_EQ(list.Back(), 1);
}

TEST(ListTest, AddFrontMovedVal)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ { 1 }, mallocator };
	list.AddFront(42);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 2);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
	ASSERT_EQ(list.Back(), 1);
}

TEST(ListTest, AddFrontDynArr)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::List<u32> list{ { 42 }, mallocator };
	list.AddFront(src);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 8);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 42);
}

TEST(ListTest, AddFrontMovedDynArr)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> src({ 0, 1, 2, 3, 4, 5, 6 }, mallocator);
	Core::List<u32> list{ { 42 }, mallocator };
	list.AddFront(Move(src));

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 8);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(src.Begin(), Core::List<u32>::Iterator{});
	EXPECT_TRUE(src.IsEmpty());

	ASSERT_EQ(list.Front(), 0);
	ASSERT_EQ(list.Back(), 42);
}

TEST(ListTest, EmplacFrontBack)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ { 1 }, mallocator };
	list.EmplaceFront(42u);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 2);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 42);
	ASSERT_EQ(list.Back(), 1);
}

TEST(ListTest, Pop)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ 20, 42, mallocator };
	list.Pop();

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 19);
	EXPECT_FALSE(list.IsEmpty());
}

TEST(ListTest, PopBackElem)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ { 1 }, mallocator };
	list.Pop();

	ASSERT_EQ(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 0);
	EXPECT_TRUE(list.IsEmpty());
}

TEST(ListTest, PopFront)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ 20, 42, mallocator };
	list.PopFront();

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 19);
	EXPECT_FALSE(list.IsEmpty());
}

TEST(ListTest, PopFrontBackElem)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list{ { 1 }, mallocator };
	list.PopFront();

	ASSERT_EQ(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 0);
	EXPECT_TRUE(list.IsEmpty());
}

TEST(ListTest, Erase)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, mallocator);
	Core::List<u32>::Iterator it = list.Begin();
	list.EraseAfter(it + 3);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 9);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(*it, 0);
	ASSERT_EQ(*(it + 3), 3);
	ASSERT_EQ(*(it + 4), 5);
	ASSERT_EQ(*(it + 8), 9);
}

TEST(ListTest, EraseCount)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, mallocator);
	Core::List<u32>::Iterator it = list.Begin();
	list.EraseAfter(it + 3, 3);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(*it, 0);
	ASSERT_EQ(*(it + 3), 3);
	ASSERT_EQ(*(it + 5), 8);
	ASSERT_EQ(*(it + 6), 9);
}

TEST(ListTest, EraseIterators)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, mallocator);
	Core::List<u32>::Iterator it = list.Begin();
	list.EraseAfter(it + 3, it + 7);

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 7);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(*it, 0);
	ASSERT_EQ(*(it + 3), 3);
	ASSERT_EQ(*(it + 5), 8);
	ASSERT_EQ(*(it + 6), 9);
}

TEST(ListTest, Reverse)
{
	Core::Alloc::Mallocator mallocator;
	Core::List<u32> list({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, mallocator);
	list.Reverse();

	ASSERT_NE(list.Begin(), Core::List<u32>::Iterator{});
	ASSERT_EQ(list.GetAllocator(), &mallocator);
	ASSERT_EQ(list.Size(), 10);
	EXPECT_FALSE(list.IsEmpty());

	ASSERT_EQ(list.Front(), 9);
	ASSERT_EQ(list.Back(), 0);
}