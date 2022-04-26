#include "Keyboard.h"

#include "InputManager.h"
#include "core/logging/Logger.h"


namespace Core::Input
{
	auto ToString(KeyCode key) noexcept -> String
	{
		switch (key)
		{
		case KeyCode::Invalid     : return "Invalid"_s;
		case KeyCode::Backspace   : return "Backspace"_s;
		case KeyCode::Tab         : return "Tab"_s;
		case KeyCode::Enter       : return "Enter"_s;
		case KeyCode::Pause       : return "Pause"_s;
		case KeyCode::Escape      : return "Escape"_s;
		case KeyCode::PageUp      : return "Page Up"_s;
		case KeyCode::PageDown    : return "Page Down"_s;
		case KeyCode::Home        : return "Home"_s;
		case KeyCode::End         : return "End"_s;
		case KeyCode::Left        : return "Left Arrow"_s;
		case KeyCode::Right       : return "Right Arrow"_s;
		case KeyCode::Up          : return "Up Arrow"_s;
		case KeyCode::Down        : return "Down Arrow"_s;
		case KeyCode::PrintScreen : return "Printscreen"_s;
		case KeyCode::Insert      : return "Insert"_s;
		case KeyCode::Delete      : return "Delete"_s;
		case KeyCode::Space       : return "Space"_s;
		case KeyCode::Num0        : return "Numpad 0"_s;
		case KeyCode::Num1        : return "Numpad 1"_s;
		case KeyCode::Num2        : return "Numpad 2"_s;
		case KeyCode::Num3        : return "Numpad 3"_s;
		case KeyCode::Num4        : return "Numpad 4"_s;
		case KeyCode::Num5        : return "Numpad 5"_s;
		case KeyCode::Num6        : return "Numpad 6"_s;
		case KeyCode::Num7        : return "Numpad 7"_s;
		case KeyCode::Num8        : return "Numpad 8"_s;
		case KeyCode::Num9        : return "Numpad 9"_s;
		case KeyCode::NumMultiply : return "Numpad *"_s;
		case KeyCode::NumAdd      : return "Numpad +"_s;
		case KeyCode::NumSub      : return "Numpad -"_s;
		case KeyCode::NumDivide   : return "Numpad /"_s;
		case KeyCode::NumSeparator: return "Numpad ,"_s;
		case KeyCode::NumDecimal  : return "Numpad ."_s;
		case KeyCode::F1          : return "F1"_s;
		case KeyCode::F2          : return "F2"_s;
		case KeyCode::F3          : return "F3"_s;
		case KeyCode::F4          : return "F4"_s;
		case KeyCode::F5          : return "F5"_s;
		case KeyCode::F6          : return "F6"_s;
		case KeyCode::F7          : return "F7"_s;
		case KeyCode::F8          : return "F8"_s;
		case KeyCode::F9          : return "F9"_s;
		case KeyCode::F10         : return "F10"_s;
		case KeyCode::F11         : return "F11"_s;
		case KeyCode::F12         : return "F12"_s;
		case KeyCode::Clear       : return "Clear"_s;
		case KeyCode::CapsLock    : return "Caps-lock"_s;
		case KeyCode::NumLock     : return "Num-lock"_s;
		case KeyCode::ScrollLock  : return "Scroll-lock"_s;
		case KeyCode::LeftShift   : return "Left Shift"_s;
		case KeyCode::RightShift  : return "Right Shift"_s;
		case KeyCode::LeftCtrl    : return "Left Ctrl"_s;
		case KeyCode::RightCtrl   : return "Right Ctrl"_s;
		case KeyCode::LeftAlt     : return "Left Alt"_s;
		case KeyCode::RightAlt    : return "Right Alt"_s;
		case KeyCode::LeftMeta    : return "Left Meta"_s;
		case KeyCode::RightMeta   : return "Right Meta"_s;
		case KeyCode::Count       : return "Invalid"_s;
		case KeyCode::Shift       : return "Shift"_s;
		case KeyCode::Ctrl        : return "Ctrl"_s;
		case KeyCode::Alt         : return "Alt"_s;
		case KeyCode::Meta        : return "Meta"_s;
		default: 
			return String{ UCodepoint(key) };
		}
	}

