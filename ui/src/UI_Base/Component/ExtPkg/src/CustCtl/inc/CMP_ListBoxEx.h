#ifndef __CMP_LISTBOXEX_H_
#define __CMP_LISTBOXEX_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define atLISTBOX_EXT_ATTR_TRANSPARENT			(0x0000)
#define atLISTBOX_EXT_ATTR_OUTLINE				(0x8000)
#define atLISTBOX_EXT_ATTR_OPAQUE					(0x4000)

#define atLISTBOX_EXT_ATTR_MOVE_NORMAL			(0x0000)
#define atLISTBOX_EXT_ATTR_MOVE_WRAP 			(0x1000)
#define atLISTBOX_EXT_ATTR_MOVE_NEXTCONTROL 		(0x2000)

#define atLISTBOX_EXT_ATTR_FLEXHEIGHT				(0x0800)

//#define atLISTBOX_EXT_SCROLLTYPE_PANNING		(0x0000)
//#define atLISTBOX_EXT_SCROLLTYPE_SCROLLL		(0x0001)

#define		atLISTBOX_EXT_STR_LEN				128

typedef enum {
	atLISTBOX_EXT_IMG_POST = 0,
	atLISTBOX_EXT_IMG_POST_SELECT,
	atLISTBOX_EXT_IMG_POST_DISABLE,
	atLISTBOX_EXT_IMG_TAIL,
	atLISTBOX_EXT_IMG_TAIL_SELECT,
	atLISTBOX_EXT_IMG_TAIL_DISABLE,
	atLISTBOX_EXT_IMG_MAX
} atLISTBOX_EXT_IMG;

typedef enum {
	atLISTBOX_EXT_STR_NORMAL = 0,
	atLISTBOX_EXT_STR_SELECT,
	atLISTBOX_EXT_STR_DISABLE,
	atLISTBOX_EXT_STR_MAX
} atLISTBOX_EXT_STR;

typedef	struct	_tagatLISTBOX_EXT_ITEM {
	int				nNO;
	int				nItemID;
	atTCHAR			szItemName[atLISTBOX_EXT_STR_MAX][atLISTBOX_EXT_STR_LEN];
	atDWORD			nItemNameIds[atLISTBOX_EXT_STR_MAX];
	atCOLOR			clItemColor[atLISTBOX_EXT_STR_MAX];
	atHBITMAP		imgItemImage[atLISTBOX_EXT_IMG_MAX];  // [6] -> [2];
	atSREGION		imgItemImageRegion[atLISTBOX_EXT_IMG_MAX];  // [6] -> [2];
	atBOOL			bEnable;
	atBOOL			bChecked;
	void			*data;
} atLISTBOX_EXT_ITEM, * atLPLISTBOX_EXT_ITEM;

struct _tagatLISTBOX_EXT;
typedef atVOID (atLISTBOX_EXT_DRAWITEM_BG_FUNC)( atHGDC hCanvas, atHCONTROL hControl, atINT nIndex, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atLPLISTBOX_EXT_ITEM lpItem, atBOOL bSelected, atBOOL bFocused, atLPVOID lpParam );
typedef atVOID (atLISTBOX_EXT_DRAWITEM_FUNC)( atHGDC hCanvas, atHCONTROL hControl, atINT nIndex, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atBOOL bFirstNo, atINT nCheckType, atLPLISTBOX_EXT_ITEM lpItem, atBOOL bSelected, atBOOL bFocused, atLPVOID lpParam );
typedef atINT (atLISTBOX_EXT_GETITEMHEIGHT_FUNC)( atHCONTROL hControl, atINT nIndex, atLPVOID lpParam );
typedef atINT (atLISTBOX_EXT_GETCOUNT_FUNC)( struct _tagatLISTBOX_EXT *, atLPVOID );
typedef atBOOL (atLISTBOX_EXT_GETITEM_FUNC)( struct _tagatLISTBOX_EXT *, atLPVOID, atINT, atLPLISTBOX_EXT_ITEM );
typedef atBOOL (atLISTBOX_EXT_SETITEM_FUNC)( struct _tagatLISTBOX_EXT *, atLPVOID, atINT, atLPLISTBOX_EXT_ITEM );

