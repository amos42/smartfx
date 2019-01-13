/** 
  @file Control.c
  @brief Control에 관한 API
  @remark WinObj를 상속받은 Control 개체에 대한 API
  @author Amos
  @date 2010-04-19 오전 11:41:00
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
 @brief 사용자 데이터를 지정한다.
 
 사용자 데이터를 지정한다.

 @param hControl [in] : Control의 핸들
 @param lpUserData [in] : 사용자 데이터의 포인터
 @return 없음
*/
void atCONTROL_SetUserDataPtr( atHCONTROL hControl, void *lpUserData )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr( hControl );
	if( lpControl ) lpControl->lpUserData = lpUserData;
}


/** 
 @brief 사용자 데이터를 얻는다.
 
 사용자 데이터를 얻는다.

 @param hControl [in] : Control의 핸들
 @return 사용자 데이터의 포인터. 무효시 atNULL
*/
void * atCONTROL_GetUserDataPtr(atHCONTROL hControl)
{
	atLPCONTROL lpControl = atCONTROL_GetPtr( hControl );
	if( lpControl ) return lpControl->lpUserData;
	else return atNULL;
}


/** 
 @brief Control을 생성한다. (생성시, Anchor 지정)
 
 Control을 생성한다. (생성시, Anchor 지정)

 @param nID [in] : 컨트롤의 ID
 @param nWidth, nHeight [in] : 컨트롤의 크기
 @param rtAnchor [in] : 컨트롤의 Anchor
 @param attr [in] : 컨트롤 속성
 @param dwUserDataSize [in] : 사용자 데이터 크기
 @param fnCtlProc [in] : 컨트롤의 핸들러
 @param lpCreateParam [in] : 컨트롤 생성시, 사용자가 지정한 시작 파라미터
 @return 생성된 Control의 핸들
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
 @brief Control을 생성한다.
 
 Control을 생성한다.

 @param nID [in] : 컨트롤의 ID
 @param nX, nY [in] : 컨트롤의 위치
 @param nWidth, nHeight [in] : 컨트롤의 크기
 @param attr [in] : 컨트롤 속성
 @param dwUserDataSize [in] : 사용자 데이터 크기
 @param fnCtlProc [in] : 컨트롤의 핸들러
 @param lpCreateParam [in] : 컨트롤 생성시, 사용자가 지정한 시작 파라미터
 @return 생성된 Control의 핸들
 @see atWINOBJ_Create
*/
atHCONTROL atCONTROL_Create( int nID, int nX, int nY, int nWidth, int nHeight, int attr, atDWORD dwUserDataSize, atLPCTRLPROC fnCtlProc, void *lpCreateParam )
{
	atMARGIN rtAnchor;
	SET_MARGIN( rtAnchor, nY, atANCHOR_IGNORE, nX, atANCHOR_IGNORE );
	return atCONTROL_CreateWithAnchor( nID, nWidth, nHeight, &rtAnchor, attr, dwUserDataSize, fnCtlProc, lpCreateParam );
}


/** 
 @brief Control을 소멸시킨다.
 
 Control을 소멸시킨다.

 @param hControl [in] : Control의 핸들
 @return 없음
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
 @brief Control의 컬러를 얻는다.
 
 Control의 컬러를 얻는다.

 @param hControl [in] : Control의 핸들
 @param lpCtlColor [out] : Control 컬러 세트
 @return 성공하면 atTRUE, 실패하면 atFALSE
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
 @brief Control의 컬러를 지정한다.
 
 Control의 컬러를 지정한다.

 @param hControl [in] : Control의 핸들
 @param lpCtlColor [in] : Control 컬러 세트
 @return 성공하면 atTRUE, 실패하면 atFALSE
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
 @brief Control의 컬러를 지정한다.
 
 Control의 컬러를 지정한다.\n
 플랙을 이용하여 실제 적용될 컬러를 특정할 수 있다.

 @param hControl [in] : Control의 핸들
 @param dwChangeFlag [in] : 실제 적용될 컬러를 나타내는 플랫
 @param lpCtlColor [in] : Control 컬러 세트
 @return 성공하면 atTRUE, 실패하면 atFALSE
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
 @brief Control의 폰트를 설정한다.
 
 Control의 폰트를 설정한다.

 @param hControl [in] : Control의 핸들
 @param hFont [in] : 폰트의 핸들
 @return 이전에 설정되어 있던 Control의 폰트
*/
atHFONT atCONTROL_SetFont(atHCONTROL hControl, atHFONT hFont)
{
	atLPCONTROL		lpControl = atCONTROL_GetPtr(hControl);
	
	if( hFont == atNULL ) return atNULL;
	lpControl->hFont = hFont;
	
	return atUIAPI_SetFont( atCONTROL_GetGDC(hControl), hFont );
}