	Keyboard::Keyboard()
		: Device("keyboard"_s)
	{
		m_devInfo.type = DeviceType::Keyboard;
		m_devInfo.uid = "keyboard"; // Special case for uid
	}

	void Keyboard::PreTick() noexcept
	{
		m_prevKeys = m_curKeys;
	}

	void Keyboard::Tick() noexcept
	{
		if (!m_pManager || !m_pManager->IsMappingActive())
			return;

		for (usize i = 0; i < KeyCount; ++i)
		{
			if (IsPressed(KeyCode(i)))
				m_pManager->NotifyKeyForMapping(m_nativeKeys[i], Value{true});
		}
	}
	
	auto Keyboard::GetInputValue(u32 id) const noexcept -> Value
	{
		return IsDown(KeyCode(id));
	}
	
	void Keyboard::OnRegister(InputManager* pManager) noexcept
	{
		m_pManager = pManager;

		pManager->RegisterKey(this, "backspace"_s   , { KeyFlag::Button                    , "Backspace"_s        }, u32(KeyCode::Backspace));
		pManager->RegisterKey(this, "tab"_s         , { KeyFlag::Button                    , "Tab"_s              }, u32(KeyCode::Tab));
		pManager->RegisterKey(this, "enter"_s       , { KeyFlag::Button                    , "Enter"_s            }, u32(KeyCode::Enter));
		pManager->RegisterKey(this, "pause"_s       , { KeyFlag::Button                    , "Pause/Break"_s      }, u32(KeyCode::Escape));
		pManager->RegisterKey(this, "escape"_s      , { KeyFlag::Button                    , "Escape"_s           }, u32(KeyCode::Escape));
		pManager->RegisterKey(this, "pageup"_s      , { KeyFlag::Button                    , "Page up"_s          }, u32(KeyCode::PageUp));
		pManager->RegisterKey(this, "pagedown"_s    , { KeyFlag::Button                    , "Page down"_s        }, u32(KeyCode::PageDown));
		pManager->RegisterKey(this, "home"_s        , { KeyFlag::Button                    , "Home"_s             }, u32(KeyCode::Home));
		pManager->RegisterKey(this, "end"_s         , { KeyFlag::Button                    , "End"_s              }, u32(KeyCode::End));
		pManager->RegisterKey(this, "left"_s        , { KeyFlag::Button                    , "Left"_s             }, u32(KeyCode::Left));
		pManager->RegisterKey(this, "right"_s       , { KeyFlag::Button                    , "Right"_s            }, u32(KeyCode::Right));
		pManager->RegisterKey(this, "up"_s          , { KeyFlag::Button                    , "Up"_s               }, u32(KeyCode::Up));
		pManager->RegisterKey(this, "down"_s        , { KeyFlag::Button                    , "Down"_s             }, u32(KeyCode::Down));
		pManager->RegisterKey(this, "printscreen"_s , { KeyFlag::Button                    , "Print-screen"_s     }, u32(KeyCode::PrintScreen));
		pManager->RegisterKey(this, "insert"_s      , { KeyFlag::Button                    , "Insert"_s           }, u32(KeyCode::Insert));
		pManager->RegisterKey(this, "delete"_s      , { KeyFlag::Button                    , "Delete"_s           }, u32(KeyCode::Delete));

		pManager->RegisterKey(this, "numpad0"_s     , { KeyFlag::Button                    , "Numpad 0"_s         }, u32(KeyCode::Num0));
		pManager->RegisterKey(this, "numpad1"_s     , { KeyFlag::Button                    , "Numpad 1"_s         }, u32(KeyCode::Num1));
		pManager->RegisterKey(this, "numpad2"_s     , { KeyFlag::Button                    , "Numpad 2"_s         }, u32(KeyCode::Num2));
		pManager->RegisterKey(this, "numpad3"_s     , { KeyFlag::Button                    , "Numpad 3"_s         }, u32(KeyCode::Num3));
		pManager->RegisterKey(this, "numpad4"_s     , { KeyFlag::Button                    , "Numpad 4"_s         }, u32(KeyCode::Num4));
		pManager->RegisterKey(this, "numpad5"_s     , { KeyFlag::Button                    , "Numpad 5"_s         }, u32(KeyCode::Num5));
		pManager->RegisterKey(this, "numpad6"_s     , { KeyFlag::Button                    , "Numpad 6"_s         }, u32(KeyCode::Num6));
		pManager->RegisterKey(this, "numpad7"_s     , { KeyFlag::Button                    , "Numpad 7"_s         }, u32(KeyCode::Num7));
		pManager->RegisterKey(this, "numpad8"_s     , { KeyFlag::Button                    , "Numpad 8"_s         }, u32(KeyCode::Num8));
		pManager->RegisterKey(this, "numpad9"_s     , { KeyFlag::Button                    , "Numpad 9"_s         }, u32(KeyCode::Num9));
		pManager->RegisterKey(this, "numpadmul"_s   , { KeyFlag::Button                    , "Numpad *"_s         }, u32(KeyCode::NumMultiply));
		pManager->RegisterKey(this, "numpadadd"_s   , { KeyFlag::Button                    , "Numpad +"_s         }, u32(KeyCode::NumAdd));
		pManager->RegisterKey(this, "numpadsub"_s   , { KeyFlag::Button                    , "Numpad -"_s         }, u32(KeyCode::NumSub));
		pManager->RegisterKey(this, "numpaddiv"_s   , { KeyFlag::Button                    , "Numpad /"_s         }, u32(KeyCode::NumDivide));
		pManager->RegisterKey(this, "numpadsep"_s   , { KeyFlag::Button                    , "Numpad separator"_s }, u32(KeyCode::NumSeparator));
		pManager->RegisterKey(this, "numpaddec"_s   , { KeyFlag::Button                    , "Numpad decimal"_s   }, u32(KeyCode::NumDecimal));

		pManager->RegisterKey(this, "space"_s       , { KeyFlag::Button                    , "Space"_s            }, u32(KeyCode::Space));
		pManager->RegisterKey(this, "singlequote"_s , { KeyFlag::Button                    , "Single quote"_s     }, u32(KeyCode::SingleQuote));
		pManager->RegisterKey(this, "comma"_s       , { KeyFlag::Button                    , "Comma"_s            }, u32(KeyCode::Comma));
		pManager->RegisterKey(this, "minus"_s       , { KeyFlag::Button                    , "Minus"_s            }, u32(KeyCode::Minus));
		pManager->RegisterKey(this, "period"_s      , { KeyFlag::Button                    , "Period"_s           }, u32(KeyCode::Period));
		pManager->RegisterKey(this, "slash"_s       , { KeyFlag::Button                    , "Slash"_s            }, u32(KeyCode::Slash));

		pManager->RegisterKey(this, "0"_s           , { KeyFlag::Button                    , "0"_s                }, u32(KeyCode::Digit0));
		pManager->RegisterKey(this, "1"_s           , { KeyFlag::Button                    , "1"_s                }, u32(KeyCode::Digit1));
		pManager->RegisterKey(this, "2"_s           , { KeyFlag::Button                    , "2"_s                }, u32(KeyCode::Digit2));
		pManager->RegisterKey(this, "3"_s           , { KeyFlag::Button                    , "3"_s                }, u32(KeyCode::Digit3));
		pManager->RegisterKey(this, "4"_s           , { KeyFlag::Button                    , "4"_s                }, u32(KeyCode::Digit4));
		pManager->RegisterKey(this, "5"_s           , { KeyFlag::Button                    , "5"_s                }, u32(KeyCode::Digit5));
		pManager->RegisterKey(this, "6"_s           , { KeyFlag::Button                    , "6"_s                }, u32(KeyCode::Digit6));
		pManager->RegisterKey(this, "7"_s           , { KeyFlag::Button                    , "7"_s                }, u32(KeyCode::Digit7));
		pManager->RegisterKey(this, "8"_s           , { KeyFlag::Button                    , "8"_s                }, u32(KeyCode::Digit8));
		pManager->RegisterKey(this, "9"_s           , { KeyFlag::Button                    , "9"_s                }, u32(KeyCode::Digit9));

		pManager->RegisterKey(this, "semicolon"_s   , { KeyFlag::Button                    , "Semicolon"_s        }, u32(KeyCode::Semicolon));
		pManager->RegisterKey(this, "equals"_s      , { KeyFlag::Button                    , "Equals"_s           }, u32(KeyCode::Equals));

		pManager->RegisterKey(this, "f1"_s          , { KeyFlag::Button                    , "F1"_s               }, u32(KeyCode::F1));
		pManager->RegisterKey(this, "f2"_s          , { KeyFlag::Button                    , "F2"_s               }, u32(KeyCode::F2));
		pManager->RegisterKey(this, "f3"_s          , { KeyFlag::Button                    , "F3"_s               }, u32(KeyCode::F3));
		pManager->RegisterKey(this, "f4"_s          , { KeyFlag::Button                    , "F4"_s               }, u32(KeyCode::F4));
		pManager->RegisterKey(this, "f5"_s          , { KeyFlag::Button                    , "F5"_s               }, u32(KeyCode::F5));
		pManager->RegisterKey(this, "f6"_s          , { KeyFlag::Button                    , "F6"_s               }, u32(KeyCode::F6));
		pManager->RegisterKey(this, "f7"_s          , { KeyFlag::Button                    , "F7"_s               }, u32(KeyCode::F7));
		pManager->RegisterKey(this, "f8"_s          , { KeyFlag::Button                    , "F8"_s               }, u32(KeyCode::F8));
		pManager->RegisterKey(this, "f9"_s          , { KeyFlag::Button                    , "F9"_s               }, u32(KeyCode::F9));
		pManager->RegisterKey(this, "f10"_s         , { KeyFlag::Button                    , "F10"_s              }, u32(KeyCode::F10));
		pManager->RegisterKey(this, "f11"_s         , { KeyFlag::Button                    , "F11"_s              }, u32(KeyCode::F11));
		pManager->RegisterKey(this, "f12"_s         , { KeyFlag::Button                    , "F12"_s              }, u32(KeyCode::F12));

		pManager->RegisterKey(this, "capslock"_s    , { KeyFlag::Button                    , "Caps-lock"_s        }, u32(KeyCode::CapsLock));
		pManager->RegisterKey(this, "numlock"_s     , { KeyFlag::Button                    , "Num-lock"_s         }, u32(KeyCode::NumLock));
		pManager->RegisterKey(this, "scrolllock"_s  , { KeyFlag::Button                    , "Scroll-lock"_s      }, u32(KeyCode::ScrollLock));

		pManager->RegisterKey(this, "leftshift"_s   , { KeyFlag::Button | KeyFlag::Modifier, "Left shift"_s       }, u32(KeyCode::LeftShift));
		pManager->RegisterKey(this, "rightshift"_s  , { KeyFlag::Button | KeyFlag::Modifier, "Right shift"_s      }, u32(KeyCode::RightShift));
		pManager->RegisterKey(this, "shift"_s       , { KeyFlag::Button | KeyFlag::Modifier, "Shift"_s            }, u32(KeyCode::Shift));
		pManager->RegisterKey(this, "leftctrl"_s    , { KeyFlag::Button | KeyFlag::Modifier, "Left ctrl"_s        }, u32(KeyCode::LeftCtrl));
		pManager->RegisterKey(this, "rightctrl"_s   , { KeyFlag::Button | KeyFlag::Modifier, "Right ctrl"_s       }, u32(KeyCode::RightCtrl));
		pManager->RegisterKey(this, "ctrl"_s        , { KeyFlag::Button | KeyFlag::Modifier, "Ctrl"_s             }, u32(KeyCode::Ctrl));
		pManager->RegisterKey(this, "leftalt"_s     , { KeyFlag::Button | KeyFlag::Modifier, "Left alt"_s         }, u32(KeyCode::LeftAlt));
		pManager->RegisterKey(this, "rightalt"_s    , { KeyFlag::Button | KeyFlag::Modifier, "Right alt"_s        }, u32(KeyCode::RightAlt));
		pManager->RegisterKey(this, "alt"_s         , { KeyFlag::Button | KeyFlag::Modifier, "Alt"_s              }, u32(KeyCode::Alt));
		pManager->RegisterKey(this, "leftmeta"_s    , { KeyFlag::Button | KeyFlag::Modifier, "Left meta"_s        }, u32(KeyCode::LeftMeta));
		pManager->RegisterKey(this, "rightmeta"_s   , { KeyFlag::Button | KeyFlag::Modifier, "Right meta"_s       }, u32(KeyCode::RightMeta));
		pManager->RegisterKey(this, "meta"_s        , { KeyFlag::Button | KeyFlag::Modifier, "Meta"_s             }, u32(KeyCode::Meta));

		pManager->RegisterKey(this, "leftbracket"_s , { KeyFlag::Button                    , "Left bracket"_s     }, u32(KeyCode::LeftBracket));
		pManager->RegisterKey(this, "backslash"_s   , { KeyFlag::Button                    , "Backslash"_s        }, u32(KeyCode::BackSlash));
		pManager->RegisterKey(this, "rightbracket"_s, { KeyFlag::Button                    , "Right bracket"_s    }, u32(KeyCode::RightBracket));
		pManager->RegisterKey(this, "backquote"_s   , { KeyFlag::Button                    , "Backquote"_s        }, u32(KeyCode::BackQuote));

		pManager->RegisterKey(this, "a"_s           , { KeyFlag::Button                    , "a"_s                }, u32(KeyCode::A));
		pManager->RegisterKey(this, "b"_s           , { KeyFlag::Button                    , "b"_s                }, u32(KeyCode::B));
		pManager->RegisterKey(this, "c"_s           , { KeyFlag::Button                    , "c"_s                }, u32(KeyCode::C));
		pManager->RegisterKey(this, "d"_s           , { KeyFlag::Button                    , "d"_s                }, u32(KeyCode::D));
		pManager->RegisterKey(this, "e"_s           , { KeyFlag::Button                    , "e"_s                }, u32(KeyCode::E));
		pManager->RegisterKey(this, "f"_s           , { KeyFlag::Button                    , "f"_s                }, u32(KeyCode::F));
		pManager->RegisterKey(this, "g"_s           , { KeyFlag::Button                    , "g"_s                }, u32(KeyCode::G));
		pManager->RegisterKey(this, "h"_s           , { KeyFlag::Button                    , "h"_s                }, u32(KeyCode::H));
		pManager->RegisterKey(this, "i"_s           , { KeyFlag::Button                    , "i"_s                }, u32(KeyCode::I));
		pManager->RegisterKey(this, "j"_s           , { KeyFlag::Button                    , "j"_s                }, u32(KeyCode::J));
		pManager->RegisterKey(this, "k"_s           , { KeyFlag::Button                    , "k"_s                }, u32(KeyCode::K));
		pManager->RegisterKey(this, "l"_s           , { KeyFlag::Button                    , "l"_s                }, u32(KeyCode::L));
		pManager->RegisterKey(this, "m"_s           , { KeyFlag::Button                    , "m"_s                }, u32(KeyCode::M));
		pManager->RegisterKey(this, "n"_s           , { KeyFlag::Button                    , "n"_s                }, u32(KeyCode::N));
		pManager->RegisterKey(this, "o"_s           , { KeyFlag::Button                    , "o"_s                }, u32(KeyCode::O));
		pManager->RegisterKey(this, "p"_s           , { KeyFlag::Button                    , "p"_s                }, u32(KeyCode::P));
		pManager->RegisterKey(this, "q"_s           , { KeyFlag::Button                    , "q"_s                }, u32(KeyCode::Q));
		pManager->RegisterKey(this, "r"_s           , { KeyFlag::Button                    , "r"_s                }, u32(KeyCode::R));
		pManager->RegisterKey(this, "s"_s           , { KeyFlag::Button                    , "s"_s                }, u32(KeyCode::S));
		pManager->RegisterKey(this, "t"_s           , { KeyFlag::Button                    , "t"_s                }, u32(KeyCode::T));
		pManager->RegisterKey(this, "u"_s           , { KeyFlag::Button                    , "u"_s                }, u32(KeyCode::U));
		pManager->RegisterKey(this, "v"_s           , { KeyFlag::Button                    , "v"_s                }, u32(KeyCode::V));
		pManager->RegisterKey(this, "w"_s           , { KeyFlag::Button                    , "w"_s                }, u32(KeyCode::W));
		pManager->RegisterKey(this, "x"_s           , { KeyFlag::Button                    , "x"_s                }, u32(KeyCode::X));
		pManager->RegisterKey(this, "y"_s           , { KeyFlag::Button                    , "y"_s                }, u32(KeyCode::Y));
		pManager->RegisterKey(this, "z"_s           , { KeyFlag::Button                    , "z"_s                }, u32(KeyCode::Z));
	}

