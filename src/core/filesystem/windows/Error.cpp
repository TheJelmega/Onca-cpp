#include "../Error.h"
#if PLATFORM_WINDOWS

#include "Windows.h"

namespace Core::FileSystem
{
	auto TranslateFSError() noexcept -> Error
	{
		u32 winErrorCode = ::GetLastError();
		Error err;
		switch (winErrorCode)
		{
		case ERROR_FILE_NOT_FOUND:          err.code = ErrorCode::InvalidPath;     break;
		case ERROR_PATH_NOT_FOUND:          err.code = ErrorCode::InvalidPath;     break;
		case ERROR_TOO_MANY_OPEN_FILES:     err.code = ErrorCode::CouldNotOpen;    break;
		case ERROR_ACCESS_DENIED:           err.code = ErrorCode::AccessDenied;    break;
		case ERROR_WRITE_PROTECT:           err.code = ErrorCode::WriteProtected;  break;
		case ERROR_WRITE_FAULT:             err.code = ErrorCode::WriteFault;      break;
		case ERROR_READ_FAULT:              err.code = ErrorCode::ReadFault;       break;
		case ERROR_SHARING_VIOLATION:       err.code = ErrorCode::ShareViolation;  break;
		case ERROR_LOCK_VIOLATION:          err.code = ErrorCode::LockViolation;   break;
		case ERROR_HANDLE_DISK_FULL:        err.code = ErrorCode::DriveFull;       break;
		case ERROR_FILE_EXISTS:             err.code = ErrorCode::AlreadyExists;   break;
		case ERROR_NEGATIVE_SEEK:           err.code = ErrorCode::NegativeSeek;    break;
		case ERROR_BAD_PATHNAME:            err.code = ErrorCode::InvalidPath;     break;
		case ERROR_INVALID_NAME:            err.code = ErrorCode::InvalidPath;     break;
		case ERROR_CANNOT_COPY:             err.code = ErrorCode::CannotCopy;      break;
		case ERROR_DIRECTORY:               err.code = ErrorCode::InvalidPath;     break;
		case ERROR_DIR_NOT_EMPTY:           err.code = ErrorCode::DirNotEmpty;     break;
		default:                            err.code = ErrorCode::Unknown;         break;
		}
		
		if (err != ErrorCode::Success)
		{
			if (u8(err.code) < NumDefErrMessages)
			{
				err.info = DefaultErrorMessages[u8(err.code)];
			}

			err.info += ", Win32 Error: "_s;
			do
			{
				LPWSTR winStr;
				u32 numChars = ::FormatMessageW
				(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					nullptr,
					winErrorCode,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					reinterpret_cast<LPWSTR>(&winStr),
					0,
					nullptr
				);

				err.info.Add(String{ reinterpret_cast<char16_t*>(winStr), numChars });
				LocalFree(winStr);
			} while (::GetLastError() == ERROR_MORE_DATA);
		}
		return err;
	}
}

#endif