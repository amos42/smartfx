//
// MenuWnd.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_ScrollBarEx.h"


#define DEFAULT_CURSOR_MIN_SIZE 			20

#define CHECK_ACCELTICK_TIME_LIMIT			10		// 스무스 스크롤을 스무스 랜더링하기 위한 Timer
#define CHECK_ACCELMOVE_TIME_LIMIT			500		// 스무스 스크롤에 대한 이동 제한 시간 (이후 스무스 스크롤 초기화)

#define ACCEL_VALUE_START_VELOCITY		(200.0f) //(atREAL)0x01900000//26214400//(400.0f*65536)	// 최대 속도(속력)
#define ACCEL_VALUE_MAX_VELOCITY			(8000.0f) //(atREAL)0x01900000//26214400//(400.0f*65536)	// 최대 속도(속력)
//#define ACCEL_VALUE_DEACCEL_FACTOR		(0.90f)
#define ACCEL_VALUE_FRICTION_FACTOR		(-0.10f)
#define ACCEL_VALUE_FRICTION_FACTOR_2	(-0.60f)
#define ACCEL_VALUE_STICK_FACTOR		(0.30f)
//#define ACCEL_VALUE_DEACCEL_FACTOR		(0.90f)

typedef struct _atSCROLLBAR {
	atSCROLLBAR_EXT_TYPE style;
	
	int	nSize;
	int nPosition;

	int nCursorX;
	int nCursorTopMargin;
	int nCursorBottomMargin;
	int nCursorMinSize;

	atHBITMAP	lpScrollbarImgTop;
	atHBITMAP	lpScrollbarImage;
	atHBITMAP	lpScrollbarImgBottom;
	//atHBITMAP	lpScrollcursorImage;
	atHBITMAP	lpScrollcursorImgTop;
	atHBITMAP	lpScrollcursorImgMid;
	atHBITMAP	lpScrollcursorImgBot;

	atREGION tCursorRgn;
	atBOOL	bActionValidate;
	// Kyle Start [Add 2009/06/22-14:25]
	atINT	nFirstMouseY;
	atINT	nFirstViewTopY;
	atINT	nTouchCursorPosY;
	atBOOL	bUsedTouch;		// 터치 사용여부	
	atINT	nHeight;
	// Kyle [Add 2009/06/22-14:25] End

	atBOOL 	bGestureScroll;
	atINT 	nGestureStartY;
	atINT	nGestureCurMouseY;
	atINT 	nGesturePrevY;
	atINT 	nGestureCurY;
	atLONG 	nGesturePrevTime;
	atLONG 	nGestureCurTime;
	atFLOAT fGestureVelocity;
	atFLOAT fGestureStickFactor;
	
	atBOOL 	bUseInertiaScroll;

	atBOOL 	bProcessInertiaScroll;

	atBOOL		bUseSmoothScroll;		// 스무스 스크롤 사용여부
	
	atINT		nMaxPos;
	atINT		nScrollMode;
} atSCROLLBAR,  *atLPSCROLLBAR;


//static void atSCROLLBAR_EXT_OnRelease( atHCONTROL hControl );
//static void atSCROLLBAR_EXT_OnDraw( atHGDC hCanvas, atHCONTROL hControl, int nX, int nY, atBOOL bFocused );
static void recalc_cursor_rect( atHCONTROL hControl, atLPSCROLLBAR lpScrollBar );
static int ScrollBarProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );


struct _CreateParam {
	 atSCROLLBAR_EXT_TYPE style;
	int nCursorX;
	int nCursorTopMargin;
	int nCursorBottomMargin;
	int nCursorMinSize;
	 atHBITMAP lpScrollbarImgTop, lpScrollbarImage, lpScrollbarImgBottom;
	 atHBITMAP lpScrollcursorImgTop, lpScrollcursorImgMid, lpScrollcursorImgBot;
};



