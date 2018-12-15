#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_ListBoxEx.h"
#include "CMP_ScrollBarEx.h"
#include "CMP_TickerBox.h"
#include "AEECOMP_BasePkg.h"

// 터치 포인트를 움직일 때, 선택 오차인지 이동/스크롤인지 체크하기 위한 한계값. (unit : Inch)
#define CHECK_POINT_MOVE_LIMIT			0.05

// Kyle Start [Add 2009/07/23-14:51]
#define CHECK_ACCELTICK_TIME_LIMIT		20		// 스무스 스크롤을 스무스 랜더링하기 위한 Timer
#define CHECK_ACCELMOVE_TIME_LIMIT		200		// 스무스 스크롤에 대한 이동 제한 시간 (이후 스무스 스크롤 초기화)

//#define ACCEL_VALUE_DRAGDISTANCE_FACTOR	5.0f
//#define ACCEL_VALUE_DRAGDISTANCE_FACTOR	(atREAL)0x00020000//131072//(2.0f*65536)		// 드래그 거리에 적용인자
#define ACCEL_VALUE_DRAGDISTANCE_FACTOR		(atREAL)0x00030000//196608//(3.0f*65536)		// 드래그 거리에 적용인자
//#define ACCEL_VALUE_MAX_VELOCITY			500.0f
//#define ACCEL_VALUE_MAX_VELOCITY			(atREAL)0x012C0000//19660800//(300.0f*65536)	// 최대 속도(속력)
#define ACCEL_VALUE_MAX_VELOCITY			(atREAL)0x01900000//26214400//(400.0f*65536)	// 최대 속도(속력)
//#define ACCEL_VALUE_DEACCEL_FACTOR		0.9f
//#define ACCEL_VALUE_DEACCEL_FACTOR		(atREAL)0x0000CCCC//52428//(0.8f*65536)		// 감속도 인자 퍼센티지(%) - 현재 20%
#define ACCEL_VALUE_DEACCEL_FACTOR			(atREAL)0x0000D999//55705//(0.85f*65536)	// 감속도 인자 퍼센티지(%) - 현재 15%
// Kyle [Add 2009/07/23-14:51] End

static void _SetScrollBarPosition(atHCONTROL hControl, atLPLISTBOX_EXT lpListBox);
static void _Realloc_Items( atHCONTROL hControl );
static void _SetScrollPos( atHCONTROL hControl, int scroll_pos, atBOOL bChangeScrollBar );

static int ListBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );

#define 	COLOR_SELECTBAR 		(atUIAPI_GetRGB(0,120,200))
#define		TICKER_INTERVAL			50
#define		IMAGE_WIDTH				16

struct _CreateParam {
	int nItemHeight, nItemGap;
	int nWidthMargin, nHeightMargin;
	int nWidthBorder, nHeightBorder;
	atBOOL bFirstNO;
	int nCheckType;
	atBOOL bTicker;
	atHBITMAP hImgNormalLeft, hImgNormalCenter, hImgNormalRight;
	atHBITMAP hImgSelectLeft, hImgSelectCenter, hImgSelectRight;
	atHBITMAP hImgDisableLeft, hImgDisableCenter, hImgDisableRight;
	atHBITMAP lpUncheckedImage, lpCheckedImage;
	//atHBITMAP lpScrollbarImgTop, lpScrollbarImage, lpScrollbarImgBottom, lpScrollcursorImage;
	atHBITMAP lpScrollbarImgTop, lpScrollbarImage, lpScrollbarImgBottom;
	atHBITMAP lpScrollcursorImgTop, lpScrollcursorImgMid, lpScrollcursorImgBot;
} ;


static atINT _defGetItemHeight( atHCONTROL hControl, atINT nIndex, atLPVOID lpParam )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr(hControl);

	return (lpListBox != atNULL)? lpListBox->nItemHeight : 0;
}

static atVOID _defDrawItemBG( atHGDC hCanvas, atHCONTROL hControl, atINT nIndex, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atLPLISTBOX_EXT_ITEM lpItem, atBOOL bSelected, atBOOL bFocused, atLPVOID lpParam )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr(hControl);
	int x, y, w, h, xx;
	atHBITMAP			lpBM 		= atNULL;
//	atCOLOR nColor;

	x = nX;
	y = nY;
	w = nWidth;
	h = nHeight;
	xx = x + w;

	if( lpItem == atNULL ){
		return;
	}
		
	if(bSelected){	// List Item Select
		// Select Bar Draw
		if( bFocused ){
			if( lpListBox->hImgSelectLeft && lpListBox->hImgSelectCenter && lpListBox->hImgSelectRight ){
				atUIAPI_DrawClient1Line( hCanvas, x, nY, w, 
									  	 lpListBox->hImgSelectLeft, lpListBox->hImgSelectCenter, lpListBox->hImgSelectRight );
			} else {
				atUIAPI_FillRect(hCanvas, x, nY, w, nHeight, lpControl->tControlColor.clFocusedFrColor);
			}
		} else {
			if( lpListBox->hImgDisableLeft && lpListBox->hImgDisableCenter && lpListBox->hImgDisableRight ){
				atUIAPI_DrawClient1Line( hCanvas, x, nY, w, 
									  	 lpListBox->hImgDisableLeft, lpListBox->hImgDisableCenter, lpListBox->hImgDisableRight );
			} else {
				atUIAPI_FillRect(hCanvas, x, nY, w, nHeight, lpControl->tControlColor.clDisableBgColor);
			}
		}
	} else { // List Item nonSelect
		if( lpListBox->hImgNormalLeft && lpListBox->hImgNormalCenter && lpListBox->hImgNormalRight ){
			atUIAPI_DrawClient1Line( hCanvas, x, nY, w, 
									 lpListBox->hImgNormalLeft, lpListBox->hImgNormalCenter, lpListBox->hImgNormalRight );
		} else {
			atUIAPI_FillRect(hCanvas, x, nY, w, nHeight, lpControl->tControlColor.clNormalBgColor);
		}
	}

	if( lpItem == atNULL ){
		return;
	}
}

static atVOID _defDrawItem( atHGDC hCanvas, atHCONTROL hControl, atINT nIndex, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atBOOL bFirstNo, atINT nCheckType, atLPLISTBOX_EXT_ITEM lpItem, atBOOL bSelected, atBOOL bFocused, atLPVOID lpParam )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr(hControl);
	int x, y, w, h, xx;
//	int nTempWidth1 = 0, nTempWidth2 = 0, nTempWidth3 = 0, nTempWidth4 = 0;
	int	wHeadImage = 0, hHeadImage = 0;
	int	xChk, xNO, xImg, xStr;
	int ChkWidth, NoWidth, ImgWidth, StrWidth;
	atHBITMAP			lpBM 		= atNULL;
//	atLPLISTBOX_EXT_ITEM	lpItem		= atNULL;
//	int nCurPage = 0, nCurIndex = 0, nTotalPage = 1;
	atLPTSTR disp_str;
	atTCHAR szBuff[atLISTBOX_EXT_STR_LEN] = {0};
