#include "InputManager.h"

#include "Mapping.h"
#include "core/logging/Logger.h"

namespace Onca::Input
{
	InputManager::InputManager(bool createKbM)
		: m_pKeyboard(nullptr)
		, m_pMouse(nullptr)
		, m_allowMultiUser(false)
		, m_addUsersDynamic(false)
		, m_maxUsers(1)
	{
		SystemInit();
		if (createKbM)
		{
			RegisterDevice(Unique<Keyboard>::Create());
			RegisterDevice(Unique<Mouse>::Create());
		}

		AddUser(User{ InvalidId, true });
	}

	void InputManager::Shutdown() noexcept
	{
		m_devMapping.Clear(true);
		m_devs.Clear(true);
		m_devInitMapping.Clear(true);

		m_pKeyboard = nullptr;
		m_pMouse = nullptr;

		m_keys.Clear(true);
		m_keyDevNative.Clear(true);

		m_controlSchemes.Clear(true);
		m_controlSets.Clear(true);

		m_users.Clear(true);

		m_rawDevInfos.Clear(true);

		m_defNewUserMappingContext = nullptr;

		m_rebindContext.Invalidate();
		m_rebindState.Clear();

		m_deviceAddedCallbacks.Clear(true);
		m_deviceRemovedCallbacks.Clear(true);
		m_deviceReconnectedCallbacks.Clear(true);
		m_userAddedCallbacks.Clear(true);
		m_userRemovedCallbacks.Clear(true);
	}

	void InputManager::PreTick() noexcept
	{
		for (Unique<Device>& device : m_devs)
		{
			device->PreTick();
		}
	}

	void InputManager::Tick(Chrono::DeltaTime dt) noexcept
	{
		SystemTick();

		for (Unique<Device>& device : m_devs)
		{
			device->Tick();
		}
		
		if (IsMappingActive())
		{
			m_rebindState.time += dt.GetTime(false);
		
			if (m_rebindContext.IsTimeout(m_rebindState.time))
				m_rebindContext.Cancel(m_rebindState);
			return;
		}
		
		for (usize i = 0; i < m_users.Size(); ++i)
		{
			User& user = m_users[i];
			if (!user.IsValid())
				continue;
		
			user.RebuildMappings();
			user.ResetTriggerInfoAndUpdatePrevState();
		
			for (InternalMapping& mapping : user.GetMappings())
			{
				TriggerInfo info = mapping.Execute(this, user, dt);
				user.UpdateTriggerInfo(mapping.GetInputAction(), info);
			}
		
			// Dispatch callbacks for triggered actions
			for (Pair<const Weak<InputAction>, TriggerInfo>& pair : user.GetTriggerInfos())
			{
				Weak<InputAction> action = pair.first;
				TriggerState prevState = user.GetPrevTriggerState(action);
				TriggerEvent event = CalculateEvent(prevState, pair.second.state);
				action->DispatchCallbacks(user, event, pair.second.value, dt);
			}
		}
	}

	void InputManager::AllowMultiUser(bool allow) noexcept
	{
		if (m_allowMultiUser == allow)
			return;
		m_allowMultiUser = allow;

		if (m_users.IsEmpty())
			return;

		if (allow)
		{
			if (m_users.IsEmpty())
				return;

			User& user = m_users[0];
			const DynArray<u32>& controlSetIds = m_users[0].GetControlSetIds();
			if (controlSetIds.Size() == 1)
				return;

			for (u32 id : controlSetIds)
				RemoveControlSet(id);
			user.UnassignAllControlSets();
		}
		else
		{
			if (m_users.Size() <= 1)
				return;
			
			for (usize i = 1; i < m_users.Size(); ++i)
			{
				for (u32 controlSetId : m_users[i].GetControlSetIds())
				{
					RemoveControlSet(controlSetId);
				} 
			}
			m_users.Resize(1);
		}
	}

