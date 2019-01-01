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
#include "_QUD_func.h"
#include "AT_Real.h"


static void _DrawHorizLine( atGRPX_CANVAS *lpCanvas, atINT scan_y,
                            atREAL lx, atREAL ly, atREAL rx, atREAL ry,
                            atREAL ldxdy, atREAL rdxdy, atREAL alpha_init,
							atLONG ExParam1,
							__FQUDCALLBACK__DRAW_HLINE *fnDrawHLine )
{
	atRECT *clip;
    atREAL temp;
	atREAL cx1, cx2;

//    if( x1 == x2 ) return;

    if( lx > rx ){
      SWAP( lx, rx, temp );
      SWAP( ldxdy, rdxdy, temp );
    }

	clip = atGRPX_GetClipRect(lpCanvas);
    cx1 = INTTOREAL(clip->nStartX);
	cx2 = INTTOREAL(clip->nEndX);

    // 좌우 클리핑
	if( (rx < cx1) || (lx > cx2) ) return;
    if( lx < cx1 ){ lx = cx1; ldxdy = INTTOREAL(0); }
    if( rx > cx2 ){ rx = cx2; rdxdy = INTTOREAL(0); }

	fnDrawHLine( lpCanvas, scan_y, lx, ly, rx, ry, ldxdy, rdxdy, alpha_init, ExParam1 );
}


#define SPF_CALC_SLOPE(vv, v1, v2) \
   vv = (v2) - (v1);

// lpCanvas, sy, ey, lx, rx, ldx, ldy, rdx, rdy, clip, ExParam1, fnDrawHLine - 입력 파라미터
// ldxdy, rdxdy, i, t, t2 - 임시 변수
#define SPF_FILL_LINE( lpCanvas, sy, ey, lx, rx, ldx, ldy, rdx, rdy, clip, ExParam1, fnDrawHLine, ldxdy, rdxdy, i, t, t2 ) \
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
		    for( i = sy; i < ey; i++ ){       \
		      _DrawHorizLine( lpCanvas, lx, rx, i, ldxdy, rdxdy, ExParam1, fnDrawHLine ); \
		      lx += ldxdy; rx += rdxdy;        \
            }   \
          } \
        }


#define SPF_FILL_LINE2( lpCanvas, r_sy, r_ey, lx, rx, ldx, ldy, rdx, rdy, clip, ExParam1, fnDrawHLine, ldxdy, rdxdy, i, t, t2, r_i, r_t, r_t2 ) \
        if( r_sy <= INTTOREAL(clip->nEndY) ){ \
          if( r_ey < INTTOREAL(clip->nStartY) ){ \
            r_t = ey - sy; \
		    lx += REALMUL(ldxdy, r_t); rx += REALMUL(rdxdy, r_t); \
          } else { \
            if( r_ey > INTTOREAL(clip->nEndY) ){    \
              r_ey = INTTOREAL(clip->nEndY);           \
            }                             \
            if( r_sy < INTTOREAL(clip->nStartY) ){  \
              r_t = r_t2 = MIN( INTTOREAL(clip->nStartY), r_ey ); \
              r_t2 -= r_sy;                 \
              r_sy = r_t;                     \
		      lx += REALMUL(ldxdy, t2); rx += REALMUL(rdxdy, t2); \
            }                                   \
		    for( r_i = r_sy; r_i < r_ey; r_i += REAL_ONE ){       \
		      _DrawHorizLine( lpCanvas, lx, rx, REALTOINT(r_i), ldxdy, rdxdy, ExParam1, fnDrawHLine ); \
		      lx += ldxdy; rx += rdxdy;        \
            }   \
          } \
        }


extern void FlushCurrent();


