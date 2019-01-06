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

