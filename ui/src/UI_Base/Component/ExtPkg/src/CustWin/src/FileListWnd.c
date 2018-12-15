//
// FileListWnd.h
//
// Date: 2005-12-08
// Coder: You Keun Sook
//
///////////////////////////////////////////////////////////////////////


#include "A_UIENG.h"
#include "UI_Util.h"
#include "FileListWnd.h"
#include "AEECOMP_ExtPkg.h"


typedef struct _FILELISTWNDDATA {
	atLPTSTR		szOutput;

	atHCONTROL	   hListBox;
} FILELISTWNDDATA,  * LPFILELISTWNDDATA;


typedef struct _CreateParam {
	atLPTSTR szPath;
	atLPTSTR szFilter;	
	int max_cnt;
	atBOOL bCheckbox;
	atLPTSTR szOutput;
} CreateParam;

#define KTHIM_LINE_INTERVAL 20


static int	FileListWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);


atBOOL      RegistFileListWndClass(void)
{
	atWINCLASS wc;
	
	atUIAPI_memset( &wc, 0, sizeof(atWINCLASS) );
	wc.dwAttr = atWS_FRAMEWINDOW | atWS_MODAL;
 	wc.dwUserDataSize = sizeof(FILELISTWNDDATA);
	wc.fnWndProc = FileListWndOnEvent;
	
	return atAPP_RegistWinClass( atAPPMNG_GetCurApp(), EXTPKG_CUSTWIN_FILELIST, &wc );
}


atHWINDOW	CreateFileListWnd(atHWINDOW hParentWnd, int id, atLPTSTR szTitle, atLPTSTR szPath, atLPTSTR szFilter, int max_cnt, atBOOL bCheckbox, atLPTSTR szOutput )
{
	atHWINDOW		hWnd;
	struct _CreateParam param;
	atMARGIN anchor;

	param.max_cnt = max_cnt;
	param.szPath = szPath;
	param.szFilter = szFilter;
	param.bCheckbox = bCheckbox;
	param.szOutput = szOutput;
	
	SET_MARGIN( anchor, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE );
    hWnd = atAPP_CreateWindowByClass( atAPPMNG_GetCurApp(), hParentWnd, id, szTitle, 
								200, 200,
								&anchor,
								EXTPKG_CUSTWIN_FILELIST, (void *)&param );
	atAPP_ShowWindow( atAPPMNG_GetCurApp(), hWnd, atTRUE );
	
	return hWnd;
}

static void	FileListWndOnInit(atHWINDOW hWnd, struct _CreateParam *lpParam)
{
	LPFILELISTWNDDATA lpData = (LPFILELISTWNDDATA)atWINDOW_GetDataPtr(hWnd);
	int				nLineNum;

	lpData->szOutput = lpParam->szOutput;
	
	lpData->hListBox = ExtPkg_CreateListBox( hWnd, 100, 2, 2, 80, 80, 12, atFALSE, lpParam->bCheckbox, atFALSE );
	atCONTROL_SetFont( lpData->hListBox, atUIAPI_GetStockFont(6) );
	atCONTROL_SetAnchor( lpData->hListBox, 2, 2, 2, 2 );

	nLineNum = MIN( lpParam->max_cnt, 10 );
	if( nLineNum <= 0 ) nLineNum = 5;
/*
	{
	int i;
	atFINDFILEENTRY fe;
	atBOOL ff;
	atTCHAR filename[256];

	ff = atUIAPI_FindFirstFile( lpParam->szPath, lpParam->szFilter, &fe );
	i = 0;
	while( ff ){
		atLISTBOX_EXT_Start_AddItem(lpData->hListBox);
		atLISTBOX_EXT_AddItem_NO(lpData->hListBox, ++i);
		if( fe.dwAttr & atFINDFILE_ATTR_DIRECTORY ){
			atUIAPI_sprintf( filename, _AT("[%s]"), fe.szFileName );
			atLISTBOX_EXT_AddItem_String(lpData->hListBox, filename, 0 );
		} else
			atLISTBOX_EXT_AddItem_String(lpData->hListBox, fe.szFileName, 0 );
		atLISTBOX_EXT_Update_AddItem(lpData->hListBox);

		ff = atUIAPI_FindNextFile( &fe );
    }	
	atUIAPI_CloseFindFile( &fe );
	}
*/
}

static void	FileListWndOnRelease(atHWINDOW hWnd)
{
//	LPFILELISTWNDDATA		lpData	= (LPFILELISTWNDDATA)atWINDOW_GetDataPtr(hWnd);
}

static int		FileListWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
//	LPFILELISTWNDDATA		lpData	= (LPFILELISTWNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch (nKeyCode)
	{
	case atVKEY_CLEAR:
		atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
		break;
	default:
		return 0;
	}

	return 1;
}

static void	FileListWndOnPaint(atHWINDOW hWnd)
{
//	LPFILELISTWNDDATA		lpData	= (LPFILELISTWNDDATA)atWINDOW_GetDataPtr(hWnd);

}

static int	FileListWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	LPFILELISTWNDDATA		lpData	= (LPFILELISTWNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch( nMsg )
	{
		case WNDMSG_INIT:
			FileListWndOnInit( hWnd, (void *)nParam1 );
			break;
		case WNDMSG_RELEASE:
			FileListWndOnRelease( hWnd );
			break;
		case WNDMSG_PAINT:
			FileListWndOnPaint( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return FileListWndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_COMMAND:
			if( nParam1 == 100 ){
				atWINDOW_PostMessage( atWINDOW_GetParentWindow(hWnd), nMsg, atWINDOW_GetID(hWnd), nParam2 );
				atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			}
			break;
		default:
			return 0;
			break;
	}

	return 1;
}

