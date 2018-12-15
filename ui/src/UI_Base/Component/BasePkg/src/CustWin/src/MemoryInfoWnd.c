//
// MemoryInfoWnd.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_Util.h"
#include "MemoryInfoWnd.h"


//#define USES_DALLOC


/*
typedef struct _MEMORYINFOWNDDATA {

} MEMORYINFOWNDDATA, * LPMEMORYINFOWNDDATA;
*/

static int MemoryInfoWndOnEvent(atHWINDOW hWnd, int nType, long nParm1, long nParm2);


atBOOL      RegistMemoryInfoWndClass(void)
{
	atWINCLASS wc;
	
	atUIAPI_memset( &wc, 0, sizeof(atWINCLASS) );
	wc.dwAttr = atWS_FRAMEWINDOW | atWS_MODAL;
 	wc.dwUserDataSize = 0/*sizeof(MEMORYINFOWNDDATA)*/;
	wc.fnWndProc = MemoryInfoWndOnEvent;
	
	return atAPP_RegistWinClass( atAPPMNG_GetCurApp(), BASEPKG_CUSTWIN_MEMORYINFO, &wc );
}

atVOID      UnregistMemoryInfoWndClass(void)
{
	atAPP_UnregistWinClass( atAPPMNG_GetCurApp(), BASEPKG_CUSTWIN_MEMORYINFO );
}

atHWINDOW	CreateMemoryInfoWnd(atHWINDOW hParentWnd)
{
	atHWINDOW		hWnd;
	atMARGIN anchor;

	SET_MARGIN( anchor, 0, 0, 0, 0 );
	hWnd	= atAPP_CreateWindowByClass( atAPPMNG_GetCurApp(), hParentWnd, 0, _AT("Memory Info"),
								0, 0, &anchor,
								BASEPKG_CUSTWIN_MEMORYINFO, atNULL);
	atAPP_ShowWindow( atAPPMNG_GetCurApp(), hWnd, atTRUE );

	return hWnd;
}

static void MemoryInfoWndOnInit(atHWINDOW hWnd)
{
//	LPMEMORYINFOWNDDATA lpData = (LPMEMORYINFOWNDDATA)atWINDOW_GetDataPtr(LPMEMORYINFOWNDDATA, hWnd);

	atWINDOW_SetSoftKey( hWnd, _AT(""), _AT("OK"), _AT("") );
}

static void MemoryInfoWndOnRelease(atHWINDOW hWnd)
{
//	LPMEMORYINFOWNDDATA lpData = (LPMEMORYINFOWNDDATA)atWINDOW_GetDataPtr(LPMEMORYINFOWNDDATA, hWnd);
}

static int MemoryInfoWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	switch(nKeyCode)
	{
		case atVKEY_SELECT:			
		case atVKEY_MENU:			
		case atVKEY_CLEAR:			
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );		
			break;
		default:
			return 0;
	}

	return 1;
}

#define RECT_SIZE 3
#define BLK_SIZE (1024*4)

struct _draw_heap_info {
	atHGDC hDC;
	int width, height;
};

static void func( atLPVOID lpParam, void *addr, int pos, int size, int type )
{
	int i, sx, sy;
	atCOLOR cl[3];
	struct _draw_heap_info *p = (struct _draw_heap_info *)lpParam;
	
	cl[0] = atUIAPI_GetRGB(255, 0, 0);
	cl[1] = atUIAPI_GetRGB(168, 168, 168);
	cl[2] = atUIAPI_GetRGB(64, 128, 255);

//	sx = pos / BLK_SIZE % ((240-10*2)/RECT_SIZE);
//	sy = pos / BLK_SIZE / ((240-10*2)/RECT_SIZE);
	sx = pos / BLK_SIZE % ((p->width-10*2)/RECT_SIZE);
	sy = pos / BLK_SIZE / ((p->height-10*2)/RECT_SIZE);

	size /= BLK_SIZE;
	if( size <= 0 ) size = 1;

	for( i = 0; i < size; i ++ ){
		atUIAPI_FillRect( p->hDC, sx*RECT_SIZE+10-3, sy*RECT_SIZE+60-24-27, RECT_SIZE, RECT_SIZE,  cl[type] );
		atUIAPI_DrawRect( p->hDC, sx*RECT_SIZE+10-3, sy*RECT_SIZE+60-24-27, RECT_SIZE+1, RECT_SIZE+1,  UI_COLOR_BLACK, 1 );
		sx ++;
		if( sx >= (p->width-10*2)/RECT_SIZE ){ sx = 0; sy ++; }
	}
}

static void MemoryInfoWndOnPaint(atHWINDOW hWnd)
{
//#ifdef USES_DALLOC
//	LPMEMORYINFOWNDDATA lpData = (LPMEMORYINFOWNDDATA)atWINDOW_GetDataPtr(hWnd);
	atHGDC hCanvas = atWINDOW_GetClientGDC(hWnd);
	atREGION area;
	atTCHAR str[64];

//	ClearAll(UI_COLOR_WHITE);
	atUIAPI_GetGDCArea( hCanvas, &area );

	if( atUIAPI_CheckValidateHeap() ){
		struct _draw_heap_info p;
		p.hDC = hCanvas;
		p.width = area.nWidth;
		p.height = area.nHeight;
		atUIAPI_GetHeapInfo( (void *)&p, func );
	} else {
		atUIAPI_SetFont( hCanvas, atUIAPI_GetStockFont(1) );
		atUIAPI_DrawStringAlign(hCanvas, area.nWidth/2, area.nHeight/2, _AT("Memory Error!"), ALIGN_CENTER_MIDDLE, UI_COLOR_BLACK);
		return;
	}

	UI_SPRINTF( str, _AT("BlkSize:%d  FreeSize:%d"), BLK_SIZE, atUIAPI_GetTotalLeftHeapSize() );
	atUIAPI_SetFont( hCanvas, atUIAPI_GetStockFont(3) );
	atUIAPI_DrawStringAlign(hCanvas, 4, area.nHeight-2, str, ALIGN_LEFT_BOTTOM, UI_COLOR_BLACK);
//#endif
}

static int MemoryInfoWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	switch( nMsg )
	{
		case WNDMSG_INIT:
			MemoryInfoWndOnInit( hWnd );
			break;
		case WNDMSG_RELEASE:
			MemoryInfoWndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			MemoryInfoWndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return MemoryInfoWndOnKeyDown( hWnd, nParam1 );
			break;
		default:
			return 0;
			break;
	}

	return 1;
}

