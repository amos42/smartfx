/**
  @file UI_Util.c
  @brief UI엔진의 Utility API
  @remark UI엔진의 Utility API
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Util.h
*/
#include "UI_Types.h"
#include "UI_Util.h"
#include "UI_API/UI_API.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
//#include "A_DALLOC.h"



static int alloc_count = 0;



atLPTSTR	myStringCut( atHGDC hDC, atLPTSTR szIn, atLPTSTR szOut, int nScreenWidth, atBOOL bFlag )
{
	int		nIndex	= 0;
	int		length	= 0;
	atHFONT hFont;
	int		string_length;
	
	hFont = atUIAPI_GetFont(hDC);

	string_length = atUIAPI_GetStringWidth(hFont, szIn, -1);
	szOut[0] = _ATC('\0');

#ifdef __USES_UNICODE_
	if (string_length > nScreenWidth)
	{
		if( bFlag ) nScreenWidth -= atUIAPI_GetStringWidth(hFont, _AT("..."), 3);
		
		while (length < nScreenWidth)
		{
			if (szIn[nIndex] == _ATC('\0')) break;
			
			length	+= atUIAPI_GetStringWidth(hFont, &szIn[nIndex], 1);
			nIndex++;
		}
		
		if (nIndex-1 < 0)
		{
			szOut[0] = 0;
		}
		else
		{
			UI_STRNCPY(szOut, szIn, nIndex-1);
			szOut[nIndex-1] = _ATC('\0');
		}
		
		if( bFlag ) UI_STRCAT(szOut, _AT("..."));
	}
#else
	if (string_length > nScreenWidth)
	{
		if( bFlag ) nScreenWidth -= atUIAPI_GetStringWidth(hFont, "...", 3);
		
		while (length < nScreenWidth)
		{
			if (szIn[nIndex] == '\0')
				break;
			
			if ((szIn[nIndex] < 0) || (szIn[nIndex] > 128))
			{
				nIndex++;
				
				if ((szIn[nIndex] < 0) || (szIn[nIndex] > 128))
				{
					length	+= atUIAPI_GetStringWidth(hFont, &szIn[nIndex-1], 2);
				}
			}
			else
			{
				length	+= atUIAPI_GetStringWidth(hFont, &szIn[nIndex], 1);
			}
			
			nIndex++;
		}
		
		if (nIndex-1 < 0)
		{
			szOut[0] = 0;
		}
		else
		{
			strncpy(szOut, szIn, nIndex-1);
			szOut[nIndex-1] = '\0';
		}
		
		if( bFlag ) UI_STRCAT(szOut, "...");
	}
#endif
	else
	{
		UI_STRCPY(szOut, szIn);
	}

	return szOut;
}


int myGetLineNum(atLPTSTR szMsg)
{
	int		nCount, ii, nFin;
	atTCHAR *lpPos;

	nFin	= UI_STRLEN(szMsg);
	lpPos	= szMsg;
	if ( nFin == 0 )
		return 0;

	nCount	= 1;

	for ( ii = 0; ii < nFin; ii++ )
	{
		if ( *lpPos == _ATC('\n') )
		{
			nCount++;
		}
		if ( *lpPos == 0 )
		{
			break;
		}
		lpPos++;
	}
	return nCount;
}


int myGetStrLine(atLPTSTR szDestBuffer, atLPTSTR szSrcBuffer, int nLineNo, atTCHAR byDivideCode)
{
	int		nCount, ii, nFin, nStrNum;
	atTCHAR *lpPos, *lpStartPos;

	nFin	= UI_STRLEN(szSrcBuffer);
	lpPos	= szSrcBuffer;
	if ( nFin == 0 )
		return 0;

	nCount	= 0;

	lpStartPos	= lpPos;
	nStrNum		= 0;
	for ( ii = 0; ii < nFin; ii++ )
	{
		if ( *lpPos == byDivideCode )
		{
			nCount++;
			if ( nCount == nLineNo )
			{
				atUIAPI_memset(szDestBuffer, 0, UI_STRLEN(szDestBuffer) * sizeof(atTCHAR));
				UI_STRNCPY(szDestBuffer, lpStartPos, nStrNum);
				szDestBuffer[nStrNum] = _ATC('\0');
				return UI_STRLEN(szDestBuffer);
			}
			nStrNum		= 0;
			lpPos++;
			lpStartPos	= lpPos;
			continue;
		}
		if ( *lpPos == 0 )
		{
			break;
		}
		nStrNum++;
		lpPos++;
	}
	UI_STRNCPY(szDestBuffer, lpStartPos, nStrNum);
	szDestBuffer[nStrNum] = _ATC('\0');

	return UI_STRLEN(szDestBuffer);
}



