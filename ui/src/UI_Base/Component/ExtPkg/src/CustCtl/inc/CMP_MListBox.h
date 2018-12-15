#ifndef __CMP_MLISTBOX_H_
#define __CMP_MLISTBOX_H_

#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif // __A_UIENG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define atMLISTBOX_ATTR_TRANSPARENT				(0x0000)
#define atMLISTBOX_ATTR_OUTLINE					(0x8000)
#define atMLISTBOX_ATTR_OPAQUE					(0x4000)
#define atMLISTBOX_ATTR_CLICK					(0x0100)	// Click시에만 PAINT

#define atMLISTBOX_ATTR_MOVE_NORMAL				(0x0000)
#define atMLISTBOX_ATTR_MOVE_WRAP 				(0x1000)
#define atMLISTBOX_ATTR_MOVE_NEXTCONTROL 		(0x2000)

#define atMLISTBOX_STR_LEN						128
#define atMLISTBOX_LINE_MAX						5

#define atMLISTBOX_MOSTALIGN_IGNORE				0x0000
#define atMLISTBOX_MOSTALIGN_LEFT				0x1000
//#define atMLISTBOX_MOSTALIGN_CENTER				0x2000
#define atMLISTBOX_MOSTALIGN_RIGHT				0x4000

typedef enum {
	atMLISTBOX_IMG_POST = 0,
	atMLISTBOX_IMG_POST_SELECT,
	atMLISTBOX_IMG_POST_DISABLE,
	atMLISTBOX_IMG_TAIL,
	atMLISTBOX_IMG_TAIL_SELECT,
	atMLISTBOX_IMG_TAIL_DISABLE,
	atMLISTBOX_IMG_MAX
} atMLISTBOX_IMG;

typedef enum {
	atMLISTBOX_STR_NORMAL = 0,
	atMLISTBOX_STR_SELECT,
	atMLISTBOX_STR_DISABLE,
	atMLISTBOX_STR_MAX
} atMLISTBOX_STR;

typedef enum {
	atMLISTBOX_SELECT_NORMAL = 0,
	atMLISTBOX_SELECT_CHECK,
	atMLISTBOX_SELECT_RADIO,
	atMLISTBOX_SELECT_MAX
} atMLISTBOX_SELECT_TYPE;

typedef	struct _tagatMLISTBOX_ITEM {
	int				nNO;
	int				nItemID;
	atTCHAR			szItemName[atMLISTBOX_LINE_MAX][atMLISTBOX_STR_MAX][atMLISTBOX_STR_LEN];
	atDWORD			nItemNameIds[atMLISTBOX_LINE_MAX][atMLISTBOX_STR_MAX];
	atCOLOR			clItemColor[atMLISTBOX_LINE_MAX][atMLISTBOX_STR_MAX];
	atCOLOR			clItemChkBoxColor[atMLISTBOX_STR_MAX];
	atCOLOR			clItemFirstNoColor[atMLISTBOX_STR_MAX];
	atHBITMAP		imgItemImage[atMLISTBOX_IMG_MAX];			// [6] -> [2];
	atSREGION		imgItemImageRegion[atMLISTBOX_IMG_MAX];		// [6] -> [2];
	atRECT			imgItemDrawRect[2];							// POST,TAIL 2개만 구분, 땜빵,수정필요
	int				nImgSelectIdx;
	atTCHAR			szHighLight[atMLISTBOX_STR_LEN];			// 하이라이트 문자열
	atBYTE			bWordHL[atMLISTBOX_STR_LEN];				// 하이라이트 속성
	atCOLOR			clHLColor;									// 하이라이트 색상
	int				nFontSize[atMLISTBOX_LINE_MAX];				// 폰트 크기
	atBOOL			bEnable;
	atBOOL			bChecked;
	atBOOL			bNotUseFilter;								// 특수문자 필터링 미사용 여부
	int				nStartHLIdx;								// 특수문자 필터링 시 앞쪽 Skip Index
	int				nEndHLIdx;									// 특수문자 필터링 시 뒤쪽 Skip Index
	void			*data;
} atMLISTBOX_ITEM, * atLPMLISTBOX_ITEM;

