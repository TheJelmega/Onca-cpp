#pragma once
#include "core/MinInclude.h"
#include "core/string/Include.h"

namespace Core
{
	struct LogCategory
	{
		constexpr LogCategory(const ConstString<64>& name) noexcept
			: name(name)
		{}

		ConstString<64> name;
	};

	
}

#define DEFINE_LOG_CATEGORY(name) constexpr Core::LogCategory name{ #name };

namespace LogCategories
{
	/**
	 * Logging category for core library
	 */
	DEFINE_LOG_CATEGORY(CORE);
}
