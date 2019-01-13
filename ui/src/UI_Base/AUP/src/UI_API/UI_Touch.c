/**
  @file UI_Touch.c
  @brief Touch/Mouse에 관한 API
  @remark Touch/Mouse에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Touch.h
*/
#include "UI_API/UI_API.h"
#include "OEM_UI_Touch.h"
#include "UI_Kern.h"


static OEM_UI_TOUCH_T tOemFn;

atVOID	OEM_UI_Touch_SetOemFn( OEM_UI_TOUCH_T *fnOem )
{
	tOemFn = *fnOem;
}


atVOID	OEM_UI_Touch_GetOemFn( OEM_UI_TOUCH_T *fnOem )
{
	*fnOem = tOemFn;
}


/** 
 @brief 입력 디바이스를 초기화 한다. (내부용)
 
 입력 디바이스를 초기화 한다.

 @param lSysInst [in] : UI엔진의 인스턴스 (OEM 전달용. 내부용)
 @param lInitData1, lInitData2 [in] : 초기화 파라미터 (OEM 전달용. 내부용)
 @return 성공하면 atTRUE, 실패하면 atFALSE
 @remark 내부용으로 사용된다.
*/
atBOOL atUIAPI_InitTouchDev( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
    atBOOL r;
	if( tOemFn.fnInitTouchDev != atNULL ){
		r =  tOemFn.fnInitTouchDev( lSysInst, lInitData1, lInitData2 );
	} else {
		r = atFALSE;
	}

    if( !r ) return atFALSE;

    return r;    
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
void atUIAPI_FinalTouchDev(void)
{
	if( tOemFn.fnFinalTouchDev != atNULL ){
		tOemFn.fnFinalTouchDev();
	}
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL atUIAPI_GetMousePos( int *x, int *y )
{
	if( tOemFn.fnGetMousePos != atNULL ){
		return tOemFn.fnGetMousePos( x, y );
	}

	return atFALSE;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int atUIAPI_ConvOEMPtr2VPtr( int *x, int *y )
{
	if( tOemFn.fnConvOEMPtr2VPtr != atNULL ){
		return tOemFn.fnConvOEMPtr2VPtr( x, y );
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int atUIAPI_ConvVPtr2OemPtr( int *x, int *y )
{
	if( tOemFn.fnConvVPtr2OemPtr != atNULL ){
		return tOemFn.fnConvVPtr2OemPtr( x, y );
	}

	return 0;
}
