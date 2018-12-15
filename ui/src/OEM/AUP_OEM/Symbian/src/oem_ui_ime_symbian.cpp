#include "A_UIENG.h"
#include "OEM_UI_IME.h"



#ifdef __cplusplus
extern "C" {
#endif

atTCHAR OEM_UI_IME_Buffer[256];
atLPUI_IME_NOTIFY_FUNC fnImeMethodFunc;
void OEM_UI_StartIME(void);

#ifdef __cplusplus
}
#endif


#include "oem_ime/oem_ui_ime_multitab.c.h"
#include "oem_ime/oem_ui_ime_pti.cpp.h"

atBOOL OEM_UI_InitIME( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( (void *)lSysInst == atNULL ) return atFALSE;
	
	return atTRUE;
}


void OEM_UI_FinalIME(void)
{
}


int OEM_UI_GetIMEMethodClassStockCount()
{
	return 6;
}

int OEM_UI_GetIMEMethodClassStock( atINT nIndex, atUI_IMEMETHODCLASS *lpImeClass )
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
//			defIMEMethod_PTI(atNULL, atUI_IMEMETHODMSG_GET_CLASSINFO, (long)lpImeClass, 1 );
			defIMEMethod(atNULL, atUI_IMEMETHODMSG_GET_CLASSINFO, (long)lpImeClass, 4 );
			break;
		case  5 : 
//			defIMEMethod_PTI(atNULL, atUI_IMEMETHODMSG_GET_CLASSINFO, (long)lpImeClass, 2 );
			defIMEMethod(atNULL, atUI_IMEMETHODMSG_GET_CLASSINFO, (long)lpImeClass, 5 );
			break;
	}

	return 0;
}

