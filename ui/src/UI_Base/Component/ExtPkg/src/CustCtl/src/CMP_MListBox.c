#include "A_UIENG.h"
#include "UI_Util.h"
#include "CMP_MListBox.h"
#include "CMP_ScrollBarEx.h"
#include "CMP_TickerBox.h"

// 터치 포인트를 움직일 때, 선택 오차인지 이동/스크롤인지 체크하기 위한 한계값. (unit : Inch)
#define CHECK_POINT_MOVE_LIMIT			0.05


static void	_SetScrollBarPosition( atHCONTROL hControl, atLPMLISTBOX lpMListBox );
static void	_Realloc_Items( atHCONTROL hControl );
static void 	_SetScrollPos( atHCONTROL hControl, int scroll_pos, atBOOL is_change_scrollbar );
static int MListBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );

#define COLOR_SELECTBAR		(atUIAPI_GetRGB(0,120,200))
#define TICKER_INTERVAL		50
#define IMAGE_WIDTH			16

struct _CreateParam {
	int		nItemHeight, nItemGap;
	int		nWidthMargin, nHeightMargin;
	atBOOL	bFirstNO;
	int		nCheckType;
	atBOOL	bTicker;
	int		nLineCnt;

	atHBITMAP	hImgNormalLeft, hImgNormalCenter, hImgNormalRight;
	atHBITMAP	hImgSelectLeft, hImgSelectCenter, hImgSelectRight;
	atHBITMAP	hImgDisableLeft, hImgDisableCenter, hImgDisableRight;
	atHBITMAP	lpUncheckedImage, lpCheckedImage;
	atHBITMAP	lpScrollbarImgTop, lpScrollbarImage, lpScrollbarImgBottom;//, lpScrollcursorImage;
	atHBITMAP	lpScrollCursorImgTop, lpScrollCursorImgMid, lpScrollCursorImgBot;
};

static void _FilterHighLight(atTCHAR * _cpName, atBYTE * _flag, atBOOL bFilterSpecialWord)
{
	atTCHAR cpResName[256] = {0, };
	atTCHAR cpVal;
	atINT iLen, iCnt, ii;
	
	iLen = atUIAPI_strlen(_cpName);
	if(iLen < 1) return;
	if(iLen > 250) iLen = 250;

	atUIAPI_strncpy(cpResName, _cpName, iLen);
	cpResName[iLen] = 0;
	
	// 움나우트 문자들은 해당 알파벳(대문자)로 변환
	for(ii = 0 ; ii < iLen ; ii++)
	{
		cpVal = _cpName[ii];

		if(_ATC('A') <= cpVal && cpVal <= _ATC('Z'))
			continue;

		// 소문자
		if(_ATC('a') <= cpVal && cpVal <= _ATC('z'))
			cpResName[ii] -= 32;
		// 'A, "a
		else if((192 <= cpVal && cpVal <= 198) || (224 <= cpVal && cpVal <= 230) || cpVal == 170)
			cpResName[ii] = 65;
		// c 
		else if(cpVal == 199 || cpVal == 231)
			cpResName[ii] = 67;
		// e
		else if((200 <= cpVal && cpVal <= 203) || (232 <= cpVal && cpVal <= 235))
			cpResName[ii] = 69;
		// i
		else if((204 <= cpVal && cpVal <= 207) || (236 <= cpVal && cpVal <= 239))
			cpResName[ii] = 73;
		// d
		else if(cpVal == 208 || cpVal == 240)
			cpResName[ii] = 68;
		// n
		else if(cpVal == 209 || cpVal == 241)
			cpResName[ii] = 78;
		// o
		else if((210 <= cpVal && cpVal <= 214) || (242 <= cpVal && cpVal <= 246) || cpVal == 216 || cpVal == 248 || cpVal ==140 || cpVal == 156)
			cpResName[ii] = 79;
		// x
		else if(cpVal == 215 || cpVal == 247)
			cpResName[ii] = 88;
		// u
		else if((217 <= cpVal  && cpVal <= 220) || (249 <= cpVal && cpVal <= 252) || cpVal == 181)
			cpResName[ii] = 85;
		// y
		else if(cpVal == 221 || cpVal == 253 || cpVal == 159)
			cpResName[ii] = 89;
		// p
		else if(cpVal == 222 || cpVal == 254)
			cpResName[ii] = 80;
		// s
		else if(cpVal == 223 || cpVal == 138 || cpVal == 154)
			cpResName[ii] = 83;
		// z
		else if(cpVal == 142 || cpVal == 158)
			cpResName[ii] = 90 ;
		else
			cpResName[ii] = cpVal;
	}

	if(bFilterSpecialWord)
	{
		iCnt = 0;
		for(ii = 0 ; ii < iLen ; ii++)
		{
			if((65 <= cpResName[ii] && cpResName[ii] <= 90) ||
			   (48 <= cpResName[ii] && cpResName[ii] <= 57) ||
			   (224 <= cpResName[ii] && cpResName[ii] <= 255) ||
			   (1005 <= cpResName[ii] && cpResName[ii] <= 1263)
#ifdef __USES_UNICODE_
			   || (44032 <= cpResName[ii] && cpResName[ii] <= 55203)	// 한글
#endif
			   )
			{
				_cpName[iCnt++] = cpResName[ii];
				if(ii == 0 || (_flag[ii-1] != 1 && _flag[ii-1] != 2))
				{
					_flag[ii] = 1;	// start
				}
				else
				{
					_flag[ii] = 2;	// used
				}
			}
			else
			{
				switch (cpResName[ii])
				{
					case _ATC('('): 
					case _ATC(')'): 
					case _ATC('-'): 
						break;
					case _ATC(' '): 
					case _ATC('&'): 
					case _ATC('_'): 
					case _ATC('#'): 
					case _ATC('/'): 
					case _ATC(','): 
					case _ATC('~'): 
					case _ATC('<'): 
					case _ATC('>'): 
					case _ATC('\''): 
						_cpName[iCnt++] = _ATC(' ');
						_flag[ii] = 0xFF;	// blank
						break;
				}
			}
		}

		_cpName[iCnt] = 0;
	}
	else
	{
		atUIAPI_strncpy(_cpName, cpResName, atUIAPI_strlen(cpResName));
		for(ii = 0 ; ii < iLen ; ii++)
		{
			if(_cpName[ii] == _ATC(' '))
				_flag[ii] = 0xFF;	// blank
			else if(ii == 0 || (_flag[ii-1] != 1 && _flag[ii-1] != 2))
				_flag[ii] = 1;		// start
			else
				_flag[ii] = 2;		// used
		}
		
	}
}

static void _FilterKeyWord(atTCHAR * _cpName, atDWORD dwMode)
{
	atTCHAR cpResName[256] = {0, };
	atTCHAR cpVal;
	atINT iLen, iCnt, ii;

	iLen = atUIAPI_strlen(_cpName);
	if(iLen < 1) return;
	if(iLen > 250) iLen = 250;

	atUIAPI_strncpy(cpResName, _cpName, iLen);
	cpResName[iLen] = 0;

	// 움나우트 문자들은 해당 알파벳(대문자)로 변환
	for(ii = 0 ; ii < iLen ; ii++)
	{
		cpVal = _cpName[ii];

		if(_ATC('A') <= cpVal && cpVal <= _ATC('Z'))
			continue;

		// A ,A 'A, "a
		if((192 <= cpVal && cpVal <= 198))
			cpResName[ii] = 65;
		// c 
		else if(cpVal == 199)
			cpResName[ii] = 67;
		// d
		else if(cpVal == 208)
			cpResName[ii] = 68;
		// e
		else if((200 <= cpVal && cpVal <= 203))
			cpResName[ii] = 69;
		// i
		else if((204 <= cpVal && cpVal <= 207))
			cpResName[ii] = 73;
		// n
		else if(cpVal == 209)
			cpResName[ii] = 78;
		// o
		else if((210 <= cpVal && cpVal <= 214) || cpVal == 216)
			cpResName[ii] = 79;
		// x
		else if(cpVal == 215)
			cpResName[ii] = 88;
		// u
		else if((217 <= cpVal  && cpVal <= 220))
			cpResName[ii] = 85;
		// y
		else if(cpVal == 221)
			cpResName[ii] = 89;
		// p
		else if(cpVal == 222)
			cpResName[ii] = 80;
		// s
		else if(cpVal == 223)
			cpResName[ii] = 83;
		// a~z 대문자로 
		else if('a' <= cpVal && cpVal <= 'z')
			cpResName[ii] -= 32;
		else
			cpResName[ii] = cpVal;
	
	}

	iCnt = 0;
	if(dwMode)
	{
		for(ii = 0 ; ii < iLen ; ii++)
		{
			switch(cpResName[ii])
			{
				case _ATC('&'): 
				case _ATC('+'):
				case _ATC('_'): 
				case _ATC('#'): 
				case _ATC('/'): 
				case _ATC(','): 
				case _ATC('~'): 
				case _ATC('<'): 
				case _ATC('>'): 
				case _ATC('('): 
				case _ATC(')'): 
				case _ATC('\''): 
				case _ATC('-'): 
				case _ATC('.'): // . 도 ' '(공백)으로 처리한다.
					_cpName[iCnt++] = _ATC(' ');
					break;
				default:
					_cpName[iCnt++] = cpResName[ii];
					break;
			}
		}
	}
	else
	{
		for(ii = 0 ; ii < iLen ; ii++)
		{
			cpVal = cpResName[ii];

			// 숫자 or 영문 
			if((65 <= cpVal && cpVal <= 90) || (48 <= cpVal && cpVal <= 57) ||
			   (128 <= cpVal && cpVal <= 255) || (1005 <= cpVal && cpVal <= 1263))
			{
			   _cpName[iCnt++] = cpResName[ii];
			}
		}
	}

	_cpName[iCnt] =0;
}

static void _MakeHLList(atTCHAR * lpszText, atTCHAR * lpszHL, atBYTE * lpByteHL, atBOOL bUseFilter, atDWORD dwStartHLIdx, atDWORD dwEndHLIdx)
{
	atTCHAR szCopyStr[256] = {0, };
	atTCHAR szKeyWord[256] = {0, };
	atBYTE pFlag[256];
	atINT nTokenPos[32] = {0, };
	atINT nTokenSize[32] = {0, };
	atINT ii;

	atDWORD dwTokenCnt = 0;
	atBOOL bChangeToken = atFALSE;

	atUIAPI_memset(pFlag, 0x00, sizeof(pFlag));

	atUIAPI_strncpy(szCopyStr, lpszText, atUIAPI_strlen(lpszText));
	_FilterHighLight(szCopyStr, pFlag, bUseFilter);
	atUIAPI_strncpy(szKeyWord, lpszHL, atUIAPI_strlen(lpszHL));
	_FilterKeyWord(szKeyWord, bUseFilter ? 1 : 0xFF); //bUseFilter);

	for(ii = 0 ; ii < atUIAPI_strlen(szKeyWord) ; ii++)
	{
		if(bChangeToken)
		{
			// 공백인 문자열을 찾는다. 또는 NULL문자를 찾는다.
			if(szKeyWord[ii] == _ATC(' '))
			{
				dwTokenCnt++;						// 문자열이 끝났으므로 Count 증가
				bChangeToken = !bChangeToken;		// 공백이 아닌 문자 찾기 시작
			} 
			else
			{
				// NULL 문자가 아니므로 Size 증가
				nTokenSize[dwTokenCnt]++;
			}
		}
		else
		{
			// 공백이 아닌 문자열을 찾는다.
			if(szKeyWord[ii] != _ATC(' '))
			{
				nTokenPos[dwTokenCnt] = ii;			// 시작위치 등록
				nTokenSize[dwTokenCnt] = 1;			// 시작문자열 하나 증가
				bChangeToken = !bChangeToken;		// 공백문자 찾기 시작
			}
		}
	}
	
	// 마지막은 NULL 이므로 마지막 버퍼의 Size 가 있는지 보고 있으면 Count 증가
	if(nTokenSize[dwTokenCnt] > 0) dwTokenCnt++;

	{
		atDWORD dwLenKey = 0;
		atDWORD dwLenOrg = atUIAPI_strlen(lpszText);
		atDWORD dwIndex = 0;
		atBOOL IsContinue;
		atDWORD i, j, k, l;

		atTCHAR * pFind, * pCurKey;
		for(i = 0 ; i < dwLenOrg ; i++)
		{
			if(!pFlag[i]) continue;

			if(pFlag[i] == 1)
			{
				pFind = szCopyStr + dwIndex;
				for(j = 0 ; j < dwTokenCnt ; j++)
				{ 
					pCurKey = szKeyWord + nTokenPos[j];
					if(!pCurKey) continue;

					dwLenKey = nTokenSize[j];
					//if(wcsncmp(pFind, pCurKey, dwLenKey)) continue;
					IsContinue = atFALSE;
					for(k = 0 ; k < dwLenKey ; k++)
					{
						if(pFind[k] != pCurKey[k])
						{
							IsContinue = atTRUE;
							break;
						}
					}

					if(IsContinue) continue;
					
					for(k = i, l = 0 ; k < dwLenOrg ; k++)
					{
						if (!pFlag[k]) continue;
					
						lpByteHL[k] = ((k < dwStartHLIdx || (dwEndHLIdx > 0 && k > dwEndHLIdx)) ? atFALSE : atTRUE);
						l++;

						if(l == dwLenKey) break;
					}
				}
			}

			dwIndex++;
		}
	}
}


static atINT _defGetItemHeight( atHCONTROL hControl, atMLISTBOX *lpListBox, atINT nIndex, atLPVOID lpParam );

static int _GetTotalItemHeight(atHCONTROL hControl, atMLISTBOX *lpMListBox, int item_cnt, int *sel_item_y, int *sel_item_height)
{
	int i, hh;
	int all_item_height;
//	int itm_y, itm_height, view_y;
	atMLISTBOX_GETITEMHEIGHT_FUNC *get_height;
	
	get_height = (lpMListBox->fnFuncs.fnGetItemHeight)? lpMListBox->fnFuncs.fnGetItemHeight : _defGetItemHeight;
	all_item_height = lpMListBox->nHeightMargin;
	for( i = 0; i < item_cnt; i++ ){
		hh = get_height( hControl, lpMListBox, i, lpMListBox->lpFuncParam );
		if( i == lpMListBox->nSelectIndex ){
			if( sel_item_y ) *sel_item_y = all_item_height;
			if( sel_item_height ) *sel_item_height = hh;
		}
		all_item_height += hh;
		if( i < item_cnt-1 ) all_item_height += lpMListBox->nItemGap;
	}
	all_item_height += lpMListBox->nHeightMargin;

	return all_item_height;
}

static int _FindFirstIndex(atHCONTROL hControl, atMLISTBOX *lpMListBox, int nPosY, int item_cnt, int *first_item_y, int *first_item_height)
{
	int i, y;
	int nFirstIdx;
	int h, hh;
	atMLISTBOX_GETITEMHEIGHT_FUNC *get_height;
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	
	get_height = (lpMListBox->fnFuncs.fnGetItemHeight)? lpMListBox->fnFuncs.fnGetItemHeight : _defGetItemHeight;

	y = -lpMListBox->nViewTopY - lpMListBox->nHeightMargin;
	nFirstIdx = -1;
	h = RECT_HEIGHT(lpControl->Base.rect);
	// check-me : 아래의 루틴 무한루프에 빠지는 현상이 있어
	// i < item_cnt 부분 추가함 
	for( i = 0; y < h && i < item_cnt; i++ ){
		hh = get_height( hControl, lpMListBox, i, lpMListBox->lpFuncParam );
		if( y + hh >= nPosY ){
 			nFirstIdx = i;
			if( first_item_y ) *first_item_y = nPosY - y;
			if( first_item_height ) *first_item_height = hh;
			break;
		}
		y += hh;
		if( i < item_cnt-1 ) y += lpMListBox->nItemGap;
	}

	return nFirstIdx;
}

	
static atINT _defGetItemHeight( atHCONTROL hControl, atMLISTBOX *lpListBox, atINT nIndex, atLPVOID lpParam )
{
	return (lpListBox != atNULL)? lpListBox->nItemHeight : 0;
}

