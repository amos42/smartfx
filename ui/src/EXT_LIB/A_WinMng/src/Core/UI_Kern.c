/**
  @file UI_Kern.c
  @brief UI엔진의 Kernel에 관한 API
  @remark UI엔진의 Kernel에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Kern.h
*/
#include "UI_Types.h"
#include "Msg.h"
#include "UI_MNG.h"
#include "UI_Desk.h"
#include "WinMng.h"
#include "Window.h"
/* Pactory 개념이 불분명해서 제외 
#include "WinObjPT.h"
*/


atBOOL g_bIsUIEngInitialize = atFALSE;
static atHWINOBJ g_hMouseTarget = atNULL;
static atHWINOBJ g_hCaptureMouse = atNULL;
static atHWINOBJ g_hCaptureKey = atNULL;
static atBOOL g_bMouseDown = atFALSE;
static int g_nSoftkeyDown = 0;
static int g_nLockInput = 0;
//static atHCRSECTION g_hScreenLock = atNULL;

static int g_nLastKeyEvent, g_nLastKey;



/** 
 @brief UI 엔진 커널 초기화 (내부용)
 
 UI 엔진의 커널을 초기화 한다.\n
 엔진 사용에 앞서 반드시 단 한번 호출해 줘야 한다.

 @param lpInitData [in] : OEM 레이어에 전달할 파라미터
 @return 성공하면 atTRUE, 실패하면 atFALSE
 @remark 내부용
*/
atBOOL   atKERNEL_Initialize( atUIAPI_INIT_DATA *lpInitData )
{
	atBOOL r;
	
    if( g_bIsUIEngInitialize ) return atTRUE;
	
    r = atUIAPI_Initialize( lpInitData );
	if( !r ){
		return atFALSE;
	}
	
	//g_hScreenLock = atUIAPI_CreateCriticalSection();
		
	atUIAPI_InitEventProcessor();
	atUIAPI_InitDisplayProcessor(); // 반드시 atUIAPI_InitEventProcessor 이후에 Call되어야 함.
	
	/* Pactory 개념이 불분명해서 제외
	r = atWINPACTORY_Initialize();
	if( !r ){
		atUIAPI_Finalize();
		return atFALSE;
	}
	*/

	g_bIsUIEngInitialize = atTRUE;

    atDESKTOP_InitDesktop(0);

	g_hMouseTarget = atNULL;
	g_hCaptureMouse = atNULL;
	g_hCaptureKey = atNULL;
	g_bMouseDown = atFALSE;
	g_nSoftkeyDown = 0;

	return atTRUE;
}


/** 
 @brief UI 엔진 커널 종료 (내부용)
 
 UI 엔진의 커널을 종료 한다.\n
 엔진 종료 과정에서 반드시 단 한번 호출해 줘야 한다.

 @return 없음
 @remark 내부용
*/
void atKERNEL_Finalize( void )
{
    if( !g_bIsUIEngInitialize ) return;

	/* Pactory 개념이 불분명해서 제외
	atWINPACTORY_Finalize();
	*/

    atDESKTOP_FinalDesktop();
    
	g_hMouseTarget = atNULL;
	g_hCaptureMouse = atNULL;
	g_hCaptureKey = atNULL;
	g_bMouseDown = atFALSE;
	g_nSoftkeyDown = 0;

	atUIAPI_FinalDisplayProcessor(); // 반드시 atUIAPI_FinalEventProcessor 이전에 Call되어야 함.
	atUIAPI_FinalEventProcessor();
	
	//if( g_hScreenLock != atNULL ){
	//	atUIAPI_DestroyCriticalSection( g_hScreenLock );
	//	g_hScreenLock = atNULL;
	//}
		
    atUIAPI_Finalize();

	g_bIsUIEngInitialize = atFALSE;	
}


/** 
 @brief 화면 갱신을 막는다. 
 
 화면 갱신을 막는다.

 @return 없음
*/
void	atKERNEL_LockScreen(void)
{
	//if( g_hScreenLock == atNULL ) return;
	//atUIAPI_EnterCriticalSection( g_hScreenLock );
}


/** 
 @brief 화면 갱신을 재개한다.
 
 화면 갱신을 재개한다.

 @return 없음
*/
void	atKERNEL_UnlockScreen(void)
{
	//if( g_hScreenLock == atNULL ) return;
	//atUIAPI_LeaveCriticalSection( g_hScreenLock );
}


/** 
 @brief 입력을 막는다.
 
 입력을 막는다.

 @return 없음
*/
void	atKERNEL_LockInput(void)
{
	g_nLockInput++;
}


