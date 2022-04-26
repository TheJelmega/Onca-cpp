#pragma once
#include "core/MinInclude.h"
#include "StringConstants.generated.h"

namespace Onca::Unicode
{
    // 0xxx xxxx == 1
    // 110x xxxx == 2
    // 1110 xxxx == 3
    // 1111 0xxx == 4
    // 10xx xxxx == invalid
    constexpr u8 Utf8SizeTable[16] =
    {
		1, 1, 1, 1, // 0000 - 0011
		1, 1, 1, 1, // 0100 - 0111
		0, 0, 0, 0, // 1000 - 1011
		2, 2, 3, 4  // 1100 - 1111
	};

	// see: https://en.wikipedia.org/wiki/Whitespace_character
	constexpr usize UnicodeWhitespaceTableSize = 25;
    constexpr UCodepoint UnicodeWhitespaceTable[UnicodeWhitespaceTableSize] =
    {
		0x0009, // Horizontal tab '\t'
		0x000A, // Line feed/newline '\n'
		0x000B, // Vertical tab '\v'
		0x000C, // Form feed '\f'
		0x000D, // Carriage return '\r'
		0x0020, // Space ' '
		0x0085, // Next line  
		0x00A0, // No-break space
		0x1680, // Ogham space mark
		0x2000, // EN quad
		0x2001, // EM quad
		0x2002, // EN space
		0x2003, // EM space
		0x2004, // Three per em space
		0x2005, // Four per em space
		0x2006, // Six per em space
		0x2007, // Figure space
		0x2008, // Punctuation space
		0x2009, // Thin space
		0x200A, // Hair space
		0x2028, // Line separator
		0x2029, // Paragraph separator
		0x202F, // Narrow no-break space
		0x205F, // Medium Mathematical space
		0x3000, // Ideographic space
    };

	constexpr Utf8Char Utf8WhitespaceTable[UnicodeWhitespaceTableSize] =
	{
		{ { 0x09, 0x00, 0x00, 0x00 }, 1 }, // Horizontal tab '\t'
		{ { 0x0A, 0x00, 0x00, 0x00 }, 1 }, // Line feed/newline '\n
		{ { 0x0B, 0x00, 0x00, 0x00 }, 1 }, // Vertical tab '\v'
		{ { 0x0C, 0x00, 0x00, 0x00 }, 1 }, // Form feed '\f'
		{ { 0x0D, 0x00, 0x00, 0x00 }, 1 }, // Carriage return '\r'
		{ { 0x20, 0x00, 0x00, 0x00 }, 1 }, // Space ' '
		{ { 0xC2, 0x85, 0x00, 0x00 }, 1 }, // Next line  
		{ { 0xC2, 0xA0, 0x00, 0x00 }, 1 }, // No-break space
		{ { 0xE1, 0x9A, 0x80, 0x00 }, 1 }, // Ogham space mark
		{ { 0xE2, 0x80, 0x80, 0x00 }, 1 }, // EN quad
		{ { 0xE2, 0x80, 0x81, 0x00 }, 1 }, // EM quad
		{ { 0xE2, 0x80, 0x82, 0x00 }, 1 }, // EN space
		{ { 0xE2, 0x80, 0x83, 0x00 }, 1 }, // EM space
		{ { 0xE2, 0x80, 0x84, 0x00 }, 1 }, // Three per em space
		{ { 0xE2, 0x80, 0x85, 0x00 }, 1 }, // Four per em space
		{ { 0xE2, 0x80, 0x86, 0x00 }, 1 }, // Six per em space
		{ { 0xE2, 0x80, 0x87, 0x00 }, 1 }, // Figure space
		{ { 0xE2, 0x80, 0x88, 0x00 }, 1 }, // Punctuation space
		{ { 0xE2, 0x80, 0x89, 0x00 }, 1 }, // Thin space
		{ { 0xE2, 0x80, 0x8A, 0x00 }, 1 }, // Hair space
		{ { 0xE2, 0x80, 0xA8, 0x00 }, 1 }, // Line separator
		{ { 0xE2, 0x80, 0xA9, 0x00 }, 1 }, // Paragraph separator
		{ { 0xE2, 0x80, 0xAF, 0x00 }, 1 }, // Narrow no-break space
		{ { 0xE2, 0x81, 0x9F, 0x00 }, 1 }, // Medium Mathematical space
		{ { 0xE3, 0x80, 0x80, 0x00 }, 1 }, // Ideographic space
	};
}