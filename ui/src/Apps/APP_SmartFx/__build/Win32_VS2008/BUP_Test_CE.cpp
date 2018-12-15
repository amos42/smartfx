// BUP_Test.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "BUP_Test_CE.h"

#include "BUP.h"
#include "APP_Entry.h"

#define _SCR_RESOLUTION_WIDTH	(240)
#define _SCR_RESOLUTION_HEIGHT	(320)


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE			g_hInst;			// 현재 인스턴스입니다.
HWND				g_hWndMenuBar;		// 메뉴 모음 핸들입니다.

static int g_screen_width, g_screen_height;


// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	MSG msg;

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BUP_TEST));

	// 기본 메시지 루프입니다.
	while (GetMessage(&msg, NULL, 0, 0)) 
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
//  목적: 창 클래스를 등록합니다.
//
//  설명:
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BUP_TEST));
	wc.hCursor       = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
    TCHAR szTitle[MAX_LOADSTRING];		// 제목 표시줄 텍스트
    TCHAR szWindowClass[MAX_LOADSTRING];	// 주 창 클래스 이름

    g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    // CAPEDIT 및 SIPPREF와 같은 모든 장치 특수 컨트롤을 초기화하기 위해
    // 응용 프로그램을 초기화하는 동안 SHInitExtraControls를 한 번 호출해야 합니다.
    //SHInitExtraControls();

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_BUP_TEST, szWindowClass, MAX_LOADSTRING);

    //이미 실행 중인 경우 창에 포커스를 둔 다음 끝냅니다.
    hWnd = FindWindow(szWindowClass, szTitle);	
    if (hWnd) 
    {
        // 맨 앞 자식 창에 포커스를 설정합니다.
        // "| 0x00000001"을 사용하여 소유한 모든 창을 전경으로 보낸 다음
        // 활성화합니다.
        SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));
        return 0;
    } 

    if (!MyRegisterClass(hInstance, szWindowClass))
    {
    	return FALSE;
    }

#ifdef __WCE_EMULATOR_
	RECT rc = {0, 0, 240, 320};
	//AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE);
	g_screen_width = rc.right-rc.left;
	g_screen_height = rc.bottom-rc.top;
#else
	g_screen_width = GetSystemMetrics(SM_CXSCREEN);
	g_screen_height = GetSystemMetrics(SM_CYSCREEN);
#endif

    hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
        0, 0, g_screen_width, g_screen_height, HWND_DESKTOP, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

/*
    // CW_USEDEFAULT를 사용하여 주 창을 만드는 경우 메뉴 모음의 높이는
    // 고려되지 않습니다. 따라서 메뉴 모음이 있는 경우
    // 창을 만든 후 창의 크기를 조정합니다.
    if (g_hWndMenuBar)
    {
        RECT rc;
        RECT rcMenuBar;

        GetWindowRect(hWnd, &rc);
        GetWindowRect(g_hWndMenuBar, &rcMenuBar);
        rc.bottom -= (rcMenuBar.bottom - rcMenuBar.top);
		
        MoveWindow(hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
    }
*/

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

#ifndef __WCE_EMULATOR_
   SHFullScreen( hWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON );
#endif

   SetForegroundWindow( hWnd );

    return TRUE;
}



//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
	RECT rect;
    PAINTSTRUCT ps;
    HDC hdc;

    static SHACTIVATEINFO s_sai;
	
    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
            // 메뉴의 선택 영역을 구문 분석합니다.
            switch (wmId)
            {
                case IDM_HELP_ABOUT:
                    break;
                case IDM_OK:
                    SendMessage (hWnd, WM_CLOSE, 0, 0);				
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;

        case WM_CREATE:
/*
            SHMENUBARINFO mbi;

            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize     = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hWnd;
            mbi.nToolBarId = IDR_MENU;
            mbi.hInstRes   = g_hInst;

            if (!SHCreateMenuBar(&mbi)) 
            {
                g_hWndMenuBar = NULL;
            }
            else
            {
                g_hWndMenuBar = mbi.hwndMB;
            }

            // 셸 활성화 정보 구조 초기화
            memset(&s_sai, 0, sizeof (s_sai));
            s_sai.cbSize = sizeof (s_sai);
*/

			APP_Entry_Init((void *)g_hInst, (void *)hWnd, _SCR_RESOLUTION_WIDTH, _SCR_RESOLUTION_HEIGHT);
			APP_Entry_Enter();

            break;

        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 그리기 코드를 추가합니다.
			APP_Entry_Draw();
            EndPaint(hWnd, &ps);
            break;

  		case WM_SIZE:
			GetClientRect(hWnd, &rect);
			atUIENG_OnResize( rect.right-rect.left, rect.bottom-rect.top );
			atUIAPI_FlushScreen();
			break;

		case WM_CLOSE:
			APP_Entry_Exit();
			DestroyWindow(hWnd);
			break;

      case WM_DESTROY:
//            CommandBar_Destroy(g_hWndMenuBar);
			APP_Entry_Final();
            PostQuitMessage(0);
            break;

/*
        case WM_ACTIVATE:
            // 활성화 메시지를 셸에 알립니다.
            SHHandleWMActivate(hWnd, wParam, lParam, &s_sai, FALSE);
            break;
        case WM_SETTINGCHANGE:
            SHHandleWMSettingChange(hWnd, wParam, lParam, &s_sai);
            break;
*/

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
