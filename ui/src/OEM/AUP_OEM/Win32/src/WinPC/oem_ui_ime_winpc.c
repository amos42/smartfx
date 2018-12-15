#include "A_UIENG.h"
#include "OEM_UI_IME.h"


#include "oem_ime/oem_ui_ime_multitab.c.h"
//#include "oem_ime/oem_ui_ime_han.c.h"


static atTCHAR OEM_UI_IME_Buffer[256];
static atLPUI_IME_NOTIFY_FUNC fnImeMethodFunc;


static atBOOL OEM_UI_InitIME( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( (void *)lSysInst == atNULL ) return atFALSE;

	return atTRUE;
}


static void OEM_UI_FinalIME(void)
{
}


static int OEM_UI_GetIMEMethodClassStockCount(void)
{
	return 6;
//	return 7;
}

static int OEM_UI_GetIMEMethodClassStock( atINT nIndex, atUI_IMEMETHODCLASS *lpImeClass )
{
	switch( nIndex ){
		case  0 : 
			defIMEMethod(atNULL, atUI_IMEMETHODMSG_GET_CLASSINFO, (long)lpImeClass, 0 );
			break;
		case  1 : 
			defIMEMethod(atNULL, atUI_IMEMETHODMSG_GET_CLASSINFO, (long)lpImeClass, 1 );
			break;
		case  2 : 
			defIMEMethod(atNULL, atUI_IMEMETHODMSG_GET_CLASSINFO, (long)lpImeClass, 2 );
			break;
		case  3 : 
			defIMEMethod(atNULL, atUI_IMEMETHODMSG_GET_CLASSINFO, (long)lpImeClass, 3 );
			break;
		case  4 : 
			defIMEMethod(atNULL, atUI_IMEMETHODMSG_GET_CLASSINFO, (long)lpImeClass, 4 );
			break;
		case  5 : 
			defIMEMethod(atNULL, atUI_IMEMETHODMSG_GET_CLASSINFO, (long)lpImeClass, 5 );
			break;
//		case  6 : 
//			defIMEMethod_han(atNULL, atUI_IMEMETHODMSG_GET_CLASSINFO, (long)lpImeClass, 0 );
//			break;
	}

	return 0;
}



void init_uiapi_ime_oem() 
{
	OEM_UI_IME_T oem_funcs = {
		OEM_UI_InitIME,
		OEM_UI_FinalIME,

		OEM_UI_GetIMEMethodClassStockCount,
		OEM_UI_GetIMEMethodClassStock,

		atNULL,  // OME_UI_IME_CreateIME,
		atNULL,  // OME_UI_IME_DestroyIME,

		atNULL,  // OEM_UI_IME_Start,
		atNULL,  // OEM_UI_IME_Input,
		atNULL,  // OEM_UI_IME_GetComplete,
		atNULL,  // OEM_UI_IME_GetComposite,
		atNULL,  // OEM_UI_IME_Flush,
		atNULL  // OEM_UI_IME_Stop	
	};

	OEM_UI_IME_SetOemFn( &oem_funcs );
}
