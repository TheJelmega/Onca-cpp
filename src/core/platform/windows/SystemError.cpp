#include "../SystemError.h"
#if PLATFORM_WINDOWS 

#include "Windows.h" 

namespace Onca
{
	auto TranslateSystemError() noexcept -> SystemError
	{
		u32 winErrorCode = ::GetLastError();
		SystemError err;
		switch (winErrorCode)
		{
		case NO_ERROR:                      err.code = SystemErrorCode::Success;         break;
		case ERROR_FILE_NOT_FOUND:          err.code = SystemErrorCode::InvalidPath;     break;
		case ERROR_PATH_NOT_FOUND:          err.code = SystemErrorCode::InvalidPath;     break;
		case ERROR_TOO_MANY_OPEN_FILES:     err.code = SystemErrorCode::CouldNotOpen;    break;
		case ERROR_ACCESS_DENIED:           err.code = SystemErrorCode::AccessDenied;    break;
		case ERROR_WRITE_PROTECT:           err.code = SystemErrorCode::WriteProtected;  break;
		case ERROR_WRITE_FAULT:             err.code = SystemErrorCode::WriteFault;      break;
		case ERROR_READ_FAULT:              err.code = SystemErrorCode::ReadFault;       break;
		case ERROR_SHARING_VIOLATION:       err.code = SystemErrorCode::ShareViolation;  break;
		case ERROR_LOCK_VIOLATION:          err.code = SystemErrorCode::LockViolation;   break;
		case ERROR_HANDLE_DISK_FULL:        err.code = SystemErrorCode::DriveFull;       break;
		case ERROR_FILE_EXISTS:             err.code = SystemErrorCode::AlreadyExists;   break;
		case ERROR_NEGATIVE_SEEK:           err.code = SystemErrorCode::NegativeSeek;    break;
		case ERROR_BAD_PATHNAME:            err.code = SystemErrorCode::InvalidPath;     break;
		case ERROR_INVALID_NAME:            err.code = SystemErrorCode::InvalidPath;     break;
		case ERROR_CANNOT_COPY:             err.code = SystemErrorCode::CannotCopy;      break;
		case ERROR_DIRECTORY:               err.code = SystemErrorCode::InvalidPath;     break;
		case ERROR_DIR_NOT_EMPTY:           err.code = SystemErrorCode::DirNotEmpty;     break;
		case ERROR_NOT_ENOUGH_MEMORY:       err.code = SystemErrorCode::NotEnoughMemory; break;
		default:                            err.code = SystemErrorCode::Unknown;         break;
		}

		if (err != SystemErrorCode::Success)
		{
			if (u8(err.code) < NumDefErrMessages)
			{
				err.info = DefaultSystemErrorMessages[u8(err.code)];
				err.info += ", "_s;
			}

			err.info += "Win32 Error: "_s;
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