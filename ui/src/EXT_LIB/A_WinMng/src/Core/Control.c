/** 
  @file Control.c
  @brief Control�� ���� API
  @remark WinObj�� ��ӹ��� Control ��ü�� ���� API
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see Control.h WinObj.h
*/
#include "UI_Types.h"
#include "CtlMng.h"
#include "Window.h"
#include "Control.h"
#include "UI_API/UI_API.h"
#include "UI_Util.h"
#include "UI_Kern.h"
#include <string.h>
#include <stdarg.h>


static atHWINOBJ g_hCaptureKeyObj = atNULL;


/** 
 @brief ����� �����͸� �����Ѵ�.
 
 ����� �����͸� �����Ѵ�.

 @param hControl [in] : Control�� �ڵ�
 @param lpUserData [in] : ����� �������� ������
 @return ����
*/
void atCONTROL_SetUserDataPtr( atHCONTROL hControl, void *lpUserData )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr( hControl );
	if( lpControl ) lpControl->lpUserData = lpUserData;
}


/** 
 @brief ����� �����͸� ��´�.
 
 ����� �����͸� ��´�.

 @param hControl [in] : Control�� �ڵ�
 @return ����� �������� ������. ��ȿ�� atNULL
*/
void * atCONTROL_GetUserDataPtr(atHCONTROL hControl)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr( hControl );
	if( lpControl ) return lpControl->lpUserData;
	else return atNULL;
}


/** 
 @brief Control�� �����Ѵ�. (������, Anchor ����)
 
 Control�� �����Ѵ�. (������, Anchor ����)

 @param nID [in] : ��Ʈ���� ID
 @param nWidth, nHeight [in] : ��Ʈ���� ũ��
 @param rtAnchor [in] : ��Ʈ���� Anchor
 @param attr [in] : ��Ʈ�� �Ӽ�
 @param dwUserDataSize [in] : ����� ������ ũ��
 @param fnCtlProc [in] : ��Ʈ���� �ڵ鷯
 @param lpCreateParam [in] : ��Ʈ�� ������, ����ڰ� ������ ���� �Ķ����
 @return ������ Control�� �ڵ�
 @see atWINOBJ_Create
*/
atHCONTROL atCONTROL_CreateWithAnchor( int nID, int nWidth, int nHeight, atMARGIN *rtAnchor, int attr, atDWORD dwUserDataSize, atLPCTRLPROC fnCtlProc, void *lpCreateParam )
{
	atHCONTROL hControl;
	atLPCONTROL lpControl;
	atDWORD dwAttr;

	dwAttr = atOBJATTR_NORMAL | atOBJATTR_CHILD | atOBJATTR_FOCUSABLE | atOBJATTR_EVENTTARGET;
	if( attr & atCS_TRANSPARENT ) dwAttr |= atOBJATTR_TRANSPARENT;	
	dwAttr |= attr;	
	
	hControl = (atHCONTROL)atWINOBJ_PreCreate( atNULL, 0x0011, nID, _AT(""), dwAttr, 
								atNULL,  
								dwUserDataSize, 
								atCONTROL_DefControlProc, fnCtlProc, lpCreateParam, atNULL,
								sizeof(atCONTROL) - sizeof(atWINOBJ) );
	if( hControl == atNULL ) return atNULL;
	
	atCONTROL_SetVisible( hControl, atTRUE );

	lpControl = atCONTROL_GetPtr(hControl);
	
	lpControl->hFont			= atUIAPI_GetStockFont(0);
	
	lpControl->tControlColor.clNormalFrColor = atUI_COLOR_BLACK;
	lpControl->tControlColor.clNormalTxtColor = atUI_COLOR_BLACK;
	lpControl->tControlColor.clNormalBgColor = atUI_COLOR_WHITE;
	lpControl->tControlColor.clFocusedFrColor = atUI_COLOR_WHITE;
	lpControl->tControlColor.clFocusedTxtColor = atUI_COLOR_WHITE;
	lpControl->tControlColor.clFocusedBgColor = atUI_COLOR_BLUE;
	lpControl->tControlColor.clDisableFrColor = atUI_COLOR_GRAY;
	lpControl->tControlColor.clDisableTxtColor = atUI_COLOR_GRAY;
	lpControl->tControlColor.clDisableBgColor = atUI_COLOR_DARKGRAY;

	lpControl->lstProperties = atUIAPI_CreateList();
	lpControl->lstMethods = atUIAPI_CreateList();

	lpControl->lpUserData = atNULL;
	
	atWINOBJ_Activate( _TO_WINOBJ(hControl) );

	atWINOBJ_PostCreate( _TO_WINOBJ(hControl), nWidth, nHeight, rtAnchor );	
	
	return hControl;
}


