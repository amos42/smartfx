 //
// Canvas_wipi.c
//
////////////////////////////////////////////////////////////////

#include "OEM_SA_Scr.h"
#include "OEM_SA_Mem.h"
#include "SYSAPI.h"
#include "GrpX.h"

#define MAKE565(r,g,b) ((atWORD)(((((atWORD)((r) & 0xF8)) << (5+6-3))) | \
					   ((((atWORD)((g) & 0xFC)) << (5-2))) | \
					   (((atWORD)((b) & 0xF8)) >> 3)))
#define GET565R(cl)    ((atBYTE)(((cl) >> 8) & 0xF8))
#define GET565G(cl)    ((atBYTE)(((cl) >> 3) & 0xFC))
#define GET565B(cl)    ((atBYTE)(((cl) << 3) & 0xF8))


static int		g_nScreenWidth	= 0;
static int		g_nScreenHeight	= 0;
static int		g_nScreenBPP	= 0;
static unsigned short  * g_pScreenBuffer = NULL;
static int		g_nScreenVMemWidth	= 0;
static int		g_nScreenBufferSize = 0;
static unsigned short *g_addr = (unsigned short *)0xc0000000;

static int g_NowDrawing = 0;

// Canvas의 프레임 버퍼를 화면에 표시한다.

extern void UB_Graphic2D_CopyImgDMA2(unsigned short *addr, int sx, int sy, int width, int height );

static void _FlushToScreen( unsigned short *addr, int sx, int sy, int width, int height )
{
	
	int i,j;
	unsigned short *src;
	int skip;

	//SYSAPI_DebugPrintf(_AT("_FlushToScreen(%x, %d,%d,%d,%d)\r\n"), addr, sx, sy, width, height);
	
	if( g_pScreenBuffer == atNULL ){
	   SYSAPI_DebugPrintf(_AT("[error] Invalide Screen Buffer\r\n"));
		return;
	}

	if(!addr){
	   SYSAPI_DebugPrintf(_AT("[error] Invalide video addr\r\n"));
	    return;
	}


//	{
//		unsigned short *addr2 = addr;
//		for(i = 0; i < 480; i++)
//		{
//			for( j = 0; j < 320; j++)
//			{
//				*addr2++ = 0;
//			}
//		}
//   }
	
	
//	if(sx == 0 && sy == 0 && width == g_nScreenWidth && height == g_nScreenHeight) {


		src = g_pScreenBuffer;// + sy * g_nScreenWidth + sx;
		//UB_Graphic2D_CopyImgDMA2(src, 0, 0, 320, 480);
		UB_Graphic2D_CopyImgDMA2(src, sx, sy, width, height);
// } else {
//		src = g_pScreenBuffer + sy * g_nScreenWidth + sx;
//		addr = addr + sy * g_nScreenWidth + sx;
//		skip = g_nScreenWidth - width;
//		
//		for(i = 0; i < height; i++)
//		{
//			for( j = 0; j < width; j++)
//			{
//				*addr++ = *src++;
//			}
//			addr += skip;
//			src += skip;
//		}
//	}
	//SYSAPI_DebugPrintf(_AT("_FlushToScreen ...done\r\n"));
}



atDWORD OEM_SYSAPI_GetDisplayCaps( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2, SYS_DISPLAY_CAPS *lpScrCaps )
{
	return 0;
}

atBOOL OEM_SYSAPI_InitDisplay( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	int *vv = (int *)lInitData1;

	SYSAPI_DebugPrintf(_AT("OEM_SYSAPI_InitDisplay\r\n"));

	if( vv == atNULL ) return atFALSE;

	g_nScreenWidth     = vv[0];
	g_nScreenHeight    = vv[1];
	g_nScreenBPP       = vv[2];
	g_pScreenBuffer    = (atVOID *)vv[3];
	g_nScreenVMemWidth = vv[4];
	
	if(g_pScreenBuffer == atNULL) {
		SYSAPI_DebugPrintf(_AT("Alloc ScreenBuffer...:%d\r\n"), g_nScreenWidth * g_nScreenHeight * g_nScreenBPP/8 );
		g_pScreenBuffer = SYSAPI_AllocMem( g_nScreenWidth * g_nScreenHeight * g_nScreenBPP/8 );
	}

	SYSAPI_DebugPrintf(_AT("buf_addr ptr:%x\r\n"), g_pScreenBuffer);
	SYSAPI_DebugPrintf(_AT("width:%d,height:%d,bpp:%d\r\n"), g_nScreenWidth, g_nScreenHeight, g_nScreenBPP);
	//g_SYS_WndProc_Paint = WndProc;

	g_nScreenBufferSize = g_nScreenVMemWidth * g_nScreenHeight * g_nScreenBPP/8;

	if (!g_pScreenBuffer)
	{	
		g_pScreenBuffer = SYSAPI_AllocMem( g_nScreenBufferSize );
		SYSAPI_DebugPrintf(_AT("re : Alloc ScreenBuffer (%d bytes)\r\n"), g_nScreenBufferSize);
		if (!g_pScreenBuffer) {
			SYSAPI_DebugPrintf(_AT("Alloc Fail\r\n"));
			return atFALSE;
		}
		
		SYSAPI_DebugPrintf(_AT("Memory set\r\n"));
		SYSAPI_memset( g_pScreenBuffer, 0, g_nScreenBufferSize );
	} else {
		SYSAPI_DebugPrintf(_AT("Alloc Success:%x\r\n"), g_nScreenBufferSize);
		SYSAPI_memset( g_pScreenBuffer, 0, g_nScreenBufferSize );
	}

	return atTRUE;
}

