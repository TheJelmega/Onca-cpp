#pragma once
#include "SystemError.h"
#include "core/MinInclude.h"
#include "core/filesystem/Path.h"
#include "core/utils/Result.h"

namespace Onca
{
	/**
	 * Dynamic Library (dll/so)
	 */
	class CORE_API DynLib
	{
	public:
		DEFINE_OPAQUE_HANDLE(NativeHandle);

		DISABLE_COPY(DynLib);

		DynLib();
		
		DynLib(DynLib&& other) noexcept;
		~DynLib();
		
		auto operator=(DynLib&& other) noexcept -> DynLib&;

		/**
		 * Get a function by name
		 * \param[in] name Name of the function
		 * \return Result with pointer to a function or an error
		 */
		auto GetFunc(const String& name) const noexcept -> Result<void*, SystemError>;

		/**
		 * Get the native handle
		 * \return Native handle
		 */
		auto GetNativeHandle() const noexcept -> NativeHandle;
		/**
		 * Check if the dynamic library is valid
		 * \return Whether the dynamic library is valid
		 */
		auto IsValid() const noexcept -> bool;

		/**
		 * Load a dynamic library
		 * \param[in] path Path the dynamic library
		 * \return Dynamic library
		 */
		static auto Create(const FileSystem::Path& path) noexcept -> Result<DynLib, SystemError>;

	private:
		NativeHandle m_handle; ///< Handle to dynamic library
	};
}
