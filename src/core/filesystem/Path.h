#pragma once
#include "core/MinInclude.h"
#include "core/string/Include.h"
#include "core/utils/Delegate.h"

namespace Core::FileSystem
{

	/**
	 * File path
	 */
	class CORE_API Path
	{
	public:

#if PLATFORM_WINDOWS
		constexpr static UCodepoint NativePathSeparator = '\\';
#else
		constexpr static UCodepoint NativePathSeparator = '/';
#endif

		/**
		 * Create an empty path
		 * \param[in] alloc Allocator
		 */
		Path(Alloc::IAllocator& alloc = g_GlobalAlloc) noexcept;
		/**
		 * Create a path from a string
		 * \param[in] str String
		 * \param[in] alloc Allocator
		 */
		explicit Path(const String& str) noexcept;
		/**
		 * Create a path from a string
		 * \param[in] str String
		 */
		explicit Path(const String& str, Alloc::IAllocator& alloc) noexcept;
		/**
		 * Create a path from a moved string
		 * \param[in] str String
		 */
		explicit Path(String&& str) noexcept;
		/**
		 * Create a path from a moved string
		 * \param[in] str String
		 * \param[in] alloc Allocator
		 */
		explicit Path(String&& str, Alloc::IAllocator& alloc) noexcept;

		auto operator=(const String& str) noexcept -> Path&;
		auto operator=(String&& str) noexcept -> Path&;

		auto operator+(const Path& other) const noexcept -> Path;
		auto operator/(const Path& other) const noexcept -> Path;

		auto operator+=(const Path& other) noexcept -> Path&;
		auto operator/=(const Path& other) noexcept -> Path&;

		auto operator==(const Path& other) const noexcept -> bool;
		auto operator!=(const Path& other) const noexcept -> bool;

		/**
		 * Concatenate the given paths without a path separator
		 * \param[in] other Other path
		 * \return Concatenated path
		 */
		auto Concat(const Path& other) noexcept -> Path&;
		/**
		 * Concatenate the given paths with a path separator
		 * \param[in] other Other path
		 * \return Concatenated path
		 */
		auto Append(const Path& other) noexcept -> Path&;
		/**
		 * Clear the contents of the path
		 */
		auto Clear() noexcept -> void;

		/**
		 * Remove the file name from the path
		 * \return Reference to the filepath
		 */
		auto RemoveFileName() noexcept -> Path&;
		/**
		 * Replace the filename
		 * \param[in] filename New filename
		 * \return Reference to the string
		 * \note If no file name exists, the filename gets appended to the path
		 */
		auto ReplaceFileName(const Path& filename) noexcept -> Path&;
		/**
		 * Replace the file extension
		 * \param[in] extension New extension
		 * \return Reference to the string
		 * \note If no file extension exists, the filename gets appended to the path
		 */
		auto ReplaceExtension(const Path& extension) noexcept -> Path&;
		/**
		 * Replace the macros in the string using the provided query delegate
		 * \param[in] macroQuery Query to get the macro path based on its identifier
		 * \return Reference to the path
		 */
		auto ReplaceMacros(Delegate<Path(const String&)>& macroQuery) noexcept -> Path&;

		/**
		 * Get the root name
		 * Examples:
		 * - windows local: "C:"
		 * - windows network: "\\RootName"
		 * - Posix: no root name, as posix does not have this
		 * \return Root name
		 */
		auto GetRootName() const noexcept -> Path;
		/**
		 * Get the root directory
		 * Examples:
		 * - windows local: "C:\"
		 * - windows network: "\\RootName\"
		 * - Posix: "/"
		 * \return Root directory
		 */
		auto GetRootDir() const noexcept -> Path;
		/**
		 * Get the root directory
		 * Examples:
		 * - windows local: "\"
		 * - windows network: "RootName\" (first 'directory' after '\\')
		 * - Posix: "/"
		 * \return Root directory
		 */
		auto GetRootPath() const noexcept -> Path;
		/**
		 * Get the parent path
		 * \return Parent path
		 */
		auto GetParentPath() const noexcept -> Path;
		/**
		 * Get a path relative to the root path
		 * \param[in] rootPath Root path
		 * \return Relative path
		 * \note if the root path is not the root path of this string, an empty path gets returned
		 */
		auto GetRelativePath(const Path& rootPath) noexcept -> Path;
		/**
		 * Get the file name
		 * \return File name
		 */
		auto GetFileName() const noexcept -> Path;
		/**
		 * Get the stem of the path (filepath without extension)
		 * \return Stem path
		 */
		auto GetStem() const noexcept -> Path;
		/**
		 * Get the file extension
		 * \return File extension
		 */
		auto GetExtension() const noexcept -> String;

