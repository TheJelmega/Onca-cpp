#pragma once
#ifdef __RESHARPER__
#include "StringId.h"
#endif

namespace Core
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
		static Hashing::FVN1A_64 hasher;
		m_id = hasher(str.Data(), str.DataSize());
	}

	template <usize Cap>
	constexpr StringId::StringId(const ConstString<Cap>& str) noexcept
	{
		static Hashing::FVN1A_64 hasher;
		m_id = hasher(str.Data(), str.DataSize());
	}

	constexpr StringId::operator u64() const noexcept
	{
		return m_id;
	}
}
