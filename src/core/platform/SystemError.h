#pragma once
#include "core/MinInclude.h"
#include "core/string/Include.h"
#include "core/utils/Utils.h"

namespace Onca
{
	// There might be a couple error codes missing
	enum class SystemErrorCode : u8
	{
		// Success
		Success,         ///< No error

		// Errors
		InvalidHandle,   ///< Invalid handle
		InvalidPath,     ///< The specified pathname is invalid
		ExpectedFile,    ///< Expected a file
		NoAsyncSupport,  ///< File handle does not support async
		NoReadPerms,     ///< File does not have read permission
		NoWritePerms,    ///< File does not have read permission
		OffOutOfRange,   ///< File offset is out of range
		AsyncIncomplete, ///< Async operation is incomplete
		AsyncAbandoned,  ///< The async I/O operation was abandoned
		WriteFault,      ///< Could not write to the directory/file
		ReadFault,       ///< Could not read from the directory/file
		ShareViolation,  ///< Cannot access file, because it is being used by another process
		LockViolation,   ///< Cannot access file, because it is locked by another process
		WriteProtected,  ///< Directory/file is write protected
		AccessDenied,    ///< Access is denied
		AlreadyExists,   ///< Cannot create a file that already exists
		DirNotEmpty,     ///< The directory is not empty
		CouldNotOpen,    ///< System could not open the directory/file
		DriveFull,       ///< The drive is full
		CannotCopy,      ///< Cannot copy file, reason might be unknown
		DeletePending,   ///< File cannot be opened, because it is in the process of being deleted
		NegativeSeek,    ///< An attempt was made to move the file pointer before the beginning of the file
		
		NotEnoughMemory, ///< Cannot create thread since not enough memory is available for the stack
		CouldNotSetDesc, ///< Could not set the thread description

		Unknown = 0xFF,  ///< Unknown error
	};

	// TODO: should be replaced by relfection system enum -> string/c-string converter
	constexpr const char* DefaultSystemErrorMessages[] =
	{
		""                                                        , ///< Success
		"Invalid handle"                                          , ///< InvalidHandle
		"Invalid path"                                            , ///< InvalidPath  
		"Expected a file"                                         , ///< ExpectedFile
		"Handle does not have async support"                      , ///< NoAsyncSupport
		"Handle does not have read permissions"                   , ///< NoReadPerms
		"Handle does not have write permissions"                  , ///< NoWritePerms
		"File offset out of range"                                , ///< OffOutOfRange
		"Asynchronous operation is incomplete"                    , ///< AsyncIncomplete
		"Asynchronous operation is abandoned"                     , ///< AsyncAbandoned
		"A fault happened when writing"                           , ///< WriteFault
		"A fault happened when reading"                           , ///< ReadFault
		"Handle is being used by another process and isn't shared", ///< ShareViolation
		"Handle has been locked by another application"           , ///< LockViolation
		"Handle is write protected"                               , ///< WriteProtected
		"Access to handle has been denied"                        , ///< AccessDenied
		"The directory or file already exists"                    , ///< AlreadyExists
		"Directory is not empty"                                  , ///< DirNotEmpty
		"Could not open the handle"                               , ///< CouldNotOpen
		"The drive is full"                                       , ///< DriveFull
		"Could not copy the directory or file"                    , ///< CannotCopy
		"A delete of the directory or file is pending"            , ///< DeletePending

		"Not enough memory is available"                          , ///< NotEnoughMemory
		"Could not set value"                                     , ///< CouldNotSetDesc
	};
	constexpr usize NumDefErrMessages = ArraySize(DefaultSystemErrorMessages);

	struct SystemError
	{
		SystemError() noexcept {}
		SystemError(SystemErrorCode code) noexcept
			: code(code)
		{
			if (u8(code) < NumDefErrMessages)
				info = DefaultSystemErrorMessages[u8(code)];
		}

		SystemError(SystemErrorCode code, const String& str) noexcept : code(code), info(str) {}

		SystemError(const SystemError&) noexcept = default;
		SystemError(SystemError&&) noexcept = default;

		auto operator=(const SystemError&) noexcept -> SystemError& = default;
		auto operator=(SystemError&&) noexcept -> SystemError & = default;

		SystemErrorCode code = SystemErrorCode::Success;
		String info;

		auto Succeeded() const noexcept -> bool { return code == SystemErrorCode::Success; }
		operator SystemErrorCode() const noexcept { return code; }
		operator bool() const noexcept { return Succeeded(); }
	};

#if PLATFORM_WINDOWS
	/**
	 * Translate the native windows error to a file system error
	 */
	CORE_API auto TranslateSystemError() noexcept -> SystemError;
#endif
}
