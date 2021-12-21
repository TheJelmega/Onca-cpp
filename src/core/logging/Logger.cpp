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
	{
	}

	Logger::Logger(const FileSystem::Path& filePath, bool logToConsole) noexcept
		: m_maxLevel(LogLevel::Verbose)
		, m_logToSysConsole(logToConsole)
		, m_logToFile(true)
		, m_logToDebugger(Debugger::IsAttached())
		, m_ignoreMaxLevelForFile(true)
	{
		auto res = FileSystem::File::Create(filePath, FileSystem::FileCreateKind::CreateAlways);

		if (res.Success())
			m_file = Move(res.MoveValue());
		else
			m_logToFile = false;
	}

	auto Logger::Shutdown() noexcept -> void
	{
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
		bool validLevel = u8(level) <= u8(m_maxLevel);
		if (!validLevel && !m_ignoreMaxLevelForFile)
			return;

		Chrono::DateTime dateTime = Chrono::DateTime::Now();
		String formatted = Format("{} {} [{}]: {}\n"_s, dateTime, LogLevelNames[u8(level)], category.name, message);

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

		SystemConsole::SetForeColor(colors[u8(level)]);
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
