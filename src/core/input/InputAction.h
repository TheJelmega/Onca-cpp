#pragma once
#include "Common.h"
#include "core/utils/Delegate.h"

namespace Core::Input
{
	class User;
	class InputManager;
	class IModifier;
	class ITrigger;
	class Binding;

	// TODO: Add modifiers to apply after all mappings are combined
	class InputAction
	{
	public:
		// Callback(u32 user, TriggerEvent event, Value value, float dt)
		using Callback = Delegate<void(const User&, TriggerEvent, Value, float)>;

		/**
		 * Create an input action
		 * \param[in] name Name
		 * \param[in] valueType Value type
		 * \param[in] consumeInput Whether the action consumes the input
		 */
		InputAction(const String& name, ValueType valueType, bool consumeInput = false) noexcept;

		/**
		 * Get the name
		 * \return Name
		 */
		auto GetName() const noexcept -> const String& { return m_name; }
		/**
		 * Set if the action consumes the input
		 * \param[in] consume Whether to consume the input
		 */
		void SetConsumesInput(bool consume) noexcept { m_consumeInput = consume; }
		/**
		 * Check if the action consumes the input
		 * \return Whether the action consumes the input
		 */
		auto ConsumesInput() const noexcept -> bool { return m_consumeInput; }

		/**
		 * Register a callback
		 * \param[in] events Events for which to trigger the callback
		 * \param[in] callback callback
		 */
		void RegisterCallback(TriggerEvents events, const Callback& callback) noexcept;
		/**
		 * Unregister a callback for the given events
		 * \param[in] events Events to unregister for
		 * \param[in] callback Callback
		 */
		void UnregisterCallbackForEvents(TriggerEvents events, const Callback& callback) noexcept;
		/**
		 * Unregister a callback
		 * \param[in] callback Callback
		 */
		void UnregisterCallback(const Callback& callback) noexcept;

		/**
		 * Dispatch callbacks for this event
		 * \param[in] user User
		 * \param[in] event Trigger event
		 * \param[in] value Value
		 * \param[in] dt Delta time
		 */
		void DispatchCallbacks(const User& user, TriggerEvent event, Value value, f32 dt) noexcept;

	private:
		String                                  m_name;            ///< Input action name // TODO: Use interned string
		ValueType                               m_valueType;       ///< Value type
		bool                                    m_consumeInput;    ///< Whether the action consumes all input bound to the action (prevent other actions from using it after this action)
		DynArray<Pair<TriggerEvents, Callback>> m_callbacks;       ///< Registered callbacks
	};

}
