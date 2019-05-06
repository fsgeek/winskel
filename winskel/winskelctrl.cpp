//
// This is the generic control interface for WinSkel
//
// (C) Copyright 2019 Tony Mason, All Rights Reserved
//
//

#include"wsdevobj.h"

#pragma warning(suppress:26461 28101)
NTSTATUS WinSkelControlInit(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	return STATUS_NOT_IMPLEMENTED;
}