/** 
 @brief Control�� �����Ѵ�.
 
 Control�� �����Ѵ�.

 @param nID [in] : ��Ʈ���� ID
 @param nX, nY [in] : ��Ʈ���� ��ġ
 @param nWidth, nHeight [in] : ��Ʈ���� ũ��
 @param attr [in] : ��Ʈ�� �Ӽ�
 @param dwUserDataSize [in] : ����� ������ ũ��
 @param fnCtlProc [in] : ��Ʈ���� �ڵ鷯
 @param lpCreateParam [in] : ��Ʈ�� ������, ����ڰ� ������ ���� �Ķ����
 @return ������ Control�� �ڵ�
 @see atWINOBJ_Create
*/
atHCONTROL atCONTROL_Create( int nID, int nX, int nY, int nWidth, int nHeight, int attr, atDWORD dwUserDataSize, atLPCTRLPROC fnCtlProc, void *lpCreateParam )
{
	atMARGIN rtAnchor;
	SET_MARGIN( rtAnchor, nY, atANCHOR_IGNORE, nX, atANCHOR_IGNORE );
	return atCONTROL_CreateWithAnchor( nID, nWidth, nHeight, &rtAnchor, attr, dwUserDataSize, fnCtlProc, lpCreateParam );
}


/** 
 @brief Control�� �Ҹ��Ų��.
 
 Control�� �Ҹ��Ų��.

 @param hControl [in] : Control�� �ڵ�
 @return ����
*/
void atCONTROL_Destroy( atHCONTROL hControl )
{
	atCONTROL_ProcessControlProc( hControl, CTLMSG_REQ_DESTROY, 0, 0 );
}


static void _atCONTROL_Destroy( atHCONTROL hControl )
{
	atLPCONTROL		lpControl = atCONTROL_GetPtr(hControl);

	if( lpControl == atNULL ) return;
	if( !atCONTROL_IsValidate(hControl) ) return;	
	
	atUIAPI_DestroyList(lpControl->lstProperties, atTRUE);
	atUIAPI_DestroyList(lpControl->lstMethods, atTRUE);
}


/** 
 @brief Control�� �÷��� ��´�.
 
 Control�� �÷��� ��´�.

 @param hControl [in] : Control�� �ڵ�
 @param lpCtlColor [out] : Control �÷� ��Ʈ
 @return �����ϸ� atTRUE, �����ϸ� atFALSE
*/
atBOOL atCONTROL_GetControlColor( atHCONTROL hControl, atCONTROL_COLOR *lpCtlColor )
{
	atLPCONTROL		lpControl = atCONTROL_GetPtr(hControl);
	if( lpControl == atNULL ) return atFALSE;

	if( lpCtlColor  == atNULL ) return atFALSE;

	*lpCtlColor = lpControl->tControlColor;
		
	return atTRUE;
}


/** 
 @brief Control�� �÷��� �����Ѵ�.
 
 Control�� �÷��� �����Ѵ�.

 @param hControl [in] : Control�� �ڵ�
 @param lpCtlColor [in] : Control �÷� ��Ʈ
 @return �����ϸ� atTRUE, �����ϸ� atFALSE
*/
atBOOL atCONTROL_SetControlColor( atHCONTROL hControl, atCONTROL_COLOR *lpCtlColor )
{
	atLPCONTROL		lpControl = atCONTROL_GetPtr(hControl);
	if( lpControl == atNULL ) return atFALSE;

	if( lpCtlColor  == atNULL ) return atFALSE;

	lpControl->tControlColor = *lpCtlColor;

	atCONTROL_ProcessControlProc( hControl, CTLMSG_CHANGELAYOUT, 0, 0 );
		
	return atTRUE;
}


