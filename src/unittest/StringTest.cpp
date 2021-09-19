#include "gtest/gtest.h"
#include "core/Core.h"

TEST(StringTest, DefaultInit)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ alloc };

	ASSERT_EQ(str.Data(), nullptr);
	ASSERT_EQ(str.Capacity(), 0);
	ASSERT_EQ(str.DataSize(), 0);
	ASSERT_EQ(str.Length(), 0);
	ASSERT_TRUE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);
}

TEST(StringTest, CapacityInit)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ 9, alloc };

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 0);
	ASSERT_EQ(str.Length(), 0);
	ASSERT_TRUE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);
}

TEST(StringTest, CountInit)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ 'A', 9, alloc };

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');
}

TEST(StringTest, CStrLenInit)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAAAAAAAAAAAA", 9, alloc };

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');
}

TEST(StringTest, CStrInit)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAAAAAAAA", alloc };

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');
}

TEST(StringTest, InitializerListInit)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ { 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', }, alloc};

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');
}

TEST(StringTest, IteratorInit)
{
	Core::Alloc::Mallocator alloc;
	char src[9] = { 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', };
	Core::String str{ (char*)src, src + 9, alloc };

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');
}

TEST(StringTest, OtherInit)
{
	Core::Alloc::Mallocator alloc;
	Core::String src{ "AAAAAAAAA", alloc };
	Core::String str{ src };

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');

	ASSERT_NE(src.Data(), nullptr);
	ASSERT_GE(src.Capacity(), 9);
	ASSERT_EQ(src.DataSize(), 9);
	ASSERT_EQ(src.Length(), 9);
	ASSERT_FALSE(src.IsEmpty());
	ASSERT_EQ(src.GetAllocator(), &alloc);
}

TEST(StringTest, SubStrInit)
{
	Core::Alloc::Mallocator alloc;
	Core::String src{ "AABAAACAA", alloc };
	Core::String str{ src, 2, 5 };

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 5);
	ASSERT_EQ(str.DataSize(), 5);
	ASSERT_EQ(str.Length(), 5);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'B');
	ASSERT_EQ(str.Back(), 'C');

	ASSERT_NE(src.Data(), nullptr);
	ASSERT_GE(src.Capacity(), 9);
	ASSERT_EQ(src.DataSize(), 9);
	ASSERT_EQ(src.Length(), 9);
	ASSERT_FALSE(src.IsEmpty());
	ASSERT_EQ(src.GetAllocator(), &alloc);
}

TEST(StringTest, MoveOtherInit)
{
	Core::Alloc::Mallocator alloc;
	Core::String src{ "AAAAAAAAA", alloc };
	Core::String str{ Move(src) };

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');

	ASSERT_EQ(src.Data(), nullptr);
	ASSERT_EQ(src.Capacity(), 0);
	ASSERT_EQ(src.DataSize(), 0);
	ASSERT_EQ(src.Length(), 0);
	ASSERT_TRUE(src.IsEmpty());
	ASSERT_EQ(src.GetAllocator(), &alloc);
}

TEST(StringTest, CStrAssignOp)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ alloc };
	str = "AAAAAAAAA";

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');
}

TEST(StringTest, IntializerListAssignOp)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ alloc };
	str = { 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', };

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');
}


TEST(StringTest, OtherAssignOp)
{
	Core::Alloc::Mallocator alloc;
	Core::String src{ "AAAAAAAAA", alloc };
	Core::String str{ alloc };
	str = src;

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');

	ASSERT_NE(src.Data(), nullptr);
	ASSERT_GE(src.Capacity(), 9);
	ASSERT_EQ(src.DataSize(), 9);
	ASSERT_EQ(src.Length(), 9);
	ASSERT_FALSE(src.IsEmpty());
	ASSERT_EQ(src.GetAllocator(), &alloc);
}

TEST(StringTest, MoveOtherAssignOp)
{
	Core::Alloc::Mallocator alloc;
	Core::String src{ "AAAAAAAAA", alloc };
	Core::String str{ alloc };
	str = Move(src);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');

	ASSERT_EQ(src.Data(), nullptr);
	ASSERT_EQ(src.Capacity(), 0);
	ASSERT_EQ(src.DataSize(), 0);
	ASSERT_EQ(src.Length(), 0);
	ASSERT_TRUE(src.IsEmpty());
}

