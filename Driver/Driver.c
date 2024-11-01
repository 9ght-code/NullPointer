#include "Driver.h"


//GLOBALS
PDRIVER_OBJECT Object;
PEPROCESS target_process = NULL;

void Unload(_In_ PDRIVER_OBJECT DriverObject) {
	UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\SecondMouse");

	IoDeleteSymbolicLink(&symLink);
	IoDeleteDevice(DriverObject->DeviceObject);

	DbgPrint("[+] Driver Unloaded [+]");

}


NTSTATUS DeviceControl(_In_ PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);

	PRequest request = (PRequest)Irp->AssociatedIrp.SystemBuffer;
	NTSTATUS status = STATUS_SUCCESS;

	PIO_STACK_LOCATION stackIRP = IoGetCurrentIrpStackLocation(Irp);

	if (stackIRP == NULL) {
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		DbgPrint("[-] NULL STACK [-]");

		return STATUS_UNSUCCESSFUL;
	}

	switch (stackIRP->Parameters.DeviceIoControl.IoControlCode) {
		case IO_ATTACH:

			if (request->proccess_id == NULL) {
				DbgPrint("[-] No Process ID [-]");
				status = STATUS_UNSUCCESSFUL;

				break;
			}

			DbgPrint("[+] Attached to the process [+]");

			status = PsLookupProcessByProcessId(request->proccess_id, &target_process);

			if (!NT_SUCCESS(status)) {
				DbgPrint("[-] Attaching Non Successful [-] ");
			}

			DbgPrint("[+] Attaching Successful [+]");

			if (stackIRP->Parameters.DeviceIoControl.InputBufferLength < sizeof(Request) ||
				stackIRP->Parameters.DeviceIoControl.OutputBufferLength < sizeof(Request)) {

				DbgPrint("[-] BUFFER IS TOO SMALL [-]");

				status = STATUS_BUFFER_TOO_SMALL; break;
			}

			if (target_process == NULL) {
				DbgPrint("[-] NO TARGET PROCESS [-]");
				status = STATUS_UNSUCCESSFUL;

				break;
			}

			request->BaseAdress = PsGetProcessSectionBaseAddress(target_process);

			if (request->BaseAdress == NULL) {
				DbgPrint("[-] ADDRESS_NOT_ASSOCIATED [-]");
				status = STATUS_ADDRESS_NOT_ASSOCIATED;

				break;
			}

			else
				DbgPrint("Base address: %p", request->BaseAdress);

			break;

		case IO_READ:

			if (target_process != NULL) {
				status = MmCopyVirtualMemory(target_process, request->target,
					PsGetCurrentProcess(), request->buffer,
					request->size, KernelMode, &request->return_size);

				DbgPrint("[+] Reading is successful [+]");

				if (!NT_SUCCESS(status))
					DbgPrint("[-] Some error while reading [-]");
			}

			break;

		case IO_WRITE:

			if (target_process != NULL) {
				status = MmCopyVirtualMemory(PsGetCurrentProcess(), request->buffer,
					target_process, request->target,
					request->size, KernelMode, &request->return_size);

				DbgPrint("[+] Writing is successful [+]");

				if (!NT_SUCCESS(status))
					DbgPrint("[-] Some error while writing [-]");
			}

			break;


		case IO_UNLOAD:
			Unload(Object);

			break;

		default:
			status = STATUS_INVALID_DEVICE_REQUEST;

			break;
	}

	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = sizeof(Request);

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return status;
}


NTSTATUS DriverCreateClose(_In_ PDEVICE_OBJECT DriverObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DriverObject);

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information= 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	
	return STATUS_SUCCESS;

}


NTSTATUS CreateDevice(PDRIVER_OBJECT DriverObject) {

	PDEVICE_OBJECT Device;
	UNICODE_STRING devName = RTL_CONSTANT_STRING(L"\\Device\\SecondMouse");
	NTSTATUS status = IoCreateDevice(DriverObject, 0, &devName, FILE_DEVICE_MOUSE, 0, FALSE, &Device);
	NTSTATUS createLink;

	if (!NT_SUCCESS(status)) {
		DbgPrint("[-] Failed to create device, unloading... [-]");
		return status;

	}

	else {
		DbgPrint("[+] Device Created[+]");

	}


	UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\SecondMouse");

	createLink = IoCreateSymbolicLink(&symLink, &devName);

	if (!NT_SUCCESS(createLink)) {
		DbgPrint("[-] Failed to create link... [-]");
		IoDeleteSymbolicLink(&symLink);
		return STATUS_FAILED_DRIVER_ENTRY;
	}

	DbgPrint("[+] Symbolic link created [+]");

	SetFlag(Device->Flags, DO_BUFFERED_IO);

	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceControl;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = DriverObject->MajorFunction[IRP_MJ_CLOSE] = DriverCreateClose;
	DriverObject->DriverUnload = Unload;

	Object = DriverObject;

	ClearFlag(Device->Flags, DO_DEVICE_INITIALIZING);

	return STATUS_SUCCESS;
}


NTSTATUS DriverMain(_In_ PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(RegistryPath);

	NTSTATUS deviceStatus;

	DbgPrint("[+] Driver loaded [+]");

	deviceStatus = CreateDevice(DriverObject);

	if (!NT_SUCCESS(deviceStatus)) {
		return STATUS_FAILED_DRIVER_ENTRY;
	}

	return STATUS_SUCCESS;
}

NTSTATUS DriverEntry() { //fake driver entry to get the ability to load the driver by KDmapper


	UNICODE_STRING driverName = { 0 };
	RtlInitUnicodeString(&driverName, L"\\Driver\\SecondMouse");

	return IoCreateDriver(&driverName, &DriverMain);

}