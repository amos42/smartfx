/*#############################################################################
-------------------------------------------------------------------------------

                                  Line.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Line function definition

-------------------------------------------------------------------------------
#############################################################################*/

#include "TinyGrpX.h"
#include "A_Math.h"
#include "_LIN_int.h"
#include "_LIN_func.h"



atINT __atTGRPX_GetClippedEdge_REAL( atREAL *lprDesStartX, atREAL *lprDesStartY, atREAL *lprDesEndX, atREAL *lprDesEndY, atREAL rStartX, atREAL rStartY, atREAL rEndX, atREAL rEndY, atRRECT *rtClipR )
{
   int code1, code2;
   atREAL xl, xr, yl, yr;
   atREAL dx, dy, off  ;
   int n = 0;
   atREAL x[2], y[2];

   // Ŭ���� ������ ���� CS �ڵ� ����
   MAKE_CS_CODE_2( code1, code2, rStartX, rStartY, rEndX, rEndY, rtClipR->rStartX, rtClipR->rStartY, rtClipR->rEndX, rtClipR->rEndY  );

   // ���۰� ����ǥ�� ���ÿ� ���� Ŭ���� ������ ����� ����.
   if( code1 & code2 ) return -1; 

   // Ŭ���� �� �ʿ䰡 ������ �׳� �� �׸�.
   if( (code1 == CS_CODE_NONE) && (code2 == CS_CODE_NONE) ){
     *lprDesStartX = rStartX;
     *lprDesStartY = rStartY;
     *lprDesEndX = rEndX;
     *lprDesEndY = rEndY;
     return 0;
   }

   // �������� Ŭ���� ���� ������ �����Ѵ�.
   if( code1 > code2 ){
     atREAL temp;
	 SWAP( code1, code2, temp );
	 SWAP( rStartX, rEndX, temp );
	 SWAP( rStartY, rEndY, temp );
   }

   dx = rEndX - rStartX;
   dy = rEndY - rStartY;
   off = REALMUL(rEndX, rStartY) - REALMUL(rEndY, rStartX);

   if( dy != 0 ){
     xl = REALMULDIV(dx, (rtClipR->rStartY - off), dy);
     xr = REALMULDIV(dx, (rtClipR->rEndY   - off), dy);
   } else {
     xl = 0;
     xr = 0;
   }
   if( dx != 0 ){
     yl = REALMULDIV(dy, (rtClipR->rStartX + off), dx);
     yr = REALMULDIV(dy, (rtClipR->rEndX   + off), dx);
   } else {
     yl = 0;
     yr = 0;
   }

   // ������ Ŭ������ ���� �� ������ Ŭ���� �Ѵ�.
   if( code1 == CS_CODE_NONE ){
     switch( code2 ){
       case CS_CODE_TOP :
				rEndX = xl;
                rEndY = rtClipR->rStartY;
                break;
       case CS_CODE_BOTTOM :
				rEndX = xr;
                rEndY = rtClipR->rEndY;
                break;
       case CS_CODE_LEFT :
				rEndX = rtClipR->rStartX;
                rEndY = yl;
                break;
       case CS_CODE_RIGHT :
				rEndX = rtClipR->rEndX;
                rEndY = yr;
                break;
       case CS_CODE_TOP + CS_CODE_LEFT :
				rEndX = rtClipR->rStartX;
                rEndY = yl;
                if( (rEndY < rtClipR->rStartY) || (rtClipR->rEndY < rEndY) ){
                  rEndX = xl;
                  rEndY = rtClipR->rStartY;
                }
                break;
       case CS_CODE_BOTTOM + CS_CODE_LEFT :
				rEndX = rtClipR->rStartX;
                rEndY = yl;
                if( (rEndY < rtClipR->rStartY) || (rtClipR->rEndY < rEndY) ){
                  rEndX = xr;
                  rEndY = rtClipR->rEndY;
                }
                break;
       case CS_CODE_TOP + CS_CODE_RIGHT :
				rEndX = rtClipR->rEndX;
                rEndY = yr;
                if( (rEndY < rtClipR->rStartY) || (rtClipR->rEndY < rEndY) ){
                  rEndX = xl;
                  rEndY = rtClipR->rStartY;
                }
                break;
       case CS_CODE_BOTTOM + CS_CODE_RIGHT :
				rEndX = rtClipR->rEndX;
                rEndY = yr;
                if( (rEndY < rtClipR->rStartY) || (rtClipR->rEndY < rEndY) ){
                  rEndX = xr;
                  rEndY = rtClipR->rEndY;
                }
                break;
     }

    	*lprDesStartX = rStartX;
    	*lprDesStartY = rStartY;
    	*lprDesEndX = rEndX;
    	*lprDesEndY = rEndY;
		
     return 1;
   }

   if( (rtClipR->rStartX <  xl) && (xl <  rtClipR->rEndX) ){ 
	   x[n] = xl; y[n++] = rtClipR->rStartY; 
   }
   if( (rtClipR->rStartY <= yl) && (yl <= rtClipR->rEndY) ){ 
	   y[n] = yl; x[n++] = rtClipR->rStartX; 
   }
   if( (rtClipR->rStartX <  xr) && (xr <  rtClipR->rEndX) ){ 
	   x[n] = xr; y[n++] = rtClipR->rEndY; 
   }
   if( (rtClipR->rStartY <= yr) && (yr <= rtClipR->rEndY) ){ 
	   y[n] = yr; x[n++] = rtClipR->rEndX; 
   }

    *lprDesStartX = x[0];
    *lprDesStartY = y[0];
    *lprDesEndX = x[1];
    *lprDesEndY = y[1];

	return 2;
}



