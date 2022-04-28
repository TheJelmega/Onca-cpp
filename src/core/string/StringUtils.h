#pragma once
#include "core/MinInclude.h"
#include "StringUtilTypes.h"

namespace Onca
{
	template<typename T>
	concept ConvertableToUnicode =
		SameAs<T, char>     ||
		SameAs<T, char8_t>  ||
		SameAs<T, char16_t> ||
		SameAs<T, char32_t> ||
		SameAs<T, UCodepoint>;

	template<typename T>
	concept DereferencableToUnicode =
		requires(T t) { ConvertableToUnicode<decltype(*t)>; };

	/**
	 * Get the length of a c-string
	 * \tparam C Character type
	 * \param str C-string
	 * \return Length of the c-string
	 */
	template<CharacterType C>
	constexpr auto StrLen(const C* str) noexcept -> usize;

	namespace Unicode
	{
		/**
		 * Check if 2 utf8 characters match
		 * \param pCh0 Pointer to the first byte of the first character
		 * \param pCh1 Pointer to the first byte of the second character
		 * \return Whether the characters matches
		 */
		constexpr auto MatchChar(const u8* pCh0, const u8* pCh1) noexcept -> bool;
		/**
		 * Check if 2 utf8 characters match
		 * \param pCh0 Pointer to the first byte of the first character
		 * \param pCh1 Pointer to the first byte of the second character
		 * \param size Size of the character
		 * \return Whether the characters matches
		 */
		constexpr auto MatchChar(const u8* pCh0, const u8* pCh1, usize size) noexcept -> bool;

		/**
		 * Check if a codepoint is whitespace
		 * \param codepoint Codepoint
		 * \return Whether the codepoint is whitespace
		 */
		constexpr auto IsWhitespace(UCodepoint codepoint) noexcept -> bool;
		/**
		 * Check if a codepoint is whitespace
		 * \param pCh Pointer to first byte of utf8 character
		 * \return Whether the codepoint is whitespace
		 */
		constexpr auto IsWhitespace(const u8* pCh) noexcept -> bool;

		/**
		 * Check if a codepoint is a digit
		 * \param codepoint Codepoint
		 * \return Whether the codepoint is a digit
		 */
		constexpr auto IsDigit(const UCodepoint codepoint) noexcept -> bool;

		/**
		 * Convert a codepoint to its uppercase version
		 * \param codepoint Codepoint
		 * \return Uppercase version of the codepoint
		 */
		constexpr auto ToUpper(UCodepoint codepoint) noexcept -> UCodepoint;
		/**
		 * Convert a codepoint to its uppercase version
		 * \param pCh Pointer to the first byte of the character
		 * \return Uppercase version of the codepoint as a Utf8Char
		 */
		constexpr auto ToUpper(const u8* pCh) noexcept -> Utf8Char;
		/**
		 * Convert a codepoint to its lowercase version
		 * \param codepoint Codepoint
		 * \return Lowercase version of the codepoint
		 */
		constexpr auto ToLower(UCodepoint codepoint) noexcept -> UCodepoint;
		/**
		 * Convert a codepoint to its lowercase version
		 * \param pCh Pointer to the first byte of the character
		 * \return Lowercase version of the codepoint as a Utf8Char
		 */
		constexpr auto ToLower(const u8* pCh) noexcept -> Utf8Char;

		/**
		 * Get the length of a c-string in characters and bytes
		 * \param str c-string
		 * \return Pair with the length of a c-string in characters and bytes
		 */
		template<CharacterType C>
		constexpr auto GetStrLen(const C* str) noexcept -> Pair<usize, usize>;
		/**
		 * Get the length of an utf16 c-string in characters and bytes
		 * \param str c-string
		 * \return Pair with the length of the utf16 c-string in characters and bytes
		 */
		constexpr auto GetUtf16StrLen(const char16_t* str) noexcept -> Pair<usize, usize>;

		/**
		 * Get the size of an utf8 character
		 * \param firstByte First byte of the utf8 character
		 * \return Size of an utf8 character
		 */
		constexpr auto GetUtf8Size(u8 firstByte) noexcept -> u8;
		/**
		 * Get the size of an utf8 character
		 * \param firstWord First word of the utf16 character
		 * \return Size of an utf8 character
		 */
		constexpr auto GetUtf16Size(u16 firstWord) noexcept -> u8;

		/**
		 * Get the size of the codepoint in its utf8 representation
		 * \param codepoint Codepoint
		 * \return Size of the codepoint in its utf8 representation
		 */
		constexpr auto GetCodepointSizeInUtf8(UCodepoint codepoint) noexcept -> u8;
		/**
		 * Get the size of the codepoint in its utf8 representation
		 * \param codepoint Codepoint
		 * \return Size of the codepoint in its utf8 representation
		 */
		constexpr auto GetCodepointSizeInUtf16(UCodepoint codepoint) noexcept -> u8;

		/**
		 * Get the utf8 representation of a codepoint
		 * \param codepoint Codepoint
		 * \return Utf8 representation of the codepoint
		 */
		constexpr auto GetUtf8FromCp(UCodepoint codepoint) noexcept -> Utf8Char;
		/**
		 * Convert a utf16 character to a utf8 character
		 * \param pCh Pointer to first utf16 character
		 * \return Utf8 representation of the character
		 */
		constexpr auto GetUtf8FromUtf16(const char16_t* pCh) noexcept -> Utf8Char;

		/**
		 * Get the utf16 representation of a codepoint
		 * \param codepoint Codepoint
		 * \return Utf16 representation of the codepoint
		 */
		constexpr auto GetUtf16FromCp(UCodepoint codepoint) noexcept -> Utf16Char;
		/**
		 * Convert a utf9 character to a utf16 character
		 * \param pCh Pointer to first utf8 character
		 * \return Utf16 representation of the character
		 */
		constexpr auto GetUtf16FromUtf8(const u8* pCh) noexcept -> Utf16Char;

		/**
		 * Get a codepoint from it'ss unicode representation
		 * \param pCh Pointer to first byte of utf8 character
		 * \return Unicode codepoint
		 */
		constexpr auto GetCpFromUtf8(const u8* pCh) noexcept -> UCodepoint;
		/**
		 * Get a codepoint from it'ss unicode representation
		 * \param pCh Pointer to first byte of utf8 character
		 * \return Unicode codepoint
		 */
		constexpr auto GetCpFromUtf16(const u16* pCh) noexcept -> UCodepoint;

		/**
		 * Get the utf8 representation of a character
		 * \tparam C Character type
		 * \param pCh Pointer to the first part of the character to convert
		 * \return Pair with the utf8 representation of the character and the amount of elements to skip
		 */
		template<ConvertableToUnicode C>
		constexpr auto ToUtf8(const C* pCh) noexcept -> Pair<Utf8Char, usize>;
	}
}

#include "StringUtils.inl"