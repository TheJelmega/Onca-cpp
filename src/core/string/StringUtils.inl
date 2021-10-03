#pragma once
#if __RESHARPER__
#include "StringUtils.h"
#endif

#include "StringConstants.h"

namespace Core
{
	template<CharacterType C>
	constexpr auto StrLen(const C* str) noexcept -> usize
	{
		auto [len, _] = Unicode::GetStrLen(str);
		return len;
	}

	
}

namespace Core::Unicode
{
	constexpr auto MatchChar(const u8* pCh0, const u8* pCh1) noexcept -> bool
	{
		usize size = GetUtf8Size(pCh0[0]);
		return MatchChar(pCh0, pCh1, size);
	}

	constexpr auto MatchChar(const u8* pCh0, const u8* pCh1, usize size) noexcept -> bool
	{
		return            pCh0[0] == pCh1[0]  &&
			(size <= 1 || pCh0[1] == pCh1[1]) &&
			(size <= 2 || pCh0[2] == pCh1[2]) &&
			(size <= 3 || pCh0[3] == pCh1[3]);
	}

	constexpr auto IsWhitespace(UCodepoint codepoint) noexcept -> bool
	{
		for (usize i = 0; i < UnicodeWhitespaceTableSize; ++i)
		{
			if (codepoint == UnicodeWhitespaceTable[i])
				return true;
		}
		return false;
	}

	constexpr auto IsWhitespace(const u8* pCh) noexcept -> bool
	{
		for (usize i = 0; i < UnicodeWhitespaceTableSize; ++i)
		{
			const Utf8Char& whitespace = Utf8WhitespaceTable[i];
			if (MatchChar(pCh, whitespace.data, whitespace.size))
				return true;
		}
		return false;
	}
	
	constexpr auto ToUpper(UCodepoint codepoint) noexcept -> UCodepoint
	{
		// ASCII shortcut
		if (codepoint <= 0x7F)
			return codepoint >= 'a' && codepoint <= 'z' ? codepoint & ~UCodepoint(0x20) : codepoint;

		for (usize i = 'z' - 'a'; i < UnicodeCaseTableSize; ++i)
		{
			if (codepoint == UnicodeCaseLowerTable[i])
				return UnicodeCaseUpperTable[i];
		}
		return codepoint;
	}

	constexpr auto ToUpper(const u8* pCh) noexcept -> Utf8Char
	{
		// ASCII shortcut
		if (pCh[0] <= 0x7F)
		{
			Utf8Char c;
			c.size = 1;
			c.data[0] = pCh[0] >= 'a' && pCh[0] <= 'z' ? pCh[0] & ~u8(0x20) : pCh[0];
			return c;
		}

		usize curSize = GetUtf8Size(pCh[0]);
		for (usize i = 'z' - 'a'; i < UnicodeCaseTableSize; ++i)
		{
			const Utf8Char lower = Utf8CaseLowerTable[i];
			if (MatchChar(pCh, lower.data, Math::Min(curSize, usize(lower.size))))
				return Utf8CaseUpperTable[i];
		}

		Utf8Char c;
		c.size = u32(curSize);
		MemCpy(c.data, pCh, curSize);
		return c;
	}

	constexpr auto ToLower(UCodepoint codepoint) noexcept -> UCodepoint
	{
		// ASCII shortcut
		if (codepoint <= 0x7F)
			return codepoint >= 'A' && codepoint <= 'Z' ? codepoint | UCodepoint(0x20) : codepoint;

		for (usize i = 'Z' - 'A'; i < UnicodeCaseTableSize; ++i)
		{
			if (codepoint == UnicodeCaseUpperTable[i])
				return UnicodeCaseLowerTable[i];
		}
		return codepoint;
	}

	constexpr auto ToLower(const u8* pCh) noexcept -> Utf8Char
	{
		// ASCII shortcut
		if (pCh[0] <= 0x7F)
		{
			Utf8Char c;
			c.size = 1;
			c.data[0] = pCh[0] >= 'A' && pCh[0] <= 'Z' ? pCh[0] | u8(0x20) : pCh[0];
			return c;
		}

		usize curSize = GetUtf8Size(pCh[0]);
		for (usize i = 'z' - 'a'; i < UnicodeCaseTableSize; ++i)
		{
			const Utf8Char upper = Utf8CaseUpperTable[i];
			if (MatchChar(pCh, upper.data, Math::Min(curSize, usize(upper.size))))
				return Utf8CaseLowerTable[i];
		}

		Utf8Char c;
		c.size = u32(curSize);
		MemCpy(c.data, pCh, curSize);
		return c;
	}

	template <CharacterType C>
	constexpr auto GetStrLen(const C* str) noexcept -> Pair<usize, usize>
	{
		if constexpr (SameAs<C, char>)
		{
			IF_CONSTEVAL
			{
				const char* s = str;
				for (; s; ++s)
					;
				usize len = usize(s - str);
				return { len, len };
			}
			else
			{
				usize len = strlen(str);
				return { len, len };
			}
		}
		else if constexpr (SameAs<C, char16_t>)
		{
			usize len = 0;
			const char16_t* s = str;
			for (; s; ++s)
				len += GetUtf16Size(*s);
			return { usize(s - str), len };
		}
		else if constexpr (SameAs<C, char8_t>)
		{
			usize len = 0;
			const char16_t* s = str;
			for (; s; ++s)
				len += GetUtf8Size(*s);
			return { usize(s - str), len };
		}
		else // SameAs<C, char32_t>
		{
			const C* s = str;
			for (; s; ++s)
				;
			usize len = usize(s - str);
			return { len, len * sizeof(char32_t) };
		}
	}

