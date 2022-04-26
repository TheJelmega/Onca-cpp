#include "../Window.h"

#include "core/input/InputManager.h"
#include "core/input/Keyboard.h"
#include "core/windowing/WindowManager.h"

#if PLATFORM_WINDOWS
#include "core/platform/Platform.h"

namespace Core
{
	void HandleVirtualKey(bool down, WPARAM wParam, LPARAM lParam, Input::InputManager* pInputManager) noexcept
	{
		if (down && (HIWORD(lParam) & KF_REPEAT) == KF_REPEAT)
			return;

		Input::KeyCode key = Input::KeyCode::Invalid;

		bool extended = lParam & 0x0100'0000;
		u32 scanCode = u32(lParam & 0x00FF'0000) >> 16;

		switch (wParam)
		{
		case VK_BACK      : key = Input::KeyCode::Backspace;    break;
		case VK_TAB       : key = Input::KeyCode::Tab;          break;
		case VK_CLEAR     : key = Input::KeyCode::Clear;        break;
		case VK_RETURN    : key = Input::KeyCode::Enter;        break;
		case VK_PAUSE     : key = Input::KeyCode::Pause;        break;
		case VK_CAPITAL   : key = Input::KeyCode::CapsLock;     break;
		case VK_ESCAPE    : key = Input::KeyCode::Escape;       break;
		case VK_SPACE     : key = Input::KeyCode::Space;        break;
		case VK_PRIOR     : key = Input::KeyCode::PageUp;       break;
		case VK_NEXT      : key = Input::KeyCode::PageDown;     break;
		case VK_END       : key = Input::KeyCode::End;          break;
		case VK_HOME      : key = Input::KeyCode::Home;         break;
		case VK_LEFT      : key = Input::KeyCode::Left;         break;
		case VK_UP        : key = Input::KeyCode::Up;           break;
		case VK_RIGHT     : key = Input::KeyCode::Right;        break;
		case VK_DOWN      : key = Input::KeyCode::Down;         break;
		case VK_SNAPSHOT  : key = Input::KeyCode::PrintScreen;  break;
		case VK_INSERT    : key = Input::KeyCode::Insert;       break;
		case VK_DELETE    : key = Input::KeyCode::Delete;       break;
		case VK_LWIN      : key = Input::KeyCode::LeftMeta;     break;
		case VK_RWIN      : key = Input::KeyCode::RightMeta;    break;
		case VK_NUMPAD0   : key = Input::KeyCode::Num0;         break;
		case VK_NUMPAD1   : key = Input::KeyCode::Num1;         break;
		case VK_NUMPAD2   : key = Input::KeyCode::Num2;         break;
		case VK_NUMPAD3   : key = Input::KeyCode::Num3;         break;
		case VK_NUMPAD4   : key = Input::KeyCode::Num4;         break;
		case VK_NUMPAD5   : key = Input::KeyCode::Num5;         break;
		case VK_NUMPAD6   : key = Input::KeyCode::Num6;         break;
		case VK_NUMPAD7   : key = Input::KeyCode::Num7;         break;
		case VK_NUMPAD8   : key = Input::KeyCode::Num8;         break;
		case VK_NUMPAD9   : key = Input::KeyCode::Num9;         break;
		case VK_MULTIPLY  : key = Input::KeyCode::NumMultiply;  break;
		case VK_ADD       : key = Input::KeyCode::NumAdd;       break;
		case VK_SEPARATOR : key = Input::KeyCode::NumSeparator; break;
		case VK_SUBTRACT  : key = Input::KeyCode::NumSub;       break;
		case VK_DECIMAL   : key = Input::KeyCode::NumDecimal;   break;
		case VK_DIVIDE    : key = Input::KeyCode::NumDivide;    break;
		case VK_F1        : key = Input::KeyCode::F1;           break;
		case VK_F2        : key = Input::KeyCode::F2;           break;
		case VK_F3        : key = Input::KeyCode::F3;           break;
		case VK_F4        : key = Input::KeyCode::F4;           break;
		case VK_F5        : key = Input::KeyCode::F5;           break;
		case VK_F6        : key = Input::KeyCode::F6;           break;
		case VK_F7        : key = Input::KeyCode::F7;           break;
		case VK_F8        : key = Input::KeyCode::F8;           break;
		case VK_F9        : key = Input::KeyCode::F9;           break;
		case VK_F10       : key = Input::KeyCode::F10;          break;
		case VK_F11       : key = Input::KeyCode::F11;          break;
		case VK_F12       : key = Input::KeyCode::F12;          break;
		case VK_NUMLOCK   : key = Input::KeyCode::NumLock;      break;
		case VK_SCROLL    : key = Input::KeyCode::ScrollLock;   break;
		case VK_LSHIFT    : key = Input::KeyCode::LeftShift;    break;
		case VK_RSHIFT    : key = Input::KeyCode::RightShift;   break;
		case VK_LCONTROL  : key = Input::KeyCode::LeftCtrl;     break;
		case VK_RCONTROL  : key = Input::KeyCode::RightCtrl;    break;
		case VK_LMENU     : key = Input::KeyCode::LeftAlt;      break;
		case VK_RMENU     : key = Input::KeyCode::RightAlt;     break;
		case VK_OEM_1     : key = Input::KeyCode::Semicolon;    break;
		case VK_OEM_PLUS  : key = Input::KeyCode::Equals;       break;
		case VK_OEM_COMMA : key = Input::KeyCode::Comma;        break;
		case VK_OEM_MINUS : key = Input::KeyCode::Minus;        break;
		case VK_OEM_PERIOD: key = Input::KeyCode::Period;       break;
		case VK_OEM_2     : key = Input::KeyCode::Slash;        break;
		case VK_OEM_3     : key = Input::KeyCode::BackQuote;    break;
		case VK_OEM_4     : key = Input::KeyCode::LeftBracket;  break;
		case VK_OEM_5     : key = Input::KeyCode::BackSlash;    break;
		case VK_OEM_6     : key = Input::KeyCode::RightBracket; break;
		case VK_OEM_7     : key = Input::KeyCode::SingleQuote;  break;

		case VK_SHIFT:
		{
			wParam = ::MapVirtualKeyW(scanCode, MAPVK_VSC_TO_VK_EX);
;			key = wParam == VK_LSHIFT ? Input::KeyCode::LeftShift : Input::KeyCode::RightShift;
			break;
		}
		case VK_CONTROL: key = extended ? Input::KeyCode::RightCtrl : Input::KeyCode::LeftCtrl; break;
		case VK_MENU: key = extended ? Input::KeyCode::RightAlt : Input::KeyCode::LeftAlt; break;

		default:
		{
			if (wParam >= '0' && wParam <= '9')
				key = Input::KeyCode(wParam);
			if (wParam >= 'A' && wParam <= 'Z')
				key = Input::KeyCode(wParam | 0x20);

			break;
		};
		}

		if (key != Input::KeyCode::Invalid)
		{
			if (down)
				pInputManager->GetKeyboard()->OnKeyPressed(key);
			else
				pInputManager->GetKeyboard()->OnKeyReleased(key);
		}
	}

	auto CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
	{
		Window* pWindow = nullptr;
		if (hwnd)
			pWindow = reinterpret_cast<Window*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA));
		if (!pWindow)
			return DefWindowProcW(hwnd, msg, wParam, lParam);

