/**
  @file SoftKey.c
  @brief SoftKey�� ���� API
  @remark SoftKey�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see SoftKey.h
*/
#include "UI_Types.h"
#include "SoftKey.h"
#include "UI_API/UI_API.h"
#include "UI_Util.h"
#include "UI_MNG.h"


static struct atSOFTKEY {
	atBOOL		bActive;
	atDWORD		dwAttr;
	atREGION	rgnArea;
	atHGDC		hCanvas;
	atHFONT		hFont;
	atCOLOR		clNomalColor, clFrColor, clDisableColor, clBgColor;
//	atINT		nMarginWidth;
	atHBITMAP	hBGImageLeft, hBGImageCenter, hBGImageRight;
	atHBITMAP	hHLImageLeft, hHLImageCenter, hHLImageRight;
	atHBITMAP	hBGImage, hHLImage;
	atTCHAR		szSoftKey[3][atSOFTKEY_TEXT_LEN];		// ����ƮŰ �ؽ�Ʈ   
	atBOOL		bSoftKeyEnabled[3];
	atINT		nDownButton;
} g_SoftKey = {
	atFALSE,
	atSOFTKEYATTR_NONE,
	{ 0, 0, 0, 0},
	(atHGDC)atNULL,
	(atHFONT)atNULL,
	(atCOLOR)0xFFFFFF, (atCOLOR)0xFFFFFF, (atCOLOR)0x888888, (atCOLOR)0x000000,
//	8,
	(atHBITMAP)atNULL, (atHBITMAP)atNULL, (atHBITMAP)atNULL,
	(atHBITMAP)atNULL, (atHBITMAP)atNULL, (atHBITMAP)atNULL,
	(atHBITMAP)atNULL, (atHBITMAP)atNULL,
	{ _AT(""), _AT(""), _AT("") },
	{ atTRUE, atTRUE, atTRUE },
	0
};	



/** 
 @brief SoftKey �ʱ�ȭ
 
 SoftKey�� �ʱ�ȭ �Ѵ�.\n
 ���α׷� ���� �� ó�� �ѹ��� ȣ���� �ָ� �ȴ�.

 @param dwAttr SoftKey �Ӽ�
 @return ����
*/
atVOID atSOFTKEY_InitSoftKey( atDWORD dwAttr )
{
	g_SoftKey.dwAttr = dwAttr;

	atSOFTKEY_SetSoftKeyAreaHeight( 20 );

	g_SoftKey.bActive = atFALSE;
	
	g_SoftKey.hBGImageLeft = (atHBITMAP)atNULL;
	g_SoftKey.hBGImageCenter = (atHBITMAP)atNULL;
	g_SoftKey.hBGImageRight = (atHBITMAP)atNULL;
	
	g_SoftKey.hHLImageLeft = (atHBITMAP)atNULL;
	g_SoftKey.hHLImageCenter = (atHBITMAP)atNULL;
	g_SoftKey.hHLImageRight = (atHBITMAP)atNULL;

	g_SoftKey.hBGImage = (atHBITMAP)atNULL;
	g_SoftKey.hHLImage = (atHBITMAP)atNULL;

	g_SoftKey.nDownButton = 0;

	atUIAPI_strcpy( g_SoftKey.szSoftKey[0], _AT("") );
	atUIAPI_strcpy( g_SoftKey.szSoftKey[1], _AT("") );
	atUIAPI_strcpy( g_SoftKey.szSoftKey[2], _AT("") );

	g_SoftKey.bSoftKeyEnabled[0] = atTRUE;
	g_SoftKey.bSoftKeyEnabled[1] = atTRUE; 
	g_SoftKey.bSoftKeyEnabled[2] = atTRUE; 

	atSOFTKEY_SetSoftKeyFont( atNULL );
	atSOFTKEY_SetSoftKeyColor( atUI_COLOR_WHITE, atUI_COLOR_GRAY, atUI_COLOR_GRAY, atUI_COLOR_DARKGRAY );
}


