#include <ntifs.h>
#include <ntddk.h>
#include "DriverCE.h"

UNICODE_STRING usDeivceName = RTL_CONSTANT_STRING(L"\\Device\\DriverCE");
UNICODE_STRING usSymbolicName = RTL_CONSTANT_STRING(L"\\??\\DriverCE");


void Unload(PDRIVER_OBJECT Driver);
NTSTATUS InitDeviceSymbolic(PDRIVER_OBJECT Driver);
NTSTATUS DispatchFuncCreateClose(PDEVICE_OBJECT Deivce, PIRP Irp);
NTSTATUS DisPatchFuncDeivceIoControl(PDEVICE_OBJECT Device, PIRP Irp);
NTSTATUS MyZwReadVirtualMemory(PREADDATA data);
NTSTATUS MyZwWriteVirtualMemory(PWRITEDTA data);


EXTERN_C NTSTATUS DriverEntry(PDRIVER_OBJECT Driver, PUNICODE_STRING usRegPath) {

	NTSTATUS status = STATUS_SUCCESS;
	UNREFERENCED_PARAMETER(Driver);

	UNREFERENCED_PARAMETER(usRegPath);

	Driver->DriverUnload = Unload;


	status = InitDeviceSymbolic(Driver);

	//Err
	
	if (!NT_SUCCESS(status)) {


		return status;
	}


	Driver->MajorFunction[IRP_MJ_CREATE] = Driver->MajorFunction[IRP_MJ_CLOSE] = DispatchFuncCreateClose;

	Driver->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DisPatchFuncDeivceIoControl;


	return status;

}

void Unload(PDRIVER_OBJECT Driver) {

	UNREFERENCED_PARAMETER(Driver);

	IoDeleteDevice(Driver->DeviceObject);

	IoDeleteSymbolicLink(&usSymbolicName);

	KdPrint(("[Driver-CE]:Unload Successly\r\n"));


}


NTSTATUS InitDeviceSymbolic(PDRIVER_OBJECT Driver) {
	NTSTATUS status=STATUS_SUCCESS;
	PDEVICE_OBJECT pdeojb = nullptr;
	
	status = IoCreateDevice(Driver, 0, &usDeivceName, FILE_DEVICE_UNKNOWN, 0, 0, &pdeojb);

	if (!NT_SUCCESS(status)) {

		KdPrint(("[Driver-CE]:failed to create device\r\n"));

		return status;

	}

	status = IoCreateSymbolicLink(&usSymbolicName, &usDeivceName);

	if (!NT_SUCCESS(status)) {

		IoDeleteDevice(pdeojb);

		KdPrint(("[Driver-CE]:failed to create symbolic name\r\n"));

		return status;
	}

	KdPrint(("[Driver-CE]:Create Successly\r\n"));

	return status;

}

NTSTATUS DispatchFuncCreateClose(PDEVICE_OBJECT Deivce, PIRP Irp)
{
	UNREFERENCED_PARAMETER(Deivce);

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS DisPatchFuncDeivceIoControl(PDEVICE_OBJECT Device, PIRP Irp)
{
	UNREFERENCED_PARAMETER(Device);
	NTSTATUS status = STATUS_SUCCESS;
	auto stack = IoGetCurrentIrpStackLocation(Irp);


	switch (stack->Parameters.DeviceIoControl.IoControlCode)
	{

	case CTL_CODE_READ:{
	
		
		//»º³åIO 
		PREADDATA data = (PREADDATA)Irp->AssociatedIrp.SystemBuffer;
		
		status=MyZwReadVirtualMemory(data);

		Irp->IoStatus.Status = status;

		Irp->IoStatus.Information = sizeof(READDATA);

		break;
		
	}
	case CTL_CODE_WRITE: {
		//»º³åIO 
		PWRITEDTA data = (PWRITEDTA)Irp->AssociatedIrp.SystemBuffer;

		status = MyZwWriteVirtualMemory(data);

		Irp->IoStatus.Status = status;

		Irp->IoStatus.Information = sizeof(PWRITEDTA);

		break;



	}
	default: {


		break;

	}
	}

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return status;
}

#pragma warning(disable : 4244)

NTSTATUS MyZwReadVirtualMemory(PREADDATA data) {

	NTSTATUS status = STATUS_SUCCESS;
	PEPROCESS FromProcess = nullptr, ToProcess = nullptr;


	status = ObReferenceObjectByHandle(data->hProcess, 0, *PsProcessType, KernelMode, (PVOID*)&FromProcess, 0);

	if (!NT_SUCCESS(status)) {

		KdPrint(("[Driver-CE]:unable to get object by handle errcode==0x%x\r\n",status));

		return status;
	}

	ToProcess = IoGetCurrentProcess();

	SIZE_T size = 0;

	status=MmCopyVirtualMemory(FromProcess, data->lpBaseAddress, ToProcess, data->lpBuffer, data->nSize, KernelMode, &size);


	if (!NT_SUCCESS(status)) {

		ObDereferenceObject(FromProcess);

		KdPrint(("[Driver-CE]:unable to read errcode==0x%x\r\n", status));

		return status;

	}

	if (MmIsAddressValid(data->lpNumberOfBytesRead)) {

		*(data->lpNumberOfBytesRead) = size;
	}

	ObDereferenceObject(FromProcess);
	return status;

}

NTSTATUS MyZwWriteVirtualMemory(PWRITEDTA data) {

	NTSTATUS status = STATUS_SUCCESS;
	PEPROCESS FromProcess = nullptr, ToProcess = nullptr;


	status = ObReferenceObjectByHandle(data->hProcess, 0, *PsProcessType, KernelMode, (PVOID*)&ToProcess, 0);

	if (!NT_SUCCESS(status)) {

		KdPrint(("[Driver-CE]:unable to get object by handle errcode==0x%x\r\n", status));

		return status;
	}

	FromProcess = IoGetCurrentProcess();

	SIZE_T size = 0;

	status = MmCopyVirtualMemory(FromProcess, data->lpBuffer, ToProcess, data->lpBaseAddress, data->nSize, KernelMode, &size);


	if (!NT_SUCCESS(status)) {


		ObDereferenceObject(ToProcess);
		KdPrint(("[Driver-CE]:unable to read errcode==0x%x\r\n", status));

		return status;

	}

	if (MmIsAddressValid(data->lpNumberOfBytesWritten)) {

		*(data->lpNumberOfBytesWritten) = size;
	}

	ObDereferenceObject(ToProcess);
	return status;


}