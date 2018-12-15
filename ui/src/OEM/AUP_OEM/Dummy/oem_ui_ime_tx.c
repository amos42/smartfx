#include "A_UIENG.h"
#include "OEM_UI_IME.h"


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
	return 0;
}


int OEM_UI_GetIMEMethodClassStock( atINT nIndex, atUI_IMEMETHODCLASS *lpImeClass )
{
	return 0;
}

