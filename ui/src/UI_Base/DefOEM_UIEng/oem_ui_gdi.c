 //
// Canvas_wipi.c
//
////////////////////////////////////////////////////////////////

#include "OEM_UI_GDI.h"
#include "OEM_UI_Font.h"
#include "UI_API/UI_API.h"
#include "GrpX.h"
#include "SYSAPI.h"


atLPGRPXMNG g_lpGrpX; // Font¿¡¼­µµ ¾¸
static int x_dpi, y_dpi;

#include "oem_ui_gdi_grpx.inl"


atBOOL OEM_UI_InitGraphicSystem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	atGRPXFUNCS funcs;
	int *vv = (int *)lInitData1;
	int vv2[6];
	SYS_DISPLAY_CAPS caps;

	if( vv == atNULL ) return atFALSE;

	vv2[0] = vv[0];
	vv2[1] = vv[1];
	vv2[2] = vv[2];
	vv2[3] = vv[3];
	vv2[4] = vv[4];
	vv2[5] = vv[5];
	SYSAPI_InitDisplay( lSysInst, (long)vv2, lInitData2 );
	SYSAPI_GetDisplayCaps( lSysInst, (long)vv2, lInitData2, &caps );
	x_dpi = caps.nX_DPI;
	y_dpi = caps.nY_DPI;
		
	funcs.lpData = atNULL;
	funcs.fnAllocMem = atUIAPI_AllocMem;
	funcs.fnFreeMem = atUIAPI_FreeMem; 
   	g_lpGrpX = atGRPX_Initialize( SYSAPI_GetScreenWidth(), SYSAPI_GetScreenHeight(), 
								SYSAPI_GetScreenBPP(), SYSAPI_GetFrameBufferPtr(), SYSAPI_GetFrameVMemWidth(), &funcs );

	return atTRUE;
}

void OEM_UI_CloseGraphicSystem(void)
{
	atGRPX_Finalize(g_lpGrpX);
	g_lpGrpX = atNULL;

	SYSAPI_FinalDisplay();
}

atBOOL OEM_UI_SetScreenResolution( atINT nWidth, atINT nHeight )
{
	if( !SYSAPI_SetDisplayResolution( nWidth, nHeight, (nWidth + 0x3) & ~0x3 ) ) return atFALSE;
	
	atGRPX_SetFrameBuffer( g_lpGrpX, SYSAPI_GetFrameBufferPtr() );
	atGRPX_SetScreenResolution( g_lpGrpX, SYSAPI_GetScreenWidth(), SYSAPI_GetScreenHeight(), SYSAPI_GetFrameVMemWidth() );

	return atTRUE;
}

atBOOL OEM_UI_GetScreenDPI( atINT *lpnX_DPI, atINT *lpnY_DPI )
{
	if( lpnX_DPI ) *lpnX_DPI = x_dpi;
	if( lpnY_DPI ) *lpnY_DPI = y_dpi;

	return atTRUE;
}

atINT OEM_UI_GetScreenWidth( NOARGS )
{
	return SYSAPI_GetScreenWidth();
}

atINT OEM_UI_GetScreenHeight( NOARGS )
{
	return SYSAPI_GetScreenHeight();
}

atINT OEM_UI_GetScreenBPP( NOARGS )
{
	return SYSAPI_GetScreenBPP();
}

atVOID * OEM_UI_GetScreenBufferPtr( NOARGS )
{
	return SYSAPI_GetFrameBufferPtr();
}

atBOOL OEM_UI_BeginDraw( NOARGS )
{
	return SYSAPI_BeginDraw();
}

atVOID OEM_UI_EndDraw( NOARGS )
{
	SYSAPI_EndDraw();
}

atVOID OEM_UI_FlushScreenRgn( atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	SYSAPI_FlushScreenRgn( nX, nY, nWidth, nHeight );
}

atLONG 		OEM_UI_ExtGraphicMethod( atBOOL CanvasActivate, OEM_GDC gdc, atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
    switch( func_no ){
        case 1 : atGRPX_DrawLineAA( (atGRPX_CANVAS *)gdc, param1, param2, param3, param4, param5 );
                 break;
    }
            
            
    return 0;
}

