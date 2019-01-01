/**
  @file UI_Kern.c
  @brief UI������ Kernel�� ���� API
  @remark UI������ Kernel�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see UI_Kern.h
*/
#include "UI_Types.h"
#include "Msg.h"
#include "UI_MNG.h"
#include "UI_Desk.h"
#include "WinMng.h"
#include "Window.h"
/* Pactory ������ �Һи��ؼ� ���� 
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
 @brief UI ���� Ŀ�� �ʱ�ȭ (���ο�)
 
 UI ������ Ŀ���� �ʱ�ȭ �Ѵ�.\n
 ���� ��뿡 �ռ� �ݵ�� �� �ѹ� ȣ���� ��� �Ѵ�.

 @param lpInitData [in] : OEM ���̾ ������ �Ķ����
 @return �����ϸ� atTRUE, �����ϸ� atFALSE
 @remark ���ο�
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
	atUIAPI_InitDisplayProcessor(); // �ݵ�� atUIAPI_InitEventProcessor ���Ŀ� Call�Ǿ�� ��.
	
	/* Pactory ������ �Һи��ؼ� ����
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
 @brief UI ���� Ŀ�� ���� (���ο�)
 
 UI ������ Ŀ���� ���� �Ѵ�.\n
 ���� ���� �������� �ݵ�� �� �ѹ� ȣ���� ��� �Ѵ�.

 @return ����
 @remark ���ο�
*/
void atKERNEL_Finalize( void )
{
    if( !g_bIsUIEngInitialize ) return;

	/* Pactory ������ �Һи��ؼ� ����
	atWINPACTORY_Finalize();
	*/

    atDESKTOP_FinalDesktop();
    
	g_hMouseTarget = atNULL;
	g_hCaptureMouse = atNULL;
	g_hCaptureKey = atNULL;
	g_bMouseDown = atFALSE;
	g_nSoftkeyDown = 0;

	atUIAPI_FinalDisplayProcessor(); // �ݵ�� atUIAPI_FinalEventProcessor ������ Call�Ǿ�� ��.
	atUIAPI_FinalEventProcessor();
	
	//if( g_hScreenLock != atNULL ){
	//	atUIAPI_DestroyCriticalSection( g_hScreenLock );
	//	g_hScreenLock = atNULL;
	//}
		
    atUIAPI_Finalize();

	g_bIsUIEngInitialize = atFALSE;	
}


/** 
 @brief ȭ�� ������ ���´�. 
 
 ȭ�� ������ ���´�.

 @return ����
*/
void	atKERNEL_LockScreen(void)
{
	//if( g_hScreenLock == atNULL ) return;
	//atUIAPI_EnterCriticalSection( g_hScreenLock );
}


/** 
 @brief ȭ�� ������ �簳�Ѵ�.
 
 ȭ�� ������ �簳�Ѵ�.

 @return ����
*/
void	atKERNEL_UnlockScreen(void)
{
	//if( g_hScreenLock == atNULL ) return;
	//atUIAPI_LeaveCriticalSection( g_hScreenLock );
}


/** 
 @brief �Է��� ���´�.
 
 �Է��� ���´�.

 @return ����
*/
void	atKERNEL_LockInput(void)
{
	g_nLockInput++;
}


/** 
 @brief �Է��� �簳�Ѵ�.
 
 �Է��� �簳�Ѵ�.

 @return ����
*/
void	atKERNEL_UnlockInput(void)
{
	g_nLockInput--;
	if( g_nLockInput < 0 ) g_nLockInput = 0;
}


/* * 
 @brief ���콺�� �̺�Ʈ�� Ư�� ��ü�� �����Ѵ�.
 
 ���콺�� �̺�Ʈ�� Ư�� ��ü�� �����Ͽ� ������Ų��.

 @param hTargetObj [in] : ���콺 �̺�Ʈ�� ��� ��ü
 @return ������ atTRUE, ���н� atFALSE
*/
/*
atBOOL atKERNEL_CaptureMouse( atHANDLE hTargetObj )
{
	g_hCaptureMouse = hTargetObj;
	return atTRUE;
}
*/

/* * 
 @brief Ű �Է� ��� ��ü�� �����Ѵ�.
 
 Ű �Է� ��� ��ü�� �����Ѵ�.

 @return ����
*/
/*
void atKERNEL_ResetKey(void)
{
	g_hCaptureKey = atNULL;

	atUIAPI_EndKeyTimer();
}
*/

