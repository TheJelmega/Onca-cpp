#include "../InputManager.h"

#include "core/logging/Logger.h"
#if PLATFORM_WINDOWS

#include "core/platform/Platform.h"

namespace Onca::Input
{
	auto FillDevInfoForDevice(DeviceInfo& info, HANDLE hDevice) noexcept -> bool
	{
		info.handle = hDevice;

		DynArray<wchar_t> devNameArr;
		UINT size = 0;

		UINT res = ::GetRawInputDeviceInfoW(hDevice, RIDI_DEVICENAME, nullptr, &size);
		if (res != NO_ERROR)
		{
			g_Logger.Error(LogCategories::INPUT, "Failed to get raw input device name size");
			return false;
		}
		devNameArr.Resize(size);
		res = ::GetRawInputDeviceInfoW(hDevice, RIDI_DEVICENAME, devNameArr.Data(), &size);
		if (res != size)
		{
			g_Logger.Error(LogCategories::INPUT, "Failed to get raw input device name");
			return false;
		}
		
		info.uid = String{ reinterpret_cast<const char16_t*>(devNameArr.Data()), size - 1 };

		RID_DEVICE_INFO devInfo = {};
		devInfo.cbSize = size = sizeof(RID_DEVICE_INFO);
		res = ::GetRawInputDeviceInfoW(hDevice, RIDI_DEVICEINFO, &devInfo, &size);
		if (res != size)
		{
			g_Logger.Error(LogCategories::INPUT, "Failed to get raw input device info for '{}'", info.uid);
			return false;
		}

		switch (devInfo.dwType)
		{
		case RIM_TYPEMOUSE: info.type = DeviceType::Mouse; break;
		case RIM_TYPEKEYBOARD: info.type = DeviceType::Keyboard; break;
		case RIM_TYPEHID:
		{
			info.type = DeviceType::Other;
			info.vendorId = u16(devInfo.hid.dwVendorId);
			info.productId = u16(devInfo.hid.dwProductId);
			info.usagePage = u16(devInfo.hid.usUsagePage);
			info.usage = u16(devInfo.hid.usUsage);
			break;
		}
		default:
			break;
		}

		HANDLE handle = CreateFileW(devNameArr.Data(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
		if (handle != INVALID_HANDLE_VALUE)
		{
			DynArray<wchar_t> nameArr{ usize(128), '\0', g_GlobalAlloc };

			String productName;
			if (HidD_GetManufacturerString(handle, nameArr.Data(), ULONG(nameArr.Size())))
				productName.Assign(reinterpret_cast<const char16_t*>(nameArr.Data()));
			if (HidD_GetProductString(handle, nameArr.Data(), ULONG(nameArr.Size())))
			{
				productName += ' ';
				productName.Add(String{ reinterpret_cast<const char16_t*>(nameArr.Data()) });
			}
			info.productName = productName;

			if (HidD_GetSerialNumberString(handle, nameArr.Data(), ULONG(nameArr.Size())))
				info.serialNumber = String{ reinterpret_cast<const char16_t*>(nameArr.Data()) };

			CloseHandle(handle);
		}
		return true;
	}

	void InputManager::SystemInit() noexcept
	{
		UINT numDevs = 0;
		DynArray<RAWINPUTDEVICELIST> devs;

		DWORD res = ::GetRawInputDeviceList(nullptr, &numDevs, sizeof(RAWINPUTDEVICELIST));
		if (res != NO_ERROR)
		{
			g_Logger.Error(LogCategories::INPUT, "Failed to get number of raw input devices");
			return;
		}

		devs.Resize(numDevs);
		res = ::GetRawInputDeviceList(devs.Data(), &numDevs, sizeof(RAWINPUTDEVICELIST));
		if (res != numDevs)
		{
			g_Logger.Error(LogCategories::INPUT, "Failed to get raw input devices");
			return;
		}

		for (RAWINPUTDEVICELIST& dev : devs)
		{
			DeviceInfo info;
			if (FillDevInfoForDevice(info, dev.hDevice))
				m_rawDevInfos.Add(Move(info));
		}
	}

	void InputManager::SystemTick() noexcept
	{
		// Check for device reconnections/disconnections
		UINT numDevs = 0;
		DWORD res = ::GetRawInputDeviceList(nullptr, &numDevs, sizeof(RAWINPUTDEVICELIST));
		if (res != NO_ERROR)
		{
			g_Logger.Error(LogCategories::INPUT, "Failed to get number of raw input devices");
			return;
		}

		// To avoid random fails
		++numDevs;
		DynArray<RAWINPUTDEVICELIST> devs;
		devs.Resize(numDevs);
		res = ::GetRawInputDeviceList(devs.Data(), &numDevs, sizeof(RAWINPUTDEVICELIST));
		if (res == UINT(-1))
		{
			u32 actErr = ::GetLastError();
			g_Logger.Error(LogCategories::INPUT, "Failed to get raw input devices");
			return;
		}

		if (m_rawDevInfos.Size() != numDevs)
			i32 br = 0;


		DynArray<DeviceInfo> infos;
		infos.Reserve(m_rawDevInfos.Size());
		
		for (RAWINPUTDEVICELIST& dev : devs)
		{
			// Should only happen because of additional dev we get to avoid random fails
			if (!dev.hDevice)
				continue;;

			bool found = false;
			for (usize i = 0; i < m_rawDevInfos.Size(); ++i)
			{
				if (m_rawDevInfos[i].handle == dev.hDevice)
				{
					infos.Add(m_rawDevInfos.Extract(i));
					found = true;
					break;
				}
			}
		
			if (found)
				continue;
		
			DeviceInfo info;
			if (FillDevInfoForDevice(info, dev.hDevice))
			{
				infos.Add(info);
				OnDeviceConnected(info);
			}
		
			g_Logger.Verbose(LogCategories::INPUT, "Device connected (vid: {:X} | pid: {:X} | uid: '{}' | name: '{}')", 
							 info.vendorId, info.productId, info.uid, info.productName);
		}
		
		for (DeviceInfo& info : m_rawDevInfos)
		{
			g_Logger.Verbose(LogCategories::INPUT, "Device disconnected (vid: {:X} | pid: {:X} | uid: '{}' | name: '{}')",
							 info.vendorId, info.productId, info.uid, info.productName);

			OnDeviceDisconnected(info);
		}
		
		m_rawDevInfos = Move(infos);
	}
}

#endif
