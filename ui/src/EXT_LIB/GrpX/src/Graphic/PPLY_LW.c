/*###########################################################################
-----------------------------------------------------------------------------

                                PLY_LW.c
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


/*******************
  �ٰ��� ������ �׸���.
********************/
void atGRPX_DrawPolyPolyLineWidth( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth )
{
    int i;
    for( i = 0; i < nPolyCount; i++ ){
      atGRPX_DrawPolyLineWidth( lpCanvas, lpPtrList, nPartCount[i], bClosed, Color, nWidth );
      lpPtrList += nPartCount[i];
    }
}


/*******************
  �ٰ��� ������ �׸���.
  (Anti-Aliasing)
********************/
void atGRPX_DrawPolyPolyLineWidthAA( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth )
{
    int i;
    for( i = 0; i < nPolyCount; i++ ){
      atGRPX_DrawPolyLineWidthAA( lpCanvas, lpPtrList, nPartCount[i], bClosed, Color, nWidth );
      lpPtrList += nPartCount[i];
    }
}