static void __draw( atGRPX_CANVAS *lpCanvas, atINT *p_scan_y, atREAL *p_lx, atREAL *p_rx,
                                atREAL l_sy, atREAL l_ey, atREAL r_sy, atREAL r_ey,
//                               atRPOINT *l_sp, atRPOINT *l_ep, atRPOINT *r_sp, atRPOINT *r_ep,
                               atREAL ldx, atREAL ldy, atREAL rdx, atREAL rdy,
                               int change_edge,
						       atLONG ExParam1,
						      __FQUDCALLBACK__DRAW_HLINE *fnDrawHLine )
{
int i;
atREAL ldxdy, rdxdy;
atREAL ldxdy2, rdxdy2;
    atRECT *clip;

atREAL lx = *p_lx;
atREAL rx = *p_rx;
int ii, cnt;
int scan_y = *p_scan_y;
atREAL rey = MIN( l_ey, r_ey );
int ey = REALTOINT( rey );

//    if( (ABS(ldy) < FLOATTOREAL(0.000001f)) || (ABS(rdy) < FLOATTOREAL(0.000001f)) ) return; // 아주 작은 수일 경우, 사실상 0 취급

    clip = atGRPX_GetClipRect( lpCanvas );

//    if( REALTOINT(sy) > clip->nEndY ) return;
    if( scan_y > clip->nEndY ) return;

    if( ldy != INTTOREAL(0) )
        ldxdy = REALDIV( ldx, ldy );
    else {
        ldxdy = INTTOREAL(0);
        lx += ldx;
    }
    if( rdy != INTTOREAL(0) )
        rdxdy = REALDIV( rdx, rdy );
    else {
        rdxdy = INTTOREAL(0);
        rx += rdx;
    }

    // 맨 끝 지점도 클립핑 영역에 못 미치면, 좌표 계산만 마치고 나온다.
    if( ey < clip->nStartY ){
    	lx += REALMUL( ldxdy, l_ey - l_sy );
	    rx += REALMUL( rdxdy, r_ey - r_sy );
    	*p_scan_y = ey;
	    *p_lx = lx;
	    *p_rx = rx;
	    return;
    }

    if( scan_y < clip->nStartY ){
	    lx += REALMUL( ldxdy, INTTOREAL(clip->nStartY) - l_sy );
	    rx += REALMUL( rdxdy, INTTOREAL(clip->nStartY) - r_sy );
        scan_y = clip->nStartY;
    }
    if( ey > clip->nEndY ){
        ey = clip->nEndY;
    }

	// 수직 변위가 1보다 작을 때...
//	if( (rey - sy) < REAL_ONE ){
//		ldxdy2 = REALMUL( ldxdy, REALREALK(rey-y) );
//		rdxdy2 = REALMUL( rdxdy, REALREALK(rey-y) );
//        if( xxx ){
//           _DrawHorizLine( lpCanvas, scan_y+i, lx, INTTOREAL(scan_y), rx, INTTOREAL(scan_y+i),
//                           ldxdy, rdxdy, alp, ExParam1, fnDrawHLine );
//           scan_y ++;
//        }
//        lx += ldxdy2;
//        rx += rdxdy2;
//	} else

if( REALREALK(l_sy) > INTTOREAL(0) ){
	ldxdy2 = REALMUL( ldxdy, REAL_ONE-REALREALK(l_sy) );
} else {
    ldxdy2 = ldxdy;
}
if( REALREALK(r_sy) > INTTOREAL(0) ){
	rdxdy2 = REALMUL( rdxdy, REAL_ONE-REALREALK(r_sy) );
} else {
    rdxdy2 = rdxdy;
}

    {
		// 정수로 떨어지는 중간 라인들...
	      for( i = scan_y; i < ey; i++ ){
	           _DrawHorizLine( lpCanvas, i, lx, INTTOREAL(i), rx, INTTOREAL(i),
                                ldxdy, rdxdy, REAL_ONE, ExParam1, fnDrawHLine );
	        lx += ldxdy2;
	        rx += rdxdy2;
            
//FlushCurrent();

ldxdy2 = ldxdy;
rdxdy2 = rdxdy;
          }

        scan_y = ey;
	}

	*p_scan_y = scan_y;
	*p_lx = lx;
	*p_rx = rx;
}