//	atLPCONTROL		lpControl2;
	atCOLOR nColor;
	int img_width, img_height;
	int img2_width, img2_height;
		atTCHAR	szNO[12]	= {0, };

	x = nX;
	y = nY;
	w = nWidth;
	h = nHeight;
	xx = x + w;

	if( lpItem == atNULL ){
		return;
	}
		
	// 체크박스 찍을 위치
	xChk	= x + lpListBox->nWidthMargin ;
	if( (lpListBox->nCheckType == 1) || (lpListBox->nCheckType == 2) ){
		lpBM = lpListBox->lpUncheckedImage;
		if( lpBM ){
			if( atUIAPI_GetBitmapHeight(lpBM) <= nHeight ){
				ChkWidth = atUIAPI_GetBitmapWidth(lpBM);
			} else {
				ChkWidth = atUIAPI_GetBitmapWidth(lpBM);
			}
		} else {
			ChkWidth = lpListBox->nItemHeight;
		}
	} else
		ChkWidth = 0;

	// 숫자 찍을 위치
	xNO		= xChk + ChkWidth; if( ChkWidth > 0 ) xNO += lpListBox->nItemGap;
	// 가장 큰 숫자의 수평 크기
	if (lpListBox->bFirstNO == atTRUE){
		UI_SPRINTF(szBuff, _AT("%d."), lpItem->nNO );
		NoWidth = atUIAPI_GetStringWidth(atUIAPI_GetFont(hCanvas), szBuff, -1);
	} else 
		NoWidth = 0;

	// 첫 이미지 찍을 위치
	xImg	= xNO + NoWidth; if( NoWidth > 0 ) xImg += lpListBox->nItemGap;
	// 첫번째 이미지 수평 크기
	if ((lpItem->imgItemImage[0] != atNULL) || (lpItem->imgItemImage[1] != atNULL)){
		if( lpItem->imgItemImage[0] != atNULL ) {
			if( lpItem->imgItemImageRegion[0].nWidth >= 0 ){
				ImgWidth = lpItem->imgItemImageRegion[0].nWidth;
			} else {
				ImgWidth = atUIAPI_GetBitmapWidth(lpItem->imgItemImage[0]);
			}
		} else {
			ImgWidth = atUIAPI_GetBitmapWidth(lpItem->imgItemImage[1]);
		}
	} else {
		ImgWidth = 0;
	}
	
	if(bSelected){	// List Item Select
		// to-do 
		//     String Resource가 변결된 시점에만, Text를 갱신하도록 변경 필요.
		if(lpItem->nItemNameIds[1])
			atUIAPI_LoadString(lpItem->nItemNameIds[1], lpItem->szItemName[1], atLISTBOX_EXT_STR_LEN);

		disp_str = lpItem->szItemName[1];
		if( UI_STRLEN(disp_str) <= 0 ){ 
			if(lpItem->nItemNameIds[0])
				atUIAPI_LoadString(lpItem->nItemNameIds[0], lpItem->szItemName[0],  atLISTBOX_EXT_STR_LEN);

			disp_str = lpItem->szItemName[0];
			//nColor =  (lpItem->clItemColor[0] != 0xFFFFFFFF)? lpItem->clItemColor[0] : lpControl->tControlColor.clFocusedTxtColor;
			nColor =  (lpItem->clItemColor[1] != 0xFFFFFFFF)? lpItem->clItemColor[1] : lpControl->tControlColor.clFocusedTxtColor;
		} else {
			nColor =  (lpItem->clItemColor[1] != 0xFFFFFFFF)? lpItem->clItemColor[1] : lpControl->tControlColor.clFocusedTxtColor;
		}
	} else { // List Item nonSelect

		if(lpItem->nItemNameIds[0])
		{
			atUIAPI_LoadString(lpItem->nItemNameIds[0], lpItem->szItemName[0], atLISTBOX_EXT_STR_LEN);
		}

		disp_str = lpItem->szItemName[0];
		if( lpItem->bEnable )
			nColor =  (lpItem->clItemColor[0] != 0xFFFFFFFF)? lpItem->clItemColor[0] : lpControl->tControlColor.clNormalTxtColor;
		else
			nColor = atUIAPI_GetRGB(102, 102, 102);
			//nColor = atUIAPI_GetRGB(160, 160, 160);
	}

	if (lpItem->imgItemImage[atLISTBOX_EXT_IMG_POST] != atNULL){
		if( lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_POST].nWidth >= 0 ) {
			wHeadImage	= lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_POST].nWidth;
		} else {
			wHeadImage	= atUIAPI_GetBitmapWidth(lpItem->imgItemImage[atLISTBOX_EXT_IMG_POST]);
		}
		if( lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_POST].nHeight >= 0 ) {
			hHeadImage	= lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_POST].nHeight;
		} else {
			hHeadImage	= atUIAPI_GetBitmapHeight(lpItem->imgItemImage[atLISTBOX_EXT_IMG_POST]);
		}
	}

	if ( (lpListBox->nCheckType == 1) || (lpListBox->nCheckType == 2) ){
		lpBM = (lpItem->bChecked)? lpListBox->lpCheckedImage : lpListBox->lpUncheckedImage;
		if( lpBM ){
			if( atUIAPI_GetBitmapHeight(lpBM) <= lpListBox->nItemHeight ){
				atUIAPI_DrawImageAlign(hCanvas, xChk+1, nY+lpListBox->nItemHeight/2, lpBM, ALIGN_LEFT_MIDDLE );
			} else {
				atUIAPI_DrawImageStretch(hCanvas, xChk+1, nY, lpBM, lpListBox->nItemHeight, lpListBox->nItemHeight );
			}
		} else {
			// 체크박스 찍을 위치
			if ( lpListBox->nCheckType == 1 ){
				int len = lpListBox->nItemHeight - 2*2;
				if(lpItem->bChecked){
					atUIAPI_DrawRect( hCanvas, xChk, nY+2, len, len, nColor, 1 );
					atUIAPI_DrawLine( hCanvas, xChk, nY+2, xChk+len-1, nY+2+len-1, nColor, 1, atFALSE );
					atUIAPI_DrawLine( hCanvas, xChk, nY+2+len-1, xChk+len-1, nY+2, nColor, 1, atFALSE );
				} else {
					atUIAPI_DrawRect( hCanvas, xChk, nY+2, len, len, nColor, 1 );
				}
			} else if ( lpListBox->nCheckType == 2 ){
				int ratio = (lpListBox->nItemHeight - 2*2) / 2;
				if(lpItem->bChecked){
					atUIAPI_DrawCircle( hCanvas, xChk+ratio, nY+lpListBox->nItemHeight/2, ratio, nColor, 1 );
					atUIAPI_FillCircle( hCanvas, xChk+ratio, nY+lpListBox->nItemHeight/2, 5, nColor );
				} else {
					atUIAPI_DrawCircle( hCanvas, xChk+ratio, nY+lpListBox->nItemHeight/2, ratio, nColor, 1 );
				}
			}
		}
	}

	if( lpListBox->bFirstNO ){
		UI_SPRINTF(szNO, _AT("%d."), lpItem->nNO);
		atUIAPI_DrawStringAlign(hCanvas, xNO, nY+lpListBox->nItemHeight/2, szNO, ALIGN_LEFT_MIDDLE, nColor);
	}
	
	if( bSelected && (lpItem->imgItemImage[atLISTBOX_EXT_IMG_POST_SELECT] != atNULL) ){
		img_width = lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_POST_SELECT].nWidth;
		if( img_width <= 0 ) img_width = atUIAPI_GetBitmapWidth(lpItem->imgItemImage[atLISTBOX_EXT_IMG_POST_SELECT]);
		img_height = lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_POST_SELECT].nHeight;
		if( img_height <= 0 ) img_height = atUIAPI_GetBitmapHeight(lpItem->imgItemImage[atLISTBOX_EXT_IMG_POST_SELECT]);
		if( img_height > lpListBox->nItemHeight ) img_height = lpListBox->nItemHeight;
		atUIAPI_DrawImageAlignPart(hCanvas, xImg, nY+lpListBox->nItemHeight/2, 
									lpItem->imgItemImage[atLISTBOX_EXT_IMG_POST_SELECT], 
									lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_POST_SELECT].nX, lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_POST_SELECT].nY, img_width, img_height,
									ALIGN_LEFT_MIDDLE );
	} else if( lpItem->imgItemImage[atLISTBOX_EXT_IMG_POST] != atNULL ){
		img_width = lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_POST].nWidth;
		if( img_width <= 0 ) img_width = atUIAPI_GetBitmapWidth(lpItem->imgItemImage[atLISTBOX_EXT_IMG_POST]);
		img_height = lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_POST].nHeight;
		if( img_height <= 0 ) img_height = atUIAPI_GetBitmapHeight(lpItem->imgItemImage[atLISTBOX_EXT_IMG_POST]);
		if( img_height > lpListBox->nItemHeight ) img_height = lpListBox->nItemHeight;
		atUIAPI_DrawImageAlignPart(hCanvas, xImg, nY+lpListBox->nItemHeight/2, 
									lpItem->imgItemImage[atLISTBOX_EXT_IMG_POST], 
									lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_POST].nX, lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_POST].nY, img_width, img_height,
									ALIGN_LEFT_MIDDLE );
	} else {
		img_width = img_height = 0;
	}

	if( bSelected && (lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL_SELECT] != atNULL) ){
		img2_width = lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL_SELECT].nWidth;
		if( img2_width <= 0 ) img2_width = atUIAPI_GetBitmapWidth(lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL_SELECT]);
		img2_height = lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL_SELECT].nHeight;
		if( img2_height <= 0 ) img2_height = atUIAPI_GetBitmapHeight(lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL_SELECT]);
		if( img2_height > lpListBox->nItemHeight ) img2_height = lpListBox->nItemHeight;
		atUIAPI_DrawImageAlignPart(hCanvas, w - (img2_width + lpListBox->nItemGap), nY+lpListBox->nItemHeight/2, 
									lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL_SELECT], 
									lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL_SELECT].nX, lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL_SELECT].nY, img2_width, img2_height,
									ALIGN_RIGHT_MIDDLE );
	} else if( !lpItem->bEnable && lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL_DISABLE] != atNULL ){
		img2_width = lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL_DISABLE].nWidth;
		if( img2_width <= 0 ) img2_width = atUIAPI_GetBitmapWidth(lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL_DISABLE]);
		img2_height = lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL_DISABLE].nHeight;
		if( img2_height <= 0 ) img2_height = atUIAPI_GetBitmapHeight(lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL_DISABLE]);
		if( img2_height > lpListBox->nItemHeight ) img2_height = lpListBox->nItemHeight;
		atUIAPI_DrawImageAlignPart(hCanvas, w - (img2_width + lpListBox->nItemGap), nY+lpListBox->nItemHeight/2, 
									lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL_DISABLE], 
									lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL_DISABLE].nX, lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL_DISABLE].nY, img2_width, img2_height,
									ALIGN_RIGHT_MIDDLE );
	} else if( lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL] != atNULL ){
		img2_width = lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL].nWidth;
		if( img2_width <= 0 ) img2_width = atUIAPI_GetBitmapWidth(lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL]);
		img2_height = lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL].nHeight;
		if( img2_height <= 0 ) img2_height = atUIAPI_GetBitmapHeight(lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL]);
		if( img2_height > lpListBox->nItemHeight ) img2_height = lpListBox->nItemHeight;
		atUIAPI_DrawImageAlignPart(hCanvas, w - (img2_width + lpListBox->nItemGap), nY+lpListBox->nItemHeight/2, 
									lpItem->imgItemImage[atLISTBOX_EXT_IMG_TAIL], 
									lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL].nX, lpItem->imgItemImageRegion[atLISTBOX_EXT_IMG_TAIL].nY, img2_width, img2_height,
									ALIGN_RIGHT_MIDDLE );
	} else {
		img2_width = img2_height = 0;
	}

	xStr	= xImg + img_width;
	if( img_width > 0 ) xStr  += lpListBox->nItemGap;
	StrWidth = w - xStr;
	if( img2_width > 0 ) StrWidth -= img2_width + lpListBox->nItemGap;
	
	if( bSelected && lpListBox->bTickerEnable ){
		if(atCONTROL_IsVisible(lpListBox->hTickerBox) && lpListBox->bTickerActivate ){
//				atCONTROL_Draw( lpListBox->hTickerBox, atFALSE);
//				atCONTROL_Refresh( lpListBox->hTickerBox );
		} else {
			atUIAPI_DrawStringAlign(hCanvas, xStr, nY+lpListBox->nItemHeight/2, disp_str, ALIGN_LEFT_MIDDLE, nColor);
		}
	} else {
	  myStringCut( hCanvas, disp_str, szBuff, StrWidth, atTRUE);	
	  atUIAPI_DrawStringAlign(hCanvas, xStr, nY+lpListBox->nItemHeight/2, szBuff, ALIGN_LEFT_MIDDLE, nColor);
	}

//	if(atCONTROL_IsVisible(lpListBox->hScrollBar) == atTRUE)	{
//		atCONTROL_Draw( lpListBox->hScrollBar, atFALSE );
//	}

//atUIAPI_DrawRect( hCanvas, 0, 0, RECT_WIDTH(lpControl->Base.rect), RECT_HEIGHT(lpControl->Base.rect), atUI_COLOR_RED, 1 );
//atUIAPI_DrawLine( hCanvas, 0, 0, RECT_WIDTH(lpControl->Base.rect)-1, RECT_HEIGHT(lpControl->Base.rect)-1, atUI_COLOR_RED, 1, atFALSE );
//atUIAPI_DrawLine( hCanvas, RECT_WIDTH(lpControl->Base.rect)-1, 0, 0, RECT_HEIGHT(lpControl->Base.rect)-1, atUI_COLOR_RED, 1, atFALSE );
}


atHCONTROL atLISTBOX_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight,
						  int nItemHeight, int nItemLineGap, 
						  int nWidthMargin, int nHeightMargin,
						  atBOOL bFirstNO, atINT nCheckType, atBOOL bTicker, 
						  atHBITMAP hImgNormalLeft, atHBITMAP hImgNormalCenter, atHBITMAP hImgNormalRight,
						  atHBITMAP hImgSelectLeft, atHBITMAP hImgSelectCenter, atHBITMAP hImgSelectRight,
						  atHBITMAP hImgDisableLeft, atHBITMAP hImgDisableCenter, atHBITMAP hImgDisableRight,
						  atHBITMAP lpUncheckedImage, atHBITMAP lpCheckedImage,
						  //atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom, atHBITMAP lpScrollcursorImage )
						  atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom,
						  atHBITMAP lpScrollcursorImgTop, atHBITMAP lpScrollcursorImgMid, atHBITMAP lpScrollcursorImgBot )
{
	return atLISTBOX_EXT_CreateEx( nIndex,  nX,  nY,  nWidth,  nHeight,
						   nItemHeight,  nItemLineGap, 
						   nWidthMargin,  nHeightMargin,
						  0, 0,
						   bFirstNO,  nCheckType,  bTicker, 
						   hImgNormalLeft,  hImgNormalCenter,  hImgNormalRight,
						   hImgSelectLeft,  hImgSelectCenter,  hImgSelectRight,
						   hImgDisableLeft,  hImgDisableCenter,  hImgDisableRight,
						   lpUncheckedImage,  lpCheckedImage,
						   lpScrollbarImgTop,  lpScrollbarImage,  lpScrollbarImgBottom,
						   lpScrollcursorImgTop,  lpScrollcursorImgMid,  lpScrollcursorImgBot );
}

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
						  atHBITMAP lpScrollcursorImgTop, atHBITMAP lpScrollcursorImgMid, atHBITMAP lpScrollcursorImgBot )
{
	atHCONTROL hControl;
	struct _CreateParam param;

	param.nItemHeight = nItemHeight;
	param.nItemGap = nItemLineGap;
	param.nWidthMargin = nWidthMargin;
	param.nHeightMargin = nHeightMargin;
	param.nWidthBorder = nWidthBorder;
	param.nHeightBorder = nHeightBorder;
	param.bTicker		= bTicker;
	param.bFirstNO	= bFirstNO;
	param.nCheckType		= nCheckType;
	param.hImgNormalLeft	= hImgNormalLeft;
	param.hImgNormalCenter	= hImgNormalCenter;
	param.hImgNormalRight	= hImgNormalRight;
	param.hImgSelectLeft	= hImgSelectLeft;
	param.hImgSelectCenter	= hImgSelectCenter;
	param.hImgSelectRight	= hImgSelectRight;
	param.hImgDisableLeft	= hImgDisableLeft;
	param.hImgDisableCenter	= hImgDisableCenter;
	param.hImgDisableRight	= hImgDisableRight;
	param.lpUncheckedImage 	= lpUncheckedImage;
	param.lpCheckedImage	= lpCheckedImage;
	param.lpScrollbarImgTop	= lpScrollbarImgTop;
	param.lpScrollbarImage	= lpScrollbarImage;
	param.lpScrollbarImgBottom	= lpScrollbarImgBottom;
	//param.lpScrollcursorImage	= lpScrollcursorImage;
	param.lpScrollcursorImgTop	= lpScrollcursorImgTop;
	param.lpScrollcursorImgMid	= lpScrollcursorImgMid;
	param.lpScrollcursorImgBot	= lpScrollcursorImgBot;

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(atLISTBOX_EXT), ListBoxProc, &param );
//	if( hControl == atNULL ) return atNULL;

	return hControl;
}

static void atLISTBOX_OnInit( atHCONTROL hControl, struct _CreateParam *param )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atRECT rect;
	int nWidth, nHeight;
	atCONTROL_COLOR cl;

	atCONTROL_GetControlColor( hControl, &cl );
	cl.clNormalFrColor		= UI_COLOR_WHITE;
	cl.clNormalTxtColor	= UI_COLOR_BLACK;
	cl.clNormalBgColor		= UI_COLOR_WHITE;
	cl.clFocusedFrColor		= COLOR_SELECTBAR;
	cl.clFocusedTxtColor	= UI_COLOR_WHITE;
	cl.clFocusedBgColor	= COLOR_SELECTBAR;
	cl.clDisableFrColor		= UI_COLOR_GRAY;
	cl.clDisableTxtColor	= UI_COLOR_WHITE;
	cl.clDisableBgColor		= UI_COLOR_GRAY;
	atCONTROL_SetControlColor( hControl, &cl );

	lpListBox->dwAttr	= atLISTBOX_EXT_ATTR_TRANSPARENT | atLISTBOX_EXT_ATTR_MOVE_NORMAL;
		
	lpListBox->lpListBoxList = atUIAPI_CreateList();
	lpListBox->lpAddItemData = atNULL;
	lpListBox->nAddItemIndex = -1;

	lpListBox->nSelectIndex 		=	-1;
	lpListBox->nItemHeight			=	param->nItemHeight;
	lpListBox->nItemGap			=	param->nItemGap;
	lpListBox->nWidthMargin		=	param->nWidthMargin;
	lpListBox->nHeightMargin		= 	param->nHeightMargin;
	lpListBox->nWidthBorder		=	param->nWidthBorder;
	lpListBox->nHeightBorder		= 	param->nHeightBorder;

	lpListBox->bTickerEnable		=	param->bTicker;
	lpListBox->bFirstNO 			=	param->bFirstNO;
	lpListBox->nCheckType			=	param->nCheckType;

	atCONTROL_GetControlRect( hControl, &rect );
	nWidth = RECT_WIDTH(rect);
	nHeight = RECT_HEIGHT(rect);
	
