//
// Pannel.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "CMP_PaintBox.h"
#include "AEECOMP_BasePkg.h"


typedef struct _atPAINTBOX {
	int dummy;
	atLPPAINTBOX_RENDERER lpRenderer;
} atPAINTBOX,  *atLPPAINTBOX;


static int PaintBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


struct _CreateParam {
	atLPPAINTBOX_RENDERER lpRenderer;
};


atHCONTROL atPAINTBOX_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atDWORD dwAttr, atLPPAINTBOX_RENDERER lpRenderer )
{
	atHCONTROL		hControl;
	struct _CreateParam param;

	param.lpRenderer = lpRenderer;
	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(atPAINTBOX), PaintBoxProc, &param );

	return hControl;
}

static void atPAINTBOX_OnInit( atHCONTROL hControl, struct _CreateParam *param )
{
	atLPPAINTBOX lpPaintBox = (atLPPAINTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpPaintBox == atNULL ) return; 

	if(param != atNULL) {
		lpPaintBox->lpRenderer = param->lpRenderer;
	}
}

static void atPAINTBOX_OnRelease( atHCONTROL hControl )
{
//	atPAINTBOX_SetScrollPos( hControl, 0 );
}

static void atPAINTBOX_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPPAINTBOX lpPaintBox = (atLPPAINTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpPaintBox == atNULL ) return; 
	
	if( lpPaintBox->lpRenderer != atNULL ) {
		lpPaintBox->lpRenderer( hControl, hCanvas, bFocused ); 
	}
}

static int PaintBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	int ret = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			atPAINTBOX_OnInit( hControl, (struct _CreateParam *)lParam1 );
			break;
		case CTLMSG_RELEASE:
			atPAINTBOX_OnRelease( hControl );
			break;
		case CTLMSG_PAINT:
			atPAINTBOX_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			ret = 1;
			break;
	}

	return ret;
}


//===================================================

atBOOL	atPAINTBOX_SetRenderer( atHCONTROL hControl, atLPPAINTBOX_RENDERER lpRenderer )
{
	atLPPAINTBOX lpPaintBox = (atLPPAINTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpPaintBox == atNULL ) return atFALSE; 
	
	lpPaintBox->lpRenderer = lpRenderer;

	return atTRUE;
}
