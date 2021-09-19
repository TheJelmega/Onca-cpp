#include "gtest/gtest.h"
#include "core/Core.h"

using Utf8Char = Core::Unicode::Utf8Char;

TEST(UnicodeUtilsTest, IsWhitespace)
{
	ASSERT_TRUE(Core::Unicode::IsWhitespace(' '));
	ASSERT_FALSE(Core::Unicode::IsWhitespace('1'));
	ASSERT_TRUE(Core::Unicode::IsWhitespace(0x205F));

	Utf8Char c = Core::Unicode::GetUtf8FromCp(' ');
	ASSERT_TRUE(Core::Unicode::IsWhitespace(c.data));
	c = Core::Unicode::GetUtf8FromCp(0x10912);
	ASSERT_FALSE(Core::Unicode::IsWhitespace(c.data));
	c = Core::Unicode::GetUtf8FromCp(0x205F);
	ASSERT_TRUE(Core::Unicode::IsWhitespace(c.data));
}

TEST(UnicodeUtilsTest, ToUpper)
{
	ASSERT_EQ(Core::Unicode::ToUpper('a'), 'A');
	ASSERT_EQ(Core::Unicode::ToUpper(' '), ' ');
	ASSERT_EQ(Core::Unicode::ToUpper(Core::Unicode::UnicodeCaseLowerTable[387]), Core::Unicode::UnicodeCaseUpperTable[387]);

	Utf8Char c = Core::Unicode::GetUtf8FromCp(' ');
	Utf8Char res = Core::Unicode::ToUpper(c.data);
	ASSERT_EQ(res.data[0], ' ');

	c = Core::Unicode::GetUtf8FromCp('a');
	res = Core::Unicode::ToUpper(c.data);
	ASSERT_EQ(res.data[0], 'A');

	c = Core::Unicode::GetUtf8FromCp(Core::Unicode::ToUpper(Core::Unicode::UnicodeCaseLowerTable[387]));
	res = Core::Unicode::ToLower(c.data);
	UCodepoint upper = Core::Unicode::GetCpFromUtf8(res.data);
	ASSERT_EQ(upper, Core::Unicode::UnicodeCaseUpperTable[387]);
}

TEST(UnicodeUtilsTest, ToLower)
{
	ASSERT_EQ(Core::Unicode::ToLower('A'), 'a');
	ASSERT_EQ(Core::Unicode::ToLower(' '), ' ');

	Utf8Char c = Core::Unicode::GetUtf8FromCp(' ');
	Utf8Char res = Core::Unicode::ToLower(c.data);
	ASSERT_EQ(res.data[0], ' ');

	c = Core::Unicode::GetUtf8FromCp('A');
	res = Core::Unicode::ToLower(c.data);
	ASSERT_EQ(res.data[0], 'a');

	c = Core::Unicode::GetUtf8FromCp(Core::Unicode::ToLower(Core::Unicode::UnicodeCaseLowerTable[387]));
	res = Core::Unicode::ToLower(c.data);
	UCodepoint lower = Core::Unicode::GetCpFromUtf8(res.data);
	ASSERT_EQ(lower, Core::Unicode::UnicodeCaseUpperTable[387]);
}

TEST(UnicodeUtilsTest, MatchChar)
{
	
}

TEST(UnicodeUtilsTest, CodepointInUtf8Size)
{
	UCodepoint codepoint = 'A';
	ASSERT_EQ(Core::Unicode::GetCodepointSizeInUtf8(codepoint), 1);
	codepoint = 0x2F8;
	ASSERT_EQ(Core::Unicode::GetCodepointSizeInUtf8(codepoint), 2);
	codepoint = 0xA32;
	ASSERT_EQ(Core::Unicode::GetCodepointSizeInUtf8(codepoint), 3);
	codepoint = 0x10345;
	ASSERT_EQ(Core::Unicode::GetCodepointSizeInUtf8(codepoint), 4);
}

TEST(UnicodeUtilsTest, Utf8Size)
{
	u8 firstByte = 'A';
	ASSERT_EQ(Core::Unicode::GetUtf8Size(firstByte), 1);
	firstByte = 0xC1;
	ASSERT_EQ(Core::Unicode::GetUtf8Size(firstByte), 2);
	firstByte = 0xE7;
	ASSERT_EQ(Core::Unicode::GetUtf8Size(firstByte), 3);
	firstByte = 0xF7;
	ASSERT_EQ(Core::Unicode::GetUtf8Size(firstByte), 4);
}

TEST(UnicodeUtilsTest, CodepointToUtf8)
{
	Utf8Char utf8 = Core::Unicode::GetUtf8FromCp('A');
	ASSERT_EQ(utf8.data[0], 'A');

	utf8 = Core::Unicode::GetUtf8FromCp(0x434);
	ASSERT_EQ(utf8.data[0], 0xD0);
	ASSERT_EQ(utf8.data[1], 0xB4);

	utf8 = Core::Unicode::GetUtf8FromCp(0x16C8);
	ASSERT_EQ(utf8.data[0], 0xE1);
	ASSERT_EQ(utf8.data[1], 0x9B);
	ASSERT_EQ(utf8.data[2], 0x88);

	utf8 = Core::Unicode::GetUtf8FromCp(0x10912);
	ASSERT_EQ(utf8.data[0], 0xF0);
	ASSERT_EQ(utf8.data[1], 0x90);
	ASSERT_EQ(utf8.data[2], 0xA4);
	ASSERT_EQ(utf8.data[3], 0x92);
}

TEST(UnicodeUtilsTest, Utf8ToCodepoint)
{
	u8 utf8[4] = { 'A', 0, 0, 0 };
	
	ASSERT_EQ(Core::Unicode::GetCpFromUtf8(utf8), 'A');

	utf8[0] = 0xD0;
	utf8[1] = 0xB4;
	ASSERT_EQ(Core::Unicode::GetCpFromUtf8(utf8), 0x434);

	utf8[0] = 0xE1;
	utf8[1] = 0x9B;
	utf8[2] = 0x88;
	ASSERT_EQ(Core::Unicode::GetCpFromUtf8(utf8), 0x16C8);

	utf8[0] = 0xF0;
	utf8[1] = 0x90;
	utf8[2] = 0xA4;
	utf8[3] = 0x92;
	ASSERT_EQ(Core::Unicode::GetCpFromUtf8(utf8), 0x10912);
}