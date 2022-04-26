#pragma once
#include "Device.h"
#include "core/MinInclude.h"
#include "core/utils/Flags.h"
#include "core/containers/InplaceBitSet.h"
#include "IInput.h"
#include "core/string/Include.h"

namespace Onca::Input
{
	/**
	 * Keyboard key codes
	 * \note keys are represented using their location relative to a QWERTY international equivalent,
	 *       e.g. A on AZERTY is represented as Q, as it matched the location on a QWERTY international keyboard
	 */
	enum class KeyCode : u8
	{
		Invalid      = 0xFF, ///< Invalid code
		Backspace    = 0   , ///< Backspace
		Tab                , ///< Tab
		Enter              , ///< Enter/Return
		Pause              , ///< Pause/Break
		Escape             , ///< Escape
		PageUp             , ///< Page up
		PageDown           , ///< Page down
		Home               , ///< Home
		End                , ///< End
		Left               , ///< Left
		Right              , ///< Right
		Up                 , ///< Up
		Down               , ///< Down
		PrintScreen        , ///< Printscreen
		Insert             , ///< Insert
		Delete             , ///< Delete

		Num0               , ///< Numpad 0
		Num1               , ///< Numpad 1
		Num2               , ///< Numpad 2
		Num3               , ///< Numpad 3
		Num4               , ///< Numpad 4
		Num5               , ///< Numpad 5
		Num6               , ///< Numpad 6
		Num7               , ///< Numpad 7
		Num8               , ///< Numpad 8
		Num9               , ///< Numpad 9
		NumMultiply        , ///< Numpad multiply
		NumAdd             , ///< Numpad add
		NumSub             , ///< Numpad subtract
		NumDivide          , ///< Numpad divide
		NumSeparator       , ///< Numpad separator
		NumDecimal         , ///< Numpad decimal

		Space        = ' ' , ///< Space
		SingleQuote  = '\'', ///< Single quote (QWERTY '")
		Comma        = ',' , ///< Comma (QWERTY ,<)
		Minus        = '-' , ///< Minus (QWERTY -_)
		Period       = '.' , ///< Period (QWERTY .>)
		Slash        = '/' , ///< Slash (QWERTY /?)

		Digit0       = '0' , ///< 0
		Digit1             , ///< 1
		Digit2             , ///< 2
		Digit3             , ///< 3
		Digit4             , ///< 4
		Digit5             , ///< 5
		Digit6             , ///< 6
		Digit7             , ///< 7
		Digit8             , ///< 8
		Digit9             , ///< 9
		
		Semicolon    = ';' , ///< Semicolon (QWERTY ;:)
		Equals       = '=' , ///< Equals (QWERTY =+)

		F1                 , ///< F1 
		F2                 , ///< F2 
		F3                 , ///< F3 
		F4                 , ///< F4 
		F5                 , ///< F5 
		F6                 , ///< F6 
		F7                 , ///< F7 
		F8                 , ///< F8 
		F9                 , ///< F9 
		F10                , ///< F10
		F11                , ///< F11
		F12                , ///< F12

		Clear              , ///< Clear key

		CapsLock           , ///< Caps lock
		NumLock            , ///< NumLock
		ScrollLock         , ///< Scroll lock

		LeftShift          , ///< Left shift
		RightShift         , ///< Right shift
		LeftCtrl           , ///< Left control
		RightCtrl          , ///< Right control
		LeftAlt            , ///< Left alt/menu
		RightAlt           , ///< Right alt/menu
		LeftMeta           , ///< Left Meta key (e.g. windows key)
		RightMeta          , ///< Right Meta key (e.g. windows key)

		LeftBracket  = '[' , ///< Left bracket (QWERTY [{)
		BackSlash    = '\\', ///< Back slash (QWERTY \| )
		RightBracket = ']' , ///< Left bracket (QWERTY ]})
		BackQuote    = '`' , ///< Back-quote (QWERTY `~)

		A            = 'a' , ///< A
		B                  , ///< B
		C                  , ///< C
		D                  , ///< D
		E                  , ///< E
		F                  , ///< F
		G                  , ///< G
		H                  , ///< H
		I                  , ///< I
		J                  , ///< J
		K                  , ///< K
		L                  , ///< L
		M                  , ///< M
		N                  , ///< N
		O                  , ///< O
		P                  , ///< P
		Q                  , ///< Q
		R                  , ///< R
		S                  , ///< S
		T                  , ///< T
		U                  , ///< U
		V                  , ///< V
		W                  , ///< W
		X                  , ///< X
		Y                  , ///< Y
		Z                  , ///< Z

		Count              , ///< Number of keys

