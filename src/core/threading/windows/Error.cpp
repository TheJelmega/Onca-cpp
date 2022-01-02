#include "../Error.h"

#if PLATFORM_WINDOWS
#include "core/platform/Platform.h"

namespace Core::Threading
{
	auto TranslateError() noexcept -> Error
	{
		u32 winErrorCode = ::GetLastError();
		Error err;
		switch (winErrorCode)
		{
		case NO_ERROR:                err.code = ErrorCode::Success;         break;
		case ERROR_INVALID_HANDLE:    err.code = ErrorCode::InvalidHandle;   break;
		case ERROR_NOT_ENOUGH_MEMORY: err.code = ErrorCode::NotEnoughMemory; break;
		default:                      err.code = ErrorCode::Unknown;         break;
		}

		if (err != ErrorCode::Success)
		{
			if (u8(err.code) < NumDefErrMessages)
			{
				err.info = DefaultErrorMessages[u8(err.code)];
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