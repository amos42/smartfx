#include "OEM_SA_mem.h"
#include "SYSAPI.h"


#ifdef _DEBUG
#ifndef UNDER_CE			
#define __USES_DALLOC_          // <- 메모리 문제 간헐적 발생으로 사용하지 않도록 변경
#define __USE_SHARED_HEAP_MEMORY_
#endif
#endif


#ifdef __USES_DALLOC_
#include "A_DALLOC.h"
#include <windows.h>
#else
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#endif


#ifdef __USES_DALLOC_
#ifdef __USE_SHARED_HEAP_MEMORY_
#include <tchar.h>
#define SYS_HEAP_BUFFER _TEXT("SYS_HEAP_BUFFER")

static HANDLE g_hSharedMem;
static atVOID* g_baseAddress;
#else
static HGLOBAL g_heap_handle;
#endif
static CRITICAL_SECTION g_MemCrSection;

static void _LockMem(void)
{
	EnterCriticalSection( &g_MemCrSection );
}

static void _UnlockMem(void)
{
	LeaveCriticalSection( &g_MemCrSection );
}
#endif


static atBOOL OEM_SYSAPI_InitMemory( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
#ifdef __USES_DALLOC_
#ifdef __USE_SHARED_HEAP_MEMORY_
	atLONG *ptr;
#endif

	if( lInitData2 <= 0 ) return atFALSE;

#ifdef __USE_SHARED_HEAP_MEMORY_
	g_hSharedMem = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, lInitData2 + sizeof(long)*4, SYS_HEAP_BUFFER );	
	g_baseAddress = MapViewOfFile( g_hSharedMem, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
	ptr = (atLONG *)g_baseAddress;
	ptr[0] = lInitData2;
	ptr[1] = 0;
	ptr[2] = 0;
	ptr[3] = 0;
	lInitData1 = (long)(atVOID *)((atBYTE *)ptr + sizeof(long)*4);	
#else
	if( (void *)lInitData1 == atNULL ){
		g_heap_handle = GlobalAlloc( GMEM_MOVEABLE, lInitData2 );
		lInitData1 = (atLONG)GlobalLock(g_heap_handle);
	}
#endif

	InitializeCriticalSection( &g_MemCrSection );

	_LockMem();
 	atDALLOC_InitGlobalHeap( (void *)(((unsigned long)lInitData1 + 3) / 4 * 4), lInitData2/4*4 - 4 );	
	_UnlockMem();

#endif

	return atTRUE;
}


static void OEM_SYSAPI_FinalMemory(void)
{
#ifdef __USES_DALLOC_
	_LockMem();
   	atDALLOC_ResetGlobalHeap();
	_UnlockMem();

	DeleteCriticalSection( &g_MemCrSection );
	
#ifdef __USE_SHARED_HEAP_MEMORY_
	UnmapViewOfFile(g_baseAddress);
	g_baseAddress = atNULL;
	CloseHandle( g_hSharedMem );
	g_hSharedMem = atNULL;
#else
	if( g_heap_handle != NULL ){
		GlobalUnlock(g_heap_handle);
		GlobalFree(g_heap_handle);
		g_heap_handle = NULL;
	}
#endif	
#endif
}


static atLONG  OEM_SYSAPI_GetSystemMemorySize(atVOID)
{
	MEMORYSTATUS ms;

	ms.dwLength = sizeof(ms);
	GlobalMemoryStatus(&ms);

	return (int)ms.dwAvailVirtual;
}


static void * OEM_SYSAPI_AllocMem( long size )
{
#ifdef __USES_DALLOC_
	void *p;

//if( !OEM_SYSAPI_CheckValidateHeap() ){
//	return atNULL;
//}

	_LockMem();
	p = atDALLOC_AllocGlobalMem( size );
	_UnlockMem();	

//if( p == atNULL ){
//	return atNULL;
//}

	return p;
#else
	return (void *)malloc( size );
#endif
}


static void * OEM_SYSAPI_ReallocMem( void *ptr, long size )
{
#ifdef __USES_DALLOC_
	void *p;

//if( !OEM_SYSAPI_CheckValidateHeap() ){
//	return atNULL;
//}

	_LockMem();
	p = atDALLOC_ReallocGlobalMem( ptr, size );
	_UnlockMem();

//if( p == atNULL ){
//	return atNULL;
//}

	return p;
#else
	return (void *)realloc( ptr, size );
#endif
}


static int OEM_SYSAPI_FreeMem( void *ptr )
{
#ifdef __USES_DALLOC_
//if( !OEM_SYSAPI_CheckValidateHeap() ){
//	return atNULL;
//}

	_LockMem();
	atDALLOC_FreeGlobalMem( ptr );
	_UnlockMem();

//if( !OEM_SYSAPI_CheckValidateHeap() ){
//	return atNULL;
//}

#else
	free( ptr );
#endif
	return 0;
}

