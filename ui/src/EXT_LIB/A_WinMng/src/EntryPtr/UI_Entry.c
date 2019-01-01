/**
  @file UI_Entry.c
  @brief UI엔진의 Entroy Pointer
  @remark UI엔진의 Entroy Pointer
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Entry.h
*/
#include "UI_Types.h"
#include "UI_MNG.h"


#define __DO_NOT_MULTIKEY_  (0)


#if __DO_NOT_MULTIKEY_
static int g_nCurKey;
#endif


/** 
 @brief UI 엔진 초기화

 UI 엔진을 초기화 한다.\n
 엔진 사용에 앞서 반드시 단 한번 호출해 줘야 한다.

 @param lpInitData [in] : OEM 레이어에 전달할 파라미터
 @return 성공하면 atTRUE, 실패하면 atFALSE
*/
atBOOL   atUIENG_Initialize( atUIAPI_INIT_DATA *lpInitData )
{
#if __DO_NOT_MULTIKEY_
    g_nCurKey = 0x00;
#endif

    return atKERNEL_Initialize( lpInitData );    
}


/** 
 @brief UI 엔진 종료
 
 UI 엔진을 종료시킨다.

 @return 없음
*/
void atUIENG_Finalize( void )
{
#if __DO_NOT_MULTIKEY_
    g_nCurKey = 0x00;
#endif

    atKERNEL_Finalize();
}


/** 
 @brief KeyDown에 대한 엔트리 포인트
 
 KeyDown에 대한 엔트리 포인트

 @param nKeyCode [in] : 키코드
 @return 성공하면 atTRUE, 실패하면 atFALSE
*/
atBOOL atUIENG_OnKeyDown(int nKeyCode)
{
#if __DO_NOT_MULTIKEY_
	if( g_nCurKey != 0 ) return atFALSE;
	g_nCurKey = nKeyCode;
#endif
	
	atKERNEL_KeyDown(nKeyCode);
	return atTRUE;
}


/** 
 @brief Long KeyDown에 대한 엔트리 포인트
 
 Long KeyDown에 대한 엔트리 포인트

 @param nKeyCode [in] : 키코드
 @return 성공하면 atTRUE, 실패하면 atFALSE
*/
atBOOL atUIENG_OnLongKeyDown(int nKeyCode)
{
	atKERNEL_LongKeyDown(nKeyCode);
	return atTRUE;
}


/** 
 @brief KeyUp에 대한 엔트리 포인트
 
 KeyUp에 대한 엔트리 포인트

 @param nKeyCode [in] : 키코드
 @return 성공하면 atTRUE, 실패하면 atFALSE
 @remark KeyDown과 KeyUp은 반드시 쌍으로 호출되어야 한다.
*/
atBOOL atUIENG_OnKeyUp(int nKeyCode)
{
#if __DO_NOT_MULTIKEY_
	if( g_nCurKey != nKeyCode ) return atFALSE;
	g_nCurKey = 0x00;
#endif
	
	atKERNEL_KeyUp( nKeyCode );
	return atTRUE;
}


/** 
 @brief 문자키 입력을 받는 엔트리 포인트
 
 문자키 입력을 받는 엔트리 포인트

 @param nCharCode [in] : Char 코드
 @return 성공하면 atTRUE, 실패하면 atFALSE
*/
atBOOL atUIENG_OnKeyChar(int nCharCode)
{
	// 일반 영숫자로 입력을 한정한다.
	if( (nCharCode != ' ') && (nCharCode != '-') && (nCharCode != '.') &&
	   !(nCharCode >= '0' && nCharCode <= '9') &&
	   !(nCharCode >= 'a' && nCharCode <= 'z') &&
	   !(nCharCode >= 'A' && nCharCode <= 'Z') ) return atFALSE;
	
	atKERNEL_KeyChar( nCharCode );

	return atTRUE;
}


