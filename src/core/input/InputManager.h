#pragma once
#include "Common.h"
#include "ControlSet.h"
#include "core/containers/DynArray.h"
#include "core/containers/HashMap.h"
#include "core/string/Include.h"
#include "core/memory/RefCounted.h"
#include "core/memory/Unique.h"
#include "Keyboard.h"
#include "Gamepad.h"
#include "Key.h"
#include "Rebind.h"
#include "Mouse.h"
#include "User.h"

namespace Onca::Input
{
	class MappingContext;
	class InternalMapping;

	// TODO: Input device change notifications
	class CORE_API InputManager
	{
	public:

		using DevInitDelegate = Delegate<Unique<Device>(const DeviceInfo& info)>;

		DISABLE_COPY(InputManager);

		/**
		 * Create an input manager
		 * \param[in] createKbM Whether to create a keyboard and mouse device by default
		 */
		InputManager(bool createKbM = true);

		/**
		 * System specific initialization
		 */
		void SystemInit() noexcept;
		/**
		 * System specific tick
		 */
		void SystemTick() noexcept;

		/**
		 * Shut down the manager
		 */
		void Shutdown() noexcept;

		/**
		 * Tick the input manager at the end of the frame (e.g. reset pressed and released keys)
		 */
		void PreTick() noexcept;
		/**
		 * Tick the input manager
		 */
		void Tick(Chrono::DeltaTime dt) noexcept;

		/**
		 * Set whether to allow multiple input users
		 * \param[in] allow Whether to allow multiple input users
		 * \note When switching from single to multi user, if the single user uses more than 1 control set,
		 *       unassign all and give the control to the first device that is used
		 */
		void AllowMultiUser(bool allow) noexcept;
		/**
		 * Check if multi user input is allowed
		 * \return Whether multi user input is allowed
		 */
		auto IsMultiUserAllowed() const noexcept -> bool { return m_allowMultiUser; }

		/**
		 * Set whether to allow new users to be crated when a new control set can be created
		 * \param[in] allow Whether to allow new users to be crated when a new control set can be created
		 */
		void AllowDynamicUserCreation(bool allow) noexcept { m_addUsersDynamic = allow; }
		/**
		 * Check if it is allowed to dynamically add users when a new control set can be created
		 * \return Whether it is allowed to dynamically add users when a new control set can be created
		 */
		auto IsDynamicUserCreationAllowed() const noexcept -> bool { return m_addUsersDynamic; }

		/**
		 * Set the maximum number of allowed users
		 * \param[in] maxUsers Maximum number of users
		 */
		void SetMaxUsers(u8 maxUsers) noexcept;
		/**
		 * Get the maximum number of allowed users
		 * \return Maximum number of allowed users
		 */
		auto GetMaxUsers() const noexcept -> u8 { return m_maxUsers; }

		/**
		 * Set the default input mapping for new users
		 * \param[in] mapping Default input mapping for new users
		 */
		void SetDefaultMappingForNewUser(const Rc<MappingContext>& mapping) noexcept { m_defNewUserMappingContext = mapping; }
		/**
		 * Get the default input mapping for new users
		 * \return Default input mapping for new users
		 */
		auto GetDefaultMappingForNewUser() const noexcept -> const Rc<MappingContext>& { return m_defNewUserMappingContext; }

		/**
		 * Get the raw input value for a key
		 * \param[in] user User
		 * \param[in] key Key
		 * \return Value
		 */
		auto GetRawValue(const User& user, const Key& key) noexcept -> Value;
		
		/**
		 * Register an input device
		 * \param[in] device Device to register
		 * \return Whether the input device was registered successfully
		 */
		auto RegisterDevice(Unique<Device>&& device) noexcept -> bool;
		/**
		 * Unregister a device
		 * \param[in] device Device
		 */
		void UnregisterDevice(Unique<Device>& device) noexcept;

