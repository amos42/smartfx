/*###########################################################################
-----------------------------------------------------------------------------

                                rTRI.c
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
  다각형 라인을 그린다.
********************/
void atGRPX_DrawPolyPolyLine( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atBOOL bClosed, atGRPX_COLOR Color )
{
    int i;
    for( i = 0; i < nPolyCount; i++ ){
      atGRPX_DrawPolyLine( lpCanvas, lpPtrList, nPartCount[i], bClosed, Color );
      lpPtrList += nPartCount[i];
    }
}


/*******************
  다각형 라인을 그린다.
  (Anti-Aliasing)
********************/
void atGRPX_DrawPolyPolyLineAA( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount, atBOOL bClosed, atGRPX_COLOR Color )
{
    int i;
    for( i = 0; i < nPolyCount; i++ ){
      atGRPX_DrawPolyLineAA( lpCanvas, lpPtrList, nPartCount[i], bClosed, Color );
      lpPtrList += nPartCount[i];
    }
}
