// winPC.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include <winnls.h>

 
#include "../../inc/EntryPtr.h"
#include "SYSAPI.h"


#define MAX_LOADSTRING 100
#define _USE_VLD_	0

// Kyle Test Code
#if _USE_VLD_
#pragma comment(lib, "vld-10/vld.lib")
#include "vld-10/vld.h"	// Visual Stuido Memory Leak Detector
#endif
// Kyle Test Code

static HWND g_hWnd = NULL;		// 현재 최상위 메인 윈도우 핸들
static HWND g_hSysVarWnd = NULL;
static HWND g_hUserKeyWnd = NULL;


#define _SCR_RESOLUTION_WIDTH	/*240*/ 320
#define _SCR_RESOLUTION_HEIGHT	/*320*/ 480


#define MAX_LOADSTRING 100

// Global Variables:
static HINSTANCE hInst;								// current instance

static TCHAR szTitle[256];				// The title bar text

static int g_scale = 1;
static int cur_screen_x, cur_screen_y;
static int g_debug_console = 0;
static int g_sysvar_editor = 0;
static int g_userkey_editor = 0;

//TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
static TCHAR szWindowClass[MAX_LOADSTRING] = _T("AUP_Launcher");								// The title bar text

// Foward declarations of functions included in this code module:
static ATOM				MyRegisterClass(HINSTANCE hInstance);
static BOOL				InitInstance(HINSTANCE, int);
static LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
static INT_PTR CALLBACK	SysVarDlgProc(HWND, UINT, WPARAM, LPARAM);
static INT_PTR CALLBACK	UserKeyDlgProc(HWND, UINT, WPARAM, LPARAM);



//int GetLastSnapNo();
//BOOL WriteSnapFiles(int snapNo);

