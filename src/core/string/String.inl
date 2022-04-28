#pragma once
#if __RESHARPER__
#include "String.h"
#endif

namespace Onca
{
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
		m_data.Reserve(il.size());
		Assign(il.begin(), il.end());
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

	template <CharacterType C>
	auto String::operator+(const C* str) const noexcept -> String
	{
		String copy{ *this };
		copy.Add(str);
		return copy;
	}

	template <CharacterType C>
	auto String::operator+=(const C* str) noexcept -> String&
	{
		return Add(str);
	}

	template <CharacterType C>
	void String::Assign(const C* str) noexcept
	{
		Assign(str, StrLen(str));
	}

	template <CharacterType C>
	void String::Assign(const C* str, usize length) noexcept
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
	void String::Assign(const InitializerList<T>& il) noexcept
	{
		m_data.Clear();
		m_data.Reserve(il.size());
		Assign(il.begin(), il.end());
	}

	template <ForwardIterator It> requires DereferencableToUnicode<It>
	void String::Assign(const It& begin, const It& end) noexcept
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
}

namespace Literals
{
	inline auto operator ""_s(const char* cstr, usize size) noexcept -> Onca::String
	{
		return Onca::String{ cstr, size };
	}

	inline auto operator ""_s(const char16_t* cstr, usize size) noexcept -> Onca::String
	{
		return Onca::String{ cstr, size };
	}

	inline auto operator ""_s(const char32_t* cstr, usize size) noexcept -> Onca::String
	{
		return Onca::String{ cstr, size };
	}
}