#include "Utils.h"
#if PLATFORM_WINDOWS

namespace Onca::FileSystem::Windows
{
	auto TranslateToFileAttribs(u32 winAttribs) noexcept -> FileAttributes
	{
		// FIleAttributes match the win32 attributes, so we jus mask out the once that aren't supported
		return (FileAttributes)(winAttribs & u32(FileAttribute::Mask));
	}
}

#endif