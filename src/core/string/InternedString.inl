#pragma once
#if __RESHARPER__
#include "InternedString.h"
#endif


namespace Onca
{
	inline InternedString::InternedString() noexcept
	{
	}

	inline InternedString::InternedString(const String& string) noexcept
		: m_id(GetInternedStringManager().AddString(string))
	{
	}

	inline auto InternedString::operator=(const String& string) noexcept -> InternedString&
	{
		m_id = GetInternedStringManager().AddString(string);
		return *this;
	}

	inline auto InternedString::operator==(const InternedString& other) const noexcept -> bool
	{
		return m_id == other.m_id;
	}

	inline auto InternedString::Get() const noexcept -> const String&
	{
		return GetInternedStringManager().GetString(m_id);
	}

	inline InternedString::operator const String&() noexcept
	{
		return GetInternedStringManager().GetString(m_id);
	}

	inline InternedStringManager::InternedStringManager() noexcept
	{
	}
	
	inline void InternedStringManager::Shutdown() noexcept
	{
		m_strings.Clear(true);
	}

	inline auto InternedStringManager::AddString(const String& str) noexcept -> StringId
	{
		StringId id{ str };
		m_strings.TryInsert(id, str);
		return id;
	}

	inline auto InternedStringManager::GetString(StringId id) const noexcept -> const String&
	{
		auto it = m_strings.Find(id);
		if (it != m_strings.End())
			it->second;

		static String empty;
		return empty;
	}

	inline auto GetInternedStringManager() noexcept -> InternedStringManager&
	{
		static InternedStringManager manager;
		return manager;
	}
}
