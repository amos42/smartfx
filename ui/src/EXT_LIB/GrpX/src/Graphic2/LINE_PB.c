/*#############################################################################
-------------------------------------------------------------------------------

                                  Line.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Line function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "GrpX.h"


void atGRPX_DrawLineWithPen( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_PEN *lpPen )
{
    switch( lpPen->nPenType ){
        case atGRPX_PENTYPE_SOLID  :
            if( lpPen->nAlpha == 0x00 ){
                // Nothing
            } else if( lpPen->nAlpha == 0xFF ){
                if( lpPen->bAntiAliasing )
                    atGRPX_DrawLineWidthExtAA( lpCanvas, StartX, StartY, EndX, EndY, lpPen->clColor, lpPen->nPenWidth, atGRPX_LINEEXT_EXTEND );
                else 
                    atGRPX_DrawLineWidthExt( lpCanvas, StartX, StartY, EndX, EndY, lpPen->clColor, lpPen->nPenWidth, atGRPX_LINEEXT_EXTEND );
            } else {
                // 미지원...
            }
            break;
        case atGRPX_PENTYPE_BITMAP :
            // 미지원...
            break;
    }
}

