#pragma once
#include "Common.h"
#include "InputAction.h"
#include "Bindings.h"
#include "Key.h"
#include "core/chrono/DeltaTime.h"
#include "core/containers/HashSet.h"
#include "core/memory/RefCounted.h"
#include "core/memory/Unique.h"

namespace Core::Input
{
	struct RebindState;

	struct BindingMappingInfo
	{
		bool   mappable = false; ///< Whether the binding is mappable
		String name;             ///< Mapping name (bindings with the same name will be mapped to the same key)
		String displayName;      ///< Display name // TODO: Localized string
		String category;         ///< Category // TODO: Localized string
	};

	// TODO: UI: visually split actions from keys: i.e. mapping sets action, different keys are visualized as bindings under that mapping
	class InternalMapping
	{
	public:

		InternalMapping() noexcept = default;

		InternalMapping(const Rc<InputAction>& action, Unique<Binding>&& binding, const DynArray<u32>& validSchemes, DynArray<Unique<ITrigger>>&& triggers, DynArray<Unique<IModifier>>&& modifiers) noexcept;

		InternalMapping(const InternalMapping& other) noexcept;
		InternalMapping(InternalMapping&& other) noexcept = default;

		auto operator=(InternalMapping&& other) noexcept -> InternalMapping& = default;
		
		/**
		 * Get the input action
		 * \return Input action
		 */
		auto GetInputAction() const noexcept -> const Rc<InputAction>& { return m_action; }

		/**
		 * Get the value for this binding
		 * \param[in] pManager Input manager
		 * \param[in] user Current user
		 * \return Binding value85
		 */
		auto GetValue(InputManager* pManager, User& user) noexcept -> Value;
		
		/**
		 * Get the key
		 * \return Key
		 */
		auto GetKey() const noexcept -> const Key& { return m_key; }
		/**
		 * Get the binding
		 * \return Binding
		 */
		auto GetBinding() const noexcept -> const Unique<Binding>& { return m_binding; }

		/**
		 * Add a control scheme for which this binding is valid
		 * \param[in] id Control set ID
		 */
		void AddValidScheme(u32 id) noexcept { m_validSchemes.AddUnique(id); }
		/**
		 * Remove a control scheme for which the binding is valid
		 * \param[in] id Control scheme ID
		 */
		void RemoveValidScheme(u32 id) noexcept { m_validSchemes.Erase(id); }
		/**
		 * Get the control schemes for which the binding is valid
		 * \return Controls schemes for which the binding is valid
		 */
		auto GetValidSchemes() const noexcept -> DynArray<u32> { return m_validSchemes; }

		/**
		 * Set the trigger for the binding
		 * \param[in] trigger Trigger
		 */
		void AddTrigger(Unique<ITrigger>&& trigger) noexcept;
		/**
		 * Get the trigger for the binding
		 * \return Trigger
		 */
		auto GetTriggers() noexcept -> DynArray<Unique<ITrigger>>& { return m_triggers; }
		/**
		 * Get the trigger for the binding
		 * \return Trigger
		 */
		auto GetTriggers() const noexcept -> const DynArray<Unique<ITrigger>>& { return m_triggers; }
		
		/**
		 * Get the binding specific modifiers
		 * \return Modifiers
		 */
		auto GetModifiers() noexcept -> DynArray<Unique<IModifier>>& { return m_modifiers; }
		/**
		 * Get the binding specific modifiers
		 * \return Modifiers
		 */
		auto GetModifiers() const noexcept -> const DynArray<Unique<IModifier>>& { return m_modifiers; }


		/**
		 * Get the actions on which the binding relies and add them to the given array of actions
		 * \param[in] actions Actions
		 */
		void GetChordedActions(HashSet<Weak<InputAction>>& actions) const noexcept;

		/**
		 * Check if the mapping uses chorded actions
		 * \return Whether the mapping uses chorded actions
		 */
		auto UsesChords() const noexcept -> bool;

		/**
		 * Execute the input action (process bindings, dispatch callbacks, etc)
		 * \param[in] pManager Input manager
		 * \param[in] user Current user
		 * \param[in] dt Delta time
		 */
		auto Execute(InputManager* pManager, User& user, Chrono::DeltaTime dt) noexcept -> TriggerInfo;