int myAdjustStr( int swidth, int sheight, atLPTSTR src_str, int stidx, void *data, int (*strwidth)(void*,atLPTSTR,int), void (*drawfunc)(void*,int,int,int,atLPTSTR) )
{
	int size, total_len;
	atTCHAR strbuff[256];
	atTCHAR *buff;
	int idx;
	atTCHAR ch, ch2;
	atLPTSTR str;
	atLPTSTR str0;
	int len;

	if( strwidth == atNULL ) return 0;

	idx = 0;

	str = &src_str[stidx];
    total_len = UI_STRLEN(str);

	if( *str == _ATC('\0') ) return 0;
	
	do {
		buff = strbuff;
		str0 = str;
		len = 0;

		while( (ch = *str++) != _ATC('\0') )
		{
			ch2 = *str; 
			if( ch == _ATC('\n') )
			{
				if( (ch2 != _ATC('\0')) && (ch2 == _ATC('\r')) ) str++;
				break;
			}
			else if( ch == _ATC('\r') )
			{
				if( (ch2 != _ATC('\0')) && (ch2 == _ATC('\n')) ) str++;
				break;
			}
			
			if( ch != _ATC('\0') )
			{
				int chlen = 0;

				*buff++ = ch; chlen++;

#ifndef __USES_UNICODE_				
				// 한글일 경우 한문자를 더 넣는다.
				if( (unsigned char)ch & 0x80 )
				{
					ch = *str++;
					if( ch != _ATC('\0') )
					{
						*buff++ = ch; chlen++;
					}
				}
#endif

//				*buff = _ATC('\0');
				size = strwidth(data, strbuff, len+chlen);
				if( size > swidth ){ 
					if( len <= 0 ) return 0;
					str -= chlen; break; 
				}
				len += chlen;
				if( size == swidth ) break;
			}
		} 

		if( drawfunc ) drawfunc( data, str0-src_str, len, idx, strbuff );
		
		idx ++;
    } while( ch != _ATC('\0') );

	return idx;
}

// Dlg, DlgCtrl 에서 자주 사용하는 함수 인데 어디다 두어야 할지 몰라 여기다가 빼두었습니다.
// 2009/5/25  Adolph
atDWORD AutoLineDown(atLPTSTR szText, atHFONT hFont, atINT nMaxLen, atLONG lMaxWidth)
{
	//	atTCHAR szTemp[256] = {0, };
	atTCHAR szBackup[256] = {0, };
	atTCHAR *szTemp = szText;

	atINT iIndex, ii, iWidth;
	atINT nStIndex = 0;
	atINT nTgIndex = 0;
	atINT iLength = atUIAPI_strlen(szText);

	if(szText == atNULL) return 0;
	if(lMaxWidth <= 0) return 0;

	//	SYSAPI_tcscpy(szTemp, szText);

	for(ii = 0 , iIndex = 0; ii < iLength ; ii++, iIndex++)
	{
		iWidth = atUIAPI_GetStringWidth(hFont, &szTemp[nStIndex], iIndex);

		if(iWidth >= lMaxWidth)
		{
			atUIAPI_strncpy(&szBackup[nTgIndex], &szTemp[nStIndex], iIndex);
			nStIndex += iIndex - 1;
			nTgIndex += iIndex - 1;

			szBackup[nTgIndex] = _ATC('\n');
			nTgIndex++;

			iIndex = 0;
		}
	}

	// 마지막 남은 문자열을 뒤에 붙여준다 
	//SYSAPI_tcsncpy(&szBackup[nTgIndex], &szTemp[nStIndex], iLength - nStIndex);
	atUIAPI_strcpy(&szBackup[nTgIndex], &szTemp[nStIndex]);

	if(atUIAPI_strlen(szBackup) > nMaxLen) return 0;
	if(!atUIAPI_strlen(szBackup)) return 0;

	//	SYSAPI_memset(szText, NULL, sizeof(szText));
	atUIAPI_strcpy(szText, szBackup);

	return 1;
}

void GetStringCut( atLPTSTR szOut, int out_size, atLPTSTR szIn, atHFONT hFt, int scr_cx )
{
	int in_idx=0, out_idx=0;
	int nWidth=0;
	int char_cx=0;
	atTCHAR szBuf;

	out_size--;
	while( szIn[in_idx] && (out_idx<out_size) )
	{
		szBuf = szIn[in_idx];
		char_cx = atUIAPI_GetStringWidth(hFt, &szBuf, 1);

		if( scr_cx < (nWidth+char_cx) )
		{

			if( out_idx < out_size-2 )
			{
				szOut[out_idx++] = _ATC('\n');
				szOut[out_idx] = szBuf;
				nWidth = char_cx;
			}
			else
			{
				break;
			}
		}
		else if( szBuf == _ATC('\n') )
		{
			szOut[out_idx] = szBuf;
			nWidth = 0;
		}
		else
		{
			szOut[out_idx] = szBuf;
			nWidth += char_cx;
		}

		in_idx++;
		out_idx++;
	}

	szOut[out_idx] = 0;
}

