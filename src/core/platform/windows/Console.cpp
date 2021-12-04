#include "../Console.h"
#if PLATFORM_WINDOWS

#include "Win.h"

namespace Core
{
	SystemConsole::NativeHandle SystemConsole::StdOutHandle = INVALID_HANDLE_VALUE;
	SystemConsole::NativeHandle SystemConsole::StdErrHandle = INVALID_HANDLE_VALUE;
	SystemConsole::NativeHandle SystemConsole::StdInHandle = INVALID_HANDLE_VALUE;
	bool SystemConsole::CreatedConsole = false;
	bool SystemConsole::SupportsUTF8 = false;

	SystemConsoleColor SystemConsole::Fore = SystemConsoleColor::Default;
	SystemConsoleColor SystemConsole::Back = SystemConsoleColor::Default;


	auto SystemConsole::IsValid() noexcept -> bool
	{
		return StdOutHandle != INVALID_HANDLE_VALUE;
	}

	auto SystemConsole::SetForeColor(SystemConsoleColor color) noexcept -> void
	{
		Fore = color;
	}

	auto SystemConsole::SetBackColor(SystemConsoleColor color) noexcept -> void
	{
		Back = color;
	}

	auto SystemConsole::SetColor(SystemConsoleColor fore, SystemConsoleColor back) noexcept -> void
	{
		Fore = fore;
		Back = back;
	}

	auto SystemConsole::Write(const String& str, bool writeToErr) noexcept -> void
	{
		if (str.IsEmpty())
			return;

		HANDLE handle = writeToErr ? StdErrHandle : StdOutHandle;
		if (handle == INVALID_HANDLE_VALUE)
			return;

		DWORD written;
		if (!str.StartsWith('\x1B'))
		{
			// Set the colors
			::WriteConsoleA(handle, ForeCodes[u8(Fore)], 5, &written, nullptr);

			DWORD backBytes = 5 + (u8(Back) >= u8(SystemConsoleColor::LightGrey));
			::WriteConsoleA(handle, BackCodes[u8(Back)], backBytes, &written, nullptr);
		}

		if (SupportsUTF8)
		{
			::WriteConsoleA(handle, str.Data(), DWORD(str.DataSize()), &written, nullptr);
		}
		else
		{
			DynArray<char16_t> utf16 = str.ToUtf16();
			::WriteConsoleW(handle, utf16.Data(), DWORD(utf16.Size()), &written, nullptr);
		}
	}

	auto SystemConsole::Read() noexcept -> String
	{
		if (StdInHandle == INVALID_HANDLE_VALUE)
			return String{};

		if (SupportsUTF8)
		{
			char buffer[1024] = {};
			DWORD bytesRead;
			::ReadConsoleA(StdInHandle, buffer, 1023, &bytesRead, nullptr);
			return String{ buffer };
		}
		else
		{
			char16_t buffer[1024] = {};
			DWORD charsRead;
			::ReadConsoleW(StdInHandle, buffer, 1023, &charsRead, nullptr);
			return String{ buffer };
		}
	}

	auto SystemConsole::Clear() noexcept -> void
	{
		DWORD written;
		WriteConsoleA(StdOutHandle, "\033c", 2, &written, nullptr);
	}

	auto SystemConsole::Init() noexcept -> void
	{
		if (StdOutHandle != INVALID_HANDLE_VALUE)
			return;

		StdOutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);

		// If no console exists already, create a new one
		if (StdOutHandle == INVALID_HANDLE_VALUE)
		{
			::AllocConsole();
			CreatedConsole = true;
			StdOutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
		}

		StdErrHandle = ::GetStdHandle(STD_ERROR_HANDLE);
		StdInHandle = ::GetStdHandle(STD_INPUT_HANDLE);

		// Set the console to support UTF-8, so we do not need to convert to UTF-16 all the time
		SupportsUTF8 = ::IsValidCodePage(CP_UTF8);
		if (SupportsUTF8)
		{
			::SetConsoleOutputCP(CP_UTF8);
			::SetConsoleCP(CP_UTF8);
		}

		// Set console to use virtual terminal commands (posix like escape codes)
		::SetConsoleMode(StdOutHandle, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		::SetConsoleMode(StdErrHandle, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		::SetConsoleMode(StdInHandle, ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_VIRTUAL_TERMINAL_INPUT);
	}

	auto SystemConsole::Shutdown() noexcept -> void
	{
		if (CreatedConsole)
		{
			::FreeConsole();
			CreatedConsole = false;
		}
		else
		{
			DWORD written;
			::WriteConsoleA(StdOutHandle, ForeCodes[u8(SystemConsoleColor::Default)], 5, &written, nullptr);
			::WriteConsoleA(StdOutHandle, BackCodes[u8(SystemConsoleColor::Default)], 5, &written, nullptr);
			::WriteConsoleA(StdErrHandle, ForeCodes[u8(SystemConsoleColor::Default)], 5, &written, nullptr);
			::WriteConsoleA(StdErrHandle, BackCodes[u8(SystemConsoleColor::Default)], 5, &written, nullptr);
		}

		StdOutHandle = INVALID_HANDLE_VALUE;
		StdErrHandle = INVALID_HANDLE_VALUE;
		StdInHandle = INVALID_HANDLE_VALUE;
	}
}

#endif
