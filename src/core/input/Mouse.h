#pragma once
#include "Common.h"
#include "Device.h"
#include "Key.h"
#include "core/containers/InplaceBitSet.h"

namespace Onca::Input
{
	enum class MouseCode : u8
	{
		Invalid      = 0xFF, ///< Invalid mouse input code
		LeftButton   = 0   , ///< Left mouse button
		RightButton        , ///< Right mouse button
		MiddleButton       , ///< Middle mouse button
		ThumbButton0       , ///< First mouse thumb button
		ThumbButton1       , ///< Second mouse thumb button
		ScrollUp           , ///< Mouse scroll up
		ScrollDown         , ///< Mouse scroll down
		ButtonCount        , ///< Button count
		ScrollAxis         , ///< Mouse scroll axis
		X                  , ///< Mouse X axis
		Y                  , ///< Mouse Y axis
		XY                 , ///< Mouse XY 2D Axes
		DeltaX             , ///< Mouse X delta
		DeltaY             , ///< Mouse Y delta
		DeltaXY            , ///< Mouse XY delta
	};

	CORE_API auto ToString(MouseCode code) noexcept -> String;

	class CORE_API Mouse final : Device
	{
	public:
		Mouse();

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
		 * Check if the button was pressed this frame
		 * \param[in] button Key code
		 * \return Whether the key was pressed this frame
		 */
		auto IsPressed(MouseCode button) const noexcept -> bool;
		/**
		 * Check if the button was down this frame
		 * \param[in] button Key code
		 * \return Whether the key was down this frame
		 */
		auto IsDown(MouseCode button) const noexcept -> bool;
		/**
		 * Check if the button was released this frame
		 * \param[in] button Key code
		 * \return Whether the key was released this frame
		 */
		auto IsReleased(MouseCode button) const noexcept -> bool;

		/**
		 * Button pressed callback
		 * \param[in] button Button code
		 * \note Should not be called by the user
		 */
		void OnButtonPressed(MouseCode button) noexcept;
		/**
		 * Button pressed callback
		 * \param[in] button Button code
		 * \note Should not be called by the user
		 */
		void OnButtonReleased(MouseCode button) noexcept;
		/**
		 * Mouse moved callback
		 * \param[in] position Mouse position
		 */
		void OnMouseMoved(i32v2 position) noexcept;

	private:
		static constexpr usize ButtonCount = usize(MouseCode::ButtonCount);

		InplaceBitSet<ButtonCount> m_prevButtons; ///< Buttons that were down last frame
		InplaceBitSet<ButtonCount> m_curButtons;  ///< Buttons that are currently down
		i32v2                      m_oldPos;      ///< Old position
		i32v2                      m_curPos;      ///< New position
	};
}