static atHFONT font;
static int height;
static atHGDC dc;
static int strwidth( void *data, atLPTSTR buff, int len )
{
	return atUIAPI_GetStringWidth(atUIAPI_GetFont(dc), buff, len);
}
static void drawfunc( void *data, int stidx, int len, int y, atLPTSTR str )
{
	atUIAPI_DrawString( dc, 10, 70 + y*height, str, UI_COLOR_WHITE );
}

void myAssert( atLPTSTR fmt, ... )
{
	atTCHAR str[256];
	va_list args;

#ifdef WIN32
#if _MSC_VER>1200 //MSVS2008 ver
__asm int 3; // 디버깅을 위해 트랩
#endif
#endif

    va_start( args, fmt );
	atUIAPI_vsprintf( str, fmt, args );
	va_end( args );

	dc = atUIAPI_GetScreenCanvas();

	atUIAPI_FillRect( dc, 0, 0, atUIAPI_GetScreenWidth(), atUIAPI_GetScreenHeight(), atUIAPI_GetRGB(0, 0, 255));	

	font = atUIAPI_GetStockFont(0);

	atUIAPI_SetFont( dc, font );
	
    height = atUIAPI_GetFontHeight(font);

//	myAdjustStr( scr_info.m_width - 20, scr_info.m_height, str, strwidth, drawfunc );
	myAdjustStr( atUIAPI_GetScreenWidth() - 20, atUIAPI_GetScreenHeight(), str, 0, atNULL, strwidth, drawfunc );

	atUIAPI_FlushScreen();

	while(1) {}
}

void *myAlloc( long size, char *filename, int line )
{
	void *ptr;

	if (size == 0) return atNULL;

//myMemCheck( filename, line );
//UI_MSG_F("FREEMEM(%d), REQMEM(%d)", atDALLOC_GetGlobalLeftSize(), size);
	ptr = atUIAPI_AllocMem( size );
//UI_MSG_F("@@@ AllocMem(%d) (%s:%d) - %d free(%%d)\r\n", size, filename, line, alloc_count/*, atDALLOC_GetGlobalTotalLeftSize()*/ );
//myMemCheck( filename, line );

	if( ptr == atNULL )
	{
myAssert(_AT("Error MEM Alloc !\r\n File:%s \r\n Line:%d \r\n"), filename, line );
		UI_MSG_F( _AT(">>>>>>>>>>>>>>>>>>>>> Error MEM Alloc !!! (%s:%d)\r\n"), filename, line );
		return atNULL;
	}

	alloc_count ++;
//	UI_MSG_F("------------ MEM Alloc = sz:%d %x (%d) [%s:%d]\r\n", size, ptr, alloc_count, filename, line );

	return ptr;
}


atBOOL myFree( void *ptr, char *filename, int line )
{
	int r;

	if (ptr == atNULL) return atTRUE;

//myMemCheck( filename, line );
	r = atUIAPI_FreeMem(ptr);
//    UI_MSG_F("@@@ FreeMEM(0x%x) (%s:%d) - %d(%%d)\r\n", ptr, filename, line, alloc_count-1 /*, atDALLOC_GetGlobalTotalLeftSize() */);
//myMemCheck( filename, line );

	if( r < 0 )
	{
//		myAssert("Error MEM Free !\r\n File:%s \r\n Line:%d \r\n", filename, line );
		UI_MSG_F( _AT(">>>>>>>>>>>>>>>>>>>>> Error MEM Free !!! [%s:%d]\r\n"), filename, line );
		return atFALSE;
	}

	alloc_count --;
//	UI_MSG_F("------------ MEM Free = %x (%d) [%s:%d]\r\n", ptr, alloc_count, filename, line );

	return atTRUE;
}

  
atBOOL myMemCheck( char *filename, int line )
{
	int r;
	
	r = atUIAPI_CheckValidateHeap();
//r = atTRUE;

	if( !r )
	{
		myAssert( _AT("Error MEM Check !\r\n File:%s \r\n Line:%d \r\n"), filename, line );
//		UI_MSG_F(">>>>>>>>>>>>>>>>>>>>> Error MEM Check !!! [%s:%d]\r\n", filename, line );
		return atFALSE;
	}

//	UI_MSG_F("------------ MEM Check [%s:%d]\r\n", filename, line );

	return atTRUE;
}