typedef struct _tagatLISTBOX_EXT {
	atDWORD      dwAttr;

//	atLP_LISTBOX_DRAWITEM_FUNC fnDrawLayout;
//	atLP_LISTBOX_CHANGEITEM_FUNC fnChange;
	atLISTBOX_EXT_DRAWITEM_BG_FUNC *fnDrawItemBG;
	atLISTBOX_EXT_DRAWITEM_FUNC *fnDrawItem;
	atLISTBOX_EXT_GETITEMHEIGHT_FUNC *fnGetItemHeight;
	atLISTBOX_EXT_GETCOUNT_FUNC *fnGetCount;
	atLISTBOX_EXT_GETITEM_FUNC *fnGetItem;
	atLISTBOX_EXT_SETITEM_FUNC *fnSetItem;
	atLPVOID		lpFuncParam;

	int			nSelectIndex;
   	int			nItemHeight, nItemGap;
	int			nWidthMargin, nHeightMargin;
	int			nWidthBorder, nHeightBorder;
//   	int			nOneScreenCount;

	 atBOOL			bFirstNO;
	 atINT			nCheckType;

	 atBOOL			bTickerEnable;
	 atBOOL			bImageVision;

	 atUILIST		lpListBoxList;
	 atLPLISTBOX_EXT_ITEM		lpAddItemData;
	 atINT		nAddItemIndex;

	atHBITMAP		lpUncheckedImage, lpCheckedImage;
	atHBITMAP		hImgNormalLeft, hImgNormalCenter, hImgNormalRight;
	atHBITMAP		hImgSelectLeft, hImgSelectCenter, hImgSelectRight;
	atHBITMAP		hImgDisableLeft, hImgDisableCenter, hImgDisableRight;
	atHCONTROL		hScrollBar;
	atHCONTROL      hTickerBox;

	atINT			SelectKey, CheckKey;

	atINT		nViewTopY;
	atINT      		nCurCheckedIndex;
	atBOOL		bTickerActivate;

	atBOOL		bMouseDown, bMouseScroll;
	atBOOL		bActionValidate; 
	int			nFirstViewTopY;
	int			nFirstMouseX, nFirstMouseY;
	int			nPrevMouseX, nPrevMouseY;
	//int			nScrollType;

	atBOOL		bIsChange;

	// Kyle Start [Add 2009/07/23 - 14:45]
	atBOOL		bUseSmoothScroll;		// 스무스 스크롤 사용여부
	atBOOL		bIsAcceleration;		// 스무스 스크롤 Timer 사용여부
	atDWORD		dwAccelSTick;			// 스무스 스크롤 시작한 시간(ms = TickCount) [가속도를 구하기 위한]
	atINT		nDraggedDistance;		// 스무스 스크롤 Dragged 거리 가중치
	atREAL		fVelocity;				// 속도(속력 변화량) 
	atBOOL		bMMLockFlag;			// 스무스 스크롤 Lock Boolean 값
	// Kyle [Add 2009/07/23 - 14:45] End
} atLISTBOX_EXT,  *atLPLISTBOX_EXT;


atHCONTROL atLISTBOX_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight,
						  int nItemHeight, int nItemLineGap, int nWidthMargin, int nHeightMargin,
						  atBOOL bFirstNO, atINT nCheckType, atBOOL bTicker,
						  atHBITMAP hImgNormalLeft, atHBITMAP hImgNormalCenter, atHBITMAP hImgNormalRight,
						  atHBITMAP hImgSelectLeft, atHBITMAP hImgSelectCenter, atHBITMAP hImgSelectRight,
						  atHBITMAP hImgDisableLeft, atHBITMAP hImgDisableCenter, atHBITMAP hImgDisableRight,
						  atHBITMAP lpUncheckedImage, atHBITMAP lpCheckedImage,
						  //atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom, atHBITMAP lpScrollcursorImage );
						  atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom,
						  atHBITMAP lpScrollcursorImgTop, atHBITMAP lpScrollcursorImgMid, atHBITMAP lpScrollcursorImgBot );

