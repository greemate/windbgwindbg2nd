
/*++
   Copyright (c) 2008  Mycrosoft Corporation

Module:

    nttype.h

    ntddk.h, ntdef.h 등에 정의되어 있는 타입들을 정의한다.

--*/


//typedef char  BOOLEAN;
typedef short CSHORT;
typedef DWORD ULONG;
typedef DWORD NTSTATUS;

typedef PVOID PDEVICE_OBJECT;
typedef PVOID PVPB;
typedef PVOID PIO_COMPLETION_CONTEXT;


typedef struct _DISPATCHER_HEADER {
    UCHAR Type;
    UCHAR Absolute;
    UCHAR Size;
    UCHAR Inserted;
    LONG SignalState;
    LIST_ENTRY WaitListHead;
} DISPATCHER_HEADER;


typedef struct _KEVENT {
    DISPATCHER_HEADER Header;
} KEVENT, *PKEVENT, *RESTRICTED_POINTER PRKEVENT;


typedef struct _SECTION_OBJECT_POINTERS {
    PVOID DataSectionObject;
    PVOID SharedCacheMap;
    PVOID ImageSectionObject;
} SECTION_OBJECT_POINTERS;
typedef SECTION_OBJECT_POINTERS *PSECTION_OBJECT_POINTERS;


typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;


typedef struct _FILE_OBJECT {
    CSHORT Type;
    CSHORT Size;
    PDEVICE_OBJECT DeviceObject;
    PVPB Vpb;
    PVOID FsContext;
    PVOID FsContext2;
    PSECTION_OBJECT_POINTERS SectionObjectPointer;
    PVOID PrivateCacheMap;
    NTSTATUS FinalStatus;
    struct _FILE_OBJECT *RelatedFileObject;
    BOOLEAN LockOperation;
    BOOLEAN DeletePending;
    BOOLEAN ReadAccess;
    BOOLEAN WriteAccess;
    BOOLEAN DeleteAccess;
    BOOLEAN SharedRead;
    BOOLEAN SharedWrite;
    BOOLEAN SharedDelete;
    ULONG Flags;
    UNICODE_STRING FileName;
    LARGE_INTEGER CurrentByteOffset;
    ULONG Waiters;
    ULONG Busy;
    PVOID LastLock;
    KEVENT Lock;
    KEVENT Event;
    PIO_COMPLETION_CONTEXT CompletionContext;
} FILE_OBJECT, *PFILE_OBJECT;;


typedef struct _VACB {
    PVOID               		BaseAddress;
    struct _SHARED_CACHE_MAP   	*SharedCacheMap;
    union {
        LARGE_INTEGER   		FileOffset;
        USHORT          		ActiveCount;
    } Overlay;
    LIST_ENTRY          		LruList;
} VACB, *PVACB;


typedef struct _SHARED_CACHE_MAP {
    CSHORT                      NodeTypeCode;
    CSHORT                      NodeByteSize;
    ULONG                       OpenCount;
    LARGE_INTEGER               FileSize;
    LIST_ENTRY                  BcbList;
    LARGE_INTEGER               SectionSize;
    LARGE_INTEGER               ValidDataLength;
    LARGE_INTEGER               ValidDataGoal;
    PVACB                       InitialVacbs[4];
    PVACB                       *Vacbs;
    PFILE_OBJECT                FileObject;
    PVACB                       ActiveVacb;
    PVOID                       NeedToZero;
    ULONG                       ActivePage;
    ULONG                       NeedToZeroPage;
	
	// 생략 ...

} SHARED_CACHE_MAP, *PSHARED_CACHE_MAP;