/*
static void CaptureScreenToClipboard(void)
{
	atREGION rc;
	atDESKTOP_GetDesktopArea(atDESKTOP_AREA_ALL, &rc);

	int iW = rc.nWidth; //GD_GetFrameRect()->right - GD_GetFrameRect()->left;
	int iH = rc.nHeight; //GD_GetFrameRect()->bottom - GD_GetFrameRect()->top ;

	HWND hWnd = DebugGetOnwerWnd();
	if (hWnd == NULL)
		return;

	HDC hOrgDC = GetDC (hWnd); //GetDC(GD_GetFrameHwnd());//m_mapview->GetMemDC();

	HDC	 hdc = CreateCompatibleDC(hOrgDC);
	HBITMAP	hBmp = CreateCompatibleBitmap( hOrgDC, iW, iH) ;
	HBITMAP	hOldBitmap = (HBITMAP) SelectObject (hdc, hBmp);
	BitBlt(hdc, 0,0,iW, iH, hOrgDC, 0, 0, SRCCOPY);

	OpenClipboard(hWnd);
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, hBmp);
	CloseClipboard();

	SelectObject( hdc, hOldBitmap ) ;
	DeleteObject( hBmp ) ;
	DeleteDC( hdc ) ;
	ReleaseDC(hWnd, hOrgDC);
}

typedef struct tagBITMAPINFO_256 {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[256];
} BITMAPINFO_256;

static void CaptureScreenToFile(void)
{
	atREGION rc;
	atDESKTOP_GetDesktopArea(atDESKTOP_AREA_ALL, &rc);

	int iW = rc.nWidth; //GD_GetFrameRect()->right - GD_GetFrameRect()->left;
	int iH = rc.nHeight; //GD_GetFrameRect()->bottom - GD_GetFrameRect()->top ;

	HWND hWnd = DebugGetOnwerWnd();
	if (hWnd == NULL)
		return;

	HDC hOrgDC = GetDC (hWnd); //GetDC(GD_GetFrameHwnd());//m_mapview->GetMemDC();

	HDC	 hdc = CreateCompatibleDC(hOrgDC);
	HBITMAP	hBmp = CreateCompatibleBitmap( hOrgDC, iW, iH) ;
	HBITMAP	hOldBitmap = (HBITMAP) SelectObject (hdc, hBmp);
	BitBlt( hdc, 0, 0, iW, iH, hOrgDC, 0, 0, SRCCOPY );

	BITMAPINFO_256 bi;
	memset( &bi, 0, sizeof(BITMAPINFO_256) );
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    GetDIBits( hdc, hBmp, 0, 0, NULL, (LPBITMAPINFO)&bi, DIB_RGB_COLORS );

	//if( bi.bmiHeader.biBitCount == 32 ) bi.bmiHeader.biBitCount = 24;
    int nColors = (1 << bi.bmiHeader.biBitCount);
    if( nColors > 256 ) nColors = 0;

    if( bi.bmiHeader.biSizeImage == 0 ){
        bi.bmiHeader.biSizeImage = ((((bi.bmiHeader.biWidth * bi.bmiHeader.biBitCount) + 31) & ~31) / 8) * bi.bmiHeader.biHeight;
    }

	char *lpbmp = new char[ bi.bmiHeader.biSizeImage ];
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    int ln = GetDIBits( hdc, hBmp, 0, bi.bmiHeader.biHeight, (LPBYTE)lpbmp, (LPBITMAPINFO)&bi, DIB_RGB_COLORS );

	SelectObject( hdc, hOldBitmap ) ;
	DeleteObject( hBmp ) ;
	DeleteDC( hdc ) ;
	ReleaseDC(hWnd, hOrgDC);

	if( ln <= 0 ){
		delete [] lpbmp;
		return;
	}

    BITMAPFILEHEADER      hdr;
    hdr.bfType        = ((WORD) ('M' << 8) | 'B');        
    hdr.bfSize        = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColors + bi.bmiHeader.biSizeImage;
    hdr.bfReserved1   = 0;
    hdr.bfReserved2   = 0;
    hdr.bfOffBits	  = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColors;

	char filename[256];
	time_t t = time(atNULL);
	struct tm *tmr = localtime(&t);
	sprintf( filename, "capture_%04d%02d%02d_%02d%02d%02d.bmp", tmr->tm_year+1900, tmr->tm_mon+1, tmr->tm_mday, tmr->tm_hour, tmr->tm_min, tmr->tm_sec );
	
	FILE *fp = fopen( filename, "wb" );
	fwrite( &hdr, sizeof(BITMAPFILEHEADER), 1, fp );
	fwrite( &bi.bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp );
	fwrite( &bi.bmiColors, sizeof(RGBQUAD) * nColors, 1, fp );
	fwrite( lpbmp, bi.bmiHeader.biSizeImage, 1, fp );
	fclose( fp );
    
    delete [] lpbmp;

}
*/

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)

{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	ENTRYPTR_Initialize();

	char ttt[256];
	ENTRYPTR_GetName( ttt );
#ifdef UNICODE
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, ttt, strlen(ttt), szTitle, 256 );
#else
	strcpy( szTitle, ttt );
#endif

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_AUP_Launcher, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance);
	
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) {
		return FALSE;
	}

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AUP_Launcher));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	return msg.wParam;
}


//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
static ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AUP_Launcher));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_AUP_Launcher);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	RECT rc = {0, 0, _SCR_RESOLUTION_WIDTH, _SCR_RESOLUTION_HEIGHT};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE); // 풀다운 메뉴 영역 포함한 윈도우 크기

	g_hWnd = hWnd = CreateWindow(szWindowClass, szTitle, 
   						WS_OVERLAPPEDWINDOW,
      					CW_USEDEFAULT, CW_USEDEFAULT, 
						rc.right-rc.left, rc.bottom-rc.top,
      					nullptr, nullptr, hInstance, nullptr);

   if (!hWnd) return FALSE;

//   DebugSetOnwerWnd(hWnd);
//   DebugSetMsgFlag (1);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


static void SetWindowClientSize(HWND hWnd, int cx, int cy)
{
	RECT rc, rc_win;
	int  width, height;

	SYSAPI_ExtGraphicMethod( 0x8001, g_scale, 0, 0, 0, 0, 0 );
	
	GetWindowRect(hWnd, &rc_win);
	rc.left   = 0; 
	rc.right  = cx;
	rc.top    = 0;
	rc.bottom = cy;

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE); 

	width  = rc.right - rc.left;
	height = rc.bottom - rc.top;

	if(rc_win.top < 0) rc_win.top = 10;
	if(rc_win.left < 0)	rc_win.left = 10;

	MoveWindow(hWnd, rc_win.left, rc_win.top, width, height, TRUE);
}