		/**
		 * Check if any device with a giver name exists
		 * \param[in] name Name of device
		 * \return Whether any device with the given name exists
		 */
		auto HasDeviceWithName(const String& name) const noexcept -> bool;
		/**
		 * Check if any device of a given type exists
		 * \param[in] type Device type
		 * \return Whether any device of a given type exists
		 */
		auto HasDeviceOfType(DeviceType type) const noexcept -> bool;
		/**
		 * Get a device from it's id
		 * \param[in] devId Device id
		 * \return Device
		 */
		auto GetDevice(u32 devId) const noexcept -> const Device*;
		/**
		 * Check if a device is in use
		 * \param[in] devId Device id
		 * \return Whether the device is in use
		 */
		auto IsDeviceInUse(u32 devId) const noexcept -> bool;
		/**
		 * Get the control set id that is currently using a device
		 * \param[in] devId Device id
		 * \return Control set id
		 */
		auto GetDeviceControlSet(u32 devId) const noexcept -> u32;


		/**
		 * Add a control scheme to the input manager
		 * \param[in] scheme Control scheme
		 * \return True if the scheme was added, false if it already exists
		 */
		auto AddControlScheme(ControlScheme&& scheme) noexcept -> u32;
		/**
		 * Remove a control scheme
		 * \param[in] name Control scheme name
		 */
		void RemoveControlScheme(const String& name) noexcept;

		/**
		 * Try to create new control sets with available devices and assign them to a user
		 */
		void ComposePossibleControlSets() noexcept;
		/**
		 * Get a control set from its id
		 * \param[in] id Control set id
		 * \return Control set
		 */
		auto GetControlSet(u32 id) const noexcept -> const ControlSet&;

		/**
		 * Create a new user
		 * \param[in] scheme Preferred control scheme id
		 * \param[in] allowDynamicSchemeChange Whether the user is allowed to dynamically change control schemes (ignored if more than 1 user)
		 * \return 
		 */
		auto CreateNewUser(u32 scheme = InvalidId, bool allowDynamicSchemeChange = false) noexcept -> bool;
		/**
		 * Create a new user
		 * \param[in] scheme Preferred control scheme id
		 * \param[in] mapping Initial input mapping
		 * \param[in] allowDynamicSchemeChange Whether the user is allowed to dynamically change control schemes (ignored if more than 1 user)
		 * \return
		 */
		auto CreateNewUser(const Rc<MappingContext>& mapping, u32 scheme = InvalidId, bool allowDynamicSchemeChange = false) noexcept -> bool;
		/**
		 * Remove a user from the input manager
		 * \param[in] id User id
		 */
		void RemoveUser(u32 id) noexcept;
		/**
		 * Get a user from its id
		 * \param[in] id User id
		 * \return User
		 */
		auto GetUser(u32 id) noexcept -> User&;
		
		/**
		 * Clear all input mappings for all users
		 */
		void ClearAllMappingContexts() noexcept;

		/**
		 * Begin mapping of the given context
		 * \param[in] context Mapping context
		 */
		void Rebind(const Rc<MappingContext>& mapping, RebindContext&& context);
		/**
		 * Notify the manager that a key is available for mapping
		 * \param[in] key Key
		 */
		void NotifyKeyForMapping(const Key& key, const Value& value);
		/**
		 * Check if the manager is currently mapping a binding
		 * \return Whether the manager is currently mapping a binding
		 */
		auto IsMappingActive() const noexcept -> bool;
		
		/**
		 * Register a new key to the input manager
		 * \param[in] pDev Device
		 * \param[in] name Key name (excluding device name)
		 * \param[in] details Key details to register
		 * \param[in] nativeCode Code representing the device's native key code
		 * \return Key
		 * \note If the key was already registered, the registration will not do anything
		 */
		void RegisterKey(Device* pDev, const String& name, KeyDetails details, u32 nativeCode) noexcept;
		/**
		 * Unregister a key from the input manager
		 * \param[in] pDev Device
		 * \param[in] key Key to unregister
		 */
		void UnregisterKey(Device* pDev, const Key& key) noexcept;

		/**
		 * Connect a new device to the input manager
		 * \param[in] info Device info
		 */
		void OnDeviceConnected(const DeviceInfo& info) noexcept;
		/**
		 * Disconnect a device from the input manager
		 * \param[in] info Device info
		 */
		void OnDeviceDisconnected(const DeviceInfo& info) noexcept;

