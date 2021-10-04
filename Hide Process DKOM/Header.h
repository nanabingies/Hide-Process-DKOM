#pragma once
#include <ntifs.h>
#include <ntddk.h>

//UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\HideProc");
//UNICODE_STRING SymbolName = RTL_CONSTANT_STRING(L"\\DosDevices\\HideProc");

#define FILE_DEVICE_RK 0x00008001
#define IOCTL_TEST_CMD CTL_CODE(FILE_DEVICE_RK, 0x801, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)

NTSTATUS DefaultDispatch(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
NTSTATUS IoctlDispatch(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);

//DRIVER_INITIALIZE DriverEntry;

NTSTATUS RemoveEntry(PLIST_ENTRY);
VOID Unload(PDRIVER_OBJECT);
VOID Operation(PHANDLE*);