#include "Mouse.h"

#include "InputManager.h"

namespace Onca::Input
{
	auto ToString(MouseCode code) noexcept -> String
	{
		switch (code)
		{
		case MouseCode::LeftButton  : return "Left button"_s;
		case MouseCode::RightButton : return "Right button"_s;
		case MouseCode::MiddleButton: return "Middle button"_s;
		case MouseCode::ThumbButton0: return "Thumb button 0"_s;
		case MouseCode::ThumbButton1: return "Thumb button 1"_s;
		case MouseCode::ScrollUp    : return "Scroll up"_s;
		case MouseCode::ScrollDown  : return "Scroll down"_s;
		case MouseCode::ScrollAxis  : return "Scroll axis"_s;
		case MouseCode::X           : return "Mouse X"_s;
		case MouseCode::Y           : return "Mouse Y"_s;
		case MouseCode::XY          : return "Mouse XY"_s;
		case MouseCode::ButtonCount:
		case MouseCode::Invalid:
		default:  return "Invalid"_s;
		}
	}

	Mouse::Mouse()
		: Device("mouse"_s)
	{
		m_devInfo.type = DeviceType::Mouse;
		m_devInfo.uid = "mouse"; // Special case for uid
	}

	void Mouse::PreTick() noexcept
	{
		m_prevButtons = m_curButtons;
		m_oldPos = m_curPos;
	}

	void Mouse::Tick() noexcept
	{
		for (usize i = 0; i < ButtonCount; ++i)
		{
			if (!IsPressed(MouseCode(i)))
				m_pManager->NotifyKeyForMapping(m_nativeKeys[i], Value{true});
		}

		// Scroll
		m_pManager->NotifyKeyForMapping(m_nativeKeys[usize(MouseCode::ScrollAxis)], GetInputValue(u32(MouseCode::ScrollAxis)));

		// delta
		m_pManager->NotifyKeyForMapping(m_nativeKeys[usize(MouseCode::DeltaX)], GetInputValue(u32(MouseCode::DeltaX)));
		m_pManager->NotifyKeyForMapping(m_nativeKeys[usize(MouseCode::DeltaY)], GetInputValue(u32(MouseCode::DeltaY)));
		m_pManager->NotifyKeyForMapping(m_nativeKeys[usize(MouseCode::DeltaXY)], GetInputValue(u32(MouseCode::DeltaXY)));

		// position
		m_pManager->NotifyKeyForMapping(m_nativeKeys[usize(MouseCode::X)], GetInputValue(u32(MouseCode::X)));
		m_pManager->NotifyKeyForMapping(m_nativeKeys[usize(MouseCode::Y)], GetInputValue(u32(MouseCode::Y)));
		m_pManager->NotifyKeyForMapping(m_nativeKeys[usize(MouseCode::XY)], GetInputValue(u32(MouseCode::XY)));
	}
	
	auto Mouse::GetInputValue(u32 id) const noexcept -> Value
	{
		switch (MouseCode(id))
		{
		case MouseCode::LeftButton:
		case MouseCode::RightButton:
		case MouseCode::MiddleButton:
		case MouseCode::ThumbButton0:
		case MouseCode::ThumbButton1:
		case MouseCode::ScrollUp:
		case MouseCode::ScrollDown:
			return m_curButtons[id];
		case MouseCode::X:
			return f32(m_curPos.x);
		case MouseCode::Y:
			return f32(m_curPos.y);
		case MouseCode::XY:
			return f32v2{ m_curPos };
		case MouseCode::ScrollAxis:
			return { m_curButtons[usize(MouseCode::ScrollUp)  ] ?  1.0f :
				     m_curButtons[usize(MouseCode::ScrollDown)] ? -1.0f :
				                                                   0.0f };
		case MouseCode::DeltaX:
			return f32(m_curPos.x - m_oldPos.x);
		case MouseCode::DeltaY:
			return f32(m_curPos.y - m_oldPos.y);
		case MouseCode::DeltaXY:
			f32v2{ m_curPos.x - m_oldPos.x, m_curPos.y - m_oldPos.y };
		case MouseCode::Invalid:
		case MouseCode::ButtonCount:
		default:
			return false;
		}
	}