/** 
 @brief 입력을 재개한다.
 
 입력을 재개한다.

 @return 없음
*/
void	atKERNEL_UnlockInput(void)
{
	g_nLockInput--;
	if( g_nLockInput < 0 ) g_nLockInput = 0;
}


/* * 
 @brief 마우스의 이벤트를 특정 개체로 한정한다.
 
 마우스의 이벤트를 특정 개체로 한정하여 고정시킨다.

 @param hTargetObj [in] : 마우스 이벤트의 대상 개체
 @return 성공시 atTRUE, 실패시 atFALSE
*/
/*
atBOOL atKERNEL_CaptureMouse( atHANDLE hTargetObj )
{
	g_hCaptureMouse = hTargetObj;
	return atTRUE;
}
*/

/* * 
 @brief 키 입력 대상 개체를 해제한다.
 
 키 입력 대상 개체를 해제한다.

 @return 없음
*/
/*
void atKERNEL_ResetKey(void)
{
	g_hCaptureKey = atNULL;

	atUIAPI_EndKeyTimer();
}
*/

/** 
 @brief KeyDown에 대한 처리
 
 KeyDown에 대한 처리

 @param nVKey [in] : 가상 키코드
 @return 없음
*/
void atKERNEL_KeyDown(int nVKey)
{
	atHWINDOW hWnd;
	//atHWINOBJ hWinObj;;

	if( nVKey == 0 ) return;

	if( g_nLockInput > 0 ) return;

	if( g_nLastKeyEvent == 0x0100 && g_nLastKey == nVKey ) return;
	g_nLastKeyEvent = 0x0100;
	g_nLastKey = nVKey;
		
	hWnd = atAPP_GetCurWindow( atAPPMNG_GetCurApp() );
	//hWinObj = atAPP_GetFocusedObj( atAPPMNG_GetCurApp() );
	g_hCaptureKey = hWnd;
	
	if ( hWnd != atNULL ){
	    atWINOBJ_PostMessage( hWnd, OBJMSG_KEYDOWN, nVKey, 0 );
	    atUIAPI_StartKeyTimer( nVKey );
	}
}


/** 
 @brief 키의 반복 스트로크 값 처리
 
 키의 반복 스트로크 값 처리

 @param nVKey [in] : 가상 키코드
 @return 없음
*/
void atKERNEL_KeyPress(int nVKey)
{
	atHWINDOW hWnd;
	if( nVKey == 0 ) return;
	
	if( g_nLockInput > 0 ) return;

	hWnd = g_hCaptureKey;
	
	if ( hWnd != atNULL ){
	    //atWINDOW_PostMessage( hWnd, WNDMSG_KEYPRESS, nVKey, 0 );
		atWINOBJ_PostMessage( hWnd, OBJMSG_KEYPRESS, nVKey, 0 );
	}
}


/** 
 @brief Long KeyDown에 대한 처리
 
 Long KeyDown에 대한 처리

 @param nVKey [in] : 가상 키코드
 @return 없음
*/
void atKERNEL_LongKeyDown(int nVKey)
{
	atHWINDOW hWnd;
	if( nVKey == 0 ) return;
	
	hWnd = g_hCaptureKey;
	
	if( g_nLockInput > 0 ) return;

	if ( hWnd != atNULL ){
	    //atWINDOW_PostMessage( hWnd, WNDMSG_LONGKEYDOWN, nVKey, 0 );
		atWINOBJ_PostMessage( hWnd, OBJMSG_LONGKEYDOWN, nVKey, 0 );
	}
}


/** 
 @brief KeyUp에 대한 처리
 
 KeyUp에 대한 처리

 @param nVKey [in] : 가상 키코드
 @return 없음
 @remark KeyDown과 KeyUp은 반드시 쌍으로 호출되어야 한다.
*/
void atKERNEL_KeyUp(int nVKey)
{
	atHWINDOW hWnd;
	if( nVKey == 0 ) return;

	if( g_nLockInput > 0 ){
		g_hCaptureKey = atNULL;
		return;
	}

	if( g_nLastKeyEvent == 0x0101 && g_nLastKey == nVKey ) return;
	g_nLastKeyEvent = 0x0101;
	g_nLastKey = nVKey;
	
	atUIAPI_EndKeyTimer();
	
	hWnd = g_hCaptureKey;
	g_hCaptureKey = atNULL;

	if ( hWnd != atNULL ){
	    //atWINDOW_PostMessage( hWnd, WNDMSG_KEYUP, nVKey, 0 );
		atWINOBJ_PostMessage( hWnd, OBJMSG_KEYUP, nVKey, 0 );
	}
}


