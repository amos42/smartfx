/**
  @file UI_APIs.c
  @brief UI ���� �ʱ�ȭ API
  @remark UI ���� �ʱ�ȭ API
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see UI_APIs.h
*/
#include "UI_API/UI_API.h"
#include "OEM_UI_System.h"


static OEM_UI_SYSTEM_T tOemFn;

atVOID	OEM_UI_System_SetOemFn( OEM_UI_SYSTEM_T *fnOem )
{
	tOemFn = *fnOem;
}


atVOID	OEM_UI_System_GetOemFn( OEM_UI_SYSTEM_T *fnOem )
{
	*fnOem = tOemFn;
}


/** 
 @brief UI API �ʱ�ȭ
 
 UI API�� �ʱ�ȭ��Ų��. (���ο�)

 @param lpInitData [in] : OEM ���̾ ������ UI API�� �Ķ����
 @return ����
 @remark �� API ȣ�⿡ �ռ� �� �ѹ� ȣ���� ��� �Ѵ�. (���ο�)
*/
atBOOL atUIAPI_Initialize( atUIAPI_INIT_DATA *lpInitData )
{
	if( tOemFn.fnInitialize != atNULL ){
		lpInitData->lOemInst = tOemFn.fnInitialize( lpInitData );
	} else {
		lpInitData->lOemInst = atNULL;
	}

#ifdef __USES_UI_MEM_
	atUIAPI_InitMem( lpInitData->lSysInst, lpInitData->lMemData1, lpInitData->lMemData2 );
#endif
#ifdef __USES_UI_TIMER_
	atUIAPI_InitTimer( lpInitData->lSysInst, lpInitData->lTimerData1, lpInitData->lTimerData2 );
#endif
#ifdef __USES_UI_RES_
	atUIAPI_InitRes( lpInitData->lSysInst, lpInitData->lResData1, lpInitData->lResData2 );
#endif
#ifdef __USES_UI_SCR_
	atUIAPI_InitGraphic( lpInitData->lSysInst, lpInitData->lScrData1, lpInitData->lScrData2 );
#endif
#ifdef __USES_UI_FONT_
	atUIAPI_InitFont( lpInitData->lSysInst, lpInitData->lFontData1, lpInitData->lFontData2 );
#endif
#ifdef __USES_UI_TOUCH_
	atUIAPI_InitTouchDev( lpInitData->lSysInst, lpInitData->lTouchData1, lpInitData->lTouchData2 );
#endif
#ifdef __USES_UI_PROC_
	atUIAPI_InitProcessor( lpInitData->lSysInst, lpInitData->lProcData1, lpInitData->lProcData2 );
#endif
#ifdef __USES_UI_DEBUG_
	atUIAPI_InitDebug( lpInitData->lSysInst, lpInitData->lDebugData1, lpInitData->lDebugData2 );
#endif
#ifdef __USES_UI_IME_
	atUIAPI_InitIME( lpInitData->lSysInst, lpInitData->lIMEData1, lpInitData->lIMEData2 );
#endif
#ifdef __USES_UI_OIC_
	atUIAPI_InitOIC( lpInitData->lSysInst, lpInitData->lOICData1, lpInitData->lOICData2 );
#endif

	return atTRUE;
}


/** 
 @brief UI API ����
 
 UI API�� �����Ѵ�.(���ο�)

 @return ����
 @remark ���α׷��� ������ ��, ���������� �� �ѹ� ȣ���� ��� �Ѵ�.
*/
void atUIAPI_Finalize(void)
{
#ifdef __USES_UI_OIC_
    atUIAPI_FinalOIC();
#endif
#ifdef __USES_UI_IME_
    atUIAPI_FinalIME();
#endif
#ifdef __USES_UI_DEBUG_
    atUIAPI_FinalDebug();
#endif
#ifdef __USES_UI_PROC_
    atUIAPI_FinalProcessor();
#endif
#ifdef __USES_UI_FONT_
	atUIAPI_FinalFont();
#endif
#ifdef __USES_UI_SCR_
    atUIAPI_FinalGraphic();
#endif
#ifdef __USES_UI_RES_
	atUIAPI_FinalRes();
#endif
#ifdef __USES_UI_TIMER_
	atUIAPI_FinalTimer();
#endif
#ifdef __USES_UI_MEM_
	atUIAPI_FinalMem();
#endif

	if( tOemFn.fnFinalize != atNULL ){
		tOemFn.fnFinalize();
	}
}

