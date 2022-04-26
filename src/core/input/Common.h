#pragma once
#include "core/MinInclude.h"
#include "core/math/Math.h"
#include "core/logging/LogCategory.h"

namespace LogCategories
{
	DEFINE_LOG_CATEGORY(INPUT);
}

namespace Onca::Input
{
	constexpr u32 InvalidId = u32(-1);

	struct InputCode
	{
		InputCode() : devId(u16(-1)), code(0) {}
		InputCode(u16 devId, u16 code) : devId(devId), code(code) {}

		u16 devId;
		u16 code;
	};


	enum class ValueType : u8
	{
		Digital, ///< Boolean value
		Axis   , ///< Axis
		Axis2D , ///< 2D axis
		Axis3D , ///< 3D axis
	};

	struct Value
	{
		Value() noexcept;

		Value(bool val) noexcept;
		Value(f32 val) noexcept;
		Value(f32v2 val) noexcept;
		Value(f32v3 val) noexcept;
		Value(ValueType type) noexcept;

		Value(const Value& other, ValueType type) noexcept;

		auto operator+=(const Value& other) noexcept -> Value&;

		/**
		 * Get the digital value
		 * \return Digital value
		 * \note Can also be called on a value of type Axis
		 */
		auto Digital() const noexcept -> bool;
		/**
		 * Get the axis value
		 * \return Axis value
		 * \note Can also be called on a value of type Digital
		 */
		auto Axis() const noexcept -> f32;
		/**
		 * Get the 2D axis value
		 * \return 2D axis value
		 */
		auto Axis2D() const noexcept -> f32v2;
		/**
		 * Get the 3D axis value
		 * \return 3D axis value
		 */
		auto Axis3D() const noexcept -> f32v3;

		/**
		 * Clamp the axis values between -1 and 1
		 */
		void Clamp() noexcept;
		
		ValueType type;    ///< Input value type
		f32v3     value;   ///< Input value
	};

	/**
	 * Type of trigger, i.e. tells what triggers need to be triggered for the action to execute
	 */
	enum class TriggerType
	{
		Explicit, ///< Explicit, if present, any explicit trigger needs to be triggered
		Implicit, ///< Implicit, if present, all implicit triggers need to be triggered
		Blocker , ///< Blocker, if present, no blockers should be triggered
	};

	/**
	 * Current state of the trigger
	 */
	enum class TriggerState : u8
	{
		None     , ///< The trigger is waiting for input
		Ongoing  , ///< The trigger is busy processing an input
		Triggered, ///< The trigger is triggered
	};

	/**
	 * Trigger events that can be passed to an input action
	 */
	enum class TriggerEvent : u8
	{
		None             = BIT(0), ///< None (None)
		Started          = BIT(1), ///< Trigger started (None -> Ongoing, None -> Triggered)
		Ongoing          = BIT(2), ///< Trigger in progress (None -> Ongoing, Ongoing -> Ongoing)
		Triggered        = BIT(3), ///< Triggered (None -> Triggered, Ongoing -> Triggered, Triggered -> Triggered)
		Canceled         = BIT(4), ///< Trigger Canceled (Ongoing -> None)
		Ended            = BIT(5), ///< Trigger ended (Triggered -> None)
		StartedTriggered = Started | Triggered, /// Started + Triggered
		Any              = Started | Ongoing | Triggered | Canceled | Ended, ///< Any trigger state (for input action callback binding)
	};
	DEFINE_FLAGS(TriggerEvent);

	/**
	 * Info about the current state and value of a processed trigger
	 */
	struct TriggerInfo
	{
		TriggerState state; ///< State
		Value        value; ///< Value
	};

	/**
	 * Calculate the trigger event based of the previous and current trigger state
	 * \param[in] prev Previous trigger state
	 * \param[in] cur Current trigger state
	 * \return Trigger event
	 */
	auto CalculateEvent(TriggerState prev, TriggerState cur) noexcept -> TriggerEvent;
}

DEFINE_ENUM_FLAG_OPS(Onca::Input::TriggerEvent);

#include "Common.inl"
