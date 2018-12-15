#include <windows.h>
#include <tchar.h>
#include <string.h>

#include "AT_Types.h"
#include "A_DALLOC.h"
#include "APP_Entry.h"


#define SYS_HEAP_BUFFER _TEXT("SYS_HEAP_BUFFER")

#define REFRESH_INTERVAL 500


static HWND		g_hWnd;
static HDC		g_hMemDC;
static HBITMAP	g_hBitmap;

static int		g_nGlobalHeapSize;
static void *	g_pGlobalHeap;

static int g_nClientWidth, g_nClientHeight;


#define RECT_SIZE 3
#define BLK_SIZE (1024*4)

struct _draw_heap_info {
	HDC dc;
	int width, height;
};

static void func( void *lpParam, void *addr, int pos, int size, int type )
{
	struct _draw_heap_info *p = (struct _draw_heap_info *)lpParam;
	int i, sx, sy, xx, yy;
	COLORREF cl[3];
	HBRUSH brush[3];
	HBRUSH oldbrush;
	
	cl[0] = RGB(255, 0, 0);
	cl[1] = RGB(168, 168, 168);
	cl[2] = RGB(64, 128, 255);

	brush[0] = CreateSolidBrush( cl[0] );
	brush[1] = CreateSolidBrush( cl[1] );
	brush[2] = CreateSolidBrush( cl[2] );

//	sx = pos / BLK_SIZE % ((240-10*2)/RECT_SIZE);
//	sy = pos / BLK_SIZE / ((240-10*2)/RECT_SIZE);
	sx = pos / BLK_SIZE % ((p->width-10*2)/RECT_SIZE);
	sy = pos / BLK_SIZE / ((p->height-10*2)/RECT_SIZE);

	size /= BLK_SIZE;
	if( size <= 0 ) size = 1;

	oldbrush = (HBRUSH)SelectObject( p->dc, brush[0] );
	for( i = 0; i < size; i ++ ){
		SelectObject( p->dc, brush[type] );
		xx = 10 + sx*RECT_SIZE;
		yy = 16 +56-24-27 + sy*RECT_SIZE;
		Rectangle( p->dc, xx, yy, xx+RECT_SIZE, yy+RECT_SIZE );
		RECT rt;
		rt.left = xx; rt.top = yy;
		rt.right = xx + RECT_SIZE; rt.bottom = yy + RECT_SIZE;
		FrameRect( p->dc, &rt, brush[type] );
//		atUIAPI_DrawRect( p->dc, sx*RECT_SIZE+10-3, sy*RECT_SIZE+60-24-27, RECT_SIZE+1, RECT_SIZE+1,  UI_COLOR_BLACK, 1 );
		sx ++;
		if( sx >= (p->width-10*2)/RECT_SIZE ){ sx = 0; sy ++; }
	}
	SelectObject( p->dc, oldbrush );

	DeleteObject( brush[0] );
	DeleteObject( brush[1] );
	DeleteObject( brush[2] );
}

static void _FlushToScreen( HDC hDC, int sx, int sy, int width, int height )
{
//#ifdef USES_DALLOC
//	LPMEMORYINFOWNDDATA lpData = (LPMEMORYINFOWNDDATA)atWINDOW_GetDataPtr(hWnd);
	//atREGION area;
	RECT rt;
	atTCHAR str[64];

//	ClearAll(UI_COLOR_WHITE);
	Rectangle( hDC, sx, sy, sx+width, sy+height );

	if( atDALLOC_CheckValidateHeap(g_pGlobalHeap) ){
		struct _draw_heap_info p;
		p.dc = hDC;
		p.width = width;
		p.height = height;
		atDALLOC_GetHeapInfo( g_pGlobalHeap, (void *)&p, func );
	} else {
//		atUIAPI_SetFont( hCanvas, atUIAPI_GetStockFont(1) );
		rt.left = sx; rt.top = sy;
		rt.right = sx + width; rt.bottom = sy + height;
		DrawTextEx( hDC, _AT("Memory Error!"), 13, &rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE, NULL );
		return;
	}

	_stprintf( str, _T("BlkSize:%d"), BLK_SIZE );
	rt.left = sx; rt.top = sy+1;
	rt.right = sx + width-1; rt.bottom = sy + 16;
	DrawTextEx( hDC, str, _tcslen(str), &rt, DT_RIGHT | DT_VCENTER | DT_SINGLELINE, NULL );
	
	_stprintf( str, _T("AllocSize:%d,  FreeSize:%d"), atDALLOC_GetTotalAllocSize(g_pGlobalHeap), atDALLOC_GetTotalLeftSize(g_pGlobalHeap) );
	rt.left = sx; rt.top = sy + height - 2 - 16;
	rt.right = sx + width; rt.bottom = sy + height;
	DrawTextEx( hDC, str, _tcslen(str), &rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE, NULL );
//#endif
}

