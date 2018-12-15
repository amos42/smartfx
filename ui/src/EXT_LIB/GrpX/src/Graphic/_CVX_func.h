/*###########################################################################
-----------------------------------------------------------------------------

                                _CVX_func.h
                         (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : ä���� Convex �ٰ��� ���� �Լ����� ����.

-----------------------------------------------------------------------------
###########################################################################*/
#ifndef ___CVX_FUNC_H_
#define ___CVX_FUNC_H_


#include "GrpX.h"
#include "AT_Real.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef void __FCVXCALLBACK__DRAW_HLINE( atGRPX_CANVAS *lpCanvas, atINT scan_y, atREAL lx, atREAL ly, atREAL rx, atREAL ry, atREAL ldxdy, atREAL rdxdy, atREAL alpha_init, atDWORD dwAttr, atLONG ExParam );


void __atGRPX_FillConvexPolygon0_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrs[], atINT nCount,
						              atINT lpAttr[], atLONG ExParam1,
						              __FCVXCALLBACK__DRAW_HLINE *fnDrawHLine );
/*
void __atGRPX_FillConvexPolygon0( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrs[], atINT nCount,
						         atLONG ExParam1,
						         __FCVXCALLBACK__DRAW_HLINE *fnDrawHLine );
*/						         


#ifdef __cplusplus
}
#endif


#endif