//	lpListBox->nOneScreenCount	=	nHeight / param->nItemHeight;		

	lpListBox->lpUncheckedImage = param->lpUncheckedImage;
	lpListBox->lpCheckedImage	= param->lpCheckedImage;

	lpListBox->bTickerActivate = atFALSE;
	lpListBox->bActionValidate = atFALSE;

	lpListBox->hImgNormalLeft = param->hImgNormalLeft;
	lpListBox->hImgNormalCenter = param->hImgNormalCenter;
	lpListBox->hImgNormalRight = param->hImgNormalRight;
	lpListBox->hImgSelectLeft = param->hImgSelectLeft;
	lpListBox->hImgSelectCenter = param->hImgSelectCenter;
	lpListBox->hImgSelectRight = param->hImgSelectRight;
	lpListBox->hImgDisableLeft = param->hImgDisableLeft;
	lpListBox->hImgDisableCenter = param->hImgDisableCenter;
	lpListBox->hImgDisableRight = param->hImgDisableRight;

	lpListBox->SelectKey = atVKEY_SELECT;
	lpListBox->CheckKey = atVKEY_SOFT2;

	lpListBox->fnGetCount = atNULL;
	lpListBox->fnGetItem = atNULL;
	lpListBox->fnSetItem = atNULL;
		
//	lpListBox->hScrollBar = CreateScrollBar( hControl, 0, 0+nWidth-8, 0, 8, nHeight, 0, 0, 0, 0, atFALSE );
	lpListBox->hScrollBar = atSCROLLBAR_EXT_Create( 100, 0+nWidth-8, 0, 8, nHeight, 
												   atSCROLLBAR_EXT_TYPE_WINDOW /*atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION_STICK*/,
												   0, 0, 0, 0,
						  						   param->lpScrollbarImgTop, param->lpScrollbarImage, param->lpScrollbarImgBottom, 
						  						   param->lpScrollcursorImgTop, param->lpScrollcursorImgMid, param->lpScrollcursorImgBot );
	atCONTROL_SetAttr( lpListBox->hScrollBar, atCONTROL_GetAttr(lpListBox->hScrollBar) | CTL_PARENTTYPE_CONTROL );
	atCONTROL_SetAnchor( lpListBox->hScrollBar, 0, 0, atANCHOR_IGNORE, 0 );
	atCONTROL_GetControlColor( hControl, &cl );
	atCONTROL_SetControlColor( lpListBox->hScrollBar, &cl );
	atCONTROL_SetFocusable(lpListBox->hScrollBar, atFALSE);
	atCONTROL_SetVisible( lpListBox->hScrollBar, atFALSE );
	atCONTROL_AddChildControl( hControl, lpListBox->hScrollBar );

	if( param->bTicker ){
	    if( (param->nCheckType != 0) || param->bFirstNO || (param->nWidthMargin >= 4) )
		 lpListBox->hTickerBox = atTICKERBOX_Create( 0000, 0, 0, nWidth, param->nItemHeight, _AT(""), 0, TICKER_INTERVAL, 4, -20, 0, param->nWidthMargin, param->hImgSelectCenter, param->hImgSelectCenter, param->hImgSelectRight );
	    else 
		 lpListBox->hTickerBox = atTICKERBOX_Create( 0000, 0, 0, nWidth, param->nItemHeight, _AT(""), 0, TICKER_INTERVAL, 4, -20, 0, param->nWidthMargin, param->hImgSelectLeft, param->hImgSelectCenter, param->hImgSelectRight );
		atCONTROL_SetAttr( lpListBox->hTickerBox, (atCONTROL_GetAttr(lpListBox->hScrollBar) | CTL_PARENTTYPE_CONTROL) & ~atOBJATTR_EVENTTARGET);

		atCONTROL_GetControlColor( hControl, &cl );
		atCONTROL_SetControlColor( lpListBox->hTickerBox, &cl );
		atCONTROL_SetVisible(lpListBox->hTickerBox, atFALSE );
		atCONTROL_AddChildControl( hControl, lpListBox->hTickerBox );
	} else
		lpListBox->hTickerBox = atNULL;

	// 삭제함: 무조건 적인 소프트키 설정은 외부에서 소프트키를 바꿀 경우
	// 바꾸는 과정이 보여서 화면이 깜박이게 됨.
	// 밖에서만 설정하게 해야함.
	//atCONTROL_SetSoftKey( hControl, _AT(""), _AT("OK"), _AT("") );

	lpListBox->nViewTopY = 0;
	lpListBox->bMouseDown = atFALSE;
	lpListBox->bIsChange = atTRUE;

	// Kyle Start [Add 2009/07/23-15:04]
	//lpListBox->nScrollType = atLISTBOX_EXT_SCROLLTYPE_PANNING;		// 패닝(역방향) 스크롤 (기본)
	//lpListBox->nScrollType = atLISTBOX_EXT_SCROLLTYPE_SCROLLL;	// 일반(정방향) 스크롤 (옵션)
	lpListBox->bUseSmoothScroll = lpListBox->bMMLockFlag = lpListBox->bIsAcceleration = atFALSE;
	//lpListBox->bUseSmoothScroll = atTRUE; // Kyle Test Code
	lpListBox->fVelocity = lpListBox->nDraggedDistance = lpListBox->dwAccelSTick = 0;

	// 스무스 스크롤 랜더링용 Timer
	atCONTROL_AddControlTimer( hControl, 0, CHECK_ACCELTICK_TIME_LIMIT, atTRUE, atFALSE, atFALSE );
	// Kyle [Add 2009/07/23-15:04] End
}

static void atLISTBOX_OnDestroy( atHCONTROL hControl )
{	
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return;

	if( lpListBox->lpAddItemData ){
		UI_MEMFREE( lpListBox->lpAddItemData );
		lpListBox->lpAddItemData = atNULL;
	}

	atUIAPI_DestroyList(lpListBox->lpListBoxList, atTRUE);
	lpListBox->lpListBoxList = atNULL;

/*
	if( lpListBox->hTickerBox ){
		atCONTROL_Destroy( lpListBox->hTickerBox );
		lpListBox->hTickerBox = atNULL;
	}
	if( lpListBox->hScrollBar ){
		atCONTROL_Destroy( lpListBox->hScrollBar );
		lpListBox->hScrollBar = atNULL;
	}
*/
}

static void atLISTBOX_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr(hControl);
	int i, itm_cnt;	
	int x, y, w, h, hh;
	int ix, iy, iw, ih;
	atLISTBOX_EXT_ITEM Item;
	atLPCONTROL		lpControl2;
	atLISTBOX_EXT_DRAWITEM_BG_FUNC *draw_item_bg;
	atLISTBOX_EXT_DRAWITEM_FUNC *draw_item;
	atLISTBOX_EXT_GETITEMHEIGHT_FUNC *get_height;
	atRECT clip;
	atBOOL r;
	
	// 표시 좌표 보정
	x = 0;
	y = 0;
	w = RECT_WIDTH(lpControl->Base.rect);
	h = RECT_HEIGHT(lpControl->Base.rect);
	
	if ( atCONTROL_IsVisible(lpListBox->hScrollBar) ){
		lpControl2 = atCONTROL_GetPtr(lpListBox->hScrollBar);
		w -= RECT_WIDTH(lpControl2->Base.rect);
	}

//atUIAPI_FillRect( hCanvas, x, y, w, h, atUI_COLOR_RED );  // 배경을 지운다.

	if( lpListBox->dwAttr & atLISTBOX_EXT_ATTR_OPAQUE ){
		atUIAPI_FillRect( hCanvas, x, y, w, h, lpControl->tControlColor.clNormalBgColor );  // 배경을 지운다.
	}
	
	ix = x + lpListBox->nWidthMargin;
	iw = w - lpListBox->nWidthMargin * 2;
	
	// 아이템이 하나도 없으면 여기서 중지.
	itm_cnt = atLISTBOX_EXT_GetCount(hControl);
	if( itm_cnt <= 0 ) return;

//	nCurIndex = lpListBox->nViewTopY / lpListBox->nItemHeight;
//	y -= (lpListBox->nViewTopY % lpListBox->nItemHeight);

	draw_item_bg = (lpListBox->fnDrawItemBG)? lpListBox->fnDrawItemBG : _defDrawItemBG;
	draw_item = (lpListBox->fnDrawItem)? lpListBox->fnDrawItem : _defDrawItem;
	get_height = (lpListBox->fnGetItemHeight)? lpListBox->fnGetItemHeight : _defGetItemHeight;

	y -= lpListBox->nViewTopY;
	for( i = 0; y < h; i++ ){
		if(i < itm_cnt) r = atLISTBOX_EXT_GetItem(hControl, i, &Item); else r = atFALSE;
		hh = get_height( hControl, i, lpListBox->lpFuncParam );
		if( y + hh >= 0 ){
			if( r ){
				SET_RECT( clip, x, y, x+w-1, y+hh-1 );
				atUIAPI_SetClipRect( hCanvas, &clip );
				draw_item_bg( hCanvas, hControl, i, x, y, w, hh, &Item, (i == lpListBox->nSelectIndex), bFocused, lpListBox->lpFuncParam );
				iy = y + lpListBox->nHeightMargin;
				ih = hh - lpListBox->nHeightMargin * 2;
				iy = y;
				ih = hh;
				SET_RECT( clip, ix, iy, ix+iw-1, iy+ih-1 );
				draw_item( hCanvas, hControl, i, ix, iy, iw, ih, lpListBox->bFirstNO, lpListBox->nCheckType, &Item, (i == lpListBox->nSelectIndex), bFocused, lpListBox->lpFuncParam );
			}
		}
		y += hh;
		if( i < itm_cnt-1 ) y += lpListBox->nItemGap;
	}
}


static void _SetScrollPos( atHCONTROL hControl, int scroll_pos, atBOOL bChangeScrollBar )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr(hControl);
//	atREGION rgn;

	if( atCONTROL_IsVisible( lpListBox->hScrollBar ) ){
		if( scroll_pos < 0 ) scroll_pos = 0;
		if( scroll_pos >= atSCROLLBAR_EXT_GetSize(lpListBox->hScrollBar) ) scroll_pos = atSCROLLBAR_EXT_GetSize(lpListBox->hScrollBar) - 1;
		if( bChangeScrollBar ) atSCROLLBAR_EXT_SetPosition( lpListBox->hScrollBar, scroll_pos );	
		scroll_pos = atSCROLLBAR_EXT_GetPosition( lpListBox->hScrollBar );
		lpListBox->nViewTopY = scroll_pos;
        lpListBox->bIsChange = atTRUE;
		_Realloc_Items( hControl );
	} else {
		lpListBox->nViewTopY = 0;
	}
}

