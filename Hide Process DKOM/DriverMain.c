#include "Header.h"

UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\HideProc");
UNICODE_STRING SymbolName = RTL_CONSTANT_STRING(L"\\DosDevices\\HideProc");
PDEVICE_OBJECT DeviceObject;

VOID Unload(PDRIVER_OBJECT pDriverObject) {
	DbgPrint("[+] Unloading Kernel Driver\n");
	IoDeleteSymbolicLink(&SymbolName);
	IoDeleteDevice(pDriverObject->DeviceObject);

}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath) {
	UNREFERENCED_PARAMETER(pRegistryPath);

	if (!pDriverObject) {
		DbgPrint("[+] Driver Initialization Failed\n");
		return STATUS_UNSUCCESSFUL;
	}

	NTSTATUS Status;
	DbgPrint("[+] Creating handle to Device Driver\n");
	Status = IoCreateDevice(pDriverObject, 0, &DeviceName, FILE_DEVICE_RK,
		0, TRUE, &DeviceObject);
	if (!NT_SUCCESS(Status)) {
		DbgPrint("[-] IoCreateDevice Failed\n");
		return STATUS_UNSUCCESSFUL;
	}

	Status = IoCreateSymbolicLink(&SymbolName, &DeviceName);
	if (!NT_SUCCESS(Status)) {
		DbgPrint("[-] IoCreateSymbolicLink Failed\n");
		return STATUS_UNSUCCESSFUL;
	}

	DbgPrint("[+] Setting Default Dispatch Routines\n");
	for (ULONG i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
		pDriverObject->MajorFunction[i] = DefaultDispatch;

	DbgPrint("[+] Setting IRP Dispatch for our IOCTL\n");
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoctlDispatch;

	pDriverObject->DriverUnload = Unload;

	return STATUS_SUCCESS;
}