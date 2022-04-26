#pragma once
#include "Common.h"
#include "Device.h"
#include "core/memory/RefCounted.h"

namespace Core::Input
{
	class Device;
	class InputManager;

	struct DeviceRequirement
	{
		String name;         ///< Device name // TODO: use interned string
		bool   optional : 1; ///< If the device is optional
		bool   orNext   : 1; ///< If this OR the next device need to be available, otherwise this AND the next device are required
		u8     idx;          ///< Control scheme device index (set by control scheme)
	};

	/**
	 * Control scheme
	 * \note
	 * The scheme can contain required and optional devices, for a control set to be created,
	 * all devices that the scheme requires need to be available and not already part of another control set.
	 * If a device is required in some schemes and optional in others, the schemes that require the device will be prioritized (if they can be created)
	 * \note
	 * If only 1 user of the input system exists, any scheme can be used by the user, otherwise if multiple users exists, each user can only use a single
	 * \note
	 * A device cannot be required by multiple control schemes
	 */
	class CORE_API ControlScheme
	{
	public:
		struct MatchResult
		{
			MatchResult() noexcept = default;

			bool valid                      = 0; ///< Whether the match is valid
			u32 score                       = 0; ///< Score of the match

			DynArray<String> usedDevices;        ///< Names of devices used
			DynArray<String> missingOptDevs;     ///< Names of missing optimal devices
		};

		/**
		 * Create an invalid control scheme
		 */
		ControlScheme() noexcept;
		/**
		 * Create a new control scheme
		 * \param[in] name Name
		 * \param[in] reqs Device requirements
		 */
		ControlScheme(const String& name, DynArray<DeviceRequirement>&& reqs) noexcept;

		/**
		 * Get the name
		 * \return Name
		 */
		auto Name() const noexcept -> const String& { return m_name; }

		/**
		 * Check if the control scheme can be used, if so, return additional info about the match
		 * \param[in] availableDevices Available devices
		 * \return Control scheme match result
		 */
		auto Match(const DynArray<Device*>& availableDevices) const noexcept -> MatchResult;

		/**
		 * Check if the control scheme supports a device
		 * \param[in] name Device name
		 * \return Whether the control scheme supports the device
		 */
		auto SupportsDevice(const String& name) const noexcept -> bool;
		/**
		 * Check if a device is required for the control scheme
		 * \param[in] name Device name
		 * \return Whether the device is required for the control scheme
		 */
		auto IsRequired(const String& name) const noexcept -> bool;

		/**
		 * Check if the control scheme is valid
		 * \return whether the control scheme is valid
		 */
		auto IsValid() const noexcept -> bool { return m_name.IsEmpty(); }

		auto operator==(const ControlScheme& other) const noexcept -> bool { return m_name == other.m_name; }

	private:
		String                      m_name;          ///< Control scheme name // TODO: use interned string
		DynArray<DeviceRequirement> m_reqs;          ///< Device requirements
	};
		
	/**
	 * Represents a collection of devices that should act as a single input system
	 * A control set can be thought of as an instance of a control scheme
	 *
	 * \note
	 * If only 1 user exists, all control sets that can created will be assigned to that user.
	 * \note
	 * If multiple users exist, only 1 control set can be used by each user, even if devices are available, no new control set will be created
	 */
	class CORE_API ControlSet
	{
	public:
		/**
		 * Create a null control set
		 */
		ControlSet() noexcept;
		/**
		 * Create a new control set
		 * \param[in] schemeId Control scheme id
		 * \param[in] devIds Device ids
		 */
		ControlSet(u32 schemeId, DynArray<u32>&& devIds) noexcept;

		/**
		 * Check if the control set contains a device
		 * \param[in] devId Device id
		 * \return Whether the control set contains the device
		 */
		auto ContainsDevice(u32 devId) const noexcept -> bool;
		/**
		 * Get the scheme id
		 * \return Scheme id
		 */
		auto GetSchemeId() const noexcept -> u32 { return m_schemeId; }

		/**
		 * Clear the set of all devices
		 */
		void ClearDevices() noexcept;
		/**
		 * Invalidate the set to a null set
		 */
		void Invalidate() noexcept;

		/**
		 * Check if the set is invalid
		 * \return Whether the set is invalid
		 */
		auto IsNullSet() const noexcept -> bool { return m_devIds.IsEmpty(); }



	private:
		friend class InputManager;

		u32                  m_schemeId;             ///< ID of the control scheme the set represents
		DynArray<u32>        m_devIds;               ///< Device ids // TODO: index in DynArray relative to index in scheme

		// TODO: also track unique identifiers for reconnecting devices
	};
}