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

#include "GrpX.h"
#include "_TRI_int.h"
#include "_TRI_func.h"
#include "AT_Real.h"


static void _DrawHorizLine( atGRPX_CANVAS *lpCanvas, atREAL x1, atREAL x2, int y, atREAL dx1, atREAL dx2, 
							atLONG ExParam1,
							__FTRICALLBACK__DRAW_HLINE *fnDrawHLine )
{
	atRECT *clip;
    atREAL temp;
	atREAL cx1, cx2;

//    if( x1 == x2 ) return;

    if( x1 > x2 ){
      SWAP( x1, x2, temp );
      SWAP( dx1, dx2, temp );
    }

	clip = atGRPX_GetClipRect(lpCanvas);
    cx1 = INTTOREAL(clip->nStartX);
	cx2 = INTTOREAL(clip->nEndX);

    // �¿� Ŭ����
	if( (x2 < cx1) || (x1 > cx2) ) return;
    if( x1 < cx1 ){ x1 = cx1; dx1 = INTTOREAL(0); }
    if( x2 > cx2 ){ x2 = cx2; dx2 = INTTOREAL(0); }

	fnDrawHLine( lpCanvas, x1, x2, y, dx1, dx2, ExParam1 );
}


// lpCanvas, sy, ey, lx, rx, ldx, ldy, rdx, rdy, clip, ExParam1, fnDrawHLine - �Է� �Ķ����
// ldxdy, rdxdy, i, t, t2 - �ӽ� ����
#define SPF_FILL_LINE( lpCanvas, sy, ey, lx, rx, ldx, ldy, rdx, rdy, clip, ExParam1, fnDrawHLine, ldxdy, rdxdy, i, t, t2 ) \
        ldxdy = REALDIV( ldx, ldy ); \
        rdxdy = REALDIV( rdx, rdy ); \
        if( sy <= clip->nEndY ){ \
          if( ey < clip->nStartY ){ \
            t = ey - sy + 1; \
		    lx += ldxdy * t; rx += rdxdy * t; \
          } else { \
            if( ey > clip->nEndY ){    \
              ey = clip->nEndY;           \
            }                             \
            if( sy < clip->nStartY ){  \
              t = t2 = MIN( clip->nStartY, ey ); \
              t2 -= sy;                 \
              sy = t;                     \
		      lx += ldxdy * t2; rx += rdxdy * t2; \
            }                                   \
		    for( i = sy; i <= ey; i++ ){       \
		      _DrawHorizLine( lpCanvas, lx, rx, i, ldxdy, rdxdy, ExParam1, fnDrawHLine ); \
		      lx += ldxdy; rx += rdxdy;        \
            }   \
          } \
        }

