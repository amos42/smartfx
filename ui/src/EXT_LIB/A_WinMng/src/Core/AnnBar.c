/**
  @file AnnBar.c
  @brief Annunciate Bar에 관한 API
  @remark Annunciate Bar에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see AnnBar.h
*/
#include "UI_Types.h"
#include "AnnBar.h"
#include "UI_API/UI_API.h"
#include "UI_Util.h"
#include "UI_MNG.h"


static struct atANNBAR {
	atBOOL		bActive;
	atDWORD		dwAttr;
    atREGION   rgnArea;
	atHGDC		hCanvas;
	atHBITMAP	hBGImageLeft, hBGImageCenter, hBGImageRight;
	atCOLOR		clBgColor;
	atLPANNBAR_DRAWFUNC fnDrawFunc;
} g_AnnBar = {
	atFALSE,
	0,
	{ 0, 0, 0, 0},
	(atHGDC)atNULL,
	(atHBITMAP)atNULL, (atHBITMAP)atNULL, (atHBITMAP)atNULL,
	(atCOLOR)0x0,
	atNULL
};


/** 
 @brief AnnBar 초기화
 
 AnnBar를 초기화 한다.\n
 프로그램 실행 중 처음 한번만 호출해 주면 된다.

 @param dwAttr AnnBar 속성
 @return 없음
*/
atVOID atANNBAR_InitAnnBar( atDWORD dwAttr )
{
	g_AnnBar.dwAttr = dwAttr;
	g_AnnBar.bActive = atFALSE;

	atANNBAR_SetAnnBarAreaHeight( 20 );

	atANNBAR_SetAnnBarColor( atUI_COLOR_WHITE );

	g_AnnBar.fnDrawFunc = (atLPANNBAR_DRAWFUNC)atNULL;
}


/** 
 @brief AnnBar 종료
 @return 없음
*/
atVOID atANNBAR_FinalAnnBar( NOARGS )
{
	atANNBAR_DeactivateAnnBar();

	g_AnnBar.dwAttr = 0;

	g_AnnBar.hBGImageLeft = atNULL;
	g_AnnBar.hBGImageCenter = atNULL;
	g_AnnBar.hBGImageRight = atNULL;	
}


/** 
 @brief AnnBar를 활성화
 @return 없음
*/
atVOID atANNBAR_ActivateAnnBar( NOARGS )
{
	if( g_AnnBar.hCanvas == atNULL ){
		g_AnnBar.hCanvas = atUIAPI_CreateGDC( g_AnnBar.rgnArea.nX, g_AnnBar.rgnArea.nY, 
												g_AnnBar.rgnArea.nWidth, g_AnnBar.rgnArea.nHeight, 
										   		atUIAPI_GetScreenBPP(), 0 );
	} else {
		atUIAPI_SetGDCArea( g_AnnBar.hCanvas, &g_AnnBar.rgnArea );
	}

	g_AnnBar.bActive = atTRUE;
}


/** 
 @brief AnnBar를 비활성화
 @return 없음
*/
atVOID atANNBAR_DeactivateAnnBar( NOARGS )
{
	if( g_AnnBar.hCanvas ){
		atUIAPI_DestroyGDC( g_AnnBar.hCanvas );
		g_AnnBar.hCanvas = atNULL;
	}
	
	g_AnnBar.bActive = atFALSE;
}


/** 
 @brief AnnBar를 활성화되었는가 여부를 알아냄
 @return atTRUE  : 활성화 되었음 \n
         atFALSE : 비활성화 되었음
*/
atBOOL atANNBAR_IsActiveAnnBar( NOARGS )
{
	return g_AnnBar.bActive;
}


/** 
 @brief AnnBar의 영역을 얻어냄
 @return AnnBar의 영역
*/
atREGION * atANNBAR_GetAnnBarArea( NOARGS )
{
	return &g_AnnBar.rgnArea;
}


/** 
 @brief AnnBar의 영역을 설정함
 @param nX, nY : AnnBar 영역의 시작 위치
 @param nWidth, nHeight : AnnBar 영역의 수평, 수직 크기
 @return 없음
*/
atVOID atANNBAR_SetAnnBarArea( atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
    g_AnnBar.rgnArea.nX = nX;
    g_AnnBar.rgnArea.nY = nY;
    g_AnnBar.rgnArea.nWidth = nWidth;
    g_AnnBar.rgnArea.nHeight = nHeight;

    if( g_AnnBar.hCanvas )
	atUIAPI_SetGDCArea( g_AnnBar.hCanvas, &g_AnnBar.rgnArea );
}