/** 
 @brief 시스템 키에 대한 엔트리 포인트
 
 시스템 키에 대한 엔트리 포인트

 @param nKeyCode [in] : 키코드
 @return 성공하면 atTRUE, 실패하면 atFALSE
 @remark 시스템에 따라 다르게 처리되기에, 호환성을 보장할 수 없다.
*/
atBOOL atUIENG_OnSysKeyDown(int nVKeyCode)
{
	if( nVKeyCode == 0 ) return atFALSE;
	atKERNEL_SysKeyDown(nVKeyCode);
	return atTRUE;
}


/** 
 @brief 화면을 갱신할 때의 엔트리 포인트
 
 화면을 갱신할 때의 엔트리 포인트

 @return 없음
*/
void atUIENG_OnPaint(void)
{    
    atKERNEL_Paint();
}


/** 
 @brief 디스플레이 시스템의 크기 변경 이벤트에 대한 엔트리 포인트
 
 디스플레이 시스템의 크기 변경 이벤트에 대한 엔트리 포인트

 @param width, height [in] : 새로 바뀐 디스플레이 장치의 크기
 @return 없음
*/
void atUIENG_OnResize( int width, int height )
{
	atKERNEL_Resize( width, height );
}


/** 
 @brief MouseDown에 대한 엔트리 포인트
 
 MouseDown에 대한 엔트리 포인트

 @param btn [in] : 눌린 버튼
 @param x, y [in] : 마우스가 눌린 좌표
 @return 없음
*/
void atUIENG_OnMouseDown( int btn, int x, int y )
{    
	atUIAPI_ConvOEMPtr2VPtr( &x, &y );
	atKERNEL_MouseDown( btn, x, y );
}


/** 
 @brief MouseMove에 대한 엔트리 포인트
 
 MouseMove에 대한 엔트리 포인트

 @param btn [in] : 현재 눌려 있는 마우스 버튼
 @param x, y [in] : 마우스가 이동한 좌표
 @return 없음
*/
void atUIENG_OnMouseMove( int btn, int x, int y )
{    
	atUIAPI_ConvOEMPtr2VPtr( &x, &y );
	atKERNEL_MouseMove( btn, x, y );
}


/** 
 @brief MouseUp에 대한 엔트리 포인트
 
 MouseUp에 대한 엔트리 포인트

 @param btn [in] : 마우스를 뗀 버튼
 @param x, y [in] : 마우스 버튼을 뗀 좌표
 @return 없음
 @remark MouseDown과 MouseUp은 반드시 쌍으로 발생되어야 한다.
*/
void atUIENG_OnMouseUp( int btn, int x, int y )
{    
	atUIAPI_ConvOEMPtr2VPtr( &x, &y );
	atKERNEL_MouseUp( btn, x, y );
}


/** 
 @brief Mouse 버튼을 길게 누른 이벤트의 엔트리 포인트
 
 Mouse 버튼을 길게 누른 이벤트의 엔트리 포인트

 @param btn [in] : 마우스를 길게 누른 버튼
 @param x, y [in] : 마우스 길게 눌렀을 때의 좌표
 @return 없음
*/
void atUIENG_OnMouseLongClick( int btn, int x, int y )
{    
	atUIAPI_ConvOEMPtr2VPtr( &x, &y );
	atKERNEL_MouseLongClick( btn, x, y );
}


/** 
 @brief 시스템을 Pause시킨다.
 
 시스템을 Pause시킨다.

 @return 없음
*/
void atUIENG_OnPause(void)
{
	atKERNEL_Pause();
}


/** 
 @brief Pause된 시스템을 Resume시킨다.
 
 Pause된 시스템을 Resume시킨다.

 @return 없음
*/
void atUIENG_OnResume(void)
{
	atKERNEL_Resume();
}


/** 
 @brief 시스템에 Notify를 날린다.
 
 시스템에 Notify를 날린다.

 @param nParam1, nParam2 [in] : 이벤트의 파라미터
 @return 없음
*/
void atUIENG_OnSystemNotify( long nParam1, long nParam2 )
{
    atKERNEL_SystemNotify( nParam1, nParam2 );
}


/** 
 @brief Timer tick.
 
 시스템에 Timer tick을 날린다.

 @return 없음
*/
void atUIENG_OnTimerTick() 
{
    //atUIAPI_TimerTick();
}