	void InputManager::SetMaxUsers(u8 maxUsers) noexcept
	{
		m_maxUsers = maxUsers;
		if (m_users.Size() > m_maxUsers)
		{
			for (u8 i = m_maxUsers; i < m_users.Size(); ++i)
			{
				for (u32 controlSetId : m_users[i].GetControlSetIds())
				{
					RemoveControlSet(controlSetId);
				}
			}
			m_users.Resize(m_maxUsers);
		}
	}

	auto InputManager::GetRawValue(const User& user, const Key& key) noexcept -> Value
	{
		Value accumulator;
		for (u32 controlSetId : user.GetControlSetIds())
		{
			const ControlSet& controlSet = m_controlSets[controlSetId];
			if (controlSet.IsNullSet())
				continue;

			if (controlSet.GetSchemeId() != user.GetSchemeId())
				continue;

			auto keyIt = m_keyDevNative.Find(key);
			if (keyIt == m_keyDevNative.End())
				continue;

			const String& devName = keyIt->second.first;
			u32 code = keyIt->second.second;

			auto devIt = m_devMapping.Find(devName);
			if (devIt == m_devMapping.End())
				continue;

			Device* pFoundDev = nullptr;
			for (u32 devId : devIt->second)
			{
				if (controlSet.ContainsDevice(devId))
				{
					pFoundDev = m_devs[devId].Get();
					break;
				}
			}

			// It's extremely unlikely to get multiple inputs, in the same scheme and in multiple different control set
			// But since their is a possibility, accumulate all of the raw values
			if (pFoundDev)
				accumulator += pFoundDev->GetInputValue(code);
		}
		return accumulator;
	}

	auto InputManager::RegisterDevice(Unique<Device>&& device) noexcept -> bool
	{
		const DeviceType type = device->GetDeviceInfo().type;
		if (type == DeviceType::Keyboard || type == DeviceType::Mouse)
		{
			for (Unique<Device>& dev : m_devs)
			{
				if (dev->GetDeviceInfo().type == type)
				{
					g_Logger.Warning(LogCategories::INPUT, "Only 1 keyboard and mouse are supported");
					return false;
				}
			}
		}

		// Find empty device
		u32 devId = 0;
		for (; devId < m_devs.Size(); ++devId)
		{
			if (!m_devs[devId])
				break;
		}

		device->m_deviceId = devId;
		if (devId == m_devs.Size())
			m_devs.Add(Move(device));
		else
			m_devs[devId] = Move(device);
		
		for (const InternedString& iden : m_devs[devId]->GetDeviceNames())
		{
			auto it = m_devMapping.Find(iden);
			if (it == m_devMapping.End())
			{
				it = m_devMapping.Insert(Move(iden), DynArray<u32>{}).first;
			}
			it->second.Add(devId);
		}

		// The memory stored by a unique pointer cannot be relocated, so we can cache the actual pointer to it
		if (type == DeviceType::Mouse)
			m_pMouse = reinterpret_cast<Mouse*>(m_devs[devId].Get());
		else if (type == DeviceType::Keyboard)
			m_pKeyboard = reinterpret_cast<Keyboard*>(m_devs[devId].Get());

		m_devs[devId]->OnRegister(this);
		OnDeviceAdded(devId, m_devs[devId].Get());

		for (User& user : m_users)
		{
			u32 schemeId = user.GetDisconnectedDeviceScheme(m_devs[devId]->GetDeviceInfo());
			if (schemeId == InvalidId)
				continue;

			ReconnectDevice(user, devId, schemeId);
			return true;
		}

		return true;
	}

	void InputManager::UnregisterDevice(Unique<Device>& device) noexcept
	{
		auto it = m_devs.Find(device);
		if (it == m_devs.end())
			return;

		u32 idx = u32(it - m_devs.Begin());
		for (Pair<const InternedString, DynArray<u32>>& pair : m_devMapping)
		{
			pair.second.Erase(idx);
		}
	}

	auto InputManager::HasDeviceWithName(const String& name) const noexcept -> bool
	{
		return m_devMapping.Contains(name);
	}

	auto InputManager::HasDeviceOfType(DeviceType type) const noexcept -> bool
	{
		return m_devs.ContainsIf([type](const Unique<Device>& dev) -> bool
		{
			return dev->GetDeviceInfo().type == type;
		});
	}

