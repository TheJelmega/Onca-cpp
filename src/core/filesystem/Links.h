#pragma once
#include "core/MinInclude.h"
#include "Path.h"
#include "Error.h"

namespace Core::FileSystem
{

	/**
	 * Create a symlink to a target location
	 * \param[in] path Path to the symlink
	 * \param[in] target Target of the symlink
	 * \return Error
	 * \note Symlinks can be used in the other filesystem functions
	 */
	CORE_API auto CreateSymLink(const Path& path, const Path& target) noexcept -> Error;

	/**
	 * Create a hardlink to a file
	 * \param[in] path Path to the hardlink
	 * \param[in] target Target of the hardlink
	 * \return Error
	 * \note This function can only be used for files
	 * \note On systems that do not distinguish between sym or hard links, this will create a symlink
	 */
	CORE_API auto CreateHardLink(const Path& path, const Path& target) noexcept -> Error;
	
}
