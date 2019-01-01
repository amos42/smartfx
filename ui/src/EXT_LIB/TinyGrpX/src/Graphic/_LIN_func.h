/*###########################################################################
-----------------------------------------------------------------------------

                                _LIN_func.h
                         (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : 선 그리기 위한 매크로들의 모임.

-----------------------------------------------------------------------------
###########################################################################*/
#ifndef ___LIN_FUNC_H_
#define ___LIN_FUNC_H_


#include "TinyGrpX.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef void __LINECALLBACK__DRAW_PIXEL( atTGRPX_CANVAS *, atINT, atINT, atLONG );
typedef void __LINECALLBACK__DRAW_LINE( atTGRPX_CANVAS *, atINT, atINT, atINT, atINT, atLONG );
typedef int __LINECALLBACK__DRAW_LINE_DASH( atTGRPX_CANVAS *, atINT, atINT, atINT, atINT, atLONG, atINT, atINT, atINT );
typedef void __LINECALLBACK__DRAW_H_LINE( atTGRPX_CANVAS *, atINT, atINT, atINT, atLONG );
typedef int __LINECALLBACK__DRAW_H_LINE_DASH( atTGRPX_CANVAS *, atINT, atINT, atINT, atLONG, atINT, atINT, atINT );
typedef void __LINECALLBACK__DRAW_V_LINE( atTGRPX_CANVAS *, atINT, atINT, atINT, atLONG );
typedef int __LINECALLBACK__DRAW_V_LINE_DASH( atTGRPX_CANVAS *, atINT, atINT, atINT, atLONG, atINT, atINT, atINT );


atINT  __atTGRPX_GetClippedEdge_REAL( atREAL *lprDesStartX, atREAL *lprDesStartY, atREAL *lprDesEndX, atREAL *lprDesEndY, atREAL rStartX, atREAL rStartY, atREAL rEndX, atREAL rEndY, atRRECT *rtClipR );
atINT  __atTGRPX_GetClippedEdge( atINT *lpDesStartX, atINT *lpDesStartY, atINT *lpDesEndX, atINT *lpDesEndY, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atRECT *rtClip );

atVOID __atTGRPX_DrawLine0( atTGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY,
						atLONG ExParam1,
						__LINECALLBACK__DRAW_PIXEL *fnDrawPixel,  __LINECALLBACK__DRAW_LINE *fnDrawLine,
						__LINECALLBACK__DRAW_H_LINE *fnDrawHLine, __LINECALLBACK__DRAW_V_LINE *fnDrawVLine );

int __atTGRPX_DrawLineDash0( atTGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY,
						  atLONG ExParam1, atINT nDashLen, atINT nGap, atINT init_gapdash,
						  __LINECALLBACK__DRAW_PIXEL *fnDrawPixel,  __LINECALLBACK__DRAW_LINE_DASH *fnDrawLine,
						  __LINECALLBACK__DRAW_H_LINE_DASH *fnDrawHLine, __LINECALLBACK__DRAW_V_LINE_DASH *fnDrawVLine );


#ifdef __cplusplus
}
#endif


#endif