/** 
 @brief AnnBar의 영역의 수직 크기를 설정함
 @param nHeight : AnnBar 영역의 수직 크기
 @return 없음
*/
atVOID atANNBAR_SetAnnBarAreaHeight( atINT nHeight )
{
	atANNBAR_SetAnnBarArea( 0, 0, atUIAPI_GetScreenWidth(), nHeight );
}


/** 
 @brief AnnBar의 영역의 크기를 재계산함.
 @return 없음
*/
atVOID atANNBAR_RecalcAnnBarArea( NOARGS )
{
	atANNBAR_SetAnnBarArea( 0, 0, atUIAPI_GetScreenWidth(), g_AnnBar.rgnArea.nHeight );
}


/** 
 @brief AnnBar의 영역을 페인팅할 색깔을 지정함.
 @param clBgColor : 페인팅할 색깔
 @return 없음
*/
atVOID atANNBAR_SetAnnBarColor( atCOLOR clBgColor )
{
	g_AnnBar.clBgColor = clBgColor;
}


/** 
 @brief AnnBar의 영역을 그릴 때 호출할 Callback 함수 설정
 @param fnDrawFunc : Callback 함수
 @return 없음
*/
atVOID atANNBAR_SetDrawFunc( atLPANNBAR_DRAWFUNC fnDrawFunc )
{
    g_AnnBar.fnDrawFunc = fnDrawFunc;
}


/** 
 @brief AnnBar의 영역을 그릴 이미지 설정
 @param hBgImageLeft : 영역의 왼쪽 이미지
 @param hBgImageCenter : 영역의 중앙 이미지
 @param hBgImageRight : 영역을 오른쪽 이미지
 @return 없음
 @remark 영역의 가운데 이미지는 영역의 크기에 따라 반복해서 그려짐.
*/
atVOID atANNBAR_SetAnnBarBitmap( atHBITMAP hBgImageLeft, atHBITMAP hBgImageCenter, atHBITMAP hBgImageRight )
{
    g_AnnBar.hBGImageLeft = hBgImageLeft;
    g_AnnBar.hBGImageCenter = hBgImageCenter;
    g_AnnBar.hBGImageRight = hBgImageRight;
}


/** 
 @brief AnnBar의 영역을 화면 갱신
 @return 없음
 @remark 엔진 내부에서 주로 사용되기 때문에, 사용자가 직접 호출하지 않도록 한다.
*/
atVOID atANNBAR_FlushAnnBarArea( NOARGS )
{
	if( !g_AnnBar.bActive ) return;
	
	if( g_AnnBar.hCanvas == atNULL ) return;
	atUIAPI_Flush( g_AnnBar.hCanvas );
}


/** 
 @brief AnnBar의 영역을 화면에 그리는 코드
 @return 없음
 @remark 엔진 내부에서 주로 사용되기 때문에, 사용자가 직접 호출하지 않도록 한다.
*/
atVOID atANNBAR_DrawAnnBar( NOARGS )
{
	if( !g_AnnBar.bActive ) return;
	if( g_AnnBar.hCanvas == atNULL ) return;

	if( g_AnnBar.hBGImageLeft && g_AnnBar.hBGImageCenter && g_AnnBar.hBGImageRight ){
		atUIAPI_DrawClient1Line( g_AnnBar.hCanvas, 0, 0, g_AnnBar.rgnArea.nWidth, 
								 g_AnnBar.hBGImageLeft, g_AnnBar.hBGImageCenter, g_AnnBar.hBGImageRight );
	} else {
		atUIAPI_Clear( g_AnnBar.hCanvas, g_AnnBar.clBgColor );
	}

	if( g_AnnBar.fnDrawFunc ) g_AnnBar.fnDrawFunc( g_AnnBar.hCanvas );

//    if( bFlush ) atANNBAR_FlushAnnBarArea();
}


/**
 @brief AnnBar의 영역을 화면에 갱신하도록 요청.
 @return 없음.
*/
atVOID atANNBAR_RefreshAnnBar( NOARGS )
{
	if( !g_AnnBar.bActive ) return;
	
	atUIAPI_AddInvalideArea( atANNBAR_GetAnnBarArea() );
}

