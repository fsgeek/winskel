#include <ntifs.h>
#include <ntstatus.h>
#include "cpprt.h"



static DRIVER_UNLOAD WinSkelUnload;
#pragma warning(suppress:26461)
static void WinSkelUnload(PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
	return;
}

extern "C" DRIVER_INITIALIZE DriverEntry;
#pragma warning(suppress:26461)
extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	NTSTATUS status = STATUS_NOT_SUPPORTED;
	NTSTATUS status2 = STATUS_NOT_SUPPORTED;
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	status = cpp_rt_pre_init();
	if (!NT_SUCCESS(status)) {
		return status;
	}

	// I use a loop for clean exit control
	while (NT_SUCCESS(status)) {

		DriverObject->DriverUnload = WinSkelUnload;

		// Done
		break;
	}

	status2 = cpp_rt_post_init(DriverObject, RegistryPath);
	if (!NT_SUCCESS(status2)) {
		return NT_SUCCESS(status) ? status2 : status; // things are seriously screwed up
	}

	if (!NT_SUCCESS(status)) {
		// something went wrong, so we will go through the cleanup path.
		DriverObject->DriverUnload(DriverObject);
	}

	return status;
}
