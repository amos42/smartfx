 //
// Canvas_wipi.c
//
////////////////////////////////////////////////////////////////

#include "OEM_SA_Scr.h"
#include "OEM_SA_Mem.h"
#include "SYSAPI.h"


atDWORD OEM_SYSAPI_GetDisplayCaps( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2, SYS_DISPLAY_CAPS *lpScrCaps )
{
	return 0;
}

atBOOL OEM_SYSAPI_InitDisplay( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return atFALSE;
}

void OEM_SYSAPI_FinalDisplay(void)
{
}

atBOOL OEM_SYSAPI_SetDisplayResolution( atINT nWidth, atINT nHeight, atINT nFrameWidth )
{
	return atTRUE;
}

atINT OEM_SYSAPI_GetScreenWidth( NOARGS )
{
	return 0;

}

atINT OEM_SYSAPI_GetScreenHeight( NOARGS )
{
	return 0;
}

atINT OEM_SYSAPI_GetScreenBPP( NOARGS )
{
	return 0;
}

atVOID * OEM_SYSAPI_GetFrameBufferPtr( NOARGS )
{
	return atNULL;
}

atINT OEM_SYSAPI_GetFrameVMemWidth( NOARGS )
{
	return 0;
}

atVOID OEM_SYSAPI_FlushScreenRgn( atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
}

atULONG OEM_SYSAPI_MakeColor(atBYTE r, atBYTE g, atBYTE b)
{
	return 0;
}

atBYTE OEM_SYSAPI_GetRValue(atULONG nColor)
{
	return 0;
}

atBYTE OEM_SYSAPI_GetGValue(atULONG nColor)
{
	return 0;
}

atBYTE OEM_SYSAPI_GetBValue(atULONG nColor)
{
	return 0;
}


atLONG OEM_SYSAPI_ExtGraphicMethod( atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
    switch( func_no ){
        case 0x1000 :
        		     break;
    }

    return 0;
}

