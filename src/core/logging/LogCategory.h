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

namespace LogCategories
{
	constexpr Core::LogCategory Core{ "Core" };
}
