#pragma once
#include "core/MinInclude.h"
#include "Enums.h"
#include "Path.h"
#include "Error.h"

namespace Core::FileSystem
{
	/**
	 * Options for filesystem entry iterations
	 */
	struct EnumerationOptions
	{
		FileAttributes toSkip       = FileAttribute::None; ///< Attributes to skip (skipped if any attribute matches)
		u16 maxResursionDepth       = u16(-1);             ///< Max recursion depth
		bool ignoreInaccessible : 1 = false;               ///< Skip the file if it cannot be accessed
		bool matchCase          : 1 = false;               ///< Match the case when matching against a pattern
		bool recurseSubDirs     : 1 = false;               ///< Recurse through sub-directories
		bool returnSpecialDirs  : 1 = false;               ///< Visit special directories like '.' and '..'
		bool onlyVisitFiles     : 1 = false;               ///< Only visit files
		bool onlyVisitDirs      : 1 = false;               ///< Only visit directories
	};

	struct Entry
	{
		Path path;               ///< Path to entry
		FileAttributes attribs;  ///< Attributes
		u64 creationTimeStamp;   ///< Timestamp of its creation (needs to be converted to know actual time)
		u64 lastAccessTimestamp; ///< Timestamp of its last access (needs to be converted to know actual time)
		u64 lastWriteTimestamp;  ///< Timestamp of its last write (needs to be converted to know actual time)
		u64 size;                ///< Size of the entry (invalid for directories)
	};

	enum class MoveFlag : u8
	{
		None               = 0     , ///< None
		ReplaceExisting    = BIT(0), ///< Replace the existing entry in the destination if it exists
		AllowBetweenDrives = BIT(1), ///< Allow the entry to be moved between drives (can be copy+delete)
		Wait               = BIT(3), ///< Wait until the move is finished before returning from the function
	};
	DEFINE_FLAGS(MoveFlag);

	/**
	 * Enumerate over all files using the provided delegate
	 * \param[in] del Delegate
	 * \param[in] path Path to directory to enumerate
	 * \param[in] recursive Whether to recursively iterate over all directories
	 * \return Result
	 * \note For more option, use EnumerateFileSystem using EnumerationOptions::onlyVisitFiles = true
	 */
	CORE_API auto EnumerateFiles(Delegate<void(const Entry&)>& del, const Path& path, bool recursive = false) noexcept -> Error;
	/**
	 * Enumerate over all directories using the provided delegate
	 * \param[in] del Delegate
	 * \param[in] path Path to directory to enumerate
	 * \param[in] recursive Whether to recursively iterate over all directories
	 * \return Result
	 * \note For more option, use EnumerateFileSystem using EnumerationOptions::onlyVisitDirs = true
	 */
	CORE_API auto EnumerateDirectories(Delegate<void(const Entry&)>& del, const Path& path, bool recursive = false) noexcept -> Error;
	/**
	 * Enumerate over all entries using the provided delegate
	 * \param[in] del Delegate
	 * \param[in] path Path to directory to enumerate
	 * \param[in] recursive Whether to recursively iterate over all directories
	 * \return Result
	 */
	CORE_API auto EnumerateFileSystem(Delegate<void(const Entry&)>& del, const Path& path, bool recursive = false) noexcept -> Error;
	/**
	 * Enumerate over all entries using the provided delegate
	 * \param[in] del Delegate
	 * \param[in] path Path to directory to enumerate
	 * \param pattern Pattern to match
	 * \param options Enumeration options
	 * \return Result
	 */
	CORE_API auto EnumerateFileSystem(Delegate<void(const Entry&)>& del, const Path& path, const Path& pattern, EnumerationOptions options = {}) noexcept -> Error;

	/**
	 * Check if the path points to a valid file or directory
	 * \param[in] path Path
	 * \return Whether the path points to a valid file or directory
	 */
	CORE_API auto Exists(const Path& path) noexcept -> bool;

	/**
	 * Get the file system entry at the given path
	 * \param[in] path Path
	 * \return Entry, empty if the path is invalid
	 */
	CORE_API auto GetEntry(const Path& path) noexcept -> Entry;
	/**
	 * Get the creation timestamp (needs to be converted to know actual time)
	 * \param[in] path Path
	 * \return Timestamp, 0 if the path is invalid
	 */
	CORE_API auto GetCreationTimestamp(const Path& path) noexcept -> u64;
	/**
	 * Get the last access timestamp (needs to be converted to know actual time)
	 * \param[in] path Path
	 * \return Timestamp, 0 if the path is invalid
	 */
	CORE_API auto GetLastAccessTimestamp(const Path& path) noexcept -> u64;
	/**
	 * Get the last write timestamp (needs to be converted to know actual time)
	 * \param[in] path Path
	 * \return Timestamp, 0 if the path is invalid
	 */
	CORE_API auto GetLastWriteTimestamp(const Path& path) noexcept -> u64;

	/**
	 * Move a file or directory to a new location
	 * \param[in] from Path to file/directory to move
	 * \param[in] to Move destination
	 * \param[in] flags Move flags
	 * \return Result
	 */
	CORE_API auto Move(const Path& from, const Path& to, MoveFlags flags) noexcept -> Error;
}

DEFINE_ENUM_FLAG_OPS(Core::FileSystem::MoveFlags);