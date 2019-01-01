/**
  @file UI_Desk.c
  @brief 데스크탑에 관한 API
  @remark 데스크탑에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Desk.h
*/
#include "UI_Types.h"
#include "UI_Desk.h"
#include "UI_API/UI_API.h"
#include "UI_Util.h"
#include "UI_MNG.h"


static struct atDESKTOP {
	atDWORD		dwAttr;
    atHBITMAP   hBGImage;
	atCOLOR		clBgColor;
	atREGION   rgnArea;
} g_Desktop = {
	0,
	(atHBITMAP)atNULL,
	(atCOLOR)0x0
};	



/** 
 @brief DeskTop 초기화
 
 DeskTop를 초기화 한다.\n
 프로그램 실행 중 처음 한번만 호출해 주면 된다.

 @param dwAttr DeskTop 속성
 @return 없음
*/
atVOID atDESKTOP_InitDesktop( atDWORD dwAttr )
{
	g_Desktop.dwAttr = dwAttr;

	atANNBAR_InitAnnBar(0);
	atSOFTKEY_InitSoftKey(0);
	
    g_Desktop.rgnArea.nX = 0;
    g_Desktop.rgnArea.nY = 0;
    g_Desktop.rgnArea.nWidth = atUIAPI_GetScreenWidth();
    g_Desktop.rgnArea.nHeight = atUIAPI_GetScreenHeight();

	atDESKTOP_SetDesktopColor( atUIAPI_GetRGB(64, 64, 64) );
}


/** 
 @brief DeskTop 종료
 @return 없음
*/
atVOID atDESKTOP_FinalDesktop( NOARGS )
{
	atSOFTKEY_FinalSoftKey();
	atANNBAR_FinalAnnBar();
	
	g_Desktop.dwAttr = 0;
}


/** 
 @brief DeskTop의 영역을 설정함
 @param nX, nY : DeskTop 영역의 시작 위치
 @param nWidth, nHeight : DeskTop 영역의 수평, 수직 크기
 @return 없음
*/
atVOID atDESKTOP_SetDesktopArea( atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
    g_Desktop.rgnArea.nX = nX;
    g_Desktop.rgnArea.nY = nY;
    g_Desktop.rgnArea.nWidth = nWidth;
    g_Desktop.rgnArea.nHeight = nHeight;
}


/** 
 @brief DeskTop의 영역의 크기를 재계산함.
 @return 없음
*/
atVOID atDESKTOP_RecalcDesktopArea( NOARGS )
{
    g_Desktop.rgnArea.nX = 0;
    g_Desktop.rgnArea.nY = 0;
    g_Desktop.rgnArea.nWidth = atUIAPI_GetScreenWidth();
    g_Desktop.rgnArea.nHeight = atUIAPI_GetScreenHeight();

	atANNBAR_RecalcAnnBarArea();
	atSOFTKEY_RecalcSoftKeyArea();
}


/** 
 @brief DeskTop의 영역의 크기를 얻는다.
 @param nAreaType [in] : atDESKTOP_AREA_ALL : 데스크탑 영역 전체 \n
						atDESKTOP_AREA_CLIENT : 클라이언트 영역만 \n
						atDESKTOP_AREA_ANNBAR : AnnBar 크기만 \n
						atDESKTOP_AREA_SOFTKEY : SoftKey 크기만 \n
						atDESKTOP_AREA_EMBEDED_ANNBAR : AnnBar를 포함한 클라이언트 영역 \n
						atDESKTOP_AREA_EMBEDED_SOFTKEY : SoftKey를 포함한 클라이언트 영역
 @param lpRegion [out] : 영역 값을 얻어올 포인터
 @return 없음
*/
atVOID atDESKTOP_GetDesktopArea( atINT nAreaType, atREGION *lpRegion )
{
	int sy, ey;
	
	switch( nAreaType ){
		case atDESKTOP_AREA_ALL:
			sy = 0;
			ey = -1;
			break;
		case atDESKTOP_AREA_CLIENT:
			sy = (atANNBAR_IsActiveAnnBar()) ? atANNBAR_GetAnnBarArea()->nHeight : 0;
			ey = (atSOFTKEY_IsActiveSoftKey())? -atSOFTKEY_GetSoftKeyArea()->nHeight-1 : -1;
			break;
		case atDESKTOP_AREA_ANNBAR:
			sy = 0;
			ey = (atANNBAR_IsActiveAnnBar()) ? atANNBAR_GetAnnBarArea()->nHeight : 0;
			break;
		case atDESKTOP_AREA_SOFTKEY:
			if( atSOFTKEY_IsActiveSoftKey() ){
				sy = -atSOFTKEY_GetSoftKeyArea()->nHeight;
				ey = atSOFTKEY_GetSoftKeyArea()->nHeight;
			} else {
				sy = g_Desktop.rgnArea.nHeight;
				ey = 0;
			}
			break;
		case atDESKTOP_AREA_EMBEDED_ANNBAR:
			sy = 0;
			ey = (atSOFTKEY_IsActiveSoftKey())? -atSOFTKEY_GetSoftKeyArea()->nHeight-1 : -1;
			break;
		case atDESKTOP_AREA_EMBEDED_SOFTKEY:
			sy = (atANNBAR_IsActiveAnnBar()) ? atANNBAR_GetAnnBarArea()->nHeight : 0;
			ey = -1;
			break;
		default:
			sy = 0;
			ey = 0;
			break;
	}

	lpRegion->nX = g_Desktop.rgnArea.nX;
	lpRegion->nY = (sy >= 0)? g_Desktop.rgnArea.nY + sy : (g_Desktop.rgnArea.nY+g_Desktop.rgnArea.nHeight) + sy + 1;
	lpRegion->nWidth = g_Desktop.rgnArea.nWidth;
	lpRegion->nHeight = (ey >= 0)? ey : g_Desktop.rgnArea.nHeight - lpRegion->nY + ey + 1;
}


