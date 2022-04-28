#pragma once
#include "core/MinInclude.h"
#include "core/containers/InplaceDynArray.h"
#include "StringUtils.h"

namespace Onca
{
	// TODO: Unittest
	/**
	 * Utf8 string that lives on the stack
	 * \tparam Cap String capacity
	 */
	template<usize Cap>
	class ConstString
	{
	public:
		/**
		 * Constant used to tell ConstString manipulation function to use all utf8 character until the end
		 */
		constexpr static usize NPos = usize(-1);

		/**
		 * ConstString iterator
		 * \node The iterator does not allow modification of the elements
		 */
		class Iterator
		{
		public:
			constexpr Iterator() noexcept;
			constexpr Iterator(const Iterator& other) noexcept;
			constexpr Iterator(Iterator&& other) noexcept;

			constexpr auto operator=(const Iterator& other) noexcept -> Iterator&;
			constexpr auto operator=(Iterator&& other) noexcept -> Iterator&;

			constexpr auto operator*() const noexcept -> UCodepoint;

			constexpr auto operator++() noexcept -> Iterator&;
			constexpr auto operator++(int) noexcept -> Iterator;

			constexpr auto operator--() noexcept -> Iterator&;
			constexpr auto operator--(int) noexcept -> Iterator;

			constexpr auto operator+(usize count) const noexcept -> Iterator;
			constexpr auto operator-(usize count) const noexcept -> Iterator;
			constexpr auto operator-(const Iterator& other) const noexcept -> isize;

			constexpr auto operator+=(usize count) noexcept -> Iterator&;
			constexpr auto operator-=(usize count) noexcept -> Iterator&;

			constexpr auto operator==(const Iterator& other) const noexcept -> bool;
			constexpr auto operator!=(const Iterator& other) const noexcept -> bool;
			constexpr auto operator<=>(const Iterator& other) const noexcept -> std::partial_ordering;

			constexpr auto operator[](usize idx) const noexcept -> const UCodepoint&;

		private:
			constexpr Iterator(const ConstString<Cap>* pData, usize idx);

			const ConstString<Cap>* m_pCStr; ///< Const string
			usize                   m_idx;   ///< Index

			friend class ConstString;
		};
		using ConstIterator = const Iterator;

	public:
		/**
		 * Create a ConstString with an allocator
		 */
		explicit constexpr ConstString() noexcept;
		/**
		 * Create a ConstString with a capacity an allocator
		 * \param[in] capacity Initial capacity of the ConstString (in bytes)
		 */
		explicit constexpr ConstString(usize capacity) noexcept;
		/**
		 * Create a ConstString with a number of unicode codepoints
		 * \param[in] codepoint Unicode codepoint to add
		 * \param[in] count Number of codepoints to add
		 */
		explicit constexpr ConstString(UCodepoint codepoint, usize count) noexcept;
		/**
		 * Create a ConstString which is a subConstString of another ConstString
		 * \param[in] other ConstString to get subConstString from
		 * \param[in] pos Start position of the subConstString
		 */
		explicit constexpr ConstString(const ConstString& other, usize pos) noexcept;
		/**
		 * Create a ConstString which is a subConstString of another ConstString
		 * \param[in] other ConstString to get subConstString from
		 * \param[in] pos Start position of the subConstString
		 * \param[in] length Length of the subConstString
		 */
		explicit constexpr ConstString(const ConstString& other, usize pos, usize length) noexcept;
		/**
		 * Create a ConstString from a c-ConstString
		 * \tparam C Character type
		 * \param[in] str C-ConstString
		 */
		template<CharacterType C>
		constexpr ConstString(const C* str) noexcept;
		/**
		 * Create a ConstString from a c-ConstString
		 * \tparam C Character type
		 * \param[in] str C-ConstString
		 * \param[in] length Length of the c-ConstString
		 */
		template<CharacterType C>
		explicit constexpr ConstString(const C* str, usize length) noexcept;
		/**
		 * Create a ConstString from an initializer list
		 * \tparam T Unicode compatible type
		 * \param il Initializer list
		 */
		template<ConvertableToUnicode T>
		explicit constexpr ConstString(const InitializerList<T>& il) noexcept;
		/**
		 * Create a ConstString from a range of utf8 characters or codepoints
		 * \tparam It Forward iterator
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \note Iterators need to dereference to either utf8 bytes or UCodepoint
		 */
		template<ForwardIterator It>
			requires DereferencableToUnicode<It>
		explicit constexpr ConstString(const It& begin, const It& end) noexcept;
		/**
		 * Create a ConstString with the contents of another ConstString
		 * \param[in] other ConstString to copy
		 */
		constexpr ConstString(const ConstString& other) noexcept;
		/**
		 * Move another ConstString into a new ConstString
		 * \param[in] other ConstString to move from
		 */
		constexpr ConstString(ConstString&& other) noexcept;