		Input::InputManager* pInputManager = pWindow->GetWindowManager()->GetInputManager();

		switch (msg)
		{
		case WM_ENABLE:
			pWindow->OnInputEnabled(wParam);
			break;
		case WM_EXITSIZEMOVE:
			pWindow->OnEndMoveSize();
			break;
		case WM_MOVE:
		{
			i32 x = GET_X_LPARAM(lParam);
			i32 y = GET_Y_LPARAM(lParam);
			pWindow->OnMoved({ x, y });
			break;
		}
		case WM_SIZE:
		{
			i32 x = LOWORD(lParam);
			i32 y = HIWORD(lParam);
			pWindow->OnResized({ x, y });
			break;
		}
		case WM_SHOWWINDOW:
			pWindow->OnShow(wParam);
			break;
		case WM_SETFOCUS:
			pWindow->OnFocus(true);
			break;
		case WM_KILLFOCUS:
			pWindow->OnFocus(false);
			break;
		case WM_MOUSEMOVE:
			pWindow->OnMouseMove();
			break;
		case WM_MOUSEHOVER:
			pWindow->OnEnterLeave(true);
			break;
		case WM_MOUSELEAVE:
			pWindow->OnEnterLeave(false);
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			HandleVirtualKey(true, wParam, lParam, pInputManager);
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			HandleVirtualKey(false, wParam, lParam, pInputManager);
			break;
		case WM_LBUTTONDOWN:
			pInputManager->GetMouse()->OnButtonPressed(Input::MouseCode::LeftButton);
			break;
		case WM_LBUTTONUP:
			pInputManager->GetMouse()->OnButtonReleased(Input::MouseCode::LeftButton);
			break;
		case WM_RBUTTONDOWN:
			pInputManager->GetMouse()->OnButtonPressed(Input::MouseCode::RightButton);
			break;
		case WM_RBUTTONUP:
			pInputManager->GetMouse()->OnButtonReleased(Input::MouseCode::RightButton);
			break;
		case WM_MBUTTONDOWN:
			pInputManager->GetMouse()->OnButtonPressed(Input::MouseCode::MiddleButton);
			break;
		case WM_MBUTTONUP:
			pInputManager->GetMouse()->OnButtonReleased(Input::MouseCode::MiddleButton);
			break;
		case WM_XBUTTONDOWN:
			pInputManager->GetMouse()->OnButtonPressed(GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? Input::MouseCode::ThumbButton0 : Input::MouseCode::ThumbButton1);
			break;
		case WM_XBUTTONUP:
			pInputManager->GetMouse()->OnButtonPressed(GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? Input::MouseCode::ThumbButton0 : Input::MouseCode::ThumbButton1);
			break;
		case WM_MOUSEWHEEL:
		{
			Input::MouseCode button = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? Input::MouseCode::ScrollUp : Input::MouseCode::ScrollDown;
			pInputManager->GetMouse()->OnButtonPressed(button);
		}
		case WM_CLOSE:
			// TODO: delay close (should be end of frame)
			pWindow->Close();
			return 0;
		case WM_DESTROY:
			return 0;
		
		default: break;
		}