atBOOL myMemCheck2( void *ptr, char *filename, int line )
{
	int r;
	
//	r = atDALLOC_CheckGlobalValidatePtr(ptr);
r = atTRUE;

	if( !r )
	{
		myAssert( _AT("Error MEM Check 2 !\r\n File:%s \r\n Line:%d \r\n"), ptr, filename, line );
//		UI_MSG_F(">>>>>>>>>>>>>>>>>>>>> Error MEM Check 2 !!! [%s:%d]\r\n", ptr, filename, line );
		return atFALSE;
	}

//	UI_MSG_F("------------ MEM Check 2 = %x [%s:%d]\r\n", ptr, filename, line );

	return atTRUE;
}



int myDebugOut( const atLPTSTR fmt, ... )
{
	atTCHAR str[512];
	int l;
	va_list args;

    va_start( args, fmt );
	l = UI_SPRINTF( str, fmt, args );
	va_end( args );

#ifdef __USES_UI_DEBUG_
    atUIAPI_DebugPrint( str );
#endif
    
    return( l );
}


int mySprintf( atLPTSTR str, const atLPTSTR fmt, ... )
{
	int l;
	va_list args;

    va_start( args, fmt );
	l = atUIAPI_vsprintf( str, fmt, args );
	va_end( args );

    return( l );
}




atBOOL atUIAPI_IsPtInRect( atINT nX, atINT nY, atRECT *lpSrcRect )
{
	return ((nX >= lpSrcRect->nStartX) && (nX <= lpSrcRect->nEndX) && 
		     (nY >= lpSrcRect->nStartY) && (nY <= lpSrcRect->nEndY))? atTRUE : atFALSE;
}

atBOOL atUIAPI_IsPtInRect2( atPOINT *lpPt, atRECT *lpSrcRect )
{
	return atUIAPI_IsPtInRect( lpPt->nX, lpPt->nY, lpSrcRect );
}

atBOOL atUIAPI_IsEmbededRect( atRECT *lpRect, atRECT *lpParentRect )
{
	if( lpRect->nStartX < lpParentRect->nStartX ) return atFALSE;
	if( lpRect->nEndX > lpParentRect->nEndX ) return atFALSE;
	
	if( lpRect->nStartY < lpParentRect->nStartY ) return atFALSE;
	if( lpRect->nEndY > lpParentRect->nEndY ) return atFALSE;

	return atTRUE;
}

atBOOL atUIAPI_IsIntersectRect( atRECT *lpSrcRect1, atRECT *lpSrcRect2 )
{
	if( lpSrcRect1->nStartX <= lpSrcRect2->nStartX ){
		if( lpSrcRect2->nStartX > lpSrcRect1->nEndX ) return atFALSE;
	} else {
		if( lpSrcRect1->nStartX > lpSrcRect2->nEndX ) return atFALSE;
	}
 
	if( lpSrcRect1->nStartY <= lpSrcRect2->nStartY ){
		if( lpSrcRect2->nStartY > lpSrcRect1->nEndY ) return atFALSE;
	} else {
		if( lpSrcRect1->nStartY > lpSrcRect2->nEndY ) return atFALSE;
	}

	return atTRUE;
}

atBOOL atUIAPI_IntersectRect( atRECT *lpDesRect, atRECT *lpSrcRect1, atRECT *lpSrcRect2 )
{
	atRECT SrcRect1 = *lpSrcRect1;
	atRECT SrcRect2 = *lpSrcRect2;

	if( SrcRect1.nStartX <= SrcRect2.nStartX ){
		if( SrcRect2.nStartX > SrcRect1.nEndX ) return atFALSE;
		lpDesRect->nStartX = SrcRect2.nStartX;
		lpDesRect->nEndX = MIN( SrcRect1.nEndX, SrcRect2.nEndX );
	} else {
		if( SrcRect1.nStartX > SrcRect2.nEndX ) return atFALSE;
		lpDesRect->nStartX = SrcRect1.nStartX;
		lpDesRect->nEndX = MIN( SrcRect2.nEndX, SrcRect1.nEndX );
	}
 
	if( SrcRect1.nStartY <= SrcRect2.nStartY ){
		if( SrcRect2.nStartY > SrcRect1.nEndY ) return atFALSE;
		lpDesRect->nStartY = SrcRect2.nStartY;
		lpDesRect->nEndY = MIN( SrcRect1.nEndY, SrcRect2.nEndY );
	} else {
		if( SrcRect1.nStartY > SrcRect2.nEndY ) return atFALSE;
		lpDesRect->nStartY = SrcRect1.nStartY;
		lpDesRect->nEndY = MIN( SrcRect2.nEndY, SrcRect1.nEndY );
	}

	return atTRUE;
}

