#include "OEM_SA_Mem.h"
#include "SYSAPI.h"
#include <sys/sysinfo.h>


atBOOL OEM_SYSAPI_InitMem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atTRUE;
}


void OEM_SYSAPI_FinalMem(void)
{
}


void * OEM_SYSAPI_AllocMem( long size )
{
	return (void *)malloc( size );
}


void * OEM_SYSAPI_ReallocMem( void *ptr, long size )
{
	return (void *)realloc( ptr, size );
}


int OEM_SYSAPI_FreeMem( void *ptr )
{
	free( ptr );
	return 0;
}


atBOOL  OEM_SYSAPI_CheckValidateHeap( NOARGS )
{
	return atTRUE;
}


atVOID  OEM_SYSAPI_GetHeapInfo( atLPVOID lpParam, void (*pViewFunc)(atLPVOID lpParam, void *addr, int pos, int size, int type) )
{
}

atLONG OEM_SYSAPI_GetTotalHeapSize()
{
	struct sysinfo info;
	sysinfo(&info);
	return info.totalram;
}

atLONG  OEM_SYSAPI_GetTotalLeftHeapSize( NOARGS )
{
	struct sysinfo info;
	sysinfo(&info);
	return info.freeram;
}


atVOID  OEM_SYSAPI_CompactHeap( NOARGS )
{
}


void init_sysapi_oem() 
{
	OEM_SYSAPI_MEM_T oem_mem = {
		OEM_SYSAPI_InitMemory,
		OEM_SYSAPI_FinalMemory,
		OEM_SYSAPI_GetSystemMemorySize,

		OEM_SYSAPI_AllocMem,
		OEM_SYSAPI_ReallocMem,
		OEM_SYSAPI_FreeMem,

		atNULL,
		atNULL,
		atNULL,
		atNULL,

		OEM_SYSAPI_CheckValidateHeap,
		OEM_SYSAPI_GetHeapInfo,
		OEM_SYSAPI_GetTotalHeapSize,
		OEM_SYSAPI_GetTotalLeftHeapSize,
		OEM_SYSAPI_CompactHeap
	};

	OEM_SYSAPI_MEM_SetOemFn(&oem_mem);
}
