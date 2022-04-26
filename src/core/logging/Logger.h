#pragma once
#include "LogCategory.h"
#include "core/filesystem/FileSystem.h"

namespace Onca
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
		DISABLE_COPY(Logger);
		DISABLE_MOVE(Logger);

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

		/**
		 * Shutdown the logger
		 */
		void Shutdown() noexcept;

		/**
		 * Set the filepath to log to and enable logging to file
		 * \param[in] filePath Path to log file
		 */
		void SetLogFile(const FileSystem::Path& filePath) noexcept;

		/**
		 * Set the maximum log level
		 * \param[in] level Maximum log level
		 */
		void SetMaxLogLevel(LogLevel level) noexcept;
		/**
		 * Set if the logger needs to log to its file
		 * \param[in] enable Whether to log to the file
		 */
		void SetLogToFile(bool enable) noexcept;
		/**
		 * Set if the logger needs to log to the system console
		 * \param[in] enable Whether to log to the system console
		 */
		void SetLogToSystemConsole(bool enable) noexcept;
		/**
		 * Set if the logger needs to log to the debugger
		 * \param[in] enable Whether to log to the debugger
		 */
		void SetLogToDebugger(bool enable) noexcept;

		/**
		 * Log a message with the given level and category
		 * \param[in] level Log level
		 * \param[in] category Log category
		 * \param[in] message Message
		 */
		void Log(LogLevel level, const LogCategory& category, const String& message) noexcept;

		/**
		 * Log a message with the given level and category
		 * \tparam Args Argument types
		 * \param[in] level Log level
		 * \param[in] category Log category
		 * \param[in] format Message format
		 * \param[in] args Arguments
		 */
		template<typename... Args>
		void Log(LogLevel level, const LogCategory& category, const String& format, const Args&... args) noexcept;

		/**
		 * Log a severe error message with the given category
		 * \tparam Args Argument types
		 * \param[in] category Log category
		 * \param[in] format Message format
		 * \param[in] args Arguments
		 */
		template<typename... Args>
		void Severe(const LogCategory& category, const String& format, const Args&... args) noexcept;
		/**
		 * Log an error message with the given category
		 * \tparam Args Argument types
		 * \param[in] category Log category
		 * \param[in] format Message format
		 * \param[in] args Arguments
		 */
		template<typename... Args>
		void Error(const LogCategory& category, const String& format, const Args&... args) noexcept;
		/**
		 * Log a warning message with the given category
		 * \tparam Args Argument types
		 * \param[in] category Log category
		 * \param[in] format Message format
		 * \param[in] args Arguments
		 */
		template<typename... Args>
		void Warning(const LogCategory& category, const String& format, const Args&... args) noexcept;
		/**
		 * Log info message with the given category
		 * \tparam Args Argument types
		 * \param[in] category Log category
		 * \param[in] format Message format
		 * \param[in] args Arguments
		 */
		template<typename... Args>
		void Info(const LogCategory& category, const String& format, const Args&... args) noexcept;
		/**
		 * Log verbose info message with the given category
		 * \tparam Args Argument types
		 * \param[in] category Log category
		 * \param[in] format Message format
		 * \param[in] args Arguments
		 */
		template<typename... Args>
		void Verbose(const LogCategory& category, const String& format, const Args&... args) noexcept;
		/**
		 * Append a message to the previous value with the given category
		 * \tparam Args Argument types
		 * \param[in] category Log category
		 * \param[in] format Message format
		 * \param[in] args Arguments
		 */
		template<typename... Args>
		void Append(const String& format, const Args&... args) noexcept;

	private:

		/**
		 * Log a string to the logger's file
		 * \param[in] str String to log
		 */
		void LogToFile(const String& str) noexcept;
		/**
		 * Log a string to the system's console
		 * \param[in] str String to log
		 * \param[in] level Log level (used for color in console)
		 */
		void LogToSysConsole(const String& str, LogLevel level) noexcept;
		/**
		 * Log a string to the attached debugger
		 * \param[in] str String to log
		 */
		void LogToDebugger(const String& str) noexcept;

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

#define g_Logger (Onca::GetLogger())

#include "Logger.inl"