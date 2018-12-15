/** 
  @file App.c
  @brief Application�� ���� API
  @remark Application ���� API�� ����.
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see App.h
*/
#include "UI_Types.h"
#include "UI_MNG.h"
#include "App.h"
#include "WinMng.h"
#include "Window.h"
#include "UI_API/UI_API.h"
#include "UI_Util.h"
#include <string.h>


/** @brief Application ��ü (���ο�) */
typedef struct _tagatAPP {
	atLPVOID	   lpAppHeap;
	atHWINMNG      hWinMng;	
	atHMSGPROC     hMsgProc;
} atAPP, *atLPAPP;



/** 
 @brief Application ��ü�� �����͸� ��´�. (���ο�)
 
 Application ��ü�� �����͸� ��´�. (���ο�)

 @param hApp [in] : Application �ڵ�
 @return Application ��ü�� ������
 @remark ���ο�
*/
atLPAPP atAPP_GetPtr(atHAPP hApp)
{
	return (atLPAPP)hApp;
}


/** 
 @brief Application ��ü�� �ڵ��� ��´�. (���ο�)
 
 Application ��ü�� �ڵ��� ��´�. (���ο�)

 @param lpApp [in] : Application ��ü�� ������
 @return Application�� �ڵ�
 @remark ���ο�
*/
atHAPP atAPP_GetHandle(atLPAPP lpApp)
{
	return (atHAPP)lpApp;
}


/** 
 @brief Application�� �����Ѵ�.
 
 Application�� �����Ѵ�.

 @param lHeapSize [in] : ���ø����̼��� ����ϴ� Heap ������
 @return ������ Application�� �ڵ�
 @remark ���� Heap ũ��� ������ �ʴ´�.
*/
atHAPP	atAPP_CreateApplication( atLONG lHeapSize )
{
	atLPAPP lpApp;

	lpApp = (atLPAPP)UI_MEMALLOC(sizeof(atAPP));
	if ( lpApp == atNULL ) return atNULL;

	lpApp->hWinMng = atWINMNG_CreateWinMng( atNULL );
	lpApp->hMsgProc = atMSG_CreateMsgProc();
	
	return atAPP_GetHandle(lpApp);
}


/** 
 @brief Application�� �ı��Ѵ�.
 
 Application�� �ı��Ѵ�.

 @param hApp [in] : Application �ڵ�
 @return ����
*/
void atAPP_DestroyApplication( atHAPP hApp )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	if( lpApp->hWinMng ){
		atWINMNG_DestroyWinMng( lpApp->hWinMng );
		lpApp->hWinMng = atNULL;
	}
	
	if( lpApp->hMsgProc ){
		atMSG_DestroyMsgProc( lpApp->hMsgProc );
		lpApp->hMsgProc = atNULL;
	}
	
	UI_MEMFREE( lpApp );
}


/** 
 @brief Window�� Default ���̾ƿ��� ����
 
 Window�� Default ���̾ƿ��� �����Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param DefIndex [in] : ���̾ƿ� �ε���. 0~2���� ��� ����. 0-FrameWindow, 1-PopupWindow, 2-UserDefWindow
 @param lpLayoutBitmap [in] : ���̾ƿ��� ���� 8���� ��Ʈ��
 @param lpLayoutMargin [in] : ���̾ƿ��� ���� ũ��
 @return ������ atTRUE, ���н� atFALSE
*/
atBOOL  atAPP_SetDefaultLayout( atHAPP hApp, atINT DefIndex, atLAYOUTBITMAP *lpLayoutBitmap, atMARGIN *lpLayoutMargin )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atFALSE;
	
	return atWINMNG_SetDefaultLayout( lpApp->hWinMng, DefIndex, lpLayoutBitmap, lpLayoutMargin );
}


/** 
 @brief WinClass�� ����Ѵ�.
 
 WinClass�� ����Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param szClassName [in] : Ŭ���� �̸�
 @param pWinClass [in] : ����� WinClass ������ ������
 @return ������ atTRUE, ���н� atFALSE
*/
atBOOL		atAPP_RegistWinClass( atHAPP hApp, atLPTSTR szClassName, atWINCLASS *pWinClass )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atFALSE;
	
	return atWINMNG_RegistWinClass( lpApp->hWinMng, szClassName, pWinClass );
}


