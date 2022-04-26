#pragma once

#if __RESHARPER__
#include "Common.h"
#endif

namespace Onca::Input
{
	inline Value::Value() noexcept
		: type(ValueType::Axis)
	{}

	inline Value::Value(bool val) noexcept
		: type(ValueType::Digital)
	    , value(val ? 1.f : 0.f)
	{}

	inline Value::Value(f32 val) noexcept
		: type(ValueType::Axis)
	    , value(val, 0.f, 0.f)
	{}

	inline Value::Value(f32v2 val) noexcept
		: type(ValueType::Axis2D)
	     , value(val, 0.f)
	{}

	inline Value::Value(f32v3 val) noexcept
		: type(ValueType::Axis3D)
		, value(val)
	{}

	inline Value::Value(const Value& other, ValueType type) noexcept
		: type(type)
		, value(other.value)
	{}

	inline auto Value::operator+=(const Value& other) noexcept -> Value&
	{
		value += other.value;
		return *this;
	}

	inline Value::Value(ValueType type) noexcept
		: type(type)
	{}

	inline auto Value::Digital() const noexcept -> bool
	{
		ASSERT(type == ValueType::Digital || type == ValueType::Axis, "");
		return value.x > Math::Consts::MathEpsilon<f32>;
	}

	inline auto Value::Axis() const noexcept -> f32
	{
		ASSERT(type == ValueType::Digital || type == ValueType::Axis, "");
		return value.x;
	}

	inline auto Value::Axis2D() const noexcept -> f32v2
	{
		ASSERT(type == ValueType::Axis2D, "");
		return { value.x, value.y };
	}

	inline auto Value::Axis3D() const noexcept -> f32v3
	{
		ASSERT(type == ValueType::Axis3D, "");
		return value;
	}

	inline void Value::Clamp() noexcept
	{
		value.Clamp(-1.f, 1.f);
	}

	// State to Event:
	// Old state -> New state = Event
	//
	// None      -> None      = None
	// None      -> Ongoing   = Started
	// None      -> Triggered = Started + Triggered
	// Ongoing   -> None      = Canceled
	// Ongoing   -> Ongoing   = Ongoing
	// Ongoing   -> Triggered = Triggered
	// Triggered -> None      = Ended
	// Triggered -> Ongoing   = Ongoing
	// Triggered -> Triggered = Triggered
	inline auto CalculateEvent(TriggerState prev, TriggerState cur) noexcept -> TriggerEvent
	{
		switch (prev)
		{
		case TriggerState::None:
		{
			switch (cur)
			{
			case TriggerState::None:      return TriggerEvent::None;
			case TriggerState::Ongoing:   return TriggerEvent::Started;
			case TriggerState::Triggered: return TriggerEvent::StartedTriggered;
			default:                      return TriggerEvent::None;
			}
		}
		case TriggerState::Ongoing:
		{
			switch (cur)
			{
			case TriggerState::None:      return TriggerEvent::Canceled;
			case TriggerState::Ongoing:   return TriggerEvent::Ongoing;
			case TriggerState::Triggered: return TriggerEvent::Triggered;
			default:                      return TriggerEvent::None;
			}
		}
		case TriggerState::Triggered:
		{
			switch (cur)
			{
			case TriggerState::None:      return TriggerEvent::Ended;
			case TriggerState::Ongoing:   return TriggerEvent::Ongoing;
			case TriggerState::Triggered: return TriggerEvent::Triggered;
			default:                      return TriggerEvent::None;
			}
		}
		default:                          return TriggerEvent::None;
		}
	}
}
