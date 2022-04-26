#include "Mapping.h"

#include "BuiltinTriggers.h"
#include "User.h"
#include "InputManager.h"
#include "core/logging/Logger.h"

namespace Core::Input
{
	InternalMapping::InternalMapping(const Rc<InputAction>& action, Unique<Binding>&& binding, const DynArray<u32>& validSchemes, DynArray<Unique<ITrigger>>&& triggers,
	                                 DynArray<Unique<IModifier>>&& modifiers) noexcept
		: m_action(action)
		, m_binding(Move(binding))
		, m_validSchemes(validSchemes)
		, m_triggers(Move(triggers))
		, m_modifiers(Move(m_modifiers))
		, m_prevState(TriggerState::None)
	{
	}

	InternalMapping::InternalMapping(const InternalMapping& other) noexcept
		: m_action(other.m_action)
		, m_prevState(TriggerState::None)
	{
		for (const Unique<ITrigger>& trigger : other.m_triggers)
			m_triggers.Add(trigger->Clone());

		for (const Unique<IModifier>& modifier : other.m_modifiers)
			m_modifiers.Add(modifier->Clone());
	}

	auto InternalMapping::GetValue(InputManager* pManager, User& user) noexcept -> Value
	{
		return m_binding->GetValue(pManager, user);
	}
	
	void InternalMapping::AddTrigger(Unique<ITrigger>&& trigger) noexcept
	{
		TriggerType type = trigger->GetTriggerType();

		// Insert the triggers in the most optimal order
		// TODO: could this be done with a .Sort() ???
		switch (type)
		{
		default:
		case TriggerType::Explicit:
		{
			m_triggers.Add(Move(trigger));
			break;
		}
		case TriggerType::Implicit:
		{
			auto it = m_triggers.Begin();
			for (auto end = m_triggers.End(); it != end; ++it)
			{
				if ((*it)->GetTriggerType() == TriggerType::Explicit)
					break;
			}
			//m_triggers.Insert(it, Move(trigger));
			break;
		}
		case TriggerType::Blocker:
		{
			m_triggers.Insert(m_triggers.Begin(), Move(trigger));
			break;
		}
		}
	}

	void InternalMapping::GetChordedActions(HashSet<Weak<InputAction>>& actions) const noexcept
	{
		for (const Unique<ITrigger>& trigger : m_triggers)
		{
			// Use RTTI instead of builtin dynamic cast
			if (const ChordedActionTrigger* pChordedAction = dynamic_cast<const ChordedActionTrigger*>(trigger.Get()))
				actions.Insert(pChordedAction->GetAction());
		}
	}

	auto InternalMapping::UsesChords() const noexcept -> bool
	{
		for (const Unique<ITrigger>& trigger : m_triggers)
		{
			// Use RTTI instead of builtin dynamic cast
			if (const ChordedActionTrigger* pChordedAction = dynamic_cast<const ChordedActionTrigger*>(trigger.Get()))
				return true;
		}
		return false;
	}

	auto InternalMapping::Execute(InputManager* pManager, User& user, Chrono::DeltaTime dt) noexcept -> TriggerInfo
	{
		Value val = GetValue(pManager, user);
		for (Unique<IModifier>& modifier : m_modifiers)
			modifier->Modify(val);

		TriggerState state = CalcTriggerState(m_triggers, user, val, dt);

		val.Clamp();
		return { state, val };
	}

	auto InternalMapping::CalcTriggerState(DynArray<Unique<ITrigger>>& triggers, User& user, const Value& val, Chrono::DeltaTime dt) noexcept -> TriggerState
	{
		if (triggers.IsEmpty())
			return val.value.IsNearlyZero() ? TriggerState::None : TriggerState::Triggered;

		bool anyExplicit = false;
		bool anyTriggered = false;
		bool anyOngoing = false;

		// Triggers are sorted beforehand, so we can rely on that order
		
		auto it = triggers.Begin();
		auto end = triggers.End();
		for (; it != end && (*it)->GetTriggerType() == TriggerType::Blocker; ++it)
		{
			TriggerState state = (*it)->Tick(user, val, dt);

			// Early exit
			if (state == TriggerState::Triggered)
				return TriggerState::None;

			(*it)->m_oldValue = val;
		}

		bool onlyBlockers = true;
		for (; it != end && (*it)->GetTriggerType() == TriggerType::Implicit; ++it)
		{
			TriggerState state = (*it)->Tick(user, val, dt);

			// Early exit
			if (state != TriggerState::Triggered)
				return TriggerState::None;

			(*it)->m_oldValue = val;
			onlyBlockers = false;
			anyOngoing |= state != TriggerState::None;
		}

		for (; it != end; ++it) // TriggerType::Explicit
		{
			TriggerState state = (*it)->Tick(user, val, dt);

			anyExplicit = true;
			onlyBlockers = false;
			anyTriggered |= state == TriggerState::Triggered;
			anyOngoing |= state != TriggerState::None;
		}

		// If the mapping only contains blockers, activate for any non-zero value
		if (onlyBlockers)
			return val.value.IsNearlyZero() ? TriggerState::None : TriggerState::Triggered;

		bool triggered = !anyExplicit || anyTriggered;
		return triggered ? TriggerState::Triggered : anyOngoing ? TriggerState::Ongoing : TriggerState::None;
	}

