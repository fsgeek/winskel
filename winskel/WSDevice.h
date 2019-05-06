#pragma once

#include <wdm.h>

class WSDevice
{

	NTSTATUS InvalidDeviceRequest(_In_ PIRP Irp);
	PDEVICE_OBJECT m_DeviceObject = nullptr;
	void* operator new(size_t size, void* memory) { UNREFERENCED_PARAMETER(size); return memory; }

public:
	virtual NTSTATUS Create(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS CreateNamedPipe(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS Close(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS Read(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS Write(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS QueryInformation(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS SetInformation(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS QueryEa(_In_ PIRP Irp)   { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS SetEa(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS QueryVolumeInformation(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS SetVolumeInformation(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS DirectoryControl(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS FileSystemControl(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS DeviceControl(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS InternalDeviceControl(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS Shutdown(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS LockControl(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS Cleanup(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS CreateMailslot(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS QuerySecurity(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS SetSecurity(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS Power(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS SystemControl(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS DeviceChange(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS QueryQuota(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS SetQuota(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS PnP(_In_ PIRP Irp)  { return InvalidDeviceRequest(Irp); }
	virtual NTSTATUS CompleteRequest(_In_ _Post_invalid_ PIRP Irp, NTSTATUS Status);
	virtual ~WSDevice();

	PDEVICE_OBJECT GetDeviceObject(void) noexcept { return m_DeviceObject; }

	_Must_inspect_result_
	static WSDevice* CreateWSDevice(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING DeviceName);
};

