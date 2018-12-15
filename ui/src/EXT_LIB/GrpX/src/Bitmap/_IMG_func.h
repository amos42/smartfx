/*###########################################################################
-----------------------------------------------------------------------------

                                _IMG_func.h
                         (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : 이미지를 그리기 위한 매크로들의 모임.

-----------------------------------------------------------------------------
###########################################################################*/
#ifndef ___IMG_FUNC_H_
#define ___IMG_FUNC_H_


#include "GrpX.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef void __IMAGECALLBACK__DRAW_PIXEL( atGRPX_PIXEL *, atGRPX_BITMAP *, atINT, atINT, atLONG );
typedef void __IMAGECALLBACK__DRAW_IMAGE_PART( atGRPX_CANVAS *, atGRPX_BITMAP *, atINT, atINT, atINT, atINT, atINT, atINT, atLONG );
typedef void __IMAGECALLBACK__DRAW_IMAGE_LINE( atGRPX_PIXEL *, atGRPX_BITMAP *, atINT, atREAL, atREAL, atREAL, atREAL, atLONG );
typedef void __IMAGECALLBACK__DRAW_IMAGE_STRETCH( atGRPX_CANVAS *, atGRPX_BITMAP *, atINT, atINT, atINT, atINT, atINT, atINT, atREAL, atREAL, atLONG );


void __atGRPX_DrawBitmapRotate0( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atREAL Angle,
								atLONG ExParam1,
								__IMAGECALLBACK__DRAW_PIXEL *fnDrawPixel,
								__IMAGECALLBACK__DRAW_IMAGE_LINE *fnDrawImageLine );

void __atGRPX_DrawBitmapPart0( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y,
							  atINT ImageX, atINT ImageY, atINT ImageWidth, atINT ImageHeight,
							  atLONG ExParam1,
							  __IMAGECALLBACK__DRAW_IMAGE_PART *fnDrawImage );

void __atGRPX_DrawBitmapStretch0( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, 
								  atINT tWidth, atINT tHeight,
								  atLONG ExParam1,
							      __IMAGECALLBACK__DRAW_IMAGE_STRETCH *fnDrawBitmap );


#ifdef __cplusplus
}
#endif


#endif