static atVOID _defDrawItemBG( atHCONTROL hControl, atMLISTBOX *lpMListBox, atHGDC hCanvas, atINT nIndex, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atLPMLISTBOX_ITEM lpItem, atBOOL bSelected, atBOOL bFocused, atLPVOID lpParam )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	int x, y, w, h, xx;
	int	wHeadImage = 0, hHeadImage = 0;
	int xImg=0, yImg=0, xImg2=0, yImg2=0;
	atHBITMAP lpBM = atNULL;

	x = nX;
	y = nY;
	w = nWidth;
	h = nHeight;
	xx = x + w;

	if( lpItem == atNULL ) 	return;
	
	if(bSelected) {	// List Item Select
		// Select Bar Draw
		if( bFocused ) {
			if( lpMListBox->hImgSelectLeft && lpMListBox->hImgSelectCenter && lpMListBox->hImgSelectRight ) {
				atUIAPI_DrawClient1Line( hCanvas, x, nY, w, 
									  	 lpMListBox->hImgSelectLeft, 
										 lpMListBox->hImgSelectCenter,
										 lpMListBox->hImgSelectRight );
			} else if( lpMListBox->hImgNormalLeft && lpMListBox->hImgNormalCenter && lpMListBox->hImgNormalRight ) {
				atUIAPI_DrawClient1Line( hCanvas, x, nY, w, 
										 lpMListBox->hImgNormalLeft, 
										 lpMListBox->hImgNormalCenter, 
										 lpMListBox->hImgNormalRight );
			} else {
				atUIAPI_FillRect(hCanvas, x, nY, w, nHeight, COLOR_SELECTBAR);
			}
		} else {
			if( lpMListBox->hImgDisableLeft && lpMListBox->hImgDisableCenter && lpMListBox->hImgDisableRight ) { 
				atUIAPI_DrawClient1Line( hCanvas, x, nY, w, 
									  	 lpMListBox->hImgDisableLeft, 
										 lpMListBox->hImgDisableCenter, 
										 lpMListBox->hImgDisableRight );
			} else if( lpMListBox->hImgNormalLeft && lpMListBox->hImgNormalCenter && lpMListBox->hImgNormalRight ) {
				atUIAPI_DrawClient1Line( hCanvas, x, nY, w, 
					 					 lpMListBox->hImgNormalLeft, 
										 lpMListBox->hImgNormalCenter, 
										 lpMListBox->hImgNormalRight );
			} else {
				atUIAPI_FillRect(hCanvas, x, nY, w, nHeight, UI_COLOR_GRAY);
			}
		}
	} else { // List Item nonSelect
		if( lpMListBox->hImgNormalLeft && lpMListBox->hImgNormalCenter && lpMListBox->hImgNormalRight ) {
			atUIAPI_DrawClient1Line( hCanvas, x, nY, w, 
									 lpMListBox->hImgNormalLeft, 
									 lpMListBox->hImgNormalCenter, 
									 lpMListBox->hImgNormalRight );
		} else {
			atUIAPI_FillRect(hCanvas, x, nY, w, nHeight, UI_COLOR_WHITE);
			// Kyle Test Code
			//atUIAPI_DrawRect(hCanvas, x, nY, w, lpMListBox->nItemHeight, UI_COLOR_BLACK, 1);	
		}
	}
}

static atVOID _defDrawItem( atHCONTROL hControl, atMLISTBOX *lpMListBox, atHGDC hCanvas, atINT nIndex, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atBOOL bFirstNo, atINT nCheckType, atLPMLISTBOX_ITEM lpItem, atBOOL bSelected, atBOOL bFocused, atLPVOID lpParam )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atHFONT hFont = atNULL, hTempFont = atNULL;
	
	int x, y, w, h, xx;
	int	wHeadImage = 0, hHeadImage = 0;
	int	xChk, xNO, xStr;
	int xImg=0, yImg=0, xImg2=0, yImg2=0;
	int ChkWidth, NoWidth, ImgWidth, StrWidth;
	
	atHBITMAP lpBM = atNULL;
	atLPTSTR disp_str;
	atTCHAR szBuff[atMLISTBOX_STR_LEN] = {0};
	atTCHAR szBuffTemp[atMLISTBOX_STR_LEN] = {0};
	atCOLOR nColor;

	int img_width, img_height;
	int img2_width, img2_height;
	atTCHAR	szNO[12] = {0, };
	int i, nPos, nTempWidth;
	int vv1, vv2;

	x = nX;
	y = nY;
	w = nWidth;
	h = nHeight;
	xx = x + w;

	if( lpItem == atNULL ) 	return;
		
	// 체크박스 찍을 위치
//	xChk = x + lpMListBox->nWidthMargin;
	xChk = x;
	if( (lpMListBox->nCheckType == 1) || (lpMListBox->nCheckType == 2) ) {
		lpBM = lpMListBox->lpUncheckedImage;
		if( lpBM ) {
			if( atUIAPI_GetBitmapHeight(lpBM) <= nHeight ) {
				ChkWidth = atUIAPI_GetBitmapWidth(lpBM);
			} else {
				ChkWidth = atUIAPI_GetBitmapWidth(lpBM);
			}
		} else {
			ChkWidth = nHeight;
		}
	} else {
		ChkWidth = 0;
	}

	// 숫자 찍을 위치
	xNO = xChk + ChkWidth; 
	if( ChkWidth > 0 ) xNO += lpMListBox->nItemGap;
	// 가장 큰 숫자의 수평 크기
	if (lpMListBox->bFirstNO == atTRUE) {
		UI_SPRINTF(szBuff, _AT("%d."), lpItem->nNO );
		NoWidth = atUIAPI_GetStringWidth(atUIAPI_GetFont(hCanvas), szBuff, -1);
	} else {
		NoWidth = 0;
	}

	// 첫 이미지 찍을 위치
	xImg = xNO + NoWidth; 
	if( NoWidth > 0 ) xImg += lpMListBox->nItemGap;
	// 첫번째 이미지 수평 크기
	if ((lpItem->imgItemImage[0] != atNULL) || (lpItem->imgItemImage[1] != atNULL)) {
		if( lpItem->imgItemImage[0] != atNULL ) {
			if( lpItem->imgItemImageRegion[0].nWidth >= 0 ) {
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
	
	if (lpItem->imgItemImage[atMLISTBOX_IMG_POST] != atNULL) {
		if( lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST].nWidth >= 0 ) {
			wHeadImage	= lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST].nWidth;
		} else {
			wHeadImage	= atUIAPI_GetBitmapWidth(lpItem->imgItemImage[atMLISTBOX_IMG_POST]);
		}

		if( lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST].nHeight >= 0 ) {
			hHeadImage	= lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST].nHeight;
		} else {
			hHeadImage	= atUIAPI_GetBitmapHeight(lpItem->imgItemImage[atMLISTBOX_IMG_POST]);
		}
	}

	if ( (lpMListBox->nCheckType == 1) || (lpMListBox->nCheckType == 2) ) {
		lpBM = (lpItem->bChecked) ? lpMListBox->lpCheckedImage : lpMListBox->lpUncheckedImage;
		if( lpBM ) {
			if( atUIAPI_GetBitmapHeight(lpBM) <= nHeight ) {
				atUIAPI_DrawImageAlign(hCanvas, xChk+1, nY+nHeight/2, lpBM, ALIGN_LEFT_MIDDLE );
			} else {
				atUIAPI_DrawImageStretch(hCanvas, xChk+1, nY, lpBM, nHeight, nHeight );
			}
		} else {
			if(bSelected) { // List Item Select
				nColor =  (lpItem->clItemChkBoxColor[1] != 0xFFFFFFFF) ? lpItem->clItemChkBoxColor[1] : lpControl->tControlColor.clFocusedTxtColor;
			} else { // List Item nonSelect
				if( lpItem->bEnable ) {
					nColor =  (lpItem->clItemChkBoxColor[0] != 0xFFFFFFFF) ? lpItem->clItemChkBoxColor[0] : lpControl->tControlColor.clNormalTxtColor;
				} else {
					nColor = atUIAPI_GetRGB(102, 102, 102);
				}
			}

			// 체크박스 찍을 위치
			if ( lpMListBox->nCheckType == 1 ) {
				int len = nHeight - 2*2;
				if(lpItem->bChecked) {
					atUIAPI_DrawRect( hCanvas, xChk, nY+2, len, len, nColor, 1 );
					atUIAPI_DrawLine( hCanvas, xChk, nY+2, xChk+len-1, nY+2+len-1, nColor, 1, atFALSE );
					atUIAPI_DrawLine( hCanvas, xChk, nY+2+len-1, xChk+len-1, nY+2, nColor, 1, atFALSE );
				} else {
					atUIAPI_DrawRect( hCanvas, xChk, nY+2, len, len, nColor, 1 );
				}
			} else if ( lpMListBox->nCheckType == 2 ) {
				int ratio = (nHeight - 2*2) / 2;
				if(lpItem->bChecked){
					atUIAPI_DrawCircle( hCanvas, xChk+ratio, nY+nHeight/2, ratio, nColor, 1 );
					atUIAPI_FillCircle( hCanvas, xChk+ratio, nY+nHeight/2, 5, nColor );
				} else {
					atUIAPI_DrawCircle( hCanvas, xChk+ratio, nY+nHeight/2, ratio, nColor, 1 );
				}
			}
		}
	}

	if( lpMListBox->bFirstNO ) {
		if(bSelected) { // List Item Select
			nColor =  (lpItem->clItemFirstNoColor[1] != 0xFFFFFFFF) ? lpItem->clItemFirstNoColor[1] : lpControl->tControlColor.clFocusedTxtColor;
		} else { // List Item nonSelect
			if( lpItem->bEnable ) {
				nColor =  (lpItem->clItemFirstNoColor[0] != 0xFFFFFFFF) ? lpItem->clItemFirstNoColor[0] : lpControl->tControlColor.clNormalTxtColor;
			} else {
				nColor = atUIAPI_GetRGB(102, 102, 102);
			}
		}

		UI_SPRINTF(szNO, _AT("%d."), lpItem->nNO);
		if(lpMListBox->nNoAlignLine <= 0) {
			// nNoAlign 값이 0이거나 음수일 경우 중앙정렬한다.
			atUIAPI_DrawStringAlign(hCanvas, xNO, nY+nHeight/2, szNO, ALIGN_LEFT_MIDDLE, nColor);
		} else {
			// 라인을 기준으로 정렬한다.
			atUIAPI_DrawStringAlign(hCanvas, xNO, nY+(nHeight/lpMListBox->nLineCnt)*(lpMListBox->nNoAlignLine-1)+(nHeight/lpMListBox->nLineCnt)/2,
									szNO, ALIGN_LEFT_MIDDLE, nColor);
		}
	}
	
	if( bSelected && (lpItem->imgItemImage[atMLISTBOX_IMG_POST_SELECT] != atNULL) ) {
		img_width = lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST_SELECT].nWidth;
		if( img_width <= 0 ) img_width = atUIAPI_GetBitmapWidth(lpItem->imgItemImage[atMLISTBOX_IMG_POST_SELECT]);
		img_height = lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST_SELECT].nHeight;
		if( img_height <= 0 ) img_height = atUIAPI_GetBitmapHeight(lpItem->imgItemImage[atMLISTBOX_IMG_POST_SELECT]);
		if( img_height > nHeight ) img_height = nHeight;
		/* TextView 에서 이미지 아이콘이 Y < 0 일때 깨지는 현상 발생
		atUIAPI_DrawImageAlignPart( hCanvas, xImg, nY+lpMListBox->nItemHeight/2, 
									lpItem->imgItemImage[atMLISTBOX_IMG_POST_SELECT], 
									lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST_SELECT].nX, 
									lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST_SELECT].nY, 
									img_width, img_height, ALIGN_LEFT_MIDDLE );
		*/
		if(nY >= -(nHeight-img_height)/2) {
			yImg = nY+(nHeight-img_height)/2;
			atUIAPI_DrawImageAlignPart(hCanvas, xImg, yImg/*nY+lpMListBox->nItemHeight/2*/, 
										lpItem->imgItemImage[atMLISTBOX_IMG_POST_SELECT], 
										lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST_SELECT].nX, lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST_SELECT].nY, img_width, img_height,
										ALIGN_LEFT_TOP/*ALIGN_LEFT_MIDDLE*/ );
		} else {
			yImg = 0;
			atUIAPI_DrawImageAlignPart(hCanvas, xImg, yImg, 
										lpItem->imgItemImage[atMLISTBOX_IMG_POST_SELECT], 
										lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST_SELECT].nX, 
										lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST_SELECT].nY-(nY+(nHeight-img_height)/2), 
										img_width, img_height+(nY+(nHeight-img_height)/2),
										ALIGN_LEFT_TOP );
		}
	} else if( lpItem->imgItemImage[atMLISTBOX_IMG_POST] != atNULL && lpItem->bEnable) {
		img_width = lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST].nWidth;
		if( img_width <= 0 ) img_width = atUIAPI_GetBitmapWidth(lpItem->imgItemImage[atMLISTBOX_IMG_POST]);
		img_height = lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST].nHeight;
		if( img_height <= 0 ) img_height = atUIAPI_GetBitmapHeight(lpItem->imgItemImage[atMLISTBOX_IMG_POST]);
		if( img_height > nHeight ) img_height = nHeight;
		/* TextView 에서 이미지 아이콘이 Y < 0 일때 깨지는 현상 발생
		atUIAPI_DrawImageAlignPart( hCanvas, xImg, nY+lpMListBox->nItemHeight/2, 
									lpItem->imgItemImage[atMLISTBOX_IMG_POST], 
									lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST].nX, 
									lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST].nY, 
									img_width, img_height, ALIGN_LEFT_MIDDLE );
		*/
		if(nY >= -(nHeight-img_height)/2) {
			yImg = nY+(nHeight-img_height)/2;
			atUIAPI_DrawImageAlignPart(hCanvas, xImg, yImg/*nY+lpMListBox->nItemHeight/2*/, 
										lpItem->imgItemImage[atMLISTBOX_IMG_POST], 
										lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST].nX, lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST].nY, img_width, img_height,
										ALIGN_LEFT_TOP/*ALIGN_LEFT_MIDDLE*/ );
		} else {
			yImg = 0;
			atUIAPI_DrawImageAlignPart(hCanvas, xImg, yImg, 
										lpItem->imgItemImage[atMLISTBOX_IMG_POST], 
										lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST].nX, 
										lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST].nY-(nY+(nHeight-img_height)/2), 
										img_width, img_height+(nY+(nHeight-img_height)/2),
										ALIGN_LEFT_TOP );
		}
	} else if(lpItem->imgItemImage[atMLISTBOX_IMG_POST_DISABLE] != atNULL && !lpItem->bEnable) {
		img_width = lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST_DISABLE].nWidth;
		if( img_width <= 0 ) img_width = atUIAPI_GetBitmapWidth(lpItem->imgItemImage[atMLISTBOX_IMG_POST_DISABLE]);
		img_height = lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST_DISABLE].nHeight;
		if( img_height <= 0 ) img_height = atUIAPI_GetBitmapHeight(lpItem->imgItemImage[atMLISTBOX_IMG_POST_DISABLE]);
		if( img_height > nHeight ) img_height = nHeight;

		if(nY >= -(nHeight-img_height)/2) {
			yImg = nY+(nHeight-img_height)/2;
			atUIAPI_DrawImageAlignPart(hCanvas, xImg, yImg, 
										lpItem->imgItemImage[atMLISTBOX_IMG_POST_DISABLE], 
										lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST_DISABLE].nX, lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST].nY, img_width, img_height,
										ALIGN_LEFT_TOP );
		} else {
			yImg = 0;
			atUIAPI_DrawImageAlignPart(hCanvas, xImg, yImg, 
										lpItem->imgItemImage[atMLISTBOX_IMG_POST_DISABLE], 
										lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST_DISABLE].nX, 
										lpItem->imgItemImageRegion[atMLISTBOX_IMG_POST_DISABLE].nY-(nY+(nHeight-img_height)/2), 
										img_width, img_height+(nY+(nHeight-img_height)/2),
										ALIGN_LEFT_TOP );
		}
	} else {
		img_width = img_height = 0;
		yImg = 0;
	}

	lpItem->imgItemDrawRect[0].nStartX = xImg;
	lpItem->imgItemDrawRect[0].nStartY = yImg;
	lpItem->imgItemDrawRect[0].nEndX = xImg + img_width;
  	lpItem->imgItemDrawRect[0].nEndY = yImg + img_height;

	if( bSelected && (lpItem->imgItemImage[atMLISTBOX_IMG_TAIL_SELECT] != atNULL) ) {
		xImg2 = xImg + img_width + lpMListBox->nItemGap;
		img2_width = lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL_SELECT].nWidth;
		if( img2_width <= 0 ) img2_width = atUIAPI_GetBitmapWidth(lpItem->imgItemImage[atMLISTBOX_IMG_TAIL_SELECT]);
		img2_height = lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL_SELECT].nHeight;
		if( img2_height <= 0 ) img2_height = atUIAPI_GetBitmapHeight(lpItem->imgItemImage[atMLISTBOX_IMG_TAIL_SELECT]);
		if( img2_height > nHeight ) img2_height = nHeight;
