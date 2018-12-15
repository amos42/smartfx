 //
// Canvas_wipi.c
//
////////////////////////////////////////////////////////////////

#include "OEM_SA_Scr.h"
#include "OEM_SA_Mem.h"
#include "SYSAPI.h"
#include <string.h>
#include "GrpX.h"

#include "GDI.h"	// CoachFW
// for test
#include <stdlib.h>

#define MAKE565(r,g,b) ((atWORD)(((((atWORD)((r) & 0xF8)) << (5+6-3))) | \
					   ((((atWORD)((g) & 0xFC)) << (5-2))) | \
					   (((atWORD)((b) & 0xF8)) >> 3)))
#define GET565R(cl)    ((atBYTE)(((cl) >> 8) & 0xF8))
#define GET565G(cl)    ((atBYTE)(((cl) >> 3) & 0xFC))
#define GET565B(cl)    ((atBYTE)(((cl) << 3) & 0xF8))


#if 0
typedef struct tagBITMAPINFO16 {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[4];
} BITMAPINFO16, FAR *LPBITMAPINFO16, *PBITMAPINFO16;


extern HWND g_SYS_hWindow;

extern LRESULT (CALLBACK *g_SYS_WndProc_Paint)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

HDC g_hCameraDC = atNULL;		// SDISPLAYCONTEXT

static int		g_nScreenWidth	= 0;
static int		g_nScreenHeight	= 0;
static int		g_nScreenBPP	= 0;
static short  * g_pScreenBuffer = NULL;
static int		g_nScreenVMemWidth	= 0;
static int		g_nScreenBufferSize = 0;

static unsigned char* g_pBufferY = NULL;
static unsigned char* g_pBufferU = NULL;
static unsigned char* g_pBufferV = NULL;



void rgb2yuv422()
{
	int j, i;
	int idx;
	unsigned int sum[2];

	unsigned short cl;
	unsigned int r, g, b;
	unsigned short* pos;
	unsigned short* line;

	int height, width;

	width  = g_nScreenWidth;
	height = g_nScreenHeight;

	unsigned char* y = g_pBufferY;
	unsigned char *u = g_pBufferU;
	unsigned char *v = g_pBufferV;

	SYSAPI_DebugPrintf(_AT("rgb2yuv422(%d %d)\n"), width, height);
	
	idx = 0; line = g_pScreenBuffer;
	for( j = 0 ; j < height ; j++ ){
		pos = line;
		for( i = 0 ; i < width ; i+=2 ){

			cl = *pos++;
			r = GET_RED(cl);
			g = GET_GREEN(cl);
			b = GET_BLUE(cl);

			sum[0] = (( -38 * r -  74 * g + 112 * b + 128) >> 8) + 128;
			sum[1] = (( 112 * r -  94 * g -  18 * b + 128) >> 8) + 128;

			y[idx] = ((  66 * r + 129 * g +  25 * b + 128) >> 8) + 16;
			idx++;

			cl = *pos++;
			r = GET_RED(cl);
			g = GET_GREEN(cl);
			b = GET_BLUE(cl);

			sum[0] += (( -38 * r -  74 * g + 112 * b + 128) >> 8) + 128;
			sum[1] += (( 112 * r -  94 * g -  18 * b + 128) >> 8) + 128;

			y[idx] = ((  66 * r + 129 * g +  25 * b + 128) >> 8) + 16;
			u[idx-1] = sum[0]/2;
			v[idx-1] = sum[1]/2;
			idx++;

		}
		line += width;
	}

}


// Canvas의 프레임 버퍼를 화면에 표시한다.

static void _FlushToScreen( HDC hDC, int sx, int sy, int width, int height )
{
	
	int i,j, idx;
	unsigned char *addr = NULL;

	SYSAPI_DebugPrintf(_AT("_FlushToScreen(%d,%d,%d,%d)\n"), sx, sy, width, height);
	
	if( !hDC || g_pScreenBuffer == atNULL )
	{
		return;
	}

	if(!hDC->pbyAddr) return;

	addr = hDC->pbyAddr;

	idx = 0;

	rgb2yuv422();

	for(i = 0; i < height; i++)
	{
		for( j = 0; j < width; j++, idx++ )
		{
			*addr++ = g_pBufferY[idx];
			*addr++ = ( idx & 0x1 )? g_pBufferV[idx & ~0x1] : g_pBufferU[idx & ~0x1];
		}
	}

	SYSAPI_DebugPrintf(_AT("_FlushToScreen ...done\n"));
}



#if 0
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;

	switch (message) {
		case WM_PAINT:
			BeginPaint(hWnd, &ps);
			_FlushToScreen( ps.hdc, 0, 0, g_nScreenWidth, g_nScreenHeight );
			EndPaint(hWnd, &ps);
			break;
   }

   return 0;
}
#endif


atDWORD OEM_SYSAPI_GetDisplayCaps( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return 0;
}