/** 
 @brief WinClass ����� �����Ѵ�.
 
 WinClass ����� �����Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param szClassName [in] : Ŭ���� �̸�
 @return ������ atTRUE, ���н� atFALSE
*/
atBOOL		atAPP_UnregistWinClass( atHAPP hApp, atLPTSTR szClassName )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atFALSE;
	
	return atWINMNG_UnregistWinClass( lpApp->hWinMng, szClassName );
}


/** 
 @brief WinClass�� ��´�.
 
 Ŭ���� �̸��� �̿��Ͽ� ��ϵ� WinClass�� ��´�.

 @param hApp [in] : Application �ڵ�
 @param szClassName [in] : �� Ŭ���� �̸�
 @return WinClass�� ������. ���н� atNULL
*/
atWINCLASS *atAPP_GetWinClass( atHAPP hApp, atLPTSTR szClassName  )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_GetWinClass( lpApp->hWinMng, szClassName );
}


/** 
 @brief Window�� ���� �����Ѵ�.
 
 Window�� ���� �����Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param hOwnerWnd [in] : �θ� Window�� �ڵ�
 @param nID [in] : Window�� ID
 @param nStyle [in] : Window Style
 @param szWindowText [in] : Window Ÿ��Ʋ
 @param nWidth, nHeight [in] : ������ �������� ũ�� 
 @param rtLayoutMargin [in] : �ܰ����� ����. �� ������ �����Ͽ� Ŭ���̾�Ʈ�� ũ�Ⱑ �����ȴ�.
 @param rtAnchor [in] : Anchor�� �����Ѵ�.
 @param dwUserDataSize [in] : ����� �߰� ������ ũ��
 @param fnWndProc [in] : ����� �̺�Ʈ �ڵ鷯.
 @param lpCreateParam [in] : ������ ������ �Ķ����
 @param lpExData [in] : ������ ������ �߰� ������. �ַ� WinClass���� ����.
 @return Window �ڵ�
*/
atHWINDOW	atAPP_PreCreateWindowEx( atHAPP hApp, atHWINDOW hOwnerWnd, atINT nID, int nStyle, atLPTSTR szWindowText, 
								int nWidth, int nHeight, atMARGIN *rtLayoutMargin, atMARGIN *rtAnchor,
								atDWORD dwUserDataSize, atLPWNDPROC fnWndProc, 
								void *lpCreateParam, void *lpExData )
{
	atHWINDOW hWnd;
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	hWnd = atWINMNG_CreateWindowEx( lpApp->hWinMng, hOwnerWnd, nID, nStyle, szWindowText, 
									nWidth, nHeight, rtLayoutMargin, rtAnchor,
									dwUserDataSize, fnWndProc, 
									atNULL, 
									lpCreateParam, lpExData );

	return hWnd;
}


/** 
 @brief Window�� �����Ѵ�. (Ȯ��)
 
 Window�� �����Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param hOwnerWnd [in] : �θ� Window�� �ڵ�
 @param nID [in] : Window�� ID
 @param nStyle [in] : Window Style
 @param szWindowText [in] : Window Ÿ��Ʋ
 @param nWidth, nHeight [in] : ������ �������� ũ�� 
 @param rtLayoutMargin [in] : �ܰ����� ����. �� ������ �����Ͽ� Ŭ���̾�Ʈ�� ũ�Ⱑ �����ȴ�.
 @param rtAnchor [in] : Anchor�� �����Ѵ�.
 @param dwUserDataSize [in] : ����� �߰� ������ ũ��
 @param fnWndProc [in] : ����� �̺�Ʈ �ڵ鷯.
 @param lpCreateParam [in] : ������ ������ �Ķ����
 @param lpExData [in] : ������ ������ �߰� ������. �ַ� WinClass���� ����.
 @return Window �ڵ�
*/
atHWINDOW	atAPP_CreateWindowEx( atHAPP hApp, atHWINDOW hOwnerWnd, atINT nID, int nStyle, atLPTSTR szWindowText, 
								int nWidth, int nHeight, atMARGIN *rtLayoutMargin, atMARGIN *rtAnchor,
								atDWORD dwUserDataSize, atLPWNDPROC fnWndProc, 
								void *lpCreateParam, void *lpExData )
{
	atHWINDOW hWnd;
	
	atKERNEL_LockInput();

	hWnd = atAPP_PreCreateWindowEx( hApp, hOwnerWnd, nID, nStyle, szWindowText, 
									nWidth, nHeight, rtLayoutMargin, rtAnchor,
									dwUserDataSize, fnWndProc, 
									lpCreateParam, lpExData );
	atAPP_RegistWindow( hApp, hWnd );

	atKERNEL_UnlockInput();

	return hWnd;
}