/*******************************
  ���� ä���� �ﰢ���� �׸���.
********************************/
void __atGRPX_FillTriangle0( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, 
						atLONG ExParam1,
						__FTRICALLBACK__DRAW_HLINE *fnDrawHLine )
{
    int y, height;
    atPOINT Top, Middle, Bottom;
    atREAL X_TpBt, X_TpMdBt;
    atREAL dX_TpBt, dX_TpMd, dX_MdBt;
	atRECT *clip;
	int mxx, mix;

	clip = atGRPX_GetClipRect( lpCanvas );

    // Ŭ���� ������ ������ ����� ����
	mix = MIN3(X1, X2, X3);
    if( mix > clip->nEndX ) return;
	mxx = MAX3(X1, X2, X3);
    if( mxx < clip->nStartX ) return;

    // Sorting Triangle Vertex
    SORT_TRI_VERTEXS( Top,Middle,Bottom, X1,Y1,X2,Y2,X3,Y3 );

    // Ŭ���� ������ ������ ����� ����
    if( Top.nY > clip->nEndY ) return;
    if( Bottom.nY < clip->nStartY ) return;

    // ����¥��
    if( Top.nY == Bottom.nY ){
	  _DrawHorizLine( lpCanvas, INTTOREAL(mix), INTTOREAL(mxx), Top.nY, INTTOREAL(0), INTTOREAL(0), ExParam1, fnDrawHLine );
      return;
    }

    // ������ ���⸦ ���Ѵ�.
    CALC_DELTA( Top,Middle,Bottom, dX_TpBt,dX_TpMd,dX_MdBt ); 

    //======== �������� ���������� �׸���. ===========
    
    X_TpBt = X_TpMdBt = INTTOREAL(Top.nX);
//    y = Top.nY;

    // ó������ �߰����� �׸���.
    if( (Top.nY < Middle.nY) && (Middle.nY >= clip->nStartY) ){
        y = Top.nY;
        
        if( Top.nY < clip->nStartY ){
          height = (clip->nStartY - Top.nY);
          X_TpBt   += (dX_TpBt * height);
          X_TpMdBt += (dX_TpMd * height);
          y = clip->nStartY;
        }
        if( Middle.nY > clip->nEndY ){
          Middle.nY = clip->nEndY;
        }

//        if( Middle.nY >= clip->nStartY ){
          while( y < Middle.nY ){
//            _DrawHorizLine( lpCanvas, REALTOINT(X_TpBt), REALTOINT(X_TpMdBt), y, ExParam1, fnDrawHLine );
            _DrawHorizLine( lpCanvas, X_TpBt, X_TpMdBt, y, dX_TpBt, dX_TpMd, ExParam1, fnDrawHLine );

            y ++;
            X_TpBt += dX_TpBt;
            X_TpMdBt += dX_TpMd;
          }
//        }
    }

    if( Middle.nY >= clip->nEndY ) return;

	// ���� �߰� ������ Ŭ���� ���� �ٱ��̸�...
      if( Middle.nY < clip->nStartY ){ 
          height = (Middle.nY - Top.nY + 1);
          X_TpBt   += (dX_TpBt * height);
          X_TpMdBt += (dX_TpMd * height);
      }

//    X_TpBt = INTTOREAL(Top.nX) + (dX_TpBt * (Middle.nY - Top.nY + 1)); // �̹� �ռ� ����� ����Ǿ� �����Ƿ�...
    X_TpMdBt = INTTOREAL(Middle.nX);

    // �߰����� ������ �׸���.
    if( Middle.nY < Bottom.nY ){
        y = Middle.nY;
    
        if( Middle.nY < clip->nStartY ){
          height = (clip->nStartY - Middle.nY + 1);
          X_TpBt += (dX_TpBt * height);
          X_TpMdBt += (dX_MdBt * height);        
    
          y = clip->nStartY;
        }
        if( Bottom.nY > clip->nEndY ){
          Bottom.nY = clip->nEndY;
        }
    
        while( y <= Bottom.nY ){
//            _DrawHorizLine( lpCanvas, REALTOINT(X_TpBt), REALTOINT(X_TpMdBt), y, ExParam1, fnDrawHLine );
            _DrawHorizLine( lpCanvas, X_TpBt, X_TpMdBt, y, dX_TpBt, dX_MdBt, ExParam1, fnDrawHLine );
    
            X_TpBt += dX_TpBt;
            X_TpMdBt += dX_MdBt;

            y++;
        }
    }
}




