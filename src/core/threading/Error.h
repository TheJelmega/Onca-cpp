#pragma once
#include "core/MinInclude.h"
#include "core/string/Include.h"

namespace Core::Threading
{
	enum class ErrorCode
	{
		Success        , ///< No error

		InvalidHandle  , ///< Invalid thread handle
		NotEnoughMemory, ///< Cannot create thread since not enough memory is available for the stack
		CouldNotSetDesc, ///< Could not set the thread description

		Unknown        , ///< Unknown error
	};

	constexpr const char* DefaultErrorMessages[] =
	{
		"No error",
		"Invalid thread handle",
		"Cannot create thread since not enough memory is available for the stack",
		"Could not set the thread description",
	};
	constexpr usize NumDefErrMessages = ArraySize(DefaultErrorMessages);

	struct Error
	{
		Error() noexcept {}
		Error(ErrorCode code) noexcept
			: code(code)
		{
			if (u8(code) < NumDefErrMessages)
				info = DefaultErrorMessages[u8(code)];
		}

		Error(ErrorCode code, const String& str) noexcept : code(code), info(str) {}

		ErrorCode code = ErrorCode::Success;
		String info;

		auto Succeeded() const noexcept -> bool { return code == ErrorCode::Success; }
		operator ErrorCode() const noexcept { return code; }
		operator bool() const noexcept { return Succeeded(); }
	};

#if PLATFORM_WINDOWS
	/**
	 * Translate the native windows error to a threading error
	 */
	auto TranslateError() noexcept -> Error;
#endif
}