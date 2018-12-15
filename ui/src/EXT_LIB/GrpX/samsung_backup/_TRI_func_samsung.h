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
#ifndef ___TRI_FUNC_SAMSUNG_H_
#define ___TRI_FUNC_SAMSUNG_H_


#include "GrpX.h"
#include "AT_Real.h"
#include "_TRI_FUNC.h"


#ifdef __cplusplus
extern "C" {
#endif


void __atGRPX_FillRoundCapTriangle0( atGRPX_CANVAS *lpCanvas, atLONG X1, atLONG Y1, atLONG X2, atLONG Y2, atLONG X3, atLONG Y3,
							atLONG ExParam1,
							__FTRICALLBACK__DRAW_HLINE *fnDrawHLine );


#ifdef __cplusplus
};
#endif


#endif
