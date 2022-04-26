#include "InputAction.h"

#include "ControlSet.h"
#include "InputManager.h"

namespace Core::Input
{
	InputAction::InputAction(const String& name, ValueType valueType, bool consumeInput) noexcept
		: m_name(name)
		, m_valueType(valueType)
		, m_consumeInput(consumeInput)
	{
	}

	void InputAction::RegisterCallback(TriggerEvents events, const Callback& callback) noexcept
	{
		bool found = false;
		for (Pair<TriggerEvents, Callback>& pair : m_callbacks)
		{
			if (pair.second != callback)
				continue;

			pair.first |= events;
			found = true;
			break;
		}

		if (!found)
			m_callbacks.EmplaceBack(events, callback);
	}

	void InputAction::UnregisterCallbackForEvents(TriggerEvents events, const Callback& callback) noexcept
	{
		for (Pair<TriggerEvents, Callback>& pair : m_callbacks)
		{
			if (pair.second == callback)
				pair.first &= ~events;
		}

		m_callbacks.EraseIf([&callback](const Pair<TriggerEvents, Callback>& pair) -> bool
		{
			return pair.first == TriggerEvent::None && pair.second == callback;
		});
	}

	void InputAction::UnregisterCallback(const Callback& callback) noexcept
	{
		m_callbacks.EraseIf([&callback](const Pair<TriggerEvents, Callback>& pair) -> bool
		{
			return pair.second == callback;
		});
	}

	void InputAction::DispatchCallbacks(const User& user, TriggerEvent event, Value value, f32 dt) noexcept
	{
		value = { value, m_valueType };

		for (Pair<TriggerEvents, Callback>& pair : m_callbacks)
		{
			if (!(pair.first & event))
				continue;

			if (event == TriggerEvent::StartedTriggered)
			{
				if (pair.first & TriggerEvent::Started)
					pair.second(user, TriggerEvent::Started, value, dt);
				if (pair.first & TriggerEvent::Triggered)
					pair.second(user, TriggerEvent::Triggered, value, dt);
			}
			else
			{
				pair.second(user, event, value, dt);
			}
		}
	}
}