		template<CharacterType C>
		constexpr auto operator=(const C* str) noexcept -> ConstString&;
		template<ConvertableToUnicode T>
		constexpr auto operator=(const InitializerList<T>& il) noexcept -> ConstString&;
		constexpr auto operator=(const ConstString& other) noexcept -> ConstString&;
		constexpr auto operator=(ConstString&& other) noexcept -> ConstString&;

		constexpr auto operator+(UCodepoint codepoint) const noexcept -> ConstString;
		template<CharacterType C>
		constexpr auto operator+(const C* str) const noexcept -> ConstString;
		constexpr auto operator+(const ConstString& other) const noexcept -> ConstString;

		constexpr auto operator+=(UCodepoint codepoint) noexcept -> ConstString&;
		template<CharacterType C>
		constexpr auto operator+=(const C* str) noexcept -> ConstString&;
		constexpr auto operator+=(const ConstString& other) noexcept -> ConstString&;

		constexpr auto operator<=>(const ConstString& other) const noexcept -> std::strong_ordering;
		constexpr auto operator==(const ConstString& other) const noexcept -> bool;
		constexpr auto operator!=(const ConstString& other) const noexcept -> bool;

		/**
		 * Assign a number of codepoints to a ConstString
		 * \param codepoint Codepoint to assign
		 * \param count Number of codepoints to assign
		 */
		constexpr void Assign(UCodepoint codepoint, usize count) noexcept;
		/**
		 * Assign a subConstString of another ConstString to this ConstString
		 * \param other ConstString to get subConstString from
		 * \param pos Start position of the subConstString
		 * \param length Length of the subConstString
		 */
		constexpr void Assign(const ConstString& other, usize pos, usize length = NPos) noexcept;
		/**
		 * Assign a c-ConstString to the ConstString
		 * \tparam C Character type
		 * \param str C-ConstString
		 */
		template<CharacterType C>
		constexpr void Assign(const C* str) noexcept;
		/**
		 * Assign a c-ConstString to the ConstString
		 * \tparam C Character type
		 * \param str C-ConstString
		 * \param length Length of the c-ConstString
		 */
		template<CharacterType C>
		constexpr void Assign(const C* str, usize length) noexcept;
		/**
		 * Assign an initializer list to the ConstString
		 * \tparam T Unicode compatible type
		 * \param il Initializer list
		 */
		template<ConvertableToUnicode T>
		constexpr void Assign(const InitializerList<T>& il) noexcept;
		/**
		 * Assign a range of utf8 character to the ConstString
		 * \tparam It Forward iterator
		 * \param begin Begin iterator
		 * \param end End iterator
		 */
		template<ForwardIterator It>
			requires DereferencableToUnicode<It>
		constexpr void Assign(const It& begin, const It& end) noexcept;

		/**
		 * Resize the ConstString and fill missing elements if needed
		 * \param[in] newSize New size of the ConstString
		 * \param[in] codepoint codepoint to fill missing characters with
		 */
		constexpr void Resize(usize newSize, UCodepoint codepoint) noexcept;

		/**
		 * Add a number of codepoints to a ConstString
		 * \param codepoint Codepoint to add
		 * \param count Number of codepoints to add
		 * \return Reference to the ConstString
		 */
		constexpr auto Add(UCodepoint codepoint, usize count = 1) noexcept -> ConstString&;
		/**
		 * Add a subConstString of another ConstString to this ConstString
		 * \param other ConstString to get subConstString from
		 * \param pos Start position of the subConstString
		 * \param length Length of the subConstString
		 * \return Reference to the ConstString
		 */
		constexpr auto Add(const ConstString& other, usize pos = 0, usize length = NPos) noexcept -> ConstString&;