static void _Realloc_Items( atHCONTROL hControl )
{
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr(hControl);
	int i, hh;
	atLPTSTR szSelStr;
	atHBITMAP hBm;
	atRECT rect;
	int all_item_height;
	atCONTROL_COLOR cl;
int xxx, yyy;
int nHeight;
int cnt;
int show_sb = 0;
	atBOOL active_ticker;
	int itm_y, itm_height, view_y;
	atLISTBOX_EXT_GETITEMHEIGHT_FUNC *get_height;


	if( !lpListBox->bIsChange ) return;

	atCONTROL_GetControlRect( hControl, &rect );

	nHeight = RECT_HEIGHT(lpControl->Base.rect);


//	if(RECT_HEIGHT(lpControl->Base.rect) < atLISTBOX_EXT_GetCount(lpListBox) * lpListBox->nItemHeight) 
	{		
//		lpListBox->nOneScreenCount		=	(lpListBox->nItemHeight != 0)? nHeight / lpListBox->nItemHeight : 0;		
//		lpListBox->hScrollBar			=	atLISTBOX_EXT_GetCount(hControl)-lpListBox->nOneScreenCount;
//		lpListBox->ScrollBar.nCount		=	atLISTBOX_EXT_GetCount(hControl)-lpListBox->nOneScreenCount;
	}
//	else 
	{			
//		lpListBox->nOneScreenCount			=	atLISTBOX_EXT_GetCount(hControl);
	}	

	cnt = atLISTBOX_EXT_GetCount(hControl);
//	if( (cnt <= 0) || (lpListBox->nOneScreenCount <= 0) ){
	if( cnt <= 0 ){
		if( lpListBox->hScrollBar != atNULL )
			atCONTROL_SetVisible( lpListBox->hScrollBar, atFALSE );
		if( lpListBox->hTickerBox != atNULL ){
			atCONTROL_SetVisible( lpListBox->hTickerBox, atFALSE );
		}
		lpListBox->bTickerActivate = atFALSE;
		return;
	}

//	if( cnt > lpListBox->nOneScreenCount ){
//		atRECT r;
//		atCONTROL_GetControlRect( lpListBox->hScrollBar, &r );
//		atCONTROL_SetControlRegion( lpListBox->hScrollBar, RECT_WIDTH(rect)-RECT_WIDTH(r), 0, RECT_WIDTH(r), RECT_HEIGHT(rect) );

	get_height = (lpListBox->fnGetItemHeight)? lpListBox->fnGetItemHeight : _defGetItemHeight;
	all_item_height = lpListBox->nHeightMargin;
	for( i = 0; i < cnt; i++ ){
		hh = get_height( hControl, i, lpListBox->lpFuncParam );
		if( i == lpListBox->nSelectIndex ){
			itm_y = all_item_height;
			itm_height = hh;
		}
		all_item_height += hh;
		if( i < cnt-1 ) all_item_height += lpListBox->nItemGap;
	}
	all_item_height += lpListBox->nHeightMargin;
	
	if( all_item_height > nHeight ){
		if( !atCONTROL_IsVisible(lpListBox->hScrollBar) ){
			atCONTROL_SetVisible( lpListBox->hScrollBar, atTRUE );
		}
//		atSCROLLBAR_EXT_SetSize( lpListBox->hScrollBar, cnt );
//atSCROLLBAR_EXT_SetSize( lpListBox->hScrollBar, cnt * lpListBox->nItemHeight - nHeight );
atSCROLLBAR_EXT_SetSize( lpListBox->hScrollBar, all_item_height );
show_sb = 1;
	} else {
		if( lpListBox->hScrollBar != atNULL /*atCONTROL_IsVisible(lpListBox->hScrollBar)*/ ){
			atCONTROL_SetVisible( lpListBox->hScrollBar, atFALSE );
		}
show_sb = 0;
_SetScrollPos( hControl, 0, atTRUE );
	}

	if( lpListBox->bTickerEnable /*&& atCONTROL_IsFocus(hControl)*/ ){
		int ChkWidth, BmWidth, SbWidth, NoWidth;
		atTCHAR szBuff[16];
		
		if( (lpListBox->nCheckType == 1) || (lpListBox->nCheckType == 2) ){
			atHBITMAP lpBM = lpListBox->lpUncheckedImage;
			ChkWidth = (lpBM)? atUIAPI_GetBitmapWidth(lpBM) : 16;
			if( ChkWidth > 0 ) ChkWidth += lpListBox->nItemGap;
		} else {
			ChkWidth = 0;
		}
//		if( atCONTROL_IsVisible(lpListBox->hScrollBar) ){
		if( show_sb ){
			atRECT rect;
			atCONTROL_GetControlRect( lpListBox->hScrollBar, &rect);
			SbWidth = RECT_WIDTH(rect);
		} else {
			SbWidth = 0;
		}

		if( lpListBox->nSelectIndex >= 0 ){
			atLISTBOX_EXT_ITEM itm;	
			atBOOL r = atLISTBOX_EXT_GetSelectItem(hControl, &itm);
			
			if( lpListBox->bFirstNO ){
				UI_SPRINTF( szBuff, _AT("%d."), itm.nNO );
				NoWidth = atUIAPI_GetStringWidth(atCONTROL_GetFont(hControl), szBuff, -1);
				if( NoWidth > 0 ) NoWidth += lpListBox->nItemGap;
			} else {
				NoWidth = 0;
			}

			szSelStr = itm.szItemName[1];
			if( szSelStr && UI_STRLEN(szSelStr) <= 0 ) szSelStr = itm.szItemName[0];
			hBm = itm.imgItemImage[1];
			if( hBm ) BmWidth = (itm.imgItemImageRegion[1].nWidth>0)? itm.imgItemImageRegion[1].nWidth : atUIAPI_GetBitmapWidth(hBm);
			else {
				hBm = itm.imgItemImage[0];
				if( hBm ) BmWidth = (itm.imgItemImageRegion[0].nWidth>0)? itm.imgItemImageRegion[0].nWidth : atUIAPI_GetBitmapWidth(hBm);
				else BmWidth = 0;
			}
			if( BmWidth > 0 ) BmWidth += lpListBox->nItemGap;

			active_ticker = atFALSE;
			xxx = RECT_WIDTH(lpControl->Base.rect) - ChkWidth - BmWidth - SbWidth - NoWidth - lpListBox->nWidthMargin*2;
			yyy = atUIAPI_GetStringWidth(atCONTROL_GetFont(hControl), szSelStr, -1);
			if( yyy >= xxx ){
				view_y = itm_y - lpListBox->nViewTopY;
				if( (view_y + itm_height >= 0) && (view_y < nHeight) ){
					active_ticker = atTRUE;
				}
			}
			if( active_ticker ){
				if( atUIAPI_strcmp( atTICKERBOX_GetText(lpListBox->hTickerBox), szSelStr ) != 0 ){
					atTICKERBOX_SetText( lpListBox->hTickerBox, szSelStr, -1 );
//						atTICKERBOX_ResetTick( lpListBox->hTickerBox );
				}

				atCONTROL_SetSize( lpListBox->hTickerBox, 100, itm_height );
				//atCONTROL_SetAnchor( lpListBox->hTickerBox, view_y, atANCHOR_IGNORE, lpListBox->nWidthMargin + ChkWidth + NoWidth + BmWidth, lpListBox->nWidthMargin + SbWidth/* + lpListBox->nWidthMargin*/);
				atCONTROL_SetAnchor( lpListBox->hTickerBox, view_y-4, atANCHOR_IGNORE, lpListBox->nWidthMargin + ChkWidth + NoWidth + BmWidth + 4, lpListBox->nWidthMargin + SbWidth/* + lpListBox->nWidthMargin*/);
				if( !lpListBox->bTickerActivate ){
					atCONTROL_SetFont( lpListBox->hTickerBox, atCONTROL_GetFont(hControl) );
					atCONTROL_GetControlColor( hControl, &cl );
					cl.clNormalTxtColor = cl.clFocusedTxtColor;
					cl.clNormalBgColor = cl.clFocusedBgColor;
					atCONTROL_SetControlColor( lpListBox->hTickerBox, &cl );
					if( !atCONTROL_IsVisible(lpListBox->hTickerBox) )
						atCONTROL_SetVisible( lpListBox->hTickerBox, atTRUE );
					//if( atCONTROL_IsFocus(hControl) )
						atTICKERBOX_StartTicker( lpListBox->hTickerBox );

					lpListBox->bTickerActivate = atTRUE;
				}
			} else {
				atCONTROL_SetVisible(lpListBox->hTickerBox, atFALSE );
				lpListBox->bTickerActivate = atFALSE;
			}
		} else {
			atCONTROL_SetVisible(lpListBox->hTickerBox, atFALSE );
			lpListBox->bTickerActivate = atFALSE;
		}
	}

	atCONTROL_AdjustAllChildControls(hControl);

	lpListBox->bIsChange = atFALSE;
}

// Kyle Start [Add 2009/07/23-15:14]
static void atLISTBOX_EXT_StartAccelTimer(atHCONTROL hControl)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return;
	if( !lpListBox->bUseSmoothScroll ) return;

	if( atCONTROL_IsVisible(hControl) ) {
		atCONTROL_StartControlTimer( hControl, 0 );
	}
	
	lpListBox->bIsAcceleration = atTRUE;	
}

static void atLISTBOX_EXT_StopAccelTimer(atHCONTROL hControl )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return;
	if( !lpListBox->bUseSmoothScroll ) return;

	atCONTROL_StopControlTimer( hControl, 0 );
	lpListBox->bIsAcceleration = atFALSE;	
}

static int atLISTBOX_EXT_OnKeyDown( atHCONTROL hControl, atINT nKeyCode )
{
	int r = 0;
	atLISTBOX_EXT_ITEM itm;
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return 0;
					
	switch( nKeyCode ){
		case atVKEY_UP:
		case atVKEY_REW:
			if( lpListBox->nSelectIndex <= 0 ){
				if( lpListBox->dwAttr & atLISTBOX_EXT_ATTR_MOVE_NEXTCONTROL ){
					return 0;
				}
			}
			return 1;
			break;
		case atVKEY_DOWN:
		case atVKEY_FF:
			if( lpListBox->nSelectIndex >= atLISTBOX_EXT_GetCount(hControl)-1 ){
				if( lpListBox->dwAttr & atLISTBOX_EXT_ATTR_MOVE_NEXTCONTROL ){
					return 0;
				}
			}
			return 1;
			break;
	}

	if( nKeyCode == lpListBox->SelectKey ){
		if( lpListBox->nSelectIndex >= 0 ){
			if( lpListBox->nCheckType == 2 ){
				if( atLISTBOX_EXT_GetSelectItem(hControl, &itm) ){
					atLISTBOX_EXT_SetItemCheck(hControl, atLISTBOX_EXT_GetSelectIndex(hControl), atTRUE);
					if( lpListBox->CheckKey == atVKEY_SOFT1 )
						atCONTROL_SetSoftKey(hControl, _AT(""), atNULL, atNULL);
					else if( lpListBox->CheckKey == atVKEY_SOFT2 )
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
					atSOFTKEY_RefreshSoftKey();
				}
				atCONTROL_Refresh( hControl );
			}
			if( atLISTBOX_EXT_GetSelectItem(hControl, &itm) ){
				if( itm.nItemID != -1 ){
					atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)lpControl->Base.nID, (long)itm.nItemID );
				} else {
					atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)lpControl->Base.nID, (long)lpListBox->nSelectIndex );
				}
			}
			r = 1;
		}
	} else if( nKeyCode == lpListBox->CheckKey ){
		if( lpListBox->nCheckType == 1 ){
			if( atLISTBOX_EXT_GetSelectItem(hControl, &itm) ){
				atBOOL bChecked = atLISTBOX_EXT_ToggleSelectItemChecked(hControl);
				if( bChecked ){
					if( lpListBox->CheckKey == atVKEY_SOFT1 )
						atCONTROL_SetSoftKey(hControl, _AT("UnChk"), atNULL, atNULL);
					else if( lpListBox->CheckKey == atVKEY_SOFT2 )
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("UnChk"));
					else if( lpListBox->CheckKey == atVKEY_SELECT )
						atCONTROL_SetSoftKey(hControl, atNULL, _AT("UnChk"), atNULL);
				} else {
					if( lpListBox->CheckKey == atVKEY_SOFT1 )
						atCONTROL_SetSoftKey(hControl, _AT("Check"), atNULL, atNULL);
					else if( lpListBox->CheckKey == atVKEY_SOFT2 )
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
					else if( lpListBox->CheckKey == atVKEY_SELECT )
						atCONTROL_SetSoftKey(hControl, atNULL, _AT("Check"), atNULL);
				}
				atCONTROL_Refresh( hControl );
				atSOFTKEY_RefreshSoftKey();
				r = 1;
			}
		} else if( lpListBox->nCheckType == 2 ){
			if( atLISTBOX_EXT_GetSelectItem(hControl, &itm) ){
				atLISTBOX_EXT_SetItemCheck(hControl, atLISTBOX_EXT_GetSelectIndex(hControl), atTRUE);
				if( lpListBox->CheckKey == atVKEY_SOFT1 )
					atCONTROL_SetSoftKey(hControl, _AT(""), atNULL, atNULL);
				else if( lpListBox->CheckKey == atVKEY_SOFT2 )
					atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
				else if( lpListBox->CheckKey == atVKEY_SELECT )
					atCONTROL_SetSoftKey(hControl, atNULL, _AT(""), atNULL);
//						atCONTROL_Refresh( hControl );
				atSOFTKEY_RefreshSoftKey();
				r = 1;
			}
		}
	}

	return r;
}

static int atLISTBOX_EXT_OnKeyUp( atHCONTROL hControl, atINT nKeyCode )
{
	int r = 0;
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return 0;
/*					
	switch( nKeyCode ){
		case atVKEY_UP:
		case atVKEY_REW:
			if( lpListBox->nSelectIndex <= 0 ){
				if( lpListBox->dwAttr & atLISTBOX_EXT_ATTR_MOVE_NEXTCONTROL ){
					return 0;
				}
			}
			return 1;
			break;
		case atVKEY_DOWN:
		case atVKEY_FF:
			if( lpListBox->nSelectIndex >= atLISTBOX_EXT_GetCount(hControl)-1 ){
				if( lpListBox->dwAttr & atLISTBOX_EXT_ATTR_MOVE_NEXTCONTROL ){
					return 0;
				}
			}
			return 1;
			break;
	}

	if( nKeyCode == lpListBox->SelectKey ){
		if( lpListBox->nSelectIndex >= 0 ){
			if( lpListBox->nCheckType == 2 ){
				lpItem = atLISTBOX_EXT_GetSelectItem(hControl);
				if( lpItem ){
					atLISTBOX_EXT_SetItemCheck(hControl, atLISTBOX_EXT_GetSelectIndex(hControl), atTRUE);
//							atCONTROL_Draw(hControl, atTRUE);
					if( lpListBox->CheckKey == atVKEY_SOFT1 )
						atCONTROL_SetSoftKey(hControl, _AT(""), atNULL, atNULL);
					else if( lpListBox->CheckKey == atVKEY_SOFT2 )
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
					atSOFTKEY_RefreshSoftKey();
				}
				atCONTROL_Refresh( hControl );
			}
//					if( lpListBox->bFirstNO ){
			lpItem = atLISTBOX_EXT_GetSelectItem(hControl);
			if( lpItem->nItemID != -1 ){
				atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)lpControl->Base.nID, (long)lpItem->nItemID );
			} else {
				atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)lpControl->Base.nID, (long)lpListBox->nSelectIndex );
			}
			r = 1;
		}
	} else if( nKeyCode == lpListBox->CheckKey ){
		if( lpListBox->nCheckType == 1 ){
			lpItem = atLISTBOX_EXT_GetSelectItem(hControl);
			if( lpItem ){
				atBOOL bChecked = atLISTBOX_EXT_ToggleSelectItemChecked(hControl);
//						atCONTROL_Draw(hControl, atTRUE);
				if( bChecked ){
					if( lpListBox->CheckKey == atVKEY_SOFT1 )
						atCONTROL_SetSoftKey(hControl, _AT("UnChk"), atNULL, atNULL);
					else if( lpListBox->CheckKey == atVKEY_SOFT2 )
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("UnChk"));
					else if( lpListBox->CheckKey == atVKEY_SELECT )
						atCONTROL_SetSoftKey(hControl, atNULL, _AT("UnChk"), atNULL);
				} else {
					if( lpListBox->CheckKey == atVKEY_SOFT1 )
						atCONTROL_SetSoftKey(hControl, _AT("Check"), atNULL, atNULL);
					else if( lpListBox->CheckKey == atVKEY_SOFT2 )
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
					else if( lpListBox->CheckKey == atVKEY_SELECT )
						atCONTROL_SetSoftKey(hControl, atNULL, _AT("Check"), atNULL);
				}
				atCONTROL_Refresh( hControl );
				atSOFTKEY_RefreshSoftKey();
				r = 1;
			}
		} else if( lpListBox->nCheckType == 2 ){
			lpItem = atLISTBOX_EXT_GetSelectItem(hControl);
			if( lpItem ){
				atLISTBOX_EXT_SetItemCheck(hControl, atLISTBOX_EXT_GetSelectIndex(hControl), atTRUE);
//						atCONTROL_Draw(hControl, atTRUE);
				if( lpListBox->CheckKey == atVKEY_SOFT1 )
					atCONTROL_SetSoftKey(hControl, _AT(""), atNULL, atNULL);
				else if( lpListBox->CheckKey == atVKEY_SOFT2 )
					atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
				else if( lpListBox->CheckKey == atVKEY_SELECT )
					atCONTROL_SetSoftKey(hControl, atNULL, _AT(""), atNULL);
//						atCONTROL_Refresh( hControl );
				atSOFTKEY_RefreshSoftKey();
				r = 1;
			}
		}
	}
*/
	return r;
}

