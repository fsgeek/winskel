//
// Generic definition of a device object class object for Windows Skeleton
//
// (C) Copyright 2019 Tony Mason.  All Rights Reserved.
//
//
#pragma once

#include <ntifs.h>
#include <sal.h>
// #include "cpprt.h"

static NTSTATUS WinSkelInvalidDeviceRequest(PDEVICE_OBJECT DeviceObject, PIRP Irp);
static NTSTATUS WinSkelInvokeMajorFunction(PDEVICE_OBJECT DeviceObject, PIRP Irp);

class WinSkelDeviceObject;

typedef struct _WINSKEL_DEVICE_EXTENSION {
	PDEVICE_OBJECT DeviceObject;
	WinSkelDeviceObject* WinSkelDevice;
} WINSKEL_DEVICE_EXTENSION, *PWINSKEL_DEVICE_EXTENSION;


class WinSkelDeviceObject
{
public:

	static WinSkelDeviceObject* CreateDeviceObject(_In_ PDRIVER_OBJECT DriverObject, _In_opt_ _Maybenull_ const PUNICODE_STRING DeviceName) noexcept;
	static void DeleteDeviceObject(_In_ _Post_invalid_ WinSkelDeviceObject* DeviceObject) noexcept;
	void SetMajorFunction(UINT8 MajorFunction, NTSTATUS(*DispatchFunction)(PIRP)) noexcept;
	NTSTATUS CallMajorFunction(PIRP Irp) noexcept;

private:
#pragma warning(push)
#pragma warning(disable: 26432) // what other default operations does it want me to override?  Nothing I'm using (yet)
	WinSkelDeviceObject() noexcept {}
	~WinSkelDeviceObject() noexcept {}
#pragma warning(pop)

	PDEVICE_OBJECT m_DeviceObject = nullptr;
	NTSTATUS(*m_DispatchFunctions[IRP_MJ_MAXIMUM_FUNCTION])(PIRP Irp) = { nullptr };
	NTSTATUS InvalidDeviceRequest(PIRP Irp) noexcept {
		return CompleteRequest(Irp, STATUS_INVALID_DEVICE_REQUEST);
	}

	static NTSTATUS CompleteRequest(PIRP Irp, NTSTATUS Status, CCHAR PriorityBoost = IO_NO_INCREMENT) noexcept {
		Irp->IoStatus.Status = Status;
		IoCompleteRequest(Irp, PriorityBoost);
		return Status;
	}
	

};