/** 
 @brief 문자키 입력을 처리
 
 문자키 입력을 처리

 @param nCharCode [in] : Char 코드
 @return 없음
*/
void atKERNEL_KeyChar(int nCharKey)
{
	atHWINDOW hWnd;
	if( nCharKey == 0 ) return;

	if( g_nLockInput > 0 ) return;

	hWnd = atAPP_GetCurWindow( atAPPMNG_GetCurApp() );
	//hWnd = atAPP_GetFocusedObj( atAPPMNG_GetCurApp() );
	
	if ( hWnd != atNULL ){
	    atWINDOW_PostMessage( hWnd, WNDMSG_KEYCHAR, nCharKey, 0 );
	}
}


/** 
 @brief 시스템 키에 대한 처리
 
 시스템 키에 대한 처리

 @param nVKey [in] : 가상 키코드
 @return 없음
 @remark 시스템에 따라 다르게 처리되기에, 호환성을 보장할 수 없다.
*/
void atKERNEL_SysKeyDown(int nVKey)
{
	atHWINDOW hWnd;
	if( nVKey == 0 ) return;

	if( g_nLockInput > 0 ) return;

	hWnd = atAPP_GetCurWindow( atAPPMNG_GetCurApp() );
	//hWnd = atAPP_GetFocusedObj( atAPPMNG_GetCurApp() );

	if ( hWnd != atNULL ){
	    atWINDOW_PostMessage( hWnd, WNDMSG_SYSKEYDOWN, nVKey, 0 );
	}
}


/** 
 @brief 화면을 갱신할 때의 처리
 
 화면을 갱신할 때의 처리

 @return 없음
*/
void atKERNEL_Paint(void)
{    
	atKERNEL_LockScreen();
    atAPP_RedrawAllWindow( atAPPMNG_GetCurApp() );
    atUIAPI_FlushScreen();
	atKERNEL_UnlockScreen();
}


/** 
 @brief 디스플레이 시스템의 크기 변경 이벤트에 대한 처리
 
 디스플레이 시스템의 크기 변경 이벤트에 대한 처리

 @param width, height [in] : 새로 바뀐 디스플레이 장치의 크기
 @return 없음
*/
void atKERNEL_Resize( int width, int height )
{
	atKERNEL_LockScreen();
	atUIAPI_SetScreenResolution( width, height );
	atKERNEL_UnlockScreen();
	
	atDESKTOP_RecalcDesktopArea();
	atAPP_AdjustAllWindowArea(atAPPMNG_GetCurApp());
	
//	atAPP_RedrawAllWindow(atAPPMNG_GetCurApp(), atFALSE);	
//	atUIAPI_AddInvalideAreaAll();
	atUIAPI_DisplayForceAll();
}


/** 
 @brief MouseDown에 대한 처리
 
 MouseDown에 대한 처리

 @param btn [in] : 눌린 버튼
 @param x, y [in] : 마우스가 눌린 좌표
 @return 없음
*/
void atKERNEL_MouseDown( int btn, int x, int y )
{    
	atHWINDOW hWnd;
	atRECT rect;
	
	if( g_nLockInput > 0 ) return;

	g_bMouseDown = atTRUE;

	if( atSOFTKEY_IsActiveSoftKey() ){
		atREGION *sk_area;
		sk_area = atSOFTKEY_GetSoftKeyArea();
		if( x >= sk_area->nX && x < sk_area->nX + sk_area->nWidth &&
		    y >= sk_area->nY && y < sk_area->nY + sk_area->nHeight ){
		     x = (x - sk_area->nX) / (sk_area->nWidth/3);
		     if( x == 0 ){
			 	if( atSOFTKEY_GetSoftKeyEnabledOne(0) && atUIAPI_strcmp(atSOFTKEY_GetSoftKeyTextOne(0), _AT("\0")) != 0 ) g_nSoftkeyDown = 1;
		     } else if( x == 1 ){
			 	if( atSOFTKEY_GetSoftKeyEnabledOne(1) && atUIAPI_strcmp(atSOFTKEY_GetSoftKeyTextOne(1), _AT("\0")) != 0 ) g_nSoftkeyDown = 2;
		     } else if( x == 2 ){
			 	if( atSOFTKEY_GetSoftKeyEnabledOne(2) && atUIAPI_strcmp(atSOFTKEY_GetSoftKeyTextOne(2), _AT("\0")) != 0 ) g_nSoftkeyDown = 3;
		     }
			 atSOFTKEY_DownSoftKey( g_nSoftkeyDown );
			 atSOFTKEY_RefreshSoftKey();
		     return;
		}
	}
 	g_nSoftkeyDown = 0;
	
	if( g_hCaptureMouse != atNULL ){
		hWnd = g_hCaptureMouse;
	} else {
		atHWINDOW hWnd0 = atAPP_GetCurWindow( atAPPMNG_GetCurApp() );
		//atHWINDOW hWnd0 = atAPP_GetFocusedObj( atAPPMNG_GetCurApp() );
		hWnd = atAPP_FindWindowAt( atAPPMNG_GetCurApp(), x, y );
		if( (hWnd != atNULL) && (hWnd != hWnd0) ){
			if( atWINDOW_GetAttr( hWnd0 ) & atOBJATTR_MODAL ){
				hWnd = hWnd0;
			} else {
				atAPP_SetTopWindow( atAPPMNG_GetCurApp(), hWnd );
				atAPP_SetFocusToWindow( atAPPMNG_GetCurApp(), hWnd );
			}
		}
	}

	if ( hWnd != atNULL ){
		atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);
		if( lpWnd != atNULL ){
			g_hMouseTarget = hWnd;
			atWINDOW_GetWindowRect( hWnd, &rect );
			x -= rect.nStartX;
			y -= rect.nStartY;
			atWINDOW_PostMessage( hWnd, WNDMSG_PREMOUSEDOWN, btn, (x << 16) | (y & 0xffff) );
		}
	}
}


