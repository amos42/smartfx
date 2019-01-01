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
#include "_CVX_func.h"
#include "AT_Real.h"


static void _DrawHorizLine( atGRPX_CANVAS *lpCanvas, atINT scan_y,
                            atREAL lx, atREAL ly, atREAL rx, atREAL ry,
                            atREAL ldxdy, atREAL rdxdy, atREAL alpha_init, atDWORD dwAttr,
							atLONG ExParam1,
							__FCVXCALLBACK__DRAW_HLINE *fnDrawHLine )
{
	atRECT *clip;
    atREAL temp;
	atREAL cx1, cx2;
	atDWORD attr;

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
    attr = 0x0;	 
    if( dwAttr & 0x1000 ) attr |= 1;
    if( dwAttr & 0x2000 ) attr |= 2;
    if( lx < cx1 ){ lx = cx1; ldxdy = INTTOREAL(0); attr &= ~1; }
    if( rx > cx2 ){ rx = cx2; rdxdy = INTTOREAL(0); attr &= ~2; }

	fnDrawHLine( lpCanvas, scan_y, lx, ly, rx, ry, ldxdy, rdxdy, alpha_init, attr, ExParam1 );
}

#define SPF_CALC_SLOPE(vv, v1, v2) \
   vv = (v2) - (v1);

static atBOOL _draw( atGRPX_CANVAS *lpCanvas, atINT *p_scan_y, atREAL *p_lx, atREAL *p_rx,
                                atREAL l_sy, atREAL l_ey, atREAL r_sy, atREAL r_ey,
                               atREAL ldx, atREAL ldy, atREAL rdx, atREAL rdy,
                               int change_edge, atBOOL IsTop,
						       atLONG ExParam1,
						      __FCVXCALLBACK__DRAW_HLINE *fnDrawHLine )
{
	int i;
	atREAL ldxdy, rdxdy;
	atREAL ldxdy2, rdxdy2;
	atRECT *clip;

	atREAL lx = *p_lx;
	atREAL rx = *p_rx;
	int scan_y = *p_scan_y;
	atREAL rey = MIN( l_ey, r_ey );
	int ey = REALTOINT( rey );
	atREAL alp;
	atDWORD dwAttr;

    atBOOL isend = atFALSE;

    clip = atGRPX_GetClipRect( lpCanvas );

    if( scan_y > clip->nEndY ) return atTRUE;

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
    	lx += REALMUL( ldxdy, INTTOREAL(ey) - l_sy );
	    rx += REALMUL( rdxdy, INTTOREAL(ey) - r_sy );
    	*p_scan_y = ey;
	    *p_lx = lx;
	    *p_rx = rx;
	    return atFALSE;
    }

    if( scan_y < clip->nStartY ){
	    lx += ldxdy * (clip->nStartY - scan_y);
	    rx += rdxdy * (clip->nStartY - scan_y);
        scan_y = clip->nStartY;
    }
    if( ey > clip->nEndY ){
        ey = clip->nEndY;
        isend = atTRUE;
    }

	dwAttr = ((change_edge & 0x1000)? 0x1000:0x0000) | ((change_edge & 0x2000)? 0x2000:0x0000);

    if( scan_y < ey )
	if( REALREALK(l_sy) > 0 || REALREALK(r_sy) > 0 ){
	    if( REALREALK(l_sy) > INTTOREAL(0) )
		    ldxdy2 = REALMUL( ldxdy, REAL_ONE-REALREALK(l_sy) );
    	else
	        ldxdy2 = ldxdy;
    	if( REALREALK(r_sy) > INTTOREAL(0) )
	    	rdxdy2 = REALMUL( rdxdy, REAL_ONE-REALREALK(r_sy) );
    	else
	        rdxdy2 = rdxdy;

		alp = (IsTop)? REAL_ONE - (REALREALK(l_sy)+ REALREALK(r_sy)) / 2 : REAL_ONE;
	   	_DrawHorizLine( lpCanvas, scan_y, lx, INTTOREAL(scan_y), rx, INTTOREAL(MIN(scan_y+1,ey)), ldxdy2, rdxdy2, alp, dwAttr, ExParam1, fnDrawHLine );
		lx += ldxdy2;
		rx += rdxdy2;

        scan_y ++;
    }

	alp = REAL_ONE;

	for( i = scan_y; i < ey; i++ ){
	   	_DrawHorizLine( lpCanvas, i, lx, INTTOREAL(i), rx, INTTOREAL(i),
	                    ldxdy, rdxdy, REAL_ONE, dwAttr, ExParam1, fnDrawHLine );
		lx += ldxdy;
		rx += rdxdy;
	}

	// 임시...