		return DefWindowProcW(hwnd, msg, wParam, lParam);
	}

	auto WindowStyleToWin32Style(WindowStyles style) noexcept -> DWORD
	{
		DWORD wndStyle = 0;
		if (style & WindowStyle::Border)
			wndStyle |= WS_BORDER;
		if (style & WindowStyle::TitleBar)
			wndStyle |= WS_CAPTION;
		if (style & WindowStyle::Resizable)
			wndStyle |= WS_SIZEBOX;
		if (style & WindowStyle::Minimizable)
			wndStyle |= WS_MINIMIZEBOX;
		if (style & WindowStyle::Maximizable)
			wndStyle |= WS_MAXIMIZEBOX;
		if (style & WindowStyle::HScroll)
			wndStyle |= WS_HSCROLL;
		if (style & WindowStyle::VScroll)
			wndStyle |= WS_VSCROLL;
		if (style & (WindowStyle::TitleBar | WindowStyle::SysMenu))
			wndStyle |= WS_SYSMENU;
		if (style & WindowStyle::Minimized)
			wndStyle |= WS_MINIMIZE;
		if (style & WindowStyle::Maximized)
			wndStyle |= WS_MAXIMIZE;
		if (style & WindowStyle::Child)
			wndStyle |= WS_CHILD;
		if (style & WindowStyle::Popup)
			wndStyle |= WS_POPUP;
		return wndStyle;
	}

	auto WindowStyleToWin32StyleEx(WindowStyles style) noexcept -> DWORD
	{
		DWORD wndStyle = 0;

		if (style & WindowStyle::AcceptFiles)
			wndStyle |= WS_EX_ACCEPTFILES;
		if (style & WindowStyle::AppWindow)
			wndStyle |= WS_EX_APPWINDOW;
		if (style & WindowStyle::Border3D)
			wndStyle |= WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE;
		if (style & WindowStyle::ToolWindow)
			wndStyle |= WS_EX_TOOLWINDOW;
		if (style & WindowStyle::TopMost)
			wndStyle |= WS_EX_TOPMOST;

		// TODO: should be based on localized language, not shell language
		//wndStyle |= WS_EX_RTLREADING; // Support left-to-right languages
		//if (style & WindowStyle::HScroll)
		//	wndStyle |= WS_EX_LEFTSCROLLBAR; // Put the scrollbar on the left for left-to-right languages

		return wndStyle;
	}

	Window::Window()
		: m_handle(nullptr)
		, m_nativeData(0)
		, m_winId(u32(-1))
		, m_inputEnabled(true)
		, m_visible(false)
		, m_minimized(false)
		, m_maximized(false)
		, m_inFocus(false)
		, m_inWindow(false)
		, m_endedMoveSize(false)
		, m_pWindowManager(nullptr)
	{
	}

	Window::Window(Window&& other) noexcept
		: m_title(Move(other.m_title))
		, m_handle(other.m_handle)
		, m_nativeData(other.m_nativeData)
		, m_size(other.m_size)
		, m_position(other.m_position)
		, m_oldPosition(other.m_oldPosition)
		, m_oldSize(other.m_oldSize)
		, m_winId(other.m_winId)
		, m_style(other.m_style)
		, m_inputEnabled(other.m_inputEnabled)
		, m_visible(other.m_visible)
		, m_minimized(other.m_minimized)
		, m_maximized(other.m_maximized)
		, m_inFocus(other.m_inFocus)
		, m_inWindow(other.m_inWindow)
		, m_endedMoveSize(other.m_endedMoveSize)
		, m_pWindowManager(other.m_pWindowManager)

		, m_closeCallbacks(Move(other.m_closeCallbacks))
		, m_inputEnableCallbacks(Move(other.m_inputEnableCallbacks))
		, m_sizeCallbacks(Move(other.m_sizeCallbacks))
		, m_moveCallbacks(Move(other.m_moveCallbacks))
		, m_minimizedCallbacks(Move(other.m_minimizedCallbacks))
		, m_maximizedCallbacks(Move(other.m_maximizedCallbacks))
		, m_restoredCallbacks(Move(other.m_restoredCallbacks))
		, m_focusCallbacks(Move(other.m_focusCallbacks))
		, m_enterLeaveCallbacks(Move(other.m_enterLeaveCallbacks))
		, m_showCallbacks(Move(other.m_showCallbacks))
	{
		other.m_handle = nullptr;

		if (IsValid())
			::SetWindowLongPtrW(static_cast<HWND>(m_handle), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	}

	Window::~Window()
	{
		if (IsValid())
			Close();
	}

	auto Window::operator=(Window&& other) noexcept -> Window&
	{
		this->~Window();

		m_title = Move(other.m_title);
		m_handle = other.m_handle;
		m_nativeData = other.m_nativeData;
		m_position = other.m_position;
		m_size = other.m_size;
		m_oldPosition = other.m_oldPosition;
		m_oldSize = other.m_oldSize;
		m_winId = other.m_winId;
		m_style = other.m_style;
		m_inputEnabled = other.m_inputEnabled;
		m_visible = other.m_visible;
		m_minimized = other.m_minimized;
		m_maximized = other.m_maximized;
		m_inFocus = other.m_inFocus;
		m_inWindow = other.m_inWindow;
		m_endedMoveSize = other.m_endedMoveSize;
		m_pWindowManager = other.m_pWindowManager;

		m_closeCallbacks = Move(other.m_closeCallbacks);
		m_inputEnableCallbacks = Move(other.m_inputEnableCallbacks);
		m_sizeCallbacks = Move(other.m_sizeCallbacks);
		m_moveCallbacks = Move(other.m_moveCallbacks);
		m_minimizedCallbacks = Move(other.m_minimizedCallbacks);
		m_maximizedCallbacks = Move(other.m_maximizedCallbacks);
		m_restoredCallbacks = Move(other.m_restoredCallbacks);
		m_focusCallbacks = Move(other.m_focusCallbacks);
		m_enterLeaveCallbacks = Move(other.m_enterLeaveCallbacks);
		m_showCallbacks = Move(other.m_showCallbacks);

		other.m_handle = nullptr;

		if (IsValid())
			::SetWindowLongPtrW(static_cast<HWND>(m_handle), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		return *this;
	}

	auto Window::Close() noexcept -> SystemError
	{
		OnClose();

		if (m_handle)
		{
			bool res = DestroyWindow(static_cast<HWND>(m_handle));
			if (!res)
				return TranslateSystemError();
			m_handle = nullptr;
		}

		if (m_nativeData)
		{
			bool res = UnregisterClassW(reinterpret_cast<LPCWSTR>(m_nativeData), ::GetModuleHandleW(nullptr));
			if (!res)
				return TranslateSystemError();
			m_nativeData = 0;
		}
		return SystemErrorCode::Success;
	}

	auto Window::Focus() noexcept -> SystemError
	{
		if (!m_handle)
			return SystemErrorCode::InvalidHandle;

		bool res = ::BringWindowToTop(static_cast<HWND>(m_handle));
		if (!res)
			return TranslateSystemError();

		m_inFocus = true;
		return SystemErrorCode::Success;
	}

	auto Window::MinMaxRestore() noexcept -> SystemError
	{
		if (!m_handle)
			return SystemErrorCode::InvalidHandle;

		if (m_minimized)
		{
			bool res = ::OpenIcon(static_cast<HWND>(m_handle));
			if (!res)
				return TranslateSystemError();
			m_minimized = false;
		}
		else
		{

			m_maximized = false;
		}
		return SystemErrorCode::Success;
	}

	auto Window::Hide() noexcept -> SystemError
	{
		if (!m_handle)
			return SystemErrorCode::InvalidHandle;
		if (!m_visible)
			return SystemErrorCode::Success;

		bool res = ::ShowWindow(static_cast<HWND>(m_handle), SW_HIDE);
		if (!res)
			return TranslateSystemError();

		m_visible = false;
		return SystemErrorCode::Success;
	}

	auto Window::Show() noexcept -> SystemError
	{
		if (!m_handle)
			return SystemErrorCode::InvalidHandle;
		if (m_visible)
			return SystemErrorCode::Success;

		bool res = ::ShowWindow(static_cast<HWND>(m_handle), SW_SHOW);
		if (!res)
			return TranslateSystemError();

		m_visible = true;
		return SystemErrorCode::Success;
	}

	auto Window::Minimize() noexcept -> SystemError
	{
		if (!m_handle)
			return SystemErrorCode::InvalidHandle;
		if (m_minimized)
			return SystemErrorCode::Success;

		bool res = ::ShowWindow(static_cast<HWND>(m_handle), SW_MINIMIZE);
		if (!res)
			return TranslateSystemError();

		m_maximized = false;
		m_minimized = true;
		return SystemErrorCode::Success;
	}

	auto Window::Maximize() noexcept -> SystemError
	{
		if (!m_handle)
			return SystemErrorCode::InvalidHandle;
		if (m_maximized)
			return SystemErrorCode::Success;

		bool res = ::ShowWindow(static_cast<HWND>(m_handle), SW_MAXIMIZE);
		if (!res)
			return TranslateSystemError();

		m_minimized = false;
		m_maximized = true;
		return SystemErrorCode::Success;
	}

	auto Window::Restore() noexcept -> SystemError
	{
		if (!m_handle)
			return SystemErrorCode::InvalidHandle;
		if (!m_minimized && !m_maximized)
			return SystemErrorCode::Success;

		bool res = ::ShowWindow(static_cast<HWND>(m_handle), SW_RESTORE);
		if (!res)
			return TranslateSystemError();

		m_minimized = false;
		m_maximized = false;
		return SystemErrorCode::Success;
	}

	auto Window::SetTitle(const String& title) noexcept -> SystemError
	{
		if (!m_handle)
			return SystemErrorCode::InvalidHandle;

		DynArray<char16_t> utf16 = title.ToUtf16();
		bool res = ::SetWindowTextW(static_cast<HWND>(m_handle), reinterpret_cast<LPCWSTR>(utf16.Data()));
		if (!res)
			return TranslateSystemError();
		return SystemErrorCode::Success;
	}

	auto Window::SetPosition(const i32v2& pos) noexcept -> SystemError
	{
		if (!m_handle)
			return SystemErrorCode::InvalidHandle;

		bool res = ::SetWindowPos(static_cast<HWND>(m_handle), nullptr,
								  pos.x, pos.y,
								  0, 0,
								  SWP_NOSIZE | SWP_NOZORDER);
		if (!res)
			return TranslateSystemError();

		m_position = pos;
		return SystemErrorCode::Success;
	}

	auto Window::SetSize(const u32v2& size) noexcept -> SystemError
	{
		if (!m_handle)
			return SystemErrorCode::InvalidHandle;

		bool res = ::SetWindowPos(static_cast<HWND>(m_handle), nullptr,
								  0, 0,
								  size.x, size.y,
								  SWP_NOMOVE | SWP_NOZORDER);
		if (!res)
			return TranslateSystemError();

		return SystemErrorCode::Success;
	}

	auto Window::SetStyle(WindowStyles style) noexcept -> SystemError
	{
		if (!m_handle)
			return SystemErrorCode::InvalidHandle;

		DWORD winStyle = WindowStyleToWin32Style(style);
		bool res = ::SetWindowLongPtrW(static_cast<HWND>(m_handle), GWL_STYLE, winStyle);
		if (!res)
			return TranslateSystemError();

		DWORD winStyleEx = WindowStyleToWin32StyleEx(style);
		res = ::SetWindowLongPtrW(static_cast<HWND>(m_handle), GWL_EXSTYLE, winStyleEx);
		if (!res)
			return TranslateSystemError();

		return SystemErrorCode::Success;
	}

	auto Window::SetInputEnabled(bool enable) noexcept -> SystemError
	{
		if (!m_handle)
			return SystemErrorCode::InvalidHandle;

		bool res = ::EnableWindow(static_cast<HWND>(m_handle), enable);
		if (!res)
			return TranslateSystemError();

		m_inputEnabled = enable;
		return SystemErrorCode::Success;
	}

	auto Window::GetWindowRect() const noexcept -> Math::Rect<i32>
	{
		if (!m_handle)
			return {};

		RECT rect;
		bool res = ::GetWindowRect(static_cast<HWND>(m_handle), &rect);
		if (!res)
			return {};

		return { rect.top, rect.right, rect.bottom - 1, rect.left - 1 };
	}

	auto Window::GetClientRect() const noexcept -> Math::Rect<i32>
	{
		return { 0, 0, i32(m_size.x), i32(m_size.y) };
	}

	auto Window::IsValid() const noexcept -> bool
	{
		return m_handle;
	}

	void Window::OnMouseMove() noexcept
	{
		if (m_inWindow)
			return;

		// Tell windows to generate MOUSE_LEAVE event when mouse leaves window
		TRACKMOUSEEVENT trackEvnt;
		trackEvnt.cbSize = sizeof(TRACKMOUSEEVENT);
		trackEvnt.hwndTrack = static_cast<HWND>(m_handle);
		trackEvnt.dwHoverTime = 0; // value doesn't matter
		trackEvnt.dwFlags = TME_LEAVE;
		::TrackMouseEvent(&trackEvnt);

		OnEnterLeave(true);
	}

	auto Window::Create(WindowManager* pManager, const WindowCreationInfo& createInfo, u32 id, Window* pParent) noexcept -> Result<Window, SystemError>
	{
		String wndClassName = "Window"_s + ToString(id);
		DynArray<char16_t> utf16WndClassName = wndClassName.ToUtf16();

		HINSTANCE hIst = ::GetModuleHandleW(nullptr);
		WNDCLASSEXW wndClass = {};
		wndClass.cbSize = sizeof(WNDCLASSEXW);
		wndClass.lpfnWndProc = &WndProc;
		wndClass.hInstance = hIst;
		wndClass.hIcon = nullptr; // TODO
		wndClass.hIconSm = nullptr; // TODO
		wndClass.hCursor = nullptr; // TODO
		wndClass.hbrBackground = reinterpret_cast<HBRUSH>(::CreateSolidBrush(BLACK_BRUSH));
		wndClass.lpszClassName = reinterpret_cast<LPCWSTR>(utf16WndClassName.Data());

		u32 classAtom = ::RegisterClassExW(&wndClass);
		if (classAtom == 0)
			return TranslateSystemError();

		DynArray<char16_t> utf16Title = createInfo.title.ToUtf16();
		
		DWORD wndStyle = WindowStyleToWin32Style(createInfo.style);
		if (createInfo.disableInput)
			wndStyle |= WS_DISABLED;

		DWORD wndStyleEx = WindowStyleToWin32StyleEx(createInfo.style);

		RECT sizeRect = { 0, 0, LONG(createInfo.size.x), LONG(createInfo.size.y) };
		::AdjustWindowRectEx(&sizeRect, wndStyle, false, wndStyleEx);

		i32v2 pos =
		{
			createInfo.position.x == DefaultPosition.x ? CW_USEDEFAULT : createInfo.position.x + sizeRect.left,
			createInfo.position.y == DefaultPosition.y ? CW_USEDEFAULT : createInfo.position.y + sizeRect.top
		};

		HWND parentHandle = nullptr; (pParent && pParent->IsValid()) ? static_cast<HWND>(pParent->GetNativeHandle()) : nullptr;
		HWND hwnd = ::CreateWindowExW(wndStyleEx,
									reinterpret_cast<LPCWSTR>(utf16WndClassName.Data()),
									reinterpret_cast<LPCWSTR>(utf16Title.Data()),
									wndStyle,
									  pos.x,
									  pos.y,
									sizeRect.right - sizeRect.left,
									sizeRect.bottom - sizeRect.top,
									parentHandle,
									nullptr, // TODO: Menu
									hIst,
									nullptr);


		Window window;
		window.m_handle = hwnd;
		window.m_nativeData = classAtom;
		window.m_winId = id;
		window.m_pWindowManager = pManager;

		window.m_title = createInfo.title;

		if (createInfo.position.x == DefaultPosition.x || createInfo.position.y == DefaultPosition.y)
		{
			RECT rect;
			::GetWindowRect(hwnd, &rect);
			window.m_oldPosition.x = window.m_position.x = rect.left - sizeRect.left;
			window.m_oldPosition.y = window.m_position.y = rect.top - sizeRect.top;
		}
		else
		{
			window.m_oldPosition = window.m_position = createInfo.position;
		}
		
		window.m_oldSize = window.m_size = createInfo.size;
		window.m_style = createInfo.style;

		window.m_inputEnabled = !createInfo.disableInput;
		window.m_visible = true;
		window.m_minimized = createInfo.style & WindowStyle::Minimized;
		window.m_maximized = createInfo.style & WindowStyle::Maximized;

		::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&window));
		::ShowWindow(hwnd, SW_SHOW);

		return window;
	}
}

#endif