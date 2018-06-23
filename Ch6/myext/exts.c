/*-----------------------------------------------------------------------------
   Copyright (c) 2000  Microsoft Corporation

Module:
  exts.c

Sample old windbg style interface using extension 
  ----------------------------------------------------------------------------
  을 수정하여 만듦.

  덤프파일에서 파일옵젝트에 해당하는 파일을 추출해 주는 익스텐션 작성!
  
  현재는 데이터 파일로 오픈되어 데이터 섹션이 존재하는 파일만 지원함.
  실행파일은 이미지 섹션을 읽어야 하는데 언제 지원할지 모름. -_-;;;

  Written by GreeMate 2008/01/22 
------------------------------------------------------------------------------*/

#include "myext.h"

//
// ntddk.h 를 include 하지 않기 때문에 필요한 타입만 정의한 nttype.h 를 만든다.
// 기본적으로 dll 이기 때문에 windows.h 를 include 하는데 ntddk.h 와 쫑나는 곳이 많다.
// 일이 잡아주는게 귀찮으므로 nttype.h 를 만들어 버린다.
//
#include "nttype.h"

//
// C 런타임 파일 함수를 사용하기 위하여 stdio.h 를 include 한다.
//
#include <stdio.h>


//
// 전역 디파인
//
#define SIZE_1MB	0x100000		// 내부 VACB 관리 기준 1MB
#define SIZE_32MB	0x2000000		// 다중 VACB 관리 기준 32MB

#define SIZE_256KB	0x40000			// VACB 당 Cache View 크기
#define SIZE_4KB	0x1000			// x86 PAGE 크기
#define SIZE_PAGE	SIZE_4KB		// x86 PAGE 크기

#define PAGE_NUM	(SIZE_256KB / SIZE_PAGE)	// Cache View 하나에 포함된 페이지 갯수

#define MAX_VACB	128				// 단일 VACB 관리시 최대 갯수


//
// 전역 변수
//

// 0 으로 초기화된 4KB 짜리 버퍼를 준비한다.
char	g_ZeroData[SIZE_PAGE] = {0,};



//
// 함수 구현
//

void FillZeroData(FILE *fp)
/**
	하나의 Cache View 영역을 파일에 0 으로 채운다.

	@param[in]	fp		파일포인터

	@retval     없음

	@remarks

	@see	
*/
{
	ULONG i;
	
#if DBG
    dprintf("FillZeroData 256KB\n");
#else
	dprintf("x");		// 데이터를 구하지 못했다는 표시를 한다.
#endif

	//
	// PAGE_NUM 만큼 루프를 돌면서 1 페이지씩 0 으로 채운다.
	//
	for (i = 0; i < PAGE_NUM; i++)
	{
		if (fwrite( g_ZeroData, 1, SIZE_PAGE, fp ) != SIZE_PAGE)
		{
			dprintf( "FillZeroData-fwrite Error\n" );
			return; 	
		}	
	}
}

void CopyVacbData(PVACB pVacb, FILE *fp)
/**
	VACB 가 가리키는 Cache View 256KB 를 파일에 쓴다.

	@param[in]	pVacb	VACB 주소
	@param[in]	fp		파일포인터

	@retval     없음

	@remarks

	@see	
*/
{
	VACB 	stVacb;
	ULONG	cb;
	PCHAR	pData;
	ULONG 	i, j;
	char	buf[SIZE_PAGE];
	
	//
	// pVacb 주소의 내용을 stVacb 로 읽어온다.
	//
	if (ReadMemory( (ULONG64)pVacb, &stVacb, sizeof(VACB), &cb ) == 0)
	{
		// 실패하면 이 VACB 가 가리키는 영역을 모두 0 으로 파일에 쓰고 리턴한다.
#if DBG
	    dprintf("CopyVacbData-pVacb reading error.\n");
#endif
		FillZeroData( fp );
	    return;
	}
		
	//
	// PAGE_NUM 만큼 루프를 돌면서 Cache View 256KB 를 파일에 쓴다.
	//
	for (i = 0; i < PAGE_NUM; i++)
	{
		// PAGE 크기만큼 증가시키면서 복사할 데이터 주소를 구한다.
		pData = (PCHAR)((ULONG64)stVacb.BaseAddress + (i * SIZE_PAGE));

		// 주소에서 데이터를 PAGE 단위로 읽는다.
		if (ReadMemory( (ULONG64)pData, buf, SIZE_PAGE, &cb ) != 0)
		{
#if DBG
			for (j = 0; j < cb; j=j+16)
			{
				dprintf( "%08x  %02x %02x %02x %02x %02x %02x %02x %02x-%02x %02x %02x %02x %02x %02x %02x %02x\n", 
						pData + j,
						buf[j], buf[j+1], buf[j+2], buf[j+3], buf[j+4], buf[j+5], buf[j+6], buf[j+7], buf[j+8], buf[j+9], buf[j+10], buf[j+11], buf[j+12], buf[j+13], buf[j+14], buf[j+15] );
			}
#endif
			// 성공하면 파일에 쓴다.
			if (fwrite( buf, 1, SIZE_PAGE, fp ) != SIZE_PAGE)
			{
				dprintf( "CopyVacbData-fwrite Error\n" );
				return; 	
			}
		}
		else
		{
#if DBG
			dprintf( "%08x  4KB Data Page Out\n", pData );
#endif
			// 실패하면 0 으로 채워진 4KB 데이터를 파일에 쓴다.
			if (fwrite( g_ZeroData, 1, SIZE_PAGE, fp ) != SIZE_PAGE)
			{
				dprintf( "CopyVacbData-fwrite ZeroData Error\n" );
				return; 	
			}	
		}
	}
}

