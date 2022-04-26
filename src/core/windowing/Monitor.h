#pragma once
#include "core/MinInclude.h"
#include "core/string/Include.h"
#include "core/math/Math.h"

namespace Onca
{
	class Monitor
	{
	public:
		DEFINE_OPAQUE_HANDLE(NativeHandle);

		Monitor() noexcept;
		explicit Monitor(NativeHandle handle) noexcept;

		/**
		 * Get the monitor name
		 * \return Monitor name
		 */
		auto GetName() const noexcept -> const String& { return m_name; }
		/**
		 * Get the monitor size
		 * \return Monitor size
		 */
		auto GetSize() const noexcept -> u32v2 { return m_size; }
		/**
		 * Get the monitor origin
		 * \return Monitor origin
		 */
		auto GetOrigin() const noexcept -> i32v2 { return m_origin; }
		/**
		 * Get the work region size  (work region == monitor without the OS UI (e.g. taskbar))
		 * \return Work region size
		 */
		auto GetWorkSize() const noexcept -> u32v2 { return m_size; }
		/**
		 * Get the work region origin (work region == monitor without the OS UI (e.g. taskbar))
		 * \return Work region origin
		 */
		auto GetWorkOrigin() const noexcept -> i32v2 { return m_origin; }
		/**
		 * Get the native monitor handle
		 * \return Native monitor handle
		 */
		auto GetNativeHandle() const noexcept -> NativeHandle { return m_handle; }
		/**
		 * Check if the monitor is the primary monitor
		 * \return Whether the monitor is the primary monitor
		 */
		auto IsPrimary() const noexcept -> bool { return m_isPrimary; }

	private:
		String       m_name;       ///< Monitor name
		NativeHandle m_handle;     ///< Handle to the monitor
		u32v2        m_size;       ///< Size of the monitor
		i32v2        m_origin;     ///< Origin of the monitor
		u32v2        m_workSize;   ///< Size of the monitor without the OS UI (e.g. taskbar)
		u32v2        m_workOrigin; ///< Origin of the monitor without the OS UI (e.g. taskbar)
		bool         m_isPrimary;  ///< Whether the monitor is the primary monitor
	};
}