	auto InputManager::GetDevice(u32 devId) const noexcept -> const Device*
	{
		if (devId >= m_devs.Size())
			return nullptr;
		return m_devs[devId].Get();
	}

	auto InputManager::IsDeviceInUse(u32 devId) const noexcept -> bool
	{
		return GetDeviceControlSet(devId) != InvalidId;
	}

	auto InputManager::GetDeviceControlSet(u32 devId) const noexcept -> u32
	{
		for (u32 i = 0; i < m_controlSets.Size(); ++i)
		{
			if (m_controlSets[i].ContainsDevice(devId))
				return i;
		}
		return InvalidId;
	}

	auto InputManager::AddControlScheme(ControlScheme&& scheme) noexcept -> u32
	{
		const bool alreadyExists = m_controlSchemes.ContainsIf([&scheme](const ControlScheme& other) -> bool
		{
			return scheme == other;
		});
		if (alreadyExists)
			return InvalidId;

		u32 id = 0;
		for (; id < m_controlSchemes.Size(); ++id)
		{
			if (!m_controlSchemes[id].IsValid())
				break;
		}

		if (id == m_controlSchemes.Size())
			m_controlSchemes.Add(Move(scheme));
		else
			m_controlSchemes[id] = Move(scheme);

		return id;
	}

	void InputManager::RemoveControlScheme(const String& name) noexcept
	{
		auto it = m_controlSchemes.FindIf([&name](const ControlScheme& scheme) -> bool
		{
			return scheme.Name() == name;
		});

		if (it != m_controlSchemes.End())
			*it = ControlScheme{};
	}

	void InputManager::ComposePossibleControlSets() noexcept
	{
		// Collect all unused devices
		DynArray<Device*> unusedDevices;
		for (u32 i = 0; i < m_devs.Size(); ++i)
		{
			if (!IsDeviceInUse(i))
				unusedDevices.Add(m_devs[i].Get());
		}

		while (!unusedDevices.IsEmpty())
		{
			ControlScheme::MatchResult bestMatch;
			DynArray<String> conflictingDevices;
			u32 schemeId = 0;

			// Find a control scheme that can be created
			for (u32 i = 0; i < m_controlSchemes.Size(); ++i)
			{
				ControlScheme::MatchResult curMatch = m_controlSchemes[i].Match(unusedDevices);
				if (!curMatch.valid)
					continue;

				if (!bestMatch.valid)
				{
					bestMatch = curMatch;
				}
				else if (curMatch.score > bestMatch.score)
				{
					bestMatch = curMatch;
					schemeId = i;
					conflictingDevices.Clear();
				}
				else if (curMatch.score == bestMatch.score)
				{
					// Check for conflicting devices
					for (const String& name : bestMatch.usedDevices)
					{
						if (curMatch.usedDevices.Contains(name))
							conflictingDevices.Add(name);
					}
				}
			}

			if (!bestMatch.valid)
				return;

			// If we have conflicting devices, remove them and try again
			if (!conflictingDevices.IsEmpty())
			{
				unusedDevices.EraseIf([&conflictingDevices](const Device* dev) -> bool
				{
					const DynArray<InternedString>& devNames = dev->GetDeviceNames();
					for (const String& name : conflictingDevices)
					{
						if (devNames.Contains(name))
							return true;
					}
					return false;
				});
				continue;
			}

			// We found a scheme that can be created, so create it and assign it to the correct user
			DynArray<u32> devIds;
			for (const String& devName : bestMatch.usedDevices)
			{
				const DynArray<u32>& existingDevs = m_devMapping[devName];
				for (u32 devId : existingDevs)
				{
					if (!IsDeviceInUse(devId))
					{
						devIds.Add(devId);
						break;
					}
				}
				unusedDevices.Erase(m_devs[devIds.Back()].Get());
			}
			u32 setId = AddControlSet(ControlSet{ schemeId, Move(devIds) });

			// Assign the control set to a user
			if (m_allowMultiUser && m_maxUsers > 1)
			{
				for (User& user : m_users)
				{
					if (user.GetControlSetIds().IsEmpty())
					{
						user.AddControlSet(setId);
						setId = InvalidId;
						break;
					}
				}

				if (setId != InvalidId && m_addUsersDynamic && m_users.Size() < m_maxUsers)
				{
					u32 userId = AddUser(User{ schemeId, false });
					if (userId != InvalidId)
						m_users[userId].AddControlSet(setId);
				}
			}
			else
			{
				m_users[0].AddControlSet(setId);
			}
		}
	}

