//
// PopupMenuWnd.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __POPUPPOPUPMENUWND_H_
#define __POPUPPOPUPMENUWND_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define EXTPKG_CUSTWIN_POPUPMENU      _AT("ExtPkg.CustWin.PopupMenuWnd")

#define SUPPORT_SUBMENU_BACKBUTTON		1

#define MENU_ALIGN_HORIZ_MASK   0x00FF
#define MENU_ALIGN_VERT_MASK    0xFF00
#define MENU_ALIGN_LEFT   0x0000
#define MENU_ALIGN_RIGHT  0x0001
#define MENU_ALIGN_CENTER 0x0002
#define MENU_ALIGN_TOP    0x0000
#define MENU_ALIGN_BOTTOM 0x0100
#define MENU_ALIGN_MIDDLE 0x0200

#define MAX_POPUPMENU_ITEM_COUNT 32
#define POPUPMENU_ITEM_STR_LENGTH 32
#define POPUPMENU_BACK -1

typedef atTCHAR atPOPUPMENUITEMDATA[ POPUPMENU_ITEM_STR_LENGTH ];


atBOOL      RegistPopupMenuWndClass( atHRESSRC hResSrc );
void		UnregistPopupMenuWndClass(void);


atHWINDOW	CreatePopupMenuWnd0( atHWINDOW hParentWnd, int id, int x, int y, int width, int height, int item_height, atPOPUPMENUITEMDATA *menuitem, int menu_count, int view_count, int select, int align, atHCONTROL *lphControl );
atHWINDOW	CreatePopupMenuWnd( atHWINDOW hParentWnd, int id, int x, int y, int width, int height, int item_height, atPOPUPMENUITEMDATA *menuitem, int menu_count, int view_count, int select, int align, atHCONTROL *lphControl );
atHWINDOW	CreatePopupMenuWndIDS0( atHWINDOW hParentWnd, int id, int x, int y, int width, int height, int item_height, atDWORD* menuIDS, int menu_count, int view_count, int select, int align, atHCONTROL *lphControl );
atHWINDOW	CreatePopupMenuWndIDS( atHWINDOW hParentWnd, int id, int x, int y, int width, int height, int item_height, atDWORD* menuIDS, int menu_count, int view_count, int select, int align, atHCONTROL *lphControl );

atVOID 	atPOPUPMENU_RemoveAllMenuItem( atHWINDOW hPopupWnd );
atINT 	atPOPUPMENU_InsertMenuItem( atHWINDOW hPopupWnd, atINT nIndex, atINT nID, atLPTSTR szText, atBOOL bEnable );
atINT 	atPOPUPMENU_InsertMenuItemIDS( atHWINDOW hPopupWnd, atINT nIndex, atINT nID, atDWORD nTextIDS, atBOOL bEnable );
atINT 	atPOPUPMENU_AddMenuItem( atHWINDOW hPopupWnd, atLPTSTR szText, atBOOL bEnable );
atINT 	atPOPUPMENU_AddMenuItemIDS( atHWINDOW hPopupWnd, atDWORD nTextIDS, atBOOL bEnable );
atINT 	atPOPUPMENU_AddSubMenu( atHWINDOW hPopupWnd,atINT nSelectID, atINT nStartID, atPOPUPMENUITEMDATA *menuitem, int menu_count, int sel_index );
atINT 	atPOPUPMENU_AddSubMenuItem( atHWINDOW hPopupWnd, atINT nSelectID, atINT nEventID, atLPTSTR szText, atBOOL bEnable );
atINT	atPOPUPMENU_SelectSubMenuItem( atHWINDOW hPopupWnd, atINT nMainMenuID, atINT nSelectIndex );
atINT	atPOPUPMENU_AddSubSubMenu( atHWINDOW hPopupWnd, atINT nSelectID1, atINT nSelectID2, atINT nStartID, atPOPUPMENUITEMDATA *menuitem, int menu_count, int sel_index );
atINT 	atPOPUPMENU_AddSubSubMenuItem( atHWINDOW hPopupWnd, atINT nSelectID1, atINT nSelectID2, atINT nEventID, atLPTSTR szText, atBOOL bEnable );
atINT	atPOPUPMENU_SelectSubSubMenuItem( atHWINDOW hPopupWnd, atINT nMainMenuID, atINT nSubMenuID, atINT nSelectIndex );
atBOOL 	atPOPUPMENU_RemoveSubMenu( atHWINDOW hPopupWnd, atINT nMainIndex );
atBOOL 	atPOPUPMENU_RemoveMenuItem( atHWINDOW hPopupWnd, atINT nMainIndex );
atBOOL 	atPOPUPMENU_RemoveSubMenuItem( atHWINDOW hPopupWnd, atINT nMainIndex, atINT nSubIndex );
atVOID	atPOPUPMENU_EnableSubMenu( atHWINDOW hWnd, atBOOL bEnable, atINT nIndex1, atINT nIndex2, atINT nIndex3);
atVOID	atPOPUPMENU_SetTextSize( atHWINDOW hPopupWnd, atINT nSize, atINT nTitleSize, atINT nFontStyle );
atVOID	atPOPUPMENU_SetSubLayout( atHWINDOW hPopupWnd, atLAYOUTBITMAP bmLayout );
#if SUPPORT_SUBMENU_BACKBUTTON
atVOID	atPOPUPMENU_AddSubBackBtn( atHWINDOW hPopupWnd, atLPTSTR szText, 
								  atHBITMAP lpNormalImg_Left, atHBITMAP lpNormalImg_Center, atHBITMAP lpNormalImg_Right,
								  atHBITMAP lpFocusedImg_Left, atHBITMAP lpFocusedImg_Center, atHBITMAP lpFocusedImg_Right,
								  atHBITMAP lpSelectedImg_Left, atHBITMAP lpSelectedImg_Center, atHBITMAP lpSelectedImg_Right,
								  atHBITMAP lpDisabledImg_Left, atHBITMAP lpDisabledImg_Center, atHBITMAP lpDisabledImg_Right );
#endif

void StartAnimationPopup(atHWINDOW hWnd, int direction );
void RecalcPopupMenuWnd( atHWINDOW hWnd );
atHCONTROL GetPopupMenuList(atHWINDOW hWnd);
int GetPopupMenuItemCount(atHWINDOW hWnd);
void EnablePopupMenuItem(atHWINDOW hWnd, int index, atBOOL bEnable );
void SetSelectIndex(atHWINDOW hWnd, int index);
void SetPopupMenuKey( atHWINDOW hWnd, atINT SelectKey, atINT ClearKey, atINT BackKey );
void SetPopupMenuSoftKeyStr( atHWINDOW hWnd, atLPTSTR szSelectKey, atLPTSTR szClearKey, atLPTSTR szBackKey );
void SetPopupMenuSoftKeyStrIDS( atHWINDOW hWnd, atINT idSelectKey, atINT idClearKey, atINT idBackKey );
void SetPopupMenuColor( atHWINDOW hWnd, atCOLOR clNormal, atCOLOR clFocus);
void SetPopupMenuBitmap( atHWINDOW hWnd, atHBITMAP hBmSubMenu, atHBITMAP hBmSubMenuFocus, atHBITMAP hBmSubMenuDisable );

#ifdef __cplusplus
}
#endif



#endif // __POPUPPOPUPMENUWND_H_
