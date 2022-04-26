#pragma once
#include "Common.h"
#include "Key.h"

namespace Onca::Input
{
	class InputManager;
	
	enum class DeviceType
	{
		Unknown , ///< Unknown/invalid device
		Keyboard, ///< Keyboard (only single device allowed)
		Mouse   , ///< Mouse (only single device allowed)
		Other   , ///< Human Interface Device (any other input device, like a gamepad)
	};

	DEFINE_OPAQUE_HANDLE(NativeDevice);

	struct DeviceInfo
	{
		NativeDevice handle       = nullptr;            ///< Native device handle
		DeviceType   type        = DeviceType::Unknown; ///< Device type
		u16          vendorId    = 0;                   ///< Vendor id (only valid for 'Other' type)
		u16          productId   = 0;                   ///< Product id (only valid for 'Other' type)
		u16          usagePage   = 0;                   ///< HID usage page (only valid for 'Other' type)
		u16          usage       = 0;                   ///< HID usage (only valid for 'Other' type)
		String       uid;                               ///< Unique identifier (usually system dependent) // TODO: Use interned strings
		String       productName;                       ///< HID product name  (only valid for 'Other' type) // TODO: Use interned strings
		String       serialNumber;                      ///< HID serial number
	};

	/**
	 * Input device
	 * \note Devices are differentiated by their identifier, 2 devices that correspond to the same identifier cannot be used for the same control
	 */
	class CORE_API Device
	{
	public:
		/**
		 * Create a new device
		 * \param[in] name Device name
		 */
		Device(const String& name);
		

		virtual ~Device();

		/**
		 * Tick the input device before the input is polled from the system
		 */
		virtual void PreTick() noexcept = 0;
		/**
		 * Tick the input device
		 */
		virtual void Tick() noexcept = 0;

		/**
		 * Get the value of an input at a certain ID
		 * \param[in] id Id of the input
		 * \return Input value
		 */
		virtual auto GetInputValue(u32 id) const noexcept -> Value = 0;

		/**
		 * Get the input device identifier
		 * \return Input device identifier
		 */
		auto GetDeviceNames() const noexcept -> const DynArray<String>& { return m_devNames; };
		/**
		 * Get the input device id
		 * \return Input device id
		 */
		auto GetDeviceId() const noexcept -> u32 { return m_deviceId; }
		/**
		 * Get the input device info
		 * \return Input device info
		 */
		auto GetDeviceInfo() const noexcept -> const DeviceInfo& { return m_devInfo; }
		/**
		 * Add a key to the device
		 * \param[in] idx Native index
		 * \param[in] key Key
		 * \note Should only be called from the input manager
		 */
		void AddKey(u32 idx, const Key& key) noexcept;

		/**
		 * Callback for when a device is registered to the input manager
		 * \param[in] pManager Pointer to the input manager
		 * \note m_deviceId is set by the input manager before invoking this callback
		 */
		virtual void OnRegister(InputManager* pManager) noexcept;
		/**
		 * Callback for when a device is unregistered/removed from the input manager
		 */
		virtual void OnUnregister() noexcept;

	protected:
		friend class InputManager;

		DeviceInfo       m_devInfo;    ///< Device info
		u32              m_deviceId;   ///< Device id
		InputManager*    m_pManager;   ///< Input manager
		DynArray<String> m_devNames;   ///< Device names (e.g. ["category", "generic name", "name"] or ["keyboard"]) // TODO: Use interned strings
		DynArray<Key>    m_nativeKeys; ///< Native to key mapping
	};
}