atHCONTROL atLISTBOX_EXT_CreateEx( int nIndex, int nX, int nY, int nWidth, int nHeight,
						  int nItemHeight, int nItemLineGap, 
						  int nWidthMargin, int nHeightMargin,
						  int nWidthBorder, int nHeightBorder,
						  atBOOL bFirstNO, atINT nCheckType, atBOOL bTicker,
						  atHBITMAP hImgNormalLeft, atHBITMAP hImgNormalCenter, atHBITMAP hImgNormalRight,
						  atHBITMAP hImgSelectLeft, atHBITMAP hImgSelectCenter, atHBITMAP hImgSelectRight,
						  atHBITMAP hImgDisableLeft, atHBITMAP hImgDisableCenter, atHBITMAP hImgDisableRight,
						  atHBITMAP lpUncheckedImage, atHBITMAP lpCheckedImage,
						  //atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom, atHBITMAP lpScrollcursorImage );
						  atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom,
						  atHBITMAP lpScrollcursorImgTop, atHBITMAP lpScrollcursorImgMid, atHBITMAP lpScrollcursorImgBot );

atDWORD				atLISTBOX_EXT_GetAttr(atHCONTROL hControl );
void				atLISTBOX_EXT_SetAttr(atHCONTROL hControl, atDWORD dwAttr );


void				atLISTBOX_EXT_SetBitmap( atHCONTROL hControl,
						  atHBITMAP hImgNormalLeft, atHBITMAP hImgNormalCenter, atHBITMAP hImgNormalRight,
						  atHBITMAP hImgSelectLeft, atHBITMAP hImgSelectCenter, atHBITMAP hImgSelectRight,
						  atHBITMAP hImgDisableLeft, atHBITMAP hImgDisableCenter, atHBITMAP hImgDisableRight );

void				atLISTBOX_EXT_Start_AddItem(atHCONTROL hControl);
void				atLISTBOX_EXT_Start_AddItemEx(atHCONTROL hControl, atINT nIndex);
void 				atLISTBOX_EXT_AddItem_Enabled(atHCONTROL hControl, atBOOL bEnabled);
void				atLISTBOX_EXT_AddItem_Checked(atHCONTROL hControl, atBOOL bChecked);
void				atLISTBOX_EXT_AddItem_NO(atHCONTROL hControl, int nNO);
void				atLISTBOX_EXT_AddItem_ItemID(atHCONTROL hControl, int nItemID);
void				atLISTBOX_EXT_AddItem_String(atHCONTROL hControl, atLPTSTR szItemName, int nIndex) ;
void				atLISTBOX_EXT_AddItem_IDS(atHCONTROL hControl, atDWORD ids, int nIndex);
void				atLISTBOX_EXT_AddItem_String2(atHCONTROL hControl, atLPTSTR szItemName, atCOLOR clTextColor, int nIndex) ;
void				atLISTBOX_EXT_AddItem_Image(atHCONTROL hControl, atHBITMAP imgListImage, int nIndex);
void				atLISTBOX_EXT_AddItem_Image2(atHCONTROL hControl, atHBITMAP imgListImage, int x, int y, int width, int height, int nIndex);
void				atLISTBOX_EXT_Update_AddItem(atHCONTROL hControl);

void 				atLISTBOX_EXT_ClearAllItem( atHCONTROL hControl );
void    			atLISTBOX_EXT_DeleteItem(atHCONTROL hControl, int nIndex);

int 				atLISTBOX_EXT_GetCount(atHCONTROL hControl);

atBOOL 			atLISTBOX_EXT_SetItem(atHCONTROL hControl, int nIndex, atLPLISTBOX_EXT_ITEM lpItem);
atBOOL 			atLISTBOX_EXT_GetItem(atHCONTROL hControl, int nIndex, atLISTBOX_EXT_ITEM *lpItem);
atVOID 			atLISTBOX_EXT_RealizeItem(atHCONTROL hControl);

int 				atLISTBOX_EXT_GetSelectIndex(atHCONTROL hControl);
atBOOL 			atLISTBOX_EXT_GetSelectItem(atHCONTROL hControl, atLISTBOX_EXT_ITEM *lpItem);

