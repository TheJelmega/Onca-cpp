#include "../Monitor.h"

#if PLATFORM_WINDOWS
#include "core/platform/Platform.h"

namespace Core
{
	Monitor::Monitor() noexcept
		: m_handle(nullptr)
		, m_isPrimary(false)
	{
	}

	Monitor::Monitor(NativeHandle handle) noexcept
		: m_handle(handle)
	{
		MONITORINFO info = {};
		info.cbSize = sizeof(MONITORINFO);
		::GetMonitorInfoW(static_cast<HMONITOR>(m_handle), &info);

		m_origin     = { info.rcMonitor.left, info.rcMonitor.top };
		m_size       = { info.rcMonitor.right - info.rcMonitor.left, info.rcMonitor.bottom - info.rcMonitor.top };
		m_workOrigin = { info.rcWork.left, info.rcWork.top };
		m_workSize   = { info.rcWork.right - info.rcWork.left, info.rcWork.bottom - info.rcWork.top };
		m_isPrimary = info.dwFlags & MONITORINFOF_PRIMARY;

		MONITORINFOEXW nameInfo = {};
		nameInfo.cbSize = sizeof(MONITORINFOEXW);
		::GetMonitorInfoW(static_cast<HMONITOR>(m_handle), &nameInfo);
		m_name.Assign(reinterpret_cast<const char16_t*>(nameInfo.szDevice));
	}
}

#endif
