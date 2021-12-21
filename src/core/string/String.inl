#pragma once
#if __RESHARPER__
#include "String.h"
#endif

namespace Core
{
	inline String::Iterator::Iterator() noexcept
		: m_pData(nullptr)
		, m_idx(0)
	{
	}

	inline String::Iterator::Iterator(const Iterator& other) noexcept
		: m_pData(other.m_pData)
		, m_idx(other.m_idx)
	{
	}

	inline String::Iterator::Iterator(Iterator&& other) noexcept
		: m_pData(other.m_pData)
		, m_idx(other.m_idx)
	{
	}

	inline auto String::Iterator::operator=(const Iterator& other) noexcept -> Iterator&
	{
		m_pData = other.m_pData;
		m_idx = other.m_idx;
		return *this;
	}

	inline auto String::Iterator::operator=(Iterator&& other) noexcept -> Iterator&
	{
		m_pData = other.m_pData;
		m_idx = other.m_idx;
		return *this;
	}
	
	inline auto String::Iterator::operator*() const noexcept -> UCodepoint
	{
		return Unicode::GetCpFromUtf8(m_pData->Data() + m_idx);
	}

	inline auto String::Iterator::operator++() noexcept -> Iterator&
	{
		m_idx += Unicode::GetUtf8Size(m_pData->Data()[m_idx]);
		return *this;
	}
	
	inline auto String::Iterator::operator++(int) noexcept -> Iterator
	{
		Iterator it{ *this };
		operator++();
		return it;
	}
	
	inline auto String::Iterator::operator--() noexcept -> Iterator&
	{
		const u8* pData = m_pData->Data();
		--m_idx;
		while (m_idx && (pData[m_idx] & 0x80) == 0x80)
			--m_idx;
		return *this;
	}
	
	inline auto String::Iterator::operator--(int) noexcept -> Iterator
	{
		Iterator it{ *this };
		operator--();
		return it;
	}
	
	inline auto String::Iterator::operator+(usize count) const noexcept -> Iterator
	{
		Iterator it{ *this };
		for (usize i = 0; i < count; ++i)
			++it;
		return it;
	}

	inline auto String::Iterator::operator-(usize count) const noexcept -> Iterator
	{
		Iterator it{ *this };
		for (usize i = 0; i < count; ++i)
			--it;
		return it;
	}
	
	inline auto String::Iterator::operator-(const Iterator& other) const noexcept -> isize
	{
		ASSERT(*this >= other, "Cannot subtract iterator coming before this iterator");
		usize diff = 0;
		for (Iterator it{ *this }; it != other; ++it)
			++diff;
		return diff;
	}
	
	inline auto String::Iterator::operator+=(usize count) noexcept -> Iterator&
	{
		for (usize i = 0; i < count; ++i)
			operator++();
		return *this;
	}

	inline auto String::Iterator::operator-=(usize count) noexcept -> Iterator&
	{
		for (usize i = 0; i < count; ++i)
			operator--();
		return *this;
	}
	
	inline auto String::Iterator::operator==(const Iterator& other) const noexcept -> bool
	{
		return m_pData == other.m_pData && m_idx == other.m_idx;
	}

