#ifndef _MYDRV_H_
#define _MYDRV_H_

#include <ntddk.h>

//  Debugging macros

#if DBG
#define MYDRV_KDPRINT(_x_) \
                DbgPrint("MYDRV.SYS: ");\
                DbgPrint _x_;
#else

#define MYDRV_KDPRINT(_x_)

#endif



#define MYDRV_DEVICE_NAME_U     L"\\Device\\MyDrv"
#define MYDRV_DOS_DEVICE_NAME_U L"\\DosDevices\\MyDrv"

/*
typedef struct _DEVICE_EXTENSION{

    BOOLEAN ThreadShouldStop;
    
    // Irps waiting to be processed are queued here
    LIST_ENTRY   PendingIrpQueue;

    //  SpinLock to protect access to the queue
    KSPIN_LOCK QueueLock;

    IO_CSQ CancelSafeQueue;   

    // Time at which the device was last polled
    LARGE_INTEGER LastPollTime;  

    // Polling interval (retry interval)
    LARGE_INTEGER PollingInterval;

    KSEMAPHORE IrpQueueSemaphore;

    PETHREAD ThreadObject;

    
}  DEVICE_EXTENSION, *PDEVICE_EXTENSION;
*/
NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT  DriverObject,
    IN PUNICODE_STRING registryPath
	);

NTSTATUS
MyDrvCreateClose (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

VOID
MyDrvUnload(
    IN PDRIVER_OBJECT DriverObject
    );

NTSTATUS 
MyDrvDeviceControl(
	IN PDEVICE_OBJECT pDeviceObject, 
	IN PIRP pIrp
	);

#endif