atHCONTROL atSCROLLBAR_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atSCROLLBAR_EXT_TYPE style, 	
								 int nCursorX, int nCursorTopMargin, int nCursorBottomMargin, int nCursorMinSize,
					  			 atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom, 
					  			 atHBITMAP lpScrollcursorImgTop, atHBITMAP lpScrollcursorImgMid, atHBITMAP lpScrollcursorImgBot )
{
	atHCONTROL		hControl;
	struct _CreateParam param;

	param.style = style;
	param.nCursorX = nCursorX;
	param.nCursorTopMargin = nCursorTopMargin;
	param.nCursorBottomMargin = nCursorBottomMargin;
	param.nCursorMinSize = nCursorMinSize;
	param.lpScrollbarImgTop	= lpScrollbarImgTop;
	param.lpScrollbarImage		= lpScrollbarImage;
	param.lpScrollbarImgBottom	= lpScrollbarImgBottom;
	param.lpScrollcursorImgTop	= lpScrollcursorImgTop;
	param.lpScrollcursorImgMid	= lpScrollcursorImgMid;
	param.lpScrollcursorImgBot	= lpScrollcursorImgBot;

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(atSCROLLBAR), ScrollBarProc, &param );

	return hControl;
}



static atBOOL scr_to_pos( atHCONTROL hControl, atLPSCROLLBAR lpScrollBar, int scr_y, int *pos_y )
{
	int win_height;
	atREGION rgn;
	int v;
	
	atCONTROL_GetControlRegion(hControl, &rgn);
	win_height = rgn.nHeight - lpScrollBar->nCursorTopMargin - lpScrollBar->nCursorBottomMargin;

	if( lpScrollBar->style == atSCROLLBAR_EXT_TYPE_NORMAL ){
		win_height -= lpScrollBar->tCursorRgn.nHeight;
		v = scr_y * lpScrollBar->nSize / win_height;
	} else if( (lpScrollBar->style == atSCROLLBAR_EXT_TYPE_WINDOW) ||
			   (lpScrollBar->style == atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION) ||
			   (lpScrollBar->style == atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION_STICK) ){
		v = scr_y * lpScrollBar->nSize / win_height;
	}

	*pos_y = v;

	return atTRUE;
}

static atBOOL pos_to_scr( atHCONTROL hControl, atLPSCROLLBAR lpScrollBar, int pos_y, int *scr_y )
{
	int win_height;
	atREGION rgn;
	int v;

	atCONTROL_GetControlRegion(hControl, &rgn);
	if( lpScrollBar->nSize <= rgn.nHeight ) return atFALSE;

	win_height = rgn.nHeight - lpScrollBar->nCursorTopMargin - lpScrollBar->nCursorBottomMargin;

	if( lpScrollBar->style == atSCROLLBAR_EXT_TYPE_NORMAL ){
		win_height -= lpScrollBar->tCursorRgn.nHeight;
		v = pos_y * win_height / lpScrollBar->nSize;
	} else if( (lpScrollBar->style == atSCROLLBAR_EXT_TYPE_WINDOW) ||
			   (lpScrollBar->style == atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION) ||
			   (lpScrollBar->style == atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION_STICK) ){
		v = pos_y * win_height / lpScrollBar->nSize;
	}

	*scr_y = v;

	return atTRUE;
}


static void recalc_cursor_size( atHCONTROL hControl, atLPSCROLLBAR lpScrollBar )
{
	int win_height, cursor_height = 0;
	int style = lpScrollBar->style;
	int size = lpScrollBar->nSize;
	int pos = lpScrollBar->nPosition;
	atREGION rgn;
	atBOOL r;

	if( size <= 0 ) return;

	atCONTROL_GetControlRegion(hControl, &rgn);

	if( style == atSCROLLBAR_EXT_TYPE_NORMAL ){
		size = lpScrollBar->nCursorMinSize;
		if( size < DEFAULT_CURSOR_MIN_SIZE ) size = DEFAULT_CURSOR_MIN_SIZE;
		lpScrollBar->tCursorRgn.nWidth = rgn.nWidth-2;
		lpScrollBar->tCursorRgn.nHeight = size;
	} else if( (style == atSCROLLBAR_EXT_TYPE_WINDOW) ||
			 (style == atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION) ||
			 (style == atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION_STICK) ){
		win_height = rgn.nHeight - lpScrollBar->nCursorTopMargin - lpScrollBar->nCursorBottomMargin;
		//cursor_height = win_height * win_height / size;
		r = pos_to_scr( hControl, lpScrollBar, win_height, &cursor_height );
		if( r ){
			if( cursor_height < DEFAULT_CURSOR_MIN_SIZE ) cursor_height = DEFAULT_CURSOR_MIN_SIZE;
			lpScrollBar->tCursorRgn.nWidth = rgn.nWidth-2;
			lpScrollBar->tCursorRgn.nHeight = cursor_height;
		} else {
			lpScrollBar->tCursorRgn.nWidth = rgn.nWidth-2;
			lpScrollBar->tCursorRgn.nHeight = win_height;
		}
	}
}