	void Keyboard::OnUnregister() noexcept
	{
		m_prevKeys.Clear();
		m_curKeys.Clear();
	}

	bool Keyboard::IsPressed(KeyCode key) const noexcept
	{
		if (u8(key) >= u8(KeyCode::Count))
		{
			switch (key)
			{
			case KeyCode::Shift: return IsPressed(KeyCode::LeftShift) || IsPressed(KeyCode::RightShift);
			case KeyCode::Ctrl : return IsPressed(KeyCode::LeftCtrl)  || IsPressed(KeyCode::RightCtrl);
			case KeyCode::Alt  : return IsPressed(KeyCode::LeftAlt)   || IsPressed(KeyCode::RightAlt);
			case KeyCode::Meta : return IsPressed(KeyCode::LeftMeta)  || IsPressed(KeyCode::RightMeta);
			default: return false;
			}
		}
		return m_curKeys[u8(key)] && !m_prevKeys[u8(key)];
	}

	bool Keyboard::IsDown(KeyCode key) const noexcept
	{
		if (u8(key) >= u8(KeyCode::Count))
		{
			switch (key)
			{
			case KeyCode::Shift: return IsDown(KeyCode::LeftShift) || IsDown(KeyCode::RightShift);
			case KeyCode::Ctrl : return IsDown(KeyCode::LeftCtrl)  || IsDown(KeyCode::RightCtrl);
			case KeyCode::Alt  : return IsDown(KeyCode::LeftAlt)   || IsDown(KeyCode::RightAlt);
			case KeyCode::Meta : return IsDown(KeyCode::LeftMeta)  || IsDown(KeyCode::RightMeta);
			default: return false;
			}
		}
		return m_curKeys[u8(key)];
	}

