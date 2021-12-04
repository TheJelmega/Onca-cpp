#pragma once
#include "core/MinInclude.h"
#include "core/utils/Result.h"
#include "core/containers/ByteBuffer.h"
#include "Enums.h"
#include "Path.h"
#include "Error.h"
#include "IOTask.h"

namespace Core::FileSystem
{
	struct FileRegion
	{
		u64 offset;
		u64 size;
	};
	
	class CORE_API File
	{
	public:
		DEFINE_OPAQUE_HANDLE(NativeHandle);

		/**
		 * Create a null file
		 */
		File();

		File(const File&) = delete;
		File(File&& other) noexcept;

		~File();

		auto operator=(const File&) = delete;
		auto operator=(File&& other) noexcept -> File&;

		/**
		 * Re-open a file with different access mode, share mode or flags
		 * \param[in] access Access mode
		 * \param[in] share Share mode
		 * \param[in] flags Flags
		 * \return Error
		 */
		auto ReOpen(AccessMode access, ShareMode share, FileFlags flags) noexcept -> Error;
		/**
		 * Close the file
		 * \return Error
		 */
		auto Close() noexcept -> Error;

		/**
		 * Seek to an offset in the file
		 * \param[in] offset Offset relative to seek dir (negative values expected when using SeekDir::End)
		 * \param[in] dir Seek direction
		 * \return Error
		 */
		auto Seek(isize offset, SeekDir dir) noexcept -> Error;
		/**
		 * Get the current offset in the file
		 * \return Current offset in the file, or usize(-1) if the file is not valid
		 */
		auto GetFileOffset() const noexcept -> usize;

		/**
		 * Lock the file for reading/writing
		 * \param[in] shared Get shared access to the file (exclusive write for the first lock, shared read for all), otherwise exclusive read/write will be acquired
		 * \param[in] waitForLock Block the call until the lock is acquired
		 * \return Error
		 * \note The user needs to keep track of what files they have locked
		 */
		auto Lock(bool shared = false, bool waitForLock = false) noexcept -> Error;
		/**
		 * Lock a region in the file for reading/writing
		 * \param[in] region File region to lock
		 * \param[in] shared Get shared access to the file (exclusive write for the first lock, shared read for all), otherwise exclusive read/write will be acquired
		 * \param[in] waitForLock Block the call until the lock is acquired
		 * \return Error
		 * \note The user needs to keep track of what regions they have locked
		 */
		auto Lock(const FileRegion& region, bool shared = false, bool waitForLock = false) noexcept -> Error;

		/**
		 * Unlock a previously locked file
		 * \return Result
		 */
		auto Unlock() noexcept -> Error;
		/**
		 * Unlock a previously locked file
		 * \param[in] region File region to lock
		 * \return Result
		 */
		auto Unlock(const FileRegion& region) noexcept -> Error;

		/**
		 * Read the entire file into a byte buffer
		 * \return Result
		 * \note Reads are currently limited to 4GiB, this might change in the future
		 */
		auto Read() const noexcept -> Result<ByteBuffer, Error>;
		/**
		 * Read a region of the file into a byte buffer
		 * \param[in] region File region to read
		 * \return Result
		 * \note Reads are currently limited to 4GiB, this might change in the future
		 */
		auto Read(const FileRegion& region) const noexcept -> Result<ByteBuffer, Error>;

		/**
		 * Initiate an async I/O read operation
		 * \param[in] callback Callback on async completion
		 * \return I/O read tack
		 */
		auto ReadAsync(AsyncReadCallback callback) const noexcept -> IOReadTask;
		/**
		 * Initiate an async I/O read operation
		 * \param[in] region File region to read
		 * \param[in] callback Callback on async completion
		 * \return I/O read tack
		 */
		auto ReadAsync(const FileRegion& region, AsyncReadCallback callback) const noexcept -> IOReadTask;

		/**
		 * Write a buffer to a file with an optional offset into the file
		 * \param[in] buffer Buffer to write
		 * \param[in] offset Offset in file to write
		 * \return Error
		 * \note Writing to a file will overwrite the data that is currently at that location
		 */
		auto Write(const ByteBuffer& buffer, usize offset = Math::Consts::MaxVal<usize>) noexcept -> Error;
		
		/**
		 * Initiate an async I/O write operation
		 * \param[in] buffer Buffer to write
		 * \param[in] callback Callback on async completion
		 * \param[in] offset Offset in file to write
		 * \return I/O read tack
		 * \note Writing to a file will overwrite the data that is currently at that location
		 * \note A copy of the buffer will be made when writing and will be deallocated when the async write has been completed
		 */
		auto WriteAsync(const ByteBuffer& buffer, AsyncWriteCallback callback, usize offset = Math::Consts::MaxVal<usize>) const noexcept -> IOWriteTask;

		/**
		 * Check if the file is readable
		 * \return Whether the file is readable
		 */
		auto IsRead() const noexcept -> bool { return m_access & AccessMode::Read; }
		/**
		 * Check if the file is writable
		 * \return Whether the file is writable
		 */
		auto IsWrite() const noexcept -> bool { return m_access & AccessMode::Write; }
		/**
		 * Check if the file is executable
		 * \return Whether the file is executable
		 */
		auto IsExecute() const noexcept -> bool { return m_access & AccessMode::Execute; }