static int atLISTBOX_EXT_OnKeyPress( atHCONTROL hControl, atINT nKeyCode )
{
	int r = 0;
	atLISTBOX_EXT_ITEM itm;
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return 0;
	
	switch( nKeyCode ){
		case atVKEY_UP:
		case atVKEY_REW:
			atLISTBOX_EXT_MoveSelect(hControl, -1);
			if( lpListBox->nCheckType == 1 ){
				if( atLISTBOX_EXT_GetSelectItem(hControl, &itm) ){
					if( itm.bChecked ){
						if( lpListBox->CheckKey == atVKEY_SOFT1 )
							atCONTROL_SetSoftKey(hControl, _AT("UnChk"), atNULL, atNULL);
						else if( lpListBox->CheckKey == atVKEY_SOFT2 )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("UnChk"));
						else if( lpListBox->CheckKey == atVKEY_SELECT )
							atCONTROL_SetSoftKey(hControl, atNULL, _AT("UnChk"), atNULL);
					} else {
						if( lpListBox->CheckKey == atVKEY_SOFT1 )
							atCONTROL_SetSoftKey(hControl, _AT("Check"), atNULL, atNULL);
						else if( lpListBox->CheckKey == atVKEY_SOFT2 )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
						else if( lpListBox->CheckKey == atVKEY_SELECT )
							atCONTROL_SetSoftKey(hControl, atNULL, _AT("Check"), atNULL);
					}
					atSOFTKEY_RefreshSoftKey();
				}
			} else if( lpListBox->nCheckType == 2 ){
				if( atLISTBOX_EXT_GetSelectItem(hControl, &itm) ){
					if( itm.bChecked ){
						if( lpListBox->CheckKey == atVKEY_SOFT1 )
							atCONTROL_SetSoftKey(hControl, _AT(""), atNULL, atNULL);
						else if( lpListBox->CheckKey == atVKEY_SOFT2 )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
						else if( lpListBox->CheckKey == atVKEY_SELECT )
							atCONTROL_SetSoftKey(hControl, atNULL, _AT(""), atNULL);
					} else {
						if( lpListBox->CheckKey == atVKEY_SOFT1 )
							atCONTROL_SetSoftKey(hControl, _AT("Select"), atNULL, atNULL);
						else if( lpListBox->CheckKey == atVKEY_SOFT2 )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Select"));
						else if( lpListBox->CheckKey == atVKEY_SELECT )
							atCONTROL_SetSoftKey(hControl, atNULL, _AT("Select"), atNULL);
					}
					atSOFTKEY_RefreshSoftKey();
				}
			}
			atCONTROL_Refresh( hControl );
			r = 1;
			break;
		case atVKEY_DOWN:
		case atVKEY_FF:
			atLISTBOX_EXT_MoveSelect(hControl, 1);
			if( lpListBox->nCheckType == 1 ){
				if( atLISTBOX_EXT_GetSelectItem(hControl, &itm) ){
					if( itm.bChecked ){
						if( lpListBox->CheckKey == atVKEY_SOFT1 )
							atCONTROL_SetSoftKey(hControl, _AT("UnChk"), atNULL, atNULL);
						else if( lpListBox->CheckKey == atVKEY_SOFT2 )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("UnChk"));
						else if( lpListBox->CheckKey == atVKEY_SELECT )
							atCONTROL_SetSoftKey(hControl, atNULL, _AT("UnChk"), atNULL);
					} else {
						if( lpListBox->CheckKey == atVKEY_SOFT1 )
							atCONTROL_SetSoftKey(hControl, _AT("Check"), atNULL, atNULL);
						else if( lpListBox->CheckKey == atVKEY_SOFT2 )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
						else if( lpListBox->CheckKey == atVKEY_SELECT )
							atCONTROL_SetSoftKey(hControl, atNULL, _AT("Check"), atNULL);
					}
					atSOFTKEY_RefreshSoftKey();
				}
			} else if( lpListBox->nCheckType == 2 ){
				if( atLISTBOX_EXT_GetSelectItem(hControl, &itm) ){
					if( itm.bChecked ){
						if( lpListBox->CheckKey == atVKEY_SOFT1 )
							atCONTROL_SetSoftKey(hControl, _AT(""), atNULL, atNULL);
						else if( lpListBox->CheckKey == atVKEY_SOFT2 )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
						else if( lpListBox->CheckKey == atVKEY_SELECT )
							atCONTROL_SetSoftKey(hControl, atNULL, _AT(""), atNULL);
					} else {
						if( lpListBox->CheckKey == atVKEY_SOFT1 )
							atCONTROL_SetSoftKey(hControl, _AT("Select"), atNULL, atNULL);
						else if( lpListBox->CheckKey == atVKEY_SOFT2 )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Select"));
						else if( lpListBox->CheckKey == atVKEY_SELECT )
							atCONTROL_SetSoftKey(hControl, atNULL, _AT("Select"), atNULL);
					}
					atSOFTKEY_RefreshSoftKey();
				}
			}
			atCONTROL_Refresh( hControl );
			r = 1;
			break;
	}

	return r;
}

static int atLISTBOX_EXT_OnMouseDown( atHCONTROL hControl, atINT nX, atINT nY )
{
	int r = 1;
	int i, itm_cnt;	
	int y, h, hh;
	int nFirstIdx;
	atLISTBOX_EXT_GETITEMHEIGHT_FUNC *get_height;
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return 0;
	
	itm_cnt = atLISTBOX_EXT_GetCount(hControl);
	if( itm_cnt <= 0 ) return 0;

	// Kyle Start [Add 2009/06/22-14:25]
	if( atCONTROL_IsVisible(lpListBox->hScrollBar) ){
		atRECT rect;
		atCONTROL_GetControlRect(lpListBox->hScrollBar, &rect);
		if( nX >= rect.nStartX && nX <= rect.nEndX && nY >= rect.nStartY && nY <= rect.nEndY ) {
			return 0;
		}
	}
	// Kyle [Add 2009/06/22-14:25] End

//			nFirstIdx = lpListBox->nSelectIndex / lpListBox->nOneScreenCount * lpListBox->nOneScreenCount;
//			lpListBox->bActionValidate = atLISTBOX_EXT_SetSelectIndex( hControl, nFirstIdx + y / lpListBox->nItemHeight );
//	nFirstIdx = (nY + lpListBox->nViewTopY)  / lpListBox->nItemHeight;
	
	get_height = (lpListBox->fnGetItemHeight)? lpListBox->fnGetItemHeight : _defGetItemHeight;

	y = -lpListBox->nViewTopY - lpListBox->nHeightMargin;
	nFirstIdx = 0;
	h = RECT_HEIGHT(lpControl->Base.rect);
	for( i = 0; y < h; i++ ){
		hh = get_height( hControl, i, lpListBox->lpFuncParam );
		if( y + hh >= nY ){
 			nFirstIdx	= i;
			break;
		}
		y += hh;
		if( i < itm_cnt-1 ) y += lpListBox->nItemGap;
	}
	
	lpListBox->bActionValidate = atLISTBOX_EXT_SetSelectIndex( hControl, nFirstIdx );
	if( lpListBox->bActionValidate ){
		atCONTROL_Refresh( hControl );
	}else {
//		lpListBox->nSelectIndex = -1;
	}
	lpListBox->nFirstMouseX = lpListBox->nPrevMouseX = nX;
	lpListBox->nFirstMouseY = lpListBox->nPrevMouseY = nY;
	lpListBox->nFirstViewTopY = lpListBox->nViewTopY;
	lpListBox->bMouseDown = atTRUE;
	lpListBox->bMouseScroll = atFALSE;

	if( lpListBox->bUseSmoothScroll ) {
		atLISTBOX_EXT_StopAccelTimer( hControl );
		lpListBox->nDraggedDistance = lpListBox->dwAccelSTick = 0;
		lpListBox->bMMLockFlag = atFALSE;	// 스무스 스크롤 드래그 초기화 UnLock
	}

	return r;
}

static int atLISTBOX_EXT_OnMouseMove( atHCONTROL hControl, atINT nX, atINT nY )
{
	int r = 1;
	static int limit_y = -1;
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return 0;

	if( limit_y < 0 ){
		atUIAPI_GetScreenDPI( atNULL, &limit_y );
		limit_y = (int)(limit_y * CHECK_POINT_MOVE_LIMIT);
	}
		
	// Kyle Start [Add 2009/06/22-14:25]
	if( lpListBox->bMouseDown ){
		if( !lpListBox->bMouseScroll ){
			if( ABS(lpListBox->nFirstMouseY - nY) > limit_y ){
				lpListBox->bMouseScroll = atTRUE;
				lpListBox->bActionValidate = atFALSE;
			}
		}

		if( lpListBox->bMouseScroll ){
			if( lpListBox->nPrevMouseY != nY ) {
				_SetScrollPos( hControl, lpListBox->nFirstViewTopY + (lpListBox->nFirstMouseY - nY), atTRUE );
				atCONTROL_Refresh( hControl );
			}

			// Kyle Start [Add 2009/07/23-15:19]
			if( lpListBox->bUseSmoothScroll ) {
				if(lpListBox->dwAccelSTick > 0) {
					if(atUIAPI_GetCurrTime() - lpListBox->dwAccelSTick >= CHECK_ACCELMOVE_TIME_LIMIT)
						lpListBox->bMMLockFlag = atFALSE;	// 스무스 스크롤 드래그 초기화 UnLock
				}

				if(!lpListBox->bMMLockFlag) {
					lpListBox->dwAccelSTick = atUIAPI_GetCurrTime();	// 스무스 스크롤 시작 시간 초기화
					lpListBox->nDraggedDistance = 0;					// 드래그 거리 초기화
					lpListBox->bMMLockFlag = atTRUE;					// 스무스 스크롤 드래그 초기화 Lock
				} else {
					lpListBox->nDraggedDistance += lpListBox->nPrevMouseY - nY;	// 드래그 거리 가중치 증가
				}
			}
			// Kyle [Add 2009/07/23-15:19] End
		}

		lpListBox->nPrevMouseX = nX;
		lpListBox->nPrevMouseY = nY;
	} else {
		if( atCONTROL_IsVisible(lpListBox->hScrollBar) ) {
			atRECT rect;
			atCONTROL_GetControlRect(lpListBox->hScrollBar, &rect);
			//if( x >= rect.nStartX && x <= rect.nEndX && y >= rect.nStartY && y <= rect.nEndY ){
			if( nY >= rect.nStartY && nY <= rect.nEndY ) {
				return 0;
			}
		}
	}
	// Kyle [Add 2009/06/22-14:25] End

	return r;
}

