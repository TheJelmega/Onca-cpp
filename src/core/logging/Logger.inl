#pragma once
#if __RESHARPER__
#include "Logger.h"
#endif

namespace Core
{
	template <typename ... Args>
	auto Logger::Log(LogLevel level, const LogCategory& category, const String& format, const Args&... args) noexcept -> void
	{
		String message = Format(format, args...);
		Log(level, category, message);
	}

	template <typename ... Args>
	auto Logger::Severe(const LogCategory& category, const String& format, const Args&... args) noexcept -> void
	{
		Log(LogLevel::Severe, category, format, args...);
	}

	template <typename ... Args>
	auto Logger::Error(const LogCategory& category, const String& format, const Args&... args) noexcept -> void
	{
		Log(LogLevel::Error, category, format, args...);
	}

	template <typename ... Args>
	auto Logger::Warning(const LogCategory& category, const String& format, const Args&... args) noexcept -> void
	{
		Log(LogLevel::Warning, category, format, args...);
	}

	template <typename ... Args>
	auto Logger::Info(const LogCategory& category, const String& format, const Args&... args) noexcept -> void
	{
		Log(LogLevel::Info, category, format, args...);
	}

	template <typename ... Args>
	auto Logger::Verbose(const LogCategory& category, const String& format, const Args&... args) noexcept -> void
	{
		Log(LogLevel::Verbose, category, format, args...);
	}

	template <typename ... Args>
	auto Logger::Append(const String& format, const Args&... args) noexcept -> void
	{
		constexpr LogCategory appendCategory{ "" };
		Log(LogLevel::Append, appendCategory, format, args...);
	}
}