#if 0
		atUIAPI_DrawImageAlignPart( hCanvas, w - (img2_width + lpMListBox->nItemGap), nY+lpMListBox->nItemHeight/2, 
									lpItem->imgItemImage[atMLISTBOX_IMG_TAIL_SELECT], 
									lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL_SELECT].nX, 
									lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL_SELECT].nY, 
									img2_width, img2_height, ALIGN_RIGHT_MIDDLE );
#else
		yImg2 = nY+(nHeight-img2_height)/2;
		atUIAPI_DrawImageAlignPart( hCanvas, xImg2, yImg2,
									lpItem->imgItemImage[atMLISTBOX_IMG_TAIL_SELECT], 
									lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL_SELECT].nX, 
									lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL_SELECT].nY, 
									img2_width, img2_height, ALIGN_LEFT_TOP );
#endif
	} else if( !lpItem->bEnable && lpItem->imgItemImage[atMLISTBOX_IMG_TAIL_DISABLE] != atNULL ) {
		xImg2 = xImg + img_width + lpMListBox->nItemGap;
		img2_width = lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL_DISABLE].nWidth;
		if( img2_width <= 0 ) img2_width = atUIAPI_GetBitmapWidth(lpItem->imgItemImage[atMLISTBOX_IMG_TAIL_DISABLE]);
		img2_height = lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL_DISABLE].nHeight;
		if( img2_height <= 0 ) img2_height = atUIAPI_GetBitmapHeight(lpItem->imgItemImage[atMLISTBOX_IMG_TAIL_DISABLE]);
		if( img2_height > nHeight) img2_height = nHeight;
#if 0
		atUIAPI_DrawImageAlignPart( hCanvas, xImg2 w - (img2_width + lpMListBox->nItemGap), nY+lpMListBox->nItemHeight/2, 
									lpItem->imgItemImage[atMLISTBOX_IMG_TAIL_DISABLE], 
									lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL_DISABLE].nX, 
									lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL_DISABLE].nY, 
									img2_width, img2_height, ALIGN_RIGHT_MIDDLE );
#else
		yImg2 = nY+(nHeight-img2_height)/2;
		atUIAPI_DrawImageAlignPart( hCanvas, xImg2, yImg2,
									lpItem->imgItemImage[atMLISTBOX_IMG_TAIL_DISABLE], 
									lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL_DISABLE].nX, 
									lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL_DISABLE].nY, 
									img2_width, img2_height, ALIGN_LEFT_TOP );
#endif
	} else if( lpItem->imgItemImage[atMLISTBOX_IMG_TAIL] != atNULL ) {
		xImg2 = xImg + img_width + lpMListBox->nItemGap;
		img2_width = lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL].nWidth;
		if( img2_width <= 0 ) img2_width = atUIAPI_GetBitmapWidth(lpItem->imgItemImage[atMLISTBOX_IMG_TAIL]);
		img2_height = lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL].nHeight;
		if( img2_height <= 0 ) img2_height = atUIAPI_GetBitmapHeight(lpItem->imgItemImage[atMLISTBOX_IMG_TAIL]);
		if( img2_height > nHeight ) img2_height = nHeight;
#if 0
		atUIAPI_DrawImageAlignPart( hCanvas, w - (img2_width + lpMListBox->nItemGap), nY+lpMListBox->nItemHeight/2, 
									lpItem->imgItemImage[atMLISTBOX_IMG_TAIL], 
									lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL].nX, 
									lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL].nY, 
									img2_width, img2_height, ALIGN_RIGHT_MIDDLE );
#else
		yImg2 = nY+(nHeight-img2_height)/2;
		atUIAPI_DrawImageAlignPart( hCanvas, xImg2, yImg2, 
									lpItem->imgItemImage[atMLISTBOX_IMG_TAIL], 
									lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL].nX, 
									lpItem->imgItemImageRegion[atMLISTBOX_IMG_TAIL].nY, 
									img2_width, img2_height, ALIGN_LEFT_TOP );
#endif
	} else {
		img2_width = img2_height = 0;
		yImg2 = 0;
	}

	if(xImg2 > 0 && yImg2 > 0) {
		lpItem->imgItemDrawRect[1].nStartX = xImg2;
		lpItem->imgItemDrawRect[1].nStartY = yImg2;
		lpItem->imgItemDrawRect[1].nEndX = xImg2 + img2_width;
  		lpItem->imgItemDrawRect[1].nEndY = yImg2 + img2_height;
	}

	xStr = xImg + img_width;
	if( img_width > 0 ) xStr  += lpMListBox->nItemGap;
	StrWidth = w - xStr;
#if 0
	if( img2_width > 0 ) StrWidth -= img2_width + lpMListBox->nItemGap;
#else
	if(xImg2 > 0 && img2_width > 0) {
		xStr = xImg2 + img2_width + lpMListBox->nItemGap;
		StrWidth = w - xStr;
	}
#endif

	for(i = 0 ; i < lpMListBox->nLineCnt ; i++) {
		atBOOL IsDrawing = atFALSE;

		if(lpItem->nFontSize[i]) {
			hFont = atUIAPI_CreateFont(atUIAPI_FACE_SYSTEM, lpItem->nFontSize[i], atUIAPI_STYLE_PLAIN, atTRUE);
			if(hFont) {
				hTempFont = atUIAPI_SetFont(hCanvas, hFont);
			}
		}

		if(bSelected){	// List Item Select
			// String Resource가 변결된 시점에만, Text를 갱신하도록 변경 필요.
			if(lpItem->nItemNameIds[i][1])
				atUIAPI_LoadString(lpItem->nItemNameIds[i][1], lpItem->szItemName[i][1], atMLISTBOX_STR_LEN);

			disp_str = lpItem->szItemName[i][1];
			if( UI_STRLEN(disp_str) <= 0 ) {  
				if(lpItem->nItemNameIds[i][0])
					atUIAPI_LoadString(lpItem->nItemNameIds[i][0], lpItem->szItemName[i][0],  atMLISTBOX_STR_LEN);

				disp_str = lpItem->szItemName[i][0];
				nColor =  (lpItem->clItemColor[i][1] != 0xFFFFFFFF) ? lpItem->clItemColor[i][1] : lpControl->tControlColor.clFocusedTxtColor;
			} else {
				nColor =  (lpItem->clItemColor[i][1] != 0xFFFFFFFF) ? lpItem->clItemColor[i][1] : lpControl->tControlColor.clFocusedTxtColor;
			}
		} else { // List Item nonSelect
			if(lpItem->nItemNameIds[i][0]) {
				atUIAPI_LoadString(lpItem->nItemNameIds[i][0], lpItem->szItemName[i][0], atMLISTBOX_STR_LEN);
			}

			disp_str = lpItem->szItemName[i][0];
			if( lpItem->bEnable ) {
				nColor =  (lpItem->clItemColor[i][0] != 0xFFFFFFFF) ? lpItem->clItemColor[i][0] : lpControl->tControlColor.clNormalTxtColor;
			} else {
				nColor = atUIAPI_GetRGB(102, 102, 102);
			}
		}

		// Kyle Start [Modify 2009/08/03-14:47]
		if(lpMListBox->nMostAlign[i] == atMLISTBOX_MOSTALIGN_LEFT)			
		{
			if(lpMListBox->nWidthPercent[i] <= 0)			nTempWidth = 0;
			else if(lpMListBox->nWidthPercent[i] >= 100)	nTempWidth = StrWidth - lpMListBox->nWidthMargin;
			else
			{
				vv1 = (int)(((atINT64)100 << 32) / ((100-lpMListBox->nWidthPercent[i]) << 16));
				vv2 = (int)(((atINT64)(StrWidth - lpMListBox->nWidthMargin) << 32) / vv1);
				nTempWidth = (StrWidth - lpMListBox->nWidthMargin) - (vv2 >> 16);
				//nTempWidth = (StrWidth - lpMListBox->nWidthMargin) - (StrWidth - lpMListBox->nWidthMargin) / (100 / (100-lpMListBox->nWidthPercent[i])); // Percent
			}

			nPos = nTempWidth;
		}
		else if(lpMListBox->nMostAlign[i] == atMLISTBOX_MOSTALIGN_RIGHT)	
		{
			if(lpMListBox->nWidthPercent[i] <= 0)			nTempWidth = StrWidth - lpMListBox->nWidthMargin;
			else if(lpMListBox->nWidthPercent[i] >= 100)	nTempWidth = 0;
			else
			{
				vv1 = (int)(((atINT64)100 << 32) / (lpMListBox->nWidthPercent[i] << 16));
				vv2 = (int)(((atINT64)(StrWidth - lpMListBox->nWidthMargin) << 32) / vv1);
				nTempWidth = (StrWidth - lpMListBox->nWidthMargin) - (vv2 >> 16);
				//nTempWidth = (StrWidth - lpMListBox->nWidthMargin) - (StrWidth - lpMListBox->nWidthMargin) / (100 / lpMListBox->nWidthPercent[i]); // Percent
			}
			
			nPos = StrWidth - lpMListBox->nWidthMargin;
		}
		// Kyle [Modify 2009/08/03-14:47] End

		if(lpMListBox->nMostAlign[i] == atMLISTBOX_MOSTALIGN_RIGHT) {
			lpMListBox->rtControlRect[i].nStartX = nPos;
			lpMListBox->rtControlRect[i].nEndX = 0;
		} else if(lpMListBox->nMostAlign[i] == atMLISTBOX_MOSTALIGN_LEFT) {
			lpMListBox->rtControlRect[i].nStartX = 0;
			lpMListBox->rtControlRect[i].nEndX = nPos;
		} else {
			lpMListBox->rtControlRect[i].nStartX = lpMListBox->rtControlRect[i].nEndX = 0;
		}

		if( bSelected && lpMListBox->bTickerEnable ) {
			if(atCONTROL_IsVisible(lpMListBox->hTickerBox[i]) && lpMListBox->bTickerActivate[i] ) {
				//atCONTROL_Draw( lpMListBox->hTickerBox[i], atFALSE);
				//atCONTROL_Refresh( lpMListBox->hTickerBox[i] );	
			} else {
				IsDrawing = atTRUE;
			}
		} else {
			IsDrawing = atTRUE;
		}

		if(IsDrawing) {
			if(lpMListBox->nMostAlign[i] == atMLISTBOX_MOSTALIGN_IGNORE) {
				myStringCut( hCanvas, disp_str, szBuff, StrWidth, atTRUE);
			} else {
				// Kyle Start [Modify 2009/08/03-14:48]
				if(lpMListBox->nWidthPercent[i] > 0)	// 영역이 1% 이상일때 StringCut 을 한다.
				{
					if(lpMListBox->nMostAlign[i] == atMLISTBOX_MOSTALIGN_LEFT)			myStringCut( hCanvas, disp_str, szBuff, nPos, atTRUE);
					else if(lpMListBox->nMostAlign[i] == atMLISTBOX_MOSTALIGN_RIGHT)	myStringCut( hCanvas, disp_str, szBuff, nPos - nTempWidth, atTRUE);
				}
				// Kyle [Modify 2009/08/03-14:48] End
			}

			// Kyle Start [Modify 2009/08/03-14:48]
			if(lpMListBox->nMostAlign[i] == atMLISTBOX_MOSTALIGN_IGNORE ||
			   (lpMListBox->nMostAlign[i] != atMLISTBOX_MOSTALIGN_IGNORE && lpMListBox->nWidthPercent[i] > 0))
			{
				if(atUIAPI_strlen(lpItem->szHighLight) > 0)
				{
 					atINT nDrawLen = atUIAPI_strlen(szBuff);
					if(nDrawLen > 0)
					{
						atINT nDrawOffset = 0, nLeftOffset = 0;
						atINT ii;
						atTCHAR szWord[2] = {0, };

						atUIAPI_memset(lpItem->bWordHL, 0x00, sizeof(lpItem->bWordHL));
						_MakeHLList(disp_str, lpItem->szHighLight, lpItem->bWordHL, !lpItem->bNotUseFilter, 
									lpItem->nStartHLIdx, lpItem->nEndHLIdx );

						nLeftOffset = (lpMListBox->nMostAlign[i] == atMLISTBOX_MOSTALIGN_RIGHT) ? atUIAPI_GetStringWidth(atUIAPI_GetFont(hCanvas), szBuff, -1) : 0;
						for(ii = 0 ; ii < nDrawLen ; ii++)
						{
							szWord[0] = szBuff[ii];
							atUIAPI_DrawStringAlign(hCanvas, xStr+lpMListBox->rtControlRect[i].nStartX+nDrawOffset-nLeftOffset, 
											nY+lpMListBox->rtControlRect[i].nStartY+(RECT_HEIGHT(lpMListBox->rtControlRect[i])/2),
											szWord, 
											ALIGN_LEFT_MIDDLE,
											!lpItem->bWordHL[ii] || szWord[0] == _ATC('.') ? nColor : lpItem->clHLColor);

							nDrawOffset += atUIAPI_GetStringWidth(atUIAPI_GetFont(hCanvas), szWord, -1);
						}
					}
				} 
				else
				{
					atUIAPI_DrawStringAlign(hCanvas, xStr+lpMListBox->rtControlRect[i].nStartX, 
											nY+lpMListBox->rtControlRect[i].nStartY+(RECT_HEIGHT(lpMListBox->rtControlRect[i])/2),
											szBuff, 
											(lpMListBox->nMostAlign[i] == atMLISTBOX_MOSTALIGN_RIGHT) ? ALIGN_RIGHT_MIDDLE : ALIGN_LEFT_MIDDLE,
											//ALIGN_LEFT_MIDDLE,
											nColor);
				}
				// Kyle [Modify 2009/08/03-14:48] End
			}
		}

		if(hFont) {
			atUIAPI_SetFont(hCanvas, hTempFont);
			atUIAPI_DestroyFont(hFont);
			hTempFont = hFont = atNULL;
		}
	}
}

static atINT _defTouchItem( atHCONTROL hControl, struct _tagatMLISTBOX *lpListBox, atINT nIndex, atINT nMsg, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atLPVOID lpParam )
{
	return 0;
}



atHCONTROL atMLISTBOX_Create( int nIndex, int nLineCnt, int nX, int nY, int nWidth, int nHeight,
							  int nItemHeight, int nItemGap, int nWidthMargin, int nHeightMargin,
							  atBOOL bFirstNO, atINT nCheckType, atBOOL bTicker, 
							  atHBITMAP hImgNormalLeft, atHBITMAP hImgNormalCenter, atHBITMAP hImgNormalRight,
							  atHBITMAP hImgSelectLeft, atHBITMAP hImgSelectCenter, atHBITMAP hImgSelectRight,
							  atHBITMAP hImgDisableLeft, atHBITMAP hImgDisableCenter, atHBITMAP hImgDisableRight,
							  atHBITMAP lpUncheckedImage, atHBITMAP lpCheckedImage,
							  //atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom, atHBITMAP lpScrollcursorImage )
							  atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom,
							  atHBITMAP lpScrollCursorImgTop, atHBITMAP lpScrollCursorImgMid, atHBITMAP lpScrollCursorImgBot )
{
	atHCONTROL hControl;
	struct _CreateParam param;

	if(nLineCnt > atMLISTBOX_LINE_MAX) nLineCnt = atMLISTBOX_LINE_MAX;

	param.nItemHeight	= nItemHeight;
	param.nItemGap		= nItemGap;
	param.nWidthMargin	= nWidthMargin;
	param.nHeightMargin	= nHeightMargin;
	param.bTicker		= bTicker;
	param.bFirstNO		= bFirstNO;
	param.nCheckType	= nCheckType;
	param.nLineCnt		= (nLineCnt <= 0) ? 1 : nLineCnt;
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
	param.lpScrollCursorImgTop	= lpScrollCursorImgTop;
	param.lpScrollCursorImgMid	= lpScrollCursorImgMid;
	param.lpScrollCursorImgBot	= lpScrollCursorImgBot;

	hControl = atCONTROL_Create( nIndex, nX, nY, nWidth, nHeight, CTL_PARENTTYPE_WINDOW, sizeof(atMLISTBOX), MListBoxProc, &param );
	return hControl;
}

