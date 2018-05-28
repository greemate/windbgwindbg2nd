/*++
Example Code for WinDbg for Kernel Debugging
--*/

#include "Mydrv.h"
#include "IoCtl.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text( INIT, DriverEntry )
#pragma alloc_text( PAGE, MyDrvCreateClose)
#pragma alloc_text( PAGE, MyDrvUnload)
#pragma alloc_text( PAGE, MyDrvDeviceControl)
#endif // ALLOC_PRAGMA


ULONG g_ulGlobal = 0x5678;
char g_szBuffer[16];

KEVENT Event;
KMUTEX MutexA;
KMUTEX MutexB;
ERESOURCE EresourceA;
ERESOURCE EresourceB;



NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT  DriverObject,
    IN PUNICODE_STRING RegistryPath
    )
/*++

Routine Description:

    Installable driver initialization entry point.
    This entry point is called directly by the I/O system.

Arguments:

    DriverObject - pointer to the driver object

    registryPath - pointer to a unicode string representing the path,
                   to driver-specific key in the registry.

Return Value:

    STATUS_SUCCESS if successful,
    STATUS_UNSUCCESSFUL otherwise

--*/
{
    NTSTATUS            status = STATUS_SUCCESS;
    UNICODE_STRING      unicodeDeviceName;   
    UNICODE_STRING      unicodeDosDeviceName;  
    PDEVICE_OBJECT      deviceObject;

    UNREFERENCED_PARAMETER (RegistryPath);

    (void) RtlInitUnicodeString(&unicodeDeviceName, MYDRV_DEVICE_NAME_U);

    status = IoCreateDevice(
                DriverObject,
                0, // sizeof(DEVICE_EXTENSION),
                &unicodeDeviceName,
                FILE_DEVICE_UNKNOWN,
                0,
                (BOOLEAN) FALSE,
                &deviceObject
                );

    if (!NT_SUCCESS(status))
    {
        return status;
    }

    DbgPrint("DeviceObject %p\n", deviceObject);
    
    //
    // Allocate and initialize a Unicode String containing the Win32 name
    // for our device.
    //

    (void)RtlInitUnicodeString( &unicodeDosDeviceName, MYDRV_DOS_DEVICE_NAME_U );

    status = IoCreateSymbolicLink(
                (PUNICODE_STRING) &unicodeDosDeviceName,
                (PUNICODE_STRING) &unicodeDeviceName
                );

    if (!NT_SUCCESS(status))
    {
        IoDeleteDevice(deviceObject);
        return status;
    }

    DriverObject->MajorFunction[IRP_MJ_CREATE]= 
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = MyDrvCreateClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = MyDrvDeviceControl;

    DriverObject->DriverUnload = MyDrvUnload;
    
	KeInitializeEvent( &Event, SynchronizationEvent, FALSE );
    KeInitializeMutex( &MutexA, 0 );
    KeInitializeMutex( &MutexB, 0 );
    ExInitializeResourceLite( &EresourceA );
    ExInitializeResourceLite( &EresourceB );

    return status;
}