		/**
		 * Get direct access to the keyboard
		 * \return Keyboard
		 * \note A call to this is only valid if keyboard and mouse support is available
		 * \note Direct access to the keyboard should only be used for tools/debugging, otherwise input actions should be used
		 */
		auto GetKeyboard() noexcept -> Keyboard*;
		/**
		 * Get direct access to the mouse
		 * \return Mouse
		 * \note Direct access to the mouse should only be used for tools/debugging, otherwise input actions should be used
		 */
		auto GetMouse() noexcept -> Mouse*;

		/**
		 * Add an initializer for a device based on its HID vendor and product id
		 * \param[in] vid HID vendor id
		 * \param[in] pid HID product id
		 * \param[in] initializer Initializer(const DeviceInfo&) -> Unique<Device>
		 */
		void RegisterDeviceInitializer(u16 vid, u16 pid, const DevInitDelegate& initializer) noexcept;

		/**
		 * Add a device added callback (device reconnected will be called instead if the device is s)
		 * \param[in] callback Callback(u32, const Input::IDevice*)
		 */
		void RegisterDeviceAddedCallback(const Delegate<void(u32, const Device*)>& callback) noexcept;
		/**
		 * Add a device removed callback
		 * \param[in] callback Callback(u32, const Input::IDevice*)
		 */
		void RegisterDeviceRemovedCallback(const Delegate<void(u32, const Device*)>& callback) noexcept;
		/**
		 * Add a device reconnected callback
		 * \param[in] callback Callback(u32, const Input::IDevice*)
		 */
		void RegisterDeviceReconnectedCallback(const Delegate<void(u32, const Device*)>& callback) noexcept;

		/**
		 * Add a user added callback
		 * \param[in] callback Callback(u32, const Input::User&)
		 */
		void RegisterUserAddedCallback(const Delegate<void(u32, const User&)>& callback) noexcept;
		/**
		 * Add a user removed callback
		 * \param[in] callback Callback(u32, const Input::User&)
		 */
		void RegisterUserRemovedCallback(const Delegate<void(u32, const User&)>& callback) noexcept;

		/**
		 * Remove a device initializer
		 * \param[in] vid HID vendor id
		 * \param[in] pid HID product id
		 */
		void UnregisterDeviceInitializer(u16 vid, u16 pid) noexcept;
		/**
		 * Remove a device added callback (device reconnected will be called instead if the device is s)
		 * \param[in] callback Callback(u32, const Input::IDevice*)
		 */
		void UnregisterDeviceAddedCallback(const Delegate<void(u32, const Device*)>& callback) noexcept;
		/**
		 * Remove a device removed callback
		 * \param[in] callback Callback(u32, const Input::IDevice*)
		 */
		void UnregisterDeviceRemovedCallback(const Delegate<void(u32, const Device*)>& callback) noexcept;
		/**
		 * Remove a device reconnected callback
		 * \param[in] callback Callback(u32, const Input::IDevice*)
		 */
		void UnregisterDeviceReconnectedCallback(const Delegate<void(u32, const Device*)>& callback) noexcept;
		/**
		 * Remove a user added callback
		 * \param[in] callback Callback(u32, const Input::User&)
		 */
		void UnregisterUserAddedCallback(const Delegate<void(u32, const User&)>& callback) noexcept;
		/**
		 * Remove a user removed callback
		 * \param[in] callback Callback(u32, const Input::User&)
		 */
		void UnregisterUserRemovedCallback(const Delegate<void(u32, const User&)>& callback) noexcept;

	private:
		/**
		 * Add a new control set to the input manager
		 * \param[in] set Control set
		 * \return Control set id
		 */
		auto AddControlSet(ControlSet&& set) noexcept -> u32;
		/**
		 * Remove a control set
		 * \param[in] id Control set id
		 */
		void RemoveControlSet(u32 id) noexcept;

		/**
		 * Add a user to the input manager
		 * \param[in] user User
		 * \return Whether the user was successfully added
		 */
		auto AddUser(User&& user) noexcept -> u32;
		/**
		 * Try to compose a control scheme for a user
		 * \param[in] userId User id
		 * \param[in] scheme Scheme id
		 * \param[in] reconnectedDev Reconnected device id
		 * \return Whether the scheme for the user could be created
		 */
		auto TryComposeControlSchemeForUser(u32 userId, u32 scheme, u32 reconnectedDev = InvalidId) noexcept -> bool;