/** 
 @brief KeyDown�� ���� ó��
 
 KeyDown�� ���� ó��

 @param nVKey [in] : ���� Ű�ڵ�
 @return ����
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
 @brief Ű�� �ݺ� ��Ʈ��ũ �� ó��
 
 Ű�� �ݺ� ��Ʈ��ũ �� ó��

 @param nVKey [in] : ���� Ű�ڵ�
 @return ����
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
 @brief Long KeyDown�� ���� ó��
 
 Long KeyDown�� ���� ó��

 @param nVKey [in] : ���� Ű�ڵ�
 @return ����
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
 @brief KeyUp�� ���� ó��
 
 KeyUp�� ���� ó��

 @param nVKey [in] : ���� Ű�ڵ�
 @return ����
 @remark KeyDown�� KeyUp�� �ݵ�� ������ ȣ��Ǿ�� �Ѵ�.
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
 @brief ����Ű �Է��� ó��
 
 ����Ű �Է��� ó��

 @param nCharCode [in] : Char �ڵ�
 @return ����
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
 @brief �ý��� Ű�� ���� ó��
 
 �ý��� Ű�� ���� ó��

 @param nVKey [in] : ���� Ű�ڵ�
 @return ����
 @remark �ý��ۿ� ���� �ٸ��� ó���Ǳ⿡, ȣȯ���� ������ �� ����.
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
 @brief ȭ���� ������ ���� ó��
 
 ȭ���� ������ ���� ó��

 @return ����
*/
void atKERNEL_Paint(void)
{    
	atKERNEL_LockScreen();
    atAPP_RedrawAllWindow( atAPPMNG_GetCurApp() );
    atUIAPI_FlushScreen();
	atKERNEL_UnlockScreen();
}


/** 
 @brief ���÷��� �ý����� ũ�� ���� �̺�Ʈ�� ���� ó��
 
 ���÷��� �ý����� ũ�� ���� �̺�Ʈ�� ���� ó��

 @param width, height [in] : ���� �ٲ� ���÷��� ��ġ�� ũ��
 @return ����
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
 @brief MouseDown�� ���� ó��
 
 MouseDown�� ���� ó��

 @param btn [in] : ���� ��ư
 @param x, y [in] : ���콺�� ���� ��ǥ
 @return ����
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
 @brief MouseMove�� ���� ó��
 
 MouseMove�� ���� ó��

 @param btn [in] : ���� ���� �ִ� ���콺 ��ư
 @param x, y [in] : ���콺�� �̵��� ��ǥ
 @return ����
*/
void atKERNEL_MouseMove( int btn, int x, int y )
{    
	atHWINDOW hWnd;
	atLPWINDOW lpWnd;
	atRECT rect;
	
	if( g_nLockInput > 0 ) return;

	if( !g_bMouseDown ){
		// ��ġ �������̽� �󿡼� ���ʿ��ϱ⿡, 
		// �����ս��� ����... ���콺 Ŭ������ ���� �͵��� Move���� ����.
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
 @brief MouseUp�� ���� ó��
 
 MouseUp�� ���� ó��

 @param btn [in] : ���콺�� �� ��ư
 @param x, y [in] : ���콺 ��ư�� �� ��ǥ
 @return ����
 @remark MouseDown�� MouseUp�� �ݵ�� ������ �߻��Ǿ�� �Ѵ�.
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
 @brief Mouse ��ư�� ��� ������ ���� ó��
 
 Mouse ��ư�� ��� ������ ���� ó��

 @param btn [in] : ���콺�� ��� ���� ��ư
 @param x, y [in] : ���콺 ��� ������ ���� ��ǥ
 @return ����
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
 @brief �ý����� Pause��Ų��.
 
 �ý����� Pause��Ų��.

 @return ����
*/
void atKERNEL_Pause(void)
{
	atHWINDOW hWnd;
	
	hWnd = atAPP_GetCurWindow( atAPPMNG_GetCurApp() );
	if(hWnd != atNULL){
		atWINDOW_PostMessage( hWnd, WNDMSG_PAUSE, 0,0 );
	}
	// Ÿ�̸Ӹ� ���� ����
}


/** 
 @brief Pause�� �ý����� Resume��Ų��.
 
 Pause�� �ý����� Resume��Ų��.

 @return ����
*/
void atKERNEL_Resume(void)
{
	// ������ Ÿ�̸� �츰��.
	atHWINDOW hWnd;

	hWnd = atAPP_GetCurWindow( atAPPMNG_GetCurApp() );
	if(hWnd != atNULL){
		atWINDOW_PostMessage( hWnd, WNDMSG_RESUME, 0,0 );
	}
}


/** 
 @brief �ý��ۿ� Notify�� ������.
 
 �ý��ۿ� Notify�� ������.

 @param nParam1, nParam2 [in] : �̺�Ʈ�� �Ķ����
 @return ����
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
 @brief Timer Tick ȣ��.

 Timer Tick�� �����Ѵ�. 

 @return ����
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




