#include "../Links.h"

#if PLATFORM_WINDOWS
#include "core/platform/Platform.h"
#include "../Entry.h"

namespace Onca::FileSystem
{
	auto CreateSymLink(const Path& path, const Path& target) noexcept -> SystemError
	{
		const DynArray<char16_t> utf16Path = ("\\\\?\\"_path + path.AsAbsolute()).ToNative().GetString().ToUtf16();
		const DynArray<char16_t> utf16Target = ("\\\\?\\"_path + target.AsAbsolute()).ToNative().GetString().ToUtf16();

		const u32 flags = ::GetFileAttributesW(reinterpret_cast<LPCWSTR>(utf16Target.Data()));
		if (flags == INVALID_FILE_ATTRIBUTES)
			return SystemErrorCode::InvalidPath;

		const u32 createFlags = flags & FILE_ATTRIBUTE_DIRECTORY ? SYMBOLIC_LINK_FLAG_DIRECTORY : 0;
		const bool res = ::CreateSymbolicLinkW(reinterpret_cast<LPCWSTR>(utf16Path.Data()), reinterpret_cast<LPCWSTR>(utf16Target.Data()), createFlags);
		return res ? SystemError{} : TranslateSystemError();
	}

	auto CreateHardLink(const Path& path, const Path& target) noexcept -> SystemError
	{
		const DynArray<char16_t> utf16Path = ("\\\\?\\"_path + path.AsAbsolute()).ToNative().GetString().ToUtf16();
		const DynArray<char16_t> utf16Target = ("\\\\?\\"_path + target.AsAbsolute()).ToNative().GetString().ToUtf16();

		const u32 flags = ::GetFileAttributesW(reinterpret_cast<LPCWSTR>(utf16Target.Data()));

		if (flags == INVALID_FILE_ATTRIBUTES)
			return SystemErrorCode::InvalidPath;
		if (flags & FILE_ATTRIBUTE_DIRECTORY)
			return SystemErrorCode::ExpectedFile;

		const bool res = ::CreateHardLinkW(reinterpret_cast<LPCWSTR>(utf16Path.Data()), reinterpret_cast<LPCWSTR>(utf16Target.Data()), nullptr);
		return res ? SystemError{} : TranslateSystemError();
	}
}

#endif
