#include "User.h"

#include "BuiltinTriggers.h"
#include "Rebind.h"

namespace Onca::Input
{
	User::User() noexcept
		: m_id(InvalidId)
		, m_schemeId(InvalidId)
		, m_allowSchemeChange(true)
		, m_valid(false)
		, m_needMappingRebuild(false)
	{
	}

	User::User(u32 scheme, bool allowDynamicSchemeChange) noexcept
		: m_id(InvalidId)
		, m_schemeId(scheme)
		, m_allowSchemeChange(allowDynamicSchemeChange)
		, m_valid(false)
		, m_needMappingRebuild(false)
	{
	}

	void User::AddMappingContext(const Rc<MappingContext>& mapping, i32 priority) noexcept
	{
		auto it = m_mappingContexts.Begin();
		while (it != m_mappingContexts.End())
		{
			if (it->first <= priority)
			{
				m_mappingContexts.Emplace(it, priority, mapping);
				return;
			}
		}

		m_mappingContexts.EmplaceBack(priority, mapping);
	}

	void User::RemoveMappingContext(const Rc<MappingContext>& context) noexcept
	{
		m_mappingContexts.EraseIf([&context](const Pair<i32, Rc<MappingContext>>& pair) -> bool
		{
			return pair.second == context;
		});
	}

	void User::ClearMappingContexts() noexcept
	{
		m_mappingContexts.Clear();
	}

	void User::SetNeedToRebuildMappings(bool needed) noexcept
	{
		m_needMappingRebuild = needed;
	}

	void User::RebuildMappings()
	{
		if (!m_needMappingRebuild)
			return;

		m_mappings.Clear();

		HashSet<Key> encounteredKeys;
		for (Pair<i32, Rc<MappingContext>>& pair : m_mappingContexts)
		{
			DynArray<InternalMapping> orderedMappings = pair.second->GetOrderedMappings(*this);

			// Now add the mappings, but ignore ghosted input, i.e. inputs with keys that already occurred and that don't have chords
			for (usize i = 0; i < orderedMappings.Size();)
			{
				if (orderedMappings[i].UsesChords())
				{
					m_mappings.Add(orderedMappings.Extract(i));
				}
				else
				{
					if (encounteredKeys.Contains(orderedMappings[i].GetKey()))
					{
						++i;
						continue;
					}

					if (orderedMappings[i].GetInputAction()->ConsumesInput())
						encounteredKeys.Insert(orderedMappings[i].GetKey());

					m_mappings.Add(orderedMappings.Extract(i));
				}
			}
		}

		// Prepend modifiers
		for (Pair<const Key, Rc<InputAction>>& pair : m_modifierActions)
		{
			InternalMapping mapping{ pair.second, Unique<KeyBinding>::Create(pair.first), DynArray<u32>{},
				DynArray<Unique<ITrigger>>{ Unique<DownTrigger>::Create() }, DynArray<Unique<IModifier>>{} };
			m_mappings.Insert(m_mappings.Begin(), Move(mapping));
		}

		InjectChordBlockers();

		m_needMappingRebuild = false;
	}
	
	void User::UpdateTriggerInfo(const Weak<InputAction>& action, const TriggerInfo& info) noexcept
	{
		auto it = m_triggerInfo.Find(action);
		if (it == m_triggerInfo.End())
		{
			m_triggerInfo.Insert(action, info);
		}
		else
		{
			TriggerInfo& curInfo = it->second;
			if (u8(info.state) > u8(curInfo.state))
				curInfo = info;
			else
				curInfo.value.value += info.value.value;
		}
	}

	auto User::GetPrevTriggerState(const Weak<InputAction>& action) noexcept -> TriggerState
	{
		auto it = m_prevTriggerState.Find(action);
		if (it != m_prevTriggerState.End())
			return it->second;
		return TriggerState::None;
	}

	void User::ResetTriggerInfoAndUpdatePrevState() noexcept
	{
		m_prevTriggerState.Clear();
		for (Pair<const Weak<InputAction>, TriggerInfo>& pair : m_triggerInfo)
			m_prevTriggerState.Insert(pair.first, pair.second.state);

		m_triggerInfo.Clear();
	}

	void User::SetInputActionTriggered(Rc<InputAction>& action)
	{
		m_triggeredActions.Insert(action);
	}

	auto User::IsInputActionTriggered(const Rc<InputAction>& action) const noexcept -> bool
	{
		return m_triggeredActions.Contains(action);
	}

	auto User::IsInputActionTriggered(const Weak<InputAction>& action) const noexcept -> bool
	{
		return m_triggeredActions.Contains(action);
	}

	void User::ResetInputActionsTriggered() noexcept
	{
		m_triggeredActions.Clear();
	}

	void User::AddDisconnectedDevice(const DeviceInfo& info, u32 schemeId) noexcept
	{
		m_disconnectedDevices.Add({ info, schemeId });
	}

	void User::RemoveDisconnectedDevice(const DeviceInfo& info) noexcept
	{
		m_disconnectedDevices.EraseIf([&info](const Pair<DeviceInfo, u32>& pair) -> bool
		{
			return info.uid == pair.first.uid;
		});
	}

	void User::RemoveDisconnectedDeviceForScheme(u32 schemeId) noexcept
	{
		m_disconnectedDevices.EraseIf([&schemeId](const Pair<DeviceInfo, u32>& pair) -> bool
		{
			return schemeId == pair.second;
		});
	}

	auto User::GetDisconnectedDeviceScheme(const DeviceInfo& info) const noexcept -> u32
	{
		auto it = m_disconnectedDevices.FindIf([&info](const Pair<DeviceInfo, u32>& pair) -> bool
		{
			return info.uid == pair.first.uid;
		});

		if (it != m_disconnectedDevices.End())
			return it->second;
		return InvalidId;
	}

	auto User::RequestModifierChordAction(const Key& key) noexcept -> Rc<InputAction>
	{
		auto it = m_modifierActions.Find(key);
		if (it != m_modifierActions.End())
			return it->second;

		Rc<InputAction> action = Rc<InputAction>::Create(Format("__Modifier{}"_s, key.path), ValueType::Digital);
		m_modifierActions.Insert(key, action);
		return action;
	}

	auto User::IsValid() const noexcept -> bool
	{
		return m_valid;
	}

	void User::AddControlSet(u32 id) noexcept
	{
		m_controlSetIds.AddUnique(id);
	}

	void User::RemoveControlSet(u32 id) noexcept
	{
		m_controlSetIds.Erase(id);
	}

	void User::UnassignAllControlSets() noexcept
	{
		m_controlSetIds.Clear();
	}

	void User::InjectChordBlockers() noexcept
	{
		HashMap<Key, DynArray<Weak<InputAction>>> chordingActions;

		HashSet<Weak<InputAction>> chordedActions;
		for (InternalMapping& mapping : m_mappings)
		{
			chordedActions.Clear();
			mapping.GetChordedActions(chordedActions);

			const Key& key = mapping.GetKey();
			auto it = chordingActions.Find(key);

			// Add blockers for any chording action that is not chorded
			if (it != chordingActions.End())
			{
				for (Weak<InputAction>& action : it->second)
				{
					if (!chordedActions.Contains(action))
						mapping.AddTrigger(Unique<ChordedActionTriggerBlocker>::Create(action));
				}
			}

			// Add new chorded actions
			if (chordedActions.IsEmpty())
				continue;
			
			if (it == chordingActions.End())
				it = chordingActions.Insert(key, DynArray<Weak<InputAction>>{}).first;

			for (const Weak<InputAction>& action : chordedActions)
				it->second.AddUnique(action);
		}
	}
}
