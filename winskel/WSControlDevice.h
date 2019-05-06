#pragma once
#include "WSDevice.h"
class WSControlDevice :
	public WSDevice
{

public:
	WSControlDevice() noexcept;
	~WSControlDevice();

	NTSTATUS Create(_In_ PIRP Irp) override;
	NTSTATUS Close(_In_ PIRP Irp) override;
	// NTSTATUS QueryInformation(_In_ PIRP Irp);
	// NTSTATUS QueryEa(_In_ PIRP Irp);
	// NTSTATUS DirectoryControl(_In_ PIRP Irp);
	NTSTATUS FileSystemControl(_In_ PIRP Irp) override;
	// NTSTATUS Cleanup(_In_ PIRP Irp);
	// NTSTATUS QuerySecurity(_In_ PIRP Irp);
	// NTSTATUS CompleteRequest(_In_ PIRP Irp, NTSTATUS Status);

};