/*
typedef struct __EXMEM {
	long ex_type;
	HANDLE hMem;
	void *ptr;
} EXMEM;

static OEM_SYS_EXMEM * OEM_SYSAPI_CreateExMem( long ex_type, long size )
{
	if( ex_type == 1 ){
		EXMEM *mem = (EXMEM *)OEM_SYSAPI_AllocMem( sizeof(EXMEM) );
		if( mem == atNULL ) return atNULL;
		
		mem->hMem = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, SYS_HEAP_BUFFER );	
		mem->ex_type = ex_type;
		mem->ptr = atNULL;
		return (OEM_SYS_EXMEM)mem;
	}
	
	return (OEM_SYS_EXMEM)atNULL;
}


static OEM_SYS_EXMEM * OEM_SYSAPI_ResizeExMem( OEM_SYS_EXMEM ExMem, long size )
{
	EXMEM *mem = (EXMEM *)ExMem;
	if( mem == atNULL ) return atNULL;
	
	if( mem->hMem == atNULL ){
		return OEM_SYSAPI_AllocExMem( mem->ex_type, size );
	}

	if( mem>ex_type == 1 ){
		g_hSharedMem = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, SYS_HEAP_BUFFER );	
		g_baseAddress = MapViewOfFile( g_hSharedMem, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
		return (OEM_SYS_EXMEM)mem;
	}

	return (OEM_SYS_EXMEM)atNULL;
}


static int     OEM_SYSAPI_DestoryExMem( OEM_SYS_EXMEM ExMem )
{
	EXMEM *mem = (EXMEM *)ExMem;
	if( mem == atNULL ) return -1;
	
	UnmapViewOfFile(g_baseAddress);
	g_baseAddress = atNULL;
	CloseHandle( g_hSharedMem );
	g_hSharedMem = atNULL;
	return -1;
}


static void  * OEM_SYSAPI_GetExMemPtr( OEM_SYS_EXMEM ExMem )
{
	g_hSharedMem = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, lInitData2 + sizeof(long)*4, SYS_HEAP_BUFFER );	
	g_baseAddress = MapViewOfFile( g_hSharedMem, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
	return atNULL;
}
*/

static atBOOL  OEM_SYSAPI_CheckValidateHeap( NOARGS )
{
#ifdef __USES_DALLOC_
	atBOOL r;

	_LockMem();
	r = atDALLOC_CheckGlobalValidateHeap();
	_UnlockMem();

	return r;
#else
	return atTRUE;
#endif
}


static atVOID  OEM_SYSAPI_GetHeapInfo( atLPVOID lpParam, void (*pViewFunc)(atLPVOID lpParam, void *addr, int pos, int size, int type) )
{
#ifdef __USES_DALLOC_
	_LockMem();
	atDALLOC_GetGlobalHeapInfo( lpParam, pViewFunc );
	_UnlockMem();
#endif
}

static atLONG OEM_SYSAPI_GetTotalHeapSize( NOARGS )
{
#ifdef __USES_DALLOC_
	atLONG r;

	_LockMem();
	r = atDALLOC_GetGlobalHeapSize();
	_UnlockMem();

	return r;
#else
	MEMORYSTATUS ms;

	ms.dwLength = sizeof(ms);
	GlobalMemoryStatus(&ms);

	return (int)ms.dwAvailVirtual;
#endif
}

static atLONG  OEM_SYSAPI_GetTotalLeftHeapSize( NOARGS )
{
#ifdef __USES_DALLOC_
	atLONG r;

	_LockMem();
	r = atDALLOC_GetGlobalTotalLeftSize();
	_UnlockMem();

	return r;
#else
	MEMORYSTATUS ms;

	ms.dwLength = sizeof(ms);
	GlobalMemoryStatus(&ms);

	return (int)ms.dwAvailVirtual;
#endif
}


static atVOID  OEM_SYSAPI_CompactHeap( NOARGS )
{
#ifdef __USES_DALLOC_
	_LockMem();
	atDALLOC_CompactGlobalHeap();
	_UnlockMem();
#endif
}



void init_sysapi_mem_oem() 
{
	OEM_SYSAPI_MEM_T oem_funcs = {
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
		atNULL,
		atNULL,
		atNULL,

		OEM_SYSAPI_CheckValidateHeap,
		OEM_SYSAPI_GetHeapInfo,
		OEM_SYSAPI_GetTotalHeapSize,
		OEM_SYSAPI_GetTotalLeftHeapSize,
		OEM_SYSAPI_CompactHeap
	};

	OEM_SYSAPI_MEM_SetOemFn( &oem_funcs );
}
