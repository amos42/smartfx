#include "A_UIENG.h"
#include "OEM_UI_IME.h"
#include <e32keys.h>
#include <e32cons.h>
#include "ptiengine.h"
//#include "pticompositiondataif.h"


typedef struct tagIME_pti_TBL {
	CPtiEngine* eng;
	atBOOL pred;
	int state;
	int nCursorPos;
	atTCHAR szTempStr[64];
	atLPTSTR szText;
} IME_pti_TBL;


static atINT _IME_pti_OnCreateClass( atUI_IMEMETHODCLASS *lpImeMethodClass )
{
	lpImeMethodClass->dwAttr = 0;
	lpImeMethodClass->dwDataSize = sizeof(IME_pti_TBL);

	return 0;
}


static atINT _IME_pti_OnDestroyClass(void)
{
	return 0;
}


static void _IME_pti_OnCreateInstance( atUI_HIMEMETHOD hImeMethod, atLPTSTR szText )
{
	atUI_IMEMETHOD *lpImeMethod = (atUI_IMEMETHOD *)hImeMethod;
	IME_pti_TBL *lpImeData = (IME_pti_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return;

	// Create an engine instance
	lpImeData->eng = CPtiEngine::NewL( ETrue );
	//CleanupStack::PushL( lpImeData->eng );
	
	TInt err;
	// Activate a language in default input mode
	// Activate a language, in specified input mode
	if( lpImeMethod->lParam == 1 ){
//		err = lpImeData->eng->ActivateLanguageL( ELangKorean, EPtiEngineMultitapping );
		err = lpImeData->eng->ActivateLanguageL( ELangEnglish, EPtiEnginePredictive );
//		err = lpImeData->eng->ActivateLanguageL( ELangEnglish, EPtiEngineMultitapping );
		lpImeData->pred = atTRUE;
	} else if( lpImeMethod->lParam == 2 ){
		err = lpImeData->eng->ActivateLanguageL( ELangEnglish, EPtiEngineMultitapping );
		lpImeData->pred = atFALSE;
	}

	lpImeData->nCursorPos = 0;
	lpImeData->szText = szText;
	lpImeData->state = atUI_IMESTATE_NONE;
	atUIAPI_strcpy(lpImeData->szTempStr, _AT(""));
}

static void _IME_pti_OnDestroyInstance( atUI_HIMEMETHOD hImeMethod )
{
	IME_pti_TBL *lpImeData = (IME_pti_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return;

	lpImeData->eng->CloseCurrentLanguageL();	
	delete lpImeData->eng;
}


static void _IME_pti_OnReset( atUI_HIMEMETHOD hImeMethod )
{
	IME_pti_TBL *lpImeData = (IME_pti_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return;
	
	atUIAPI_strcpy(lpImeData->szTempStr, _AT(""));
	lpImeData->eng->ClearCurrentWord();
	lpImeData->state = atUI_IMESTATE_READY;

	/*
	len = atUIAPI_strlen(lpImeData->szTempText);
	if( len > 0 ){
		atUIAPI_strins( lpImeData->szText, lpImeData->szTempText, lpImeData->nCursorPos, -1 );
		atUIAPI_strcpy(lpImeData->szTempText, _AT(""));
	}
	*/

//	return len;
}

static int _IME_pti_Flush( atUI_HIMEMETHOD hImeMethod, atBOOL bCursorMove )
{
	int len;
	IME_pti_TBL *lpImeData = (IME_pti_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	if( lpImeData->state != atUI_IMESTATE_COMPOSING ) return 0;
	
	len = atUIAPI_strlen(lpImeData->szTempStr);
	if( len > 0 ){
		if( lpImeData->szText ){
			atUIAPI_strins( lpImeData->szText, lpImeData->szTempStr, lpImeData->nCursorPos, -1 );
		}
	}

	if( bCursorMove ) lpImeData->nCursorPos += len;

	_IME_pti_OnReset( hImeMethod );

	return len;
}

static void _IME_pti_OnTimeout( atUI_HIMEMETHOD hImeMethod )
{
	IME_MULTITAB_TBL *lpImeData = (IME_MULTITAB_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return;

	_IME_pti_Flush( hImeMethod, atTRUE );
}

static int _IME_pti_GetState( atUI_HIMEMETHOD hImeMethod )
{
	IME_pti_TBL *lpImeData = (IME_pti_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return atUI_IMESTATE_NONE;

	return lpImeData->state;
}

static int _IME_pti_GetCursorPos( atUI_HIMEMETHOD hImeMethod )
{
	IME_pti_TBL *lpImeData = (IME_pti_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	return lpImeData->nCursorPos;
}

static int _IME_pti_SetCursorPos( atUI_HIMEMETHOD hImeMethod, int pos )
{
	IME_pti_TBL *lpImeData = (IME_pti_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	lpImeData->nCursorPos = pos;

	return lpImeData->nCursorPos;
}

static int _IME_pti_OnStart( atUI_HIMEMETHOD hImeMethod, atLPTSTR szText, atLONG nMaxLen )
{
	IME_pti_TBL *lpImeData;

	lpImeData = (IME_pti_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	lpImeData->szText = szText;
	//lpImeData->nMaxLen = nMaxLen;

	_IME_pti_OnReset( hImeMethod );

	return 0;
}

static int _IME_pti_OnStop( atUI_HIMEMETHOD hImeMethod )
{
	IME_pti_TBL *lpImeData;

	lpImeData = (IME_pti_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	lpImeData->state = atUI_IMESTATE_NONE;

	return 0;
}

static int _IME_pti_OnInput( atUI_HIMEMETHOD hImeMethod, long key )
{
	IME_pti_TBL *lpImeData;
	int key_idx;
	atBOOL draw_flag = atFALSE;

	lpImeData = (IME_pti_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return atUI_IMESTATE_NONE;

	switch( key ){
		case atVKEY_0: 	
			if( lpImeData->pred ){
				_IME_pti_Flush( lpImeData, atTRUE );
				draw_flag = atTRUE;
				break;
			}
		case atVKEY_1: case atVKEY_2: case atVKEY_3: case atVKEY_4: case atVKEY_5: 
		case atVKEY_6: case atVKEY_7: case atVKEY_8: case atVKEY_9:
			key_idx = key - atVKEY_0 + (int)EPtiKey0;
			lpImeData->eng->AppendKeyPress( (TPtiKey)key_idx );
			lpImeData->state = atUI_IMESTATE_COMPOSING;
			draw_flag = atTRUE;
			break;
			
		case atVKEY_STAR:
			if( lpImeData->pred ){
				lpImeData->eng->NextCandidate();
				draw_flag = atTRUE;
			}
			break;
			
		case atVKEY_CLEAR:
			lpImeData->eng->DeleteKeyPress();
			draw_flag = atTRUE;
			break;

		case atVKEY_LEFT:
/*			
			//lpImeData->eng->HandleCommandL( EPtiCommandUserActionLeft );
			if( lpImeData->state == atUI_IMESTATE_READY ){
				if( lpImeData->nCursorPos > 0 ) lpImeData->nCursorPos--;
				ch = pImeData->szText[lpImeData->nCursorPos];
				if( ((ch >= _ATC('a')) && (ch <= _ATC('z')) || ((ch >= _ATC('A')) && (ch <= _ATC('Z')) ){
				}
			} else {
				lpImeData->szText[lpImeData->nCursorPos]
			}
*/			
			break;

		case atVKEY_RIGHT:
			//lpImeData->eng->HandleCommandL( EPtiCommandUserActionRight );
			break;

	}
	if( !draw_flag ) return atUI_IMESTATE_NONE;
	
//			atUIAPI_strncpy( lpImeData->szTempStr, (atLPTSTR)lpImeData->eng->CurrentWord().Ptr(), 32 ); 
	TBuf<62> temp;
	temp.Copy( lpImeData->eng->CurrentWord() );

	atUIAPI_strncpy( lpImeData->szTempStr, (atLPTSTR)temp.Ptr(), temp.Length() );
	lpImeData->szTempStr[temp.Length()] = _ATC('\0');
	
//			atUIAPI_strncpy( lpImeData->szTempStr, (atLPTSTR)temp.Ptr(), temp.Length() );
//			lpImeData->szTempStr[temp.Length()] = _ATC('\0');
//			lpImeData->eng->CommitCurrentWord();

/*			
MPtiEngineCompositionDataInterface *cc = lpImeData->eng->CompositionData();	
if( cc ){
RDebug::Printf( ">> NumberOfPhrase = %d\n", 	cc->NumberOfPhrase() );
} else {
RDebug::Printf( ">> !! CompositionData is NULL\n" );
}
*/

//RDebug::Printf( "NumberOfPhrase = %d\n", lpImeData->eng->CompositionData() >NumberOfPhrase() );			
	
//			lpImeData->state = atUI_IMESTATE_COMPOSING;
	draw_flag = atTRUE;

	return lpImeData->state;
}

static int _IME_pti_GetComposeText( atUI_HIMEMETHOD hImeMethod, atLPTSTR szComposeText )
{
	IME_pti_TBL *lpImeData;

	lpImeData = (IME_pti_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	atUIAPI_strncpy( szComposeText, lpImeData->szTempStr, 64 );

	return 0;
}

static int _IME_pti_GetHilightLength( atUI_HIMEMETHOD hImeMethod )
{
	IME_pti_TBL *lpImeData;

	lpImeData = (IME_pti_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	if( !lpImeData->pred ) return 0;
	
	return atUIAPI_strlen(lpImeData->szTempStr);
}


atINT	defIMEMethod_PTI( atUI_HIMEMETHOD hImeMethod, atINT nMsg, atLONG nParam1, atLONG nParam2 )
{
	atUI_IMEMETHODCLASS *lpImeMethodClass;

	switch( nMsg ){
		case atUI_IMEMETHODMSG_GET_CLASSINFO :
			lpImeMethodClass = (atUI_IMEMETHODCLASS *)nParam1;
			if( nParam2 == 1 ){
				atUIAPI_strncpy( lpImeMethodClass->szName, _AT("abc"), 32 );
			} else if( nParam2 == 2 ){
				atUIAPI_strncpy( lpImeMethodClass->szName, _AT("ABC"), 32 );
			}
			lpImeMethodClass->dwAttr = 0;
			lpImeMethodClass->dwDataSize = sizeof(IME_pti_TBL);
			lpImeMethodClass->fnInputProc = defIMEMethod_PTI;
			lpImeMethodClass->lParam = nParam2;
			break;
		case atUI_IMEMETHODMSG_CREATE_CLASS :
			_IME_pti_OnCreateClass( (atUI_IMEMETHODCLASS *)nParam1 );
			break;
		case atUI_IMEMETHODMSG_DESTROY_CLASS :
			_IME_pti_OnDestroyClass();
			break;

		case atUI_IMEMETHODMSG_CREATE_INSTANCE : 
			_IME_pti_OnCreateInstance( hImeMethod, (atLPTSTR)nParam1 );
			break;

		case atUI_IMEMETHODMSG_DESTORY_INSTANCE :
			_IME_pti_OnDestroyInstance( hImeMethod );
			break;

		case atUI_IMEMETHODMSG_START :
			_IME_pti_OnStart( hImeMethod, (atLPTSTR)nParam1, nParam2 );
			break;

		case atUI_IMEMETHODMSG_STOP :
			_IME_pti_OnStop( hImeMethod );
			break;

		case atUI_IMEMETHODMSG_GET_STATE :
			return _IME_pti_GetState( hImeMethod );

		case atUI_IMEMETHODMSG_INPUT : 
			_IME_pti_OnInput( hImeMethod, nParam1 );
			break;

		case atUI_IMEMETHODMSG_RESET :
			_IME_pti_OnReset( hImeMethod );
			break;

		case atUI_IMEMETHODMSG_GET_CURSORPOS :
			return _IME_pti_GetCursorPos( hImeMethod );
		
		case atUI_IMEMETHODMSG_SET_CURSORPOS :
			_IME_pti_SetCursorPos( hImeMethod, nParam1 );
			break;

		case atUI_IMEMETHODMSG_GET_COMPOSETEXT :
			_IME_pti_GetComposeText( hImeMethod, (atLPTSTR)nParam1 );
			break;	

		case atUI_IMEMETHODMSG_GET_HILIGHT_LENGTH :
			return _IME_pti_GetHilightLength( hImeMethod );		
			
		case atUI_IMEMETHODMSG_TIMEOUT :
			_IME_pti_OnTimeout( hImeMethod );		
			break;		
	}

	return 0;
}

