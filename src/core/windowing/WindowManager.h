#pragma once
#include "Monitor.h"
#include "core/MinInclude.h"
#include "Window.h"
#include "core/threading/Threading.h"

namespace LogCategories
{
	DEFINE_LOG_CATEGORY(WINDOWING);
}

namespace Core
{
	namespace Input
	{
		class InputManager;
	}

	class CORE_API WindowManager
	{
	public:
		DISABLE_COPY(WindowManager);

		WindowManager() noexcept;
		
		WindowManager(WindowManager&& other) noexcept;
		
		auto operator=(WindowManager&& other) noexcept -> WindowManager&;

		/**
		 * Set the input manager
		 * \param[in] pManager Input manager
		 */
		auto SetInputManager(Input::InputManager* pManager) noexcept -> void { m_pInputManager = pManager; };
		/**
		 * Get the input manager
		 * \return Input manager
		 */
		auto GetInputManager() noexcept -> Input::InputManager* { return m_pInputManager; };

		/**
		 * Collect all available monitors
		 */
		auto CollectMonitors() noexcept -> void;
		/**
		 * Add a monitor to the window manager from a native handle
		 * \param[in] pHandle Native monitor handle
		 */
		auto AddMonitorFromNativeHandle(void* pHandle) noexcept -> void;
		/**
		 * Get all available monitors
		 * \return Available monitors
		 */
		auto GetMonitors() noexcept -> const DynArray<Monitor>& { return m_monitors; }
		/**
		 * Get the monitor at a certain index
		 * \param[in] idx Index
		 * \return Monitor
		 */
		auto GetMonitor(usize idx) noexcept -> const Monitor&;
		/**
		 * Get the primary monitor
		 * \return Primary monitor
		 */
		auto GetPrimaryMonitor() noexcept -> const Monitor&;

		/**
		 * Create a new window
		 * \param[in] info Creation info
		 * \return Reference to the created window
		 */
		auto CreateWindow(WindowCreationInfo& info) noexcept -> Window&;
		/**
		 * Get a window from an id, if the id is invalid, return a ref to an invalid window
		 * \param[in] id Window id
		 * \return Window with the given ID
		 */
		auto GetWindow(u32 id) noexcept -> Window&;

		/**
		 * Tick/update the window manager
		 * \note Should only be called from the thread that created the window manager
		 */
		auto Tick() noexcept -> void;

		/**
		 * Get the id of the thread that handles window management
		 * \return Id of the thread that handles window management
		 */
		auto GetThreadId() const noexcept -> Threading::ThreadID { return m_threadId; }

	private:

		/**
		 * Tick/update platform specific code
		 */
		auto PlatformTick() noexcept -> void;

		DynArray<Monitor>    m_monitors;      ///< Available monitors

		Window               m_invalidWindow; ///< Invalid dummy window
		DynArray<Window>     m_windows;       ///< Windows

		Threading::ThreadID  m_threadId;      ///< Thread ID of thread that own the manager

		Input::InputManager* m_pInputManager; ///< Input manager
	};
}