atVOID atUIAPI_UnionRect( atRECT *lpDesRect, atRECT *lpSrcRect1, atRECT *lpSrcRect2 )
{
	atRECT SrcRect1 = *lpSrcRect1;
	atRECT SrcRect2 = *lpSrcRect2;

	lpDesRect->nStartX = MIN( SrcRect1.nStartX, SrcRect2.nStartX );  
	lpDesRect->nStartY = MIN( SrcRect1.nStartY, SrcRect2.nStartY );  
	lpDesRect->nEndX = MAX( SrcRect1.nEndX, SrcRect2.nEndX );  
	lpDesRect->nEndY = MAX( SrcRect1.nEndY, SrcRect2.nEndY );  
}

atVOID atUIAPI_ToEmbededRect( atRECT *lpDesRect, atRECT *lpParentRect )
{
	if( lpDesRect->nStartX < lpParentRect->nStartX ) lpDesRect->nStartX = lpParentRect->nStartX;
	if( lpDesRect->nStartY < lpParentRect->nStartY ) lpDesRect->nStartY = lpParentRect->nStartY;
	if( lpDesRect->nEndX > lpParentRect->nEndX ) lpDesRect->nEndX = lpParentRect->nEndX;
	if( lpDesRect->nEndY > lpParentRect->nEndY ) lpDesRect->nEndY = lpParentRect->nEndY;
}

atVOID atUIAPI_MoveRect(atRECT *lpDesRect, atRECT *lpSrcRect, atINT nX, atINT nY )
{
	lpDesRect->nStartX = lpSrcRect->nStartX + nX;  
	lpDesRect->nStartY = lpSrcRect->nStartY + nY;  
	lpDesRect->nEndX = lpSrcRect->nEndX + nX;  
	lpDesRect->nEndY = lpSrcRect->nEndY + nY;  
}


atVOID atUIAPI_OffsetRect(atRECT *lpDesRect, atINT nX, atINT nY )
{
	lpDesRect->nStartX += nX;  
	lpDesRect->nStartY += nY;  
	lpDesRect->nEndX   += nX;  
	lpDesRect->nEndY   += nY;  
}

void atUIAPI_DrawClient1LineStretch( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight, 
					atHBITMAP lpTopLeftBM, atHBITMAP lpTopCenterBM, atHBITMAP lpTopRightBM )
{
	atINT i;
	atINT cnt, tmp, xx, yy;
	atINT ww1, ww2, ww3;

	if( hCanvas == atNULL ) return;
	if( nWidth <= 0 ) return;

	if( (lpTopLeftBM == atNULL) || (lpTopCenterBM == atNULL) || (lpTopRightBM == atNULL) ) return;
	
	ww1 = atUIAPI_GetBitmapWidth(lpTopLeftBM);
	ww2 = atUIAPI_GetBitmapWidth(lpTopCenterBM);
	ww3 = atUIAPI_GetBitmapWidth(lpTopRightBM);

	// 맨 윗줄을 그린다.
	yy = nY;
	xx = nX;
	atUIAPI_DrawImageStretch(hCanvas, xx, yy, lpTopLeftBM, ww1, nHeight);
	xx += ww1;
	tmp = nWidth - ww1 - ww3;
	cnt = tmp / ww2;
	if( (tmp % ww2) > 0 ) cnt ++;
	for( i = 0; i < cnt; i ++ ){
		atUIAPI_DrawImageStretch(hCanvas, xx, yy, lpTopCenterBM, ww2, nHeight);
		xx += ww2;
	}
	xx = nX + nWidth - ww3;
	atUIAPI_DrawImageStretch(hCanvas, xx, yy, lpTopRightBM, ww3, nHeight);
}

