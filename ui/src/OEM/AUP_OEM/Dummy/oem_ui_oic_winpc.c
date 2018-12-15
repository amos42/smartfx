#include "A_UIENG.h"
#include "OEM_UI_OIC.h"
#include "SYSAPI.h"


atBOOL OEM_UI_InitOIC( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atFALSE;
}


void OEM_UI_FinalOIC(void)
{
}


atBOOL OEM_UI_IsSupportedOIC()
{
	return atFALSE;
}

OEM_UI_OIC OEM_UI_CreateOIC( int x, int y, int width, int height, atUI_FUNC_OIC_NOTIFY *fnFunc, void *param )
{
  return atNULL;
}

void OEM_UI_DestroyOIC( OEM_UI_OIC hOemOIC )
{
}

void OEM_UI_SetOICFont( OEM_UI_OIC hOemOIC, int font_size )
{
}

void OEM_UI_GetOICPos( OEM_UI_OIC hOemOIC, int *x, int *y )
{
}

void OEM_UI_GetOICSize( OEM_UI_OIC hOemOIC, int *width, int *height )
{
}

void OEM_UI_SetOICSize( OEM_UI_OIC hOemOIC, int width, int height )
{
}

void OEM_UI_MoveOIC( OEM_UI_OIC hOemOIC, int x, int y )
{
}

void OEM_UI_ShowAndSetFocusOIC( OEM_UI_OIC hOemOIC )
{
}

void OEM_UI_HideAndLostFocusOIC( OEM_UI_OIC hOemOIC )
{
}

void OEM_UI_RefreshOIC( OEM_UI_OIC hOemOIC )
{
}

void OEM_UI_SetOICText( OEM_UI_OIC hOemOIC, atLPCTSTR szText, atINT nLen )
{
}

void OEM_UI_GetOICText( OEM_UI_OIC hOemOIC, atLPTSTR szText, atINT nLen )
{
}

