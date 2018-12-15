/*###########################################################################
-----------------------------------------------------------------------------

                                _STR_func.h
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
#ifndef ___STR_FUNC_H_
#define ___STR_FUNC_H_


#include "GrpX.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef void __STRCALLBACK__DRAW_GLYPH( atU8 *, atGRPX_GLYPHINFO *, atU8 *, atINT, atINT, atINT, atINT, atINT, atLONG );


void __atGRPX_DrawString0( atGRPX_FONT *lpFont, atLPBYTE lpDes, int nBPP, int widthbytes, atRECT *clip, atINT X, atINT Y, atLPTSTR StrData, atINT nLen, atINT nLineGap, 
						   atLONG ExParam1,
						   __STRCALLBACK__DRAW_GLYPH *fnDrawGlyph );


#ifdef __cplusplus
};
#endif


#endif
