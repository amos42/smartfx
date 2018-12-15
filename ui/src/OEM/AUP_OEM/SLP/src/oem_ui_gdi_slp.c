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


static atBOOL OEM_UI_InitGraphicSystem( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
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

static void OEM_UI_CloseGraphicSystem(void)
{
	atGRPX_Finalize(g_lpGrpX);
	g_lpGrpX = atNULL;

	SYSAPI_FinalDisplay();
}

static atBOOL OEM_UI_SetScreenResolution( atINT nWidth, atINT nHeight )
{
	if( !SYSAPI_SetDisplayResolution( nWidth, nHeight, (nWidth + 0x3) & ~0x3 ) ) return atFALSE;
	
	atGRPX_SetFrameBuffer( g_lpGrpX, SYSAPI_GetFrameBufferPtr() );
	atGRPX_SetScreenResolution( g_lpGrpX, SYSAPI_GetScreenWidth(), SYSAPI_GetScreenHeight(), SYSAPI_GetFrameVMemWidth() );

	return atTRUE;
}

static atBOOL OEM_UI_GetScreenDPI( atINT *lpnX_DPI, atINT *lpnY_DPI )
{
	if( lpnX_DPI ) *lpnX_DPI = x_dpi;
	if( lpnY_DPI ) *lpnY_DPI = y_dpi;

	return atTRUE;
}

static atINT OEM_UI_GetScreenWidth( NOARGS )
{
	return SYSAPI_GetScreenWidth();
}

static atINT OEM_UI_GetScreenHeight( NOARGS )
{
	return SYSAPI_GetScreenHeight();
}

static atINT OEM_UI_GetScreenBPP( NOARGS )
{
	return SYSAPI_GetScreenBPP();
}

static atVOID * OEM_UI_GetScreenBufferPtr( NOARGS )
{
	return SYSAPI_GetFrameBufferPtr();
}

static atBOOL OEM_UI_BeginDraw( NOARGS )
{
	return SYSAPI_BeginDraw();
}

static atVOID OEM_UI_EndDraw( NOARGS )
{
	SYSAPI_EndDraw();
}

static atVOID OEM_UI_FlushScreenRgn( atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	SYSAPI_FlushScreenRgn( nX, nY, nWidth, nHeight );
}

static atLONG 		OEM_UI_ExtGraphicMethod( atBOOL CanvasActivate, OEM_UI_GDC gdc, atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
    switch( func_no ){
        case 1 : atGRPX_DrawLineAA( (atGRPX_CANVAS *)gdc, param1, param2, param3, param4, param5 );
                 break;
    }
            
            
    return 0;
}



void init_uiapi_gdi_oem() 
{
	OEM_UI_GDI_T oem_funcs = {
		OEM_UI_InitGraphicSystem,
		OEM_UI_CloseGraphicSystem,
		atNULL,  // OEM_UI_FlushScreenEnable,

		OEM_UI_SetScreenResolution,
		OEM_UI_GetScreenDPI,
		OEM_UI_GetScreenWidth,
		OEM_UI_GetScreenHeight,
		OEM_UI_GetScreenBPP,
		OEM_UI_GetScreenBufferPtr,

		OEM_UI_BeginDraw,
		OEM_UI_EndDraw,

		//atNULL,  // OEM_GetAnnounceHeight,
		OEM_UI_FlushScreenRgn,
		//atNULL,  // OEM_UI_FlushToDC,

		OEM_GetRGB,
		OEM_GetRValue,
		OEM_GetGValue,
		OEM_GetBValue,

		//atNULL,  // OEM_GetStringWidth,
		//atNULL,  // OEM_GetFontHeight,

		OEM_CreateCanvas,
		OEM_DestroyCanvas,
		OEM_GetCanvasAttr,

		OEM_GetCanvasArea,
		OEM_SetCanvasArea,
		OEM_GetCanvasClipRect,
		OEM_SetCanvasClipRect,
		//atNULL,  // OEM_GetScreenBufferPtr,
		OEM_CopyCanvas,

		//atNULL,  // OEM_FlushCanvasRgn,

		//atNULL,  // OEM_SetFont,
		//atNULL,  // OEM_GetFont,
		OEM_SetFrColor,
		OEM_GetFrColor,
		OEM_SetBgColor,
		OEM_GetBgColor,

		OEM_GetPixel,
		OEM_DrawPixel,
		OEM_FillRect,
		OEM_FillRectXOR,
		OEM_DrawRect,
		OEM_DrawLine,
		OEM_DrawCircle,
		OEM_FillCircle,
		OEM_DrawEllipse,
		OEM_FillEllipse,
		OEM_DrawPolyLine,
		OEM_DrawPolyLineEx,
		OEM_FillPolygon,
		OEM_FillPolygonBitmap,
		OEM_FillPolyPolygon,
		OEM_FillPolyPolygonEx,
		OEM_FillPolyPolygonBitmap,
		OEM_FillPolyPolygonBitmapEx,

		OEM_DrawString,
		OEM_DrawStringRotate,

		OEM_CreateStrBitmap,
		OEM_DestroyStrBitmap,
		OEM_GetStrBitmapMetric,
		OEM_DrawStrBitmap,

		OEM_CreateBlankBitmap,
		OEM_DestroyBitmap,
		OEM_GetBitmapBits,
		OEM_GetBitmapBPP,
		OEM_GetBitmapWidth,
		OEM_GetBitmapHeight,
		OEM_GetBitmapWidthBytes,
		OEM_GetBitmapLogPos,
		OEM_SetBitmapLogPos,
		OEM_SetBitmapTransColor,
		OEM_ClearBitmapTransColor,
		//atNULL, //OEM_SetBitmapMask,
		OEM_CreateBitmapFromStream,

		//atNULL, //OEM_CreateBitmapMask,
		//atNULL, //OEM_DestroyBitmapMask,

		OEM_GetBitmap,
		OEM_DrawBitmap,
		//atNULL, //OEM_DrawBitmapWithAlphaChannel,
		OEM_DrawBitmapStretch,
		//atNULL, //OEM_DrawBitmapStretchWithAlphaChannel,
		OEM_DrawBitmapRotate,
		//atNULL, //OEM_DrawBitmapRotateWithAlphaChannel,

		OEM_UI_ExtGraphicMethod
	};

	OEM_UI_GDI_SetOemFn( &oem_funcs );
}

