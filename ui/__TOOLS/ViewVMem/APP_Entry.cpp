#include <windows.h>
#include <tchar.h>
#include <string.h>

#include "APP_Entry.h"


#define SYS_VIDEO_BUFFER _TEXT("SYS_VIDEO_BUFFER")


//static char heap[16*1024*1024 + 1024];
//static short    pScreenBuffer[320*240];
//static short    pScreenBuffer[240*320];
//static short    pScreenBuffer[1024*768];


static HWND g_hWnd;

static int		g_nScreenWidth	= 0;
static int		g_nScreenHeight	= 0;
static int		g_nScreenBPP	= 0;
static short  * g_pScreenBuffer = NULL;
static int		g_nScreenVMemWidth	= 0;
static int		g_nScreenBufferSize = 0;

int		g_SYS_nScale = 1;

typedef struct tagBITMAPINFO16 {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[4];
} BITMAPINFO16, FAR *LPBITMAPINFO16, *PBITMAPINFO16;

// Canvas의 프레임 버퍼를 윈도우 화면에 표시한다.
static void _FlushToScreen( HDC hDC, int sx, int sy, int width, int height )
{
	HDC hmemdc;
	HBITMAP bitmap;
	BITMAPINFO16	Bm;
	BYTE*			pBits;

	if( g_pScreenBuffer == NULL ) return;

	hmemdc = CreateCompatibleDC( hDC );
	if( hmemdc == NULL ) return;

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

	bitmap = CreateDIBitmap( hDC, &Bm.bmiHeader, CBM_INIT, pBits, (BITMAPINFO *)&Bm, DIB_RGB_COLORS );
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


void APP_Entry_Init( void *hInst, void *hWnd, int width, int height )
{
	g_hWnd = (HWND)hWnd;
	SetTimer( g_hWnd, 0x1000, 100, NULL );
}


void APP_Entry_Final(void)
{
	KillTimer( g_hWnd, 0x1000 );

	if( g_pScreenBuffer ){
		free( g_pScreenBuffer );
		g_pScreenBuffer = NULL;
		g_nScreenBufferSize = 0;
	}
}


void APP_Entry_Enter(void)
{
}


void APP_Entry_Exit(void)
{
}


void APP_Entry_Timer(int id)
{
	if( id == 0x1000 ){
		HANDLE hMemMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SYS_VIDEO_BUFFER);
		if ( hMemMap == NULL ) return;

		long *ptr = (long *)MapViewOfFile(hMemMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if ( ptr == NULL ) return;

		g_nScreenWidth = ptr[0];
		g_nScreenHeight = ptr[1];
		g_nScreenVMemWidth = ptr[2];
		g_nScreenBPP = ptr[3];
		long buffsize = g_nScreenVMemWidth * g_nScreenHeight * g_nScreenBPP/8;
		if( buffsize != g_nScreenBufferSize ){
			if( g_pScreenBuffer != NULL ){
				free( g_pScreenBuffer );
				g_nScreenBufferSize = 0;
			}
			g_pScreenBuffer = (short *)malloc( buffsize );
			if( g_pScreenBuffer != NULL ) g_nScreenBufferSize = buffsize;
		}

		if( g_pScreenBuffer != NULL ){
			memcpy( g_pScreenBuffer, (BYTE *)ptr + sizeof(long)*4, g_nScreenVMemWidth * g_nScreenHeight * g_nScreenBPP/8 );
		}

		UnmapViewOfFile( ptr );
		CloseHandle( hMemMap );

		if( g_pScreenBuffer != NULL ){
			HDC hDC = GetDC(g_hWnd);
			_FlushToScreen( hDC, 0, 0, g_nScreenWidth, g_nScreenHeight );
			ReleaseDC(g_hWnd, hDC);
		}
	}
}


void APP_Entry_Draw( void *hDC )
{
	_FlushToScreen( (HDC)hDC, 0, 0, g_nScreenWidth, g_nScreenHeight );
}


void APP_Entry_Debug(int param1, int param2)
{
}
