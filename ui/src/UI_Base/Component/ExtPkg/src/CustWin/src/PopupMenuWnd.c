//
// PopupMenuWnd.c
//
///////////////////////////////////////////////////////////////////////

#include "A_UIENG.h"
#include "UI_Util.h"
#include "PopupMenuWnd.h"
#include "AEECOMP_ExtPkg.h"


#define MENU_ANI_ENABLE (atTRUE)
//#define MENU_ANI_ENABLE (atFALSE)

#define MENU_ANI_INC 200

//#define POPUP_ITEM_HIGHT		20
#define SUB_POPUP_X				200//300
#define SUB_POPUP_MARGIN_TOP	0	// 서브 팝업 타이틀을 위해 추가

#define WNDMSG_ALL_DLG_CLOSE		WNDMSG_USERDEF+1

struct tagPOPUPMENU_DATA;

typedef struct tagPOPUPMENU_ITEM {
	struct tagPOPUPMENU_DATA *lpSubMenu;
	atINT		nID;
	atTCHAR		szText[POPUPMENU_ITEM_STR_LENGTH+1];
	atDWORD     nTextIDS;
	atBOOL		bEnable;
} POPUPMENU_ITEM;

typedef struct tagPOPUPMENU_DATA {
	struct tagPOPUPMENU_DATA *lpParentMenu;
	atINT				nItemCount;
	POPUPMENU_ITEM		lstItems[MAX_POPUPMENU_ITEM_COUNT];
	atINT				nSelIndex;
} POPUPMENU_DATA;


typedef struct _POPUPMENUWNDDATA {
	atHWINDOW		hTopWnd; // 최상위.(고정)
	atHWINDOW		hTargetWnd; // Dlg
	atHWINDOW	   hOwnerMenu;  // 상위.
	
	atHCONTROL	    hPopupMenu;
#if SUPPORT_SUBMENU_BACKBUTTON
	atHCONTROL		hBackButton;
	atTCHAR			szButton[32];
	atHBITMAP		lpNormalImg_Left,	lpNormalImg_Center,		lpNormalImg_Right;
	atHBITMAP		lpFocusedImg_Left,	lpFocusedImg_Center,	lpFocusedImg_Right;
	atHBITMAP		lpSelectedImg_Left, lpSelectedImg_Center,	lpSelectedImg_Right;
	atHBITMAP		lpDisabledImg_Left, lpDisabledImg_Center,	lpDisabledImg_Right;
	atBOOL			bSubBackBtn;
#endif
	int nItemHeight;
	int view_count;
	int align;
	int nSelectKey, nClearKey, nBackKey;
	atTCHAR szSelectKey[32], szClearKey[32], szBackKey[32];
	atINT nSelectKeyIDS, nClearKeyIDS, nBackKeyIDS;

	atHBITMAP		hImgSubMenu, hImgSubMenuSelected, hImgSubMenuDisable;
	POPUPMENU_DATA *lpMenuData;
	atCOLOR clItemColorNormal;
	atCOLOR clItemColorFocus;
	void			*lpUserData;

	atBOOL		  bAnimation;
	atHTIMER		  hAniTimer;
	int			  cur_y, goal_y, inc_y;

	atHFONT		hFont;
	atHFONT		hTitleFont;
	atLAYOUTBITMAP	bmLayout;
} POPUPMENUWNDDATA,  * LPPOPUPMENUWNDDATA;


struct _ClassExData {
	atHBITMAP		hImgSubMenu, hImgSubMenuSelected, hImgSubMenuDisable;
};
static struct _ClassExData ExData;


// Local Function .................
static void CalcMenuWindowSize( atHWINDOW hWnd, int view_count, int align );
static int	PopupMenuWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2);
POPUPMENU_ITEM *atPOPUPMENU_GetMenuItem( atHWINDOW hPopupWnd, atINT nMainIndex );
void GetSubPopUpRegion(atHWINDOW hWnd, atHWINDOW hOwnerWnd, POPUPMENU_DATA *pItem, atREGION *subItemPt );
//	void SetDataListInfo(atHWINDOW hWnd, POPUPMENU_DATA *pSubItem);
//	atHWINDOW GetTopHwindow(POPUPMENU_DATA *pItem);



atBOOL      RegistPopupMenuWndClass( atHRESSRC hResSrc )
{
	atWINCLASS wc;
	struct _ClassExData *ExData;

	atUIAPI_memset( &wc, 0, sizeof(atWINCLASS) );
	wc.dwAttr = atWS_USERDEFINE | atWS_STAYONTOP | atWS_MODAL | atWS_UNFOCUSGRAY | atWS_NOTITLE;
 	wc.dwUserDataSize = sizeof(POPUPMENUWNDDATA);
	wc.fnWndProc = PopupMenuWndOnEvent;
	
    wc.tLayoutBm.bmTopLeft      = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_mnu_tl.img") );
    wc.tLayoutBm.bmTopCenter    = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_mnu_tc.img") );
    wc.tLayoutBm.bmTopRight     = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_mnu_tr.img") );
    wc.tLayoutBm.bmMiddleLeft   = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_mnu_ml.img") );
    wc.tLayoutBm.bmMiddleCenter = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_mnu_mc.img") );
    wc.tLayoutBm.bmMiddleRight  = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_mnu_mr.img") );
    wc.tLayoutBm.bmBottomLeft   = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_mnu_bl.img") );
    wc.tLayoutBm.bmBottomCenter = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_mnu_bc.img") );
    wc.tLayoutBm.bmBottomRight  = atUIAPI_CreateBitmapFromRes( hResSrc, _AT("pop_mnu_br.img") );

	SET_MARGIN( wc.rtMargin, 10, 4, 6, 6 );
	//SET_MARGIN( wc.rtMargin, 12, 10, 6, 6 );

    ExData = (struct _ClassExData *)wc.ExData;
    ExData->hImgSubMenu				= atUIAPI_CreateBitmapFromRes( hResSrc, _AT("menu_arrow_off.img") );
	atUIAPI_SetBitmapTransColor( ExData->hImgSubMenu, atUI_COLOR_TRANS );
    ExData->hImgSubMenuSelected		= atUIAPI_CreateBitmapFromRes( hResSrc, _AT("menu_arrow_on.img") );
	atUIAPI_SetBitmapTransColor( ExData->hImgSubMenuSelected, atUI_COLOR_TRANS );
    ExData->hImgSubMenuDisable		= atNULL;

//	atUIAPI_memcpy( wc.ExData, &ExData, sizeof(struct _ClassExData) );
	
	return atAPP_RegistWinClass( atAPPMNG_GetCurApp(), EXTPKG_CUSTWIN_POPUPMENU, &wc );
}


void	UnregistPopupMenuWndClass(void)
{
	atWINCLASS *wc;
	struct _ClassExData *ExData;

    wc = atAPP_GetWinClass( atAPPMNG_GetCurApp(), EXTPKG_CUSTWIN_POPUPMENU );
    if( wc == atNULL ) return;
	
    ExData = (struct _ClassExData *)wc->ExData;
	
    atUIAPI_DestroyBitmap( ExData->hImgSubMenu );
    atUIAPI_DestroyBitmap( ExData->hImgSubMenuSelected );

	// 윈도우 레이아웃용 비트맵을 해제한다. 임시로 이렇게 했고, 나중에 이미지 관리하는 적절한 방법을 구상하자.
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmTopLeft );
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmTopCenter ); 
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmTopRight ); 
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmMiddleLeft ); 
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmMiddleCenter ); 
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmMiddleRight );
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmBottomLeft ); 
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmBottomCenter );
    atUIAPI_DestroyBitmap( wc->tLayoutBm.bmBottomRight ); 
}


void DestroyParentPopupMenuWnd(atHWINDOW hWnd)
{
/*
	LPPOPUPMENUWNDDATA lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);
	atLPPOPUPMENU lpPopupMenu = lpData->lpMenu;

	if ( lpPopupMenu != atNULL )
	{
		if ( lpPopupMenu->lpParentMenuData != atNULL )
		{
			lpPopupMenu = lpPopupMenu->lpParentMenuData;
			DestroyParentPopupMenuWnd(lpPopupMenu->lpTargetWnd);
		}
		atWINMNG_DestroyWindow(lpWnd);
	}
*/
}

static atINT xxx( atLISTBOX_EXT *lpListBox, atLPVOID lpParam )
{
	LPPOPUPMENUWNDDATA lpData = (LPPOPUPMENUWNDDATA)lpParam;
	POPUPMENU_DATA *lpMenuData = (lpData)? lpData->lpMenuData : atNULL;
	atINT nItemCount = 0;
	if( lpMenuData == atNULL ) return 0;

	nItemCount = lpMenuData->nItemCount;

	return nItemCount;
}


static atBOOL yyy( atLISTBOX_EXT *lpListBox, atLPVOID lpParam, atINT nIndex, atLPLISTBOX_EXT_ITEM lpItem )
{
	LPPOPUPMENUWNDDATA lpData = (LPPOPUPMENUWNDDATA)lpParam;
	POPUPMENU_DATA *lpMenuData = (lpData)? lpData->lpMenuData : atNULL;

	if( lpMenuData == atNULL ) return atFALSE;

	atUIAPI_memset( lpItem, 0, sizeof(atLISTBOX_EXT_ITEM) );
	atUIAPI_strncpy( lpItem->szItemName[0], lpMenuData->lstItems[nIndex].szText, atLISTBOX_EXT_STR_LEN );
	lpItem->nItemNameIds[0] = lpMenuData->lstItems[nIndex].nTextIDS;
	
	if( lpMenuData->lstItems[nIndex].lpSubMenu != atNULL ){
//		atUIAPI_strcat( lpItem->szItemName[0], _AT("  >") );
//		lpItem->imgItemImage[atLISTBOX_EXT_IMG_POST] = lpData->hImgSubMenu;
//		lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_POST].nWidth = -1;

		lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL] = lpData->hImgSubMenu;
		lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL_SELECT] = lpData->hImgSubMenuSelected;
		lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL_DISABLE] = lpData->hImgSubMenuDisable;
		lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL].nWidth = -1;
		lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL_SELECT].nWidth = -1;
		lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL_DISABLE].nWidth = -1;