	auto InputManager::GetControlSet(u32 id) const noexcept -> const ControlSet&
	{
		static ControlSet dummy;
		if (id >= m_controlSets.Size() || m_controlSets[id].IsNullSet())
			return dummy;
		return m_controlSets[id];
	}

	auto InputManager::CreateNewUser(u32 scheme, bool allowDynamicSchemeChange) noexcept -> bool
	{
		u32 id = AddUser(User{ InvalidId, allowDynamicSchemeChange });
		if (id == InvalidId)
			return false;

		if (scheme == InvalidId || !TryComposeControlSchemeForUser(id, scheme))
			TryComposeControlSchemeForUser(id, InvalidId);
		return true;
	}

	auto InputManager::CreateNewUser(const Rc<MappingContext>& mapping, u32 scheme, bool allowDynamicSchemeChange) noexcept -> bool
	{
		if (mapping->IsEmpty())
			return false;

		u32 id = AddUser(User{ InvalidId, allowDynamicSchemeChange });
		if (id == InvalidId)
			return false;

		m_users[id].AddMappingContext(mapping);

		if (scheme == InvalidId || !TryComposeControlSchemeForUser(id, scheme))
			TryComposeControlSchemeForUser(id, InvalidId);
		return true;
	}

	void InputManager::RemoveUser(u32 id) noexcept
	{
		if (id >= m_users.Size())
			return;

		OnUserRemoved(id, m_users[id]);
		m_users[id].m_valid = false;
		m_users.Erase(m_users.IteratorAt(id));
	}

	auto InputManager::GetUser(u32 id) noexcept -> User&
	{
		static User dummy;
		if (id >= m_users.Size() || !m_users[id].IsValid())
			return dummy;
		return m_users[id];
	}
	
	void InputManager::ClearAllMappingContexts() noexcept
	{
		for (User& user : m_users)
		{
			if (user.IsValid())
				user.ClearMappingContexts();
		}
	}

	void InputManager::Rebind(const Rc<MappingContext>& mapping, RebindContext&& context)
	{
		m_rebindContext = Move(context);
		m_rebindState.mapping = mapping;
		m_rebindState.action = m_rebindContext.GetAction();
		m_rebindState.path = m_rebindContext.GetPath();
	}

	// TODO: Modifiers
	void InputManager::NotifyKeyForMapping(const Key& key, const Value& value)
	{
		if (!m_rebindContext.IsValid())
			return;

		if (value.value.IsNearlyZero())
		{
			if (m_rebindContext.IsModifier(key))
				m_rebindState.modifiers.Erase(key);
			return;
		}

		if (m_rebindContext.IsCancelKey(key))
		{
			m_rebindContext.Cancel(m_rebindState);
			return;
		}

		if (!m_rebindContext.IsKeyValid(key, value))
			return;

		m_rebindState.mapping->Rebind(m_rebindState);
		for (User& user : m_users)
		{
			if (!user.IsValid())
				continue;
			user.SetNeedToRebuildMappings(true);
		}

		m_rebindContext.Complete(m_rebindState);
	}

	auto InputManager::IsMappingActive() const noexcept -> bool
	{
		return m_rebindContext.IsValid();
	}

