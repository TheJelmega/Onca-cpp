#include "../WindowManager.h"

#if PLATFORM_WINDOWS
#include "core/platform/Platform.h"

namespace Core
{
	auto EnumMonitors(HMONITOR monitor, HDC, LPRECT, LPARAM userData) -> BOOL
	{
		WindowManager* pWinManager = reinterpret_cast<WindowManager*>(userData);
		pWinManager->AddMonitorFromNativeHandle(monitor);
		return true;
	}

	auto WindowManager::CollectMonitors() noexcept -> void
	{
		::EnumDisplayMonitors(nullptr, nullptr, &EnumMonitors, reinterpret_cast<LPARAM>(this));
	}

	auto WindowManager::PlatformTick() noexcept -> void
	{
		for (const Window& window : m_windows)
		{
			HWND hwnd = static_cast<HWND>(window.GetNativeHandle());

			MSG msg = {};
			while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
	}
}

#endif