NTSTATUS
MyDrvCreateClose(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
/*++

Routine Description:

   Process the Create and close IRPs sent to this device.

Arguments:

   DeviceObject - pointer to a device object.

   Irp - pointer to an I/O Request Packet.

Return Value:

      NT Status code

--*/
{
    PIO_STACK_LOCATION   irpStack;
    NTSTATUS             status = STATUS_SUCCESS;

    PAGED_CODE ();

    MYDRV_KDPRINT(("MyDrvCreateClose Enter\n"));

    //
    // Get a pointer to the current location in the Irp.
    //

    irpStack = IoGetCurrentIrpStackLocation(Irp);

    switch(irpStack->MajorFunction)
    {
        case IRP_MJ_CREATE:

            // 
            // The dispatch routine for IRP_MJ_CREATE is called when a 
            // file object associated with the device is created. 
            // This is typically because of a call to CreateFile() in 
            // a user-mode program or because a another driver is 
            // layering itself over a this driver. A driver is 
            // required to supply a dispatch routine for IRP_MJ_CREATE.
            //

            MYDRV_KDPRINT(("IRP_MJ_CREATE\n"));
            Irp->IoStatus.Information = 0;
            break;

        case IRP_MJ_CLOSE:

            //
            // The IRP_MJ_CLOSE dispatch routine is called when a file object
            // opened on the driver is being removed from the system; that is,
            // all file object handles have been closed and the reference count
            // of the file object is down to 0. 
            //

            MYDRV_KDPRINT(("IRP_MJ_CLOSE\n"));
            Irp->IoStatus.Information = 0;
            break;

        default:
            MYDRV_KDPRINT((" Invalid CreateClose Parameter\n"));
            status = STATUS_INVALID_PARAMETER;
            break;
    }

    //
    // Save Status for return and complete Irp
    //
    
    Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    MYDRV_KDPRINT((" MyDrvCreateClose Exit = %x\n", status));

    return status;
}


VOID
MyDrvUnload(
    IN PDRIVER_OBJECT DriverObject
    )
/*++

Routine Description:

    Free all the allocated resources, etc.

Arguments:

    DriverObject - pointer to a driver object.

Return Value:

    VOID
--*/
{
    PDEVICE_OBJECT       deviceObject = DriverObject->DeviceObject;
    UNICODE_STRING      uniWin32NameString;

    PAGED_CODE();

    MYDRV_KDPRINT(("MyDrvUnload Enter\n"));

	ExDeleteResourceLite( &EresourceA );
	ExDeleteResourceLite( &EresourceB );

    RtlInitUnicodeString( &uniWin32NameString, MYDRV_DOS_DEVICE_NAME_U );

    IoDeleteSymbolicLink( &uniWin32NameString );

    ASSERT(!deviceObject->AttachedDevice);
    
    IoDeleteDevice( deviceObject );
 
    MYDRV_KDPRINT(("MyDrvUnload Exit\n"));
    return;
}

NTSTATUS OpenClose(PCWSTR pPath)
{
	char szBuf[0x1000-0x100];		// 4KB-256
	NTSTATUS ntStatus;
	OBJECT_ATTRIBUTES objectAttributes;
	ACCESS_MASK ulMask;
	ULONG ulOption;
	IO_STATUS_BLOCK ioStatusBlk;
	UNICODE_STRING usFilePath;
	HANDLE hFile;
	
	if( pPath == NULL )
		return STATUS_INVALID_PARAMETER;
		
	RtlInitUnicodeString( &usFilePath, pPath );

	InitializeObjectAttributes( &objectAttributes, &usFilePath, OBJ_CASE_INSENSITIVE, NULL, NULL );

	ulMask = SYNCHRONIZE | GENERIC_READ;
	ulOption = FILE_SYNCHRONOUS_IO_NONALERT | FILE_NON_DIRECTORY_FILE | FILE_COMPLETE_IF_OPLOCKED;
				
	ntStatus = ZwCreateFile ( &hFile, ulMask, &objectAttributes, &ioStatusBlk, NULL, 0,
							  FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, FILE_OPEN,
							  ulOption,	NULL, 0 );

	if (NT_SUCCESS(ntStatus))
		ZwClose( hFile );

	return ntStatus;
}

KEVENT event;

void BugCheckA(void)
{
	KeSetEvent( &event, IO_NO_INCREMENT, FALSE );
}
	
void BugCheck50(void)
{
	PCHAR p = (PCHAR)0xfffffff0;
	
	*p = 0;	
}

void UseStack2(void)
{
	char szBuf[0x1000-0x10];		// 4KB-16
	
	OpenClose( L"\\??\\C:\\boot.ini" );
}

void UseStack1(void)
{
	char szBuf[0x1000-0x10];		// 4KB-16

	UseStack2();
}	

void BugCheck7F(void)
{
	char szBuf[0x1000-0x10];		// 4KB-16

	UseStack1();	
}

void MyStrCpy(PCHAR pDest, PCHAR pSrc)
{
	ULONG dwSrcLen, i;

	dwSrcLen = strlen( pSrc );
	
	for (i = 0; i < dwSrcLen; i++)
	{
		pDest[i] = pSrc[i];
	}

	pDest[i] = 0;
}

void BugCheck3B(void)
{
	int i;
	char *pBuffer[2] = { g_szBuffer, NULL };

	for (i = 0; i < 2; i++)
	{
		MyStrCpy( pBuffer[i], "BugCheck 0x3B" );
	}
}

void BugCheckBE(void)
{
}

void BugCheckC4(void)
{
	PCHAR p = (PCHAR)ExAllocatePool( NonPagedPool, 8 );

	strcpy( p, "Find Me" );
}

void BugCheckD6(void)
{
	PCHAR p = (PCHAR)ExAllocatePool( NonPagedPool, 8 );

	p[8] = 0;

	ExFreePool( p );
}

void PrintBreakPoint(PCHAR pParamStr)
{
	char *pLocalStr = "PrintBreakPoint Function\n";

	DbgPrint( pLocalStr );
	DbgPrint( pParamStr );
}

void OnBreakPoint(void)
{
	PrintBreakPoint( "Hello Debugging!\n" );
}

KDPC  HangDpc;

VOID
HangDpcRoutine(
    PKDPC Dpc,
    PVOID Context, 
    PVOID SystemArgument1,
    PVOID SystemArgument2
    )
{
    while( 1 );
}

VOID
Hang(VOID)
{
    CCHAR  i;

    for( i = 0; i < KeNumberProcessors; i++ ) 
	{
		KeInitializeDpc( &HangDpc, HangDpcRoutine, NULL );
		KeSetTargetProcessorDpc(&HangDpc, i );
		KeInsertQueueDpc( &HangDpc, NULL, NULL );
    }
}


NTSTATUS 
MyDrvDeviceControl(
	IN PDEVICE_OBJECT pDeviceObject, 
	IN PIRP pIrp
	)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
    ULONG ulIoControlCode;
	PVOID pInputBuffer, pOutputBuffer;
    ULONG ulInputBufferLength, ulOutputBufferLength;
    ULONG i;
    LARGE_INTEGER interval1, interval2;
    
    PIO_STACK_LOCATION pCurIrpStack = IoGetCurrentIrpStackLocation(pIrp);

    ulIoControlCode = pCurIrpStack->Parameters.DeviceIoControl.IoControlCode;
    
    pInputBuffer = pIrp->AssociatedIrp.SystemBuffer;
    pOutputBuffer = pIrp->AssociatedIrp.SystemBuffer;
    ulInputBufferLength = pCurIrpStack->Parameters.DeviceIoControl.InputBufferLength;
    ulOutputBufferLength = pCurIrpStack->Parameters.DeviceIoControl.OutputBufferLength;
    
    switch (ulIoControlCode)
    {
	case MYDRV_IOCTL_BREAKPOINT:
		OnBreakPoint();

    	ntStatus = STATUS_SUCCESS;
        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 0;
		break;

	case MYDRV_IOCTL_HANG:
		Hang();
		break;
    case MYDRV_IOCTL_IOVERIFY:
    	ntStatus = STATUS_TIMEOUT;
        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 0;
    	break;
    	
	case MYDRV_IOCTL_EVENT1:
		KeWaitForSingleObject( &Event, Executive, KernelMode, FALSE, NULL );
	
        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 0;
		break;

	case MYDRV_IOCTL_EVENT2:
		KeSetEvent( &Event, IO_NO_INCREMENT, FALSE );
	
        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 0;
		break;

	case MYDRV_IOCTL_MUTEX1:
		KeWaitForSingleObject( &MutexA, Executive, KernelMode, FALSE, NULL );

		interval1.QuadPart = -30000000;
		KeDelayExecutionThread( KernelMode, FALSE, &interval1 );

		KeWaitForSingleObject( &MutexB, Executive, KernelMode, FALSE, NULL );

		KeReleaseMutex( &MutexA, FALSE );
		KeReleaseMutex( &MutexB, FALSE );
		
        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 0;
		break;

	case MYDRV_IOCTL_MUTEX2:
		KeWaitForSingleObject( &MutexB, Executive, KernelMode, FALSE, NULL );

		interval2.QuadPart = -30000000;
		KeDelayExecutionThread( KernelMode, FALSE, &interval2 );

		KeWaitForSingleObject( &MutexA, Executive, KernelMode, FALSE, NULL );

		KeReleaseMutex( &MutexB, FALSE );
		KeReleaseMutex( &MutexA, FALSE );
		
        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 0;
		break;

	case MYDRV_IOCTL_ERESOURCE1:
		KeEnterCriticalRegion();
		ExAcquireResourceExclusiveLite( &EresourceA, TRUE );
		
		interval2.QuadPart = -30000000;
		KeDelayExecutionThread( KernelMode, FALSE, &interval2 );

		ExAcquireResourceExclusiveLite( &EresourceB, TRUE );
		
		ExReleaseResourceLite( &EresourceA );
		ExReleaseResourceLite( &EresourceB );
		KeLeaveCriticalRegion();
		
        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 0;
		break;

	case MYDRV_IOCTL_ERESOURCE2:
		KeEnterCriticalRegion();
		ExAcquireResourceExclusiveLite( &EresourceB, TRUE );
		
		interval2.QuadPart = -30000000;
		KeDelayExecutionThread( KernelMode, FALSE, &interval2 );

		ExAcquireResourceExclusiveLite( &EresourceA, TRUE );
		
		ExReleaseResourceLite( &EresourceB );
		ExReleaseResourceLite( &EresourceA );
		KeLeaveCriticalRegion();

        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 0;
		break;

	case MYDRV_IOCTL_BUGCHECK_0xA:
		BugCheckA();

        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 0;
		break;

	case MYDRV_IOCTL_BUGCHECK_0x1E:
		break;

	case MYDRV_IOCTL_BUGCHECK_0x50:
		BugCheck50();

        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 0;
		break;

	case MYDRV_IOCTL_BUGCHECK_0x7F:
		{
			char szBuf[0x1000-0x100];		// 4KB-256
			BugCheck7F();
		}

        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 0;
		break;

	case MYDRV_IOCTL_BUGCHECK_0x3B:
		BugCheck3B();

        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 0;
		break;
		
	case MYDRV_IOCTL_BUGCHECK_0xBE:
		BugCheckBE();

        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 0;
		break;

	case MYDRV_IOCTL_BUGCHECK_0xC4:
		BugCheckC4();

        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 0;
		break;

	case MYDRV_IOCTL_BUGCHECK_0xD6:
		BugCheckD6();

        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = 0;
		break;

	default:
		ntStatus = STATUS_INVALID_PARAMETER;
        pIrp->IoStatus.Status = STATUS_INVALID_PARAMETER;
        pIrp->IoStatus.Information = 0;
		break;	
    }

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	
	return ntStatus;
}

