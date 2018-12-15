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
#include <e32std.h>
#include <hal.h>


atBOOL OEM_SYSAPI_InitMem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
//    return atVD_MEM_Initialize( lSysInst, (atVOID *)lInitData1, lInitData2 );
//	if( (void *)lInitData1 == atNULL ) return atFALSE;
// 	atDALLOC_InitGlobalHeap( (void *)(((unsigned long)lInitData1 + 3) / 4 * 4), lInitData2/4*4 - 4 );	
	return atTRUE;
}


void OEM_SYSAPI_FinalMem(void)
{
//    atVD_MEM_Finalize();
}


void * OEM_SYSAPI_AllocMem( long size )
{
//    return atVD_MEM_AllocMem( size );
//	return atDALLOC_AllocGlobalMem( size );
return malloc( size );
}


void * OEM_SYSAPI_ReallocMem( void *ptr, long size )
{
//    return atVD_MEM_ReallocMem( ptr, size );
//	return atDALLOC_ReallocGlobalMem( ptr, size );
return realloc( ptr, size );
}


int OEM_SYSAPI_FreeMem( void *ptr )
{
//    atVD_MEM_FreeMem( ptr );
//	atDALLOC_FreeGlobalMem( ptr );
	free( ptr );
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
	TInt iFreeRamSize;
	User::LeaveIfError(HAL::Get(HALData::EMemoryRAMFree, iFreeRamSize));
	
	return (atLONG)iFreeRamSize;
}


atLONG  OEM_SYSAPI_GetTotalLeftHeapSize( NOARGS )
{
	TInt iFreeHeapSize;
	TInt iBiggestBlockSize;
	iFreeHeapSize = User::Available(iBiggestBlockSize);
		
	return (atLONG)iFreeHeapSize;
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
