#include "Path.h"

namespace Core::FileSystem
{
	Path::Path(Alloc::IAllocator& alloc) noexcept
		: m_string(alloc)
	{
	}

	Path::Path(const String& str) noexcept
		: m_string(str)
	{
	}

	Path::Path(const String& str, Alloc::IAllocator& alloc) noexcept
		: m_string(str, alloc)
	{
	}

	Path::Path(String&& str) noexcept
		: m_string(Move(str))
	{
	}

	Path::Path(String&& str, Alloc::IAllocator& alloc) noexcept
		: m_string(Move(str), alloc)
	{
	}

	auto Path::operator=(const String& str) noexcept -> Path&
	{
		m_string = str;
		return *this;
	}

	auto Path::operator=(String&& str) noexcept -> Path&
	{
		m_string = Move(str);
		return *this;
	}

	auto Path::operator+(const Path& other) const noexcept -> Path
	{
		Path path = *this;
		path.Concat(other);
		return path;
	}

	auto Path::operator/(const Path& other) const noexcept -> Path
	{
		Path path = *this;
		path.Append(other);
		return path;
	}

	auto Path::operator+=(const Path& other) noexcept -> Path&
	{
		return Concat(other);
	}

	auto Path::operator/=(const Path& other) noexcept -> Path&
	{
		return Append(other);
	}

	auto Path::operator==(const Path& other) const noexcept -> bool
	{
		return m_string == other.m_string;
	}

	auto Path::operator!=(const Path& other) const noexcept -> bool
	{
		return !(*this == other);
	}

	auto Path::Concat(const Path& other) noexcept -> Path&
	{
		m_string += other.m_string;
		return *this;
	}

	auto Path::Append(const Path& other) noexcept -> Path&
	{
		m_string += '/';
		m_string += other.m_string;
		return *this;
	}

	auto Path::Clear() noexcept -> void
	{
		// TODO: String::Clear() ???
		m_string = "";
	}

	auto Path::RemoveFileName() noexcept -> Path&
	{
		const usize idx = m_string.RFindFirstOf("/\\");
		m_string.Erase(idx);
		return *this;
	}

	auto Path::ReplaceFileName(const Path& filename) noexcept -> Path&
	{
		const usize idx = m_string.RFindFirstOf("/\\"_s);
		m_string.Replace(idx, String::NPos, filename.m_string);
		return *this;
	}

	auto Path::ReplaceExtension(const Path& extension) noexcept -> Path&
	{
		// If the path neds with a '/', don't add an extension
		const usize slashIdx = m_string.RFindFirstOf("/\\"_s);
		if (slashIdx == m_string.Length() - 1)
			return *this;

		const usize dotIdx = m_string.RFind('.');
		if (dotIdx != String::NPos && dotIdx > slashIdx)
			m_string.Replace(dotIdx, String::NPos, extension.m_string);
		else
			m_string.Add(extension.m_string);
		return *this;
	}

	auto Path::ReplaceMacros(Delegate<Path(const String&)>& macroQuery) noexcept -> Path&
	{
		usize idx = 0;
		while (true)
		{
			idx = m_string.Find("${", idx);
			if (idx == String::NPos)
				break;
			
			usize endIdx = m_string.Find('}', idx + 2);
			String macro = m_string.SubString(idx, endIdx - idx);
			Path path = macroQuery(macro);
			if (!path.IsEmpty())
				m_string.Replace(idx, endIdx - idx + 1, path.m_string);
			idx = endIdx + 1;
		}
		return *this;
	}

	auto Path::GetRootName() const noexcept -> Path
	{
#if PLATFORM_WINDOWS
		if (m_string.Length() <= 2)
			return Path{};

		// Check for local path
		if (m_string[1] == ':')
			return Path{ m_string.SubString(0, 2) };
		// Check for network path 
		if (m_string.StartsWith("\\\\"_s) || m_string.StartsWith("//"_s))
			return Path{ m_string.SubString(0, 2) };
		return Path{};
#else
		return Path{};
#endif
	}

	auto Path::GetRootDir() const noexcept -> Path
	{
#if PLATFORM_WINDOWS
		if (m_string.Length() <= 3)
			return Path{};

		// Check for local path
		if (m_string[1] == ':' && (m_string[2] == '\\' || m_string[2] == '//'))
			return Path{ m_string.SubString(2, 3) };
		// Check for network path 
		if (m_string.StartsWith("\\\\"_s) || m_string.StartsWith("//"_s))
		{
			const usize idx = m_string.FindFirstOf("/\\"_s, 2);
			if (idx == String::NPos)
				return *this;
			return Path{ m_string.SubString(2, idx + 1) };
		}
		return Path{};
#else
		if (m_string.Length() > 0 && (m_string[0] == '/' || m_string[0] == '\\'))
			return Path{ "/"_s };
#endif
	}

	auto Path::GetRootPath() const noexcept -> Path
	{
#if PLATFORM_WINDOWS
		if (m_string.Length() <= 3)
			return Path{};

		// Check for local path
		if (m_string[1] == ':' && (m_string[2] == '\\' || m_string[2] == '//'))
			return Path{ m_string.SubString(0, 3) };
		// Check for network path 
		if (m_string.StartsWith("\\\\"_s) || m_string.StartsWith("//"_s))
		{
			const usize idx = m_string.FindFirstOf("/\\"_s, 2);
			if (idx == String::NPos)
				return *this;
			return Path{ m_string.SubString(0, idx + 1) };
		}
		return Path{};
#else
		if (m_string.Length() > 0 && (m_string[0] == '/' || m_string[0] == '\\'))
			return Path{ "/"_s };
#endif
	}