/** 
 @brief SoftKey ����
 @return ����
*/
atVOID atSOFTKEY_FinalSoftKey( NOARGS )
{
	atSOFTKEY_DeactivateSoftKey();

	g_SoftKey.dwAttr = atSOFTKEYATTR_NONE;
	
	g_SoftKey.hBGImageLeft = (atHBITMAP)atNULL;
	g_SoftKey.hBGImageCenter = (atHBITMAP)atNULL;
	g_SoftKey.hBGImageRight = (atHBITMAP)atNULL;
	
	g_SoftKey.hHLImageLeft = (atHBITMAP)atNULL;
	g_SoftKey.hHLImageCenter = (atHBITMAP)atNULL;
	g_SoftKey.hHLImageRight = (atHBITMAP)atNULL;

	g_SoftKey.hBGImage = (atHBITMAP)atNULL;
	g_SoftKey.hHLImage = (atHBITMAP)atNULL;

	g_SoftKey.nDownButton = 0;
}


/** 
 @brief SoftKey�� Ȱ��ȭ
 @return ����
*/
atVOID atSOFTKEY_ActivateSoftKey( NOARGS )
{
	if( g_SoftKey.hCanvas == atNULL ){
		g_SoftKey.hCanvas = atUIAPI_CreateGDC( g_SoftKey.rgnArea.nX, g_SoftKey.rgnArea.nY, 
												g_SoftKey.rgnArea.nWidth, g_SoftKey.rgnArea.nHeight, 
										   	atUIAPI_GetScreenBPP(), 0 );
		if( g_SoftKey.hFont != atNULL ) atSOFTKEY_SetSoftKeyFont( g_SoftKey.hFont );
	}  else {
		atUIAPI_SetGDCArea( g_SoftKey.hCanvas, &g_SoftKey.rgnArea );
	}

	g_SoftKey.bActive = atTRUE;
}


/** 
 @brief SoftKey�� ��Ȱ��ȭ
 @return ����
*/
atVOID atSOFTKEY_DeactivateSoftKey( NOARGS )
{
	if( g_SoftKey.hCanvas ){
		atUIAPI_DestroyGDC( g_SoftKey.hCanvas );
		g_SoftKey.hCanvas = atNULL;
	}	

	g_SoftKey.bActive = atFALSE;
}


/** 
 @brief SoftKey�� Ȱ��ȭ�Ǿ��°� ���θ� �˾Ƴ�
 @return atTRUE  : Ȱ��ȭ �Ǿ��� \n
         atFALSE : ��Ȱ��ȭ �Ǿ���
*/
atBOOL atSOFTKEY_IsActiveSoftKey( NOARGS )
{
	return g_SoftKey.bActive;
}


/** 
 @brief SoftKey�� ������ ��
 @return SoftKey�� ����
*/
atREGION * atSOFTKEY_GetSoftKeyArea( NOARGS )
{
    if (g_SoftKey.bActive) {
      return &g_SoftKey.rgnArea;
    } else {
      return atNULL;
    }
}


/** 
 @brief SoftKey�� ������ ������
 @param nX, nY : SoftKey ������ ���� ��ġ
 @param nWidth, nHeight : SoftKey ������ ����, ���� ũ��
 @return ����
*/
atVOID atSOFTKEY_SetSoftKeyArea( atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
    g_SoftKey.rgnArea.nX = nX;
    g_SoftKey.rgnArea.nY = nY;
    g_SoftKey.rgnArea.nWidth = nWidth;
    g_SoftKey.rgnArea.nHeight = nHeight;

    if( g_SoftKey.hCanvas != atNULL )
	  atUIAPI_SetGDCArea( g_SoftKey.hCanvas, &g_SoftKey.rgnArea );
}


/** 
 @brief SoftKey�� ������ ���� ũ�⸦ ������
 @param nHeight : SoftKey ������ ���� ũ��
 @return ����
*/
atVOID atSOFTKEY_SetSoftKeyAreaHeight( atINT nHeight )
{
	atSOFTKEY_SetSoftKeyArea( 0, atUIAPI_GetScreenHeight() - nHeight, atUIAPI_GetScreenWidth(), nHeight );
}