atINT __atTGRPX_GetClippedEdge( atINT *lpDesStartX, atINT *lpDesStartY, atINT *lpDesEndX, atINT *lpDesEndY, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atRECT *rtClip )
{
    int code1, code2;
    int xl, xr, yl, yr;
    long dx, dy, off  ;
    int n = 0;
    int x[2], y[2];
   
    // Ŭ���� ������ ���� CS �ڵ� ����
    MAKE_CS_CODE_2( code1, code2, StartX, StartY, EndX, EndY, rtClip->nStartX, rtClip->nStartY, rtClip->nEndX, rtClip->nEndY  );

    // ���۰� ����ǥ�� ���ÿ� ���� Ŭ���� ������ ����� ����.
    if( code1 & code2 ) return -1; 

    // Ŭ���� �� �ʿ䰡 ������ �׳� �� �׸�.
    if( (code1 == CS_CODE_NONE) && (code2 == CS_CODE_NONE) ){
      *lpDesStartX = StartX;
      *lpDesStartY = StartY;
      *lpDesEndX = EndX;
      *lpDesEndY = EndY;
      return 0;
	}

    // �������� Ŭ���� ���� ������ �����Ѵ�.
    if( code1 > code2 ){
      int temp;
	  SWAP( code1, code2, temp );
	  SWAP( StartX, EndX, temp );
	  SWAP( StartY, EndY, temp );
	}

    dx = EndX - StartX;
    dy = EndY - StartY;
    off= EndX * StartY - EndY * StartX;

    xl = (dx * rtClip->nStartY - off) / dy;
    yl = (dy * rtClip->nStartX + off) / dx;
    xr = (dx * rtClip->nEndY   - off) / dy;
    yr = (dy * rtClip->nEndX   + off) / dx;

    // ������ Ŭ������ ���� �� ������ Ŭ���� �Ѵ�.
    if( code1 == CS_CODE_NONE ){
      switch( code2 ){
        case CS_CODE_TOP :
				EndX = xl;
                EndY = rtClip->nStartY;
                break;
        case CS_CODE_BOTTOM :
				EndX = xr;
                EndY = rtClip->nEndY;
                break;
        case CS_CODE_LEFT :
				EndX = rtClip->nStartX;
                EndY = yl;
                break;
        case CS_CODE_RIGHT :
				EndX = rtClip->nEndX;
                EndY = yr;
                break;
        case CS_CODE_TOP + CS_CODE_LEFT :
				EndX = rtClip->nStartX;
                EndY = yl;
                if( (EndY < rtClip->nStartY) || (rtClip->nEndY < EndY) ){
                  EndX = xl;
                  EndY = rtClip->nStartY;
                }
                break;
        case CS_CODE_BOTTOM + CS_CODE_LEFT :
				EndX = rtClip->nStartX;
                EndY = yl;
                if( (EndY < rtClip->nStartY) || (rtClip->nEndY < EndY) ){
                  EndX = xr;
                  EndY = rtClip->nEndY;
                }
                break;
        case CS_CODE_TOP + CS_CODE_RIGHT :
				EndX = rtClip->nEndX;
                EndY = yr;
                if( (EndY < rtClip->nStartY) || (rtClip->nEndY < EndY) ){
                  EndX = xl;
                  EndY = rtClip->nStartY;
                }
                break;
        case CS_CODE_BOTTOM + CS_CODE_RIGHT :
				EndX = rtClip->nEndX;
                EndY = yr;
                if( (EndY < rtClip->nStartY) || (rtClip->nEndY < EndY) ){
                  EndX = xr;
                  EndY = rtClip->nEndY;
                }
                break;
	  }

    	*lpDesStartX = StartX;
    	*lpDesStartY = StartY;
    	*lpDesEndX = EndX;
    	*lpDesEndY = EndY;

      return 1;
	}

    if( (rtClip->nStartX <  xl) && (xl <  rtClip->nEndX) ){
      x[n] = xl; y[n++]= rtClip->nStartY;
	}
    if( (rtClip->nStartY <= yl) && (yl <= rtClip->nEndY) ){
      y[n] = yl; x[n++]= rtClip->nStartX; 
	}
    if( (rtClip->nStartX <  xr) && (xr <  rtClip->nEndX) ){ 
 	  x[n] = xr; y[n++]= rtClip->nEndY; 
	}
    if( (rtClip->nStartY <= yr) && (yr <= rtClip->nEndY) ){ 
	  y[n] = yr; x[n++]= rtClip->nEndX; 
	}

    *lpDesStartX = x[0];
    *lpDesStartY = y[0];
    *lpDesEndX = x[1];
    *lpDesEndY = y[1];

	return 2;
}