		/**
		 * Create a control set for a specific scheme or for any possible scheme
		 * \param[in] scheme Scheme, if invalid, find the first best match
		 * \param[in] reconnectedDev Reconnected device id
		 * \return Control set id
		 */
		auto CreateControlSetForScheme(u32 scheme, u32 reconnectedDev) noexcept -> u32;

		/**
		 * Reconnect a device
		 * \param[in] user User
		 * \param[in] devId Device id
		 * \param[in] schemeId Control scheme id
		 */
		void ReconnectDevice(User& user, u32 devId, u32 schemeId) noexcept;

		/**
		 * Call all device added callbacks
		 * \param[in] id Device id
		 * \param[in] pDev Device
		 */
		void OnDeviceAdded(u32 id, const Device* pDev) noexcept;
		/**
		 * Call all device added callbacks
		 * \param[in] id Device id
		 * \param[in] pDev Device
		 */
		void OnDeviceRemoved(u32 id, const Device* pDev) noexcept;
		/**
		 * Call all device added callbacks
		 * \param[in] id Device id
		 * \param[in] pDev Device
		 */
		void OnDeviceReconnected(u32 id, const Device* pDev) noexcept;

		/**
		 * Call all user added callbacks
		 * \param[in] id User id
		 * \param[in] user User
		 */
		void OnUserAdded(u32 id, const User& user) noexcept;
		/**
		 * Call all user removed callbacks
		 * \param[in] id User id
		 * \param[in] user User
		 */
		void OnUserRemoved(u32 id, const User& user) noexcept;


		using DeviceMapping = HashMap<InternedString, DynArray<u32>>;
		using DevInitMapping = HashMap<u32, DevInitDelegate>;

		using DeviceRegistry = DynArray<Unique<Device>>;

		using KeyRegistry = HashMap<Key, Rc<KeyDetails>>;
		using KeyDevNativeRegistry = HashMap<Key, Pair<InternedString, u32>>;
		
		DeviceMapping           m_devMapping;               ///< Name to id mapping
		DeviceRegistry          m_devs;                     ///< Input device
		DevInitMapping          m_devInitMapping;           ///< Device initializer mapping

		Keyboard*               m_pKeyboard;                ///< Cached pointer to current keyboard
		Mouse*                  m_pMouse;                   ///< Cached pointer to current mouse

		KeyRegistry             m_keys;                     ///< Key details
		KeyDevNativeRegistry    m_keyDevNative;             ///< Key to device and key code mapping

		DynArray<ControlScheme> m_controlSchemes;           ///< Control schemes
		DynArray<ControlSet>    m_controlSets;              ///< Control sets

		DynArray<User>          m_users;                    ///< Users

		DynArray<DeviceInfo>    m_rawDevInfos;              ///< Device info for all devices currently connected to the system

		bool                    m_allowMultiUser;           ///< Whether we can have multiple users
		bool                    m_addUsersDynamic;          ///< Can we dynamically add new users if a new control set can be created
		u8                      m_maxUsers;                 ///< Maximum number of input users
		Rc<MappingContext>      m_defNewUserMappingContext; ///< Default input mapping for dynamically created users

		RebindContext           m_rebindContext;            ///< Context for key that is currently being bound
		RebindState             m_rebindState;              ///< Current rebind state

		/// Callbacks

		DynArray<Delegate<void(u32, const Device*)>> m_deviceAddedCallbacks;       ///< Device added callbacks
		DynArray<Delegate<void(u32, const Device*)>> m_deviceRemovedCallbacks;     ///< Device removed callbacks
		DynArray<Delegate<void(u32, const Device*)>> m_deviceReconnectedCallbacks; ///< Device reconnected callbacks

		DynArray<Delegate<void(u32, const User&)>> m_userAddedCallbacks;   ///< User added callbacks
		DynArray<Delegate<void(u32, const User&)>> m_userRemovedCallbacks; ///< User removed callbacks
	};

}