TEST(StringTest, CountAssign)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ alloc };
	str.Assign('A', 9);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');
}

TEST(StringTest, CStrLenAssign)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ alloc };
	str.Assign("AAAAAAAAAAAAA", 9);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');
}

TEST(StringTest, CStrAssign)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ alloc };
	str.Assign("AAAAAAAAA");

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');
}

TEST(StringTest, InitializerListAssign)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ alloc };
	str.Assign({ 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', });

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');
}

TEST(StringTest, IteratorAssign)
{
	Core::Alloc::Mallocator alloc;
	char src[9] = { 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', };
	Core::String str{ alloc };
	str.Assign((char*)src, src + 9);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');
}

TEST(StringTest, SubStrAssign)
{
	Core::Alloc::Mallocator alloc;
	Core::String src{ "AABAAACAA", alloc };
	Core::String str{ alloc };
	str.Assign(src, 2, 5);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 5);
	ASSERT_EQ(str.DataSize(), 5);
	ASSERT_EQ(str.Length(), 5);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'B');
	ASSERT_EQ(str.Back(), 'C');

	ASSERT_NE(src.Data(), nullptr);
	ASSERT_GE(src.Capacity(), 9);
	ASSERT_EQ(src.DataSize(), 9);
	ASSERT_EQ(src.Length(), 9);
	ASSERT_FALSE(src.IsEmpty());
	ASSERT_EQ(src.GetAllocator(), &alloc);
}

TEST(StringTest, Reserve)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ alloc };

	str.Reserve(9);
	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 0);
	ASSERT_EQ(str.Length(), 0);
	ASSERT_TRUE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);
}


TEST(StringTest, ResizeSmaller)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAAAAABAA", alloc };

	str.Resize(7, ' ');

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 7);
	ASSERT_EQ(str.Length(), 7);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'B');
}

TEST(StringTest, ResizeLarger)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAAAAAAAA", alloc };

	str.Resize(12, ' ');

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 12);
	ASSERT_EQ(str.DataSize(), 12);
	ASSERT_EQ(str.Length(), 12);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), ' ');
}

TEST(StringTest, ShrinkToFit)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAAAAAAAA", alloc };
	str.ShrinkToFit();

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_EQ(str.Capacity(), 9 + 1);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'A');
}

TEST(StringTest, AddSingleCodepoint)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAAAAAAAA", alloc };

	str.Add('Z');

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 10);
	ASSERT_EQ(str.DataSize(), 10);
	ASSERT_EQ(str.Length(), 10);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'Z');
}

TEST(StringTest, AddCodepoints)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAAAAAAAA", alloc };

	str.Add('Z', 3);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 12);
	ASSERT_EQ(str.DataSize(), 12);
	ASSERT_EQ(str.Length(), 12);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'Z');
}

TEST(StringTest, AddOther)
{
	Core::Alloc::Mallocator alloc;
	Core::String src{ "BCZ", alloc };
	Core::String str{ "AAAAAAAAA", alloc };

	str.Add(src);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 12);
	ASSERT_EQ(str.DataSize(), 12);
	ASSERT_EQ(str.Length(), 12);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'Z');
}

TEST(StringTest, AddSubStr)
{
	Core::Alloc::Mallocator alloc;
	Core::String src{ "BBCZZ", alloc };
	Core::String str{ "AAAAAAAAA", alloc };

	str.Add(src, 2, 1);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 10);
	ASSERT_EQ(str.DataSize(), 10);
	ASSERT_EQ(str.Length(), 10);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), 'C');
}

TEST(StringTest, PadLeft)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAAAAAAAA", alloc };

	str.PadLeft(2);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 11);
	ASSERT_EQ(str.DataSize(), 11);
	ASSERT_EQ(str.Length(), 11);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), ' ');
	ASSERT_EQ(str.Back(), 'A');
}

TEST(StringTest, PadRight)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAAAAAAAA", alloc };

	str.PadRight(2);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 11);
	ASSERT_EQ(str.DataSize(), 11);
	ASSERT_EQ(str.Length(), 11);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str.Front(), 'A');
	ASSERT_EQ(str.Back(), ' ');
}

