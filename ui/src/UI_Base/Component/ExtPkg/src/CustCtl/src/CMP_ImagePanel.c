//
// ImagePannel.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "CMP_ImagePanel.h"
#include "AEECOMP_BasePkg.h"


typedef struct _atPANEL {
	int dummy;
	atHBITMAP bmTopLeft, bmTopCenter, bmTopRight,
						bmMiddleLeft, bmMiddleCenter, bmMiddleRight,
						bmBottomLeft, bmBottomCenter, bmBottomRight;
} atPANEL,  *atLPPANEL;


static int PanelProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


struct _CreateParam {
	atHBITMAP bmTopLeft, bmTopCenter, bmTopRight,
						bmMiddleLeft, bmMiddleCenter, bmMiddleRight,
						bmBottomLeft, bmBottomCenter, bmBottomRight;
};


atHCONTROL atIMAGEPANEL_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atDWORD dwAttr, 
    														atHBITMAP bmTopLeft, atHBITMAP bmTopCenter, atHBITMAP bmTopRight,
    														atHBITMAP bmMiddleLeft, atHBITMAP bmMiddleCenter, atHBITMAP bmMiddleRight,
    														atHBITMAP bmBottomLeft, atHBITMAP bmBottomCenter, atHBITMAP bmBottomRight )
{
	atHCONTROL		hControl;
	struct _CreateParam param;

	param.bmTopLeft = bmTopLeft;
	param.bmTopCenter = bmTopCenter;
	param.bmTopRight = bmTopRight;
	param.bmMiddleLeft = bmMiddleLeft;
	param.bmMiddleCenter = bmMiddleCenter;
	param.bmMiddleRight = bmMiddleRight;
	param.bmBottomLeft = bmBottomLeft;
	param.bmBottomCenter = bmBottomCenter;
	param.bmBottomRight = bmBottomRight;

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(atPANEL), PanelProc, &param );

	return hControl;
}

static void atIMAGEPANEL_OnInit( atHCONTROL hControl, struct _CreateParam *param )
{
	atLPPANEL lpPanel = (atLPPANEL)atCONTROL_GetDataPtr( hControl );

	lpPanel->bmTopLeft = param->bmTopLeft;
	lpPanel->bmTopCenter = param->bmTopCenter;
	lpPanel->bmTopRight = param->bmTopRight;
	lpPanel->bmMiddleLeft = param->bmMiddleLeft;
	lpPanel->bmMiddleCenter = param->bmMiddleCenter;
	lpPanel->bmMiddleRight = param->bmMiddleRight;
	lpPanel->bmBottomLeft = param->bmBottomLeft;
	lpPanel->bmBottomCenter = param->bmBottomCenter;
	lpPanel->bmBottomRight = param->bmBottomRight;
}

static void atIMAGEPANEL_OnRelease( atHCONTROL hControl )
{
}

static void atIMAGEPANEL_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	atLPPANEL lpPanel = (atLPPANEL)atCONTROL_GetDataPtr( hControl );
	atREGION rgn;

	atCONTROL_GetClientRegion( hControl, &rgn );

	if( lpPanel->bmTopLeft && lpPanel->bmTopCenter && lpPanel->bmTopRight ){
		if( lpPanel->bmMiddleLeft && lpPanel->bmMiddleCenter && lpPanel->bmMiddleRight &&
			lpPanel->bmBottomLeft && lpPanel->bmBottomCenter && lpPanel->bmBottomRight ){
				atUIAPI_DrawClient( hCanvas, rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight,
					lpPanel->bmTopLeft,    lpPanel->bmTopCenter,    lpPanel->bmTopRight,
					lpPanel->bmMiddleLeft, lpPanel->bmMiddleCenter, lpPanel->bmMiddleRight,
					lpPanel->bmBottomLeft, lpPanel->bmBottomCenter, lpPanel->bmBottomRight,
					atTRUE );
		} else {
				atUIAPI_DrawClient1Line( hCanvas, rgn.nX, rgn.nY, rgn.nWidth,
					lpPanel->bmTopLeft,    lpPanel->bmTopCenter,    lpPanel->bmTopRight );
		}
	}
}

static int PanelProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	int ret = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			atIMAGEPANEL_OnInit( hControl, (struct _CreateParam *)lParam1 );
			break;
		case CTLMSG_RELEASE:
			atIMAGEPANEL_OnRelease( hControl );
			break;
		case CTLMSG_PAINT:
			atIMAGEPANEL_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			ret = 1;
			break;
	}

	return ret;
}


//===================================================

atBOOL	atIMAGEPANEL_AddControl( atHCONTROL hControl, atHCONTROL hChildControl )
{
	atLPPANEL lpPanel = (atLPPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return atFALSE;

	atCONTROL_AddChildControl( hControl, hChildControl );

	return atTRUE;
}

atVOID	atIMAGEPANEL_SetControlFocus( atHCONTROL hControl, atHCONTROL hChildControl )
{
	atLPPANEL lpPanel = (atLPPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return;

	atCONTROL_SetCurChildControl( hControl, hChildControl );
}

atHCONTROL atIMAGEPANEL_GetControlFocus( atHCONTROL hControl )
{
	atLPPANEL lpPanel = (atLPPANEL)atCONTROL_GetDataPtr( hControl );
	if( lpPanel == atNULL ) return atNULL;

	return atCONTROL_GetCurChildControl( hControl );
}
