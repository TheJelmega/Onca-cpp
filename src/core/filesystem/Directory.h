#pragma once
#include "core/MinInclude.h"
#include "Error.h"
#include "Path.h"

namespace Core::FileSystem
{
	/**
	 * Get the current working directory
	 * \return Path to working directory
	 */
	CORE_API auto GetCurrentWorkingDirectory() noexcept -> Path;
	/**
	 * Set the current working directory
	 * \param[in] path New working directory
	 * \return Result
	 */
	CORE_API auto SetCurrentWorkingDirectory(const Path& path) noexcept -> Error;

	/**
	 * Create a new directory
	 * \param[in] path Path to directory
	 * \return Result
	 */
	CORE_API auto CreateDirectory(const Path& path) noexcept -> Error;
	/**
	 * Delete a directory (optionally only when empty)
	 * \param[in] path Path to directory
	 * \param[in] recursively Whether to recursively delete the content of the directory
	 * \return Result
	 */
	CORE_API auto DeleteDirectory(const Path& path, bool recursively = false) noexcept -> Error;

	/**
	 * Check if a path points to a directory
	 * \param[in] path Path
	 * \return Whether the path points to a directory
	 */
	CORE_API auto IsDirectory(const Path& path) noexcept -> bool;

	/**
	 * Get an array with logical drive names
	 * \return Array with logical drive names
	 */
	CORE_API auto GetLogicalDrives() noexcept -> DynArray<Path>;

}
