/*###########################################################################
-----------------------------------------------------------------------------

                                _PLY_func.h
                         (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : 다각형을 그리기 위한 매크로들의 모임.

-----------------------------------------------------------------------------
###########################################################################*/
#ifndef ___PLY_FUNC_H_
#define ___PLY_FUNC_H_


#include "GrpX.h"


#ifdef __cplusplus
extern "C" {
#endif


//typedef void __POLYCALLBACK__DRAW_LINE( atGRPX_CANVAS *, atREAL, atREAL, atINT, atLONG );
typedef void __POLYCALLBACK__DRAW_LINE( atGRPX_CANVAS *lpCanvas, atREAL x1, atREAL x2, atINT y, atREAL dx1, atREAL dx2, atDWORD dwAttr, atLONG ExParam );


void __atGRPX_FillPolyPolygon0_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount,
						        atLONG ExParam1,
						        __POLYCALLBACK__DRAW_LINE *fnDrawLine );

void __atGRPX_FillPolyPolygon0( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount,
						        atLONG ExParam1,
						        __POLYCALLBACK__DRAW_LINE *fnDrawLine,
							 atLPGRPX_FUNC_COORD_CONV lpCnvFunc, atLPVOID lpParam );

                            
#ifdef __cplusplus
};
#endif


#endif