	ControlBinding::ControlBinding(const ControlBinding& binding) noexcept
		: m_binding(binding.m_binding->Clone())
		, m_modifierKeys(binding.m_modifierKeys)
		, m_validSchemes(binding.m_validSchemes)
		, m_mappingInfo(binding.m_mappingInfo)
	{
		for (const Unique<ITrigger>& trigger : binding.m_triggers)
			m_triggers.Add(trigger->Clone());
		for (const Unique<IModifier>& modifier : binding.m_modifiers)
			m_modifiers.Add(modifier->Clone());
	}

	ControlBinding::ControlBinding(ControlBinding&& binding) noexcept
		: m_binding(Move(binding.m_binding))
		, m_modifierKeys(Move(binding.m_modifierKeys))
		, m_validSchemes(Move(m_validSchemes))
		, m_triggers(Move(binding.m_triggers))
		, m_modifiers(Move(m_modifiers))
		, m_mappingInfo(Move(m_mappingInfo))
	{
	}

	auto ControlBinding::operator=(const ControlBinding& binding) noexcept -> ControlBinding&
	{
		m_binding = binding.m_binding->Clone();
		m_modifierKeys = binding.m_modifierKeys;
		m_validSchemes = binding.m_validSchemes;
		m_mappingInfo = binding.m_mappingInfo;

		for (const Unique<ITrigger>& trigger : binding.m_triggers)
			m_triggers.Add(trigger->Clone());
		for (const Unique<IModifier>& modifier : binding.m_modifiers)
			m_modifiers.Add(modifier->Clone());

		return *this;
	}

	auto ControlBinding::operator=(ControlBinding&& binding) noexcept -> ControlBinding&
	{
		m_binding = Move(binding.m_binding);
		m_modifierKeys = Move(binding.m_modifierKeys);
		m_validSchemes = Move(m_validSchemes);
		m_triggers = Move(binding.m_triggers);
		m_modifiers = Move(m_modifiers);
		m_mappingInfo = Move(m_mappingInfo);
		return *this;
	}

	void ControlBinding::SetBinding(Unique<Binding>&& binding) noexcept
	{
		m_binding = Move(binding);
	}

	void ControlBinding::AddModifierKey(const Key& key) noexcept
	{
		if (key.IsModifier())
			m_modifierKeys.AddUnique(key);
	}

	void ControlBinding::RemoveModifierKey(const Key& key) noexcept
	{
		m_modifierKeys.Erase(key);
	}

	void ControlBinding::AddValidScheme(u32 schemeId) noexcept
	{
		m_validSchemes.AddUnique(schemeId);
	}

	void ControlBinding::AddTrigger(Unique<ITrigger>&& trigger) noexcept
	{
		m_triggers.Add(Move(trigger));
	}

	void ControlBinding::RemoveTrigger(usize idx) noexcept
	{
		if (idx < m_triggers.Size())
			m_triggers.Erase(m_triggers.IteratorAt(idx));
	}

	void ControlBinding::AddModifier(Unique<IModifier>&& modifier) noexcept
	{
		m_modifiers.Add(Move(modifier));
	}

	void ControlBinding::RemoveModifier(usize idx) noexcept
	{
		if (idx < m_modifiers.Size())
			m_triggers.Erase(m_triggers.IteratorAt(idx));
	}

	void ControlBinding::MoveModifier(usize from, usize to) noexcept
	{
		if (from >= m_modifiers.Size() || to >= m_modifiers.Size())
			return;

		Unique<IModifier> modifier = m_modifiers.Extract(from);
		m_modifiers.Insert(m_modifiers.IteratorAt(to), Move(modifier));
	}

	void ControlBinding::Rebind(RebindState& state) noexcept
	{
		// TODO: use RTTI
		if (KeyBinding* pKeyBinding = dynamic_cast<KeyBinding*>(m_binding.Get()))
			m_modifierKeys = state.modifiers;
		m_binding->BindKey(state.path, state.key);
	}

	void ControlBinding::RemoveValidScheme(u32 schemeId) noexcept
	{
		m_validSchemes.Erase(schemeId);
	}

	void ActionMapping::AddControlBinding(ControlBinding&& binding) noexcept
	{
		m_bindings.Add(Move(binding));
	}

	void ActionMapping::RemoveControlBinding(usize idx) noexcept
	{
		if (idx < m_bindings.Size())
			m_bindings.Erase(m_bindings.IteratorAt(idx));
	}

	void MappingContext::AddInputMapping(const ActionMapping& mapping) noexcept
	{
		m_mappings.Add(mapping);
	}

	void MappingContext::RemoveInputMapping(u32 idx) noexcept
	{
		if (idx >= m_mappings.Size())
			return;

		m_mappings.Erase(m_mappings.IteratorAt(idx));
	}

	void MappingContext::SetDescription(const String& desc) noexcept
	{
		m_desc = desc;
	}