/*
		lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL] = lpData->hImgSubMenu;
		lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL_SELECT] = lpData->hImgSubMenuSelected;
		lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL].nX = 0;
		lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL].nY = 0;
		lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL].nWidth = -1;
		lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL].nHeight = -1;
		lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL_SELECT].nX = 0;
		lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL_SELECT].nY = 0;
		lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL_SELECT].nWidth = -1;
		lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL_SELECT].nHeight = -1;
*/
	} else {
		lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL] = atNULL;
		lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL_SELECT] = atNULL;
		lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL_DISABLE] = atNULL;
	}

	lpItem->clItemColor[0] = lpData->clItemColorNormal;
	lpItem->clItemColor[1] = lpData->clItemColorFocus;
	lpItem->clItemColor[0] = 0xFFFFFFFF;

	lpItem->nNO = nIndex+1;
	lpItem->nItemID = lpMenuData->lstItems[nIndex].nID;
	lpItem->bEnable = lpMenuData->lstItems[nIndex].bEnable;

	return atTRUE;
}

static atBOOL zzz( atLISTBOX_EXT *lpListBox, atLPVOID lpParam, atINT nIndex, atLPLISTBOX_EXT_ITEM lpItem )
{
	LPPOPUPMENUWNDDATA lpData = (LPPOPUPMENUWNDDATA)lpParam;
	POPUPMENU_DATA *lpMenuData = (lpData)? lpData->lpMenuData : atNULL;
	static atLISTBOX_EXT_ITEM ll;

	if( lpMenuData == atNULL ) return atFALSE;
//	atUIAPI_strncpy( ll.szItemName[0], lpMenuData->lstItems[nIndex].szText, atLISTBOX_EXT_STR_LEN );
//	if( lpMenuData->lstItems[nIndex].lpSubMenu != atNULL ) atUIAPI_strcat( ll.szItemName[0], _AT(" >") );
//	ll.nNO = nIndex+1;
	lpMenuData->lstItems[nIndex].bEnable = lpItem->bEnable;

	return atTRUE;
}

static void _SetSoftKey( atHWINDOW hWnd, int is_sub )
{
	LPPOPUPMENUWNDDATA lpData = atWINDOW_GetDataPtr(hWnd);
	int soft1, soft2, soft3;
	int sel = 1, can = 2, bak = 3;	
	
	soft1 = soft2 = soft3 = 0;

	if( lpData->nSelectKey == atVKEY_SOFT1 ){
		soft1 = sel;
	} else if( lpData->nSelectKey == atVKEY_SELECT ){
		soft2 = sel;
	} else if( lpData->nSelectKey == atVKEY_SOFT2 ){
		soft3 = sel;
	}

	if( !is_sub ){
		if( lpData->nClearKey == atVKEY_SOFT1 ){
			soft1 = can;
		} else if( lpData->nClearKey == atVKEY_SELECT ){
			soft2 = can;
		} else if( lpData->nClearKey == atVKEY_SOFT2 ){
			soft3 = can;
		}
	} else {
		if( lpData->nBackKey == atVKEY_SOFT1 ){
			soft1 = bak;
		} else if( lpData->nBackKey == atVKEY_SELECT ){
			soft2 = bak;
		} else if( lpData->nBackKey == atVKEY_SOFT2 ){
			soft3 = bak;
		}
	}

#ifndef IDS_NULL
#define IDS_NULL 9999
#endif

	if( soft1 == sel ) soft1 = lpData->nSelectKeyIDS;
	else if( soft1 == can ) soft1 = lpData->nClearKeyIDS;
	else if( soft1 == bak ) soft1 = lpData->nBackKeyIDS;
	else soft1 = IDS_NULL;
	
	if( soft2 == sel ) soft2 = lpData->nSelectKeyIDS;
	else if( soft2 == can ) soft2 = lpData->nClearKeyIDS;
	else if( soft2 == bak ) soft2 = lpData->nBackKeyIDS;
	else soft2 = IDS_NULL;
	
	if( soft3 == sel ) soft3 = lpData->nSelectKeyIDS;
	else if( soft3 == can ) soft3 = lpData->nClearKeyIDS;
	else if( soft3 == bak ) soft3 = lpData->nBackKeyIDS;
	else soft3 = IDS_NULL;

	atWINDOW_SetSoftKeyIDS( hWnd, soft1, soft2, soft3 );
	atSOFTKEY_RefreshSoftKey( );
}



struct _CreateParam {
	atHWINDOW hTopWnd;
	atHWINDOW hTargetWnd;
	atHWINDOW hOwnerMenu;
	POPUPMENU_DATA *lpMenuData;
	atPOPUPMENUITEMDATA *menuitem;
	atBOOL		  bAnimation;
	int item_height;
	int menu_count;
	int view_count;
	int select;
	int align;
	atDWORD *lpMenuItemIDS;
};

atHWINDOW	CreatePopupMenuWnd0( atHWINDOW hParentWnd, int id, int x, int y, int width, int height, int item_height, atPOPUPMENUITEMDATA *menuitem, int menu_count, int view_count, int select, int align, atHCONTROL *lphControl )
{
	POPUPMENU_DATA *lpMenuData;

	atHWINDOW		hWnd;
	atMARGIN rtAnchor;
	struct _CreateParam param;
	LPPOPUPMENUWNDDATA lpData;
	int i;

	lpMenuData = __NEW(POPUPMENU_DATA);
	if( lpMenuData == atNULL ) return atNULL;
	atUIAPI_memset(lpMenuData, 0, sizeof(POPUPMENU_DATA));
	lpMenuData->nItemCount = 0;

	param.hTopWnd = atNULL;
	param.hTargetWnd = hParentWnd;
	param.menuitem = menuitem;
	param.item_height = item_height;
	param.menu_count = menu_count;
	param.view_count = view_count;
	param.select = select;
	param.align = align;
	param.hOwnerMenu = atNULL;
	param.bAnimation = MENU_ANI_ENABLE;
	
	param.lpMenuData = lpMenuData;

	SET_MARGIN( rtAnchor, atANCHOR_IGNORE, 0, atANCHOR_IGNORE, 0 );
    hWnd = atAPP_CreateWindowByClass( atAPPMNG_GetCurApp(), hParentWnd, id, _AT("PopupMenu"), 
								width, height, &rtAnchor,
								EXTPKG_CUSTWIN_POPUPMENU, &param );
	if( hWnd == atNULL ){
		__DELETE( lpMenuData );
		return atNULL;
	}

	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);
	lpData->hTopWnd = hWnd; // 생성된 핸들을 리스트에 저장시켜 놓음.
	
	for( i = 0; i < menu_count; i++ ){
		atPOPUPMENU_InsertMenuItem( hWnd, i, i, menuitem[i], atTRUE );
	}

	CalcMenuWindowSize( hWnd, lpData->view_count, lpData->align );
	atLISTBOX_EXT_ResetSelect(lpData->hPopupMenu);
	atLISTBOX_EXT_RealizeItem(lpData->hPopupMenu);

	atLISTBOX_EXT_SetSelectIndex( lpData->hPopupMenu, select );

	if( lphControl ) *lphControl = lpData->hPopupMenu;
		
	return hWnd;
}

atHWINDOW	CreatePopupMenuWnd( atHWINDOW hParentWnd, int id, int x, int y, int width, int height, int item_height, atPOPUPMENUITEMDATA *menuitem, int menu_count, int view_count, int select, int align, atHCONTROL *lphControl )
{
	atHWINDOW		hWnd;

	hWnd = CreatePopupMenuWnd0( hParentWnd, id, x, y, width, height, item_height, menuitem, menu_count, view_count, select, align, lphControl );
	if( hWnd == atNULL ) return atNULL;

	StartAnimationPopup(hWnd, 0);
	
	return hWnd;
}


atHWINDOW	CreatePopupMenuWndIDS0( atHWINDOW hParentWnd, int id, int x, int y, int width, int height, int item_height, atDWORD* menuitemIDS, int menu_count, int view_count, int select, int align, atHCONTROL *lphControl )
{
	POPUPMENU_DATA *lpMenuData;

	atHWINDOW		hWnd;
	atMARGIN rtAnchor;
	struct _CreateParam param;
	LPPOPUPMENUWNDDATA lpData;
	int i;

	lpMenuData = __NEW(POPUPMENU_DATA);
	if( lpMenuData == atNULL ) return atNULL;
	atUIAPI_memset(lpMenuData, 0, sizeof(POPUPMENU_DATA));
	lpMenuData->nItemCount = 0;

	param.hTopWnd = atNULL;
	param.hTargetWnd = hParentWnd;
	param.lpMenuData = atNULL;
	param.menuitem = atNULL;//menuitem;
	param.lpMenuItemIDS = menuitemIDS;
	param.item_height = item_height;
	param.menu_count = menu_count;
	param.view_count = view_count;
	param.select = select;
	param.align = align;
	param.hOwnerMenu = atNULL;
	param.lpMenuData = lpMenuData;
	param.bAnimation = MENU_ANI_ENABLE;

	SET_MARGIN( rtAnchor, atANCHOR_IGNORE, 0, 0, atANCHOR_IGNORE );
	hWnd = atAPP_CreateWindowByClass( atAPPMNG_GetCurApp(), hParentWnd, id, _AT("PopupMenu"), 
								width, height, &rtAnchor,
								EXTPKG_CUSTWIN_POPUPMENU, &param );
	if( hWnd == atNULL ){
		__DELETE( lpMenuData );
		return atNULL;
	}

	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);
	lpData->hTopWnd = hWnd; // 생성된 핸들을 리스트에 저장시켜 놓음.

	for( i = 0; i < menu_count; i++ ){
		atPOPUPMENU_InsertMenuItemIDS( hWnd, i, i, menuitemIDS[i], atTRUE );
	}

	CalcMenuWindowSize( hWnd, lpData->view_count, lpData->align );
	atLISTBOX_EXT_ResetSelect(lpData->hPopupMenu);
	atLISTBOX_EXT_RealizeItem(lpData->hPopupMenu);
	
	atLISTBOX_EXT_SetSelectIndex( lpData->hPopupMenu, select );

	if( lphControl ) *lphControl = lpData->hPopupMenu;
		
	return hWnd;
}

