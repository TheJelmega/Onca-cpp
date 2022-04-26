#pragma once
#if __RESHARPER__
#include "ConstString.h"
#endif
#include "ConstString.h"

namespace Onca
{
	template<usize Cap>
	constexpr ConstString<Cap>::Iterator::Iterator() noexcept
		: m_pCStr(nullptr)
		, m_idx(0)
	{
	}

	template<usize Cap>
	constexpr ConstString<Cap>::Iterator::Iterator(const Iterator& other) noexcept
		: m_pCStr(other.m_pCStr)
		, m_idx(other.m_idx)
	{
	}

	template<usize Cap>
	constexpr ConstString<Cap>::Iterator::Iterator(Iterator&& other) noexcept
		: m_pCStr(other.m_pCStr)
		, m_idx(other.m_idx)
	{
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Iterator::operator=(const Iterator& other) noexcept -> Iterator&
	{
		m_pCStr = other.m_pCStr;
		m_idx = other.m_idx;
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Iterator::operator=(Iterator&& other) noexcept -> Iterator&
	{
		m_pCStr = other.m_pCStr;
		m_idx = other.m_idx;
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Iterator::operator*() const noexcept -> UCodepoint
	{
		return Unicode::GetCpFromUtf8(m_pCStr->Data() + m_idx);
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Iterator::operator++() noexcept -> Iterator&
	{
		m_idx += Unicode::GetUtf8Size(m_pCStr->Data()[m_idx]);
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Iterator::operator++(int) noexcept -> Iterator
	{
		Iterator it{ *this };
		operator++();
		return it;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Iterator::operator--() noexcept -> Iterator&
	{
		const u8* pData = m_pCStr->Data();
		--m_idx;
		while (m_idx && (pData[m_idx] & 0x80) == 0x80)
			--m_idx;
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Iterator::operator--(int) noexcept -> Iterator
	{
		Iterator it{ *this };
		operator--();
		return it;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Iterator::operator+(usize count) const noexcept -> Iterator
	{
		Iterator it{ *this };
		for (usize i = 0; i < count; ++i)
			++it;
		return it;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Iterator::operator-(usize count) const noexcept -> Iterator
	{
		Iterator it{ *this };
		for (usize i = 0; i < count; ++i)
			--it;
		return it;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Iterator::operator-(const Iterator& other) const noexcept -> isize
	{
		ASSERT(*this >= other, "Cannot subtract iterator coming before this iterator");
		usize diff = 0;
		for (Iterator it{ *this }; it != other; ++it)
			++diff;
		return diff;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Iterator::operator+=(usize count) noexcept -> Iterator&
	{
		for (usize i = 0; i < count; ++i)
			operator++();
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Iterator::operator-=(usize count) noexcept -> Iterator&
	{
		for (usize i = 0; i < count; ++i)
			operator--();
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Iterator::operator==(const Iterator& other) const noexcept -> bool
	{
		return m_pCStr == other.m_pCStr && m_idx == other.m_idx;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Iterator::operator!=(const Iterator& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Iterator::operator<=>(const Iterator& other) const noexcept -> std::partial_ordering
	{
		if (m_pCStr != other.m_pCStr)
			return std::partial_ordering::unordered;

		if (m_idx < other.m_idx) return std::partial_ordering::less;
		if (m_idx > other.m_idx) return std::partial_ordering::greater;
		return std::partial_ordering::equivalent;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Iterator::operator[](usize idx) const noexcept -> const UCodepoint&
	{
		return *(*this + idx);
	}

	template<usize Cap>
	constexpr ConstString<Cap>::Iterator::Iterator(const ConstString<Cap>* pData, usize idx)
		: m_pCStr(pData)
		, m_idx(idx)
	{
	}

	template<usize Cap>
	constexpr ConstString<Cap>::ConstString() noexcept
		: m_data()
		, m_length(0)
	{
	}

	template<usize Cap>
	constexpr ConstString<Cap>::ConstString(usize capacity) noexcept
		: m_data(capacity)
		, m_length(0)
	{
	}

	template<usize Cap>
	constexpr ConstString<Cap>::ConstString(UCodepoint codepoint, usize count) noexcept
		: m_data()
		, m_length(0)
	{
		Assign(codepoint, count);
	}

	template<usize Cap>
	constexpr ConstString<Cap>::ConstString(const ConstString& other, usize pos) noexcept
		: m_data()
		, m_length(0)
	{
		Assign(other, pos);
	}

	template<usize Cap>
	constexpr ConstString<Cap>::ConstString(const ConstString& other, usize pos, usize length) noexcept
		: m_data()
		, m_length(0)
	{
		Assign(other, pos, length);
	}
	
	template<usize Cap>
	template <CharacterType C>
	constexpr ConstString<Cap>::ConstString(const C* str) noexcept
		: m_data()
		, m_length(0)
	{
		Assign(str);
	}

	template<usize Cap>
	template <CharacterType C>
	constexpr ConstString<Cap>::ConstString(const C* str, usize length) noexcept
		: m_data()
		, m_length(0)
	{
		Assign(str, length);
	}

	template<usize Cap>
	template <ConvertableToUnicode T>
	constexpr ConstString<Cap>::ConstString(const InitializerList<T>& il) noexcept
		: m_data()
		, m_length(0)
	{
		Assign(il);
	}

	template<usize Cap>
	template <ForwardIterator It> requires DereferencableToUnicode<It>
	constexpr ConstString<Cap>::ConstString(const It& begin, const It& end) noexcept
		: m_data()
		, m_length(0)
	{
		Assign(begin, end);
	}

	template<usize Cap>
	template <CharacterType C>
	constexpr auto ConstString<Cap>::operator=(const C* str) noexcept -> ConstString&
	{
		Assign(str);
		return *this;
	}

	template<usize Cap>
	template <ConvertableToUnicode T>
	constexpr auto ConstString<Cap>::operator=(const InitializerList<T>& il) noexcept -> ConstString&
	{
		Assign(il);
		return *this;
	}

	template<usize Cap>
	constexpr ConstString<Cap>::ConstString(const ConstString& other) noexcept
		: m_data(other.m_data)
		, m_length(other.m_length)
	{
	}
	
	template<usize Cap>
	constexpr ConstString<Cap>::ConstString(ConstString&& other) noexcept
		: m_data(Move(other.m_data))
		, m_length(other.m_length)
	{
		other.m_length = 0;
	}
	
	template<usize Cap>
	constexpr auto ConstString<Cap>::operator=(const ConstString& other) noexcept -> ConstString&
	{
		m_data = other.m_data;
		m_length = other.m_length;
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::operator=(ConstString&& other) noexcept -> ConstString&
	{
		m_data = Move(other.m_data);
		m_length = other.m_length;
		other.m_length = 0;
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::operator+(UCodepoint codepoint) const noexcept -> ConstString
	{
		ConstString copy{ *this };
		copy.Add(codepoint);
		return copy;
	}

	template<usize Cap>
	template <CharacterType C>
	constexpr auto ConstString<Cap>::operator+(const C* str) const noexcept -> ConstString
	{
		ConstString copy{ *this };
		copy.Add(str);
		return copy;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::operator+(const ConstString& other) const noexcept -> ConstString
	{
		ConstString copy{ *this };
		copy.Add(other);
		return copy;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::operator+=(UCodepoint codepoint) noexcept -> ConstString&
	{
		return Add(codepoint);
	}

	template<usize Cap>
	template <CharacterType C>
	constexpr auto ConstString<Cap>::operator+=(const C* str) noexcept -> ConstString&
	{
		return Add(str);
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::operator+=(const ConstString& other) noexcept -> ConstString&
	{
		return Add(other);
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::operator<=>(const ConstString& other) const noexcept -> std::strong_ordering
	{
		i8 res = Compare(other);
		if (res == 0) return std::strong_ordering::equal;
		if (res < 0) return std::strong_ordering::less;
		return std::strong_ordering::greater;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::operator==(const ConstString& other) const noexcept -> bool
	{
		if (m_data.Size() != other.DataSize())
			return false;

		return MemCmp(m_data.Data(), other.Data(), m_data.Size()) == 0;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::operator!=(const ConstString& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	template<usize Cap>
	constexpr void ConstString<Cap>::Assign(UCodepoint codepoint, usize count) noexcept
	{
		m_data.Clear();
		m_length = 0;
		Add(codepoint, count);
	}

	template<usize Cap>
	constexpr void ConstString<Cap>::Assign(const ConstString& other, usize pos, usize length) noexcept
	{
		m_data.Clear();
		m_length = 0;
		Add(other, pos, length);
	}

	template<usize Cap>
	template <CharacterType C>
	constexpr void ConstString<Cap>::Assign(const C* str) noexcept
	{
		Assign(str, StrLen(str));
	}

	template<usize Cap>
	template <CharacterType C>
	constexpr void ConstString<Cap>::Assign(const C* str, usize length) noexcept
	{
		m_data.Clear();
		m_length = 0;
		for (usize i = 0; i < length; ++i)
		{
			const auto [c, toSkip] = Unicode::ToUtf8(str);
			str += toSkip;

			const usize size = m_data.Size();
			if (size + c.size + 1 >= Cap) // +1 for null terminator
				break;
			m_data.Resize(size + c.size);
			MemCpy<u8>(m_data.Data() + size, c.data, c.size);
			++m_length;
		}
		NullTerminate();
	}

	template<usize Cap>
	template <ConvertableToUnicode T>
	constexpr void ConstString<Cap>::Assign(const InitializerList<T>& il) noexcept
	{
		m_data.Clear();
		Assign(il.begin(), il.end());
	}

	template<usize Cap>
	template <ForwardIterator It> requires DereferencableToUnicode<It>
	constexpr void ConstString<Cap>::Assign(const It& begin, const It& end) noexcept
	{
		m_data.Clear();
		m_length = 0;
		for (It it = begin; it != end;)
		{
			const auto [c, toSkip] = Unicode::ToUtf8(&*it);
			for (usize i = 0; i < toSkip; ++i)
				++it;

			const usize size = m_data.Size();
			if (size + c.size + 1 >= Cap) // +1 for null terminator
				break;
			m_data.Resize(size + c.size);
			MemCpy<u8>(m_data.Data() + size, c.data, c.size);
			++m_length;
		}
		NullTerminate();
	}
	
	template<usize Cap>
	constexpr void ConstString<Cap>::Resize(usize newSize, UCodepoint codepoint) noexcept
	{
		if (newSize > m_length)
		{
			Add(codepoint, newSize - m_length);
		}
		else
		{
			usize endIdx = IndexAtCharPos(newSize);
			endIdx += Unicode::GetUtf8Size(m_data.Data()[endIdx]);
			m_data.Resize(endIdx - 1);
		}
		m_length = newSize;
	}
	
	template<usize Cap>
	constexpr auto ConstString<Cap>::Add(UCodepoint codepoint, usize count) noexcept -> ConstString&
	{
		const Unicode::Utf8Char c = Unicode::GetUtf8FromCp(codepoint);
		usize size = count * c.size;
		while (m_data.Size() + size + 1 > Cap)
		{
			--count;
			size -= c.size;
		}
		m_data.Insert(m_data.IteratorAt(m_data.Size()), count * c.size, 0);

		u8* pData = m_data.Data() + m_data.Size();
		for (usize i = 0; i < count; ++i, pData += c.size)
			MemCpy<u8>(pData, c.data, c.size);

		m_length += count;
		NullTerminate();
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Add(const ConstString& other, usize pos, usize length) noexcept -> ConstString&
	{
		ASSERT(pos < other.Length(), "'pos' needs to point to a character inside the ConstString");
		if (length > other.Length() - pos)
			length = other.Length() - pos;

		if (length == 0)
			return *this;

		const usize startIdx = other.IndexAtCharPos(pos);
		const usize endIdx = other.IndexForOffset(startIdx, length);
		usize byteLen = endIdx - startIdx;

		const usize size = m_data.Size();
		while (size + byteLen + 1 > Cap)
		{
			--length;
			const usize tmpIdx = other.IndexForOffset(startIdx, length);
			byteLen = tmpIdx - startIdx;
		}
		
		MemCpy<u8>(m_data.Data() + size, other.Data() + startIdx, byteLen);
		m_length += length;
		NullTerminate();
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::PadLeft(usize count, UCodepoint codepoint) noexcept -> ConstString&
	{
		return Insert(0, codepoint, count);
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::PadRight(usize count, UCodepoint codepoint) noexcept -> ConstString&
	{
		return Add(codepoint, count);
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Erase(usize pos, usize count) noexcept -> ConstString&
	{
		if (count > m_length - pos)
			count = m_length - pos;

		const usize idx = IndexAtCharPos(pos);
		const usize end = IndexForOffset(idx, count);
		m_data.Erase(m_data.IteratorAt(idx), end - idx);

		m_length -= count;
		NullTerminate();
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::TrimLeft(UCodepoint codepoint) noexcept -> ConstString&
	{
		if (m_length == 0)
			return *this;

		const Unicode::Utf8Char c = Unicode::GetUtf8FromCp(codepoint);
		const usize endByte = m_data.Size();
		const u8* pData = m_data.Data();
		usize idx = 0, len = 0;
		while (idx < endByte)
		{
			if (!Unicode::MatchChar(pData + idx, c.data, c.size))
				break;

			idx += Unicode::GetUtf8Size(*(pData + idx));
			++len;
		}
		if (idx == 0)
			return *this;

		m_data.Erase(m_data.Begin(), m_data.IteratorAt(idx));
		m_length -= len;
		NullTerminate();
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::TrimLeft() noexcept -> ConstString&
	{
		if (m_length == 0)
			return *this;

		const usize endByte = m_data.Size();
		const u8* pData = m_data.Data();
		usize idx = 0, len = 0;
		while (idx < endByte)
		{
			if (!Unicode::IsWhitespace(pData + idx))
				break;

			idx += Unicode::GetUtf8Size(*(pData + idx));
			++len;
		}
		if (idx == 0)
			return *this;

		m_data.Erase(m_data.Begin(), m_data.IteratorAt(idx));
		m_length -= len;
		NullTerminate();
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::TrimRight(UCodepoint codepoint) noexcept -> ConstString&
	{
		if (m_length == 0)
			return *this;

		const Unicode::Utf8Char c = Unicode::GetUtf8FromCp(codepoint);
		const u8* pData = m_data.Data();
		usize idx = PrevIdx(pData, m_data.Size());

		usize len = 0;
		while (idx)
		{
			if (!Unicode::MatchChar(pData + idx, c.data, c.size))
				break;

			idx = PrevIdx(pData, idx);
			++len;
		}
		if (idx == m_data.Size() - 1)
			return *this;

		idx += Unicode::GetUtf8Size(pData[idx]);
		m_data.Erase(m_data.IteratorAt(idx), m_data.End());
		m_length -= len;
		NullTerminate();
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::TrimRight() noexcept -> ConstString&
	{
		if (m_length == 0)
			return *this;

		const u8* pData = m_data.Data();
		usize idx = PrevIdx(pData, m_data.Size());

		usize len = 0;
		while (idx)
		{
			if (!Unicode::IsWhitespace(pData + idx))
				break;

			idx = PrevIdx(pData, idx);
			++len;
		}
		if (idx == m_data.Size() - 1)
			return *this;

		idx += Unicode::GetUtf8Size(pData[idx]);
		m_data.Erase(m_data.IteratorAt(idx), m_data.End());
		m_length -= len;
		NullTerminate();
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Trim(UCodepoint codepoint) noexcept -> ConstString&
	{
		if (m_length == 0)
			return *this;

		TrimLeft(codepoint);
		TrimRight(codepoint);
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Trim() noexcept -> ConstString&
	{
		if (m_length == 0)
			return *this;

		TrimLeft();
		TrimRight();
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Insert(usize pos, const ConstString& str, usize strPos, usize strLength) noexcept -> ConstString&
	{
		ASSERT(pos < m_length, "'pos' needs to point to a character inside the ConstString");
		ASSERT(strPos < str.Length(), "'strPos' needs to point to a character inside 'str'");
		if (strLength > str.Length() - strPos)
			strLength = str.Length() - strPos;

		const usize idx = IndexAtCharPos(pos);
		const usize strIdx = str.IndexAtCharPos(strPos);
		const usize strEnd = str.IndexForOffset(strIdx, strLength);
		usize otherSize = strEnd - strIdx;

		while (m_data.Size() + otherSize + 1 > Cap)
		{
			--strLength;
			const usize tmpIdx = str.IndexForOffset(strIdx, strLength);
			otherSize = tmpIdx - strIdx;
		}

		m_data.Insert(m_data.IteratorAt(idx), otherSize, 0);
		MemCpy<u8>(m_data.Data() + idx, str.m_data.Data() + strIdx, otherSize);
		m_length += strLength;
		NullTerminate();
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Insert(usize pos, UCodepoint codepoint, usize count) noexcept -> ConstString&
	{
		ASSERT(pos < m_length, "'pos' needs to point to a character inside the ConstString");
		const usize idx = IndexAtCharPos(pos);
		const Unicode::Utf8Char c = Unicode::GetUtf8FromCp(codepoint);

		usize size = count * c.size;
		while (m_data.Size() + size + 1 > Cap)
		{
			--count;
			size -= c.size;
		}
		m_data.Insert(m_data.IteratorAt(idx), count * c.size, 0);

		u8* pData = m_data.Data() + idx;
		for (usize i = 0; i < count; ++i, pData += c.size)
			MemCpy<u8>(pData, c.data, c.size);

		m_length += count;
		NullTerminate();
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Replace(usize pos, usize length, const ConstString& str, usize strPos, usize strLength) noexcept -> ConstString&
	{
		ASSERT(pos < m_length, "'pos' needs to point to a character inside the ConstString");
		ASSERT(strPos < str.Length(), "'pos' needs to point to a character inside 'str'");
		if (strLength > str.Length() - strPos)
			strLength = str.Length() - strPos;

		if (length > m_length - pos)
			length = m_length - pos;

		const usize idx = IndexAtCharPos(pos);
		const usize endIdx = IndexForOffset(idx, length);
		const usize byteLength = endIdx - idx;
		const usize strIdx = str.IndexAtCharPos(strPos);
		const usize strEnd = str.IndexForOffset(strIdx, strLength);
		return ReplaceInternal(idx, byteLength, length, str, strIdx, strEnd, strLength);
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Replace(usize pos, usize length, UCodepoint codepoint, usize count) noexcept -> ConstString&
	{
		ASSERT(pos < m_length, "'pos' needs to point to a character inside the ConstString");
		if (length > m_length - pos)
			length = m_length - pos;

		const usize idx = IndexAtCharPos(pos);
		const usize endIdx = IndexForOffset(idx, length);
		const usize byteLength = endIdx - idx;
		const Unicode::Utf8Char c = Unicode::GetUtf8FromCp(codepoint);
		return ReplaceInternal(idx, byteLength, length, c, count);
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Replace(UCodepoint toReplace, UCodepoint replaceWith) noexcept -> ConstString&
	{
		const Unicode::Utf8Char c = Unicode::GetUtf8FromCp(replaceWith);

		usize charIdx = 0, byteIdx = 0;
		u8* pData = m_data.Data();
		while (charIdx < m_length)
		{
			auto [cIdx, bIdx] = FindInternal(toReplace, charIdx, byteIdx, NPos);
			if (cIdx == NPos)
				break;

			charIdx = cIdx;
			byteIdx = bIdx;

			const usize curLen = Unicode::GetUtf8Size(*(pData + byteIdx));
			ReplaceInternal(byteIdx, curLen, 1, c, 1);

			++charIdx;
			byteIdx += c.size;
		}
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Replace(UCodepoint toReplace, const ConstString& replaceWith) noexcept -> ConstString&
	{
		usize charIdx = 0, byteIdx = 0;
		const usize otherSize = replaceWith.DataSize();
		const usize otherLength = replaceWith.Length();

		const usize cpSize = Unicode::GetCodepointSizeInUtf8(toReplace);
		while (charIdx < m_length)
		{
			auto [cIdx, bIdx] = FindInternal(toReplace, charIdx, byteIdx, NPos);
			if (cIdx == NPos)
				break;

			charIdx = cIdx;
			byteIdx = bIdx;

			ReplaceInternal(byteIdx, cpSize, 1, replaceWith, 0, otherSize, replaceWith.m_length);

			charIdx += otherLength;
			byteIdx += otherSize;
		}
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Replace(const ConstString& toReplace, UCodepoint replaceWith) noexcept -> ConstString&
	{
		const Unicode::Utf8Char c = Unicode::GetUtf8FromCp(replaceWith);
		const usize curLen = toReplace.DataSize();
		const usize replLength = toReplace.Length();

		usize charIdx = 0, byteIdx = 0;
		while (charIdx < m_length)
		{
			auto [cIdx, bIdx] = FindInternal(toReplace, charIdx, byteIdx, NPos);
			if (cIdx == NPos)
				break;

			charIdx = cIdx;
			byteIdx = bIdx;

			ReplaceInternal(byteIdx, curLen, replLength, c, 1);

			++charIdx;
			byteIdx += c.size;
		}
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Replace(const ConstString& toReplace, const ConstString& replaceWith) noexcept -> ConstString&
	{
		usize charIdx = 0, byteIdx = 0;
		const usize otherSize = replaceWith.DataSize();
		const usize otherLength = replaceWith.Length();
		const usize byteLength = toReplace.DataSize();
		const usize curLen = toReplace.Length();
		while (charIdx < m_length)
		{
			auto [cIdx, bIdx] = FindInternal(toReplace, charIdx, byteIdx, NPos);
			if (cIdx == NPos)
				break;

			charIdx = cIdx;
			byteIdx = bIdx;

			ReplaceInternal(byteIdx, byteLength, curLen, replaceWith, 0, otherSize, replaceWith.m_length);

			charIdx += otherLength;
			byteIdx += otherSize;
		}
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::ToUpper() noexcept -> ConstString&
	{
		u8* pData = m_data.Data();
		for (usize i = 0, size = m_data.Size(); i < size;)
		{
			const usize utf8size = Unicode::GetUtf8Size(pData[i]);
			const Unicode::Utf8Char upper = Unicode::ToUpper(pData + i);
			if (upper.size > utf8size)
			{
				m_data.Insert(pData + i, upper.size - utf8size, 0);
				pData = m_data.Data();
			}
			else if (upper.size < utf8size)
			{
				m_data.Erase(pData + i, utf8size - upper.size);
			}
			MemCpy<u8>(pData + i, upper.data, upper.size);
			i += upper.size;
		}
		NullTerminate();
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::AsUpper() const noexcept -> ConstString
	{
		ConstString res;
		const u8* pSourceData = m_data.Data();
		u8* pData = res.Data();
		for (usize i = 0, size = m_data.Size(); i < size;)
		{
			const usize utf8size = Unicode::GetUtf8Size(*pSourceData);
			const Unicode::Utf8Char upper = Unicode::ToUpper(pSourceData);

			res.m_data.Resize(res.DataSize() + upper.size);
			pData = res.Data();

			MemCpy<u8>(pData + i, upper.data, upper.size);
			i += upper.size;
			pSourceData += utf8size;
		}

		res.m_length = m_length;
		res.NullTerminate();
		return res;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::ToLower() noexcept -> ConstString&
	{
		u8* pData = m_data.Data();
		for (usize i = 0, size = m_data.Size(); i < size;)
		{
			const usize utf8size = Unicode::GetUtf8Size(pData[i]);
			const Unicode::Utf8Char upper = Unicode::ToLower(pData + i);
			if (upper.size > utf8size)
			{
				m_data.Insert(pData + i, upper.size - utf8size, 0);
				pData = m_data.Data();
			}
			else if (upper.size < utf8size)
			{
				m_data.Erase(pData + i, utf8size - upper.size);
			}
			MemCpy<u8>(pData + i, upper.data, upper.size);
			i += upper.size;
		}
		NullTerminate();
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::AsLower() const noexcept -> ConstString
	{
		ConstString res;
		const u8* pSourceData = m_data.Data();
		u8* pData = res.Data();
		for (usize i = 0, size = m_data.Size(); i < size;)
		{
			const usize utf8size = Unicode::GetUtf8Size(*pSourceData);
			const Unicode::Utf8Char upper = Unicode::ToLower(pSourceData);

			res.m_data.Resize(res.DataSize() + upper.size);
			pData = res.Data();

			MemCpy<u8>(pData + i, upper.data, upper.size);
			i += upper.size;
			pSourceData += utf8size;
		}

		res.m_length = m_length;
		res.NullTerminate();
		return res;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::SubConstString(usize pos, usize length) const noexcept -> ConstString
	{
		return ConstString{ *this, pos, length };
	}

	template<usize Cap>
	template<usize Max>
	constexpr auto ConstString<Cap>::Split(UCodepoint delimiter, StringSplitOptions options) noexcept -> InplaceDynArray<ConstString, Cap * sizeof(this)>
	{
		usize curPos = 0;
		usize curIdx = 0;

		const usize strSize = m_data.Size();
		
		InplaceDynArray<ConstString, Cap * sizeof(this)> res;
		usize max = Max;
		while (curIdx < strSize && max > 1)
		{
			auto [pos, idx] = FindInternal(delimiter, curPos, curIdx, NPos);
			usize size = idx - curIdx;
			usize len = pos - curPos;

			if (size || !(options & StringSplitOption::RemoveEmpty))
			{
				ConstString subStr;
				subStr.AssignSubConstString(*this, curIdx, curPos, size, len);
				subStr.NullTerminate();

				if (options & StringSplitOption::TrimEntries)
					subStr.Trim();
				if (!subStr.IsEmpty() || !(options & StringSplitOption::RemoveEmpty))
				{
					res.Add(Move(subStr));
				}
			}

			curPos = pos + 1;
			curIdx = idx + 1;
			--max;
		}

		if (max == 1)
		{
			res.EmplaceBack();
			res.Back().AssignSubConstString(*this, curIdx, curPos, strSize - curIdx, m_length - curPos);
			res.Back().NullTerminate();
		}
		return res;
	}

	template<usize Cap>
	template<usize Max>
	constexpr auto ConstString<Cap>::Split(const ConstString& delimiter, StringSplitOptions options) const noexcept -> InplaceDynArray<ConstString, Cap * sizeof(this)>
	{
		usize curPos = 0;
		usize curIdx = 0;

		const usize strSize = m_data.Size();
		const usize delimiterSize = delimiter.DataSize();
		const usize delimiterLen = delimiter.Length();
		
		InplaceDynArray<ConstString, Cap * sizeof(this)> res;
		usize max = Max;
		while (curIdx < strSize && max > 1)
		{
			auto [pos, idx] = FindInternal(delimiter, curPos, curIdx, NPos);
			usize size = idx - curIdx;
			usize len = pos - curPos;

			if (size || !(options & StringSplitOption::RemoveEmpty))
			{
				ConstString subStr;
				subStr.AssignSubConstString(*this, curIdx, curPos, size, len);
				subStr.NullTerminate();

				if (options & StringSplitOption::TrimEntries)
					subStr.Trim();
				if (!subStr.IsEmpty() || !(options & StringSplitOption::RemoveEmpty))
				{
					res.Add(Move(subStr));
				}
			}

			curPos = pos + delimiterSize;
			curIdx = idx + delimiterLen;
			--max;
		}

		if (max == 1)
		{
			res.EmplaceBack();
			res.Back().AssignSubConstString(*this, curIdx, curPos, strSize - curIdx, m_length - curPos);
			res.Back().NullTerminate();
		}
		return res;
	}

	template<usize Cap>
	template<usize Max>
	constexpr auto ConstString<Cap>::SplitWhitespace(StringSplitOptions options) const noexcept -> InplaceDynArray<ConstString, Cap * sizeof(this)>
	{
		usize curPos = 0;
		usize curIdx = 0;

		const usize strSize = m_data.Size();
		
		InplaceDynArray<ConstString, Cap * sizeof(this)> res;
		usize max = Max;
		while (curIdx < strSize && max > 1)
		{
			auto [pos, idx] = FindWhitespaceInternal(curPos, curIdx, NPos);
			usize size = idx - curIdx;
			usize len = pos - curPos;

			if (size || !(options & StringSplitOption::RemoveEmpty))
			{
				ConstString subStr;
				subStr.AssignSubConstString(*this, curIdx, curPos, size, len);
				subStr.NullTerminate();

				if (options & StringSplitOption::TrimEntries)
					subStr.Trim();
				if (!subStr.IsEmpty() || !(options & StringSplitOption::RemoveEmpty))
				{
					res.Add(Move(subStr));
				}
			}

			curPos = pos + 1;
			curIdx = idx + 1;
			--max;
		}

		if (max == 1)
		{
			res.EmplaceBack();
			res.Back().AssignSubConstString(*this, curIdx, curPos, strSize - curIdx, m_length - curPos);
			res.Back().NullTerminate();
		}
		return res;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Find(UCodepoint codepoint, usize pos, usize count) const noexcept -> usize
	{
		ASSERT(pos < m_length, "'pos' needs to point to a character inside the ConstString");
		const usize startIdx = IndexAtCharPos(pos);
		auto [ret, _] = FindInternal(codepoint, pos, startIdx, count);
		return ret;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Find(const ConstString& str, usize pos, usize count) const noexcept -> usize
	{
		ASSERT(pos < m_length, "'pos' needs to point to a character inside the ConstString");
		if (str.Length() > m_length)
			return NPos;

		const usize startIdx = IndexAtCharPos(pos);
		auto [ret, _] = FindInternal(str, pos, startIdx, count);
		return ret;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::RFind(UCodepoint codepoint, usize pos, usize count) const noexcept -> usize
	{
		if (pos == NPos)
			pos = m_length - 1;
		else
			ASSERT(pos < m_length, "'pos' needs to point to a character inside the ConstString");

		if (count > pos)
			count = pos;

		const Unicode::Utf8Char c = Unicode::GetUtf8FromCp(codepoint);
		const usize endPos = pos - count;
		const u8* pData = m_data.Data();

		usize idx = IndexAtCharPos(pos);

		while (pos >= endPos)
		{
			if (Unicode::MatchChar(pData + idx, c.data, c.size))
				return pos;
			if (idx == 0)
				return NPos;

			idx = PrevIdx(pData, idx);
			--pos;
		}
		return pos;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::RFind(const ConstString& str, usize pos, usize count) const noexcept -> usize
	{
		if (pos == NPos)
			pos = m_length - 1;
		else
			ASSERT(pos < m_length, "'pos' needs to point to a character inside the ConstString");

		if (str.Length() > m_length)
			return NPos;

		if (count > pos)
			count = pos;

		const usize endPos = pos - count;
		if (pos > m_length - str.Length())
			pos = m_length - str.Length();

		const u8* pData = m_data.Data();
		const u8* pOtherData = str.Data();
		const usize otherSize = str.DataSize();

		usize idx = IndexAtCharPos(pos);

		while (pos >= endPos)
		{
			bool found = true;
			for (usize i = 0; found && i < otherSize; ++i)
			{
				if (pData[idx + i] != pOtherData[i])
					found = false;
			}
			if (found)
				return pos;
			if (idx == 0)
				return NPos;

			idx = PrevIdx(pData, idx);
			--pos;
		}
		return pos;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::FindFirstOf(const ConstString& codepoints, usize pos, usize count) const noexcept -> usize
	{
		const u8* pData = m_data.Data();
		const usize len = Math::Min(m_length, count);

		const u8* pCodepoints = codepoints.Data();
		const usize numCodepoints = codepoints.Length();

		while (pos < len)
		{
			const usize utf8Size = Unicode::GetUtf8Size(*pData);

			const u8* pCurCp = pCodepoints;
			for (usize i = 0; i < numCodepoints; ++i)
			{
				const usize cpSize = Unicode::GetUtf8Size(*pCurCp);
				if (Unicode::MatchChar(pData, pCurCp, Math::Min(utf8Size, cpSize)))
					return pos;
				pCurCp += cpSize;
			}

			pData += Unicode::GetUtf8Size(*pData);
			++pos;
		}
		return NPos;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::FindFirstNotOf(const ConstString& codepoints, usize pos, usize count) const noexcept -> usize
	{
		const u8* pData = m_data.Data();
		const usize len = Math::Min(m_length, count);

		const u8* pCodepoints = codepoints.Data();
		const usize numCodepoints = codepoints.Length();

		while (pos < len)
		{
			const usize utf8Size = Unicode::GetUtf8Size(*pData);

			const u8* pCurCp = pCodepoints;
			bool found = false;
			for (usize i = 0; i < numCodepoints; ++i)
			{
				const usize cpSize = Unicode::GetUtf8Size(*pCurCp);
				if (Unicode::MatchChar(pData, pCurCp, Math::Min(utf8Size, cpSize)))
				{
					found = true;
					break;
				}
				pCurCp += cpSize;
			}
			if (!found)
				return pos;

			pData += Unicode::GetUtf8Size(*pData);
			++pos;
		}
		return NPos;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::RFindFirstOf(const ConstString& codepoints, usize pos, usize count) const noexcept -> usize
	{
		const u8* pData = PrevCharPtr(m_data.Data() + m_data.Size());
		const u8* pCodepoints = codepoints.Data();
		const usize numCodepoints = codepoints.Length();

		if (count > m_length)
			count = m_length;
		if (pos >= m_length)
			pos = m_length - 1;

		while (pos && count)
		{
			const usize utf8Size = Unicode::GetUtf8Size(*pData);

			const u8* pCurCp = pCodepoints;
			for (usize i = 0; i < numCodepoints; ++i)
			{
				const usize cpSize = Unicode::GetUtf8Size(*pCurCp);
				if (Unicode::MatchChar(pData, pCurCp, Math::Min(utf8Size, cpSize)))
					return pos;
				pCurCp += cpSize;
			}

			pData = PrevCharPtr(pData);
			--pos;
			--count;
		}
		return 0;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::RFindFirstNotOf(const ConstString& codepoints, usize pos, usize count) const noexcept -> usize
	{
		const u8* pData = PrevCharPtr(m_data.Data() + m_data.Size());

		const u8* pCodepoints = codepoints.Data();
		const usize numCodepoints = codepoints.Length();

		if (count > m_length)
			count = m_length;
		if (pos >= m_length)
			pos = m_length - 1;

		while (pos && count)
		{
			const usize utf8Size = Unicode::GetUtf8Size(*pData);

			const u8* pCurCp = pCodepoints;
			bool found = false;
			for (usize i = 0; i < numCodepoints; ++i)
			{
				const usize cpSize = Unicode::GetUtf8Size(*pCurCp);
				if (Unicode::MatchChar(pData, pCurCp, Math::Min(utf8Size, cpSize)))
				{
					found = true;
					break;
				}
				pCurCp += cpSize;
			}
			if (!found)
				return pos;

			pData = PrevCharPtr(pData);
			--pos;
			--count;
		}
		return NPos;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Contains(UCodepoint codepoint) const noexcept -> bool
	{
		return Find(codepoint) != NPos;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Contains(const ConstString& str) const noexcept -> bool
	{
		return Find(str) != NPos;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::StartsWith(UCodepoint codepoint) const noexcept -> bool
	{
		if (m_length == 0)
			return false;

		const Unicode::Utf8Char c = Unicode::GetUtf8FromCp(codepoint);
		if (m_data.Size() < c.size)
			return false;

		return MemCmp(m_data.Data(), c.data, c.size) == 0;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::StartsWith(const ConstString& str) const noexcept -> bool
	{
		const usize strSize = str.DataSize();
		const usize size = m_data.Size();
		if (size < strSize)
			return false;
		return MemCmp(m_data.Data(), str.Data(), strSize) == 0;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::EndsWith(UCodepoint codepoint) const noexcept -> bool
	{
		if (m_length == 0)
			return false;

		const Unicode::Utf8Char c = Unicode::GetUtf8FromCp(codepoint);
		if (m_data.Size() < c.size)
			return false;

		const u8* pData = PrevCharPtr(m_data.Data() + m_data.Size());
		return MemCmp(pData, c.data, c.size) == 0;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::EndsWith(const ConstString& str) const noexcept -> bool
	{
		const usize strSize = str.DataSize();
		const usize size = m_data.Size();
		if (size < strSize)
			return false;
		return MemCmp(m_data.Data() + size - strSize, str.Data(), strSize) == 0;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::ToAscii() const noexcept -> InplaceDynArray<char, Cap>
	{
		InplaceDynArray<char, Cap> ascii;
		usize len = m_length;
		const u8* pData = m_data.Data();

		while (len)
		{
			u8 b = *pData;
			ascii.Add(char(b > 0x7F ? 0x80 : b));
			pData += Unicode::GetUtf8Size(*pData);
			--len;
		}

		ascii.Add(0);
		ascii.Pop();
		return ascii;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::ToUtf8() const noexcept -> InplaceDynArray<char8_t, Cap>
	{
		InplaceDynArray<char8_t, Cap> utf8;
		MemCpy(utf8.Data(), m_data.Data(), m_data.Size());

		utf8.Pop();
		return utf8;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::ToUtf16() const noexcept -> InplaceDynArray<char16_t, Cap * sizeof(char16_t)>
	{
		InplaceDynArray<char16_t, Cap * sizeof(char16_t)> utf16;
		usize len = m_length;
		const u8* pData = m_data.Data();

		while (len)
		{
			Unicode::Utf16Char c = Unicode::GetUtf16FromUtf8(pData);
			utf16.Add(c.data[0]);
			if (c.size > 1)
				utf16.Add(c.data[1]);

			pData += Unicode::GetUtf8Size(*pData);
			--len;
		}

		utf16.Add(0);
		utf16.Pop();
		return utf16;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::ToUtf32() const noexcept -> InplaceDynArray<char32_t, Cap * sizeof(char32_t)>
	{
		InplaceDynArray<char32_t, Cap * sizeof(char32_t)> utf32;
		usize len = m_length;
		const u8* pData = m_data.Data();

		while (len)
		{
			utf32.Add(char32_t(Unicode::GetCpFromUtf8(pData)));
			pData += Unicode::GetUtf8Size(*pData);
			--len;
		}

		utf32.Add(0);
		utf32.Pop();
		return utf32;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::ToCodepoints() const noexcept -> InplaceDynArray<UCodepoint, Cap * sizeof(UCodepoint)>
	{
		InplaceDynArray<UCodepoint, Cap * sizeof(UCodepoint)> codepoints;
		usize len = m_length;
		const u8* pData = m_data.Data();

		while (len)
		{
			codepoints.Add(Unicode::GetCpFromUtf8(pData));
			pData += Unicode::GetUtf8Size(*pData);
			--len;
		}

		codepoints.Add(0);
		codepoints.Pop();
		return codepoints;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Compare(const ConstString& str) const noexcept -> i8
	{
		const usize size = m_data.Size();
		const usize strSize = str.DataSize();

		i8 res = MemCmp(m_data.Data(), str.Data(), Math::Min(size, strSize));
		if (res != 0)
			return res;
		if (size == strSize)
			return 0;

		return size < strSize ? -1 : 1;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::IsWhitespace() const noexcept -> bool
	{
		usize len = m_length;
		const u8* pData = m_data.Data();

		while (len)
		{
			if (!Unicode::IsWhitespace(pData))
				return false;
			pData += Unicode::GetUtf8Size(*pData);
			--len;
		}
		return true;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::At(usize idx) const noexcept -> Optional<UCodepoint>
	{
		if (idx >= m_length)
			return NullOpt;
		return m_data.Data()[IndexAtCharPos(idx)];
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::operator[](usize idx) const noexcept -> UCodepoint
	{
		ASSERT(idx < m_length, "Index out of range");
		return m_data.Data()[IndexAtCharPos(idx)];
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Length() const noexcept -> usize
	{
		return m_length;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::IsEmpty() const noexcept -> bool
	{
		return m_length == 0;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Capacity() const noexcept -> usize
	{
		return m_data.Capacity();
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::DataSize() const noexcept -> usize
	{
		return m_data.Size();
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Data() noexcept -> u8*
	{
		return m_data.Data();
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Data() const noexcept -> const u8*
	{
		return m_data.Data();
	}
	
	template<usize Cap>
	constexpr auto ConstString<Cap>::Front() const noexcept -> UCodepoint
	{
		ASSERT(m_length, "Invalid when ConstString is empty");
		return Unicode::GetCpFromUtf8(m_data.Data());
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Back() const noexcept -> UCodepoint
	{
		ASSERT(m_length, "Invalid when ConstString is empty");
		const u8* pData = m_data.Data();
		usize end = PrevIdx(pData, m_data.Size());
		return Unicode::GetCpFromUtf8(pData + end);
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Begin() noexcept -> Iterator
	{
		return { this, 0 };
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::Begin() const noexcept -> ConstIterator
	{
		return { this, 0 };
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::End() noexcept -> Iterator
	{
		return { this, m_data.Size() };
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::End() const noexcept -> ConstIterator
	{
		return { this, m_data.Size() };
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::RBegin() noexcept -> Iterator
	{
		return End();
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::RBegin() const noexcept -> ConstIterator
	{
		return End();
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::REnd() noexcept -> Iterator
	{
		return Begin();
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::REnd() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::begin() noexcept -> Iterator
	{
		return Begin();
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::begin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::cbegin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::end() noexcept -> Iterator
	{
		return End();
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::end() const noexcept -> ConstIterator
	{
		return End();
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::cend() const noexcept -> ConstIterator
	{
		return End();
	}

	template<usize Cap>
	constexpr void ConstString<Cap>::AssignSubConstString(const ConstString& other, usize idx, usize pos, usize size, usize length) noexcept
	{
		if (size > other.DataSize() - idx)
			size = other.DataSize() - idx;
		if (length > other.Length() - pos)
			length = other.Length() - pos;
		if (size == 0)
			return;
		
		MemCpy(m_data.Data(), other.Data() + idx, size);
		NullTerminate();
		m_length = length;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::ReplaceInternal(usize idx, usize byteLength, usize length, Unicode::Utf8Char c, usize count) noexcept -> ConstString&
	{
		usize needed = count * c.size;
		if (needed > byteLength)
			m_data.Insert(m_data.IteratorAt(idx), needed - byteLength, 0);
		else if (needed < byteLength)
			m_data.Erase(m_data.IteratorAt(idx), byteLength - needed);

		if (byteLength < needed)
		{
			while (m_data.Size() - byteLength + needed + 1 > Cap)
			{
				--count;
				needed -= c.size;
			}
		}
		else
		{
			m_data.Erase(m_data.IteratorAt(idx), byteLength - needed);
		}

		u8* pData = m_data.Data() + idx;
		for (usize i = 0; i < count; ++i, pData += c.size)
			MemCpy(pData, c.data, c.size);

		m_length -= length;
		m_length += count;
		NullTerminate();
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::ReplaceInternal(usize idx, usize byteLength, usize length, const ConstString& str, usize strIdx, usize strByteEnd, usize strLength) noexcept -> ConstString&
	{
		usize otherSize = strByteEnd - strIdx;

		if (byteLength < otherSize)
		{
			const usize strBeginByte = str.IndexAtCharPos(strIdx);
			while (m_data.Size() - byteLength + otherSize + 1 > Cap)
			{
				--strLength;
				strByteEnd = str.IndexForOffset(strIdx, strLength) - strBeginByte;
				otherSize = strByteEnd - strIdx;
			}
		}
		else
		{
			m_data.Erase(m_data.IteratorAt(idx), byteLength - otherSize);
		}

		MemCpy(m_data.Data() + idx, str.m_data.Data() + strIdx, otherSize);
		m_length -= length;
		m_length += strLength;
		NullTerminate();
		return *this;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::FindInternal(UCodepoint codepoint, usize pos, usize idx, usize count) const noexcept -> Pair<usize, usize>
	{
		const Unicode::Utf8Char c = Unicode::GetUtf8FromCp(codepoint);
		const usize endByte = m_data.Size();

		if (count > m_length - pos)
			count = m_length - pos;

		const usize endPos = pos + count;
		const u8* pData = m_data.Data();
		while (idx < endByte && pos < endPos)
		{
			if (Unicode::MatchChar(pData + idx, c.data, c.size))
				return { pos, idx };

			idx += Unicode::GetUtf8Size(*(pData + idx));
			++pos;
		}
		return { NPos, endByte };
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::FindInternal(const ConstString& str, usize pos, usize idx, usize count) const noexcept -> Pair<usize, usize>
	{
		if (count > m_length - pos)
			count = m_length - pos;

		const usize endPos = pos + count - str.Length();
		if (endPos < pos)
			return { NPos, NPos };

		const usize endByte = m_data.Size();
		const u8* pData = m_data.Data();

		const u8* pOtherData = str.Data();
		const usize otherSize = str.DataSize();

		while (idx < endByte && pos < endPos)
		{
			bool found = true;
			const usize endIdx = idx + otherSize;
			for (usize i = idx, j = 0; found && i < endIdx; ++i, ++j)
			{
				if (pData[i] != pOtherData[j])
				{
					found = false;
					break;
				}
			}
			if (found)
				return { pos, idx };

			idx += Unicode::GetUtf8Size(*(pData + idx));
			++pos;
		}
		return { NPos, endByte };
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::FindWhitespaceInternal(usize pos, usize idx, usize count) const noexcept -> Pair<usize, usize>
	{
		if (count > m_length - pos)
			count = m_length - pos;

		const usize endByte = m_data.Size();
		const usize endPos = pos + count;
		const u8* pData = m_data.Data();
		while (idx < endByte&& pos < endPos)
		{
			if (Unicode::IsWhitespace(pData + idx))
				return { pos, idx };

			idx += Unicode::GetUtf8Size(*(pData + idx));
			++pos;
		}
		return { NPos, endByte };
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::IndexAtCharPos(usize pos) const noexcept -> usize
	{
		if (pos > m_length / 2)
		{
			const u8* pData = m_data.Data();
			usize end = PrevIdx(pData, m_data.Size());

			for (usize i = m_length - 1; i > pos; --i)
				end = PrevIdx(pData, end);
			return end;
		}
		else
		{
			return IndexForOffset(0, pos);
		}
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::IndexForOffset(usize startIdx, usize offset) const noexcept -> usize
	{
		usize idx = startIdx;
		const u8* pData = m_data.Data();
		for (usize i = 0; i < offset; ++i)
			idx += Unicode::GetUtf8Size(*(pData + idx));
		return idx;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::PrevIdx(const u8* pData, usize idx) const noexcept -> usize
	{
		--idx;
		while ((pData[idx] & 0xC0) == 0x80)
			--idx;
		return idx;
	}

	template<usize Cap>
	constexpr auto ConstString<Cap>::PrevCharPtr(const u8* pData) const noexcept -> const u8*
	{
		--pData;
		while ((*pData & 0xC0) == 0x80)
			--pData;
		return pData;
	}

	template<usize Cap>
	constexpr void ConstString<Cap>::NullTerminate() noexcept
	{
		const usize size = m_data.Size();
		m_data.Data()[size] = 0;
	}
}