TEST(StringTest, EraseCount)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "ABCDEFGHI", alloc };
	str.Erase(3, 2);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 7);
	ASSERT_EQ(str.DataSize(), 7);
	ASSERT_EQ(str.Length(), 7);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);
	
	ASSERT_EQ(str[2], 'C');
	ASSERT_EQ(str[3], 'F');
}

TEST(StringTest, Erase)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "ABCDEFGHI", alloc };
	str.Erase(7);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 7);
	ASSERT_EQ(str.DataSize(), 7);
	ASSERT_EQ(str.Length(), 7);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);
}

TEST(StringTest, TrimLeftCp)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "  \tABC  \n  ", alloc };
	Core::String res{ "\tABC  \n  ", alloc };
	str.TrimLeft(' ');

	ASSERT_EQ(str, res);
}

TEST(StringTest, TrimLeft)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "  \tABC  \n  ", alloc };
	Core::String res{ "ABC  \n  ", alloc };
	str.TrimLeft();

	ASSERT_EQ(str, res);
}

TEST(StringTest, TrimRightCp)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "  \tABC  \n  ", alloc };
	Core::String res{ "  \tABC  \n", alloc };
	str.TrimRight(' ');

	ASSERT_EQ(str, res);
}

TEST(StringTest, TrimRight)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "  \tABC  \n  ", alloc };
	Core::String res{ "  \tABC", alloc };
	str.TrimRight();

	ASSERT_EQ(str, res);
}

TEST(StringTest, TrimCp)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "  \tABC  \n  ", alloc };
	Core::String res{ "\tABC  \n", alloc };
	str.Trim(' ');

	ASSERT_EQ(str, res);
}

TEST(StringTest, Trim)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "  \tABC  \n  ", alloc };
	Core::String res{ "ABC", alloc };
	str.Trim();

	ASSERT_EQ(str, res);
}

TEST(StringTest, InsertOther)
{
	Core::Alloc::Mallocator alloc;
	Core::String src{ "BBB", alloc };
	Core::String str{ "AAAAAAAAA", alloc };

	str.Insert(3, src);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 12);
	ASSERT_EQ(str.DataSize(), 12);
	ASSERT_EQ(str.Length(), 12);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str[2], 'A');
	ASSERT_EQ(str[3], 'B');
	ASSERT_EQ(str[5], 'B');
	ASSERT_EQ(str[6], 'A');
}


TEST(StringTest, InsertSubStr)
{
	Core::Alloc::Mallocator alloc;
	Core::String src{ "CCCCBBBDDDD", alloc };
	Core::String str{ "AAAAAAAAA", alloc };

	str.Insert(3, src, 4, 2);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 11);
	ASSERT_EQ(str.DataSize(), 11);
	ASSERT_EQ(str.Length(), 11);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str[2], 'A');
	ASSERT_EQ(str[3], 'B');
	ASSERT_EQ(str[4], 'B');
	ASSERT_EQ(str[5], 'A');
}

TEST(StringTest, InsertCodepoint)
{
	Core::Alloc::Mallocator alloc;
	Core::String src{ "BBB", alloc };
	Core::String str{ "AAAAAAAAA", alloc };

	str.Insert(3, 'B', 1);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 10);
	ASSERT_EQ(str.DataSize(), 10);
	ASSERT_EQ(str.Length(), 10);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str[2], 'A');
	ASSERT_EQ(str[3], 'B');
	ASSERT_EQ(str[4], 'A');
}

TEST(StringTest, ReplaceRegion)
{
	Core::Alloc::Mallocator alloc;
	Core::String src{ "CCC", alloc };
	Core::String str{ "AAABBAAAA", alloc };

	str.Replace(3, 2, src);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 10);
	ASSERT_EQ(str.DataSize(), 10);
	ASSERT_EQ(str.Length(), 10);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str[2], 'A');
	ASSERT_EQ(str[3], 'C');
	ASSERT_EQ(str[5], 'C');
	ASSERT_EQ(str[6], 'A');
}

