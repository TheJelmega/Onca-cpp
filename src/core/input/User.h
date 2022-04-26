#pragma once
#include "Common.h"
#include "Device.h"
#include "Mapping.h"
#include "Key.h"
#include "core/containers/HashMap.h"
#include "core/containers/HashSet.h"

namespace Onca::Input
{
	class RebindContext;
	class ITrigger;
	class InputAction;
	class InternalMapping;
	class Binding;

	/**
	 * Input user
	 * \note
	 * If only 1 user exists, all control schemes and sets will be associated with the single user
	 * \note
	 * If multiple user exists, each user is restricted to a single control scheme and set, these can not automatically switch between schemes/sets and need to be programatically changed
	 */
	class CORE_API User
	{
	public:
		User() noexcept;

		/**
		 * Create an input user
		 * \param[in] scheme Initial control scheme id
		 * \param[in] allowDynamicSchemeChange Allow the dynamic changing between control schemes (when only this user exists)
		 */
		User(u32 scheme, bool allowDynamicSchemeChange) noexcept;

		/**
		 * Get the user id
		 * \return User id
		 */
		auto GetId() const noexcept -> u32 { return m_id; }
		/**
		 * Get the id of the current control scheme
		 * \return Current control scheme id
		 */
		auto GetSchemeId() const noexcept -> u32 { return m_schemeId; }
		/**
		 * Check if the user allows for automatic control scheme switching
		 * \return Whether the user allows automatic control scheme switching
		 */
		auto CanChangeScheme() const noexcept -> bool { return m_allowSchemeChange; }

		/**
		 * Get the ids of the control sets associated with this user
		 * \return Control set ids
		 */
		auto GetControlSetIds() const noexcept -> const DynArray<u32>& { return m_controlSetIds; }

		/**
		 * Add an input mapping with a priority
		 * \param[in] mapping Input mapping
		 * \param[in] priority Priority
		 */
		void AddMappingContext(const Rc<MappingContext>& mapping, i32 priority = 0) noexcept;
		/**
		 * Remove an input mapping
		 * \param[in] mapping Input mapping
		 */
		void RemoveMappingContext(const Rc<MappingContext>& context) noexcept;
		/**
		 * Clear all input mappings for this user
		 */
		void ClearMappingContexts() noexcept;
		/**
		 * Get all input mappings assigned to this user
		 * \return Input mappings
		 */
		auto GetMappingContexts() noexcept -> DynArray<Pair<i32, Rc<MappingContext>>>& { return m_mappingContexts; }

		/**
		 * Set whether mapping needs to be rebuild
		 * \param[in] needed Whether mapping needs to be rebuild
		 */
		void SetNeedToRebuildMappings(bool needed) noexcept;
		/**
		 * Rebuild the current mappings based on the mapping contexts
		 */
		void RebuildMappings();
		/**
		 * Get the current mappings
		 * \return Mappings
		 */
		auto GetMappings() noexcept -> DynArray<InternalMapping>& { return m_mappings; }

		/**
		 * Update the current trigger info
		 * \param[in] action Action
		 * \param[in] info Info
		 */
		void UpdateTriggerInfo(const Weak<InputAction>& action, const TriggerInfo& info) noexcept;
		/**
		 * Get the current trigger info
		 * \return Trigger info
		 */
		auto GetTriggerInfos() noexcept -> const HashMap<Weak<InputAction>, TriggerInfo>& { return m_triggerInfo; }

		/**
		 * Get the previous trigger state for an action
		 * \param[in] action Action
		 * \return Previous trigger state
		 */
		auto GetPrevTriggerState(const Weak<InputAction>& action) noexcept -> TriggerState;

		/**
		 * Reset input actions trigger info
		 */
		void ResetTriggerInfoAndUpdatePrevState() noexcept;

		/**
		 * Check if an input action was triggered this frame
		 * \param[in] action Action
		 * \return Whether the action was triggered this frame
		 */
		void SetInputActionTriggered(Rc<InputAction>& action);
		/**
		 * Check if an action is currently triggered
		 * \param[in] action Action
		 * \return Whether the action is currently triggered
		 */
		auto IsInputActionTriggered(const Rc<InputAction>& action) const noexcept -> bool;
		/**
		 * Check if an action is currently triggered
		 * \param[in] action Action
		 * \return Whether the action is currently triggered
		 */
		auto IsInputActionTriggered(const Weak<InputAction>& action) const noexcept -> bool;
		/**
		 * Reset input actions triggered for this frame
		 */
		void ResetInputActionsTriggered() noexcept;

		/**
		 * Add a disconnected device
		 * \param[in] info Device info
		 * \param[in] schemeId Scheme id
		 */
		void AddDisconnectedDevice(const DeviceInfo& info, u32 schemeId) noexcept;
		/**
		 * Remove a disconnected device
		 * \param[in] info Device info
		 */
		void RemoveDisconnectedDevice(const DeviceInfo& info) noexcept;
		/**
		 * Remove all disconnected devices for a given scheme id
		 * \param[in] schemeId Scheme id
		 */
		void RemoveDisconnectedDeviceForScheme(u32 schemeId) noexcept;
		/**
		 * Get the scheme a disconnected device was part off
		 * \param[in] info Device info
		 * \return Scheme id
		 */
		auto GetDisconnectedDeviceScheme(const DeviceInfo& info) const noexcept -> u32;

		/**
		 * Get an input action for a modifier key
		 * \param[in] key Modifier key
		 * \return Modifier action
		 */
		auto RequestModifierChordAction(const Key& key) noexcept -> Rc<InputAction>;

		/**
		 * Check if the user is valid
		 * \return Whether the user is valid
		 */
		auto IsValid() const noexcept -> bool;

	private:
		friend class InputManager;

		/**
		 * Add a control set to this user
		 * \param[in] id Control set id
		 */
		void AddControlSet(u32 id) noexcept;
		/**
		 * Remove a control set to this user
		 * \param[in] id Control set id
		 */
		void RemoveControlSet(u32 id) noexcept;

		/**
		 * Unassign all control sets
		 */
		void UnassignAllControlSets() noexcept;

		/**
		 * Inject blockers into the current mappings
		 */
		void InjectChordBlockers() noexcept;

		u32                                      m_id;                  ///< User id
		u32                                      m_schemeId;            ///< Id of the current control scheme
		bool                                     m_allowSchemeChange;   ///< Whether the control scheme can automatically switch, based on the last input (ignored when multiple users exist)
		bool                                     m_valid;               ///< Whether the user is valid (handled by the input manager)
		bool                                     m_needMappingRebuild;  ///< Whether mapping needs to be rebuild

		HashMap<Key, Rc<InputAction>>            m_modifierActions;     ///< Modifier actions
		DynArray<Pair<i32, Rc<MappingContext>>>  m_mappingContexts;     ///< Input mapping contexts
		DynArray<InternalMapping>                m_mappings;            ///< Input mappings, ordered based on contexts

		DynArray<u32>                            m_controlSetIds;       ///< Associated control set ids (only 1 if there are multiple users)
		
		HashMap<Weak<InputAction>, TriggerInfo>  m_triggerInfo;         ///< Current trigger info
		HashMap<Weak<InputAction>, TriggerState> m_prevTriggerState;    ///< Previous trigger states

		HashSet<Weak<InputAction>>               m_triggeredActions;    ///< Input actions that were trigger for the user this frame

		DynArray<Pair<DeviceInfo, u32>>          m_disconnectedDevices; ///< Disconnected devices from the user, together with the id the scheme it was part off
	};
}
