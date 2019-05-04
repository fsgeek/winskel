#include <ntifs.h>
#include <ntstatus.h>

extern "C" {
	NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, const PUNICODE_STRING RegistryPath);
}

DRIVER_INITIALIZE DriverEntry;
#pragma ignore(warning:26461) // we don't mark it as const because we will be modifying it
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, const PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	return STATUS_NOT_IMPLEMENTED;
}