		/**
		 * Check if the path has a root name
		 * \return Whether the path has a root name
		 */
		auto HasRootName() const noexcept -> bool;
		/**
		 * Check if the path has a root directory
		 * \return Whether the path has a root directory
		 */
		auto HasRootDir() const noexcept -> bool;
		/**
		 * Check if the path has a root path
		 * \return Whether the path has a root path
		 */
		auto HasRootPath() const noexcept -> bool;
		/**
		 * Check if the path has a parent path
		 * \return Whether the path has a parent path
		 */
		auto HasParentPath() const noexcept -> bool;
		/**
		 * Check if the path has a file name
		 * \return Whether the path has a file name
		 */
		auto HasFileName() const noexcept -> bool;
		/**
		 * Check if the path has a stem
		 * \return Whether the path has a stem
		 */
		auto HasStem() const noexcept -> bool;
		/**
		 * Check if the path has an extension
		 * \return Whether the path has an extension
		 */
		auto HasExtension() const noexcept -> bool;

		/**
		 * Check if the path contains macros
		 * \return Whether the path contains macros
		 */
		auto HasMacros() const noexcept -> bool;

		/**
		 * Check if the path is absolute
		 * \return Whether the path is absolute
		 */
		auto IsAbsolute() const noexcept -> bool;
		/**
		 * Check if the path is relative
		 * \return Whether the path is relative
		 */
		auto IsRelative() const noexcept -> bool;

		/**
		 * Check if the string is empty
		 * \return Whether the path is empty
		 */
		auto IsEmpty() const noexcept -> bool;

		/**
		 * Get a copy of the string using the native path separator
		 * \return Native path
		 */
		auto ToNative() noexcept -> Path&;
		/**
		 * Make string use the native path separator
		 * \return Reference to the path
		 */
		auto AsNative() const noexcept -> Path;
		/**
		 * Get a copy of the string using the general path separator '/'
		 * \return Native path
		 */
		auto ToGeneral() noexcept -> Path&;
		/**
		 * Make string use the general path separator '/'
		 * \return Native path
		 */
		auto AsGeneral() const noexcept -> Path;

		/**
		 * Get the underlying string
		 * \return Underlying string
		 */
		auto GetString() const noexcept -> const String&;
		/**
		 * Get the underlying string
		 * \return Underlying string
		 */
		auto GetString() noexcept -> String&;

		/**
		 * Get the iterator to the first character of the underlying string
		 * \return Iterator to the first character of the underlying string
		 */
		auto Begin() const noexcept -> String::ConstIterator;
		/**
		 * Get the iterator to the first character of the underlying string
		 * \return Iterator to the first character of the underlying string
		 */
		auto Begin() noexcept -> String::Iterator;
		/**
		 * Get the iterator to the end of the underlying string
		 * \return Iterator to the end of the underlying string
		 */
		auto End() const noexcept -> String::ConstIterator;
		/**
		 * Get the iterator to the end of the underlying string
		 * \return Iterator to the end of the underlying string
		 */
		auto End() noexcept -> String::Iterator;

	private:
		String m_string;
	};

}

namespace Literals
{
	CORE_API auto operator""_path(const char* cstr, usize size) noexcept -> Core::FileSystem::Path;
	CORE_API auto operator""_path(const char16_t* cstr, usize size) noexcept -> Core::FileSystem::Path;
	CORE_API auto operator""_path(const char32_t* cstr, usize size) noexcept -> Core::FileSystem::Path;
}
