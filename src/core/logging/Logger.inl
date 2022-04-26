#pragma once
#if __RESHARPER__
#include "Logger.h"
#endif

namespace Onca
{
	template <typename ... Args>
	void Logger::Log(LogLevel level, const LogCategory& category, const String& format, const Args&... args) noexcept
	{
		String message = Format(format, args...);
		Log(level, category, message);
	}

	template <typename ... Args>
	void Logger::Severe(const LogCategory& category, const String& format, const Args&... args) noexcept
	{
		Log(LogLevel::Severe, category, format, args...);
	}

	template <typename ... Args>
	void Logger::Error(const LogCategory& category, const String& format, const Args&... args) noexcept
	{
		Log(LogLevel::Error, category, format, args...);
	}

	template <typename ... Args>
	void Logger::Warning(const LogCategory& category, const String& format, const Args&... args) noexcept
	{
		Log(LogLevel::Warning, category, format, args...);
	}

	template <typename ... Args>
	void Logger::Info(const LogCategory& category, const String& format, const Args&... args) noexcept
	{
		Log(LogLevel::Info, category, format, args...);
	}

	template <typename ... Args>
	void Logger::Verbose(const LogCategory& category, const String& format, const Args&... args) noexcept
	{
		Log(LogLevel::Verbose, category, format, args...);
	}

	template <typename ... Args>
	void Logger::Append(const String& format, const Args&... args) noexcept
	{
		constexpr LogCategory appendCategory{ "" };
		Log(LogLevel::Append, appendCategory, format, args...);
	}
}
