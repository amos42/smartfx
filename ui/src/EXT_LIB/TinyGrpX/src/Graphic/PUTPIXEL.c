/*#############################################################################
-------------------------------------------------------------------------------

                                  putPixel.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : PutPixel related function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "TinyGrpX.h"


/*********************************
  지정된 좌표의 점을 찍는다.
**********************************/
atTGRPX_PIXEL *atTGRPX_DrawPixel( atTGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atTGRPX_COLOR Color )
{
    atRECT *clip;
	atTGRPX_PIXEL *ptr;

    clip = atTGRPX_GetClipRect(lpCanvas);

    if( X < clip->nStartX || X > clip->nEndX ||
        Y < clip->nStartY || Y > clip->nEndY ) return atNULL;

	ptr = atTGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );
	
    *ptr = COLOR_TO_PIXEL(Color);

	return ptr;
}