/** 
 @brief Window�� �����Ѵ�.
 
 Window�� �����Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param hOwnerWnd [in] : �θ� Window�� �ڵ�
 @param nID [in] : Window�� ID
 @param nStyle [in] : Window Style
 @param szWindowText [in] : Window Ÿ��Ʋ
 @param nStartX, nStartY [in] : ������ �������� ��ġ
 @param nWidth, nHeight [in] : ������ �������� ũ�� 
 @param dwUserDataSize [in] : ����� �߰� ������ ũ��
 @param fnWndProc [in] : ����� �̺�Ʈ �ڵ鷯.
 @param lpCreateParam [in] : ������ ������ �Ķ����
 @param lpExData [in] : ������ ������ �߰� ������. �ַ� WinClass���� ����.
 @return Window �ڵ�
*/
atHWINDOW	atAPP_CreateWindow( atHAPP hApp, atHWINDOW hOwnerWnd, atINT nID, int nStyle, atLPTSTR szWindowText, 
								int nStartX, int nStartY, int nWidth, int nHeight,
								atDWORD dwUserDataSize, atLPWNDPROC fnWndProc, 
								void *lpCreateParam, void *lpExData )
{
	atHWINDOW hWnd;
	atMARGIN rtAnchor;
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	
	if( lpApp == atNULL ) return atNULL;
	
	SET_MARGIN( rtAnchor, nStartY, atANCHOR_IGNORE, nStartX, atANCHOR_IGNORE );	
	hWnd = atAPP_CreateWindowEx( hApp, hOwnerWnd, nID, nStyle, szWindowText, 
								nWidth, nHeight, atNULL, &rtAnchor,
								dwUserDataSize, fnWndProc, lpCreateParam, lpExData );

	return hWnd;
}


/** 
 @brief WinClass�κ��� Window�� �����Ѵ�.
 
 WinClass�κ��� Window�� �����Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param hOwnerWnd [in] : �θ� Window�� �ڵ�
 @param nID [in] : Window�� ID
 @param nStyle [in] : Window Style
 @param szWindowText [in] : Window Ÿ��Ʋ
 @param nWidth, nHeight [in] : ������ �������� ũ�� 
 @param rtAnchor [in] : Anchor�� �����Ѵ�.
 @param szClassName [in] : WinClass �̸�
 @param lpCreateParam [in] : ������ ������ �Ķ����
 @return Window �ڵ�
*/
atHWINDOW	atAPP_CreateWindowByClass( atHAPP hApp, atHWINDOW hOwerWnd, atINT nID, atLPTSTR szWindowText, 
										int nWidth, int nHeight, atMARGIN *rtAnchor,
										atLPTSTR szClassName, void *lpCreateParam )
{
	atHWINDOW hWnd;
	atLPWINDOW lpWnd;
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	hWnd = atWINMNG_CreateWindowByClass( lpApp->hWinMng, hOwerWnd, nID, szWindowText, 
											nWidth, nHeight, rtAnchor,
											szClassName, lpCreateParam );
//	atAPP_RegistWindow( hApp, hWnd );
	lpWnd = atWINDOW_GetPtr(hWnd);
	if( lpWnd == atNULL ) return atNULL;
	lpWnd->hApp = hApp;	

	return hWnd;
}

	
/** 
 @brief Window�� �ı�
 
 Window�� �ı��Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param hWnd [in] : �ı��� Window �ڵ�
 @return ����
*/
void atAPP_DestroyWindow( atHAPP hApp, atHWINDOW hWnd )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_DestroyWindow( lpApp->hWinMng, hWnd );
}


