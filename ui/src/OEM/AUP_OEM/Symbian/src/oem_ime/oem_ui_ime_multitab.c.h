#include "A_UIENG.h"
#include "OEM_UI_IME.h"



static atTCHAR* num_tbl[10] = {	_AT("0"), 
								_AT("1"),	_AT("2"),	_AT("3"),
								_AT("4"),	_AT("5"),	_AT("6"),
								_AT("7"), 	_AT("8"),	_AT("9") };
static atTCHAR* eng_lower_tbl[10] = {	_AT(" 0"), 
								_AT(".,-1"),	_AT("abc2"),	_AT("def3"),
								_AT("ghi4"),	_AT("jkl5"),	_AT("mno6"),
								_AT("pqrs7"), _AT("tuv8"),	_AT("wxyz9") };
static atTCHAR* eng_upper_tbl[10] = {	_AT(" 0"),
								_AT(".,-1"),	_AT("ABC2"),	_AT("DEF3"),
								_AT("GHI4"),	_AT("JKL5"),	_AT("MNO6"),
								_AT("PQRS7"), _AT("TUV8"), _AT("WXYZ9") };
#ifdef __USES_UNICODE_
static atBOOL is_uni_table = atFALSE;
static atTCHAR lower_tbl0[10][12] = {	_AT(" 0"), 
								_AT(".,-1"),	_AT("abc2-------"),	_AT("def3----"),
								_AT("ghi4------"),	_AT("jkl5"),	_AT("mno6-------"),
								_AT("pqrs7"), _AT("tuv8"),	_AT("wxyz9") };
static atTCHAR *lower_tbl[10];
static atTCHAR* upper_tbl[10] = {	_AT(" 0"),
								_AT(".,-1"),	_AT("ABC2"),	_AT("DEF3"),
								_AT("GHI4"),	_AT("JKL5"),	_AT("MNO6"),
								_AT("PQRS7"), _AT("TUV8"), _AT("WXYZ9") };
#else
static atTCHAR* lower_tbl[10] = {	_AT(" 0"), 
								_AT(".,-1"),	_AT("abc2\xe3\xe1"),	_AT("def3"),
								_AT("ghi4"),	_AT("jkl5"),	_AT("mno6"),
								_AT("pqrs7"), _AT("tuv8"),	_AT("wxyz9") };
static atTCHAR* upper_tbl[10] = {	_AT(" 0"),
								_AT(".,-1"),	_AT("ABC2"),	_AT("DEF3"),
								_AT("GHI4"),	_AT("JKL5"),	_AT("MNO6"),
								_AT("PQRS7"), _AT("TUV8"), _AT("WXYZ9") };
#endif

static atTCHAR* num_lower_tbl[10] = {	_AT("0 "), 
								_AT("1.,-"),	_AT("2abc"),	_AT("3def"),
								_AT("4ghi"),	_AT("5jkl"),	_AT("6mno"),
								_AT("7pqrs"), _AT("8tuv"),	_AT("9wxyz") };


//atTCHAR OEM_UI_IME_Buffer[256];
//atLPUI_IME_NOTIFY_FUNC fnImeMethodFunc;


typedef struct tagIME_MUTITAB_TBL {
	atTCHAR **automata_tbl;
	int state;
	int nCursorPos;
	int nCurKeyIdx;
	int nCurLoopIdx;
	atTCHAR szTempStr[32];
	atLPTSTR szText;
} IME_MULTITAB_TBL;


static atTCHAR ** _GetAutomata( atINT nInputMode )
{
	atTCHAR	**lpAutomata;
	
	switch( nInputMode ){
		case 0:
			lpAutomata = num_tbl;
			break;
		case 1:
			lpAutomata = lower_tbl;
			break;
		case 2:
			lpAutomata = upper_tbl;
			break;
		case 3:
			lpAutomata = num_lower_tbl;
			break;
		case 4:
			lpAutomata = lower_tbl;
			break;
		case 5:
			lpAutomata = upper_tbl;
			break;
		default :
			lpAutomata = num_tbl;
			break;
	}

	return lpAutomata;
}


