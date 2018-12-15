//
// Pannel.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "CMP_Panel.h"
#include "AEECOMP_BasePkg.h"


typedef struct _atPANEL {
	int dummy;
	int nBorderWidth;
} atPANEL,  *atLPPANEL;


static int PanelProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


struct _CreateParam {
	int nBorderWidth;
};


atHCONTROL atPANEL_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atDWORD dwAttr, int nBorderWidth )
{
	atHCONTROL		hControl;
	struct _CreateParam param;

	param.nBorderWidth = nBorderWidth;

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(atPANEL), PanelProc, &param );

	return hControl;
}

static void atPANEL_OnInit( atHCONTROL hControl, struct _CreateParam *param )
{
	atLPPANEL lpPanel = (atLPPANEL)atCONTROL_GetDataPtr( hControl );

	lpPanel->nBorderWidth = param->nBorderWidth;
}

static void atPANEL_OnRelease( atHCONTROL hControl )
{
//	atPANEL_SetScrollPos( hControl, 0 );
}

static void atPANEL_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
/*
	atREGION rgn;

	atCONTROL_GetClientRegion( hControl, &rgn );
		
	atUIAPI_FillRect( hCanvas, rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight, atUI_COLOR_BLUE );
	atUIAPI_DrawRect( hCanvas, rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight, atUI_COLOR_WHITE, 1 );
	atUIAPI_DrawLine( hCanvas, rgn.nX, rgn.nY, rgn.nWidth-1, rgn.nHeight-1, atUI_COLOR_WHITE, 1, atFALSE );
	atUIAPI_DrawLine( hCanvas, rgn.nWidth-1, rgn.nY, rgn.nX, rgn.nHeight-1, atUI_COLOR_WHITE, 1, atFALSE );
*/	
}

static int PanelProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	int ret = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			atPANEL_OnInit( hControl, (struct _CreateParam *)lParam1 );
			break;
		case CTLMSG_RELEASE:
			atPANEL_OnRelease( hControl );
			break;
		case CTLMSG_PAINT:
			atPANEL_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			ret = 1;
			break;
	}

	return ret;
}


//===================================================

atBOOL	atPANEL_AddControl( atHCONTROL hControl, atHCONTROL hChildControl )
{
	atLPPANEL lpPanel = (atLPPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return atFALSE;

	atCONTROL_AddChildControl( hControl, hChildControl );

	return atTRUE;
}

atVOID	atPANEL_SetControlFocus( atHCONTROL hControl, atHCONTROL hChildControl )
{
	atLPPANEL lpPanel = (atLPPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return;

	atCONTROL_SetCurChildControl( hControl, hChildControl );
}

atHCONTROL atPANEL_GetControlFocus( atHCONTROL hControl )
{
	atLPPANEL lpPanel = (atLPPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return atNULL;

	return atCONTROL_GetCurChildControl( hControl );
}
