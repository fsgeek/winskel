#include "WSDevice.h"

#pragma warning(disable:26409) // "don't use new/delete"
#pragma warning(disable:26400) // "don't use raw pointers from allocation"

#if 0
const ULONG WSDeviceMemTag = (ULONG)"eDsW";

__drv_allocatesMem(Mem)
void* WSDevice::operator new(size_t size)
{
	return ExAllocatePoolWithTag(PagedPool, size, WSDeviceMemTag);
}

void WSDevice::operator delete(void* p)
{
	if (nullptr != p) {
		ExFreePoolWithTag(p, WSDeviceMemTag);
	}
}
#endif // 0


NTSTATUS WSDevice::InvalidDeviceRequest(_In_ PIRP Irp)
{
	const NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;

	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return status;
}



#pragma warning(push)
#pragma warning(disable:26432) // "you must override all default operations" - yeah, right
WSDevice::~WSDevice() noexcept
{
}
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable:6014) // it insists the device object is being leaked, but we've saved it
WSDevice* WSDevice::CreateWSDevice(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING DeviceName)
{
	PDEVICE_OBJECT deviceObject = nullptr;
	WSDevice* wsDevObj = nullptr;
	NTSTATUS status = STATUS_INVALID_PARAMETER;

#pragma warning(suppress:6211) // we don't have an exception in this path, we won't leak the deviceObject
	status = IoCreateDevice(DriverObject, sizeof(WSDevice), DeviceName, FILE_DEVICE_FILE_SYSTEM, 0, FALSE, &deviceObject);

	while (NT_SUCCESS(status)) {

		if ((nullptr == deviceObject) || (nullptr == deviceObject->DeviceExtension)) {
			status = STATUS_NO_MEMORY;
			break;
		}

		wsDevObj = new (deviceObject->DeviceExtension) WSDevice;
		wsDevObj->m_DeviceObject = deviceObject;
		
		// Done
		break;
	}

	if (!NT_SUCCESS(status)) {
		// Error cleanup path
		if (nullptr != wsDevObj) {
			delete wsDevObj;
			wsDevObj = nullptr;
		}
	}

	return wsDevObj;
}
#pragma warning(pop)