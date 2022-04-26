#include "Window.h"

namespace Onca
{
	template<typename T>
	void RemoveForObject(DynArray<T>& callbacks, void* ptr) noexcept
	{
		for (usize i = 0; i < callbacks.Size();)
		{
			if (callbacks[i].IsCalledOn(ptr))
				callbacks.Erase(callbacks.IteratorAt(i));
			else
				++i;
		}
	}

	void Window::RemoveCallbacksForObject(void* ptr) noexcept
	{
		RemoveForObject(m_closeCallbacks, ptr);
		RemoveForObject(m_sizeCallbacks, ptr);
		RemoveForObject(m_moveCallbacks, ptr);
		RemoveForObject(m_minimizedCallbacks, ptr);
		RemoveForObject(m_maximizedCallbacks, ptr);
		RemoveForObject(m_restoredCallbacks, ptr);
	}

	void Window::RegisterCloseCallback(const Delegate<void(Window&)>& callback) noexcept
	{
		m_closeCallbacks.Add(callback);
	}

	void Window::RegisterInputEnableCallback(const Delegate<void(Window&, bool)>& callback) noexcept
	{
		m_inputEnableCallbacks.Add(callback);
	}

	void Window::RegisterSizeCallback(const Delegate<void(Window&, u32v2, bool)>& callback) noexcept
	{
		m_sizeCallbacks.Add(callback);
	}

	void Window::RegisterMoveCallback(const Delegate<void(Window&, i32v2, bool)>& callback) noexcept
	{
		m_moveCallbacks.Add(callback);
	}

	void Window::RegisterMinimizedCallback(const Delegate<void(Window&)>& callback) noexcept
	{
		m_minimizedCallbacks.Add(callback);
	}

	void Window::RegisterMaximizedCallback(const Delegate<void(Window&)>& callback) noexcept
	{
		m_maximizedCallbacks.Add(callback);
	}

	void Window::RegisterRestoreCallback(const Delegate<void(Window&, u32v2, i32v2, bool)>& callback) noexcept
	{
		m_restoredCallbacks.Add(callback);
	}

	void Window::RegisterFocusCallback(const Delegate<void(Window&, bool)>& callback) noexcept
	{
		m_focusCallbacks.Add(callback);
	}

	void Window::RegisterEnterLeaveCallback(const Delegate<void(Window&, bool)>& callback) noexcept
	{
		m_enterLeaveCallbacks.Add(callback);
	}

	void Window::RegisterShowCallback(const Delegate<void(Window&, bool)>& callback) noexcept
	{
		m_showCallbacks.Add(callback);
	}

	void Window::OnClose() noexcept
	{
		for (Delegate<void(Window&)>& callback : m_closeCallbacks)
		{
			callback(*this);
		}
	}

	void Window::OnInputEnabled(bool enabled) noexcept
	{
		m_inputEnabled = enabled;
		for (Delegate<void(Window& , bool)>& callback : m_inputEnableCallbacks)
		{
			callback(*this, enabled);
		}
	}

	void Window::OnResized(u32v2 size) noexcept
	{
		for (Delegate<void(Window&, u32v2, bool)>& callback : m_sizeCallbacks)
		{
			callback(*this, size, true);
		}
		m_size = size;
		m_endedMoveSize = false;
	}

	void Window::OnMoved(i32v2 pos) noexcept
	{
		for (Delegate<void(Window&, i32v2, bool)>& callback : m_moveCallbacks)
		{
			callback(*this, pos, true);
		}
		m_position = pos;
		m_endedMoveSize = false;
	}

	void Window::OnMinimized() noexcept
	{
		for (Delegate<void(Window&)>& callback : m_minimizedCallbacks)
		{
			callback(*this);
		}
	}

	void Window::OnMaximized() noexcept
	{
		for (Delegate<void(Window&)>& callback : m_maximizedCallbacks)
		{
			callback(*this);
		}
	}

	void Window::OnRestored(bool fromMaximized) noexcept
	{
		for (Delegate<void(Window&, u32v2, i32v2, bool)>& callback : m_restoredCallbacks)
		{
			callback(*this, m_size, m_position, fromMaximized);
		}
	}

	void Window::OnFocus(bool inFocus) noexcept
	{
		m_inFocus = inFocus;
		for (Delegate<void(Window&, bool)>& callback : m_focusCallbacks)
		{
			callback(*this, inFocus);
		}
	}

	void Window::OnEnterLeave(bool entered) noexcept
	{
		m_inWindow = entered;
		for (Delegate<void(Window&, bool)>& callback : m_enterLeaveCallbacks)
		{
			callback(*this, entered);
		}
	}

	void Window::OnShow(bool shown) noexcept
	{
		m_visible = shown;
		for (Delegate<void(Window&, bool)>& callback : m_showCallbacks)
		{
			callback(*this, shown);
		}
	}

	void Window::OnEndMoveSize() noexcept
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
