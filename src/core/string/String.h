#pragma once
#include "core/MinInclude.h"
#include "core/containers/DynArray.h"
#include "core/utils/Flags.h"
#include "StringUtils.h"

namespace Onca
{
	class ByteBuffer;
	/**
	 * Utf8 string
	 */
	class CORE_API String
	{
	public:
		/**
		 * Constant used to tell string manipulation function to use all utf8 character until the end
		 */
		constexpr static usize NPos = usize(-1);

		/**
		 * String iterator
		 * \node The iterator does not allow modification of the elements
		 */
		class CORE_API Iterator
		{
		public:
			Iterator() noexcept;
			Iterator(const Iterator& other) noexcept;
			Iterator(Iterator&& other) noexcept;
			
			auto operator=(const Iterator& other) noexcept -> Iterator&;
			auto operator=(Iterator&& other) noexcept -> Iterator&;

			auto operator*() const noexcept -> UCodepoint;

			auto operator++() noexcept -> Iterator&;
			auto operator++(int) noexcept -> Iterator;

			auto operator--() noexcept -> Iterator&;
			auto operator--(int) noexcept -> Iterator;

			auto operator+(usize count) const noexcept -> Iterator;
			auto operator-(usize count) const noexcept -> Iterator;
			auto operator-(const Iterator& other) const noexcept -> isize;

			auto operator+=(usize count) noexcept -> Iterator&;
			auto operator-=(usize count) noexcept -> Iterator&;

			auto operator==(const Iterator& other) const noexcept -> bool;
			auto operator!=(const Iterator& other) const noexcept -> bool;
			auto operator<=>(const Iterator& other) const noexcept -> std::partial_ordering;
			
			auto operator[](usize idx) const noexcept -> UCodepoint;

		private:
			Iterator(const DynArray<u8>* pData, usize idx);

			const DynArray<u8>* m_pData; ///< String data
			usize               m_idx;   ///< Index

			friend class String;
		};
		using ConstIterator = Iterator;

