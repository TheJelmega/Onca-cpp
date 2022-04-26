#include "WindowManager.h"

#include "core/logging/Logger.h"


namespace Core
{
	WindowManager::WindowManager() noexcept
		: m_threadId(Threading::GetCurrentThreadId())
	{
	}

	WindowManager::WindowManager(WindowManager&& other) noexcept
		: m_monitors(Move(other.m_monitors))
		, m_windows(Move(other.m_windows))
		, m_threadId(other.m_threadId)
	{
	}

	auto WindowManager::operator=(WindowManager&& other) noexcept -> WindowManager&
	{
		m_monitors = Move(other.m_monitors);
		m_windows = Move(other.m_windows);
		m_threadId = other.m_threadId;

		return *this;
	}

	auto WindowManager::AddMonitorFromNativeHandle(void* pHandle) noexcept -> void
	{
		for (Monitor& monitor : m_monitors)
		{
			if (monitor.GetNativeHandle() == pHandle)
				return;
		}

		Monitor monitor{ pHandle };
		if (monitor.IsPrimary())
			m_monitors.Insert(m_monitors.Begin(), Move(monitor));
		else
			m_monitors.Add(Move(monitor));
	}

	auto WindowManager::GetMonitor(usize idx) noexcept -> const Monitor&
	{
		static Monitor invalidMonitor;
		if (idx >= m_monitors.Size())
			return invalidMonitor;
		return m_monitors[idx];
	}

	auto WindowManager::GetPrimaryMonitor() noexcept -> const Monitor&
	{
		static Monitor invalidMonitor;
		if (m_monitors.IsEmpty())
			return invalidMonitor;
		return m_monitors[0];
	}

	auto WindowManager::CreateWindow(WindowCreationInfo& info) noexcept -> Window&
	{
		Threading::ThreadID curThread = Threading::GetCurrentThreadId();
		if (curThread != m_threadId)
		{
			g_Logger.Warning(LogCategories::WINDOWING, "Trying to call WindowManager::CreateWindow() on thread {}, expected thread {}"_s, curThread, m_threadId);
			return m_invalidWindow;
		}

		u32 id = 0;
		for (; id < m_windows.Size(); ++id)
		{
			if (!m_windows[id].IsValid())
				break;
		}

		Window* pParent = nullptr;
		if (info.parentId < m_windows.Size())
		{
			if (m_windows[info.parentId].IsValid())
				pParent = &m_windows[info.parentId];
		}

		Result<Window, SystemError> res = Window::Create(this, info, id, pParent);
		if (res.Failed())
		{
			g_Logger.Error(LogCategories::WINDOWING, "Failed to create window, got error: {}"_s, res.Error().info);
			return m_invalidWindow;
		}

		m_windows.Add(res.MoveValue());
		return m_windows.Back();
	}

	auto WindowManager::GetWindow(u32 id) noexcept -> Window&
	{
		if (id < m_windows.Size() && m_windows[id].IsValid())
			return m_windows[id];
		return m_invalidWindow;
	}

	auto WindowManager::Tick() noexcept -> void
	{
		Threading::ThreadID curThread = Threading::GetCurrentThreadId();
		if (curThread != m_threadId)
		{
			g_Logger.Warning(LogCategories::WINDOWING, "Trying to call WindowManager::Tick() on thread {}, expected thread {}"_s, curThread, m_threadId);
			return;
		}

		PlatformTick();
	}
}