static LRESULT OnOpen(HWND hWnd)
{
//	TCHAR str[32];
//	GetModuleFileName
//	GetPrivateProfileString( _T("Debug"), _T("DebugScreen"), _T("0"), str, 32, _T("Tornado.ini") );
}

static LRESULT OnClose(HWND hWnd)
{
//	TCHAR str[32];
//	WritePrivateProfileString( _T("Debug"), _T("DebugScreen"), str, _T("Tornado.ini") );
}

static LRESULT OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HMENU hMenu;

	switch(wParam)
	{
	case IDM_SCREEN_240x320 :
		SetWindowClientSize(hWnd, 240*g_scale, 320*g_scale); break;
	case IDM_SCREEN_320x240 :
		SetWindowClientSize(hWnd, 320*g_scale, 240*g_scale); break;
	case IDM_SCREEN_240x400 :
		SetWindowClientSize(hWnd, 240*g_scale, 400*g_scale); break;
	case IDM_SCREEN_400x240 :
		SetWindowClientSize(hWnd, 400*g_scale, 240*g_scale); break;
	case IDM_SCREEN_320x320 :
		SetWindowClientSize(hWnd, 320*g_scale, 320*g_scale); break;
	case IDM_SCREEN_320x480:
		SetWindowClientSize(hWnd, 320*g_scale, 480*g_scale); break;
	case IDM_SCREEN_480x272 :
		SetWindowClientSize(hWnd, 480*g_scale, 272*g_scale); break;
	case IDM_SCREEN_800x480 :
		SetWindowClientSize(hWnd, 800*g_scale, 480*g_scale); break;
	case IDM_SCREEN_SCALE_X1 :
		if( g_scale != 1 ){
			hMenu = GetMenu(hWnd);
			CheckMenuItem( hMenu, IDM_SCREEN_SCALE_X1, MF_CHECKED );
			CheckMenuItem( hMenu, IDM_SCREEN_SCALE_X2, MF_UNCHECKED );

			cur_screen_x = cur_screen_x / g_scale * 1;
			cur_screen_y = cur_screen_y / g_scale * 1;
			g_scale = 1;
			SetWindowClientSize(hWnd, cur_screen_x, cur_screen_y);
		}
		break;
	case IDM_SCREEN_SCALE_X2 :
		if( g_scale != 2 ){
			hMenu = GetMenu(hWnd);
			CheckMenuItem( hMenu, IDM_SCREEN_SCALE_X2, MF_CHECKED );
			CheckMenuItem( hMenu, IDM_SCREEN_SCALE_X1, MF_UNCHECKED );

			cur_screen_x = cur_screen_x / g_scale * 2;
			cur_screen_y = cur_screen_y / g_scale * 2;
			g_scale = 2;
			SetWindowClientSize(hWnd, cur_screen_x, cur_screen_y);
		}
		break;
	case IDM_VIEW_DEBUG:
		hMenu = GetMenu(hWnd);
		g_debug_console = !g_debug_console;
		if( g_debug_console )
			CheckMenuItem( hMenu, IDM_VIEW_DEBUG, MF_CHECKED );
		else
			CheckMenuItem( hMenu, IDM_VIEW_DEBUG, MF_UNCHECKED );
		ENTRYPTR_ActivateDebug( g_debug_console );
		break;
	case IDM_VIEW_SYSTEMVAR:
		hMenu = GetMenu(hWnd);
		g_sysvar_editor = !g_sysvar_editor;
		if( g_sysvar_editor )
			CheckMenuItem( hMenu, IDM_VIEW_SYSTEMVAR, MF_CHECKED );
		else
			CheckMenuItem( hMenu, IDM_VIEW_SYSTEMVAR, MF_UNCHECKED );

		if(g_sysvar_editor) {
			//g_hDiag = CreateDialog(hInst, MAKEINTRESOURCE(IDD_SYSVAR), hWnd, (DLGPROC) SysVarDlgProc);
			//g_hDiag = CreateDialog(hInst, MAKEINTRESOURCE(IDD_SYSVAR), GetDesktopWindow(), (DLGPROC) SysVarDlgProc);
			g_hSysVarWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_SYSVAR), hWnd, (DLGPROC) SysVarDlgProc);
			if(g_hSysVarWnd != NULL) {
				int sw, dw;
				RECT rect;
				GetWindowRect(g_hSysVarWnd, &rect);
				dw = rect.right - rect.left;
				sw = GetSystemMetrics(SM_CXSCREEN);
				GetWindowRect(hWnd, &rect);
				if(rect.right + dw > sw) {
					SetWindowPos(g_hSysVarWnd, HWND_TOP, rect.left - dw, rect.top, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
				} else {
					SetWindowPos(g_hSysVarWnd, HWND_TOP, rect.right, rect.top, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
				}
				//ShowWindow(g_hDiag, SW_SHOW);
			}
		} else {
			if(g_hSysVarWnd != NULL){
				DestroyWindow(g_hSysVarWnd);
				g_hSysVarWnd = NULL;
			}
		}
		break;
	case IDM_VIEW_SYSTEMVAR + 0x1000:
		hMenu = GetMenu(hWnd);
		g_sysvar_editor = 0;
		CheckMenuItem( hMenu, IDM_VIEW_SYSTEMVAR, MF_UNCHECKED );
		break;
	case IDM_VIEW_USERKEY:
		hMenu = GetMenu(hWnd);
		g_userkey_editor = !g_userkey_editor;
		if( g_userkey_editor )
			CheckMenuItem( hMenu, IDM_VIEW_USERKEY, MF_CHECKED );
		else
			CheckMenuItem( hMenu, IDM_VIEW_USERKEY, MF_UNCHECKED );

		if(g_userkey_editor) {
			//g_hDiag = CreateDialog(hInst, MAKEINTRESOURCE(IDD_SYSVAR), hWnd, (DLGPROC) SysVarDlgProc);
			//g_hDiag = CreateDialog(hInst, MAKEINTRESOURCE(IDD_SYSVAR), GetDesktopWindow(), (DLGPROC) SysVarDlgProc);
			g_hUserKeyWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_USERKEY), GetDesktopWindow(), (DLGPROC) UserKeyDlgProc);
			if(g_hUserKeyWnd != NULL) {
				int sw, dw;
				RECT rect;
				GetWindowRect(g_hUserKeyWnd, &rect);
				dw = rect.right - rect.left;
				sw = GetSystemMetrics(SM_CXSCREEN);
				GetWindowRect(hWnd, &rect);
				if(rect.right + dw > sw) {
					SetWindowPos(g_hUserKeyWnd, HWND_TOP, rect.left - dw, rect.top, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
				} else {
					SetWindowPos(g_hUserKeyWnd, HWND_TOP, rect.right, rect.top, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
				}
				//ShowWindow(g_hDiag, SW_SHOW);
			}
		} else {
			if(g_hUserKeyWnd != NULL){
				DestroyWindow(g_hUserKeyWnd);
				g_hUserKeyWnd = NULL;
			}
		}
		break;
	case IDM_VIEW_USERKEY + 0x1000:
		hMenu = GetMenu(hWnd);
		g_sysvar_editor = 0;
		CheckMenuItem( hMenu, IDM_VIEW_USERKEY, MF_UNCHECKED );
		break;
	case IDM_BMP:
		{
//			WriteSnapFiles(GetLastSnapNo());
			break;
		}
	}

	return 0;
}


