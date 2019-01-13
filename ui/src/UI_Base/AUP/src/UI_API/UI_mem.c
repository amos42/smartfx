/**
  @file UI_mem.c
  @brief Memory에 관한 API
  @remark Memory에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_mem.h
*/
#include "OEM_UI_Mem.h"
#include "UI_API/UI_API.h"


static OEM_UI_MEM_T tOemFn;

atVOID	OEM_UI_Mem_SetOemFn( OEM_UI_MEM_T *fnOem )
{
	tOemFn = *fnOem;
}


atVOID	OEM_UI_Mem_GetOemFn( OEM_UI_MEM_T *fnOem )
{
	*fnOem = tOemFn;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL atUIAPI_InitMem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( tOemFn.fnInitMem != atNULL ){
		return tOemFn.fnInitMem( lSysInst, lInitData1, lInitData2 );
	}

	return atFALSE;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_FinalMem( NOARGS )
{
	if( tOemFn.fnFinalMem != atNULL ){
		tOemFn.fnFinalMem();
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
void * atUIAPI_AllocMem( long size )
{
	if( tOemFn.fnAllocMem != atNULL ){
		return tOemFn.fnAllocMem( size );
	}

	return atNULL;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
void * atUIAPI_ReallocMem( void *ptr, long size )
{
	if( tOemFn.fnReallocMem != atNULL ){
		return tOemFn.fnReallocMem( ptr, size );
	}

	return atNULL;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int atUIAPI_FreeMem( void *ptr )
{
	if( tOemFn.fnFreeMem != atNULL ){
		return tOemFn.fnFreeMem( ptr );
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL  atUIAPI_CheckValidateHeap( NOARGS )
{
	if( tOemFn.fnCheckValidateHeap != atNULL ){
		return tOemFn.fnCheckValidateHeap();
	}

	return atFALSE;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_GetHeapInfo( atLPVOID lpParam, void (*pViewFunc)(atLPVOID lpParam, void *addr, int pos, int size, int type) )
{
	if( tOemFn.fnGetHeapInfo != atNULL ){
		tOemFn.fnGetHeapInfo( lpParam, pViewFunc );
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atLONG atUIAPI_GetTotalLeftHeapSize( NOARGS )
{	
	if( tOemFn.fnGetTotalLeftHeapSize != atNULL ){
		return tOemFn.fnGetTotalLeftHeapSize();
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID  atUIAPI_CompactHeap( NOARGS )
{
	if( tOemFn.fnCompactHeap != atNULL ){
		tOemFn.fnCompactHeap();
	}
}