static void recalc_cursor_pos( atHCONTROL hControl, atLPSCROLLBAR lpScrollBar )
{
	int size = lpScrollBar->nSize;
	int pos = lpScrollBar->nPosition;
	int posY;

	if( !pos_to_scr(hControl, lpScrollBar, pos, &posY) ) return;	

	lpScrollBar->tCursorRgn.nX = lpScrollBar->nCursorX;
	lpScrollBar->tCursorRgn.nY = posY;
}

static void recalc_cursor_rect( atHCONTROL hControl, atLPSCROLLBAR lpScrollBar )
{
	recalc_cursor_size( hControl, lpScrollBar );
	recalc_cursor_pos( hControl, lpScrollBar );
}


static void atSCROLLBAR_EXT_OnInit( atHCONTROL hControl, struct _CreateParam *param )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLBAR lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr( hControl );

	lpScrollBar->style = param->style;
	lpScrollBar->nCursorX = param->nCursorX;
	lpScrollBar->nCursorTopMargin = param->nCursorTopMargin;
	lpScrollBar->nCursorBottomMargin = param->nCursorBottomMargin;
	lpScrollBar->nCursorMinSize = param->nCursorMinSize;
	lpScrollBar->lpScrollbarImgTop	= param->lpScrollbarImgTop;
	lpScrollBar->lpScrollbarImage		= param->lpScrollbarImage;
	lpScrollBar->lpScrollbarImgBottom	= param->lpScrollbarImgBottom;
	lpScrollBar->lpScrollcursorImgTop	= param->lpScrollcursorImgTop;
	lpScrollBar->lpScrollcursorImgMid	= param->lpScrollcursorImgMid;
	lpScrollBar->lpScrollcursorImgBot	= param->lpScrollcursorImgBot;

	lpScrollBar->nSize = 0;
	lpScrollBar->nPosition = 0;		
	SET_REGION( lpScrollBar->tCursorRgn, 0, 0, 0, 0 );

	lpScrollBar->bActionValidate = atFALSE;
	//lpScrollBar->bUsedTouch = atFALSE; // 기본으로 터치 불가능
	lpScrollBar->bUsedTouch = atTRUE; // 기본으로 터치 가능

	lpScrollBar->bProcessInertiaScroll = atFALSE;

	lpScrollBar->bGestureScroll = atFALSE;
	lpScrollBar->nGestureStartY = 0;
	lpScrollBar->nGestureCurY = lpScrollBar->nGesturePrevY = 0;
	lpScrollBar->nGestureCurTime = lpScrollBar->nGesturePrevTime = 0;

	lpScrollBar->fGestureVelocity = 0;
	lpScrollBar->fGestureStickFactor = 0;

	lpScrollBar->nScrollMode = 0;
	lpScrollBar->nMaxPos = 0;
	
	atCONTROL_AddControlTimer( hControl, 0x1000, CHECK_ACCELTICK_TIME_LIMIT, atTRUE, atFALSE, atFALSE );
	
}

static void atSCROLLBAR_EXT_OnRelease( atHCONTROL hControl )
{
//	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
//	atLPSCROLLBAR lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr( hControl );

}