/*
int GetLastSnapNo()
{
	atBOOL	ff = atFALSE;
	TCHAR fname[256]={0,};
	SYS_FINDFILEENTRY	fe;
	int i=0;

	SYSAPI_tsprintf(fname,_AT("%s"),GD_GetPathName(FT_DATA, _T("Snaps")));
	ff = SYSAPI_FindFirstFile( fname, _AT("*.bmp"), &fe );

	if(ff) i++;

	while( ff )
	{
		if( !SYSAPI_tstrcmp( fe.szFileName, _AT(".") )
			|| !SYSAPI_tstrcmp( fe.szFileName, _AT("..") ) )
		{
			ff = SYSAPI_FindNextFile( &fe );
			continue;
		}

		ff = SYSAPI_FindNextFile( &fe );
		if(ff)
		{
			i++;
		}
	}

	return i;
}

BOOL WriteSnapFiles(int snapNo)
{
	TCHAR szPath[MAX_PATH];

	if (GetFileAttributes(GD_GetPathName(FT_DATA, _T("Snaps"))) == -1L)
		CreateDirectory( GD_GetPathName(FT_DATA, _T("Snaps")), NULL);

	_tcscpyn(szPath, COUNTOF(szPath), GD_GetPathName(FT_DATA, _T("Snaps\\%05d.BMP"), snapNo));

	//DC내용을 bmp 파일로 저장됨.
	CaptureCanvasToFile(atWINDOW_GetClientGDC(GetMainHwnd()),szPath);

	return TRUE;
}
*/


