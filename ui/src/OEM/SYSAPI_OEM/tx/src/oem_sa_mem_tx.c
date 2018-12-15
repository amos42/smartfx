//
// sound_wipic.c
//
////////////////////////////////////////////////////////////////

#include "SA_Types.h"
#include "OEM_SA_mem.h"
//#include "MemorySizeChecker.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "HceDirect.h"


atBOOL OEM_SYSAPI_InitMem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
//    return atVD_MEM_Initialize( lSysInst, (atVOID *)lInitData1, lInitData2 );
//	if( (void *)lInitData1 == atNULL ) return atFALSE;
// 	atDALLOC_InitGlobalHeap( (void *)(((unsigned long)lInitData1 + 3) / 4 * 4), lInitData2/4*4 - 4 );	
	return atTRUE;
}


void OEM_SYSAPI_FinalMem(void)
{
	// to-do
//    atVD_MEM_Finalize();
}


void * OEM_SYSAPI_AllocMem( long size )
{
	//fprintf(stderr, "OEM_SYSAPI_AllocMem %d bytes\n", size);
	return DirectPoolAlloc(size);
}


void * OEM_SYSAPI_ReallocMem( void *ptr, long size )
{	
	return DirectPoolRealloc(ptr, size);
}


int OEM_SYSAPI_FreeMem( void *ptr )
{
	DirectPoolFree(ptr);
	return 0;
}


atBOOL  OEM_SYSAPI_CheckValidateHeap(void)
{
	return atTRUE;
}


atVOID  OEM_SYSAPI_GetHeapInfo( atLPVOID lpParam, void (*pViewFunc)(atLPVOID lpParam, void *addr, int pos, int size, int type) )
{
}


atLONG  OEM_SYSAPI_GetTotalHeapSize()
{
	// to-do

	return 0;
}


atLONG  OEM_SYSAPI_GetTotalLeftHeapSize( NOARGS )
{
	// to-do
	return 0;
}


atVOID  OEM_SYSAPI_CompactHeap( NOARGS )
{
//	atDALLOC_CompactGlobalHeap();
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