		/**
		 * Check if the file has buffered read
		 * \return Whether the file has buffered read
		 */
		auto IsBufferedRead() const noexcept -> bool { return !(m_flags & FileFlag::Unbuffered); }
		/**
		 * Check if the file has buffered write
		 * \return Whether the file has buffered write
		 */
		auto IsBufferedWrite() const noexcept -> bool { return !(m_flags & (FileFlag::Unbuffered | FileFlag::WriteThrough)); }

		/**
		 * Check if the file is shared
		 * \return Whether the file is shared
		 */
		auto IsShared() const noexcept -> bool { return m_share != ShareMode::None; }
		/**
		 * Check if the file has shared reading
		 * \return Whether the file has shared reading
		 */
		auto IsSharedRead() const noexcept -> bool { return m_share & ShareMode::Read; }
		/**
		 * Check if the file has shared writing
		 * \return Whether the file has shared writing
		 */
		auto IsSharedWrite() const noexcept -> bool { return m_share & ShareMode::Write; }
		/**
		 * Check if the file has shared delete
		 * \return Whether the file has shared delete
		 */
		auto IsSharedDelete() const noexcept -> bool { return m_share & ShareMode::Delete; }

		/**
		 * Check if a delete is pending for this file
		 * \return Whether a delete is pending for this file
		 */
		auto IsDeletePending() const noexcept -> bool;

		/**
		 * Get the size of the file
		 * \return Size of the file
		 */
		u64 GetFileSize() const noexcept;

		/**
		 * Get the creation timestamp (needs to be converted to know actual time)
		 * \return Timestamp, 0 if the file is invalid
		 */
		auto GetCreationTimestamp() const noexcept -> u64;
		/**
		 * Get the last access timestamp (needs to be converted to know actual time)
		 * \return Timestamp, 0 if the file is invalid
		 */
		auto GetLastAccessTimestamp() const noexcept -> u64;
		/**
		 * Get the last write timestamp (needs to be converted to know actual time)
		 * \return Timestamp, 0 if the file is invalid
		 */
		auto GetLastWriteTimestamp() const noexcept -> u64;

		/**
		 * Get the allocated size on the drive
		 * \return Allocates size on the drive
		 */
		auto GetAllocSize()  const noexcept -> u64;

		/**
		 * Get the file path
		 * \return File path
		 */
		auto GetPath() const noexcept -> const Path& { return m_path; }
		/**
		 * Get the access mode
		 * \return Access mode
		 */
		auto GetAccessMode() const noexcept -> AccessModes { return m_access; }
		/**
		 * Get the share mode
		 * \return Share mode
		 */
		auto GetShareMode() const noexcept -> ShareModes { return m_share; }
		/**
		 * Get the flags
		 * \return Flags
		 */
		auto GetFlags() const noexcept -> FileFlags { return m_flags; }
		/**
		 * Get the native file handle
		 * \return Native file handle
		 */
		auto GetNative() noexcept -> NativeHandle { return m_handle; }


		/**
		 * Create a file
		 * \param[in] path Path to file
		 * \param[in] createKind Create kind
		 * \param[in] access Access mode
		 * \param[in] share Share mode
		 * \param[in] flags Flags
		 * \return Result with opened file or error
		 */
		static auto Create(const Path& path, FileCreateKind createKind, AccessMode access = AccessMode::ReadWrite, 
						   ShareModes share = ShareMode::None, FileAttributes attribs = FileAttribute::None, 
						   FileFlags flags = FileFlag::None) noexcept -> Result<File, Error>;
		/**
		 * Open a file
		 * \param[in] path Path to file
		 * \param[in] truncate Whether to truncate the content on opening (clear content)
		 * \param[in] access Access mode
		 * \param[in] share Share mode
		 * \param[in] flags Flags
		 * \return Result with opened file or error
		 */
		static auto Open(const Path& path, bool truncate = false, AccessMode access = AccessMode::ReadWrite, 
						 ShareModes share = ShareMode::None, FileFlags flags = FileFlag::None) noexcept -> Result<File, Error>;

	private:

		/**
		 * Create a file from a native handle
		 * \param[in] handle Native handle
		 * \param[in] access Access mode
		 */
		File(const Path& path, NativeHandle handle, AccessMode access, ShareMode share, FileFlags flags);

		Path         m_path;   ///< File path
		NativeHandle m_handle; ///< Handle to file
		AccessModes  m_access; ///< Access mode
		ShareModes   m_share;  ///< Share mode
		FileFlags    m_flags;  ///< Flags
	};

	/**
	 * Check if a path points to a file
	 * \param[in] path Path
	 * \return If the path points to a file
	 */
	auto IsFile(const Path& path) noexcept -> bool;

	/**
	 * Delete a file
	 * \param[in] path Path to file
	 * \return Result
	 */
	auto DeleteFile(const Path& path) noexcept -> Error;
}

