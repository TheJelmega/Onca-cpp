#pragma once
#include "LogCategory.h"
#include "core/filesystem/FileSystem.h"

namespace Core
{
	enum class LogLevel
	{
		None   , ///< None
		Severe , ///< Severe error
		Error  , ///< Error
		Warning, ///< Warning
		Info   , ///< Info
		Verbose, ///< Verbose info
		Append , ///< Append to previous line
	};


	/**
	 * Logger
	 */
	// TODO: Allow log messages with colors + write them correctly to each output
	class CORE_API Logger
	{
	public:
		/**
		 * Create a console only logger
		 */
		Logger() noexcept;
		/**
		 * Create a logger that can log to a file and the console
		 * \param[in] filePath Filepath to log file
		 * \param[in] logToConsole Whether to log the output to the console
		 */
		Logger(const FileSystem::Path& filePath, bool logToConsole = true) noexcept;

		Logger(const Logger&) = delete;
		Logger(Logger&&) = delete;

		auto operator=(const Logger&) = delete;
		auto operator=(Logger&) = delete;

		/**
		 * Shutdown the logger
		 */
		auto Shutdown() noexcept -> void;

		/**
		 * Set the filepath to log to and enable logging to file
		 * \param[in] filePath Path to log file
		 */
		auto SetLogFile(const FileSystem::Path& filePath) noexcept -> void;

		/**
		 * Set the maximum log level
		 * \param[in] level Maximum log level
		 */
		auto SetMaxLogLevel(LogLevel level) noexcept -> void;
		/**
		 * Set if the logger needs to log to its file
		 * \param[in] enable Whether to log to the file
		 */
		auto SetLogToFile(bool enable) noexcept -> void;
		/**
		 * Set if the logger needs to log to the system console
		 * \param[in] enable Whether to log to the system console
		 */
		auto SetLogToSystemConsole(bool enable) noexcept -> void;
		/**
		 * Set if the logger needs to log to the debugger
		 * \param[in] enable Whether to log to the debugger
		 */
		auto SetLogToDebugger(bool enable) noexcept -> void;

		/**
		 * Log a message with the given level and category
		 * \param[in] level Log level
		 * \param[in] category Log category
		 * \param[in] message Message
		 */
		auto Log(LogLevel level, const LogCategory& category, const String& message) noexcept -> void;

		/**
		 * Log a message with the given level and category
		 * \tparam Args Argument types
		 * \param[in] level Log level
		 * \param[in] category Log category
		 * \param[in] format Message format
		 * \param[in] args Arguments
		 */
		template<typename... Args>
		auto Log(LogLevel level, const LogCategory& category, const String& format, const Args&... args) noexcept -> void;

		/**
		 * Log a severe error message with the given category
		 * \tparam Args Argument types
		 * \param[in] category Log category
		 * \param[in] format Message format
		 * \param[in] args Arguments
		 */
		template<typename... Args>
		auto Severe(const LogCategory& category, const String& format, const Args&... args) noexcept -> void;
		/**
		 * Log an error message with the given category
		 * \tparam Args Argument types
		 * \param[in] category Log category
		 * \param[in] format Message format
		 * \param[in] args Arguments
		 */
		template<typename... Args>
		auto Error(const LogCategory& category, const String& format, const Args&... args) noexcept -> void;
		/**
		 * Log a warning message with the given category
		 * \tparam Args Argument types
		 * \param[in] category Log category
		 * \param[in] format Message format
		 * \param[in] args Arguments
		 */
		template<typename... Args>
		auto Warning(const LogCategory& category, const String& format, const Args&... args) noexcept -> void;
		/**
		 * Log info message with the given category
		 * \tparam Args Argument types
		 * \param[in] category Log category
		 * \param[in] format Message format
		 * \param[in] args Arguments
		 */
		template<typename... Args>
		auto Info(const LogCategory& category, const String& format, const Args&... args) noexcept -> void;
		/**
		 * Log verbose info message with the given category
		 * \tparam Args Argument types
		 * \param[in] category Log category
		 * \param[in] format Message format
		 * \param[in] args Arguments
		 */
		template<typename... Args>
		auto Verbose(const LogCategory& category, const String& format, const Args&... args) noexcept -> void;
		/**
		 * Append a message to the previous value with the given category
		 * \tparam Args Argument types
		 * \param[in] category Log category
		 * \param[in] format Message format
		 * \param[in] args Arguments
		 */
		template<typename... Args>
		auto Append(const String& format, const Args&... args) noexcept -> void;

	private:

		/**
		 * Log a string to the logger's file
		 * \param[in] str String to log
		 */
		auto LogToFile(const String& str) noexcept -> void;
		/**
		 * Log a string to the system's console
		 * \param[in] str String to log
		 * \param[in] level Log level (used for color in console)
		 */
		auto LogToSysConsole(const String& str, LogLevel level) noexcept -> void;
		/**
		 * Log a string to the attached debugger
		 * \param[in] str String to log
		 */
		auto LogToDebugger(const String& str) noexcept -> void;

		static constexpr const char* LogLevelNames[] =
		{
			"NONE",
			"SEVERE",
			"ERROR",
			"WARNING",
			"INFO",
			"VERBOSE",
			"INVALID LOG LEVEL"
		};

		FileSystem::File m_file;                      ///< File to log to
		LogLevel         m_maxLevel;                  ///< Maximum log level to output
		bool             m_logToSysConsole : 1;       ///< Whether to log to the system console
		bool             m_logToFile       : 1;       ///< Whether to log to a file
		bool             m_logToDebugger   : 1;       ///< Whether to log to the debugger

		bool             m_ignoreMaxLevelForFile : 1; ///< Whether to ignore the max log level when writing to the log file

		LogLevel         m_prevLevel;                 ///< Log level of previous message
		u32              m_prevPrefixLen;             ///< Length of the prefix of the previous level
	};

	CORE_API auto GetLogger() noexcept -> Logger&;
}

#define g_Logger (Core::GetLogger())

#include "Logger.inl"