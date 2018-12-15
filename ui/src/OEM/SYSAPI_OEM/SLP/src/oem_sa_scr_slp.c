 //
// Canvas_wipi.c
//
////////////////////////////////////////////////////////////////

#include "OEM_SA_Scr.h"
#include "OEM_SA_Mem.h"
#include "SYSAPI.h"

#include <gtk/gtk.h>


#define MAKE565(r,g,b) ((atWORD)(((((atWORD)((r) & 0xF8)) << (5+6-3))) | \
					   ((((atWORD)((g) & 0xFC)) << (5-2))) | \
					   (((atWORD)((b) & 0xF8)) >> 3)))
#define GET565R(cl)    ((atBYTE)(((cl) >> 8) & 0xF8))
#define GET565G(cl)    ((atBYTE)(((cl) >> 3) & 0xFC))
#define GET565B(cl)    ((atBYTE)(((cl) << 3) & 0xF8))

extern void *	g_SYS_hWindow;

static int		g_nScreenWidth	= 0;
static int		g_nScreenHeight	= 0;
static int		g_nScreenBPP	= 0;
static short  * g_pScreenBuffer = atNULL;
static unsigned char *g_pScreenBufferT = atNULL;
static int		g_nScreenVMemWidth	= 0;
static int		g_nScreenBufferSize = 0;

static int		g_nScreenTWidth	= 0;
static int		g_nScreenTHeight	= 0;
static int		g_nScreenTVMemWidth	= 0;
static int		g_nScreenBufferTSize = 0;

int		g_SYS_nScale;


atDWORD OEM_SYSAPI_GetDisplayCaps( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return 0;
}

atBOOL OEM_SYSAPI_InitDisplay( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	int *vv = (int *)lInitData1;

	if( vv == atNULL ) return atFALSE;

	g_nScreenWidth = vv[0];
	g_nScreenHeight = vv[1];
	g_nScreenBPP = vv[2];
//	g_pScreenBuffer = (atVOID *)vv[3];
	g_nScreenVMemWidth = vv[4];

	g_nScreenBufferSize = g_nScreenVMemWidth * g_nScreenHeight * g_nScreenBPP/8;

	g_pScreenBuffer = SYSAPI_AllocMem( g_nScreenBufferSize );
	if( g_pScreenBuffer == atNULL ) return atFALSE;

	SYSAPI_memset( g_pScreenBuffer, 0, g_nScreenBufferSize );

	g_SYS_nScale = 2;
	g_nScreenTWidth = vv[0] * g_SYS_nScale;
	g_nScreenTHeight = vv[1] * g_SYS_nScale;
	g_nScreenTVMemWidth = vv[4] * g_SYS_nScale;
	g_nScreenBufferTSize = g_nScreenTVMemWidth * g_nScreenTHeight * 3;

	g_pScreenBufferT = SYSAPI_AllocMem( g_nScreenBufferTSize );
	if( g_pScreenBufferT == atNULL ) return atFALSE;

	SYSAPI_memset( g_pScreenBufferT, 0xff, g_nScreenBufferTSize );

	return atTRUE;
}

void OEM_SYSAPI_FinalDisplay(void)
{
	if( g_pScreenBuffer ){
		SYSAPI_FreeMem( g_pScreenBuffer );

		g_pScreenBuffer = atNULL;
		g_nScreenBufferSize = 0;
	}

	if( g_pScreenBufferT ){
		SYSAPI_FreeMem( g_pScreenBufferT );

		g_pScreenBufferT = atNULL;
		g_nScreenBufferTSize = 0;
	}
}