// pc release ���� �Լ� �Ķ���� ���� �Ѵܰ辿 �з� �ѱ�� ��� ������..
#pragma optimize( "g", off )


#if 0 /// Ŭ���� �Լ��� �и��� �ڵ尡 �������� �ؼ�, �ӽ÷� ���� �ڵ�� ���� ����.

/*****************************************
  Ŭ�����Ͽ� �� �� ���� �մ� ���� �׸���.
******************************************/
void __atTGRPX_DrawLine0( atTGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY,
                        atLONG ExParam1,
						__LINECALLBACK__DRAW_PIXEL *fnDrawPixel, __LINECALLBACK__DRAW_LINE *fnDrawLine,
						__LINECALLBACK__DRAW_H_LINE *fnDrawHLine, __LINECALLBACK__DRAW_V_LINE *fnDrawVLine )
{
//   int code1, code2;
//   int xl, xr, yl, yr;
//   long dx, dy, off  ;
   int n;
//   int x[2], y[2];
   atRECT *clip;
   int r;
//   int x1, y1, x2, y2;

   // �������� ������ ��� ������ �׳� ���� ��� ������.
   if( (StartX == EndX) && (StartY == EndY) ){
     fnDrawPixel( lpCanvas, StartX, StartY, ExParam1 );
	 return;
   }
   // �������� ������ X��ǥ�� ������ �������� �׸��� ������.
   if( StartX == EndX ){
	 if( StartY > EndY ) SWAP( StartY, EndY, n );
     fnDrawVLine( lpCanvas, StartX, StartY, EndY - StartY + 1, ExParam1 );
     return;
   }
   // �������� ������ Y��ǥ�� ������ ���򼱸� �׸��� ������.
   if( StartY == EndY ){
	 if( StartX > EndX ) SWAP( StartX, EndX, n );
     fnDrawHLine( lpCanvas, StartX, StartY, EndX - StartX + 1, ExParam1 );
     return;
   }

   // Ŭ���� ������ ��´�.
   clip = atTGRPX_GetClipRect(lpCanvas);

    r = __atTGRPX_GetClippedEdge( &StartX, &StartY, &EndX, &EndY, StartX, StartY, EndX, EndY, clip );
	if( r == -1 ) return;

   // �������� ������ ��� ������ �׳� ���� ��� ������.
   if( (StartX == EndX) && (StartY == EndY) ){
     fnDrawPixel( lpCanvas, StartX, StartY, ExParam1 );
	 return;
   }
   // �������� ������ X��ǥ�� ������ �������� �׸��� ������.
   if( StartX == EndX ){
	 if( StartY > EndY ) SWAP( StartY, EndY, n );
     fnDrawVLine( lpCanvas, StartX, StartY, EndY - StartY + 1, ExParam1 );
     return;
   }
   // �������� ������ Y��ǥ�� ������ ���򼱸� �׸��� ������.
   if( StartY == EndY ){
	 if( StartX > EndX ) SWAP( StartX, EndX, n );
     fnDrawHLine( lpCanvas, StartX, StartY, EndX - StartX + 1, ExParam1 );
     return;
   }

   fnDrawLine( lpCanvas, StartX, StartY, EndX, EndY, ExParam1 );

	return;
}

