#include "Logger.h"
#include "core/platform/Console.h"
#include "core/platform/Debugger.h"
#include <chrono>

#include "core/chrono/DateTime.h"

namespace Core
{
	Logger::Logger() noexcept
		: m_maxLevel(LogLevel::Verbose)
		, m_logToSysConsole(true)
		, m_logToFile(false)
		, m_logToDebugger(Debugger::IsAttached())
		, m_ignoreMaxLevelForFile(true)
		, m_prevLevel(LogLevel::None)
		, m_prevPrefixLen(0)
	{
		Info(LogCategories::CORE, "Logger intialized"_s);
	}

	Logger::Logger(const FileSystem::Path& filePath, bool logToConsole) noexcept
		: m_maxLevel(LogLevel::Verbose)
		, m_logToSysConsole(logToConsole)
		, m_logToFile(true)
		, m_logToDebugger(Debugger::IsAttached())
		, m_ignoreMaxLevelForFile(true)
		, m_prevLevel(LogLevel::None)
		, m_prevPrefixLen(0)
	{
		auto res = FileSystem::File::Create(filePath, FileSystem::FileCreateKind::CreateAlways);

		if (res.Success())
			m_file = Move(res.MoveValue());
		else
			m_logToFile = false;

		Info(LogCategories::CORE, "Logger intialized with file: {}"_s, filePath);
	}

	auto Logger::Shutdown() noexcept -> void
	{
		Info(LogCategories::CORE, "Logger Shutdown"_s);

		if (m_file)
			m_file.~File();
	}

	auto Logger::SetLogFile(const FileSystem::Path& filePath) noexcept -> void
	{
		if (m_file)
			m_file.Close();

		auto res = FileSystem::File::Create(filePath, FileSystem::FileCreateKind::CreateAlways);
		if (res.Success())
		{
			m_file = Move(res.MoveValue());
			m_logToFile = true;

			Info(LogCategories::CORE, "Logger file path set: {}"_s, filePath);
		}
		else
		{
			m_logToFile = false;
		}
	}

	auto Logger::SetMaxLogLevel(LogLevel level) noexcept -> void
	{
		m_maxLevel = level;
	}

	auto Logger::SetLogToFile(bool enable) noexcept -> void
	{
		m_logToFile = enable && m_file;
	}

	auto Logger::SetLogToSystemConsole(bool enable) noexcept -> void
	{
		m_logToSysConsole = enable;
	}

	auto Logger::SetLogToDebugger(bool enable) noexcept -> void
	{
		m_logToDebugger = enable && Debugger::IsAttached();
	}

	auto Logger::Log(LogLevel level, const LogCategory& category, const String& message) noexcept -> void
	{
		bool validLevel = u8(level) <= u8(m_maxLevel) || (level == LogLevel::Append && u8(m_prevLevel) < u8(m_maxLevel));
		if (!validLevel && !m_ignoreMaxLevelForFile)
			return;

		String prefix;
		if (level == LogLevel::Append)
		{
			prefix.PadLeft(m_prevPrefixLen);
		}
		else
		{
			Chrono::DateTime dateTime = Chrono::DateTime::Now();
			prefix = Format("{} {} [{}]: "_s, dateTime, LogLevelNames[u8(level)], category.name);
			m_prevPrefixLen = u32(prefix.Length());
		}
		String formatted = prefix + message + '\n';

		if (m_logToFile && (validLevel || m_ignoreMaxLevelForFile))
			LogToFile(formatted);

		if (!validLevel)
			return;

		if (m_logToSysConsole)
			LogToSysConsole(formatted, level);
		if (m_logToDebugger)
			Debugger::OutputDebugString(formatted);
	}

	auto Logger::LogToFile(const String& str) noexcept -> void
	{
		if (!m_file)
			return;

		ByteBuffer buffer{ str.Data(), str.DataSize() };
		m_file.Write(buffer);
	}

	auto Logger::LogToSysConsole(const String& str, LogLevel level) noexcept -> void
	{
		constexpr SystemConsoleColor colors[usize(LogLevel::Verbose) + 1] =
		{
			SystemConsoleColor::Default,
			SystemConsoleColor::DarkRed,
			SystemConsoleColor::Red,
			SystemConsoleColor::Yellow,
			SystemConsoleColor::Default,
			SystemConsoleColor::Default,
		};

		SystemConsole::SetForeColor(colors[u8(level == LogLevel::Append ? m_prevLevel : level)]);
		SystemConsole::Write(str);
		SystemConsole::SetForeColor(SystemConsoleColor::Default);
	}

	auto Logger::LogToDebugger(const String& str) noexcept -> void
	{
		Debugger::OutputDebugString(str);
	}

	auto GetLogger() noexcept -> Logger&
	{
		static Logger logger;
		return logger;
	}
}