		// Multi key keycodes
		Shift              , ///< Shift
		Ctrl               , ///< Shift
		Alt                , ///< Shift
		Meta               , ///< Shift 
	};

	CORE_API auto ToString(KeyCode key) noexcept -> String;

	enum class KeyModifier : u16
	{
		None       = 0                        , ///< No modifier
		LeftShift  = BIT(0)                   , ///< Left shift
		RightShift = BIT(1)                   , ///< Right shift
		Shift      = LeftShift | RightShift   , ///< Shift
		LeftCtrl   = BIT(2)                   , ///< Left control
		RightCtrl  = BIT(3)                   , ///< Right control
		Ctrl       = LeftCtrl  | RightCtrl    , ///< Shift
		LeftAlt    = BIT(4)                   , ///< Left alt
		RightAlt   = BIT(5)                   , ///< Right alt
		Alt        = LeftAlt   | RightAlt     , ///< Shift
		LeftMeta   = BIT(6)                   , ///< Left Meta key (e.g. windows key)
		RightMeta  = BIT(7)                   , ///< Right Meta key (e.g. windows key)
		Meta       = LeftMeta  | RightMeta    , ///< Shift
		ModMask    = Shift | Ctrl | Alt | Meta, ///< All modifiers
	};
	DEFINE_FLAGS(KeyModifier);

	/**
	 * Convert a key code to a modifier
	 * \param[in] code Key code
	 * \return Key modifier
	 */
	constexpr auto KeyCodeToModifier(KeyCode code) noexcept -> KeyModifier
	{
		switch (code)
		{
		case KeyCode::LeftShift:  return KeyModifier::LeftShift;
		case KeyCode::RightShift: return KeyModifier::RightShift;
		case KeyCode::Shift:      return KeyModifier::Shift;
		case KeyCode::LeftCtrl:   return KeyModifier::LeftCtrl;
		case KeyCode::RightCtrl:  return KeyModifier::RightCtrl;
		case KeyCode::Ctrl:       return KeyModifier::Ctrl;
		case KeyCode::LeftAlt:    return KeyModifier::LeftAlt;
		case KeyCode::RightAlt:   return KeyModifier::RightAlt;
		case KeyCode::Alt:        return KeyModifier::Alt;
		case KeyCode::LeftMeta:   return KeyModifier::LeftMeta;
		case KeyCode::RightMeta:  return KeyModifier::RightMeta;
		case KeyCode::Meta:       return KeyModifier::Meta;
		default:                  return KeyModifier::None;
		}
	}

	// TODO: Raw text from scancodes
	class CORE_API Keyboard final : Device
	{
	public:
		Keyboard();

		/**
		 * Tick before input is polled from the system
		 */
		void PreTick() noexcept override;
		/**
		 * Tick the input device
		 */
		void Tick() noexcept override;
		/**
		 * Get the value of an input at a certain ID
		 * \param[in] id Id of the input
		 * \return Input value
		 */
		auto GetInputValue(u32 id) const noexcept -> Value override;
		/**
		 * Callback for when a device is registered to the input manager
		 * \param[in] pManager Pointer to the input manager
		 * \note m_deviceId is set by the input manager before invoking this callback
		 */
		void OnRegister(InputManager* pManager) noexcept override;
		/**
		 * Callback fro when a device is unregistered/removed from the input manager
		 */
		void OnUnregister() noexcept override;

		/**
		 * Check if the key was pressed this frame
		 * \param[in] key Key code
		 * \return Whether the key was pressed this frame
		 */
		bool IsPressed(KeyCode key) const noexcept;
		/**
		 * Check if the key was down this frame
		 * \param[in] key Key code
		 * \return Whether the key was down this frame
		 */
		bool IsDown(KeyCode key) const noexcept;
		/**
		 * Check if the key was released this frame
		 * \param[in] key Key code
		 * \return Whether the key was released this frame
		 */
		bool IsReleased(KeyCode key) const noexcept;

		/**
		 * Key pressed callback
		 * \param[in] key Key code
		 * \note Should not be called by the user
		 */
		void OnKeyPressed(KeyCode key) noexcept;
		/**
		 * Key pressed callback
		 * \param[in] key Key code
		 * \note Should not be called by the user
		 */
		void OnKeyReleased(KeyCode key) noexcept;


	private:
		static constexpr usize KeyCount = usize(KeyCode::Count);

		InplaceBitSet<KeyCount> m_prevKeys; ///< Keys that were actuated this frame
		InplaceBitSet<KeyCount> m_curKeys;  ///< Keys that are currently down
	};
}

DEFINE_ENUM_FLAG_OPS(Input::KeyCode);
DEFINE_ENUM_FLAG_OPS(Input::KeyModifier);
