/**
  @file WinMng.inl
  @brief WinMng에 관한 Inline API
  @remark WinMng에 관한 Inline API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see WinMng.h
*/
#ifndef __WINMNG_INL_
#define __WINMNG_INL_


#ifdef __cplusplus
extern "C" {
#endif


#define atWINMNG_GetPtr( hWinMng )						(atLPWINMNG)atWINOBJMNG_GetPtr( _TO_WINOBJMNG(hWinMng) )

//#define atWINMNG_CreateWindowMng( hParentWinObj )		(atHCTLMNG)atWINOBJMNG_CreateWinObjMng( _TO_WINOBJ(hParentWinObj), sizeof(atWINMNG) - sizeof(atWINOBJMNG) )
//#define atWINMNG_DestroyWindowMng( hWinMng )			atWINOBJMNG_DestroyWinObjMng( _TO_WINOBJMNG(hWinMng) )

//#define atWINMNG_DestroyWindow( hWinMng, hWindow )	atWINOBJMNG_DestroyWinObj( _TO_WINOBJMNG(hWinMng), _TO_WINOBJ(hWindow) )
//#define atWINMNG_DestroyAllWindows( hWinMng )			atWINOBJMNG_DestroyAllWinObjs( _TO_WINOBJMNG(hWinMng) )
//#define atWINMNG_DestroyWindowsEx( hWinMng, fnIsDel )	atWINOBJMNG_DestroyWinObjsEx( _TO_WINOBJMNG(hWinMng), (atBOOL (*)(atHWINOBJ))fnIsDel )

#define atWINMNG_GetWindowList( hWinMng )				atWINOBJMNG_GetWinObjList( _TO_WINOBJMNG(hWinMng) )
#define atWINMNG_GetWindowCount( hWinMng )				atWINOBJMNG_GetWinObjCount( _TO_WINOBJMNG(hWinMng) )
#define atWINMNG_GetWindowByIndex( hWinMng, nIndex )	(atHWINDOW)atWINOBJMNG_GetWinObjByIndex( _TO_WINOBJMNG(hWinMng), (nIndex) )

#define atWINMNG_RegistWindow( hWinMng, hWindow )		atWINOBJMNG_RegistWinObj( _TO_WINOBJMNG(hWinMng), _TO_WINOBJ(hWindow) )
#define atWINMNG_ReleaseWindow( hWinMng, hWindow )		atWINOBJMNG_ReleaseWinObj( _TO_WINOBJMNG(hWinMng), _TO_WINOBJ(hWindow) )
   
#define atWINMNG_GetCurWindow( hWinMng )				(atHWINDOW)atWINOBJMNG_GetCurWinObj( _TO_WINOBJMNG(hWinMng) )
#define atWINMNG_GetCurWindowOrder( hWinMng )			atWINOBJMNG_GetCurWinObjOrder( _TO_WINOBJMNG(hWinMng) )
#define atWINMNG_SetCurWindow( hWinMng, hWindow )		(atHWINDOW)atWINOBJMNG_SetCurWinObj( _TO_WINOBJMNG(hWinMng), _TO_WINOBJ(hWindow) )
#define atWINMNG_SetTopWindow( hWinMng, hWindow )		(atHWINDOW)atWINOBJMNG_SetTopWinObj( _TO_WINOBJMNG(hWinMng), _TO_WINOBJ(hWindow) )
#define atWINMNG_SetBottomWindow( hWinMng, hWindow )	(atHWINDOW)atWINOBJMNG_SetBottomWinObj( _TO_WINOBJMNG(hWinMng), _TO_WINOBJ(hWindow) )

#define atWINMNG_FindWindowByID( hWinMng, nWindowID )	(atHWINDOW)atWINOBJMNG_FindWinObjByID( _TO_WINOBJMNG(hWinMng), (nWindowID) )
#define atWINMNG_FindWindowByName( hWinMng, szWindowText )	(atHWINDOW)atWINOBJMNG_FindWinObjByName( _TO_WINOBJMNG(hWinMng), (szWindowText) )
#define atWINMNG_FindWindowAt( hWinMng, nX, nY )		(atHWINDOW)atWINOBJMNG_FindWinObjAt( _TO_WINOBJMNG(hWinMng), (nX), (nY) )

#define atWINMNG_SetDrawFlag( hWinMng, IsDraw )				atWINOBJMNG_SetDrawFlag( _TO_WINOBJMNG(hWinMng), (IsDraw) )
#define atWINMNG_ShowWindow( hWinMng, hWindow, bShow )	atWINOBJMNG_ShowWinObj( _TO_WINOBJMNG(hWinMng), _TO_WINOBJ(hWindow), (bShow) )

#define atWINMNG_RedrawAllWindowEx( hWinMng, hWindowBase )	atWINOBJMNG_RedrawAllWinObjEx( _TO_WINOBJMNG(hWinMng), _TO_WINOBJ(hWindowBase), atTRUE )
#define atWINMNG_RedrawAllWindow( hWinMng )				atWINOBJMNG_RedrawAllWinObj( _TO_WINOBJMNG(hWinMng), atTRUE )
#define atWINMNG_RedrawAllWindowArea( hWinMng, rtCtlRect )	atWINOBJMNG_RedrawAllWinObjArea( _TO_WINOBJMNG(hWinMng), (rtCtlRect), atTRUE )

#define atWINMNG_GetNextEnabledWindow( hWinMng, hWindow )	(atHWINDOW)atWINOBJMNG_GetNextEnabledWinObj( _TO_WINOBJMNG(hWinMng), _TO_WINOBJ(hWindow) )
#define atWINMNG_GetPrevEnabledWindow( hWinMng, hWindow )	(atHWINDOW)atWINOBJMNG_GetPrevEnabledWinObj( _TO_WINOBJMNG(hWinMng), _TO_WINOBJ(hWindow) )

#define atWINMNG_AdjustWindowArea( hWinMng, hWindow )		atWINOBJMNG_AdjustWinObjArea( _TO_WINOBJMNG(hWinMng), _TO_WINOBJ(hWindow) )
#define atWINMNG_AdjustAllWindowArea( hWinMng )			atWINOBJMNG_AdjustAllWinObjArea( _TO_WINOBJMNG(hWinMng) )
#define atWINMNG_SetWindowRegion( hWinMng, hWindow, nX, nY, nWidth, nHeight )	atWINOBJMNG_SetWinObjRegion( _TO_WINOBJMNG(hWinMng), _TO_WINOBJ(hWindow), (nX), (nY), (nWidth), (nHeight) )


#ifdef __cplusplus
}
#endif


#endif // __WINMNG_INL_
