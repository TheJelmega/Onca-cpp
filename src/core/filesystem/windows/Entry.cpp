#include "../Entry.h"
#if PLATFORM_WINDOWS

#include "core/platform/Platform.h"
#include "Utils.h"

namespace Core::FileSystem
{
	auto EnumerateFiles(Delegate<void(const Entry&)>& del, const Path& path, bool recursive) noexcept -> SystemError
	{
		const EnumerationOptions options{ .recurseSubDirs = recursive, .onlyVisitFiles = true };
		return EnumerateFileSystem(del, path, ""_path, options);
	}

	auto EnumerateDirectories(Delegate<void(const Entry&)>& del, const Path& path, bool recursive) noexcept -> SystemError
	{
		const EnumerationOptions options{ .recurseSubDirs = recursive, .onlyVisitDirs = true };
		return EnumerateFileSystem(del, path, ""_path, options);
	}

	auto EnumerateFileSystem(Delegate<void(const Entry&)>& del, const Path& path, bool recursive) noexcept -> SystemError
	{ 
		const EnumerationOptions options{ .recurseSubDirs = recursive };
		return EnumerateFileSystem(del, path, ""_path, options);
	}

	auto EnumerateFileSystem(Delegate<void(const Entry&)>& del, const Path& path, const Path& pattern, EnumerationOptions options) noexcept -> SystemError
	{
		Path searchPath = "\\\\?\\"_path + path;
		if (!pattern.IsEmpty())
			searchPath /= pattern;
		else
			searchPath /= "*"_path;

		const DynArray<char16_t> utf16 = searchPath.AsAbsolute().ToNative().GetString().ToUtf16();

		WIN32_FIND_DATAW findData;
		const HANDLE handle = ::FindFirstFileExW
		(
			reinterpret_cast<LPCWSTR>(utf16.Data()),
			FindExInfoBasic, // Ignore short name
			&findData,
			FindExSearchNameMatch,
			nullptr,
			options.matchCase ? FIND_FIRST_EX_CASE_SENSITIVE : 0
		);
		if (handle == INVALID_HANDLE_VALUE)
			return TranslateSystemError();

		while (true)
		{
			bool visit = true;

			FileAttributes attribs = Windows::TranslateToFileAttribs(findData.dwFileAttributes);
			if (attribs & options.toSkip)
				visit = false;

			if (visit && options.ignoreInaccessible)
			{
				// Easiest way to check this on windows is by trying to open the file and checking if it succeeds or gives an error
				const HANDLE fileHandle = ::CreateFileW
				(
					findData.cFileName,
					GENERIC_READ, // Should be enough to tell if the file is accessible
					FILE_SHARE_READ, // Use read share, just in case it's already opened, but shared
					nullptr,
					OPEN_EXISTING,
					0,
					nullptr
				);

				if (fileHandle != INVALID_HANDLE_VALUE)
					::CloseHandle(fileHandle);
				else
					visit = false;
			}

			bool isSpecialDir = false;
			const String fileName{ reinterpret_cast<const char16_t*>(findData.cFileName) };
			if (visit)
			{
				isSpecialDir = fileName == "." || fileName == "..";
				if (isSpecialDir && !options.returnSpecialDirs)
					visit = false;
			}

			if (visit)
			{
				Entry entry =
				{
					.path = path / Path{ fileName },
					.attribs = attribs,
					.creationTimeStamp = (u64(findData.ftCreationTime.dwHighDateTime) << 32) | findData.ftCreationTime.dwLowDateTime,
					.lastAccessTimestamp = (u64(findData.ftLastAccessTime.dwHighDateTime) << 32) | findData.ftLastAccessTime.dwLowDateTime,
					.lastWriteTimestamp = (u64(findData.ftLastWriteTime.dwHighDateTime) << 32) | findData.ftLastWriteTime.dwLowDateTime,
					.size = (u64(findData.nFileSizeHigh) << 32) | findData.nFileSizeLow
				};

				const bool isDirectory = attribs & FileAttribute::Directory;
				if ((options.onlyVisitFiles && !isDirectory) ||
					(options.onlyVisitDirs && isDirectory))
					del(entry);

				// TODO: symbolic links, hardlink, and junctions
				if (!isSpecialDir && 
					options.recurseSubDirs && 
					attribs & FileAttribute::Directory && 
					options.maxResursionDepth)
				{
					EnumerationOptions opt = options;
					--opt.maxResursionDepth;
					EnumerateFileSystem(del, entry.path, pattern, opt);
				}
			}


			const bool findRes = FindNextFileW(handle, &findData);
			if (!findRes)
			{
				::FindClose(handle);

				u32 error = ::GetLastError();
				if (error == ERROR_NO_MORE_FILES)
					return SystemErrorCode::Success;
				::SetLastError(error);
				return TranslateSystemError();
			}
		}
	}