#else

/*****************************************
  Ŭ�����Ͽ� �� �� ���� �մ� ���� �׸���.
******************************************/
void __atTGRPX_DrawLine0( atTGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY,
                        atLONG ExParam1,
						__LINECALLBACK__DRAW_PIXEL *fnDrawPixel, __LINECALLBACK__DRAW_LINE *fnDrawLine,
						__LINECALLBACK__DRAW_H_LINE *fnDrawHLine, __LINECALLBACK__DRAW_V_LINE *fnDrawVLine )
{
   int code1, code2;
   int xl, xr, yl, yr;
   long dx, dy, off  ;
   int n = 0;
   int x[2], y[2];
   atRECT *clip;

   // �������� ������ ��� ������ �׳� ���� ��� ������.
   if( (StartX == EndX) && (StartY == EndY) ){
     fnDrawPixel( lpCanvas, StartX, StartY, ExParam1 );
	 return;
   }
   // �������� ������ X��ǥ�� ������ �������� �׸��� ������.
   if( StartX == EndX ){
	 if( StartY > EndY ) SWAP( StartY, EndY, n );
     fnDrawVLine( lpCanvas, StartX, StartY, EndY - StartY + 1, ExParam1 );
     return;
   }
   // �������� ������ Y��ǥ�� ������ ���򼱸� �׸��� ������.
   if( StartY == EndY ){
	 if( StartX > EndX ) SWAP( StartX, EndX, n );
     fnDrawHLine( lpCanvas, StartX, StartY, EndX - StartX + 1, ExParam1 );
     return;
   }

   // Ŭ���� ������ ��´�.
   clip = atTGRPX_GetClipRect(lpCanvas);

   // Ŭ���� ������ ���� CS �ڵ� ����
   MAKE_CS_CODE_2( code1, code2, StartX, StartY, EndX, EndY, clip->nStartX, clip->nStartY, clip->nEndX, clip->nEndY  );

   // ���۰� ����ǥ�� ���ÿ� ���� Ŭ���� ������ ����� ����.
   if( code1 & code2 ) return; 

   // Ŭ���� �� �ʿ䰡 ������ �׳� �� �׸�.
   if( (code1 == CS_CODE_NONE) && (code2 == CS_CODE_NONE) ){
     fnDrawLine( lpCanvas, StartX, StartY, EndX, EndY, ExParam1 );
     return;
   }

   // �������� Ŭ���� ���� ������ �����Ѵ�.
   if( code1 > code2 ){
     int temp;
	 SWAP( code1, code2, temp );
	 SWAP( StartX, EndX, temp );
	 SWAP( StartY, EndY, temp );
   }

   dx = EndX - StartX;
   dy = EndY - StartY;
   off= EndX * StartY - EndY * StartX;

   xl = (dx * clip->nStartY - off) / dy;
   yl = (dy * clip->nStartX + off) / dx;
   xr = (dx * clip->nEndY   - off) / dy;
   yr = (dy * clip->nEndX   + off) / dx;

   // ������ Ŭ������ ���� �� ������ Ŭ���� �Ѵ�.
   if( code1 == CS_CODE_NONE ){
     switch( code2 ){
       case CS_CODE_TOP :
				EndX = xl;
                EndY = clip->nStartY;
                break;
       case CS_CODE_BOTTOM :
				EndX = xr;
                EndY = clip->nEndY;
                break;
       case CS_CODE_LEFT :
				EndX = clip->nStartX;
                EndY = yl;
                break;
       case CS_CODE_RIGHT :
				EndX = clip->nEndX;
                EndY = yr;
                break;
       case CS_CODE_TOP + CS_CODE_LEFT :
				EndX = clip->nStartX;
                EndY = yl;
                if( (EndY < clip->nStartY) || (clip->nEndY < EndY) ){
                  EndX = xl;
                  EndY = clip->nStartY;
                }
                break;
       case CS_CODE_BOTTOM + CS_CODE_LEFT :
				EndX = clip->nStartX;
                EndY = yl;
                if( (EndY < clip->nStartY) || (clip->nEndY < EndY) ){
                  EndX = xr;
                  EndY = clip->nEndY;
                }
                break;
       case CS_CODE_TOP + CS_CODE_RIGHT :
				EndX = clip->nEndX;
                EndY = yr;
                if( (EndY < clip->nStartY) || (clip->nEndY < EndY) ){
                  EndX = xl;
                  EndY = clip->nStartY;
                }
                break;
       case CS_CODE_BOTTOM + CS_CODE_RIGHT :
				EndX = clip->nEndX;
                EndY = yr;
                if( (EndY < clip->nStartY) || (clip->nEndY < EndY) ){
                  EndX = xr;
                  EndY = clip->nEndY;
                }
                break;
     }

	 if( (StartX == EndX) && (StartY == EndY) ){
       fnDrawPixel( lpCanvas, StartX, StartY, ExParam1 );
     } else {
       fnDrawLine( lpCanvas, StartX, StartY, EndX, EndY, ExParam1 );
	 }
     return;
   }

   if( (clip->nStartX <  xl) && (xl <  clip->nEndX) ){ x[n] = xl; y[n++]= clip->nStartY; }
   if( (clip->nStartY <= yl) && (yl <= clip->nEndY) ){ y[n] = yl; x[n++]= clip->nStartX; }
   if( (clip->nStartX <  xr) && (xr <  clip->nEndX) ){ x[n] = xr; y[n++]= clip->nEndY; }
   if( (clip->nStartY <= yr) && (yr <= clip->nEndY) ){ y[n] = yr; x[n++]= clip->nEndX; }

   if( n == 1 )
     fnDrawPixel( lpCanvas, x[0], y[0], ExParam1 );
   else if( n == 2 )
     fnDrawLine( lpCanvas, x[0], y[0], x[1], y[1], ExParam1 );
}