void ReadFileContent(SHARED_CACHE_MAP stSharedCcMap, ULONG ulFileSize, FILE *fp)
/**
	SHARED_CACHE_MAP 을 이용하여 파일 데이터를 읽고 파일에 쓴다.

	@param[in]	stSharedCcMap	SHARED_CACHE_MAP 구조체
	@param[in]	ulFileSize		파일크기
	@param[in]	fp				파일포인터

	@retval     없음

	@remarks

	@see	
*/
{
	PVACB	apVacb[MAX_VACB];
	PVACB	pVacb;
	ULONG	ulMaxIndex, ulIndex;
	ULONG	cb;

	ulMaxIndex = (ULONG)(ulFileSize / SIZE_256KB);
	ulMaxIndex++;

#if DBG	
	dprintf( "VACBs Num : %d\n", ulMaxIndex );
#endif

	// 파일크기가 1MB 보다 크면 pSharedCcMap->InitialVacbs 룰 사용하지 않고 pSharedCcMap->Vacbs 를 사용한다.
	if (ulFileSize > SIZE_1MB)
	{
		// Vacbs 주소에 저장된 VACB 포인터 리스트를 읽는다.
		if (ReadMemory( (ULONG64)stSharedCcMap.Vacbs, &apVacb, (sizeof(VACB)*ulMaxIndex), &cb ) == 0)
		{
#if DBG
		    dprintf("pSharedCcMap->Vacbs reading error.\n");
#endif
		    return;
		}
		else
		{
			// aVacb[] 채우기 성공
		}
	}
	else
	{
		// 1MB보다 작으면 256KB 크기 4군데로 저장한 Vacb 주소 얻기 
		apVacb[0] = stSharedCcMap.InitialVacbs[0];
		apVacb[1] = stSharedCcMap.InitialVacbs[1];
		apVacb[2] = stSharedCcMap.InitialVacbs[2];
		apVacb[3] = stSharedCcMap.InitialVacbs[3];
	}

	// VACB 하나에 해당하는 256KB 씩 데이터를 읽으면서 모든 VACB 를 읽는다.
	for (ulIndex = 0; ulIndex < ulMaxIndex; ulIndex++)
	{
#if !DBG
		// Release 빌드에서 동작중임을 표시한다.
		dprintf(".");
#endif		

		pVacb = apVacb[ulIndex];

		if (pVacb == NULL)
		{
			// pVacb 가 NULL 이면 메모리에 존재하지 않는 것이므로 그냥 0 으로 채운다.
			FillZeroData( fp );	
		}
		else
		{
			// 포인터가 존재하면 Cache view 에 존재하는 내용을 파일에 쓴다.
			CopyVacbData( pVacb, fp );
		}
	}
}