/** 
 @brief Control�� �÷��� �����Ѵ�.
 
 Control�� �÷��� �����Ѵ�.\n
 �÷��� �̿��Ͽ� ���� ����� �÷��� Ư���� �� �ִ�.

 @param hControl [in] : Control�� �ڵ�
 @param dwChangeFlag [in] : ���� ����� �÷��� ��Ÿ���� �÷�
 @param lpCtlColor [in] : Control �÷� ��Ʈ
 @return �����ϸ� atTRUE, �����ϸ� atFALSE
*/
atBOOL atCONTROL_SetControlColorEx( atHCONTROL hControl, atDWORD dwChangeFlag, atCONTROL_COLOR *lpCtlColor )
{
	atLPCONTROL		lpControl = atCONTROL_GetPtr(hControl);
	if( lpControl == atNULL ) return atFALSE;

	if( dwChangeFlag & atCONTROL_COLORFLAG_NORMAL_FORE_COLOR )
		lpControl->tControlColor.clNormalFrColor = lpCtlColor->clNormalFrColor;
	if( dwChangeFlag & atCONTROL_COLORFLAG_NORMAL_TEXT_COLOR )
		lpControl->tControlColor.clNormalTxtColor = lpCtlColor->clNormalTxtColor;
	if( dwChangeFlag & atCONTROL_COLORFLAG_NORMAL_BACK_COLOR )
		lpControl->tControlColor.clNormalBgColor = lpCtlColor->clNormalBgColor;
	if( dwChangeFlag & atCONTROL_COLORFLAG_SELECT_FORE_COLOR )
		lpControl->tControlColor.clFocusedFrColor = lpCtlColor->clFocusedFrColor;
	if( dwChangeFlag & atCONTROL_COLORFLAG_SELECT_TEXT_COLOR )
		lpControl->tControlColor.clFocusedTxtColor = lpCtlColor->clFocusedTxtColor;
	if( dwChangeFlag & atCONTROL_COLORFLAG_SELECT_BACK_COLOR )
		lpControl->tControlColor.clFocusedBgColor = lpCtlColor->clFocusedBgColor;
	if( dwChangeFlag & atCONTROL_COLORFLAG_DISABLE_FORE_COLOR )
		lpControl->tControlColor.clDisableFrColor = lpCtlColor->clDisableFrColor;
	if( dwChangeFlag & atCONTROL_COLORFLAG_DISABLE_TEXT_COLOR )
		lpControl->tControlColor.clDisableTxtColor = lpCtlColor->clDisableTxtColor;
	if( dwChangeFlag & atCONTROL_COLORFLAG_DISABLE_BACK_COLOR )
		lpControl->tControlColor.clDisableBgColor = lpCtlColor->clDisableBgColor;

	atCONTROL_ProcessControlProc( hControl, CTLMSG_CHANGELAYOUT, 0, 0 );
		
	return atTRUE;
}


/** 
 @brief Control�� ��Ʈ�� �����Ѵ�.
 
 Control�� ��Ʈ�� �����Ѵ�.

 @param hControl [in] : Control�� �ڵ�
 @param hFont [in] : ��Ʈ�� �ڵ�
 @return ������ �����Ǿ� �ִ� Control�� ��Ʈ
*/
atHFONT atCONTROL_SetFont(atHCONTROL hControl, atHFONT hFont)
{
	atLPCONTROL		lpControl = atCONTROL_GetPtr(hControl);
	
	if( hFont == atNULL ) return atNULL;
	lpControl->hFont = hFont;
	
	return atUIAPI_SetFont( atCONTROL_GetGDC(hControl), hFont );
}


/** 
 @brief Control�� ��Ʈ�� ��´�.
 
 Control�� ��Ʈ�� ��´�.

 @param hControl [in] : Control�� �ڵ�
 @return Control�� ��Ʈ
*/
atHFONT atCONTROL_GetFont(atHCONTROL hControl)
{
	atLPCONTROL		lpControl = atCONTROL_GetPtr(hControl);
	return lpControl->hFont;
//	return atUIAPI_GetFont( atCONTROL_GetGDC(hControl) );
}


/** 
 @brief Control�� GDC�� ��´�.
 
 Control�� GDC (Graphic Device Context)�� ��´�.

 @param hControl [in] : Control�� �ڵ�
 @return Control�� GDC
*/
atHGDC atCONTROL_GetGDC(atHCONTROL hControl)
{
	atLPCONTROL		lpControl = atCONTROL_GetPtr(hControl);
	atHGDC hGDC;

	if( lpControl == atNULL ) return atNULL;
//	if( !atCONTROL_IsVisible(hControl) ) return atNULL;

	hGDC = atWINOBJ_GetGDC( _TO_WINOBJ(hControl) );
	if( hGDC == atNULL ) return atNULL;

	atUIAPI_SetFont( hGDC, lpControl->hFont );
	
	return hGDC;
}