TEST(StringTest, ReplaceRegionSubstr)
{
	Core::Alloc::Mallocator alloc;
	Core::String src{ "DDDCCCEEE", alloc };
	Core::String str{ "AAABBAAAA", alloc };

	str.Replace(3, 2, src, 3, 3);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 10);
	ASSERT_EQ(str.DataSize(), 10);
	ASSERT_EQ(str.Length(), 10);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str[2], 'A');
	ASSERT_EQ(str[3], 'C');
	ASSERT_EQ(str[5], 'C');
	ASSERT_EQ(str[6], 'A');
}

TEST(StringTest, ReplaceRegionCodepoint)
{
	Core::Alloc::Mallocator alloc;
	Core::String src{ "DDDCCCEEE", alloc };
	Core::String str{ "AAABBAAAA", alloc };

	str.Replace(3, 2, 'C');

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 8);
	ASSERT_EQ(str.DataSize(), 8);
	ASSERT_EQ(str.Length(), 8);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str[2], 'A');
	ASSERT_EQ(str[3], 'C');
	ASSERT_EQ(str[4], 'A');
}

TEST(StringTest, ReplaceCpWithCp)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABBAAAA", alloc };

	str.Replace('B', 'C');

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str[2], 'A');
	ASSERT_EQ(str[3], 'C');
	ASSERT_EQ(str[4], 'C');
	ASSERT_EQ(str[5], 'A');
}

TEST(StringTest, ReplaceCpWithString)
{
	Core::Alloc::Mallocator alloc;
	Core::String replaceWith{ "CD", alloc };
	Core::String str{ "AAABBAAAA", alloc };

	str.Replace('B', replaceWith);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 11);
	ASSERT_EQ(str.DataSize(), 11);
	ASSERT_EQ(str.Length(), 11);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str[2], 'A');
	ASSERT_EQ(str[3], 'C');
	ASSERT_EQ(str[4], 'D');
	ASSERT_EQ(str[5], 'C');
	ASSERT_EQ(str[6], 'D');
	ASSERT_EQ(str[7], 'A');
}

TEST(StringTest, ReplaceStringWithCp)
{
	Core::Alloc::Mallocator alloc;
	Core::String toReplace{ "BB", alloc };
	Core::String str{ "AAABBAAAA", alloc };

	str.Replace(toReplace, 'C');

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 8);
	ASSERT_EQ(str.DataSize(), 8);
	ASSERT_EQ(str.Length(), 8);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str[2], 'A');
	ASSERT_EQ(str[3], 'C');
	ASSERT_EQ(str[4], 'A');
}

TEST(StringTest, ReplaceStringWithString)
{
	Core::Alloc::Mallocator alloc;
	Core::String toReplace{ "BB", alloc };
	Core::String replaceWith{ "CD", alloc };
	Core::String str{ "AAABBAAAA", alloc };

	str.Replace(toReplace, replaceWith);

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str[2], 'A');
	ASSERT_EQ(str[3], 'C');
	ASSERT_EQ(str[4], 'D');
	ASSERT_EQ(str[5], 'A');
}

TEST(StringTest, ToUpper)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "aaabbaaaa", alloc };
	Core::String res{ "AAABBAAAA", alloc };

	str.ToUpper();

	ASSERT_EQ(str, res);
}

TEST(StringTest, AsUpper)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "aaabbaaaa", alloc };
	Core::String res{ "AAABBAAAA", alloc };

	Core::String upper = str.AsUpper();
	ASSERT_EQ(upper, res);
}

TEST(StringTest, ToLower)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABBAAAA", alloc };
	Core::String res{ "aaabbaaaa", alloc };

	str.ToLower();

	ASSERT_EQ(str, res);
}

TEST(StringTest, AsLower)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABBAAAA", alloc };
	Core::String res{ "aaabbaaaa", alloc };

	Core::String lower = str.AsLower();
	ASSERT_EQ(lower, res);
}

TEST(StringTest, SubString)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABBAAAA", alloc };
	Core::String res{ "ABBA", alloc };

	Core::String subStr = str.SubString(2, 4);
	ASSERT_EQ(subStr, res);
}

