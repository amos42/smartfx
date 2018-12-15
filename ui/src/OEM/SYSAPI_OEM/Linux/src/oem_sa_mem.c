#include "OEM_SA_mem.h"
#include "SYSAPI.h"


#define __USES_DALLOC_


#ifdef __USES_DALLOC_
#include "A_DALLOC.h"
#endif


static atVOID* g_baseAddress;
static int g_GlobalHeapSize;


static void _LockMem(void)
{

}

static void _UnlockMem(void)
{
}


atBOOL OEM_SYSAPI_InitMemory( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
#ifdef __USES_DALLOC_
  g_GlobalHeapSize = lInitData2/4*4 - 4;
   
	_LockMem();
 	atDALLOC_InitGlobalHeap( (void *)(((unsigned long)lInitData1 + 3) / 4 * 4), g_GlobalHeapSize );	
	_UnlockMem();

	return atTRUE;
#else
	return atFALSE;
#endif  
}


void OEM_SYSAPI_FinalMemory(void)
{
#ifdef __USES_DALLOC_
	_LockMem();
  atDALLOC_ResetGlobalHeap();
	_UnlockMem();
#endif
}


atLONG  OEM_SYSAPI_GetSystemMemorySize(atVOID)
{
	return g_GlobalHeapSize;
}


void * OEM_SYSAPI_AllocMem( long size )
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
	return atNULL;
#endif
}


void * OEM_SYSAPI_ReallocMem( void *ptr, long size )
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
	return atNULL;
#endif
}


int OEM_SYSAPI_FreeMem( void *ptr )
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

#endif
	return 0;
}


atBOOL  OEM_SYSAPI_CheckValidateHeap( NOARGS )
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


atVOID  OEM_SYSAPI_GetHeapInfo( atLPVOID lpParam, void (*pViewFunc)(atLPVOID lpParam, void *addr, int pos, int size, int type) )
{
#ifdef __USES_DALLOC_
	_LockMem();
	atDALLOC_GetGlobalHeapInfo( lpParam, pViewFunc );
	_UnlockMem();
#endif
}

atLONG OEM_SYSAPI_GetTotalHeapSize()
{
#ifdef __USES_DALLOC_
	atLONG r;

	_LockMem();
	r = atDALLOC_GetGlobalHeapSize();
	_UnlockMem();

	return r;
#else
  return 0;
#endif
}

atLONG  OEM_SYSAPI_GetTotalLeftHeapSize( NOARGS )
{
#ifdef __USES_DALLOC_
	atLONG r;

	_LockMem();
	r = atDALLOC_GetGlobalTotalLeftSize();
	_UnlockMem();

	return r;
#else
  return 0;
#endif
}


atVOID  OEM_SYSAPI_CompactHeap( NOARGS )
{
#ifdef __USES_DALLOC_
	_LockMem();
	atDALLOC_CompactGlobalHeap();
	_UnlockMem();
#endif
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