		/**
		 * Pad the left side of the ConstString
		 * \param count Number of codepoints to pad with
		 * \param codepoint Codepoint to pad with
		 * \return Reference to the ConstString
		 */
		constexpr auto PadLeft(usize count, UCodepoint codepoint = ' ') noexcept -> ConstString&;
		/**
		 * Pad the right side of the ConstString
		 * \param count Number of codepoints to pad with
		 * \param codepoint Codepoint to pad with
		 * \return Reference to the ConstString
		 */
		constexpr auto PadRight(usize count, UCodepoint codepoint = ' ') noexcept -> ConstString&;

		/**
		 * Remove a range of utf8 characters from the ConstString
		 * \param pos Position of first utf8 character to erase
		 * \param count Number of character to remove
		 * \return Reference to the ConstString
		 */
		constexpr auto Erase(usize pos, usize count = NPos) noexcept -> ConstString&;

		/**
		 * Trim a codepoint from the left of the ConstString
		 * \param codepoint Codepoint to trim
		 * \return Reference to the ConstString
		 */
		constexpr auto TrimLeft(UCodepoint codepoint) noexcept -> ConstString&;
		/**
		 * Trim any whitespace from the left of the ConstString
		 * \return Reference to the ConstString
		 */
		constexpr auto TrimLeft() noexcept -> ConstString&;
		/**
		 * Trim a codepoint from the right of the ConstString
		 * \param codepoint Codepoint to trim
		 * \return Reference to the ConstString
		 */
		constexpr auto TrimRight(UCodepoint codepoint) noexcept -> ConstString&;
		/**
		 * Trim any whitespace from the right of the ConstString
		 * \return Reference to the ConstString
		 */
		constexpr auto TrimRight() noexcept -> ConstString&;
		/**
		 * Trim a codepoint from both sides of the ConstString
		 * \param codepoint Codepoint to trim
		 * \return Reference to the ConstString
		 */
		constexpr auto Trim(UCodepoint codepoint) noexcept -> ConstString&;
		/**
		 * Trim any whitespace from both sides of the ConstString
		 * \return Reference to the ConstString
		 */
		constexpr auto Trim() noexcept -> ConstString&;

		/**
		 * Insert a section of the ConstString with a subConstString of another ConstString
		 * \param pos Position to section to insert
		 * \param str ConstString to insert the section with
		 * \param strPos Position to the start of the subConstString to insert the section with
		 * \param strLength Length of the subConstString to insert the section with
		 * \return Reference to the ConstString
		 */
		constexpr auto Insert(usize pos, const ConstString& str, usize strPos = 0, usize strLength = NPos) noexcept -> ConstString&;
		/**
		 * Insert a section of the ConstString with a number of codepoints
		 * \param pos Position to section to insert
		 * \param codepoint Codepoint to insert section with
		 * \param count Number of character to insert the section with
		 * \return Reference to the ConstString
		 */
		constexpr auto Insert(usize pos, UCodepoint codepoint, usize count) noexcept -> ConstString&;

		/**
		 * Replace a section of the ConstString with a subConstString of another ConstString
		 * \param pos Position to section to replace
		 * \param length Length of the section to replace
		 * \param str ConstString to replace the section with
		 * \param strPos Position to the start of the subConstString to replace the section with
		 * \param strLength Length of the subConstString to replace the section with
		 * \return Reference to the ConstString
		 */
		constexpr auto Replace(usize pos, usize length, const ConstString& str, usize strPos = 0, usize strLength = NPos) noexcept -> ConstString&;
		/**
		 * Replace a section of the ConstString with a number of codepoints
		 * \param pos Position to section to replace
		 * \param length Length of the section to replace
		 * \param codepoint Codepoint to replace section with
		 * \param count Number of character to replace the section with
		 * \return Reference to the ConstString
		 */
		constexpr auto Replace(usize pos, usize length, UCodepoint codepoint, usize count = 1) noexcept -> ConstString&;
		/**
		 * Replace an utf8 character with another utf8 character
		 * \param toReplace Codepoint to replace
		 * \param replaceWith Codepoint to replace with
		 * \return Reference to the ConstString
		 */
		constexpr auto Replace(UCodepoint toReplace, UCodepoint replaceWith) noexcept -> ConstString&;
		/**
		 * Replace an utf8 character with a ConstString
		 * \param toReplace Codepoint to replace
		 * \param replaceWith ConstString to replace with
		 * \return Reference to the ConstString
		 */
		constexpr auto Replace(UCodepoint toReplace, const ConstString& replaceWith) noexcept -> ConstString&;
		/**
		 * Replace a ConstString with a utf8 character
		 * \param toReplace ConstString to replace
		 * \param replaceWith Codepoint to replace with
		 * \return Reference to the ConstString
		 */
		constexpr auto Replace(const ConstString& toReplace, UCodepoint replaceWith) noexcept -> ConstString&;
		/**
		 * Replace a ConstString with another ConstString
		 * \param toReplace ConstString to replace
		 * \param replaceWith ConstString to replace with
		 * \return Reference to the ConstString
		 */
		constexpr auto Replace(const ConstString& toReplace, const ConstString& replaceWith) noexcept -> ConstString&;