TEST(StringTest, SplitCp)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAACBBAACCAA", alloc };
	Core::String res0{ "AAA", alloc };
	Core::String res1{ "BBAA", alloc };
	Core::String res2{ "", alloc };
	Core::String res3{ "AA", alloc };

	Core::DynArray<Core::String> res = str.Split('C');

	ASSERT_EQ(res.Size(), 4);
	ASSERT_EQ(res[0], res0);
	ASSERT_EQ(res[1], res1);
	ASSERT_EQ(res[2], res2);
	ASSERT_EQ(res[3], res3);
}

TEST(StringTest, SplitString)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAACDBBAACDAA", alloc };
	Core::String splitStr{ "CD", alloc };
	Core::String res0{ "AAA", alloc };
	Core::String res1{ "BBAA", alloc };
	Core::String res2{ "AA", alloc };

	Core::DynArray<Core::String> res = str.Split(splitStr);

	ASSERT_EQ(res.Size(), 3);
	ASSERT_EQ(res[0], res0);
	ASSERT_EQ(res[1], res1);
	ASSERT_EQ(res[2], res2);
}

TEST(StringTest, SplitWhitespace)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAA BBAA\nAA", alloc };
	Core::String splitStr{ "CD", alloc };
	Core::String res0{ "AAA", alloc };
	Core::String res1{ "BBAA", alloc };
	Core::String res2{ "AA", alloc };

	Core::DynArray<Core::String> res = str.SplitWhitespace();

	ASSERT_EQ(res.Size(), 3);
	ASSERT_EQ(res[0], res0);
	ASSERT_EQ(res[1], res1);
	ASSERT_EQ(res[2], res2);
}

TEST(StringTest, SplitRemoveEmpty)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAACCBBAACCAA", alloc };
	Core::String res0{ "AAA", alloc };
	Core::String res1{ "BBAA", alloc };
	Core::String res2{ "AA", alloc };

	Core::DynArray<Core::String> res = str.Split('C', Core::String::NPos, Core::StringSplitOption::RemoveEmpty);

	ASSERT_EQ(res.Size(), 3);
	ASSERT_EQ(res[0], res0);
	ASSERT_EQ(res[1], res1);
	ASSERT_EQ(res[2], res2);
}

TEST(StringTest, SplitTrimEntries)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAA  C  BBAA   C AA", alloc };
	Core::String res0{ "AAA", alloc };
	Core::String res1{ "BBAA", alloc };
	Core::String res2{ "AA", alloc };

	Core::DynArray<Core::String> res = str.Split('C', Core::String::NPos, Core::StringSplitOption::TrimEntries);

	ASSERT_EQ(res.Size(), 3);
	ASSERT_EQ(res[0], res0);
	ASSERT_EQ(res[1], res1);
	ASSERT_EQ(res[2], res2);
}

TEST(StringTest, FindCodepoint)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABBAAAA", alloc };

	ASSERT_EQ(str.Find('A'), 0);
	ASSERT_EQ(str.Find('B'), 3);
	ASSERT_EQ(str.Find('B', 4), 4);
	ASSERT_EQ(str.Find('C'), Core::String::NPos);
}

TEST(StringTest, FindString)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABBAAAA", alloc };

	ASSERT_EQ(str.Find({ "AAA", alloc }), 0);
	ASSERT_EQ(str.Find({ "AAA", alloc }, 3), 5);
	ASSERT_EQ(str.Find({ "ABA", alloc }), Core::String::NPos);
}

TEST(StringTest, RFindCodepoint)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABBAAAA", alloc };

	ASSERT_EQ(str.RFind('A'), 8);
	ASSERT_EQ(str.RFind('B'), 4);
	ASSERT_EQ(str.RFind('B', 3), 3);
	ASSERT_EQ(str.RFind('C'), Core::String::NPos);
}

TEST(StringTest, RFindString)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABBAAAA", alloc };

	ASSERT_EQ(str.RFind({ "AAA", alloc }), 6);
	ASSERT_EQ(str.RFind({ "AAA", alloc }, 4), 0);
	ASSERT_EQ(str.RFind({ "ABA", alloc }), Core::String::NPos);
}

TEST(StringTest, FindFirstOf)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABCDAAA", alloc };
	Core::String toFind{ "DC", alloc };

	ASSERT_EQ(str.FindFirstOf(toFind), 4);
}