	void InputManager::RegisterKey(Device* pDev, const String& name, KeyDetails details, u32 nativeCode) noexcept
	{
		if (!pDev)
			return;

		const DynArray<InternedString>& devNames = pDev->GetDeviceNames();
		Key key;
		for (usize i = 0; i < devNames.Size(); ++i)
		{
			const InternedString& devName = devNames[i];
			key = { Format("<{}>/{}", devName, name) };

			auto it = m_keys.Find(key);
			if (it != m_keys.End())
			{
				u32 curNativeCode = m_keyDevNative[key].second;
				if (curNativeCode != nativeCode)
				{
					const InternedString& actDevName = pDev->GetDeviceNames().Back();
					g_Logger.Warning(LogCategories::INPUT, "Key '{}' has incompatible native code for '{}' when registering keys for '{}'", key.path, devName, actDevName);
				}

				pDev->AddKey(nativeCode, it->first);
				++it->second->devRefCnt;
				continue;;
			}
		}

		Rc<KeyDetails> rcDetails = Rc<KeyDetails>::Create(Move(details));
		++rcDetails->devRefCnt;
		key.details = rcDetails;
		m_keys.Insert(key, rcDetails);

		m_keyDevNative.Insert(key, { devNames.Back(), nativeCode });
		pDev->AddKey(nativeCode, key);
	}

	void InputManager::UnregisterKey(Device* pDev, const Key& key) noexcept
	{
		const String& path = key.path.Get();
		usize slashIdx = path.Find('/');
		String keyName = path.SubString(0, slashIdx);

		for (const String& devName : pDev->GetDeviceNames())
		{
			Key curKey{ Format("<{}>/{}", devName, keyName) };
			auto it = m_keys.Find(key);
			ASSERT(it->second->devRefCnt == 0, "devRefCount cannot be 0 before unregistering it");

			--it->second->devRefCnt;
			if (it->second->devRefCnt == 0)
			{
				m_keys.Erase(key);
				m_keyDevNative.Erase(key);
			}
		}
	}

	void InputManager::OnDeviceConnected(const DeviceInfo& info) noexcept
	{
		u32 id = (u32(info.vendorId) << 16) | info.productId;
		auto it = m_devInitMapping.Find(id);
		if (it == m_devInitMapping.End())
			return;

		Unique<Device> dev = it->second(info);
		RegisterDevice(Move(dev));
	}

	void InputManager::OnDeviceDisconnected(const DeviceInfo& info) noexcept
	{
		for (Unique<Device>& dev : m_devs)
		{
			if (dev->GetDeviceInfo().uid != info.uid)
				continue;


			return;
		}
	}

	auto InputManager::GetKeyboard() noexcept -> Keyboard*
	{
		ASSERT(m_pKeyboard, "No keyboard device is available");
		return m_pKeyboard;
	}

	auto InputManager::GetMouse() noexcept -> Mouse*
	{
		ASSERT(m_pMouse, "No mouse device is available");
		return m_pMouse;
	}

	void InputManager::RegisterDeviceInitializer(u16 vid, u16 pid, const DevInitDelegate& initializer) noexcept
	{
		u32 id = (u32(vid) << 16) | pid;
		m_devInitMapping.Insert(id, initializer);

		for (DeviceInfo& devInfo : m_rawDevInfos)
		{
			if (devInfo.vendorId == vid && devInfo.productId == pid)
				OnDeviceConnected(devInfo);
		}
	}

	void InputManager::RegisterDeviceAddedCallback(const Delegate<void(u32, const Device*)>& callback) noexcept
	{
		m_deviceAddedCallbacks.AddUnique(callback);
	}

	void InputManager::RegisterDeviceRemovedCallback(const Delegate<void(u32, const Device*)>& callback) noexcept
	{
		m_deviceRemovedCallbacks.AddUnique(callback);
	}

	void InputManager::RegisterDeviceReconnectedCallback(const Delegate<void(u32, const Device*)>& callback) noexcept
	{
		m_deviceReconnectedCallbacks.AddUnique(callback);
	}

	void InputManager::RegisterUserAddedCallback(const Delegate<void(u32, const User&)>& callback) noexcept
	{
		m_userAddedCallbacks.AddUnique(callback);
	}

	void InputManager::RegisterUserRemovedCallback(const Delegate<void(u32, const User&)>& callback) noexcept
	{
		m_userRemovedCallbacks.AddUnique(callback);
	}

