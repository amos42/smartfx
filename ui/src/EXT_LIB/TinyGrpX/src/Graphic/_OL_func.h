/*###########################################################################
-----------------------------------------------------------------------------

                                _OL_func.h
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
#ifndef ___OL_FUNC_H_
#define ___OL_FUNC_H_


#include "GrpX.h"


#ifdef __cplusplus
extern "C" {
#endif


atREAL __atGRPX_GetOutlinePtrs( atINT kiL, atINT kiT, atINT kiR, atINT kiB, atINT nWidth, atPOINT pkPtOut[], atBOOL isSExt, atBOOL isEExt );
atREAL __atGRPX_GetOutlinePtrs_REAL( atREAL kiL, atREAL kiT, atREAL kiR, atREAL kiB, atREAL rWidth, atRPOINT pkPtOut[], atBOOL isSExt, atBOOL isEExt );


#ifdef __cplusplus
};
#endif


#endif