/** 
 @brief DeskTop 배경 컬러를 지정한다.
 @param clBgColor [in] : 배경 컬러 값
 @return 없음
*/
atVOID atDESKTOP_SetDesktopColor( atCOLOR clBgColor )
{
	g_Desktop.clBgColor = clBgColor;
}


/** 
 @brief DeskTop 배경 비트맵을 지정한다.
 @param hBitmap [in] : DeskTop 배경 비트맵 핸들
 @return 없음
*/
atVOID atDESKTOP_SetDesktopBitmap( atHBITMAP hBitmap )
{
    g_Desktop.hBGImage = hBitmap;
}


/** 
 @brief DeskTop 영역을 Flush시킨다.
 @param nAreaType [in] : atDESKTOP_AREA_ALL : 데스크탑 영역 전체 \n
						atDESKTOP_AREA_CLIENT : 클라이언트 영역만 \n
						atDESKTOP_AREA_ANNBAR : AnnBar 크기만 \n
						atDESKTOP_AREA_SOFTKEY : SoftKey 크기만 \n
						atDESKTOP_AREA_EMBEDED_ANNBAR : AnnBar를 포함한 클라이언트 영역 \n
						atDESKTOP_AREA_EMBEDED_SOFTKEY : SoftKey를 포함한 클라이언트 영역
 @return 없음
*/
atVOID atDESKTOP_FlushDesktopArea( atINT nAreaType )
{
	atREGION area;

	atDESKTOP_GetDesktopArea(nAreaType, &area);

	atUIAPI_FlushRgn( atUIAPI_GetScreenCanvas(), area.nX, area.nY, area.nWidth, area.nHeight );
}


/** 
 @brief DeskTop 배경을 그린다.
 @return 없음
*/
atVOID atDESKTOP_DrawDesktopBackground( NOARGS )
{
	atREGION area;

	atDESKTOP_GetDesktopArea( atDESKTOP_AREA_CLIENT, &area );

	if( g_Desktop.hBGImage ){
		atUIAPI_DrawImage( atUIAPI_GetScreenCanvas(), area.nX, area.nY, g_Desktop.hBGImage );
	} else {
		atUIAPI_FillRect( atUIAPI_GetScreenCanvas(), area.nX, area.nY, area.nWidth, area.nHeight, g_Desktop.clBgColor );
	}
}


/** 
 @brief DeskTop 영역을 그린다.
 @return 없음
*/
atVOID atDESKTOP_DrawDesktop( NOARGS )
{
	atDESKTOP_DrawDesktopBackground();
	
	atANNBAR_DrawAnnBar();
	atSOFTKEY_DrawSoftKey();

//    if( bFlush ) atDESKTOP_FlushDesktopArea( atDESKTOP_AREA_ALL );
}


/** 
 @brief DeskTop 영역을 Refresh시킨다.
 @param nAreaType [in] : atDESKTOP_AREA_ALL : 데스크탑 영역 전체 \n
						atDESKTOP_AREA_CLIENT : 클라이언트 영역만 \n
						atDESKTOP_AREA_ANNBAR : AnnBar 크기만 \n
						atDESKTOP_AREA_SOFTKEY : SoftKey 크기만 \n
						atDESKTOP_AREA_EMBEDED_ANNBAR : AnnBar를 포함한 클라이언트 영역 \n
						atDESKTOP_AREA_EMBEDED_SOFTKEY : SoftKey를 포함한 클라이언트 영역
 @return 없음
*/
atVOID atDESKTOP_Refresh( atINT nAreaType )
{
	atREGION area;

	atDESKTOP_GetDesktopArea(nAreaType, &area);

	atUIAPI_AddInvalideArea( &area );
}