atHWINDOW	CreatePopupMenuWndIDS( atHWINDOW hParentWnd, int id, int x, int y, int width, int height, int item_height, atDWORD* menuitemIDS, int menu_count, int view_count, int select, int align, atHCONTROL *lphControl )
{
	atHWINDOW		hWnd;

	hWnd = CreatePopupMenuWndIDS0( hParentWnd, id, x, y, width, height, item_height, menuitemIDS, menu_count, view_count, select, align, lphControl );
	if( hWnd == atNULL ) return atNULL;

	StartAnimationPopup(hWnd, 0);
	
	return hWnd;
}


static atHWINDOW	CreatePopupMenuWnd2(atHWINDOW hWndTop, atHWINDOW hParentWnd, atHWINDOW hParentMenuWnd, int id, int x, int y, int width, int height, int item_height, POPUPMENU_DATA *lpMenuData, int view_count, int select, atREGION subRegion, int align )
{
	atHWINDOW		hWnd;
	atMARGIN rtAnchor;
	atMARGIN rtMargin;
	atLAYOUTBITMAP bm;
	struct _CreateParam param;
	LPPOPUPMENUWNDDATA lpData, lpParentData, lpTopData;
	atLPLISTBOX_EXT lpListBox;
	atCONTROL_COLOR cl;

	param.hTopWnd = hWndTop;
	param.hTargetWnd = hParentWnd;
	param.menuitem = atNULL;
	param.item_height = item_height;
	param.menu_count = lpMenuData->nItemCount;
	param.view_count = view_count;
	param.select = select;
	param.align = align;
	param.hOwnerMenu = hParentMenuWnd;
	param.lpMenuData = lpMenuData;
	param.bAnimation = MENU_ANI_ENABLE;

	//SubText위치 설정함. 	//Top, bottom, left, right
	//SET_MARGIN( rtAnchor, atANCHOR_IGNORE, 0, 20, atANCHOR_IGNORE );
	SET_MARGIN( rtAnchor, subRegion.nY, subRegion.nHeight, subRegion.nX, subRegion.nWidth);

	hWnd = atAPP_CreateWindowByClass( atAPPMNG_GetCurApp(), hParentMenuWnd, id, _AT("PopupMenu2"),
								width, height, &rtAnchor,
								EXTPKG_CUSTWIN_POPUPMENU, &param );
	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);
//	lpData->hTopWnd = hWndTop; // 생성된 핸들을 리스트에 저장시켜 놓음. Top Windows 넣음.
//	lpData->lpMenuData = lpMenuData;
//	atCONTROL_SetUserDataPtr( lpData->hPopupMenu, lpData );

	// 부모 메뉴 스타일을 일치시킨다.
	atWINDOW_SetAttr( hWnd, atWINDOW_GetAttr(hParentMenuWnd) );
	atWINDOW_GetLayoutMargin( hParentMenuWnd, &rtMargin );
	rtMargin.nTop += SUB_POPUP_MARGIN_TOP;
	atWINDOW_SetLayoutMargin2( hWnd, &rtMargin );

	// 상단 이미지만 있으면 적용시키자
	lpTopData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWndTop);
	if(lpTopData && (lpTopData->bmLayout.bmTopLeft && lpTopData->bmLayout.bmTopCenter && lpTopData->bmLayout.bmTopRight) ) {
		atWINDOW_SetLayoutBitmap2( hWnd, &lpTopData->bmLayout );
	} else {	
		atWINDOW_GetLayoutBitmap( hParentMenuWnd, &bm );
		atWINDOW_SetLayoutBitmap2( hWnd, &bm );
	}
	
	lpParentData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hParentMenuWnd);
	atCONTROL_SetAttr( lpData->hPopupMenu, atCONTROL_GetAttr(lpParentData->hPopupMenu) );
	atCONTROL_GetControlColor( lpParentData->hPopupMenu, &cl );
	atCONTROL_SetControlColor( lpData->hPopupMenu, &cl );
	lpData->clItemColorNormal = lpParentData->clItemColorNormal;
	lpData->clItemColorFocus = lpParentData->clItemColorFocus;
	lpData->hImgSubMenu = lpParentData->hImgSubMenu;
	lpData->hImgSubMenuSelected = lpParentData->hImgSubMenuSelected;
	lpData->hImgSubMenuDisable = lpParentData->hImgSubMenuDisable;
	lpData->nSelectKey = lpParentData->nSelectKey;
	lpData->nClearKey = lpParentData->nClearKey;
	lpData->nBackKey = lpParentData->nBackKey;
	atUIAPI_strncpy( lpData->szSelectKey, lpParentData->szSelectKey, 32 );
	atUIAPI_strncpy( lpData->szClearKey, lpParentData->szClearKey, 32 );
	atUIAPI_strncpy( lpData->szBackKey, lpParentData->szBackKey, 32 );
	lpData->nSelectKeyIDS = lpParentData->nSelectKeyIDS;
	lpData->nClearKeyIDS = lpParentData->nClearKeyIDS;
	lpData->nBackKeyIDS = lpParentData->nBackKeyIDS;
	lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( lpParentData->hPopupMenu );
	if( lpListBox != atNULL ){
		atLISTBOX_EXT_SetBitmap( lpData->hPopupMenu, 
			lpListBox->hImgNormalLeft, lpListBox->hImgNormalCenter, lpListBox->hImgNormalRight,
			lpListBox->hImgSelectLeft, lpListBox->hImgSelectCenter, lpListBox->hImgSelectRight,
			lpListBox->hImgDisableLeft, lpListBox->hImgDisableCenter, lpListBox->hImgDisableRight );
	}
	CalcMenuWindowSize( hWnd, lpData->view_count, lpData->align );
	atLISTBOX_EXT_ResetSelect(lpData->hPopupMenu);
	atLISTBOX_EXT_RealizeItem(lpData->hPopupMenu);
	
	//atLISTBOX_EXT_SetSelectIndex( lpData->hPopupMenu, select );

	//_SetSoftKey( hWnd, atTRUE );
	
	if( lpData->bAnimation ){
		atREGION rgn, rgn2;
		atWINDOW_GetWindowRegion( hWnd, &rgn );
//		atWINDOW_SetWindowRegion( hWnd, rgn.nX, rgn.nY, rgn.nWidth, 0 );
		atDESKTOP_GetDesktopArea( atDESKTOP_AREA_CLIENT, &rgn2 );
		lpData->cur_y = rgn2.nHeight;
		atWINDOW_SetPosition( hWnd, rgn.nX, lpData->cur_y );
//		atAPP_AdjustWindowArea( atAPPMNG_GetCurApp(), hWnd );
	}
	atAPP_ShowWindow( atAPPMNG_GetCurApp(), hWnd, atTRUE );
	if( lpData->bAnimation ){
		lpData->inc_y = -MENU_ANI_INC;
		atWINDOW_EnableTimer( hWnd, lpData->hAniTimer, atTRUE );
	}

	return hWnd;
}


static void CalcMenuWindowSize( atHWINDOW hWnd, int view_count, int align )
{
	atLPWINDOW	lpWnd = atWINDOW_GetPtr(hWnd);
	LPPOPUPMENUWNDDATA lpData = atWINDOW_GetDataPtr(hWnd);
	int itm_height;
	int itm_count;
	int all_height;
	atSIZE sz;
	atREGION rgn, dskarea;

	atWINDOW_GetWindowRegion( hWnd, &rgn );

	itm_count = (lpData->lpMenuData != atNULL)? lpData->lpMenuData->nItemCount : 0;
	itm_height = lpData->nItemHeight;

	itm_count = MIN(view_count, itm_count);
	//all_height = 4*2 + (itm_height + 4) * itm_count - 4;
	all_height = itm_height * itm_count;

#if SUPPORT_SUBMENU_BACKBUTTON
	if(lpData->hOwnerMenu &&  lpData->hOwnerMenu == lpData->hTopWnd && lpData->bSubBackBtn)
		all_height = itm_height * (itm_count+1);
#endif
	
	SET_SIZE( sz, 100, all_height );
	atWINDOW_GetClientAdjustSize( hWnd, &sz, &sz );

	if( lpData->bAnimation ){
		atDESKTOP_GetDesktopArea( atDESKTOP_AREA_CLIENT, &dskarea );
		rgn.nY = dskarea.nY + dskarea.nHeight;
		rgn.nHeight = sz.nHeight;
		lpData->goal_y = rgn.nY - sz.nHeight;
	} else {
		atDESKTOP_GetDesktopArea( atDESKTOP_AREA_CLIENT, &dskarea );
		rgn.nHeight = sz.nHeight;
		rgn.nY = dskarea.nY + dskarea.nHeight - sz.nHeight;
	}
	
	atWINDOW_SetWindowRegion( hWnd, rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight );
}


static void PopupMenuWndOnInit( atHWINDOW hWnd, struct _CreateParam *lpParam, struct _ClassExData *ExData )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);
	LPPOPUPMENUWNDDATA	lpOwnerData = atNULL;
//	atRECT			rect;

	// 윈도우 레이아웃을 설정한다. 임시로 이렇게 했고, 나중에 이미지 관리하는 적절한 방법을 구상하자.