int __atTGRPX_DrawLineDash0( atTGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY,
                        atLONG ExParam1, atINT nDashLen, atINT nGap, atINT init_gapdash,
						__LINECALLBACK__DRAW_PIXEL *fnDrawPixel, __LINECALLBACK__DRAW_LINE_DASH *fnDrawLine,
						__LINECALLBACK__DRAW_H_LINE_DASH *fnDrawHLine, __LINECALLBACK__DRAW_V_LINE_DASH *fnDrawVLine )
{
   int code1, code2;
   int xl, xr, yl, yr;
   long dx, dy, off;
   int n = 0;
   int x[2], y[2];
   atRECT *clip;

   // �������� ������ ��� ������ �׳� ���� ��� ������.
   if( (StartX == EndX) && (StartY == EndY) ){
     fnDrawPixel( lpCanvas, StartX, StartY, ExParam1 );
	 return 0;
   }
   // �������� ������ X��ǥ�� ������ �������� �׸��� ������.
   if( StartX == EndX ){
	 if( StartY > EndY ) SWAP( StartY, EndY, n );
     init_gapdash = fnDrawVLine( lpCanvas, StartX, StartY, EndY - StartY + 1, ExParam1, nDashLen, nGap, init_gapdash );
     return init_gapdash;
   }
   // �������� ������ Y��ǥ�� ������ ���򼱸� �׸��� ������.
   if( StartY == EndY ){
	 if( StartX > EndX ) SWAP( StartX, EndX, n );
     init_gapdash = fnDrawHLine( lpCanvas, StartX, StartY, EndX - StartX + 1, ExParam1, nDashLen, nGap, init_gapdash );
     return init_gapdash;
   }

   // Ŭ���� ������ ��´�.
   clip = atTGRPX_GetClipRect(lpCanvas);

   // Ŭ���� ������ ���� CS �ڵ� ����
   MAKE_CS_CODE_2( code1, code2, StartX, StartY, EndX, EndY, clip->nStartX, clip->nStartY, clip->nEndX, clip->nEndY  );

   // ���۰� ����ǥ�� ���ÿ� ���� Ŭ���� ������ ����� ����.
   if( code1 & code2 ) return 0; 

   // Ŭ���� �� �ʿ䰡 ������ �׳� �� �׸�.
   if( (code1 == CS_CODE_NONE) && (code2 == CS_CODE_NONE) ){
     init_gapdash = fnDrawLine( lpCanvas, StartX, StartY, EndX, EndY, ExParam1, nDashLen, nGap, init_gapdash );
     return init_gapdash;
   }

   // �������� Ŭ���� ���� ������ �����Ѵ�.
   if( code1 > code2 ){
     int temp;
	 SWAP( code1, code2, temp );
	 SWAP( StartX, EndX, temp );
	 SWAP( StartY, EndY, temp );
   }

   dx = EndX - StartX;
   dy = EndY - StartY;
   off= EndX * StartY - EndY * StartX;

   xl = (dx * clip->nStartY - off) / dy;
   yl = (dy * clip->nStartX + off) / dx;
   xr = (dx * clip->nEndY   - off) / dy;
   yr = (dy * clip->nEndX   + off) / dx;

   // ������ Ŭ������ ���� �� ������ Ŭ���� �Ѵ�.
   if( code1 == CS_CODE_NONE ){
     switch( code2 ){
       case CS_CODE_TOP :
				EndX = xl;
                EndY = clip->nStartY;
                break;
       case CS_CODE_BOTTOM :
				EndX = xr;
                EndY = clip->nEndY;
                break;
       case CS_CODE_LEFT :
				EndX = clip->nStartX;
                EndY = yl;
                break;
       case CS_CODE_RIGHT :
				EndX = clip->nEndX;
                EndY = yr;
                break;
       case CS_CODE_TOP + CS_CODE_LEFT :
				EndX = clip->nStartX;
                EndY = yl;
                if( (EndY < clip->nStartY) || (clip->nEndY < EndY) ){
                  EndX = xl;
                  EndY = clip->nStartY;
                }
                break;
       case CS_CODE_BOTTOM + CS_CODE_LEFT :
				EndX = clip->nStartX;
                EndY = yl;
                if( (EndY < clip->nStartY) || (clip->nEndY < EndY) ){
                  EndX = xr;
                  EndY = clip->nEndY;
                }
                break;
       case CS_CODE_TOP + CS_CODE_RIGHT :
				EndX = clip->nEndX;
                EndY = yr;
                if( (EndY < clip->nStartY) || (clip->nEndY < EndY) ){
                  EndX = xl;
                  EndY = clip->nStartY;
                }
                break;
       case CS_CODE_BOTTOM + CS_CODE_RIGHT :
				EndX = clip->nEndX;
                EndY = yr;
                if( (EndY < clip->nStartY) || (clip->nEndY < EndY) ){
                  EndX = xr;
                  EndY = clip->nEndY;
                }
                break;
     }

	 if( (StartX == EndX) && (StartY == EndY) ){
       fnDrawPixel( lpCanvas, StartX, StartY, ExParam1 );
	   init_gapdash = 0;
     } else {
       init_gapdash = fnDrawLine( lpCanvas, StartX, StartY, EndX, EndY, ExParam1, nDashLen, nGap, init_gapdash );
	 }
     return init_gapdash;
   }

   if( (clip->nStartX <  xl) && (xl <  clip->nEndX) ){ x[n] = xl; y[n++]= clip->nStartY; }
   if( (clip->nStartY <= yl) && (yl <= clip->nEndY) ){ y[n] = yl; x[n++]= clip->nStartX; }
   if( (clip->nStartX <  xr) && (xr <  clip->nEndX) ){ x[n] = xr; y[n++]= clip->nEndY; }
   if( (clip->nStartY <= yr) && (yr <= clip->nEndY) ){ y[n] = yr; x[n++]= clip->nEndX; }

   if( n == 1 )
   {
     fnDrawPixel( lpCanvas, x[0], y[0], ExParam1 );
     init_gapdash = 0;
   }
   else if( n == 2 )
     init_gapdash = fnDrawLine( lpCanvas, x[0], y[0], x[1], y[1], ExParam1, nDashLen, nGap, init_gapdash );

   return init_gapdash;
}

#endif

#pragma optimize( "g", on )
