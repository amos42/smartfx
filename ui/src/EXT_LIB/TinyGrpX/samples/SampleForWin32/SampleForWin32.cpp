// SampleForWin32.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "SampleForWin32.h"
#include "TinyGrpX.h"


typedef struct tagBITMAPINFO16 {
	BITMAPINFOHEADER    bmiHeader;
	RGBQUAD             bmiColors[4];
} BITMAPINFO16, FAR *LPBITMAPINFO16, *PBITMAPINFO16;

static void _FlushToScreen(HDC hDC, char *framebuff, int bufwidth, int bpp, int sx, int sy, int width, int height, int scale)
{
	HDC hmemdc;
	HBITMAP bitmap;
	BITMAPINFO16	Bm;
	BYTE*			pBits;

	if (framebuff == atNULL) return;

	hmemdc = CreateCompatibleDC(hDC);
	if (hmemdc == atNULL) return;

	pBits = (BYTE *)framebuff;

	memset(&Bm, 0, sizeof(BITMAPINFO16));
	Bm.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	Bm.bmiHeader.biWidth = bufwidth;
	Bm.bmiHeader.biHeight = -height;
	Bm.bmiHeader.biPlanes = 1;
	Bm.bmiHeader.biBitCount = bpp;
	Bm.bmiHeader.biXPelsPerMeter = 0;
	Bm.bmiHeader.biYPelsPerMeter = 0;
	Bm.bmiHeader.biClrUsed = 0;
	Bm.bmiHeader.biClrImportant = 0;

	switch (Bm.bmiHeader.biBitCount)
	{
	case 24:
		Bm.bmiHeader.biCompression = BI_RGB;
		Bm.bmiHeader.biSizeImage = bufwidth * height * 3;
		break;
	case 16:
		Bm.bmiHeader.biCompression = BI_BITFIELDS;
		Bm.bmiHeader.biSizeImage = bufwidth * height * 2;

		//11111000 00011111
		*(DWORD*)(Bm.bmiColors + 0) = 0x0000f800;
		*(DWORD*)(Bm.bmiColors + 1) = 0x000007e0;
		*(DWORD*)(Bm.bmiColors + 2) = 0x0000001f;
		break;
	}

	//	SYSAPI_EnterCriticalSection( g_SYS_ScrCrSection );
	bitmap = CreateDIBitmap(hDC, &Bm.bmiHeader, CBM_INIT, pBits, (BITMAPINFO *)&Bm, DIB_RGB_COLORS);
	//	SYSAPI_LeaveCriticalSection( g_SYS_ScrCrSection );

	if (bitmap) {
		HBITMAP oldbitmap = (HBITMAP)SelectObject(hmemdc, bitmap);
		if (scale == 1)
			BitBlt(hDC, sx, sy, width, height, hmemdc, sx, sy, SRCCOPY);
		else
			StretchBlt(hDC, sx*scale, sy*scale, width*scale, height*scale, hmemdc, sx, sy, width, height, SRCCOPY);
		SelectObject(hmemdc, oldbitmap);
		DeleteObject(bitmap);
	}

	DeleteDC(hmemdc);
}



#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SAMPLEFORWIN32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SAMPLEFORWIN32));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SAMPLEFORWIN32));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SAMPLEFORWIN32);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}






static short framebuf[640 * 480];
static atGRPXMNG grpx;
static atTGRPX_CANVAS canvas;

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
			atTGRPX_Initialize(&grpx, 640, 480, 16, framebuf, 640);
			for (int i = 0; i < 480; i++)
				for (int j = 0; j < 640; j++)
					framebuf[i*640+j] = atTGRPX_GetRGB(0, 0, i * 256 / 480);
			atTGRPX_InitCanvas(&canvas, &grpx, 0, 0, 640, 480);
			atTGRPX_DrawRect(&canvas, 20, 20, 640 - 20, 480 - 20, atTGRPX_GetRGB(255, 255, 255));
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

			_FlushToScreen(hdc, (char *)framebuf, 640, 16, 0, 0, 640, 480, 1);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
