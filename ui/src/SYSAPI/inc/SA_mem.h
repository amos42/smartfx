//
// UI_Mem.h
//
////////////////////////////////////////////////////////////////

#ifndef __SA_MEM_H_
#define __SA_MEM_H_

#include "SA_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef void * SYS_EXMEM;


atBOOL  SYSAPI_InitMemory( atLONG lSysInst, atLONG lParam1, atLONG lParam2 );
atVOID  SYSAPI_FinalMemory( NOARGS );
atLONG  SYSAPI_GetSystemMemorySize( NOARGS );

void  * SYSAPI_AllocMem( long size );
void  * SYSAPI_ReallocMem( void *ptr, long size );
int     SYSAPI_FreeMem( void *ptr );

SYS_EXMEM SYSAPI_CreateExMem( long ex_type, long size );
atBOOL  SYSAPI_DestroyExMem( SYS_EXMEM hExMem );
atBOOL  SYSAPI_ResizeExMem( SYS_EXMEM hExMem, long size );
void *  SYSAPI_GetExMemPtr( SYS_EXMEM hExMem );
void *  SYSAPI_AllocMemEx( SYS_EXMEM hExMem, long size );
void *  SYSAPI_ReallocMemEx( SYS_EXMEM hExMem, void *ptr, long size );
atBOOL  SYSAPI_FreeMemEx( SYS_EXMEM hExMem, void *ptr );

atBOOL  SYSAPI_CheckValidateHeap( NOARGS );
atVOID  SYSAPI_GetHeapInfo( atLPVOID lpParam, void (*pViewFunc)(atLPVOID lpParam, void *addr, int pos, int size, int type) );
atLONG  SYSAPI_GetTotalHeapSize( NOARGS );
atLONG  SYSAPI_GetTotalLeftHeapSize( NOARGS );
atVOID  SYSAPI_CompactHeap( NOARGS );



#ifdef __cplusplus
}
#endif


#endif //  __SA_MEM_H_