/** 
 @brief SoftKey�� ������ ũ�⸦ ������.
 @return ����
*/
atVOID atSOFTKEY_RecalcSoftKeyArea( NOARGS )
{
	atSOFTKEY_SetSoftKeyArea( 0, atUIAPI_GetScreenHeight() - g_SoftKey.rgnArea.nHeight, atUIAPI_GetScreenWidth(), g_SoftKey.rgnArea.nHeight );
}

/** 
 @brief SoftKey�� ������ �������� ������ ������.
 @param clBgColor : �������� ����
 @return ����
*/
atVOID atSOFTKEY_SetSoftKeyColor( atCOLOR clNormalColor, atCOLOR clFrColor, atCOLOR clDisableColor, atCOLOR clBgColor )
{
	g_SoftKey.clNomalColor	= clNormalColor;
	g_SoftKey.clFrColor		= clFrColor;
	g_SoftKey.clDisableColor= clDisableColor;
	g_SoftKey.clBgColor		= clBgColor;
}


/** 
 @brief SoftKey�� ������ �׸� �̹��� ����
 @param hBgImageLeft : ������ ���� �̹���
 @param hBgImageCenter : ������ �߾� �̹���
 @param hBgImageRight : ������ ������ �̹���
 @return ����
 @remark ������ ��� �̹����� ������ ũ�⿡ ���� �ݺ��ؼ� �׷���.
*/
atVOID atSOFTKEY_SetSoftKeyBitmap( atHBITMAP hBgImageLeft, atHBITMAP hBgImageCenter, atHBITMAP hBgImageRight )
{
    g_SoftKey.hBGImageLeft = hBgImageLeft;
    g_SoftKey.hBGImageCenter = hBgImageCenter;
    g_SoftKey.hBGImageRight = hBgImageRight;
}


/** 
 @brief SoftKey�� ������ Ŭ����, Highlight�� �̹��� ����
 @param hBgImageLeft : Highlight ������ ���� �̹���
 @param hBgImageCenter : Highlight ������ �߾� �̹���
 @param hBgImageRight : Highlight ������ ������ �̹���
 @return ����
 @remark ������ ��� �̹����� ������ ũ�⿡ ���� �ݺ��ؼ� �׷���.
*/
atVOID atSOFTKEY_SetSoftKeyHLBitmap( atHBITMAP hBgImageLeft, atHBITMAP hBgImageCenter, atHBITMAP hBgImageRight )
{
    g_SoftKey.hHLImageLeft = hBgImageLeft;
    g_SoftKey.hHLImageCenter = hBgImageCenter;
    g_SoftKey.hHLImageRight = hBgImageRight;
}


/** 
 @brief SoftKey�� ������ �׸� �̹��� ����. ���� �̹����� ó���� ��
 @param hBgImage : ��� �̹���
 @param hHLImage : Highlight �̹���
 @return ����
 @remark �ӽ÷� �߰��� �Լ�.
*/
atVOID atSOFTKEY_SetSoftKeyBitmap2( atHBITMAP hBgImage, atHBITMAP hHLImage )
{
	g_SoftKey.hBGImage = hBgImage;
	g_SoftKey.hHLImage = hHLImage;
}


/** 
 @brief SoftKey�� �����Ѵ�.
 @param szLeft [in] : ���� SoftKey ���ڿ�
 @param szCenter [in] : ��� SoftKey ���ڿ�
 @param szRight [in] : ������ SoftKey ���ڿ�
 @return ����
*/
atVOID atSOFTKEY_SetSoftKeyText( atLPCTSTR szLeft, atLPCTSTR szCenter, atLPCTSTR szRight )
{
	if( szLeft )   atUIAPI_strcpy( g_SoftKey.szSoftKey[0], szLeft );
	if( szCenter ) atUIAPI_strcpy( g_SoftKey.szSoftKey[1], szCenter );
	if( szRight )  atUIAPI_strcpy( g_SoftKey.szSoftKey[2], szRight );
}