static atINT _IME_multitab_OnCreateClass( atUI_IMEMETHODCLASS *lpImeMethodClass )
{
#ifdef __USES_UNICODE_
	int i;

	lower_tbl0[2][4] = 0x00E0;
	lower_tbl0[2][5] = 0x00E2;
	lower_tbl0[2][6] = 0x00E6;
	lower_tbl0[2][7] = 0x00E7;
	lower_tbl0[2][8] = 0x00E4;
	lower_tbl0[2][9] = 0x00E1;
	lower_tbl0[2][10] = 0x00E3;

	lower_tbl0[3][4] = 0x00E9;
	lower_tbl0[3][5] = 0x00E8;
	lower_tbl0[3][6] = 0x00EA;
	lower_tbl0[3][7] = 0x00EB;

	lower_tbl0[4][4] = 0x00EF;
	lower_tbl0[4][5] = 0x00EE;
	lower_tbl0[4][6] = 0x00EC;
	lower_tbl0[4][7] = 0x00ED;
	lower_tbl0[4][8] = 0x011F;
	lower_tbl0[4][9] = 0x0131;

/*
	lower_tbl0[6][4] = 0x00EF;
	lower_tbl0[6][5] = 0x00EE;
	lower_tbl0[6][6] = 0x00EC;
	lower_tbl0[6][7] = 0x00ED;
	lower_tbl0[6][8] = 0x011F;
	lower_tbl0[6][9] = 0x0131;
*/
	
	for( i = 0; i < 10; i++ ) lower_tbl[i] = (atTCHAR *)lower_tbl0[i];
#endif
	
	is_uni_table = atTRUE;

	lpImeMethodClass->dwAttr = 0;
	lpImeMethodClass->dwDataSize = sizeof(IME_MULTITAB_TBL);

	return 0;
}


static atINT _IME_multitab_OnDestroyClass(void)
{
	return 0;
}