/*******************************
  ���� ä���� �ﰢ���� �׸���.
********************************/
void __atGRPX_FillTriangle0_REAL( atGRPX_CANVAS *lpCanvas, atREAL rX1, atREAL rY1, atREAL rX2, atREAL rY2, atREAL rX3, atREAL rY3, 
						atLONG ExParam1,
						__FTRICALLBACK__DRAW_HLINE *fnDrawHLine )
{
int X1, Y1, X2, Y2, X3, Y3;
    int y, height;
    atPOINT Top, Middle, Bottom;
    atREAL X_TpBt, X_TpMdBt;
    atREAL dX_TpBt, dX_TpMd, dX_MdBt;
	atRECT *clip;
	int mxx, mix;

X1 = REALTOINT(rX1); Y1 = REALTOINT(rY1);
X2 = REALTOINT(rX2); Y2 = REALTOINT(rY2);
X3 = REALTOINT(rX3); Y3 = REALTOINT(rY3);

	clip = atGRPX_GetClipRect( lpCanvas );

    // Ŭ���� ������ ������ ����� ����
	mix = MIN3(X1, X2, X3);
    if( mix > clip->nEndX ) return;
	mxx = MAX3(X1, X2, X3);
    if( mxx < clip->nStartX ) return;

    // Sorting Triangle Vertex
    SORT_TRI_VERTEXS( Top,Middle,Bottom, X1,Y1,X2,Y2,X3,Y3 );

    // Ŭ���� ������ ������ ����� ����
    if( Top.nY > clip->nEndY ) return;
    if( Bottom.nY < clip->nStartY ) return;

    // ����¥��
    if( Top.nY == Bottom.nY ){
	  _DrawHorizLine( lpCanvas, INTTOREAL(mix), INTTOREAL(mxx), Top.nY, INTTOREAL(0), INTTOREAL(0), ExParam1, fnDrawHLine );
      return;
    }

    // ������ ���⸦ ���Ѵ�.
    CALC_DELTA( Top,Middle,Bottom, dX_TpBt,dX_TpMd,dX_MdBt ); 

    //======== �������� ���������� �׸���. ===========
    
    X_TpBt = X_TpMdBt = INTTOREAL(Top.nX);
//    y = Top.nY;

    // ó������ �߰����� �׸���.
    if( (Top.nY < Middle.nY) && (Middle.nY >= clip->nStartY) ){
        y = Top.nY;
        
        if( Top.nY < clip->nStartY ){
          height = (clip->nStartY - Top.nY);
          X_TpBt   += (dX_TpBt * height);
          X_TpMdBt += (dX_TpMd * height);
          y = clip->nStartY;
        }
        if( Middle.nY > clip->nEndY ){
          Middle.nY = clip->nEndY;
        }

//        if( Middle.nY >= clip->nStartY ){
          while( y < Middle.nY ){
//            _DrawHorizLine( lpCanvas, REALTOINT(X_TpBt), REALTOINT(X_TpMdBt), y, ExParam1, fnDrawHLine );
            _DrawHorizLine( lpCanvas, X_TpBt, X_TpMdBt, y, dX_TpBt, dX_TpMd, ExParam1, fnDrawHLine );

            y ++;
            X_TpBt += dX_TpBt;
            X_TpMdBt += dX_TpMd;
          }
//        }
    }

    if( Middle.nY >= clip->nEndY ) return;

	// ���� �߰� ������ Ŭ���� ���� �ٱ��̸�...
      if( Middle.nY < clip->nStartY ){ 
          height = (Middle.nY - Top.nY + 1);
          X_TpBt   += (dX_TpBt * height);
          X_TpMdBt += (dX_TpMd * height);
      }

//    X_TpBt = INTTOREAL(Top.nX) + (dX_TpBt * (Middle.nY - Top.nY + 1)); // �̹� �ռ� ����� ����Ǿ� �����Ƿ�...
    X_TpMdBt = INTTOREAL(Middle.nX);

    // �߰����� ������ �׸���.
    if( Middle.nY < Bottom.nY ){
        y = Middle.nY;
    
        if( Middle.nY < clip->nStartY ){
          height = (clip->nStartY - Middle.nY + 1);
          X_TpBt += (dX_TpBt * height);
          X_TpMdBt += (dX_MdBt * height);        
    
          y = clip->nStartY;
        }
        if( Bottom.nY > clip->nEndY ){
          Bottom.nY = clip->nEndY;
        }
    
        while( y <= Bottom.nY ){
//            _DrawHorizLine( lpCanvas, REALTOINT(X_TpBt), REALTOINT(X_TpMdBt), y, ExParam1, fnDrawHLine );
            _DrawHorizLine( lpCanvas, X_TpBt, X_TpMdBt, y, dX_TpBt, dX_MdBt, ExParam1, fnDrawHLine );
    
            X_TpBt += dX_TpBt;
            X_TpMdBt += dX_MdBt;

            y++;
        }
    }
}