	auto Exists(const Path& path) noexcept -> bool
	{
		const DynArray<char16_t> utf16 = ("\\\\?\\"_path + path.AsAbsolute()).ToNative().GetString().ToUtf16();
		return ::GetFileAttributesW(reinterpret_cast<LPCWSTR>(utf16.Data())) != INVALID_FILE_ATTRIBUTES;
	}

	auto GetEntry(const Path& path) noexcept -> Entry
	{
		const DynArray<char16_t> utf16 = ("\\\\?\\"_path + path.AsAbsolute()).ToNative().GetString().ToUtf16();
		WIN32_FILE_ATTRIBUTE_DATA data;
		const bool res = ::GetFileAttributesExW(reinterpret_cast<LPCWSTR>(utf16.Data()), GetFileExInfoStandard, &data);

		if (!res)
			return {};
		return
		{
			.path = path,
			.attribs = Windows::TranslateToFileAttribs(data.dwFileAttributes),
			.creationTimeStamp = (u64(data.ftCreationTime.dwHighDateTime) << 32) | data.ftCreationTime.dwLowDateTime,
			.lastAccessTimestamp = (u64(data.ftLastAccessTime.dwHighDateTime) << 32) | data.ftLastAccessTime.dwLowDateTime,
			.lastWriteTimestamp = (u64(data.ftLastWriteTime.dwHighDateTime) << 32) | data.ftLastWriteTime.dwLowDateTime,
			.size = (u64(data.nFileSizeHigh) << 32) | data.nFileSizeLow
		};
	}

	auto GetCreationTimestamp(const Path& path) noexcept -> u64
	{
		const DynArray<char16_t> utf16 = ("\\\\?\\"_path + path.AsAbsolute()).ToNative().GetString().ToUtf16();
		WIN32_FILE_ATTRIBUTE_DATA data;
		const bool res = ::GetFileAttributesExW(reinterpret_cast<LPCWSTR>(utf16.Data()), GetFileExInfoStandard, &data);
		return res ? (u64(data.ftCreationTime.dwHighDateTime) << 32) | data.ftCreationTime.dwLowDateTime : 0;
	}

	auto GetLastAccessTimestamp(const Path& path) noexcept -> u64
	{
		const DynArray<char16_t> utf16 = ("\\\\?\\"_path + path.AsAbsolute()).ToNative().GetString().ToUtf16();
		WIN32_FILE_ATTRIBUTE_DATA data;
		const bool res = ::GetFileAttributesExW(reinterpret_cast<LPCWSTR>(utf16.Data()), GetFileExInfoStandard, &data);
		return res ? (u64(data.ftLastAccessTime.dwHighDateTime) << 32) | data.ftLastAccessTime.dwLowDateTime : 0;
	}

	auto GetLastWriteTimestamp(const Path& path) noexcept -> u64
	{
		const DynArray<char16_t> utf16 = ("\\\\?\\"_path + path.AsAbsolute()).ToNative().GetString().ToUtf16();
		WIN32_FILE_ATTRIBUTE_DATA data;
		const bool res = ::GetFileAttributesExW(reinterpret_cast<LPCWSTR>(utf16.Data()), GetFileExInfoStandard, &data);
		return res ? (u64(data.ftLastWriteTime.dwHighDateTime) << 32) | data.ftLastWriteTime.dwLowDateTime : 0;
	}

	auto Move(const Path& from, const Path& to, MoveFlags flags) noexcept -> SystemError
	{
		const DynArray<char16_t> fromUtf16 = ("\\\\?\\"_path + from.AsAbsolute()).ToNative().GetString().ToUtf16();
		const DynArray<char16_t> toUtf16 = ("\\\\?\\"_path + to.AsAbsolute()).ToNative().GetString().ToUtf16();
		const bool res = ::MoveFileExW(reinterpret_cast<LPCWSTR>(fromUtf16.Data()), 
								       reinterpret_cast<LPCWSTR>(toUtf16.Data()), 
								       (DWORD)flags);
		return res ? SystemError{} : TranslateSystemError();
	}
}

#endif