static void atMLISTBOX_OnInit( atHCONTROL hControl, struct _CreateParam *param )
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atRECT rect;
	int nWidth, nHeight;
	atCONTROL_COLOR cl;
	int i;

	atCONTROL_GetControlColor( hControl, &cl );
	cl.clNormalFrColor		= UI_COLOR_WHITE;
	cl.clNormalTxtColor		= UI_COLOR_BLACK;
	cl.clNormalBgColor		= UI_COLOR_WHITE;
	cl.clFocusedFrColor		= COLOR_SELECTBAR;
	cl.clFocusedTxtColor	= UI_COLOR_WHITE;
	cl.clFocusedBgColor		= COLOR_SELECTBAR;
	cl.clDisableFrColor		= UI_COLOR_GRAY;
	cl.clDisableTxtColor	= UI_COLOR_WHITE;
	cl.clDisableBgColor		= UI_COLOR_GRAY;
	atCONTROL_SetControlColor( hControl, &cl );

	lpMListBox->dwAttr			= atMLISTBOX_ATTR_TRANSPARENT | atMLISTBOX_ATTR_MOVE_NORMAL;
	lpMListBox->lpListBoxList	= atUIAPI_CreateList();
	lpMListBox->lpAddItemData	= atNULL;
	lpMListBox->nAddItemIndex	= -1;
	lpMListBox->nSelectIndex = lpMListBox->nPrevSelectIndex	= -1;
	lpMListBox->nItemHeight		= param->nItemHeight;
	lpMListBox->nItemGap		= param->nItemGap;
	lpMListBox->nWidthMargin	= param->nWidthMargin;
	lpMListBox->nHeightMargin	= param->nHeightMargin;
	lpMListBox->bTickerEnable	= param->bTicker;
	lpMListBox->bScrollVisible	= atTRUE;
	lpMListBox->bFirstNO 		= param->bFirstNO;
	lpMListBox->nCheckType		= param->nCheckType;
	lpMListBox->nLineCnt		= param->nLineCnt;
	lpMListBox->nNoAlignLine	= 0;

	atCONTROL_GetControlRect( hControl, &rect );
	nWidth = RECT_WIDTH(rect);
	nHeight = RECT_HEIGHT(rect);
	
//	lpMListBox->nOneScreenCount		= nHeight / param->nItemHeight;		
	lpMListBox->lpUncheckedImage	= param->lpUncheckedImage;
	lpMListBox->lpCheckedImage		= param->lpCheckedImage;

	for(i = 0 ; i < atMLISTBOX_LINE_MAX ; i++) {
		lpMListBox->bTickerActivate[i] = atFALSE;
		lpMListBox->nMostAlign[i] = atMLISTBOX_MOSTALIGN_IGNORE;
	}

	lpMListBox->bActionValidate		= atFALSE;
	lpMListBox->hImgNormalLeft		= param->hImgNormalLeft;
	lpMListBox->hImgNormalCenter	= param->hImgNormalCenter;
	lpMListBox->hImgNormalRight		= param->hImgNormalRight;
	lpMListBox->hImgSelectLeft		= param->hImgSelectLeft;
	lpMListBox->hImgSelectCenter	= param->hImgSelectCenter;
	lpMListBox->hImgSelectRight		= param->hImgSelectRight;
	lpMListBox->hImgDisableLeft		= param->hImgDisableLeft;
	lpMListBox->hImgDisableCenter	= param->hImgDisableCenter;
	lpMListBox->hImgDisableRight	= param->hImgDisableRight;
	lpMListBox->SelectKey			= atVKEY_SELECT;
	lpMListBox->CheckKey			= atVKEY_SOFT2;
	atUIAPI_memset( &lpMListBox->fnFuncs, 0, sizeof(atMLISTBOX_FUNCS) );

	lpMListBox->hScrollBar = atSCROLLBAR_EXT_Create( 100, 0+nWidth-14, 0, 14, nHeight, atSCROLLBAR_EXT_TYPE_WINDOW_FRICTION_STICK, 0, 0, 0, 0,
													param->lpScrollbarImgTop, param->lpScrollbarImage, param->lpScrollbarImgBottom, 
													param->lpScrollCursorImgTop, param->lpScrollCursorImgMid, param->lpScrollCursorImgBot );
													//param->lpScrollcursorImage );

	atCONTROL_SetAttr( lpMListBox->hScrollBar, atCONTROL_GetAttr(lpMListBox->hScrollBar) | CTL_PARENTTYPE_CONTROL );
	atCONTROL_SetAnchor( lpMListBox->hScrollBar, 0, 0, atANCHOR_IGNORE, 0 );
	atCONTROL_GetControlColor( hControl, &cl );
	atCONTROL_SetControlColor( lpMListBox->hScrollBar, &cl );
	atCONTROL_SetFocusable(lpMListBox->hScrollBar, atFALSE);
	atCONTROL_SetVisible( lpMListBox->hScrollBar, atFALSE );
	atCONTROL_AddChildControl( hControl, lpMListBox->hScrollBar );

	for(i = 0 ; i < atMLISTBOX_LINE_MAX ; i++) {
		lpMListBox->hTickerBox[i] = atNULL;
		SET_RECT(lpMListBox->rtControlRect[i], 0, 0, 0, 0);
	}

	if( param->bTicker ) {
		for(i = 0 ; i < param->nLineCnt ; i++) {
			if( (param->nCheckType != 0) || param->bFirstNO || (param->nWidthMargin >= 4) ) {
				lpMListBox->hTickerBox[i] = atTICKERBOX_Create( 0000, 0, param->nItemHeight/param->nLineCnt*i, nWidth, param->nItemHeight/param->nLineCnt, _AT(""), 0, TICKER_INTERVAL, 4, -20, 0, param->nWidthMargin, param->hImgSelectCenter, param->hImgSelectCenter, param->hImgSelectRight );
			} else {
				lpMListBox->hTickerBox[i] = atTICKERBOX_Create( 0000, 0, param->nItemHeight/param->nLineCnt*i, nWidth, param->nItemHeight/param->nLineCnt, _AT(""), 0, TICKER_INTERVAL, 4, -20, 0, param->nWidthMargin, param->hImgSelectLeft, param->hImgSelectCenter, param->hImgSelectRight );
			}

			atCONTROL_SetAttr( lpMListBox->hTickerBox[i], atCONTROL_GetAttr(lpMListBox->hScrollBar) | CTL_PARENTTYPE_CONTROL );
			atCONTROL_GetControlColor( hControl, &cl );
			atCONTROL_SetControlColor( lpMListBox->hTickerBox[i], &cl );
			atCONTROL_SetVisible(lpMListBox->hTickerBox[i], atFALSE );
			atCONTROL_AddChildControl( hControl, lpMListBox->hTickerBox[i] );

			atCONTROL_GetControlRect( lpMListBox->hTickerBox[i], &lpMListBox->rtControlRect[i] );
			// ticker 가 여러개인 경우 비트맵 배경이 자동으로 크기가 조정되어서 그라데이션이 매칭이 안된다.
			// 그래서 불투명(atTICKERBOXATTR_OPAQUE) 옵션을 OFF 해서 Parent 배경을 쓰면 매칭이 깔끔해진다.
			atTICKERBOX_SetAttr( lpMListBox->hTickerBox[i], atTICKERBOX_GetAttr(lpMListBox->hTickerBox[i]) & ~atTICKERBOXATTR_OPAQUE);
		}
	}
	
	// 삭제함: 무조건 적인 소프트키 설정은 외부에서 소프트키를 바꿀 경우
	// 바꾸는 과정이 보여서 화면이 깜박이게 됨.
	// 밖에서만 설정하게 해야함.
	//atCONTROL_SetSoftKey( hControl, _AT(""), _AT("OK"), _AT("") );

	lpMListBox->nViewTopY = 0;
	lpMListBox->bMouseDown = atFALSE;
	lpMListBox->bIsChange = atTRUE;

	lpMListBox->bReadyClick = atFALSE;
	lpMListBox->bReadySmoothScroll = atFALSE;
}

static void atMLISTBOX_OnRelease( atHCONTROL hControl )
{	
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return;

	if( lpMListBox->lpAddItemData ) {
		UI_MEMFREE( lpMListBox->lpAddItemData );
		lpMListBox->lpAddItemData = atNULL;
	}

	if( lpMListBox->lpListBoxList ){
		atUIAPI_DestroyList(lpMListBox->lpListBoxList, atTRUE);
		lpMListBox->lpListBoxList = atNULL;
	}
}
 
static void atMLISTBOX_OnDraw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr(hControl);

	int i, itm_cnt;	
	int x, y, w, h, hh;
	int ix, iy, iw, ih;
	atMLISTBOX_ITEM Item;
	atLPCONTROL lpControl2;
	atMLISTBOX_DRAWITEM_BG_FUNC *draw_item_bg;
	atMLISTBOX_DRAWITEM_FUNC *draw_item;
	atMLISTBOX_GETITEMHEIGHT_FUNC *get_height;
	atBOOL bSelected;
	atRECT clip;
	atBOOL r;
	
	if( lpMListBox == atNULL ) return;

	// 표시 좌표 보정
	x = 0;
	y = 0;
	w = RECT_WIDTH(lpControl->Base.rect);
	h = RECT_HEIGHT(lpControl->Base.rect);
	
	if ( atCONTROL_IsVisible(lpMListBox->hScrollBar) ) {
		lpControl2 = atCONTROL_GetPtr(lpMListBox->hScrollBar);
		w -= RECT_WIDTH(lpControl2->Base.rect);
	}

	if( lpMListBox->dwAttr & atMLISTBOX_ATTR_OPAQUE ) {
		atUIAPI_FillRect( hCanvas, x, y, w, h, lpControl->tControlColor.clNormalBgColor );  // 배경을 지운다.
	}
	
	ix = x + lpMListBox->nWidthMargin;
	iw = w - lpMListBox->nWidthMargin * 2;

	// 아이템이 하나도 없으면 여기서 중지.
	itm_cnt = atMLISTBOX_GetCount(hControl);
	if( itm_cnt <= 0 ) return;

	draw_item_bg = (lpMListBox->fnFuncs.fnDrawItemBG) ? lpMListBox->fnFuncs.fnDrawItemBG : _defDrawItemBG;
	draw_item = (lpMListBox->fnFuncs.fnDrawItem) ? lpMListBox->fnFuncs.fnDrawItem : _defDrawItem;
	get_height = (lpMListBox->fnFuncs.fnGetItemHeight)? lpMListBox->fnFuncs.fnGetItemHeight : _defGetItemHeight;
	
	y -= lpMListBox->nViewTopY;
	for( i = 0; y < h && i < itm_cnt; i++ ) {
		if(i < itm_cnt) 
			r = atMLISTBOX_GetItem(hControl, i, &Item);
		else 
			r = atFALSE;
// 		if( r && (Item.nItemID > 20 || Item.nItemID < -1) ){
// 			if(i < itm_cnt) r = atMLISTBOX_GetItem(hControl, i, &Item); else r = atFALSE;
// 			continue;
// 		}
		hh = get_height( hControl, lpMListBox, i, lpMListBox->lpFuncParam );
		if( y + hh >= 0 ){
			// fix
			bSelected = (i == lpMListBox->nSelectIndex);
			if(bSelected) {
				if( (lpMListBox->dwAttr&atMLISTBOX_ATTR_CLICK) == atMLISTBOX_ATTR_CLICK )
					if( !lpMListBox->bMouseDown )
						bSelected = atFALSE;
			}
			if( r ){
				SET_RECT( clip, x, y, x+w-1, y+hh-1 );
				atUIAPI_SetClipRect( hCanvas, &clip );
				draw_item_bg( hControl, lpMListBox, hCanvas, i, x, y, w, hh, &Item, bSelected, bFocused, lpMListBox->lpFuncParam );
				iy = y + lpMListBox->nHeightMargin;
				ih = hh - lpMListBox->nHeightMargin * 2;
				SET_RECT( clip, ix, iy, ix+iw-1, iy+ih-1 );
				draw_item( hControl, lpMListBox, hCanvas, i, ix, iy, iw, ih, lpMListBox->bFirstNO, lpMListBox->nCheckType, &Item, bSelected, bFocused, lpMListBox->lpFuncParam );
			}
		}
		y += hh;
		if( i < itm_cnt-1 ) y += lpMListBox->nItemGap;
	}
}

static void _SetScrollPos( atHCONTROL hControl, int scroll_pos, atBOOL is_change_scrollbar )
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr(hControl);

	if( atCONTROL_IsVisible( lpMListBox->hScrollBar ) ) {
//		if( scroll_pos < 0 ) scroll_pos = 0;
//		if( scroll_pos >= atSCROLLBAR_EXT_GetSize(lpMListBox->hScrollBar) ) 
//			scroll_pos = atSCROLLBAR_EXT_GetSize(lpMListBox->hScrollBar) - 1;

		atSCROLLBAR_EXT_SetPosition( lpMListBox->hScrollBar, scroll_pos );	
		lpMListBox->nViewTopY = scroll_pos;
		lpMListBox->bIsChange = atTRUE;
		_Realloc_Items( hControl );
	} else {
		lpMListBox->nViewTopY = 0;
	}
}

