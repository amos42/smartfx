/**
  @file WObjMng.h
  @brief WinObjMng에 관한 API
  @remark WinObjMng에 관한 API
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see WObjMng.c
*/
#ifndef __WOBJMNG_H_
#define __WOBJMNG_H_


#include "UI_Types.h"
#include "WinObj.h"
#include "UI_Event.h"
#include "UI_API/UI_API.h"
//#include "UI_Util.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atHANDLE  atHWINOBJMNG;


#define _TO_WINOBJMNG(a) 	((atHWINOBJMNG)(a))


/** 
 @brief WinObj Manager 개체

 WinObj Manager 개체 구조체
 
 @remark WinObjMng를 상속 받을 개체는 반드시 개체의 맨 앞 필드에 WINOBJMNG 개체를 위치시키야 한다. (C++의 형태를 흉내) 
*/
typedef struct _tagatWINOBJMNG {
	atBOOL			bActivate;				/**< WinObjMng가 활성화 되었는가 */	
	
	atHWINOBJ		hParentWinObj;			/**< 매니저를 포함하는 WinObj 핸들 */	
	
	atUILIST			lpWinObjList;			/**< WinObj의 리스트 */	
	
	atHWINOBJ		hCurWinObj;				/**< 현재 포커스를 가진 WinObj */	
	atINT			nCurWinObjOrder;		/**< 현재 포커스를 가진 WinObj의 Index */	
	
	atBOOL          		bIsDraw;				/**< 화면 갱신을 할 것인가 */	
} atWINOBJMNG, *atLPWINOBJMNG;


atLPWINOBJMNG 	atWINOBJMNG_GetPtr(atHWINOBJMNG hWinObjMng);
atHWINOBJMNG 	atWINOBJMNG_GetHandle(atLPWINOBJMNG lpWinObjMng);

atHWINOBJMNG	atWINOBJMNG_CreateWinObjMng0( atLPWINOBJMNG lpWinObjMng, atHWINOBJ hParentWinObj );
atHWINOBJMNG	atWINOBJMNG_CreateWinObjMng( atHWINOBJ hParentWinObj, atINT ExtObjMngSize );
void		atWINOBJMNG_DestroyWinObjMng( atHWINOBJMNG hWinObjMng );

//atBOOL  atWINOBJMNG_SetDefaultLayout( atHWINOBJMNG hWinObjMng, atINT DefIndex, atLAYOUTBITMAP *lpLayoutBitmap, atMARGIN *lpLayoutMargin );

/*
atHWINOBJ	atWINOBJMNG_CreateWinObj0( atHWINOBJMNG hWinObjMng, atHWINOBJ hParentWnd, int nStyle, atLPTSTR szWinObjText, 
											int nStartX, int nStartY, int nWidth, int nHeight,
											atDWORD dwUserDataSize, atLPWINOBJPROC fnWndProc, 
											atLAYOUTBITMAP *lpLayoutBm, atMARGIN *lpLayoutMargin,
											void *CreateParam, void *ExData );
atHWINOBJ	atWINOBJMNG_CreateWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hParentWnd, int nStyle, atLPTSTR szWinObjText, 
											int nStartX, int nStartY, int nWidth, int nHeight,
											atDWORD dwUserDataSize, atLPWINOBJPROC fnWndProc, void *CreateParam, void *ExData );
atHWINOBJ	atWINOBJMNG_CreateWinObjByClass( atHWINOBJMNG hWinObjMng, atHWINOBJ hParentWnd, atLPTSTR szWinObjText, 
											int nStartX, int nStartY, int nWidth, int nHeight,
											atLPTSTR szClassName, void *CreateParam );
*/
void		atWINOBJMNG_DestroyWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj );

void 		atWINOBJMNG_DestroyAllWinObjs( atHWINOBJMNG hWinObjMng );
void 		atWINOBJMNG_DestroyWinObjsEx( atHWINOBJMNG hWinObjMng, atBOOL (*fnIsDel)(atHWINOBJ) );

atINT 		atWINOBJMNG_GetWinObjCount( atHWINOBJMNG hWinObjMng );
atUILIST	atWINOBJMNG_GetWinObjList( atHWINOBJMNG hWinObjMng );
atHWINOBJ 	atWINOBJMNG_GetWinObjByIndex( atHWINOBJMNG hWinObjMng, int nIndex );
atINT atWINOBJMNG_GetWinObjIndex( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj );

atBOOL		atWINOBJMNG_RegistWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj );
atBOOL 		atWINOBJMNG_ReleaseWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj );

atHWINOBJ	atWINOBJMNG_FindTopActiveWinObj( atHWINOBJMNG hWinObjMng );
atHWINOBJ	atWINOBJMNG_FindTopActiveWinObjEx( atHWINOBJMNG hWinObjMng, atHWINOBJ hExcludeObj );
atHWINOBJ	atWINOBJMNG_FindWinObjByID( atHWINOBJMNG hWinObjMng, atINT nWinObjID );
atHWINOBJ	atWINOBJMNG_FindWinObjByName( atHWINOBJMNG hWinObjMng, atLPTSTR szWinObjText );
atHWINOBJ	atWINOBJMNG_FindWinObjAt( atHWINOBJMNG hWinObjMng, atINT nX, atINT nY );

atHWINOBJ	atWINOBJMNG_GetCurWinObj( atHWINOBJMNG hWinObjMng );
atHWINOBJ	atWINOBJMNG_SetCurWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj );
atINT 		atWINOBJMNG_GetCurWinObjOrder( atHWINOBJMNG hWinObjMng );
atHWINOBJ	atWINOBJMNG_SetTopWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj );
atHWINOBJ	atWINOBJMNG_SetBottomWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj );

void		atWINOBJMNG_SetDrawFlag( atHWINOBJMNG hWinObjMng, atBOOL IsDraw );
void 		atWINOBJMNG_ShowWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj, atBOOL bShow );

int		atWINOBJMNG_RedrawAllWinObjEx( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObjBase, atBOOL bFocused );
int		atWINOBJMNG_RedrawAllWinObj( atHWINOBJMNG hWinObjMng, atBOOL bFocused );
int 		atWINOBJMNG_RedrawAllWinObjArea( atHWINOBJMNG hWinObjMng, atRECT *rtWinRect, atBOOL bFocused );

atHWINOBJ atWINOBJMNG_GetNextEnabledWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj );
atHWINOBJ atWINOBJMNG_GetPrevEnabledWinObj( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj );

atBOOL 	atWINOBJMNG_GetAnchorAdjRect( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj, atRECT *pRect );
atBOOL	atWINOBJMNG_GetAnchorAdjRegion( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj, atREGION *pRegion );

void		atWINOBJMNG_AdjustWinObjArea( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj );
void		atWINOBJMNG_AdjustAllWinObjArea( atHWINOBJMNG hWinObjMng );

atVOID	atWINOBJMNG_SetWinObjRegion( atHWINOBJMNG hWinObjMng, atHWINOBJ hWinObj, atINT nX, atINT nY, atINT nWidth, atINT nHeight );

atVOID	atWINOBJMNG_SendMessageAllWinObjs( atHWINOBJMNG hWinObjMng, int nMsg, long lParam1, long lParam2 );
atINT	atWINOBJMNG_SendMessageToParentWinObj( atHWINOBJMNG hWinObjMng, int nMsg, long lParam1, long lParam2 );


#ifdef __cplusplus
}
#endif


#endif // __WOBJMNG_H_

