/*#############################################################################
-------------------------------------------------------------------------------

                                  CLRscr.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Clear Screen function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "GrpX.h"
#include <string.h>


/*********************************
  화면을 지운다.
**********************************/
void atGRPX_ClearScreen( atGRPX_CANVAS *lpCanvas, atGRPX_COLOR Color )
{
#if atGRPX_PIXEL_BPP == 8    
    memset( atGRPX_GetFrameBufferPtr(lpCanvas), Color, VMEM_SIZE );
#elif atGRPX_PIXEL_BPP == 16
    int i, j;
    atDWORD c;
	atRECT * clip = atGRPX_GetClipRect(lpCanvas); 
    atDWORD *p = (atDWORD *)atGRPX_GetFrameBufferPtrXY(lpCanvas, clip->nStartX, clip->nStartY);
	int width = clip->nEndX - clip->nStartX + 1;
	int dx = (atGRPX_GetVMemWidth(lpCanvas) - width) >> 1;

    c = (atDWORD)Color;
    c |= c << 16;

    for( i = 0; i < (clip->nEndY - clip->nStartY + 1); i ++ ){
	  if( width & 0x1 ){
	    for( j = 0; j < (width >> 1); j ++ ){
		  *p++ = c;
        }
		*(atGRPX_PIXEL*)p = COLOR_TO_PIXEL(Color);
        p ++;
	  } else {
	    for( j = 0; j < (width >> 1); j ++ ){
		  *p++ = c;
	    }
	  }
      p += dx;
    }
#elif atGRPX_PIXEL_BPP == 24
    int i, j;
    atGRPX_PIXEL *p = (atGRPX_PIXEL *)atGRPX_GetFrameBufferPtr(lpCanvas);
	int dx = (atGRPX_GetVMemWidth(lpCanvas) - atGRPX_GetCanvasWidth(lpCanvas));

    for( i = 0; i < lpCanvas->Height; i ++ ){
      for( j = 0; j < lpCanvas->Width; j ++ )
        p[0] = Color[0];
        p[1] = Color[1];
        p[2] = Color[2];
        p ++;
      }
      p += dx;
    }
#endif

/*    
    int i, j;
    DWORD *p = (DWORD *)_Video;
    DWORD c;

    if( Color == TRANS_COLOR ) return;

    c = (DWORD)Color;
    c |= c << 8;
    c |= c << 16;

    for( i = 0; i < SCREEN_HEIGHT; i ++ ){
      for( j = 0; j < SCREEN_WIDTH / sizeof(DWORD); j ++ )
        *p++ = c;
      p += (VMEM_WIDTH - SCREEN_WIDTH) / sizeof(DWORD);
    }
*/    
}


/*********************************
  화면을 지운다.
**********************************/
void atGRPX_ClearScreenAll( atGRPX_CANVAS *lpCanvas, atGRPX_COLOR Color )
{
#if atGRPX_PIXEL_BPP == 8    
    memset( atGRPX_GetFrameBufferPtr(lpCanvas), Color, VMEM_SIZE );
#elif atGRPX_PIXEL_BPP == 16
    int i, j;
    atDWORD c;
	atREGION * rgn = atGRPX_GetCanvasArea(lpCanvas); 
    atDWORD *p = (atDWORD *)atGRPX_GetFrameBufferPtr(lpCanvas);
	int dx = ( atGRPX_GetVMemWidth(lpCanvas) - rgn->nWidth )/2;

//    if( Color == TRANS_COLOR ) return;

    c = (atDWORD)Color;
    c |= c << 16;

    for( i = 0; i < rgn->nHeight; i ++ ){
      for( j = 0; j < (rgn->nWidth)>>1; j ++ )
        *p++ = c;
      p += dx;
    }
#elif atGRPX_PIXEL_BPP == 24
    int i, j;
    atGRPX_PIXEL *p = (atGRPX_PIXEL *)atGRPX_GetFrameBufferPtr(lpCanvas);
	int dx = (atGRPX_GetVMemWidth(lpCanvas) - atGRPX_GetCanvasWidth(lpCanvas));

    for( i = 0; i < lpCanvas->Height; i ++ ){
      for( j = 0; j < lpCanvas->Width; j ++ )
        p[0] = Color[0];
        p[1] = Color[1];
        p[2] = Color[2];
        p ++;
      }
      p += dx;
    }
#endif

/*    
    int i, j;
    DWORD *p = (DWORD *)_Video;
    DWORD c;

    if( Color == TRANS_COLOR ) return;

    c = (DWORD)Color;
    c |= c << 8;
    c |= c << 16;

    for( i = 0; i < SCREEN_HEIGHT; i ++ ){
      for( j = 0; j < SCREEN_WIDTH / sizeof(DWORD); j ++ )
        *p++ = c;
      p += (VMEM_WIDTH - SCREEN_WIDTH) / sizeof(DWORD);
    }
*/    
}

