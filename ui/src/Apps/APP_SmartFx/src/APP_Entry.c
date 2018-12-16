#include "SYSAPI.h"
#include "A_UIENG.h"
#include <string.h>

#include "APP_Entry.h"
#include "AEECOMP_BasePkg.h"
#include "AEECOMP_ExtPkg.h"

//#include "AmosPkg_ResDump.h"

#if 1
static int _HookWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	if( (nMsg == WNDMSG_KEYDOWN) && (nParam1 == atVKEY_DEBUG1) ){
		BasePkg_CreateMemoryInfoWnd(atNULL);
	}
	return 0;
}
#endif


extern void OEM_InitSYSAPIfunc(void);
extern void OEM_InitUIAPIfunc(void);


void APP_Entry_Init( void *hInst, void *hWnd, void *pHeap, long heap_size, int width, int height, void *pScreenBuffer, void *pResPtr, long nResSize )
{
	SYSAPI_INIT_DATA sdata;
	atUIAPI_INIT_DATA data;
	int vv[6];
	
	OEM_InitSYSAPIfunc();
	OEM_InitUIAPIfunc();

	//memset( &sdata, 0, sizeof(SYSAPI_INIT_DATA) );
	//SYSAPI_Initialize( &sdata, SYS_INITMASK_ALL );

	memset( &data, 0, sizeof(atUIAPI_INIT_DATA) );
	data.lSysInst = (atINTPTR)hInst;
	data.lSysData1 = (atINTPTR)hWnd;

	data.lMemData1 = (atINTPTR)pHeap;
	data.lMemData2 = heap_size;

	vv[0] = width;
	vv[1] = height;
	vv[2] = 16;
	vv[3] = (atINTPTR)pScreenBuffer;
	vv[4] = (width + 0x3) & ~0x3;;
	vv[5] = (atINTPTR)hWnd;
	data.lScrData1 = (atINTPTR)vv;
	data.lScrData2 = 0;

	// 초기화 과정에서 미리 리소스를 선로드 해 놓는다. (Font 로드를 위해)
	data.lResData1 = (atINTPTR)_AT("TESTMAIN");
	long resv[2] = { (atINTPTR)pResPtr, nResSize };
	data.lResData2 = (atINTPTR)resv;

	//data.lFontData1 = (atINTPTR)_AT("tahoma.ttf");
	//data.lFontData1 = (atINTPTR)_AT("x:\\나눔고딕Bold.ttf");
	//data.lFontData1 = (atINTPTR)_AT("notokr-medium.ttf");
	data.lFontData1 = (atINTPTR)_AT("notokr-medium.ttf");
	data.lFontData2 = (atINTPTR)_AT("TESTMAIN");

	atUIENG_Initialize( &data );

	atAPPMNG_SetCurApp( atAPP_CreateApplication(heap_size) );

	AEECOMP_BasePkg_Initialize();
	AEECOMP_ExtPkg_Initialize();

	//atUIAPI_UseKeyTimer( atTRUE );	// 반복키, LongKey 등을 활성화시킨다.
	//atSOFTKEY_ActivateSoftKey();
	//atANNBAR_ActivateAnnBar();

//	atHRESSRC hResSrc = atUIAPI_AddResSource(_AT("TESTMAIN"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pUser_ResData, sizeof(pUser_ResData));

//	atRESMNG_AddGlobalResource( atRES_CreateResource( pResData, 0, atRES_SRC_RESSTREAM ) );

/*	atUIAPI_AddResSource( _AT("System"), atUI_RES_SRC_RESSTREAM, (atLPVOID)pAmosPkg_ResData, sizeof(pAmosPkg_ResData) );
	atUIAPI_SetCurRes( _AT("System") );

	atLAYOUTBITMAP lpWinLayout;
	atSRECT2 margin;

    lpWinLayout.bmTopLeft      = atUIAPI_CreateBitmapFromRes( _AT("win_tl.img") );
    lpWinLayout.bmTopCenter    = atUIAPI_CreateBitmapFromRes( _AT("win_tc.img") );
    lpWinLayout.bmTopRight     = atUIAPI_CreateBitmapFromRes( _AT("win_tr.img") );
    lpWinLayout.bmMiddleLeft   = atUIAPI_CreateBitmapFromRes( _AT("win_ml.img") );
    lpWinLayout.bmMiddleCenter = atUIAPI_CreateBitmapFromRes( _AT("win_mc.img") );
    lpWinLayout.bmMiddleRight  = atUIAPI_CreateBitmapFromRes( _AT("win_mr.img") );
    lpWinLayout.bmBottomLeft   = atUIAPI_CreateBitmapFromRes( _AT("win_bl.img") );
    lpWinLayout.bmBottomCenter = atUIAPI_CreateBitmapFromRes( _AT("win_bc.img") );
    lpWinLayout.bmBottomRight  = atUIAPI_CreateBitmapFromRes( _AT("win_br.img") );
	SET_MARGIN( margin, 26, 3, 3, 3 );
//	atAPP_SetDefaultLayout( atAPPMNG_GetCurApp(), 0, &lpWinLayout, &margin );

    lpWinLayout.bmTopLeft      = atUIAPI_CreateBitmapFromRes( _AT("pop_lst_tl.img") );
    lpWinLayout.bmTopCenter    = atUIAPI_CreateBitmapFromRes( _AT("pop_lst_tc.img") );
    lpWinLayout.bmTopRight     = atUIAPI_CreateBitmapFromRes( _AT("pop_lst_tr.img") );
    lpWinLayout.bmMiddleLeft   = atUIAPI_CreateBitmapFromRes( _AT("pop_lst_ml.img") );
    lpWinLayout.bmMiddleCenter = atUIAPI_CreateBitmapFromRes( _AT("pop_lst_mc.img") );
    lpWinLayout.bmMiddleRight  = atUIAPI_CreateBitmapFromRes( _AT("pop_lst_mr.img") );
    lpWinLayout.bmBottomLeft   = atUIAPI_CreateBitmapFromRes( _AT("pop_lst_bl.img") );
    lpWinLayout.bmBottomCenter = atUIAPI_CreateBitmapFromRes( _AT("pop_lst_bc.img") );
    lpWinLayout.bmBottomRight  = atUIAPI_CreateBitmapFromRes( _AT("pop_lst_br.img") );
	SET_MARGIN( margin, 24, 6, 6, 6 );
//	atAPP_SetDefaultLayout( atAPPMNG_GetCurApp(), 2, &lpWinLayout, &margin );
*/

#if 1
	atAPP_AddMessageHooker( atAPPMNG_GetCurApp(), _HookWndOnEvent );
#endif
}