	constexpr auto GetUtf16StrLen(const char16_t* str) noexcept -> Pair<usize, usize>
	{
		usize byteLen = 0;
		usize len = 0;

		while (str)
		{
			usize size = 0;

			str += size;
			byteLen += size;
			++len;
		}

		return { byteLen, len };
	}

	constexpr auto GetUtf8Size(u8 firstByte) noexcept -> u8
	{
		return Utf8SizeTable[firstByte >> 4];
	}

	constexpr auto GetUtf16Size(u16 firstWord) noexcept -> u8
	{
		// utf16 character is 2 bytes when the first word is a high surrogate
		return 1 + ((firstWord & 0xB800) == 0xB800);
	}

	constexpr auto GetCodepointSizeInUtf8(UCodepoint codepoint) noexcept -> u8
	{
		return 1 + 
			(codepoint > 0x007F) + 
			(codepoint > 0x07FF) + 
			(codepoint > 0xFFFF);
	}

	constexpr auto GetCodepointSizeInUtf16(UCodepoint codepoint) noexcept -> u8
	{
		return 1 + (codepoint > 0xFFFF);
	}

	constexpr auto GetUtf8FromCp(UCodepoint codepoint) noexcept -> Utf8Char
	{
		constexpr u8 prefixes[4]   = { 0b0000'0000, 0b1100'0000, 0b1110'0000, 0b1111'0000 };
		constexpr u8 masks   [4]   = { 0b0111'1111, 0b0001'1111, 0b0000'1111, 0b0000'0111 };
		constexpr u8 extension     = 0b1000'0000;
		constexpr u8 extensionMask = 0b0011'1111;

		const u8 size   = GetCodepointSizeInUtf8(codepoint);
		const u8 prefix = prefixes[size - 1];
		const u8 mask   = masks   [size - 1];

		Utf8Char c;
		c.size = size;
		c.data[3] = extension | (codepoint & extensionMask);
		codepoint >>= (size == 4) * 6;
		c.data[2] = extension | (codepoint & extensionMask);
		codepoint >>= (size >= 3) * 6;
		c.data[1] = extension | (codepoint & extensionMask);
		codepoint >>= (size >= 2) * 6;
		c.data[0] = prefix    | (codepoint & mask);
		return c;
	}

	constexpr auto GetUtf8FromUtf16(const char16_t* pCh) noexcept -> Utf8Char
	{
		return GetUtf8FromCp(GetCpFromUtf16(reinterpret_cast<const u16*>(pCh)));
	}

	constexpr auto GetUtf16FromCp(UCodepoint codepoint) noexcept -> Utf16Char
	{
		Utf16Char c;
		c.size = GetCodepointSizeInUtf16(codepoint);
		if (c.size == 1)
		{
			c.data[0] = u16(codepoint);
		}
		else
		{
			c.data[0] = 0xD800 | (codepoint >> 10);
			c.data[1] = 0xDC00 | (codepoint & 0x3FF);
		}
		return c;
	}

	constexpr auto GetUtf16FromUtf8(const u8* pCh) noexcept -> Utf16Char
	{
		return GetUtf16FromCp(GetCpFromUtf8(pCh));
	}

	constexpr auto GetCpFromUtf8(const u8* pCh) noexcept -> UCodepoint
	{
		const usize len = GetUtf8Size(*pCh);

		if (len == 1)
			return *pCh;

		if (len == 2)
			return ((pCh[0] & 0b0001'1111) << 6) | 
			        (pCh[1] & 0b0011'1111);

		if (len == 3)
			return ((pCh[0] & 0b0000'1111) << 12) |
			       ((pCh[1] & 0b0011'1111) << 6 ) |
			        (pCh[2] & 0b0011'1111);

		// len == 4
		return ((pCh[0] & 0b0000'0111) << 18) |
		       ((pCh[1] & 0b0011'1111) << 12) |
		       ((pCh[2] & 0b0011'1111) << 6 ) |
		        (pCh[3] & 0b0011'1111);
	}

	constexpr auto GetCpFromUtf16(const u16* pCh) noexcept -> UCodepoint
	{
		usize size = GetUtf16Size(*pCh);

		if (size == 1)
			return *pCh;
		return ((*pCh & 0x3FF) << 10) | (*(pCh + 1) & 0x3FF);
	}

	template<ConvertableToUnicode C>
	constexpr auto ToUtf8(const C* pCh) noexcept ->  Pair<Utf8Char, usize>
	{
		if constexpr (SameAs<C, char8_t>)
		{
			Utf8Char c;
			c.size = 1;
			c.data[0] = u8(*pCh);
			return { c, 1 };
		}
		// Treat char separately, since it may contain extended ASCII
		else if constexpr (SameAs<C, char>)
		{
			Utf8Char c;
			c.size = 1;
			c.data[0] = *pCh > 0x7F ? 0x7F : u8(*pCh);
			return { c, 1 };
		}
		else if constexpr (SameAs<C, char16_t>)
		{
			usize toSkip = GetUtf16Size(*pCh);
			Utf8Char c = GetUtf8FromUtf16(pCh);
			return { c, toSkip };
		}
		else // SameAs<C, UCodepoint> || SameAs<C, char32_t>
		{
			return { GetUtf8FromCp(UCodepoint(*pCh)), 1 };
		}
	}
}

