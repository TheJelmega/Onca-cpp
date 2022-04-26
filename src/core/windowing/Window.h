#pragma once
#include "Window.h"
#include "core/MinInclude.h"
#include "core/string/Include.h"
#include "core/math/Math.h"
#include "core/platform/SystemError.h"
#include "core/utils/Delegate.h"
#include "core/utils/Result.h"

namespace Onca
{
	class Window;
	class WindowManager;

	enum class WindowStyle
	{
		None        = 0              , ///< None (OS default)
		Border      = BIT(0)         , ///< Window has a border
		TitleBar    = BIT(1) | Border, ///< Window has a title bar (implicitly add a border)
		Resizable   = BIT(2)         , ///< Window can be resized using its border
		Minimizable = BIT(3)         , ///< Window has a button to be maximized
		Maximizable = BIT(4)         , ///< Window has a button to be minimized
		HScroll     = BIT(5)         , ///< Window has a horizontal scrollbar
		VScroll     = BIT(6)         , ///< Window has a vertical scrollbar
		SysMenu     = BIT(7)         , ///< Window has a system menu (TitleBar needs to be set)
		Maximized   = BIT(8)         , ///< Create the window in a maximized state
		Minimized   = BIT(9)         , ///< Create the window in a minimized state
		Child       = BIT(10)        , ///< Window is a child window of another window
		Popup       = BIT(11)        , ///< Window is a popup

		// These might be windows specific, we need to check linux APIs
		AcceptFiles = BIT(20)        , ///< Window accepts drag-drop files
		AppWindow   = BIT(21)        , ///< Forces the top-level window onto the taskbar when the window is visible
		Border3D    = BIT(22)        , ///< Give the window a 3D border effect
		ToolWindow  = BIT(23)        , ///< Window is a tool window, meaning it will have a smaller title bar with a smaller font, the window is also not shown in the taskbar
		TopMost     = BIT(24)        , ///< Window should be the top-most window

		Default     = TitleBar | Resizable | Minimizable | Maximizable | SysMenu | AppWindow, ///< Default window style
	};
	DEFINE_FLAGS(WindowStyle);

	struct WindowCreationInfo
	{
		u32          parentId     = u32(-1);                             ///< Parent window
		String       title;                                              ///< Window title
		u32v2        size;                                               ///< Size of render area
		i32v2        position     = i32v2{ Math::Consts::MinVal<i32> }; ///< Position of the render area (either member can be the corresponding value of Window::DefaultValue)
		WindowStyles style        = WindowStyle::Default;                ///< Window style
		bool         disableInput = false;                               ///< Create the window with input disabled
		bool         visible      = true;                                ///< Whether the created window should be visible
	};

	/**
	 * Window
	 * \note All windows should be created on just 1 thread
	 */
	class CORE_API Window
	{
	public:
		constexpr static i32v2 DefaultPosition{ Math::Consts::MinVal<i32> };

		DEFINE_OPAQUE_HANDLE(NativeHandle);

		DISABLE_COPY(Window);

		Window();
		Window(Window&& other) noexcept;
		~Window();
		
		auto operator=(Window&& other) noexcept -> Window&;

		/**
		 * Close the window
		 * \return Error
		 */
		auto Close() noexcept -> SystemError;
		/**
		 * Bring the window on top of all other windows
		 * \return Error
		 */
		auto Focus() noexcept -> SystemError;
		/**
		 * Restore the window back to its size and position from a minimized or maximized state
		 * \return Error
		 */
		auto MinMaxRestore() noexcept -> SystemError;

		/**
		 * Hide the window
		 * \return Error
		 */
		auto Hide() noexcept -> SystemError;
		/**
		 * Show the window
		 * \return Error
		 */
		auto Show() noexcept -> SystemError;
		/**
		 * Minimize the window
		 * \return Error
		 */
		auto Minimize() noexcept -> SystemError;
		/**
		 * Maximize the window
		 * \return Error
		 */
		auto Maximize() noexcept -> SystemError;
		/**
		 * Restore the window from a minimized/maximized state
		 * \return Error
		 */
		auto Restore() noexcept -> SystemError;

		/**
		 * Set the window title
		 * \param[in] title Title
		 * \return Error
		 */
		auto SetTitle(const String& title) noexcept -> SystemError;
		/**
		 * Set the window position
		 * \param[in] pos Position
		 * \return Error
		 */
		auto SetPosition(const i32v2& pos) noexcept -> SystemError;
		/**
		 * Set the window size
		 * \param[in] size Size
		 * \return Error
		 */
		auto SetSize(const u32v2& size) noexcept -> SystemError;
		/**
		 * Set the window style
		 * \param[in] style Style
		 * \return Error
		 */
		auto SetStyle(WindowStyles style) noexcept -> SystemError;
		/**
		 * Set if input should be enabled on the window
		 * \param[in] enable Whether input should be enabled on the window
		 * \return Error
		 */
		auto SetInputEnabled(bool enable) noexcept -> SystemError;


