/**
  @file Window.inl
  @brief Window에 관한 Inline API
  @remark Window에 관한 Inline API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see Window.h Window.c WinObj.h Control.h
*/
#ifndef __WINDOW_INL_
#define __WINDOW_INL_


#ifdef __cplusplus
extern "C" {
#endif


#define 	atWINDOW_GetPtr( hWnd )		(atLPWINDOW)atWINOBJ_GetPtr( _TO_WINOBJ(hWnd) )
#define	atWINDOW_GetDataPtr(hWnd)		atWINOBJ_GetDataPtr( _TO_WINOBJ(hWnd) )
#define		atWINDOW_GetWindowProc( hWnd )	(atLPWNDPROC)atWINOBJ_GetObjProc( _TO_WINOBJ(hWnd) )
#define		atWINDOW_SetWindowProc( hWnd, lpProc )	(atLPWNDPROC)atWINOBJ_SetObjProc( _TO_WINOBJ(hWnd), (lpProc) )
#define		atWINDOW_CallWindowProc( lpProc, hWnd, nMsg, lParam1, lParam2 )	atWINOBJ_CallObjProc( (lpProc), _TO_WINOBJ(hWnd), (nMsg), (lParam1), (lParam2) )
#define		atWINDOW_ProcessWindowProc( hWnd, nMsg, lParam1, lParam2 )	atWINOBJ_ProcessObjProc( _TO_WINOBJ(hWnd), (nMsg), (lParam1), (lParam2) )
#define	atWINDOW_IsValidate( hWnd )	atWINOBJ_IsValidate( _TO_WINOBJ(hWnd) )
#define	atWINDOW_IsActivate( hWnd )	atWINOBJ_IsActivate( _TO_WINOBJ(hWnd) )
#define	atWINDOW_SetAttr( hWnd, uAttr )	atWINOBJ_SetAttr( _TO_WINOBJ(hWnd), (uAttr) )
#define 	atWINDOW_GetAttr( hWnd )		atWINOBJ_GetAttr( _TO_WINOBJ(hWnd) )
#define 	atWINDOW_GetCreateParam( hWnd ) atWINOBJ_GetCreateParam( _TO_WINOBJ(hWnd) )
#define	atWINDOW_Draw( hWnd, bFocus )			atWINOBJ_Draw( _TO_WINOBJ(hWnd), bFocus )
#define 	atWINDOW_GetOwnerWindow(hWnd)	(atHWINDOW)atWINOBJ_GetOwnerObj( _TO_WINOBJ(hWnd) )
#define 	atWINDOW_GetParentWindow(hWnd)	(atHWINDOW)atWINOBJ_GetParentObj( _TO_WINOBJ(hWnd) )
#define	atWINDOW_GetID( hWnd )		atWINOBJ_GetID( _TO_WINOBJ(hWnd) )
#define	atWINDOW_GetName( hWnd )		atWINOBJ_GetName( _TO_WINOBJ(hWnd) )
#define	atWINDOW_SetVisible( hWnd, bVisible)	atWINOBJ_SetVisible( _TO_WINOBJ(hWnd), (bVisible) )
#define 	atWINDOW_IsVisible( hWnd )		atWINOBJ_IsVisible( _TO_WINOBJ(hWnd) )
#define 	atWINDOW_UseAnchor( hWnd, bUseAnchor )		atWINOBJ_UseAnchor( _TO_WINOBJ(hWnd), (bUseAnchor) )
#define 	atWINDOW_SetPosition( hWnd, nX, nY )		atWINOBJ_SetPosition( _TO_WINOBJ(hWnd), (nX), (nY), atTRUE )
#define 	atWINDOW_SetWindowRect( hWnd, nStartX, nStartY, nEndX, nEndY )		atWINOBJ_SetRect( _TO_WINOBJ(hWnd), (nStartX), (nStartY), (nEndX), (nEndY), atTRUE )
#define 	atWINDOW_GetWindowRect( hWnd, pRect )		atWINOBJ_GetRect( _TO_WINOBJ(hWnd), (pRect) )
#define 	atWINDOW_SetWindowRegion( hWnd, nX, nY, nWidth, nHeight )		atWINOBJ_SetRegion( _TO_WINOBJ(hWnd), (nX), (nY), (nWidth), (nHeight), atTRUE )
#define 	atWINDOW_GetWindowRegion( hWnd, pRegion )		atWINOBJ_GetRegion( _TO_WINOBJ(hWnd), (pRegion) )
#define 	atWINDOW_GetClientRect( hWnd, pRect )		atWINOBJ_GetClientRect( _TO_WINOBJ(hWnd), (pRect) )
#define 	atWINDOW_GetClientRegion( hWnd, pRegion )		atWINOBJ_GetClientRegion( _TO_WINOBJ(hWnd), (pRegion) )
#define   atWINDOW_GetScreenRect( hWnd, pRect )		atWINOBJ_GetScreenRect( _TO_WINOBJ(hWnd), (pRect) )
#define   atWINDOW_GetScreenRegion( hWnd, pRegion )		atWINOBJ_GetScreenRegion( _TO_WINOBJ(hWnd), (pRegion) )
#define 	atWINDOW_GetLayoutMargin( hWnd, pMargin )		atWINOBJ_GetLayoutMargin( _TO_WINOBJ(hWnd), (pMargin) )
#define 	atWINDOW_SetLayoutMargin( hWnd, nTop, nBottom, nLeft, nRight )		atWINOBJ_SetLayoutMargin( _TO_WINOBJ(hWnd), (nTop), (nBottom), (nLeft), (nRight) )
#define 	atWINDOW_SetLayoutMargin2( hWnd, lpMargin )		atWINOBJ_SetLayoutMargin2( _TO_WINOBJ(hWnd), (lpMargin) )
#define 	atWINDOW_Refresh( hWnd )		atWINOBJ_Refresh( _TO_WINOBJ(hWnd) )
#define 	atWINDOW_RefreshClient( hWnd )		atWINOBJ_RefreshClient( _TO_WINOBJ(hWnd) )
#define 	atWINDOW_SetAnchor( hWnd, nTop, nBottom, nLeft, nRight )		atWINOBJ_SetAnchor( _TO_WINOBJ(hWnd), (nTop), (nBottom), (nLeft), (nRight) )
#define 	atWINDOW_GetClientAdjustSize( hWnd, lpWinSize, lpClientSize )		atWINOBJ_GetClientAdjustSize( _TO_WINOBJ(hWnd), (lpWinSize), (lpClientSize) )
#define 	atWINDOW_ActivateSoftKey( hWnd, bActivate )		atWINOBJ_ActivateSoftKey( _TO_WINOBJ(hWnd), (bActivate) )
#define 	atWINDOW_SetSoftKey( hWnd, szLeft, szCenter, szRight )		atWINOBJ_SetSoftKey( _TO_WINOBJ(hWnd), (szLeft), (szCenter), (szRight) )
#define 	atWINDOW_SetSoftKeyEnabled( hWnd, bLeft, bCenter, bRight )		atWINOBJ_SetSoftKeyEnabled( _TO_WINOBJ(hWnd), (bLeft), (bCenter), (bRight) )
#define 	atWINDOW_GetSoftKeyEnabled( hWnd, pbLeft, pbCenter, pbRight )		atWINOBJ_GetSoftKeyEnabled( _TO_WINOBJ(hWnd), (pbLeft), (pbCenter), (pbRight) )
#define 	atWINDOW_SetSoftKeyIDS( hWnd, leftIds, centerIds, righIds )		atWINOBJ_SetSoftKeyIDS( _TO_WINOBJ(hWnd), (leftIds), (centerIds), (righIds) )
#define 	atWINDOW_GetWindowGDC( hWnd )		atWINOBJ_GetGDC( _TO_WINOBJ(hWnd) )
#define 	atWINDOW_Flush( hWnd )		atWINOBJ_Flush( _TO_WINOBJ(hWnd) )
#define 	atWINDOW_FlushRegion( hWnd, nX, nY, nWidth, nHeight )		atWINOBJ_FlushRegion( _TO_WINOBJ(hWnd), (nX), (nY), (nWidth), (nHeight) )
#define 	atWINDOW_FlushClient( hWnd )		atWINOBJ_FlushClient( _TO_WINOBJ(hWnd) )
#define 	atWINDOW_FlushClientRegion( hWnd, nX, nY, nWidth, nHeight )		atWINOBJ_FlushClientRegion( _TO_WINOBJ(hWnd), (nX), (nY), (nWidth), (nHeight) )

//#define 	atWINDOW_AddControl( hWnd, hControl )		atWINOBJ_AddChildWinObj( _TO_WINOBJ(hWnd), _TO_WINOBJ(hControl) )
//#define 	atWINDOW_RemoveControl( hWnd, hControl )		atWINOBJ_RemoveChildWinObj( _TO_WINOBJ(hWnd), _TO_WINOBJ(hControl) )
#define 	atWINDOW_GetControlCount( hWnd )		atWINOBJ_GetChildWinObjCount( _TO_WINOBJ(hWnd) )
#define 	atWINDOW_GetCurControl( hWnd )		(atHCONTROL)atWINOBJ_GetCurChildWinObj( _TO_WINOBJ(hWnd) )
#define   atWINDOW_GetControl( hWnd, nControlID ) 	(atHCONTROL)atWINOBJ_FindChildWinObjByID( _TO_WINOBJ(hWnd), (nControlID) )
#define   atWINDOW_FindControlByName( hWnd, szControlName ) 	(atHCONTROL)atWINOBJ_FindChildWinObjByName( _TO_WINOBJ(hWnd), (szControlName) )
#define 	atWINDOW_SetCurControl( hWnd, hControl )		atWINOBJ_SetCurChildWinObj( _TO_WINOBJ(hWnd), _TO_WINOBJ(hControl) )
#define 	atWINDOW_ShowControl( hWnd, hControl, bShow )		atWINOBJ_ShowChildWinObj( _TO_WINOBJ(hWnd), _TO_WINOBJ(hControl), (bShow) )
#define 	atWINDOW_SetTopControl( hWnd, hControl )		(atHCONTROL)atWINOBJ_SetTopChildWinObj( _TO_WINOBJ(hWnd), _TO_WINOBJ(hControl) )
#define 	atWINDOW_SetBottomControl( hWnd, hControl )		(atHCONTROL)atWINOBJ_SetBottomChildWinObj( _TO_WINOBJ(hWnd), _TO_WINOBJ(hControl) )
#define 	atWINDOW_PostMessageAllControls( hWnd, nMsg, lParam1, lParam2 )		atWINOBJ_PostMessageAllChildWinObjs( _TO_WINOBJ(hWnd), (nMsg), (lParam1), (lParam2) )
#define 	atWINDOW_RemoveAllControls( hWnd )		atWINOBJ_RemoveAllChildWinObjs( _TO_WINOBJ(hWnd) )
#define 	atWINDOW_DrawAllControls( hWnd, hFocusObj )		atWINOBJ_DrawAllChildWinObjs( _TO_WINOBJ(hWnd), hFocusObj )
#define 	atWINDOW_AdjustAllControls( hWnd )		atWINOBJ_AdjustAllChildWinObjs( _TO_WINOBJ(hWnd) )

#define 	atWINDOW_AddTimer( hWnd, id, interval, IsRepeat, IsEnable, IsFocus )		(atHWINTIMER)atWINOBJ_AddTimer( _TO_WINOBJ(hWnd), (id), (interval), (IsRepeat), (IsEnable), (IsFocus) )
#define 	atWINDOW_GetTimer( hWnd, id )								(atHWINTIMER)atWINOBJ_GetTimer( _TO_WINOBJ(hWnd), (id) )
#define 	atWINDOW_SetTimerInterval( hWnd, hTimer, nInterval )		atWINOBJ_SetTimerInterval( _TO_WINOBJ(hWnd), (hTimer),  (nInterval) )
#define 	atWINDOW_EnableTimer( hWnd, hTimer, IsEnable )		atWINOBJ_EnableTimer( _TO_WINOBJ(hWnd), (hTimer), (IsEnable) )
#define 	atWINDOW_ResetTimer( hWnd, hTimer )		atWINOBJ_ResetTimer( _TO_WINOBJ(hWnd), (hTimer) )
#define 	atWINDOW_RemoveTimer( hWnd, hTimer )		atWINOBJ_RemoveTimer( _TO_WINOBJ(hWnd), (hTimer) )
#define 	atWINDOW_SuspendTimer( hWnd, hTimer )		atWINOBJ_SuspendTimer( _TO_WINOBJ(hWnd), (hTimer) )
#define 	atWINDOW_ResumeTimer( hWnd, hTimer )		atWINOBJ_ResumeTimer( _TO_WINOBJ(hWnd), (hTimer) )
#define 	atWINDOW_EnableAllTimers( hWnd, IsEnable )		atWINOBJ_EnableAllTimers( _TO_WINOBJ(hWnd), (IsEnable) )
#define 	atWINDOW_RemoveAllTimers( hWnd )		atWINOBJ_RemoveAllTimers( _TO_WINOBJ(hWnd) )
#define 	atWINDOW_SuspendAllTimers( hWnd, bFocusedSuspend )		atWINOBJ_SuspendAllTimers( _TO_WINOBJ(hWnd), (bFocusedSuspend) )
#define 	atWINDOW_ResumeAllTimers( hWnd )		atWINOBJ_ResumeAllTimers( _TO_WINOBJ(hWnd) )


#ifdef __cplusplus
}
#endif


#endif // __WINDOW_INL_