atBOOL OEM_SYSAPI_SetDisplayResolution( atINT nWidth, atINT nHeight, atINT nFrameWidth )
{
	int new_size;

	g_nScreenWidth = nWidth;
	g_nScreenHeight = nHeight;
	g_nScreenVMemWidth = nFrameWidth;

	new_size = g_nScreenVMemWidth * g_nScreenHeight * g_nScreenBPP/8;

	if( new_size > g_nScreenBufferSize ){
		if( g_pScreenBuffer ){
			SYSAPI_FreeMem( g_pScreenBuffer );
			g_pScreenBuffer = NULL;
		}

		g_pScreenBuffer = (atSHORT *)SYSAPI_AllocMem( new_size );
		if( g_pScreenBuffer != atNULL )
			g_nScreenBufferSize = new_size;

		if( g_pScreenBufferT )
		{
			SYSAPI_FreeMem( g_pScreenBufferT );
			g_pScreenBufferT = NULL;
		}

		g_nScreenTWidth = nWidth * g_SYS_nScale;
		g_nScreenTVMemWidth = nFrameWidth * g_SYS_nScale;
		g_nScreenTHeight = nHeight * g_SYS_nScale;
		new_size = g_nScreenTVMemWidth * g_nScreenTHeight * 3;

		if(new_size && new_size != g_nScreenBufferTSize)
		{
			g_pScreenBufferT = (atUCHAR *)SYSAPI_AllocMem( new_size );
			if(g_pScreenBufferT != atNULL)
				g_nScreenBufferTSize = new_size;

		}
	}

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


typedef struct _tagFlushInfo {
	GdkDrawable *drawable;
	GdkGC *gc;
	guchar *flush_pos;
	int nX, nY;
	int nW, nH;
} _FlushInfo;
static _FlushInfo g_info;

gboolean _FlushScreenRgn(gpointer data)
{
	_FlushInfo *pInfo = (_FlushInfo*)data;
	if(pInfo != NULL && pInfo->flush_pos != NULL)
	{
		gdk_threads_enter();
		gdk_draw_rgb_image(pInfo->drawable, pInfo->gc,
								 pInfo->nX, pInfo->nY, pInfo->nW, pInfo->nH,
								 GDK_RGB_DITHER_NONE, pInfo->flush_pos, g_nScreenTVMemWidth * 3);
		gdk_flush();
		gdk_threads_leave();
	}

	return 0;
}

atVOID OEM_SYSAPI_FlushScreenRgn( atINT nX, atINT nY, atINT nWidth, atINT nHeight )
{
	GtkWidget *form = (GtkWidget*)g_SYS_hWindow;
	GdkDrawable *drawable = GDK_DRAWABLE(form->window);
	GdkGC* gc = gdk_gc_new(drawable);

	//gushort *src = g_pScreenBuffer;
	//guchar *dest = (guchar*)g_pScreenBufferT;
	gushort *src;
	guchar *dest, *pos;
	guchar *destT = NULL;
	guchar _red, _green, _blue;
	int x, y, z;
	int sx, sy, sw, sh;
	int ds_offset;

	if( g_pScreenBuffer == atNULL || g_pScreenBufferT == atNULL ) return;
	if( form == atNULL) return;

	sx = nX * g_SYS_nScale;
	sy = nY * g_SYS_nScale;
	sw = nWidth * g_SYS_nScale;
	sh = nHeight * g_SYS_nScale;

	ds_offset = sy * g_nScreenTVMemWidth * 3 + sx * 3;
	src = g_pScreenBuffer + (nY * g_nScreenVMemWidth + nX);
	dest = (guchar*)g_pScreenBufferT + ds_offset;
	pos = dest;

	for (y = 0; y < nHeight ; y++)
	{
		destT = dest;
		for (x = 0; x < nWidth ; x++)
		{
			// X Line
			for(z = 0 ; z < g_SYS_nScale ; z++ )
			{
				_red = (*src & 0xF800) >> 11;
				_green = (*src & 0x7E0) >> 5;
				_blue = (*src & 0x1F);

				*dest++ = _red << 3;
				*dest++ = _green << 2;
				*dest++ = _blue << 3;
			}

			src++;
		}

		// Y Line
		for(z = 1 ; z < g_SYS_nScale ; z++ )
		{
			SYSAPI_memcpy(dest, destT, sw * 3);
			dest += sw * 3;
		}

		src += g_nScreenVMemWidth - nWidth;
		dest += (g_nScreenTVMemWidth - sw) * 3;
	}

	//pos = &g_pScreenBufferT[ds_offset];
	//gdk_draw_rgb_image(drawable, gc, nX, nY, nWidth, nHeight, GDK_RGB_DITHER_NONE,
	//						 pos, g_nScreenVMemWidth * 3);

	//g_printf("%d, %d, %d, %d - %d, %d, %d, %d\n", nX, nY, nWidth, nHeight, sx, sy, sw, sh);

	g_info.drawable= drawable;
	g_info.gc = gc;
	g_info.flush_pos = pos;
	g_info.nX = sx;
	g_info.nY = sy;
	g_info.nW = sw;
	g_info.nH = sh;
	g_idle_add((GSourceFunc)_FlushScreenRgn, &g_info);
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


atLONG OEM_SYSAPI_ExtGraphicMethod( atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 )
{
    switch( func_no ){
        case 0x1000 :
        		     break;
        case 0x8001 : g_SYS_nScale = param1;
        		     break;
    }

    return 0;
}