static int atLISTBOX_EXT_OnMouseUp( atHCONTROL hControl, atINT nX, atINT nY )
{
	int r = 1;
	atRECT rect;
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return 0;

	atCONTROL_GetControlRect(lpListBox->hScrollBar, &rect);
	atCONTROL_ProcessControlProc( lpListBox->hScrollBar, CTLMSG_MOUSEUP, 0, ((nX-rect.nStartX) << 16) | nY );
	// Kyle [Add 2009/06/22-14:25] End

	// Kyle Start [Add 2009/07/23-15:27]
	if( lpListBox->bMouseScroll) {
		if( lpListBox->bUseSmoothScroll && lpListBox->bMMLockFlag) {
			if(atUIAPI_GetCurrTime() - lpListBox->dwAccelSTick >= CHECK_ACCELMOVE_TIME_LIMIT) {
				lpListBox->nDraggedDistance = 0;							// 드래그 거리 초기화
			} else {
				lpListBox->nDraggedDistance += lpListBox->nPrevMouseY - nY;	// 드래그 거리 가중치 증가
			}

			// 속도(속력) 구하기
			lpListBox->fVelocity = MIN(MAX(
										   REALMUL(ACCEL_VALUE_DRAGDISTANCE_FACTOR, INTTOREAL(lpListBox->nDraggedDistance)),
										   -ACCEL_VALUE_MAX_VELOCITY
										  ), 
									   ACCEL_VALUE_MAX_VELOCITY
									  );
									
			if(lpListBox->fVelocity)	atLISTBOX_EXT_StartAccelTimer( hControl );
			else						lpListBox->bMMLockFlag = atFALSE;	// 스무스 스크롤 드래그 초기화 UnLock
		}
	}
	// Kyle [Add 2009/07/23-15:27] End

	if( lpListBox->bActionValidate ){
		// Kyle Start [Delete 2009/06/22-14:25]
		//x = lParam2 >> 16;
		//y = (short)(lParam2 & 0xffff);
		// Kyle [Delete 2009/06/22-14:25] End
		lpListBox->bActionValidate = atFALSE;
		atCONTROL_ProcessControlProc( hControl, CTLMSG_KEYDOWN, atVKEY_SELECT, 0 );
	}
	lpListBox->bMouseDown = atFALSE;

	return r;
}

static int atLISTBOX_EXT_OnGotFocus( atHCONTROL hControl )
{
	int r = 0;
	atLISTBOX_EXT_ITEM itm;
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return 0;
	
	if( lpListBox->bTickerActivate ){
	    atCONTROL_SetVisible( lpListBox->hTickerBox, atTRUE );
	    atTICKERBOX_StartTicker( lpListBox->hTickerBox );
	}
	if( lpListBox->nCheckType == 1 ){
		if( atLISTBOX_EXT_GetSelectItem(hControl, &itm) ){
			if( itm.bChecked ){
				if( lpListBox->CheckKey == atVKEY_SOFT1 )
					atCONTROL_SetSoftKey(hControl, _AT("UnChk"), atNULL, atNULL);
				else if( lpListBox->CheckKey == atVKEY_SOFT2 )
					atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("UnChk"));
				else if( lpListBox->CheckKey == atVKEY_SELECT )
					atCONTROL_SetSoftKey(hControl, atNULL, _AT("UnChk"), atNULL);
			} else {
				if( lpListBox->CheckKey == atVKEY_SOFT1 )
					atCONTROL_SetSoftKey(hControl, _AT("Check"), atNULL, atNULL);
				else if( lpListBox->CheckKey == atVKEY_SOFT2 )
					atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
				else if( lpListBox->CheckKey == atVKEY_SELECT )
					atCONTROL_SetSoftKey(hControl, atNULL, _AT("Check"), atNULL);
			}
			atSOFTKEY_RefreshSoftKey();
			r = 1;
		}
	} else if( lpListBox->nCheckType == 2 ){
		if( atLISTBOX_EXT_GetSelectItem(hControl, &itm) ){
			if( itm.bChecked ){
				if( lpListBox->CheckKey == atVKEY_SOFT1 )
					atCONTROL_SetSoftKey(hControl, _AT(""), atNULL, atNULL);
				else if( lpListBox->CheckKey == atVKEY_SOFT2 )
					atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
				else if( lpListBox->CheckKey == atVKEY_SELECT )
					atCONTROL_SetSoftKey(hControl, atNULL, _AT(""), atNULL);
			} else {
				if( lpListBox->CheckKey == atVKEY_SOFT1 )
					atCONTROL_SetSoftKey(hControl, _AT("Select"), atNULL, atNULL);
				else if( lpListBox->CheckKey == atVKEY_SOFT2 )
					atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Select"));
				else if( lpListBox->CheckKey == atVKEY_SELECT )
					atCONTROL_SetSoftKey(hControl, atNULL, _AT("Select"), atNULL);
			}
			atSOFTKEY_RefreshSoftKey();
			r = 1;
		}
	}

	return r;
}

static int atLISTBOX_EXT_OnLostFocus( atHCONTROL hControl )
{
	int r = 0;
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return 0;
	
	if( lpListBox->bTickerActivate ){
	    atTICKERBOX_StopTicker( lpListBox->hTickerBox );
	    atCONTROL_SetVisible( lpListBox->hTickerBox, atFALSE );
	}
	if( lpListBox->nCheckType == 1 ){
//			RestoreSoftKey( lpControl->Base.hParentObj );
		r = 1;
	}

	return r;
}

static void atLISTBOX_EXT_OnTimer( atHCONTROL hControl, atLONG id )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atREAL fElapsedTime, fDeltaDistance, fTempVelocity;
	atINT nDeltaDistance;
	atDWORD dwInterval;

	if( lpListBox == atNULL ) return;
	if( !lpListBox->bUseSmoothScroll ) return;
	if( !lpListBox->bIsAcceleration ) return;
	
	lpListBox->bMMLockFlag = atFALSE;	// 스무스 스크롤 드래그 초기화 UnLock
	if( lpListBox->bMouseDown ) return;

	fTempVelocity = REALMUL(lpListBox->fVelocity, ACCEL_VALUE_DEACCEL_FACTOR);
	lpListBox->fVelocity = fTempVelocity;
	
	dwInterval = atUIAPI_GetCurrTime() - lpListBox->dwAccelSTick;
	if(dwInterval > CHECK_ACCELMOVE_TIME_LIMIT) dwInterval = CHECK_ACCELMOVE_TIME_LIMIT;

	fElapsedTime = REALDIV(INTTOFIXED(dwInterval), INTTOFIXED(1000));
	fDeltaDistance = REALMUL(fElapsedTime, lpListBox->fVelocity);
	nDeltaDistance = REALTOINT(fDeltaDistance);

	// 고정수소점을 사용하다보니 수소점 6자리 이후가 계산이 안되어 1 ~ -1까지 초기화 범위를 사용하고 있음.
	if(nDeltaDistance > 1 || nDeltaDistance < -1){
		_SetScrollPos( hControl, atSCROLLBAR_EXT_GetPosition(lpListBox->hScrollBar) + nDeltaDistance, atTRUE );	
		atCONTROL_Refresh( hControl );
	} else {
		atLISTBOX_EXT_StopAccelTimer( hControl );
	}
}
// Kyle [Add 2009/07/23-15:14] End

static int ListBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX_EXT lpListBox	= (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	int x, y;
	atCONTROL_COLOR cl;
	int r = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			atLISTBOX_OnInit( hControl, (struct _CreateParam *)lParam1 );
			r = 1;
			break;
		case CTLMSG_RELEASE:
			atLISTBOX_OnDestroy( hControl );
			r = 1;
			break;
		case CTLMSG_CHANGE:
			if( lParam1 == 100 ) {	// CMP_ScrollBarEx
				// Kyle Start [Modify 2009/06/22-14:25]
				_SetScrollPos( hControl, lParam2, atFALSE );
				// Kyle [Modify 2009/06/22-14:25] End
				atCONTROL_Refresh( hControl );
				r = 1;
			}
			break;
		// Kyle Start [Add 2009/07/23-15:14]
		case CTLMSG_HIDE:
		case CTLMSG_SUSPEND:
			if( lpListBox->bIsAcceleration )
				atCONTROL_StopControlTimer( hControl, 0 );

			r = 0;
			break;
		// Kyle [Add 2009/07/23-15:14] End
		case CTLMSG_SHOW:
		case CTLMSG_RESIZE:
			_SetScrollBarPosition( hControl, lpListBox );
			lpListBox->bIsChange = atTRUE;
			_Realloc_Items(hControl);

			// Kyle Start [Add 2009/07/23-15:14]
			if( lpListBox->bIsAcceleration )
				atCONTROL_StartControlTimer( hControl, 0 );
			// Kyle [Add 2009/07/23-15:14] End

			r = 0;
			break;
		case CTLMSG_PAINT:
			atLISTBOX_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
			
		case CTLMSG_GOTFOCUS:
			r = atLISTBOX_EXT_OnGotFocus( hControl );
			break;
			
		case CTLMSG_LOSTFOCUS:
			r = atLISTBOX_EXT_OnLostFocus( hControl );
			break;
			
		case CTLMSG_KEYDOWN:
			r = atLISTBOX_EXT_OnKeyDown( hControl, lParam1 );
			break;
			
		case CTLMSG_KEYUP:
			r = atLISTBOX_EXT_OnKeyUp( hControl, lParam1 );
			break;
			
		case CTLMSG_KEYPRESS:
			r = atLISTBOX_EXT_OnKeyPress( hControl, lParam1 );
			break;

		// Kyle Start [Add 2009/07/23-15:40]
		case CTLMSG_TIMER:
			atLISTBOX_EXT_OnTimer( hControl, lParam1 );
			break;
		// Kyle [Add 2009/07/23-15:40] End
		
		case CTLMSG_MOUSEDOWN:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			r = atLISTBOX_EXT_OnMouseDown( hControl, x, y );
			break;

		case CTLMSG_MOUSEMOVE:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			r = atLISTBOX_EXT_OnMouseMove( hControl, x, y );
			break;
			
		case CTLMSG_MOUSEUP:
			// Kyle Start [Add 2009/06/22-14:25]
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			r = atLISTBOX_EXT_OnMouseUp( hControl, x, y );
			break;
			
		case CTLMSG_CHANGELAYOUT:
			atCONTROL_GetControlColor( hControl, &cl );
			atCONTROL_SetControlColor( lpListBox->hScrollBar, &cl );
			break;
	}

	return r;
}


//==============================================================================================


atDWORD atLISTBOX_EXT_GetAttr(atHCONTROL hControl )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return 0;

	return lpListBox->dwAttr;
}

void	atLISTBOX_EXT_SetAttr( atHCONTROL hControl, atDWORD dwAttr )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return;

	lpListBox->dwAttr = dwAttr;
}

void	atLISTBOX_EXT_SetBitmap( atHCONTROL hControl,
					  	atHBITMAP hImgNormalLeft, atHBITMAP hImgNormalCenter, atHBITMAP hImgNormalRight,
						  atHBITMAP hImgSelectLeft, atHBITMAP hImgSelectCenter, atHBITMAP hImgSelectRight,
						  atHBITMAP hImgDisableLeft, atHBITMAP hImgDisableCenter, atHBITMAP hImgDisableRight )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return;

	lpListBox->hImgNormalLeft = hImgNormalLeft;
	lpListBox->hImgNormalCenter = hImgNormalCenter;
	lpListBox->hImgNormalRight = hImgNormalRight;
	lpListBox->hImgSelectLeft = hImgSelectLeft;
	lpListBox->hImgSelectCenter = hImgSelectCenter;
	lpListBox->hImgSelectRight = hImgSelectRight;
	lpListBox->hImgDisableLeft = hImgDisableLeft;
	lpListBox->hImgDisableCenter = hImgDisableCenter;
	lpListBox->hImgDisableRight = hImgDisableRight;

	atTICKERBOX_SetBGBitmap(lpListBox->hTickerBox, hImgSelectCenter, hImgSelectCenter, hImgSelectRight);

	lpListBox->bIsChange = atTRUE;
}

void atLISTBOX_EXT_ClearAllItem( atHCONTROL hControl )
{
	atLPCONTROL 	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );

	atUIAPI_RemoveListAll(lpListBox->lpListBoxList, atTRUE);

	lpListBox->nSelectIndex = -1;
	atCONTROL_SetVisible( lpListBox->hScrollBar, atFALSE );
	atCONTROL_SetVisible( lpListBox->hTickerBox, atFALSE );
	lpListBox->bTickerActivate = atFALSE;

	lpListBox->bIsChange = atTRUE;
}

static void _SetScrollBarPosition(atHCONTROL hControl, atLPLISTBOX_EXT lpListBox)
{
	atREGION rcDlg;
	atINT iScrollPos, iSetScroll;
	int item_s_y;
	int item_e_y;
	int v_item_s_y;
	int v_item_e_y;
	atLISTBOX_EXT_GETITEMHEIGHT_FUNC *get_height;
	int i, idx, hh, itm_cnt;

	atCONTROL_GetClientRegion(hControl, &rcDlg);
	
	iScrollPos = lpListBox->nViewTopY;
	
	iSetScroll = iScrollPos;

	get_height = (lpListBox->fnGetItemHeight)? lpListBox->fnGetItemHeight : _defGetItemHeight;
	itm_cnt = atLISTBOX_EXT_GetCount(hControl);

//	item_s_y = lpListBox->nSelectIndex * lpListBox->nItemHeight;
	idx = lpListBox->nSelectIndex;
	if( idx >= 0 ){
		item_s_y = 0;
		if( idx > 0 ) item_s_y += lpListBox->nHeightMargin;
		hh = 0;
		for( i = 0; i < idx; i++ ){
			hh = get_height( hControl, i, lpListBox->lpFuncParam );
			item_s_y += hh;
			item_s_y += lpListBox->nItemGap;
		}
		hh = get_height( hControl, idx, lpListBox->lpFuncParam );
		item_e_y = item_s_y + hh;
		if( idx == itm_cnt-1 ) item_e_y += lpListBox->nHeightMargin;
		v_item_s_y = item_s_y - iScrollPos;
		v_item_e_y = item_e_y - iScrollPos;

		if( v_item_e_y > rcDlg.nHeight )
			iSetScroll = item_e_y - rcDlg.nHeight;
		if( v_item_s_y < 0 )
			iSetScroll = item_s_y;
	}

	if( iSetScroll != iScrollPos ){
		atSCROLLBAR_EXT_SetPosition( lpListBox->hScrollBar, iSetScroll);
		lpListBox->nViewTopY = iSetScroll;
        lpListBox->bIsChange = atTRUE;
//		_Realloc_Items( hControl );
//		atCONTROL_Refresh(hControl);
	}
}