static void _Realloc_Items( atHCONTROL hControl )
{
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr(hControl);
	atLPTSTR szSelStr;
	atHBITMAP hBm;
	atRECT rect;
	atCONTROL_COLOR cl;

	int i;
	int all_item_height;
	int itm_y, itm_height, view_y;

	int xxx, yyy;
	int nHeight;
	int cnt;
	int show_sb = 0;
	atBOOL active_ticker = atFALSE;
	atBOOL active_settext = atFALSE;
	int nTempWidth;
	int vv1, vv2;

	if( !lpMListBox->bIsChange ) return;

	atCONTROL_GetControlRect( hControl, &rect );
	nHeight = RECT_HEIGHT(lpControl->Base.rect);
//	lpMListBox->nOneScreenCount	= (lpMListBox->nItemHeight != 0) ? nHeight / lpMListBox->nItemHeight : 0;		

	cnt = atMLISTBOX_GetCount(hControl);
//	if( (cnt <= 0) || (lpMListBox->nOneScreenCount <= 0) ) {
	if( (cnt <= 0) ) {
		if( lpMListBox->hScrollBar != atNULL )
			atCONTROL_SetVisible( lpMListBox->hScrollBar, atFALSE );

		for(i = 0 ; i < lpMListBox->nLineCnt ; i++) {
			if( lpMListBox->hTickerBox[i] != atNULL )
				atCONTROL_SetVisible( lpMListBox->hTickerBox[i], atFALSE );

			lpMListBox->bTickerActivate[i] = atFALSE;
		}
		return;
	}

	itm_y = -1;
	itm_height = 0;
	all_item_height = _GetTotalItemHeight( hControl, lpMListBox, cnt, &itm_y, &itm_height );

	if( lpMListBox->bScrollVisible ) {
		if( all_item_height > nHeight ){
//		if( cnt * lpMListBox->nItemHeight > nHeight ) {
 			if( !atCONTROL_IsVisible( lpMListBox->hScrollBar ) ) {
 				atCONTROL_SetVisible( lpMListBox->hScrollBar, atTRUE );
 			}

//			atSCROLLBAR_EXT_SetSize( lpMListBox->hScrollBar, all_item_height - nHeight );
			atSCROLLBAR_EXT_SetSize( lpMListBox->hScrollBar, all_item_height );
			show_sb = 1;
		} else {
			if( lpMListBox->hScrollBar != atNULL ){
				atCONTROL_SetVisible( lpMListBox->hScrollBar, atFALSE );
			}
		
			show_sb = 0;
			_SetScrollPos( hControl, 0, atTRUE );
		}
	}

	if( lpMListBox->bTickerEnable ){
		int ChkWidth, BmWidth, SbWidth, NoWidth;
		atTCHAR szBuff[16];
		
		if( (lpMListBox->nCheckType == 1) || (lpMListBox->nCheckType == 2) ) {
			atHBITMAP lpBM = lpMListBox->lpUncheckedImage;
			ChkWidth = (lpBM) ? atUIAPI_GetBitmapWidth(lpBM) : 16;
			if( ChkWidth > 0 ) ChkWidth += lpMListBox->nItemGap;
		} else {
			ChkWidth = 0;
		}

		if( show_sb ) {
			atRECT rect;
			atCONTROL_GetControlRect( lpMListBox->hScrollBar, &rect);
			SbWidth = RECT_WIDTH(rect);
		} else {
			SbWidth = 0;
		}

		for(i = 0 ; i < lpMListBox->nLineCnt ; i++) {
			if( lpMListBox->nSelectIndex >= 0 ){
				atMLISTBOX_ITEM itm;	
				atBOOL r = atMLISTBOX_GetSelectItem(hControl, &itm);
				
				if( lpMListBox->bFirstNO ) {
					UI_SPRINTF( szBuff, _AT("%d."), itm.nNO );
					NoWidth = atUIAPI_GetStringWidth(atCONTROL_GetFont(hControl), szBuff, -1);
					if( NoWidth > 0 ) NoWidth += lpMListBox->nItemGap;
				} else {
					NoWidth = 0;
				}

				szSelStr = itm.szItemName[i][1];
				if( szSelStr && UI_STRLEN(szSelStr) <= 0 ) szSelStr = itm.szItemName[i][0];
				hBm = itm.imgItemImage[1];
				if( hBm ) {
					BmWidth = (itm.imgItemImageRegion[1].nWidth>0) ? itm.imgItemImageRegion[1].nWidth : atUIAPI_GetBitmapWidth(hBm);
				} else {
					hBm = itm.imgItemImage[0];
					if( hBm )	BmWidth = (itm.imgItemImageRegion[0].nWidth>0) ? itm.imgItemImageRegion[0].nWidth : atUIAPI_GetBitmapWidth(hBm);
					else		BmWidth = 0;
				}

				if( BmWidth > 0 ) BmWidth += lpMListBox->nItemGap;

				active_settext = atTICKERBOX_SetFontSize(lpMListBox->hTickerBox[i], itm.nFontSize[i]);
				active_ticker = atFALSE;

				atTICKERBOX_SetColorHighLight( lpMListBox->hTickerBox[i], itm.clHLColor);
				atUIAPI_memset(itm.bWordHL, 0x00, sizeof(itm.bWordHL));
				if(atUIAPI_strlen(itm.szHighLight) > 0)
				{
					_MakeHLList(szSelStr, itm.szHighLight, itm.bWordHL, !itm.bNotUseFilter, 
								itm.nStartHLIdx, itm.nEndHLIdx);
					atTICKERBOX_SetHLWordAtrribute( lpMListBox->hTickerBox[i], itm.bWordHL, sizeof(itm.bWordHL));
				}

				atCONTROL_SetControlRect(lpMListBox->hTickerBox[i], 
										 0, lpMListBox->rtControlRect[i].nStartY, 
										 0, lpMListBox->rtControlRect[i].nEndY);

				//xxx = RECT_WIDTH(lpControl->Base.rect) - ChkWidth - BmWidth - SbWidth - NoWidth - lpMListBox->nWidthMargin*2;
				nTempWidth = RECT_WIDTH(lpControl->Base.rect) - ChkWidth - BmWidth - SbWidth - NoWidth - lpMListBox->nWidthMargin*2;
				// Kyle Start [Modify 2009/08/03-14:51]
				if(lpMListBox->nMostAlign[i] == atMLISTBOX_MOSTALIGN_LEFT)			
				{
					if(lpMListBox->nWidthPercent[i] <= 0)			xxx = 0;
					else if(lpMListBox->nWidthPercent[i] >= 100)	xxx = nTempWidth;
					else
					{
						vv1 = (int)(((atINT64)100 << 32) / ((100-lpMListBox->nWidthPercent[i]) << 16));
						vv2 = (int)(((atINT64)nTempWidth << 32) / vv1);
						xxx = nTempWidth - (vv2 >> 16);
						//xxx = nTempWidth - (nTempWidth / (100 / (100-lpMListBox->nWidthPercent[i]))); // Percent
					}
				}
				else if(lpMListBox->nMostAlign[i] == atMLISTBOX_MOSTALIGN_RIGHT)	
				{
					if(lpMListBox->nWidthPercent[i] <= 0)			xxx = 0;
					else if(lpMListBox->nWidthPercent[i] >= 100)	xxx = nTempWidth;
					else
					{
						vv1 = (int)(((atINT64)100 << 32) / (lpMListBox->nWidthPercent[i] << 16));
						vv2 = (int)(((atINT64)nTempWidth << 32) / vv1);
						xxx = (vv2 >> 16);
						//xxx = nTempWidth / (100 / lpMListBox->nWidthPercent[i]);	// Percent
					}
				}
				else
				{
					xxx = nTempWidth;
				}
				// Kyle [Modify 2009/08/03-14:51] End

				//yyy = atUIAPI_GetStringWidth(atCONTROL_GetFont(hControl), szSelStr, -1);
				yyy = atUIAPI_GetStringWidth(atCONTROL_GetFont(lpMListBox->hTickerBox[i]), szSelStr, -1);
				if( yyy >= xxx ) {
					view_y = itm_y;//lpMListBox->nSelectIndex * lpMListBox->nItemHeight - lpMListBox->nViewTopY;
					//view_y += (lpMListBox->nItemHeight/lpMListBox->nLineCnt)*i;
					view_y += lpMListBox->rtControlRect[i].nStartY;
					if( (view_y + itm_height >= 0) && (view_y < nHeight) ) {
						active_ticker = atTRUE;
					}
				}
				if( active_ticker ) {
					int StrWidth = RECT_WIDTH(lpControl->Base.rect) - ChkWidth - BmWidth - SbWidth - NoWidth - lpMListBox->nWidthMargin*2;
					if( active_settext || atUIAPI_strcmp( atTICKERBOX_GetText(lpMListBox->hTickerBox[i]), szSelStr ) != 0) {
						atTICKERBOX_SetText( lpMListBox->hTickerBox[i], szSelStr, -1 );
					}

					if(lpMListBox->nMostAlign[i] == atMLISTBOX_MOSTALIGN_RIGHT) {
						lpMListBox->rtControlRect[i].nStartX = nTempWidth - xxx;
						lpMListBox->rtControlRect[i].nEndX = 0;
					} else if(lpMListBox->nMostAlign[i] == atMLISTBOX_MOSTALIGN_LEFT) {
						lpMListBox->rtControlRect[i].nStartX = 0;
						// Kyle Start [Modify 2009/08/03-14:51]
						lpMListBox->rtControlRect[i].nEndX = nTempWidth - xxx/* + lpMListBox->nWidthMargin*/;
						// Kyle [Modify 2009/08/03-14:51] End
					} else {
						lpMListBox->rtControlRect[i].nStartX = lpMListBox->rtControlRect[i].nEndX = 0;
					}

					atCONTROL_SetAnchor( lpMListBox->hTickerBox[i], view_y, atANCHOR_IGNORE, 
										 lpMListBox->nWidthMargin + ChkWidth + NoWidth + BmWidth + lpMListBox->rtControlRect[i].nStartX, 
										 SbWidth + lpMListBox->rtControlRect[i].nEndX);
				
					if( !lpMListBox->bTickerActivate[i] ) {
						//atCONTROL_SetFont( lpMListBox->hTickerBox[i], atCONTROL_GetFont(hControl) );
						atCONTROL_GetControlColor( hControl, &cl );
						cl.clNormalTxtColor = (itm.clItemColor[i][1] != 0xFFFFFFFF) ? itm.clItemColor[i][1] : cl.clFocusedTxtColor;
						cl.clNormalBgColor = cl.clFocusedBgColor;
						atCONTROL_SetControlColor( lpMListBox->hTickerBox[i], &cl );
						if( !atCONTROL_IsVisible(lpMListBox->hTickerBox[i]) )
							atCONTROL_SetVisible( lpMListBox->hTickerBox[i], atTRUE );
						if( atCONTROL_IsFocus(hControl) )
							atTICKERBOX_StartTicker( lpMListBox->hTickerBox[i] );
						lpMListBox->bTickerActivate[i] = atTRUE;
					}
				} else {
					atCONTROL_SetVisible(lpMListBox->hTickerBox[i], atFALSE );
					lpMListBox->bTickerActivate[i] = atFALSE;
				}
			} else {
				atCONTROL_SetVisible(lpMListBox->hTickerBox[i], atFALSE );
				lpMListBox->bTickerActivate[i] = atFALSE;
			}
		}
	}

	atCONTROL_AdjustAllChildControls(hControl);
	lpMListBox->bIsChange = atFALSE;
}

static int atMLISTBOX_OnKeyDown( atHCONTROL hControl, atINT nKeyCode )
{
	int r = 0;
	atMLISTBOX_ITEM itm;
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return 0;

	switch( nKeyCode ) {
		case atVKEY_UP:
		case atVKEY_REW:
			if( lpMListBox->nSelectIndex <= 0 ) {
				if( lpMListBox->dwAttr & atMLISTBOX_ATTR_MOVE_NEXTCONTROL ) {
					return 0;
				}
			}
			return 1;
			break;
		case atVKEY_DOWN:
		case atVKEY_FF:
			if( lpMListBox->nSelectIndex >= atMLISTBOX_GetCount(hControl)-1 ) {
				if( lpMListBox->dwAttr & atMLISTBOX_ATTR_MOVE_NEXTCONTROL ) {
					return 0;
				}
			}
			return 1;
			break;
	}

	if( nKeyCode == lpMListBox->SelectKey ) {
		if( lpMListBox->nSelectIndex >= 0 ) {
			if( lpMListBox->nCheckType == 2 ) {
				if( atMLISTBOX_GetSelectItem(hControl, &itm) ) {
					atMLISTBOX_SetItemCheck(hControl, atMLISTBOX_GetSelectIndex(hControl), atTRUE);
					if( lpMListBox->CheckKey == atVKEY_SOFT1 )
						atCONTROL_SetSoftKey(hControl, _AT(""), atNULL, atNULL);
					else if( lpMListBox->CheckKey == atVKEY_SOFT2 )
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
					atSOFTKEY_RefreshSoftKey();
				}
				atCONTROL_Refresh( hControl );
			}
			if( atMLISTBOX_GetSelectItem(hControl, &itm) ){
				if( itm.nItemID != -1 ) {
					atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)atCONTROL_GetID(hControl), (long)itm.nItemID );
				} else {
					atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND, (long)atCONTROL_GetID(hControl), (long)lpMListBox->nSelectIndex );
				}
			}
			r = 1;
		}
	} else if( nKeyCode == lpMListBox->CheckKey ) {
		if( lpMListBox->nCheckType == 1 ) {
			if( atMLISTBOX_GetSelectItem(hControl, &itm) ) {
				atBOOL bChecked = atMLISTBOX_ToggleSelectItemChecked(hControl);
				if( bChecked ) {
					if( lpMListBox->CheckKey == atVKEY_SOFT1 )
						atCONTROL_SetSoftKey(hControl, _AT("UnChk"), atNULL, atNULL);
					else if( lpMListBox->CheckKey == atVKEY_SOFT2 )
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("UnChk"));
					else if( lpMListBox->CheckKey == atVKEY_SELECT )
						atCONTROL_SetSoftKey(hControl, atNULL, _AT("UnChk"), atNULL);
				} else {
					if( lpMListBox->CheckKey == atVKEY_SOFT1 )
						atCONTROL_SetSoftKey(hControl, _AT("Check"), atNULL, atNULL);
					else if( lpMListBox->CheckKey == atVKEY_SOFT2 )
						atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
					else if( lpMListBox->CheckKey == atVKEY_SELECT )
						atCONTROL_SetSoftKey(hControl, atNULL, _AT("Check"), atNULL);
				}
				atCONTROL_Refresh( hControl );
				atSOFTKEY_RefreshSoftKey();
				r = 1;
			}
		} else if( lpMListBox->nCheckType == 2 ) {
			if( atMLISTBOX_GetSelectItem(hControl, &itm) ) {
				atMLISTBOX_SetItemCheck(hControl, atMLISTBOX_GetSelectIndex(hControl), atTRUE);
				if( lpMListBox->CheckKey == atVKEY_SOFT1 )
					atCONTROL_SetSoftKey(hControl, _AT(""), atNULL, atNULL);
				else if( lpMListBox->CheckKey == atVKEY_SOFT2 )
					atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
				else if( lpMListBox->CheckKey == atVKEY_SELECT )
					atCONTROL_SetSoftKey(hControl, atNULL, _AT(""), atNULL);
				atSOFTKEY_RefreshSoftKey();
				r = 1;
			}
		}
	}

	return r;
}

static int atMLISTBOX_OnKeyUp( atHCONTROL hControl, atINT nKeyCode )
{
	return 0;
}

