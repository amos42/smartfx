
#include <windows.h>
#include "OEM_SA_Scr.h"
#include "OEM_SA_Mem.h"
#include "SYSAPI.h"
#include <string.h>


#define MAKE565(r,g,b) ((atWORD)(((((atWORD)((r) & 0xF8)) << (5+6-3))) | \
					   ((((atWORD)((g) & 0xFC)) << (5-2))) | \
					   (((atWORD)((b) & 0xF8)) >> 3)))
#define GET565R(cl)    ((atBYTE)(((cl) >> 8) & 0xF8))
#define GET565G(cl)    ((atBYTE)(((cl) >> 3) & 0xFC))
#define GET565B(cl)    ((atBYTE)(((cl) << 3) & 0xF8))



#define __USES_GAPI_	(0)


#if __USES_GAPI_

#include "gapi/gx.h"
#pragma comment(lib, "gx.lib")

atBOOL g_bGXInit;

#else

typedef struct tagBITMAPINFO16 {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[4];
} BITMAPINFO16, FAR *LPBITMAPINFO16, *PBITMAPINFO16;

static HDC g_hScrMemDC;
static HBITMAP g_hScrBitmap, g_hPrevBitmap;
static int g_NowDrawing = 0;

#endif

#ifdef __cplusplus
extern "C" {
#endif

extern HWND g_SYS_hWindow;
extern LRESULT (CALLBACK *g_SYS_WndProc_Paint)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#ifdef __cplusplus
}
#endif

static int		g_nScreenWidth	= 0;
static int		g_nScreenHeight	= 0;
static int		g_nScreenBPP	= 0;
static void *   g_pScreenBuffer = NULL;
static int		g_nScreenVMemWidth	= 0;
static long		g_nScreenBufferSize = 0;;


// Canvas의 프레임 버퍼를 윈도우 화면에 표시한다.
static void _FlushToScreen( HDC hDC, int sx, int sy, int width, int height )
{
#if __USES_GAPI_
	int i;
	void *pFrame;
	short *s_ptr, *d_ptr;

	if( !g_bGXInit ) return;
		
	GXDisplayProperties GXDP = GXGetDisplayProperties();

	pFrame = (atVOID *)GXBeginDraw();
	if( pFrame == NULL ) return;
	
	s_ptr = (short *)g_pScreenBuffer + sy * g_nScreenVMemWidth + sx;
//	d_ptr = (short *)((BYTE *)pFrame + (sx * GXDP.cbxPitch) + (sy * GXDP.cbyPitch));
	d_ptr = (short *)pFrame + sy * GXDP.cxWidth + sx;

//	if( GXDP.ffFormat == KfLandscape
	for( i = 0; i < height; i++ ){
		memcpy( d_ptr, s_ptr, width * sizeof(short) );
		s_ptr += g_nScreenVMemWidth;
//		d_ptr += GXDP.cbxPitch;
		d_ptr += GXDP.cxWidth;
	}
	GXEndDraw();
#else

#ifdef _DEBUG
	//printf("%d : FlushToScreen(%d,%d,%d,%d)\n", GetTickCount(), sx,sy, width, height);
#endif
	if( hDC && g_hScrMemDC )
		BitBlt( hDC, sx, sy, width, height, g_hScrMemDC, sx, sy, SRCCOPY );	
#endif//#if USE_GAPI
}


static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (message) {
		case WM_PAINT:
			if( g_NowDrawing ) break;

#ifdef _DEBUG
			printf("WM_PAINT\n");
#endif

			if( g_pScreenBuffer != NULL ){
				BeginPaint(hWnd, &ps);
				_FlushToScreen( ps.hdc, 0, 0, g_nScreenWidth, g_nScreenHeight );
				EndPaint(hWnd, &ps);
			}
			break;
   }

   return 0;
}


