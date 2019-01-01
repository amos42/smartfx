/*#############################################################################
-------------------------------------------------------------------------------

                                  GetPixel.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Get pixel function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "TinyGrpX.h"


/*********************************
  지정된 좌표의 점을 읽어온다.
**********************************/
atTGRPX_COLOR atTGRPX_GetPixel( atTGRPX_CANVAS *lpCanvas, atINT X, atINT Y )
{
	atRECT *clip = atTGRPX_GetClipRect( lpCanvas );

    if( X < clip->nStartX || X > clip->nEndX ||
        Y < clip->nStartY || Y > clip->nEndY ) return( (atTGRPX_COLOR)0x000000 );

    return PIXEL_TO_COLOR(*(atTGRPX_PIXEL *)atTGRPX_GetFrameBufferPtrXY(lpCanvas, X, Y) );
}

