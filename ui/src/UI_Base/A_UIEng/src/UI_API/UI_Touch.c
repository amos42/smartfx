/**
  @file UI_Touch.c
  @brief Touch/Mouse�� ���� API
  @remark Touch/Mouse�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
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
 @brief �Է� ����̽��� �ʱ�ȭ �Ѵ�. (���ο�)
 
 �Է� ����̽��� �ʱ�ȭ �Ѵ�.

 @param lSysInst [in] : UI������ �ν��Ͻ� (OEM ���޿�. ���ο�)
 @param lInitData1, lInitData2 [in] : �ʱ�ȭ �Ķ���� (OEM ���޿�. ���ο�)
 @return �����ϸ� atTRUE, �����ϸ� atFALSE
 @remark ���ο����� ���ȴ�.
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
 @return ����
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
 @return ����
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
 @return ����
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
 @return ����
*/
int atUIAPI_ConvVPtr2OemPtr( int *x, int *y )
{
	if( tOemFn.fnConvVPtr2OemPtr != atNULL ){
		return tOemFn.fnConvVPtr2OemPtr( x, y );
	}

	return 0;
}