static void _IME_multitab_OnCreateInstance( atUI_HIMEMETHOD hImeMethod, atLPTSTR szText )
{
	atUI_IMEMETHOD *lpImeMethod = (atUI_IMEMETHOD *)hImeMethod;
	IME_MULTITAB_TBL *lpImeData = (IME_MULTITAB_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return;

	lpImeData->automata_tbl = (atLPTSTR *)_GetAutomata( lpImeMethod->lParam );	
	lpImeData->nCurKeyIdx = -1;
	lpImeData->nCurLoopIdx = 0;
	lpImeData->nCursorPos = 0;
	lpImeData->szText = szText;
	lpImeData->state = atUI_IMESTATE_NONE;
	atUIAPI_strcpy(lpImeData->szTempStr, _AT(""));
}

static void _IME_multitab_OnDestroyInstance( atUI_HIMEMETHOD hImeMethod )
{
}


static void _IME_multitab_OnReset( atUI_HIMEMETHOD hImeMethod )
{
	IME_MULTITAB_TBL *lpImeData = (IME_MULTITAB_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return;
	
	lpImeData->nCurKeyIdx = -1;
	lpImeData->nCurLoopIdx = 0;
	atUIAPI_strcpy(lpImeData->szTempStr, _AT(""));
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

static int _IME_multitab_Flush( atUI_HIMEMETHOD hImeMethod, atBOOL bCursorMove )
{
	int len;
	IME_MULTITAB_TBL *lpImeData = (IME_MULTITAB_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	len = atUIAPI_strlen(lpImeData->szTempStr);
	if( len > 0 ){
		if( lpImeData->szText ){
			atUIAPI_strins( lpImeData->szText, lpImeData->szTempStr, lpImeData->nCursorPos, -1 );
		}
	}

	if( bCursorMove ) lpImeData->nCursorPos += len;

	_IME_multitab_OnReset( hImeMethod );

	return len;
}

static void _IME_multitab_OnTimeout( atUI_HIMEMETHOD hImeMethod )
{
	IME_MULTITAB_TBL *lpImeData = (IME_MULTITAB_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return;

	_IME_multitab_Flush( hImeMethod, atTRUE );
}

static int _IME_multitab_GetState( atUI_HIMEMETHOD hImeMethod )
{
	IME_MULTITAB_TBL *lpImeData = (IME_MULTITAB_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return atUI_IMESTATE_NONE;

	return lpImeData->state;
}

static int _IME_multitab_GetCursorPos( atUI_HIMEMETHOD hImeMethod )
{
	IME_MULTITAB_TBL *lpImeData = (IME_MULTITAB_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	return lpImeData->nCursorPos;
}

static int _IME_multitab_SetCursorPos( atUI_HIMEMETHOD hImeMethod, int pos )
{
	IME_MULTITAB_TBL *lpImeData = (IME_MULTITAB_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	lpImeData->nCursorPos = pos;

	return lpImeData->nCursorPos;
}

static int _IME_multitab_OnStart( atUI_HIMEMETHOD hImeMethod, atLPTSTR szText, atLONG nMaxLen )
{
	IME_MULTITAB_TBL *lpImeData;

	lpImeData = (IME_MULTITAB_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	lpImeData->szText = szText;
	//lpImeData->nMaxLen = nMaxLen;

	_IME_multitab_OnReset( hImeMethod );

	return 0;
}

static int _IME_multitab_OnStop( atUI_HIMEMETHOD hImeMethod )
{
	IME_MULTITAB_TBL *lpImeData;

	lpImeData = (IME_MULTITAB_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	lpImeData->state = atUI_IMESTATE_NONE;

	return 0;
}

static int _IME_multitab_OnInput( atUI_HIMEMETHOD hImeMethod, long key )
{
	IME_MULTITAB_TBL *lpImeData;
	int key_idx, cur_idx;
	atTCHAR str[2] = { '\0', '\0' };
	atBOOL draw_flag = atFALSE;
	int com_len;

	lpImeData = (IME_MULTITAB_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return atUI_IMESTATE_NONE;
	if( lpImeData->szText == atNULL ) return atUI_IMESTATE_NONE;	

	switch( key ){
		case atVKEY_0: case atVKEY_1: case atVKEY_2: case atVKEY_3: case atVKEY_4: 
		case atVKEY_5: case atVKEY_6: case atVKEY_7: case atVKEY_8: case atVKEY_9:
			key_idx = key - atVKEY_0;

			if( key_idx == lpImeData->nCurKeyIdx ){
				cur_idx = lpImeData->nCurLoopIdx + 1;
				if( lpImeData->automata_tbl[key_idx][cur_idx] == _ATC('\0') ) cur_idx = 0;
			} else {
//				com_len = atUIAPI_strlne( lpImeData->szTempStr );
//				atUIAPI_IME_Flush( hIme );
				_IME_multitab_Flush( hImeMethod, atTRUE );
				cur_idx = 0;
				lpImeData->nCurKeyIdx = key_idx;
			}
			str[0] = lpImeData->automata_tbl[key_idx][cur_idx];
			atUIAPI_strcpy( lpImeData->szTempStr, str );
			lpImeData->nCurLoopIdx = cur_idx;
			// 멀티탭 갯수가 한개뿐인 경우엔, 바로 다음으로 이동.
			if( lpImeData->automata_tbl[key_idx][1] == _ATC('\0') ){
//				com_len = atUIAPI_strlne( str );
				_IME_multitab_Flush( hImeMethod, atTRUE );
//				lpImeData->nCursorPos += com_len;
				lpImeData->nCurKeyIdx = key_idx;
			}
			lpImeData->state = atUI_IMESTATE_COMPOSING;
			draw_flag = atTRUE;
			break;

		case atVKEY_LEFT:
			com_len = _IME_multitab_Flush( hImeMethod, atFALSE );
			if( com_len <= 0 ){
				if( lpImeData->nCursorPos > 0 ) 
					lpImeData->nCursorPos --;
			}
			break;
			
		case atVKEY_RIGHT:
			com_len = _IME_multitab_Flush( hImeMethod, atTRUE );
			if( com_len <= 0 ){
				if( lpImeData->nCursorPos < atUIAPI_strlen(lpImeData->szText) ) 
					lpImeData->nCursorPos ++;
			}
			break;
			
		case atVKEY_CLEAR:
			if( atUIAPI_strlen(lpImeData->szTempStr) > 0 ){
				atUIAPI_strcpy(lpImeData->szTempStr, _AT(""));
				draw_flag = atTRUE;
			} else if( atUIAPI_strlen(lpImeData->szText) > 0 ){
				if( lpImeData->nCursorPos > 0 ){
					lpImeData->nCursorPos--;
					atUIAPI_strdel( lpImeData->szText, lpImeData->nCursorPos, 1 );
					draw_flag = atTRUE;
				}
			}
			lpImeData->nCurLoopIdx = 0;
			lpImeData->state = atUI_IMESTATE_READY;
			break;

		default:
			break;
	}

	return lpImeData->state;
}

static int _IME_multitab_GetComposeText( atUI_HIMEMETHOD hImeMethod, atLPTSTR szComposeText )
{
	IME_MULTITAB_TBL *lpImeData;

	lpImeData = (IME_MULTITAB_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	atUIAPI_strncpy( szComposeText, lpImeData->szTempStr, 32 );

	return 0;
}

static int _IME_multitab_GetHilightLength( atUI_HIMEMETHOD hImeMethod )
{
	IME_MULTITAB_TBL *lpImeData;

	lpImeData = (IME_MULTITAB_TBL *)atUIAPI_IME_GetIMEMethodData(hImeMethod);
	if( lpImeData == atNULL ) return -1;

	return atUIAPI_strlen( lpImeData->szTempStr );
}


atINT	defIMEMethod( atUI_HIMEMETHOD hImeMethod, atINT nMsg, atLONG nParam1, atLONG nParam2 )
{
	atUI_IMEMETHODCLASS *lpImeMethodClass;

	switch( nMsg ){
		case atUI_IMEMETHODMSG_GET_CLASSINFO :
			lpImeMethodClass = (atUI_IMEMETHODCLASS *)nParam1;
			if( nParam2 == 0 ){
				atUIAPI_strncpy( lpImeMethodClass->szName, _AT("NUM"), 32 );
			} else if( nParam2 == 1 ){
				atUIAPI_strncpy( lpImeMethodClass->szName, _AT("eng"), 32 );
			} else if( nParam2 == 2 ){
				atUIAPI_strncpy( lpImeMethodClass->szName, _AT("ENG"), 32 );
			} else if( nParam2 == 3 ){
				atUIAPI_strncpy( lpImeMethodClass->szName, _AT("123abc"), 32 );
			} else if( nParam2 == 4 ){
				atUIAPI_strncpy( lpImeMethodClass->szName, _AT("abc"), 32 );
			} else if( nParam2 == 5 ){
				atUIAPI_strncpy( lpImeMethodClass->szName, _AT("ABC"), 32 );
			}
			lpImeMethodClass->dwAttr = 0;
			lpImeMethodClass->dwDataSize = sizeof(IME_MULTITAB_TBL);
			lpImeMethodClass->fnInputProc = defIMEMethod;
			lpImeMethodClass->lParam = nParam2;
			break;
		case atUI_IMEMETHODMSG_CREATE_CLASS :
			_IME_multitab_OnCreateClass( (atUI_IMEMETHODCLASS *)nParam1 );
			break;
		case atUI_IMEMETHODMSG_DESTROY_CLASS :
			_IME_multitab_OnDestroyClass();
			break;

		case atUI_IMEMETHODMSG_CREATE_INSTANCE : 
			_IME_multitab_OnCreateInstance( hImeMethod, (atLPTSTR)nParam1 );
			break;

		case atUI_IMEMETHODMSG_DESTORY_INSTANCE :
			_IME_multitab_OnDestroyInstance( hImeMethod );
			break;

		case atUI_IMEMETHODMSG_START :
			_IME_multitab_OnStart( hImeMethod, (atLPTSTR)nParam1, nParam2 );
			break;

		case atUI_IMEMETHODMSG_STOP :
			_IME_multitab_OnStop( hImeMethod );
			break;

		case atUI_IMEMETHODMSG_GET_STATE :
			return _IME_multitab_GetState( hImeMethod );

		case atUI_IMEMETHODMSG_INPUT : 
			_IME_multitab_OnInput( hImeMethod, nParam1 );
			break;

		case atUI_IMEMETHODMSG_RESET :
			_IME_multitab_OnReset( hImeMethod );
			break;

		case atUI_IMEMETHODMSG_GET_CURSORPOS :
			return _IME_multitab_GetCursorPos( hImeMethod );
		
		case atUI_IMEMETHODMSG_SET_CURSORPOS :
			_IME_multitab_SetCursorPos( hImeMethod, nParam1 );
			break;

		case atUI_IMEMETHODMSG_GET_COMPOSETEXT :
			_IME_multitab_GetComposeText( hImeMethod, (atLPTSTR)nParam1 );
			break;	

		case atUI_IMEMETHODMSG_GET_HILIGHT_LENGTH :
			return _IME_multitab_GetHilightLength( hImeMethod );		
			
		case atUI_IMEMETHODMSG_TIMEOUT :
			_IME_multitab_OnTimeout( hImeMethod );		
			break;
	}

	return 0;
}