static int atMLISTBOX_OnKeyPress( atHCONTROL hControl, atINT nKeyCode )
{
	int r = 0;
	atMLISTBOX_ITEM itm;
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return 0;

	switch( nKeyCode ) {
		case atVKEY_UP:
		case atVKEY_REW:
			atMLISTBOX_MoveSelect(hControl, -1);
			if( lpMListBox->nCheckType == 1 ) {
				if( atMLISTBOX_GetSelectItem(hControl, &itm) ) {
					if( itm.bChecked ) {
						if( lpMListBox->CheckKey == atVKEY_SOFT1 )
							atCONTROL_SetSoftKey(hControl, _AT("UnChk"), atNULL, atNULL);
						else if( lpMListBox->CheckKey == atVKEY_SOFT2 )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("UnChk"));
						else if( lpMListBox->CheckKey == atVKEY_SELECT )
							atCONTROL_SetSoftKey(hControl, atNULL, _AT("UnChk"), atNULL);
					} else {
						if( lpMListBox->CheckKey == atVKEY_SOFT1 )
							atCONTROL_SetSoftKey(hControl, _AT("Check"), atNULL, atNULL);
						else if( lpMListBox->CheckKey == atVKEY_SOFT2 )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
						else if( lpMListBox->CheckKey == atVKEY_SELECT )
							atCONTROL_SetSoftKey(hControl, atNULL, _AT("Check"), atNULL);
					}
					atSOFTKEY_RefreshSoftKey();
				}
			} else if( lpMListBox->nCheckType == 2 ) {
				if( atMLISTBOX_GetSelectItem(hControl, &itm) ) {
					if( itm.bChecked ) {
						if( lpMListBox->CheckKey == atVKEY_SOFT1 )
							atCONTROL_SetSoftKey(hControl, _AT(""), atNULL, atNULL);
						else if( lpMListBox->CheckKey == atVKEY_SOFT2 )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
						else if( lpMListBox->CheckKey == atVKEY_SELECT )
							atCONTROL_SetSoftKey(hControl, atNULL, _AT(""), atNULL);
					} else {
						if( lpMListBox->CheckKey == atVKEY_SOFT1 )
							atCONTROL_SetSoftKey(hControl, _AT("Select"), atNULL, atNULL);
						else if( lpMListBox->CheckKey == atVKEY_SOFT2 )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Select"));
						else if( lpMListBox->CheckKey == atVKEY_SELECT )
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
			atMLISTBOX_MoveSelect(hControl, 1);
			if( lpMListBox->nCheckType == 1 ) {
				if( atMLISTBOX_GetSelectItem(hControl, &itm) ) {
					if( itm.bChecked ) {
						if( lpMListBox->CheckKey == atVKEY_SOFT1 )
							atCONTROL_SetSoftKey(hControl, _AT("UnChk"), atNULL, atNULL);
						else if( lpMListBox->CheckKey == atVKEY_SOFT2 )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("UnChk"));
						else if( lpMListBox->CheckKey == atVKEY_SELECT )
							atCONTROL_SetSoftKey(hControl, atNULL, _AT("UnChk"), atNULL);
					} else {
						if( lpMListBox->CheckKey == atVKEY_SOFT1 )
							atCONTROL_SetSoftKey(hControl, _AT("Check"), atNULL, atNULL);
						else if( lpMListBox->CheckKey == atVKEY_SOFT2 )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
						else if( lpMListBox->CheckKey == atVKEY_SELECT )
							atCONTROL_SetSoftKey(hControl, atNULL, _AT("Check"), atNULL);
					}
					atSOFTKEY_RefreshSoftKey();
				}
			} else if( lpMListBox->nCheckType == 2 ) {
				if( atMLISTBOX_GetSelectItem(hControl, &itm) ) {
					if( itm.bChecked ) {
						if( lpMListBox->CheckKey == atVKEY_SOFT1 )
							atCONTROL_SetSoftKey(hControl, _AT(""), atNULL, atNULL);
						else if( lpMListBox->CheckKey == atVKEY_SOFT2 )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
						else if( lpMListBox->CheckKey == atVKEY_SELECT )
							atCONTROL_SetSoftKey(hControl, atNULL, _AT(""), atNULL);
					} else {
						if( lpMListBox->CheckKey == atVKEY_SOFT1 )
							atCONTROL_SetSoftKey(hControl, _AT("Select"), atNULL, atNULL);
						else if( lpMListBox->CheckKey == atVKEY_SOFT2 )
							atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Select"));
						else if( lpMListBox->CheckKey == atVKEY_SELECT )
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

static int atMLISTBOX_OnMouseDown( atHCONTROL hControl, atINT nX, atINT nY )
{
	int r = 0;
	int itm_cnt;	
	int nFirstIdx, nFirstY, nItmWidth, nItmHeight;
	atMLISTBOX_TOUCHITEM_FUNC *touch_item;
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return 0;

	itm_cnt = atMLISTBOX_GetCount(hControl);
	if( itm_cnt <= 0 ) return r;

	lpMListBox->bReadyClick = atTRUE;

	nItmWidth = RECT_WIDTH(lpControl->Base.rect);

	if( atCONTROL_IsVisible(lpMListBox->hScrollBar) ) {
//		atRECT rect;
//		atCONTROL_GetControlRect(lpMListBox->hScrollBar, &rect);
//		if( nX >= rect.nStartX && nX <= rect.nEndX && nY >= rect.nStartY && nY <= rect.nEndY ) {
//			return r;
//		}
//		nItmWidth -= RECT_WIDTH(rect);
		atREGION rgn;
		atCONTROL_GetControlRegion(lpMListBox->hScrollBar, &rgn);
		nItmWidth -= rgn.nWidth;
		if( atSCROLLBAR_EXT_IsProcessGestureScroll(lpMListBox->hScrollBar) ){
			lpMListBox->bReadyClick = atFALSE;
		}
	}

	if( lpMListBox->bReadyClick ){
		touch_item = (lpMListBox->fnFuncs.fnTouchItem)? lpMListBox->fnFuncs.fnTouchItem : _defTouchItem;

		nFirstIdx = _FindFirstIndex( hControl, lpMListBox, nY, itm_cnt, &nFirstY, &nItmHeight );
		if( nFirstIdx < 0 ) return r;
			
		lpMListBox->bActionValidate = atMLISTBOX_SetSelectIndex( hControl, nFirstIdx );
		if( lpMListBox->bActionValidate ) {
			atCONTROL_Refresh( hControl );
		}else {
			lpMListBox->nSelectIndex = -1;
		}
	}
	
	lpMListBox->nFirstMouseX = lpMListBox->nPrevMouseX = nX;
	lpMListBox->nFirstMouseY = lpMListBox->nPrevMouseY = nY;
	lpMListBox->nFirstViewTopY = lpMListBox->nViewTopY;
	lpMListBox->bMouseDown = atTRUE;
	lpMListBox->bMouseScroll = atFALSE;

	if( atCONTROL_IsVisible(lpMListBox->hScrollBar) ){
		if( atSCROLLBAR_EXT_IsProcessGestureScroll(lpMListBox->hScrollBar) ){
			atSCROLLBAR_EXT_StopAndResetGestureScroll(lpMListBox->hScrollBar);
//			lpMListBox->bReadySmoothScroll = atFALSE;
			lpMListBox->bReadySmoothScroll = atTRUE;
			lpMListBox->bReadyClick = atFALSE;
		} else {
			lpMListBox->bReadySmoothScroll = atTRUE;
		}
	}
	
	if( lpMListBox->bReadyClick ){
		touch_item( hControl, lpMListBox, nFirstIdx, 0, nX, nFirstY, nItmWidth, nItmHeight, lpMListBox->lpFuncParam );
	}

	return r;
}

static int atMLISTBOX_OnMouseMove( atHCONTROL hControl, atINT nX, atINT nY )
{
//	int i;
	int r = 0;
//	atLPMLISTBOX_ITEM lpItem;
	static int limit_y = -1;
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return 0;

	if( !lpMListBox->bReadySmoothScroll ) return 0;
	
	if( limit_y < 0 ){
		atUIAPI_GetScreenDPI( atNULL, &limit_y );
		limit_y = (int)(limit_y * CHECK_POINT_MOVE_LIMIT);
	}
		
	if( lpMListBox->bMouseDown ) {
		if( lpMListBox->bUseSmoothScroll ) {
			if( !lpMListBox->bMouseScroll ) {
				if( ABS(lpMListBox->nFirstMouseY - nY) > limit_y ) {
					atMLISTBOX_SetSelectIndex( hControl, -1/*lpMListBox->nPrevSelectIndex*/ );
					lpMListBox->bMouseScroll = atTRUE;
					atSCROLLBAR_EXT_StartGestureScroll( lpMListBox->hScrollBar, lpMListBox->nFirstMouseY );
			//		atSCROLLBAR_EXT_ProcessGestureScroll( lpMListBox->hScrollBar, nY );
					lpMListBox->bReadyClick = atFALSE;			
				}
			} 
			if( lpMListBox->bMouseScroll ) {
				atSCROLLBAR_EXT_ProcessGestureScroll( lpMListBox->hScrollBar, nY );
			}
		}

		lpMListBox->nPrevMouseX = nX;
		lpMListBox->nPrevMouseY = nY;
	} 

	return r;
}

static int atMLISTBOX_OnMouseUp( atHCONTROL hControl, atINT nX, atINT nY )
{
//	int i;
	int r = 0;
	atMLISTBOX_ITEM itm;
//	atRECT rect;
	atMLISTBOX_TOUCHITEM_FUNC *touch_item;
	int nFirstIdx;
	int itm_cnt;
	int nFirstY, nItmWidth, nItmHeight;
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( !lpMListBox || !lpControl ) return 0;

	nItmWidth = RECT_WIDTH(lpControl->Base.rect);
	
	if( atCONTROL_IsVisible(lpMListBox->hScrollBar) )	{
		atRECT rect;

		if( !lpMListBox->bReadySmoothScroll ) return 0;
	
		if( lpMListBox->bMouseScroll ) {
			atSCROLLBAR_EXT_EndGestureScroll( lpMListBox->hScrollBar, nY );
			lpMListBox->bMouseScroll = atFALSE;
		}
		
		atCONTROL_GetControlRect(lpMListBox->hScrollBar, &rect);
		if( !atUIAPI_IsPtInRect(nX, nY, &rect) )
			nItmWidth -= RECT_WIDTH(rect);
	}

	touch_item = (lpMListBox->fnFuncs.fnTouchItem)? lpMListBox->fnFuncs.fnTouchItem : _defTouchItem;
	
	if( lpMListBox->bReadyClick ){
		if( lpMListBox->bActionValidate ) {
			lpMListBox->bActionValidate = atFALSE;

			if( atMLISTBOX_GetSelectItem(hControl, &itm) ){
				if( atUIAPI_IsPtInRect( nX, nY, &(itm.imgItemDrawRect[0])) )
					itm.nImgSelectIdx = 0;
				else if( atUIAPI_IsPtInRect( nX, nY, &(itm.imgItemDrawRect[1])) )
					itm.nImgSelectIdx = 1;
				else
					itm.nImgSelectIdx = -1;

				// fix
				itm_cnt = atMLISTBOX_GetCount( hControl );
				nFirstIdx = _FindFirstIndex( hControl, lpMListBox, nY, itm_cnt, &nFirstY, &nItmHeight );

				if(nFirstIdx == atMLISTBOX_GetSelectIndex(hControl))
				{
					touch_item(hControl, lpMListBox, nFirstIdx, 1, nX, nFirstY, nItmWidth, nItmHeight, lpMListBox->lpFuncParam);

					atMLISTBOX_SetSelectItem(hControl, &itm);
					atCONTROL_ProcessControlProc( hControl, CTLMSG_KEYDOWN, atVKEY_SELECT, 0 );
				}
				else
				{
					atMLISTBOX_SetSelectIndex(hControl, -1);
					// 아래 코드 중복되는거 같으면 빼자
					atCONTROL_Refresh(hControl);
				}
				
			}
			
	//		touch_item( hControl, lpMListBox, nFirstIdx, 1, nX, nY, lpMListBox->lpFuncParam );
		}
	}

	lpMListBox->bMouseDown = atFALSE;
	r = 1;

	return r;
}

static int atMLISTBOX_OnMouseLongClick( atHCONTROL hControl, atINT nX, atINT nY )
{
	int r = 0;
	int itm_cnt;	
	int nFirstIdx;
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return 0;

	itm_cnt = atMLISTBOX_GetCount(hControl);
	if( itm_cnt <= 0 ) return 1;

	if( atCONTROL_IsVisible(lpMListBox->hScrollBar) ) {
		atRECT rect;
		atCONTROL_GetControlRect(lpMListBox->hScrollBar, &rect);
		if( nX >= rect.nStartX && nX <= rect.nEndX && nY >= rect.nStartY && nY <= rect.nEndY ) {
			return 0;
		}
	}

	nFirstIdx = _FindFirstIndex( hControl, lpMListBox, nY, itm_cnt, atNULL, atNULL );
	if( nFirstIdx < 0 ) return r;

	lpMListBox->bActionValidate = atFALSE;
	atCONTROL_PostMessageToParent( hControl, WNDMSG_COMMAND_EX, (long)atCONTROL_GetID(hControl), (long)nFirstIdx );
	
	return r;	
}

static int atMLISTBOX_OnGotFocus( atHCONTROL hControl )
{
	int i;
	int r = 0;
	atMLISTBOX_ITEM itm;
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return 0;
	
	for(i = 0 ; i < lpMListBox->nLineCnt ; i++) {
		if(lpMListBox->bTickerActivate[i]) {
			atCONTROL_SetVisible( lpMListBox->hTickerBox[i], atTRUE );
			atTICKERBOX_StartTicker( lpMListBox->hTickerBox[i] );
		}
	}

	if( lpMListBox->nCheckType == 1 ) {
		if( atMLISTBOX_GetSelectItem(hControl, &itm) ) {
			if( itm.bChecked ) {
				if( lpMListBox->CheckKey == atVKEY_SOFT1 )
					atCONTROL_SetSoftKey(hControl, _AT("UnChk"), atNULL, atNULL);
				else if( lpMListBox->CheckKey == atVKEY_SOFT2 )
					atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("UnChk"));
				else if( lpMListBox->CheckKey == atVKEY_SELECT )
					atCONTROL_SetSoftKey(hControl, atNULL, _AT("UnChk"), atNULL);
			} else {
				if( lpMListBox->CheckKey == atVKEY_SOFT1 )
					atCONTROL_SetSoftKey(hControl, _AT("Check"), atNULL, atNULL);
				else if( lpMListBox->CheckKey == atVKEY_SOFT2 )
					atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Check"));
				else if( lpMListBox->CheckKey == atVKEY_SELECT )
					atCONTROL_SetSoftKey(hControl, atNULL, _AT("Check"), atNULL);
			}
			atSOFTKEY_RefreshSoftKey();
			r = 1;
		}
	} else if( lpMListBox->nCheckType == 2 ) {
		if( atMLISTBOX_GetSelectItem(hControl, &itm) ) {
			if( itm.bChecked ) {
				if( lpMListBox->CheckKey == atVKEY_SOFT1 )
					atCONTROL_SetSoftKey(hControl, _AT(""), atNULL, atNULL);
				else if( lpMListBox->CheckKey == atVKEY_SOFT2 )
					atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
				else if( lpMListBox->CheckKey == atVKEY_SELECT )
					atCONTROL_SetSoftKey(hControl, atNULL, _AT(""), atNULL);
			} else {
				if( lpMListBox->CheckKey == atVKEY_SOFT1 )
					atCONTROL_SetSoftKey(hControl, _AT("Select"), atNULL, atNULL);
				else if( lpMListBox->CheckKey == atVKEY_SOFT2 )
					atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("Select"));
				else if( lpMListBox->CheckKey == atVKEY_SELECT )
					atCONTROL_SetSoftKey(hControl, atNULL, _AT("Select"), atNULL);
			}
			atSOFTKEY_RefreshSoftKey();
			r = 1;
		}
	}

	return r;
}

static int atMLISTBOX_OnLostFocus( atHCONTROL hControl )
{
	int i;
	int r = 0;
//	atLPMLISTBOX_ITEM lpItem;
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return 0;
	
	for(i = 0 ; i < lpMListBox->nLineCnt ; i++) {
		if(lpMListBox->bTickerActivate[i]) {
			atTICKERBOX_StopTicker( lpMListBox->hTickerBox[i] );
			atCONTROL_SetVisible( lpMListBox->hTickerBox[i], atFALSE );
		}
	}

	if( lpMListBox->nCheckType == 1 ) {
		r = 1;
	}

	return r;
}

static int MListBoxProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 )
{
	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
//	atLPMLISTBOX_ITEM lpItem;
	atCONTROL_COLOR cl;	
//	atRECT rect;
	int x, y;
//	int nFirstIdx;
	int r = 0, r2 = 0;

	switch( nMsg )
	{
		case CTLMSG_INIT:
			atMLISTBOX_OnInit( hControl, (struct _CreateParam *)lParam1 );
			r = 1;
			break;
		case CTLMSG_RELEASE:
			atMLISTBOX_OnRelease( hControl );
			r = 1;
			break;
		case CTLMSG_CHANGE:
			if( lParam1 == 100 ) {	// CMP_ScrollBarEx
				_SetScrollPos( hControl, lParam2, atFALSE );
				atCONTROL_Refresh( hControl );
				r = 1;
			}
			break;
		case CTLMSG_SHOW:
		case CTLMSG_RESIZE:
			if( lpMListBox->bScrollVisible )
				_SetScrollBarPosition( hControl, lpMListBox );

			lpMListBox->bIsChange = atTRUE;
			_Realloc_Items(hControl);
			r = 0;
			break;
		case CTLMSG_PAINT:
			atMLISTBOX_OnDraw( atCONTROL_GetGDC(hControl), hControl, (atBOOL)lParam2 );
			r = 1;
			break;
		case CTLMSG_GOTFOCUS:
			r = atMLISTBOX_OnGotFocus( hControl );
			break;
		case CTLMSG_LOSTFOCUS:
			r = atMLISTBOX_OnLostFocus( hControl );
			break;
		case CTLMSG_KEYDOWN:
			r = atMLISTBOX_OnKeyDown( hControl, lParam1 );
			break;
		case CTLMSG_KEYPRESS:
			r = atMLISTBOX_OnKeyPress( hControl, lParam1 );
			break;
		case CTLMSG_KEYUP:
			r = atMLISTBOX_OnKeyUp( hControl, lParam1 );
			break;
		case CTLMSG_MOUSEDOWN:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			r = atMLISTBOX_OnMouseDown( hControl, x, y );
			break;
		case CTLMSG_MOUSEMOVE:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			r = atMLISTBOX_OnMouseMove( hControl, x, y );
			break;
		case CTLMSG_MOUSEUP:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			r = atMLISTBOX_OnMouseUp( hControl, x, y );
			break;
		case CTLMSG_MOUSELONGCLICK:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			r = atMLISTBOX_OnMouseLongClick( hControl, x, y );
			break;
		case CTLMSG_CHANGELAYOUT:
			atCONTROL_GetControlColor( hControl, &cl );
			atCONTROL_SetControlColor( lpMListBox->hScrollBar, &cl );
			break;
	}

	return r;
}

atDWORD atMLISTBOX_GetAttr(atHCONTROL hControl )
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return 0;

	return lpMListBox->dwAttr;
}

void atMLISTBOX_SetAttr( atHCONTROL hControl, atDWORD dwAttr )
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return;

	lpMListBox->dwAttr = dwAttr;
}

void atMLISTBOX_SetBitmap( atHCONTROL hControl,
						   atHBITMAP hImgNormalLeft, atHBITMAP hImgNormalCenter, atHBITMAP hImgNormalRight,
						   atHBITMAP hImgSelectLeft, atHBITMAP hImgSelectCenter, atHBITMAP hImgSelectRight,
						   atHBITMAP hImgDisableLeft, atHBITMAP hImgDisableCenter, atHBITMAP hImgDisableRight )
{
	int i;

	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return;

	lpMListBox->hImgNormalLeft = hImgNormalLeft;
	lpMListBox->hImgNormalCenter = hImgNormalCenter;
	lpMListBox->hImgNormalRight = hImgNormalRight;
	lpMListBox->hImgSelectLeft = hImgSelectLeft;
	lpMListBox->hImgSelectCenter = hImgSelectCenter;
	lpMListBox->hImgSelectRight = hImgSelectRight;
	lpMListBox->hImgDisableLeft = hImgDisableLeft;
	lpMListBox->hImgDisableCenter = hImgDisableCenter;
	lpMListBox->hImgDisableRight = hImgDisableRight;

	// Kyle Start [Modify 2009/08/13-15:51]
	// 활성화된 복수개의 TickerBox에 BGBitmap을 설정하지 않아 생기는 Access Violation 을 수정
	for(i = 0 ; i < lpMListBox->nLineCnt ; i++)
		atTICKERBOX_SetBGBitmap(lpMListBox->hTickerBox[i], hImgSelectCenter, hImgSelectCenter, hImgSelectRight);
	// Kyle [Modify 2009/08/13-15:51] End

	lpMListBox->bIsChange = atTRUE;
}

void atMLISTBOX_ClearAllItem( atHCONTROL hControl )
{
	int i;

	atLPCONTROL lpControl = atCONTROL_GetPtr(hControl);
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );

	atUIAPI_RemoveListAll(lpMListBox->lpListBoxList, atTRUE);

	lpMListBox->nSelectIndex = -1;
	atCONTROL_SetVisible( lpMListBox->hScrollBar, atFALSE );

	for(i = 0 ; i < lpMListBox->nLineCnt ; i++) {
		atCONTROL_SetVisible( lpMListBox->hTickerBox[i], atFALSE );
		lpMListBox->bTickerActivate[i] = atFALSE;
	}

	lpMListBox->bIsChange = atTRUE;
}

