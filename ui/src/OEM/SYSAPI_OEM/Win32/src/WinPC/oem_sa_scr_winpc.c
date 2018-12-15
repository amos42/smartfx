 //
// Canvas_wipi.c
//
////////////////////////////////////////////////////////////////

#include "OEM_SA_Scr.h"
#include "OEM_SA_Mem.h"
#include "SYSAPI.h"
#include <string.h>
#include <windows.h>


#define __USE_SHARED_SCREEN_MEMORY_


#ifdef __USE_SHARED_SCREEN_MEMORY_
#include <tchar.h>
#define SYS_VIDEO_BUFFER _TEXT("SYS_VIDEO_BUFFER")
#endif


#define MAKE565(r,g,b) ((atWORD)(((((atWORD)((r) & 0xF8)) << (5+6-3))) | \
					   ((((atWORD)((g) & 0xFC)) << (5-2))) | \
					   (((atWORD)((b) & 0xF8)) >> 3)))
#define GET565R(cl)    ((atBYTE)(((cl) >> 8) & 0xF8))
#define GET565G(cl)    ((atBYTE)(((cl) >> 3) & 0xFC))
#define GET565B(cl)    ((atBYTE)(((cl) << 3) & 0xF8))


typedef struct tagBITMAPINFO16 {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[4];
} BITMAPINFO16, FAR *LPBITMAPINFO16, *PBITMAPINFO16;


extern HWND g_SYS_hWindow;

extern LRESULT (CALLBACK *g_SYS_WndProc_Paint)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static int		g_nScreenWidth	= 0;
static int		g_nScreenHeight	= 0;
static int		g_nScreenBPP	= 0;
static short  * g_pScreenBuffer = atNULL;
static int		g_nScreenVMemWidth	= 0;
static int		g_nScreenBufferSize = 0;

int		g_SYS_nScale;
//static SYS_CRSECTION g_SYS_ScrCrSection;
static int g_NowDrawing = 0;

#ifdef __USE_SHARED_SCREEN_MEMORY_
static HANDLE g_hSharedMem;
static atVOID* g_baseAddress;
#endif



// Canvas의 프레임 버퍼를 윈도우 화면에 표시한다.
static void _FlushToScreen( HDC hDC, int sx, int sy, int width, int height )
{
	HDC hmemdc;
	HBITMAP bitmap;
	BITMAPINFO16	Bm;
	BYTE*			pBits;

	if( g_pScreenBuffer == atNULL ) return;

	hmemdc = CreateCompatibleDC( hDC );
	if( hmemdc == atNULL ) return;

	pBits = (BYTE *)g_pScreenBuffer;

	memset( &Bm, 0, sizeof(BITMAPINFO16) );
	Bm.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	Bm.bmiHeader.biWidth = g_nScreenVMemWidth;
	Bm.bmiHeader.biHeight = -g_nScreenHeight;
	Bm.bmiHeader.biPlanes = 1;
	Bm.bmiHeader.biBitCount = g_nScreenBPP;
	Bm.bmiHeader.biXPelsPerMeter = 0;
	Bm.bmiHeader.biYPelsPerMeter = 0;
	Bm.bmiHeader.biClrUsed = 0;
	Bm.bmiHeader.biClrImportant = 0;

	switch( Bm.bmiHeader.biBitCount ) 
	{
	case 24:
		Bm.bmiHeader.biCompression = BI_RGB;
		Bm.bmiHeader.biSizeImage = g_nScreenVMemWidth * g_nScreenHeight * 3;
		break;
	case 16:
		Bm.bmiHeader.biCompression = BI_BITFIELDS;
		Bm.bmiHeader.biSizeImage = g_nScreenVMemWidth * g_nScreenHeight * 2;

		//11111000 00011111
		*(DWORD*)(Bm.bmiColors+0) = 0x0000f800;
		*(DWORD*)(Bm.bmiColors+1) = 0x000007e0;
		*(DWORD*)(Bm.bmiColors+2) = 0x0000001f;
		break;
	}

//	SYSAPI_EnterCriticalSection( g_SYS_ScrCrSection );
	bitmap = CreateDIBitmap( hDC, &Bm.bmiHeader, CBM_INIT, pBits, (BITMAPINFO *)&Bm, DIB_RGB_COLORS );
//	SYSAPI_LeaveCriticalSection( g_SYS_ScrCrSection );

	if( bitmap ){
	   HBITMAP oldbitmap = (HBITMAP)SelectObject( hmemdc, bitmap );
	   if( g_SYS_nScale == 1 )
		 BitBlt( hDC, sx, sy, width, height, hmemdc, sx, sy, SRCCOPY );
	   else
	     StretchBlt( hDC, sx*g_SYS_nScale, sy*g_SYS_nScale, width*g_SYS_nScale, height*g_SYS_nScale, hmemdc, sx, sy, width, height, SRCCOPY );
	   SelectObject( hmemdc, oldbitmap );
	   DeleteObject( bitmap );
	}

	DeleteDC( hmemdc );
}