/** 
 @brief Control의 폰트를 얻는다.
 
 Control의 폰트를 얻는다.

 @param hControl [in] : Control의 핸들
 @return Control의 폰트
*/
atHFONT atCONTROL_GetFont(atHCONTROL hControl)
{
	atLPCONTROL		lpControl = atCONTROL_GetPtr(hControl);
	return lpControl->hFont;
//	return atUIAPI_GetFont( atCONTROL_GetGDC(hControl) );
}


/** 
 @brief Control의 GDC를 얻는다.
 
 Control의 GDC (Graphic Device Context)를 얻는다.

 @param hControl [in] : Control의 핸들
 @return Control의 GDC
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
 @brief Control GDC 사용을 준비한다.
 
 Control GDC 사용을 준비한다.

 @param hControl [in] : Control의 핸들
 @param hGDC [in] : GDC의 핸들
 @param bFocus [in] : 포커스를 가진 control인가 여부
 @return 없음
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
 @brief Control이 포커스를 가질 수 있는가 없는가 여부를 결정한다.
 
 Control이 포커스를 가질 수 있는가 없는가 여부를 결정한다.

 @param hControl [in] : Control의 핸들
 @param bFocusable [in] : 포커스를 가질 수 있는가 없는가 여부
 @return 없음
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
// 상위 개체인 Window을 액세스 한다. 구조상 상호참조에 해당하기 때문에, 나중에 이쁘게 다시 만들자.
{
atHWINDOW hWnd = atCONTROL_GetParentWindow(hControl);
atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);
if( lpWnd != atNULL ){
	hctl = atCTLMNG_GetNextEnabledControl(lpWnd->Base.hChildObjMng, hControl);
	if( atWINOBJ_GetClassID(hWnd) == 0x0001 ){ // 부모가 윈도우 타입일 때만
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
 @brief Control의 크기를 설정한다.
 
 Control의 크기를 설정한다.

 @param hControl [in] : Control의 핸들
 @param nWidth, nHeight [in] : 수평, 수직 크기
 @return 없음
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
 @brief Control의 크기를 조정한다.
 
 Control의 크기를 조정한다.

 @param hControl [in] : Control의 핸들
 @param ndX, ndY [in] : 변화시킬 수평, 수직 크기. (+의 경우 늘어나고, -일 경우 줄어든다.)
 @return 없음
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
 @brief Control의 부모 개체에게 메시지를 전달한다.
 
 Control의 부모 개체에게 메시지를 전달한다.

 @param hControl [in] : Control의 핸들
 @param msg [in] : 메시지
 @param param1, param2 [in] : 메시지의 파라미터
 @return 메시지 핸들러의 결과값
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
 @brief Control의 Default 메시지 핸들러
 
 Control의 Default 메시지 핸들러

 @param hControl [in] : Control의 핸들
 @param nMsg [in] : 전송할 메시지
 @param lParam1, lParam2 [in] : 메시지의 파라미터
 @return 메시지 핸들러의 결과값
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
		// Control의 상위 구조인, WinObj의 핸들러 처리로 넘긴다.
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
    // C에선 가변 타입을 지원하지 않아서 까다로움. 구현 안 함
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