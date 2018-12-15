 //
// Canvas_wipi.c
//
////////////////////////////////////////////////////////////////

#include "OEM_UI_GDI.h"
#include "OEM_UI_Font.h"
#include "UI_API/UI_API.h"
#include "GrpX.h"
#include "SYSAPI.h"

// for test
#include <stdio.h>

#ifdef _SDIC_VIEWMAP_
#include "I43_Api.h"

#ifndef PUBLIC
#define PUBLIC
#endif

#include "Gps.h"
#include "CtrlLeds.h"
#endif


atLPGRPXMNG g_lpGrpX = atNULL; // Font¿¡¼­µµ ¾¸

//#include "oem_ui_gdi_grpx.c"


atBOOL OEM_UI_InitGraphicSystem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	SYSAPI_DebugPrintf(_AT("OEM_UI_InitGraphicSystem\n"));

	atGRPXFUNCS funcs;
	int *vv = (int *)lInitData1;
	int vv2[6];

	if( vv == atNULL ) return atFALSE;

	vv2[0] = vv[0];
	vv2[1] = vv[1];
	vv2[2] = vv[2];
	vv2[3] = vv[3];
	vv2[4] = vv[4];
	vv2[5] = vv[5];  // HGDC

	SYSAPI_DebugPrintf(_AT("SYSAPI_InitDisplay\n"));
	
	SYSAPI_InitDisplay( lSysInst, (long)vv2, lInitData2 );
		
	funcs.lpData = atNULL;
	funcs.fnAllocMem = atUIAPI_AllocMem_rel;
	funcs.fnFreeMem  = atUIAPI_FreeMem; 

	SYSAPI_DebugPrintf(_AT("atGRPX_Initialize\n"));
   	g_lpGrpX = atGRPX_Initialize( SYSAPI_GetScreenWidth(), SYSAPI_GetScreenHeight(), 
								SYSAPI_GetScreenBPP(), SYSAPI_GetFrameBufferPtr(), SYSAPI_GetFrameVMemWidth(), &funcs );

	SYSAPI_DebugPrintf(_AT("OEM_UI_InitGraphicSystem g_lpGrpX=0x%x...done\n"), g_lpGrpX);
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
#ifdef _SDIC_VIEWMAP_
    switch( func_no )
	{
        case 1 :
			atGRPX_DrawLineAA( (atGRPX_CANVAS *)gdc, param1, param2, param3, param4, param5 );
			break;

		case 0x2000:
			{
				if(param1)
					PwmTimerLedEnable( PWM_TIMER_AF_4HZ );
				else
					PwmTimerLedDisable( PWM_TIMER_AF_4HZ );
			}
			break;

		case 0x4000:
			switch(param1)
			{
			case 1:
				I43_SendHCEMessage(HM_GPS_GUI_UPDATE, GPS_INFO_MAPVIEW);
				break;
			case 2:
				I43_SendHCEMessage(HM_GPS_GUI_UPDATE, GPS_INFO_PHOTOVIEW);
				break;
			case 3:
				I43_SendHCEMessage(HM_GPS_GUI_UPDATE, GPS_INFO_SINGLEVIEW);
				break;
			case 4:
				I43_SendHCEMessage(HM_GPS_GUI_UPDATE, GPS_UP_INFO_CLEAR);
				break;
			case 5:
				I43_SendHCEMessage(HM_GPS_GUI_UPDATE, GPS_DOWN_INFO_CLEAR);
				break;
			case 6:
				I43_SendHCEMessage(HM_GPS_GUI_UPDATE, GPS_MAPVIEW_ERROR);
				break;
			case 7:
				I43_SendHCEMessage(HM_GPS_GUI_UPDATE, GPS_LOGO_SHOW);
				break;
			case 8:
				I43_SendHCEMessage(HM_GPS_GUI_UPDATE, GPS_MAP_LOADING_SHOW);
				break;
			case 9:
				I43_SendHCEMessage(HM_GPS_GUI_UPDATE, GPS_MAP_LOADING_HIDE);
				break;
			default:
				break;
			}
			break;

		default:
			break;
	   }

#else
	switch( func_no ){
	case 1 : atGRPX_DrawLineAA( (atGRPX_CANVAS *)gdc, param1, param2, param3, param4, param5 );
		break;
    }
#endif
            
    return 0;
}

