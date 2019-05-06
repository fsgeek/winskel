//
// This is the generic control interface for WinSkel
//
// (C) Copyright 2019 Tony Mason, All Rights Reserved
//
//

#include"wsdevobj.h"

static PWCH WinSkelControlDeviceName = L"\\FileSystem\\WinSkelControl";

class WinSkelControlDevice
{
private:
	static WinSkelControlDevice *s_ControlDevice;
	WinSkelDeviceObject* m_DeviceObject = nullptr;
	UNICODE_STRING m_DeviceName = { 0, 0, nullptr };
	const PWCH m_DeviceNameString = L"\\FileSystem\\WinSkelControl";
#pragma warning(push)
#pragma warning(disable:26432) // what other default functions need to be defined?
	WinSkelControlDevice() noexcept {}
	~WinSkelControlDevice() noexcept {}
#pragma warning(pop)

public:
#pragma warning(suppress:26461) // it complains the RegistryPath should be const, but no const change makes it happy
	static WinSkelControlDevice* CreateControlDevice(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
		UNREFERENCED_PARAMETER(RegistryPath);

		if (nullptr == s_ControlDevice) {
#pragma warning(suppress:26409) // we're in kernel, we call new/delete
			s_ControlDevice = new WinSkelControlDevice;
			while (nullptr != s_ControlDevice) {
				RtlInitUnicodeString(&s_ControlDevice->m_DeviceName, WinSkelControlDeviceName);
				s_ControlDevice->m_DeviceObject = WinSkelDeviceObject::CreateDeviceObject(DriverObject, &s_ControlDevice->m_DeviceName);
				if (nullptr == s_ControlDevice->m_DeviceObject) {
					DeleteControlDevice();
				}
			}
		}
		return s_ControlDevice;
	}

	static void DeleteControlDevice(void) noexcept {
		if (nullptr == s_ControlDevice) {
			return;
		}

		if (nullptr != s_ControlDevice->m_DeviceObject) {
			WinSkelDeviceObject::DeleteDeviceObject(s_ControlDevice->m_DeviceObject);
			s_ControlDevice->m_DeviceObject = nullptr;
		}
		delete s_ControlDevice;
		s_ControlDevice = nullptr;
	}

	void SetMajorFunction(UINT8 MajorFunction, NTSTATUS(*DispatchFunction)(PIRP)) noexcept {
		if (nullptr != m_DeviceObject) {
			m_DeviceObject->SetMajorFunction(MajorFunction, DispatchFunction);
		}
	}


};

#pragma warning(suppress:26461 28101)
NTSTATUS WinSkelControlInit(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	UNICODE_STRING deviceName = { 0, 0, nullptr };
	WinSkelDeviceObject* devobj = nullptr;
	NTSTATUS status = STATUS_NO_MEMORY;

	UNREFERENCED_PARAMETER(RegistryPath);

	RtlInitUnicodeString(&deviceName, WinSkelControlDeviceName);
	devobj = WinSkelDeviceObject::CreateDeviceObject(DriverObject, &deviceName);

	while (nullptr != devobj) {
		// Initialize the Dispatch entry points

		// Done.
		status = STATUS_SUCCESS;
		break;
	}

	return status;
}