static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (message) {
		case WM_PAINT:
			if( g_NowDrawing ) break;

			BeginPaint(hWnd, &ps);
			_FlushToScreen( ps.hdc, 0, 0, g_nScreenWidth, g_nScreenHeight );
			EndPaint(hWnd, &ps);
			break;
   }

   return 0;
}


static atDWORD OEM_SYSAPI_GetDisplayCaps( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2, SYS_DISPLAY_CAPS *lpScrCaps )
{
	HDC dc;
	
	SYSAPI_memset( lpScrCaps, 0, sizeof(SYS_DISPLAY_CAPS) );

	dc = GetDC( NULL );
	lpScrCaps->nX_DPI = GetDeviceCaps( dc, LOGPIXELSX );
	lpScrCaps->nY_DPI = GetDeviceCaps( dc, LOGPIXELSY );
	ReleaseDC( NULL, dc );
	
	return 0;
}

static atBOOL OEM_SYSAPI_InitDisplay( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	int *vv = (int *)lInitData1;

	if( vv == atNULL ) return atFALSE;

	g_nScreenWidth = vv[0];
	g_nScreenHeight = vv[1];
	g_nScreenBPP = vv[2];
//	g_pScreenBuffer = (atVOID *)vv[3];
	g_nScreenVMemWidth = vv[4];

	g_SYS_WndProc_Paint = WndProc;

	g_nScreenBufferSize = 0;
	g_pScreenBuffer = atNULL;

	OEM_SYSAPI_SetDisplayResolution( g_nScreenWidth, g_nScreenHeight, g_nScreenVMemWidth );

	if( g_pScreenBuffer == atNULL ) return atFALSE;

	SYSAPI_memset( g_pScreenBuffer, 0, g_nScreenBufferSize );

	g_SYS_nScale = 1;

//    g_SYS_ScrCrSection = SYSAPI_CreateCriticalSection();

	return atTRUE;
}

static void OEM_SYSAPI_FinalDisplay(void)
{
	g_SYS_WndProc_Paint = atNULL;

	if( g_pScreenBuffer ){
#ifdef __USE_SHARED_SCREEN_MEMORY_
		UnmapViewOfFile(g_baseAddress);
		g_baseAddress = atNULL;
		CloseHandle( g_hSharedMem );
		g_hSharedMem = atNULL;
#else
		SYSAPI_FreeMem( g_pScreenBuffer );
#endif

		g_pScreenBuffer = atNULL;
		g_nScreenBufferSize = 0;
	}

//	if( g_SYS_ScrCrSection ){
//		SYSAPI_DestroyCriticalSection( g_SYS_ScrCrSection );
//		g_SYS_ScrCrSection = atNULL;
//	}
}

static atBOOL OEM_SYSAPI_SetDisplayResolution( atINT nWidth, atINT nHeight, atINT nFrameWidth )
{
	int new_size;

	g_nScreenWidth = nWidth;
	g_nScreenHeight = nHeight;
	g_nScreenVMemWidth = nFrameWidth;
	
	new_size = g_nScreenVMemWidth * g_nScreenHeight * g_nScreenBPP/8;

	if( new_size > g_nScreenBufferSize ){
#ifdef __USE_SHARED_SCREEN_MEMORY_
		atLONG *ptr;
		if( g_pScreenBuffer ){
			UnmapViewOfFile( g_baseAddress );
			g_baseAddress = atNULL;
			CloseHandle( g_hSharedMem );
		}
		g_hSharedMem = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, new_size + sizeof(long)*4, SYS_VIDEO_BUFFER );	
		g_baseAddress = MapViewOfFile( g_hSharedMem, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
		ptr = (atLONG *)g_baseAddress;
		ptr[0] = g_nScreenWidth;
		ptr[1] = g_nScreenHeight;
		ptr[2] = g_nScreenVMemWidth;
		ptr[3] = g_nScreenBPP;
		g_pScreenBuffer = (atVOID *)((atBYTE *)ptr + sizeof(long)*4);
#else
		if( g_pScreenBuffer ){
			SYSAPI_FreeMem( g_pScreenBuffer );
		}
		g_pScreenBuffer = (atSHORT *)SYSAPI_AllocMem( new_size );
#endif

		if( g_pScreenBuffer != atNULL )
			g_nScreenBufferSize = new_size;
	} else {
#ifdef __USE_SHARED_SCREEN_MEMORY_
		atLONG *ptr;
		ptr = (atLONG *)g_baseAddress;
		ptr[0] = g_nScreenWidth;
		ptr[1] = g_nScreenHeight;
		ptr[2] = g_nScreenVMemWidth;
		ptr[3] = g_nScreenBPP;
		g_pScreenBuffer = (atVOID *)((atBYTE *)ptr + sizeof(long)*4);
#endif
	}

	return (g_pScreenBuffer != atNULL)? atTRUE : atFALSE;
}