	void InputManager::UnregisterDeviceInitializer(u16 vid, u16 pid) noexcept
	{
		u32 id = (u32(vid) << 16) | pid;
		m_devInitMapping.Erase(id);
	}

	void InputManager::UnregisterDeviceAddedCallback(const Delegate<void(u32, const Device*)>& callback) noexcept
	{
		m_deviceAddedCallbacks.Erase(callback);
	}

	void InputManager::UnregisterDeviceRemovedCallback(const Delegate<void(u32, const Device*)>& callback) noexcept
	{
		m_deviceRemovedCallbacks.Erase(callback);
	}

	void InputManager::UnregisterDeviceReconnectedCallback(const Delegate<void(u32, const Device*)>& callback) noexcept
	{
		m_deviceReconnectedCallbacks.Erase(callback);
	}

	void InputManager::UnregisterUserAddedCallback(const Delegate<void(u32, const User&)>& callback) noexcept
	{
		m_userAddedCallbacks.Erase(callback);
	}

	void InputManager::UnregisterUserRemovedCallback(const Delegate<void(u32, const User&)>& callback) noexcept
	{
		m_userRemovedCallbacks.Erase(callback);
	}
	
	auto InputManager::AddControlSet(ControlSet&& set) noexcept -> u32
	{
		for (u32 i = 0; i < m_controlSets.Size(); ++i)
		{
			if (m_controlSets[i].IsNullSet())
			{
				m_controlSets[i] = Move(set);
				return i;
			}
		}
		u32 id = u32(m_controlSets.Size());
		m_controlSets.Add(Move(set));
		return id;
	}

	void InputManager::RemoveControlSet(u32 id) noexcept
	{
		if (id >= m_controlSets.Size())
			return;
		m_controlSets[id].Invalidate();
	}

	auto InputManager::AddUser(User&& user) noexcept -> u32
	{
		if (!m_users.IsEmpty() && (!m_allowMultiUser || m_users.Size() >= m_maxUsers))
			return InvalidId;

		u32 id = 0;
		for (; id < m_users.Size(); ++id)
		{
			if (user.IsValid())
				break;
		}

		if (id >= m_maxUsers)
			return InvalidId;

		if (user.GetMappingContexts().IsEmpty() && m_defNewUserMappingContext && !m_defNewUserMappingContext->IsEmpty())
			user.AddMappingContext(m_defNewUserMappingContext);

		if (id == m_users.Size())
			m_users.Add(Move(user));
		else
			m_users[id] = Move(user);
		m_users[id].m_valid = true;

		OnUserAdded(id, m_users.Back());
		return id;
	}

	auto InputManager::TryComposeControlSchemeForUser(u32 userId, u32 scheme, u32 reconnectedDev) noexcept -> bool
	{
		if (userId >= m_users.Size() ||
			!m_users[userId].IsValid())
			return false;

		u32 setId = CreateControlSetForScheme(scheme, reconnectedDev);
		if (setId == InvalidId)
			return false;

		for (User& user : m_users)
			user.RemoveDisconnectedDeviceForScheme(scheme);

		m_users[userId].AddControlSet(setId);
		return true;
	}