/** 
 @brief ��� Window�� �ı�
 
 ���� Application�� ��ϵ� ��� Window�� �ı��Ѵ�.

 @param hApp [in] : Application �ڵ�
 @return ����
*/
void atAPP_DestroyAllWindows( atHAPP hApp )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_DestroyAllWindows( lpApp->hWinMng );
}


/** 
 @brief ��� Window�� �ı� (���� �Ǵ� �ڵ鷯)
 
 ���� Application�� ��ϵ� ��� Window��, ���ǿ� �´� �����츸 ��� �ı��Ѵ�.\n
 �̶�, �ı��� ���� �Ǵ� �ڵ鷯�� �Ķ���ͷ� �����Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param fnIsDel [in] : ���� �Ǵ� �ڵ鷯.
 @return ����
*/
void atAPP_DestroyWindowsEx( atHAPP hApp, atBOOL (*fnIsDel)(atHWINDOW) )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_DestroyWindowsEx( lpApp->hWinMng, fnIsDel );
}


/** 
 @brief Window�� ������ ��´�.
 
 ���� Application�� ��ϵ� Window�� ������ ��´�.

 @param hApp [in] : Application �ڵ�
 @return Window�� ����
*/
atINT atAPP_GetWindowCount( atHAPP hApp )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return 0;
	
	return atWINMNG_GetWindowCount( lpApp->hWinMng );
}


/** 
 @brief Ư�� �ε����� Window�� ��´�.
 
 Ư�� �ε����� Window�� ��´�.

 @param hApp [in] : Application �ڵ�
 @param nIndex [in] : Window Index
 @return Window �ڵ�
 @remark Window�� Index�� ���÷� ���ϱ� ������ Unique���� �ʴ�. ���ο����� �ַ� ���ȴ�.
*/
atHWINDOW atAPP_GetWindowByIndex( atHAPP hApp, int nIndex )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_GetWindowByIndex( lpApp->hWinMng, nIndex );
}


/** 
 @brief Window�� ���
 
 Application�� Window�� ����Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param hWnd [in] : Window �ڵ�
 @return ������ atTRUE, ���н� atFALSE
*/
atBOOL atAPP_RegistWindow( atHAPP hApp, atHWINDOW hWnd )
{
	atLPWINDOW lpWnd;
	atBOOL ret;
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atFALSE;
	
	lpWnd = atWINDOW_GetPtr(hWnd);
	if( lpWnd == atNULL ) return atFALSE;
	
	lpWnd->hApp = hApp;
	
	ret = atWINMNG_RegistWindow( lpApp->hWinMng, hWnd );
	if( ret == atFALSE ) return atFALSE;

//	atWINDOW_Refresh( hWnd );
		
	return ret;
}
 

/** 
 @brief ��ϵ� Window ����
 
 Application�� ��ϵ� Window�� �����Ѵ�.\n
 ��ϵǾ��ٰ� ������ ������� �ڵ����� �ı��ȴ�. 

 @param hApp [in] : Application �ڵ�
 @param hWnd [in] : Window �ڵ�
 @return ������ atTRUE, ���н� atFALSE
*/
atBOOL atAPP_ReleaseWindow( atHAPP hApp, atHWINDOW hWnd )
{
	atBOOL ret;
	atREGION rgn;
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atFALSE;

	atWINDOW_GetScreenRegion( hWnd, &rgn );
	ret = atWINMNG_ReleaseWindow( lpApp->hWinMng, hWnd );
	if( ret == atFALSE ) return atFALSE;
	
	atUIAPI_AddInvalideArea( &rgn );
		
	return ret;
}   


/** 
 @brief ���� ��Ŀ���� ���� Window�� ��´�.
 
 ���� ��Ŀ���� ���� Window�� ��´�.

 @param hApp [in] : Application �ڵ�
 @return ��Ŀ���� ���� Window�� �ڵ�. ���� ��, atNULL
*/
atHWINDOW atAPP_GetCurWindow( atHAPP hApp )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_GetCurWindow( lpApp->hWinMng );
}