/*
    if( REALREALK(rey) > 0 ){
	if( REALTOINT(rey) != REALTOINT(l_sy) )
        	ldxdy2 = REALMUL( REALREALK(rey), ldxdy );
	else
        	ldxdy2 = REALMUL( rey-l_sy, ldxdy );
	if( REALTOINT(rey) != REALTOINT(r_sy) )
        	rdxdy2 = REALMUL( REALREALK(rey), rdxdy );
	else
        	rdxdy2 = REALMUL( rey-r_sy, rdxdy );
   	_DrawHorizLine( lpCanvas, i, lx, rey, rx, rey, ldxdy2, rdxdy2, REAL_ONE, ExParam1, fnDrawHLine );
    }
*/

    if( ey > scan_y ) scan_y = ey;

	*p_scan_y = scan_y;
	*p_lx = lx;
	*p_rx = rx;

    return isend;
}


/*******************************
  속이 채워진 사각형을 그린다.
********************************/
void __atGRPX_FillConvexPolygon0_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrs[], atINT nPtrCount,
						              atINT lpAttr[], atLONG ExParam1,
						              __FCVXCALLBACK__DRAW_HLINE *fnDrawHLine )
{
	int i, t;
    atRPOINT *l_sp, *l_ep, *r_sp, *r_ep;
    atREAL min_y, max_y, min_x, max_x;
    int min_idx;
//    int sy, ey;
    atRECT *clip;
int scan_y, old_scan_y;
int step = 0;

    atREAL ldx, ldy, rdx, rdy;
    atREAL ldxdy, rdxdy;
    atREAL ldxdy2, rdxdy2;
    atREAL y;
    atREAL lx, ly, rx, ry;

    int is_cw;
    int l_idx, r_idx;
    int l_sp_idx, l_ep_idx, r_sp_idx, r_ep_idx;
    int l_idx_inc, r_idx_inc;
    int change_edge;
    int edge_cnt;


    atBOOL IsTop;
    atBOOL isend;

    rdx   = 0;
    rdxdy = 0;
    
    if( (nPtrCount > 3) &&
        (lpPtrs[0].rX == lpPtrs[nPtrCount-1].rX) && (lpPtrs[0].rY == lpPtrs[nPtrCount-1].rY) ){
        nPtrCount--;
    }


    max_y = min_y = lpPtrs[0].rY;
    max_x = min_x = lpPtrs[0].rX;
    min_idx = 0;

    // 가장 작은 y값과 인덱스를 얻는다.
	for( i = 1; i < nPtrCount; i++ ){
	  if( lpPtrs[i].rY < min_y ){
	    min_y = lpPtrs[i].rY;
	    min_idx = i;
	  }
	  if( lpPtrs[i].rY > max_y ) max_y = lpPtrs[i].rY;
	  if( lpPtrs[i].rX < min_x ) min_x = lpPtrs[i].rX;
	  if( lpPtrs[i].rX > max_x ) max_x = lpPtrs[i].rX;
	}

    clip = atGRPX_GetClipRect( lpCanvas );

    // 완전히 벗어나면 종료
    if( (min_y > INTTOREAL(clip->nEndY)) || (max_y < INTTOREAL(clip->nStartY)) ) return;
    if( (min_x > INTTOREAL(clip->nEndX)) || (max_x < INTTOREAL(clip->nStartX)) ) return;

    // 만약 맨 처음 시작과 맨 마지막 끝이 같으면 그냥 수평선 그리고 끝.
    if( REALTOINT(min_y) == REALTOINT(max_y) ){
      _DrawHorizLine( lpCanvas, REALTOINT(min_y), min_x, REALTOINT(min_y), max_x, REALTOINT(min_y), INTTOREAL(0), INTTOREAL(0), REAL_ONE, 0x1000|0x2000, ExParam1, fnDrawHLine );
      return;
    }

    l_sp = r_sp = &lpPtrs[ min_idx ];
    l_sp_idx = r_sp_idx = min_idx;
    r_idx = (min_idx + 1) % nPtrCount;
    l_idx = (min_idx + (nPtrCount-1)) % nPtrCount;
    r_ep = &lpPtrs[ r_idx ];
    l_ep = &lpPtrs[ l_idx ];

    //만약 시작점과 같은 점이 있다면, 같은 점 없을 때까지 뒤져야 한다.
    edge_cnt = nPtrCount;
    while( (l_sp->rX == l_ep->rX) && (l_sp->rY == l_ep->rY) ){
	l_idx--; if(l_idx < 0) l_idx += nPtrCount; l_idx %= nPtrCount;
	l_ep = &lpPtrs[ l_idx ];
	edge_cnt --;
    }
    while( (r_sp->rX == r_ep->rX) && (r_sp->rY == r_ep->rY) ){
	r_idx++; r_idx %= nPtrCount;
	r_ep = &lpPtrs[ r_idx ];
	edge_cnt --;
    }
	
    // 두 벡터의 외적으로 사각형 방향을 얻는다.
    is_cw = REALTOINT(r_ep->rX - r_sp->rX) * REALTOINT(l_ep->rY - l_sp->rY) - REALTOINT(r_ep->rY - r_sp->rY) * REALTOINT(l_ep->rX - l_sp->rX);
    if( is_cw >= 0 ){
        r_idx_inc = 1;
        l_idx_inc = -1;
    } else {
        r_idx_inc = -1;
        l_idx_inc = 1;
        SWAP( l_idx, r_idx, i );
        r_ep = &lpPtrs[ r_idx ];
        l_ep = &lpPtrs[ l_idx ];
    }

	lx = rx = l_sp->rX;

    y = min_y;

    ly = ry = y;
    step = 0;
    scan_y = REALTOINT(y);

    IsTop = atTRUE;

    // change_edge 1번째 자리:새로운 엣지를 찾아야 함. 2번째 자리:새로운 엣지를 계산해야 함. 5번째 자리: 엣지에 안티알리아싱 적용 여부
    change_edge = 0x10 | 0x20;
    l_ep_idx = l_idx;
    r_ep_idx = r_idx;
	if( lpAttr ){
        if( ((l_sp_idx==0) && (l_ep_idx==(nPtrCount-1))) || ((l_ep_idx==0) && (l_sp_idx==(nPtrCount-1))) ) t = nPtrCount-1; else t = MIN(l_sp_idx,l_ep_idx);
		change_edge |= (lpAttr[t])? 0x1000 : 0x0000;
        if( ((r_sp_idx==0) && (r_ep_idx==(nPtrCount-1))) || ((r_ep_idx==0) && (r_sp_idx==(nPtrCount-1))) ) t = nPtrCount-1; else t = MIN(r_sp_idx,r_ep_idx);
		change_edge |= (lpAttr[t])? 0x2000 : 0x0000;
	} else {
		change_edge |= 0x1000 | 0x2000;
	}

    // 각 스캔라인을 처리한다.
    while( step < (edge_cnt-1) && (scan_y <= clip->nEndY) ){
        if( change_edge & 0x01 ){
		    do {
           	    l_idx += l_idx_inc; if( l_idx < 0 ) l_idx += nPtrCount; else l_idx %= nPtrCount;
           	    l_ep = &lpPtrs[ l_idx ];
       		    step++;
          	} while( (l_ep->rX == l_sp->rX) && (l_ep->rY == l_sp->rY) );
            l_ep_idx = l_idx;
		if( lpAttr ){
            if( ((l_sp_idx==0) && (l_ep_idx==(nPtrCount-1))) || ((l_ep_idx==0) && (l_sp_idx==(nPtrCount-1))) ) t = nPtrCount-1; else t = MIN(l_sp_idx,l_ep_idx);
			if( lpAttr[t] ) change_edge |= 0x1000; else change_edge &= ~0x1000;
		}else
			change_edge |= 0x1000;
        }
        if( change_edge & 0x02 ){
		    do {
                r_idx += r_idx_inc; if( r_idx < 0 ) r_idx += nPtrCount; else r_idx %= nPtrCount;
                r_ep = &lpPtrs[ r_idx ];
	            step++;
        	} while( (r_ep->rX == r_sp->rX) && (r_ep->rY == r_sp->rY) );
            r_ep_idx = r_idx;
		if( lpAttr ){
            if( ((r_sp_idx==0) && (r_ep_idx==(nPtrCount-1))) || ((r_ep_idx==0) && (r_sp_idx==(nPtrCount-1))) ) t = nPtrCount-1; else t = MIN(r_sp_idx,r_ep_idx);
			if( lpAttr[t] ) change_edge |= 0x2000; else change_edge &= ~0x2000;
		}else
			change_edge |= 0x2000;
        }

          if( step >= (edge_cnt-1) ) break;

	      if( change_edge & 0x10 ){
            ldx = l_ep->rX - l_sp->rX;
            ldy = l_ep->rY - l_sp->rY;
			lx = l_sp->rX;
            ly = l_sp->rY;
            ldxdy = (ldy != INTTOREAL(0))? REALDIV( ldx, ldy ) : INTTOREAL(0);
	      }
	      if( change_edge & 0x20 ){
            rdx = r_ep->rX - r_sp->rX;
            rdy = r_ep->rY - r_sp->rY;
			rx = r_sp->rX;
            ry = r_sp->rY;
            rdxdy = (rdy != INTTOREAL(0))? REALDIV( rdx, rdy ) : INTTOREAL(0);
	      }

          old_scan_y = scan_y;
            isend = _draw( lpCanvas, &scan_y, &lx, &rx,
                               ly, l_ep->rY, ry, r_ep->rY,
                               ldx, ldy, rdx, rdy,
                               change_edge, IsTop,
						       ExParam1, fnDrawHLine );
            if( isend ) break;

	     if( scan_y > old_scan_y ){
           // 임시...
           IsTop = atFALSE;
           ly = ry = INTTOREAL(scan_y);
         }

         change_edge &= ~(0x11|0x22);
         if( scan_y >= REALTOINT(l_ep->rY) ){
            l_sp = l_ep;
            l_sp_idx = l_ep_idx;
            change_edge |= 0x11;
         }
         if( scan_y >= REALTOINT(r_ep->rY) ){
            r_sp = r_ep;
            r_sp_idx = r_ep_idx;
            change_edge |= 0x22;
         }

    }

    if( REALREALK(max_y) > INTTOREAL(0) ){
	    if( REALTOINT(l_sp->rY) == scan_y ){
	        ldxdy2 = REALMUL( ldxdy, max_y - l_sp->rY );
	    } else {
	        ldxdy2 = REALMUL( ldxdy, REALREALK(max_y) );
	    }
	    if( REALTOINT(r_sp->rY) == scan_y ){
	        rdxdy2 = REALMUL( rdxdy, max_y - r_sp->rY );
	    } else {
	        rdxdy2 = REALMUL( rdxdy, REALREALK(max_y) );
	    }
	    if( ldxdy2 > 0 && ldxdy2 > ldx ) ldxdy2 = ldx;
	    else if( ldxdy2 < 0 && ldxdy2 < ldx ) ldxdy2 = ldx;
	    if( rdxdy2 > 0 && rdxdy2 > rdx ) rdxdy2 = rdx;
	    else if( rdxdy2 < 0 && rdxdy2 < rdx ) rdxdy2 = rdx;
		
	    _DrawHorizLine( lpCanvas, scan_y, lx, max_y, rx, max_y, ldxdy2, rdxdy2, REALREALK(max_y), change_edge, ExParam1, fnDrawHLine );
    }
}