	private:

		auto CalcTriggerState(DynArray<Unique<ITrigger>>& triggers, User& user, const Value& val, Chrono::DeltaTime dt) noexcept -> TriggerState;

		friend class InputManager;

		Rc<InputAction>             m_action;       ///< Associated input action
		Unique<Binding>             m_binding;
		Key                         m_key;          ///< Key

		DynArray<u32>               m_validSchemes; ///< Control schemes for which this binding is valid
		DynArray<Unique<ITrigger>>  m_triggers;     ///< Triggers
		DynArray<Unique<IModifier>> m_modifiers;    ///< Binding specific modifiers

		TriggerState                m_prevState;    ///< Previous trigger state
	};

	class ControlBinding
	{
	public:
		ControlBinding() noexcept = default;

		ControlBinding(const ControlBinding& binding) noexcept;
		ControlBinding(ControlBinding&& binding) noexcept;

		auto operator=(const ControlBinding& binding) noexcept -> ControlBinding&;
		auto operator=(ControlBinding&& binding) noexcept -> ControlBinding&;

		/**
		 * Set the actual binding
		 * \param[in] binding Binding
		 */
		void SetBinding(Unique<Binding>&& binding) noexcept;
		/**
		 * Get the actual binding
		 * \return Binding
		 */
		auto GetBinding() const noexcept -> const Unique<Binding>& { return m_binding; }

		/**
		 * Add a modifier key (key needs to have the modifier flag set)
		 * \param[in] key Modifier key
		 */
		void AddModifierKey(const Key& key) noexcept;
		/**
		 * Remove a key
		 * \param[in] key Modifier key
		 */
		void RemoveModifierKey(const Key& key) noexcept;
		/**
		 * Get the modifier keys
		 * \return Modifier keys
		 */
		auto GetModifierKeys() const noexcept -> const DynArray<Key>& { return m_modifierKeys; }

		/**
		 * Add a scheme id for which this binding is valid
		 * \param[in] schemeId Scheme id
		 */
		void AddValidScheme(u32 schemeId) noexcept;
		/**
		 * Remove a scheme id for which this binding is valid
		 * \param[in] schemeId Scheme id
		 */
		void RemoveValidScheme(u32 schemeId) noexcept;
		/**
		 * Get all valid scheme ids
		 * \return Scheme ids
		 */
		auto GetValidSchemes() const noexcept -> const DynArray<u32>& { return m_validSchemes; }

		/**
		 * Add a trigger to the binding
		 * \param[in] trigger Trigger
		 */
		void AddTrigger(Unique<ITrigger>&& trigger) noexcept;
		/**
		 * Remove a trigger at the given index
		 * \param[in] idx Index of trigger
		 */
		void RemoveTrigger(usize idx) noexcept;
		/**
		 * Get the triggers
		 * \return Trigger
		 */
		auto GetTriggers() const noexcept -> const DynArray<Unique<ITrigger>>& { return m_triggers; }

		/**
		 * Add a modifier to the binding
		 * \param[in] modifier Modifier
		 */
		void AddModifier(Unique<IModifier>&& modifier) noexcept;
		/**
		 * Remove a modifier at the given index
		 * \param[in] idx Index of modifier
		 */
		void RemoveModifier(usize idx) noexcept;
		/**
		 * Move a modifier from an index to another
		 * \param[in] from From idx
		 * \param[in] to To idx
		 */
		void MoveModifier(usize from, usize to) noexcept;
		/**
		 * Get the modifier
		 * \return Modifiers
		 */
		auto GetModifiers() const noexcept -> const DynArray<Unique<IModifier>>& { return m_modifiers; }

		/**
		 * Set the mapping info
		 * \param[in] mappingInfo Mapping info
		 */
		void SetMappingInfo(const BindingMappingInfo& mappingInfo) noexcept { m_mappingInfo = mappingInfo; }
		/**
		 * Get the mapping info
		 * \return Mapping info
		 */
		auto GetMappingInfo() noexcept -> BindingMappingInfo& { return m_mappingInfo; }
		/**
		 * Get the mapping info
		 * \return Mapping info
		 */
		auto GetMappingInfo() const noexcept -> const BindingMappingInfo& { return m_mappingInfo; }