//    atWINDOW_SetLayoutBitmap2( hWnd, &ExData->WinLayout );
//    atWINDOW_SetLayoutMargin2( hWnd, &ExData->margin );

	lpData->hTargetWnd = lpParam->hTargetWnd;	
	lpData->hOwnerMenu = lpParam->hOwnerMenu;

	lpData->hTopWnd = lpParam->hTopWnd; /// ???

	lpData->lpMenuData = lpParam->lpMenuData;
	
	lpData->nItemHeight = lpParam->item_height;
	lpData->view_count = lpParam->view_count;
	lpData->align = lpParam->align;
	
	lpData->nSelectKey = atVKEY_SELECT;
	lpData->nClearKey = atVKEY_CLEAR;
	lpData->nBackKey = atVKEY_CLEAR;
	atUIAPI_strcpy( lpData->szSelectKey, _AT("OK") );
	atUIAPI_strcpy( lpData->szClearKey, _AT("Cancel") );
	atUIAPI_strcpy( lpData->szBackKey, _AT("Back") );
	lpData->nSelectKeyIDS = 0;
	lpData->nClearKeyIDS = 0;
	lpData->nBackKeyIDS = 0;
	
	lpData->clItemColorNormal = atUI_COLOR_BLACK;
	lpData->clItemColorFocus = atUI_COLOR_WHITE;
		
	lpData->hImgSubMenu = (ExData)? ExData->hImgSubMenu : atNULL;
	lpData->hImgSubMenuSelected = (ExData)? ExData->hImgSubMenuSelected : atNULL;
	lpData->hImgSubMenuDisable = (ExData)? ExData->hImgSubMenuDisable : atNULL;

#if SUPPORT_SUBMENU_BACKBUTTON
	lpOwnerData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(lpData->hOwnerMenu);
	if(lpOwnerData && lpOwnerData->bSubBackBtn)
	{
		lpData->bSubBackBtn = lpOwnerData->bSubBackBtn;
		atUIAPI_strncpy(lpData->szButton, lpOwnerData->szButton, 32);

		lpData->lpNormalImg_Left	= lpOwnerData->lpNormalImg_Left;
		lpData->lpNormalImg_Center	= lpOwnerData->lpNormalImg_Center;
		lpData->lpNormalImg_Right	= lpOwnerData->lpNormalImg_Right;

		lpData->lpFocusedImg_Left	= lpOwnerData->lpFocusedImg_Left;
		lpData->lpFocusedImg_Center	= lpOwnerData->lpFocusedImg_Center;
		lpData->lpFocusedImg_Right	= lpOwnerData->lpFocusedImg_Right;

		lpData->lpSelectedImg_Left	= lpOwnerData->lpSelectedImg_Left;
		lpData->lpSelectedImg_Center= lpOwnerData->lpSelectedImg_Center;
		lpData->lpSelectedImg_Right	= lpOwnerData->lpSelectedImg_Right;

		lpData->lpDisabledImg_Left	= lpOwnerData->lpDisabledImg_Left;
		lpData->lpDisabledImg_Center= lpOwnerData->lpDisabledImg_Center;
		lpData->lpDisabledImg_Right	= lpOwnerData->lpDisabledImg_Right;
	}
#endif

	//CalcMenuWindowSize( hWnd, lpData->view_count, lpData->align );

	lpData->bAnimation = lpParam->bAnimation;
	if( lpParam->bAnimation ){
		lpData->cur_y = 0;
		lpData->hAniTimer = atWINDOW_AddTimer( hWnd, 0, 40, atTRUE, atFALSE, atFALSE );	
	} else {
		lpData->hAniTimer = atNULL;
	}
	
//	atWINDOW_GetClientRect( hWnd, &rect ); 
//	lpData->hPopupMenu = ExtPkg_CreatePopupMenu( hWnd, 101, 1, 1, -1, -1, lpParam->menuitem, lpParam->count, atTRUE, 0 );
//	lpData->hPopupMenu = ExtPkg_CreateListBox2( hWnd, 101, 1, 1, -1, -1, 22, atTRUE, atFALSE, atTRUE, lpParam->menuitem, lpParam->count );
//	lpData->hPopupMenu = ExtPkg_CreateListBox( hWnd, 101, 1, 1, -1, -1, 22, atTRUE, atFALSE, atTRUE );
	lpData->hPopupMenu = ExtPkg_CreateListBox( hWnd, 101, 0, 0, 100, 100, lpParam->item_height, atFALSE, atFALSE, atTRUE );
	atCONTROL_SetAnchor( lpData->hPopupMenu, 0, 0, 0, 0 );
	atLISTBOX_EXT_SetItemMargin(lpData->hPopupMenu, 4, 0);
	atLISTBOX_EXT_SetItemGap(lpData->hPopupMenu, 0);

	atLISTBOX_EXT_SetAttr( lpData->hPopupMenu, atLISTBOX_EXT_GetAttr(lpData->hPopupMenu) | atLISTBOX_EXT_ATTR_MOVE_WRAP );
	atLISTBOX_EXT_SetupFunc( lpData->hPopupMenu, atNULL, atNULL, atNULL, xxx, yyy, zzz, lpData );

#if SUPPORT_SUBMENU_BACKBUTTON
	if(lpData->hOwnerMenu &&  lpData->hOwnerMenu == lpData->hTopWnd && lpData->bSubBackBtn)
	{
		lpOwnerData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(lpData->hOwnerMenu);

		lpData->hBackButton = ExtPkg_CreateButton(hWnd, 102, 1, 1, 1, 62, lpData->szButton );
		atCONTROL_SetAnchor( lpData->hBackButton, lpParam->item_height*(lpData->view_count-2)+5, atANCHOR_IGNORE, 10, 10);
		atBUTTON_EXT_SetBitmap( lpData->hBackButton, 
								lpData->lpNormalImg_Left, lpData->lpNormalImg_Center, lpData->lpNormalImg_Right,
								lpData->lpFocusedImg_Left, lpData->lpFocusedImg_Center, lpData->lpFocusedImg_Right,
								lpData->lpSelectedImg_Left, lpData->lpSelectedImg_Center, lpData->lpSelectedImg_Right,
								lpData->lpDisabledImg_Left, lpData->lpDisabledImg_Center, lpData->lpDisabledImg_Right );
		atCONTROL_SetFocusable( lpData->hBackButton, atFALSE );
		atWINDOW_SetCurControl( hWnd, lpData->hPopupMenu );
	}
#endif

	atCONTROL_SetUserDataPtr( lpData->hPopupMenu, lpData );
	
//	SetSoftKey(hWnd, SOFTKEY_CLOSE, SOFTKEY_OK, SOFTKEY_NULL);
//	_SetSoftKey( hWnd, (lpData->lpMenuData->lpParentMenu != atNULL) );
}

static void PopupMenuWndOnRelease(atHWINDOW hWnd)
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);

//	atPOPUPMENU_RemoveAllMenuItem( hWnd );
	
	atCONTROL_SetUserDataPtr( lpData->hPopupMenu, atNULL );
	if( lpData->lpMenuData != atNULL ){
		// 실제 메뉴 데이터는 최상위 부모만 가지고 있고, 서브 메뉴는 가지고 있지 않기 때문에, 선별하여 삭제
		if( lpData->lpMenuData->lpParentMenu == atNULL ){
			atPOPUPMENU_RemoveAllMenuItem( hWnd );
			__DELETE( lpData->lpMenuData );

			if( lpData->hFont )
			{
				atUIAPI_DestroyFont( lpData->hFont );
				lpData->hFont = NULL;
			}

			if( lpData->hTitleFont )
			{
				atUIAPI_DestroyFont( lpData->hTitleFont );
				lpData->hTitleFont = NULL;
			}
		}
	}
}

static void PopupMenuWndOnTimer( atHWINDOW hWnd, long nParam1, long nParam2 )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);
	atREGION rgn;
	atINT inc_y = 0;
	atINT isGap = 0;
	atBOOL is_stop = atFALSE;

	atWINDOW_GetWindowRegion( hWnd, &rgn );
#if 1

	inc_y = lpData->inc_y;

	if( ABS(lpData->goal_y-lpData->cur_y) >= 80 ){
		inc_y /= 2;
	} else if( ABS(lpData->goal_y-lpData->cur_y) >= 50 ){
		inc_y /= 4;
	} else if( ABS(lpData->goal_y-lpData->cur_y) >= 30 ){
		inc_y /= 8;
	} else if( ABS(lpData->goal_y-lpData->cur_y) >= 10 ){
		inc_y /= 16;
	}
	if( ABS(inc_y) < 1 ) inc_y = (inc_y>0)? 1 : -1;

	if( lpData->cur_y < 0 ) lpData->cur_y = 0;
	lpData->cur_y += inc_y;

#else

	isGap = lpData->cur_y*2 + 8;

	lpData->cur_y = isGap;
#endif


	// Time 종료 조건.
	if(ABS(lpData->goal_y-lpData->cur_y) <= ABS(inc_y)){
		lpData->cur_y = lpData->goal_y;
		atWINDOW_EnableTimer( hWnd, lpData->hAniTimer, atFALSE );
		is_stop = atTRUE;
	} 
	
	rgn.nY = /*rgn.nY -*/ lpData->cur_y;// + rgn.nHeight;
	atWINDOW_Refresh( hWnd ); // 이전 것 갱신 영역에 추가
	atWINDOW_SetPosition( hWnd, rgn.nX, rgn.nY );
	atWINDOW_Refresh( hWnd );	

	if( is_stop ){
		atWINDOW_UseAnchor( hWnd, atTRUE );	
	}
}

