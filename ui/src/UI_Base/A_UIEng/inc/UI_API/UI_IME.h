/**
  @file UI_IME.h
  @brief IME (Input Method Engine)에 관한 API
  @remark IME (Input Method Engine)에 관한 API
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_IME.c
*/
#ifndef __UI_IME_H_
#define __UI_IME_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


#define atUI_IMEATTR_NONE			0x0000
#define atUI_IMEATTR_OEM			0x8000

#define atUI_IMEMETHODATTR_NONE					0x0000
#define atUI_IMEMETHODATTR_NORMAL				0x0000
#define atUI_IMEMETHODATTR_CANDIDATE_FULL		(0x8000 | 0x4000)
#define atUI_IMEMETHODATTR_CANDIDATE_PARTIAL	(0x8000 | 0x2000)
#define atUI_IMEMETHODATTR_CANDIDATE_STICK		(0x8000 | 0x1000)

typedef enum {
	atUI_IMESTATE_NONE,
	atUI_IMESTATE_READY,
	atUI_IMESTATE_COMPOSING,
	atUI_IMESTATE_USERDEF
} atUI_IMESTATE;

typedef enum {
	atUI_IMEMSG_CREATE_NONE = 0,
		
	atUI_IMEMSG_CREATE_INSTANCE,
	atUI_IMEMSG_DESTORY_INSTANCE,
	atUI_IMEMSG_GOT_FOCUS,
	atUI_IMEMSG_LOST_FOCUS,
	atUI_IMEMSG_SHOW_CANDIDATE,
	atUI_IMEMSG_HIDE_CANDIDATE,
	atUI_IMEMSG_START_COMPOSITE,
	atUI_IMEMSG_END_COMPOSITE,
	atUI_IMEMSG_COMPLETE_ONE,
	atUI_IMEMSG_COMPLETE_ALL,
	
	atUI_IMEMSG_CREATE_MAX		
} atUI_IMEMSG;

typedef enum {
	atUI_IMEMETHODMSG_NONE = 0,

	atUI_IMEMETHODMSG_GET_CLASSINFO,
	atUI_IMEMETHODMSG_CREATE_CLASS,
	atUI_IMEMETHODMSG_DESTROY_CLASS,
	
	atUI_IMEMETHODMSG_CREATE_INSTANCE,
	atUI_IMEMETHODMSG_DESTORY_INSTANCE,
	atUI_IMEMETHODMSG_START,
	atUI_IMEMETHODMSG_STOP,
	atUI_IMEMETHODMSG_GET_STATE,
	atUI_IMEMETHODMSG_GOT_FOCUS,
	atUI_IMEMETHODMSG_LOST_FOCUS,
	atUI_IMEMETHODMSG_SHOW_CANDIDATE,
	atUI_IMEMETHODMSG_IS_CANDIDATE,
	atUI_IMEMETHODMSG_DRAW_CANDIDATE,
	atUI_IMEMETHODMSG_START_COMPOSITE,
	atUI_IMEMETHODMSG_END_COMPOSITE,
	atUI_IMEMETHODMSG_INPUT,
	atUI_IMEMETHODMSG_GET_COMPOSETEXT,
	atUI_IMEMETHODMSG_GET_HILIGHT_LENGTH,
	atUI_IMEMETHODMSG_RESET,
	atUI_IMEMETHODMSG_FLUSH,
	atUI_IMEMETHODMSG_TIMEOUT,
	atUI_IMEMETHODMSG_COMPLETE_ONE,
	atUI_IMEMETHODMSG_COMPLETE_ALL,
	atUI_IMEMETHODMSG_GET_CURSORPOS,
	atUI_IMEMETHODMSG_SET_CURSORPOS,
	atUI_IMEMETHODMSG_CLEAR,
	
	atUI_IMEMETHODMSG_MAX
} atUI_IMEMETHODMSG;


typedef atHANDLE		atUI_HIME;
typedef atHANDLE		atUI_HIMEMETHODCLASS;
typedef atHANDLE		atUI_HIMEMETHOD;


typedef atINT	(atUI_IME_NOTIFY_FUNC)( atUI_HIME hIme, atINT nMsg, atLONG nParam1, atLONG nParam2 );
typedef atUI_IME_NOTIFY_FUNC * atLPUI_IME_NOTIFY_FUNC;

typedef atINT	(atUI_IMEMETHOD_FUNC)( atUI_HIMEMETHOD hImeMethod, atINT nMsg, atLONG nParam1, atLONG nParam2 );
typedef atUI_IMEMETHOD_FUNC * atLPUI_IMEMETHOD_FUNC;

typedef struct _tagatIMEMETHODCLASS {
	atTCHAR			szName[32];

	atDWORD		dwAttr;
	atDWORD		dwDataSize;
	atLPUI_IMEMETHOD_FUNC fnInputProc;
	atLONG		lParam;
	
	atDWORD		dwClassDataSize;
	atLPVOID	lpClassDataPtr;
} atUI_IMEMETHODCLASS;

typedef struct __tagatUI_IMEMETHOD {
	atTCHAR		szMethodName[32];
	atLPUI_IMEMETHOD_FUNC fnImeMethodFunc;
	atLPVOID		lpMethodData;
	atLONG		lParam;
} atUI_IMEMETHOD;