/** 
 @brief Window�� ��Ŀ���� �̵���Ų��.
 
 Window�� ��Ŀ���� �̵���Ų��.

 @param hApp [in] : Application �ڵ�
 @param hWnd [in] : Window �ڵ�
 @return ������ ��Ŀ���� ���� Window�� �ڵ�. ���� ��, atNULL
*/
atHWINDOW	atAPP_SetCurWindow( atHAPP hApp, atHWINDOW hWnd )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_SetCurWindow( lpApp->hWinMng, hWnd );
}

void atAPP_SetFocusToWindow( atHAPP hApp, atHWINDOW hWindow )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_SetFocusToWindow( lpApp->hWinMng, hWindow );
}

//atHWINOBJ atAPP_GetFocusedObj( atHAPP hApp )
//{
//	atLPAPP lpApp = atAPP_GetPtr(hApp);
//	if( lpApp == atNULL ) return atNULL;
//	
//	return atWINMNG_GetFocusedObj( lpApp->hWinMng );
//}

/** 
 @brief Window�� Anchor�� �����Ѵ�.
 
 Window�� Anchor�� �����Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param hWnd [in] : Window �ڵ�
 @param nTop [in] : ���� Anchor ��. ������ �� atANCHOR_IGNORE
 @param nBottom [in] : �Ʒ��� Anchor ��. ������ �� atANCHOR_IGNORE
 @param nLeft [in] : ���� Anchor ��. ������ �� atANCHOR_IGNORE
 @param nRight [in] : ������ Anchor ��. ������ �� atANCHOR_IGNORE
 @return ����
 @remark Top/Bottom, Left/Right ��, ������ ��� atANCHOR_IGNORE�� �����ϸ� ũ�� ���� ���� ��ġ�� �߾����� �̵��ϴ� ȿ���� ���´�.\n
 				 �ش� ������� �ڵ����� ���̾ƿ� �����ı��� ���� �ش�.
*/
atVOID	atAPP_SetAnchor( atHAPP hApp, atHWINDOW hWnd, atINT nTop, atINT nBottom, atINT nLeft, atINT nRight )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINDOW_SetAnchor( hWnd, nTop, nBottom, nLeft, nRight );
	atWINMNG_AdjustWindowArea( lpApp->hWinMng, hWnd );
}


/** 
 @brief Window�� ���� ������ �Ű� �´�.
 
 Window�� ���� ������ �Ű� �´�.

 @param hApp [in] : Application �ڵ�
 @param hWnd [in] : Window �ڵ�
 @return ������ ���� �������� �Ű��� �������� �ڵ�
 @remark StayOnTop �Ӽ��� ���� ���� �������� �ű��� ���� ���� �ִ�.
*/
atHWINDOW	atAPP_SetTopWindow( atHAPP hApp, atHWINDOW hWnd )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_SetTopWindow( lpApp->hWinMng, hWnd );
}


/** 
 @brief Window�� ���� �������� �ű��.
 
 Window�� ���� �������� �ű��.

 @param hApp [in] : Application �ڵ�
 @param hWnd [in] : Window �ڵ�
 @return ������ ���� ���ʿ� �Ű��� �������� �ڵ�
*/
atHWINDOW	atAPP_SetBottomWindow( atHAPP hApp, atHWINDOW hWnd )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_SetBottomWindow( lpApp->hWinMng, hWnd );
}


/** 
 @brief �ش� ID�� �����츦 ã�´�.
 
 �ش� ID�� �����츦 ã�´�.

 @param hApp [in] : Application �ڵ�
 @param nWindowID [in] : Window�� ID
 @return ã�� Window�� �ڵ�. ���� �� atNULL
 @remark ID�� �ߺ��� ���Ǳ� ������ ���� ID�� �����찡 2�� �̻��� ��, ù��° �͸� ã�� �� �ִ�.
*/
atHWINDOW	atAPP_FindWindowByID( atHAPP hApp, atINT nWindowID )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_FindWindowByID( lpApp->hWinMng, nWindowID );
}


