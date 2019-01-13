/**
  @file SoftKey.h
  @brief SoftKey에 관한 API
  @remark SoftKey에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see SoftKey.c
*/
#ifndef __SOFTKEY_H_
#define __SOFTKEY_H_


#include "UI_Types.h"
#include "UI_API/UI_API.h"


#ifdef __cplusplus
extern "C" {
#endif
    

#define 	atSOFTKEYATTR_NONE 			0x0000
#define 	atSOFTKEYATTR_TRANSPARENT 	0x8000


#define	atSOFTKEY_TEXT_LEN	32

#define	atSOFTKEY_LEFT			0
#define	atSOFTKEY_CENTER		1
#define	atSOFTKEY_RIGHT		2


atVOID		atSOFTKEY_InitSoftKey( atDWORD dwAttr );
atVOID		atSOFTKEY_FinalSoftKey( NOARGS );
atVOID		atSOFTKEY_ActivateSoftKey( NOARGS );
atVOID		atSOFTKEY_DeactivateSoftKey( NOARGS );
atBOOL		atSOFTKEY_IsActiveSoftKey( NOARGS );
atREGION *	atSOFTKEY_GetSoftKeyArea( NOARGS );
atVOID		atSOFTKEY_SetSoftKeyArea( atINT nX, atINT nY, atINT nWidth, atINT nHeight );
atVOID		atSOFTKEY_SetSoftKeyAreaHeight( atINT nHeight );
atVOID		atSOFTKEY_RecalcSoftKeyArea( NOARGS );
//atVOID 		atSOFTKEY_SetSoftKeyMarginWidth( atINT nMarginWidth );
atVOID 		atSOFTKEY_SetSoftKeyColor( atCOLOR clNormalColor, atCOLOR clFrColor, atCOLOR clDisableColor, atCOLOR clBgColor );
atVOID		atSOFTKEY_SetSoftKeyBitmap( atHBITMAP hBgImageLeft, atHBITMAP hBgImageCenter, atHBITMAP hBgImageRight );
atVOID		atSOFTKEY_SetSoftKeyHLBitmap( atHBITMAP hBgImageLeft, atHBITMAP hBgImageCenter, atHBITMAP hBgImageRight );
atVOID		atSOFTKEY_SetSoftKeyBitmap2( atHBITMAP hBgImage, atHBITMAP hHLImage );
atVOID		atSOFTKEY_SetSoftKeyText( atLPCTSTR szLeft, atLPCTSTR szCenter, atLPCTSTR szRight );
atVOID		atSOFTKEY_SetSoftKeyTextOne( atINT nSoftPos, atLPCTSTR szText );
atVOID		atSOFTKEY_GetSoftKeyText( atLPTSTR szLeft, atLPTSTR szCenter, atLPTSTR szRight );
atLPTSTR 	atSOFTKEY_GetSoftKeyTextOne( atINT nSoftPos );
atVOID		atSOFTKEY_GetSoftKeyEnabled( atBOOL *pbLeft, atBOOL *pbCenter, atBOOL *pbRight );
atBOOL		atSOFTKEY_GetSoftKeyEnabledOne( atINT nSoftPos );
atVOID		atSOFTKEY_SetSoftKeyEnabled( atBOOL bLeft, atBOOL bCenter, atBOOL bRight );
atBOOL		atSOFTKEY_SetSoftKeyEnabledOne( atINT nSoftPos, atBOOL bEnabled );
atHFONT 		atSOFTKEY_SetSoftKeyFont( atHFONT hFont );
atVOID		atSOFTKEY_FlushSoftKeyArea( NOARGS );
atVOID		atSOFTKEY_DrawSoftKey( NOARGS );

atVOID 		atSOFTKEY_RefreshSoftKey( NOARGS );

atVOID		atSOFTKEY_DownSoftKey( atINT nDownButton );


#ifdef __cplusplus
}
#endif


#endif
