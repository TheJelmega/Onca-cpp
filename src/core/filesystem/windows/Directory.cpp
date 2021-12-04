#include "core/filesystem/Directory.h"
#if PLATFORM_WINDOWS

#include "core/platform/Platform.h"
#include "Utils.h"

namespace Core::FileSystem
{
	auto GetCurrentWorkingDirectory() noexcept -> Path
	{
		const u32 len = ::GetCurrentDirectoryW(0, nullptr);
		DynArray<char16_t> rawChars;
		rawChars.Resize(len, '\0');
		::GetCurrentDirectoryW(len, reinterpret_cast<LPWSTR>(rawChars.Data()));
		return Path{ String{ rawChars.Data(), len - 1 } }.ToGeneral();
	}

	auto SetCurrentWorkingDirectory(const Path& path) noexcept -> Error
	{
		const DynArray<char16_t> utf16 = ("\\\\?\\"_path + path).ToNative().GetString().ToUtf16();
		const bool res = ::SetCurrentDirectoryW(reinterpret_cast<LPCWSTR>(utf16.Data()));
		return res ? Error{} : TranslateFSError();
	}

	auto CreateDirectory(const Path& path) noexcept -> Error
	{
		const DynArray<char16_t> utf16 = ("\\\\?\\"_path + path).ToNative().GetString().ToUtf16();
		const bool res = ::CreateDirectoryW(reinterpret_cast<LPCWSTR>(utf16.Data()), nullptr);
		return res ? Error{} : TranslateFSError();
	}

	auto DeleteDirectory(const Path& path, bool recursively) noexcept -> Error
	{
		const DynArray<char16_t> utf16 = ("\\\\?\\"_path + path).ToNative().GetString().ToUtf16();
		if (recursively)
		{
			// TODO: recursively delete directory content
		}

		const bool res = ::RemoveDirectoryW(reinterpret_cast<LPCWSTR>(utf16.Data()));
		return res ? Error{} : TranslateFSError();
	}

	auto IsDirectory(const Path& path) noexcept -> bool
	{
		const DynArray<char16_t> utf16 = ("\\\\?\\"_path + path).ToNative().GetString().ToUtf16();
		const u32 attribs = ::GetFileAttributesW(reinterpret_cast<LPCWSTR>(utf16.Data()));
		return attribs != INVALID_FILE_ATTRIBUTES && (attribs & FILE_ATTRIBUTE_DIRECTORY);
	}

	auto GetLogicalDrives() noexcept -> DynArray<Path>
	{
		const u32 len = ::GetLogicalDriveStringsW(0, nullptr);
		DynArray<char16_t> names;
		names.Resize(len);
		::GetLogicalDriveStringsW(len, reinterpret_cast<LPWSTR>(names.Data()));

		DynArray<Path> drives;
		usize idx = 0;
		while (idx < len - 1)
		{
			String str{ &names[idx] };
			idx += str.Length() + 1;
			drives.EmplaceBack(str);
		}
		return drives;
	}
}

#endif