/** 
 @brief Control GDC ����� �غ��Ѵ�.
 
 Control GDC ����� �غ��Ѵ�.

 @param hControl [in] : Control�� �ڵ�
 @param hGDC [in] : GDC�� �ڵ�
 @param bFocus [in] : ��Ŀ���� ���� control�ΰ� ����
 @return ����
*/
void atCONTROL_PrepareGDC(atHCONTROL hControl, atHGDC hGDC, atBOOL bFocus)
{
	atLPCONTROL		lpControl = atCONTROL_GetPtr(hControl);

	//atUIAPI_SetFont( hGDC, lpControl->hFont );
	if( bFocus ){
		atUIAPI_SetFrColor( hGDC, lpControl->tControlColor.clFocusedFrColor );
		atUIAPI_SetBgColor( hGDC, lpControl->tControlColor.clFocusedBgColor );
	} else {
		atUIAPI_SetFrColor( hGDC, lpControl->tControlColor.clNormalFrColor );
		atUIAPI_SetBgColor( hGDC, lpControl->tControlColor.clNormalBgColor );
	}
}


/** 
 @brief Control�� ��Ŀ���� ���� �� �ִ°� ���°� ���θ� �����Ѵ�.
 
 Control�� ��Ŀ���� ���� �� �ִ°� ���°� ���θ� �����Ѵ�.

 @param hControl [in] : Control�� �ڵ�
 @param bFocusable [in] : ��Ŀ���� ���� �� �ִ°� ���°� ����
 @return ����
*/
/*
void atCONTROL_SetFocusable( atHCONTROL hControl, atBOOL bFocusable)
{
	atLPCONTROL		lpControl = atCONTROL_GetPtr(hControl);
	atHCONTROL hctl = atNULL;
	atDWORD attr;
	atBOOL bOldFocusable;

	attr = atWINOBJ_GetAttr( _TO_WINOBJ(hControl) );
	bOldFocusable = (attr & atOBJATTR_FOCUSABLE)? atTRUE : atFALSE;

	if ( bOldFocusable != bFocusable ){
		if(  bFocusable ) attr |= atOBJATTR_FOCUSABLE; else attr &= ~atOBJATTR_FOCUSABLE;
		atWINOBJ_SetAttr( _TO_WINOBJ(hControl), attr );

		if( !bFocusable && lpControl->Base.bFocus ){
// ���� ��ü�� Window�� �׼��� �Ѵ�. ������ ��ȣ������ �ش��ϱ� ������, ���߿� �̻ڰ� �ٽ� ������.
{
atHWINDOW hWnd = atCONTROL_GetParentWindow(hControl);
atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);
if( lpWnd != atNULL ){
	hctl = atCTLMNG_GetNextEnabledControl(lpWnd->Base.hChildObjMng, hControl);
	if( atWINOBJ_GetClassID(hWnd) == 0x0001 ){ // �θ� ������ Ÿ���� ����
		atWINDOW_SetControlFocus( hWnd, hctl );
	}
	if( hControl ) atCONTROL_Refresh(hControl);
	if( hctl ) atCONTROL_Refresh(hctl);
}				
}
		}
	}
}
*/


/** 
 @brief Control�� ũ�⸦ �����Ѵ�.
 
 Control�� ũ�⸦ �����Ѵ�.

 @param hControl [in] : Control�� �ڵ�
 @param nWidth, nHeight [in] : ����, ���� ũ��
 @return ����
*/
void atCONTROL_SetSize( atHCONTROL hControl, int nWidth, int nHeight )
{
	atLPCONTROL		lpControl = atCONTROL_GetPtr(hControl);
	int w, h;
	
	if( lpControl == atNULL ) return;

	w = RECT_WIDTH(lpControl->Base.rect);
	h = RECT_HEIGHT(lpControl->Base.rect);
	if( (w == nWidth) && (h == nHeight) ) return;
	
	lpControl->Base.rect.nEndX = lpControl->Base.rect.nStartX + nWidth - 1;
	lpControl->Base.rect.nEndY = lpControl->Base.rect.nStartY + nHeight - 1;
	
	atCONTROL_ProcessControlProc( hControl, OBJMSG_PRERESIZE, 0, 0 );
	atCONTROL_ProcessControlProc( hControl, OBJMSG_RESIZE, 0, 0 );
}


