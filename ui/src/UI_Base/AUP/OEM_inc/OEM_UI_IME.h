/**
  @file OEM_UI_IME.h
  @brief IME에 관한 포팅 OEM API 선언
  @remark IME에 관한 포팅 OEM API 선언
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_IME.h
*/
#ifndef __OEM_UI_IME_H_
#define __OEM_UI_IME_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef void * OEM_UI_IME;
typedef void * OEM_UI_IMEMETHOD;


typedef int			FN_OEM_UI_InitIME( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef void		FN_OEM_UI_FinalIME(void);

typedef int 		FN_OEM_UI_GetIMEMethodClassStockCount(void);
typedef atBOOL 		FN_OEM_UI_GetIMEMethodClassStock( int nIndex, atUI_IMEMETHODCLASS *lpImeMethodClass );

typedef OEM_UI_IME 	FN_OME_UI_IME_CreateIME(void);
typedef int 		FN_OME_UI_IME_DestroyIME( OEM_UI_IME hOemIme );

typedef int 		FN_OEM_UI_IME_Start( OEM_UI_IME hOemIme );
typedef int 		FN_OEM_UI_IME_Input( OEM_UI_IME hOemIme, int keycode );
typedef int 		FN_OEM_UI_IME_GetComplete( OEM_UI_IME hOemIme, atLPTSTR szText );
typedef int 		FN_OEM_UI_IME_GetComposite( OEM_UI_IME hOemIme, atLPTSTR szTempStr );
typedef int 		FN_OEM_UI_IME_Flush( OEM_UI_IME hOemIme );
typedef int 		FN_OEM_UI_IME_Stop( OEM_UI_IME hOemIme );


typedef struct tagOEM_UI_IME {
	FN_OEM_UI_InitIME						*fnInitIME;
	FN_OEM_UI_FinalIME						*fnFinalIME;

	FN_OEM_UI_GetIMEMethodClassStockCount	*fnGetIMEMethodClassStockCount;
	FN_OEM_UI_GetIMEMethodClassStock		*fnGetIMEMethodClassStock;

	FN_OME_UI_IME_CreateIME					*fnIME_CreateIME;
	FN_OME_UI_IME_DestroyIME				*fnIME_DestroyIME;

	FN_OEM_UI_IME_Start						*fnIME_Start;
	FN_OEM_UI_IME_Input						*fnIME_Input;
	FN_OEM_UI_IME_GetComplete				*fnIME_GetComplete;
	FN_OEM_UI_IME_GetComposite				*fnIME_GetComposite;
	FN_OEM_UI_IME_Flush						*fnIME_Flush;
	FN_OEM_UI_IME_Stop						*fnIME_Stop;
} OEM_UI_IME_T;


atVOID	OEM_UI_IME_SetOemFn( OEM_UI_IME_T *fnOem );
atVOID	OEM_UI_IME_GetOemFn( OEM_UI_IME_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif 