//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (message) 
	{
	case WM_CREATE:
		{
			void * appui = (void *)hInst;
			void * appview = (void *)hWnd;

			cur_screen_x = _SCR_RESOLUTION_WIDTH;
			cur_screen_y = _SCR_RESOLUTION_HEIGHT;

//			if(!InitAUP( cur_screen_x, cur_screen_y, appview, appui ))
			if( !ENTRYPTR_Create( appui, appview, cur_screen_x, cur_screen_y ) )
			{
				PostMessage(hWnd, WM_CLOSE, 0,0);
			}
		}
		break;

	case WM_SHOWWINDOW:
		ENTRYPTR_Enter();
		break;

	case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hWnd, &rect);
			cur_screen_x = rect.right-rect.left;
			cur_screen_y = rect.bottom-rect.top;
//			atUIENG_OnResize( cur_screen_x/g_scale, cur_screen_y/g_scale ); 
//			atUIAPI_FlushScreen();
			ENTRYPTR_Resize( cur_screen_x/g_scale, cur_screen_y/g_scale );
		}
		break;

	case WM_CLOSE:
		ENTRYPTR_Exit();
		DestroyWindow(hWnd);
		break;

	case WM_PAINT:
		BeginPaint( hWnd, &ps );
		ENTRYPTR_Draw();
		EndPaint( hWnd, &ps );
		break;

	case WM_DESTROY:
		ENTRYPTR_Destroy();
		PostQuitMessage(0);
		//break;
		return 1;

	case WM_KEYDOWN:
		switch( wParam )
		{
			case VK_F1:
//				CaptureScreenToClipboard();
				break;
			case VK_F2:
//				CaptureScreenToFile();
				break;
		}
		break;
		
	case WM_COMMAND:
		OnCommand(hWnd, wParam, lParam);
		break;

/*
#if (_IS_PC_ && USE_DEBUG_MSG)
	case WM_COPYDATA:
		{
			PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT) (lParam);
			if (pcds->dwData == COPYDATA_KEY_SET_VIEWCONFIG){

				if (g_iGetMessageFlag)
				{
					PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT) (lParam);
					CViewConfigUnit oUnit;
					memcpy (&oUnit, pcds->lpData, pcds->cbData);
					
					if ((oUnit.hWnd != hWnd) && (pcds->dwData == COPYDATA_KEY_SET_VIEWCONFIG))
					{

						CMV* pMV = NULL;
						CMainLogic* pMainL = NULL;
						if (g_pMainWnd)
							pMainL = g_pMainWnd->GetMainLogic();
						if (pMainL)
							pMV = pMainL->GetCMV();
						if (pMV)
						{
							atBOOL bImm = pMV->MV_GetImmovable();
							if (bImm == atFALSE)
								pMainL->SetImmovable(atTRUE);
							
							pMV->MV_SetBrodCast(0);
							pMV->MV_SetMapRegion( oUnit.dx, oUnit.dy, oUnit.ds );
							pMV->MV_SetBrodCast(1);
							pMainL->MapDrawInvalidate();
							g_pMainWnd->RedrawAllWindows();
						}
					}
				}
			}
			else if(pcds->dwData == COPYDATA_GPS_LOG)
			{
				MM_StartThread();
				GPS_Process((const char*)pcds->lpData, pcds->cbData);
			}
		}
		break;
#endif
*/
		
	default:
		//	atAPP_PostMessage(atAPPMNG_GetCurApp(), hWnd, message, wParam, lParam);
		return DefWindowProc(hWnd, message, wParam, lParam);
   }

   return 0;
}


