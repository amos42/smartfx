#include "A_UIENG.h"
#include "OEM_UI_mem.h"
#include "SYSAPI.h"


static atBOOL OEM_UI_InitMem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
// Memory, String, File, Debug, Task, Timer, Input, Sound, GPS는 시스템에서 초기화를 관리한다.
//	return SYSAPI_InitMem( lSysInst, lInitData1, lInitData2 );
	return atTRUE;
}


static void OEM_UI_FinalMem(void)
{
// Memory, String, File, Debug, Task, Timer, Input, Sound, GPS는 시스템에서 초기화를 관리한다.
//	SYSAPI_FinalMem();
}


static void * OEM_UI_AllocMem( long size )
{
	return SYSAPI_AllocMem( size );
}


static void * OEM_UI_ReallocMem( void *ptr, long size )
{
	return SYSAPI_ReallocMem( ptr, size );
}


static int OEM_UI_FreeMem( void *ptr )
{
	return SYSAPI_FreeMem( ptr );
}


static atBOOL  OEM_UI_CheckValidateHeap( NOARGS )
{
	return SYSAPI_CheckValidateHeap();
}


static atVOID  OEM_UI_GetHeapInfo( atLPVOID lpParam, void (*pViewFunc)(atLPVOID lpParam, void *addr, int pos, int size, int type) )
{
	SYSAPI_GetHeapInfo( lpParam, pViewFunc );
}


static atLONG  OEM_UI_GetTotalLeftHeapSize( NOARGS )
{
	return SYSAPI_GetTotalLeftHeapSize();
}


static atVOID  OEM_UI_CompactHeap( NOARGS )
{
	SYSAPI_CompactHeap();
}


void init_uiapi_mem_oem() 
{
	OEM_UI_MEM_T oem_funcs = {
		OEM_UI_InitMem,
		OEM_UI_FinalMem,

		OEM_UI_AllocMem,
		OEM_UI_ReallocMem,
		OEM_UI_FreeMem,

		OEM_UI_CheckValidateHeap,
		OEM_UI_GetHeapInfo,
		OEM_UI_GetTotalLeftHeapSize,
		OEM_UI_CompactHeap
	};

	OEM_UI_Mem_SetOemFn( &oem_funcs );
}

