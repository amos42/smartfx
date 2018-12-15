#include "SA_Types.h"
#include "SYSAPI.h"
//#include "SA_Util.h"
#include "OEM_SA_Scr.h"
#include <stdio.h>


#ifdef __USES_SYSAPI_DISPLAY_


#define RGB24TORGB16(_X_)   (unsigned short)((((unsigned long)(_X_) & 0x00F80000L) >> 8) | (((unsigned long)(_X_) & 0x0000FC00L) >> 5) | (((unsigned long)(_X_) & 0x000000F8L) >> 3))
#define RGB16TORGB24(_X_)   (unsigned short)((((unsigned long)(_X_) & 0x0000F800L) << 8) | (((unsigned long)(_X_) & 0x000007E0L) << 5) | (((unsigned long)(_X_) & 0x0000001FL) << 3))
#define RGB24ITORGB16(_X_)   (unsigned short)((((unsigned long)(_X_) & 0x00F80000L) >> 19) | (((unsigned long)(_X_) & 0x0000FC00L) >> 5) | (((unsigned long)(_X_) & 0x000000F8L) << 8))
#define RGB16TORGB24I(_X_)   (unsigned short)((((unsigned long)(_X_) & 0x0000F800L) >> 8) | (((unsigned long)(_X_) & 0x000007E0L) << 5) | (((unsigned long)(_X_) & 0x0000001FL) << 11))


static atBOOL g_bSysDispIntitialized = atFALSE;

static OEM_SYSAPI_SCR_T tOemFn;


atVOID OEM_SYSAPI_SCR_SetOemFn(OEM_SYSAPI_SCR_T *fnOem)
{
	tOemFn = *fnOem;
}


atVOID OEM_SYSAPI_SCR_GetOemFn(OEM_SYSAPI_SCR_T *fnOem)
{
	*fnOem = tOemFn;
}


atDWORD   SYSAPI_GetDisplayCaps( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2, SYS_DISPLAY_CAPS *lpScrCaps )
{
	if( tOemFn.fnGetDisplayCaps != atNULL ){
		return tOemFn.fnGetDisplayCaps( lSysInst, lInitData1, lInitData2, lpScrCaps );
	}

	return 0x0000;
}

atBOOL SYSAPI_InitDisplay( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	if( g_bSysDispIntitialized ) return atTRUE;
	
	if(tOemFn.fnInitDisplay != atNULL ){
		g_bSysDispIntitialized = tOemFn.fnInitDisplay( lSysInst, lInitData1, lInitData2 );
	}
	
	return g_bSysDispIntitialized;
}

atVOID SYSAPI_FinalDisplay( NOARGS )
{
	if( !g_bSysDispIntitialized ) return;
	
	if(tOemFn.fnFinalDisplay != atNULL ){
		tOemFn.fnFinalDisplay();
	}
	
	g_bSysDispIntitialized = atFALSE;
}

atBOOL SYSAPI_SetDisplayResolution( atINT nWidth, atINT nHeight, atINT nFrameWidth )
{
	if(tOemFn.fnSetDisplayResolution != atNULL ){
		return tOemFn.fnSetDisplayResolution( nWidth, nHeight, nFrameWidth );
	}

	return atFALSE;
}


SYS_COLOR SYSAPI_MakeColor(atBYTE r, atBYTE g, atBYTE b)
{
	if(tOemFn.fnMakeColor != atNULL ){
		return tOemFn.fnMakeColor( r, g, b );
	}

	return (SYS_COLOR)0x0000;
}

atBYTE SYSAPI_GetRValue(SYS_COLOR nColor)
{
	if(tOemFn.fnGetRValue != atNULL ){
		return tOemFn.fnGetRValue( nColor );
	}

	return 0x00;
}

atBYTE SYSAPI_GetGValue(SYS_COLOR nColor)
{
	if(tOemFn.fnGetGValue != atNULL ){
		return tOemFn.fnGetGValue( nColor );
	}

	return 0x00;
}

atBYTE SYSAPI_GetBValue(SYS_COLOR nColor)
{
	if(tOemFn.fnGetBValue != atNULL ){
		return tOemFn.fnGetBValue( nColor );
	}

	return 0x00;
}


atINT SYSAPI_GetScreenWidth( NOARGS )
{
	if(tOemFn.fnGetScreenWidth != atNULL ){
		return tOemFn.fnGetScreenWidth();
	}

	return 0;
}

atINT SYSAPI_GetScreenHeight( NOARGS )
{
	if(tOemFn.fnGetScreenHeight != atNULL ){
		return tOemFn.fnGetScreenHeight();
	}

	return 0;
}

atINT SYSAPI_GetScreenBPP( NOARGS )
{
	if(tOemFn.fnGetScreenBPP != atNULL ){
		return tOemFn.fnGetScreenBPP();
	}

	return 0;
}


atINT SYSAPI_GetFrameVMemWidth( NOARGS )
{
	if(tOemFn.fnGetFrameVMemWidth != atNULL ){
		return tOemFn.fnGetFrameVMemWidth();
	}

	return 0;
}

