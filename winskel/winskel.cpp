#include <ntifs.h>
#include <ntstatus.h>



static DRIVER_UNLOAD WinSkelUnload;
#pragma warning(suppress:26461)
static void WinSkelUnload(PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
	return;
}

extern "C" {
	DRIVER_INITIALIZE DriverEntry;
#pragma warning(suppress:26461)
	NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
	{
		UNREFERENCED_PARAMETER(DriverObject);
		UNREFERENCED_PARAMETER(RegistryPath);

		DriverObject->DriverUnload = WinSkelUnload;

		return STATUS_NOT_IMPLEMENTED;
	}
}