/** 
 @brief SoftKey�� �Ѱ��� �����Ѵ�.
 @param nSoftPos [in] : ����ƮŰ �ε��� (0:����, 1:���, 2:������)
 @param szText [in] : SoftKey ���ڿ�
 @return ����
*/
atVOID atSOFTKEY_SetSoftKeyTextOne( atINT nSoftPos, atLPCTSTR szText )
{
	if( (nSoftPos < 0) || (nSoftPos > 2) ) return;
	
	atUIAPI_strcpy( g_SoftKey.szSoftKey[nSoftPos], szText );
}


/** 
 @brief SoftKey�� ��´�.
 @param szLeft [in] : ���� SoftKey ���ڿ�
 @param szCenter [in] : ��� SoftKey ���ڿ�
 @param szRight [in] : ������ SoftKey ���ڿ�
 @return ����
*/
atVOID atSOFTKEY_GetSoftKeyText( atLPTSTR szLeft, atLPTSTR szCenter, atLPTSTR szRight )
{
	if( szLeft )   atUIAPI_strcpy( szLeft, g_SoftKey.szSoftKey[0] );
	if( szCenter ) atUIAPI_strcpy( szCenter, g_SoftKey.szSoftKey[1] );
	if( szRight )  atUIAPI_strcpy( szRight, g_SoftKey.szSoftKey[2] );
}


/** 
 @brief SoftKey�� �Ѱ��� ��´�.
 @param nSoftPos [in] : ����ƮŰ �ε��� (0:����, 1:���, 2:������)
 @return ����ƮŰ ���ڿ� ������
*/
atLPTSTR atSOFTKEY_GetSoftKeyTextOne( atINT nSoftPos )
{
	if( (nSoftPos < 0) || (nSoftPos > 2) ) return atNULL;
	
	return g_SoftKey.szSoftKey[nSoftPos];
}


/** 
 @brief SoftKey�� �� �׸��� Ȱ��ȭ�Ǿ��°��� �˾Ƴ���.
 @param pbLeft [out] : ���� SoftKey Ȱ��ȭ�� ������
 @param pbCenter [out] : ��� SoftKey Ȱ��ȭ�� ������
 @param pbRight [out] : ������ SoftKey Ȱ��ȭ�� ������
 @return ����
*/
atVOID atSOFTKEY_GetSoftKeyEnabled( atBOOL *pbLeft, atBOOL *pbCenter, atBOOL *pbRight )
{
	if( pbLeft ) *pbLeft = g_SoftKey.bSoftKeyEnabled[0];
	if( pbCenter ) *pbCenter = g_SoftKey.bSoftKeyEnabled[1]; 
	if( pbRight ) *pbRight = g_SoftKey.bSoftKeyEnabled[2]; 
}


/** 
 @brief ����ƮŰ�� Ȱ��ȭ�Ǿ� �ִ°� �˾Ƴ���. (1������)
 @param nSoftPos [in] : ����ƮŰ �ε��� (0:����, 1:���, 2:������)
 @return �ش� ����ƮŰ�� Enable�̸� atTRUE, Disable�̸� atFALSE
*/
atBOOL atSOFTKEY_GetSoftKeyEnabledOne( atINT nSoftPos )
{
	if( (nSoftPos < 0) || (nSoftPos > 2) ) return atFALSE;
	
	return g_SoftKey.bSoftKeyEnabled[nSoftPos];
}


/** 
 @brief SoftKey�� Enable/Disable ���¸� �����Ѵ�.
 @param bLeft [in] : ���� SoftKey Enable/Disable
 @param bCenter [in] : ��� SoftKey Enable/Disable
 @param bRight [in] : ������ SoftKey Enable/Disable
 @return ����
*/
atVOID atSOFTKEY_SetSoftKeyEnabled( atBOOL bLeft, atBOOL bCenter, atBOOL bRight )
{
	g_SoftKey.bSoftKeyEnabled[0] = bLeft;
	g_SoftKey.bSoftKeyEnabled[1] = bCenter; 
	g_SoftKey.bSoftKeyEnabled[2] = bRight; 
}