atBOOL OEM_SYSAPI_InitDisplay( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	int *vv = (int *)lInitData1;

	SYSAPI_DebugPrintf(_AT("OEM_SYSAPI_InitDisplay\n"));

	if( vv == atNULL ) return atFALSE;

	g_nScreenWidth     = vv[0];
	g_nScreenHeight    = vv[1];
	g_nScreenBPP       = vv[2];
	g_pScreenBuffer    = (atVOID *)vv[3];
	g_nScreenVMemWidth = vv[4];

	g_hCameraDC = (HDC)vv[5]; // HGDC 
	

	//g_SYS_WndProc_Paint = WndProc;

	g_nScreenBufferSize = g_nScreenVMemWidth * g_nScreenHeight * g_nScreenBPP/8;
	if(	!g_pScreenBuffer)
	{	
		SYSAPI_DebugPrintf(_AT("Alloc ScreenBuffer (%d bytes)\n"), g_nScreenBufferSize);
		g_pScreenBuffer = SYSAPI_AllocMem( g_nScreenBufferSize );
		if(!g_pScreenBuffer) return atFALSE;
		SYSAPI_memset( g_pScreenBuffer, 0, g_nScreenBufferSize );
	}

	g_pBufferY = SYSAPI_AllocMem(g_nScreenWidth * g_nScreenHeight);
	g_pBufferU = SYSAPI_AllocMem(g_nScreenWidth * g_nScreenHeight);
	g_pBufferV = SYSAPI_AllocMem(g_nScreenWidth * g_nScreenHeight);

	SYSAPI_memset(g_pBufferY, 0, g_nScreenWidth * g_nScreenHeight);
	SYSAPI_memset(g_pBufferU, 0, g_nScreenWidth * g_nScreenHeight);
	SYSAPI_memset(g_pBufferV, 0, g_nScreenWidth * g_nScreenHeight);


	return atTRUE;
}

void OEM_SYSAPI_FinalDisplay(void)
{
	SYSAPI_DebugPrintf(_AT("OEM_SYSAPI_FinalDisplay\n"));

	//g_SYS_WndProc_Paint = atNULL;

	if( g_pScreenBuffer ){
		SYSAPI_FreeMem( g_pScreenBuffer );
		g_pScreenBuffer = atNULL;
		//g_nScreenBufferSize = 0;
	}

	if(g_pBufferY)
	{
		SYSAPI_FreeMem(g_pBufferY);
		g_pBufferY = NULL;
	}
	if(g_pBufferU)
	{
		SYSAPI_FreeMem(g_pBufferU);
		g_pBufferU = NULL;
	}
	if(g_pBufferV)
	{
		SYSAPI_FreeMem(g_pBufferV);
		g_pBufferV = NULL;
	}
	
	g_pScreenBuffer = atNULL;
	g_nScreenBufferSize = 0;
}

atBOOL OEM_SYSAPI_SetDisplayResolution( atINT nWidth, atINT nHeight, atINT nFrameWidth )
{

	int new_size;

	g_nScreenWidth = nWidth;
	g_nScreenHeight = nHeight;
	g_nScreenVMemWidth = nFrameWidth;

	/**
	
	new_size = g_nScreenVMemWidth * g_nScreenHeight * g_nScreenBPP/8;

	if( new_size > g_nScreenBufferSize ){
		SYSAPI_FreeMem( g_pScreenBuffer );
		g_pScreenBuffer = (atSHORT *)SYSAPI_AllocMem( new_size );
		g_nScreenBufferSize = new_size;
	}
	**/

	return atTRUE;
}

atINT OEM_SYSAPI_GetScreenWidth( NOARGS )
{
	return g_nScreenWidth;
}

atINT OEM_SYSAPI_GetScreenHeight( NOARGS )
{
	return g_nScreenHeight;
}

atINT OEM_SYSAPI_GetScreenBPP( NOARGS )
{
	return g_nScreenBPP;
}

atVOID * OEM_SYSAPI_GetFrameBufferPtr( NOARGS )
{
	return g_pScreenBuffer;
}

atINT OEM_SYSAPI_GetFrameVMemWidth( NOARGS )
{
	return g_nScreenVMemWidth;
}


atVOID OEM_SYSAPI_FlushScreenRgn( atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	SYSAPI_DebugPrintf(_AT("OEM_SYSAPI_FlushScreenRgn(%d,%d,%d,%d)\n"), nX, nY, nWidth, nHeight);
	if( g_pScreenBuffer == NULL ) return;

	if(!g_hCameraDC)
	{
		fprintf(stderr, "g_hDC is NULL!!\n");
		return;
	}

	//_FlushToScreen( g_hDC, nX, nY, nWidth, nHeight );
	_FlushToScreen( g_hCameraDC, 0, 0, g_nScreenWidth, g_nScreenHeight);
}


atULONG OEM_SYSAPI_MakeColor(atBYTE r, atBYTE g, atBYTE b)
{
	return MAKE565( r,g,b );
}

atBYTE OEM_SYSAPI_GetRValue(atULONG nColor)
{
	return GET565R( nColor );
}

atBYTE OEM_SYSAPI_GetGValue(atULONG nColor)
{
	return GET565G( nColor );
}

atBYTE OEM_SYSAPI_GetBValue(atULONG nColor)
{
	return GET565B( nColor );
}


atLONG OEM_SYSAPI_ExtGraphicMethod( atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
    switch( func_no ){
		/**
		case 10     :
			g_hCameraDC =  (HDC)param1;
			break;
		**/
        case 0x1000 : 
        		     break;
		default : 
			break;
    }
            
    return 0;
}

