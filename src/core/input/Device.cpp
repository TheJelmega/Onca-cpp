#include "core/input/Device.h"

#include "InputManager.h"

namespace Onca::Input
{
	Device::Device(const String& name)
		: m_deviceId(InvalidId)
		, m_pManager(nullptr)
	{
		DynArray<String> names = name.Split('/');
		for (const String& name : names)
			m_devNames.EmplaceBack(name.AsLower());
	}

	Device::~Device()
	{
	}

	void Device::AddKey(u32 idx, const Key& key) noexcept
	{
		if (idx == m_nativeKeys.Size())
		{
			m_nativeKeys.Add(key);
			return;
		}

		m_nativeKeys.Resize(Math::Max(m_nativeKeys.Size(), idx + 1));
		m_nativeKeys[idx] = key;
	}

	void Device::OnRegister(InputManager* pManager) noexcept
	{
		m_pManager = pManager;
	}

	void Device::OnUnregister() noexcept
	{
		if (!m_pManager)
			return;

		for (const Key& key : m_nativeKeys)
		{
			m_pManager->UnregisterKey(this, key);
		}
	}
}