		/**
		 * Get the window title
		 * \return Window title
		 */
		auto GetTitle() const noexcept -> const String& { return m_title; }
		/**
		 * Get the window position
		 * \return Window position
		 */
		auto GetPosition() const noexcept -> i32v2 { return m_position; }
		/**
		 * Get the window size
		 * \return Window size
		 */
		auto GetSize() const noexcept -> u32v2 { return m_size; }
		/**
		 * Get the window style
		 * \return Window style
		 */
		auto GetStyle() const noexcept -> WindowStyles { return m_style; }
		/**
		 * Get the rectangle representing the window on screen
		 * \return Rectangle representing the window on screen
		 */
		auto GetWindowRect() const noexcept -> Math::Rect<i32>;
		/**
		 * Get the rectangle representing the client area of the window on screen
		 * \return Rectangle representing the client area of the window on screen
		 */
		auto GetClientRect() const noexcept -> Math::Rect<i32>;
		/**
		 * Get the native handle
		 * \return Native handle
		 */
		auto GetNativeHandle() const noexcept -> NativeHandle { return m_handle; }
		/**
		 * Get the window id
		 * \return Window id
		 */
		auto GetWindowId() const noexcept -> u32 { return m_winId; }
		/**
		 * Get the window manager
		 * \return Window manager
		 */
		auto GetWindowManager() noexcept -> WindowManager* { return m_pWindowManager; }
		/**
		 * Check if the window is valid
		 * \return Whether the window is valid
		 */
		auto IsValid() const noexcept -> bool;
		/**
		 * Check if the window receives input
		 * \return Whether the window receives input
		 */
		auto IsInputEnabled() const noexcept -> bool { return m_inputEnabled; }
		/**
		 * Check if the window is visible
		 * \return Whether the window is visible
		 */
		auto IsVisible() const noexcept -> bool { return m_visible; }
		/**
		 * Check if the window is minimized
		 * \return Whether the window is minimized
		 */
		auto IsMinimized() const noexcept -> bool { return m_minimized; }
		/**
		 * Check if the window is maximized
		 * \return Whether the window is maximized
		 */
		auto IsMaximized() const noexcept -> bool { return m_maximized; }


		/**
		 * Remove all callbacks that are called on a certain object
		 * \param[in] ptr Pointer to object
		 */
		void RemoveCallbacksForObject(void* ptr) noexcept;

		/**
		 * Register a close callback
		 * \param[in] callback Callback(Window& window)
		 */
		void RegisterCloseCallback(const Delegate<void(Window&)>& callback) noexcept;
		/**
		 * Register an input enable callback
		 * \param[in] callback Callback(Window& window, bool enabled)
		 */
		void RegisterInputEnableCallback(const Delegate<void(Window&, bool)>& callback) noexcept;
		/**
		 * Register an size callback
		 * \param[in] callback Callback(Window& window, u32v2 size, bool sizing)
		 * \note The new size of the window is only set after the callbacks are processed, allowing to get the old size (from the window) and the new size (passed to the callback)
		 * \note The 'sizing' value in the callback specifies if the window is still being sized or not
		 * \note If 'sizing' is false, the old value of the window will temporarily be set to the size it was before the current resizing started
		 */
		void RegisterSizeCallback(const Delegate<void(Window&, u32v2, bool)>& callback) noexcept;
		/**
		 * Register an move callback
		 * \param[in] callback Callback(Window& window, i32v2 pos, bool moving)
		 * \note The new position of the window is only set after the callbacks are processed, allowing to get the old position (from the window) and the new position (passed to the callback)
		 * \note The 'moving' value in the callback specifies if the window is still being sized or not
		 * \note If 'moving' is false, the old value of the window will temporarily be set to the position it was before the current moving started
		 */
		void RegisterMoveCallback(const Delegate<void(Window&, i32v2, bool)>& callback) noexcept;
		/**
		 * Register a minimized callback
		 * \param[in] callback Callback(Window& window)
		 */
		void RegisterMinimizedCallback(const Delegate<void(Window&)>& callback) noexcept;
		/**
		 * Register a maximized callback
		 * \param[in] callback Callback(Window& window)
		 */
		void RegisterMaximizedCallback(const Delegate<void(Window&)>& callback) noexcept;
		/**
		 * Register a close callback
		 * \param[in] callback Callback(Window& window, u32v2 size, i32v2 position, bool fromMaximized)
		 */
		void RegisterRestoreCallback(const Delegate<void(Window&, u32v2, i32v2, bool)>& callback) noexcept;
		/**
		 * Register a maximized callback
		 * \param[in] callback Callback(Window& window, bool inFocus)
		 */
		void RegisterFocusCallback(const Delegate<void(Window&, bool)>& callback) noexcept;
		/**
		 * Register a maximized callback
		 * \param[in] callback Callback(Window& window, bool entered)
		 */
		void RegisterEnterLeaveCallback(const Delegate<void(Window&, bool)>& callback) noexcept;
		/**
		 * Register a maximized callback
		 * \param[in] callback Callback(Window& window, bool shown)
		 */
		void RegisterShowCallback(const Delegate<void(Window&, bool)>& callback) noexcept;

		// TODO: unregister callbacks

