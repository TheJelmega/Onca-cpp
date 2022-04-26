#pragma once
#include "core/MinInclude.h"
#include "core/string/Include.h"

namespace Core
{
	enum class SystemConsoleColor
	{
		Default,
		Black,
		DarkRed,
		DarkGreen,
		DarkYellow,
		DarkBlue,
		DarkMagenta,
		DarkCyan,
		DarkGrey,
		LightGrey,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White,
	};

	/**
	 * System console (only 1 can be created or used)
	 */
	class CORE_API SystemConsole
	{
	public:
		DEFINE_OPAQUE_HANDLE(NativeHandle);

		/**
		 * Check if the console is valid
		 * \return Whether the console is valid
		 */
		static auto IsValid() noexcept -> bool;

		/**
		 * Set the current foreground color
		 * \param[in] color Foreground color
		 */
		static void SetForeColor(SystemConsoleColor color) noexcept;
		/**
		 * Set the current background color
		 * \param[in] color Background color
		 */
		static void SetBackColor(SystemConsoleColor color) noexcept;
		/**
		 * Set the current foreground and background color
		 * \param[in] fore Foreground color
		 * \param[in] back Background color
		 */
		static void SetColor(SystemConsoleColor fore, SystemConsoleColor back) noexcept;

		/**
		 * Write a string to the console
		 * \param[in] str String to write
		 * \note If the string starts with an 'ESC', the current colors will be ignored
		 */
		static void Write(const String& str, bool writeToErr = false) noexcept;
		/**
		 * Read the console input
		 * \return Read input
		 * \note Because of certain OS limitation, at maximum, 1023 bytes can be read from the console
		 */
		static auto Read() noexcept -> String;

		/**
		 * Clear the console output
		 */
		static void Clear() noexcept;

		/**
		 * Get or create the system console
		 * \return System console
		 */
		static void Init() noexcept;
		/**
		 * Shutdown the system console and clean it up if the program had to create the console
		 */
		static void Shutdown() noexcept;

	private:
		constexpr static const char* ForeCodes[] =
		{
			"\033[39m",
			"\033[30m",
			"\033[31m",
			"\033[32m",
			"\033[33m",
			"\033[34m",
			"\033[35m",
			"\033[36m",
			"\033[37m",
			"\033[90m",
			"\033[91m",
			"\033[92m",
			"\033[93m",
			"\033[94m",
			"\033[95m",
			"\033[96m",
			"\033[97m",
		};

		constexpr static const char* BackCodes[] =
		{
			"\033[49m",
			"\033[40m",
			"\033[41m",
			"\033[42m",
			"\033[43m",
			"\033[44m",
			"\033[45m",
			"\033[46m",
			"\033[47m",
			"\033[100m",
			"\033[101m",
			"\033[102m",
			"\033[103m",
			"\033[104m",
			"\033[105m",
			"\033[106m",
			"\033[107m",
		};

		static NativeHandle StdOutHandle;
		static NativeHandle StdErrHandle;
		static NativeHandle StdInHandle;
		static bool CreatedConsole;
		static bool SupportsUTF8;

		static SystemConsoleColor Fore;
		static SystemConsoleColor Back;
	};
}
