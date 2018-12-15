//
// TransPannel.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "CMP_TransPanel.h"
#include "AEECOMP_BasePkg.h"


typedef struct _atPANEL {
	int dummy;
	atREAL rTransparent;
} atPANEL,  *atLPPANEL;


static int PanelProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


struct _CreateParam {
	atREAL rTransparent;
};


atHCONTROL atTRANSPANEL_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atDWORD dwAttr, atREAL rTransparent )
{
	atHCONTROL		hControl;
	struct _CreateParam param;

	param.rTransparent = rTransparent;

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(atPANEL), PanelProc, &param );

	return hControl;
}

static void atTRANSPANEL_OnInit( atHCONTROL hControl, struct _CreateParam *param )
{
	atLPPANEL lpPanel = (atLPPANEL)atCONTROL_GetDataPtr( hControl );

	lpPanel->rTransparent = param->rTransparent;
}

static void atTRANSPANEL_OnRelease( atHCONTROL hControl )
{
//	atPANEL_SetScrollPos( hControl, 0 );
}

static void atTRANSPANEL_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	atLPPANEL lpPanel = (atLPPANEL)atCONTROL_GetDataPtr( hControl );
	atREGION rgn;

	atCONTROL_PrepareGDC( hControl, hCanvas, bFocused );

	atCONTROL_GetClientRegion( hControl, &rgn );
		
	atUIAPI_FillRectTrans( hCanvas, rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight, lpControl->tControlColor.clNormalBgColor, REALTOINT(lpPanel->rTransparent * 256 / 100) );
}

static int PanelProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	int ret = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			atTRANSPANEL_OnInit( hControl, (struct _CreateParam *)lParam1 );
			break;
		case CTLMSG_RELEASE:
			atTRANSPANEL_OnRelease( hControl );
			break;
		case CTLMSG_PAINT:
			atTRANSPANEL_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			ret = 1;
			break;
	}

	return ret;
}


//===================================================

atBOOL	atTRANSPANEL_AddControl( atHCONTROL hControl, atHCONTROL hChildControl )
{
	atLPPANEL lpPanel = (atLPPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return atFALSE;

	atCONTROL_AddChildControl( hControl, hChildControl );

	return atTRUE;
}

atVOID	atTRANSPANEL_SetControlFocus( atHCONTROL hControl, atHCONTROL hChildControl )
{
	atLPPANEL lpPanel = (atLPPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return;

	atCONTROL_SetCurChildControl( hControl, hChildControl );
}

atHCONTROL atTRANSPANEL_GetControlFocus( atHCONTROL hControl )
{
	atLPPANEL lpPanel = (atLPPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return atNULL;

	return atCONTROL_GetCurChildControl( hControl );
}
