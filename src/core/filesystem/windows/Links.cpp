#include "../Links.h"

#if PLATFORM_WINDOWS
#include "core/platform/Platform.h"
#include "../Entry.h"

namespace Core::FileSystem
{
	auto CreateSymLink(const Path& path, const Path& target) noexcept -> Error
	{
		const DynArray<char16_t> utf16Path = ("\\\\?\\"_path + path).ToNative().GetString().ToUtf16();
		const DynArray<char16_t> utf16Target = ("\\\\?\\"_path + target).ToNative().GetString().ToUtf16();

		const u32 flags = ::GetFileAttributesW(reinterpret_cast<LPCWSTR>(utf16Target.Data()));
		if (flags == INVALID_FILE_ATTRIBUTES)
			return Error{ ErrorCode::InvalidPath };

		const u32 createFlags = flags & FILE_ATTRIBUTE_DIRECTORY ? SYMBOLIC_LINK_FLAG_DIRECTORY : 0;
		const bool res = ::CreateSymbolicLinkW(reinterpret_cast<LPCWSTR>(utf16Path.Data()), reinterpret_cast<LPCWSTR>(utf16Target.Data()), createFlags);
		return res ? Error{} : TranslateFSError();
	}

	auto CreateHardLink(const Path& path, const Path& target) noexcept -> Error
	{
		const DynArray<char16_t> utf16Path = ("\\\\?\\"_path + path).ToNative().GetString().ToUtf16();
		const DynArray<char16_t> utf16Target = ("\\\\?\\"_path + target).ToNative().GetString().ToUtf16();

		const u32 flags = ::GetFileAttributesW(reinterpret_cast<LPCWSTR>(utf16Target.Data()));

		if (flags == INVALID_FILE_ATTRIBUTES)
			return Error{ ErrorCode::InvalidPath };
		if (flags & FILE_ATTRIBUTE_DIRECTORY)
			return Error{ ErrorCode::ExpectedFile };

		const bool res = ::CreateHardLinkW(reinterpret_cast<LPCWSTR>(utf16Path.Data()), reinterpret_cast<LPCWSTR>(utf16Target.Data()), nullptr);
		return res ? Error{} : TranslateFSError();
	}
}

#endif