static void atSCROLLBAR_EXT_OnDraw( atHGDC hCanvas, atHCONTROL hControl, int nX, int nY, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
//	int win_height;
	int size = lpScrollBar->nSize;
	int pos = lpScrollBar->nPosition;
//	int posY;
	atRECT rect;
	int sx, sy;

//	if( lpDC == atNULL ) return;

	atCONTROL_GetControlRect(hControl, &rect);
//	win_height = RECT_HEIGHT(rect) - lpScrollBar->nCursorTopMargin - lpScrollBar->nCursorBottomMargin;

	sx = nX + lpScrollBar->nCursorX;
	sy = nY + lpScrollBar->nCursorTopMargin;

//	if ( bFocused )
//	{
		if( lpScrollBar->lpScrollbarImage )	{
			atUIAPI_DrawClient1LineVert( hCanvas, nX, nY, RECT_HEIGHT(rect),
										  lpScrollBar->lpScrollbarImgTop, lpScrollBar->lpScrollbarImage, lpScrollBar->lpScrollbarImgBottom );
		} else	{
			atUIAPI_FillRect(hCanvas, nX+1, nY+1, RECT_WIDTH(rect)-2, RECT_HEIGHT(rect)-2, lpControl->tControlColor.clNormalBgColor );
			if( lpControl->Base.dwAttr & atCS_DRAW_BORDER )
				atUIAPI_DrawRect(hCanvas, nX, nY, RECT_WIDTH(rect), RECT_HEIGHT(rect), lpControl->tControlColor.clNormalFrColor, 1);
		}

		if( atCONTROL_IsEnable(hControl) ){
			if( size > 1 ){
				if( lpScrollBar->lpScrollcursorImgTop && lpScrollBar->lpScrollcursorImgMid && lpScrollBar->lpScrollcursorImgBot )
				{
					atUIAPI_DrawClient1LineVert( hCanvas, lpScrollBar->tCursorRgn.nX, lpScrollBar->tCursorRgn.nY, lpScrollBar->tCursorRgn.nHeight,
												lpScrollBar->lpScrollcursorImgTop, lpScrollBar->lpScrollcursorImgMid, lpScrollBar->lpScrollcursorImgBot );
				} else {
					atUIAPI_DrawRect( hCanvas, lpScrollBar->tCursorRgn.nX, lpScrollBar->tCursorRgn.nY, lpScrollBar->tCursorRgn.nWidth, lpScrollBar->tCursorRgn.nHeight, UI_COLOR_BLACK, 1);
				}
			} else {
			}
		}
//	}
//	else
//	{
//		atUIAPI_DrawRect(rect.nStartX, rect.nStartY, rect.nEndX-rect.nStartX+1, rect.nEndY-rect.nStartY+1, UI_COLOR_GRAY);
//	}
}


static int atSCROLLBAR_EXT_OnMouseDown( atHCONTROL hControl, atINT nButton, atINT nX, atINT nY )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	int vv;
	int ret = 0;
	
	if( lpScrollBar->nSize > 1 ) {
		if(lpScrollBar->bUsedTouch) {	// 터치 사용설정이 False 이면 리턴
			lpScrollBar->nFirstMouseY = nY;
			lpScrollBar->nFirstViewTopY = lpScrollBar->nPosition;
			lpScrollBar->nTouchCursorPosY = nY - lpScrollBar->tCursorRgn.nY;

			if( nX >= lpScrollBar->tCursorRgn.nX && nX < (lpScrollBar->tCursorRgn.nX+lpScrollBar->tCursorRgn.nWidth) &&
				nY >= lpScrollBar->tCursorRgn.nY && nY < (lpScrollBar->tCursorRgn.nY+lpScrollBar->tCursorRgn.nHeight) ) {
				lpScrollBar->bActionValidate = atTRUE;
//				atCONTROL_Refresh(hControl);
			} else {
				nY -= lpScrollBar->tCursorRgn.nHeight/2;
				if( scr_to_pos( hControl, lpScrollBar, nY, &vv ) ){
					atSCROLLBAR_EXT_SetPosition(hControl, vv);
				}
			}
		}

		ret = 1;
	}
	
	return ret;
}


static int atSCROLLBAR_EXT_OnMouseMove( atHCONTROL hControl, atINT nButton, atINT nX, atINT nY )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	int vv;
	int ret = 0;
	
	if( lpScrollBar->bActionValidate ){
		if( scr_to_pos( hControl, lpScrollBar, nY - lpScrollBar->nTouchCursorPosY, &vv ) ){
			if( vv < 0 ) vv = 0;
			if( vv > lpScrollBar->nMaxPos ) vv = lpScrollBar->nMaxPos;
			atSCROLLBAR_EXT_SetPosition(hControl, vv);
			ret = 1;
		}
	}

	return ret;
}


static int atSCROLLBAR_EXT_OnMouseUp( atHCONTROL hControl, atINT nButton, atINT nX, atINT nY )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	int ret = 0;
	
	lpScrollBar->bActionValidate = atFALSE;
	atCONTROL_Refresh(hControl);

	ret = 1;
			
	return ret;
}