static atINT OEM_SYSAPI_GetScreenWidth( NOARGS )
{
	return g_nScreenWidth;
}

static atINT OEM_SYSAPI_GetScreenHeight( NOARGS )
{
	return g_nScreenHeight;
}

static atINT OEM_SYSAPI_GetScreenBPP( NOARGS )
{
	return g_nScreenBPP;
}

static atVOID * OEM_SYSAPI_GetFrameBufferPtr( NOARGS )
{
	return g_pScreenBuffer;
}

static atINT OEM_SYSAPI_GetFrameVMemWidth( NOARGS )
{
	return g_nScreenVMemWidth;
}


static atVOID OEM_SYSAPI_FlushScreenRgn( atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	HDC hdc;
//	RECT rect;
	
	if( g_pScreenBuffer == atNULL ) return;

	hdc = GetDC( g_SYS_hWindow );
	_FlushToScreen( hdc, nX, nY, nWidth, nHeight );
	ReleaseDC( g_SYS_hWindow, hdc );

//	rect.left = nX;
//	rect.top = nY;
//	rect.right = nX + nWidth;
//	rect.bottom = nY + nHeight;
//	InvalidateRect( g_SYS_hWindow, &rect, FALSE );
}


static atULONG OEM_SYSAPI_MakeColor(atBYTE r, atBYTE g, atBYTE b)
{
	return MAKE565( r,g,b );
}

static atBYTE OEM_SYSAPI_GetRValue(atULONG nColor)
{
	return GET565R( nColor );
}

static atBYTE OEM_SYSAPI_GetGValue(atULONG nColor)
{
	return GET565G( nColor );
}

static atBYTE OEM_SYSAPI_GetBValue(atULONG nColor)
{
	return GET565B( nColor );
}


static atBOOL OEM_SYSAPI_BeginDraw( NOARGS )
{
	//SYSAPI_EnterCriticalSection( g_SYS_ScrCrSection );
	g_NowDrawing = 1;
	return atTRUE;
}

static atVOID OEM_SYSAPI_EndDraw( NOARGS )
{
	g_NowDrawing = 0;
	//SYSAPI_LeaveCriticalSection( g_SYS_ScrCrSection );
}


static atLONG OEM_SYSAPI_ExtGraphicMethod( atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
    switch( func_no ){
        case 0x1000 : 
        		     break;
        case 0x8001 : g_SYS_nScale = param1;
        		     break;
    }
            
    return 0;
}



void init_sysapi_scr_oem() 
{
	OEM_SYSAPI_SCR_T oem_funcs = {
		OEM_SYSAPI_GetDisplayCaps,
		OEM_SYSAPI_InitDisplay,
		OEM_SYSAPI_FinalDisplay,

		OEM_SYSAPI_SetDisplayResolution,

		OEM_SYSAPI_GetScreenWidth,
		OEM_SYSAPI_GetScreenHeight,
		OEM_SYSAPI_GetScreenBPP,

		OEM_SYSAPI_GetFrameVMemWidth,
		OEM_SYSAPI_GetFrameBufferPtr,

		OEM_SYSAPI_FlushScreenRgn,

		OEM_SYSAPI_MakeColor,
		OEM_SYSAPI_GetRValue,
		OEM_SYSAPI_GetGValue,
		OEM_SYSAPI_GetBValue,

		OEM_SYSAPI_BeginDraw,
		OEM_SYSAPI_EndDraw,

		OEM_SYSAPI_ExtGraphicMethod
	};

	OEM_SYSAPI_SCR_SetOemFn( &oem_funcs );
}

