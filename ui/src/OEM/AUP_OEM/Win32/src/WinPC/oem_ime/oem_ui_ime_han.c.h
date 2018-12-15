#include "A_UIENG.h"
#include "OEM_UI_IME.h"
#include "KorAutomata.h"
#include <string.h>
#include <stdio.h>

//atTCHAR OEM_UI_IME_Buffer[256];
//atLPUI_IME_NOTIFY_FUNC fnImeMethodFunc;


typedef struct tagIME_HANGUL_TBL {
	CKoreanAutomata * pAutomata;

	int nCursorPos;
	atTCHAR szTempStr[32];
	atLPTSTR szText;
} IME_HANGUL_TBL;




static atINT _IME_hangul_OnCreateClass( atUI_IMEMETHODCLASS *lpImeMethodClass )
{
	is_uni_table = atTRUE;

	lpImeMethodClass->dwAttr = 0;
	lpImeMethodClass->dwDataSize = sizeof(IME_HANGUL_TBL);

	return 0;
}


static atINT _IME_hangul_OnDestroyClass(void)
{
	return 0;
}


static void _IME_hangul_OnCreateInstance( atUI_HIMEMETHOD hImeMethod, atLPTSTR szText )
{
	atUI_IMEMETHOD *lpImeMethod = (atUI_IMEMETHOD *)hImeMethod;
	IME_HANGUL_TBL *lpImeData = (IME_HANGUL_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return;

	lpImeData->pAutomata = new CKoreanAutomata;	
	lpImeData->nCursorPos = 0;
	lpImeData->szText = szText;
	atUIAPI_strcpy(lpImeData->szTempStr, _AT(""));
}

static void _IME_hangul_OnDestroyInstance( atUI_HIMEMETHOD hImeMethod )
{
	IME_HANGUL_TBL *lpImeData = (IME_HANGUL_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return;

	delete lpImeData->pAutomata;	
}


static void _IME_hangul_OnReset( atUI_HIMEMETHOD hImeMethod )
{
	IME_HANGUL_TBL *lpImeData = (IME_HANGUL_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return;

	atUIAPI_strcpy(lpImeData->szTempStr, _AT(""));
}

static void _IME_hangul_OnFlush( atUI_HIMEMETHOD hImeMethod )
{
	IME_HANGUL_TBL *lpImeData = (IME_HANGUL_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return;
	
	lpImeData->pAutomata->Flush();
	char* cpStr = (char *)lpImeData->pAutomata->GetStringBuffer();
	atUIAPI_FromANSI( lpImeData->szText, (const atLPSTR)cpStr, strlen(cpStr));
}

static void _IME_hangul_OnTimeout( atUI_HIMEMETHOD hImeMethod )
{
	IME_HANGUL_TBL *lpImeData = (IME_HANGUL_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return;
}

static int _IME_hangul_GetState( atUI_HIMEMETHOD hImeMethod )
{
	IME_HANGUL_TBL *lpImeData = (IME_HANGUL_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return atUI_IMESTATE_NONE;

	return lpImeData->pAutomata->State()? atUI_IMESTATE_COMPOSING : atUI_IMESTATE_READY;
}

static int _IME_hangul_GetCursorPos( atUI_HIMEMETHOD hImeMethod )
{
	IME_HANGUL_TBL *lpImeData = (IME_HANGUL_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	return lpImeData->pAutomata->GetUnicodeBase_StringCount ();
}

static int _IME_hangul_SetCursorPos( atUI_HIMEMETHOD hImeMethod, int pos )
{
	IME_HANGUL_TBL *lpImeData = (IME_HANGUL_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	lpImeData->nCursorPos = pos;

	return lpImeData->nCursorPos;
}

static int _IME_hangul_OnStart( atUI_HIMEMETHOD hImeMethod, atLPTSTR szText, atLONG nMaxLen )
{
	IME_HANGUL_TBL *lpImeData;

	lpImeData = (IME_HANGUL_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	char cpTmp[1000];
	memset (cpTmp, 0x00, sizeof(cpTmp));

	atUIAPI_ToANSI( (atANSI_CHAR *)cpTmp, szText, nMaxLen );
	lpImeData->pAutomata->SetStringBuffer(cpTmp);

	lpImeData->szText = szText;
	//lpImeData->nMaxLen = nMaxLen;

	_IME_hangul_OnReset( hImeMethod );

	return 0;
}

static int _IME_hangul_OnStop( atUI_HIMEMETHOD hImeMethod )
{
	IME_HANGUL_TBL *lpImeData;

	lpImeData = (IME_HANGUL_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	_IME_hangul_OnFlush( hImeMethod );

//	lpImeData->state = atUI_IMESTATE_NONE;
	lpImeData->pAutomata->Reset();

	return 0;
}

static int _IME_hangul_OnInput( atUI_HIMEMETHOD hImeMethod, long key )
{
	IME_HANGUL_TBL *lpImeData;
	atTCHAR str[2] = { '\0', '\0' };
	atBOOL draw_flag = atFALSE;
	atCHAR *cpIngStr;

	lpImeData = (IME_HANGUL_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return atUI_IMESTATE_NONE;

	switch( key ){
/*
		// LEFT, RIGHT 구현해야 함.
		case atVKEY_LEFT:
			com_len = _IME_hangul_Flush( hImeMethod, atFALSE );
			if( com_len <= 0 ){
				if( lpImeData->nCursorPos > 0 ) 
					lpImeData->nCursorPos --;
			}
			break;
			
		case atVKEY_RIGHT:
			com_len = _IME_hangul_Flush( hImeMethod, atTRUE );
			if( com_len <= 0 ){
				if( lpImeData->nCursorPos < atUIAPI_strlen(lpImeData->szText) ) 
					lpImeData->nCursorPos ++;
			}
			break;
*/
		// 필요 한데 없어서 암거나 가져와서 함.
		case atVKEY_FF:
			lpImeData->pAutomata->KeyPush(0x20);
			cpIngStr = (char *)lpImeData->pAutomata->GetStringBuffer();
			atUIAPI_FromANSI( lpImeData->szText, (const atLPSTR)cpIngStr, strlen(cpIngStr));
			break;
		
		case atVKEY_CLEAR:
			lpImeData->pAutomata->KeyPush(0x08);
			cpIngStr = (char *)lpImeData->pAutomata->GetStringBuffer();
			atUIAPI_FromANSI( lpImeData->szText, (const atLPSTR)cpIngStr, strlen(cpIngStr));
			break;

		default:
			if( (key >= atVKEY_USERDEF_START) &&  (key <= atVKEY_USERDEF_END) ){
				key -= atVKEY_USERDEF;
				lpImeData->pAutomata->KeyPush(key);
				cpIngStr = (char *)lpImeData->pAutomata->GetStringBuffer();
				atUIAPI_FromANSI( lpImeData->szText, (const atLPSTR)cpIngStr, strlen(cpIngStr));
			}
			break;
	}

	return lpImeData->pAutomata->State()? atUI_IMESTATE_COMPOSING : atUI_IMESTATE_READY;
}

static int _IME_hangul_GetComposeText( atUI_HIMEMETHOD hImeMethod, atLPTSTR szComposeText )
{
	IME_HANGUL_TBL *lpImeData;

	lpImeData = (IME_HANGUL_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	if( lpImeData->pAutomata->State() ){
		atUSHORT nRealCode = lpImeData->pAutomata->GetConvertedUnionCode();
		char szTemp[3];
		szTemp[0] = nRealCode & 0x00ff; 
		szTemp[1] = nRealCode >> 8;
		szTemp[2] = '\0';
		atUIAPI_FromANSI( szComposeText, (const atLPSTR)szTemp, strlen(szTemp) );
	} else {
		szComposeText[0] = _ATC('\0');
	}

	return 0;
}

static int _IME_hangul_GetHilightLength( atUI_HIMEMETHOD hImeMethod )
{
	IME_HANGUL_TBL *lpImeData;

	lpImeData = (IME_HANGUL_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	return lpImeData->pAutomata->State()? 1 : 0;
}


atINT	defIMEMethod_han( atUI_HIMEMETHOD hImeMethod, atINT nMsg, atLONG nParam1, atLONG nParam2 )
{
	atUI_IMEMETHODCLASS *lpImeMethodClass;

	switch( nMsg ){
		case atUI_IMEMETHODMSG_GET_CLASSINFO :
			lpImeMethodClass = (atUI_IMEMETHODCLASS *)nParam1;
			atUIAPI_strncpy( lpImeMethodClass->szName, _AT("Hangul"), 32 );
			lpImeMethodClass->dwAttr = 0;
			lpImeMethodClass->dwDataSize = sizeof(IME_HANGUL_TBL);
			lpImeMethodClass->fnInputProc = defIMEMethod_han;
			lpImeMethodClass->lParam = nParam2;
			break;
		case atUI_IMEMETHODMSG_CREATE_CLASS :
			_IME_hangul_OnCreateClass( (atUI_IMEMETHODCLASS *)nParam1 );
			break;
		case atUI_IMEMETHODMSG_DESTROY_CLASS :
			_IME_hangul_OnDestroyClass();
			break;

		case atUI_IMEMETHODMSG_CREATE_INSTANCE : 
			_IME_hangul_OnCreateInstance( hImeMethod, (atLPTSTR)nParam1 );
			break;

		case atUI_IMEMETHODMSG_DESTORY_INSTANCE :
			_IME_hangul_OnDestroyInstance( hImeMethod );
			break;

		case atUI_IMEMETHODMSG_START :
			_IME_hangul_OnStart( hImeMethod, (atLPTSTR)nParam1, nParam2 );
			break;

		case atUI_IMEMETHODMSG_STOP :
			_IME_hangul_OnStop( hImeMethod );
			break;

		case atUI_IMEMETHODMSG_GET_STATE :
			return _IME_hangul_GetState( hImeMethod );

		case atUI_IMEMETHODMSG_INPUT : 
			_IME_hangul_OnInput( hImeMethod, nParam1 );
			break;

		case atUI_IMEMETHODMSG_RESET :
			_IME_hangul_OnReset( hImeMethod );
			break;

		case atUI_IMEMETHODMSG_FLUSH :
			_IME_hangul_OnFlush( hImeMethod );
			break;

		case atUI_IMEMETHODMSG_GET_CURSORPOS :
			return _IME_hangul_GetCursorPos( hImeMethod );
		
		case atUI_IMEMETHODMSG_SET_CURSORPOS :
			_IME_hangul_SetCursorPos( hImeMethod, nParam1 );
			break;

		case atUI_IMEMETHODMSG_GET_COMPOSETEXT :
			_IME_hangul_GetComposeText( hImeMethod, (atLPTSTR)nParam1 );
			break;	

		case atUI_IMEMETHODMSG_GET_HILIGHT_LENGTH :
			return _IME_hangul_GetHilightLength( hImeMethod );		
			
		case atUI_IMEMETHODMSG_TIMEOUT :
			_IME_hangul_OnTimeout( hImeMethod );		
			break;
	}

	return 0;
}