		/**
		 * Calls all close callbacks
		 * \note Should not be called manually
		 */
		void OnClose() noexcept;
		/**
		 * Calls all input enable callbacks
		 * \param[in] enabled Whether the window is enabled or not
		 * \note Should not be called manually
		 */
		void OnInputEnabled(bool enabled) noexcept;

		/**
		 * Calls all resizing callbacks
		 * \param[in] size Size of the window after resizing
		 * \note Should not be called manually
		 */
		void OnResized(u32v2 size) noexcept;
		/**
		 * Calls all resizing callbacks
		 * \param[in] pos Position of the window after moving
		 * \note Should not be called manually
		 */
		void OnMoved(i32v2 pos) noexcept;
		/**
		 * Calls all minimized callbacks
		 * \note Should not be called manually
		 */
		void OnMinimized() noexcept;
		/**
		 * Calls all maximized callbacks
		 * \note Should not be called manually
		 */
		void OnMaximized() noexcept;
		/**
		 * Calls all restored callbacks
		 * \param[in] fromMaximized Whether the window has been restored from a maximized window
		 * \note Should not be called manually
		 */
		void OnRestored(bool fromMaximized) noexcept;
		/**
		 * Calls all focus callbacks
		 * \param[in] inFocus Whether the window is in focus
		 * \note Should not be called manually
		 */
		void OnFocus(bool inFocus) noexcept;
		/**
		 * Calls all focus callbacks
		 * \param[in] entered Whether the window is entered
		 * \note Should not be called manually
		 */
		void OnEnterLeave(bool entered) noexcept;
		/**
		 * Calls all focus callbacks
		 * \param[in] shown Whether the window is shown
		 * \note Should not be called manually
		 */
		void OnShow(bool shown) noexcept;


		/**
		 * Calls all resizing and moving callbacks with its resizing/moving bool to false
		 * \note Should not be called manually
		 */
		void OnEndMoveSize() noexcept;
		/**
		 * Handles mouse move
		 * \note Should not be called manually
		 */
		void OnMouseMove() noexcept;

	private:
		friend class WindowManager;

		/**
		 * Create a new window
		 * \param[in] pManager Window manager
		 * \param[in] createInfo Creation info
		 * \param[in] id Window id
		 * \param[in] pParent Pointer to parent window
		 * \return Result with window or error
		 */
		static auto Create(WindowManager* pManager, const WindowCreationInfo& createInfo, u32 id, Window* pParent) noexcept -> Result<Window, SystemError>;

		String                                                m_title;                ///< Window title
		NativeHandle                                          m_handle;               ///< Window handle
		usize                                                 m_nativeData;           ///< Additional native data
		u32v2                                                 m_size;                 ///< Window size
		i32v2                                                 m_position;             ///< Window position
		i32v2                                                 m_oldPosition;          ///< Old window position
		u32v2                                                 m_oldSize;              ///< Old window size
		u32                                                   m_winId;                ///< Window id
		WindowStyles                                          m_style;                ///< Window style
		bool                                                  m_inputEnabled  : 1;    ///< Whether the window handles input
		bool                                                  m_visible       : 1;    ///< whether the window is visible
		bool                                                  m_minimized     : 1;    ///< Whether the window is minimized
		bool                                                  m_maximized     : 1;    ///< Whether the window is maximized
		bool                                                  m_inFocus       : 1;    ///< Whether the window is in focus
		bool                                                  m_inWindow      : 1;    ///< Whether the mouse is in the window
		bool                                                  m_endedMoveSize : 1;    ///< Whether the window stopped moving

		WindowManager*                                        m_pWindowManager;       ///< Window manager

		DynArray<Delegate<void(Window&)>>                     m_closeCallbacks;       ///< Callbacks for when the window closes
		DynArray<Delegate<void(Window&, bool)>>               m_inputEnableCallbacks; ///< Callbacks for when input is enabled on a window
		DynArray<Delegate<void(Window&, u32v2, bool)>>        m_sizeCallbacks;        ///< Callbacks for when the window is/has finished resizing
		DynArray<Delegate<void(Window&, i32v2, bool)>>        m_moveCallbacks;        ///< Callbacks for when the window is/has finished moving
		DynArray<Delegate<void(Window&)>>                     m_minimizedCallbacks;   ///< Callbacks for when the window has been minimized
		DynArray<Delegate<void(Window&)>>                     m_maximizedCallbacks;   ///< Callbacks for when the window has been maximized
		DynArray<Delegate<void(Window&, u32v2, i32v2, bool)>> m_restoredCallbacks;    ///< Callbacks for when the window has been been restored to the size is was before being minimized or maximized
		DynArray<Delegate<void(Window&, bool)>>               m_focusCallbacks;       ///< Callbacks for when the window goes in to/out off focus
		DynArray<Delegate<void(Window&, bool)>>               m_enterLeaveCallbacks;  ///< Callbacks for when the window is entered/left
		DynArray<Delegate<void(Window&, bool)>>               m_showCallbacks;        ///< Callbacks for when the window is being shown/hidden
	};
}

DEFINE_ENUM_FLAG_OPS(Onca::WindowStyle);
