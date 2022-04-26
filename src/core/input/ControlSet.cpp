#include "ControlSet.h"

#include "IInput.h"
#include "InputManager.h"

namespace Onca::Input
{
	ControlScheme::ControlScheme() noexcept
	{
	}

	ControlScheme::ControlScheme(const String& name, DynArray<DeviceRequirement>&& reqs) noexcept
		: m_name(name)
		, m_reqs(Move(reqs))
	{
		u8 idx = 0;
		for (DeviceRequirement& req : m_reqs)
		{
			req.idx = idx;
			idx += !req.orNext;
		}
	}

	auto ControlScheme::Match(const DynArray<Device*>& availableDevices) const noexcept -> MatchResult
	{
		MatchResult res{};
		res.valid = true;

		for (const DeviceRequirement& req : m_reqs)
		{
			u32 reqScore = 0;
			String name;
			const bool foundDev = availableDevices.ContainsIf([&req, &reqScore, &name](const Device* dev) -> bool
			{
				const DynArray<String>& names = dev->GetDeviceNames();
				const auto nameIt = names.FindIf([&req](const String& name) -> bool
				{
					return name == req.name;
				});
				if (nameIt == names.End())
					return false;

				reqScore = u32(nameIt - names.Begin());
				name = *nameIt;
				return true;
			});

			if (foundDev)
			{
				res.score += reqScore;
				res.usedDevices.Add(name);
			}
			else
			{
				if (req.orNext)
					continue;

				if (req.optional)
				{
					res.missingOptDevs.Add(req.name);
				}
				else
				{
					res.valid = false;
					return res;
				}
			}
		}

		return res;
	}

	auto ControlScheme::SupportsDevice(const String& name) const noexcept -> bool
	{
		return m_reqs.ContainsIf([&name](const DeviceRequirement& req) noexcept -> bool
		{
			return req.name == name;
		});
	}

	auto ControlScheme::IsRequired(const String& name) const noexcept -> bool
	{
		auto it = m_reqs.FindIf([&name](const DeviceRequirement& req) noexcept -> bool
		{
			return req.name == name;
		});

		if (it != m_reqs.End())
			return !it->optional;
		return false;
	}
	
	ControlSet::ControlSet() noexcept
		: m_schemeId(InvalidId)
	{
	}

	ControlSet::ControlSet(u32 schemeId, DynArray<u32>&& devIds) noexcept
		: m_schemeId(schemeId)
		, m_devIds(Move(devIds))
	{
	}

	auto ControlSet::ContainsDevice(u32 devId) const noexcept -> bool
	{
		return m_devIds.Contains(devId);
	}

	void ControlSet::ClearDevices() noexcept
	{
		m_devIds.Clear();
	}

	void ControlSet::Invalidate() noexcept
	{
		m_devIds.Clear(true);
	}
}
