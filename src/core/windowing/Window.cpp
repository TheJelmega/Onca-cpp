#include "Window.h"

namespace Core
{
	template<typename T>
	auto RemoveForObject(DynArray<T>& callbacks, void* ptr) noexcept -> void
	{
		for (usize i = 0; i < callbacks.Size();)
		{
			if (callbacks[i].IsCalledOn(ptr))
				callbacks.Erase(callbacks.IteratorAt(i));
			else
				++i;
		}
	}

	auto Window::RemoveCallbacksForObject(void* ptr) noexcept -> void
	{
		RemoveForObject(m_closeCallbacks, ptr);
		RemoveForObject(m_sizeCallbacks, ptr);
		RemoveForObject(m_moveCallbacks, ptr);
		RemoveForObject(m_minimizedCallbacks, ptr);
		RemoveForObject(m_maximizedCallbacks, ptr);
		RemoveForObject(m_restoredCallbacks, ptr);
	}

	auto Window::RegisterCloseCallback(const Delegate<void(Window&)>& callback) noexcept -> void
	{
		m_closeCallbacks.Add(callback);
	}

	auto Window::RegisterInputEnableCallback(const Delegate<void(Window&, bool)>& callback) noexcept -> void
	{
		m_inputEnableCallbacks.Add(callback);
	}

	auto Window::RegisterSizeCallback(const Delegate<void(Window&, u32v2, bool)>& callback) noexcept -> void
	{
		m_sizeCallbacks.Add(callback);
	}

	auto Window::RegisterMoveCallback(const Delegate<void(Window&, i32v2, bool)>& callback) noexcept -> void
	{
		m_moveCallbacks.Add(callback);
	}

	auto Window::RegisterMinimizedCallback(const Delegate<void(Window&)>& callback) noexcept -> void
	{
		m_minimizedCallbacks.Add(callback);
	}

	auto Window::RegisterMaximizedCallback(const Delegate<void(Window&)>& callback) noexcept -> void
	{
		m_maximizedCallbacks.Add(callback);
	}

	auto Window::RegisterRestoreCallback(const Delegate<void(Window&, u32v2, i32v2, bool)>& callback) noexcept -> void
	{
		m_restoredCallbacks.Add(callback);
	}

	auto Window::RegisterFocusCallback(const Delegate<void(Window&, bool)>& callback) noexcept -> void
	{
		m_focusCallbacks.Add(callback);
	}

	auto Window::RegisterEnterLeaveCallback(const Delegate<void(Window&, bool)>& callback) noexcept -> void
	{
		m_enterLeaveCallbacks.Add(callback);
	}

	auto Window::RegisterShowCallback(const Delegate<void(Window&, bool)>& callback) noexcept -> void
	{
		m_showCallbacks.Add(callback);
	}

	auto Window::OnClose() noexcept -> void
	{
		for (Delegate<void(Window&)>& callback : m_closeCallbacks)
		{
			callback(*this);
		}
	}

	auto Window::OnInputEnabled(bool enabled) noexcept -> void
	{
		m_inputEnabled = enabled;
		for (Delegate<void(Window& , bool)>& callback : m_inputEnableCallbacks)
		{
			callback(*this, enabled);
		}
	}

	auto Window::OnResized(u32v2 size) noexcept -> void
	{
		for (Delegate<void(Window&, u32v2, bool)>& callback : m_sizeCallbacks)
		{
			callback(*this, size, true);
		}
		m_size = size;
		m_endedMoveSize = false;
	}

	auto Window::OnMoved(i32v2 pos) noexcept -> void
	{
		for (Delegate<void(Window&, i32v2, bool)>& callback : m_moveCallbacks)
		{
			callback(*this, pos, true);
		}
		m_position = pos;
		m_endedMoveSize = false;
	}

	auto Window::OnMinimized() noexcept -> void
	{
		for (Delegate<void(Window&)>& callback : m_minimizedCallbacks)
		{
			callback(*this);
		}
	}

	auto Window::OnMaximized() noexcept -> void
	{
		for (Delegate<void(Window&)>& callback : m_maximizedCallbacks)
		{
			callback(*this);
		}
	}

	auto Window::OnRestored(bool fromMaximized) noexcept -> void
	{
		for (Delegate<void(Window&, u32v2, i32v2, bool)>& callback : m_restoredCallbacks)
		{
			callback(*this, m_size, m_position, fromMaximized);
		}
	}

	auto Window::OnFocus(bool inFocus) noexcept -> void
	{
		m_inFocus = inFocus;
		for (Delegate<void(Window&, bool)>& callback : m_focusCallbacks)
		{
			callback(*this, inFocus);
		}
	}

	auto Window::OnEnterLeave(bool entered) noexcept -> void
	{
		m_inWindow = entered;
		for (Delegate<void(Window&, bool)>& callback : m_enterLeaveCallbacks)
		{
			callback(*this, entered);
		}
	}

	auto Window::OnShow(bool shown) noexcept -> void
	{
		m_visible = shown;
		for (Delegate<void(Window&, bool)>& callback : m_showCallbacks)
		{
			callback(*this, shown);
		}
	}

	auto Window::OnEndMoveSize() noexcept -> void
	{
		if (m_oldSize != m_size)
		{
			u32v2 tmp = m_size;
			m_size = m_oldSize;
			for (Delegate<void(Window&, u32v2, bool)>& callback : m_sizeCallbacks)
			{
				callback(*this, tmp, false);
			}
			m_size = tmp;
		}
		if (m_oldPosition != m_position)
		{
			i32v2 tmp = m_position;
			m_position = m_oldPosition;
			for (Delegate<void(Window&, i32v2, bool)>& callback : m_moveCallbacks)
			{
				callback(*this, tmp, false);
			}
			m_position = tmp;
		}
		m_oldPosition = m_position;
		m_oldSize = m_size;
	}
}