void APP_Entry_Init( void *hInst, void *hWnd, int width, int height )
{
	g_hWnd = (HWND)hWnd;
	g_nClientWidth = width;
	g_nClientHeight = height;
	SetTimer( g_hWnd, 0x1000, REFRESH_INTERVAL, NULL );
}


void APP_Entry_Final(void)
{
	KillTimer( g_hWnd, 0x1000 );

	if( g_pGlobalHeap ){
		free( g_pGlobalHeap );
		g_pGlobalHeap = NULL;
		g_nGlobalHeapSize = 0;

		DeleteDC( g_hMemDC );
		DeleteObject( g_hBitmap );
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
		HANDLE hMemMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SYS_HEAP_BUFFER);
		if ( hMemMap == NULL ) return;

		long *ptr = (long *)MapViewOfFile(hMemMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if ( ptr == NULL ) return;

		long buffsize = ptr[0];
		if( buffsize != g_nGlobalHeapSize ){
			if( g_pGlobalHeap != NULL ){
				free( g_pGlobalHeap );
				g_pGlobalHeap = NULL;
				g_nGlobalHeapSize = 0;

				DeleteDC( g_hMemDC );
				DeleteObject( g_hBitmap );
			}
			g_pGlobalHeap = (void *)malloc( buffsize );
			if( g_pGlobalHeap != NULL ){
				g_nGlobalHeapSize = buffsize;

				g_hMemDC = CreateCompatibleDC(NULL);
//				g_hBitmap = CreateCompatibleBitmap(g_hMemDC, g_nClientWidth, g_nClientHeight);
				BITMAPINFO Bm;
				memset( &Bm, 0, sizeof(BITMAPINFO) );
				Bm.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				Bm.bmiHeader.biWidth = g_nClientWidth;
				Bm.bmiHeader.biHeight = g_nClientHeight;
				Bm.bmiHeader.biPlanes = 1;
				Bm.bmiHeader.biBitCount = 24;
				Bm.bmiHeader.biXPelsPerMeter = 0;
				Bm.bmiHeader.biYPelsPerMeter = 0;
				Bm.bmiHeader.biClrUsed = 0;
				Bm.bmiHeader.biClrImportant = 0;
				Bm.bmiHeader.biCompression = BI_RGB;
				Bm.bmiHeader.biSizeImage = g_nClientWidth * g_nClientHeight * 3;
				g_hBitmap = CreateDIBSection( g_hMemDC, (BITMAPINFO*)&Bm, DIB_RGB_COLORS, NULL, 0, 0 );
				SelectObject( g_hMemDC, g_hBitmap );
			}
		}

		if( g_pGlobalHeap != NULL ){
			memcpy( g_pGlobalHeap, (BYTE *)ptr + sizeof(long)*4, g_nGlobalHeapSize );
		}

		UnmapViewOfFile( ptr );
		CloseHandle( hMemMap );

		if( g_pGlobalHeap != NULL ){
			_FlushToScreen( (HDC)g_hMemDC, 0, 0, g_nClientWidth, g_nClientHeight );

			HDC hDC = GetDC(g_hWnd);
			APP_Entry_Draw( (void *)hDC );
			ReleaseDC(g_hWnd, hDC);
		}
	}
}


void APP_Entry_Resize( int width, int height )
{
	g_nClientWidth = width;
	g_nClientHeight = height;

//	_FlushToScreen( (HDC)hDC, 0, 0, g_nClientWidth, g_nClientHeight );
}


void APP_Entry_Draw( void *hDC )
{
	if( g_hMemDC != NULL ){
		BitBlt( (HDC)hDC, 0, 0, g_nClientWidth, g_nClientHeight, g_hMemDC, 0, 0, SRCCOPY );
	}
}


void APP_Entry_Debug(int param1, int param2)
{
}