struct _tagatMLISTBOX;
typedef atVOID (atMLISTBOX_DRAWITEM_BG_FUNC)( atHCONTROL hControl, struct _tagatMLISTBOX *lpListBox, atHGDC hCanvas, atINT nIndex, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atLPMLISTBOX_ITEM lpItem, atBOOL bSelected, atBOOL bFocused, atLPVOID lpParam );
typedef atVOID (atMLISTBOX_DRAWITEM_FUNC)( atHCONTROL hControl, struct _tagatMLISTBOX *lpListBox, atHGDC hCanvas, atINT nIndex, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atBOOL bFirstNo, atINT nCheckType, atLPMLISTBOX_ITEM lpItem, atBOOL bSelected, atBOOL bFocused, atLPVOID lpParam );
typedef atINT (atMLISTBOX_TOUCHITEM_FUNC)( atHCONTROL hControl, struct _tagatMLISTBOX *lpListBox, atINT nIndex, atINT nMsg, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atLPVOID lpParam );
typedef atINT (atMLISTBOX_GETITEMHEIGHT_FUNC)( atHCONTROL hControl, struct _tagatMLISTBOX *lpListBox, atINT nIndex, atLPVOID lpParam );
typedef atINT (atMLISTBOX_GETCOUNT_FUNC)( atHCONTROL hControl, struct _tagatMLISTBOX *lpListBox, atLPVOID lpParam );
typedef atBOOL (atMLISTBOX_GETITEM_FUNC)( atHCONTROL hControl, struct _tagatMLISTBOX *lpListBox, atINT nIndex, atLPMLISTBOX_ITEM lpItem, atLPVOID lpParam );
typedef atBOOL (atMLISTBOX_SETITEM_FUNC)( atHCONTROL hControl, struct _tagatMLISTBOX *lpListBox, atINT nIndex, atLPMLISTBOX_ITEM lpItem, atLPVOID lpParam );

typedef struct _tagMLISTBOX_FUNCS {
	atMLISTBOX_DRAWITEM_BG_FUNC		*fnDrawItemBG;
	atMLISTBOX_DRAWITEM_FUNC		*fnDrawItem;
	atMLISTBOX_TOUCHITEM_FUNC		*fnTouchItem;
	atMLISTBOX_GETITEMHEIGHT_FUNC 	*fnGetItemHeight;
	atMLISTBOX_GETCOUNT_FUNC		*fnGetCount;
	atMLISTBOX_GETITEM_FUNC			*fnGetItem;
	atMLISTBOX_SETITEM_FUNC			*fnSetItem;
} atMLISTBOX_FUNCS, * atLPMLISTBOX_FUNCS;

typedef struct _tagatMLISTBOX {
	atDWORD		dwAttr;

	atMLISTBOX_FUNCS fnFuncs;
	atLPVOID			  lpFuncParam;

	int			nSelectIndex;
	int			nPrevSelectIndex;
	int			nItemHeight, nItemGap;
	int			nWidthMargin, nHeightMargin;
//	int			nOneScreenCount;
	int			nLineCnt, nNoAlignLine;

	atBOOL		bFirstNO;
	atINT		nCheckType;
	atBOOL		bTickerEnable;
	atBOOL		bImageVision;
	atBOOL		bScrollVisible;

	atUILIST				lpListBoxList;
	atLPMLISTBOX_ITEM		lpAddItemData;
	atINT					nAddItemIndex;

	atHBITMAP		lpUncheckedImage, lpCheckedImage;
	atHBITMAP		hImgNormalLeft, hImgNormalCenter, hImgNormalRight;
	atHBITMAP		hImgSelectLeft, hImgSelectCenter, hImgSelectRight;
	atHBITMAP		hImgDisableLeft, hImgDisableCenter, hImgDisableRight;
	atHBITMAP		hImgClickLeft, hImgClickCenter, hImgClickRight;
	atHCONTROL		hScrollBar;
	atHCONTROL      hTickerBox[atMLISTBOX_LINE_MAX];

	atINT		SelectKey, CheckKey;
	atINT		nViewTopY;
	atINT      	nCurCheckedIndex;
	atBOOL		bTickerActivate[atMLISTBOX_LINE_MAX];
	atINT		nMostAlign[atMLISTBOX_LINE_MAX];		// Item Text Align Attribute
	atINT		nWidthPercent[atMLISTBOX_LINE_MAX];		// Item Text Draw Width Percent
	atRECT		rtControlRect[atMLISTBOX_LINE_MAX];		// Item Control Rect

	atBOOL		bMouseDown, bMouseScroll;
	atBOOL		bActionValidate; 
	int			nFirstViewTopY;
	int			nFirstMouseX, nFirstMouseY;
	int			nPrevMouseX, nPrevMouseY;
	atBOOL		bIsChange;
	
	atBOOL		bReadyClick;
	atBOOL		bReadySmoothScroll;
	atBOOL		bUseSmoothScroll;		// 스무스 스크롤 사용여부
} atMLISTBOX,  *atLPMLISTBOX;