/*******************************
  속이 채워진 사각형을 그린다.
********************************/
void __atGRPX_FillQuadrangle_REAL0( atGRPX_CANVAS *lpCanvas, atREAL rX1, atREAL rY1, atREAL rX2, atREAL rY2, atREAL rX3, atREAL rY3, atREAL rX4, atREAL rY4,
						       atLONG ExParam1,
						      __FQUDCALLBACK__DRAW_HLINE *fnDrawHLine )
{
	int i, j;
    atRPOINT p[4], *tp[4];
    atRPOINT *l_sp, *l_ep, *r_sp, *r_ep;
	struct { int idx; atREAL rY; } idx[4];
	int t, t2;
	atREAL rt;
	int pt1_y, pt2_y, pt3_y, pt4_y;
	atREAL rpt1_y, rpt2_y, rpt3_y, rpt4_y;
    int sy, ey;
    atREAL r_sy, r_ey;
    atREAL r_i, r_t, r_t2;
    atRECT *clip;
int scan_y;    
int step = 0;

    atREAL ldx, ldy, rdx, rdy;
    atREAL ldxdy, rdxdy;
    atREAL ldxdy2, rdxdy2;
    atREAL y;
    atREAL lx, ly, rx, ry;

    int is_cw;
    int l_idx, r_idx;
    int l_idx_inc, r_idx_inc;
    int change_edge;

	p[0].rX = rX1; p[0].rY = rY1;
	p[1].rX = rX2; p[1].rY = rY2;
	p[2].rX = rX3; p[2].rY = rY3;
	p[3].rX = rX4; p[3].rY = rY4;

	// 각 점을 Y좌표로 소트한다.
	for( i = 0; i < 4; i++ ){
	  idx[i].idx = i;
	  idx[i].rY = p[i].rY;
	}
	for( i = 0; i < 4-1; i++ ){
	  for( j = i+1; j < 4; j++ ){
		if( idx[i].rY > idx[j].rY ){
		  SWAP( idx[i].idx, idx[j].idx, t );
		  SWAP( idx[i].rY, idx[j].rY, rt );
		}
	  }
	}

	for( i = 0; i < 4; i++ ){
      tp[i] = &p[ idx[i].idx ];
    }

    clip = atGRPX_GetClipRect( lpCanvas );

	// 스캔라인 Y 기준점
	rpt1_y = idx[0].rY;
	rpt2_y = idx[1].rY;
	rpt3_y = idx[2].rY;
	rpt4_y = idx[3].rY;
	pt1_y = REALTOINT(idx[0].rY);
	pt2_y = REALTOINT(idx[1].rY);
	pt3_y = REALTOINT(idx[2].rY);
	pt4_y = REALTOINT(idx[3].rY);

    // 완전히 벗어나면 종료
    if( (pt1_y > clip->nEndY) || (pt4_y < clip->nStartY) ) return;
    if( MIN( tp[0]->rX, MIN( tp[1]->rX, MIN( tp[2]->rX, tp[3]->rX ) ) ) > INTTOREAL(clip->nEndX) ) return;
    if( MAX( tp[0]->rX, MAX( tp[1]->rX, MAX( tp[2]->rX, tp[3]->rX ) ) ) < INTTOREAL(clip->nStartX) ) return;

    // 만약 맨 처음 시작과 맨 마지막 끝이 같으면 그냥 수평선 그리고 끝.
    if( pt1_y == pt4_y ){
      atREAL x1, x2;
      x1 = x2 = tp[0]->rX;;
      for( i = 1; i < 4; i++ ){
        if( tp[i]->rX < x1 ) x1 = tp[i]->rX;
        if( tp[i]->rX > x2 ) x2 = tp[i]->rX;
      }
      _DrawHorizLine( lpCanvas, REALTOINT(rY1), x1, REALTOINT(rY1), x2, REALTOINT(rY1), INTTOREAL(0), INTTOREAL(0), REAL_ONE, ExParam1, fnDrawHLine );
      return;
    }

	// 꼭지점에서 중간 첫번째 지점까지 그린다.

    l_sp = r_sp = &p[ idx[0].idx ];
    r_idx = (idx[0].idx + 1) % 4;
    l_idx = (idx[0].idx + 3) % 4;
    r_ep = &p[ r_idx ];
    l_ep = &p[ l_idx ];

    // 두 벡터의 외적으로 사각형 방향을 얻는다.
    is_cw = REALTOINT(r_ep->rX - r_sp->rX) * REALTOINT(l_ep->rY - l_sp->rY) - REALTOINT(r_ep->rY - r_sp->rY) * REALTOINT(l_ep->rX - l_sp->rX);
    if( is_cw >= 0 ){
        r_idx_inc = 1;
        l_idx_inc = -1;
    } else {
        r_idx_inc = -1;
        l_idx_inc = 1;
        SWAP( l_idx, r_idx, i );
        r_ep = &p[ r_idx ];
        l_ep = &p[ l_idx ];
    }

    //change_edge = 0x1 | 0x2;
    change_edge = 0;

    SPF_CALC_SLOPE( ldx, l_sp->rX, l_ep->rX );
    SPF_CALC_SLOPE( ldy, l_sp->rY, l_ep->rY );
    SPF_CALC_SLOPE( rdx, r_sp->rX, r_ep->rX );
    SPF_CALC_SLOPE( rdy, r_sp->rY, r_ep->rY );
	lx = rx = l_sp->rX;

    y = idx[0].rY;

    ly = ry = y;
    step = 0;
    scan_y = REALTOINT(y);

/*
    // 맨 위의 Y 좌표가 정수가 아니면, 먼저 맨 윗쪽 한줄 처리를 해 놓는다.
    if( REALREALK(y) != INTTOREAL(0) ){
      if( ldy != 0 ){
        ldxdy = REALDIV( ldx, ldy );
        ldxdy2 = REALMUL( ldxdy, REAL_ONE - REALREALK(y) );
      } else {
        ldxdy2 = INTTOREAL(0);
        change_edge |= 0x1;
        lx += ldx;
      }
      if( rdy != 0 ){
        rdxdy = REALDIV( rdx, rdy );
        rdxdy2 = REALMUL( rdxdy, REAL_ONE - REALREALK(y) );
      } else {
        rdxdy2 = INTTOREAL(0);
        change_edge |= 0x2;
        rx += rdx;
      }
      if( y >= INTTOREAL(clip->nStartY) )
        _DrawHorizLine( lpCanvas, lx, ly, rx, ry, REALTOINT(y), REAL_ONE-REALREALK(y), ldxdy2, rdxdy2, ExParam1, fnDrawHLine );

      lx += ldxdy2;
      rx += rdxdy2;
      y = REALINTK(y) + REAL_ONE;

      scan_y++;
    }
*/

    // 각 스캔라인을 처리한다.
    while( step < 3 ){
        atREAL scan_dy = idx[step+1].rY - y;

//      if( scan_dy <= 0 ){
//        step++;
//        continue;
//      }

      if( change_edge & 0x1 ){
        SPF_CALC_SLOPE( ldx, l_sp->rX, l_ep->rX );
        SPF_CALC_SLOPE( ldy, l_sp->rY, l_ep->rY );
		lx = l_sp->rX;
      }
      if( change_edge & 0x2 ){
        SPF_CALC_SLOPE( rdx, r_sp->rX, r_ep->rX );
        SPF_CALC_SLOPE( rdy, r_sp->rY, r_ep->rY );
		rx = r_sp->rX;
      }

//        if( scan_dy >= REAL_ONE ){
            __draw( lpCanvas, &scan_y, &lx, &rx,
                               l_sp->rY, l_ep->rY, r_sp->rY, r_ep->rY,
                               ldx, ldy, rdx, rdy,
                               change_edge,
						       ExParam1, fnDrawHLine );
/*
        } else {
          if( scan_dy > 0 ){
            lx += REALMUL( ldxdy, scan_dy );
            rx += REALMUL( rdxdy, scan_dy );
            y += scan_dy;
          }*/
//        }

            change_edge = 0x0;
            if( scan_y >= REALTOINT(l_ep->rY) ){
              l_sp = l_ep;
              l_idx = (l_idx + l_idx_inc) & 0x3;
              l_ep = &p[ l_idx ];
              change_edge |= 0x1;
            }
            if( scan_y >= REALTOINT(r_ep->rY) ){
              r_sp = r_ep;
              r_idx = (r_idx + r_idx_inc) & 0x3;
              r_ep = &p[ r_idx ];
              change_edge |= 0x2;
            }
	     if( change_edge & (0x1 | 0x2) ){
		 y = MAX( l_sp->rY, r_sp->rY );
	     } else if( change_edge & 0x1 ){
		 y =  l_sp->rY;
	     } else if( change_edge & 0x1 ){
		 y = r_sp->rY ;
	     }

        step++;
    }
}

