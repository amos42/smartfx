/**
  @file OEM_UI_Font.h
  @brief Font에 관한 포팅 OEM API 선언
  @remark Font에 관한 포팅 OEM API 선언
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Font.h
*/
#ifndef __OEM_UI_FONT_H_
#define __OEM_UI_FONT_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef void * OEM_UI_FONT;


typedef atVOID		FN_OEM_UI_InitFontSystem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef atVOID		FN_OEM_UI_CloseFontSystem( NOARGS );
typedef atVOID 		FN_OEM_UI_SuspendFontSystem( NOARGS );
typedef atVOID 		FN_OEM_UI_ResumeFontSystem( NOARGS );
typedef OEM_UI_FONT FN_OEM_UI_CreateFont( atLPTSTR font_face, atINT font_size, atINT font_style, atBOOL IsStock );
typedef atVOID 		FN_OEM_UI_DestroyFont( OEM_UI_FONT hFont );
typedef int			FN_OEM_UI_GetStringWidth( OEM_UI_FONT hFont, atLPTSTR str, int len );
typedef int			FN_OEM_UI_GetFontHeight( OEM_UI_FONT hFont );


typedef struct tagOEM_UI_FONT {
	FN_OEM_UI_InitFontSystem	*fnInitFontSystem;
	FN_OEM_UI_CloseFontSystem	*fnCloseFontSystem;
	FN_OEM_UI_SuspendFontSystem	*fnSuspendFontSystem;
	FN_OEM_UI_ResumeFontSystem	*fnResumeFontSystem;
	FN_OEM_UI_CreateFont		*fnCreateFont;
	FN_OEM_UI_DestroyFont		*fnDestroyFont;
	FN_OEM_UI_GetStringWidth	*fnGetStringWidth;
	FN_OEM_UI_GetFontHeight		*fnGetFontHeight;
} OEM_UI_FONT_T;


atVOID	OEM_UI_Font_SetOemFn( OEM_UI_FONT_T *fnOem );
atVOID	OEM_UI_Font_GetOemFn( OEM_UI_FONT_T *fnOem );


#ifdef __cplusplus
}
#endif

#endif // __FONT_WIPI_H_