atHCONTROL	atMLISTBOX_Create( int nIndex, int nLineCnt, int nX, int nY, int nWidth, int nHeight,
							   int nItemHeight, int nItemGap, int nWidthMargin, int nHeightMargin,
							   atBOOL bFirstNO, atINT nCheckType, atBOOL bTicker, 
							   atHBITMAP hImgNormalLeft, atHBITMAP hImgNormalCenter, atHBITMAP hImgNormalRight,
							   atHBITMAP hImgSelectLeft, atHBITMAP hImgSelectCenter, atHBITMAP hImgSelectRight,
							   atHBITMAP hImgDisableLeft, atHBITMAP hImgDisableCenter, atHBITMAP hImgDisableRight,
							   atHBITMAP lpUncheckedImage, atHBITMAP lpCheckedImage,
							   //atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom, atHBITMAP lpScrollcursorImage );
							   atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom,
							   atHBITMAP lpScrollCursorImgTop, atHBITMAP lpScrollCursorImgMid, atHBITMAP lpScrollCursorImgBot );

atDWORD		atMLISTBOX_GetAttr(atHCONTROL hControl );
void		atMLISTBOX_SetAttr(atHCONTROL hControl, atDWORD dwAttr );
void		atMLISTBOX_SetBitmap( atHCONTROL hControl,
									 atHBITMAP hImgNormalLeft, atHBITMAP hImgNormalCenter, atHBITMAP hImgNormalRight,
									 atHBITMAP hImgSelectLeft, atHBITMAP hImgSelectCenter, atHBITMAP hImgSelectRight,
									 atHBITMAP hImgDisableLeft, atHBITMAP hImgDisableCenter, atHBITMAP hImgDisableRight );

void		atMLISTBOX_Start_AddItem(atHCONTROL hControl);
void		atMLISTBOX_Start_AddItemEx(atHCONTROL hControl, atINT nIndex);
void 		atMLISTBOX_AddItem_Enabled(atHCONTROL hControl, atBOOL bEnabled);
void		atMLISTBOX_AddItem_Checked(atHCONTROL hControl, atBOOL bChecked);
void		atMLISTBOX_AddItem_NO(atHCONTROL hControl, int nNO);
void		atMLISTBOX_AddItem_ItemID(atHCONTROL hControl, int nItemID);
void		atMLISTBOX_AddItem_String(atHCONTROL hControl, atLPTSTR szItemName, int nIndex, int nLineIdx);
void		atMLISTBOX_AddItem_IDS(atHCONTROL hControl, atDWORD ids, int nIndex, int nLineIdx);
void		atMLISTBOX_AddItem_String2(atHCONTROL hControl, atLPTSTR szItemName, atCOLOR clTextColor, int nIndex, int nLineIdx);
void		atMLISTBOX_AddItem_String3(atHCONTROL hControl, atLPTSTR szItemName, int nIndex, int nLineIdx, int nFontSize);
void		atMLISTBOX_AddItem_Image(atHCONTROL hControl, atHBITMAP imgListImage, int nIndex);
void		atMLISTBOX_AddItem_Image2(atHCONTROL hControl, atHBITMAP imgListImage, int x, int y, int width, int height, int nIndex);
void		atMLISTBOX_Update_AddItem(atHCONTROL hControl);
void 		atMLISTBOX_ClearAllItem( atHCONTROL hControl );
void    	atMLISTBOX_DeleteItem(atHCONTROL hControl, int nIndex);
int 		atMLISTBOX_GetCount(atHCONTROL hControl);
int 		atMLISTBOX_GetSelectIndex(atHCONTROL hControl);
atBOOL 		atMLISTBOX_SetItem(atHCONTROL hControl, int nIndex, atLPMLISTBOX_ITEM lpItem);
atBOOL		atMLISTBOX_SetSelectItem(atHCONTROL hControl, atMLISTBOX_ITEM *lpItem);
atVOID 		atMLISTBOX_RealizeItem(atHCONTROL hControl);

