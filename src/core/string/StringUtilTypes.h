#pragma once
#include "core/MinInclude.h"

namespace Core::Unicode
{
	struct Utf8Char
	{
		u8 data[4];
		u32 size;
	};

	struct Utf16Char
	{
		u16 data[2];
		u32 size;
	};
}