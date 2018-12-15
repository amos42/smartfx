#include "OEM_SA_Mem.h"
#include "SYSAPI.h"


#ifdef __USES_SYSAPI_MEMORY_


static atBOOL g_bSysMemIntitialized = atFALSE;

static OEM_SYSAPI_MEM_T tOemFn;


atVOID OEM_SYSAPI_MEM_SetOemFn(OEM_SYSAPI_MEM_T *fnOem)
{
	tOemFn = *fnOem;
}


atVOID OEM_SYSAPI_MEM_GetOemFn(OEM_SYSAPI_MEM_T *fnOem)
{
	*fnOem = tOemFn;
}


atBOOL SYSAPI_InitMemory( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( g_bSysMemIntitialized ) return atTRUE;

	if (tOemFn.fnInitMemory != atNULL) {
		g_bSysMemIntitialized = tOemFn.fnInitMemory(lSysInst, lInitData1, lInitData2);
	}

	return g_bSysMemIntitialized;
}


atVOID SYSAPI_FinalMemory( NOARGS )
{
	if( !g_bSysMemIntitialized ) return;

	if (tOemFn.fnFinalMemory != atNULL) {
		tOemFn.fnFinalMemory();
	}

	g_bSysMemIntitialized = atFALSE;
}


atLONG  SYSAPI_GetSystemMemorySize( atVOID )
{
	if( g_bSysMemIntitialized <= 0 ) return 0;
	
	if (tOemFn.fnGetSystemMemorySize != atNULL) {
		return tOemFn.fnGetSystemMemorySize();
	}

	return 0;
}


void * SYSAPI_AllocMem( long size )
{
	if( !g_bSysMemIntitialized ) return atNULL;
	if( size <= 0 ) return atNULL;

	if (tOemFn.fnAllocMem != atNULL) {
		return tOemFn.fnAllocMem(size);
	}

	return atNULL;
}


void * SYSAPI_ReallocMem( void *ptr, long size )
{
	if( !g_bSysMemIntitialized ) return atNULL;

	if (tOemFn.fnReallocMem != atNULL) {
		return tOemFn.fnReallocMem(ptr, size);
	}

	return atNULL;
}


int SYSAPI_FreeMem( void *ptr )
{
	if( !g_bSysMemIntitialized ) return 0;
	if( ptr == atNULL ) return 0;

	if (tOemFn.fnFreeMem != atNULL) {
		return tOemFn.fnFreeMem(ptr);
	}

	return 0;
}


SYS_EXMEM SYSAPI_CreateExMem( long ex_type, long size )
{
	if( !g_bSysMemIntitialized ) return atNULL;
	if( size <= 0 ) return atNULL;

	if (tOemFn.fnCreateExMem != atNULL) {
		return (SYS_EXMEM)tOemFn.fnCreateExMem( ex_type, size );
	}

	return atNULL;
}


atBOOL SYSAPI_DestroyExMem( SYS_EXMEM hExMem )
{
	if( !g_bSysMemIntitialized ) return atFALSE;
	if( hExMem == atNULL ) return atFALSE;

	if (tOemFn.fnDestroyExMem != atNULL) {
		return tOemFn.fnDestroyExMem( (OEM_SYS_EXMEM)hExMem );
	}

	return atFALSE;
}


atBOOL SYSAPI_ResizeExMem( SYS_EXMEM hExMem, long size )
{
	if( !g_bSysMemIntitialized ) return atFALSE;
	if( hExMem == atNULL ) return atFALSE;

	if (tOemFn.fnResizeExMem != atNULL) {
		return tOemFn.fnResizeExMem( (OEM_SYS_EXMEM)hExMem, size );
	}

	return atFALSE;
}


void * SYSAPI_GetExMemPtr( SYS_EXMEM hExMem )
{
	if( !g_bSysMemIntitialized ) return atNULL;
	if( hExMem == atNULL ) return atNULL;

	if (tOemFn.fnGetExMemPtr != atNULL) {
		return tOemFn.fnGetExMemPtr( (OEM_SYS_EXMEM)hExMem );
	}

	return atNULL;
}


void  * SYSAPI_AllocMemEx( SYS_EXMEM hExMem, long size )
{
	if( !g_bSysMemIntitialized ) return atNULL;
	if( size <= 0 ) return atNULL;

	if (hExMem == atNULL) {
		return SYSAPI_AllocMem(size);
	} else {
		if (tOemFn.fnAllocMemEx != atNULL) {
			return tOemFn.fnAllocMemEx((OEM_SYS_EXMEM)hExMem, size);
		}
	}

	return atNULL;
}


void  * SYSAPI_ReallocMemEx( SYS_EXMEM hExMem, void *ptr, long size )
{
	if( !g_bSysMemIntitialized ) return atNULL;

	if (hExMem == atNULL) {
		return SYSAPI_ReallocMem(ptr, size);
	} else {
		if (tOemFn.fnReallocMemEx != atNULL) {
			return tOemFn.fnReallocMemEx((OEM_SYS_EXMEM)hExMem, ptr, size);
		}
	}

	return atNULL;
}


atBOOL     SYSAPI_FreeMemEx( SYS_EXMEM hExMem, void *ptr )
{
	if( !g_bSysMemIntitialized ) return atFALSE;

	if (hExMem == atNULL) {
		SYSAPI_FreeMem(ptr);
	} else {
		if (tOemFn.fnFreeMemEx != atNULL) {
			return tOemFn.fnFreeMemEx((OEM_SYS_EXMEM)hExMem, ptr);
		}
	}

	return atFALSE;
}


atBOOL  SYSAPI_CheckValidateHeap( NOARGS )
{
	if( !g_bSysMemIntitialized ) return atFALSE;

	if (tOemFn.fnCheckValidateHeap != atNULL) {
		return tOemFn.fnCheckValidateHeap();
	}

	return atFALSE;
}


atVOID SYSAPI_GetHeapInfo( atLPVOID lpParam, void (*pViewFunc)(atLPVOID lpParam, void *addr, int pos, int size, int type) )
{
	if( !g_bSysMemIntitialized ) return;

	if (tOemFn.fnGetHeapInfo != atNULL) {
		tOemFn.fnGetHeapInfo(lpParam, pViewFunc);
	}
}


atLONG SYSAPI_GetTotalLeftHeapSize( NOARGS )
{
	if( !g_bSysMemIntitialized ) return 0;

	if (tOemFn.fnGetTotalLeftHeapSize != atNULL) {
		return tOemFn.fnGetTotalLeftHeapSize();
	}

	return 0;
}


atLONG SYSAPI_GetTotalHeapSize( NOARGS )
{
	if( !g_bSysMemIntitialized ) return 0;

	if (tOemFn.fnGetTotalHeapSize != atNULL) {
		return tOemFn.fnGetTotalHeapSize();
	}

	return 0;
}

atVOID SYSAPI_CompactHeap( NOARGS )
{
	if( !g_bSysMemIntitialized ) return;

	if (tOemFn.fnCompactHeap != atNULL) {
		tOemFn.fnCompactHeap();
	}
}


#endif