		/**
		 * Convert the ConstString to upper case
		 * \return Reference to the ConstString
		 */
		constexpr auto ToUpper() noexcept -> ConstString&;
		/**
		 * Get a copy of the ConstString that's upper case
		 * \return Reference to the ConstString
		 */
		constexpr auto AsUpper() const noexcept -> ConstString;
		/**
		 * Convert the ConstString to lower case
		 * \return Reference to the ConstString
		 */
		constexpr auto ToLower() noexcept -> ConstString&;
		/**
		 * Get a copy of the ConstString that's lower case
		 * \return Reference to the ConstString
		 */
		constexpr auto AsLower() const noexcept -> ConstString;

		/**
		 * Get a subConstString
		 * \param pos Position of the start of the subConstString
		 * \param length Length of the subConstString
		 * \return SubConstString
		 */
		constexpr auto SubConstString(usize pos, usize length = NPos) const noexcept -> ConstString;

		/**
		 * Split a ConstString into subConstStrings based on a delimiter
		 * \tparam Max Maximum number of subConstString (last subConstString will contain the rest of the ConstString)
		 * \param delimiter Delimiter
		 * \param options Splitting options (see SplitOption)
		 * \return Dynamic array with split parts
		 */
		template<usize Max>
		constexpr auto Split(UCodepoint delimiter, StringSplitOptions options = StringSplitOptions::None) noexcept -> InplaceDynArray<ConstString, Cap * sizeof(this)>;
		/**
		 * Split a ConstString into subConstStrings based on a delimiter
		 * \tparam Max Maximum number of subConstString (last subConstString will contain the rest of the ConstString)
		 * \param delimiter Delimiter
		 * \param options Splitting options (see SplitOption)
		 * \return Dynamic array with split parts
		 */
		template<usize Max>
		constexpr auto Split(const ConstString& delimiter, StringSplitOptions options = StringSplitOptions::None) const noexcept -> InplaceDynArray<ConstString, Cap * sizeof(this)>;

		/**
		 * Split a ConstString into subConstStrings based on whitespace
		 * \tparam Max Maximum number of subConstString (last subConstString will contain the rest of the ConstString)
		 * \param options Splitting options (see SplitOption)
		 * \return Dynamic array with split parts
		 */
		template<usize Max>
		constexpr auto SplitWhitespace(StringSplitOptions options = StringSplitOptions::None) const noexcept -> InplaceDynArray<ConstString, Cap * sizeof(this)>;

		/**
		 * Find a codepoint in the ConstString
		 * \param codepoint Codepoint to find
		 * \param pos Start position to find from
		 * \param count Number of utf8 characters to look at
		 * \return Position of first occurrence
		 */
		constexpr auto Find(UCodepoint codepoint, usize pos = 0, usize count = NPos) const noexcept -> usize;
		/**
		 * Find a ConstString in the ConstString
		 * \param str ConstString to find
		 * \param pos Start position to find from
		 * \param count Number of utf8 characters to look at
		 * \return Position of first occurrence
		 */
		constexpr auto Find(const ConstString& str, usize pos = 0, usize count = NPos) const noexcept -> usize;

		/**
		 * Reverse find a codepoint in the ConstString
		 * \param codepoint Codepoint to find
		 * \param pos Start position to find from
		 * \param count Number of utf8 characters to look at
		 * \return Position of last occurrence
		 */
		constexpr auto RFind(UCodepoint codepoint, usize pos = NPos, usize count = NPos) const noexcept -> usize;
		/**
		 * Reverse find a ConstString in the ConstString
		 * \param str ConstString to find
		 * \param pos Start position to find from
		 * \param count Number of utf8 characters to look at
		 * \return Position of last occurrence
		 */
		constexpr auto RFind(const ConstString& str, usize pos = NPos, usize count = NPos) const noexcept -> usize;