	void Mouse::OnRegister(InputManager* pManager) noexcept
	{
		m_pManager = pManager;

		pManager->RegisterKey(this, "leftbutton"_s  , { KeyFlag::Button, "Left button"_s     }, u32(MouseCode::LeftButton));
		pManager->RegisterKey(this, "rightbutton"_s , { KeyFlag::Button, "Right button"_s    }, u32(MouseCode::RightButton));
		pManager->RegisterKey(this, "middlebutton"_s, { KeyFlag::Button, "Middle button"_s   }, u32(MouseCode::MiddleButton));
		pManager->RegisterKey(this, "xbutton0"_s    , { KeyFlag::Button, "Thumb button 0"_s  }, u32(MouseCode::ThumbButton0));
		pManager->RegisterKey(this, "xbutton1"_s    , { KeyFlag::Button, "Thumb button 1"_s  }, u32(MouseCode::ThumbButton1));
		pManager->RegisterKey(this, "scroll"_s      , { KeyFlag::Axis1D, "Scroll axis"_s     }, u32(MouseCode::ScrollAxis));
		pManager->RegisterKey(this, "scroll/up"_s   , { KeyFlag::Button, "Scroll down"_s     }, u32(MouseCode::ScrollDown));
		pManager->RegisterKey(this, "scroll/down"_s , { KeyFlag::Button, "Scroll up"_s       }, u32(MouseCode::ScrollUp));
		pManager->RegisterKey(this, "position"_s    , { KeyFlag::Axis2D, "Mouse XY"_s        }, u32(MouseCode::XY));
		pManager->RegisterKey(this, "position/x"_s  , { KeyFlag::Axis1D, "Mouse X"_s         }, u32(MouseCode::X));
		pManager->RegisterKey(this, "position/y"_s  , { KeyFlag::Axis1D, "Mouse Y"_s         }, u32(MouseCode::Y));
		pManager->RegisterKey(this, "delta"_s       , { KeyFlag::Axis2D, "Mouse XY delta"_s  }, u32(MouseCode::DeltaXY));
		pManager->RegisterKey(this, "delta/x"_s     , { KeyFlag::Axis1D, "Mouse X delta"_s   }, u32(MouseCode::DeltaX));
		pManager->RegisterKey(this, "delta/y"_s     , { KeyFlag::Axis1D, "Mouse Y delta"_s   }, u32(MouseCode::DeltaY));
	}

	void Mouse::OnUnregister() noexcept
	{
		m_prevButtons.Clear();
		m_curButtons.Clear();
	}

	auto Mouse::IsPressed(MouseCode button) const noexcept -> bool
	{
		return u8(button) < u8(MouseCode::ButtonCount) && m_curButtons[u8(button)] && !m_prevButtons[u8(button)];
	}

	auto Mouse::IsDown(MouseCode button) const noexcept -> bool
	{
		return u8(button) < u8(MouseCode::ButtonCount) && m_curButtons[u8(button)];
	}

	auto Mouse::IsReleased(MouseCode button) const noexcept -> bool
	{
		return u8(button) < u8(MouseCode::ButtonCount) && !m_curButtons[u8(button)] && m_prevButtons[u8(button)];
	}

	void Mouse::OnButtonPressed(MouseCode button) noexcept
	{
		if (usize(button) < usize(MouseCode::ButtonCount))
			m_curButtons.Set(usize(button));
	}

	void Mouse::OnButtonReleased(MouseCode button) noexcept
	{
		if (usize(button) < usize(MouseCode::ButtonCount))
			m_curButtons.Unset(usize(button));
	}

	void Mouse::OnMouseMoved(i32v2 position) noexcept
	{
		m_curPos = position;
	}
}
