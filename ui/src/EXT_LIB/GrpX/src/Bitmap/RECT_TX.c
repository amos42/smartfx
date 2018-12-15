/*#############################################################################
-------------------------------------------------------------------------------

                                  RECT_TX.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Rectangle related function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "GrpX.h"


/*********************************
  
-- 아직 검증 덜 되었음. 검증 되면 이 문구 삭제
**********************************/
void atGRPX_DrawTextureRect( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_BITMAP *lpTextureBB )
{
    int i, j;
    int x, y;
    int w, h;
    int wcount, hcount;
    atRECT clip;
	int StartX2, StartY2;

    clip = *atGRPX_GetClipRect(lpCanvas);

	StartX2 = MAX( StartX, clip.nStartX );
	StartY2 = MAX( StartY, clip.nStartY );
	EndX = MIN( EndX, clip.nEndX );
	EndY = MIN( EndY, clip.nEndY );

    atGRPX_SetClipRect( lpCanvas, StartX2, StartY2, EndX, EndY );

    w = lpTextureBB->nWidth;
    h = lpTextureBB->nHeight;

	if( (StartX2 - StartX) >= w ) StartX = StartX2 - (StartX2 - StartX) % w;
	if( (StartY2 - StartY) >= h ) StartY = StartY2 - (StartY2 - StartY) % h;

    i = EndX - StartX + 1;
    wcount = i / w;
    if( i % w ) wcount ++;

    j = EndY - StartY + 1;
    hcount = j / h;
    if( j % h ) hcount ++;

    y = StartY;
    for( i = 0; i < hcount; i ++ ){
      x = StartX;

      for( j = 0; j < wcount; j ++ ){
        atGRPX_DrawBitmap( lpCanvas, lpTextureBB, x, y );
        x += w;
      }

      y += h;
    }

    atGRPX_SetClipRect( lpCanvas, clip.nStartX, clip.nStartY, clip.nEndX, clip.nEndY );
}