static void atSCROLLBAR_EXT_OnTimer( atHCONTROL hControl, atLONG id )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	atINT nDeltaDistance;
	atBOOL is_stop;
	atFLOAT fVelocity;//, fStick;
	atLONG dt;
	atFLOAT fAcc;

	int height = atSCROLLBAR_EXT_GetMaxPosition(hControl);

	if( lpScrollBar == atNULL ) return;

	if( lpScrollBar->nPosition < 0 ){
		lpScrollBar->fGestureStickFactor = (float)(0 - lpScrollBar->nPosition) / 2;
		lpScrollBar->nScrollMode = 1;
	} else if( lpScrollBar->nPosition > height ){
		lpScrollBar->fGestureStickFactor = (float)(height - lpScrollBar->nPosition) / 2;
		lpScrollBar->nScrollMode = 2;
	} else {
		lpScrollBar->fGestureStickFactor = 0;
		lpScrollBar->nScrollMode = 0;
	}

	fVelocity = (float)lpScrollBar->fGestureVelocity;

	dt = atUIAPI_GetCurrTime() - lpScrollBar->nGestureCurTime;
	if( lpScrollBar->nScrollMode == 0 && ABS(fVelocity * dt) > 1000 ){
		nDeltaDistance = (int)(fVelocity * dt / 1000);
	} else if( lpScrollBar->nScrollMode == 1 ){
		nDeltaDistance = (0 - lpScrollBar->nPosition) / 3;
	} else if( lpScrollBar->nScrollMode == 2 ){
		nDeltaDistance = (height - lpScrollBar->nPosition) / 3;
	} else {
		nDeltaDistance = 0;
		is_stop = atTRUE;
	}

	if( lpScrollBar->nScrollMode == 1 ){
		fAcc = fVelocity * ACCEL_VALUE_FRICTION_FACTOR_2;
	} else if( lpScrollBar->nScrollMode == 2 ){
		fAcc = fVelocity * ACCEL_VALUE_FRICTION_FACTOR_2;
	} else {	
		fAcc = fVelocity * ACCEL_VALUE_FRICTION_FACTOR;
	}
	fVelocity += fAcc;

	lpScrollBar->fGestureVelocity = fVelocity;
	lpScrollBar->nGestureCurTime = atUIAPI_GetCurrTime();

	is_stop = atFALSE;
	if( ABS(nDeltaDistance) > 1 ){
		int pos, pos2;
	
		pos = atSCROLLBAR_EXT_GetPosition( hControl );	
		pos2 = atSCROLLBAR_EXT_Scroll( hControl, nDeltaDistance );	
		atCONTROL_Refresh( hControl );
			
		is_stop = (pos == pos2);
	} else {
		if( lpScrollBar->nScrollMode == 0 ) is_stop = atTRUE;
	}

	if( lpScrollBar->nScrollMode == 1 && ABS(nDeltaDistance) < 2 ){
		atSCROLLBAR_EXT_SetPosition( hControl, 0 );	
		atCONTROL_Refresh( hControl );
		is_stop = atTRUE;
	} else if( lpScrollBar->nScrollMode == 2 && ABS(nDeltaDistance) < 2 ){
		atSCROLLBAR_EXT_SetPosition( hControl, height );	
		atCONTROL_Refresh( hControl );
		is_stop = atTRUE;
	} else {
		//is_stop = atTRUE;
	}

	if( is_stop ){
		atCONTROL_StopControlTimer( hControl, 0x1000 );
		lpScrollBar->bProcessInertiaScroll = atFALSE;
	}
}



static int ScrollBarProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	int r = 0;
	int x, y;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			atSCROLLBAR_EXT_OnInit( hControl, (struct _CreateParam *)lParam1 );
			r = 1;
			break;
		case CTLMSG_RELEASE:
			atSCROLLBAR_EXT_OnRelease( hControl );
			r = 1;
			break;
		case CTLMSG_RESIZE:
			recalc_cursor_rect( hControl, lpScrollBar );
			lpScrollBar->nMaxPos = atSCROLLBAR_EXT_GetMaxPosition(hControl);
			r = 1;
			break;
		case CTLMSG_PAINT:
			atSCROLLBAR_EXT_OnDraw( atCONTROL_GetGDC(hControl), hControl, 0, 0, (atBOOL)lParam2 );
			r = 1;
			break;
		case CTLMSG_KEYDOWN:
			switch( lParam1 ){
				case atVKEY_UP:
//				case atVKEY_LEFT:
					atSCROLLBAR_EXT_Scroll( hControl, -1 );
					r = 1;
					break;
				case atVKEY_DOWN:
//				case atVKEY_RIGHT:
					atSCROLLBAR_EXT_Scroll( hControl, 1 );
					r = 1;
					break;
			}
			break;
		case CTLMSG_MOUSEDOWN:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			r = atSCROLLBAR_EXT_OnMouseDown( hControl, lParam1, x, y );
			break;
		case CTLMSG_MOUSEMOVE:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			r = atSCROLLBAR_EXT_OnMouseMove( hControl, lParam1, x, y );
			break;
		case CTLMSG_MOUSEUP:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			r = atSCROLLBAR_EXT_OnMouseUp( hControl, lParam1, x, y );
			break;
		case CTLMSG_TIMER:
			atSCROLLBAR_EXT_OnTimer( hControl, lParam1 );
			break;
	}

	return r;
}



