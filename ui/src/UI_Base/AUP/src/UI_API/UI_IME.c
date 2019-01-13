/**
  @file UI_IME.c
  @brief IME에 관한 API
  @remark IME에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_IME.h
*/
#include "UI_API/UI_API.h"
#include "OEM_UI_IME.h"



#ifdef __USES_UI_IME_

#include <stdarg.h>

#define MAX_INPUT_MODE 12

typedef struct _tagatIMEMETHODCLASS_ITOR {
	atUI_IMEMETHODCLASS	tImeMethodClass;
	
	atDWORD				dwClassDataSize;
	atLPVOID			lpClassDataPtr;
} atUI_IMEMETHODCLASS_ITOR;

typedef struct __tagatUI_IME {
	int nModeCount;
	atUI_IMEMETHOD *lstInputMode[MAX_INPUT_MODE];
	int				nCurIMEMethodIndex;

	int			nMaxLen;
	atLPTSTR		szText;	
//	atTCHAR		szTempStr[32];

	atLPUI_IME_NOTIFY_FUNC fnNotifyFunc;
	atLPVOID		lpData;
	atLPVOID		OemImeInst;
} atUI_IME;



static int		g_nIMEMethodClassCount = 0;
//static atUI_IMEMETHODCLASS_ITOR g_lstIMEMethodClassList[32];
//atUILIST g_lstIMEMethodClass;
static atUI_HIMEMETHODCLASS g_lstIMEMethodClassList[32] = { atNULL,};
static atINT	defDummyIMEMethod( atUI_HIMEMETHOD hImeMethod, atINT nMsg, atLONG nParam1, atLONG nParam2 );


static OEM_UI_IME_T tOemFn;

atVOID	OEM_UI_IME_SetOemFn( OEM_UI_IME_T *fnOem )
{
	tOemFn = *fnOem;
}


atVOID	OEM_UI_IME_GetOemFn( OEM_UI_IME_T *fnOem )
{
	*fnOem = tOemFn;
}


