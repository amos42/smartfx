#ifndef __CMP_LISTBOX_H_
#define __CMP_LISTBOX_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define atLISTBOX_ATTR_OUTLINE				(0x8000)
#define atLISTBOX_ATTR_OPAQUE				(0x4000)

#define atLISTBOX_ATTR_MOVE_NORMAL		(0x0000)
#define atLISTBOX_ATTR_MOVE_WRAP 			(0x1000)
#define atLISTBOX_ATTR_MOVE_NEXTCONTROL 	(0x2000)


#define atLISTBOX_STR_LEN									128		// string의 최대 lenth


typedef enum {
	atLISTBOX_IMG_POST = 0,
	atLISTBOX_IMG_POST_SELECT,
//	atLISTBOX_IMG_TAIL_1,
//	atLISTBOX_IMG_TAIL_2,
//	atLISTBOX_IMG_TAIL_3,
//	atLISTBOX_IMG_TAIL_4,
	atLISTBOX_IMG_MAX
} atLISTBOX_IMG;		

typedef enum {
	atLISTBOX_STR_NORMAL = 0,
	atLISTBOX_STR_SELECT,
	atLISTBOX_STR_MAX 
} atLISTBOUI_STR;		

typedef	struct	_tagatLISTBOXITEM {
	int				nNO;
	atTCHAR			szItemName[atLISTBOX_STR_MAX][atLISTBOX_STR_LEN];
	atDWORD         nItemNameIDS[atLISTBOX_STR_MAX];
	atHBITMAP		imgItemImage[atLISTBOX_IMG_MAX];  // [6] -> [2];
	atSREGION		imgItemImageRegion[atLISTBOX_IMG_MAX];  // [6] -> [2];
	atBOOL			bEnable;
	atBOOL			bChecked;
	void				*data;
} atLISTBOXITEM, * atLPLISTBOXITEM;


typedef atINT (atLISTBOX_GETCOUNT_FUNC)( atHCONTROL hControl );
typedef atLISTBOX_GETCOUNT_FUNC * atLP_LISTBOX_GETCOUNT_FUNC;
typedef atBOOL (atLISTBOX_GETITEM_FUNC)( atHCONTROL hControl, atLPVOID, atINT, atLPLISTBOXITEM * );
typedef atLISTBOX_GETITEM_FUNC * atLP_LISTBOX_GETITEM_FUNC;
typedef atVOID (atLISTBOX_DRAWITEM_FUNC)( atHGDC hCanvas, atLPCONTROL lpControl, atINT nIndex, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atBOOL bFirstNo, atINT nCheckType, atLPLISTBOXITEM lpItem, atBOOL bSelected, atBOOL bFocused );
typedef atLISTBOX_DRAWITEM_FUNC * atLP_LISTBOX_DRAWITEM_FUNC;
typedef atVOID (atLISTBOX_CHANGEITEM_FUNC)( atINT, atINT, atLPLISTBOXITEM, atLPLISTBOXITEM, atBOOL );
typedef atLISTBOX_CHANGEITEM_FUNC * atLP_LISTBOX_CHANGEITEM_FUNC;


atHCONTROL atLISTBOX_Create( int nIndex, int nX, int nY, int nWidth, int nHeight,
						  int nItemHeight, atBOOL bFirstNO, atINT nCheckType );

void				atLISTBOX_SetFunc( atHCONTROL hControl, atLP_LISTBOX_DRAWITEM_FUNC fnDraw, atLP_LISTBOX_CHANGEITEM_FUNC fnChange );

atDWORD		atLISTBOX_GetAttr(atHCONTROL hControl );
void				atLISTBOX_SetAttr(atHCONTROL hControl, atDWORD uAttr );

void				atLISTBOX_Start_AddItem(atHCONTROL hControl);
void				atLISTBOX_Start_AddItemEx(atHCONTROL hControl, atINT nIndex);
void 			atLISTBOX_AddItem_Enabled(atHCONTROL hControl, atBOOL bEnabled);
void				atLISTBOX_AddItem_Checked(atHCONTROL hControl, atBOOL bChecked);	// List No 값 Add
void				atLISTBOX_AddItem_NO(atHCONTROL hControl, int nNO);	// List No 값 Add
void				atLISTBOX_AddItem_String(atHCONTROL hControl, atLPTSTR szItemName, int nIndex) ;
void				atLISTBOX_AddItem_Image(atHCONTROL hControl, atHBITMAP imgListImage, int nIndex);
void				atLISTBOX_AddItem_Image2(atHCONTROL hControl, atHBITMAP imgListImage, int x, int y, int width, int height, int nIndex);
void				atLISTBOX_Update_AddItem(atHCONTROL hControl);

void 				atLISTBOX_ClearAllItem( atHCONTROL hControl );
void    			atLISTBOX_DeleteItem(atHCONTROL hControl, int nIndex);

int 				atLISTBOX_GetCount(atHCONTROL hControl);

atLPLISTBOXITEM		atLISTBOX_GetItem(atHCONTROL hControl, int nIndex);

int 				atLISTBOX_GetSelectIndex(atHCONTROL hControl);
atLPLISTBOXITEM 	atLISTBOX_GetSelectItem(atHCONTROL hControl);
atVOID 			atLISTBOX_RealizeItem(atHCONTROL hControl);

atBOOL 				atLISTBOX_SetSelectIndex( atHCONTROL hControl, int nIndex );
atBOOL				atLISTBOX_SetSelectIndexByPage(atHCONTROL hControl, int page);
int 				atLISTBOX_MoveSelect(atHCONTROL hControl, int nDirect);

atBOOL				atLISTBOX_ToggleSelectItemChecked(atHCONTROL hControl);
void					atLISTBOX_SetItemCheck(atHCONTROL hControl, int nIndex, atBOOL bCheck);
atBOOL 				atLISTBOX_GetItemChecked(atHCONTROL hControl, int nIndex);
atINT 				atLISTBOX_GetCurCheckedIndex(atHCONTROL hControl);

void				atLISTBOX_SetItemEnabled(atHCONTROL hControl, int nIndex, atBOOL bEnable);
atBOOL 				atLISTBOX_GetItemEnabled(atHCONTROL hControl, int nIndex);

atLPTSTR 			atLISTBOX_GetItemString(atHCONTROL hControl, int nIndex, int nStringIndex);
atVOID 				atLISTBOX_AddItem_Data(atHCONTROL hControl, void*data);
atVOID* 			atLISTBOX_GetItemData(atHCONTROL hControl, int nIndex);
atVOID* 			atLISTBOX_GetCurItemData(atHCONTROL hControl);


#ifdef __cplusplus
}
#endif


#endif
