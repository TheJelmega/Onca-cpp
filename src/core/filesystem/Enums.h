#pragma once
#include "core/MinInclude.h"
#include "core/utils/Flags.h"

namespace Core::FileSystem
{
	enum class FileAttribute : u32
	{
		None         = 0      , ///< None
		ReadOnly     = BIT(0) , ///< Entry is read-only
		Hidden       = BIT(1) , ///< Entry is hidden
		System       = BIT(2) , ///< Entry is part of the OS
		Directory    = BIT(4) , ///< Entry is a directory 
		Normal       = BIT(7) , ///< Entry has no special attributes
		Temporary    = BIT(8) , ///< Entry is temporary, meaning it can be deleted when the application is not running
		Sparse       = BIT(9) , ///< Entry is sparse
		ReparsePoint = BIT(10), ///< Entry is a reparse point
		Compressed   = BIT(11), ///< Entry is compressed
		Encrypted    = BIT(14), ///< Entry is encrypted

		Mask = ReadOnly |
		Hidden		    |
		System		    |
		Directory	    |
		Normal		    |
		Temporary	    |
		Sparse		    |
		ReparsePoint    |
		Compressed      |
		Encrypted
	};
	DEFINE_FLAGS(FileAttribute);

	enum class AccessMode : u8
	{
		Read	  = BIT(0)             , ///< Read access
		Write	  = BIT(1)             , ///< Write access
		Execute	  = BIT(2)             , ///< Execute access
		ReadWrite = Read | Write       , ///< Read and write access
		All       = ReadWrite | Execute, ///< Read, write and execute access
	};
	DEFINE_FLAGS(AccessMode);

	enum class ShareMode : u8
	{
		None   = 0     , ///< None
		Read   = BIT(0), ///< Share read access
		Write  = BIT(1), ///< Share write access
		Delete = BIT(2), ///< Share delete access
	};
	DEFINE_FLAGS(ShareMode);

	enum class FileCreateKind : u8
	{
		CreateAlways = 2, ///< Create a file, replace the old file if it exists
		CreateNew    = 1, ///< Create a file, only when it does not exist
		CreateOpen   = 4, ///< Open the file if it exists, otherwise create it
	};

	enum class FileFlag
	{
		None          = 0      , ///< None
		DeleteOnClose = BIT(10), ///< Delete the file when all handles to it are closed
		Unbuffered    = BIT(13), ///< Unbuffered read/write
		AllowAsync    = BIT(14), ///< Allow async read/write
		RandomAccess  = BIT(12), ///< Random access hint, might optimize buffered access
		Sequential    = BIT(11), ///< Sequential access hint, might optimize buffered access
		WriteThrough  = BIT(15), ///< Write directly to the file, the write might be buffered by the system, but is guaranteed to write directly to the drive
	};
	DEFINE_FLAGS(FileFlag)

	enum class SeekDir
	{
		Begin,   ///< Seek from the start of the file
		Current, ///< Seek from the current location
		End,     ///< Seek from the end of the file
	};

}

DEFINE_ENUM_FLAG_OPS(Core::FileSystem::FileAttribute);
DEFINE_ENUM_FLAG_OPS(Core::FileSystem::AccessMode);
DEFINE_ENUM_FLAG_OPS(Core::FileSystem::ShareMode);
DEFINE_ENUM_FLAG_OPS(Core::FileSystem::FileFlag);