TEST(StringTest, FindFirstNotOf)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABCDAAA", alloc };
	Core::String toFind{ "AD", alloc };

	ASSERT_EQ(str.FindFirstNotOf(toFind), 3);
}

TEST(StringTest, RFindFirstOf)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABCDAAA", alloc };
	Core::String toFind{ "DC", alloc };

	ASSERT_EQ(str.RFindFirstOf(toFind), 5);
}

TEST(StringTest, RFindFirstNotOf)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABCDAAA", alloc };
	Core::String toFind{ "AD", alloc };

	ASSERT_EQ(str.RFindFirstNotOf(toFind), 4);
}

TEST(StringTest, ContainsCp)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABCDAAA", alloc };

	ASSERT_TRUE(str.Contains('D'));
	ASSERT_FALSE(str.Contains('E'));
}

TEST(StringTest, ContainsString)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABCDAAA", alloc };
	Core::String toFind0{ "CD", alloc };
	Core::String toFind1{ "AD", alloc };

	ASSERT_TRUE(str.Contains(toFind0));
	ASSERT_FALSE(str.Contains(toFind1));
}

TEST(StringTest, StartsWithCp)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABCDAAA", alloc };

	ASSERT_TRUE(str.StartsWith('A'));
	ASSERT_FALSE(str.StartsWith('E'));
}

TEST(StringTest, StartsWithString)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABCDAAA", alloc };
	Core::String start0{ "AA", alloc };
	Core::String start1{ "AD", alloc };

	ASSERT_TRUE(str.StartsWith(start0));
	ASSERT_FALSE(str.StartsWith(start1));
}

TEST(StringTest, EndsWithCp)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABCDAAA", alloc };

	ASSERT_TRUE(str.EndsWith('A'));
	ASSERT_FALSE(str.EndsWith('E'));
}

TEST(StringTest, EndsWithString)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "AAABCDAAA", alloc };
	Core::String end0{ "AA", alloc };
	Core::String end1{ "AD", alloc };

	ASSERT_TRUE(str.EndsWith(end0));
	ASSERT_FALSE(str.EndsWith(end1));
}

TEST(StringTest, Compare)
{
	Core::Alloc::Mallocator alloc;
	Core::String str0{ "AAABBAAAA", alloc };
	Core::String str1{ "AAAABAAAA", alloc };
	Core::String str2{ "AAABBAAAB", alloc };
	Core::String str3{ "AAABBAAA", alloc };
	Core::String str4{ "AAABBAAAAA", alloc };

	ASSERT_EQ(str0.Compare(str0), 0);
	ASSERT_EQ(str0.Compare(str1), 1);
	ASSERT_EQ(str0.Compare(str2), -1);
	ASSERT_EQ(str0.Compare(str3), 1);
	ASSERT_EQ(str0.Compare(str4), -1);
}

TEST(StringTest, IsWhitespace)
{
	Core::Alloc::Mallocator alloc;
	Core::String str0{ "AAAA", alloc };
	Core::String str1{ "  \t\n\r", alloc };

	ASSERT_FALSE(str0.IsWhitespace());
	ASSERT_TRUE(str1.IsWhitespace());
}

TEST(StringTest, Index)
{
	Core::Alloc::Mallocator alloc;
	Core::String str{ "ABCDEFGHI", alloc };

	ASSERT_NE(str.Data(), nullptr);
	ASSERT_GE(str.Capacity(), 9);
	ASSERT_EQ(str.DataSize(), 9);
	ASSERT_EQ(str.Length(), 9);
	ASSERT_FALSE(str.IsEmpty());
	ASSERT_EQ(str.GetAllocator(), &alloc);

	ASSERT_EQ(str[0], 'A');
	ASSERT_EQ(str[1], 'B');
	ASSERT_EQ(str[2], 'C');
	ASSERT_EQ(str[3], 'D');
	ASSERT_EQ(str[4], 'E');
	ASSERT_EQ(str[5], 'F');
	ASSERT_EQ(str[6], 'G');
	ASSERT_EQ(str[7], 'H');
	ASSERT_EQ(str[8], 'I');
}