		/**
		 * Rebind a key in the current mapping
		 * \param[in] state Rebind context
		 */
		void Rebind(RebindState& state) noexcept;

	private:
		Unique<Binding>             m_binding;      ///< Bindings
		DynArray<Key>               m_modifierKeys; ///< Modifiers (chorded mappings are automatically created for them)

		DynArray<u32>               m_validSchemes; ///< Control schemes for which this binding is valid
		DynArray<Unique<ITrigger>>  m_triggers;     ///< Triggers
		DynArray<Unique<IModifier>> m_modifiers;    ///< Binding specific modifiers

		BindingMappingInfo          m_mappingInfo; ///< Binding mapping info
	};

	/**
	 * User facing input mapping
	 */
	// TODO: in editor UI, combine multiple mappings with the same action into one on closing the UI
	class ActionMapping
	{
	public:

		/**
		 * Set the action
		 * \param[in] action Action
		 */
		void SetAction(const Rc<InputAction>& action) noexcept { m_action = action; }
		/**
		 * Get the action
		 * \return Action
		 */
		auto GetAction() noexcept -> Rc<InputAction>& { return m_action; }
		/**
		 * Get the action
		 * \return Action
		 */
		auto GetAction() const noexcept -> const Rc<InputAction>& { return m_action; }

		/**
		 * Add a control binding to the mapping
		 * \param[in] binding Control binding
		 */
		void AddControlBinding(ControlBinding&& binding) noexcept;
		/**
		 * Remove a control binding at a certain index
		 * \param[in] idx Index
		 */
		void RemoveControlBinding(usize idx) noexcept;
		/**
		 * Get the control bindings
		 * \return Control bindings
		 */
		auto GetControlBindings() noexcept -> DynArray<ControlBinding>& { return m_bindings; }
		/**
		 * Get the control bindings
		 * \return Control bindings
		 */
		auto GetControlBindings() const noexcept -> const DynArray<ControlBinding>& { return m_bindings; }

	private:
		Rc<InputAction>          m_action;      ///< Input action
		DynArray<ControlBinding> m_bindings;    ///< Bindings
	};


	class MappingContext
	{
	public:

		auto operator=(const MappingContext&) noexcept -> MappingContext& = default;

		/**
		 * Add an input mapping to the context
		 * \param[in] mapping Input mapping
		 */
		void AddInputMapping(const ActionMapping& mapping) noexcept;
		/**
		 * Remove an input mapping from the context
		 * \param[in] idx Index of the mapping
		 */
		void RemoveInputMapping(u32 idx) noexcept;
		/**
		 * Get the input mappings
		 * \return Input mappings
		 */
		auto GetInputMappings() noexcept -> DynArray<ActionMapping>& { return m_mappings; }
		/**
		 * Get the input mappings
		 * \return Input mappings
		 */
		auto GetInputMappings() const noexcept -> const DynArray<ActionMapping>& { return m_mappings; }

		/**
		 * Set the description
		 * \param[in] desc Description
		 */
		void SetDescription(const String& desc) noexcept;
		/**
		 * Get the description
		 * \return Description
		 */
		auto GetDescription() const noexcept -> const String& { return m_desc; }

		/**
		 * Check if the input mapping is empty
		 * \return Whether the input mapping is empty
		 */
		auto IsEmpty() const noexcept -> bool { return m_mappings.IsEmpty(); }

		/**
		 * Rebind a key in the current mapping context
		 * \param[in] path Mapping path (without the mapping name)
		 * \param[in] state Rebind context
		 */
		void Rebind(RebindState& state);

		/**
		 * Create an array of mappings that is already ordered
		 * \return Ordered mappings
		 */
		auto GetOrderedMappings(User& user) const noexcept -> DynArray<InternalMapping>;

	private:
		auto BuildInternalMapping(User& user) const noexcept -> DynArray<InternalMapping>;

		DynArray<ActionMapping> m_mappings; ///< Input mappings
		String                  m_desc;     ///< Description // TODO: Localized string
	};
}
