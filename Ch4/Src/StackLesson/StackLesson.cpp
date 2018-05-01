// StackLesson.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void NoRetNoParams()
{
	unsigned long ulOldEBP = 0;
	unsigned long ulCurrentEBP = 0;
	unsigned long ulRetIP = 0;

	_asm 
	{
		mov ulCurrentEBP, EBP
	}

	ulOldEBP = *(unsigned long*)ulCurrentEBP;
	ulRetIP = *(unsigned long*)(ulCurrentEBP + 4);

	printf("OldEBP %x, Current EBP %x, Return IP %x \n", ulOldEBP, ulCurrentEBP, ulRetIP);	
	
}

int __cdecl cdeclCallingConventionWithParams(char* Arg1, unsigned long Arg2)
{
	int nLocalVar1 = Arg2 + 2009;
	int nLocalVar2 = 0;

	_asm
	{
		push eax
		mov eax, [ebp+8]
		mov nLocalVar2, eax
		pop eax
	}
	
	printf("FunctionType: %s \n", nLocalVar2);

	return nLocalVar1;
}

int __stdcall stdcallCallingConventionWithParams(char* Arg1, unsigned long Arg2)
{
	int nLocalVar = Arg2 + 2010;

	printf("FunctionType: %s \n", Arg1);

	return nLocalVar;
}

int __fastcall fastcallCallingConventionWithParams(char* Arg1, unsigned long Arg2, unsigned long Arg3)
{
	char*			pLocalVar1;
	unsigned long	ulLocalVar2;
	unsigned long	ulLocalVar3;
	
	_asm
	{
		mov pLocalVar1, ECX
		mov ulLocalVar2, EDX
		push eax
		mov eax, [ebp+8]
		mov ulLocalVar3, eax
		pop eax
	}

	printf("FunctionType: %s \n", pLocalVar1);

	return ulLocalVar3;
}

class CSampleClass
{
public:
	CSampleClass(): nJunk(0) {}
	virtual ~CSampleClass() {}

	int nJunk;
	
	int ShowThisCallCallingConvention(char* Arg1, unsigned long Arg2, unsigned long Arg3)
	{
		CSampleClass* pThis = NULL;
		_asm mov pThis, ECX
		char* LocalVar1 = NULL;
		unsigned long LocalVar2 = Arg2;
		unsigned long LocalVar3 = Arg3;


		nJunk = Arg2 + Arg3 + 2012;

		if (pThis == this)
		{
			printf("ECX has this pointer! (%x) \n", pThis);
		}

		printf("FunctionType: %s \n", Arg1);
		return nJunk;
	}
};


int main(int argc, char* argv[])
{
	NoRetNoParams();

	// cdecl calling convention
	cdeclCallingConventionWithParams("cdecl Calling Convention", 'cdec');
	
	// stdcall calling convention
	stdcallCallingConventionWithParams("stdcall Calling Convention", 'stdc');

	// fastcall calling convention
	fastcallCallingConventionWithParams("fastcall Calling Convention", 'fast', 'call');

	// thiscall calling convention
	CSampleClass sample;
	sample.ShowThisCallCallingConvention("thiscall Calling Convention", 'this', 'call');

	return 0;
}
