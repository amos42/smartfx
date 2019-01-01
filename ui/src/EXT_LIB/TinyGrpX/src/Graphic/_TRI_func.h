/*###########################################################################
-----------------------------------------------------------------------------

                                _TRI_func.h
                         (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : 채워진 삼각형 그리기 위한 매크로들의 모임.

-----------------------------------------------------------------------------
###########################################################################*/
#ifndef ___TRI_FUNC_H_
#define ___TRI_FUNC_H_


#include "GrpX.h"
#include "AT_Real.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef void __FTRICALLBACK__DRAW_HLINE( atGRPX_CANVAS *, atREAL, atREAL, atINT, atREAL, atREAL, atLONG );


void __atGRPX_FillTriangle0( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3,
						atLONG ExParam1,
						__FTRICALLBACK__DRAW_HLINE *fnDrawHLine );
void __atGRPX_FillTriangle0_REAL( atGRPX_CANVAS *lpCanvas, atREAL rX1, atREAL rY1, atREAL rX2, atREAL rY2, atREAL rX3, atREAL rY3, 
						atLONG ExParam1,
						__FTRICALLBACK__DRAW_HLINE *fnDrawHLine );


#ifdef __cplusplus
};
#endif


#endif