	auto InputManager::CreateControlSetForScheme(u32 scheme, u32 reconnectedDev) noexcept -> u32
	{
		// Collect all unused devices
		DynArray<Device*> unusedDevices;

		if (reconnectedDev != InvalidId)
			unusedDevices.Add(m_devs[reconnectedDev].Get());

		for (u32 i = 0; i < m_devs.Size(); ++i)
		{
			if (!IsDeviceInUse(i) && i != reconnectedDev)
				unusedDevices.Add(m_devs[i].Get());
		}

		if (scheme != InvalidId)
		{
			if (scheme >= m_controlSchemes.Size() ||
				m_controlSchemes[scheme].Name().IsEmpty())
				return false;

			ControlScheme::MatchResult match = m_controlSchemes[scheme].Match(unusedDevices);
			if (!match.valid)
				return false;

			// We found a scheme that can be created
			DynArray<u32> devIds;
			for (const String& devName : match.usedDevices)
			{
				const DynArray<u32>& existingDevs = m_devMapping[devName];
				for (u32 devId : existingDevs)
				{
					if (!IsDeviceInUse(devId))
					{
						devIds.Add(devId);
						break;
					}
				}
				unusedDevices.Erase(m_devs[devIds.Back()].Get());
			}
			return AddControlSet(ControlSet{ scheme, Move(devIds) });
		}

		// Invalid id means iterate over all possibilities
		while (!unusedDevices.IsEmpty())
		{
			ControlScheme::MatchResult bestMatch;
			DynArray<String> conflictingDevices;
			u32 schemeId = 0;

			// Find a control scheme that can be created
			for (u32 i = 0; i < m_controlSchemes.Size(); ++i)
			{
				ControlScheme::MatchResult curMatch = m_controlSchemes[i].Match(unusedDevices);
				if (!curMatch.valid)
					continue;

				if (!bestMatch.valid)
				{
					bestMatch = curMatch;
				}
				else if (curMatch.score > bestMatch.score)
				{
					bestMatch = curMatch;
					schemeId = i;
					conflictingDevices.Clear();
				}
				else if (curMatch.score == bestMatch.score)
				{
					// Check for conflicting devices
					for (const String& name : bestMatch.usedDevices)
					{
						if (curMatch.usedDevices.Contains(name))
							conflictingDevices.Add(name);
					}
				}
			}

			if (!bestMatch.valid)
				return false;

			// If we have conflicting devices, remove them and try again
			if (!conflictingDevices.IsEmpty())
			{
				unusedDevices.EraseIf([&conflictingDevices](const Device* dev) -> bool
				{
					const DynArray<InternedString>& devNames = dev->GetDeviceNames();
					for (const String& name : conflictingDevices)
					{
						if (devNames.Contains(name))
							return true;
					}
					return false;
				});
				continue;
			}

			// We found a scheme that can be created
			DynArray<u32> devIds;
			for (const String& devName : bestMatch.usedDevices)
			{
				const DynArray<u32>& existingDevs = m_devMapping[devName];
				for (u32 devId : existingDevs)
				{
					if (!IsDeviceInUse(devId))
					{
						devIds.Add(devId);
						break;
					}
				}
				unusedDevices.Erase(m_devs[devIds.Back()].Get());
			}
			return AddControlSet(ControlSet{ schemeId, Move(devIds) });
		}

		return InvalidId; 
	}

	void InputManager::ReconnectDevice(User& user, u32 devId, u32 schemeId) noexcept
	{
		bool res = TryComposeControlSchemeForUser(user.GetId(), schemeId, devId);
		if (res)
			user.RemoveDisconnectedDevice(m_devs[devId]->GetDeviceInfo());
	}

	void InputManager::OnDeviceAdded(u32 id, const Device* pDev) noexcept
	{
		for (Delegate<void(unsigned, const Device*)>& callback : m_deviceAddedCallbacks)
		{
			callback(id, pDev);
		}
	}

	void InputManager::OnDeviceRemoved(u32 id, const Device* pDev) noexcept
	{
		for (Delegate<void(unsigned, const Device*)>& callback : m_deviceRemovedCallbacks)
		{
			callback(id, pDev);
		}
	}

	void InputManager::OnDeviceReconnected(u32 id, const Device* pDev) noexcept
	{
		for (Delegate<void(unsigned, const Device*)>& callback : m_deviceReconnectedCallbacks)
		{
			callback(id, pDev);
		}
	}

	void InputManager::OnUserAdded(u32 id, const User& user) noexcept
	{
		for (Delegate<void(unsigned, const User&)>& callback : m_userAddedCallbacks)
		{
			callback(id, user);
		}
	}

	void InputManager::OnUserRemoved(u32 id, const User& user) noexcept
	{
		for (Delegate<void(unsigned, const User&)>& callback : m_userRemovedCallbacks)
		{
			callback(id, user);
		}
	}
}