	inline auto String::Iterator::operator!=(const Iterator& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	inline auto String::Iterator::operator<=>(const Iterator& other) const noexcept -> std::partial_ordering
	{
		if (m_pData != other.m_pData)
			return std::partial_ordering::unordered;

		if (m_idx < other.m_idx) return std::partial_ordering::less;
		if (m_idx > other.m_idx) return std::partial_ordering::greater;
		return std::partial_ordering::equivalent;
	}

	inline auto String::Iterator::operator[](usize idx) const noexcept -> const UCodepoint&
	{
		return *(*this + idx);
	}

	inline String::Iterator::Iterator(const DynArray<u8>* pData, usize idx)
		: m_pData(pData)
		, m_idx(idx)
	{
	}

	inline String::String(Alloc::IAllocator& alloc) noexcept
		: m_data(alloc)
		, m_length(0)
	{
	}

	inline String::String(usize capacity, Alloc::IAllocator& alloc) noexcept
		: m_data(capacity, alloc)
		, m_length(0)
	{
	}

	inline String::String(UCodepoint codepoint, usize count, Alloc::IAllocator& alloc) noexcept
		: m_data(alloc)
		, m_length(0)
	{
		Assign(codepoint, count);
	}

	inline String::String(const String& other, usize pos, Alloc::IAllocator& alloc) noexcept
		: m_data(alloc)
		, m_length(0)
	{
		Assign(other, pos);
	}

	inline String::String(const String& other, usize pos, usize length) noexcept
		: m_data(*other.GetAllocator())
		, m_length(0)
	{
		Assign(other, pos, length);
	}

	inline String::String(const String& other, usize pos, usize length, Alloc::IAllocator& alloc) noexcept
		: m_data(alloc)
		, m_length(0)
	{
		Assign(other, pos, length);
	}

	template <CharacterType C>
	String::String(const C* str, Alloc::IAllocator& alloc) noexcept
		: m_data(alloc)
		, m_length(0)
	{
		Assign(str);
	}

	template <CharacterType C>
	String::String(const C* str, usize length, Alloc::IAllocator& alloc) noexcept
		: m_data(alloc)
		, m_length(0)
	{
		Assign(str, length);
	}

	template <ConvertableToUnicode T>
	String::String(const InitializerList<T>& il, Alloc::IAllocator& alloc) noexcept
		: m_data(alloc)
		, m_length(0)
	{
		Assign(il);
	}

	template <ForwardIterator It> requires DereferencableToUnicode<It>
	String::String(const It& begin, const It& end, Alloc::IAllocator& alloc) noexcept
		: m_data(alloc)
		, m_length(0)
	{
		Assign(begin, end);
	}

	template <CharacterType C>
	auto String::operator=(const C* str) noexcept -> String&
	{
		Assign(str);
		return *this;
	}

	template <ConvertableToUnicode T>
	auto String::operator=(const InitializerList<T>& il) noexcept -> String&
	{
		Assign(il);
		return *this;
	}

	inline String::String(const String& other) noexcept
		: m_data(other.m_data)
		, m_length(other.m_length)
	{
		NullTerminate();
	}

	inline String::String(const String& other, Alloc::IAllocator& alloc) noexcept
		: m_data(other.m_data, alloc)
		, m_length(other.m_length)
	{
		NullTerminate();
	}

	inline String::String(String&& other) noexcept
		: m_data(Move(other.m_data))
		, m_length(other.m_length)
	{
		other.m_length = 0;
	}

	inline String::String(String&& other, Alloc::IAllocator& alloc) noexcept
		: m_data(Move(other.m_data), alloc)
		, m_length(other.m_length)
	{
		NullTerminate();
		other.m_length = 0;
	}

	inline auto String::operator=(const String& other) noexcept -> String&
	{
		m_data = other.m_data;
		m_length = other.m_length;
		return *this;
	}

	inline auto String::operator=(String&& other) noexcept -> String&
	{
		m_data = Move(other.m_data);
		m_length = other.m_length;
		other.m_length = 0;
		return *this;
	}

	inline auto String::operator+(UCodepoint codepoint) const noexcept -> String
	{
		String copy{ *this };
		copy.Add(codepoint);
		return copy;
	}

	template <CharacterType C>
	auto String::operator+(const C* str) const noexcept -> String
	{
		String copy{ *this };
		copy.Add(str);
		return copy;
	}

	inline auto String::operator+(const String& other) const noexcept -> String
	{
		String copy{ *this };
		copy.Add(other);
		return copy;
	}

	inline auto String::operator+=(UCodepoint codepoint) noexcept -> String&
	{
		return Add(codepoint);
	}

	template <CharacterType C>
	auto String::operator+=(const C* str) noexcept -> String&
	{
		return Add(str);
	}

	inline auto String::operator+=(const String& other) noexcept -> String&
	{
		return Add(other);
	}

	inline auto String::operator<=>(const String& other) const noexcept -> std::strong_ordering
	{
		i8 res = Compare(other);
		if (res == 0) return std::strong_ordering::equal;
		if (res < 0) return std::strong_ordering::less;
		return std::strong_ordering::greater;
	}

	inline auto String::operator==(const String& other) const noexcept -> bool
	{
		if (m_data.Size() != other.DataSize())
			return false;

		return MemCmp(m_data.Data(), other.Data(), m_data.Size()) == 0;
	}

	inline auto String::operator!=(const String& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	inline auto String::Assign(UCodepoint codepoint, usize count) noexcept -> void
	{
		m_data.Clear();
		m_length = 0;
		Add(codepoint, count);
	}

	inline auto String::Assign(const String& other, usize pos, usize length) noexcept -> void
	{
		m_data.Clear();
		m_length = 0;
		Add(other, pos, length);
	}

	template <CharacterType C>
	auto String::Assign(const C* str) noexcept -> void
	{
		Assign(str, StrLen(str));
	}

	template <CharacterType C>
	auto String::Assign(const C* str, usize length) noexcept -> void
	{
		m_data.Clear();
		m_data.Reserve(length);
		for (usize i = 0; i < length; ++i)
		{
			const auto [c, toSkip] = Unicode::ToUtf8(str);
			str += toSkip;

			const usize size = m_data.Size();
			m_data.Resize(size + c.size);
			MemCpy(m_data.Data() + size, c.data, c.size);
		}
		NullTerminate();
		m_length = length;
	}

	template <ConvertableToUnicode T>
	auto String::Assign(const InitializerList<T>& il) noexcept -> void
	{
		m_data.Clear();
		m_data.Reserve(il.size());
		Assign(il.begin(), il.end());
	}

	template <ForwardIterator It> requires DereferencableToUnicode<It>
	auto String::Assign(const It& begin, const It& end) noexcept -> void
	{
		m_data.Clear();
		m_length = 0;
		for (It it = begin; it != end;)
		{
			Unicode::Utf8Char c;
			u64 toSkip;
			if constexpr (IsLValueReference<decltype(*it)>)
			{
				const auto [c_, toSkip_] = Unicode::ToUtf8(&*it);
				c = c_;
				toSkip = toSkip_;
			}
			else
			{
				decltype(*it) tmp = *it;
				const auto [c_, toSkip_] = Unicode::ToUtf8(&tmp);
				c = c_;
				toSkip = toSkip_;
			}
			for (usize i = 0; i < toSkip; ++i)
				++it;

			const usize size = m_data.Size();
			m_data.Resize(size + c.size);
			MemCpy(m_data.Data() + size, c.data, c.size);
			++m_length;
		}
		NullTerminate();
	}


	inline auto String::Reserve(usize capacity) noexcept -> void
	{
		m_data.Reserve(capacity);
	}

	inline auto String::Resize(usize newSize, UCodepoint codepoint) noexcept -> void
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

	inline auto String::ShrinkToFit() noexcept -> void
	{
		m_data.Add(0);
		m_data.ShrinkToFit();
		m_data.Pop();
	}
	
	inline auto String::Add(UCodepoint codepoint, usize count) noexcept -> String&
	{
		const Unicode::Utf8Char c = Unicode::GetUtf8FromCp(codepoint);
		const usize size = m_data.Size();
		m_data.Resize(m_data.Size() + count * c.size);

		u8* pData = m_data.Data() + size;
		for (usize i = 0; i < count; ++i, pData += c.size)
			MemCpy(pData, c.data, c.size);

		m_length += count;
		NullTerminate();
		return *this;
	}

	inline auto String::Add(const String& other, usize pos, usize length) noexcept -> String&
	{
		if (length > other.Length() - pos)
			length = other.Length() - pos;
		if (length == 0 || pos > other.Length())
			return *this;

		const usize startIdx = other.IndexAtCharPos(pos);
		const usize endIdx = other.IndexForOffset(startIdx, length);
		const usize byteLen = endIdx - startIdx;

		const usize size = m_data.Size();
		m_data.Resize(size + byteLen);
		MemCpy(m_data.Data() + size, other.Data() + startIdx, byteLen);

		m_length += length;
		NullTerminate();
		return *this;
	}

	inline auto String::PadLeft(usize count, UCodepoint codepoint) noexcept -> String&
	{
		return Insert(0, codepoint, count);
	}

	inline auto String::PadRight(usize count, UCodepoint codepoint) noexcept -> String&
	{
		return Add(codepoint, count);
	}

	inline auto String::Erase(usize pos, usize count) noexcept -> String&
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

	inline auto String::TrimLeft(UCodepoint codepoint) noexcept -> String&
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

	inline auto String::TrimLeft() noexcept -> String&
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

	inline auto String::TrimRight(UCodepoint codepoint) noexcept -> String&
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

	inline auto String::TrimRight() noexcept -> String&
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

	inline auto String::Trim(UCodepoint codepoint) noexcept -> String&
	{
		if (m_length == 0)
			return *this;

		TrimLeft(codepoint);
		TrimRight(codepoint);
		return *this;
	}

	inline auto String::Trim() noexcept -> String&
	{
		if (m_length == 0)
			return *this;

		TrimLeft();
		TrimRight();
		return *this;
	}
	
	inline auto String::Insert(usize pos, const String& str, usize strPos, usize strLength) noexcept -> String&
	{
		ASSERT(pos == 0 || pos < m_length, "'pos' needs to point to a character inside the string");
		ASSERT(strPos < str.Length(), "'strPos' needs to point to a character inside 'str'");
		if (strLength > str.Length() - strPos)
			strLength = str.Length() - strPos;

		const usize idx = IndexAtCharPos(pos);
		const usize strIdx = str.IndexAtCharPos(strPos);
		const usize strEnd = str.IndexForOffset(strIdx, strLength);
		const usize otherSize = strEnd - strIdx;

		m_data.Insert(m_data.IteratorAt(idx), otherSize, 0);
		MemCpy(m_data.Data() + idx, str.m_data.Data() + strIdx, otherSize);
		m_length += strLength;
		NullTerminate();
		return *this;
	}
	
	inline auto String::Insert(usize pos, UCodepoint codepoint, usize count) noexcept -> String&
	{
		ASSERT(pos == 0 || pos < m_length, "'pos' needs to point to a character inside the string");
		const usize idx = IndexAtCharPos(pos);
		const Unicode::Utf8Char c = Unicode::GetUtf8FromCp(codepoint);
		m_data.Insert(m_data.IteratorAt(idx), count * c.size, 0);

		u8* pData = m_data.Data() + idx;
		for (usize i = 0; i < count; ++i, pData += c.size)
			MemCpy(pData, c.data, c.size);

		m_length += count;
		NullTerminate();
		return *this;
	}
	
	inline auto String::Replace(usize pos, usize length, const String& str, usize strPos, usize strLength) noexcept -> String&
	{
		ASSERT(pos == 0 || pos < m_length, "'pos' needs to point to a character inside the string");
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
	
	inline auto String::Replace(usize pos, usize length, UCodepoint codepoint, usize count) noexcept -> String&
	{
		ASSERT(pos == 0 || pos < m_length, "'pos' needs to point to a character inside the string");
		if (length > m_length - pos)
			length = m_length - pos;

		const usize idx = IndexAtCharPos(pos);
		const usize endIdx = IndexForOffset(idx, length);
		const usize byteLength = endIdx - idx;
		const Unicode::Utf8Char c = Unicode::GetUtf8FromCp(codepoint);
		return ReplaceInternal(idx, byteLength, length, c, count);
	}

	inline auto String::Replace(UCodepoint toReplace, UCodepoint replaceWith) noexcept -> String&
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

	inline auto String::Replace(UCodepoint toReplace, const String& replaceWith) noexcept -> String&
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

	inline auto String::Replace(const String& toReplace, UCodepoint replaceWith) noexcept -> String&
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

	inline auto String::Replace(const String& toReplace, const String& replaceWith) noexcept -> String&
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

	inline auto String::ToUpper() noexcept -> String&
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
			MemCpy(pData + i, upper.data, upper.size);
			i += upper.size;
		}
		NullTerminate();
		return *this;
	}

	inline auto String::AsUpper() const noexcept -> String
	{
		String res{ m_data.Capacity(), *GetAllocator() };
		const u8* pSourceData = m_data.Data();
		u8* pData = res.Data();
		for (usize i = 0, size = m_data.Size(); i < size;)
		{
			const usize utf8size = Unicode::GetUtf8Size(*pSourceData);
			const Unicode::Utf8Char upper = Unicode::ToUpper(pSourceData);

			res.m_data.Resize(res.DataSize() + upper.size);
			pData = res.Data();

			MemCpy(pData + i, upper.data, upper.size);
			i += upper.size;
			pSourceData += utf8size;
		}

		res.m_length = m_length;
		res.NullTerminate();
		return res;
	}

	inline auto String::ToLower() noexcept -> String&
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
			MemCpy(pData + i, upper.data, upper.size);
			i += upper.size;
		}
		NullTerminate();
		return *this;
	}

	inline auto String::AsLower() const noexcept -> String
	{
		String res{ m_data.Capacity(), *GetAllocator() };
		const u8* pSourceData = m_data.Data();
		u8* pData = res.Data();
		for (usize i = 0, size = m_data.Size(); i < size;)
		{
			const usize utf8size = Unicode::GetUtf8Size(*pSourceData);
			const Unicode::Utf8Char upper = Unicode::ToLower(pSourceData);

			res.m_data.Resize(res.DataSize() + upper.size);
			pData = res.Data();

			MemCpy(pData + i, upper.data, upper.size);
			i += upper.size;
			pSourceData += utf8size;
		}

		res.m_length = m_length;
		res.NullTerminate();
		return res;
	}

	inline auto String::SubString(usize pos, usize length) const noexcept -> String
	{
		return String{ *this, pos, length, *GetAllocator() };
	}

	inline auto String::Split(UCodepoint delimiter, usize max, StringSplitOptions options) noexcept -> DynArray<String>
	{
		usize curPos = 0;
		usize curIdx = 0;

		const usize strSize = m_data.Size();

		Alloc::IAllocator& alloc = *GetAllocator();
		DynArray<String> res{ alloc };
		while (curIdx < strSize && max > 1)
		{
			auto [pos, idx] = FindInternal(delimiter, curPos, curIdx, NPos);
			usize size = idx - curIdx;
			usize len = pos - curPos;

			if (size || !(options & StringSplitOption::RemoveEmpty))
			{
				String subStr{ alloc };
				subStr.AssignSubstring(*this, curIdx, curPos, size, len);
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
			res.EmplaceBack(*GetAllocator());
			res.Back().AssignSubstring(*this, curIdx, curPos, strSize - curIdx, m_length - curPos);
			res.Back().NullTerminate();
		}
		return res;
	}

	inline auto String::Split(const String& delimiter, usize max, StringSplitOptions options) const noexcept -> DynArray<String>
	{
		usize curPos = 0;
		usize curIdx = 0;

		const usize strSize = m_data.Size();
		const usize delimiterSize = delimiter.DataSize();
		const usize delimiterLen = delimiter.Length();

		Alloc::IAllocator& alloc = *GetAllocator();
		DynArray<String> res{ alloc };
		while (curIdx < strSize && max > 1)
		{
			auto [pos, idx] = FindInternal(delimiter, curPos, curIdx, NPos);
			usize size = idx - curIdx;
			usize len = pos - curPos;

			if (size || !(options & StringSplitOption::RemoveEmpty))
			{
				String subStr{ alloc };
				subStr.AssignSubstring(*this, curIdx, curPos, size, len);
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
			res.EmplaceBack(*GetAllocator());
			res.Back().AssignSubstring(*this, curIdx, curPos, strSize - curIdx, m_length - curPos);
			res.Back().NullTerminate();
		}
		return res;
	}

	inline auto String::SplitWhitespace(usize max, StringSplitOptions options) const noexcept -> DynArray<String>
	{
		usize curPos = 0;
		usize curIdx = 0;

		const usize strSize = m_data.Size();

		Alloc::IAllocator& alloc = *GetAllocator();
		DynArray<String> res{ alloc };
		while (curIdx < strSize && max > 1)
		{
			auto [pos, idx] = FindWhitespaceInternal(curPos, curIdx, NPos);
			usize size = idx - curIdx;
			usize len = pos - curPos;

			if (size || !(options & StringSplitOption::RemoveEmpty))
			{
				String subStr{ alloc };
				subStr.AssignSubstring(*this, curIdx, curPos, size, len);
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
			res.EmplaceBack(*GetAllocator());
			res.Back().AssignSubstring(*this, curIdx, curPos, strSize - curIdx, m_length - curPos);
			res.Back().NullTerminate();
		}
		return res;
	}

	inline auto String::Find(UCodepoint codepoint, usize pos, usize count) const noexcept -> usize
	{
		if (pos >= m_length)
			return NPos;
		
		const usize startIdx = IndexAtCharPos(pos);
		auto [ret, _] = FindInternal(codepoint, pos, startIdx, count);
		return ret;
	}

	inline auto String::Find(const String& str, usize pos, usize count) const noexcept -> usize
	{
		if (pos >= m_length)
			return NPos;
		if (str.Length() > m_length)
			return NPos;

		const usize startIdx = IndexAtCharPos(pos);
		auto [ret, _] = FindInternal(str, pos, startIdx, count);
		return ret;
	}

	inline auto String::RFind(UCodepoint codepoint, usize pos, usize count) const noexcept -> usize
	{
		if (pos >= m_length)
			pos = m_length - 1;
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

	inline auto String::RFind(const String& str, usize pos, usize count) const noexcept -> usize
	{
		if (pos >= m_length)
			pos = m_length - 1;
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

	inline auto String::FindFirstOf(const String& codepoints, usize pos, usize count) const noexcept -> usize
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

	inline auto String::FindFirstNotOf(const String& codepoints, usize pos, usize count) const noexcept -> usize
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

	inline auto String::RFindFirstOf(const String& codepoints, usize pos, usize count) const noexcept -> usize
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

	inline auto String::RFindFirstNotOf(const String& codepoints, usize pos, usize count) const noexcept -> usize
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

	inline auto String::Contains(UCodepoint codepoint) const noexcept -> bool
	{
		return Find(codepoint) != NPos;
	}

	inline auto String::Contains(const String& str) const noexcept -> bool
	{
		return Find(str) != NPos;
	}

	inline auto String::StartsWith(UCodepoint codepoint) const noexcept -> bool
	{
		if (m_length == 0)
			return false;

		const Unicode::Utf8Char c = Unicode::GetUtf8FromCp(codepoint);
		if (m_data.Size() < c.size)
			return false;

		return MemCmp(m_data.Data(), c.data, c.size) == 0;
	}

	inline auto String::StartsWith(const String& str) const noexcept -> bool
	{
		const usize strSize = str.DataSize();
		const usize size = m_data.Size();
		if (size < strSize)
			return false;
		return MemCmp(m_data.Data(), str.Data(), strSize) == 0;
	}

	inline auto String::EndsWith(UCodepoint codepoint) const noexcept -> bool
	{
		if (m_length == 0)
			return false;

		const Unicode::Utf8Char c = Unicode::GetUtf8FromCp(codepoint);
		if (m_data.Size() < c.size)
			return false;

		const u8* pData = PrevCharPtr(m_data.Data() + m_data.Size());
		return MemCmp(pData, c.data, c.size) == 0;
	}

	inline auto String::EndsWith(const String& str) const noexcept -> bool
	{
		const usize strSize = str.DataSize();
		const usize size = m_data.Size();
		if (size < strSize)
			return false;
		return MemCmp(m_data.Data() + size - strSize, str.Data(), strSize) == 0;
	}

	inline auto String::ToAscii() const noexcept -> DynArray<char>
	{
		DynArray<char> ascii{ *GetAllocator() };
		ascii.Reserve(m_length);

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

	inline auto String::ToUtf8() const noexcept -> DynArray<char8_t>
	{
		DynArray<char8_t> utf8{ *GetAllocator() };
		utf8.Resize(m_data.Size() + 1);
		MemCpy(utf8.Data(), m_data.Data(), m_data.Size());

		utf8.Pop();
		return utf8;
	}

	inline auto String::ToUtf16() const noexcept -> DynArray<char16_t>
	{
		DynArray<char16_t> utf16{ *GetAllocator() };
		utf16.Reserve(m_length);

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

	inline auto String::ToUtf32() const noexcept -> DynArray<char32_t>
	{
		DynArray<char32_t> utf32{ *GetAllocator() };
		utf32.Reserve(m_length);

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

	inline auto String::ToCodepoints() const noexcept -> DynArray<UCodepoint>
	{
		DynArray<UCodepoint> codepoints{ *GetAllocator() };
		codepoints.Reserve(m_length);

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

	inline auto String::Compare(const String& str) const noexcept -> i8
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

	inline auto String::IsWhitespace() const noexcept -> bool
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

	inline auto String::At(usize idx) const noexcept -> Optional<UCodepoint>
	{
		if (idx >= m_length)
			return NullOpt;
		return m_data.Data()[IndexAtCharPos(idx)];
	}

	inline auto String::operator[](usize idx) const noexcept -> UCodepoint
	{
		ASSERT(idx < m_length, "Index out of range"); 
		return m_data.Data()[IndexAtCharPos(idx)];
	}

	inline auto String::Length() const noexcept -> usize
	{
		return m_length;
	}

	inline auto String::IsEmpty() const noexcept -> bool
	{
		return m_length == 0;
	}

	inline auto String::Capacity() const noexcept -> usize
	{
		return m_data.Capacity();
	}

	inline auto String::DataSize() const noexcept -> usize
	{
		return m_data.Size();
	}

	inline auto String::Data() noexcept -> u8*
	{
		return m_data.Data();
	}

	inline auto String::Data() const noexcept -> const u8*
	{
		return m_data.Data();
	}

	inline auto String::GetAllocator() const noexcept -> Alloc::IAllocator*
	{
		return m_data.GetAllocator();
	}

	inline auto String::Front() const noexcept -> UCodepoint
	{
		ASSERT(m_length, "Invalid when String is empty");
		return Unicode::GetCpFromUtf8(m_data.Data());
	}

	inline auto String::Back() const noexcept -> UCodepoint
	{
		ASSERT(m_length, "Invalid when String is empty");
		const u8* pData = m_data.Data();
		usize end = PrevIdx(pData, m_data.Size());
		return Unicode::GetCpFromUtf8(pData + end);
	}

	inline auto String::Begin() noexcept -> Iterator
	{
		return { &m_data, 0 };
	}

	inline auto String::Begin() const noexcept -> ConstIterator
	{
		return { &m_data, 0 };
	}

	inline auto String::End() noexcept -> Iterator
	{
		return { &m_data, m_data.Size() };
	}

	inline auto String::End() const noexcept -> ConstIterator
	{
		return { &m_data, m_data.Size() };
	}

	inline auto String::RBegin() noexcept -> Iterator
	{
		return End();
	}

	inline auto String::RBegin() const noexcept -> ConstIterator
	{
		return End();
	}

	inline auto String::REnd() noexcept -> Iterator
	{
		return Begin();
	}

	inline auto String::REnd() const noexcept -> ConstIterator
	{
		return Begin();
	}

	inline auto String::begin() noexcept -> Iterator
	{
		return Begin();
	}

	inline auto String::begin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	inline auto String::cbegin() const noexcept -> ConstIterator
	{
		return Begin();
	}

	inline auto String::end() noexcept -> Iterator
	{
		return End();
	}

	inline auto String::end() const noexcept -> ConstIterator
	{
		return End();
	}

	inline auto String::cend() const noexcept -> ConstIterator
	{
		return End();
	}

	inline auto String::AssignSubstring(const String& other, usize idx, usize pos, usize size, usize length) noexcept -> void
	{
		if (size > other.DataSize() - idx)
			size = other.DataSize() - idx;
		if (length > other.Length() - pos)
			length = other.Length() - pos;
		if (size == 0)
			return;
		
		m_data.Resize(size);
		MemCpy(m_data.Data(), other.Data() + idx, size);
		NullTerminate();
		m_length = length;
	}

	inline auto String::ReplaceInternal(usize idx, usize byteLength, usize length, Unicode::Utf8Char c, usize count) noexcept -> String&
	{
		const usize needed = count * c.size;
		if (needed > byteLength)
			m_data.Insert(m_data.IteratorAt(idx), needed - byteLength, 0);
		else if (needed < byteLength)
			m_data.Erase(m_data.IteratorAt(idx), byteLength - needed);

		u8* pData = m_data.Data() + idx;
		for (usize i = 0; i < count; ++i, pData += c.size)
			MemCpy(pData, c.data, c.size);

		m_length -= length;
		m_length += count;
		NullTerminate();
		return *this;
	}

	inline auto String::ReplaceInternal(usize idx, usize byteLength, usize length, const String& str, usize strIdx, usize strByteLength, usize strLength) noexcept -> String&
	{
		const usize otherSize = strByteLength - strIdx;

		if (byteLength < otherSize)
			m_data.Insert(m_data.IteratorAt(idx), otherSize - byteLength, 0);
		else if (byteLength > otherSize)
			m_data.Erase(m_data.IteratorAt(idx), byteLength - otherSize);

		MemCpy(m_data.Data() + idx, str.m_data.Data() + strIdx, otherSize);
		m_length -= length;
		m_length += strLength;
		NullTerminate();
		return *this;
	}

	inline auto String::FindInternal(UCodepoint codepoint, usize pos, usize idx, usize count) const noexcept -> Pair<usize, usize>
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

	inline auto String::FindInternal(const String& str, usize pos, usize idx, usize count) const noexcept -> Pair<usize, usize>
	{
		if (count > m_length - pos)
			count = m_length - pos;

		const usize endPos = pos + count - str.Length() + 1;
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

	inline auto String::FindWhitespaceInternal(usize pos, usize idx, usize count) const noexcept -> Pair<usize, usize>
	{
		if (count > m_length - pos)
			count = m_length - pos;

		const usize endByte = m_data.Size();
		const usize endPos = pos + count;
		const u8* pData = m_data.Data();
		while (idx < endByte && pos < endPos)
		{
			if (Unicode::IsWhitespace(pData + idx))
				return { pos, idx };

			idx += Unicode::GetUtf8Size(*(pData + idx));
			++pos;
		}
		return { NPos, endByte };
	}

	inline auto String::IndexAtCharPos(usize pos) const noexcept -> usize
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

	inline auto String::IndexForOffset(usize startIdx, usize offset) const noexcept -> usize
	{
		usize idx = startIdx;
		const u8* pData = m_data.Data();
		for (usize i = 0; i < offset; ++i)
			idx += Unicode::GetUtf8Size(*(pData + idx));
		return idx;
	}

	inline auto String::PrevIdx(const u8* pData, usize idx) const noexcept -> usize
	{
		--idx;
		while ((pData[idx] & 0xC0) == 0x80)
			--idx;
		return idx;
	}

	inline auto String::PrevCharPtr(const u8* pData) const noexcept -> const u8*
	{
		--pData;
		while ((*pData & 0xC0) == 0x80)
			--pData;
		return pData;
	}

	inline auto String::NullTerminate() noexcept -> void
	{
		const usize size = m_data.Size();
		m_data.Reserve(size + 1);
		m_data.Data()[size] = 0;
	}
}

inline namespace Literals
{
	inline auto operator ""_s(const char* cstr, usize size) noexcept -> Core::String
	{
		return Core::String{ cstr, size };
	}

	inline auto operator ""_s(const char16_t* cstr, usize size) noexcept -> Core::String
	{
		return Core::String{ cstr, size };
	}

	inline auto operator ""_s(const char32_t* cstr, usize size) noexcept -> Core::String
	{
		return Core::String{ cstr, size };
	}
}
