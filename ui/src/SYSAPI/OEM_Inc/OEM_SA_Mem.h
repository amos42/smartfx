//
// OEM_SYSAPI_Mem.h
//
////////////////////////////////////////////////////////////////

#ifndef __OEM_SA_MEM_H_
#define __OEM_SA_MEM_H_


#include "SA_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef void * OEM_SYS_EXMEM;

typedef atBOOL	FN_OEM_SYSAPI_InitMemory(atLONG lSysInst, atLONG lInitData1, atLONG lInitData2);
typedef void	FN_OEM_SYSAPI_FinalMemory(NOARGS);
typedef atLONG  FN_OEM_SYSAPI_GetSystemMemorySize(NOARGS);

typedef void  * FN_OEM_SYSAPI_AllocMem(long size);
typedef void  * FN_OEM_SYSAPI_ReallocMem(void *ptr, long size);
typedef int		FN_OEM_SYSAPI_FreeMem(void *ptr);

typedef OEM_SYS_EXMEM FN_OEM_SYSAPI_CreateExMem( long ex_type, long size );
typedef atBOOL        FN_OEM_SYSAPI_DestroyExMem( OEM_SYS_EXMEM hOemExMem );
typedef atBOOL        FN_OEM_SYSAPI_ResizeExMem( OEM_SYS_EXMEM hOemExMem, long size );
typedef void *		  FN_OEM_SYSAPI_GetExMemPtr(OEM_SYS_EXMEM hOemExMem);
typedef OEM_SYS_EXMEM FN_OEM_SYSAPI_AllocMemEx(OEM_SYS_EXMEM hOemExMem, long size);
typedef OEM_SYS_EXMEM FN_OEM_SYSAPI_ReallocMemEx(OEM_SYS_EXMEM hOemExMem, void *ptr, long size);
typedef atBOOL		  FN_OEM_SYSAPI_FreeMemEx(OEM_SYS_EXMEM hOemExMem, void *ptr);

typedef atBOOL  FN_OEM_SYSAPI_CheckValidateHeap(NOARGS);
typedef atVOID  FN_OEM_SYSAPI_GetHeapInfo(atLPVOID lpParam, void(*pViewFunc)(atLPVOID lpParam, void *addr, int pos, int size, int type));
typedef atLONG  FN_OEM_SYSAPI_GetTotalHeapSize(NOARGS);
typedef atLONG  FN_OEM_SYSAPI_GetTotalLeftHeapSize(NOARGS);
typedef atVOID  FN_OEM_SYSAPI_CompactHeap(NOARGS);


typedef struct tagOEM_SYSAPI_MEM_T {
	FN_OEM_SYSAPI_InitMemory *fnInitMemory;
	FN_OEM_SYSAPI_FinalMemory *fnFinalMemory;
	FN_OEM_SYSAPI_GetSystemMemorySize *fnGetSystemMemorySize;

	FN_OEM_SYSAPI_AllocMem *fnAllocMem;
	FN_OEM_SYSAPI_ReallocMem *fnReallocMem;
	FN_OEM_SYSAPI_FreeMem *fnFreeMem;

	FN_OEM_SYSAPI_CreateExMem *fnCreateExMem;
	FN_OEM_SYSAPI_DestroyExMem *fnDestroyExMem;
	FN_OEM_SYSAPI_ResizeExMem *fnResizeExMem;
	FN_OEM_SYSAPI_GetExMemPtr *fnGetExMemPtr;
	FN_OEM_SYSAPI_AllocMemEx *fnAllocMemEx;
	FN_OEM_SYSAPI_ReallocMemEx *fnReallocMemEx;
	FN_OEM_SYSAPI_FreeMemEx *fnFreeMemEx;

	FN_OEM_SYSAPI_CheckValidateHeap *fnCheckValidateHeap;
	FN_OEM_SYSAPI_GetHeapInfo *fnGetHeapInfo;
	FN_OEM_SYSAPI_GetTotalHeapSize *fnGetTotalHeapSize;
	FN_OEM_SYSAPI_GetTotalLeftHeapSize *fnGetTotalLeftHeapSize;
	FN_OEM_SYSAPI_CompactHeap *fnCompactHeap;
} OEM_SYSAPI_MEM_T;


atVOID	OEM_SYSAPI_MEM_SetOemFn( OEM_SYSAPI_MEM_T *fnOem );
atVOID	OEM_SYSAPI_MEM_GetOemFn( OEM_SYSAPI_MEM_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif 