atBOOL atLISTBOX_EXT_SetSelectIndex(atHCONTROL hControl, int index)
{
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr(hControl);
	atLISTBOX_EXT_ITEM		itm;
//	atLPTSTR szSelStr;

//	if( index < 0 ) return atFALSE;
	if( index >= atLISTBOX_EXT_GetCount(hControl) ) return atFALSE;
	if( lpListBox->nSelectIndex == index ) return atTRUE;
	
	if( index >= 0 ){
		if( atLISTBOX_EXT_GetItem(hControl, index, &itm) )
			if( !itm.bEnable ) return atFALSE;
	}

	lpListBox->nSelectIndex		= index;

//	atSCROLLBAR_EXT_SetPosition( lpListBox->hScrollBar, lpListBox->nSelectIndex );

	lpListBox->bIsChange = atTRUE;

_SetScrollBarPosition( hControl, lpListBox );

	_Realloc_Items( hControl );
	if( atCONTROL_IsVisible(lpListBox->hTickerBox) ){
		atTICKERBOX_ResetTick( lpListBox->hTickerBox );
	}

	atCONTROL_PostMessageToParent( hControl, WNDMSG_CHANGE, (long)lpControl->Base.nID, (long)lpListBox->nSelectIndex );

	return atTRUE;
}

/*
atBOOL atLISTBOX_EXT_SetSelectIndexByPage(atHCONTROL hControl, int page)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr(hControl);
	int index;

//	index = page * lpListBox->nOneScreenCount;

	return atLISTBOX_EXT_SetSelectIndex( hControl, index );
}
*/

int atLISTBOX_EXT_MoveSelect(atHCONTROL hControl, int nDirect)
{
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr(hControl);
	atLISTBOX_EXT_ITEM itm; atBOOL is_itm;
	int stidx, maxidx, curidx;
//	atLPTSTR szSelStr;
	
	maxidx = atLISTBOX_EXT_GetCount(hControl)-1;
	if( maxidx < 0 ) return -1;

	stidx = lpListBox->nSelectIndex;
	if( (stidx < 0) && (nDirect < 0) ){
		stidx = maxidx + nDirect;
	}

	is_itm = atFALSE;

	curidx = stidx;
	do {
		curidx += nDirect;

		if( lpListBox->dwAttr & atLISTBOX_EXT_ATTR_MOVE_WRAP ){
			if( nDirect < 0 && curidx < 0) 
				curidx = maxidx;
			else if( nDirect > 0 && curidx > maxidx) 
				curidx = 0;
		} else if( lpListBox->dwAttr & atLISTBOX_EXT_ATTR_MOVE_NEXTCONTROL ){
			if( nDirect < 0 && curidx < 0){
				return 0;
			} else if( nDirect > 0 && curidx > maxidx) {
				return maxidx;
			}
		} else {
			if( nDirect < 0 && curidx < 0) {
				curidx = 0;
				is_itm = atLISTBOX_EXT_GetItem(hControl, curidx, &itm);
				break;
			} else if( nDirect > 0 && curidx > maxidx){
				curidx = maxidx;
				is_itm = atLISTBOX_EXT_GetItem(hControl, curidx, &itm);
				break;
			}
		}
		
		is_itm = atLISTBOX_EXT_GetItem(hControl, curidx, &itm);
		
		if( !is_itm || itm.bEnable ) break;
	} while( curidx != stidx );

//	if( curidx == stidx ){
//		lpItem = atUIAPI_GetListAt(lpListBox->lpListBoxList, curidx);
		if( is_itm && !itm.bEnable ) curidx = -1;
//	}

	atLISTBOX_EXT_SetSelectIndex( hControl, curidx );

	return lpListBox->nSelectIndex;
}

atBOOL	atLISTBOX_EXT_ToggleSelectItemChecked(atHCONTROL hControl)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atLISTBOX_EXT_ITEM itm;

	if( lpListBox->nCheckType != 1 ) return atFALSE;

	if( !atLISTBOX_EXT_GetSelectItem(hControl, &itm) ) return atFALSE;
	itm.bChecked = !itm.bChecked;
	atLISTBOX_EXT_SetItem(hControl, atLISTBOX_EXT_GetSelectIndex(hControl), &itm);

	if( itm.bChecked ){
		lpListBox->nCurCheckedIndex = atLISTBOX_EXT_GetSelectIndex(hControl);
	}
	
	return itm.bChecked;
}

void	atLISTBOX_EXT_SetItemCheck(atHCONTROL hControl, int nIndex, atBOOL bCheck)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	int i, cnt;
	atLISTBOX_EXT_ITEM itm, itm2;

	if( lpListBox == atNULL ) return;
	if( lpListBox->nCheckType == 0 ) return;

	cnt = atLISTBOX_EXT_GetCount(hControl);
	if( cnt <= 0 ) return;
	
	if( !atLISTBOX_EXT_GetItem(hControl, nIndex, &itm) ) return;
	if( itm.bChecked == bCheck ) return;

	if( lpListBox->nCheckType == 1 ) {
		itm.bChecked = bCheck;
	} else if( lpListBox->nCheckType == 2 ) {
		if( !bCheck ) return;
		for( i = 0; i < cnt; i++ ){
			if( i == nIndex ) continue;
			atLISTBOX_EXT_GetItem(hControl, i, &itm2);
		   	itm2.bChecked = atFALSE;
			atLISTBOX_EXT_SetItem(hControl, i, &itm2);
		}
		itm.bChecked = atTRUE;
	}
	atLISTBOX_EXT_SetItem(hControl, nIndex, &itm2);
	
	if( bCheck ) lpListBox->nCurCheckedIndex = nIndex;
}

atBOOL atLISTBOX_EXT_GetItemChecked(atHCONTROL hControl, int nIndex)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atLISTBOX_EXT_ITEM itm;

	if( lpListBox == atNULL ) return atFALSE;
	if( (lpListBox->nCheckType != 1) && (lpListBox->nCheckType != 2) ) return atFALSE;

	if( !atLISTBOX_EXT_GetItem(hControl, nIndex, &itm) ) return atFALSE;

	return itm.bChecked;
}

atINT atLISTBOX_EXT_GetCurCheckedIndex(atHCONTROL hControl)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );

	if( lpListBox == atNULL ) return -1;
	if( (lpListBox->nCheckType != 1) && (lpListBox->nCheckType != 2) ) return -1;

	return lpListBox->nCurCheckedIndex;
}

void	atLISTBOX_EXT_SetItemEnabled(atHCONTROL hControl, int nIndex, atBOOL bEnable)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atLISTBOX_EXT_ITEM itm;

	if( !atLISTBOX_EXT_GetItem(hControl, nIndex, &itm) ) return;
	
	itm.bEnable = bEnable;
	atLISTBOX_EXT_SetItem(hControl, nIndex, &itm);
	if( !bEnable ){
		if( nIndex == atLISTBOX_EXT_GetSelectIndex(hControl) )
			atLISTBOX_EXT_MoveSelect( hControl, 1 );
	}
}

atBOOL atLISTBOX_EXT_GetItemEnabled(atHCONTROL hControl, int nIndex)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atLISTBOX_EXT_ITEM itm;

	if( !atLISTBOX_EXT_GetItem(hControl, nIndex, &itm) ) return atFALSE;

	return itm.bEnable;	
}

/*
void atLISTBOX_EXT_Resize( atHCONTROL hControl, int nX, int nY, int nWidte, int nHeight )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );

	lpControl->Base.rect.nStartX	=	nX;
	lpControl->Base.rect.nStartY	=	nY;	
	lpControl->Base.rect.nEndX	= nX + nWidte;
	lpControl->Base.rect.nEndY = nY + nHeight;

	_Realloc_Items(hControl);
}
*/


/******아이템 추가 함수*********/
void atLISTBOX_EXT_Start_AddItem(atHCONTROL hControl)
{	
	atLISTBOX_EXT_Start_AddItemEx( hControl, -1 );
}

void	atLISTBOX_EXT_Start_AddItemEx(atHCONTROL hControl, atINT nIndex)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOX_EXT_ITEM		lpData;
	
	if( lpListBox == atNULL ) return;
	
	lpListBox->nAddItemIndex = nIndex;
	lpListBox->lpAddItemData = (atLPLISTBOX_EXT_ITEM)UI_MEMALLOC(sizeof(atLISTBOX_EXT_ITEM));
	lpData	= lpListBox->lpAddItemData;
	atUIAPI_memset(lpData, 0, sizeof(atLISTBOX_EXT_ITEM));
	lpData->bEnable = atTRUE;
	lpData->nNO = 0;
	lpData->nItemID = -1;
	lpData->data = atNULL;
	lpData->clItemColor[atLISTBOX_EXT_STR_NORMAL] = (atCOLOR)0xFFFFFFFF;
	lpData->clItemColor[atLISTBOX_EXT_STR_SELECT] = (atCOLOR)0xFFFFFFFF;
}

void atLISTBOX_EXT_AddItem_Enabled(atHCONTROL hControl, atBOOL bEnabled)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOX_EXT_ITEM		lpData	= lpListBox->lpAddItemData;

	lpData->bEnable	= bEnabled;
}

void atLISTBOX_EXT_AddItem_Checked(atHCONTROL hControl, atBOOL bChecked)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOX_EXT_ITEM		lpData	= lpListBox->lpAddItemData;

	lpData->bChecked	= bChecked;
}

void atLISTBOX_EXT_AddItem_NO(atHCONTROL hControl, int nNO)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOX_EXT_ITEM		lpData	= lpListBox->lpAddItemData;

	lpData->nNO	= nNO;
}

void atLISTBOX_EXT_AddItem_ItemID(atHCONTROL hControl, int nItemID)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOX_EXT_ITEM		lpData	= lpListBox->lpAddItemData;

	lpData->nItemID	= nItemID;
}

void atLISTBOX_EXT_AddItem_String(atHCONTROL hControl, atLPTSTR szItemName, int nIndex) 
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOX_EXT_ITEM		lpData	= lpListBox->lpAddItemData;

	UI_STRNCPY(lpData->szItemName[nIndex], szItemName, atLISTBOX_EXT_STR_LEN-1);
	lpData->szItemName[nIndex][atLISTBOX_EXT_STR_LEN-1] = _ATC('\0');
	lpData->clItemColor[nIndex] = (atCOLOR)0xFFFFFFFF;

	lpData->nItemNameIds[nIndex] = 0;
}


void atLISTBOX_EXT_AddItem_IDS(atHCONTROL hControl, atDWORD ids, int nIndex) 
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOX_EXT_ITEM		lpData	= lpListBox->lpAddItemData;

	lpData->szItemName[nIndex][0] = 0;
	lpData->szItemName[nIndex][1] = 0;
	lpData->nItemNameIds[nIndex]  = ids;
	lpData->clItemColor[nIndex] = (atCOLOR)0xFFFFFFFF;
}


void	atLISTBOX_EXT_AddItem_String2(atHCONTROL hControl, atLPTSTR szItemName, atCOLOR clTextColor, int nIndex) 
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOX_EXT_ITEM		lpData	= lpListBox->lpAddItemData;

	UI_STRNCPY(lpData->szItemName[nIndex], szItemName, atLISTBOX_EXT_STR_LEN-1);
	lpData->szItemName[nIndex][atLISTBOX_EXT_STR_LEN-1] = _ATC('\0');
	lpData->clItemColor[nIndex] = clTextColor;
	lpData->nItemNameIds[nIndex] = 0;
}

	
void atLISTBOX_EXT_AddItem_Data(atHCONTROL hControl, atVOID*data) 
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOX_EXT_ITEM		lpData	= lpListBox->lpAddItemData;

	lpData->data = data;
}

void atLISTBOX_EXT_AddItem_Image(atHCONTROL hControl, atHBITMAP imgListImage, int nIndex)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOX_EXT_ITEM		lpData	= lpListBox->lpAddItemData;

	lpData->imgItemImage[nIndex]		= imgListImage;
	if( imgListImage == atNULL ) return;
	lpData->imgItemImageRegion[nIndex].nX = 0;
	lpData->imgItemImageRegion[nIndex].nY = 0;
	lpData->imgItemImageRegion[nIndex].nWidth = -1;//atUIAPI_GetBitmapWidth(imgListImage);
	lpData->imgItemImageRegion[nIndex].nHeight = -1;//atUIAPI_GetBitmapHeight(imgListImage);
}

