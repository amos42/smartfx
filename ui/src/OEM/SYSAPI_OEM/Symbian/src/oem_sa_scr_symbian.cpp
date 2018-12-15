 //
// Canvas_wipi.c
//
////////////////////////////////////////////////////////////////
#include "SYSAPI.h"
#include "OEM_SA_Scr.h"
#include "OEM_SA_Mem.h"

//#include "AEEEmulAPI.h"
//#include <string.h>
#include <fbs.h>
#include <bitstd.h>//for CFbsBitGc
#include <bitdev.h>//for CFbsBitmapDevice
#include <W32STD.H> 
#include <eikscrlb.h> 


#define MAKE565(r,g,b) ((atWORD)(((((atWORD)((r) & 0xF8)) << (5+6-3))) | \
					   ((((atWORD)((g) & 0xFC)) << (5-2))) | \
					   (((atWORD)((b) & 0xF8)) >> 3)))
#define GET565R(cl)    ((atBYTE)(((cl) >> 8) & 0xF8))
#define GET565G(cl)    ((atBYTE)(((cl) >> 3) & 0xFC))
#define GET565B(cl)    ((atBYTE)(((cl) << 3) & 0xF8))


static atINT		g_nScreenWidth	= 0;
static atINT		g_nScreenHeight	= 0;
static atINT		g_nScreenBPP	= 0;
static atSHORT     *g_pScreenBuffer = atNULL;
static atINT		g_nScreenVMemWidth	= 0;

// View 
// by chales
static CCoeControl *g_lpAppView   = atNULL;

// Symbian Gc
// by chales
static CFbsScreenDevice* ScreenDev    = atNULL;
static CFbsBitGc* BitGc = atNULL;
static CFbsBitmap* g_pOffScreenBitmap = atNULL; 

static atSHORT g_pFrameBuffer[480*272];



class CAutoFlushLock
{
	int m_iIng;
public:
	static CAutoFlushLock* instance();
	
	CAutoFlushLock ()
	{
		m_iIng = 0;
	}
	virtual ~CAutoFlushLock(){
		m_iIng = 0;
	}
	void Set(int _iVal)
	{
		m_iIng = _iVal;
	}
	int Get () { return m_iIng; }
};
	
CAutoFlushLock* CAutoFlushLock::instance()
{
	static CAutoFlushLock instance;
	return &instance;
}


static atVOID _FlushToDC(long dc, atINT sx, atINT sy, atINT width, atINT height)
{
	CWindowGc* pGc = (CWindowGc*)dc;
	if(!pGc) return;
	
	pGc->Clear();
	
	if( g_pOffScreenBitmap == atNULL ) return;
	if( g_pScreenBuffer == atNULL ) return;
	
	atUCHAR* pBits = (atUCHAR*)g_pScreenBuffer;

	atUCHAR* bitmap_add = (atUCHAR*)(g_pOffScreenBitmap->DataAddress());
	if (bitmap_add == atNULL )return;
	
	TSize bitmap_size = g_pOffScreenBitmap->SizeInPixels();
	TInt pixel_size   = g_nScreenBPP / 8;
	TInt stride       = g_pOffScreenBitmap->ScanLineLength(bitmap_size.iWidth, g_pOffScreenBitmap->DisplayMode());
	
	
	TInt size_sx      = pixel_size * sx;
	TInt size_width   = pixel_size * width;
	TInt offset;
	
//	Mem::Copy(bitmap_add, pBits, stride*bitmap_size.iHeight);
	for(int y = sy; y < sy+height; y++)
	{
	 	offset = stride * y + size_sx;
		Mem::Copy(bitmap_add+ offset,  pBits+ offset, size_width);
	}
	
	pGc->BitBlt(TPoint(sx,sy),g_pOffScreenBitmap, TRect(sx,sy,sx+width,sy+height));
	//pGc->DrawBitmap(TPoint(sx,sy),g_pOffScreenBitmap);
}



