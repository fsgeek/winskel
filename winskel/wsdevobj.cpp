//
// WinSkelDeviceObject Implementation
//
// (C) Copyright 2019 Tony Mason, All Rights Reserved
//
#include "wsdevobj.h"


// forward declaration
_Function_class_(DRIVER_DISPATCH)
static NTSTATUS WinSkelDispatchRequest(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);


//
// This static function is used to invoke the C++ object
//
_Use_decl_annotations_
static NTSTATUS WinSkelDispatchRequest(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	PWINSKEL_DEVICE_EXTENSION devExt = nullptr;
	WinSkelDeviceObject* devobj = nullptr;
	NTSTATUS status = STATUS_INVALID_PARAMETER;

	while (nullptr != DeviceObject) {
		devExt = static_cast<PWINSKEL_DEVICE_EXTENSION>(DeviceObject->DeviceExtension);
		// devExt = (PWINSKEL_DEVICE_EXTENSION)DeviceObject->DeviceExtension;

		if (nullptr == devExt) {
			break; // no device extension = invalid parameter
		}

#pragma warning(suppress:26489) // devExt did not go invalid
		devobj = devExt->WinSkelDevice;

		if (nullptr == devobj) {
			break; // no winskel device object = invalid parameter
		}

		__try {
			status = devobj->CallMajorFunction(Irp);
		}
#pragma warning(suppress:6320) // yes, it's a generic exception handler
		__except (EXCEPTION_EXECUTE_HANDLER) {
			status = GetExceptionCode();
		}
	}

	// if the request was queued, we don't touch it any longer - just return pending
	// caller that wants a priority boost can complete it explicitly and then return
	// STATUS_PENDING as well.
	if (STATUS_PENDING != status) {
		// otherwise, the request needs to be completed
		if (nullptr != Irp) {
			Irp->IoStatus.Status = status;
			IoCompleteRequest(Irp, IO_NO_INCREMENT);
		}
	}

	return status;
}


	//
	// CreateDeviceObject: this version takes the DriverObject and a name
#pragma warning(suppress:26487) // if our pointer goes invalid, we have bigger problems...
WinSkelDeviceObject* WinSkelDeviceObject::CreateDeviceObject(_In_ PDRIVER_OBJECT DriverObject, _In_opt_ _Maybenull_ const PUNICODE_STRING DeviceName) noexcept {
	PDEVICE_OBJECT deviceObject = nullptr;
	WinSkelDeviceObject* wsklDevObj = nullptr;
	NTSTATUS status = STATUS_INVALID_PARAMETER;

#pragma warning(suppress:6211) // we don't have an exception in this path, we won't leak the deviceObject
	status = IoCreateDevice(DriverObject, sizeof(WINSKEL_DEVICE_EXTENSION), DeviceName, FILE_DEVICE_FILE_SYSTEM, 0, FALSE, &deviceObject);
	while (NT_SUCCESS(status)) {
#pragma warning(push)
#pragma warning(disable:26400) // we're using a raw pointer anyway, not a template
#pragma warning(disable:26409) // we're calling new directly anyway; I don't think std::make_unique<T> will work for us
#pragma warning(disable:26447) // we're in the kernel - new doesn't throw
		wsklDevObj = new WinSkelDeviceObject;
#pragma warning(pop)
		if (nullptr == wsklDevObj) {
			status = STATUS_NO_MEMORY;
			break;
		}

		wsklDevObj->m_DeviceObject = deviceObject;

	}

	if (!NT_SUCCESS(status)) {
		// Error cleanup path
		if (nullptr != wsklDevObj) {
			delete wsklDevObj;
			wsklDevObj = nullptr;
		}
	}

	return wsklDevObj;
}

void WinSkelDeviceObject::DeleteDeviceObject(_In_ _Post_invalid_ WinSkelDeviceObject* DeviceObject) noexcept {
	if (nullptr == DeviceObject) {
		return;
	}

	IoDeleteDevice(DeviceObject->m_DeviceObject);
	// Note: at this point DeviceObject is _invalid_
	DeviceObject = nullptr;
	return;
}

NTSTATUS WinSkelDeviceObject::CallMajorFunction(PIRP Irp) noexcept {
#pragma warning(suppress:26462) // no marking of this with const suppresses the warning; quiescing it.
	PIO_STACK_LOCATION iosl = IoGetCurrentIrpStackLocation(Irp);

	if (nullptr == iosl) {
		CompleteRequest(Irp, STATUS_INVALID_PARAMETER);
	}

#pragma warning(push)
#pragma warning(disable:26482)
	if ((nullptr != iosl) && (iosl->MajorFunction < IRP_MJ_MAXIMUM_FUNCTION)) {
		return m_DispatchFunctions[iosl->MajorFunction](Irp);
	}
#pragma warning(pop)
	return InvalidDeviceRequest(Irp);
}


#pragma warning(push)
#pragma warning(disable:28176 28175) // we MEAN to modify the major function pointer
#pragma warning(disable:26446) // gsl::at() is not available here
#pragma warning(disable:26482) // we HAVE to index using a non-constant expression
#pragma warning(disable:28168) // add a dispatch type for IRP_MJ_SET_QUOTA?  Nah...
void WinSkelDeviceObject::SetMajorFunction(UINT8 MajorFunction, NTSTATUS(*DispatchFunction)(PIRP)) noexcept {
	if (MajorFunction < IRP_MJ_MAXIMUM_FUNCTION) {
		m_DispatchFunctions[MajorFunction] = DispatchFunction;
	}
	m_DeviceObject->DriverObject->MajorFunction[MajorFunction] = WinSkelDispatchRequest;
}
#pragma warning(pop)
