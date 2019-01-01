/** 
  @file CtlMng.inl
  @brief Control Manager에 관한 Inline API
  @remark CtlMng.c의 인터페이스
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see CtlMng.h Control.h WinObj.h WObjMng.h
*/
#ifndef __CTLMNG_INL_
#define __CTLMNG_INL_


#ifdef __cplusplus
extern "C" {
#endif


#define atCTLMNG_GetPtr( hCtlMng )						(atLPCTLMNG)atWINOBJMNG_GetPtr( _TO_WINOBJMNG(hCtlMng) )

#define atCTLMNG_CreateControlMng( hParentWinObj )		(atHCTLMNG)atWINOBJMNG_CreateWinObjMng( _TO_WINOBJ(hParentWinObj), sizeof(atCTLMNG) - sizeof(atWINOBJMNG) )
#define atCTLMNG_DestroyControlMng( hCtlMng )			atWINOBJMNG_DestroyWinObjMng( _TO_WINOBJMNG(hCtlMng) )

#define atCTLMNG_DestroyControl( hCtlMng, hControl )	atWINOBJMNG_DestroyWinObj( _TO_WINOBJMNG(hCtlMng), _TO_WINOBJ(hControl) )
#define atCTLMNG_DestroyAllControls( hCtlMng )			atWINOBJMNG_DestroyAllWinObjs( _TO_WINOBJMNG(hCtlMng) )
#define atCTLMNG_DestroyControlsEx( hCtlMng, fnIsDel )	atWINOBJMNG_DestroyWinObjsEx( _TO_WINOBJMNG(hCtlMng), (atBOOL (*)(atHWINOBJ))fnIsDel )

#define atCTLMNG_GetControlList( hCtlMng )				atWINOBJMNG_GetWinObjList( _TO_WINOBJMNG(hCtlMng) )
#define atCTLMNG_GetControlCount( hCtlMng )				atWINOBJMNG_GetWinObjCount( _TO_WINOBJMNG(hCtlMng) )
#define atCTLMNG_GetControlByIndex( hCtlMng, nIndex )	(atHCONTROL)atWINOBJMNG_GetWinObjByIndex( _TO_WINOBJMNG(hCtlMng), (nIndex) )

#define atCTLMNG_AddControl( hCtlMng, hControl )		atWINOBJMNG_RegistWinObj( _TO_WINOBJMNG(hCtlMng), _TO_WINOBJ(hControl) )
#define atCTLMNG_RemoveControl( hCtlMng, hControl )		atWINOBJMNG_ReleaseWinObj( _TO_WINOBJMNG(hCtlMng), _TO_WINOBJ(hControl) )
   
#define atCTLMNG_GetCurControl( hCtlMng )				(atHCONTROL)atWINOBJMNG_GetCurWinObj( _TO_WINOBJMNG(hCtlMng) )
#define atCTLMNG_GetCurControlOrder( hCtlMng )			atWINOBJMNG_GetCurWinObjOrder( _TO_WINOBJMNG(hCtlMng) )
#define atCTLMNG_SetCurControl( hCtlMng, hControl )		(atHCONTROL)atWINOBJMNG_SetCurWinObj( _TO_WINOBJMNG(hCtlMng), _TO_WINOBJ(hControl) )
#define atCTLMNG_SetTopControl( hCtlMng, hControl )		(atHCONTROL)atWINOBJMNG_SetTopWinObj( _TO_WINOBJMNG(hCtlMng), _TO_WINOBJ(hControl) )
#define atCTLMNG_SetBottomControl( hCtlMng, hControl )	(atHCONTROL)atWINOBJMNG_SetBottomWinObj( _TO_WINOBJMNG(hCtlMng), _TO_WINOBJ(hControl) )

#define atCTLMNG_FindControlByID( hCtlMng, nControlID )	(atHCONTROL)atWINOBJMNG_FindWinObjByID( _TO_WINOBJMNG(hCtlMng), (nControlID) )
#define atCTLMNG_FindControlByName( hCtlMng, szControlText )	(atHCONTROL)atWINOBJMNG_FindWinObjByName( _TO_WINOBJMNG(hCtlMng), (szControlText) )
#define atCTLMNG_FindControlAt( hCtlMng, nX, nY )		(atHCONTROL)atWINOBJMNG_FindWinObjAt( _TO_WINOBJMNG(hCtlMng), (nX), (nY) )

#define atCTLMNG_SetDrawFlag( hCtlMng, IsDraw )				atWINOBJMNG_SetDrawFlag( _TO_WINOBJMNG(hCtlMng), (IsDraw) )
#define atCTLMNG_ShowControl( hCtlMng, hControl, bShow )	atWINOBJMNG_ShowWinObj( _TO_WINOBJMNG(hCtlMng), _TO_WINOBJ(hControl), (bShow) )

#define atCTLMNG_RedrawAllControlEx( hCtlMng, hControlBase, bFocused )	atWINOBJMNG_RedrawAllWinObjEx( _TO_WINOBJMNG(hCtlMng), _TO_WINOBJ(hControlBase), (bFocused) )
#define atCTLMNG_RedrawAllControl( hCtlMng, bFocused )				atWINOBJMNG_RedrawAllWinObj( _TO_WINOBJMNG(hCtlMng), (bFocused) )
#define atCTLMNG_RedrawAllControlArea( hCtlMng, rtCtlRect, bFocused )	atWINOBJMNG_RedrawAllWinObjArea( _TO_WINOBJMNG(hCtlMng), (rtCtlRect), (bFocused) )

#define atCTLMNG_GetNextEnabledControl( hCtlMng, hControl )	(atHCONTROL)atWINOBJMNG_GetNextEnabledWinObj( _TO_WINOBJMNG(hCtlMng), _TO_WINOBJ(hControl) )
#define atCTLMNG_GetPrevEnabledControl( hCtlMng, hControl )	(atHCONTROL)atWINOBJMNG_GetPrevEnabledWinObj( _TO_WINOBJMNG(hCtlMng), _TO_WINOBJ(hControl) )

#define atCTLMNG_AdjustControlArea( hCtlMng, hControl )		atWINOBJMNG_AdjustWinObjArea( _TO_WINOBJMNG(hCtlMng), _TO_WINOBJ(hControl) )
#define atCTLMNG_AdjustAllControlArea( hCtlMng )			atWINOBJMNG_AdjustAllWinObjArea( _TO_WINOBJMNG(hCtlMng) )
#define atCTLMNG_SetWindowRegion( hCtlMng, hControl, nX, nY, nWidth, nHeight )	atWINOBJMNG_SetWindowRegion( _TO_WINOBJMNG(hCtlMng), _TO_WINOBJ(hControl), (nX), (nY), (nWidth), (nHeight) )


#ifdef __cplusplus
}
#endif


#endif