atVOID atUIAPI_DrawClient1Line( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, 
					atHBITMAP lpTopLeftBM, atHBITMAP lpTopCenterBM, atHBITMAP lpTopRightBM )
{
	atINT i;
	atINT cnt, tmp, xx;

	if( hCanvas == atNULL ) return;
	if( nWidth <= 0 ) return;

	if( (lpTopLeftBM == atNULL) || (lpTopCenterBM == atNULL) || (lpTopRightBM == atNULL) ) return;

	// 이미지 사이즈 보다 작을 때 Left 일부분만
	if( nWidth < atUIAPI_GetBitmapWidth(lpTopLeftBM) )
	{
		atUIAPI_DrawImagePart( hCanvas, nX, nY, lpTopLeftBM, 0, 0, nWidth, atUIAPI_GetBitmapHeight(lpTopLeftBM) );
		return ;
	}
	
	// 맨 윗줄을 그린다.
	xx = nX;
	atUIAPI_DrawImage(hCanvas, xx, nY, lpTopLeftBM);
	xx += atUIAPI_GetBitmapWidth(lpTopLeftBM);

	// 이미지 양 사이드의 이미지 보다 작을 때 Center의 일부분만
	if( nWidth < atUIAPI_GetBitmapWidth(lpTopLeftBM) + atUIAPI_GetBitmapWidth(lpTopRightBM) )
	{
		atUIAPI_DrawImagePart( hCanvas, nX+atUIAPI_GetBitmapWidth(lpTopLeftBM), nY, lpTopCenterBM, 0, 0, nWidth-atUIAPI_GetBitmapWidth(lpTopLeftBM), atUIAPI_GetBitmapHeight(lpTopCenterBM) );
		return ;
	}

	tmp = nWidth - atUIAPI_GetBitmapWidth(lpTopLeftBM) - atUIAPI_GetBitmapWidth(lpTopRightBM);
	cnt = tmp / atUIAPI_GetBitmapWidth(lpTopCenterBM);
	if( tmp % atUIAPI_GetBitmapWidth(lpTopCenterBM) > 0 ) cnt ++;
	for( i = 0; i < cnt-1; i++ ){
		atUIAPI_DrawImage(hCanvas, xx, nY, lpTopCenterBM);
		xx += atUIAPI_GetBitmapWidth(lpTopCenterBM);
	}

	if( atUIAPI_GetBitmapWidth(lpTopCenterBM) > (nWidth+nX+1) - (xx+1) - atUIAPI_GetBitmapWidth(lpTopRightBM) ){
			atUIAPI_DrawImagePart( hCanvas, xx, nY, lpTopCenterBM, 0, 0, (nWidth+nX+1) - (xx+1) - atUIAPI_GetBitmapWidth(lpTopRightBM), atUIAPI_GetBitmapHeight(lpTopCenterBM) );
	}else{
		atUIAPI_DrawImage(hCanvas, xx, nY, lpTopCenterBM);
	}

	xx = nX + nWidth - atUIAPI_GetBitmapWidth(lpTopRightBM);
	atUIAPI_DrawImage(hCanvas, xx, nY, lpTopRightBM);
}


atVOID atUIAPI_DrawClient1LineVert( atHGDC hCanvas, atINT nX, atINT nY, atINT nHeight, 
					atHBITMAP lpTopBM, atHBITMAP lpMiddleBM, atHBITMAP lpBottomBM )
{
	atINT i;
	atINT cnt, tmp, yy;

	if( hCanvas == atNULL ) return;
	if( nHeight <= 0 ) return;

	if( (lpTopBM == atNULL) || (lpMiddleBM == atNULL) || (lpBottomBM == atNULL) ) return;
	
	// 맨 윗줄을 그린다.
	yy = nY;
	atUIAPI_DrawImage(hCanvas, nX, yy, lpTopBM);
	yy += atUIAPI_GetBitmapHeight(lpTopBM);
	tmp = nHeight - atUIAPI_GetBitmapHeight(lpTopBM) - atUIAPI_GetBitmapHeight(lpBottomBM);
	cnt = tmp / atUIAPI_GetBitmapHeight(lpMiddleBM);
	if( tmp % atUIAPI_GetBitmapHeight(lpMiddleBM) > 0 ) cnt ++;
	for( i = 0; i < cnt-1; i ++ )
	{
		atUIAPI_DrawImage(hCanvas, nX, yy, lpMiddleBM);
		yy += atUIAPI_GetBitmapHeight(lpMiddleBM);
	}
	
	if( atUIAPI_GetBitmapWidth(lpMiddleBM) > (nHeight+nY+1) - (yy+1) - atUIAPI_GetBitmapHeight(lpBottomBM) ){
			atUIAPI_DrawImagePart( hCanvas, nX, yy, lpMiddleBM, 0, 0, atUIAPI_GetBitmapWidth(lpMiddleBM), (nHeight+nY+1) - (yy+1) - atUIAPI_GetBitmapHeight(lpBottomBM) );
	}else{
		atUIAPI_DrawImage(hCanvas, nX, yy, lpMiddleBM);
	}

	yy = nY + nHeight - atUIAPI_GetBitmapHeight(lpBottomBM);
	atUIAPI_DrawImage(hCanvas, nX, yy, lpBottomBM);
}

