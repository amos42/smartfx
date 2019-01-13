/**
  @file UI_OIC.h
  @brief OIC (OEM Input Control) 제어 API
  @remark OIC (OEM Input Control) 제어 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_OIC.c
*/
#ifndef __UI_OIC_H_
#define __UI_OIC_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef atHANDLE		atUI_OIC;

typedef int (atUI_FUNC_OIC_NOTIFY)( void *lpParam, int msg, long lParam1, long lParam2 );

enum {
	atUI_OICMSG_DUMMY = 0,
	atUI_OICMSG_CHANGE,
	atUI_OICMSG_MAX,
};


atBOOL 	atUIAPI_InitOIC( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
void 	atUIAPI_FinalOIC( NOARGS );
atBOOL 	atUIAPI_IsSupportedOIC( NOARGS );

atUI_OIC atUIAPI_CreateOIC( int x, int y, int width, int height, atUI_FUNC_OIC_NOTIFY *fnFunc, void *param );
void atUIAPI_DestroyOIC( atUI_OIC hOIC );

void atUIAPI_SetOICFont( atUI_OIC hOIC, int font_size );
void atUIAPI_GetOICPos( atUI_OIC hOIC, int *x, int *y );
void atUIAPI_GetOICSize( atUI_OIC hOIC, int *width, int *height );
void atUIAPI_SetOICSize( atUI_OIC hOIC, int width, int height );
void atUIAPI_MoveOIC( atUI_OIC hOIC, int x, int y );
void atUIAPI_ShowAndSetFocusOIC( atUI_OIC hOIC );
void atUIAPI_HideAndLostFocusOIC( atUI_OIC hOIC );
void atUIAPI_RefreshOIC( atUI_OIC hOIC );
void atUIAPI_SetOICText( atUI_OIC hOIC, atLPCTSTR szText, atINT nLen );
void atUIAPI_GetOICText( atUI_OIC hOIC, atLPTSTR szText, atINT nLen );


#ifdef __cplusplus
}
#endif


#endif 