/** 
 @brief MouseMove에 대한 처리
 
 MouseMove에 대한 처리

 @param btn [in] : 현재 눌려 있는 마우스 버튼
 @param x, y [in] : 마우스가 이동한 좌표
 @return 없음
*/
void atKERNEL_MouseMove( int btn, int x, int y )
{    
	atHWINDOW hWnd;
	atLPWINDOW lpWnd;
	atRECT rect;
	
	if( g_nLockInput > 0 ) return;

	if( !g_bMouseDown ){
		// 터치 인터페이스 상에서 불필요하기에, 
		// 퍼포먼스를 위해... 마우스 클릭하지 않은 것들은 Move에서 제외.
		return;
	}
	
	atUIAPI_ConvOEMPtr2VPtr( &x, &y );

	if( g_hCaptureMouse != atNULL ){
		hWnd = g_hCaptureMouse;
	} else {
		hWnd = g_hMouseTarget;//atAPP_GetCurWindow( atAPPMNG_GetCurApp() );
	}
	
	lpWnd = atWINDOW_GetPtr(hWnd);
	if( lpWnd != atNULL ){
		atWINDOW_GetWindowRect( hWnd, &rect );
		if( g_hCaptureMouse == atNULL ){
			if( x < rect.nStartX || x > rect.nEndX || y < rect.nStartY || y > rect.nEndY ) return;
		}
		
		atWINDOW_GetWindowRect( hWnd, &rect );
		x -= rect.nStartX;
		y -= rect.nStartY;
		atWINDOW_PostMessage( hWnd, WNDMSG_MOUSEMOVE, btn, (x << 16) | (y & 0xffff) );
	}
}


/** 
 @brief MouseUp에 대한 처리
 
 MouseUp에 대한 처리

 @param btn [in] : 마우스를 뗀 버튼
 @param x, y [in] : 마우스 버튼을 뗀 좌표
 @return 없음
 @remark MouseDown과 MouseUp은 반드시 쌍으로 발생되어야 한다.
*/
void atKERNEL_MouseUp( int btn, int x, int y )
{    
	atHWINDOW hWnd;
	atLPWINDOW lpWnd;
	atRECT rect;
	
	if( !g_bMouseDown )	return;
	g_bMouseDown = atFALSE;

	if( g_nLockInput > 0 ) return;

	atUIAPI_ConvOEMPtr2VPtr( &x, &y );

	if( atSOFTKEY_IsActiveSoftKey() && g_nSoftkeyDown > 0 ){
		atREGION *sk_area;

		atSOFTKEY_DownSoftKey( 0 );
		atSOFTKEY_RefreshSoftKey();

		sk_area = atSOFTKEY_GetSoftKeyArea();
		if( x >= sk_area->nX && x < sk_area->nX + sk_area->nWidth &&
		    y >= sk_area->nY && y < sk_area->nY + sk_area->nHeight ){
		     x = (x - sk_area->nX) / (sk_area->nWidth/3);
		     if( x == 0 ){
		 	 if( g_nSoftkeyDown == 1 ){
			    atKERNEL_KeyDown( atVKEY_SOFT1 );
			    atKERNEL_KeyPress( atVKEY_SOFT1 );
			    atKERNEL_KeyUp( atVKEY_SOFT1 );
			 }
		     } else if( x == 1 ){
		 	 if( g_nSoftkeyDown == 2 ){
			    atKERNEL_KeyDown( atVKEY_SELECT );
			    atKERNEL_KeyPress( atVKEY_SELECT );
			    atKERNEL_KeyUp( atVKEY_SELECT );
			 }
		     } else if( x == 2 ){
		 	 if( g_nSoftkeyDown == 3 ){
			    atKERNEL_KeyDown( atVKEY_SOFT2 );
			    atKERNEL_KeyPress( atVKEY_SOFT2 );
			    atKERNEL_KeyUp( atVKEY_SOFT2 );
			 }
		     }
		     g_nSoftkeyDown = 0;
		     return;
		}
	}
       g_nSoftkeyDown = 0;
	
	if( g_hCaptureMouse != atNULL ){
		hWnd = g_hCaptureMouse;
	} else {
		hWnd = g_hMouseTarget;//atAPP_GetCurWindow( atAPPMNG_GetCurApp() );
	}
	
	lpWnd = atWINDOW_GetPtr(hWnd);
	if ( hWnd != atNULL ){
		atWINDOW_GetWindowRect( hWnd, &rect );
		x -= rect.nStartX;
		y -= rect.nStartY;
		atWINDOW_PostMessage( hWnd, WNDMSG_MOUSEUP, btn, (x << 16) | (y & 0xffff) );
	}

	g_hMouseTarget = atNULL;
}


