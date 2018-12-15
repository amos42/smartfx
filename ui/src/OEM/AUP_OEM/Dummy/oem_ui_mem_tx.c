#include "A_UIENG.h"
#include "OEM_UI_mem.h"
#include "SYSAPI.h"


atBOOL OEM_UI_InitMem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	//return SYSAPI_InitMem( lSysInst, lInitData1, lInitData2 );
}


void OEM_UI_FinalMem(void)
{
	//SYSAPI_FinalMem();
}


void * OEM_UI_AllocMem( long size )
{
	return SYSAPI_AllocMem( size );
}


void * OEM_UI_ReallocMem( void *ptr, long size )
{
	return SYSAPI_ReallocMem( ptr, size );
}


int OEM_UI_FreeMem( void *ptr )
{
	return SYSAPI_FreeMem( ptr );
}


atBOOL  OEM_UI_CheckValidateHeap( NOARGS )
{
	return SYSAPI_CheckValidateHeap();
}


atVOID  OEM_UI_GetHeapInfo( atLPVOID lpParam, void (*pViewFunc)(atLPVOID lpParam, void *addr, int pos, int size, int type) )
{
	SYSAPI_GetHeapInfo( lpParam, pViewFunc );
}


atLONG  OEM_UI_GetTotalLeftHeapSize( NOARGS )
{
	return SYSAPI_GetTotalLeftHeapSize();
}


atVOID  OEM_UI_CompactHeap( NOARGS )
{
	SYSAPI_CompactHeap();
}

