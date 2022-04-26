#include "../Debugger.h"
#if PLATFORM_WINDOWS
#include "Win.h"

namespace Core::Debugger
{
	auto IsAttached() noexcept -> bool
	{
		return ::IsDebuggerPresent();
	}

	void OutputDebugString(const String& str) noexcept
	{
		DynArray<char16_t> utf16 = str.ToUtf16();
		::OutputDebugStringW(reinterpret_cast<LPCWSTR>(utf16.Data()));
	}
}

#endif