static void PopupMenuWndOnCommand(atHWINDOW hWnd, long nParam1, long nParam2)
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);
//	atHGDC gdc;
//	atREGION rgn;
	POPUPMENU_ITEM *lpMenuItem;
	int idx;
	atHWINDOW hSubPopup;
			
	if ( nParam1 == 101 ){
		idx = atLISTBOX_EXT_GetSelectIndex(lpData->hPopupMenu);
		lpMenuItem = atPOPUPMENU_GetMenuItem( hWnd,  idx );
		if( lpMenuItem != atNULL && lpMenuItem->lpSubMenu ){
			atREGION subItemPt; //상자가 표시해야되는 위치를 설정함.
			GetSubPopUpRegion(lpData->hTopWnd, lpData->hTargetWnd, lpMenuItem->lpSubMenu, &subItemPt ); // SubPopUp 위치 조정.

			hSubPopup = CreatePopupMenuWnd2( lpData->hTopWnd, lpData->hTargetWnd, hWnd, atWINDOW_GetID(hWnd), 0, 0, 200, 100, lpData->nItemHeight, lpMenuItem->lpSubMenu, lpData->view_count, lpMenuItem->lpSubMenu->nSelIndex, subItemPt, MENU_ALIGN_RIGHT | MENU_ALIGN_BOTTOM );

			atWINDOW_SetVisible( lpData->hTopWnd, atFALSE );

			if( lpData->hFont )
			{
				LPPOPUPMENUWNDDATA lpSubData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hSubPopup);
				lpSubData->hFont = lpData->hFont;

				atCONTROL_SetFont( lpSubData->hPopupMenu, lpSubData->hFont );
	#if SUPPORT_SUBMENU_BACKBUTTON
				if(lpSubData->bSubBackBtn)
					atCONTROL_SetFont( lpSubData->hBackButton, lpSubData->hFont );
	#endif
			}

			if( lpMenuItem->nTextIDS )
				atWINDOW_SetTitleIDS( hSubPopup, lpMenuItem->nTextIDS );
			else
				atWINDOW_SetTitleText( hSubPopup, lpMenuItem->szText );
	//		CreatePopupMenuWnd2( lpData->hTargetWnd, hWnd, lpMenuItem->lpSubMenu->lstItems[0].nID, 0, 0, 200, 100, lpMenuItem->lpSubMenu, lpData->view_count, subItemPt, MENU_ALIGN_RIGHT | MENU_ALIGN_BOTTOM );
		} else {
			atWINDOW_PostMessage( lpData->hTargetWnd, WNDMSG_COMMAND, atWINDOW_GetID(hWnd), lpMenuItem->nID );

			if ( lpData->hOwnerMenu != atNULL ){
				atHWINDOW hParentWnd = lpData->hOwnerMenu;
				atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
				atAPP_SetTopWindow( atAPPMNG_GetCurApp(), hParentWnd );
				//atAPP_SetCurWindow( atAPPMNG_GetCurApp(), hParentWnd );

				//Sub메뉴인경우 팝업의 모든 dlg를 닫는다.
				atWINDOW_PostMessage( hParentWnd, WNDMSG_ALL_DLG_CLOSE, 0, 0 );

			} else {
				atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			}
		}
#if SUPPORT_SUBMENU_BACKBUTTON
	} else if( nParam1 == 102 )
	{
		if ( lpData->hOwnerMenu != atNULL ){
			atHWINDOW hParentWnd = lpData->hOwnerMenu;
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			atWINDOW_SetVisible( hParentWnd, atTRUE );
			atAPP_SetTopWindow( atAPPMNG_GetCurApp(), hParentWnd );
			//atAPP_SetCurWindow( atAPPMNG_GetCurApp(), hParentWnd );
		}
	}
#endif
}

static int PopupMenuWndOnKeyDown(atHWINDOW hWnd, int nKeyCode)
{
	atLPWINDOW	lpWnd = atWINDOW_GetPtr(hWnd);
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);
//	atLPPOPUPMENU lpMenuData = GetMenuData(hWnd);
	POPUPMENU_ITEM *lpMenuItem;
	int idx;

#if 0
	if( nKeyCode == lpData->nSelectKey ){
		atCONTROL_ProcessControlProc( lpData->hPopupMenu, CTLMSG_KEYDOWN, atVKEY_SELECT, 0 );
		return 1;
	} else if( nKeyCode == lpData->nClearKey ){
//		atWINDOW_PostMessage( atWINDOW_GetParentWindow(hWnd), WNDMSG_COMMAND, atWINDOW_GetID(hWnd), POPUPMENU_BACK );
		atWINDOW_PostMessage( lpData->hTargetWnd, WNDMSG_COMMAND, atWINDOW_GetID(hWnd), POPUPMENU_BACK );
		atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
		return 1;
	} else if( nKeyCode == lpData->nBackKey ){
		nKeyCode= atVKEY_LEFT;
	}
#endif

	switch ( nKeyCode ){
		// fix 소프트키는 탑윈도우로 메시지를 전달하고 무조건 종료되도록
		// param1:키, param2:현재 팝업 아이디(다른거랑 순서가 다름)
		case atVKEY_SOFT1:
		case atVKEY_SOFT2:
		case atVKEY_SELECT:
			if ( lpData->hOwnerMenu != atNULL ){
				atHWINDOW hParentWnd = lpData->hOwnerMenu;
				atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
				atAPP_SetTopWindow( atAPPMNG_GetCurApp(), hParentWnd );
				//atAPP_SetCurWindow( atAPPMNG_GetCurApp(), hParentWnd );

				//Sub메뉴인경우 팝업의 모든 dlg를 닫는다.
				atWINDOW_PostMessage( hParentWnd, WNDMSG_ALL_DLG_CLOSE, 0, 0 );

			} else {
				atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			}
			break;

	case atVKEY_EXIT:
		atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
//		DestroyParentPopupMenuWnd(hWnd);
//		RepaintRect( 0, GetAnnounceHeight(), atUIAPI_GetScreenWidth(), GetScreenHeight());
//		atWINMNG_RedrawAllWindow();
		break;

	case atVKEY_RIGHT:
		idx = atLISTBOX_EXT_GetSelectIndex(lpData->hPopupMenu);
		lpMenuItem = atPOPUPMENU_GetMenuItem( hWnd,  idx );
		if( lpMenuItem != atNULL && lpMenuItem->lpSubMenu ){

			atREGION subItemPt; //상자가 표시해야되는 위치를 설정함.
			GetSubPopUpRegion(lpData->hTopWnd, lpData->hTargetWnd, lpMenuItem->lpSubMenu, &subItemPt ); // SubPopUp 위치 조정.

//			CreatePopupMenuWnd2( atWINDOW_GetParentWindow(hWnd), hWnd, (atWINDOW_GetID(hWnd) << 16) | idx, 0, 0, 200, 100, lpMenuItem->lpSubMenu, lpData->view_count, MENU_ALIGN_RIGHT | MENU_ALIGN_BOTTOM );
			CreatePopupMenuWnd2( lpData->hTopWnd, lpData->hTargetWnd, hWnd, atWINDOW_GetID(hWnd), 0, 0, 200, 100, lpData->nItemHeight, lpMenuItem->lpSubMenu, lpData->view_count, lpMenuItem->lpSubMenu->nSelIndex, subItemPt, MENU_ALIGN_RIGHT | MENU_ALIGN_BOTTOM );

		}
		break;
	case atVKEY_CLEAR:
	case atVKEY_LEFT:
		if ( lpData->hOwnerMenu != atNULL ){
			atHWINDOW hParentWnd = lpData->hOwnerMenu;
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			atAPP_SetTopWindow( atAPPMNG_GetCurApp(), hParentWnd );
			//atAPP_SetCurWindow( atAPPMNG_GetCurApp(), hParentWnd );
		}
		break;
	default:
		return 0;
	}

	return 0; // WindowOnEvent 가 호출되지 않도록 1을 리턴한다.
}

static int PopupMenuPaintLayout( hWnd )
{
	atLPWINDOW lpWnd = atWINDOW_GetPtr(hWnd);
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);
	atHGDC hGDC = atWINDOW_GetWindowGDC(hWnd);
	atHFONT fnt = NULL;
	atTCHAR str[256] = _AT("");

	atUIAPI_DrawClient( hGDC, 0, 0, RECT_WIDTH(lpWnd->Base.rect), RECT_HEIGHT(lpWnd->Base.rect),
		lpWnd->lpLayoutBm.bmTopLeft,    lpWnd->lpLayoutBm.bmTopCenter,    lpWnd->lpLayoutBm.bmTopRight,
		lpWnd->lpLayoutBm.bmMiddleLeft, lpWnd->lpLayoutBm.bmMiddleCenter, lpWnd->lpLayoutBm.bmMiddleRight,
		lpWnd->lpLayoutBm.bmBottomLeft, lpWnd->lpLayoutBm.bmBottomCenter, lpWnd->lpLayoutBm.bmBottomRight,
		!(lpWnd->nWindowStyle & atWS_NONECLIENT) );

	//if(lpWnd->nTitleIDS)
	//	atUIAPI_LoadString(lpWnd->nTitleIDS, lpWnd->szWindowTitle, WINDOWTITLE_MAX_LENGTH);

	//if( lpData->hTitleFont )
	//	fnt = atUIAPI_SetFont( hGDC, lpData->hTitleFont );
	//else if( lpData->hFont )
	//	fnt = atUIAPI_SetFont( hGDC, lpData->hFont );

	//myStringCut( hGDC,  lpWnd->szWindowTitle, str, RECT_WIDTH(lpWnd->Base.rect)-lpWnd->Base.rtLayoutMargin.nLeft - lpWnd->Base.rtLayoutMargin.nRight, atTRUE );

	//atUIAPI_DrawStringAlign(hGDC, RECT_WIDTH(lpWnd->Base.rect)/2, lpWnd->Base.rtLayoutMargin.nTop/2, str, ALIGN_CENTER_MIDDLE, UI_COLOR_WHITE);
	//
	//if( fnt )
	//	atUIAPI_SetFont( hGDC, fnt );

	return 1;
}