atBOOL 				atUIAPI_InitIME( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
void 				atUIAPI_FinalIME(void);

atUI_HIMEMETHODCLASS 	atUIAPI_IME_CreateIMEMethodClass( atLPTSTR szMethodName, atLPUI_IMEMETHOD_FUNC fnImeMethodFunc, atLONG lDataSize, atLONG lParam );
int 					atUIAPI_IME_DestroyIMEMethodClass( atUI_HIMEMETHOD hImeMethod );

atUI_HIMEMETHOD 	atUIAPI_IME_CreateIMEMethod( atUI_IMEMETHODCLASS *lpMethodClass );
atVOID 				atUIAPI_IME_DestroyIMEMethod( atUI_HIMEMETHOD hImeMethod );
atLPVOID 			atUIAPI_IME_GetIMEMethodData( atUI_HIMEMETHOD hImeMethod );
atINT 				atUIAPI_IME_CallIMEMethodFunc( atUI_HIMEMETHOD hImeMethod, atINT nMsg, atLONG nParam1, atLONG nParam2 );

atUI_HIME 			atUIAPI_IME_CreateIME( atLPTSTR szOutText, atINT nMaxLen, atLPUI_IME_NOTIFY_FUNC fnNotiFunc );
int 					atUIAPI_IME_DestroyIME( atUI_HIME hIme );
int					atUIAPI_IME_AddIMEMethod( atUI_HIME hIme, atUI_HIMEMETHOD hImeMethod );
int					atUIAPI_IME_RemoveIMEMethod( atUI_HIME hIme, atUI_HIMEMETHOD hImeMethod );

int 					atUIAPI_IME_GetIMEMethodCount( atUI_HIME hIme );
atUI_HIMEMETHOD 	atUIAPI_IME_GetIMEMethod( atUI_HIME hIme, atINT nIndex );

int					atUIAPI_IME_GetCurIMEMethodIndex( atUI_HIME hIme );
atUI_HIMEMETHOD 	atUIAPI_IME_GetCurIMEMethod( atUI_HIME hIme );
int					atUIAPI_IME_SetCurIMEMethod( atUI_HIME hIme, atINT nIndex );
int					atUIAPI_IME_SetCurIMEMethodByName( atUI_HIME hIme, atLPTSTR szImeMethodName );
int					atUIAPI_IME_ChangePrevIMEMethod( atUI_HIME hIme );
int					atUIAPI_IME_ChangeNextIMEMethod( atUI_HIME hIme );

int 					atUIAPI_IME_GetMaxLen( atUI_HIME hIme );
int 					atUIAPI_IME_SetMaxLen( atUI_HIME hIme, atINT nLen );
int 					atUIAPI_IME_SetText( atUI_HIME hIme, atLPTSTR szText );
int 					atUIAPI_IME_GetText( atUI_HIME hIme, atLPTSTR szText );
int 					atUIAPI_IME_SetNotifyFunc( atUI_HIME hIme, atLPUI_IME_NOTIFY_FUNC fnNotifyFunc );

int 					atUIAPI_IME_Start( atUI_HIME hIme );
int 					atUIAPI_IME_Stop( atUI_HIME hIme );
int 					atUIAPI_IME_GetState( atUI_HIME hIme );
int 					atUIAPI_IME_Input( atUI_HIME hIme, int keycode );
int 					atUIAPI_IME_GetFullCompositeText( atUI_HIME hIme, atLPTSTR szText );
int 					atUIAPI_IME_GetComposeText( atUI_HIME hIme, atLPTSTR szTempStr );
int 					atUIAPI_IME_GetHilighttLength( atUI_HIME hIme );

int					atUIAPI_IME_GetCursorPos( atUI_HIME hIme );
int					atUIAPI_IME_SetCursorPos( atUI_HIME hIme, atINT nPos );
int					atUIAPI_IME_Reset( atUI_HIME hIme );
int 					atUIAPI_IME_Flush( atUI_HIME hIme, atBOOL bMoveCursor );
int 					atUIAPI_IME_TimeOut( atUI_HIME hIme );
int					atUIAPI_IME_Clear( atUI_HIME hIme );
int					atUIAPI_IME_InsertString( atUI_HIME hIme, atINT nCursorPos, atLPTSTR szText, atINT nLen, atBOOL bIsFlushComposite );

int 					atUIAPI_IME_DrawCandidate( atUI_HIME hIme );
int					atUIAPI_IME_ShowCandidate( atUI_HIME hIme, atBOOL bShow );
atBOOL				atUIAPI_IME_IsCandidate( atUI_HIME hIme ); 

int					atUIAPI_IME_RegistIMEMethodClassStock( atUI_HIMEMETHODCLASS hImeMethodClass );
int					atUIAPI_IME_GetIMEMethodClassStockCount( void );
int					atUIAPI_IME_GetIMEMethodClassStock( atINT nIndex );
int					atUIAPI_IME_GetIMEMethodClassStockByName( atLPTSTR szMethodName );
atUI_HIMEMETHOD 	atUIAPI_IME_CreateIMEMethodFromStock( atLPTSTR szMethodName, atUI_IMEMETHODCLASS *lpMethodClass );
atUI_HIMEMETHOD 	atUIAPI_IME_CreateIMEMethodByName( atLPTSTR szMethodName );
atUI_HIME 			atUIAPI_IME_CreateIMEByName( atLPTSTR szOutText, atINT nMaxLen, atLPUI_IME_NOTIFY_FUNC fnNotiFunc, atLPTSTR szImeMethod, ... );


#ifdef __cplusplus
}
#endif


#endif