static atBOOL _init_bm(void)
{
#if !__USES_GAPI_
	BITMAPINFO16 Bm;
	void *pBits;
#endif

#if __USES_GAPI_
	if( GXOpenDisplay( g_SYS_hWindow, GX_FULLSCREEN ) ){
		g_bGXInit = TRUE;
	} else {
		g_bGXInit = FALSE;
	}

	g_nScreenBufferSize = g_nScreenVMemWidth * g_nScreenHeight * g_nScreenBPP/8;
	g_pScreenBuffer = SYSAPI_AllocMem( g_nScreenBufferSize );
	SYSAPI_memset( g_pScreenBuffer, 0, g_nScreenBufferSize );
	
	return g_bGXInit;
#else	
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

	switch( Bm.bmiHeader.biBitCount ){
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
	g_hScrBitmap = CreateDIBSection( g_hScrMemDC, (BITMAPINFO*)&Bm, DIB_RGB_COLORS, (void**)&pBits, 0, 0 );
	if( g_hScrBitmap == NULL ){
		DeleteDC( g_hScrMemDC );
		g_hScrMemDC = atNULL;
		g_pScreenBuffer = atNULL;
		return atFALSE;
	}
	
	g_pScreenBuffer = (atVOID *)pBits;

	return atTRUE;
#endif
}

static void _init_dc(void)
{
#if __USES_GAPI_
	if( GXOpenDisplay( g_SYS_hWindow, GX_FULLSCREEN ) ){
		g_bGXInit = TRUE;
	} else {
		g_bGXInit = FALSE;
	}

	g_nScreenBufferSize = g_nScreenVMemWidth * g_nScreenHeight * g_nScreenBPP/8;
	g_pScreenBuffer = SYSAPI_AllocMem( g_nScreenBufferSize );
	SYSAPI_memset( g_pScreenBuffer, 0, g_nScreenBufferSize );
	
	return g_bGXInit;
#else
	g_hScrMemDC = CreateCompatibleDC( atNULL );
	if( g_hScrMemDC == NULL ){
		g_hScrBitmap = atNULL;
		g_pScreenBuffer = atNULL;
		return;
	}

	_init_bm();
	
	if( g_hScrBitmap == NULL ){
		g_pScreenBuffer = atNULL;
		return;
	}
	
	g_hPrevBitmap = (HBITMAP)SelectObject( g_hScrMemDC, g_hScrBitmap );
#endif
}

static void _free_bm(void)
{
#if __USES_GAPI_
#else
	if( g_hScrMemDC ){
		SelectObject( g_hScrMemDC, g_hPrevBitmap );
		g_hPrevBitmap = atNULL;
	}

	if( g_hScrBitmap ){
		DeleteObject( g_hScrBitmap );
		g_hScrBitmap = atNULL;
	}

	g_pScreenBuffer = atNULL;
#endif
}

static void _free_dc(void)
{
#if __USES_GAPI_
	if( g_pScreenBuffer ){
		SYSAPI_FreeMem( g_pScreenBuffer );
		g_pScreenBuffer = atNULL;
		g_nScreenBufferSize = 0;
	}

	GXCloseDisplay ();
#else
	_free_bm();

	if( g_hScrMemDC ){
		DeleteDC( g_hScrMemDC );
		g_hScrMemDC = atNULL;
	}
#endif
}


atDWORD OEM_SYSAPI_GetDisplayCaps( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2, SYS_DISPLAY_CAPS *lpScrCaps )
{
	HDC dc;
	
	SYSAPI_memset( lpScrCaps, 0, sizeof(SYS_DISPLAY_CAPS) );

	dc = GetDC( NULL );
	lpScrCaps->nX_DPI = GetDeviceCaps( dc, LOGPIXELSX );
	lpScrCaps->nY_DPI = GetDeviceCaps( dc, LOGPIXELSY );
	ReleaseDC( NULL, dc );
	
	return 0;
}

atBOOL OEM_SYSAPI_InitDisplay( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	int *vv = (int *)lInitData1;

	if( vv == atNULL ) return atFALSE;

	g_nScreenWidth = vv[0];
	g_nScreenHeight = vv[1];
	g_nScreenBPP = vv[2];
//	g_pScreenBuffer = (atVOID *)vv[3];
	g_pScreenBuffer = atNULL;
	g_nScreenVMemWidth = vv[4];

	//if( !_init_dc() ) return atFALSE;
	_init_dc();
		
	g_SYS_WndProc_Paint = WndProc;
    
	return atTRUE;
}


void OEM_SYSAPI_FinalDisplay(void)
{
	g_SYS_WndProc_Paint = atNULL;

	_free_dc();
}

atBOOL OEM_SYSAPI_SetDisplayResolution( atINT nWidth, atINT nHeight, atINT nFrameWidth )
{
	_free_dc();

	g_nScreenWidth = nWidth;
	g_nScreenHeight = nHeight;
	g_nScreenVMemWidth = nFrameWidth;

//	return _init_dc();
	_init_dc();

	return ( g_pScreenBuffer != atNULL );
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


atBOOL OEM_SYSAPI_BeginDraw( NOARGS )
{
	g_NowDrawing = 1;
	return atTRUE;
}

atVOID OEM_SYSAPI_EndDraw( NOARGS )
{
	g_NowDrawing = 0;
}


atLONG OEM_SYSAPI_ExtGraphicMethod( atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
    switch( func_no ){
        case 0x1000 : 
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