int PopupMenuWndOnMouseDown(atHWINDOW hWnd, int nButton, int nMosueX, int nMouseY )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);

	atRECT rc;
	atWINDOW_GetWindowRect(hWnd, &rc);

	if(!atUIAPI_IsPtInRect(nMosueX, nMouseY, &rc))
	{
		if ( lpData->hOwnerMenu != atNULL ){
			atHWINDOW hParentWnd = lpData->hOwnerMenu;
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
			atAPP_SetTopWindow( atAPPMNG_GetCurApp(), hParentWnd );
			//atAPP_SetCurWindow( atAPPMNG_GetCurApp(), hParentWnd );

			//Sub메뉴인경우 팝업의 모든 dlg를 닫는다.
			atWINDOW_PostMessage( hParentWnd, WNDMSG_ALL_DLG_CLOSE, 0, 0 );

		} else {
			atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
		}
	}

	return 1;
}

int PopupMenuWndOnEvent(atHWINDOW hWnd, int nMsg, long nParam1, long nParam2)
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);

	switch( nMsg )
	{
		case WNDMSG_INIT:
			PopupMenuWndOnInit( hWnd, (void *)nParam1, (void *)nParam2 );
			break;
		case WNDMSG_RELEASE:
			PopupMenuWndOnRelease( hWnd );
			break;
		case WNDMSG_KEYDOWN:
			return PopupMenuWndOnKeyDown( hWnd, nParam1 );
			break;
		case WNDMSG_PAINTLAYOUT:
			return PopupMenuPaintLayout( hWnd );
			break;

		case WNDMSG_PREPAINT:
/*			
			gdc = atUIAPI_GetScreenCanvas();
			atDESKTOP_GetDesktopArea( atDESKTOP_AREA_CLIENT, &rgn );
//			atDESKTOP_GetDesktopArea( atDESKTOP_AREA_EMBEDED_ANNBAR, &rgn );
			atUIAPI_FillRectTrans(gdc, rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight, atUI_COLOR_BLACK, 0xff/2);  // 50%가 가장 빠름.
*/			
			return 1; // 윈도우 프레임을 그리도록 바이패스
			break;
		case WNDMSG_POSTPAINT:
//			gdc = atUIAPI_GetScreenCanvas();
//			atDESKTOP_GetDesktopArea( atDESKTOP_AREA_CLIENT, &rgn );
//			atUIAPI_FlushScreenRgn( rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight );
			return 0; // 윈도우 프레임을 그리도록 바이패스
			break;
		case WNDMSG_TIMER:
			PopupMenuWndOnTimer( hWnd, nParam1, nParam2 );
			break;
		case WNDMSG_COMMAND:
			PopupMenuWndOnCommand( hWnd, nParam1, nParam2 );
			break;
		case WNDMSG_MOUSEDOWN:
			PopupMenuWndOnMouseDown( hWnd, nParam1, (nParam2 >> 16) & 0xFFFF, nParam2 & 0xffff );
			break;
	
		case WNDMSG_ALL_DLG_CLOSE:
			{
				atHWINDOW hParentWnd = lpData->hOwnerMenu;
				if(!hParentWnd)
					atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
				else //아직 dlg가 존재하면 자신을 호출하며 열려있는 모든 팝업 dlg를 닫는다.
				{
					atAPP_DestroyWindow( atAPPMNG_GetCurApp(), hWnd );
					atAPP_SetTopWindow( atAPPMNG_GetCurApp(), hParentWnd );
					//atAPP_SetCurWindow( atAPPMNG_GetCurApp(), hParentWnd );

					atWINDOW_PostMessage( hParentWnd, WNDMSG_USERDEF+1, 0, 0 );
				}
			}
			break;

		default:
			return 0;
			break;
	}

	return 1;
}


//=========================================

POPUPMENU_ITEM *atPOPUPMENU_GetMenuItem( atHWINDOW hPopupWnd, atINT nMainIndex )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hPopupWnd);
	POPUPMENU_ITEM *lpItem;

	if( nMainIndex >= lpData->lpMenuData->nItemCount ) return atFALSE;
	
	lpItem = &lpData->lpMenuData->lstItems[nMainIndex];

	return lpItem;
}

static void _DeleteMenuData( POPUPMENU_DATA *lpMenuData )
{
	POPUPMENU_ITEM *lpItem;
	int i, cnt;

	if( lpMenuData == atNULL ) return;
	cnt = lpMenuData->nItemCount;
	if( cnt <= 0 ) return;
	
	for( i = 0; i < cnt; i++ ){
		lpItem = &lpMenuData->lstItems[i];
		if( lpItem->lpSubMenu != atNULL ){
			_DeleteMenuData( lpItem->lpSubMenu );
			lpItem->lpSubMenu = atNULL;
		}
	}
	
	lpMenuData->nItemCount = 0;

	__DELETE( lpMenuData );
}
atVOID atPOPUPMENU_RemoveAllMenuItem( atHWINDOW hPopupWnd )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hPopupWnd);

	_DeleteMenuData( lpData->lpMenuData );
	
	lpData->lpMenuData = atNULL;
}

atINT atPOPUPMENU_InsertMenuItem( atHWINDOW hPopupWnd, atINT nIndex, atINT nID, atLPTSTR szText, atBOOL bEnable )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hPopupWnd);
	POPUPMENU_DATA *lpMenuData;
	POPUPMENU_ITEM *lpItem;
	int i, cnt;

	lpMenuData = lpData->lpMenuData;
	if( lpMenuData == atNULL ) return -1;

	cnt = lpMenuData->nItemCount;
	if( cnt >= MAX_POPUPMENU_ITEM_COUNT ) return -1;

	if( nIndex < cnt ){
		for( i = cnt-1; i >= nIndex; i-- ){		
			lpMenuData->lstItems[i+1] = lpMenuData->lstItems[i];
		}
	} else if( nIndex > cnt ) nIndex = cnt;
	
	lpItem = &lpMenuData->lstItems[nIndex];
	lpItem->nID = nID;
	lpItem->lpSubMenu = atNULL;
	lpItem->bEnable = bEnable;
	lpItem->nTextIDS = 0;
	atUIAPI_strncpy( lpItem->szText, szText, POPUPMENU_ITEM_STR_LENGTH );
	
	lpMenuData->nItemCount++;	
	
	atLISTBOX_EXT_RealizeItem( lpData->hPopupMenu );

	return lpMenuData->nItemCount;
}


atINT atPOPUPMENU_InsertMenuItemIDS( atHWINDOW hPopupWnd, atINT nIndex, atINT nID, atDWORD textIDS, atBOOL bEnable )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hPopupWnd);
	POPUPMENU_DATA *lpMenuData;
	POPUPMENU_ITEM *lpItem;
	int i, cnt;

	lpMenuData = lpData->lpMenuData;
	if( lpMenuData == atNULL ) return -1;

	cnt = lpMenuData->nItemCount;
	if( cnt >= MAX_POPUPMENU_ITEM_COUNT ) return -1;

	if( nIndex < cnt ){
		for( i = cnt-1; i >= nIndex; i-- ){		
			lpMenuData->lstItems[i+1] = lpMenuData->lstItems[i];
		}
	} else if( nIndex > cnt ) nIndex = cnt;
	
	lpItem = &lpMenuData->lstItems[nIndex];
	lpItem->nID = nID;
	lpItem->lpSubMenu = atNULL;
	lpItem->bEnable = bEnable;
	lpItem->szText[0] = 0;
	lpItem->nTextIDS = textIDS;
	
	lpMenuData->nItemCount++;	
	
	atLISTBOX_EXT_RealizeItem( lpData->hPopupMenu );

	return lpMenuData->nItemCount;
}


atINT atPOPUPMENU_AddMenuItem( atHWINDOW hPopupWnd, atLPTSTR szText, atBOOL bEnable )
{
	int cnt = GetPopupMenuItemCount(hPopupWnd);
	return atPOPUPMENU_InsertMenuItem( hPopupWnd, cnt, -1, szText, bEnable );
}


atINT atPOPUPMENU_AddMenuItemIDS( atHWINDOW hPopupWnd, atDWORD nTextIDS, atBOOL bEnable )
{
	int cnt = GetPopupMenuItemCount(hPopupWnd);
	return atPOPUPMENU_InsertMenuItemIDS( hPopupWnd, cnt, -1, nTextIDS, bEnable );
}


atINT atPOPUPMENU_AddSubMenu( atHWINDOW hPopupWnd, atINT nSelectID, atINT nStartID, atPOPUPMENUITEMDATA *menuitem, int menu_count, int sel_index )
{
	int i, r = 0;
	
	for( i = 0; i < menu_count; i++ ){
		r = atPOPUPMENU_AddSubMenuItem( hPopupWnd, nSelectID, nStartID+i, menuitem[i], atTRUE );
	}

	atPOPUPMENU_SelectSubMenuItem( hPopupWnd, nSelectID, sel_index );

	return r;
}

atINT atPOPUPMENU_AddSubMenuItem( atHWINDOW hPopupWnd, atINT nSelectID, atINT nEventID, atLPTSTR szText, atBOOL bEnable )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hPopupWnd);
	POPUPMENU_DATA *lpMenuData;
	POPUPMENU_ITEM *lpItem;
	int cnt;

	if( lpData->lpMenuData == atNULL ) return -1;
	if( nSelectID >= lpData->lpMenuData->nItemCount ) return -1;
	
	lpItem = &lpData->lpMenuData->lstItems[nSelectID];
	if( lpItem->lpSubMenu == atNULL ){
		lpItem->lpSubMenu = __NEW( POPUPMENU_DATA );
		atUIAPI_memset(lpItem->lpSubMenu, 0, sizeof(lpItem->lpSubMenu));
		if( lpItem->lpSubMenu == atNULL ) return -1;
		lpItem->lpSubMenu->lpParentMenu = lpData->lpMenuData;
		lpItem->lpSubMenu->nItemCount = 0;
	}
	lpMenuData = lpItem->lpSubMenu;

	cnt = lpMenuData->nItemCount;
	if( cnt >= MAX_POPUPMENU_ITEM_COUNT ) return -1;
	
	lpItem = &lpMenuData->lstItems[cnt];
	lpItem->lpSubMenu = atNULL;
	lpItem->bEnable = bEnable;
	lpItem->nTextIDS = 0;
	lpItem->nID = nEventID;
	atUIAPI_strncpy( lpItem->szText, szText, POPUPMENU_ITEM_STR_LENGTH );
	
	lpMenuData->nItemCount++;	
	atLISTBOX_EXT_RealizeItem( lpData->hPopupMenu );

	return lpMenuData->nItemCount;
}


