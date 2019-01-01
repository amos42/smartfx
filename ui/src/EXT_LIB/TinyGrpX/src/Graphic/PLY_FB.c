/*###########################################################################
-----------------------------------------------------------------------------

                                PLY_F.c
                         (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : TRI related function definition

-----------------------------------------------------------------------------
###########################################################################*/

#include "GrpX.h"
#include <string.h>


/*******************
  다각형 라인을 그린다.
********************/
atVOID atGRPX_FillPolygonBitmap( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_BITMAP *lpBB )
{
    atGRPX_FillPolyPolygonBitmap( lpCanvas, lpPtrList, &nCount, 1, lpBB );
}

/*******************
  다각형 라인을 그린다.
********************/
atVOID atGRPX_FillPolygonBitmapEx( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_BITMAP *lpBB, atLPGRPX_FUNC_COORD_CONV lpCnvFunc, atLPVOID lpParam )
{
    atGRPX_FillPolyPolygonBitmapEx( lpCanvas, lpPtrList, &nCount, 1, lpBB, lpCnvFunc, lpParam );
}