// ============================================================


int atSCROLLBAR_EXT_GetSize( atHCONTROL hControl )
{
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	if( lpScrollBar == atNULL ) return 0;

	return lpScrollBar->nSize;
}

void atSCROLLBAR_EXT_SetSize( atHCONTROL hControl, int size )
{
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	if( lpScrollBar == atNULL ) return;

	if( lpScrollBar->nSize == size ) return;

	lpScrollBar->nSize = size;
	if( lpScrollBar->nSize < 0 ) lpScrollBar->nSize = 0;
	if( lpScrollBar->nPosition >= lpScrollBar->nSize ) lpScrollBar->nPosition = lpScrollBar->nSize - 1;
	if( lpScrollBar->nPosition < 0 ) lpScrollBar->nPosition = 0;

	recalc_cursor_rect( hControl, lpScrollBar );
	lpScrollBar->nMaxPos = atSCROLLBAR_EXT_GetMaxPosition( hControl );
}

int atSCROLLBAR_EXT_GetPosition( atHCONTROL hControl )
{
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	if( lpScrollBar == atNULL ) return 0;

	return lpScrollBar->nPosition;
}

int atSCROLLBAR_EXT_SetPosition( atHCONTROL hControl, int pos )
{
	int height, win_height;
	atREGION rgn;
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	if( lpScrollBar == atNULL ) return 0;

	if( lpScrollBar->nPosition == pos ) return pos;

	if( lpScrollBar->style == atSCROLLBAR_EXT_TYPE_NORMAL ) {
		height = lpScrollBar->nSize;
	} else if( (lpScrollBar->style == atSCROLLBAR_EXT_TYPE_WINDOW) ||
			 (lpScrollBar->style == atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION) ||
			 (lpScrollBar->style == atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION_STICK) ){
		atCONTROL_GetControlRegion( hControl, &rgn );
		win_height = rgn.nHeight - lpScrollBar->nCursorTopMargin - lpScrollBar->nCursorBottomMargin;
		height = lpScrollBar->nSize - win_height;
	} else { 
		return -1;
	}

	if( lpScrollBar->style != atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION_STICK ){
		if( pos >= height ) pos = height - 1;
		if( pos < 0 ) pos = 0;
	}
	lpScrollBar->nPosition = pos;

	atCONTROL_PostMessageToParent( hControl, CTLMSG_CHANGE, (long)atCONTROL_GetID(hControl), (long)lpScrollBar->nPosition );

	//recalc_cursor_rect( hControl, lpScrollBar );
	recalc_cursor_pos( hControl, lpScrollBar );

	return lpScrollBar->nPosition;
}

int atSCROLLBAR_EXT_GetMaxPosition( atHCONTROL hControl )
{
	int win_height, height;
	atREGION rgn;
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	if( lpScrollBar == atNULL ) return 0;

	if( lpScrollBar->style == atSCROLLBAR_EXT_TYPE_NORMAL )
		height = lpScrollBar->nSize;
	else if( (lpScrollBar->style == atSCROLLBAR_EXT_TYPE_WINDOW) ||
		(lpScrollBar->style == atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION) ||
		(lpScrollBar->style == atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION_STICK) ){
		atCONTROL_GetControlRegion( hControl, &rgn );
		win_height = rgn.nHeight - lpScrollBar->nCursorTopMargin - lpScrollBar->nCursorBottomMargin;
		height = lpScrollBar->nSize - win_height;
	} 
	
	return height;
}

int atSCROLLBAR_EXT_Scroll( atHCONTROL hControl, int value )
{
	return atSCROLLBAR_EXT_SetPosition( hControl, atSCROLLBAR_EXT_GetPosition( hControl ) + value );
}