/** 
 @brief �ش� �̸��� �����츦 ã�´�.
 
 �ش� ID�� �����츦 ã�´�.

 @param hApp [in] : Application �ڵ�
 @param szWindowText [in] : Window�� �̸�
 @return ã�� Window�� �ڵ�. ���� �� atNULL
*/
atHWINDOW	atAPP_FindWindowByName( atHAPP hApp, atLPTSTR szWindowText )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_FindWindowByName( lpApp->hWinMng, szWindowText );
}


/** 
 @brief ȭ����� �ش� ������ �ֻ�� �����츦 ã�´�.
 
 ȭ����� �ش� ������ �ֻ�� �����츦 ã�´�.

 @param hApp [in] : Application �ڵ�
 @param nX, nY [in] : ������ �Ǵ� ȭ����� ��ġ
 @return ã�� Window�� �ڵ�. ���� �� atNULL
*/
atHWINDOW	atAPP_FindWindowAt( atHAPP hApp, atINT nX, atINT nY )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return atNULL;
	
	return atWINMNG_FindWindowAt( lpApp->hWinMng, nX, nY );
}


/** 
 @brief Application�� Draw�� ���߰ų� �簳�Ѵ�.
 
 Application�� Draw�� ���߰ų� �簳�Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param IsDraw [in] : Draw ���� ����
 @return ����
*/
void atAPP_SetDrawFlag( atHAPP hApp,  atBOOL IsDraw )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_SetDrawFlag( lpApp->hWinMng, IsDraw );
}


/** 
 @brief Window�� Show/Hide ��Ų��.
 
 Window�� Show/Hide ��Ų��.

 @param hApp [in] : Application �ڵ�
 @param hWnd [in] : Window �ڵ�
 @param bShow [in] : Show/Hide
 @return ����
*/
void atAPP_ShowWindow( atHAPP hApp, atHWINDOW hWnd, atBOOL bShow )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_ShowWindow( lpApp->hWinMng, hWnd, bShow );
}


/** 
 @brief Window�� ��� �ٽ� �׸���.
 
 Ư�� ���̽� �����츦 �������� Application�� ���Ե� Window�� ��� �ٽ� �׸���.

 @param hApp [in] : Application �ڵ�
 @param hWndBase [in] : ������ �Ǵ� Window �ڵ�
 @return �׷��� �������� ����
*/
int atAPP_RedrawAllWindowEx( atHAPP hApp, atHWINDOW hWndBase )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return -1;
	
	return atWINMNG_RedrawAllWindowEx( lpApp->hWinMng, hWndBase );
}


/** 
 @brief Window�� ��� �ٽ� �׸���.
 
 Application�� ���Ե� Window�� ��� �ٽ� �׸���.

 @param hApp [in] : Application �ڵ�
 @return �׷��� �������� ����
*/
int atAPP_RedrawAllWindow( atHAPP hApp )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return -1;
	
	return atWINMNG_RedrawAllWindow( lpApp->hWinMng );
}


/** 
 @brief Ư�� ������ ��ģ Window�� ��� �ٽ� �׸���.
 
 Ư�� ������ ��ģ Window�� ��� �ٽ� �׸���.

 @param hApp [in] : Application �ڵ�
 @param rtWinRect [in] : �׸� ����
 @return �׷��� �������� ����
*/
int atAPP_RedrawAllWindowArea( atHAPP hApp, atRECT *rtWinRect )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return -1;
	
	return atWINMNG_RedrawAllWindowArea( lpApp->hWinMng, rtWinRect );
}


/** 
 @brief Application�� ���Ḧ ��û�Ѵ�.
 
 Application�� ���Ḧ ��û�Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param nExitCode [in] : �����ڵ�
 @return ����
*/
atVOID 	atAPP_SendExitCode( atHAPP hApp, atINT nExitCode )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atMSG_SendExitCode( lpApp->hMsgProc, nExitCode );
}


/** 
 @brief Application�� �޽��� �ڵ鷯�� �����Ѵ�.(���ο�)
 
 Application�� �޽��� �ڵ鷯�� �����Ѵ�.(���ο�)

 @param hApp [in] : Application �ڵ�
 @return ��� �ڵ�
*/
int atAPP_ProcessEvents( atHAPP hApp )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return -1;
	
	return atMSG_ProcessEvents( lpApp->hMsgProc);
}


