//
// EqSetWnd.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "AEECOMP_BasePkg.h"
#include "AEECOMP_ExtPkg.h"
#include "EqSetWnd.h"


atTCHAR eqstr[7][32] = {
	_AT("Normal"),
	_AT("Pop"),
	_AT("Classic"),
	_AT("Rock"),
	_AT("Jazz"),
	_AT("Voice"),
	_AT("User Def"),
};

int vv[7][6] = {
	{ 20, 20, 20, 20, 20, 20  },
	{ 10, 20, 30, 40, 20, 30  },
	{ 40, 20, 30, 20, 10, 30  },
	{ 20, 10, 30, 40, 20, 30  },
	{ 5,  20, 30, 30, 20, 10  },
	{ 10, 20, 30, 40, 20, 30  },
	{ 20, 20, 20, 20, 20, 20  }
};



static void EqSetWndOnInit(atHWINDOW hWnd)
{
	LPEQSETWNDDATA lpData = (LPEQSETWNDDATA)atWINDOW_GetDataPtr(hWnd);
	atHCONTROL hSpin, hButton;

	hSpin = ExtPkg_CreateSpinBox2( hWnd, 100, 20, 10, 80, 20, eqstr, 7 );
	atCONTROL_SetAnchor( hSpin, 10, atANCHOR_IGNORE, 20, 10 );
		
	lpData->hEq[0] = ExtPkg_CreateVSlider( hWnd, 101, 10+0,   40, 16, 80, 1, 40, 20 );
	lpData->hEq[1] = ExtPkg_CreateVSlider( hWnd, 102, 10+30,  40, 16, 80, 1, 40, 20 );
	lpData->hEq[2] = ExtPkg_CreateVSlider( hWnd, 103, 10+60,  40, 16, 80, 1, 40, 20 );
	lpData->hEq[3] = ExtPkg_CreateVSlider( hWnd, 104, 10+90,  40, 16, 80, 1, 40, 20 );
	lpData->hEq[4] = ExtPkg_CreateVSlider( hWnd, 105, 10+120, 40, 16, 80, 1, 40, 20 );
	lpData->hEq[5] = ExtPkg_CreateVSlider( hWnd, 106, 10+150, 40, 16, 80, 1, 40, 20 );

	hButton = ExtPkg_CreateButton( hWnd, 107, 65, 130, 60, 18, _AT("OK") );
	atCONTROL_SetAnchor( hButton, atANCHOR_IGNORE, 10, atANCHOR_IGNORE, atANCHOR_IGNORE );

	atWINDOW_SetCurControl( hWnd, hSpin );
}


static void EqSetWndOnRelease(atHWINDOW hWnd)
{
	LPEQSETWNDDATA lpData = (LPEQSETWNDDATA)atWINDOW_GetDataPtr(hWnd);
}


static int EqSetWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPEQSETWNDDATA lpData = (LPEQSETWNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch(nKeyCode)
	{
		case atVKEY_MENU:
		case atVKEY_CLEAR:			
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );			
			break;

		default:
			return 0;			
	}

	return 1;
}

static int EqSetWndOnLongKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	LPEQSETWNDDATA lpData = (LPEQSETWNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch(nKeyCode)
	{
		default:
			return 0;			
	}

	return 1;
}

static void EqSetWndOnPaint(atHWINDOW hWnd)
{
}

static void EqSetWndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPEQSETWNDDATA lpData = (LPEQSETWNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch( nParam1 ){
	case 107:
		atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd ); 		
		break;
	}
}


static void EqSetWndOnChange(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPEQSETWNDDATA lpData = (LPEQSETWNDDATA)atWINDOW_GetDataPtr(hWnd);
	int i;

	switch( nParam1 ){
	case 100:
		for( i = 0; i < 6; i++ ){
			atVSLIDER_EXT_SetPosition( lpData->hEq[i], vv[nParam2][i] );
//			atCONTROL_Draw( lpData->hEq[i], atTRUE );
		}
		atWINDOW_Refresh(hWnd);
		break;
	}
}


int EqSetWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	switch( nMsg )
	{
		case WNDMSG_INIT:
			EqSetWndOnInit( hWnd );
			break;
		case WNDMSG_RELEASE:
			EqSetWndOnRelease( hWnd );
			break;
		case WNDMSG_REGIST:
			EqSetWndOnKeyDown(hWnd, atVKEY_SELECT);
			break;
		case WNDMSG_PAINT:
			EqSetWndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return EqSetWndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_LONGKEYDOWN:
			return EqSetWndOnLongKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_COMMAND:
			EqSetWndOnCommand( hWnd, nParam1, nParam2 );
			break;
		case WNDMSG_CHANGE:
			EqSetWndOnChange( hWnd, nParam1, nParam2 );
			break;
		default:
			return 0;
			break;
	}

	return 1;
}