void OEM_SYSAPI_FinalDisplay(void)
{
	SYSAPI_DebugPrintf(_AT("OEM_SYSAPI_FinalDisplay\n"));

	//g_SYS_WndProc_Paint = atNULL;

	if( g_pScreenBuffer ){
		SYSAPI_FreeMem( g_pScreenBuffer );
		g_pScreenBuffer = atNULL;
		//g_nScreenBufferSize = 0;
	}

	g_pScreenBuffer = atNULL;
	g_nScreenBufferSize = 0;
}

atBOOL OEM_SYSAPI_SetDisplayResolution( atINT nWidth, atINT nHeight, atINT nFrameWidth )
{

	int new_size;

	g_nScreenWidth = nWidth;
	g_nScreenHeight = nHeight;
	g_nScreenVMemWidth = nFrameWidth;

	/**
	
	new_size = g_nScreenVMemWidth * g_nScreenHeight * g_nScreenBPP/8;

	if( new_size > g_nScreenBufferSize ){
		SYSAPI_FreeMem( g_pScreenBuffer );
		g_pScreenBuffer = (atSHORT *)SYSAPI_AllocMem( new_size );
		g_nScreenBufferSize = new_size;
	}
	**/

	return atTRUE;
}

atINT OEM_SYSAPI_GetScreenWidth( NOARGS )
{
	return g_nScreenWidth;
}

atINT OEM_SYSAPI_GetScreenHeight( NOARGS )
{
	return g_nScreenHeight;
}

atINT OEM_SYSAPI_GetScreenBPP( NOARGS )
{
	return g_nScreenBPP;
}

atVOID * OEM_SYSAPI_GetFrameBufferPtr( NOARGS )
{
	return g_pScreenBuffer;
}

atINT OEM_SYSAPI_GetFrameVMemWidth( NOARGS )
{
	return g_nScreenVMemWidth;
}


atVOID OEM_SYSAPI_FlushScreenRgn( atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	SYSAPI_DebugPrintf(_AT("OEM_SYSAPI_FlushScreenRgn(%d,%d,%d,%d)\r\n"), nX, nY, nWidth, nHeight);
	if( g_pScreenBuffer == NULL ) return;

	//if( g_NowDrawing ) return;

	_FlushToScreen( g_addr, nX, nY, nWidth, nHeight );
	//_FlushToScreen( g_addr, 0, 0, g_nScreenWidth, g_nScreenHeight);
}


atULONG OEM_SYSAPI_MakeColor(atBYTE r, atBYTE g, atBYTE b)
{
	return MAKE565( r,g,b );
}

atBYTE OEM_SYSAPI_GetRValue(atULONG nColor)
{
	return GET565R( nColor );
}

atBYTE OEM_SYSAPI_GetGValue(atULONG nColor)
{
	return GET565G( nColor );
}

atBYTE OEM_SYSAPI_GetBValue(atULONG nColor)
{
	return GET565B( nColor );
}


atBOOL OEM_SYSAPI_BeginDraw( NOARGS )
{
	//SYSAPI_EnterCriticalSection( g_SYS_ScrCrSection );
	g_NowDrawing = 1;
	return atTRUE;
}

atVOID OEM_SYSAPI_EndDraw( NOARGS )
{
	g_NowDrawing = 0;
	//SYSAPI_LeaveCriticalSection( g_SYS_ScrCrSection );
}

atLONG OEM_SYSAPI_ExtGraphicMethod( atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
    switch( func_no ){
		/**
		case 10     :
			g_hCameraDC =  (HDC)param1;
			break;
		**/
        case 0x1000 : 
        		     break;
		default : 
			break;
    }
            
    return 0;
}