	public:
		/**
		 * Create a String with an allocator
		 * \param[in] alloc Allocator the string should use
		 */
		explicit String(Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a String with a capacity an allocator
		 * \param[in] capacity Initial capacity of the string (in bytes)
		 * \param[in] alloc Allocator the string should use
		 */
		explicit String(usize capacity, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a String with a number of unicode codepoints
		 * \param[in] codepoint Unicode codepoint to add
		 * \param[in] count Number of codepoints to add
		 * \param[in] alloc Allocator the string should use
		 */
		explicit String(UCodepoint codepoint, usize count, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a string which is a substring of another string
		 * \param[in] other String to get substring from
		 * \param[in] pos Start position of the substring
		 * \param[in] alloc Allocator the string should use
		 */
		explicit String(const String& other, usize pos, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a string which is a substring of another string
		 * \param[in] other String to get substring from
		 * \param[in] pos Start position of the substring
		 * \param[in] length Length of the substring
		 */
		explicit String(const String& other, usize pos, usize length) noexcept;
		/**
		 * Create a string which is a substring of another string
		 * \param[in] other String to get substring from
		 * \param[in] pos Start position of the substring
		 * \param[in] length Length of the substring
		 * \param[in] alloc Allocator the string should use
		 */
		explicit String(const String& other, usize pos, usize length, Alloc::IAllocator& alloc) noexcept;
		/**
		 * Create a string from a c-string
		 * \tparam C Character type
		 * \param[in] str C-string
		 * \param[in] alloc Allocator the string should use
		 */
		template<CharacterType C>
		String(const C* str, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a string from a c-string
		 * \tparam C Character type
		 * \param[in] str C-string
		 * \param[in] length Length of the c-string
		 * \param[in] alloc Allocator the string should use
		 */
		template<CharacterType C>
		explicit String(const C* str, usize length, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a string from an initializer list
		 * \tparam T Unicode compatible type
		 * \param il Initializer list
		 * \param alloc Allocator the string should use
		 */
		template<ConvertableToUnicode T>
		explicit String(const InitializerList<T>& il, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a string from a range of utf8 characters or codepoints
		 * \tparam It Forward iterator
		 * \param[in] begin Begin iterator
		 * \param[in] end End iterator
		 * \param[in] alloc Allocator the string should use
		 * \note Iterators need to dereference to either utf8 bytes or UCodepoint
		 */
		template<ForwardIterator It>
			requires DereferencableToUnicode<It>
		explicit String(const It& begin, const It& end, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a String with a raw bytes
		 * \param[in] bytes Raw bytes
		 * \param[in] alloc Allocator the string should use
		 */
		explicit String(const ByteBuffer& bytes, Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a String with the contents of another String
		 * \param[in] other String to copy
		 */
		String(const String& other) noexcept;
		/**
		 * Create a String with the contents of another String, but with a different allocator
		 * \param[in] other String to copy
		 * \param[in] alloc Allocator the container should use
		 */
		explicit String(const String& other, Alloc::IAllocator& alloc) noexcept;
		/**
		 * Move another String into a new String
		 * \param[in] other String to move from
		 */
		String(String&& other) noexcept;
		/**
		 * Move another String into a new String, but with a different allocator
		 * \param[in] other String to move from
		 * \param[in] alloc Allocator the container should use
		 */
		explicit String(String&& other, Alloc::IAllocator& alloc) noexcept;

		template<CharacterType C>
		auto operator=(const C* str) noexcept -> String&;
		template<ConvertableToUnicode T>
		auto operator=(const InitializerList<T>& il) noexcept -> String&;
		auto operator=(const String& other) noexcept -> String&;
		auto operator=(String&& other) noexcept -> String&;

		auto operator+(UCodepoint codepoint) const noexcept -> String;
		template<CharacterType C>
		auto operator+(const C* str) const noexcept -> String;
		auto operator+(const String& other) const noexcept -> String;

		auto operator+=(UCodepoint codepoint) noexcept -> String&;
		template<CharacterType C>
		auto operator+=(const C* str) noexcept -> String&;
		auto operator+=(const String& other) noexcept -> String&;

		auto operator<=>(const String& other) const noexcept -> std::strong_ordering;
		auto operator==(const String& other) const noexcept -> bool;

		/**
		 * Assign a number of codepoints to a String
		 * \param codepoint Codepoint to assign
		 * \param count Number of codepoints to assign
		 */
		void Assign(UCodepoint codepoint, usize count) noexcept;
		/**
		 * Assign a substring of another String to this String
		 * \param other String to get substring from
		 * \param pos Start position of the substring
		 * \param length Length of the substring
		 */
		void Assign(const String& other, usize pos, usize length = NPos) noexcept;
		/**
		 * Assign a c-string to the String
		 * \tparam C Character type
		 * \param str C-string
		 */
		template<CharacterType C>
		void Assign(const C* str) noexcept;
		/**
		 * Assign a c-string to the String
		 * \tparam C Character type
		 * \param str C-string
		 * \param length Length of the c-string
		 */
		template<CharacterType C>
		void Assign(const C* str, usize length) noexcept;
		/**
		 * Assign an initializer list to the String
		 * \tparam T Unicode compatible type
		 * \param il Initializer list
		 */
		template<ConvertableToUnicode T>
		void Assign(const InitializerList<T>& il) noexcept;
		/**
		 * Assign a range of utf8 character to the string
		 * \tparam It Forward iterator
		 * \param begin Begin iterator
		 * \param end End iterator
		 */
		template<ForwardIterator It>
			requires DereferencableToUnicode<It>
		void Assign(const It& begin, const It& end) noexcept;

		/**
		 * Assign a string from raw bytes
		 * \param[in] bytes Bytes
		 */
		void AssignRaw(const ByteBuffer& bytes) noexcept;

		/**
		 * Reserve capacity for the underlying utf8 data
		 * \param capacity Capacity to reserve
		 */
		void Reserve(usize capacity) noexcept;
		/**
		 * Resize the String and fill missing elements if needed
		 * \param[in] newSize New size of the String
		 * \param[in] codepoint codepoint to fill missing characters with
		 */
		void Resize(usize newSize, UCodepoint codepoint) noexcept;
		/**
		 * Shrink the memory used by the String to the minimum needed
		 */
		void ShrinkToFit() noexcept;

		/**
		 * Clear the contents of the String, possibly also deallocate the memory
		 * \param[in] clearMemory Whether to deallocate the memory
		 */
		void Clear(bool clearMemory = false) noexcept;
		
		/**
		 * Add a number of codepoints to a String
		 * \param codepoint Codepoint to add
		 * \param count Number of codepoints to add
		 * \return Reference to the String
		 */
		auto Add(UCodepoint codepoint, usize count = 1) noexcept -> String&;
		/**
		 * Add a substring of another String to this String
		 * \param other String to get substring from
		 * \param pos Start position of the substring
		 * \param length Length of the substring
		 * \return Reference to the String
		 */
		auto Add(const String& other, usize pos = 0, usize length = NPos) noexcept -> String&;

		/**
		 * Pad the left side of the string
		 * \param count Number of codepoints to pad with
		 * \param codepoint Codepoint to pad with
		 * \return Reference to the String
		 */
		auto PadLeft(usize count, UCodepoint codepoint = ' ') noexcept -> String&;
		/**
		 * Pad the right side of the string
		 * \param count Number of codepoints to pad with
		 * \param codepoint Codepoint to pad with
		 * \return Reference to the String
		 */
		auto PadRight(usize count, UCodepoint codepoint = ' ') noexcept -> String&;

		/**
		 * Remove a range of utf8 characters from the String
		 * \param pos Position of first utf8 character to erase
		 * \param count Number of character to remove
		 * \return Reference to the String
		 */
		auto Erase(usize pos, usize count = NPos) noexcept -> String&;

		/**
		 * Trim a codepoint from the left of the String
		 * \param codepoint Codepoint to trim
		 * \return Reference to the string
		 */
		auto TrimLeft(UCodepoint codepoint) noexcept -> String&;
		/**
		 * Trim any whitespace from the left of the String
		 * \return Reference to the string
		 */
		auto TrimLeft() noexcept -> String&;
		/**
		 * Trim a codepoint from the right of the String
		 * \param codepoint Codepoint to trim
		 * \return Reference to the string
		 */
		auto TrimRight(UCodepoint codepoint) noexcept -> String&;
		/**
		 * Trim any whitespace from the right of the String
		 * \return Reference to the string
		 */
		auto TrimRight() noexcept -> String&;
		/**
		 * Trim a codepoint from both sides of the String
		 * \param codepoint Codepoint to trim
		 * \return Reference to the string
		 */
		auto Trim(UCodepoint codepoint) noexcept -> String&;
		/**
		 * Trim any whitespace from both sides of the String
		 * \return Reference to the string
		 */
		auto Trim() noexcept -> String&;
		
		/**
		 * Insert a section of the String with a substring of another String
		 * \param pos Position to section to insert
		 * \param str String to insert the section with
		 * \param strPos Position to the start of the substring to insert the section with
		 * \param strLength Length of the substring to insert the section with
		 * \return Reference to the String
		 */
		auto Insert(usize pos, const String& str, usize strPos = 0, usize strLength = NPos) noexcept -> String&;
		/**
		 * Insert a section of the String with a number of codepoints
		 * \param pos Position to section to insert
		 * \param codepoint Codepoint to insert section with
		 * \param count Number of character to insert the section with
		 * \return Reference to the String
		 */
		auto Insert(usize pos, UCodepoint codepoint, usize count) noexcept -> String&;
		
		/**
		 * Replace a section of the String with a substring of another String
		 * \param pos Position to section to replace
		 * \param length Length of the section to replace
		 * \param str String to replace the section with
		 * \param strPos Position to the start of the substring to replace the section with
		 * \param strLength Length of the substring to replace the section with
		 * \return Reference to the String
		 */
		auto Replace(usize pos, usize length, const String& str, usize strPos = 0, usize strLength = NPos) noexcept -> String&;
		/**
		 * Replace a section of the String with a number of codepoints
		 * \param pos Position to section to replace
		 * \param length Length of the section to replace
		 * \param codepoint Codepoint to replace section with
		 * \param count Number of character to replace the section with
		 * \return Reference to the String
		 */
		auto Replace(usize pos, usize length, UCodepoint codepoint, usize count = 1) noexcept -> String&;
		/**
		 * Replace an utf8 character with another utf8 character
		 * \param toReplace Codepoint to replace
		 * \param replaceWith Codepoint to replace with
		 * \return Reference to the String
		 */
		auto Replace(UCodepoint toReplace, UCodepoint replaceWith) noexcept -> String&;
		/**
		 * Replace an utf8 character with a String
		 * \param toReplace Codepoint to replace
		 * \param replaceWith String to replace with
		 * \return Reference to the String
		 */
		auto Replace(UCodepoint toReplace, const String& replaceWith) noexcept -> String&;
		/**
		 * Replace a String with a utf8 character
		 * \param toReplace String to replace
		 * \param replaceWith Codepoint to replace with
		 * \return Reference to the String
		 */
		auto Replace(const String& toReplace, UCodepoint replaceWith) noexcept -> String&;
		/**
		 * Replace a String with another String
		 * \param toReplace String to replace
		 * \param replaceWith String to replace with
		 * \return Reference to the String
		 */
		auto Replace(const String& toReplace, const String& replaceWith) noexcept -> String&;

		/**
		 * Convert the string to upper case
		 * \return Reference to the string
		 */
		auto ToUpper() noexcept -> String&;
		/**
		 * Get a copy of the string that's upper case
		 * \return Reference to the string
		 */
		auto AsUpper() const noexcept -> String;
		/**
		 * Convert the string to lower case
		 * \return Reference to the string
		 */
		auto ToLower() noexcept -> String&;
		/**
		 * Get a copy of the string that's lower case
		 * \return Reference to the string
		 */
		auto AsLower() const noexcept -> String;

		/**
		 * Get a substring
		 * \param pos Position of the start of the substring
		 * \param length Length of the substring
		 * \return Substring
		 */
		auto SubString(usize pos, usize length = NPos) const noexcept -> String;
		
		/**
		 * Split a string into substrings based on a delimiter
		 * \param delimiter Delimiter
		 * \param max Maximum number of substring (last substring will contain the rest of the string)
		 * \param options Splitting options (see SplitOption)
		 * \return Dynamic array with split parts
		 */
		auto Split(UCodepoint delimiter, usize max = NPos, StringSplitOptions options = StringSplitOption::None) const noexcept -> DynArray<String>;
		/**
		 * Split a string into substrings based on a delimiter
		 * \param delimiter Delimiter
		 * \param max Maximum number of substring (last substring will contain the rest of the string)
		 * \param options Splitting options (see SplitOption)
		 * \return Dynamic array with split parts
		 */
		auto Split(const String& delimiter, usize max = NPos, StringSplitOptions options = StringSplitOption::None) const noexcept -> DynArray<String>;

		/**
		 * Split a string into substrings based on whitespace
		 * \param max Maximum number of substring (last substring will contain the rest of the string)
		 * \param options Splitting options (see SplitOption)
		 * \return Dynamic array with split parts
		 */
		auto SplitWhitespace(usize max = NPos, StringSplitOptions options = StringSplitOption::None) const noexcept -> DynArray<String>;

		/**
		 * Find a codepoint in the string
		 * \param codepoint Codepoint to find
		 * \param pos Start position to find from
		 * \param count Number of utf8 characters to look at
		 * \return Position of first occurrence
		 */
		auto Find(UCodepoint codepoint, usize pos = 0, usize count = NPos) const noexcept -> usize;
		/**
		 * Find a string in the string
		 * \param str String to find
		 * \param pos Start position to find from
		 * \param count Number of utf8 characters to look at
		 * \return Position of first occurrence
		 */
		auto Find(const String& str, usize pos = 0, usize count = NPos) const noexcept -> usize;

		/**
		 * Reverse find a codepoint in the string
		 * \param codepoint Codepoint to find
		 * \param pos Start position to find from
		 * \param count Number of utf8 characters to look at
		 * \return Position of last occurrence
		 */
		auto RFind(UCodepoint codepoint, usize pos = NPos, usize count = NPos) const noexcept -> usize;
		/**
		 * Reverse find a string in the string
		 * \param str String to find
		 * \param pos Start position to find from
		 * \param count Number of utf8 characters to look at
		 * \return Position of last occurrence
		 */
		auto RFind(const String& str, usize pos = NPos, usize count = NPos) const noexcept -> usize;
		
		/**
		 * Find the first occurrence of a codepoints that is in the given codepoints
		 * \param codepoints String with codepoints
		 * \param pos Start position to find from
		 * \param count Number of utf8 characters to look at
		 * \return Position of the first occurrence
		 */
		auto FindFirstOf(const String& codepoints, usize pos = 0, usize count = NPos) const noexcept -> usize;
		/**
		 * Find the first occurrence of a codepoints that is not in the given codepoints
		 * \param codepoints String with codepoints
		 * \param pos Start position to find from
		 * \param count Number of utf8 characters to look at
		 * \return Position of the first occurrence
		 */
		auto FindFirstNotOf(const String& codepoints, usize pos = 0, usize count = NPos) const noexcept -> usize;
		/**
		 * Reverse findthe first occurrence of a codepoints that is in the given codepoints
		 * \param codepoints String with codepoints
		 * \param pos Start position to find from
		 * \param count Number of utf8 characters to look at
		 * \return Position of the first occurrence
		 */
		auto RFindFirstOf(const String& codepoints, usize pos = NPos, usize count = NPos) const noexcept -> usize;
		/**
		 * Reverse findthe first occurrence of a codepoints that is not in the given codepoints
		 * \param codepoints String with codepoints
		 * \param pos Start position to find from
		 * \param count Number of utf8 characters to look at
		 * \return Position of the first occurrence
		 */
		auto RFindFirstNotOf(const String& codepoints, usize pos = NPos, usize count = NPos) const noexcept -> usize;

		/**
		 * Check if the string contains a codepoint
		 * \param codepoint Codepoint to check for
		 * \return Whether the string contains the codepoint
		 */
		auto Contains(UCodepoint codepoint) const noexcept -> bool;
		/**
		 * Check if the string contains a string
		 * \param str C-string to check for
		 * \return Whether the string contains the given string
		 */
		auto Contains(const String& str) const noexcept -> bool;

		/**
		 * Check if the string starts with a codepoint
		 * \param codepoint Codepoint to check
		 * \return Whether the string starts with the codepoint
		 */
		auto StartsWith(UCodepoint codepoint) const noexcept -> bool;
		/**
		 * Check if the String starts with a c-string
		 * \param str String to check
		 * \return Whether the String starts with the given String
		 */
		auto StartsWith(const String& str) const noexcept -> bool;

		/**
		 * Check if the string ends with a codepoint
		 * \param codepoint Codepoint to check
		 * \return Whether the string ends with the codepoint
		 */
		auto EndsWith(UCodepoint codepoint) const noexcept -> bool;
		/**
		 * Check if the String ends with a c-string
		 * \param str String to check
		 * \return Whether the String ends with the given String
		 */
		auto EndsWith(const String& str) const noexcept -> bool;

		/**
		 * Get the string as an ascii representation
		 * \return Dynamic array with ascii representation
		 */
		auto ToAscii() const noexcept -> DynArray<char>;
		/**
		 * Get the string as an utf8 representation
		 * \return Dynamic array with utf8 representation
		 */
		auto ToUtf8() const noexcept -> DynArray<char8_t>;
		/**
		 * Get the string as an utf16 representation
		 * \return Dynamic array with utf16 representation
		 */
		auto ToUtf16() const noexcept -> DynArray<char16_t>;
		/**
		 * Get the string as an utf32 representation
		 * \return Dynamic array with utf32 representation
		 */
		auto ToUtf32() const noexcept -> DynArray<char32_t>;
		/**
		 * Get the string as an codepoint/utf32 representation
		 * \return Dynamic array with codepoint/utf32 representation
		 */
		auto ToCodepoints() const noexcept -> DynArray<UCodepoint>;

		/**
		 * Compare 2 strings
		 * \param str String to compare with
		 * \return 0 when string are the same, 1 when the current String is before the other String, and -1 otherwise
		 */
		auto Compare(const String& str) const noexcept -> i8;

		/**
		 * Check whether a string only contains whitespace
		 * \return Whether the string only contains whitespace
		 */
		auto IsWhitespace() const noexcept -> bool;

		/**
		 * Get the codepoint at an index
		 * \param[in] idx Index of codepoint
		 * \return Optional with value
		 * \note Will return an empty optional when index is out of bounds
		 */
		auto At(usize idx) const noexcept -> Optional<UCodepoint>;
		/**
		 * Get the codepoint at an index
		 * \param[in] idx Index of codepoint
		 * \return Codepoint at index
		 * \note Only use when certain the index is not out of bounds
		 */
		auto operator[](usize idx) const noexcept -> UCodepoint;

		/**
		 * Get the length of the String
		 * \return Length of the String
		 */
		auto Length() const noexcept -> usize;
		/**
		 * Check whether the String is empty
		 * \return Whether the String is empty
		 */
		auto IsEmpty() const noexcept -> bool;

		/**
		 * Get the capacity of the underlying Utf8 data
		 * \return Capacity of the underlying Utf8 data
		 */
		auto Capacity() const noexcept -> usize;
		/**
		 * Get the size of the underlying Utf8 data
		 * \return Size of the underlying Utf8 data
		 */
		auto DataSize() const noexcept -> usize;
		/**
		 * Get the underlying Utf8 data
		 * \return Underlying Utf8 data
		 */
		auto Data() noexcept -> u8*;
		/**
		 * Get the underlying Utf8 data
		 * \return Underlying Utf8 data
		 */
		auto Data() const noexcept -> const u8*;

		/**
		 * Get the allocator used by the String
		 * \return Allocator used by the String
		 */
		auto GetAllocator() const noexcept -> Alloc::IAllocator*;
		
		/**
		 * Get the first element in the String
		 * \return First element in the String
		 * \note Only use when the String is not empty
		 */
		auto Front() const noexcept -> UCodepoint;
		/**
		 * Get the last element in the String
		 * \return Last element in the String
		 * \note Only use when the String is not empty
		 */
		auto Back() const noexcept -> UCodepoint;

		/**
		 * Get an iterator to the first element
		 * \return Iterator to the first element
		 */
		auto Begin() noexcept -> Iterator;
		/**
		 * Get an iterator to the first element
		 * \return Iterator to the first element
		 */
		auto Begin() const noexcept -> ConstIterator;

		/**
		 * Get an iterator to the end of the elements
		 * \return Iterator to the end of the elements
		 */
		auto End() noexcept -> Iterator;
		/**
		 * Get an iterator to the end of the elements
		 * \return Iterator to the end of the elements
		 */
		auto End() const noexcept -> ConstIterator;

		/**
		 * Get a reverse Reverse iterator to the first element
		 * \return Reverse iterator to the first element
		 */
		auto RBegin() noexcept -> Iterator;
		/**
		 * Get a reverse Reverse iterator to the first element
		 * \return Reverse iterator to the first element
		 */
		auto RBegin() const noexcept -> ConstIterator;

		/**
		 * Get a reverse Reverse iterator to the end of the elements
		 * \return Reverse iterator to the end of the elements
		 */
		auto REnd() noexcept -> Iterator;
		/**
		 * Get a reverse Reverse iterator to the end of the elements
		 * \return Reverse iterator to the end of the elements
		 */
		auto REnd() const noexcept -> ConstIterator;

		// Overloads for 'for ( ... : ... )'
		auto begin() noexcept -> Iterator;
		auto begin() const noexcept -> ConstIterator;
		auto cbegin() const noexcept -> ConstIterator;
		auto end() noexcept -> Iterator;
		auto end() const noexcept -> ConstIterator;
		auto cend() const noexcept -> ConstIterator;


	private:

		/**
		 * Assign a string to this string
		 * \param other String to assign
		 * \param idx Start index in bytes
		 * \param pos Start position in characters
		 * \param size Size in bytes
		 * \param length Size in characters
		 */
		void AssignSubstring(const String& other, usize idx, usize pos, usize size, usize length) noexcept;

		/**
		 * Replace a section of the String with a number of codepoints
		 * \param idx Index to section to replace in bytes
		 * \param byteLength Length of the section to replace in bytes
		 * \param length Length of the section to replace in utf8 characters
		 * \param c Utf8 character to replace section with
		 * \param count Number of character to replace the section with
		 * \return Reference to the String
		 */
		auto ReplaceInternal(usize idx, usize byteLength, usize length, Unicode::Utf8Char c, usize count) noexcept -> String&;
		/**
		 * Replace a section of the String with a number of codepoints
		 * \param idx Index to section to replace in bytes
		 * \param byteLength Length of the section to replace in bytes
		 * \param length Length of the section to replace in utf8 characters
		 * \param str String to replace the section with
		 * \param strPos Position to the start of the substring to replace the section with
		 * \param strByteLength Length of the substring to replace the section with in characters
		 * \param strLength Length of the substring to replace the section with in bytes
		 * \return Reference to the String
		 */
		auto ReplaceInternal(usize idx, usize byteLength, usize length, const String& str, usize strPos, usize strByteLength, usize strLength) noexcept -> String&;

		/**
		 * Get the index of a codepoint
		 * \param codepoint Codepoint to find
		 * \param pos Start position to find from in characters
		 * \param idx Start position to find from in bytes
		 * \param count Number of utf8 characters to look at
		 * \return Pair with the index of a codepoint in characters and bytes
		 */
		auto FindInternal(UCodepoint codepoint, usize pos, usize idx, usize count) const noexcept -> Pair<usize, usize>;
		/**
		 * Get the index of a codepoint
		 * \param str String to find
		 * \param pos Start position to find from in characters
		 * \param idx Start position to find from in bytes
		 * \param count Number of utf8 characters to look at
		 * \return Pair with the index of a codepoint in characters and bytes
		 */
		auto FindInternal(const String& str, usize pos, usize idx, usize count) const noexcept -> Pair<usize, usize>;
		/**
		 * Get the index of a whitespace
		 * \param pos Start position to find from in characters
		 * \param idx Start position to find from in bytes
		 * \param count Number of utf8 characters to look at
		 * \return Pair with the index of a whitespace in characters and bytes
		 */
		auto FindWhitespaceInternal(usize pos, usize idx, usize count) const noexcept -> Pair<usize, usize>;

		/**
		 * Get the index of a character position
		 * \param pos Character position
		 * \return Index of the character position
		 */
		auto IndexAtCharPos(usize pos) const noexcept -> usize;
		/**
		 * Convert an offset in character to bytes, starting from a certain index
		 * \param startIdx Index to start counting from
		 * \param offset Offset of characters
		 * \return Offset in string in bytes
		 */
		auto IndexForOffset(usize startIdx, usize offset) const noexcept -> usize;
		/**
		 * Get the previous index of a character
		 * \param pData Pointer to string data
		 * \param idx Current index
		 * \return Previous index
		 */
		auto PrevIdx(const u8* pData, usize idx) const noexcept -> usize;
		/**
		 * Get the pointer to previous character
		 * \param pData Pointer to the current data in the string
		 * \return Pointer to the previous character
		 */
		auto PrevCharPtr(const u8* pData) const noexcept -> const u8*;

		/**
		 * Make sure the internal string is null-terminated 
		 */
		void NullTerminate() noexcept;

		DynArray<u8> m_data;   ///< UTF8 data
		usize        m_length; ///< String length
	};

	template<>
	struct CORE_API Hash<String>
	{
		auto operator()(const String& t) const noexcept -> u64;
	};

}

inline namespace Literals
{
	auto operator""_s(const char* cstr, usize size) noexcept -> Onca::String;
	auto operator""_s(const char16_t* cstr, usize size) noexcept -> Onca::String;
	auto operator""_s(const char32_t* cstr, usize size) noexcept -> Onca::String;
}

#include "String.inl"