atLPVOID SYSAPI_GetFrameBufferPtr( NOARGS )
{
	if(tOemFn.fnGetFrameBufferPtr != atNULL ){
		return tOemFn.fnGetFrameBufferPtr();
	}

	return atNULL;
}


SYS_COLOR SYSAPI_ConvColorFrom24BPP( SYS_COLOR nColor )
{
	if( SYSAPI_GetScreenBPP() == 24 )
		return nColor;
	else if( SYSAPI_GetScreenBPP() == 16 ){
		return RGB24TORGB16(nColor);
	} else {
		// 老窜 公矫...
		atBYTE r, g, b;
		r = 0;
		g = 0;
		b = 0;
		return SYSAPI_MakeColor(r,g,b);
	}
}


SYS_COLOR SYSAPI_ConvColorFrom24iBPP( SYS_COLOR nColor )
{
	if( SYSAPI_GetScreenBPP() == 24 )
		return ((nColor >> 16) & 0x0000FF) | ((nColor << 16) & 0xFF0000) | (nColor & 0x00FF00);
	else if( SYSAPI_GetScreenBPP() == 16 ){
		return RGB24ITORGB16(nColor);
	} else {
		// 老窜 公矫...
		atBYTE r, g, b;
		r = 0;
		g = 0;
		b = 0;
		return SYSAPI_MakeColor(r,g,b);
	}
}


SYS_COLOR SYSAPI_ConvColorFrom16BPP(SYS_COLOR nColor)
{
	if( SYSAPI_GetScreenBPP() == 16 )
		return nColor;
	else if( SYSAPI_GetScreenBPP() == 24 ){
		return RGB16TORGB24(nColor);
	} else {
		// 老窜 公矫...
		atBYTE r, g, b;
		r = 0;
		g = 0;
		b = 0;
		return SYSAPI_MakeColor(r,g,b);
	}
}


SYS_COLOR SYSAPI_ConvColorTo24BPP( SYS_COLOR nColor )
{
	if( SYSAPI_GetScreenBPP() == 24 )
		return nColor;
	else if( SYSAPI_GetScreenBPP() == 16 ){
		return RGB16TORGB24(nColor);
	} else {
		// 老窜 公矫...
		atBYTE r, g, b;
		r = 0;
		g = 0;
		b = 0;
		return SYSAPI_MakeColor(r,g,b);
	}
}


SYS_COLOR SYSAPI_ConvColorTo24iBPP( SYS_COLOR nColor )
{
	if( SYSAPI_GetScreenBPP() == 24 )
		return ((nColor >> 16) & 0x0000FF) | ((nColor << 16) & 0xFF0000) | (nColor & 0x00FF00);
	else if( SYSAPI_GetScreenBPP() == 16 ){
		return RGB16TORGB24I(nColor);
	} else {
		// 老窜 公矫...
		atBYTE r, g, b;
		r = 0;
		g = 0;
		b = 0;
		return SYSAPI_MakeColor(r,g,b);
	}
}


SYS_COLOR SYSAPI_ConvColorTo16BPP(SYS_COLOR nColor)
{
	if( SYSAPI_GetScreenBPP() == 16 )
		return nColor;
	else if( SYSAPI_GetScreenBPP() == 24 ){
		return RGB24TORGB16(nColor);
	} else {
		// 老窜 公矫...
		atBYTE r, g, b;
		r = 0;
		g = 0;
		b = 0;
		return SYSAPI_MakeColor(r,g,b);
	}
}


atVOID	SYSAPI_FlushScreenRgn( atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{  	
	if( nX < 0 ){
		nWidth += nX;
		nX = 0;
	}
	if( nY < 0 ){
		nHeight += nY;
		nY = 0;
	}

	if(tOemFn.fnGetScreenWidth != atNULL ){
		if( (nX + nWidth) > tOemFn.fnGetScreenWidth() ){
			nWidth = tOemFn.fnGetScreenWidth() - nX;
		}
	}

	if(tOemFn.fnGetScreenHeight != atNULL ){
		if( (nY + nHeight) > tOemFn.fnGetScreenHeight() ){
			nHeight = tOemFn.fnGetScreenHeight() - nY;
		}
	}

	if(tOemFn.fnFlushScreenRgn != atNULL ){
		tOemFn.fnFlushScreenRgn(nX, nY, nWidth, nHeight); 
	}
}


atVOID	SYSAPI_FlushScreen( NOARGS )
{
	SYSAPI_FlushScreenRgn( 0, 0, SYSAPI_GetScreenWidth(), SYSAPI_GetScreenHeight() );
}


atBOOL SYSAPI_BeginDraw( NOARGS )
{
	if(tOemFn.fnBeginDraw != atNULL ){
		return tOemFn.fnBeginDraw();
	}

	return atFALSE;
}

atVOID SYSAPI_EndDraw( NOARGS )
{
	if(tOemFn.fnEndDraw != atNULL ){
		tOemFn.fnEndDraw();
	}
}


atLONG   SYSAPI_ExtGraphicMethod( atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
	if(tOemFn.fnExtGraphicMethod != atNULL ){
		return tOemFn.fnExtGraphicMethod( func_no, param1, param2, param3, param4, param5, param6 );
	}

	return 0;
}


#endif

