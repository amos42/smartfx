/**
  @file UI_Desk.c
  @brief ����ũž�� ���� API
  @remark ����ũž�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
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
 @brief DeskTop �ʱ�ȭ
 
 DeskTop�� �ʱ�ȭ �Ѵ�.\n
 ���α׷� ���� �� ó�� �ѹ��� ȣ���� �ָ� �ȴ�.

 @param dwAttr DeskTop �Ӽ�
 @return ����
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
 @brief DeskTop ����
 @return ����
*/
atVOID atDESKTOP_FinalDesktop( NOARGS )
{
	atSOFTKEY_FinalSoftKey();
	atANNBAR_FinalAnnBar();
	
	g_Desktop.dwAttr = 0;
}


/** 
 @brief DeskTop�� ������ ������
 @param nX, nY : DeskTop ������ ���� ��ġ
 @param nWidth, nHeight : DeskTop ������ ����, ���� ũ��
 @return ����
*/
atVOID atDESKTOP_SetDesktopArea( atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
    g_Desktop.rgnArea.nX = nX;
    g_Desktop.rgnArea.nY = nY;
    g_Desktop.rgnArea.nWidth = nWidth;
    g_Desktop.rgnArea.nHeight = nHeight;
}


/** 
 @brief DeskTop�� ������ ũ�⸦ ������.
 @return ����
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
 @brief DeskTop�� ������ ũ�⸦ ��´�.
 @param nAreaType [in] : atDESKTOP_AREA_ALL : ����ũž ���� ��ü \n
						atDESKTOP_AREA_CLIENT : Ŭ���̾�Ʈ ������ \n
						atDESKTOP_AREA_ANNBAR : AnnBar ũ�⸸ \n
						atDESKTOP_AREA_SOFTKEY : SoftKey ũ�⸸ \n
						atDESKTOP_AREA_EMBEDED_ANNBAR : AnnBar�� ������ Ŭ���̾�Ʈ ���� \n
						atDESKTOP_AREA_EMBEDED_SOFTKEY : SoftKey�� ������ Ŭ���̾�Ʈ ����
 @param lpRegion [out] : ���� ���� ���� ������
 @return ����
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
 @brief DeskTop ��� �÷��� �����Ѵ�.
 @param clBgColor [in] : ��� �÷� ��
 @return ����
*/
atVOID atDESKTOP_SetDesktopColor( atCOLOR clBgColor )
{
	g_Desktop.clBgColor = clBgColor;
}


/** 
 @brief DeskTop ��� ��Ʈ���� �����Ѵ�.
 @param hBitmap [in] : DeskTop ��� ��Ʈ�� �ڵ�
 @return ����
*/
atVOID atDESKTOP_SetDesktopBitmap( atHBITMAP hBitmap )
{
    g_Desktop.hBGImage = hBitmap;
}


/** 
 @brief DeskTop ������ Flush��Ų��.
 @param nAreaType [in] : atDESKTOP_AREA_ALL : ����ũž ���� ��ü \n
						atDESKTOP_AREA_CLIENT : Ŭ���̾�Ʈ ������ \n
						atDESKTOP_AREA_ANNBAR : AnnBar ũ�⸸ \n
						atDESKTOP_AREA_SOFTKEY : SoftKey ũ�⸸ \n
						atDESKTOP_AREA_EMBEDED_ANNBAR : AnnBar�� ������ Ŭ���̾�Ʈ ���� \n
						atDESKTOP_AREA_EMBEDED_SOFTKEY : SoftKey�� ������ Ŭ���̾�Ʈ ����
 @return ����
*/
atVOID atDESKTOP_FlushDesktopArea( atINT nAreaType )
{
	atREGION area;

	atDESKTOP_GetDesktopArea(nAreaType, &area);

	atUIAPI_FlushRgn( atUIAPI_GetScreenCanvas(), area.nX, area.nY, area.nWidth, area.nHeight );
}


/** 
 @brief DeskTop ����� �׸���.
 @return ����
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
 @brief DeskTop ������ �׸���.
 @return ����
*/
atVOID atDESKTOP_DrawDesktop( NOARGS )
{
	atDESKTOP_DrawDesktopBackground();
	
	atANNBAR_DrawAnnBar();
	atSOFTKEY_DrawSoftKey();

//    if( bFlush ) atDESKTOP_FlushDesktopArea( atDESKTOP_AREA_ALL );
}


/** 
 @brief DeskTop ������ Refresh��Ų��.
 @param nAreaType [in] : atDESKTOP_AREA_ALL : ����ũž ���� ��ü \n
						atDESKTOP_AREA_CLIENT : Ŭ���̾�Ʈ ������ \n
						atDESKTOP_AREA_ANNBAR : AnnBar ũ�⸸ \n
						atDESKTOP_AREA_SOFTKEY : SoftKey ũ�⸸ \n
						atDESKTOP_AREA_EMBEDED_ANNBAR : AnnBar�� ������ Ŭ���̾�Ʈ ���� \n
						atDESKTOP_AREA_EMBEDED_SOFTKEY : SoftKey�� ������ Ŭ���̾�Ʈ ����
 @return ����
*/
atVOID atDESKTOP_Refresh( atINT nAreaType )
{
	atREGION area;

	atDESKTOP_GetDesktopArea(nAreaType, &area);

	atUIAPI_AddInvalideArea( &area );
}

