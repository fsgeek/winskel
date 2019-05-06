#include "WSControlDevice.h"


#pragma warning(push)
#pragma warning(disable:26432) // "you must override all default operations" - yeah, right
WSControlDevice::WSControlDevice() noexcept
{
}


WSControlDevice::~WSControlDevice()
{
}
#pragma warning(pop)


NTSTATUS WSControlDevice::Create(_In_ PIRP Irp)
{
	return CompleteRequest(Irp, STATUS_INVALID_DEVICE_REQUEST);
}

NTSTATUS WSControlDevice::Close(_In_ PIRP Irp)
{
	return CompleteRequest(Irp, STATUS_INVALID_DEVICE_REQUEST);
}

NTSTATUS WSControlDevice::FileSystemControl(_In_ PIRP Irp)
{
	return CompleteRequest(Irp, STATUS_INVALID_DEVICE_REQUEST);
}
