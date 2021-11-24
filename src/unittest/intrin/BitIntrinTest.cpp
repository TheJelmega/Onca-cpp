#include "gtest/gtest.h"
#include "core/Core.h"

namespace Intrin = Core::Intrin;

TEST(Intr, BitScan1)
{
	// u8
	ASSERT_EQ(Intrin::BitScan(u8(0)), u8(-1));
	ASSERT_EQ(Intrin::BitScan(u8(0x80)), 7);
	ASSERT_EQ(Intrin::BitScan(u8(0x40)), 6);
	ASSERT_EQ(Intrin::BitScan(u8(0x02)), 1);

	// u16
	ASSERT_EQ(Intrin::BitScan(u16(0)), u8(-1));
	ASSERT_EQ(Intrin::BitScan(u16(0x8000)), 15);
	ASSERT_EQ(Intrin::BitScan(u16(0x4000)), 14);
	ASSERT_EQ(Intrin::BitScan(u16(0x0020)), 5);

	// u32
	ASSERT_EQ(Intrin::BitScan(u32(0)), u8(-1));
	ASSERT_EQ(Intrin::BitScan(u32(0x8000'0000)), 31);
	ASSERT_EQ(Intrin::BitScan(u32(0x4000'0000)), 30);
	ASSERT_EQ(Intrin::BitScan(u32(0x0000'0200)), 9);

	// u64
	ASSERT_EQ(Intrin::BitScan(u64(0)), u8(-1));
	ASSERT_EQ(Intrin::BitScan(u64(0x8000'0000'0000'0000)), 63);
	ASSERT_EQ(Intrin::BitScan(u64(0x4000'0000'0000'0000)), 62);
	ASSERT_EQ(Intrin::BitScan(u64(0x0000'0000'0020'0000)), 21);
}

TEST(Intr, BitScanR1)
{
	// u8
	ASSERT_EQ(Intrin::BitScanR(u8(0)), u8(-1));
	ASSERT_EQ(Intrin::BitScanR(u8(0x80)), 7);
	ASSERT_EQ(Intrin::BitScanR(u8(0x40)), 6);
	ASSERT_EQ(Intrin::BitScanR(u8(0x02)), 1);

	// u16
	ASSERT_EQ(Intrin::BitScanR(u16(0)), u8(-1));
	ASSERT_EQ(Intrin::BitScanR(u16(0x8000)), 15);
	ASSERT_EQ(Intrin::BitScanR(u16(0x4000)), 14);
	ASSERT_EQ(Intrin::BitScanR(u16(0x0020)), 5);

	// u32
	ASSERT_EQ(Intrin::BitScanR(u32(0)), u8(-1));
	ASSERT_EQ(Intrin::BitScanR(u32(0x8000'0000)), 31);
	ASSERT_EQ(Intrin::BitScanR(u32(0x4000'0000)), 30);
	ASSERT_EQ(Intrin::BitScanR(u32(0x0000'0200)), 9);

	// u64
	ASSERT_EQ(Intrin::BitScanR(u64(0)), u8(-1));
	ASSERT_EQ(Intrin::BitScanR(u64(0x8000'0000'0000'0000)), 63);
	ASSERT_EQ(Intrin::BitScanR(u64(0x4000'0000'0000'0000)), 62);
	ASSERT_EQ(Intrin::BitScanR(u64(0x0000'0000'0020'0000)), 21);
}

TEST(Intr, PopCnt1)
{
	// u8
	ASSERT_EQ(Intrin::PopCnt(u8(0)), u8(-1));
	ASSERT_EQ(Intrin::PopCnt(u8(0x80)), 7);
	ASSERT_EQ(Intrin::PopCnt(u8(0x40)), 6);
	ASSERT_EQ(Intrin::PopCnt(u8(0x02)), 1);

	// u16
	ASSERT_EQ(Intrin::PopCnt(u16(0)), u8(-1));
	ASSERT_EQ(Intrin::PopCnt(u16(0x8000)), 15);
	ASSERT_EQ(Intrin::PopCnt(u16(0x4000)), 14);
	ASSERT_EQ(Intrin::PopCnt(u16(0x0020)), 5);

	// u32
	ASSERT_EQ(Intrin::PopCnt(u32(0)), u8(-1));
	ASSERT_EQ(Intrin::PopCnt(u32(0x8000'0000)), 31);
	ASSERT_EQ(Intrin::PopCnt(u32(0x4000'0000)), 30);
	ASSERT_EQ(Intrin::PopCnt(u32(0x0000'0200)), 9);

	// u64
	ASSERT_EQ(Intrin::PopCnt(u64(0)), u8(-1));
	ASSERT_EQ(Intrin::PopCnt(u64(0x8000'0000'0000'0000)), 63);
	ASSERT_EQ(Intrin::PopCnt(u64(0x4000'0000'0000'0000)), 62);
	ASSERT_EQ(Intrin::PopCnt(u64(0x0000'0000'0020'0000)), 21);
}