/** 
 @brief SoftKey�� Enable/Disable ���¸� �����Ѵ�.(1������)
 @param nSoftPos [in] : ����ƮŰ �ε��� (0:����, 1:���, 2:������)
 @param bEnabled [in] : SoftKey Enable/Disable
 @return ����
*/
atBOOL atSOFTKEY_SetSoftKeyEnabledOne( atINT nSoftPos, atBOOL bEnabled )
{
	atBOOL old;
	
	if( (nSoftPos < 0) || (nSoftPos > 2) ) return atFALSE;

	old = g_SoftKey.bSoftKeyEnabled[nSoftPos];
	g_SoftKey.bSoftKeyEnabled[nSoftPos] = bEnabled;

	return old;
}


/** 
 @brief SoftKey ��Ʈ�� �����Ѵ�.
 @param hFont [in] : ��Ʈ �ڵ�
 @return ������ �����Ǿ� �־��� ��Ʈ�� �ڵ�
*/
atHFONT atSOFTKEY_SetSoftKeyFont( atHFONT hFont )
{
	atHFONT hOldFont = g_SoftKey.hFont;
	
	if( hFont != atNULL )
		g_SoftKey.hFont = hFont;
	else
		g_SoftKey.hFont = atUIAPI_GetStockFont(0);
	
	if( g_SoftKey.hCanvas != atNULL )
		return atUIAPI_SetFont( g_SoftKey.hCanvas, g_SoftKey.hFont );
	else 
		return hOldFont;
}


/** 
 @brief SoftKey ������ Flush�Ѵ�.
 @return ����
*/
atVOID atSOFTKEY_FlushSoftKeyArea( NOARGS )
{
	if( !g_SoftKey.bActive ) return;
	
	if( g_SoftKey.hCanvas == atNULL ) return;
	atUIAPI_Flush( g_SoftKey.hCanvas );
}