	auto Path::GetParentPath() const noexcept -> Path
	{
		if (m_string.IsEmpty())
			return Path{};

		usize endIdx = m_string.Length() - 1;
		if (m_string.EndsWith("/"_s) || m_string.EndsWith("\\"_s))
			--endIdx;

		const usize idx = m_string.RFindFirstOf("/\\", endIdx);
		return Path{ m_string.SubString(0, idx) };
	}

	auto Path::GetRelativePath(const Path& rootPath) noexcept -> Path
	{
		if (m_string.StartsWith(rootPath.m_string))
			return Path{ m_string.SubString(rootPath.m_string.Length()) };
		return Path{};
	}

	auto Path::GetFileName() const noexcept -> Path
	{
		const usize idx = m_string.RFindFirstOf("/\\"_s);
		return Path{ m_string.SubString(idx) };
	}

	auto Path::GetStem() const noexcept -> Path
	{
		const usize dotIdx = m_string.RFind('.');
		if (dotIdx == String::NPos)
			return Path{ m_string };
		const usize slashIdx = m_string.RFindFirstOf("/\\"_s);

		// Avoid the case for a path like "bla.bla/"
		if (slashIdx == String::NPos || dotIdx > slashIdx)
			return Path{ m_string.SubString(0, dotIdx) };
		return Path{ m_string };
	}

	auto Path::GetExtension() const noexcept -> String
	{
		const usize dotIdx = m_string.RFind('.');
		if (dotIdx == String::NPos)
			return ""_s;

		// Avoid the case for a path like "bla.bla/"
		const usize slashIdx = m_string.RFindFirstOf("/\\"_s);
		if (slashIdx == String::NPos || dotIdx > slashIdx)
			return m_string.SubString(dotIdx + 1);
		return ""_s;
	}

	auto Path::HasRootName() const noexcept -> bool
	{
#if PLATFORM_WINDOWS
		return !m_string.IsEmpty() &&
			(m_string[1] == ':' || // Local path
			 m_string.StartsWith("\\\\"_s) || m_string.StartsWith("//"_s)); // Network path
#else
		return false;
#endif
	}

	auto Path::HasRootDir() const noexcept -> bool
	{
#if PLATFORM_WINDOWS
		return m_string.Length() >= 3 &&
			(m_string[1] == ':' && (m_string[2] == '\\' || m_string[2] == '/') ||
			 m_string.StartsWith("\\\\") || m_string.StartsWith("//"));
#else
		return m_string.StartsWith('/') || m_string.StartsWith('\\');
#endif
	}

	auto Path::HasRootPath() const noexcept -> bool
	{
		return HasRootDir();
	}

	auto Path::HasParentPath() const noexcept -> bool
	{
		usize lastSlash = m_string.RFindFirstNotOf("/\\"_s);
		if (lastSlash == m_string.Length() - 1)
			lastSlash = m_string.RFindFirstNotOf("/\\"_s, lastSlash - 1);
		return lastSlash != String::NPos;
	}

	auto Path::HasFileName() const noexcept -> bool
	{
		return !m_string.IsEmpty() && m_string.RFindFirstNotOf("/\\"_s) != m_string.Length() - 1;
	}

	auto Path::HasStem() const noexcept -> bool
	{
		return m_string.Find('.', 0, 1) != 0;
	}

	auto Path::HasExtension() const noexcept -> bool
	{
		const usize dotIdx = m_string.RFind('.');
		if (dotIdx == String::NPos)
			return false;
		const usize slashIdx = m_string.RFindFirstOf("/\\");
		return slashIdx == String::NPos || slashIdx < dotIdx;
	}

	auto Path::HasMacros() const noexcept -> bool
	{
		usize idx = m_string.Find("${");
		if (idx == String::NPos)
			return false;
		return m_string.Find('}', idx + 2) == String::NPos;
	}

	auto Path::IsAbsolute() const noexcept -> bool
	{
		return GetRootPath().m_string != "";
	}

	auto Path::IsRelative() const noexcept -> bool
	{
		return !IsAbsolute();
	}

	auto Path::IsEmpty() const noexcept -> bool
	{
		return m_string.IsEmpty();
	}

	auto Path::ToNative() noexcept -> Path&
	{
		m_string.Replace('/', NativePathSeparator);
		return *this;
	}

	auto Path::AsNative() const noexcept -> Path
	{
		Path path = *this;
		path.ToNative();
		return path;
	}

	auto Path::ToGeneral() noexcept -> Path&
	{
		m_string.Replace(NativePathSeparator, '/');
		return *this;
	}

	auto Path::AsGeneral() const noexcept -> Path
	{
		Path path = *this;
		path.ToGeneral();
		return path;
	}

	auto Path::GetString() const noexcept -> const String&
	{
		return m_string;
	}

	auto Path::GetString() noexcept -> String&
	{
		return m_string;
	}

	auto Path::Begin() const noexcept -> String::ConstIterator
	{
		return m_string.Begin();
	}

	auto Path::Begin() noexcept -> String::Iterator
	{
		return m_string.Begin();
	}

	auto Path::End() const noexcept -> String::ConstIterator
	{
		return m_string.End();
	}

	auto Path::End() noexcept -> String::Iterator
	{
		return m_string.End();
	}
}

namespace Literals
{
	auto operator ""_path(const char* cstr, usize size) noexcept -> Core::FileSystem::Path
	{
		return Core::FileSystem::Path{ Core::String{ cstr, size } };
	}

	auto operator ""_path(const char16_t* cstr, usize size) noexcept -> Core::FileSystem::Path
	{
		return Core::FileSystem::Path{ Core::String{ cstr, size } };
	}

	auto operator ""_path(const char32_t* cstr, usize size) noexcept -> Core::FileSystem::Path
	{
		return Core::FileSystem::Path{ Core::String{ cstr, size } };
	}
}