atVOID atUIAPI_DrawClient1LineVertStretch( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight, 
					atHBITMAP lpTopBM, atHBITMAP lpMiddleBM, atHBITMAP lpBottomBM )
{
	atINT i;
	atINT cnt, tmp, xx, yy;
	atINT hh1, hh2, hh3;

	if( hCanvas == atNULL ) return;
	if( nHeight <= 0 ) return;

	if( (lpTopBM == atNULL) || (lpMiddleBM == atNULL) || (lpBottomBM == atNULL) ) return;
	
	hh1 = atUIAPI_GetBitmapHeight(lpTopBM);
	hh2 = atUIAPI_GetBitmapHeight(lpMiddleBM);
	hh3 = atUIAPI_GetBitmapHeight(lpBottomBM);

	// 맨 윗줄을 그린다.
	yy = nY;
	xx = nX;
	atUIAPI_DrawImageStretch(hCanvas, xx, yy, lpTopBM, nWidth, hh1);
	yy += atUIAPI_GetBitmapHeight(lpTopBM);
	tmp = nHeight - atUIAPI_GetBitmapHeight(lpTopBM) - atUIAPI_GetBitmapHeight(lpBottomBM);
	cnt = tmp / atUIAPI_GetBitmapHeight(lpMiddleBM);
	if( tmp % atUIAPI_GetBitmapHeight(lpMiddleBM) > 0 ) cnt ++;
	for( i = 0; i < cnt; i ++ )
	{
		atUIAPI_DrawImageStretch(hCanvas, xx, yy, lpMiddleBM, nWidth, hh2);
		yy += atUIAPI_GetBitmapHeight(lpMiddleBM);
	}
	yy = nY + nHeight - atUIAPI_GetBitmapHeight(lpBottomBM);
	atUIAPI_DrawImageStretch(hCanvas, xx, yy, lpBottomBM, nWidth, hh3);
}

