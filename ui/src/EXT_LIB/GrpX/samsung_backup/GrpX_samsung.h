/*#############################################################################
-------------------------------------------------------------------------------

                                  GrpX.h
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : GrpX - 2D Graphics function pool

-------------------------------------------------------------------------------
#############################################################################*/
#ifndef __GRPX_SAMSUNG_H_
#define __GRPX_SAMSUNG_H_


#include "GrpX.h"


#ifdef __cplusplus
  extern "C" {
#endif



//=========================
// 삼성에서 추가한 것.
//=========================
//samsung
#define _USE_BRESENHAM		/* Bresenham 적용 여부 */
#define CLIP_X_START	0
#define CLIP_X_END		239
#define CLIP_Y_START	0
#define CLIP_Y_END		319
//=========================



//=========================
// 삼성에서 추가한 것.
//=========================
//samsung
atVOID          atGRPX_DrawPolyEdgeLine( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color, atINT nWidth );
atVOID			atGRPX_FillRoundCapTriangleAA( atGRPX_CANVAS *lpCanvas, atLONG X1, atLONG Y1, atLONG X2, atLONG Y2, atLONG X3, atLONG Y3, atGRPX_COLOR Color);

atVOID			atGRPX_DrawPolyBreLineWidthAAIn(atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color, atGRPX_COLOR OutColor, atINT nWidth);
atVOID			atGRPX_DrawPolyBreLineWidthAAOut(atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color, atGRPX_COLOR BkColor, atINT nWidth);
atVOID			atGRPX_DrawPolyBreLineWidthAAInEdge(atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color, atGRPX_COLOR OutColor, atINT nWidth);
//=========================


#ifdef __cplusplus
}
#endif


#endif
