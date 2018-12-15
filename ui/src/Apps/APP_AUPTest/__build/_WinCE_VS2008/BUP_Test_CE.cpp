// BUP_Test.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "BUP_Test_CE.h"

#include "BUP.h"
#include "APP_Entry.h"

#define _SCR_RESOLUTION_WIDTH	(240)
#define _SCR_RESOLUTION_HEIGHT	(320)


#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE			g_hInst;			// ���� �ν��Ͻ��Դϴ�.
HWND				g_hWndMenuBar;		// �޴� ���� �ڵ��Դϴ�.

static int g_screen_width, g_screen_height;


// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	MSG msg;

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BUP_TEST));

	// �⺻ �޽��� �����Դϴ�.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
//  ����:
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
    TCHAR szTitle[MAX_LOADSTRING];		// ���� ǥ���� �ؽ�Ʈ
    TCHAR szWindowClass[MAX_LOADSTRING];	// �� â Ŭ���� �̸�

    g_hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

    // CAPEDIT �� SIPPREF�� ���� ��� ��ġ Ư�� ��Ʈ���� �ʱ�ȭ�ϱ� ����
    // ���� ���α׷��� �ʱ�ȭ�ϴ� ���� SHInitExtraControls�� �� �� ȣ���ؾ� �մϴ�.
    //SHInitExtraControls();

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_BUP_TEST, szWindowClass, MAX_LOADSTRING);

    //�̹� ���� ���� ��� â�� ��Ŀ���� �� ���� �����ϴ�.
    hWnd = FindWindow(szWindowClass, szTitle);	
    if (hWnd) 
    {
        // �� �� �ڽ� â�� ��Ŀ���� �����մϴ�.
        // "| 0x00000001"�� ����Ͽ� ������ ��� â�� �������� ���� ����
        // Ȱ��ȭ�մϴ�.
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
    // CW_USEDEFAULT�� ����Ͽ� �� â�� ����� ��� �޴� ������ ���̴�
    // ������� �ʽ��ϴ�. ���� �޴� ������ �ִ� ���
    // â�� ���� �� â�� ũ�⸦ �����մϴ�.
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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
            // �޴��� ���� ������ ���� �м��մϴ�.
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

            // �� Ȱ��ȭ ���� ���� �ʱ�ȭ
            memset(&s_sai, 0, sizeof (s_sai));
            s_sai.cbSize = sizeof (s_sai);
*/

			APP_Entry_Init((void *)g_hInst, (void *)hWnd, _SCR_RESOLUTION_WIDTH, _SCR_RESOLUTION_HEIGHT);
			APP_Entry_Enter();

            break;

        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            // TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
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
            // Ȱ��ȭ �޽����� �п� �˸��ϴ�.
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
