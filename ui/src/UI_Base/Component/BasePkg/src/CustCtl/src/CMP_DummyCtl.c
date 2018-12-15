//
// DummyCtl.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "CMP_DummyCtl.h"


typedef struct _atDUMMYCTL {
	int dummy;
} atDUMMYCTL,  *atLPDUMMYCTL;


static int DummyCtlProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );



atHCONTROL atDUMMYCTL_Create( int nIndex, int nX, int nY, int nWidth, int nHeight )
{
	atHCONTROL		hControl;
	atLPCONTROL		lpControl;
	atLPDUMMYCTL	lpDummyCtl;

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, 0, atNULL, atNULL );
	lpControl = atCONTROL_GetPtr(hControl);
	
	if ( lpControl != atNULL ){
//		atCONTROL_SetFocusable( hControl, atFALSE );

		lpDummyCtl = (atLPDUMMYCTL)UI_MEMALLOC(sizeof(atDUMMYCTL));
		if ( lpDummyCtl == atNULL ){
			UI_MEMFREE(lpDummyCtl);
			return atNULL;
		}
		atUIAPI_memset(lpDummyCtl, 0, sizeof(atDUMMYCTL));

		lpDummyCtl->dummy = 0;
			
		lpControl->Base.nObjDataSize = sizeof(atDUMMYCTL);
		lpControl->Base.lpObjData = lpDummyCtl;
		lpControl->Base.fnObjProc = DummyCtlProc;
	}

	return hControl;
}

static void atDUMMYCTL_OnInit( atHCONTROL hControl )
{
}

static void atDUMMYCTL_OnRelease( atHCONTROL hControl )
{
}

static void atDUMMYCTL_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
}

static int atDUMMYCTL_OnKeyDown( atHCONTROL hControl, atINT nKeyCode )
{
	return 0;
}


static int DummyCtlProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	int ret = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			atDUMMYCTL_OnInit( hControl );
			break;
		case CTLMSG_RELEASE:
			atDUMMYCTL_OnRelease( hControl );
			break;
		case CTLMSG_PAINT:
			atDUMMYCTL_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			break;
		case CTLMSG_KEYDOWN:
		       ret = atDUMMYCTL_OnKeyDown( hControl, lParam1 );
			break;
	}

	return ret;
}