/** 
 @brief IME(Input Method Engine)를 초기화 한다. (내부용)
 
 IME(Input Method Engine)를 초기화 한다.

 @param lSysInst [in] : UI엔진의 인스턴스 (OEM 전달용. 내부용)
 @param lInitData1, lInitData2 [in] : 초기화 파라미터 (OEM 전달용. 내부용)
 @return 성공하면 atTRUE, 실패하면 atFALSE
 @remark 내부용으로 사용된다.
*/
atBOOL atUIAPI_InitIME( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	int i, cnt;
	atUI_IMEMETHODCLASS ime_class;
	atUI_HIMEMETHODCLASS hImeClass;

	tOemFn.fnInitIME( lSysInst, lInitData1, lInitData2 );

//	g_lstIMEMethodClass = atUIAPI_CreateList();

	defDummyIMEMethod( atNULL, atUI_IMEMETHODMSG_GET_CLASSINFO, (long)&ime_class, 0 );
	hImeClass = atUIAPI_IME_CreateIMEMethodClass( ime_class.szName, ime_class.fnInputProc, ime_class.dwDataSize, ime_class.lParam );
	atUIAPI_IME_RegistIMEMethodClassStock( hImeClass );

	cnt = tOemFn.fnGetIMEMethodClassStockCount();
	for( i = 0; i < cnt; i++ ){
		tOemFn.fnGetIMEMethodClassStock( i, &ime_class );
		hImeClass = atUIAPI_IME_CreateIMEMethodClass( ime_class.szName, ime_class.fnInputProc, ime_class.dwDataSize, ime_class.lParam );
		atUIAPI_IME_RegistIMEMethodClassStock( hImeClass );
	}
	
	return atTRUE;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_FinalIME( NOARGS )
{
/*
	for( i = 0; i < cnt; i++ ){
		imemethod = atUIAPI_GetListItem(i);
		atUIAPI_DestroyIMEMethod( imemethod );
	}
*/
	int ii =0;
	for(ii = 0; ii < g_nIMEMethodClassCount; ii++)
	{
		atUIAPI_IME_DestroyIMEMethodClass(g_lstIMEMethodClassList[ii]);
		g_lstIMEMethodClassList[ii] = atNULL;
	}

	g_nIMEMethodClassCount = 0;
	
//	atUIAPI_DestroyList( g_lstIMEMethodClass, atFALSE );
	
	tOemFn.fnFinalIME();
}

//typedef atINT	(atUI_IMEMETHOD_FUNC)( atUI_HIME hIme, atUI_HIMEMETHOD hImeMethod, atINT nMsg, atLONG nParam1, atLONG nParam2 );
//typedef atUI_IMEMETHOD_FUNC * atLPUI_IMEMETHOD_FUNC;


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atUI_HIMEMETHODCLASS 	atUIAPI_IME_CreateIMEMethodClass( atLPTSTR szMethodName, atLPUI_IMEMETHOD_FUNC fnImeMethodFunc, atLONG lDataSize, atLONG lParam )
{
	atUI_IMEMETHODCLASS_ITOR *lpImeMethodClass;
	
	lpImeMethodClass = (atUI_IMEMETHODCLASS_ITOR *)atUIAPI_AllocMem( sizeof(atUI_IMEMETHODCLASS_ITOR) );
	if( lpImeMethodClass == atNULL ) return atNULL;
	
	atUIAPI_memset( lpImeMethodClass, 0, sizeof(atUI_IMEMETHODCLASS_ITOR) );

//	atUIAPI_strncpy( lpImeMethodClass->szName, szMethodName, 32 );
//	lpImeMethodClass->fnInputProc = fnImeMethodFunc;
//	lpImeMethodClass->dwDataSize = lDataSize;
//	lpImeMethodClass->lParam = lParam;
	atUIAPI_strncpy( lpImeMethodClass->tImeMethodClass.szName, szMethodName, 32 );
	lpImeMethodClass->tImeMethodClass.dwDataSize = lDataSize;
	lpImeMethodClass->tImeMethodClass.lParam = lParam;
	lpImeMethodClass->tImeMethodClass.fnInputProc = fnImeMethodFunc;

	lpImeMethodClass->tImeMethodClass.fnInputProc( atNULL, atUI_IMEMETHODMSG_CREATE_CLASS, (long)lpImeMethodClass, 0 );

	return (atUI_HIMEMETHODCLASS)lpImeMethodClass;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_DestroyIMEMethodClass( atUI_HIMEMETHODCLASS hImeMethodClass )
{
	atUI_IMEMETHODCLASS_ITOR *lpImeMethodClass = (atUI_IMEMETHODCLASS_ITOR *)hImeMethodClass;
	if( lpImeMethodClass == atNULL ) return -1;

	lpImeMethodClass->tImeMethodClass.fnInputProc( atNULL, atUI_IMEMETHODMSG_DESTROY_CLASS, 0, 0 );

	atUIAPI_FreeMem( lpImeMethodClass );

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atUI_HIMEMETHOD 	atUIAPI_IME_CreateIMEMethod( atUI_IMEMETHODCLASS *lpMethodClass )
{
	atUI_HIMEMETHOD hImeMethod;

	atUI_IMEMETHOD *lpImeMethod = (atUI_IMEMETHOD *)atUIAPI_AllocMem( sizeof(atUI_IMEMETHOD) + lpMethodClass->dwDataSize );
	if( lpImeMethod == atNULL ) return atNULL;
	atUIAPI_memset( lpImeMethod, 0, sizeof(atUI_IMEMETHOD) );

	lpImeMethod->lpMethodData = (atLPVOID)((char *)lpImeMethod + sizeof(atUI_IMEMETHOD));
	atUIAPI_strncpy( lpImeMethod->szMethodName, lpMethodClass->szName, 32 );
	lpImeMethod->fnImeMethodFunc = lpMethodClass->fnInputProc;
	lpImeMethod->lParam = lpMethodClass->lParam;

	hImeMethod = (atUI_HIMEMETHOD)lpImeMethod;
	lpImeMethod->fnImeMethodFunc( hImeMethod, atUI_IMEMETHODMSG_CREATE_INSTANCE, 0, 0 );

	return hImeMethod;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID 	atUIAPI_IME_DestroyIMEMethod( atUI_HIMEMETHOD hImeMethod )
{
	atUI_IMEMETHOD *lpImeMethod = (atUI_IMEMETHOD *)hImeMethod;
	if( lpImeMethod == atNULL ) return;

	lpImeMethod->fnImeMethodFunc( hImeMethod, atUI_IMEMETHODMSG_DESTORY_INSTANCE, 0, 0 );

	atUIAPI_FreeMem( lpImeMethod );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atLPVOID 	atUIAPI_IME_GetIMEMethodData( atUI_HIMEMETHOD hImeMethod )
{
	atUI_IMEMETHOD *lpImeMethod = (atUI_IMEMETHOD *)hImeMethod;
	if( lpImeMethod == atNULL ) return atNULL;

	return (atLPVOID)lpImeMethod->lpMethodData;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atINT 	atUIAPI_IME_CallIMEMethodFunc( atUI_HIMEMETHOD hImeMethod, atINT nMsg, atLONG nParam1, atLONG nParam2 )
{
	atUI_IMEMETHOD *lpImeMethod = (atUI_IMEMETHOD *)hImeMethod;
	if( lpImeMethod == atNULL ) return -1;
	if( lpImeMethod->fnImeMethodFunc == atNULL ) return -1;

	return lpImeMethod->fnImeMethodFunc( hImeMethod, nMsg, nParam1, nParam2 );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atUI_HIME 	atUIAPI_IME_CreateIME( atLPTSTR szOutText, atINT nMaxLen, atLPUI_IME_NOTIFY_FUNC fnNotiFunc )
{
	atUI_IME *lpIme;

	lpIme = atUIAPI_AllocMem( sizeof(atUI_IME) );
	if( lpIme == atNULL ) return atNULL;

	atUIAPI_memset( lpIme, 0, sizeof(atUI_IME) );

	lpIme->fnNotifyFunc = fnNotiFunc;
	//lpIme->nInputMode = -1;
	lpIme->nModeCount = 0;
	lpIme->szText = szOutText;
	lpIme->nMaxLen = nMaxLen;
//	if( lpIme->szText ) lpIme->szText[0] = _ATC('\0');

	return (atUI_HIME)lpIme;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_DestroyIME( atUI_HIME hIme )
{
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;



	atUIAPI_FreeMem( lpIme );

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int	atUIAPI_IME_AddIMEMethod( atUI_HIME hIme, atUI_HIMEMETHOD hImeMethod )
{
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	if( lpIme->nModeCount >= MAX_INPUT_MODE ) return -1;

	lpIme->lstInputMode[lpIme->nModeCount] = hImeMethod;
	lpIme->nModeCount++;

	return lpIme->nModeCount;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int	atUIAPI_IME_RemoveIMEMethod( atUI_HIME hIme, atUI_HIMEMETHOD hImeMethod )
{
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	//

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int atUIAPI_IME_GetIMEMethodCount( atUI_HIME hIme )
{
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	return lpIme->nModeCount;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atUI_HIMEMETHOD atUIAPI_IME_GetIMEMethod( atUI_HIME hIme, atINT nIndex )
{
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return atNULL;
	if( (nIndex < 0) || (nIndex >= lpIme->nModeCount) ) return atNULL;
	
	return (atUI_HIMEMETHOD)lpIme->lstInputMode[nIndex];
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int	atUIAPI_IME_GetCurIMEMethodIndex( atUI_HIME hIme )
{
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	return lpIme->nCurIMEMethodIndex;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atUI_HIMEMETHOD atUIAPI_IME_GetCurIMEMethod( atUI_HIME hIme )
{
	return atUIAPI_IME_GetIMEMethod( hIme, atUIAPI_IME_GetCurIMEMethodIndex(hIme) );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int	atUIAPI_IME_SetCurIMEMethod( atUI_HIME hIme, atINT nIndex )
{
	atUI_HIMEMETHOD ime_method;
	int cursor;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	if( nIndex >= 0 && nIndex < lpIme->nModeCount ){
		ime_method = atUIAPI_IME_GetIMEMethod( hIme, lpIme->nCurIMEMethodIndex );
		if( ime_method ){
			atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_FLUSH, 0, 0 );
			cursor = atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_GET_CURSORPOS, 0, 0 );
			atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_STOP, 0, 0 );
		} else {
			cursor = -1;
		}
		
		lpIme->nCurIMEMethodIndex = nIndex;
		
		ime_method = atUIAPI_IME_GetIMEMethod( hIme, lpIme->nCurIMEMethodIndex );
		if( ime_method ){
			atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_START, (long)lpIme->szText, (long)lpIme->nMaxLen );
			if( cursor >= 0 ) atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_SET_CURSORPOS, (long)cursor, 0 );
		}
	}

	return lpIme->nCurIMEMethodIndex;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int	atUIAPI_IME_SetCurIMEMethodByName( atUI_HIME hIme, atLPTSTR szImeMethodName )
{
	int i, idx;
	atUI_HIMEMETHOD hImeMethod;
	atUI_IMEMETHOD *ime_method;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	idx = -1;
	for( i = 0; i < lpIme->nModeCount; i++ ){
		hImeMethod = lpIme->lstInputMode[i];
		ime_method = (atUI_IMEMETHOD *)hImeMethod;
		if( ime_method == atNULL ) break;
		if( atUIAPI_strcmp( ime_method->szMethodName, szImeMethodName ) == 0 ){
			idx = i;
			atUIAPI_IME_SetCurIMEMethod( hIme, idx );
			break;
		}
	}

	return idx;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int	atUIAPI_IME_ChangePrevIMEMethod( atUI_HIME hIme )
{
	int idx;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	idx = atUIAPI_IME_GetCurIMEMethodIndex( hIme ) - 1;
	if( idx < 0 ) idx = atUIAPI_IME_GetIMEMethodCount(hIme) - 1;
	return atUIAPI_IME_SetCurIMEMethod( hIme, idx );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int	atUIAPI_IME_ChangeNextIMEMethod( atUI_HIME hIme )
{
	int idx;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	idx = atUIAPI_IME_GetCurIMEMethodIndex( hIme ) + 1;
	if( idx >= atUIAPI_IME_GetIMEMethodCount(hIme) ) idx = 0;
	return atUIAPI_IME_SetCurIMEMethod( hIme, idx );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_GetMaxLen( atUI_HIME hIme )
{
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	return lpIme->nMaxLen;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_SetMaxLen( atUI_HIME hIme, atINT nLen )
{
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	lpIme->nMaxLen = nLen;

	return lpIme->nMaxLen;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_SetText( atUI_HIME hIme, atLPTSTR szText )
{
	int nCursorPos;
	atUI_HIMEMETHOD ime_method;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;
	if( lpIme->szText == atNULL ) return 0;

	atUIAPI_strncpy( lpIme->szText, szText, lpIme->nMaxLen );

	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
	if( ime_method == atNULL ) return 0;

	if( ime_method != atNULL ){
		nCursorPos = atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_GET_CURSORPOS, 0, 0 );
		if( nCursorPos > atUIAPI_strlen(lpIme->szText) ){
			nCursorPos = atUIAPI_strlen(lpIme->szText);
			atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_SET_CURSORPOS, nCursorPos, 0 );
		}
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_GetText( atUI_HIME hIme, atLPTSTR szText )
{
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	atUIAPI_strcpy( szText, lpIme->szText );

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_SetNotifyFunc( atUI_HIME hIme, atLPUI_IME_NOTIFY_FUNC fnNotifyFunc )
{
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	lpIme->fnNotifyFunc = fnNotifyFunc;

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_Start( atUI_HIME hIme )
{
	atUI_HIMEMETHOD ime_method;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return -1;

	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
	if( ime_method == atNULL ) return -1;
	atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_START, (long)lpIme->szText, lpIme->nMaxLen );
//	lpIme->fnNotifyFunc( hIme, atUI_IMEMSG_START, 0, 0 );


	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_Stop( atUI_HIME hIme )
{
	atUI_HIMEMETHOD ime_method;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return -1;

	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
	if( ime_method == atNULL ) return -1;

	atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_STOP, 0, 0 );
//	lpIme->fnNotifyFunc( hIme, atUI_IMEMSG_STOP, 0, 0 );

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int	atUIAPI_IME_GetState( atUI_HIME hIme )
{
	atUI_HIMEMETHOD ime_method;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return atUI_IMESTATE_NONE;

	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
	if( ime_method == atNULL ) return atUI_IMESTATE_NONE;

	return atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_GET_STATE, 0, 0 );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_Input( atUI_HIME hIme, int keycode )
{
	atUI_HIMEMETHOD ime_method;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
	if( ime_method == atNULL ) return -1;
	return atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_INPUT, keycode, 0 );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_GetFullCompositeText( atUI_HIME hIme, atLPTSTR szText )
{
	int r, nCursorPos;
	atUI_HIMEMETHOD ime_method;
	atTCHAR szTempStr[32];
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	atUIAPI_strcpy( szText, lpIme->szText );
	if( atUIAPI_IME_GetState(hIme) == atUI_IMESTATE_COMPOSING ){
		r = atUIAPI_IME_GetComposeText( hIme, szTempStr );
		if( (r >= 0) && atUIAPI_strlen( szTempStr ) > 0 ){
			ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
			if( ime_method != atNULL ){
				nCursorPos = atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_GET_CURSORPOS, 0, 0 );
				atUIAPI_strins( szText, szTempStr, nCursorPos, -1 );
			}
		}
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_GetComposeText( atUI_HIME hIme, atLPTSTR szTempStr )
{
	atUI_HIMEMETHOD ime_method;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

//	atUIAPI_strcpy( szTempStr, lpIme->szTempStr, 32 );
	szTempStr[0] = _ATC('\0');
	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
	if( ime_method == atNULL ) return -1;
	return atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_GET_COMPOSETEXT, (long)szTempStr, 0 );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_GetHilighttLength( atUI_HIME hIme )
{
	atUI_HIMEMETHOD ime_method;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return -1;

	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
	if( ime_method == atNULL ) return 0;
	return atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_GET_HILIGHT_LENGTH, 0, 0 );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int	atUIAPI_IME_SetCursorPos( atUI_HIME hIme, atINT nPos )
{
	int str_len;
	atUI_HIMEMETHOD ime_method;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return -1;

	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
	if( ime_method == atNULL ) return -1;

//	ime_method->fnImeMethodFunc( ime_method, atUI_IMEMETHODMSG_INPUT, nPos, 0 );

	if( lpIme->szText == atNULL ) return -1;
	str_len = atUIAPI_strlen( lpIme->szText );
	if( nPos > str_len ) nPos = str_len;
	if( nPos >= lpIme->nMaxLen ) nPos = lpIme->nMaxLen - 1;
	if( nPos < 0 ) nPos = 0;
	
//	lpIme->nCursorPos = nPos;

//	return lpIme->nCursorPos;

	return atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_SET_CURSORPOS, nPos, 0 );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int	atUIAPI_IME_GetCursorPos( atUI_HIME hIme )
{
	atUI_HIMEMETHOD ime_method;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return -1;

	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
	if( ime_method == atNULL ) return -1;

	return atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_GET_CURSORPOS, 0, 0 );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_Reset( atUI_HIME hIme )
{
	atUI_HIMEMETHOD ime_method;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
	if( ime_method == atNULL ) return -1;

	return atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_RESET, 0, 0 );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_Flush( atUI_HIME hIme, atBOOL bMoveCursor )
{
	atUI_HIMEMETHOD ime_method;
	atTCHAR szTempStr[32];
	int cursor_pos, len;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
	if( ime_method == atNULL ) return 0;

	if( atUIAPI_IME_GetState(hIme) == atUI_IMESTATE_COMPOSING ){
		atUIAPI_IME_GetComposeText( hIme, szTempStr );
		len = atUIAPI_strlen(szTempStr);
		if( len > 0 ){
			cursor_pos = atUIAPI_IME_GetCursorPos( hIme );
			atUIAPI_strins( lpIme->szText, szTempStr, cursor_pos, -1 );
			if( bMoveCursor ) atUIAPI_IME_SetCursorPos( hIme, cursor_pos + len );
		}	
	}

//	ime_method->fnImeMethodFunc( ime_method, atUI_IMEMETHODMSG_FLUSH, 0, 0 );
	atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_RESET, 0, 0 );

	return len;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_TimeOut( atUI_HIME hIme )
{
	atUI_HIMEMETHOD ime_method;
	//atTCHAR szTempStr[32];
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
	if( ime_method == atNULL ) return 0;

	if( atUIAPI_IME_GetState(hIme) == atUI_IMESTATE_COMPOSING ){
		atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_TIMEOUT, 0, 0 );
	}

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int		atUIAPI_IME_Clear( atUI_HIME hIme )
{
//	int lLen, lLen2;
	atUI_HIMEMETHOD ime_method;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return -1;
	if( lpIme->szText == atNULL ) return -1;
	
	atUIAPI_strcpy(lpIme->szText, _AT(""));

	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
	if( ime_method == atNULL ) return -1;

//	ime_method->fnImeMethodFunc( ime_method, atUI_IMEMETHODMSG_CLEAR, 0, 0 );
	atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_RESET, 0, 0 );
	return atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_SET_CURSORPOS, 0, 0 );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int		atUIAPI_IME_InsertString( atUI_HIME hIme, atINT nCursorPos, atLPTSTR szText, atINT nLen, atBOOL bIsFlushComposite )
{
	int lLen, lLen2;
//	atUI_HIMEMETHOD ime_method;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return -1;
	if( lpIme->szText == atNULL ) return -1;
	
	lLen = atUIAPI_strlen(lpIme->szText);
	if( lLen >= lpIme->nMaxLen ) return -1;
	lLen2 = atUIAPI_strlen(szText);
	if( (lLen + lLen2) >= lpIme->nMaxLen ) return -1;

	if( bIsFlushComposite ) atUIAPI_IME_Flush( hIme, atFALSE );

	atUIAPI_strins( lpIme->szText, szText, nCursorPos, -1 );
	nCursorPos += lLen2;
//	lpIme->nCursorPos = nCursorPos;

//	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
//	if( ime_method != atNULL ){
//		atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_SET_CURSORPOS, nCursorPos, 0 );
//	}

	return nCursorPos;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int 	atUIAPI_IME_DrawCandidate( atUI_HIME hIme )
{
	atUI_HIMEMETHOD ime_method;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
	if( ime_method == atNULL ) return -1;

	atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_DRAW_CANDIDATE, 0, 0 );

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int	atUIAPI_IME_ShowCandidate( atUI_HIME hIme, atBOOL bShow )
{
	atUI_HIMEMETHOD ime_method;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
	if( ime_method == atNULL ) return -1;

	atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_SHOW_CANDIDATE, (long)bShow, 0 );

	return 0;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL	atUIAPI_IME_IsCandidate( atUI_HIME hIme )
{
	atUI_HIMEMETHOD ime_method;
	atLONG bShow;
	atUI_IME *lpIme = (atUI_IME *)hIme;
	if( lpIme == atNULL ) return 0;

	bShow = 0;
	ime_method = atUIAPI_IME_GetCurIMEMethod( hIme );
	if( ime_method == atNULL ) return -1;

	atUIAPI_IME_CallIMEMethodFunc( ime_method, atUI_IMEMETHODMSG_IS_CANDIDATE, (long)&bShow, 0 );

	return (bShow != 0)? atTRUE : atFALSE;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
int	atUIAPI_IME_RegistIMEMethodClassStock( atUI_HIMEMETHODCLASS hImeMethodClass )
{
//	atUI_IMEMETHODCLASS *lpImeMethodClass;
	atUI_HIMEMETHODCLASS *lpImeMethodClass;
	
	if( g_nIMEMethodClassCount >= 32 ) return -1;

	lpImeMethodClass = &g_lstIMEMethodClassList[g_nIMEMethodClassCount];
	g_nIMEMethodClassCount++;

	*lpImeMethodClass = hImeMethodClass;
//	atUIAPI_strncpy( lpImeMethodClass->szName, szMethodName, 32 );
//	lpImeMethodClass->fnInputProc = fnImeMethodFunc;
//	lpImeMethodClass->dwDataSize = lDataSize;
//	lpImeMethodClass->lParam = lParam;

	return g_nIMEMethodClassCount;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atUI_HIMEMETHOD 	atUIAPI_IME_CreateIMEMethodByName( atLPTSTR szMethodName )
{
	int i, idx;
	atUI_HIMEMETHODCLASS hImeMethodClass;
	atUI_IMEMETHODCLASS *lpImeMethodClass;

	if( szMethodName == atNULL ) return atNULL;
	
	idx = -1;
	for( i = 0; i < g_nIMEMethodClassCount; i++ ){
		hImeMethodClass = g_lstIMEMethodClassList[i];
		lpImeMethodClass = (atUI_IMEMETHODCLASS *)hImeMethodClass;
		if( lpImeMethodClass == atNULL ) break;
		if( atUIAPI_strcmp(lpImeMethodClass->szName, szMethodName) == 0 ){
			idx = i;
			break;
		}
	}
	if( idx < 0 ) return atNULL;

	return atUIAPI_IME_CreateIMEMethod( lpImeMethodClass );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atUI_HIME 	atUIAPI_IME_CreateIMEByName( atLPTSTR szOutText, atINT nMaxLen, atLPUI_IME_NOTIFY_FUNC fnNotiFunc, atLPTSTR szImeMethod, ... )
{
	atUI_HIME hIme;
	atUI_HIMEMETHOD hImeMethod;
	int cnt;
    va_list ap;
	atLPTSTR p;

	hIme = atUIAPI_IME_CreateIME( szOutText, nMaxLen, fnNotiFunc );
	if( hIme == atNULL ) return atNULL;

	cnt = 0;
    va_start(ap, szImeMethod);
	p = szImeMethod;
	while( p != atNULL ){
		hImeMethod = atUIAPI_IME_CreateIMEMethodByName( p );
		if( hImeMethod != atNULL ){
			atUIAPI_IME_AddIMEMethod( hIme, hImeMethod );
			cnt++;
		}
		p = va_arg(ap, atLPTSTR);
	}
	va_end(ap);

	if( cnt > 0 ){
		atUIAPI_IME_SetCurIMEMethod( hIme, 0 );
	}
	
	return hIme;
}


typedef struct tagIME_DUMMY_TBL {
	int nCursorPos;
	atLPTSTR szText;
} IME_DUMMY_TBL;

static atINT	defDummyIMEMethod( atUI_HIMEMETHOD hImeMethod, atINT nMsg, atLONG nParam1, atLONG nParam2 )
{
	atUI_IMEMETHODCLASS *lpImeMethodClass;
	IME_DUMMY_TBL *lpImeData;

	switch( nMsg ){
		case atUI_IMEMETHODMSG_GET_CLASSINFO :
			lpImeMethodClass = (atUI_IMEMETHODCLASS *)nParam1;
			atUIAPI_strncpy( lpImeMethodClass->szName, _AT("Dummy"), 32 );
			lpImeMethodClass->dwAttr = 0;
			lpImeMethodClass->dwDataSize = sizeof(IME_DUMMY_TBL);
			lpImeMethodClass->fnInputProc = defDummyIMEMethod;
			lpImeMethodClass->lParam = nParam2;
			break;
		case atUI_IMEMETHODMSG_CREATE_CLASS :
			break;
		case atUI_IMEMETHODMSG_DESTROY_CLASS :
			break;

		case atUI_IMEMETHODMSG_CREATE_INSTANCE : 
			lpImeData = (IME_DUMMY_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
			if( lpImeData == atNULL ) return -1;
			lpImeData->nCursorPos = 0;
			break;

		case atUI_IMEMETHODMSG_DESTORY_INSTANCE :
			break;

		case atUI_IMEMETHODMSG_START :
			lpImeData = (IME_DUMMY_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
			if( lpImeData == atNULL ) return -1;
			lpImeData->szText = (atLPTSTR)nParam1;
			break;

		case atUI_IMEMETHODMSG_STOP :
			break;

		case atUI_IMEMETHODMSG_GET_STATE :
			return 0;

		case atUI_IMEMETHODMSG_INPUT : 
			lpImeData = (IME_DUMMY_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
			if( lpImeData == atNULL ) return -1;
			if( lpImeData->szText == atNULL ) return -1;
			switch( nParam1 ){
				case atVKEY_LEFT:
					if( lpImeData->nCursorPos > 0 ){
						lpImeData->nCursorPos --;
					}
					break;					
				case atVKEY_RIGHT:
					if( lpImeData->nCursorPos < atUIAPI_strlen(lpImeData->szText) ){
						lpImeData->nCursorPos ++;
					}
					break;					
				case atVKEY_CLEAR:
					if( atUIAPI_strlen(lpImeData->szText) > 0 ){
						if( lpImeData->nCursorPos > 0 ){
							lpImeData->nCursorPos--;
							atUIAPI_strdel( lpImeData->szText, lpImeData->nCursorPos, 1 );
						}
					}
					break;
			}			
			break;

		case atUI_IMEMETHODMSG_RESET :
			break;

		case atUI_IMEMETHODMSG_GET_CURSORPOS :
			lpImeData = (IME_DUMMY_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
			if( lpImeData == atNULL ) return -1;
			return lpImeData->nCursorPos;
		
		case atUI_IMEMETHODMSG_SET_CURSORPOS :
			lpImeData = (IME_DUMMY_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
			if( lpImeData == atNULL ) return -1;
			if( nParam1 < 0 ) nParam1 = 0;
			lpImeData->nCursorPos = nParam1;
			break;

		case atUI_IMEMETHODMSG_GET_COMPOSETEXT :
			break;	

		case atUI_IMEMETHODMSG_GET_HILIGHT_LENGTH :
			return 0;
			
		case atUI_IMEMETHODMSG_TIMEOUT :
			break;
	}

	return 0;
}

#endif