atVOID atUIAPI_DrawClient( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight,
					atHBITMAP lpTopLeftBM, atHBITMAP lpTopCenterBM, atHBITMAP lpTopRightBM,
					atHBITMAP lpMiddleLeftBM, atHBITMAP lpMiddleCenterBM, atHBITMAP lpMiddleRightBM,
					atHBITMAP lpBottomLeftBM, atHBITMAP lpBottomCenterBM, atHBITMAP lpBottomRightBM,
					atBOOL IsClientDraw )
{
	atINT i, j;
	atINT cnt, cnt2, c_cnt, c_cnt2, tmp, xx, yy;
	atINT	hTOP, hCENTER, hBOTTOM;

	if( hCanvas == atNULL ) return;

	if( (lpTopLeftBM == atNULL) || (lpTopCenterBM == atNULL) || (lpTopRightBM == atNULL) ||
		(lpMiddleLeftBM == atNULL) || (lpMiddleCenterBM == atNULL) || (lpMiddleRightBM == atNULL) ||
		(lpBottomLeftBM == atNULL) || (lpBottomCenterBM == atNULL) || (lpBottomRightBM == atNULL) ) return;
		

	hTOP	= atUIAPI_GetBitmapHeight(lpTopLeftBM);
	hCENTER	= atUIAPI_GetBitmapHeight(lpMiddleLeftBM);
	hBOTTOM	= atUIAPI_GetBitmapHeight(lpBottomLeftBM);

	// 맨 윗줄을 그린다.
	yy = nY;
	xx = nX;
	atUIAPI_DrawImage(hCanvas, xx, yy, lpTopLeftBM);
	xx += atUIAPI_GetBitmapWidth(lpTopLeftBM);
	tmp = nWidth - atUIAPI_GetBitmapWidth(lpTopLeftBM) - atUIAPI_GetBitmapWidth(lpTopRightBM);
	cnt = tmp / atUIAPI_GetBitmapWidth(lpTopCenterBM);
//	if( tmp % atUIAPI_GetBitmapWidth(lpTopCenterBM) > 0 ) cnt ++;
	cnt2 = ( tmp % atUIAPI_GetBitmapWidth(lpTopCenterBM) );
	for( i = 0; i < cnt; i ++ ){
		atUIAPI_DrawImage(hCanvas, xx, yy, lpTopCenterBM);
		xx += atUIAPI_GetBitmapWidth(lpTopCenterBM);
	}
	if( cnt2 > 0 ){
		atUIAPI_DrawImagePart(hCanvas, xx, yy, lpTopCenterBM, 0, 0, cnt2, atUIAPI_GetBitmapHeight(lpTopCenterBM) );
	}
	xx = nX + nWidth - atUIAPI_GetBitmapWidth(lpTopRightBM);
	atUIAPI_DrawImage(hCanvas, xx, yy, lpTopRightBM);

	// 중간 영역을 그린다.
	yy += hTOP;
	tmp = (nHeight - hTOP - hBOTTOM);
	c_cnt = tmp / hCENTER;
//	if( tmp % hCENTER > 0 ) c_cnt ++;
	c_cnt2 = ( tmp % hCENTER );
	tmp = nWidth - atUIAPI_GetBitmapWidth(lpMiddleLeftBM) - atUIAPI_GetBitmapWidth(lpMiddleRightBM);
	cnt = tmp / atUIAPI_GetBitmapWidth(lpMiddleCenterBM);
//	if( tmp % atUIAPI_GetBitmapWidth(lpMiddleCenterBM) > 0 ) cnt ++;
	cnt2 = ( tmp % atUIAPI_GetBitmapWidth(lpMiddleCenterBM) );
	for( i = 0; i < c_cnt; i++ ){
		xx = nX;
		atUIAPI_DrawImage(hCanvas, xx, yy, lpMiddleLeftBM);
		xx += atUIAPI_GetBitmapWidth(lpMiddleLeftBM);
		if( IsClientDraw ){
			for( j = 0; j < cnt; j ++ ){
				atUIAPI_DrawImage(hCanvas, xx, yy, lpMiddleCenterBM);
				xx += atUIAPI_GetBitmapWidth(lpMiddleCenterBM);
			}
			if( cnt2 > 0 ){
				atUIAPI_DrawImagePart(hCanvas, xx, yy, lpMiddleCenterBM, 0, 0, cnt2, atUIAPI_GetBitmapHeight(lpMiddleCenterBM) );
			}
		}
		xx = nX + nWidth - atUIAPI_GetBitmapWidth(lpMiddleRightBM);
		atUIAPI_DrawImage(hCanvas, xx, yy, lpMiddleRightBM);
		yy += hCENTER;
	}
	if( c_cnt2 > 0 ){
		xx = nX;
		atUIAPI_DrawImagePart(hCanvas, xx, yy, lpMiddleLeftBM, 0, 0, atUIAPI_GetBitmapWidth(lpMiddleLeftBM), c_cnt2);
		xx += atUIAPI_GetBitmapWidth(lpMiddleLeftBM);
		if( IsClientDraw ){
			for( j = 0; j < cnt; j ++ ){
				atUIAPI_DrawImagePart(hCanvas, xx, yy, lpMiddleCenterBM, 0, 0, atUIAPI_GetBitmapWidth(lpMiddleCenterBM), c_cnt2);
				xx += atUIAPI_GetBitmapWidth(lpMiddleCenterBM);
			}
			if( cnt2 > 0 ){
				atUIAPI_DrawImagePart(hCanvas, xx, yy, lpMiddleCenterBM, 0, 0, cnt2, c_cnt2 );
			}
		}
		xx = nX + nWidth - atUIAPI_GetBitmapWidth(lpMiddleRightBM);
		atUIAPI_DrawImagePart(hCanvas, xx, yy, lpMiddleRightBM, 0, 0, atUIAPI_GetBitmapWidth(lpMiddleRightBM), c_cnt2);
		yy += hCENTER;
	}

	// 맨 아랫줄을 그린다.
	yy = nY + nHeight - hBOTTOM;
	xx = nX;
	atUIAPI_DrawImage(hCanvas, xx, yy, lpBottomLeftBM);
	xx += atUIAPI_GetBitmapWidth(lpBottomLeftBM);
	tmp = nWidth - atUIAPI_GetBitmapWidth(lpBottomLeftBM) - atUIAPI_GetBitmapWidth(lpBottomRightBM);
	cnt = tmp / atUIAPI_GetBitmapWidth(lpBottomCenterBM);
//	if( tmp % atUIAPI_GetBitmapWidth(lpBottomCenterBM) > 0 ) cnt ++;
	cnt2 = ( tmp % atUIAPI_GetBitmapWidth(lpBottomCenterBM) );
	for( i = 0; i < cnt; i ++ ){
		atUIAPI_DrawImage(hCanvas, xx, yy, lpBottomCenterBM);
		xx += atUIAPI_GetBitmapWidth(lpBottomCenterBM);
	}
	if( cnt2 > 0 ){
		atUIAPI_DrawImagePart(hCanvas, xx, yy, lpBottomCenterBM, 0, 0, cnt2, atUIAPI_GetBitmapHeight(lpBottomCenterBM) );
	}
	xx = nX + nWidth - atUIAPI_GetBitmapWidth(lpBottomRightBM);
	atUIAPI_DrawImage(hCanvas, xx, yy, lpBottomRightBM);
}


atVOID atUIAPI_DrawClient2( atHGDC hCanvas, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atLAYOUTBITMAP *lpLayoutBitmap, atBOOL IsClientDraw )
{
	atUIAPI_DrawClient( hCanvas, nX, nY, nWidth, nHeight,
                                   lpLayoutBitmap->bmTopLeft, lpLayoutBitmap->bmTopCenter, lpLayoutBitmap->bmTopRight,
                                   lpLayoutBitmap->bmMiddleLeft, lpLayoutBitmap->bmMiddleCenter, lpLayoutBitmap->bmMiddleRight,
                                   lpLayoutBitmap->bmBottomLeft, lpLayoutBitmap->bmBottomCenter, lpLayoutBitmap->bmBottomRight,
					IsClientDraw	);
}

