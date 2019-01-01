/**
  @file OEM_UI_Mem.h
  @brief Memory에 관한 포팅 OEM API 선언
  @remark Memory에 관한 포팅 OEM API 선언
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Mem.h
*/
#ifndef __OEM_UI_MEM_H_
#define __OEM_UI_MEM_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atBOOL	FN_OEM_UI_InitMem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef void	FN_OEM_UI_FinalMem(void);

typedef void  * FN_OEM_UI_AllocMem( long size );
typedef void  * FN_OEM_UI_ReallocMem( void *ptr, long size );
typedef int		FN_OEM_UI_FreeMem( void *ptr );

typedef atBOOL  FN_OEM_UI_CheckValidateHeap( NOARGS );
typedef atVOID  FN_OEM_UI_GetHeapInfo( atLPVOID lpParam, void (*pViewFunc)(atLPVOID lpParam, void *addr, int pos, int size, int type) );
typedef atLONG  FN_OEM_UI_GetTotalLeftHeapSize( NOARGS );
typedef atVOID  FN_OEM_UI_CompactHeap( NOARGS );


typedef struct tagOEM_UI_MEM {
	FN_OEM_UI_InitMem				*fnInitMem;
	FN_OEM_UI_FinalMem				*fnFinalMem;

	FN_OEM_UI_AllocMem				*fnAllocMem;
	FN_OEM_UI_ReallocMem			*fnReallocMem;
	FN_OEM_UI_FreeMem				*fnFreeMem;

	FN_OEM_UI_CheckValidateHeap		*fnCheckValidateHeap;
	FN_OEM_UI_GetHeapInfo			*fnGetHeapInfo;
	FN_OEM_UI_GetTotalLeftHeapSize	*fnGetTotalLeftHeapSize;
	FN_OEM_UI_CompactHeap			*fnCompactHeap;
} OEM_UI_MEM_T;


atVOID	OEM_UI_Mem_SetOemFn( OEM_UI_MEM_T *fnOem );
atVOID	OEM_UI_Mem_GetOemFn( OEM_UI_MEM_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif 