static void _SetScrollBarPosition(atHCONTROL hControl, atLPMLISTBOX lpMListBox)
{
	atREGION rcDlg;
	int hh;
	atINT iScrollPos, iSetScroll;
	int item_s_y, item_e_y;
	int v_item_s_y, v_item_e_y;
	atMLISTBOX_GETITEMHEIGHT_FUNC *get_height;

	if( lpMListBox->nSelectIndex < 0 ){
		atSCROLLBAR_EXT_SetPosition( lpMListBox->hScrollBar, 0);
		lpMListBox->nViewTopY = 0;
        	lpMListBox->bIsChange = atTRUE;
		return;
	}

	atCONTROL_GetClientRegion(hControl, &rcDlg);
	iScrollPos = lpMListBox->nViewTopY;
	iSetScroll = iScrollPos;

	get_height = (lpMListBox->fnFuncs.fnGetItemHeight)? lpMListBox->fnFuncs.fnGetItemHeight : _defGetItemHeight;

//	item_s_y = lpMListBox->nSelectIndex * lpMListBox->nItemHeight;
//	item_e_y = item_s_y + lpMListBox->nItemHeight;
	item_s_y = _GetTotalItemHeight( hControl, lpMListBox, lpMListBox->nSelectIndex, atNULL, atNULL );
	hh = get_height( hControl, lpMListBox, lpMListBox->nSelectIndex, lpMListBox->lpFuncParam ); 
	item_e_y = item_s_y + hh - 1;
	v_item_s_y = item_s_y - iScrollPos;
	v_item_e_y = item_e_y - iScrollPos;

	if( v_item_e_y > rcDlg.nHeight )
		iSetScroll = item_e_y - rcDlg.nHeight;
	if( v_item_s_y < 0 )
		iSetScroll = item_s_y;

	if( iSetScroll != iScrollPos ) {
		atSCROLLBAR_EXT_SetPosition( lpMListBox->hScrollBar, iSetScroll);
		lpMListBox->nViewTopY = iSetScroll;
        lpMListBox->bIsChange = atTRUE;
	}
}

atBOOL atMLISTBOX_SetSelectIndex(atHCONTROL hControl, int index)
{
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr(hControl);
	atMLISTBOX_ITEM itm;
	int i;

//	if( index < 0 ) return atFALSE;
	if( index >= atMLISTBOX_GetCount(hControl) ) return atFALSE;
	if( lpMListBox->nSelectIndex == index ) return atTRUE;
	
	if( index >= 0 ){
		if( atMLISTBOX_GetItem(hControl, index, &itm) )
			if( !itm.bEnable ) return atFALSE;
	}

	lpMListBox->nPrevSelectIndex 	= lpMListBox->nSelectIndex;
	lpMListBox->nSelectIndex		= index;
	lpMListBox->bIsChange = atTRUE;
	
	if( index >= 0 ){
		if(lpMListBox->bScrollVisible){
			_SetScrollBarPosition( hControl, lpMListBox );
			_Realloc_Items( hControl );
		}
	}

	for(i = 0 ; i < lpMListBox->nLineCnt ; i++) {
		if( atCONTROL_IsVisible(lpMListBox->hTickerBox[i]) ) {
			atTICKERBOX_ResetTick( lpMListBox->hTickerBox[i] );
		}
	}

	atCONTROL_PostMessageToParent( hControl, WNDMSG_CHANGE, (long)lpControl->Base.nID, (long)lpMListBox->nSelectIndex );
	return atTRUE;
}

int atMLISTBOX_MoveSelect(atHCONTROL hControl, int nDirect)
{
	atLPCONTROL	lpControl = atCONTROL_GetPtr(hControl);
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr(hControl);
	atMLISTBOX_ITEM itm; atBOOL is_itm;
	int stidx, maxidx, curidx;
	
	maxidx = atMLISTBOX_GetCount(hControl)-1;
	if( maxidx < 0 ) return -1;

	stidx = lpMListBox->nSelectIndex;
	if( (stidx < 0) && (nDirect < 0) ) {
		stidx = maxidx + nDirect;
	}

	is_itm = atFALSE;

	curidx = stidx;
	do {
		curidx += nDirect;

		if (stidx < 0 && curidx > maxidx) break;	// phone number list에서 행되서 추가함

		if( lpMListBox->dwAttr & atMLISTBOX_ATTR_MOVE_WRAP ) {
			if( nDirect < 0 && curidx < 0)				curidx = maxidx;
			else if( nDirect > 0 && curidx > maxidx)	curidx = 0;
		} else if( lpMListBox->dwAttr & atMLISTBOX_ATTR_MOVE_NEXTCONTROL ) {
			if( nDirect < 0 && curidx < 0)				return 0;
			else if( nDirect > 0 && curidx > maxidx)	return maxidx;
		} else {
			if( nDirect < 0 && curidx < 0) {
				curidx = 0;
				is_itm = atMLISTBOX_GetItem(hControl, curidx, &itm);
				break;
			} else if( nDirect > 0 && curidx > maxidx) {
				curidx = maxidx;
				is_itm = atMLISTBOX_GetItem(hControl, curidx, &itm);
				break;
			}
		}
		
		is_itm = atMLISTBOX_GetItem(hControl, curidx, &itm);
		
		if( !is_itm || itm.bEnable ) break;
	} while( curidx != stidx );

	if( is_itm && !itm.bEnable ) curidx = -1;

	atMLISTBOX_SetSelectIndex( hControl, curidx );
	return lpMListBox->nSelectIndex;
}

atBOOL atMLISTBOX_ToggleSelectItemChecked(atHCONTROL hControl)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atMLISTBOX_ITEM itm;

	if( lpMListBox->nCheckType != 1 ) return atFALSE;

	if( atMLISTBOX_GetSelectItem(hControl, &itm) ){
		itm.bChecked = !itm.bChecked;
		atMLISTBOX_SetSelectItem(hControl, &itm);

		if( itm.bChecked ) {
			lpMListBox->nCurCheckedIndex = atMLISTBOX_GetSelectIndex(hControl);
		}
	
		return itm.bChecked;
	} else {
		return atFALSE;
	}
}

void atMLISTBOX_SetItemCheck(atHCONTROL hControl, int nIndex, atBOOL bCheck)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	int i, cnt;
	atMLISTBOX_ITEM itm, itm2;

	if( lpMListBox == atNULL ) return;
	if( lpMListBox->nCheckType == 0 ) return;

	cnt = atMLISTBOX_GetCount(hControl);
	if( cnt <= 0 ) return;
	
	if( !atMLISTBOX_GetItem(hControl, nIndex, &itm) ) return;
	if( itm.bChecked == bCheck ) return;

	if( lpMListBox->nCheckType == 1 ) {
		itm.bChecked = bCheck;
	} else if( lpMListBox->nCheckType == 2 ) {
		if( !bCheck ) return;
		for( i = 0; i < cnt; i++ ){
			if( i == nIndex ) continue;
			atMLISTBOX_GetItem(hControl, i, &itm2);
		   	itm2.bChecked = atFALSE;
			atMLISTBOX_SetItem(hControl, i, &itm2);
		}
		itm.bChecked = atTRUE;
	}
	atMLISTBOX_SetItem(hControl, nIndex, &itm2);
	
	if( bCheck ) lpMListBox->nCurCheckedIndex = nIndex;
}

atBOOL atMLISTBOX_GetItemChecked(atHCONTROL hControl, int nIndex)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atMLISTBOX_ITEM itm;

	if( lpMListBox == atNULL ) return atFALSE;
	if( (lpMListBox->nCheckType != 1) && (lpMListBox->nCheckType != 2) ) return atFALSE;

	if( !atMLISTBOX_GetItem(hControl, nIndex, &itm) ) return atFALSE;
	return itm.bChecked;
}

atINT atMLISTBOX_GetCurCheckedIndex(atHCONTROL hControl)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );

	if( lpMListBox == atNULL ) return -1;
	if( (lpMListBox->nCheckType != 1) && (lpMListBox->nCheckType != 2) ) return -1;

	return lpMListBox->nCurCheckedIndex;
}

void atMLISTBOX_SetItemEnabled(atHCONTROL hControl, int nIndex, atBOOL bEnable)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atMLISTBOX_ITEM itm;

	if( !atMLISTBOX_GetItem(hControl, nIndex, &itm) ) return;
	
	itm.bEnable = bEnable;
	atMLISTBOX_SetItem(hControl, nIndex, &itm);
	
	if( !bEnable ){
		if( nIndex == atMLISTBOX_GetSelectIndex(hControl) )
			atMLISTBOX_MoveSelect( hControl, 1 );
	}
}

atBOOL atMLISTBOX_GetItemEnabled(atHCONTROL hControl, int nIndex)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atMLISTBOX_ITEM itm;

	if( !atMLISTBOX_GetItem(hControl, nIndex, &itm) ) return atFALSE;

	return itm.bEnable;
}

void atMLISTBOX_Start_AddItem(atHCONTROL hControl)
{	
	atMLISTBOX_Start_AddItemEx( hControl, -1 );
}

void atMLISTBOX_Start_AddItemEx(atHCONTROL hControl, atINT nIndex)
{
	int i;

	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPMLISTBOX_ITEM lpData;
	
	if( lpMListBox == atNULL ) return;
	
	lpMListBox->nAddItemIndex = nIndex;
	lpMListBox->lpAddItemData = (atLPMLISTBOX_ITEM)UI_MEMALLOC(sizeof(atMLISTBOX_ITEM));
	
	lpData = lpMListBox->lpAddItemData;
	atUIAPI_memset(lpData, 0, sizeof(atMLISTBOX_ITEM));

	lpData->bEnable = atTRUE;
	lpData->nNO = 0;
	lpData->nItemID = -1;
	lpData->nImgSelectIdx = -1;
	lpData->data = atNULL;

	for(i = 0 ; i < lpMListBox->nLineCnt ; i++) {
		lpData->clItemColor[i][atMLISTBOX_STR_NORMAL] = (atCOLOR)0xFFFFFFFF;
		lpData->clItemColor[i][atMLISTBOX_STR_SELECT] = (atCOLOR)0xFFFFFFFF;
	}

	lpData->clItemChkBoxColor[atMLISTBOX_STR_NORMAL] = (atCOLOR)0xFFFFFFFF;
	lpData->clItemChkBoxColor[atMLISTBOX_STR_SELECT] = (atCOLOR)0xFFFFFFFF;
	lpData->clItemFirstNoColor[atMLISTBOX_STR_NORMAL] = (atCOLOR)0xFFFFFFFF;
	lpData->clItemFirstNoColor[atMLISTBOX_STR_SELECT] = (atCOLOR)0xFFFFFFFF;

	lpData->clHLColor = (atCOLOR)0xFFFFFFFF;
}

void atMLISTBOX_AddItem_Enabled(atHCONTROL hControl, atBOOL bEnabled)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPMLISTBOX_ITEM lpData = lpMListBox->lpAddItemData;

	lpData->bEnable	= bEnabled;
}

void atMLISTBOX_AddItem_Checked(atHCONTROL hControl, atBOOL bChecked)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPMLISTBOX_ITEM lpData = lpMListBox->lpAddItemData;

	lpData->bChecked = bChecked;
}

void atMLISTBOX_AddItem_NO(atHCONTROL hControl, int nNO)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPMLISTBOX_ITEM lpData = lpMListBox->lpAddItemData;

	lpData->nNO	= nNO;
}

void atMLISTBOX_AddItem_ItemID(atHCONTROL hControl, int nItemID)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPMLISTBOX_ITEM lpData = lpMListBox->lpAddItemData;

	lpData->nItemID	= nItemID;
}

void atMLISTBOX_AddItem_String(atHCONTROL hControl, atLPTSTR szItemName, int nIndex, int nLineIdx) 
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPMLISTBOX_ITEM lpData = lpMListBox->lpAddItemData;

	UI_STRNCPY(lpData->szItemName[nLineIdx][nIndex], szItemName, atMLISTBOX_STR_LEN-1);
	lpData->szItemName[nLineIdx][nIndex][atMLISTBOX_STR_LEN-1] = _ATC('\0');
	lpData->clItemColor[nLineIdx][nIndex] = (atCOLOR)0xFFFFFFFF;
	lpData->nItemNameIds[nLineIdx][nIndex] = 0;
}

void atMLISTBOX_AddItem_IDS(atHCONTROL hControl, atDWORD ids, int nIndex, int nLineIdx) 
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPMLISTBOX_ITEM lpData = lpMListBox->lpAddItemData;

	lpData->szItemName[nLineIdx][nIndex][0] = 0;
	lpData->szItemName[nLineIdx][nIndex][1] = 0;
	lpData->nItemNameIds[nLineIdx][nIndex]  = ids;
	lpData->clItemColor[nLineIdx][nIndex] = (atCOLOR)0xFFFFFFFF;
}

void atMLISTBOX_AddItem_String2(atHCONTROL hControl, atLPTSTR szItemName, atCOLOR clTextColor, int nIndex, int nLineIdx) 
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPMLISTBOX_ITEM lpData = lpMListBox->lpAddItemData;

	UI_STRNCPY(lpData->szItemName[nLineIdx][nIndex], szItemName, atMLISTBOX_STR_LEN-1);
	lpData->szItemName[nLineIdx][nIndex][atMLISTBOX_STR_LEN-1] = _ATC('\0');
	lpData->clItemColor[nLineIdx][nIndex] = clTextColor;
	lpData->nItemNameIds[nLineIdx][nIndex] = 0;
}
	
void atMLISTBOX_AddItem_String3(atHCONTROL hControl, atLPTSTR szItemName, int nIndex, int nLineIdx, int nFontSize) 
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPMLISTBOX_ITEM lpData = lpMListBox->lpAddItemData;

	UI_STRNCPY(lpData->szItemName[nLineIdx][nIndex], szItemName, atMLISTBOX_STR_LEN-1);
	lpData->szItemName[nLineIdx][nIndex][atMLISTBOX_STR_LEN-1] = _ATC('\0');
	lpData->clItemColor[nLineIdx][nIndex] = (atCOLOR)0xFFFFFFFF;
	lpData->nItemNameIds[nLineIdx][nIndex] = 0;
	lpData->nFontSize[nLineIdx] = ((nFontSize < 0) ? 0 : nFontSize);
}