void APP_Entry_Final(void)
{
	atAPP_DestroyApplication( atAPPMNG_GetCurApp() );
	atAPPMNG_SetCurApp( atNULL );
	
	AEECOMP_ExtPkg_Finalize();
	AEECOMP_BasePkg_Finalize();

	atUIENG_Finalize();
}


#include "MainWnd.h"

//CTN_MFWnd	m_MF;

void APP_Entry_Enter(void)
{

	atHWINDOW hWnd;

//	atUIAPI_AddResSource( _AT("System2"), atUI_RES_SRC_RESSTREAM, (const atLPVOID)pResdata, sizeof(pResdata) );
//	atUIAPI_SetCurRes( _AT("System2") );

//	hWnd = atAPP_CreateWindow( atAPPMNG_GetCurApp(), atNULL, 0, atWS_FRAMEWINDOW, _AT("Test Window #1"), 10, 10, 300, 220, sizeof(SETTINGWNDDATA), SettingWndOnEvent, atNULL, atNULL );

//	hWnd = CreateDummyWnd(atNULL);
//	atAPP_ShowWindow( atAPPMNG_GetCurApp(), hWnd, atTRUE );
//	return;


	hWnd = atAPP_CreateWindow( atAPPMNG_GetCurApp(), atNULL, 0, atWS_NONEFRAME | atWS_MODAL, _AT("Main Window"), 0, 0, 0, 0, sizeof(MAINWNDDATA), MainWndOnEvent, atNULL, atNULL );
	atWINDOW_SetAnchor( hWnd, 0, 0, 0, 0);
	atAPP_ShowWindow( atAPPMNG_GetCurApp(), hWnd, atTRUE );

//	hWnd = atAPP_CreateWindow( atAPPMNG_GetCurApp(), atNULL, 0, atWS_FRAMEWINDOW, _AT("Test Window #2"), -30, 150, 200, 220, sizeof(SETTINGWNDDATA), SettingWndOnEvent, atNULL, atNULL );

//	hWnd = atAPP_CreateWindow( atAPPMNG_GetCurApp(), atNULL, 0, atWS_FRAMEWINDOW, _AT("Tonrado"), 0, 0, atUIAPI_GetScreenWidth(), atUIAPI_GetScreenHeight(), 0, CTN_MFWnd::MessageProc, (atLPVOID)&m_MF, atNULL );
//	hWnd = atAPP_CreateWindow( atAPPMNG_GetCurApp(), atNULL, 0, atWS_NONE, _AT("Tonrado"), 0, 0, atUIAPI_GetScreenWidth(), atUIAPI_GetScreenHeight(), 0, CTN_MFWnd::MessageProc, (atLPVOID)&m_MF, atNULL );
//	hWnd = atWINDOW_Create( atNULL, atWS_NONE, _AT("Tonrado"), 0, 0, atUIAPI_GetScreenWidth(), atUIAPI_GetScreenHeight(), 0, CTN_MFWnd::MessageProc, (atLPVOID)&m_MF );
//	atAPP_AddWindow( atAPPMNG_GetCurApp(), hWnd );

//	hWnd = atWINDOW_Create( atNULL, atWS_FRAMEWINDOW, 0, _AT("Test Window #2"), 70, 50, 300, 220, sizeof(SETTINGWNDDATA), SettingWndOnEvent, atNULL );
//	atWINDOW_SetLayoutBitmap2( hWnd, &lpWinLayout );
//	atWINDOW_SetLayoutMargin2( hWnd, &margin );
//	atAPP_AddWindow( atAPPMNG_GetCurApp(), hWnd);
//	APP_Open();

//	CreateMsgBox( atNULL, 0, _AT("XXxxxxxxxxxxx"), _AT("YYyyyyyyyyyyyyyy\nZZzzzzzzzz"), atMBT_QUESTION_CHECK );
//	CreateMsgBoxEx( atNULL, 0, _AT("XXxxxxxxxxxxx"), _AT("YYyyyyyyyyyyyyyy\nZZzzzzzzzz"), atMBT_QUESTION_CHECK );
}


void APP_Entry_Exit(void)
{
//	atUIAPI_RemoveResSource( _AT("System2") );
}

void APP_Entry_Draw(void)
{
	atUIAPI_FlushScreen();
}

void APP_Entry_TickTimer(long nMilliSec)
{
	atUIAPI_TickTimer(nMilliSec);
}

void APP_Entry_Debug(int param1, int param2)
{
	atUIENG_OnSystemNotify(param1, param2);
}