//void atSCROLLBAR_EXT_SetBitmap(atHCONTROL hControl, atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom, atHBITMAP lpScrollcursorImage )
void atSCROLLBAR_EXT_SetBitmap(atHCONTROL hControl, atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom,
atHBITMAP lpScrollcursorImgTop, atHBITMAP lpScrollcursorImgMid, atHBITMAP lpScrollcursorImgBot )
{
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	if( lpScrollBar == atNULL ) return;

	lpScrollBar->lpScrollbarImgTop		= lpScrollbarImgTop;
	lpScrollBar->lpScrollbarImage		= lpScrollbarImage;
	lpScrollBar->lpScrollbarImgBottom	= lpScrollbarImgBottom;
	lpScrollBar->lpScrollcursorImgTop	= lpScrollcursorImgTop;
	lpScrollBar->lpScrollcursorImgMid	= lpScrollcursorImgMid;
	lpScrollBar->lpScrollcursorImgBot	= lpScrollcursorImgBot;
}

void atSCROLLBAR_EXT_SetUseTouch(atHCONTROL hControl, atBOOL bUsed)
{
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	if( lpScrollBar == atNULL ) return;

	lpScrollBar->bUsedTouch = bUsed;
}

int atSCROLLBAR_EXT_GetUseTouch(atHCONTROL hControl)
{
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	if( lpScrollBar == atNULL ) return -1;

	return (lpScrollBar->bUsedTouch == atTRUE ? 1 : 0);
}


void atSCROLLBAR_EXT_StartGestureScroll( atHCONTROL hControl, atINT nStartY )
{
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	if( lpScrollBar == atNULL ) return;

	if( lpScrollBar->nSize <= 0 ) return;

	if( lpScrollBar->bProcessInertiaScroll ){
		atCONTROL_StopControlTimer( hControl, 0x1000 );
		lpScrollBar->bProcessInertiaScroll = atFALSE;
	}

	lpScrollBar->nGestureCurMouseY = nStartY;

	lpScrollBar->nGestureCurY = lpScrollBar->nGesturePrevY = nStartY;
	lpScrollBar->nGestureCurTime = lpScrollBar->nGesturePrevTime = atUIAPI_GetCurrTime();
	lpScrollBar->nGestureStartY = nStartY;
	lpScrollBar->fGestureVelocity = 0;

	lpScrollBar->bGestureScroll = atTRUE;

	// 각각의 계산 공식 :
	// size : height = height : cursor_height
	// cursor_height = height * height / size
	// scroll_area = height - cursor_height
	// size : pos = hight : cursor_pos
	// cursor_pos = pos * height / size
}


void atSCROLLBAR_EXT_ProcessGestureScroll( atHCONTROL hControl, atINT nY )
{
	long dt;
	int dy, dy2, height; 
	float fv;
	atREGION rgn;
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	if( lpScrollBar == atNULL ) return;

	if( !lpScrollBar->bGestureScroll ) return;
	if( nY == lpScrollBar->nGestureCurY ) return;
	
	dy = lpScrollBar->nGestureCurMouseY - nY;

//	height = lpScrollBar->nCursorLimit - lpScrollBar->nCursorY - lpScrollBar->nHeight;
	atCONTROL_GetControlRegion( hControl, &rgn );
	height = rgn.nHeight;
	
//	dy2 = dy * height / lpScrollBar->nSize;
//	if( !pos_to_scr( hControl, lpScrollBar, dy, &dy2 ) ) return;
	dy2 = dy;
	if( dy2 != 0 ) atSCROLLBAR_EXT_Scroll( hControl, dy2 );

	lpScrollBar->nGestureCurMouseY = nY;


	dt = atUIAPI_GetCurrTime() - lpScrollBar->nGestureCurTime;
	if( dt > 100 ){
	    fv = (float)dy * 1000 / dt;
		if( ABS(fv) > ACCEL_VALUE_MAX_VELOCITY )
			fv = (fv > 0)? ACCEL_VALUE_MAX_VELOCITY : -ACCEL_VALUE_MAX_VELOCITY;

//		if( ABS(fv) < 200 ){
			
			lpScrollBar->fGestureVelocity = fv;

			lpScrollBar->nGesturePrevY = lpScrollBar->nGestureCurY;	
			lpScrollBar->nGestureCurY = nY;	
			lpScrollBar->nGesturePrevTime = lpScrollBar->nGestureCurTime;
			lpScrollBar->nGestureCurTime = atUIAPI_GetCurrTime();
//		}
	}
	
	//lpScrollBar->nGestureCurY = nY;	
	//lpScrollBar->nGestureCurTime = atUIAPI_GetCurrTime();

	atCONTROL_Refresh(hControl);
}