void ExtractFile( ULONG64 pAddress, PCSTR pFileName )
/**
	파일옵젝트 주소로부터 파일 데이터를 읽어서 파일로 저장한다.

	@param[in]	pAddress	파일옵젝트 주소
	@param[in]	pFileName	파일이름

	@retval     없음

	@remarks

	@see	
*/
{
	FILE_OBJECT				stFileObj;
	SECTION_OBJECT_POINTERS	stSecObjPtr;
	SHARED_CACHE_MAP		stSharedCcMap;
	ULONG					ulFileSize;
	ULONG					cb;
	FILE*					fp;
	
	// 입력 파라미터를 확인한다.
	if (pAddress == (ULONG64)NULL || pFileName == NULL)
	{
	    dprintf("Parameter is NULL! Check your parameters\n");
	    return;
	}

	// 파일옵젝트 메모리 내용을 읽어들인다.
	if (ReadMemory( pAddress, &stFileObj, sizeof(FILE_OBJECT), &cb ) == 0)
	{
	    dprintf("FILE_OBJECT reading error.\n");
	    return;
	}
	
	// 파일 옵젝트 타입을 확인한다.
	if (stFileObj.Type != 5)
	{
	    dprintf("%x is not file object.\n");
	    return;
	}
	
	// SectionObjectPointer 가 유효한지 확인한다.
	if (stFileObj.SectionObjectPointer == NULL)
	{
	    dprintf("[%x]->SectionObjectPointer == NULL.\n", pAddress);
	    return;
	}
	
	// SectionObject 주소의 내용을 읽어들인다.
	if (ReadMemory( (ULONG64)stFileObj.SectionObjectPointer, &stSecObjPtr, sizeof(SECTION_OBJECT_POINTERS), &cb ) == 0)
	{
	    dprintf("SECTION_OBJECT_POINTER reading error.\n");
	    return;
	}
	
	// SharedCacheMap 가 유효한지 확인한다.
	if (stSecObjPtr.SharedCacheMap == NULL)
	{
	    dprintf("[%x]->SectionObjectPointer->SharedCacheMap == NULL.\n", pAddress);
	    return;
	}
	
	// SharedCacheMap 을 읽어들인다.
	if (ReadMemory( (ULONG64)stSecObjPtr.SharedCacheMap, &stSharedCcMap, sizeof(SHARED_CACHE_MAP), &cb ) == 0)
	{
	    dprintf("SHARED_CACHE_MAP reading error.\n");
	    return;
	}
	
	// SharedCacheMap 내용을 출력한다.
	dprintf("SHARED_CACHE_MAP\n");
	dprintf("================\n");
	dprintf("FileSize        : %ld\n", stSharedCcMap.FileSize);
	dprintf("FileSize(Hex)   : 0x%x\n", stSharedCcMap.FileSize);
	dprintf("InitialVacbs[0] : 0x%p\n", stSharedCcMap.InitialVacbs[0]);
	dprintf("InitialVacbs[1] : 0x%p\n", stSharedCcMap.InitialVacbs[1]);
	dprintf("InitialVacbs[2] : 0x%p\n", stSharedCcMap.InitialVacbs[2]);
	dprintf("InitialVacbs[3] : 0x%p\n", stSharedCcMap.InitialVacbs[3]);
	dprintf("Vacbs           : 0x%p\n", stSharedCcMap.Vacbs);
	dprintf("FileObject      : 0x%p\n", stSharedCcMap.FileObject);
	
	// 파일크기가 32MB 가 넘는지 확인한다. 
	if (stSharedCcMap.FileSize.HighPart != 0)
	{
	    dprintf("File size over 32MB is NOT supported.\n");
	    return;
	}
	
	ulFileSize = stSharedCcMap.FileSize.LowPart;  
              
	// 32MB 가 넘는 파일은 다중 Vacb 참조를 해야하는데 귀찮으므로 지원하지 않는다.
	if (ulFileSize > SIZE_32MB)
	{
	    dprintf("File size over 32MB is NOT supported.\n");
	    return;
	}
	
	// 대상 파일을 만든다.
	fp = fopen( pFileName, "wb" );
	if (fp == NULL)
	{
	    dprintf("Creating %s failed.\n", pFileName);
	    return;
	}
		
	// VACB 를 찾아서 파일 데이터를 읽고 파일에 쓴다.
	ReadFileContent( stSharedCcMap, ulFileSize, fp );
	
	// 파일을 닫고 종료한다.
	fclose( fp );
}


PCSTR SkipSpace(PCSTR pString)
/**
	공백문자를 검사해서 공백문자가 아닌 위치를 리턴한다.

	@param[in]	pString		공백문자로 시작하는 문자열

	@retval     포인터값	공백문자가 아닌 문자의 포인터
				NULL		공백문자가 아닌 문자를 찾을 수 없음

	@remarks

	@see	
*/
{
	if (pString == NULL)
		return NULL;
		
	while( *pString != '\0' )
	{
		if (*pString != ' ')
		{
			return pString;
		}

		pString++;
	} 	
	
	return NULL;
}


//
// Extension to extract file from shared section
//  
//    !ef <fileobject address> <filename>
//
DECLARE_API( ef )
{
    ULONG cb;
    ULONG64 Address = 0;
    PCSTR pFileName = NULL;
    

	// args 문자열에서 첫번째 파라미터를 숫자로 가져온다.
    if (GetExpressionEx(args, &Address, &args) == FALSE) 
    {
        dprintf("Usage:   !ef <fileobject address> <filename>\n");
        return;
    }
    
    // 갱신된 args 문자열에서 두번째 파라미터가 존재하는지 확인한다.
    if (args[0] == 0)
    {
        dprintf("Usage:   !ef <fileobject address> <filename>\n");
        return;
    }
    	
	// args 에서 문자열 가져오는 함수가 있을 것 같은데 찾지를 못했다. -_-
	// GetExpressionEx() 를 수행하고 나면 args 는 <fileobject address> 바로 뒤의 공백을 가리키는데
	// 공백을 스킵하고 <filename> 을 얻기 위해 SkipSpace() 함수를 사용한다.
	pFileName = SkipSpace( args );
    
    dprintf("Creating \"%s\" with file object(%p)\n", pFileName, Address);
    
	// 파일 데이터를 추출하여 파일을 만든다.
    ExtractFile( Address, pFileName );
    
    dprintf("Done!\n");
}


/*
  A built-in help for the extension dll
*/

DECLARE_API ( help ) 
{
    dprintf("Help for extension dll myext.dll\n"
            "   help - Shows this help\n"
            "   ef <fileobject address> <filename> - It extract file from shared section\n"
            );
}