		/**
		 * Find the first occurrence of a codepoints that is in the given codepoints
		 * \param codepoints ConstString with codepoints
		 * \param pos Start position to find from
		 * \param count Number of utf8 characters to look at
		 * \return Position of the first occurrence
		 */
		constexpr auto FindFirstOf(const ConstString& codepoints, usize pos = 0, usize count = NPos) const noexcept -> usize;
		/**
		 * Find the first occurrence of a codepoints that is not in the given codepoints
		 * \param codepoints ConstString with codepoints
		 * \param pos Start position to find from
		 * \param count Number of utf8 characters to look at
		 * \return Position of the first occurrence
		 */
		constexpr auto FindFirstNotOf(const ConstString& codepoints, usize pos = 0, usize count = NPos) const noexcept -> usize;
		/**
		 * Reverse findthe first occurrence of a codepoints that is in the given codepoints
		 * \param codepoints ConstString with codepoints
		 * \param pos Start position to find from
		 * \param count Number of utf8 characters to look at
		 * \return Position of the first occurrence
		 */
		constexpr auto RFindFirstOf(const ConstString& codepoints, usize pos = NPos, usize count = NPos) const noexcept -> usize;
		/**
		 * Reverse findthe first occurrence of a codepoints that is not in the given codepoints
		 * \param codepoints ConstString with codepoints
		 * \param pos Start position to find from
		 * \param count Number of utf8 characters to look at
		 * \return Position of the first occurrence
		 */
		constexpr auto RFindFirstNotOf(const ConstString& codepoints, usize pos = NPos, usize count = NPos) const noexcept -> usize;

		/**
		 * Check if the ConstString contains a codepoint
		 * \param codepoint Codepoint to check for
		 * \return Whether the ConstString contains the codepoint
		 */
		constexpr auto Contains(UCodepoint codepoint) const noexcept -> bool;
		/**
		 * Check if the ConstString contains a ConstString
		 * \param str C-ConstString to check for
		 * \return Whether the ConstString contains the given ConstString
		 */
		constexpr auto Contains(const ConstString& str) const noexcept -> bool;

		/**
		 * Check if the ConstString starts with a codepoint
		 * \param codepoint Codepoint to check
		 * \return Whether the ConstString starts with the codepoint
		 */
		constexpr auto StartsWith(UCodepoint codepoint) const noexcept -> bool;
		/**
		 * Check if the ConstString starts with a c-ConstString
		 * \param str ConstString to check
		 * \return Whether the ConstString starts with the given ConstString
		 */
		constexpr auto StartsWith(const ConstString& str) const noexcept -> bool;

		/**
		 * Check if the ConstString ends with a codepoint
		 * \param codepoint Codepoint to check
		 * \return Whether the ConstString ends with the codepoint
		 */
		constexpr auto EndsWith(UCodepoint codepoint) const noexcept -> bool;
		/**
		 * Check if the ConstString ends with a c-ConstString
		 * \param str ConstString to check
		 * \return Whether the ConstString ends with the given ConstString
		 */
		constexpr auto EndsWith(const ConstString& str) const noexcept -> bool;

		/**
		 * Get the ConstString as an ascii representation
		 * \return Dynamic array with ascii representation
		 */
		constexpr auto ToAscii() const noexcept -> InplaceDynArray<char, Cap>;
		/**
		 * Get the ConstString as an utf8 representation
		 * \return Dynamic array with utf8 representation
		 */
		constexpr auto ToUtf8() const noexcept -> InplaceDynArray<char8_t, Cap>;
		/**
		 * Get the ConstString as an utf16 representation
		 * \return Dynamic array with utf16 representation
		 */
		constexpr auto ToUtf16() const noexcept -> InplaceDynArray<char16_t, Cap * sizeof(char16_t)>;
		/**
		 * Get the ConstString as an utf32 representation
		 * \return Dynamic array with utf32 representation
		 */
		constexpr auto ToUtf32() const noexcept -> InplaceDynArray<char32_t, Cap * sizeof(char32_t)>;
		/**
		 * Get the ConstString as an codepoint/utf32 representation
		 * \return Dynamic array with codepoint/utf32 representation
		 */
		constexpr auto ToCodepoints() const noexcept -> InplaceDynArray<UCodepoint, Cap * sizeof(UCodepoint)>;