void atSCROLLBAR_EXT_EndGestureScroll( atHCONTROL hControl, atINT nEndY )
{
	int dy, dt;
	float fv;
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	if( lpScrollBar == atNULL ) return;

	lpScrollBar->bGestureScroll = atFALSE;

	dt = atUIAPI_GetCurrTime() - lpScrollBar->nGestureCurTime;
	if( dt > 100 ){
		dt = atUIAPI_GetCurrTime() - lpScrollBar->nGesturePrevTime;
		dy = nEndY - lpScrollBar->nGestureCurY;
	} else {
		dt = atUIAPI_GetCurrTime() - lpScrollBar->nGesturePrevTime;
		dy = nEndY - lpScrollBar->nGesturePrevY;
	}

	fv = (float)dy * 1000 / dt;
	if( ABS(fv) > ACCEL_VALUE_MAX_VELOCITY )
		fv = (fv > 0)? ACCEL_VALUE_MAX_VELOCITY : -ACCEL_VALUE_MAX_VELOCITY;
	lpScrollBar->fGestureVelocity = -fv;

	if( ABS(fv) >= ACCEL_VALUE_START_VELOCITY ){
		if( !lpScrollBar->bProcessInertiaScroll ){
			atCONTROL_StartControlTimer( hControl, 0x1000 );
			lpScrollBar->bProcessInertiaScroll = atTRUE;
		}
	}

lpScrollBar->nScrollMode = 0;

	if( lpScrollBar->nPosition < 0 ){
		lpScrollBar->fGestureStickFactor = (float)(0 - lpScrollBar->nPosition) / 2;
		lpScrollBar->nScrollMode = 1;
		if( !lpScrollBar->bProcessInertiaScroll ){
			atCONTROL_StartControlTimer( hControl, 0x1000 );
			lpScrollBar->bProcessInertiaScroll = atTRUE;
		}
	} else {
atREGION rgn;		
int win_height, height;
atCONTROL_GetControlRegion( hControl, &rgn );
win_height = rgn.nHeight - lpScrollBar->nCursorTopMargin - lpScrollBar->nCursorBottomMargin;
height = lpScrollBar->nSize - win_height;
if( lpScrollBar->nPosition > height ){
lpScrollBar->nScrollMode = 2;
if( !lpScrollBar->bProcessInertiaScroll ){
	atCONTROL_StartControlTimer( hControl, 0x1000 );
	lpScrollBar->bProcessInertiaScroll = atTRUE;
}
}
	}

/*
if(!lpScrollBar->bProcessInertiaScroll)
{
atREGION rgn;		
int win_height, height;
int pos2;

pos2 = atSCROLLBAR_EXT_GetPosition( hControl );	

atCONTROL_GetControlRegion( hControl, &rgn );
win_height = rgn.nHeight - lpScrollBar->nCursorTopMargin - lpScrollBar->nCursorBottomMargin;
height = lpScrollBar->nSize - win_height;
		
if( pos2 < 0 ){
//pos2 = atSCROLLBAR_EXT_SetPosition( hControl, 0 );	
atCONTROL_StartControlTimer( hControl, 0x1000 );
lpScrollBar->bProcessInertiaScroll = atTRUE;
lpScrollBar->nScrollMode = 1;
//is_stop = atTRUE;
} else if( pos2 > height ){
//pos2 = atSCROLLBAR_EXT_SetPosition( hControl, height  );	
atCONTROL_StartControlTimer( hControl, 0x1000 );
lpScrollBar->bProcessInertiaScroll = atTRUE;
lpScrollBar->nScrollMode = 2;
//is_stop = atTRUE;
}
	
}
*/

}


void atSCROLLBAR_EXT_StopAndResetGestureScroll( atHCONTROL hControl )
{
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	if( lpScrollBar == atNULL ) return;

	if( !lpScrollBar->bProcessInertiaScroll ) return;

	atCONTROL_StopControlTimer( hControl, 0x1000 );
	lpScrollBar->bProcessInertiaScroll = atFALSE;
}


atBOOL atSCROLLBAR_EXT_IsProcessGestureScroll( atHCONTROL hControl )
{
	atLPSCROLLBAR	lpScrollBar = (atLPSCROLLBAR)atCONTROL_GetDataPtr(hControl);
	if( lpScrollBar == atNULL ) return atFALSE;

	return lpScrollBar->bProcessInertiaScroll;
}