atINT atPOPUPMENU_SelectSubMenuItem( atHWINDOW hPopupWnd, atINT nMainMenuID, atINT nSelectIndex )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hPopupWnd);
	POPUPMENU_DATA *lpMenuData = lpData->lpMenuData;
	POPUPMENU_ITEM *lpItem;

	if( lpMenuData == atNULL ) return -1;
	if( nMainMenuID >= lpMenuData->nItemCount ) return -1;

	lpItem = &lpMenuData->lstItems[nMainMenuID];
	if( lpItem == atNULL ) return -1;
	if( lpItem->lpSubMenu == atNULL ) return -1;

	if( (nSelectIndex < 0) || (nSelectIndex >= lpItem->lpSubMenu->nItemCount) ) return -1;

	lpItem->lpSubMenu->nSelIndex = nSelectIndex;

	return lpItem->lpSubMenu->nSelIndex;
}


// 2개이상의 스텝은 따로 구현해야됨.
atINT atPOPUPMENU_AddSubSubMenu( atHWINDOW hPopupWnd, atINT nSelectID1, atINT nSelectID2, atINT nStartID, atPOPUPMENUITEMDATA *menuitem, int menu_count, int sel_index )
{
	int i, r = 0;
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hPopupWnd);
	
	for( i = 0; i < menu_count; i++ ){
		r = atPOPUPMENU_AddSubSubMenuItem( hPopupWnd, nSelectID1, nSelectID2, nStartID+i, menuitem[i], atTRUE );
	}
	
	atPOPUPMENU_SelectSubSubMenuItem( hPopupWnd, nSelectID1, nSelectID2, sel_index );

	return r;
}

atINT atPOPUPMENU_AddSubSubMenuItem( atHWINDOW hPopupWnd, atINT nSelectID1, atINT nSelectID2, atINT nEventID, atLPTSTR szText, atBOOL bEnable )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hPopupWnd);
	POPUPMENU_DATA *lpMenuData;
	POPUPMENU_ITEM *lpItem;
	POPUPMENU_ITEM *lpItem2;
	int cnt;

	if( lpData->lpMenuData == atNULL ) return -1;
	if( nSelectID2 >= lpData->lpMenuData->nItemCount ) return -1;

	//Sub Item step1	
	lpItem = &lpData->lpMenuData->lstItems[nSelectID1];

	//Sub Item step2
	lpItem2 = &lpItem->lpSubMenu->lstItems[nSelectID2];
	

	if( lpItem2->lpSubMenu == atNULL ){
		lpItem2->lpSubMenu = __NEW( POPUPMENU_DATA );
		atUIAPI_memset(lpItem2->lpSubMenu, 0, sizeof(lpItem2->lpSubMenu));
		if( lpItem2->lpSubMenu == atNULL ) return -1;
		lpItem2->lpSubMenu->lpParentMenu = lpItem->lpSubMenu;
		lpItem2->lpSubMenu->nItemCount = 0;
	}

	lpMenuData = lpItem2->lpSubMenu;
	cnt = lpMenuData->nItemCount;

	if( cnt >= MAX_POPUPMENU_ITEM_COUNT ) return -1;
	
	lpItem2 = &lpMenuData->lstItems[cnt];
	lpItem2->lpSubMenu = atNULL;
	lpItem2->bEnable = bEnable;
	lpItem2->nTextIDS = 0;
	lpItem2->nID = nEventID;
	atUIAPI_strncpy( lpItem2->szText, szText, POPUPMENU_ITEM_STR_LENGTH );
	
	lpMenuData->nItemCount++;	
	atLISTBOX_EXT_RealizeItem( lpData->hPopupMenu );

	return lpMenuData->nItemCount;
}


atINT atPOPUPMENU_SelectSubSubMenuItem( atHWINDOW hPopupWnd, atINT nMainMenuID, atINT nSubMenuID, atINT nSelectIndex )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hPopupWnd);
	POPUPMENU_DATA *lpMenuData = lpData->lpMenuData;
	POPUPMENU_ITEM *lpItem;

	if( lpMenuData == atNULL ) return -1;
	if( nMainMenuID >= lpMenuData->nItemCount ) return -1;

	lpItem = &lpMenuData->lstItems[nMainMenuID];
	if( lpItem == atNULL ) return -1;

	lpMenuData = lpItem->lpSubMenu;
	if( lpMenuData == atNULL ) return -2;
	if( nSubMenuID >= lpMenuData->nItemCount ) return -2;

	lpItem = &lpMenuData->lstItems[nSubMenuID];
	if( lpItem == atNULL ) return -2;

	if( (nSelectIndex < 0) || (nSelectIndex >= lpItem->lpSubMenu->nItemCount) ) return -2;

	lpItem->lpSubMenu->nSelIndex = nSelectIndex;

	return lpItem->lpSubMenu->nSelIndex;
}


//atHWINDOW hWnd, : Main핸들.
//atBOOL bEnable, : 활성할것인지 판단. atFALSE:비활성화 , atTRUE:활성화.
//atINT nIndex1,	: 팝업 첫번째 index
//atINT nIndex2,	: 팝업 두번째 index
//atINT nIndex3,	: 팝업 세번째 index
// 순서대로 index번호를 입력해야됨 sub메뉴에서 첫번째 번호는 0번 입니다.
// 3뎁스까지만 지원됨.. <자동 찾기 기능도 가능은함.>
atVOID atPOPUPMENU_EnableSubMenu( atHWINDOW hWnd, atBOOL bEnable, atINT nIndex1, atINT nIndex2, atINT nIndex3)
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);
	POPUPMENU_ITEM *lpItem1 = atNULL;
	POPUPMENU_ITEM *lpItem2 = atNULL;
	POPUPMENU_ITEM *lpItem3 = atNULL;
//	POPUPMENU_DATA *lpMenuData;

	if( lpData->lpMenuData == atNULL ) return;
	
	if(nIndex1 != -1)
		lpItem1 = &lpData->lpMenuData->lstItems[nIndex1];

	if(nIndex2 != -1)
		lpItem2 = &lpItem1->lpSubMenu->lstItems[nIndex2];

	if(nIndex3 != -1)
		lpItem3 = &lpItem2->lpSubMenu->lstItems[nIndex3];


	if(nIndex3 != -1 && lpItem3)
	{
		lpItem3->bEnable = bEnable;
	}
	else if(nIndex2 != -1 && lpItem2)
	{
		lpItem2->bEnable = bEnable;
	}
	else if(nIndex1 != -1 && nIndex1)
	{
		lpItem1->bEnable = bEnable;
	}
//	atLISTBOX_EXT_SetItemEnabled(lpData->hPopupMenu, index, bEnable);
}


atBOOL atPOPUPMENU_RemoveSubMenu( atHWINDOW hPopupWnd, atINT nMainIndex )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hPopupWnd);
	POPUPMENU_ITEM *lpItem;
	
	if( nMainIndex >= lpData->lpMenuData->nItemCount ) return atFALSE;
	
	lpItem = &lpData->lpMenuData->lstItems[nMainIndex];
	if( lpItem->lpSubMenu != atNULL ){
		__DELETE( lpItem->lpSubMenu );
	}
	
	return atTRUE;
}

atBOOL atPOPUPMENU_RemoveMenuItem( atHWINDOW hPopupWnd, atINT nMainIndex )
{
/*
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hPopupWnd);
	POPUPMENU_ITEM *lpItem;
	
	if( nMainIndex >= lpData->lstMainMenu.nItemCount ) return atFALSE;
	
	lpItem = &lpData->lstMainMenu.lstItems[nMainIndex];
	if( lpItem->lpSubMenu != atNULL ){
		__DELETE( lpItem->lpSubMenu );
	}
	lpData->lstMainMenu.nItemCount = 0;
	
	return atTRUE;
*/	
	return atFALSE;
}

atBOOL atPOPUPMENU_RemoveSubMenuItem( atHWINDOW hPopupWnd, atINT nMainIndex, atINT nSubIndex )
{
/*
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hPopupWnd);
	POPUPMENU_DATA *lpMenuData;
	POPUPMENU_ITEM *lpItem;
	
	if( nMainIndex >= lpData->lstMainMenu.nItemCount ) return atFALSE;
	
	lpItem = &lpData->lstMainMenu.lstItems[nMainIndex];
	if( lpItem->lpSubMenu == atNULL ) return atFALSE;
	
	lpMenuData = lpItem->lpSubMenu;

	cnt = lpMenuData->nItemCount;
	if( cnt >= MAX_POPUPMENU_ITEM_COUNT ) return -1;
	
	lpItem = &lpMenuData->lstItems[cnt];
	lpItem->lpSubMenu = atNULL;
	atUIAPI_strncpy( lpItem->szText, szText, POPUPMENU_ITEM_STR_LENGTH );
	
	lpMenuData->nItemCount++;	
	
	return lpMenuData->nItemCount;
*/	
	return atFALSE;
}