	bool Keyboard::IsReleased(KeyCode key) const noexcept
	{
		if (u8(key) >= u8(KeyCode::Count))
		{
			switch (key)
			{
			case KeyCode::Shift: return IsReleased(KeyCode::LeftShift) || IsReleased(KeyCode::RightShift);
			case KeyCode::Ctrl : return IsReleased(KeyCode::LeftCtrl)  || IsReleased(KeyCode::RightCtrl);
			case KeyCode::Alt  : return IsReleased(KeyCode::LeftAlt)   || IsReleased(KeyCode::RightAlt);
			case KeyCode::Meta : return IsReleased(KeyCode::LeftMeta)  || IsReleased(KeyCode::RightMeta);
			default: return false;
			}
		}
		return !m_curKeys[u8(key)] && m_prevKeys[u8(key)];
	}

	void Keyboard::OnKeyPressed(KeyCode key) noexcept
	{
		if (u8(key) >= u8(KeyCode::Count))
			return;

		g_Logger.Verbose(LogCategories::INPUT, "Pressed key: {}", key);
		m_curKeys.Set(usize(key));
	}

	void Keyboard::OnKeyReleased(KeyCode key) noexcept
	{
		if (u8(key) >= u8(KeyCode::Count))
			return;

		g_Logger.Verbose(LogCategories::INPUT, "Released key: {}", key);
		m_curKeys.Unset(usize(key));
	}
}
