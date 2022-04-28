#pragma once
#ifdef __RESHARPER__
#include "StringId.h"
#endif

namespace Onca
{
	constexpr StringId::StringId() noexcept
		: m_id(0)
	{
	}

	constexpr StringId::StringId(u64 id) noexcept
		: m_id(id)
	{
	}

	inline StringId::StringId(const String& str) noexcept
	{
		m_id = Hash(reinterpret_cast<const char*>(str.Data()), str.DataSize());
	}
	
	constexpr StringId::StringId(const char* str) noexcept
		: StringId(str, StrLen(str))
	{
	}
	
	constexpr StringId::StringId(const char* str, usize len) noexcept
	{
		m_id = Hash(str, len);
	}

	template <usize Cap>
	constexpr StringId::StringId(const ConstString<Cap>& str) noexcept
	{
		constexpr Hashing::FVN1A_64 hasher;
		m_id = hasher(str.Data(), str.DataSize());
	}

	constexpr StringId::operator u64() const noexcept
	{
		return m_id;
	}

	constexpr auto StringId::Hash(const char* str, usize len) noexcept -> u64
	{
		u64 hash = Hashing::Detail::FNV1_64Offset;
		while (len--)
		{
			hash ^= u8(*str++);
			hash *= Hashing::Detail::FNV1_64Prime;
		}
		return hash;
	}

	inline auto Hash<StringId>::operator()(const StringId& t) const noexcept -> u64
	{
		return t;
	}
}

constexpr auto Literals::operator ""_sid(const char* cstr, usize size) noexcept -> Onca::StringId
{
	return Onca::StringId{ cstr, size };
}
