#include "../DynLib.h"

#if PLATFORM_WINDOWS
#include "Win.h"

namespace Core
{
	DynLib::DynLib()
		: m_handle(nullptr)
	{
	}

	DynLib::DynLib(DynLib&& other) noexcept
		: m_handle(other.m_handle)
	{
		other.m_handle = nullptr;
	}

	DynLib::~DynLib()
	{
		if (IsValid())
			::FreeLibrary(reinterpret_cast<HMODULE>(m_handle));
	}

	auto DynLib::operator=(DynLib&& other) noexcept -> DynLib&
	{
		if (IsValid())
			this->~DynLib();

		m_handle = other.m_handle;
		other.m_handle = nullptr;
		return *this;
	}

	auto DynLib::GetFunc(const String& name) const noexcept -> Result<void*, SystemError>
	{
		void* proc = ::GetProcAddress(reinterpret_cast<HMODULE>(m_handle), reinterpret_cast<LPCSTR>(name.Data()));
		if (!proc)
			return TranslateSystemError();
		return proc;
	}

	auto DynLib::GetNativeHandle() const noexcept -> NativeHandle
	{
		return m_handle;
	}

	auto DynLib::IsValid() const noexcept -> bool
	{
		return m_handle;
	}

	auto DynLib::Create(FileSystem::Path& path) noexcept -> Result<DynLib, SystemError>
	{
		DynLib dynLib;
		DynArray<char16_t> utf16 = ((path.HasParentPath() ? "\\\\?\\"_path : ""_path) + path.AsAbsolute()).ToNative().GetString().ToUtf16();
		dynLib.m_handle = LoadLibraryW(reinterpret_cast<LPCWSTR>(utf16.Data()));

		if (!dynLib.IsValid())
			return TranslateSystemError();
		return dynLib;
	}
}

#endif