static LRESULT OnSysVarDlgCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	TCHAR str[128];
	int syscode, sysvalue;
	static int codes[3] = {IDE_SYSVAR_CODE_1, IDE_SYSVAR_CODE_2, IDE_SYSVAR_CODE_3};
	static int values[3] = {IDE_SYSVAR_VALUE_1, IDE_SYSVAR_VALUE_2, IDE_SYSVAR_VALUE_3};

	switch(wParam)
	{
	case IDCLOSE :
		DestroyWindow(hWnd);
		break;
	case IDB_READ :
		for(int i = 0; i < 3; i++ ){
			GetDlgItemText(hWnd, codes[i], str, 128);
			if(str[0] != _T('\0')){
				syscode = _ttoi(str);
				sysvalue = ENTRYPTR_GetSystemVar(syscode);
				_itot_s(sysvalue, str, 10);
				SetDlgItemText(hWnd, values[i], str);
			} else {
				SetDlgItemText(hWnd, values[i], _T(""));
			}
		}
		break;
	case IDB_WRITE :
		for(int i = 0; i < 3; i++ ){
			GetDlgItemText(hWnd, codes[i], str, 128);
			if(str[0] != _T('\0')){
				syscode = _ttoi(str);
				GetDlgItemText(hWnd, values[i], str, 128);
				sysvalue = _ttoi(str);
				sysvalue = ENTRYPTR_SysCommand(syscode, sysvalue);
				_itot_s(sysvalue, str, 10);
				SetDlgItemText(hWnd, values[i], str);
			} else {
				SetDlgItemText(hWnd, values[i], _T(""));
			}
		}
		break;
	}

	return 0;
}

static INT_PTR CALLBACK SysVarDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		OnSysVarDlgCommand( hWnd, wParam, lParam);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_NCDESTROY:
		PostMessage(g_hWnd, WM_COMMAND, IDM_VIEW_SYSTEMVAR + 0x1000, 0);
		break;
	}

	return FALSE;
}


static LRESULT OnUserKeyDlgCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	TCHAR str[128];
	int keycode;
	static int codes[5] = {IDE_USERKEY_1, IDE_USERKEY_2, IDE_USERKEY_3, IDE_USERKEY_4, IDE_USERKEY_5};
	static int buttons[5] = {IDB_BUTTON1, IDB_BUTTON2, IDB_BUTTON3, IDB_BUTTON4, IDB_BUTTON5};

	switch(wParam)
	{
	case IDCLOSE :
		DestroyWindow(hWnd);
		break;
	case IDB_BUTTON1 :
	case IDB_BUTTON2 :
	case IDB_BUTTON3 :
	case IDB_BUTTON4 :
	case IDB_BUTTON5 :
		for(int i = 0; i < 5; i++ ){
			if(wParam == buttons[i]) {
				GetDlgItemText(hWnd, codes[i], str, 128);
				if(str[0] != _T('\0')){
					keycode = _ttoi(str);
					//sysvalue = ENTRYPTR_KeyDown(sysvalue);
				}
				break;
			}
		}
		break;
	}

	return 0;
}

static INT_PTR CALLBACK UserKeyDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_INITDIALOG:
		SetDlgItemText(hWnd, IDE_USERKEY_BASE, _T("0x1000"));
		return TRUE;
	case WM_COMMAND:
		OnUserKeyDlgCommand( hWnd, wParam, lParam);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_NCDESTROY:
		PostMessage(g_hWnd, WM_COMMAND, IDM_VIEW_USERKEY + 0x1000, 0);
		break;
	}

	return FALSE;
}
