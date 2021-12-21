#pragma once
#include "core/MinInclude.h"
#include "core/utils/Flags.h"

namespace Core::Unicode
{
	struct Utf8Char
	{
		u8 data[4] = {};
		u32 size   = 0;
	};

	struct Utf16Char
	{
		u16 data[2] = {};
		u32 size    = 0;
	};
}

namespace Core
{
	/**
	 * Options of String::Split
	 */
	enum class StringSplitOption
	{
		None = 0,      ///< No options
		RemoveEmpty = BIT(0), ///< Omit empty substrings from the result
		TrimEntries = BIT(1), ///< Trim whitespace from each substring
		All = 0x3,    ///< All options (will also remove substrings that are only whitespace)
	};
	DEFINE_FLAGS(StringSplitOption);
}

DEFINE_ENUM_FLAG_OPS(StringSplitOption);