/** 
 @brief SoftKey�� �׸���.
 @return ����
*/
atVOID atSOFTKEY_DrawSoftKey( NOARGS )
{
	int cx, cy, cw;
	int nx, ny;
	int align;
	atCOLOR color;

	if( !g_SoftKey.bActive ) return;
	if( g_SoftKey.hCanvas == atNULL ) return;

	if( g_SoftKey.hBGImageLeft && g_SoftKey.hBGImageCenter && g_SoftKey.hBGImageRight ){
		atUIAPI_DrawClient1Line( g_SoftKey.hCanvas, 0, 0, g_SoftKey.rgnArea.nWidth, 
								 g_SoftKey.hBGImageLeft, g_SoftKey.hBGImageCenter, g_SoftKey.hBGImageRight );
	}
	else if( g_SoftKey.hBGImage )
	{
		atUIAPI_DrawClient1Line( g_SoftKey.hCanvas, 0, 0, g_SoftKey.rgnArea.nWidth, 
								 g_SoftKey.hBGImage, g_SoftKey.hBGImage, g_SoftKey.hBGImage );
	} else {
		atUIAPI_Clear( g_SoftKey.hCanvas, g_SoftKey.clBgColor );
	}

	cx = g_SoftKey.rgnArea.nWidth >> 1;
	cy = g_SoftKey.rgnArea.nHeight >> 1;
	cw = g_SoftKey.rgnArea.nWidth / 3;

	if( g_SoftKey.nDownButton == 1 )
		if( g_SoftKey.hHLImageLeft && g_SoftKey.hHLImageCenter && g_SoftKey.hHLImageRight )
			atUIAPI_DrawClient1Line( g_SoftKey.hCanvas, 0, 0, cw, 
									 g_SoftKey.hHLImageLeft, g_SoftKey.hHLImageCenter, g_SoftKey.hHLImageRight );
		else if( g_SoftKey.hHLImage )
			atUIAPI_DrawImage( g_SoftKey.hCanvas, 0, 0, g_SoftKey.hHLImage );
	if( g_SoftKey.szSoftKey[0][0] != _ATC('\0') )
	{
		if( cw < atUIAPI_GetStringWidth(g_SoftKey.hFont, g_SoftKey.szSoftKey[0], atUIAPI_strlen(g_SoftKey.szSoftKey[0])) )	{
			align = ALIGN_LEFT_MIDDLE; nx = 0; ny = cy;
		}else{
			align = ALIGN_CENTER_MIDDLE; nx =cw>>1; ny = cy;
		}

		if(!g_SoftKey.bSoftKeyEnabled[0])	color = g_SoftKey.clDisableColor;
		else if(g_SoftKey.nDownButton == 1)	color = g_SoftKey.clFrColor;
		else								color = g_SoftKey.clNomalColor;

		atUIAPI_DrawStringAlign( g_SoftKey.hCanvas, nx, ny, g_SoftKey.szSoftKey[0], align, color );
	}

	if( g_SoftKey.nDownButton == 2 )
		if( g_SoftKey.hHLImageLeft && g_SoftKey.hHLImageCenter && g_SoftKey.hHLImageRight )
			atUIAPI_DrawClient1Line( g_SoftKey.hCanvas, cx-(cw>>1), 0, cw, 
									 g_SoftKey.hHLImageLeft, g_SoftKey.hHLImageCenter, g_SoftKey.hHLImageRight );
		else if( g_SoftKey.hHLImage )
			atUIAPI_DrawImage( g_SoftKey.hCanvas, cw, 0, g_SoftKey.hHLImage );

	if( g_SoftKey.szSoftKey[1][0] != _ATC('\0') ){
		align = ALIGN_CENTER_MIDDLE; nx = cx; ny = cy;

		if(!g_SoftKey.bSoftKeyEnabled[1])	color = g_SoftKey.clDisableColor;
		else if(g_SoftKey.nDownButton == 2)	color = g_SoftKey.clFrColor;
		else								color = g_SoftKey.clNomalColor;

 		atUIAPI_DrawStringAlign( g_SoftKey.hCanvas, nx, ny, g_SoftKey.szSoftKey[1], align, color );
	}

	if( g_SoftKey.nDownButton == 3 )
		if( g_SoftKey.hHLImageLeft && g_SoftKey.hHLImageCenter && g_SoftKey.hHLImageRight )
			atUIAPI_DrawClient1Line( g_SoftKey.hCanvas, g_SoftKey.rgnArea.nWidth-cw, 0, cw,
									 g_SoftKey.hHLImageLeft, g_SoftKey.hHLImageCenter, g_SoftKey.hHLImageRight );
		else if( g_SoftKey.hHLImage )
			atUIAPI_DrawImage( g_SoftKey.hCanvas, g_SoftKey.rgnArea.nWidth-cw, 0, g_SoftKey.hHLImage );

	if( g_SoftKey.szSoftKey[2][0] != _ATC('\0') )
	{
		if( cw < atUIAPI_GetStringWidth(g_SoftKey.hFont, g_SoftKey.szSoftKey[2], atUIAPI_strlen(g_SoftKey.szSoftKey[0])) ){
			align = ALIGN_RIGHT_MIDDLE; nx = g_SoftKey.rgnArea.nWidth-1; ny = cy;
		}else{
			align = ALIGN_CENTER_MIDDLE; nx = g_SoftKey.rgnArea.nWidth-(cw>>1); ny = cy;
		}

		if(!g_SoftKey.bSoftKeyEnabled[2])	color = g_SoftKey.clDisableColor;
		else if(g_SoftKey.nDownButton == 3)	color = g_SoftKey.clFrColor;
		else								color = g_SoftKey.clNomalColor;

		atUIAPI_DrawStringAlign( g_SoftKey.hCanvas,  nx, ny, g_SoftKey.szSoftKey[2], align, color );
	}
}


/** 
 @brief Ư�� SoftKey�� Down���·� �����.
 @param nSoftPos [in] : ����ƮŰ �ε��� (0:����, 1:���, 2:������)
 @return ����
*/
atVOID		atSOFTKEY_DownSoftKey( atINT nDownButton )
{
	g_SoftKey.nDownButton = nDownButton;
}



/** 
 @brief SoftKey ������ Refresh��Ų��.
 @return ����
*/
atVOID atSOFTKEY_RefreshSoftKey( NOARGS )
{
	if( !g_SoftKey.bActive ) return;
	
	atUIAPI_AddInvalideArea( atSOFTKEY_GetSoftKeyArea() );
}

