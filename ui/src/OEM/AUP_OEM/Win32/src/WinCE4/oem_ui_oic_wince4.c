#include "A_UIENG.h"
#include "OEM_UI_OIC.h"
#include <windows.h>
#include <tchar.h>
#include "SYSAPI.h"


extern HINSTANCE g_UI_hInstance;
extern HWND g_UI_hWindow;

typedef LRESULT (CALLBACK FAR TWndProc)( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
static TWndProc *g_UI_WndProc_Original = atNULL;

static OEM_UI_OIC zzz[100];
static atUI_FUNC_OIC_NOTIFY* ttt[100];
static void* ttt2[100];


static int find_free_id(void)
{
	int i;

	for( i = 0; i < 100; i++ ){
		if( zzz[i] == NULL ) return i;
	}

	return -1;
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;
	int id, msg;
	atUI_FUNC_OIC_NOTIFY *pFunc;

	switch (message) {
		case WM_COMMAND:
			id = wParam & 0xffff;
			msg = wParam >> 16; 
			if( zzz[id] != NULL ){
				if( msg == EN_CHANGE ){
					pFunc = ttt[id];
					if( pFunc ) pFunc( ttt2[id], atUI_OICMSG_CHANGE, 0, 0 );
					r = 1;
				}
			}
			break;

		case WM_CTLCOLOREDIT:
			{
			char sClass[256];
			HWND hwnd   = (HWND)lParam;
			HDC  hdc  = (HDC)wParam;
			HBRUSH brush = GetStockObject(WHITE_BRUSH);
			GetClassName( hwnd, sClass, 255 );
		       if( stricmp( sClass, "EDIT") == 0 ){
			    SetTextColor( hdc, RGB(255,0,0));
			    SetBkColor(   hdc, RGB(255,255,0));
			    // WM_CTLCOLORxxx 메세지를 처리한 경우는 반드시 control배경에 칠할 Brush 한개를 리턴해주어야한다.
			    return (LRESULT)brush;
			}
	}

	if( !r ){
		r = CallWindowProc( g_UI_WndProc_Original, hWnd, message, wParam, lParam );
	}

	return r;
}


atBOOL OEM_UI_InitOIC( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( (void *)lSysInst == atNULL ) return atFALSE;

	g_UI_WndProc_Original = (TWndProc *)GetWindowLong( g_UI_hWindow, GWL_WNDPROC );
	SetWindowLong( g_UI_hWindow, GWL_WNDPROC, (LONG)WndProc );

	return atTRUE;
}


void OEM_UI_FinalOIC(void)
{
	SetWindowLong( g_UI_hWindow, GWL_WNDPROC, (LONG)g_UI_WndProc_Original );
	g_UI_WndProc_Original = atNULL;
}


atBOOL OEM_UI_IsSupportedOIC()
{
	return atTRUE;
}


typedef struct _invoke_params {
	OEM_UI_OIC hOemOIC;
	int funcno;
	long param1, param2;
} invoke_params;

static int _xxx( long param )
{
	HWND hEditBox;
	LOGFONT lf;
	HFONT hFont;
	RECT rect;
	POINT pt;
	int len;
	int id;
	invoke_params *params = (invoke_params *)param;
	
	switch( params->funcno ){
		case 1 :id = find_free_id();
				if( id == -1 ) return (int)NULL;
				hEditBox = CreateWindow( __TEXT("edit"), __TEXT(""), WS_CHILD, 0, 0, 50, 20, g_UI_hWindow, (HMENU)id, GetWindowLong(g_UI_hWindow, GWL_HINSTANCE), NULL );
				SendMessage( hEditBox, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELONG( 2, 2 ) );
				zzz[id] = hEditBox;
				ttt[id] = (atUI_FUNC_OIC_NOTIFY *)params->param1;
				ttt2[id] = (void *)params->param2; 
				return (int)hEditBox;
		case 2 :id = (int)GetWindowLong( (HWND)params->hOemOIC, GWL_ID );
				zzz[id] = NULL; 
				hFont = (HFONT)SendMessage( (HWND)params->hOemOIC, WM_GETFONT, 0, 0 );	
				if( hFont ) DeleteObject( hFont );
				DestroyWindow( (HWND)params->hOemOIC );
				break;
		case 3 : hFont = (HFONT)SendMessage( (HWND)params->hOemOIC, WM_GETFONT, 0, 0 );	
				if( hFont != NULL ){
					GetObject ( hFont, sizeof(LOGFONT), &lf );
					if( lf.lfHeight != params->param1 ){
						DeleteObject( hFont );				
						hFont = NULL;
					}
				}
				if( hFont != NULL ) break;

				lf.lfHeight = params->param1;
				lf.lfWidth = 0;
				lf.lfEscapement = 0;
				lf.lfOrientation = 0;
				lf.lfWeight = FW_BOLD;
				lf.lfItalic = FALSE;
				lf.lfUnderline = FALSE;
				lf.lfStrikeOut = FALSE;
				lf.lfCharSet = DEFAULT_CHARSET;
				lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
				lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
				lf.lfQuality = DEFAULT_QUALITY;
				lf.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
				strcpy(lf.lfFaceName, "맑은 고딕");
				hFont = CreateFontIndirect( &lf );
				if( hFont != NULL )
					SendMessage( (HWND)params->hOemOIC, WM_SETFONT, (WPARAM)hFont, (LPARAM)FALSE );
				break;		
		case 4 :	GetWindowRect((HWND)params->hOemOIC, &rect);
				pt.x = rect.left; pt.y = rect.top;
				ScreenToClient (g_UI_hWindow, &pt);
				params->param1 = pt.x;
				params->param2 = pt.y;
				break;
		case 5 :	GetWindowRect((HWND)params->hOemOIC, &rect);
				params->param1 = rect.right - rect.left;
				params->param2 = rect.bottom - rect.top;
				break;
		case 6 :	GetWindowRect((HWND)params->hOemOIC, &rect);
				pt.x = rect.left; pt.y = rect.top;
				ScreenToClient (g_UI_hWindow, &pt);
  				MoveWindow( (HWND)params->hOemOIC, pt.x, pt.y, params->param1, params->param2, TRUE );
				break;
		case 7 :	GetWindowRect((HWND)params->hOemOIC, &rect);
//				ScreenToClient (g_UI_hWindow, &rect);
  				MoveWindow( (HWND)params->hOemOIC, params->param1, params->param2, rect.right-rect.left, rect.bottom-rect.top, TRUE );
				break;
		case 8 :	ShowWindow( (HWND)params->hOemOIC, SW_SHOW );
				SetFocus( (HWND)params->hOemOIC );
				break;
		case 9 :	ShowWindow( (HWND)params->hOemOIC, SW_HIDE );
				break;
		case 10:	InvalidateRect( (HWND)params->hOemOIC, NULL, FALSE);
				break;
		case 11:	SendMessage( (HWND)params->hOemOIC, WM_SETTEXT, params->param2, (LPARAM)params->param1 );
				break;
		case 12:	len = SendMessage( (HWND)params->hOemOIC,WM_GETTEXTLENGTH, 0, 0 );
				if( len > params->param2 ) len = params->param2;
				SendMessage( (HWND)params->hOemOIC, WM_GETTEXT, len+1, (LPARAM)params->param1 );			
				break;	
	}

	return 0;
 }

OEM_UI_OIC OEM_UI_CreateOIC( int x, int y, int width, int height, atUI_FUNC_OIC_NOTIFY *fnFunc, void *param )
{
	invoke_params params;
	params.funcno = 1;
	params.param1 = (long)fnFunc;
	params.param2 = (long)param;
	params.hOemOIC = (OEM_UI_OIC)SYSAPI_InvokeSysGUI( _xxx, (long)&params );
	if( params.hOemOIC == NULL ) return atNULL;
	OEM_UI_SetOICSize( params.hOemOIC, width, height );
	OEM_UI_MoveOIC( params.hOemOIC, x, y );
	return params.hOemOIC;
}

void OEM_UI_DestroyOIC( OEM_UI_OIC hOemOIC )
{
	invoke_params params;
	params.hOemOIC = hOemOIC;
	params.funcno = 2;
	SYSAPI_InvokeSysGUI( _xxx, (long)&params );	
}

void OEM_UI_SetOICFont( OEM_UI_OIC hOemOIC, int font_size )
{
	invoke_params params;
	params.hOemOIC = hOemOIC;
	params.funcno = 3;
	params.param1 = font_size;
	SYSAPI_InvokeSysGUI( _xxx, (long)&params );	
}

void OEM_UI_GetOICPos( OEM_UI_OIC hOemOIC, int *x, int *y )
{
	invoke_params params;
	params.hOemOIC = hOemOIC;
	params.funcno = 4;
	SYSAPI_InvokeSysGUI( _xxx, (long)&params );	
	*x = params.param1;
	*y = params.param2;
}

void OEM_UI_GetOICSize( OEM_UI_OIC hOemOIC, int *width, int *height )
{
	invoke_params params;
	params.hOemOIC = hOemOIC;
	params.funcno = 5;
	SYSAPI_InvokeSysGUI( _xxx, (long)&params );	
	*width = params.param1;
	*height = params.param2;
}

void OEM_UI_SetOICSize( OEM_UI_OIC hOemOIC, int width, int height )
{
	invoke_params params;
	params.hOemOIC = hOemOIC;
	params.funcno = 6;
	params.param1 = width;
	params.param2 = height;
	SYSAPI_InvokeSysGUI( _xxx, (long)&params );	
}

void OEM_UI_MoveOIC( OEM_UI_OIC hOemOIC, int x, int y )
{
	invoke_params params;
	params.hOemOIC = hOemOIC;
	params.funcno = 7;
	params.param1 = x;
	params.param2 = y;
	SYSAPI_InvokeSysGUI( _xxx, (long)&params );	
}

void OEM_UI_ShowAndSetFocusOIC( OEM_UI_OIC hOemOIC )
{
	invoke_params params;
	params.hOemOIC = hOemOIC;
	params.funcno = 8;
	SYSAPI_InvokeSysGUI( _xxx, (long)&params );	
}

void OEM_UI_HideAndLostFocusOIC( OEM_UI_OIC hOemOIC )
{
	invoke_params params;
	params.hOemOIC = hOemOIC;
	params.funcno = 9;
	SYSAPI_InvokeSysGUI( _xxx, (long)&params );	
}

void OEM_UI_RefreshOIC( OEM_UI_OIC hOemOIC )
{
	invoke_params params;
	params.hOemOIC = hOemOIC;
	params.funcno = 10;
	SYSAPI_InvokeSysGUI( _xxx, (long)&params );	
}

void OEM_UI_SetOICText( OEM_UI_OIC hOemOIC, atLPCTSTR szText, atINT nLen )
{
	invoke_params params;
	params.hOemOIC = hOemOIC;
	params.funcno = 11;
	params.param1 = (long)szText;
	params.param2 = nLen;
	SYSAPI_InvokeSysGUI( _xxx, (long)&params );	
}

void OEM_UI_GetOICText( OEM_UI_OIC hOemOIC, atLPTSTR szText, atINT nLen )
{
	invoke_params params;
	params.hOemOIC = hOemOIC;
	params.funcno = 12;
	params.param1 = (long)szText;
	params.param2 = nLen;
	SYSAPI_InvokeSysGUI( _xxx, (long)&params );	
}

