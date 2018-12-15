//
// TestWnd.c 
//
//
//   
//
///////////////////////////////////////////////////////////////////////
 
#include "A_UIENG.h"
#include "UI_Util.h"
#include "Global.h"
#include "AEECOMP_BasePkg.h"
#include "PaintBoxTestWnd.h"
#include <string.h>


static atVOID xyz( atHCONTROL hControl, atHGDC hCanvas, atBOOL bFocused )
{
	atREGION rgn;

	atCONTROL_GetControlRegion( hControl, &rgn );
		
	atUIAPI_FillRect( hCanvas, 0, 0, rgn.nWidth, rgn.nHeight, atUI_COLOR_RED );
	atUIAPI_DrawRect( hCanvas, 0, 0, rgn.nWidth, rgn.nHeight, atUI_COLOR_BLUE, 1 );
	atUIAPI_DrawLine( hCanvas, 0, 0, rgn.nWidth-1, rgn.nHeight-1, atUI_COLOR_WHITE, 1, atFALSE );
	atUIAPI_DrawLine( hCanvas, rgn.nWidth-1, 0, 0, rgn.nHeight-1, atUI_COLOR_WHITE, 1, atFALSE );
}

static void _OnInit(atHWINDOW hWnd)
{
//    LPTESTWNDDATA	lpData = (LPTESTWNDDATA)atWINDOW_GetDataPtr(hWnd);

	BasePkg_CreatePaintBox( hWnd, 100, 20, 20, 120, 100, xyz );
	BasePkg_CreatePaintBox( hWnd, 200, 50, 80, 120, 100, xyz );
	BasePkg_CreatePaintBox( hWnd, 300, 80, 140, 120, 100, xyz );
	BasePkg_CreatePaintBox( hWnd, 400, 110, 200, 120, 100, xyz );
}



static void _OnRelease(atHWINDOW hWnd)
{
//    LPPAINTBOXTESTWNDDATA	lpData = (LPPAINTBOXTESTWNDDATA)atWINDOW_GetDataPtr(hWnd);

}


static int _OnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
    LPPAINTBOXTESTWNDDATA lpData = (LPPAINTBOXTESTWNDDATA)atWINDOW_GetDataPtr(hWnd);

    switch( nKeyCode )
    {
			
        case atVKEY_SOFT1:
            break;
			
        case atVKEY_SOFT2:
            break;

        case atVKEY_CLEAR:
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
            break;
			
        default:
            return 0;
    }

    return 0; // WindowOnEvent 가 호출되지 않도록 0을 리턴한다.
}


static void _OnPaint(atHWINDOW hWnd)
{
	atHGDC hCanvas = atWINDOW_GetClientGDC(hWnd);

	//atUIAPI_DrawLine( hCanvas, -6, 23, 386, -14, atUI_COLOR_RED, 1, atFALSE );
}


int PaintBoxTestWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	int ret = 0;

	switch( nMsg )
	{
		case WNDMSG_INIT:
			_OnInit( hWnd );
			break;
		case WNDMSG_RELEASE:
			_OnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			_OnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return _OnKeyDown( hWnd, nParam1 );
			break;
		default:
			return 0;
			break;
	}

	return ret;
}
