#include "../File.h"
#if PLATFORM_WINDOWS

#include "core/platform/Platform.h"

namespace Core::FileSystem
{
	namespace Windows
	{
		void AsyncReadCallback(DWORD errCode, DWORD bytesTransfered, OVERLAPPED* pOverlapped)
		{
			IOReadTask::Data* pData = reinterpret_cast<IOReadTask::Data*>(pOverlapped->hEvent);
			if (!pData)
				return;

			pData->error.code = SystemErrorCode::Success;
			if (errCode)
			{
				const u32 curErr = ::GetLastError();
				::SetLastError(errCode);
				pData->error = TranslateSystemError();
				::SetLastError(curErr);
			}
			else
			{
				DWORD dummyBytesTransferred;
				const bool res = ::GetOverlappedResult(pData->fileHandle,
												       reinterpret_cast<LPOVERLAPPED>(&pData->nData),
												       &dummyBytesTransferred,
												       true);
				if (!res)
					pData->error = TranslateSystemError();
			}

			pData->validData = true;
			pData->callback.TryInvoke(pData->buffer, pData->error);
			::SetEvent(pData->waitHandle);
		}

		void AsyncWriteCallback(DWORD errCode, DWORD bytesTransfered, OVERLAPPED* pOverlapped)
		{
			IOWriteTask::Data* pData = reinterpret_cast<IOWriteTask::Data*>(pOverlapped->hEvent);
			if (!pData)
				return;

			pData->error.code = SystemErrorCode::Success;
			if (errCode)
			{
				const u32 curErr = ::GetLastError();
				::SetLastError(errCode);
				pData->error = TranslateSystemError();
				::SetLastError(curErr);
			}
			else
			{
				DWORD dummyBytesTransferred;
				const bool res = ::GetOverlappedResult(pData->fileHandle,
												       reinterpret_cast<LPOVERLAPPED>(&pData->nData),
												       &dummyBytesTransferred,
												       true);
				if (!res)
					pData->error = TranslateSystemError();
			}

			pData->buffer.GetContainer().Clear(true);
			pData->callback.TryInvoke(pData->error);
			::SetEvent(pData->waitHandle);
		}
	}


	File::File()
		: m_handle(INVALID_HANDLE_VALUE)
		, m_access(AccessMode::Read)
		, m_share(ShareMode::None)
		, m_flags(FileFlag::None)
	{
	}

	File::File(File&& other) noexcept
		: m_path(Move(other.m_path))
		, m_handle(other.m_handle)
		, m_access(other.m_access)
		, m_share(other.m_share)
		, m_flags(other.m_flags)
	{
		other.m_handle = INVALID_HANDLE_VALUE;
	}

	File::~File()
	{
		if (IsValid())
			Close();
	}

	auto File::operator=(File&& other) noexcept -> File&
	{
		this->~File();

		m_path = other.m_path;

		m_handle = other.m_handle;
		other.m_handle = INVALID_HANDLE_VALUE;

		m_access = other.m_access;
		m_share = other.m_share;
		m_flags = other.m_flags;

		return *this;
	}