void atLISTBOX_EXT_AddItem_Image2(atHCONTROL hControl, atHBITMAP imgListImage, int x, int y, int width, int height, int nIndex)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	atLPLISTBOX_EXT_ITEM		lpData	= lpListBox->lpAddItemData;

	lpData->imgItemImage[nIndex]		= imgListImage;
	if( imgListImage == atNULL ) return;
	lpData->imgItemImageRegion[nIndex].nX = x;
	lpData->imgItemImageRegion[nIndex].nY = y;
	lpData->imgItemImageRegion[nIndex].nWidth = (width>=0)? width : atUIAPI_GetBitmapWidth(imgListImage);
	lpData->imgItemImageRegion[nIndex].nHeight = (height>=0)? height : atUIAPI_GetBitmapHeight(imgListImage);
}

void atLISTBOX_EXT_Update_AddItem(atHCONTROL hControl)
{
	int idx;
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return;

	if ( lpListBox->lpAddItemData == atNULL ) return;

	if( lpListBox->nAddItemIndex >= 0 ){
		idx = atUIAPI_InsertListBefore(lpListBox->lpListBoxList, lpListBox->nAddItemIndex, lpListBox->lpAddItemData);
	} else {
		idx = atUIAPI_AddListTail(lpListBox->lpListBoxList, lpListBox->lpAddItemData);
	}

	if( lpListBox->nSelectIndex == -1 ){
		if( lpListBox->lpAddItemData->bEnable )
		  atLISTBOX_EXT_SetSelectIndex( hControl, idx );
	}

	if(  lpListBox->lpAddItemData->bChecked ){
		if( lpListBox->nCurCheckedIndex == -1 ) 
			lpListBox->nCurCheckedIndex = idx;
	}

//	atSCROLLBAR_EXT_SetSize( lpListBox->hScrollBar, atLISTBOX_EXT_GetCount(hControl) );

	lpListBox->lpAddItemData = atNULL;

	lpListBox->bIsChange = atTRUE;

	_Realloc_Items(hControl);

/*
	if( atLISTBOX_EXT_GetCount(hControl) > lpListBox->nOneScreenCount ){
		if( lpListBox->hScrollBar && !atCONTROL_IsVisible(lpListBox->hScrollBar) ){
			if( lpListBox->hTickerBox && atCONTROL_IsVisible(lpListBox->hTickerBox) ){
				atCONTROL_IncSize(lpListBox->hTickerBox, -8, 0);
			}
		}
		if( !atCONTROL_IsVisible(lpListBox->hScrollBar) ){
			atCONTROL_SetVisible( lpListBox->hScrollBar, atTRUE );
			_Realloc_Items(hControl);
		}
	} else {
//		atCONTROL_SetVisible( lpListBox->hScrollBar, atFALSE );
	}
*/
}


void    atLISTBOX_EXT_DeleteItem(atHCONTROL hControl, int nIndex)
{   
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	int cnt;

    atUIAPI_RemoveListAt(lpListBox->lpListBoxList, nIndex/*atUIAPI_GetListAt(lpListBox->lpListBoxList, nIndex)*/, atTRUE);

	cnt = atLISTBOX_EXT_GetCount(hControl);

	if( lpListBox->nSelectIndex >= cnt ) lpListBox->nSelectIndex = cnt - 1;

	if( nIndex == lpListBox->nCurCheckedIndex ){
		lpListBox->nCurCheckedIndex = -1;
	}
	
	lpListBox->bIsChange = atTRUE;

	_Realloc_Items(hControl);
}

int atLISTBOX_EXT_GetCount(atHCONTROL hControl)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return 0;

	if( lpListBox->fnGetCount == atNULL )
		return (lpListBox->lpListBoxList)? atUIAPI_GetListCount(lpListBox->lpListBoxList) : 0;
	else
		return lpListBox->fnGetCount( lpListBox, atCONTROL_GetUserDataPtr(hControl) );
}

int atLISTBOX_EXT_GetSelectIndex(atHCONTROL hControl)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return -1;

	return lpListBox->nSelectIndex;
}

atBOOL atLISTBOX_EXT_SetItem(atHCONTROL hControl, int nIndex, atLPLISTBOX_EXT_ITEM lpItem)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return atFALSE;

	lpListBox->bIsChange = atTRUE;

	if( lpListBox->fnSetItem == atNULL ){
		atUIAPI_SetListAt( lpListBox->lpListBoxList, nIndex, lpItem, atFALSE );
		return atTRUE;
	} else
		return lpListBox->fnSetItem( lpListBox, atCONTROL_GetUserDataPtr(hControl), nIndex, lpItem );
}

atBOOL atLISTBOX_EXT_GetItem(atHCONTROL hControl, int nIndex, atLISTBOX_EXT_ITEM *lpItem)
{
	atLPLISTBOX_EXT_ITEM itm;
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return atFALSE;

	if( lpListBox->fnGetItem == atNULL ){
		itm = (atLPLISTBOX_EXT_ITEM)atUIAPI_GetListAt( lpListBox->lpListBoxList, nIndex );
		if( itm == atNULL ) return atFALSE;
		*lpItem = *itm;
		return atTRUE;
	} else {
		return lpListBox->fnGetItem( lpListBox, atCONTROL_GetUserDataPtr(hControl), nIndex, lpItem );
	}
}

atBOOL atLISTBOX_EXT_GetSelectItem(atHCONTROL hControl, atLISTBOX_EXT_ITEM *lpItem)
{
	return atLISTBOX_EXT_GetItem( hControl, atLISTBOX_EXT_GetSelectIndex(hControl), lpItem );
}

atVOID atLISTBOX_EXT_RealizeItem(atHCONTROL hControl)
{
	if( atLISTBOX_EXT_GetSelectIndex(hControl) >= atLISTBOX_EXT_GetCount(hControl) ){
		atLISTBOX_EXT_SetSelectIndex(hControl, atLISTBOX_EXT_GetCount(hControl));
		atLISTBOX_EXT_MoveSelect( hControl, -1 );
	} else {
		//if( atLISTBOX_EXT_GetSelectItem(hControl)

		// Kyle Start [Add 2009/06/01-16:42]
		atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
		if( lpListBox == atNULL ) return;

        lpListBox->bIsChange = atTRUE;
		// Kyle [Add 2009/06/01-16:42] End
	}
	
	_Realloc_Items(hControl);
}

atLPTSTR atLISTBOX_EXT_GetItemString(atHCONTROL hControl, int nIndex, int nStringIndex)
{
	atBOOL ret;
	atLISTBOX_EXT_ITEM itm;

	ret = atLISTBOX_EXT_GetItem( hControl, nIndex, &itm );
	
	return (ret)? itm.szItemName[nStringIndex] : atNULL;
}

atVOID* atLISTBOX_EXT_GetItemData(atHCONTROL hControl, int nIndex)
{
	atBOOL ret;
	atLISTBOX_EXT_ITEM itm;

	ret = atLISTBOX_EXT_GetItem( hControl, nIndex, &itm );
	
	return (ret)? itm.data : atNULL;
}

atVOID* atLISTBOX_EXT_GetCurItemData(atHCONTROL hControl)
{
	return atLISTBOX_EXT_GetItemData(hControl, atLISTBOX_EXT_GetSelectIndex(hControl));
}


atINT atLISTBOX_EXT_ResetSelect(atHCONTROL hControl)
{
	int r;
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return -1;
	
	lpListBox->nViewTopY = 0;
	lpListBox->nSelectIndex = -1;
	r = atLISTBOX_EXT_MoveSelect( hControl, 1 );
	
	_Realloc_Items(hControl);

	return r;
}


atINT atLISTBOX_EXT_GetViewTopY(atHCONTROL hControl)
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr(hControl);
	if( lpListBox == atNULL ) return 0;

	return lpListBox->nViewTopY;
}


void atLISTBOX_EXT_SetupKey( atHCONTROL hControl, atINT SelectKey, atINT CheckKey )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return;

	if( lpListBox->SelectKey == atVKEY_SOFT1 ){
		atCONTROL_SetSoftKey(hControl, _AT(""), atNULL, atNULL);
	} else if( lpListBox->SelectKey == atVKEY_SOFT2 ){
		atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
	} else if( lpListBox->SelectKey == atVKEY_SELECT ){
		atCONTROL_SetSoftKey(hControl, atNULL, _AT(""), atNULL);
	}
	if( lpListBox->CheckKey == atVKEY_SOFT1 ){
		atCONTROL_SetSoftKey(hControl, _AT(""), atNULL, atNULL);
	} else if( lpListBox->CheckKey == atVKEY_SOFT2 ){
		atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
	} else if( lpListBox->CheckKey == atVKEY_SELECT ){
		atCONTROL_SetSoftKey(hControl, atNULL, _AT(""), atNULL);
	}
	
	lpListBox->SelectKey = SelectKey;
	lpListBox->CheckKey = CheckKey;

	if( lpListBox->SelectKey == atVKEY_SOFT1 ){
		atCONTROL_SetSoftKey(hControl, _AT("OK"), atNULL, atNULL);
	} else if( lpListBox->SelectKey == atVKEY_SOFT2 ){
		atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("OK"));
	} else if( lpListBox->CheckKey == atVKEY_SELECT ){
		atCONTROL_SetSoftKey(hControl, atNULL, _AT("OK"), atNULL);
	}

	atSOFTKEY_RefreshSoftKey();
}


void atLISTBOX_EXT_GetFunc( atHCONTROL hControl, 
									atLISTBOX_EXT_DRAWITEM_BG_FUNC **lppfnDrawItemBG, 
									atLISTBOX_EXT_DRAWITEM_FUNC **lppfnDrawItem, 
									atLISTBOX_EXT_GETITEMHEIGHT_FUNC **lppfnGetItemHeight, 
									atLISTBOX_EXT_GETCOUNT_FUNC **lppfnGetCount, 
									atLISTBOX_EXT_GETITEM_FUNC **lppfnGetItem,
									atLISTBOX_EXT_SETITEM_FUNC **lppfnSetItem,
									atLPVOID	*lpFuncParam )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return;
	
	if( lppfnDrawItemBG ) *lppfnDrawItemBG = lpListBox->fnDrawItemBG;
	if( lppfnDrawItem ) *lppfnDrawItem = lpListBox->fnDrawItem;
	if( lppfnGetItemHeight ) *lppfnGetItemHeight = lpListBox->fnGetItemHeight;
	if( lppfnGetCount ) *lppfnGetCount = lpListBox->fnGetCount;
	if( lppfnGetItem ) *lppfnGetItem = lpListBox->fnGetItem;
	if( lppfnSetItem ) *lppfnSetItem = lpListBox->fnSetItem;
	if( lpFuncParam ) *lpFuncParam = lpListBox->lpFuncParam;
}

void atLISTBOX_EXT_SetupFunc( atHCONTROL hControl, 
									atLISTBOX_EXT_DRAWITEM_BG_FUNC *fnDrawItemBG, 
									atLISTBOX_EXT_DRAWITEM_FUNC *fnDrawItem, 
									atLISTBOX_EXT_GETITEMHEIGHT_FUNC *fnGetItemHeight, 
									atLISTBOX_EXT_GETCOUNT_FUNC *fnGetCount, 
									atLISTBOX_EXT_GETITEM_FUNC *fnGetItem,
									atLISTBOX_EXT_SETITEM_FUNC *fnSetItem,
									atLPVOID	lpFuncParam )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return;
	
	lpListBox->fnDrawItemBG = (fnDrawItemBG)? fnDrawItemBG : _defDrawItemBG;
	lpListBox->fnDrawItem = (fnDrawItem)? fnDrawItem : _defDrawItem;
	lpListBox->fnGetItemHeight = (fnGetItemHeight)? fnGetItemHeight : _defGetItemHeight;
	lpListBox->fnGetCount = fnGetCount;
	lpListBox->fnGetItem = fnGetItem;
	lpListBox->fnSetItem = fnSetItem;
	lpListBox->lpFuncParam = lpFuncParam;

	atLISTBOX_EXT_ResetSelect( hControl );
	atCONTROL_Refresh( hControl );
}

// Kyle Start [Add 2009/07/23-15:45]
atVOID atLISTBOX_EXT_SetUseSmoothScroll( atHCONTROL hControl, atBOOL bUsed )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return;

	lpListBox->bUseSmoothScroll = bUsed;
}

atBOOL atLISTBOX_EXT_GetUseSmoothScroll( atHCONTROL hControl )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	return lpListBox ? lpListBox->bUseSmoothScroll : atFALSE;
}
// Kyle [Add 2009/07/23-15:45] End

atVOID atLISTBOX_EXT_SetItemMargin( atHCONTROL hControl, int nWidthMargin, int nHeightMargin )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return ;

	lpListBox->nWidthMargin  = nWidthMargin;
	lpListBox->nHeightMargin = nHeightMargin;
}

atVOID atLISTBOX_EXT_GetItemMargin( atHCONTROL hControl, int* nWidthMargin, int* nHeightMargin )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return ;

	*nWidthMargin  = lpListBox->nWidthMargin;
	*nHeightMargin = lpListBox->nHeightMargin;
}

atVOID atLISTBOX_EXT_SetItemGap( atHCONTROL hControl, int nItemGap )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return ;

	lpListBox->nItemGap = nItemGap;
}

int atLISTBOX_EXT_GetItemGap( atHCONTROL hControl )
{
	atLPLISTBOX_EXT lpListBox = (atLPLISTBOX_EXT)atCONTROL_GetDataPtr( hControl );
	if( lpListBox == atNULL ) return 0;

	return lpListBox->nItemGap;
}