atBOOL 				atLISTBOX_EXT_SetSelectIndex( atHCONTROL hControl, int nIndex );
atBOOL				atLISTBOX_EXT_SetSelectIndexByPage(atHCONTROL hControl, int page);
int 				atLISTBOX_EXT_MoveSelect(atHCONTROL hControl, int nDirect);

atBOOL				atLISTBOX_EXT_ToggleSelectItemChecked(atHCONTROL hControl);
void				atLISTBOX_EXT_SetItemCheck(atHCONTROL hControl, int nIndex, atBOOL bCheck);
atBOOL 				atLISTBOX_EXT_GetItemChecked(atHCONTROL hControl, int nIndex);
atINT 			atLISTBOX_EXT_GetCurCheckedIndex(atHCONTROL hControl);

void				atLISTBOX_EXT_SetItemEnabled(atHCONTROL hControl, int nIndex, atBOOL bEnable);
atBOOL 				atLISTBOX_EXT_GetItemEnabled(atHCONTROL hControl, int nIndex);

atLPTSTR 		atLISTBOX_EXT_GetItemString(atHCONTROL hControl, int nIndex, int nStringIndex);
atVOID 			atLISTBOX_EXT_AddItem_Data(atHCONTROL hControl, void*data);
atVOID* 		atLISTBOX_EXT_GetItemData(atHCONTROL hControl, int nIndex);
atVOID* 		atLISTBOX_EXT_GetCurItemData(atHCONTROL hControl);

atINT 		atLISTBOX_EXT_ResetSelect(atHCONTROL hControl);

atINT 		atLISTBOX_EXT_GetViewTopY(atHCONTROL hControl);

void 		atLISTBOX_EXT_SetupKey( atHCONTROL hControl, atINT SelectKey, atINT CheckKey );

void atLISTBOX_EXT_GetFunc( atHCONTROL hControl, 
									atLISTBOX_EXT_DRAWITEM_BG_FUNC **lppfnDrawItemBG, 
									atLISTBOX_EXT_DRAWITEM_FUNC **lppfnDrawItem, 
									atLISTBOX_EXT_GETITEMHEIGHT_FUNC **lppfnGetItemHeight, 
									atLISTBOX_EXT_GETCOUNT_FUNC **lppfnGetCount, 
									atLISTBOX_EXT_GETITEM_FUNC **lppfnGetItem,
									atLISTBOX_EXT_SETITEM_FUNC **lppfnSetItem,
									atLPVOID	*lppFuncParam );
void atLISTBOX_EXT_SetupFunc( atHCONTROL hControl, 
									atLISTBOX_EXT_DRAWITEM_BG_FUNC *fnDrawItemBG, 
									atLISTBOX_EXT_DRAWITEM_FUNC *fnDrawItem, 
									atLISTBOX_EXT_GETITEMHEIGHT_FUNC *fnGetItemHeight, 
									atLISTBOX_EXT_GETCOUNT_FUNC *fnGetCount, 
									atLISTBOX_EXT_GETITEM_FUNC *fnGetItem,
									atLISTBOX_EXT_SETITEM_FUNC *fnSetItem,
									atLPVOID	lpFuncParam );

// Kyle Start [Add 2009/07/23 - 15:45]
atVOID atLISTBOX_EXT_SetUseSmoothScroll( atHCONTROL hControl, atBOOL bUsed );
atBOOL atLISTBOX_EXT_GetUseSmoothScroll( atHCONTROL hControl );
// Kyle [Add 2009/07/23 - 15:45] End
atVOID	atLISTBOX_EXT_SetItemMargin( atHCONTROL hControl, int nWidthMargin, int nHeightMargin );
atVOID	atLISTBOX_EXT_GetItemMargin( atHCONTROL hControl, int* nWidthMargin, int* nHeightMargin );
atVOID	atLISTBOX_EXT_SetItemGap( atHCONTROL hControl, int nItemGap );
int		atLISTBOX_EXT_GetItemGap( atHCONTROL hControl );
#ifdef __cplusplus
}
#endif


#endif
