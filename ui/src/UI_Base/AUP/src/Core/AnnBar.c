/**
  @file AnnBar.c
  @brief Annunciate Bar�� ���� API
  @remark Annunciate Bar�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
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
 @brief AnnBar �ʱ�ȭ
 
 AnnBar�� �ʱ�ȭ �Ѵ�.\n
 ���α׷� ���� �� ó�� �ѹ��� ȣ���� �ָ� �ȴ�.

 @param dwAttr AnnBar �Ӽ�
 @return ����
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
 @brief AnnBar ����
 @return ����
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
 @brief AnnBar�� Ȱ��ȭ
 @return ����
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
 @brief AnnBar�� ��Ȱ��ȭ
 @return ����
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
 @brief AnnBar�� Ȱ��ȭ�Ǿ��°� ���θ� �˾Ƴ�
 @return atTRUE  : Ȱ��ȭ �Ǿ��� \n
         atFALSE : ��Ȱ��ȭ �Ǿ���
*/
atBOOL atANNBAR_IsActiveAnnBar( NOARGS )
{
	return g_AnnBar.bActive;
}


/** 
 @brief AnnBar�� ������ ��
 @return AnnBar�� ����
*/
atREGION * atANNBAR_GetAnnBarArea( NOARGS )
{
	return &g_AnnBar.rgnArea;
}


/** 
 @brief AnnBar�� ������ ������
 @param nX, nY : AnnBar ������ ���� ��ġ
 @param nWidth, nHeight : AnnBar ������ ����, ���� ũ��
 @return ����
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
 @brief AnnBar�� ������ ���� ũ�⸦ ������
 @param nHeight : AnnBar ������ ���� ũ��
 @return ����
*/
atVOID atANNBAR_SetAnnBarAreaHeight( atINT nHeight )
{
	atANNBAR_SetAnnBarArea( 0, 0, atUIAPI_GetScreenWidth(), nHeight );
}


/** 
 @brief AnnBar�� ������ ũ�⸦ ������.
 @return ����
*/
atVOID atANNBAR_RecalcAnnBarArea( NOARGS )
{
	atANNBAR_SetAnnBarArea( 0, 0, atUIAPI_GetScreenWidth(), g_AnnBar.rgnArea.nHeight );
}


/** 
 @brief AnnBar�� ������ �������� ������ ������.
 @param clBgColor : �������� ����
 @return ����
*/
atVOID atANNBAR_SetAnnBarColor( atCOLOR clBgColor )
{
	g_AnnBar.clBgColor = clBgColor;
}


/** 
 @brief AnnBar�� ������ �׸� �� ȣ���� Callback �Լ� ����
 @param fnDrawFunc : Callback �Լ�
 @return ����
*/
atVOID atANNBAR_SetDrawFunc( atLPANNBAR_DRAWFUNC fnDrawFunc )
{
    g_AnnBar.fnDrawFunc = fnDrawFunc;
}


/** 
 @brief AnnBar�� ������ �׸� �̹��� ����
 @param hBgImageLeft : ������ ���� �̹���
 @param hBgImageCenter : ������ �߾� �̹���
 @param hBgImageRight : ������ ������ �̹���
 @return ����
 @remark ������ ��� �̹����� ������ ũ�⿡ ���� �ݺ��ؼ� �׷���.
*/
atVOID atANNBAR_SetAnnBarBitmap( atHBITMAP hBgImageLeft, atHBITMAP hBgImageCenter, atHBITMAP hBgImageRight )
{
    g_AnnBar.hBGImageLeft = hBgImageLeft;
    g_AnnBar.hBGImageCenter = hBgImageCenter;
    g_AnnBar.hBGImageRight = hBgImageRight;
}


/** 
 @brief AnnBar�� ������ ȭ�� ����
 @return ����
 @remark ���� ���ο��� �ַ� ���Ǳ� ������, ����ڰ� ���� ȣ������ �ʵ��� �Ѵ�.
*/
atVOID atANNBAR_FlushAnnBarArea( NOARGS )
{
	if( !g_AnnBar.bActive ) return;
	
	if( g_AnnBar.hCanvas == atNULL ) return;
	atUIAPI_Flush( g_AnnBar.hCanvas );
}


/** 
 @brief AnnBar�� ������ ȭ�鿡 �׸��� �ڵ�
 @return ����
 @remark ���� ���ο��� �ַ� ���Ǳ� ������, ����ڰ� ���� ȣ������ �ʵ��� �Ѵ�.
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
 @brief AnnBar�� ������ ȭ�鿡 �����ϵ��� ��û.
 @return ����.
*/
atVOID atANNBAR_RefreshAnnBar( NOARGS )
{
	if( !g_AnnBar.bActive ) return;
	
	atUIAPI_AddInvalideArea( atANNBAR_GetAnnBarArea() );
}