/** 
 @brief Mouse 버튼을 길게 눌렀을 때의 처리
 
 Mouse 버튼을 길게 눌렀을 때의 처리

 @param btn [in] : 마우스를 길게 누른 버튼
 @param x, y [in] : 마우스 길게 눌렀을 때의 좌표
 @return 없음
*/
void atKERNEL_MouseLongClick( int btn, int x, int y )
{
	atHWINDOW hWnd;
	atLPWINDOW lpWnd;
	atRECT rect;
	
	if( g_nLockInput > 0 ) return;

	if( !g_bMouseDown ) return;

	if( g_hCaptureMouse != atNULL ){
		hWnd = g_hCaptureMouse;
	} else {
		hWnd = g_hMouseTarget;//atAPP_GetCurWindow( atAPPMNG_GetCurApp() );
	}

	lpWnd = atWINDOW_GetPtr(hWnd);
	if ( lpWnd != atNULL ){
		atWINDOW_GetWindowRect( hWnd, &rect);
		x -= rect.nStartX;
		y -= rect.nStartY;
		atWINDOW_PostMessage( hWnd, WNDMSG_PREMOUSELONGCLICK, btn, (x << 16) | y );
	}
}


/** 
 @brief 시스템을 Pause시킨다.
 
 시스템을 Pause시킨다.

 @return 없음
*/
void atKERNEL_Pause(void)
{
	atHWINDOW hWnd;
	
	hWnd = atAPP_GetCurWindow( atAPPMNG_GetCurApp() );
	if(hWnd != atNULL){
		atWINDOW_PostMessage( hWnd, WNDMSG_PAUSE, 0,0 );
	}
	// 타이머를 전부 중지
}


/** 
 @brief Pause된 시스템을 Resume시킨다.
 
 Pause된 시스템을 Resume시킨다.

 @return 없음
*/
void atKERNEL_Resume(void)
{
	// 중지한 타이머 살린다.
	atHWINDOW hWnd;

	hWnd = atAPP_GetCurWindow( atAPPMNG_GetCurApp() );
	if(hWnd != atNULL){
		atWINDOW_PostMessage( hWnd, WNDMSG_RESUME, 0,0 );
	}
}


/** 
 @brief 시스템에 Notify를 날린다.
 
 시스템에 Notify를 날린다.

 @param nParam1, nParam2 [in] : 이벤트의 파라미터
 @return 없음
*/
void atKERNEL_SystemNotify( long nParam1, long nParam2 )
{
	atHWINDOW hWnd;
	
	hWnd = atAPP_GetCurWindow( atAPPMNG_GetCurApp() );
	if ( hWnd != atNULL ){
	    atWINDOW_PostMessage( hWnd, WNDMSG_SYSNOTIFY, nParam1, nParam2 );
	}
}


/**
 @brief Timer Tick 호출.

 Timer Tick을 진행한다. 

 @return 없음
*/
void atKERNEL_Tick( long nTick )
{
	atUIAPI_TickTimer( nTick );
}

void atKERNEL_TickMessage(void)
{
	atAPP_TickMsgProcess(atAPPMNG_GetCurApp());
}

void atKERNEL_TickDisplay(void)
{
	atUIAPI_TickDispProcess();
}