/** 
 @brief Control�� ũ�⸦ �����Ѵ�.
 
 Control�� ũ�⸦ �����Ѵ�.

 @param hControl [in] : Control�� �ڵ�
 @param ndX, ndY [in] : ��ȭ��ų ����, ���� ũ��. (+�� ��� �þ��, -�� ��� �پ���.)
 @return ����
*/
void atCONTROL_IncSize( atHCONTROL hControl, int ndX, int ndY )
{
	atLPCONTROL		lpControl = atCONTROL_GetPtr(hControl);
	if( lpControl == atNULL ) return;

	if( (ndX == 0) && (ndY == 0) ) return;
	
	lpControl->Base.rect.nEndX += ndX;
	lpControl->Base.rect.nEndY += ndY;
	
	atCONTROL_ProcessControlProc( hControl, OBJMSG_PRERESIZE, 0, 0 );
	atCONTROL_ProcessControlProc( hControl, OBJMSG_RESIZE, 0, 0 );
}


/** 
 @brief Control�� �θ� ��ü���� �޽����� �����Ѵ�.
 
 Control�� �θ� ��ü���� �޽����� �����Ѵ�.

 @param hControl [in] : Control�� �ڵ�
 @param msg [in] : �޽���
 @param param1, param2 [in] : �޽����� �Ķ����
 @return �޽��� �ڵ鷯�� �����
*/
atINT	atCONTROL_PostMessageToParent( atHCONTROL hControl, int msg, long param1, long param2 )
{
	atHWINOBJ hParentObj = _TO_WINOBJ( atCONTROL_GetParentWindow(hControl) );
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr( hParentObj );
	if( lpWinObj == atNULL ) return -1;
	
	if( lpWinObj->dwClassID == 0x0011 ){
		return atWINOBJ_ProcessObjProc( hParentObj, msg, param1, param2 );
	} else {
		return atWINDOW_PostMessage( hParentObj, msg, param1, param2 );
	}
}
	

/** 
 @brief Control�� Default �޽��� �ڵ鷯
 
 Control�� Default �޽��� �ڵ鷯

 @param hControl [in] : Control�� �ڵ�
 @param nMsg [in] : ������ �޽���
 @param lParam1, lParam2 [in] : �޽����� �Ķ����
 @return �޽��� �ڵ鷯�� �����
*/
int atCONTROL_DefControlProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atHCONTROL hCtl = atNULL;
	atRECT rect;
	int ret = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			break;
		case CTLMSG_RELEASE:
			break;

		case CTLMSG_PREDESTROY:
			break;

		case CTLMSG_DESTROY:
			_atCONTROL_Destroy( hControl );
			break;

		case CTLMSG_PAINT:
			if( !(lpControl->Base.dwAttr & atOBJATTR_TRANSPARENT) ){
				rect = lpControl->Base.rect;
				atUIAPI_FillRect( atCONTROL_GetGDC(hControl), 0, 0, RECT_WIDTH(rect), RECT_HEIGHT(rect), atUIAPI_GetRGB(128, 128, 128));
			}
			ret = 1;
			break;

		case CTLMSG_POSTPAINT:
			break;

		case CTLMSG_CHANGEPOS:
			break;
			
		case CTLMSG_PRERESIZE:
			break;

		case CTLMSG_RESIZE:
			break;

		case CTLMSG_KEYDOWN:
			hCtl = atCTLMNG_GetCurControl(lpControl->Base.hChildObjMng);
			if( hCtl ){
				g_hCaptureKeyObj = hCtl;
				ret = atCONTROL_ProcessControlProc( hCtl, CTLMSG_KEYDOWN, lParam1, 0 );
			}  else {
				switch( lParam1 ){
					case atVKEY_LEFT:
					case atVKEY_UP:	
						atCONTROL_PostMessageToParent( hControl, WNDMSG_REQ_CHANGECONTROLFOCUS, (long)hControl, 0 );
						break;
					case atVKEY_RIGHT:
					case atVKEY_DOWN:
						atCONTROL_PostMessageToParent( hControl, WNDMSG_REQ_CHANGECONTROLFOCUS, (long)hControl, 1 );
						break;
					default:
						g_hCaptureKeyObj = hControl;
						break;
				}
			}
			break;

		case CTLMSG_KEYUP:
			if( hControl != g_hCaptureKeyObj ){
				ret = atCONTROL_ProcessControlProc( g_hCaptureKeyObj, CTLMSG_KEYUP, lParam1, 0 );
			}
			g_hCaptureKeyObj = atNULL;
			break;

		case CTLMSG_KEYPRESS:
			if( hControl != g_hCaptureKeyObj ){
				ret = atCONTROL_ProcessControlProc( g_hCaptureKeyObj, CTLMSG_KEYPRESS, lParam1, 0 );
			}
			break;

		case CTLMSG_PREMOUSEDOWN:
			break;
		case CTLMSG_MOUSEDOWN:
			break;
		case CTLMSG_MOUSEMOVE:
			break;
		case CTLMSG_MOUSEUP:
			break;

		case CTLMSG_TIMER:
			break;

		case CTLMSG_CHANGECONTROLFOCUS:
			atCONTROL_PostMessageToParent( hControl, CTLMSG_CHANGECONTROLFOCUS, lParam1, lParam2 );
			break;
			
		case CTLMSG_REQ_CHANGECONTROLFOCUS:
			switch( lParam2 ){
				case 0:
					hCtl = atCTLMNG_GetPrevEnabledControl( lpControl->Base.hChildObjMng, (atHCONTROL)lParam1 );
					break;
				case 1:
					hCtl = atCTLMNG_GetNextEnabledControl( lpControl->Base.hChildObjMng, (atHCONTROL)lParam1 );
					break;
			}
			if( (hCtl != atNULL) && (hCtl != (atHCONTROL)lParam1) ){
				atCONTROL_SetCurChildControl( hControl, hCtl );
				atCONTROL_Refresh((atHCONTROL)lParam1);
				atCONTROL_Refresh(hCtl);
				g_hCaptureKeyObj = hCtl;
//				atCONTROL_PostMessageToParent( hControl, CTLMSG_CHANGECONTROLFOCUS, (long)hCtl, lParam1 );
			} else {
				atCONTROL_PostMessageToParent( hControl, CTLMSG_REQ_CHANGECONTROLFOCUS, (long)hControl, lParam2 );
			}
			break;

		case CTLMSG_COMMAND:
			break;

		case CTLMSG_SUSPEND:
			break;
		case CTLMSG_RESUME:
			break;

		default :
