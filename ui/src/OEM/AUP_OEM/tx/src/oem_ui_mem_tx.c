#include "A_UIENG.h"
#include "OEM_UI_mem.h"
#include "SYSAPI.h"


atBOOL OEM_UI_InitMem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return SYSAPI_InitMem( lSysInst, lInitData1, lInitData2 );
}


void OEM_UI_FinalMem(void)
{
	SYSAPI_FinalMem();
}


#define USE_NAVI_LOCAL_POOL 

void * OEM_UI_AllocMem( long size )
{
#ifdef USE_NAVI_LOCAL_POOL
	return SYSAPI_AllocMem(size );
#else
	return SYSAPI_AllocMemEx(0,size);
#endif
}

void * OEM_UI_AllocMem_dbg( long size, atCHAR *filename, atINT line )
{
#ifdef USE_NAVI_LOCAL_POOL
	return SYSAPI_AllocMem_dbg(size, filename, line );
#else
	return SYSAPI_AllocMemEx_dbg(0,size, filename, line );
#endif
}

void * OEM_UI_ReallocMem( void *ptr, long size )
{
#ifdef USE_NAVI_LOCAL_POOL
	return SYSAPI_ReallocMem( ptr, size );
#else
	return SYSAPI_ReallocMemEx( ptr, size );
#endif
}


int OEM_UI_FreeMem( void *ptr )
{
#ifdef USE_NAVI_LOCAL_POOL
	return SYSAPI_FreeMem( ptr );	
#else
	return SYSAPI_FreeMemEx( ptr );
#endif
}


atBOOL  OEM_UI_CheckValidateHeap( NOARGS )
{
	return SYSAPI_CheckValidateHeap();
}


atVOID  OEM_UI_GetHeapInfo( atLPVOID lpParam, void (*pViewFunc)(atLPVOID lpParam, void *addr, int pos, int size, int type) )
{
/**
	SYSAPI_GetHeapInfo( lpParam, pViewFunc );
**/	
}


atLONG  OEM_UI_GetTotalLeftHeapSize( NOARGS )
{
	return SYSAPI_GetTotalLeftHeapSize();
}


atVOID  OEM_UI_CompactHeap( NOARGS )
{
	SYSAPI_CompactHeap();
}

