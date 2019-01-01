/**
  @file UI_Mem.h
  @brief Memory에 관한 API
  @remark Memory에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Mem.c
*/
#ifndef __UI_MEM_H_
#define __UI_MEM_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


atBOOL  atUIAPI_InitMem( atLONG lSysInst, atLONG lParam1, atLONG lParam2 );
atVOID    atUIAPI_FinalMem( NOARGS );

void  * atUIAPI_AllocMem( long size );
void  * atUIAPI_ReallocMem( void *ptr, long size );
int     atUIAPI_FreeMem( void *ptr );

atBOOL  atUIAPI_CheckValidateHeap( NOARGS );
atVOID  atUIAPI_GetHeapInfo( atLPVOID lpParam, void (*pViewFunc)(atLPVOID lpParam, void *addr, int pos, int size, int type) );
atLONG  atUIAPI_GetTotalLeftHeapSize( NOARGS );
atVOID  atUIAPI_CompactHeap( NOARGS );


#ifdef __cplusplus
}
#endif


#endif //  __UI_MEM_H_
