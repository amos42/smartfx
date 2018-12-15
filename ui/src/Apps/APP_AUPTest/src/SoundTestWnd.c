//
// SoundTestWnd.c 
//
//
//   
//
///////////////////////////////////////////////////////////////////////
 
#include "A_UIENG.h"
#include "UI_Util.h"
#include "Global.h"
#include "AEECOMP_BasePkg.h"
#include "AEECOMP_ExtPkg.h"
#include "SoundTestWnd.h"
#include <string.h>

#include "sound_data_16b.h"


static atTCHAR xx[][32] = {
	_AT("Add Item"),
	_AT("Remove Item"),
	_AT("Close")
};


static void SoundTestWndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
    LPSOUNDTESTWNDDATA lpData = (LPSOUNDTESTWNDDATA)atWINDOW_GetDataPtr(hWnd);
//    LPCONFUSERINFO lpConfUserInfo = lpData->lpConfUserInfo;    

    switch(nParam1)
    {
	  default:
		break;
	}
}


static void SoundTestWndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
    LPSOUNDTESTWNDDATA lpData = (LPSOUNDTESTWNDDATA)atWINDOW_GetDataPtr(hWnd);
//    LPCONFUSERINFO lpConfUserInfo = lpData->lpConfUserInfo;    

    switch(nParam1)
    {
        case 103 :
			atCONTROL_Refresh( lpData->hCtl1 );
			break;
	}
}

#include "SYSAPI.h"
SYS_SOUND snd;

static void SoundTestWndOnInit(atHWINDOW hWnd)
{
//    LPSOUNDTESTWNDDATA	lpData = (LPSOUNDTESTWNDDATA)atWINDOW_GetDataPtr(hWnd);

//	snd = SYSAPI_CreateSound( 1, 11025, 8, atNULL );
	snd = SYSAPI_CreateSound( 1, 22050, 16, atNULL );

	SYSAPI_SetStreamBufferSize( snd, sizeof(sound_data) );
	SYSAPI_PutSoundStream( snd, (void *)sound_data, sizeof(sound_data) );
	SYSAPI_PlaySound( snd );
}


static void SoundTestWndOnRelease(atHWINDOW hWnd)
{
//    LPSOUNDTESTWNDDATA	lpData = (LPSOUNDTESTWNDDATA)atWINDOW_GetDataPtr(hWnd);
	SYSAPI_DestroySound( snd );
}


static int SoundTestWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
    LPSOUNDTESTWNDDATA lpData = (LPSOUNDTESTWNDDATA)atWINDOW_GetDataPtr(hWnd);
	atHWINDOW hPopup;
//	atHCONTROL control;

    switch( nKeyCode )
    {
			
        case atVKEY_SOFT1:
			hPopup = ExtPkg_CreatePopupMenuWnd( hWnd, 2000, 0, 0, 200, 100, xx, 3, 3, MENU_ALIGN_LEFT | MENU_ALIGN_BOTTOM );
			SetPopupMenuKey( hPopup, atVKEY_SOFT1, atVKEY_SOFT2, atVKEY_SOFT2 );
			EnablePopupMenuItem( hPopup, 0, atFALSE );
			EnablePopupMenuItem( hPopup, 1, atFALSE );
			atWINDOW_Refresh(hPopup);
            break;
			
        case atVKEY_CLEAR:
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			break;

        default:
            return 0;
    }

    return 0; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}

static void SoundTestWndOnPaint(atHWINDOW hWnd)
{
}


int SoundTestWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	int ret = 0;

	switch( nMsg )
	{
		case WNDMSG_INIT:
			SoundTestWndOnInit( hWnd );
			break;
		case WNDMSG_RELEASE:
			SoundTestWndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			SoundTestWndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return SoundTestWndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_CHANGE:
			SoundTestWndOnChange( hWnd, nParam1, nParam2 );
			break;
		case WNDMSG_COMMAND:
			SoundTestWndOnCommand( hWnd, nParam1, nParam2 );
			break;
		default:
			return 0;
			break;
	}

	return ret;
}