atBOOL		atMLISTBOX_GetItem(atHCONTROL hControl, int nIndex, atMLISTBOX_ITEM *lpItem);
atBOOL 		atMLISTBOX_GetSelectItem(atHCONTROL hControl, atMLISTBOX_ITEM *lpItem);

atBOOL 		atMLISTBOX_SetSelectIndex( atHCONTROL hControl, int nIndex );
atBOOL		atMLISTBOX_SetSelectIndexByPage(atHCONTROL hControl, int page);
int 		atMLISTBOX_MoveSelect(atHCONTROL hControl, int nDirect);
atBOOL		atMLISTBOX_ToggleSelectItemChecked(atHCONTROL hControl);
void		atMLISTBOX_SetItemCheck(atHCONTROL hControl, int nIndex, atBOOL bCheck);
atBOOL 		atMLISTBOX_GetItemChecked(atHCONTROL hControl, int nIndex);
atINT 		atMLISTBOX_GetCurCheckedIndex(atHCONTROL hControl);
void		atMLISTBOX_SetItemEnabled(atHCONTROL hControl, int nIndex, atBOOL bEnable);
atBOOL 		atMLISTBOX_GetItemEnabled(atHCONTROL hControl, int nIndex);
atLPTSTR 	atMLISTBOX_GetItemString(atHCONTROL hControl, int nIndex, int nLineIdx, int nStringIndex);
atVOID 		atMLISTBOX_AddItem_Data(atHCONTROL hControl, void*data);
atVOID* 	atMLISTBOX_GetItemData(atHCONTROL hControl, int nIndex);
atVOID* 	atMLISTBOX_GetCurItemData(atHCONTROL hControl);
atINT 		atMLISTBOX_ResetSelect(atHCONTROL hControl);
atBOOL 		atMLISTBOX_SetViewTopY(atHCONTROL hControl, atINT nViewTopY);
atINT 		atMLISTBOX_GetViewTopY(atHCONTROL hControl);
void 		atMLISTBOX_SetupKey( atHCONTROL hControl, atINT SelectKey, atINT CheckKey );

void		atMLISTBOX_GetFuncs( atHCONTROL hControl, atLPMLISTBOX_FUNCS lpFuncs, atLPVOID *lppFuncParam );
void		atMLISTBOX_SetupFuncs( atHCONTROL hControl, atLPMLISTBOX_FUNCS lpFuncs, atLPVOID lpFuncParam );

atBOOL		atMLISTBOX_SetNoAlignLine(atHCONTROL hControl, int nAlignLine);
atINT		atMLISTBOX_GetNoAlignLine(atHCONTROL hControl);

atBOOL		atMLISTBOX_SetTextColor(atHCONTROL hControl, atINT nIndex, atINT nLineIdx, atINT nStringIndex, atCOLOR clTextColor);
atCOLOR		atMLISTBOX_GetTextColor(atHCONTROL hControl, atINT nIndex, atINT nLineIdx, atINT nStringIndex);

atBOOL		atMLISTBOX_SetFontSize(atHCONTROL hControl, atINT nIndex, atINT nLineIdx, atINT nFontSize);
atINT		atMLISTBOX_GetFontSize(atHCONTROL hControl, atINT nIndex, atINT nLineIdx);

// Kyle Start [Add 2009/07/23 - 15:45]
atVOID		atMLISTBOX_SetUseSmoothScroll( atHCONTROL hControl, atBOOL bUsed );
atBOOL		atMLISTBOX_GetUseSmoothScroll( atHCONTROL hControl );
// Kyle [Add 2009/07/23 - 15:45] End

atVOID		atMLISTBOX_SetVisibleScroll( atHCONTROL hControl, atBOOL bVisible );
atBOOL		atMLISTBOX_GetVisibleScroll( atHCONTROL hControl );

atINT		atMLISTBOX_GetSelectImageIndex( atHCONTROL hControl, atINT nIndex );
atVOID		atMLISTBOX_SetItemMargin( atHCONTROL hControl, int nWidthMargin, int nHeightMargin );
atVOID		atMLISTBOX_GetItemMargin( atHCONTROL hControl, int* nWidthMargin, int* nHeightMargin );
atVOID		atMLISTBOX_SetItemGap( atHCONTROL hControl, int nItemGap );
int			atMLISTBOX_GetItemGap( atHCONTROL hControl );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __CMP_MLISTBOX_H_
