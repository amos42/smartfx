/**
  @file UI_Font.h
  @brief Font에 관한 API
  @remark Font에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Font.c
*/
#ifndef __UI_FONT_H_
#define __UI_FONT_H_


#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atHANDLE	atHFONT;
typedef void * atUI_OEM_FONT; 


#define atUIAPI_MAX_FONT_COUNT	256


#define atUIAPI_FACE_SYSTEM       	_AT("System")
#define atUIAPI_FACE_FIXEDSYS    	_AT("FixedSys")

#define atUIAPI_SIZE_TINY        		8
#define atUIAPI_SIZE_SMALL        		10
#define atUIAPI_SIZE_MEDIUM       		16
#define atUIAPI_SIZE_LARGE        		20

#define atUIAPI_STYLE_PLAIN       		0
#define atUIAPI_STYLE_BOLD        		1
#define atUIAPI_STYLE_ITALIC      		2
#define atUIAPI_STYLE_UNDERLINE   		4


void 		atUIAPI_InitFont( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
void		atUIAPI_FinalFont(void);

//atHFONT 	atUIAPI_GetFontHandle( atLPVD_FONT lpFont );
//atLPVD_FONT 	atUIAPI_GetFontPtr( atHFONT hFont );

atHFONT 	atUIAPI_CreateFont( atLPTSTR font_face, int font_size, int font_style, atBOOL IsStock );
void 		atUIAPI_DestroyFont( atHFONT hFont );

atUI_OEM_FONT	atUIAPI_GetOEMPtr( atHFONT hFont );

atBOOL	atUIAPI_ChangeFont( atHFONT hFont, atLPTSTR font_face, int font_size, int font_style );

atINT         atUIAPI_GetStringWidth( atHFONT hFont, atLPTSTR szText, atINT nLength );
atINT         atUIAPI_GetFontHeight( atHFONT hFont );
atINT         atUIAPI_GetStringMetric( atHFONT hFont, atLPTSTR szText, atINT nLength, atSIZE *lpMetric );

atHFONT 	atUIAPI_GetStockFont( int font_idx );

atBOOL	   atUIAPI_ReleaseAllOEMFont( NOARGS );
atBOOL	   atUIAPI_ResyncAllOEMFont( NOARGS );


#ifdef __cplusplus
}
#endif


#endif // __FONT_H_