		/**
		 * Compare 2 ConstStrings
		 * \param str ConstString to compare with
		 * \return 0 when ConstString are the same, 1 when the current ConstString is before the other ConstString, and -1 otherwise
		 */
		constexpr auto Compare(const ConstString& str) const noexcept -> i8;

		/**
		 * Check whether a ConstString only contains whitespace
		 * \return Whether the ConstString only contains whitespace
		 */
		constexpr auto IsWhitespace() const noexcept -> bool;

		/**
		 * Get the codepoint at an index
		 * \param[in] idx Index of codepoint
		 * \return Optional with value
		 * \note Will return an empty optional when index is out of bounds
		 */
		constexpr auto At(usize idx) const noexcept -> Optional<UCodepoint>;
		/**
		 * Get the codepoint at an index
		 * \param[in] idx Index of codepoint
		 * \return Codepoint at index
		 * \note Only use when certain the index is not out of bounds
		 */
		constexpr auto operator[](usize idx) const noexcept -> UCodepoint;

		/**
		 * Get the length of the ConstString
		 * \return Length of the ConstString
		 */
		constexpr auto Length() const noexcept -> usize;
		/**
		 * Check whether the ConstString is empty
		 * \return Whether the ConstString is empty
		 */
		constexpr auto IsEmpty() const noexcept -> bool;

		/**
		 * Get the capacity of the underlying Utf8 data
		 * \return Capacity of the underlying Utf8 data
		 */
		constexpr auto Capacity() const noexcept -> usize;
		/**
		 * Get the size of the underlying Utf8 data
		 * \return Size of the underlying Utf8 data
		 */
		constexpr auto DataSize() const noexcept -> usize;
		/**
		 * Get the underlying Utf8 data
		 * \return Underlying Utf8 data
		 */
		constexpr auto Data() noexcept -> u8*;
		/**
		 * Get the underlying Utf8 data
		 * \return Underlying Utf8 data
		 */
		constexpr auto Data() const noexcept -> const u8*;

		/**
		 * Get the first element in the ConstString
		 * \return First element in the ConstString
		 * \note Only use when the ConstString is not empty
		 */
		constexpr auto Front() const noexcept -> UCodepoint;
		/**
		 * Get the last element in the ConstString
		 * \return Last element in the ConstString
		 * \note Only use when the ConstString is not empty
		 */
		constexpr auto Back() const noexcept -> UCodepoint;

		/**
		 * Get an iterator to the first element
		 * \return Iterator to the first element
		 */
		constexpr auto Begin() noexcept -> Iterator;
		/**
		 * Get an iterator to the first element
		 * \return Iterator to the first element
		 */
		constexpr auto Begin() const noexcept -> ConstIterator;

		/**
		 * Get an iterator to the end of the elements
		 * \return Iterator to the end of the elements
		 */
		constexpr auto End() noexcept -> Iterator;
		/**
		 * Get an iterator to the end of the elements
		 * \return Iterator to the end of the elements
		 */
		constexpr auto End() const noexcept -> ConstIterator;

		/**
		 * Get a reverse Reverse iterator to the first element
		 * \return Reverse iterator to the first element
		 */
		constexpr auto RBegin() noexcept -> Iterator;
		/**
		 * Get a reverse Reverse iterator to the first element
		 * \return Reverse iterator to the first element
		 */
		constexpr auto RBegin() const noexcept -> ConstIterator;

		/**
		 * Get a reverse Reverse iterator to the end of the elements
		 * \return Reverse iterator to the end of the elements
		 */
		constexpr auto REnd() noexcept -> Iterator;
		/**
		 * Get a reverse Reverse iterator to the end of the elements
		 * \return Reverse iterator to the end of the elements
		 */
		constexpr auto REnd() const noexcept -> ConstIterator;

