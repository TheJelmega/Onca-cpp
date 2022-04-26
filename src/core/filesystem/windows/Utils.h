#pragma once
#include "core/MinInclude.h"

#if PLATFORM_WINDOWS

#include "core/platform/Platform.h"
#include "../Enums.h"

namespace Onca::FileSystem::Windows
{
	struct NativeIOEventData
	{
		OVERLAPPED overlapped;
	};

	/**
	 * Translate win32 file attribute to file attributes
	 * \param[in] winAttribs Win32 file attributes
	 * \return File Attributes
	 */
	auto TranslateToFileAttribs(u32 winAttribs) noexcept -> FileAttributes;
}

#endif