atVOID	atPOPUPMENU_SetTextSize( atHWINDOW hPopupWnd, atINT nSize, atINT nTitleSize, atINT nFontStyle )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hPopupWnd);

	if( !lpData)
		return ;

	if( lpData->hFont )
	{
		atUIAPI_DestroyFont( lpData->hFont );
		lpData->hFont = NULL;
	}

	lpData->hFont = atUIAPI_CreateFont( atUIAPI_FACE_SYSTEM, nSize, nFontStyle, atFALSE );

	if(lpData->hTitleFont)
	{
		atUIAPI_DestroyFont(lpData->hTitleFont);
		lpData->hTitleFont = atNULL;
	}
	
	if( nTitleSize > 0 )
		lpData->hTitleFont = atUIAPI_CreateFont( atUIAPI_FACE_SYSTEM, nTitleSize, nFontStyle, atFALSE );

	if( lpData->hPopupMenu )
		atCONTROL_SetFont( lpData->hPopupMenu, lpData->hFont );
}

atVOID	atPOPUPMENU_SetSubLayout( atHWINDOW hPopupWnd, atLAYOUTBITMAP bmLayout )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hPopupWnd);

	if( !lpData)
		return ;

	lpData->bmLayout = bmLayout;
}

#if SUPPORT_SUBMENU_BACKBUTTON
atVOID	atPOPUPMENU_AddSubBackBtn( atHWINDOW hPopupWnd, atLPTSTR szText, 
								  atHBITMAP lpNormalImg_Left, atHBITMAP lpNormalImg_Center, atHBITMAP lpNormalImg_Right,
								  atHBITMAP lpFocusedImg_Left, atHBITMAP lpFocusedImg_Center, atHBITMAP lpFocusedImg_Right,
								  atHBITMAP lpSelectedImg_Left, atHBITMAP lpSelectedImg_Center, atHBITMAP lpSelectedImg_Right,
								  atHBITMAP lpDisabledImg_Left, atHBITMAP lpDisabledImg_Center, atHBITMAP lpDisabledImg_Right )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hPopupWnd);

	if( !lpData)		return ;

	atUIAPI_strncpy(lpData->szButton, szText, 32);
	lpData->lpNormalImg_Left		= lpNormalImg_Left;
	lpData->lpNormalImg_Center		= lpNormalImg_Center;
	lpData->lpNormalImg_Right		= lpNormalImg_Right;
	lpData->lpFocusedImg_Left		= lpFocusedImg_Left;
	lpData->lpFocusedImg_Center		= lpFocusedImg_Center;
	lpData->lpFocusedImg_Right		= lpFocusedImg_Right;
	lpData->lpSelectedImg_Left		= lpSelectedImg_Left;
	lpData->lpSelectedImg_Center	= lpSelectedImg_Center;
	lpData->lpSelectedImg_Right		= lpSelectedImg_Right;
	lpData->lpDisabledImg_Left		= lpDisabledImg_Left;
	lpData->lpDisabledImg_Center	= lpDisabledImg_Center;
	lpData->lpDisabledImg_Right		= lpDisabledImg_Right;

	lpData->bSubBackBtn = atTRUE;
}
#endif

void StartAnimationPopup(atHWINDOW hWnd, int direction )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);

	if( lpData->bAnimation ){
		atREGION rgn, rgn2;
		atWINDOW_GetWindowRegion( hWnd, &rgn );
//		atWINDOW_SetWindowRegion( hWnd, rgn.nX, rgn.nY, rgn.nWidth, 0 );
		atDESKTOP_GetDesktopArea( atDESKTOP_AREA_CLIENT, &rgn2 );
		lpData->cur_y = rgn2.nHeight;
		atWINDOW_SetPosition( hWnd, rgn.nX, lpData->cur_y );
//		atAPP_AdjustWindowArea( atAPPMNG_GetCurApp(), hWnd );
	}
	
	atAPP_ShowWindow( atAPPMNG_GetCurApp(), hWnd, atTRUE );
	
	if( lpData->bAnimation ){
		lpData->inc_y = -MENU_ANI_INC;
		atWINDOW_EnableTimer( hWnd, lpData->hAniTimer, atTRUE );
	}
}

atHCONTROL GetPopupMenuList(atHWINDOW hWnd)
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);

	return (atHCONTROL)lpData->hPopupMenu;
}

int GetPopupMenuItemCount(atHWINDOW hWnd)
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);

	return atLISTBOX_EXT_GetCount(lpData->hPopupMenu);
}

void EnablePopupMenuItem(atHWINDOW hWnd, int index, atBOOL bEnable )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);

//	atPOPUPMENU_SetMenuItemEnable(lpData->hPopupMenu, index, bEnable);
	atLISTBOX_EXT_SetItemEnabled(lpData->hPopupMenu, index, bEnable);
}

void SetSelectIndex(atHWINDOW hWnd, int index)
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);

	atLISTBOX_EXT_SetSelectIndex(lpData->hPopupMenu, index);
}

void SetPopupMenuKey( atHWINDOW hWnd, atINT SelectKey, atINT ClearKey, atINT BackKey )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);

	lpData->nSelectKey = SelectKey;
	lpData->nClearKey = ClearKey;
	lpData->nBackKey = BackKey;
	
	//_SetSoftKey( hWnd, (lpData->lpMenuData->lpParentMenu != atNULL) );
}

void SetPopupMenuSoftKeyStr( atHWINDOW hWnd, atLPTSTR szSelectKey, atLPTSTR szClearKey, atLPTSTR szBackKey )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);

	atUIAPI_strncpy( lpData->szSelectKey, szSelectKey, 32 );
	atUIAPI_strncpy( lpData->szClearKey, szClearKey, 32 );
	atUIAPI_strncpy( lpData->szBackKey, szBackKey, 32 );
	
	lpData->nSelectKeyIDS = 0;
	lpData->nClearKeyIDS = 0;
	lpData->nBackKeyIDS = 0;
	
	_SetSoftKey( hWnd, (lpData->lpMenuData->lpParentMenu != atNULL) );
}

void SetPopupMenuSoftKeyStrIDS( atHWINDOW hWnd, atINT idSelectKey, atINT idClearKey, atINT idBackKey )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);

	lpData->nSelectKeyIDS = idSelectKey;
	lpData->nClearKeyIDS = idClearKey;
	lpData->nBackKeyIDS = idBackKey;
	
	_SetSoftKey( hWnd, (lpData->lpMenuData->lpParentMenu != atNULL) );
}

void SetPopupMenuColor( atHWINDOW hWnd, atCOLOR clNormal, atCOLOR clFocus)
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(lpData->hPopupMenu);

	lpData->clItemColorNormal = clNormal;
	lpData->clItemColorFocus = clFocus;
	lpControl->tControlColor.clNormalTxtColor = clNormal;
	lpControl->tControlColor.clFocusedTxtColor = clFocus;
}

void SetPopupMenuBitmap( atHWINDOW hWnd, atHBITMAP hBmSubMenu, atHBITMAP hBmSubMenuFocus, atHBITMAP hBmSubMenuDisable )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);

	lpData->hImgSubMenu = hBmSubMenu;
	lpData->hImgSubMenuSelected = hBmSubMenuFocus;
	lpData->hImgSubMenuDisable = hBmSubMenuDisable;
}

void RecalcPopupMenuWnd( atHWINDOW hWnd )
{
	LPPOPUPMENUWNDDATA	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);
	if( lpData == atNULL ) return;
	
	CalcMenuWindowSize( hWnd, lpData->view_count, lpData->align );
}

// SubPopUp 위치 조정.
void GetSubPopUpRegion(atHWINDOW hWnd, atHWINDOW hOwnerWnd, POPUPMENU_DATA *pItem, atREGION *subItemPt )
{
	atREGION rgn;
	atREGION *softRgn;
//	atHWINDOW hWnd = atNULL;
	LPPOPUPMENUWNDDATA	lpData;
	int idx = 0;
	int topY = 0;
	atRECT lRect;
	int lHeight = 0;
	atINT subItemCount = pItem->nItemCount;

	int _Height = 0;
	
	if(!hWnd)	return;
	
	lpData = (LPPOPUPMENUWNDDATA)atWINDOW_GetDataPtr(hWnd);

	idx = atLISTBOX_EXT_GetSelectIndex(lpData->hPopupMenu);
	topY = atLISTBOX_EXT_GetViewTopY(lpData->hPopupMenu);

	atWINDOW_GetWindowRegion(hWnd, &rgn);
	atWINDOW_GetWindowRect(hWnd, &lRect);
	softRgn = atSOFTKEY_GetSoftKeyArea();

	lHeight = lRect.nEndY - softRgn->nHeight;


	subItemPt->nY = (lpData->nItemHeight * idx) + rgn.nY + 5 - topY;
	subItemPt->nHeight = lHeight - (subItemPt->nY + (subItemCount * lpData->nItemHeight) - 10);
//	subItemPt->nHeight = subItemPt->nY + (subItemCount * POPUP_ITEM_HIGHT);

	subItemPt->nX = SUB_POPUP_X;
	subItemPt->nWidth =0;
//	subItemPt->nWidth = rgn.nWidth -  (/*Gap구함.*/(lRect.nEndX - (rgn.nWidth -1)) + 150);

	//범위 넘어가는지 계산함.
	_Height = ((subItemCount * lpData->nItemHeight) + subItemPt->nY) - softRgn->nHeight;

	//리스트 아래에서 SubPopUp이 표시될경우 하단아래로 내려가므로 그것을 방지 하기 위해서.
	if(_Height > lHeight)
	{
		atINT itemp = 0;
		atINT lGap = 0;

		//팝업크기 - (표시해야되는 영역) = 넘어가는범위
		lGap = ((subItemCount * lpData->nItemHeight)-15)-((lRect.nEndY - subItemPt->nY)-softRgn->nHeight);
		
 		itemp = subItemPt->nY - lGap;

		subItemPt->nY = itemp;
	}

}