		// Overloads for 'for ( ... : ... )'
		constexpr auto begin() noexcept -> Iterator;
		constexpr auto begin() const noexcept -> ConstIterator;
		constexpr auto cbegin() const noexcept -> ConstIterator;
		constexpr auto end() noexcept -> Iterator;
		constexpr auto end() const noexcept -> ConstIterator;
		constexpr auto cend() const noexcept -> ConstIterator;


	private:

		/**
		 * Assign a ConstString to this ConstString
		 * \param other ConstString to assign
		 * \param idx Start index in bytes
		 * \param pos Start position in characters
		 * \param size Size in bytes
		 * \param length Size in characters
		 */
		constexpr void AssignSubConstString(const ConstString& other, usize idx, usize pos, usize size, usize length) noexcept;

		/**
		 * Replace a section of the ConstString with a number of codepoints
		 * \param idx Index to section to replace in bytes
		 * \param byteLength Length of the section to replace in bytes
		 * \param length Length of the section to replace in utf8 characters
		 * \param c Utf8 character to replace section with
		 * \param count Number of character to replace the section with
		 * \return Reference to the ConstString
		 */
		constexpr auto ReplaceInternal(usize idx, usize byteLength, usize length, Unicode::Utf8Char c, usize count) noexcept -> ConstString&;
		/**
		 * Replace a section of the ConstString with a number of codepoints
		 * \param idx Index to section to replace in bytes
		 * \param byteLength Length of the section to replace in bytes
		 * \param length Length of the section to replace in utf8 characters
		 * \param str ConstString to replace the section with
		 * \param strPos Position to the start of the subConstString to replace the section with
		 * \param strByteEnd Index of the subConstString to replace the section with in bytes
		 * \param strLength Length of the subConstString to replace the section with in character
		 * \return Reference to the ConstString
		 */
		constexpr auto ReplaceInternal(usize idx, usize byteLength, usize length, const ConstString& str, usize strPos, usize strByteEnd, usize strLength) noexcept -> ConstString&;

		/**
		 * Get the index of a codepoint
		 * \param codepoint Codepoint to find
		 * \param pos Start position to find from in characters
		 * \param idx Start position to find from in bytes
		 * \param count Number of utf8 characters to look at
		 * \return Pair with the index of a codepoint in characters and bytes
		 */
		constexpr auto FindInternal(UCodepoint codepoint, usize pos, usize idx, usize count) const noexcept -> Pair<usize, usize>;
		/**
		 * Get the index of a codepoint
		 * \param str ConstString to find
		 * \param pos Start position to find from in characters
		 * \param idx Start position to find from in bytes
		 * \param count Number of utf8 characters to look at
		 * \return Pair with the index of a codepoint in characters and bytes
		 */
		constexpr auto FindInternal(const ConstString& str, usize pos, usize idx, usize count) const noexcept -> Pair<usize, usize>;
		/**
		 * Get the index of a whitespace
		 * \param pos Start position to find from in characters
		 * \param idx Start position to find from in bytes
		 * \param count Number of utf8 characters to look at
		 * \return Pair with the index of a whitespace in characters and bytes
		 */
		constexpr auto FindWhitespaceInternal(usize pos, usize idx, usize count) const noexcept -> Pair<usize, usize>;

		/**
		 * Get the index of a character position
		 * \param pos Character position
		 * \return Index of the character position
		 */
		constexpr auto IndexAtCharPos(usize pos) const noexcept -> usize;
		/**
		 * Convert an offset in character to bytes, starting from a certain index
		 * \param startIdx Index to start counting from
		 * \param offset Offset of characters
		 * \return Offset in ConstString in bytes
		 */
		constexpr auto IndexForOffset(usize startIdx, usize offset) const noexcept -> usize;
		/**
		 * Get the previous index of a character
		 * \param pData Pointer to ConstString data
		 * \param idx Current index
		 * \return Previous index
		 */
		constexpr auto PrevIdx(const u8* pData, usize idx) const noexcept -> usize;
		/**
		 * Get the pointer to previous character
		 * \param pData Pointer to the current data in the ConstString
		 * \return Pointer to the previous character
		 */
		constexpr auto PrevCharPtr(const u8* pData) const noexcept -> const u8*;

		/**
		 * Make sure the internal ConstString is null-terminated
		 */
		constexpr void NullTerminate() noexcept;


		InplaceDynArray<u8, Cap> m_data;   ///< UTF8 data
		usize                    m_length; ///< ConstString length
	};

}

#include "ConstString.inl"