	void MappingContext::Rebind(RebindState& state)
	{
		usize slashIdx = state.path.Find('/');
		String mappingName = state.path.SubString(0, slashIdx);
		String subPath = state.path.SubString(slashIdx + 1);

		for (ActionMapping& mapping : m_mappings)
		{
			if (mapping.GetAction() != state.action)
				continue;

			for (ControlBinding& binding : mapping.GetControlBindings())
			{
				BindingMappingInfo& info = binding.GetMappingInfo();
				if (!info.mappable || info.name != mappingName)
					continue;

				binding.Rebind(state);
			}
		}
	}

	auto MappingContext::GetOrderedMappings(User& user) const noexcept -> DynArray<InternalMapping>
	{
		// Split mapping into chorded and other mappings
		DynArray<InternalMapping> chordedMappings;
		DynArray<InternalMapping> otherMappings;
		HashSet<Weak<InputAction>> chordedActions;
		HashSet<Weak<InputAction>> allActions;

		DynArray<InternalMapping> mapping = BuildInternalMapping(user);

		for (const InternalMapping& mapping : mapping)
		{
			DynArray<InternalMapping>& mappingArray = mapping.UsesChords() ? chordedMappings : otherMappings;
			mappingArray.Add(mapping);
			mapping.GetChordedActions(chordedActions);
			allActions.Insert(mapping.GetInputAction());
		}

		DynArray<InternalMapping> chordedChordingMappings;
		DynArray<InternalMapping> chordingMappings;

		HashSet<Weak<InputAction>> chordedChordingActions;
		HashSet<Weak<InputAction>> chordingActions;

		auto extractChording = [&chordedActions](DynArray<InternalMapping>& mappings, DynArray<InternalMapping>& chording, HashSet<Weak<InputAction>>& actions) -> void
		{
			for (usize i = 0; i < mappings.Size();)
			{
				if (chordedActions.Contains(mappings[i].GetInputAction()))
				{
					actions.Insert(mappings[i].GetInputAction());
					chording.Add(mappings.Extract(i));
				}
				else
				{
					++i;
				}
			}
		};
		extractChording(chordedMappings, chordedChordingMappings, chordedChordingActions);
		extractChording(otherMappings, chordingMappings, chordingActions);

		DynArray<InternalMapping> orderedchordedChordingMappings;
		// Now we just need to sort all chorded chording mappings
		// keep on extracting all mappings that don't rely on other unordered chorded chording actions
		// We limit this to the number of actions that need to initially be ordered, if there are still left, we have circular chording 
		HashSet<Weak<InputAction>> tmpActions;
		for (usize loopIdx = 0, loopMax = chordedChordingMappings.Size(); loopIdx < loopMax; ++loopIdx)
		{
			for (usize i = 0; i < chordedChordingMappings.Size(); ++i)
			{
				chordedChordingMappings[i].GetChordedActions(tmpActions);
				bool found = false;
				for (const Weak<InputAction>& action : tmpActions)
				{
					if (chordedChordingActions.Contains(action))
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					chordingActions.Insert(chordedChordingMappings[i].GetInputAction());
					chordedChordingActions.Erase(chordedChordingMappings[i].GetInputAction());
					orderedchordedChordingMappings.Add(chordedChordingMappings.Extract(i));
				}
				else
				{
					++i;
				}
			}
		}

		if (!chordedChordingActions.IsEmpty())
			g_Logger.Warning(LogCategories::INPUT, "Circular chord dependencies found in input mapping, input system might not act as expected");

		// Now we just combined the ordered chords in the following order
		// chording > ordered chorded chording > unordered chorded chording > chorded > other
		mapping.Clear();
		mapping.Add(Move(chordingMappings));
		mapping.Add(Move(orderedchordedChordingMappings));
		mapping.Add(Move(chordedChordingMappings));
		mapping.Add(Move(chordedMappings));
		mapping.Add(Move(otherMappings));
		return mapping;
	}

	auto MappingContext::BuildInternalMapping(User& user) const noexcept -> DynArray<InternalMapping>
	{
		DynArray<InternalMapping> mappings;

		for (const ActionMapping& mapping : m_mappings)
		{
			Rc<InputAction> action = mapping.GetAction();
			for (const ControlBinding& binding : mapping.GetControlBindings())
			{
				DynArray<Unique<ITrigger>> triggers;
				DynArray<Unique<IModifier>> modifiers;

				for (const Unique<ITrigger>& trigger : binding.GetTriggers())
					triggers.Add(trigger->Clone());
				for (const Unique<IModifier>& modifier : binding.GetModifiers())
					modifiers.Add(modifier->Clone());

				for (const Key& key : binding.GetModifierKeys())
				{
					Weak<InputAction> action = user.RequestModifierChordAction(key);
					triggers.Add(Unique<ChordedActionTrigger>::Create(action));
				}

				InternalMapping internal{ action, binding.GetBinding()->Clone(), binding.GetValidSchemes(), Move(triggers), Move(modifiers) };

				mappings.Add(Move(internal));
			}
		}

		return mappings;
	}
}