//			if( atCTLMNG_GetCurControl(lpControl->Base.hChildObjMng) ){
//				ret = atCONTROL_ProcessControlProc( atCTLMNG_GetCurControl(lpControl->Base.hChildObjMng), nMsg, lParam1, lParam2 );
//			}	
			break;
	}

	if( ret == 0 ){
		// Control�� ���� ������, WinObj�� �ڵ鷯 ó���� �ѱ��.
		ret = atWINOBJ_DefObjProc( _TO_WINOBJ(hControl), nMsg, lParam1, lParam2 );
	}
	
	return ret;
}


/*

atBOOL atCONTROL_AddProperty( atHCONTROL hControl, atLPTSTR szName, atTYPE nPropType, atDWROD dwAttr )
{
}


atUILIST atCONTROL_GetPropertiesList( atHCONTROL hControl )
{
}


atUILIST atCONTROL_GetMethodsList( atHCONTROL hControl )
{
}


atCONTROL_PROPERTY *atCONTROL_GetProperty( atHCONTROL hControl, atLPTSTR szPropName )
{

}

atINTPTR atCONTROL_GetPropertyValue( atHCONTROL hControl, atLPTSTR szPropName )
{
	atCONTROL_PROPERTY prop = atCONTROL_GetProperty( hControl, szPropName );
	if( prop == atNULL ) return atNULL;
	
	return prop->lValue;		
}


atINTPTR atCONTROL_SetPropertyValue( atHCONTROL hControl, atLPTSTR szName, atLONG lValue )
{
	atCONTROL_PROPERTY prop = atCONTROL_GetProperty( hControl, szPropName );
	if( prop != atNULL ) {
		atLONG oldValue = prop->lValue;
		prop->lValue = lValue;;
		return oldValue;
	} else {
		return atNULL;
	}
}


atCONTROL_METHOD *atCONTROL_GetMethod( atHCONTROL hControl, atLPTSTR szMethodName )
{
}


atINTPTR atCONTROL_CallMethod( atHCONTROL hControl, atLPTSTR szMethodName, ... )
{
    // C���� ���� Ÿ���� �������� �ʾƼ� ��ٷο�. ���� �� ��
	va_list args;

	atCONTROL_METHOD *method = atCONTROL_GetMethod( hControl, szMethodName );
	if( prop != atNULL ) return atNULL;

	va_start(args, szMethodName);

	for( i = 0; i < method->args_cnt; i++ ){
		v = va_arg(args, int);

		method->callback( (atLPTSTR)v, (at
	}

	va_end(args);
}
*/