void atMLISTBOX_AddItem_Data(atHCONTROL hControl, atVOID*data) 
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPMLISTBOX_ITEM lpData = lpMListBox->lpAddItemData;

	lpData->data = data;
}

void atMLISTBOX_AddItem_Image(atHCONTROL hControl, atHBITMAP imgListImage, int nIndex)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPMLISTBOX_ITEM lpData = lpMListBox->lpAddItemData;

	lpData->imgItemImage[nIndex] = imgListImage;
	if( imgListImage == atNULL ) return;

	lpData->imgItemImageRegion[nIndex].nX = 0;
	lpData->imgItemImageRegion[nIndex].nY = 0;
	lpData->imgItemImageRegion[nIndex].nWidth = -1;
	lpData->imgItemImageRegion[nIndex].nHeight = -1;
}

void atMLISTBOX_AddItem_Image2(atHCONTROL hControl, atHBITMAP imgListImage, int x, int y, int width, int height, int nIndex)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	atLPMLISTBOX_ITEM lpData = lpMListBox->lpAddItemData;

	lpData->imgItemImage[nIndex] = imgListImage;
	if( imgListImage == atNULL ) return;

	lpData->imgItemImageRegion[nIndex].nX = x;
	lpData->imgItemImageRegion[nIndex].nY = y;
	lpData->imgItemImageRegion[nIndex].nWidth = (width>=0) ? width : atUIAPI_GetBitmapWidth(imgListImage);
	lpData->imgItemImageRegion[nIndex].nHeight = (height>=0) ? height : atUIAPI_GetBitmapHeight(imgListImage);
}

void atMLISTBOX_Update_AddItem(atHCONTROL hControl)
{
	int idx;
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return;

	if( lpMListBox->lpAddItemData == atNULL ) return;
	if( lpMListBox->nAddItemIndex >= 0 ) {
		idx = atUIAPI_InsertListBefore(lpMListBox->lpListBoxList, lpMListBox->nAddItemIndex, lpMListBox->lpAddItemData);
	} else {
		idx = atUIAPI_AddListTail(lpMListBox->lpListBoxList, lpMListBox->lpAddItemData);
	}

	if( (!lpMListBox->dwAttr & ~atMLISTBOX_ATTR_CLICK) )
	{
		if( lpMListBox->nSelectIndex == -1 ) {
			if( lpMListBox->lpAddItemData->bEnable )
			  atMLISTBOX_SetSelectIndex( hControl, idx );
		}
	}

	if(  lpMListBox->lpAddItemData->bChecked ) {
		if( lpMListBox->nCurCheckedIndex == -1 ) 
			lpMListBox->nCurCheckedIndex = idx;
	}

	lpMListBox->lpAddItemData = atNULL;
	lpMListBox->bIsChange = atTRUE;
	_Realloc_Items(hControl);
}

void atMLISTBOX_DeleteItem(atHCONTROL hControl, int nIndex)
{   
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	int cnt;

    atUIAPI_RemoveListAt(lpMListBox->lpListBoxList, nIndex, atTRUE);
	cnt = atMLISTBOX_GetCount(hControl);

	if( lpMListBox->nSelectIndex >= cnt ) lpMListBox->nSelectIndex = cnt - 1;
	if( nIndex == lpMListBox->nCurCheckedIndex ) {
		lpMListBox->nCurCheckedIndex = -1;
	}
	
	lpMListBox->bIsChange = atTRUE;
	_Realloc_Items(hControl);
}

int atMLISTBOX_GetCount(atHCONTROL hControl)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return 0;

	if( lpMListBox->fnFuncs.fnGetCount == atNULL )
		return (lpMListBox->lpListBoxList) ? atUIAPI_GetListCount(lpMListBox->lpListBoxList) : 0;
	else
		return lpMListBox->fnFuncs.fnGetCount( hControl, lpMListBox, lpMListBox->lpFuncParam );
}

int atMLISTBOX_GetSelectIndex(atHCONTROL hControl)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return -1;

	return lpMListBox->nSelectIndex;
}

atBOOL atMLISTBOX_SetItem(atHCONTROL hControl, int nIndex, atLPMLISTBOX_ITEM lpItem)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return atFALSE;

	lpMListBox->bIsChange = atTRUE;

	if( (nIndex < 0) || (nIndex >= atMLISTBOX_GetCount(hControl)) ) return atFALSE;

	if( lpMListBox->fnFuncs.fnSetItem == atNULL ){
		atUIAPI_SetListAt( lpMListBox->lpListBoxList, nIndex, lpItem, atFALSE );
		return atTRUE;
	} else {
		return lpMListBox->fnFuncs.fnSetItem( hControl, lpMListBox, nIndex, lpItem, lpMListBox->lpFuncParam );
	}
}

atBOOL atMLISTBOX_GetItem(atHCONTROL hControl, int nIndex, atMLISTBOX_ITEM *lpItem)
{
	atMLISTBOX_ITEM *itm;
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return atFALSE;

	if( (nIndex < 0) || (nIndex >= atMLISTBOX_GetCount(hControl)) ) return atFALSE;

	if( lpMListBox->fnFuncs.fnGetItem == atNULL ){
		itm = (atLPMLISTBOX_ITEM)atUIAPI_GetListAt( lpMListBox->lpListBoxList, nIndex );
		if( itm == atNULL ) return atFALSE;
		*lpItem = *itm;
		return atTRUE;
	} else {
		return lpMListBox->fnFuncs.fnGetItem( hControl, lpMListBox, nIndex, lpItem, lpMListBox->lpFuncParam );
	}
}

atBOOL atMLISTBOX_GetSelectItem(atHCONTROL hControl, atMLISTBOX_ITEM *lpItem)
{
	return atMLISTBOX_GetItem( hControl, atMLISTBOX_GetSelectIndex(hControl), lpItem );
}

atBOOL atMLISTBOX_SetSelectItem(atHCONTROL hControl, atMLISTBOX_ITEM *lpItem)
{
	return atMLISTBOX_SetItem( hControl, atMLISTBOX_GetSelectIndex(hControl), lpItem );
}

atVOID atMLISTBOX_RealizeItem(atHCONTROL hControl)
{
	if( atMLISTBOX_GetSelectIndex(hControl) >= atMLISTBOX_GetCount(hControl) ) {
		atMLISTBOX_SetSelectIndex(hControl, atMLISTBOX_GetCount(hControl));
		atMLISTBOX_MoveSelect( hControl, -1 );
	} else {
		atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
		if( lpMListBox == atNULL ) return;

        lpMListBox->bIsChange = atTRUE;
	}
	
	_Realloc_Items(hControl);
}

atLPTSTR atMLISTBOX_GetItemString(atHCONTROL hControl, int nIndex, int nLineIdx, int nStringIndex)
{
	atMLISTBOX_ITEM itm;
	if( !atMLISTBOX_GetItem( hControl, nIndex, &itm ) ) return atNULL;
	return itm.szItemName[nLineIdx][nStringIndex];
}

atVOID* atMLISTBOX_GetItemData(atHCONTROL hControl, int nIndex)
{
	atMLISTBOX_ITEM itm;
	if( !atMLISTBOX_GetItem( hControl, nIndex, &itm ) ) return atNULL;
	return itm.data;
}

atVOID* atMLISTBOX_GetCurItemData(atHCONTROL hControl)
{
	return atMLISTBOX_GetItemData(hControl, atMLISTBOX_GetSelectIndex(hControl));
}

atINT atMLISTBOX_ResetSelect(atHCONTROL hControl)
{
	int r;
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return -1;
	
	lpMListBox->nViewTopY = 0;
	lpMListBox->nSelectIndex = -1;
	r = atMLISTBOX_MoveSelect( hControl, 1 );
	
	_Realloc_Items(hControl);
	return r;
}

atBOOL atMLISTBOX_SetViewTopY(atHCONTROL hControl, atINT nViewTopY)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr(hControl);
	if( lpMListBox == atNULL ) return atFALSE;

	lpMListBox->nViewTopY = nViewTopY;

	return atTRUE;
}

atINT atMLISTBOX_GetViewTopY(atHCONTROL hControl)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr(hControl);
	if( lpMListBox == atNULL ) return 0;

	return lpMListBox->nViewTopY;
}

void atMLISTBOX_SetupKey( atHCONTROL hControl, atINT SelectKey, atINT CheckKey )
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return;

	if( lpMListBox->SelectKey == atVKEY_SOFT1 ){
		atCONTROL_SetSoftKey(hControl, _AT(""), atNULL, atNULL);
	} else if( lpMListBox->SelectKey == atVKEY_SOFT2 ){
		atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
	} else if( lpMListBox->SelectKey == atVKEY_SELECT ){
		atCONTROL_SetSoftKey(hControl, atNULL, _AT(""), atNULL);
	}
	if( lpMListBox->CheckKey == atVKEY_SOFT1 ){
		atCONTROL_SetSoftKey(hControl, _AT(""), atNULL, atNULL);
	} else if( lpMListBox->CheckKey == atVKEY_SOFT2 ){
		atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT(""));
	} else if( lpMListBox->CheckKey == atVKEY_SELECT ){
		atCONTROL_SetSoftKey(hControl, atNULL, _AT(""), atNULL);
	}
	
	lpMListBox->SelectKey = SelectKey;
	lpMListBox->CheckKey = CheckKey;

	if( lpMListBox->SelectKey == atVKEY_SOFT1 ){
		atCONTROL_SetSoftKey(hControl, _AT("OK"), atNULL, atNULL);
	} else if( lpMListBox->SelectKey == atVKEY_SOFT2 ){
		atCONTROL_SetSoftKey(hControl, atNULL, atNULL, _AT("OK"));
	} else if( lpMListBox->CheckKey == atVKEY_SELECT ){
		atCONTROL_SetSoftKey(hControl, atNULL, _AT("OK"), atNULL);
	}

	atSOFTKEY_RefreshSoftKey();
}

void	atMLISTBOX_GetFuncs( atHCONTROL hControl, atLPMLISTBOX_FUNCS lpFuncs, atLPVOID *lppFuncParam )
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL || lpFuncs == atNULL ) return;

	*lpFuncs = lpMListBox->fnFuncs;
	if( lppFuncParam ) *lppFuncParam = lpMListBox->lpFuncParam;
}

void atMLISTBOX_SetupFuncs( atHCONTROL hControl, atLPMLISTBOX_FUNCS lpFuncs, atLPVOID lpFuncParam )
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL || lpFuncs == atNULL ) return;

	lpMListBox->fnFuncs = *lpFuncs;
	lpMListBox->lpFuncParam = lpFuncParam;

//	atMLISTBOX_ResetSelect( hControl );
	if( atCONTROL_IsVisible(lpMListBox->hScrollBar) ){
		atCONTROL_SetVisible( lpMListBox->hScrollBar, atFALSE );
		atSCROLLBAR_EXT_SetPosition( lpMListBox->hScrollBar, 0 );
	}
	atMLISTBOX_SetSelectIndex( hControl, -1 );
	_Realloc_Items(hControl);
	atCONTROL_Refresh( hControl );
}

atBOOL atMLISTBOX_SetNoAlignLine(atHCONTROL hControl, int nAlignLine)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return atFALSE;

	if(nAlignLine <= 0) nAlignLine = 0;
	if(nAlignLine > lpMListBox->nLineCnt) nAlignLine = lpMListBox->nLineCnt;
	
	lpMListBox->nNoAlignLine = nAlignLine;
	return atTRUE;
}

atINT atMLISTBOX_GetNoAlignLine(atHCONTROL hControl)
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return -1;

	return lpMListBox->nNoAlignLine;
}

atBOOL atMLISTBOX_SetTextColor(atHCONTROL hControl, atINT nIndex, atINT nLineIdx, atINT nStringIndex, atCOLOR clTextColor)
{
	atMLISTBOX_ITEM itm;
	if( !atMLISTBOX_GetItem(hControl, nIndex, &itm) ) return atFALSE;

	itm.clItemColor[nLineIdx][nStringIndex] = (atCOLOR)clTextColor;

	atMLISTBOX_SetItem( hControl, nIndex, &itm);
	
	return atTRUE;
}

atCOLOR atMLISTBOX_GetTextColor(atHCONTROL hControl, atINT nIndex, atINT nLineIdx, atINT nStringIndex)
{
	atMLISTBOX_ITEM itm;
	if( !atMLISTBOX_GetItem( hControl, nIndex, &itm ) ) return atUI_COLOR_BLACK;
	return itm.clItemColor[nLineIdx][nStringIndex];
}

atBOOL atMLISTBOX_SetFontSize(atHCONTROL hControl, atINT nIndex, atINT nLineIdx, atINT nFontSize)
{
	atMLISTBOX_ITEM itm;
	if( !atMLISTBOX_GetItem(hControl, nIndex, &itm) ) return atFALSE;

	if(nFontSize < 0) nFontSize = 0;
	itm.nFontSize[nLineIdx] = nFontSize;

	atMLISTBOX_SetItem( hControl, nIndex, &itm);
	
	return atTRUE;
}

atINT atMLISTBOX_GetFontSize(atHCONTROL hControl, atINT nIndex, atINT nLineIdx)
{
	atMLISTBOX_ITEM itm;
	if( !atMLISTBOX_GetItem(hControl, nIndex, &itm) ) return 0;

	return itm.nFontSize[nLineIdx];
}

// Kyle Start [Add 2009/07/23-15:45]
atVOID atMLISTBOX_SetUseSmoothScroll( atHCONTROL hControl, atBOOL bUsed )
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return;

	lpMListBox->bUseSmoothScroll = bUsed;
}

atBOOL atMLISTBOX_GetUseSmoothScroll( atHCONTROL hControl )
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	return lpMListBox ? lpMListBox->bUseSmoothScroll : atFALSE;
}
// Kyle [Add 2009/07/23-15:45] End

atVOID	atMLISTBOX_SetVisibleScroll( atHCONTROL hControl, atBOOL bVisible )
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return;

	if( lpMListBox->bScrollVisible == bVisible )	return ;

	lpMListBox->bScrollVisible = bVisible;
}

atBOOL	atMLISTBOX_GetVisibleScroll( atHCONTROL hControl )
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	return lpMListBox ? lpMListBox->bScrollVisible : atFALSE;
}

atINT atMLISTBOX_GetSelectImageIndex( atHCONTROL hControl, atINT nIndex )
{
	atLPMLISTBOX lpMListBox;
	atLPMLISTBOX_ITEM lpItem;
	atMLISTBOX_ITEM itm;
	
	lpMListBox= (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return -1;
	
	if( lpMListBox->nSelectIndex != nIndex || lpMListBox->nSelectIndex < 0)
		return -1;

	if( lpMListBox->fnFuncs.fnGetItem == atNULL ){
		lpItem = (atLPMLISTBOX_ITEM)atUIAPI_GetListAt( lpMListBox->lpListBoxList, nIndex );
		if( lpItem == atNULL ) return -1;
	} else {
		if( !lpMListBox->fnFuncs.fnGetItem( hControl, lpMListBox, nIndex, &itm, lpMListBox->lpFuncParam ) ) return -1;
	}

	return itm.nImgSelectIdx;
}

atVOID atMLISTBOX_SetItemMargin( atHCONTROL hControl, int nWidthMargin, int nHeightMargin )
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return ;

	lpMListBox->nWidthMargin  = nWidthMargin;
	lpMListBox->nHeightMargin = nHeightMargin;
}

atVOID atMLISTBOX_GetItemMargin( atHCONTROL hControl, int* nWidthMargin, int* nHeightMargin )
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return ;

	*nWidthMargin  = lpMListBox->nWidthMargin;
	*nHeightMargin = lpMListBox->nHeightMargin;
}

atVOID atMLISTBOX_SetItemGap( atHCONTROL hControl, int nItemGap )
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return ;

	lpMListBox->nItemGap = nItemGap;
}

int atMLISTBOX_GetItemGap( atHCONTROL hControl )
{
	atLPMLISTBOX lpMListBox = (atLPMLISTBOX)atCONTROL_GetDataPtr( hControl );
	if( lpMListBox == atNULL ) return 0;

	return lpMListBox->nItemGap;
}
