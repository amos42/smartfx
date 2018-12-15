
#include "OEM_UI_GDI.h"
#include "OEM_UI_Mem.h"
#include "GrpX.h"
#include "SYSAPI.h"


atLPGRPXMNG g_lpGrpX; // Font¿¡¼­µµ ¾¸

#include "oem_ui_gdi_grpx.c.h"


atBOOL OEM_UI_InitGraphicSystem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	atGRPXFUNCS funcs;
	int *vv = (int *)lInitData1;
	int vv2[6];

	if( vv == atNULL ) return atFALSE;

	vv2[0] = vv[0];
	vv2[1] = vv[1];
	vv2[2] = vv[2];
	vv2[3] = vv[3];
	vv2[4] = vv[4];
	vv2[5] = vv[5];
	SYSAPI_InitDisplay( lSysInst, (long)vv2, lInitData2 );
		
	funcs.lpData = atNULL;
	funcs.fnAllocMem = atUIAPI_AllocMem;
	funcs.fnFreeMem = (atLPGRPX_FUNC_FREE)atUIAPI_FreeMem; 
   	g_lpGrpX = atGRPX_Initialize( SYSAPI_GetScreenWidth(), SYSAPI_GetScreenHeight(), 
								SYSAPI_GetScreenBPP(), SYSAPI_GetFrameBufferPtr(), SYSAPI_GetFrameVMemWidth(), &funcs );

	return atTRUE;
}


atVOID OEM_UI_CloseGraphicSystem( NOARGS )
{
	atGRPX_Finalize(g_lpGrpX);
	g_lpGrpX = atNULL;

	SYSAPI_FinalDisplay();
}

atBOOL OEM_UI_SetScreenResolution( atINT nWidth, atINT nHeight )
{
	SYSAPI_SetDisplayResolution( nWidth, nHeight, (nWidth + 0x3) & ~0x3 );
	
	atGRPX_SetFrameBuffer( g_lpGrpX, SYSAPI_GetFrameBufferPtr() );
	atGRPX_SetScreenResolution( g_lpGrpX, SYSAPI_GetScreenWidth(), SYSAPI_GetScreenHeight(), SYSAPI_GetFrameVMemWidth() );

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

atVOID OEM_UI_FlushScreenRgn( atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	SYSAPI_FlushScreenRgn( nX, nY, nWidth, nHeight );
}

atLONG 		OEM_UI_ExtGraphicMethod( atBOOL CanvasActivate, OEM_GDC gdc, atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
    switch( func_no ){
        case 1 : atGRPX_DrawLineAA( (atGRPX_CANVAS *)gdc, param1, param2, param3, param4, param5 );
                 break;
        case 2 : SYSAPI_ExtGraphicMethod( 0x1100, 0, 0, 0, 0, 0, 0 );
        		 break;
        case 100 : SYSAPI_ExtGraphicMethod( 0x1000, param1, param2, param3, param4, param5, 0 );
        		 break;
    }
            
    return 0;
}