	auto File::ReOpen(AccessMode access, ShareMode share, FileFlags flags) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		m_handle = ::ReOpenFile(m_handle, u32(access), u32(share), u32(flags) << 16);
		return m_handle == INVALID_HANDLE_VALUE ? SystemError{} : TranslateSystemError();
	}

	auto File::Close() noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		const bool res = ::CloseHandle(reinterpret_cast<HANDLE>(m_handle));
		m_handle = INVALID_HANDLE_VALUE;
		return res ? SystemError{} : TranslateSystemError();
	}

	auto File::Seek(isize offset, SeekDir dir) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		const usize fileSize = GetFileSize();
		switch (dir)
		{
		case SeekDir::Begin:
			offset = Math::Clamp(offset, 0, fileSize);
			break;
		case SeekDir::Current:
		{
			usize curOffset = GetFileOffset();
			offset = Math::Clamp(offset, -isize(curOffset), fileSize - curOffset);
			break;
		}
		case SeekDir::End: 
			offset = Math::Clamp(offset, -isize(fileSize), 0);
			break;
		default: ;
		}

		LONG upper = LONG(offset >> 32);
		::SetFilePointer(m_handle, u32(offset), &upper, DWORD(dir));
		return ::GetLastError() == NO_ERROR ? SystemError{} : TranslateSystemError();
	}

	auto File::GetFileOffset() const noexcept -> usize
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return Math::Consts::MaxVal<usize>;

		LONG upper = 0;
		const LONG lower = ::SetFilePointer(m_handle, 0, &upper, FILE_CURRENT);
		u32 err = ::GetLastError();
		if (lower == INVALID_SET_FILE_POINTER && err != 0)
			return Math::Consts::MaxVal<usize>;

		return (isize(upper) << 32) | lower;
	}

	auto File::Lock(bool shared, bool waitForLock) noexcept -> SystemError
	{
		return Lock({ .offset = 0, .size = Math::Consts::MaxVal<u64> }, shared, waitForLock);
	}

	auto File::Lock(const FileRegion& region, bool shared, bool waitForLock) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return { SystemErrorCode::InvalidHandle };

		const usize fileSize = GetFileSize();
		const usize offset = GetFileOffset() + region.offset;
		if (offset >= fileSize)
			return SystemErrorCode::OffOutOfRange;

		const usize maxSize = Math::Min(Math::Consts::MaxVal<u32>, fileSize) - offset;
		const usize bytesToLock = Math::Min(region.size, maxSize);


		OVERLAPPED overlapped{ .Pointer = reinterpret_cast<PVOID>(offset) };
		const bool res = ::LockFileEx (m_handle,
			                           (shared ? 0 : LOCKFILE_EXCLUSIVE_LOCK) | (waitForLock ? 0 : LOCKFILE_FAIL_IMMEDIATELY),
			                           0,
			                           u32(bytesToLock), u32(bytesToLock >> 32),
			                           &overlapped);
		return res ? SystemError{} : TranslateSystemError();
	}
	
	auto File::Unlock() noexcept -> SystemError
	{
		return Unlock({ .offset = 0, .size = Math::Consts::MaxVal<u64> });
	}

	auto File::Unlock(const FileRegion& region) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;

		const usize fileSize = GetFileSize();
		const usize offset = GetFileOffset() + region.offset;
		if (offset >= fileSize)
			return SystemErrorCode::OffOutOfRange;

		const usize maxSize = Math::Min(Math::Consts::MaxVal<u32>, fileSize) - offset;
		const usize bytesToUnlock = Math::Min(region.size, maxSize);

		bool res = ::UnlockFile(m_handle,
								u32(offset), u32(offset >> 32),
								u32(bytesToUnlock), u32(bytesToUnlock >> 32));
		return res ? SystemError{} : TranslateSystemError();
	}

	auto File::Read() const noexcept -> Result<ByteBuffer, SystemError>
	{
		return Read({ .offset = 0, .size = Math::Consts::MaxVal<u64> });
	}

	auto File::Read(const FileRegion& region) const noexcept -> Result<ByteBuffer, SystemError>
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemError{ SystemErrorCode::InvalidHandle };
		if (!(m_access & AccessMode::Read))
			return SystemError{ SystemErrorCode::NoReadPerms };

		const usize fileSize = GetFileSize();
		const usize offset = GetFileOffset() + region.offset;
		if (offset >= fileSize)
			return SystemError{ SystemErrorCode::OffOutOfRange };

		const usize maxSize = Math::Min(Math::Consts::MaxVal<u32>, GetFileSize()) - offset;
		const usize bytesToRead = Math::Min(region.size, maxSize);

		ByteBuffer buffer;
		buffer.Resize(bytesToRead);

		OVERLAPPED overlapped = { .Pointer = reinterpret_cast<PVOID>(offset) };

		HANDLE syncEvent = 0;
		if (m_flags & FileFlag::AllowAsync)
		{
			syncEvent = CreateEventW(nullptr, false, false, nullptr);
			if (syncEvent == INVALID_HANDLE_VALUE)
				return SystemError{ SystemErrorCode::Unknown, "Failed to create wait event handle for sync operation on async file"_s };

			overlapped.hEvent = syncEvent;
		}

		u32 bytesRead;
		bool res = ::ReadFile(m_handle,
							  buffer.Data(),
							  u32(bytesToRead),
							  reinterpret_cast<LPDWORD>(&bytesRead),
							  &overlapped);

		if (!res && m_flags & FileFlag::AllowAsync)
		{
			u32 curErr = ::GetLastError();
			if (curErr == ERROR_IO_PENDING)
			{
				u32 err = WAIT_IO_COMPLETION;
				while (err == WAIT_IO_COMPLETION)
					err = WaitForSingleObjectEx(syncEvent, INFINITE, true);
				res = err == WAIT_OBJECT_0;
			}
			::CloseHandle(syncEvent);
		}

		if (!res)
			return TranslateSystemError();
		return buffer;
	}

	auto File::ReadAsync(AsyncReadCallback callback) const noexcept -> IOReadTask
	{
		return ReadAsync({ .offset = 0, .size = Math::Consts::MaxVal<u64> }, callback);
	}

	auto File::ReadAsync(const FileRegion& region, AsyncReadCallback callback) const noexcept -> IOReadTask
	{
		if (m_handle == INVALID_HANDLE_VALUE)
		{
			callback.TryInvoke(ByteBuffer{}, { SystemErrorCode::InvalidHandle });
			return IOReadTask{};
		}
		if (!(m_flags & FileFlag::AllowAsync))
		{
			callback.TryInvoke(ByteBuffer{}, { SystemErrorCode::NoAsyncSupport });
			return IOReadTask{};
		}
		if (!(m_access & AccessMode::Read))
		{
			callback.TryInvoke(ByteBuffer{}, { SystemErrorCode::NoReadPerms });
			return IOReadTask{};
		}

		const usize fileSize = GetFileSize();
		const usize offset = GetFileOffset() + region.offset;
		if (offset >= fileSize)
		{
			callback.TryInvoke(ByteBuffer{}, { SystemErrorCode::OffOutOfRange });
			return IOReadTask{};
		}

		const usize maxSize = Math::Min(Math::Consts::MaxVal<u32>, GetFileSize()) - offset;
		const usize bytesToRead = Math::Min(region.size, maxSize);

		IOReadTask task{ m_handle, callback, bytesToRead };
		OVERLAPPED* pOverlapped = reinterpret_cast<OVERLAPPED*>(&task.m_data->nData);
		pOverlapped->Pointer = reinterpret_cast<PVOID>(offset);

		// ReadFileEx ignores the hEvent value, so we can use it to pass our own data
		pOverlapped->hEvent = task.m_data.Get();

		const bool res = ::ReadFileEx(m_handle,
								      task.m_data->buffer.Data(),
								      u32(bytesToRead),
								      reinterpret_cast<LPOVERLAPPED>(&task.m_data->nData),
								      &Windows::AsyncReadCallback);
		if (!res)
		{
			callback.TryInvoke(ByteBuffer{}, TranslateSystemError());
			return IOReadTask{};
		}
		return task;
	}

	auto File::Write(const ByteBuffer& buffer, usize offset) noexcept -> SystemError
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return SystemErrorCode::InvalidHandle;
		if (!(m_access & AccessMode::Write))
			return SystemErrorCode::NoWritePerms;

		const usize fileSize = GetFileSize();
		if (offset == usize(-1))
			offset = 0;
		offset += GetFileOffset();

		if (offset > fileSize)
			return SystemErrorCode::OffOutOfRange;

		OVERLAPPED overlapped = { .Pointer = reinterpret_cast<PVOID>(offset) };

		HANDLE syncEvent = 0;
		if (m_flags & FileFlag::AllowAsync)
		{
			syncEvent = CreateEventW(nullptr, false, false, nullptr);
			if (syncEvent == INVALID_HANDLE_VALUE)
				return SystemError{ SystemErrorCode::Unknown, "Failed to create wait event handle for sync operation on async file"_s };

			overlapped.hEvent = syncEvent;
		}

		u32 bytesWritten;
		bool res = ::WriteFile(m_handle,
							   buffer.Data(),
							   u32(buffer.Size()),
							   reinterpret_cast<LPDWORD>(&bytesWritten),
							   &overlapped);

		if (!res && m_flags & FileFlag::AllowAsync)
		{
			u32 curErr = ::GetLastError();
			if (curErr == ERROR_IO_PENDING)
			{
				u32 err = WAIT_IO_COMPLETION;
				while (err == WAIT_IO_COMPLETION)
					err = WaitForSingleObjectEx(syncEvent, INFINITE, true);
				res = err == WAIT_OBJECT_0;
			}
			::CloseHandle(syncEvent);
		}

		return res ? SystemError{} : TranslateSystemError();
	}

	auto File::WriteAsync(const ByteBuffer& buffer, AsyncWriteCallback callback, usize offset) const noexcept -> IOWriteTask
	{
		if (m_handle == INVALID_HANDLE_VALUE)
		{
			callback.TryInvoke({ SystemErrorCode::InvalidHandle });
			return IOWriteTask{};
		}
		if (!(m_flags & FileFlag::AllowAsync))
		{
			callback.TryInvoke({ SystemErrorCode::NoAsyncSupport });
			return IOWriteTask{};
		}
		if (!(m_access & AccessMode::Write))
		{
			callback.TryInvoke({ SystemErrorCode::NoWritePerms });
			return IOWriteTask{};
		}

		const usize fileSize = GetFileSize();
		offset += GetFileOffset();
		if (offset >= fileSize)
		{
			callback.TryInvoke({ SystemErrorCode::OffOutOfRange });
			return IOWriteTask{};
		}

		IOWriteTask task(m_handle, callback, buffer);
		OVERLAPPED* pOverlapped = reinterpret_cast<OVERLAPPED*>(&task.m_data->nData);
		pOverlapped->Pointer = reinterpret_cast<PVOID>(offset);

		// WriteFileEx ignores the hEvent value, so we can use it to pass our own data
		pOverlapped->hEvent = task.m_data.Get();


		const bool res = ::WriteFileEx(m_handle,
								       task.m_data->buffer.Data(),
								       u32(task.m_data->buffer.Size()),
								       reinterpret_cast<LPOVERLAPPED>(&task.m_data->nData),
								       &Windows::AsyncWriteCallback);
		if (!res)
		{
			callback.TryInvoke(TranslateSystemError());
			return IOWriteTask{};
		}
		return task;
	}

	auto File::IsDeletePending() const noexcept -> bool
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return false;

		FILE_STANDARD_INFO info;
		const bool res = ::GetFileInformationByHandleEx(m_handle, FileStandardInfo, &info, sizeof(FILE_STANDARD_INFO));
		return res && info.DeletePending;
	}

	auto File::IsValid() const noexcept -> bool
	{
		return m_handle != INVALID_HANDLE_VALUE;
	}

	File::operator bool() const noexcept
	{
		return IsValid();
	}

	auto File::GetFileSize() const noexcept -> u64
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return 0;

		LARGE_INTEGER li;
		::GetFileSizeEx(m_handle, &li);
		return u64(li.QuadPart);
	}

	auto File::GetCreationTimestamp() const noexcept -> u64
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return 0;

		FILETIME time;
		const bool res = ::GetFileTime(m_handle, &time, nullptr, nullptr);
		return res ? (u64(time.dwHighDateTime) << 32) || time.dwLowDateTime : 0;
	}

	auto File::GetLastAccessTimestamp() const noexcept -> u64
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return 0;

		FILETIME time;
		const bool res = ::GetFileTime(m_handle, nullptr, &time, nullptr);
		return res ? (u64(time.dwHighDateTime) << 32) || time.dwLowDateTime : 0;
	}

	auto File::GetLastWriteTimestamp() const noexcept -> u64
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return 0;

		FILETIME time;
		const bool res = ::GetFileTime(m_handle, nullptr, nullptr, &time);
		return res ? (u64(time.dwHighDateTime) << 32) || time.dwLowDateTime : 0;
	}

	auto File::GetAllocSize() const noexcept -> u64
	{
		if (m_handle == INVALID_HANDLE_VALUE)
			return 0;

		FILE_STANDARD_INFO info;
		const bool res = ::GetFileInformationByHandleEx(m_handle, FileStandardInfo, &info, sizeof(FILE_STANDARD_INFO));
		return res ? info.AllocationSize.QuadPart : 0;
	}

	auto File::Create(const Path& path, FileCreateKind createKind, AccessMode access, ShareModes share, FileAttributes attribs, FileFlags flags) noexcept -> Result<File, SystemError>
	{
		const DynArray<char16_t> utf16 = ("\\\\?\\"_path + path.AsAbsolute()).ToNative().GetString().ToUtf16();

		const u32 accessMode = (u8(access) & u8(AccessMode::Read)    ? GENERIC_READ    : 0) |
			                   (u8(access) & u8(AccessMode::Write)   ? GENERIC_WRITE   : 0) |
			                   (u8(access) & u8(AccessMode::Execute) ? GENERIC_EXECUTE : 0);

		const HANDLE handle = ::CreateFileW(reinterpret_cast<LPCWSTR>(utf16.Data()),
			                                accessMode,
			                                share,
			                                nullptr,
			                                DWORD(createKind),
			                                (u32(flags) << 16) | u32(attribs),
			                                nullptr);

		if (handle == INVALID_HANDLE_VALUE)
			return TranslateSystemError();

		return File{ path.AsAbsolute(), reinterpret_cast<NativeHandle>(handle), access, share, flags};
	}

	auto File::Open(const Path& path, bool truncate, AccessMode access, ShareModes share, FileFlags flags) noexcept -> Result<File, SystemError>
	{
		const DynArray<char16_t> utf16 = ("\\\\?\\"_path + path.AsAbsolute()).ToNative().GetString().ToUtf16();

		const u32 accessMode = (u8(access) & u8(AccessMode::Read)    ? GENERIC_READ    : 0) |
			                   (u8(access) & u8(AccessMode::Write)   ? GENERIC_WRITE   : 0) |
			                   (u8(access) & u8(AccessMode::Execute) ? GENERIC_EXECUTE : 0);

		const HANDLE handle = ::CreateFileW(reinterpret_cast<LPCWSTR>(utf16.Data()),
			                                accessMode,
			                                share,
			                                nullptr,
			                                (truncate ? TRUNCATE_EXISTING : OPEN_EXISTING),
									        (u32(flags) << 16),
			                                nullptr);

		if (handle == INVALID_HANDLE_VALUE)
			return TranslateSystemError();

		return File{ path, reinterpret_cast<NativeHandle>(handle), access, share, flags };
	}

	File::File(const Path& path, NativeHandle handle, AccessMode access, ShareMode share, FileFlags flags)
		: m_path(path)
		, m_handle(handle)
		, m_access(access)
		, m_share(share)
		, m_flags(flags)
	{
	}

	// TODO: what about symlinks, etc ???
	auto IsFile(const Path& path) noexcept -> bool
	{
		const DynArray<char16_t> utf16 = ("\\\\?\\"_path + path.AsAbsolute()).ToNative().GetString().ToUtf16();
		const u32 attribs = ::GetFileAttributesW(reinterpret_cast<LPCWSTR>(utf16.Data()));
		return attribs != INVALID_FILE_ATTRIBUTES && !(attribs & FILE_ATTRIBUTE_DIRECTORY);
	}

	auto DeleteFile(const Path& path) noexcept -> SystemError
	{
		const DynArray<char16_t> utf16 = ("\\\\?\\"_path + path.AsAbsolute()).ToNative().GetString().ToUtf16();
		const bool res = ::DeleteFileW(reinterpret_cast<LPCWSTR>(utf16.Data()));
		return res ? SystemError{} : TranslateSystemError();
	}
}

#endif