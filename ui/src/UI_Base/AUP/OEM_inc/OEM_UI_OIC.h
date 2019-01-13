/**
  @file OEM_UI_OIC.h
  @brief OIC(OEM Input Control)에 관한 포팅 OEM API 선언
  @remark OIC(OEM Input Control)에 관한 포팅 OEM API 선언
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_OIC.h
*/
#ifndef __OEM_UI_OIC_H_
#define __OEM_UI_OIC_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef void * OEM_UI_OIC;


typedef atBOOL		FN_OEM_UI_InitOIC( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef void		FN_OEM_UI_FinalOIC(void);
typedef atBOOL		FN_OEM_UI_IsSupportedOIC(void);

typedef OEM_UI_OIC	FN_OEM_UI_CreateOIC( int x, int y, int width, int height, atUI_FUNC_OIC_NOTIFY *fnFunc, void *param );
typedef void		FN_OEM_UI_DestroyOIC( OEM_UI_OIC hOemOIC );

typedef void		FN_OEM_UI_SetOICFont( OEM_UI_OIC hOemOIC, int font_size );
typedef void		FN_OEM_UI_GetOICPos( OEM_UI_OIC hOemOIC, int *x, int *y );
typedef void		FN_OEM_UI_GetOICSize( OEM_UI_OIC hOemOIC, int *width, int *height );
typedef void		FN_OEM_UI_SetOICSize( OEM_UI_OIC hOemOIC, int width, int height );
typedef void		FN_OEM_UI_MoveOIC( OEM_UI_OIC hOemOIC, int x, int y );
typedef void		FN_OEM_UI_ShowAndSetFocusOIC( OEM_UI_OIC hOemOIC );
typedef void		FN_OEM_UI_HideAndLostFocusOIC( OEM_UI_OIC hOemOIC );
typedef void		FN_OEM_UI_RefreshOIC( OEM_UI_OIC hOemOIC );
typedef void		FN_OEM_UI_SetOICText( OEM_UI_OIC hOemOIC, atLPCTSTR szText, atINT nLen );
typedef void		FN_OEM_UI_GetOICText( OEM_UI_OIC hOemOIC, atLPTSTR szText, atINT nLen );


typedef struct tagOEM_UI_OIC {
	FN_OEM_UI_InitOIC				*fnInitOIC;
	FN_OEM_UI_FinalOIC				*fnFinalOIC;
	FN_OEM_UI_IsSupportedOIC		*fnIsSupportedOIC;

	FN_OEM_UI_CreateOIC				*fnCreateOIC;
	FN_OEM_UI_DestroyOIC			*fnDestroyOIC;

	FN_OEM_UI_SetOICFont			*fnSetOICFont;
	FN_OEM_UI_GetOICPos				*fnGetOICPos;
	FN_OEM_UI_GetOICSize			*fnGetOICSize;
	FN_OEM_UI_SetOICSize			*fnSetOICSize;
	FN_OEM_UI_MoveOIC				*fnMoveOIC;
	FN_OEM_UI_ShowAndSetFocusOIC	*fnShowAndSetFocusOIC;
	FN_OEM_UI_HideAndLostFocusOIC	*fnHideAndLostFocusOIC;
	FN_OEM_UI_RefreshOIC			*fnRefreshOIC;
	FN_OEM_UI_SetOICText			*fnSetOICText;
	FN_OEM_UI_GetOICText			*fnGetOICText;
} OEM_UI_OIC_T;


atVOID	OEM_UI_OIC_SetOemFn( OEM_UI_OIC_T *fnOem );
atVOID	OEM_UI_OIC_GetOemFn( OEM_UI_OIC_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif 
