/** 
  @file Control.inl
  @brief Control에 관한 Inline API
  @remark Control.c의 인터페이스
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see Control.h Control.c WinObj.h
*/
#ifndef __CONTROL_INL_
#define __CONTROL_INL_


#ifdef __cplusplus
extern "C" {
#endif


#define 	atCONTROL_GetPtr( hControl )		(atLPCONTROL)atWINOBJ_GetPtr( _TO_WINOBJ(hControl) )
#define		atCONTROL_GetDataPtr(hControl)		atWINOBJ_GetDataPtr( _TO_WINOBJ(hControl) )
#define		atCONTROL_GetControlProc( hControl )	(atLPCTRLPROC)atWINOBJ_GetObjProc( _TO_WINOBJ(hControl) )
#define		atCONTROL_SetControlProc( hControl, lpProc )	(atLPCTRLPROC)atWINOBJ_SetObjProc( _TO_WINOBJ(hControl), (lpProc) )
#define		atCONTROL_CallControlProc( lpProc, hControl, nMsg, lParam1, lParam2 )	atWINOBJ_CallObjProc( (lpProc), _TO_WINOBJ(hControl), (nMsg), (lParam1), (lParam2) )
#define		atCONTROL_ProcessControlProc( hControl, nMsg, lParam1, lParam2 )	atWINOBJ_ProcessObjProc( _TO_WINOBJ(hControl), (nMsg), (lParam1), (lParam2) )
#define		atCONTROL_IsValidate( hControl )	atWINOBJ_IsValidate( _TO_WINOBJ(hControl) )
//#define	atCONTROL_IsActivate( hControl )	atWINOBJ_IsActivate( _TO_WINOBJ(hControl) )
#define		atCONTROL_SetAttr( hControl, uAttr )	atWINOBJ_SetAttr( _TO_WINOBJ(hControl), (uAttr) )
#define 	atCONTROL_GetAttr( hControl )		atWINOBJ_GetAttr( _TO_WINOBJ(hControl) )
//#define 	atCONTROL_GetCreateParam( hControl ) atWINOBJ_GetCreateParam( _TO_WINOBJ(hControl) )
#define		atCONTROL_Draw( hControl, bFocus )			atWINOBJ_Draw( _TO_WINOBJ(hControl), bFocus )
//#define 	atCONTROL_GetOwnerWindow(hControl)	(atHWINDOW)atWINOBJ_GetOwnerObj( _TO_WINOBJ(hControl) )
#define 	atCONTROL_GetParentWindow(hControl)	(atHWINDOW)atWINOBJ_GetParentObj( _TO_WINOBJ(hControl) )
#define		atCONTROL_GetID( hControl )		atWINOBJ_GetID( _TO_WINOBJ(hControl) )
#define		atCONTROL_GetName( hControl )		atWINOBJ_GetName( _TO_WINOBJ(hControl) )
#define		atCONTROL_SetVisible( hControl, bVisible)	atWINOBJ_SetVisible( _TO_WINOBJ(hControl), (bVisible) )
#define 	atCONTROL_IsVisible( hControl )		atWINOBJ_IsVisible( _TO_WINOBJ(hControl) )
#define 	atCONTROL_IsEnable( hControl )		atWINOBJ_IsEnable( _TO_WINOBJ(hControl) )
#define 	atCONTROL_IsFocus( hControl )		atWINOBJ_IsFocus( _TO_WINOBJ(hControl) )
#define 	atCONTROL_IsFocusable( hControl )		atWINOBJ_IsFocusable( _TO_WINOBJ(hControl) )
#define 	atCONTROL_SetPosition( hControl, nX, nY )		atWINOBJ_SetPosition( _TO_WINOBJ(hControl), (nX), (nY), atTRUE )
#define 	atCONTROL_Enable( hControl, bEnable )		atWINOBJ_Enabled( _TO_WINOBJ(hControl), (bEnable) )
//#define 	atCONTROL_SetFocus( hControl, bFocus )		atWINOBJ_SetFocus( _TO_WINOBJ(hControl), (bFocus) )
#define 	atCONTROL_SetFocusable( hControl, bFocusable )		atWINOBJ_SetFocusable( _TO_WINOBJ(hControl), (bFocusable) )
#define 	atCONTROL_UseAnchor( hControl, bUseAnchor )		atWINOBJ_UseAnchor( _TO_WINOBJ(hControl), (bUseAnchor) )
#define 	atCONTROL_SetControlRect( hControl, nStartX, nStartY, nEndX, nEndY )		atWINOBJ_SetRect( _TO_WINOBJ(hControl), (nStartX), (nStartY), (nEndX), (nEndY), atTRUE )
#define 	atCONTROL_GetControlRect( hControl, pRect )		atWINOBJ_GetRect( _TO_WINOBJ(hControl), (pRect) )
#define 	atCONTROL_SetControlRegion( hControl, nX, nY, nWidth, nHeight )		atWINOBJ_SetRegion( _TO_WINOBJ(hControl), (nX), (nY), (nWidth), (nHeight), atTRUE )
#define 	atCONTROL_GetControlRegion( hControl, pRegion )		atWINOBJ_GetRegion( _TO_WINOBJ(hControl), (pRegion) )
#define 	atCONTROL_GetClientRect( hControl, pRect )		atWINOBJ_GetClientRect( _TO_WINOBJ(hControl), (pRect) )
#define 	atCONTROL_GetClientRegion( hControl, pRegion )		atWINOBJ_GetClientRegion( _TO_WINOBJ(hControl), (pRegion) )
#define   atCONTROL_GetScreenRect( hControl, pRect )		atWINOBJ_GetScreenRect( _TO_WINOBJ(hControl), (pRect) )
#define   atCONTROL_GetScreenRegion( hControl, pRegion )		atWINOBJ_GetScreenRegion( _TO_WINOBJ(hControl), (pRegion) )
//#define 	atCONTROL_GetLayoutMargin( hControl, pMargin )		atWINOBJ_GetLayoutMargin( _TO_WINOBJ(hControl), (pMargin) )
//#define 	atCONTROL_SetLayoutMargin( hControl, nTop, nBottom, nLeft, nRight )		atWINOBJ_SetLayoutMargin( _TO_WINOBJ(hControl), (nTop), (nBottom), (nLeft), (nRight) )
#define 	atCONTROL_Refresh( hControl )		atWINOBJ_Refresh( _TO_WINOBJ(hControl) )
//#define 	atCONTROL_RefreshClient( hControl )		atWINOBJ_RefreshClient( _TO_WINOBJ(hControl) )
#define 	atCONTROL_SetAnchor( hControl, nTop, nBottom, nLeft, nRight )		atWINOBJ_SetAnchor( _TO_WINOBJ(hControl), (nTop), (nBottom), (nLeft), (nRight) )
//#define 	atCONTROL_GetClientAdjustSize( hControl, lpWinSize, lpClientSize )		atWINOBJ_GetClientAdjustSize( _TO_WINOBJ(hControl), (lpWinSize), (lpClientSize) )
#define 	atCONTROL_ActivateSoftKey( hControl, bActivate )		atWINOBJ_ActivateSoftKey( _TO_WINOBJ(hControl), (bActivate) )
#define 	atCONTROL_SetSoftKey( hControl, szLeft, szCenter, szRight )		atWINOBJ_SetSoftKey( _TO_WINOBJ(hControl), (szLeft), (szCenter), (szRight) )
#define 	atCONTROL_SetSoftKeyEnabled( hControl, bLeft, bCenter, bRight )		atWINOBJ_SetSoftKeyEnabled( _TO_WINOBJ(hControl), (bLeft), (bCenter), (bRight) )
#define 	atCONTROL_GetSoftKeyEnabled( hControl, pbLeft, pbCenter, pbRight )		atWINOBJ_GetSoftKeyEnabled( _TO_WINOBJ(hControl), (pbLeft), (pbCenter), (pbRight) )
#define 	atCONTROL_SetSoftKeyIDS( hControl, leftIds, centerIds, righIds )		atWINOBJ_SetSoftKeyIDS( _TO_WINOBJ(hControl), (leftIds), (centerIds), (righIds) )
//#define 	atCONTROL_GetGDC( hControl )		atWINOBJ_GetGDC( _TO_WINOBJ(hControl) )
#define 	atCONTROL_Flush( hControl )		atWINOBJ_Flush( _TO_WINOBJ(hControl) )
#define 	atCONTROL_FlushRegion( hControl, nX, nY, nWidth, nHeight )		atWINOBJ_FlushRegion( _TO_WINOBJ(hControl), (nX), (nY), (nWidth), (nHeight) )
//#define 	atCONTROL_FlushClient( hControl )		atWINOBJ_FlushClient( _TO_WINOBJ(hControl) )
//#define 	atCONTROL_FlushClientRegion( hControl, nX, nY, nWidth, nHeight )		atWINOBJ_FlushClientRegion( _TO_WINOBJ(hControl), (nX), (nY), (nWidth), (nHeight) )

#define 	atCONTROL_AddChildControl( hControl, hChildControl )		atWINOBJ_AddChildWinObj( _TO_WINOBJ(hControl), _TO_WINOBJ(hChildControl) )
#define 	atCONTROL_RemoveChildControl( hControl, hChildControl )		atWINOBJ_RemoveChildWinObj( _TO_WINOBJ(hControl), _TO_WINOBJ(hChildControl) )
#define 	atCONTROL_GetControlCount( hControl )		atWINOBJ_GetChildWinObjCount( _TO_WINOBJ(hWnd) )
#define 	atCONTROL_GetCurChildControl( hControl )		(atHCONTROL)atWINOBJ_GetCurChildWinObj( _TO_WINOBJ(hControl) )
#define   atCONTROL_FIndChildControlByID( hControl, nControlID ) 	(atHCONTROL)atWINOBJ_FindChildWinObjByID( _TO_WINOBJ(hControl), (nControlID) )
#define   atCONTROL_FIndChildControlByName( hControl, szControlName ) 	(atHCONTROL)atWINOBJ_FindChildWinObjByName( _TO_WINOBJ(hControl), (szControlName) )
#define 	atCONTROL_SetCurChildControl( hControl, hChildControl )		atWINOBJ_SetCurChildWinObj( _TO_WINOBJ(hControl), _TO_WINOBJ(hChildControl) )
#define 	atCONTROL_ShowChildControl( hControl, hChildControl, bShow )		atWINOBJ_ShowChildWinObj( _TO_WINOBJ(hControl), _TO_WINOBJ(hChildControl), (bShow) )
#define 	atCONTROL_SetTopChildControl( hControl, hChildControl )		(atHCONTROL)atWINOBJ_SetTopChildWinObj( _TO_WINOBJ(hControl), _TO_WINOBJ(hChildControl) )
#define 	atCONTROL_SetBottomChildControl( hControl, hChildControl )		(atHCONTROL)atWINOBJ_SetBottomChildWinObj( _TO_WINOBJ(hControl), _TO_WINOBJ(hChildControl) )
#define 	atCONTROL_PostMessageAllChildControls( hControl, nMsg, lParam1, lParam2 )		atWINOBJ_PostMessageAllChildWinObjs( _TO_WINOBJ(hControl), (nMsg), (lParam1), (lParam2) )
#define 	atCONTROL_RemoveAllChildControls( hControl )		atWINOBJ_RemoveAllChildWinObjs( _TO_WINOBJ(hControl) )
#define 	atCONTROL_DrawAllChildControls( hControl )		atWINOBJ_DrawAllChildWinObjs( _TO_WINOBJ(hControl) )
#define 	atCONTROL_AdjustAllChildControls( hControl )		atWINOBJ_AdjustAllChildWinObjs( _TO_WINOBJ(hControl) )

#define 	atCONTROL_AddControlTimer( hControl, id, interval, IsRepeat, IsEnable, bFocus )		atWINOBJ_AddTimer( _TO_WINOBJ(hControl), (id), (interval), (IsRepeat), (IsEnable), (bFocus) )
#define 	atCONTROL_GetControlTimer( hControl, id )		(atHCTLTIMER)atWINOBJ_GetTimer( _TO_WINOBJ(hControl), (id) )
#define 	atCONTROL_SetControlTimerInterval( hControl, id, interval )		atWINOBJ_SetTimerInterval( _TO_WINOBJ(hControl), (atHCTLTIMER)atWINOBJ_GetTimer(_TO_WINOBJ(hControl), (id)),  (interval) )
#define 	atCONTROL_StartControlTimer( hControl, id )		atWINOBJ_EnableTimer( _TO_WINOBJ(hControl), (atHCTLTIMER)atWINOBJ_GetTimer(_TO_WINOBJ(hControl), (id)),  atTRUE )
#define 	atCONTROL_StopControlTimer( hControl, id )		atWINOBJ_EnableTimer( _TO_WINOBJ(hControl), (atHCTLTIMER)atWINOBJ_GetTimer(_TO_WINOBJ(hControl), (id)), atFALSE )


#ifdef __cplusplus
}
#endif


#endif // __CONTROL_INL_

