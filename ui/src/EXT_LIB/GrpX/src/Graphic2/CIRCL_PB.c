/*#############################################################################
-------------------------------------------------------------------------------

                                  CirCle_F.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Circle function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "GrpX.h"



/***************************

****************************/
void  atGRPX_DrawCircleWithPen( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atINT Radius, atGRPX_PEN *lpPen )
{
    switch( lpPen->nPenType ){
        case atGRPX_PENTYPE_SOLID  :
            if( lpPen->nAlpha == 0x00 ){
                // Nothing
            } else if( lpPen->nAlpha == 0xFF ){
                if( lpPen->bAntiAliasing )
                    atGRPX_DrawCircle( lpCanvas, X, Y, Radius, lpPen->clColor ); // AntiAliasing 미지원
                else 
                    atGRPX_DrawCircle( lpCanvas, X, Y, Radius, lpPen->clColor );
            } else {
                // 미지원...
            }
            break;
        case atGRPX_PENTYPE_BITMAP :
            // 미지원...
            break;
    }
}


/***************************

****************************/
void atGRPX_FillCircleWithBrush( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, atINT Radius, atGRPX_BRUSH *lpBrush )
{
//    atGRPX_PIXEL *ptr, cl;
    
    switch( lpBrush->nBrushType ){
        case atGRPX_BRUSHTYPE_SOLID  :
            if( lpBrush->nAlpha == 0x00 ){
                // Nothing
            } else if( lpBrush->nAlpha == 0xFF ){
                if( lpBrush->bAntiAliasing )
                    atGRPX_FillCircle( lpCanvas, X, Y, Radius, lpBrush->clColor ); // AntiAliasing 미지원
                else 
                    atGRPX_FillCircle( lpCanvas, X, Y, Radius, lpBrush->clColor );
            } else {
                // 미지원...
            }
            break;
        case atGRPX_BRUSHTYPE_BITMAP :
            // 미지원
            break;
    }
}

