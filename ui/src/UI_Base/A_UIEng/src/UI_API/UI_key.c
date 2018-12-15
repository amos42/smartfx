/**
  @file UI_Key.c
  @brief Key�� ���� API
  @remark Key�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see UI_Key.h
*/
#include "UI_API/UI_API.h"
#include "OEM_UI_Key.h"
#include "UI_Kern.h"


#define LONGKEY_INTERVAL		500//1500
#define KEY_REPEAT_DELAY		300//150
#define KEY_REPEAT_INTERVAL		100//150


static atBOOL g_bUseKeyTimer = atFALSE;
static atHTIMER g_hKeyTimer = atNULL;
static atBOOL g_bActivateKeyTimer = atFALSE;
static atBOOL g_bIsFirstRepeat = atFALSE;
static atBOOL g_bIsLongKey = atFALSE;
static unsigned long g_nKeyPressedTime = 0;


static void cbKeyTimer(atHTIMER hTimer, void *param)
{
	if( !g_bActivateKeyTimer ) return;
	
	atKERNEL_KeyPress( (int)param );

	if( g_bIsFirstRepeat ){
		g_bIsFirstRepeat = atFALSE;
		atUIAPI_EnableTimer( hTimer, atFALSE );
		atUIAPI_SetTimerInterval( hTimer, KEY_REPEAT_INTERVAL );
		atUIAPI_EnableTimer( hTimer, atTRUE );
	}
	
	if( g_bIsLongKey ){
		if( (atUIAPI_GetCurrTime() - g_nKeyPressedTime) >= LONGKEY_INTERVAL ){
			atKERNEL_LongKeyDown( (int)param );
			g_bIsLongKey = atFALSE;
		}
	}
}


static OEM_UI_KEY_T tOemFn;

atVOID	OEM_UI_Key_SetOemFn( OEM_UI_KEY_T *fnOem )
{
	tOemFn = *fnOem;
}


atVOID	OEM_UI_Key_GetOemFn( OEM_UI_KEY_T *fnOem )
{
	*fnOem = tOemFn;
}


/** 
 @brief �Է� ����̽��� �ʱ�ȭ �Ѵ�. (���ο�)
 
 ���� �ý����� �ʱ�ȭ �Ѵ�.

 @param lSysInst [in] : UI������ �ν��Ͻ� (OEM ���޿�. ���ο�)
 @param lInitData1, lInitData2 [in] : �ʱ�ȭ �Ķ���� (OEM ���޿�. ���ο�)
 @return �����ϸ� atTRUE, �����ϸ� atFALSE
 @remark ���ο����� ���ȴ�.
*/
atBOOL atUIAPI_InitKeypad( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
    atBOOL r;
    r =  tOemFn.fnInitInputDev( lSysInst, lInitData1, lInitData2 );
    if( !r ) return atFALSE;

    return r;    
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
void atUIAPI_FinalKeypad(void)
{
    if( g_bUseKeyTimer ){
		atUIAPI_UseKeyTimer( atFALSE );
    }
	
    tOemFn.fnFinalInputDev();
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
void atUIAPI_UseKeyTimer( atBOOL bUseKeyTimer )
{
	if( bUseKeyTimer == g_bUseKeyTimer ) return;
	
	if( bUseKeyTimer ){
       	g_hKeyTimer = atUIAPI_AddTimer( KEY_REPEAT_DELAY, cbKeyTimer, atNULL, atTRUE, atFALSE );
	} else {
		if( g_bActivateKeyTimer ){
			atUIAPI_EndKeyTimer();
		}
		if( g_hKeyTimer ){
		    atUIAPI_RemoveTimer( g_hKeyTimer );
		    g_hKeyTimer = atNULL;
		}
	}
	
	g_bUseKeyTimer = bUseKeyTimer;

}
	

/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
void atUIAPI_StartKeyTimer(int nVKey)
{
	atKERNEL_KeyPress( nVKey );

	if( !g_bUseKeyTimer ) return;
	if( g_bActivateKeyTimer ) return;
		
	g_nKeyPressedTime = atUIAPI_GetCurrTime();
	g_bIsLongKey = atTRUE;

	g_bIsFirstRepeat = atTRUE;
	atUIAPI_SetTimerInterval( g_hKeyTimer, KEY_REPEAT_DELAY );
	atUIAPI_SetTimerParam( g_hKeyTimer, (atVOID *)nVKey );
	atUIAPI_EnableTimer( g_hKeyTimer, atTRUE );

	g_bActivateKeyTimer = atTRUE;
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
void atUIAPI_EndKeyTimer(void)
{
	if( !g_bUseKeyTimer ) return;
	if( !g_bActivateKeyTimer ) return;
		
	g_bIsLongKey = atFALSE;
	atUIAPI_EnableTimer( g_hKeyTimer, atFALSE );
	g_bActivateKeyTimer = atFALSE;
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
int atUIAPI_ConvOEMKey2VKey( int OEMKey )
{
	return tOemFn.fnConvOEMKey2VKey( OEMKey );
}


/** 
 @brief 
 
 ......

 @param 
 @return ����
*/
int atUIAPI_ConvVKey2OEMKey( int VKey )
{
	return tOemFn.fnConvVKey2OEMKey( VKey );
}