static atVOID FlushToScreen(atINT sx, atINT sy, atINT width, atINT height )
{
	CAutoFlushLock oAutoChk;

	if( g_pOffScreenBitmap == atNULL ) return;
	if( g_pScreenBuffer == atNULL ) return;
	
	if ( oAutoChk.instance()->Get() == 1)
		return;
	oAutoChk.instance()->Set(1); 

	atUCHAR* pBits = (atUCHAR*)g_pScreenBuffer;

	g_pOffScreenBitmap->LockHeap();
	atUCHAR* bitmap_add = (atUCHAR*)(g_pOffScreenBitmap->DataAddress());
	if (bitmap_add == atNULL)
	{
		oAutoChk.instance()->Set(0);
		g_pOffScreenBitmap->UnlockHeap();
		return;
	}
	
	TSize bitmap_size = g_pOffScreenBitmap->SizeInPixels();
	TInt pixel_size   = g_nScreenBPP / 8;
	TInt stride       = g_pOffScreenBitmap->ScanLineLength(bitmap_size.iWidth, g_pOffScreenBitmap->DisplayMode());
	
	TInt size_sx      = pixel_size * sx;
	TInt size_width   = pixel_size * width;
	TInt offset;
	
	
//	Mem::Copy(bitmap_add, pBits, stride*bitmap_size.iHeight);
	for(int y = sy; y < sy+height; y++)
	{
		offset = stride * y + size_sx;
		Mem::Copy(bitmap_add+ offset, pBits+ offset,size_width);
	}
	BitGc->BitBlt(TPoint(sx,sy),g_pOffScreenBitmap, TRect(sx,sy,sx+width,sy+height));
	ScreenDev->Update();
	
	g_pOffScreenBitmap->UnlockHeap();
	
	oAutoChk.instance()->Set(0); 
}

static void init_dc(void)
{
    // Symbian Gc Setting  : for direct access
	ScreenDev = CFbsScreenDevice::NewL(_L("scdv"),EColor16MU); //default 16MU
	ScreenDev->CreateContext((CGraphicsContext*&)BitGc);
	
	switch(g_nScreenBPP)
	{
	case 16: //16bit
	{
		BitGc->SetUserDisplayMode(EColor64K);
		g_pOffScreenBitmap = new (ELeave) CFbsBitmap();
		g_pOffScreenBitmap->Create(TSize(g_nScreenWidth,g_nScreenHeight),EColor64K);
		
		break;
	}
	case 24: //24bit
	{
		BitGc->SetUserDisplayMode(EColor16M);
		g_pOffScreenBitmap = new (ELeave) CFbsBitmap();
		g_pOffScreenBitmap->Create(TSize(g_nScreenWidth,g_nScreenHeight),EColor16M);
		break;
	}
	default:
	{
		//error
		break;
	}
	}
	ScreenDev->ChangeScreenDevice(NULL);
//	ScreenDev->SetAutoUpdate(EFalse);
}

static void free_dc(void)
{
	delete ScreenDev;
	ScreenDev = NULL;
	
	delete BitGc;
	BitGc = NULL;
	
	delete g_pOffScreenBitmap; 
	g_pOffScreenBitmap = NULL;
}


atDWORD OEM_SYSAPI_GetDisplayCaps( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	return 0;
}

atBOOL OEM_SYSAPI_InitDisplay( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	atINT *vv = (atINT *)lInitData1;

	if( vv == atNULL ) return atFALSE;

	g_nScreenWidth = vv[0];
	g_nScreenHeight = vv[1];
	g_nScreenBPP = vv[2];
	//g_pScreenBuffer = (atSHORT*)vv[3];
	g_nScreenVMemWidth = vv[4];
	g_lpAppView = (CCoeControl *)vv[5];

	g_pScreenBuffer = g_pFrameBuffer;
	
	init_dc();

	return atTRUE;
}


atVOID OEM_SYSAPI_FinalDisplay( NOARGS )
{
	free_dc();
}

atBOOL OEM_SYSAPI_SetDisplayResolution( atINT nWidth, atINT nHeight, atINT nFrameWidth )
{
	g_nScreenWidth = nWidth;
	g_nScreenHeight = nHeight;
	g_nScreenVMemWidth = nFrameWidth;

	free_dc();
	init_dc();

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
//	FlushToScreen(nX, nY, nWidth, nHeight);

	TRect aRect;
	aRect = TRect( nX, nY, nX+nWidth, nY+nHeight );
	if( g_lpAppView ) g_lpAppView->DrawNow(aRect);
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
        case 0x1000 : _FlushToDC( param1, param2, param3, param4, param5 );
        		     break;
        case 0x1100 : if( g_lpAppView ) g_lpAppView->DrawNow();
        		     break;
    }
            
            
    return 0;
}