/** 
 @brief Window�� Message�� ������.
 
 Window�� Message�� ������.

 @param hApp [in] : Application �ڵ�
 @param hwin [in] : Window �ڵ�
 @param msg [in] : Message
 @param param1, param2 [in] : �Ķ����
 @return ������ �ڵ鷯 ȣ�� �����
*/
int atAPP_SendMessage( atHAPP hApp, atHWINDOW hwin, int msg, long param1, long param2 )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return -1;
	
	return atMSG_SendMessage( lpApp->hMsgProc, hwin, msg, param1, param2 );
}


/** 
 @brief Window�� Message�� ������.
 
 Window�� Message�� ������.

 @param hApp [in] : Application �ڵ�
 @param hwin [in] : Window �ڵ�
 @param msg [in] : Message
 @param param1, param2 [in] : �Ķ����
 @return ������ �ڵ鷯 ȣ�� �����
 @remark Message Queue�� �̿��Ͽ� ó���Ѵ�.
*/
int atAPP_PostMessage(  atHAPP hApp, atHWINDOW hwin, int msg, long param1, long param2 )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return -1;
	
	return atMSG_PostMessage( lpApp->hMsgProc, hwin, msg, param1, param2 );
}


/** 
 @brief Hooker�� ��ġ�Ѵ�.
 
 Hooker�� ��ġ�Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param fnHookerCallback [in] : Hooker�� Handler
 @return Hooker�� ID
*/
atINT atAPP_AddMessageHooker( atHAPP hApp, atLPWNDPROC fnHookerCallback )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return -1;
	
	return atMSG_AddMessageHooker( lpApp->hMsgProc, (atLPWINOBJPROC)fnHookerCallback );
}


/** 
 @brief Hooker�� �����Ѵ�.
 
 Hooker�� �����Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param nHookerID [in] : Hooker ID
 @return ����
*/
void atAPP_RemoveMessageHooker( atHAPP hApp, atINT nHookerID )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atMSG_RemoveMessageHooker( lpApp->hMsgProc, nHookerID );
}


/** 
 @brief �������� ���̾ƿ��� ������
 
 Anchor�� �����Ͽ� �������� ���̾ƿ��� ������ �Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param hWnd [in] : Window �ڵ�
 @return ����
*/
void	atAPP_AdjustWindowArea( atHAPP hApp, atHWINDOW hWnd )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_AdjustWindowArea( lpApp->hWinMng, hWnd );
}


/** 
 @brief ��� �������� ���̾ƿ��� ������
 
 ��� �������� ���̾ƿ��� Anchor�� �����Ͽ� ������ �Ѵ�.

 @param hApp [in] : Application �ڵ�
 @return ����
*/
void	atAPP_AdjustAllWindowArea( atHAPP hApp )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_AdjustAllWindowArea( lpApp->hWinMng );
}


/** 
 @brief �������� ������ ����
 
 �������� ������ �����Ѵ�.

 @param hApp [in] : Application �ڵ�
 @param hWnd [in] : Window �ڵ�
 @param nX, nY [in] : �������� ��ġ
 @param nWidth, nHeight [in] : �������� ũ��
 @return ����
*/
atVOID	atAPP_SetWindowRegion( atHAPP hApp, atHWINDOW hWnd, atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;
	
	atWINMNG_SetWindowRegion( lpApp->hWinMng, hWnd, nX, nY, nWidth, nHeight );
}


/** 
 @brief �޽��� ó�� tick
 
 �޽��� ó�� tick. ������ �ֱ�� �ݺ��ؼ� ȣ���� ��� �Ѵ�. ������ ���ο��� proc�� ���� ȣ��.

 @param hApp [in] : Application �ڵ�
 @return ����
*/
atVOID atAPP_TickMsgProcess( atHAPP hApp )
{
	atLPAPP lpApp = atAPP_GetPtr(hApp);
	if( lpApp == atNULL ) return;

	atMSG_ProcessEvents( lpApp->hMsgProc );
}
