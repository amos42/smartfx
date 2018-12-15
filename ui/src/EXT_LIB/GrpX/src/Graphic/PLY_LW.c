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
#include "TriFunc.h"
#include "_LIN_func.h"
#include "_OL_func.h"
#include "_CVX2_func.h"
#include <string.h>

#define USE_BEVELJOIN	1

typedef atGRPX_PIXEL *_FUNC_DRAW_PIXEL( atGRPX_CANVAS *, atINT, atINT, atGRPX_COLOR );
typedef void _FUNC_DRAW_LINE( atGRPX_CANVAS *, atINT, atINT, atINT, atINT, atGRPX_COLOR, atINT, atINT );


static void RoundCap( atGRPX_CANVAS *lpCanvas, atREAL rCenterX, atREAL rCenterY, atREAL rRadius, atREAL rStartAngle, atREAL rEndAngle, atREAL rAngleC, atREAL rStartX, atREAL rStartY, atREAL rEndX, atREAL rEndY, atGRPX_COLOR Color, atBOOL isAA,
					   atLPGRPX_FUNC_COORD_CONV_REAL lpCnvFunc, atLPVOID lpParam )
{
    atRPOINT ptrs[360];
    atINT attrs[360];
    int i, cnt;
    atREAL angle;
    atREAL vvv = 0;

    if( rAngleC == 0 ) return;
    if( rStartAngle == rEndAngle ) return;
    if( (rStartX == rEndX) && (rStartY == rEndY) ) return;	
    if( rRadius <= REAL_ONE ) return;

    if( rAngleC < 0 ){
        SWAP( rStartAngle, rEndAngle, angle );
        SWAP( rStartX, rEndX, angle );
        SWAP( rStartY, rEndY, angle );
        rAngleC = -rAngleC;
    }

    if( rStartAngle >= 0 && rEndAngle >= 0 ){
        if( rEndAngle > rStartAngle )
            vvv = rEndAngle - rStartAngle;
        else
            vvv = (INTTOREAL(180) - rStartAngle) + INTTOREAL(180) + rEndAngle;
    } else if( rStartAngle >= 0 && rEndAngle < 0 ){
        vvv = (INTTOREAL(180) - rStartAngle) + (INTTOREAL(180) + rEndAngle);
    } else if( rStartAngle < 0 && rEndAngle >= 0 ){
        vvv = (-rStartAngle) + rEndAngle;
    } else if( rStartAngle < 0 && rEndAngle < 0 ){
        if( rStartAngle < rEndAngle )
            vvv = rEndAngle - rStartAngle;
        else
            vvv = (-rStartAngle) + INTTOREAL(180) + (INTTOREAL(180) + rEndAngle);
    }

//    if( vvv < INTTOREAL(10) ) return;

    ptrs[0].rX = rCenterX; ptrs[0].rY = rCenterY;
    ptrs[1].rX = rStartX; ptrs[1].rY = rStartY;
    attrs[0] = atFALSE;

    angle = rStartAngle + rAngleC;
    cnt = REALTOINT((vvv - ABS(rAngleC))) / ABS(REALTOINT(rAngleC));
//    cnt = REALTOINT(vvv) / REALTOINT(rAngleC);
    if( cnt > 0 ){
        for( i = 0; i < cnt; i++ ){
    //      if( angle > INTTOREAL(360) ) angle -= INTTOREAL(360);
            ptrs[2+i].rX = rCenterX + REALMUL( COS(angle), rRadius );
            ptrs[2+i].rY = rCenterY + REALMUL( SIN(angle), rRadius );
	     attrs[1+i] = atTRUE;
            angle += rAngleC;
        }
    } else {
        cnt = 0;
    }

    attrs[1+cnt] = atTRUE;
    ptrs[2+cnt].rX = rEndX; ptrs[2+cnt].rY = rEndY;
    attrs[2+cnt] = atFALSE;
    cnt += 3;

    if( lpCnvFunc ){
        for( i = 0; i < cnt; i++ ){
          lpCnvFunc( lpParam, &ptrs[i].rX, &ptrs[i].rY, ptrs[i].rX, ptrs[i].rY );
        }
    }

    if( isAA ){
        atGRPX_FillConvexPolygonExAA_REAL( lpCanvas, ptrs, cnt, attrs, Color );
    } else {
        atGRPX_FillConvexPolygon_REAL( lpCanvas, ptrs, cnt, Color );
    }
}

// Kevin
static void RoundCap2_REAL( atGRPX_CANVAS *lpCanvas, atREAL rCenterX, atREAL rCenterY, atREAL rRadius, atREAL rStartAngle, atREAL rEndAngle, atREAL rAngleC, atREAL rStartX, atREAL rStartY, atREAL rEndX, atREAL rEndY, atGRPX_COLOR Color, atBOOL isAA,
						   atLPGRPX_FUNC_COORD_CONV_REAL lpCnvFunc, atLPVOID lpParam )
{
    atRPOINT ptrs[8];
    int i, cnt;
    atREAL angle;
    atREAL vvv = 0;
	
    if( rAngleC == 0 ) return;
    if( rStartAngle == rEndAngle ) return;
    if( (rStartX == rEndX) && (rStartY == rEndY) ) return;	
    if( rRadius <= REAL_ONE ) return;
	
    if( rAngleC < 0 ){
        SWAP( rStartAngle, rEndAngle, angle );
        SWAP( rStartX, rEndX, angle );
        SWAP( rStartY, rEndY, angle );
        rAngleC = -rAngleC;
    }
	
    if( rStartAngle >= 0 && rEndAngle >= 0 ){
        if( rEndAngle > rStartAngle )
            vvv = rEndAngle - rStartAngle;
        else
            vvv = (INTTOREAL(180) - rStartAngle) + INTTOREAL(180) + rEndAngle;
    } else if( rStartAngle >= 0 && rEndAngle < 0 ){
        vvv = (INTTOREAL(180) - rStartAngle) + (INTTOREAL(180) + rEndAngle);
    } else if( rStartAngle < 0 && rEndAngle >= 0 ){
        vvv = (-rStartAngle) + rEndAngle;
    } else if( rStartAngle < 0 && rEndAngle < 0 ){
        if( rStartAngle < rEndAngle )
            vvv = rEndAngle - rStartAngle;
        else
            vvv = (-rStartAngle) + INTTOREAL(180) + (INTTOREAL(180) + rEndAngle);
    }
	
//	if( vvv < INTTOREAL(10) ) return;
	
    ptrs[0].rX = rCenterX; ptrs[0].rY = rCenterY;
    ptrs[1].rX = rStartX; ptrs[1].rY = rStartY;
	
    angle = rStartAngle + rAngleC;
    cnt = REALTOINT((vvv - ABS(rAngleC))) / ABS(REALTOINT(rAngleC));
//	cnt = REALTOINT(vvv) / REALTOINT(rAngleC);
    if( cnt > 0 ){
        for( i = 0; i < cnt; i++ ){
	//      if( angle > INTTOREAL(360) ) angle -= INTTOREAL(360);
            ptrs[2+i].rX = rCenterX + REALMUL( COS(angle), rRadius );
            ptrs[2+i].rY = rCenterY + REALMUL( SIN(angle), rRadius );
            angle += rAngleC;
        }
    } else {
        cnt = 0;
    }
	
    ptrs[2+cnt].rX = rEndX; ptrs[2+cnt].rY = rEndY;
    cnt += 3;
	
    if( lpCnvFunc ){
        for( i = 0; i < cnt; i++ ){
			lpCnvFunc( lpParam, &ptrs[i].rX, &ptrs[i].rY, ptrs[i].rX, ptrs[i].rY );
        }
    }
	
	__atGRPX_FillConvexPolygon1_REAL(lpCanvas, ptrs, cnt, Color, isAA, atFALSE);
}


/*******************
  다각형 라인을 그린다.
********************/
static void _atGRPX_DrawPolyLineWidth0( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth,
						   _FUNC_DRAW_PIXEL *fnDrawPixel, _FUNC_DRAW_LINE *fnDrawLine, int isaa,
						   atLPGRPX_FUNC_COORD_CONV_REAL lpCnvFunc, atLPVOID lpParam )
{
	int i, j, draw_cnt;
	atPOINT *lpPtr, *lpPtr2, *lpPtrOld = atNULL;
    atBOOL IsOpen, IsDrawClose;
	atRPOINT kPtOut[4], kPtOut_t[4];
	atRPOINT kPtOut_old[4], kPtOut0[4];
	atINT attrList[10];
    atRPOINT *ptRC1, *ptRC2;
    atREAL angle, angle_old = 0, angle0;
    atREAL angle_t, angle_c;
    
    angle  = 0;
    angle0 = 0;

	if( lpPtrList == atNULL ) return;
	if( nCount <= 0 ) return;
	if( nWidth <= 0 ) return;

	if( nWidth <= 2 )
		angle_t = INTTOREAL(0);
	else if( nWidth <= 5 )
		angle_t = INTTOREAL(90);
	else if( nWidth <= 9 )
		angle_t = INTTOREAL(45);
	else if( nWidth <= 12 )
		angle_t = INTTOREAL(20);
	else 
		angle_t = INTTOREAL(10);
	
	lpPtr = lpPtrList;

	if( nCount == 1 ){
		atRPOINT pp0;
		atPOINT pp;
		if( lpCnvFunc ){
			if( !lpCnvFunc( lpParam, &pp0.rX, &pp0.rY, INTTOREAL(lpPtr->nX), INTTOREAL(lpPtr->nY) ) ) return;
			pp.nX = REALTOINT(pp0.rX); pp.nY = REALTOINT(pp0.rY);
			lpPtr = &pp;
		}
		fnDrawPixel( lpCanvas, lpPtr->nX, lpPtr->nY, Color );
		return;
    }

    if( nCount == 2 ){
      IsOpen = atTRUE;
    } else if( (lpPtrList[0].nX != lpPtrList[nCount-1].nX) || (lpPtrList[0].nY != lpPtrList[nCount-1].nY) ){
      IsOpen = atTRUE;
    } else {
      IsOpen = atFALSE;
    }
	IsDrawClose = ( bClosed || !IsOpen )? atTRUE : atFALSE;

	attrList[0] = 0;
	attrList[1] = 1;
	attrList[2] = 0;
	attrList[3] = 1;
	
	lpPtr2 = lpPtr + 1;

    draw_cnt = 0;
    if( bClosed ) nCount++;
	for( i = 1; i < nCount; i++ ){
        if( bClosed && (i == nCount-1) ){
            lpPtr2 = lpPtrList;
        } else {
    	    lpPtr2 = lpPtr + 1;
        }

		// 점의 위치가 같으면 무시
		if( (lpPtr->nX != lpPtr2->nX) || (lpPtr->nY != lpPtr2->nY) ){
    		angle = __atGRPX_GetOutlinePtrs_REAL( INTTOREAL(lpPtr->nX), INTTOREAL(lpPtr->nY), INTTOREAL(lpPtr2->nX), INTTOREAL(lpPtr2->nY), INTTOREAL(nWidth), kPtOut, atFALSE, atFALSE );
		if( lpCnvFunc ){
			for( j = 0; j < 4; j++ ) lpCnvFunc( lpParam, &kPtOut_t[j].rX, &kPtOut_t[j].rY, kPtOut[j].rX, kPtOut[j].rY );
		}

            if( draw_cnt == 0 ){
              angle0 = angle;
              memcpy( kPtOut0, kPtOut, sizeof(kPtOut));
            }
//            if( angle < 0 ) angle += INTTOREAL(360);
            if( angle > INTTOREAL(180) ) angle -= INTTOREAL(360);

            if( isaa ){
			if( lpCnvFunc )
    				atGRPX_FillConvexPolygonExAA_REAL( lpCanvas, kPtOut_t, 4, attrList, Color );
			else
    				atGRPX_FillConvexPolygonExAA_REAL( lpCanvas, kPtOut, 4, attrList, Color );
            } else {
			if( lpCnvFunc )
    				atGRPX_FillConvexPolygon_REAL( lpCanvas, kPtOut_t, 4, Color );
			else
    				atGRPX_FillConvexPolygon_REAL( lpCanvas, kPtOut, 4, Color );
            }

    		draw_cnt ++;
            if( (draw_cnt >= 2) && (angle_t > INTTOREAL(0)) ){
                atREAL s_a, e_a;
                int is_cw;
                ptRC1 = atNULL;

                // 두 벡터의 외적으로 방향을 얻는다.
                is_cw = (lpPtr2->nX - lpPtr->nX) * (lpPtrOld->nY - lpPtr->nY) - (lpPtr2->nY - lpPtr->nY) * (lpPtrOld->nX - lpPtr->nX);
                if( is_cw > 0 ){
                    ptRC1 = &kPtOut_old[2]; ptRC2 = &kPtOut[1];
                    s_a = angle_old - INTTOREAL(90);
                    if( s_a < INTTOREAL(-180) ) s_a += INTTOREAL(360);
                    e_a = angle - INTTOREAL(90);
                    if( e_a < INTTOREAL(-180) ) e_a += INTTOREAL(360);
                    angle_c = angle_t;
               } else if( is_cw < 0 ){
                    ptRC1 = &kPtOut_old[3]; ptRC2 = &kPtOut[0];
                    s_a = angle_old + INTTOREAL(90);
                    if( s_a > INTTOREAL(180) ) s_a -= INTTOREAL(360);
                    e_a = angle + INTTOREAL(90);
                    if( e_a > INTTOREAL(180) ) e_a -= INTTOREAL(360);
                    angle_c = -angle_t;
               }
               if( ptRC1 != atNULL ){
                      RoundCap( lpCanvas, INTTOREAL(lpPtr->nX), INTTOREAL(lpPtr->nY), INTTOREAL(nWidth)/2, s_a, e_a, angle_c, ptRC1->rX, ptRC1->rY, ptRC2->rX, ptRC2->rY, Color, isaa, lpCnvFunc, lpParam );
               }
            }

            memcpy( kPtOut_old, kPtOut, sizeof(kPtOut));
            angle_old = angle;
        }

        lpPtrOld = lpPtr;
	    lpPtr = lpPtr2;
	}

    if( bClosed && (draw_cnt >= 2) ){
		
	// 폐곡선일 경우, 처음과 끝이 연결되는 부분에 라운드캡 씌워야 함.
	// 귀찮아서 나중에 구현하도록 미룸... --;
		
    } else if( !bClosed && (draw_cnt >= 1) ){
        ptRC2 = &kPtOut[3]; ptRC1 = &kPtOut[2];
        RoundCap( lpCanvas, INTTOREAL(lpPtr->nX), INTTOREAL(lpPtr->nY), INTTOREAL(nWidth)/2, angle - INTTOREAL(90), angle + INTTOREAL(90), angle_t, ptRC1->rX, ptRC1->rY, ptRC2->rX, ptRC2->rY, Color, isaa, lpCnvFunc, lpParam );
        ptRC2 = &kPtOut0[1]; ptRC1 = &kPtOut0[0];
        angle0 += INTTOREAL(180);
        if( angle0 >= INTTOREAL(360) ) angle0 -= INTTOREAL(360);
        RoundCap( lpCanvas, INTTOREAL(lpPtrList[0].nX), INTTOREAL(lpPtrList[0].nY), INTTOREAL(nWidth)/2, angle0 - INTTOREAL(90), angle0 + INTTOREAL(90), angle_t, ptRC1->rX, ptRC1->rY, ptRC2->rX, ptRC2->rY, Color, isaa, lpCnvFunc, lpParam );
    }
}

/*******************
  다각형 라인을 그린다. Kevin
********************/
static void _atGRPX_DrawPolyLineWidth1( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth,
						   _FUNC_DRAW_PIXEL *fnDrawPixel, int isaa,
						   atLPGRPX_FUNC_COORD_CONV_REAL lpCnvFunc, atLPVOID lpParam )
{
	int i, j, draw_cnt;
	atPOINT *lpPtr, *lpPtr2, *lpPtrOld =atNULL;
    atBOOL IsOpen, IsDrawClose;
	atRPOINT kPtOut[4], kPtOut_t[4];
	atRPOINT kPtOut_old[4], kPtOut0[4];
    atRPOINT *ptRC1, *ptRC2;
    atREAL angle, angle_old = 0, angle0, angle_t;
#if !USE_BEVELJOIN
	atREAL s_a, e_a, angle_c;
#endif

    angle  = 0;
    angle0 = 0;

	if( lpPtrList == atNULL ) return;
	if( nCount <= 0 ) return;
	if( nWidth <= 0 ) return;

	switch(nWidth)
	{
	case 0:
	case 1:
	case 2:
		angle_t = INTTOREAL(0);
		break;
	case 3:
	case 4:
	case 5:
		angle_t = INTTOREAL(60);
		break;
	case 6:
	case 7:
	case 8:
		angle_t = INTTOREAL(36);
		break;
	default:
		angle_t = INTTOREAL(30);
		break;
	}
	
	lpPtr = lpPtrList;

	if( nCount == 1 ){
		atRPOINT pp0;
		atPOINT pp;
		if( lpCnvFunc ){
			if( !lpCnvFunc( lpParam, &pp0.rX, &pp0.rY, INTTOREAL(lpPtr->nX), INTTOREAL(lpPtr->nY) ) ) return;
			pp.nX = REALTOINT(pp0.rX); pp.nY = REALTOINT(pp0.rY);
			lpPtr = &pp;
		}
		fnDrawPixel( lpCanvas, lpPtr->nX, lpPtr->nY, Color );
		return;
    }

    if( nCount == 2 ){
      IsOpen = atTRUE;
    } else if( (lpPtrList[0].nX != lpPtrList[nCount-1].nX) || (lpPtrList[0].nY != lpPtrList[nCount-1].nY) ){
      IsOpen = atTRUE;
    } else {
      IsOpen = atFALSE;
    }
	IsDrawClose = ( bClosed || !IsOpen )? atTRUE : atFALSE;
	
	lpPtr2 = lpPtr + 1;

    draw_cnt = 0;
    if( bClosed ) nCount++;
	for( i = 1; i < nCount; i++ ){
        if( bClosed && (i == nCount-1) ){
            lpPtr2 = lpPtrList;
        } else {
    	    lpPtr2 = lpPtr + 1;
        }

		// 점의 위치가 같으면 무시
		if( (lpPtr->nX != lpPtr2->nX) || (lpPtr->nY != lpPtr2->nY) ){
    		angle = __atGRPX_GetOutlinePtrs_REAL( INTTOREAL(lpPtr->nX), INTTOREAL(lpPtr->nY), INTTOREAL(lpPtr2->nX), INTTOREAL(lpPtr2->nY), INTTOREAL(nWidth), kPtOut, atFALSE, atFALSE );
		if( lpCnvFunc ){
			for( j = 0; j < 4; j++ ) lpCnvFunc( lpParam, &kPtOut_t[j].rX, &kPtOut_t[j].rY, kPtOut[j].rX, kPtOut[j].rY );
		}

            if( draw_cnt == 0 ){
              angle0 = angle;
              memcpy( kPtOut0, kPtOut, sizeof(kPtOut));
            }
//            if( angle < 0 ) angle += INTTOREAL(360);
            if( angle > INTTOREAL(180) ) angle -= INTTOREAL(360);

			if( lpCnvFunc )
    			__atGRPX_FillConvexPolygon1_REAL( lpCanvas, kPtOut_t, 4, Color, isaa, atFALSE );
			else
    			__atGRPX_FillConvexPolygon1_REAL( lpCanvas, kPtOut, 4, Color, isaa, atFALSE );

    		draw_cnt ++;
            if( (draw_cnt >= 2) && (angle_t > 0) && (nWidth/2 > 1)){
				// 두 벡터의 외적으로 방향을 얻는다.
				atINT is_cw;
				is_cw = (lpPtr2->nX - lpPtr->nX) * (lpPtrOld->nY - lpPtr->nY) - (lpPtr2->nY - lpPtr->nY) * (lpPtrOld->nX - lpPtr->nX);
				ptRC1 = atNULL;
#if USE_BEVELJOIN
				if( is_cw > 0 ){
					ptRC1 = &kPtOut_old[2]; ptRC2 = &kPtOut[1];
				} else/* if( is_cw < 0 )*/{
					ptRC1 = &kPtOut_old[3]; ptRC2 = &kPtOut[0];
				}
				if( ptRC1 != atNULL )
				{
					if(ptRC1->rX!=ptRC2->rX || ptRC1->rY!=ptRC2->rY)
					{
						atRPOINT ptTri[3];
						ptTri[0].rX = INTTOREAL(lpPtr->nX);
						ptTri[0].rY = INTTOREAL(lpPtr->nY);
						ptTri[1].rX = ptRC1->rX;
						ptTri[1].rY = ptRC1->rY;
						ptTri[2].rX = ptRC2->rX;
						ptTri[2].rY = ptRC2->rY;
						__atGRPX_FillConvexPolygon1_REAL(lpCanvas, ptTri, 3, Color, isaa, atFALSE);
					}
				}
#else
                if( is_cw > 0 ){
                    ptRC1 = &kPtOut_old[2]; ptRC2 = &kPtOut[1];
                    s_a = angle_old - INTTOREAL(90);
                    if( s_a < INTTOREAL(-180) ) s_a += INTTOREAL(360);
                    e_a = angle - INTTOREAL(90);
                    if( e_a < INTTOREAL(-180) ) e_a += INTTOREAL(360);
                    angle_c = angle_t;
				} else if( is_cw < 0 ){
                    ptRC1 = &kPtOut_old[3]; ptRC2 = &kPtOut[0];
                    s_a = angle_old + INTTOREAL(90);
                    if( s_a > INTTOREAL(180) ) s_a -= INTTOREAL(360);
                    e_a = angle + INTTOREAL(90);
                    if( e_a > INTTOREAL(180) ) e_a -= INTTOREAL(360);
                    angle_c = -angle_t;
				}
				if( ptRC1 != atNULL ){
					RoundCap2_REAL( lpCanvas, INTTOREAL(lpPtr->nX), INTTOREAL(lpPtr->nY), INTTOREAL(nWidth/2), s_a, e_a, angle_c, ptRC1->rX, ptRC1->rY, ptRC2->rX, ptRC2->rY, Color, isaa, lpCnvFunc, lpParam );
				}
#endif
            }

            memcpy( kPtOut_old, kPtOut, sizeof(kPtOut));
            angle_old = angle;
        }

        lpPtrOld = lpPtr;
	    lpPtr = lpPtr2;
	}

    if( bClosed && (draw_cnt >= 2) ){
		
	// 폐곡선일 경우, 처음과 끝이 연결되는 부분에 라운드캡 씌워야 함.
	// 귀찮아서 나중에 구현하도록 미룸... --;
		
    } else if( !bClosed && (draw_cnt >= 1) ){
        ptRC2 = &kPtOut[3]; ptRC1 = &kPtOut[2];
        RoundCap2_REAL( lpCanvas, INTTOREAL(lpPtr->nX), INTTOREAL(lpPtr->nY), INTTOREAL(nWidth/2), angle - INTTOREAL(90), angle + INTTOREAL(90), angle_t, ptRC1->rX, ptRC1->rY, ptRC2->rX, ptRC2->rY, Color, isaa, lpCnvFunc, lpParam );
        ptRC2 = &kPtOut0[1]; ptRC1 = &kPtOut0[0];
        angle0 += INTTOREAL(180);
        if( angle0 >= INTTOREAL(360) ) angle0 -= INTTOREAL(360);
        RoundCap2_REAL( lpCanvas, INTTOREAL(lpPtrList[0].nX), INTTOREAL(lpPtrList[0].nY), INTTOREAL(nWidth/2), angle0 - INTTOREAL(90), angle0 + INTTOREAL(90), angle_t, ptRC1->rX, ptRC1->rY, ptRC2->rX, ptRC2->rY, Color, isaa, lpCnvFunc, lpParam );
    }
}

// init_gapdash < 0 : dash 길이, >0 : gap 길이
static int _atGRPX_DrawLineWidthDash(atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth, atINT bAA, atINT nDashLen, atINT nGap, atINT init_gapdash)
{
	atINT x1, y1, x2, y2, dist, sign, init_gap, init_dash;
	atREAL dx, dy, theta, cosval, sinval, rdist, len;
	atPOINT pt[2];

	init_gap = init_gapdash > 0 ? init_gapdash : 0;
	init_dash = init_gapdash < 0 ? init_gapdash : 0;

	if(nGap < nWidth)
		nGap = nWidth;

	dist = init_gap + init_dash;
	
	dx = INTTOREAL(EndX - StartX);
	if(dx==0)
	{
		rdist = INTTOREAL(dist);
		len = ABS(INTTOREAL(EndY - StartY));
		if(rdist>len)
			return REALTOINT(rdist - len);

		sign = EndY - StartY > 0 ? 1 : -1;
		x1 = StartX;
		y1 = dist > 0 ? StartY + dist * sign : StartY;

		while(1)
		{
			dist += nDashLen;
			rdist = INTTOREAL(dist);
			if(rdist>=len)
			{
				x2 = EndX;
				y2 = EndY;
				pt[0].nX = x1;
				pt[0].nY = y1;
				pt[1].nX = x2;
				pt[1].nY = y2;
				_atGRPX_DrawPolyLineWidth1( lpCanvas, pt, 2, atFALSE, Color, nWidth, atGRPX_DrawPixel, bAA, atNULL, atNULL );
				init_gapdash = REALTOINT(rdist - len) - nDashLen;
				break;
			}
			else
			{
				x2 = StartX;
				y2 = StartY + dist * sign;
				pt[0].nX = x1;
				pt[0].nY = y1;
				pt[1].nX = x2;
				pt[1].nY = y2;
				_atGRPX_DrawPolyLineWidth1( lpCanvas, pt, 2, atFALSE, Color, nWidth, atGRPX_DrawPixel, bAA, atNULL, atNULL );
			}
			
			dist += nGap;
			rdist = INTTOREAL(dist);
			if(rdist>=len)
			{
				init_gapdash = REALTOINT(rdist - len);
				break;
			}
			x1 = StartX;
			y1 = StartY + dist * sign;
		}
		return init_gapdash;
	}
	dy = INTTOREAL(EndY - StartY);
	if(dy==0)
	{
		rdist = INTTOREAL(dist);
		len = ABS(INTTOREAL(EndX - StartX));
		if(rdist>len)
			return REALTOINT(rdist - len);
		
		sign = EndX - StartX > 0 ? 1 : -1;
		x1 = dist > 0 ? StartX + dist * sign : StartX;
		y1 = StartY;
		
		while(1)
		{
			dist += nDashLen;
			rdist = INTTOREAL(dist);
			if(rdist>=len)
			{
				x2 = EndX;
				y2 = EndY;
				pt[0].nX = x1;
				pt[0].nY = y1;
				pt[1].nX = x2;
				pt[1].nY = y2;
				_atGRPX_DrawPolyLineWidth1( lpCanvas, pt, 2, atFALSE, Color, nWidth, atGRPX_DrawPixel, bAA, atNULL, atNULL );
				init_gapdash = REALTOINT(rdist - len) - nDashLen;
				break;
			}
			else
			{
				x2 = StartX + dist * sign;
				y2 = StartY;
				pt[0].nX = x1;
				pt[0].nY = y1;
				pt[1].nX = x2;
				pt[1].nY = y2;
				_atGRPX_DrawPolyLineWidth1( lpCanvas, pt, 2, atFALSE, Color, nWidth, atGRPX_DrawPixel, bAA, atNULL, atNULL );
			}
			
			dist += nGap;
			rdist = INTTOREAL(dist);
			if(rdist>=len)
			{
				init_gapdash = REALTOINT(rdist - len);
				break;
			}
			x1 = StartX + dist * sign;
			y1 = StartY;
		}
		return init_gapdash;
	}
	theta = ATAN2(dx, dy);
	cosval = COS(theta);
	sinval = SIN(theta);
	rdist = INTTOREAL(dist);
	len = REALDIV(dx, cosval);
	if(rdist>len)
		return REALTOINT(rdist - len);
	
	if(dist > 0)
	{
		x1 = StartX + REALTOINT(dist * cosval);
		y1 = StartY + REALTOINT(dist * sinval);
	}
	else
	{
		x1 = StartX;
		y1 = StartY;
	}
	
	while(1)
	{
		dist += nDashLen;
		rdist = INTTOREAL(dist);
		if(rdist>=len)
		{
			x2 = EndX;
			y2 = EndY;
			pt[0].nX = x1;
			pt[0].nY = y1;
			pt[1].nX = x2;
			pt[1].nY = y2;
			_atGRPX_DrawPolyLineWidth1( lpCanvas, pt, 2, atFALSE, Color, nWidth, atGRPX_DrawPixel, bAA, atNULL, atNULL );
			init_gapdash = REALTOINT(rdist - len) - nDashLen;
			break;
		}
		else
		{
			x2 = StartX + REALTOINT(dist * cosval);
			y2 = StartY + REALTOINT(dist * sinval);
			pt[0].nX = x1;
			pt[0].nY = y1;
			pt[1].nX = x2;
			pt[1].nY = y2;
			_atGRPX_DrawPolyLineWidth1( lpCanvas, pt, 2, atFALSE, Color, nWidth, atGRPX_DrawPixel, bAA, atNULL, atNULL );
		}
		
		dist += nGap;
		rdist = INTTOREAL(dist);
		if(rdist>=len)
		{
			init_gapdash = REALTOINT(rdist - len);
			break;
		}
		x1 = StartX + REALTOINT(dist * cosval);
		y1 = StartY + REALTOINT(dist * sinval);
	}

	return init_gapdash;
}

static int _atGRPX_DrawLineWidthDot(atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth, atINT nGap, atINT nInitGap)
{
	atINT x, y, dist, radius, sign;
	atREAL dx, dy, theta, cosval, sinval, rdist, len;

	if(nWidth<=2)
		radius = nWidth;
	else
		radius = nWidth/2;
	
	if(nGap < nWidth)
		nGap = nWidth;

	dist = nInitGap;
	
	dx = INTTOREAL(EndX - StartX);
	if(dx==0)
	{
		rdist = INTTOREAL(dist);
		len = ABS(INTTOREAL(EndY - StartY));
		if(rdist>len)
			return REALTOINT(rdist - len);
		
		sign = EndY - StartY > 0 ? 1 : -1;
		x = StartX;
		y = StartY + dist * sign;
		
		while(1)
		{
			atGRPX_FillCircle( lpCanvas, x, y, radius, Color );
			
			dist += nGap;
			rdist = INTTOREAL(dist);
			if(rdist>=len)
			{
				nInitGap = REALTOINT(rdist - len);
				break;
			}
			x = StartX;
			y = StartY + dist * sign;
		}
		
		return nInitGap;
	}
	dy = INTTOREAL(EndY - StartY);
	if(dy==0)
	{
		rdist = INTTOREAL(dist);
		len = ABS(INTTOREAL(EndX - StartX));
		if(rdist>len)
			return REALTOINT(rdist - len);
		
		sign = EndX - StartX > 0 ? 1 : -1;
		x = StartX + dist * sign;
		y = StartY;
		
		while(1)
		{
			atGRPX_FillCircle( lpCanvas, x, y, radius, Color );
			
			dist += nGap;
			rdist = INTTOREAL(dist);
			if(rdist>=len)
			{
				nInitGap = REALTOINT(rdist - len);
				break;
			}
			x = StartX + dist * sign;
			y = StartY;
		}
		
		return nInitGap;
	}
	theta = ATAN2(dx, dy);
	cosval = COS(theta);
	sinval = SIN(theta);
	rdist = INTTOREAL(dist);
	len = REALDIV(dx, cosval);
	if(rdist>len)
		return REALTOINT(rdist - len);
	
	x = StartX + REALTOINT(dist * cosval);
	y = StartY + REALTOINT(dist * sinval);
	
	while(1)
	{
		atGRPX_FillCircle( lpCanvas, x, y, radius, Color );
		
		dist += nGap;
		rdist = INTTOREAL(dist);
		if(rdist>=len)
		{
			nInitGap = REALTOINT(rdist - len);
			break;
		}
		x = StartX + REALTOINT(dist * cosval);
		y = StartY + REALTOINT(dist * sinval);
	}
	
	return nInitGap;
}

static int _atGRPX_DrawLineWidthDotAA(atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth, atINT nGap, atINT nInitGap)
{
	atINT x, y, dist, radius, sign;
	atREAL dx, dy, theta, cosval, sinval, rdist, len;

	if(nWidth<=2)
		radius = nWidth;
	else
		radius = nWidth/2;
	
	if(nGap < nWidth)
		nGap = nWidth;

	dist = nInitGap;
	
	dx = INTTOREAL(EndX - StartX);
	if(dx==0)
	{
		rdist = INTTOREAL(dist);
		len = ABS(INTTOREAL(EndY - StartY));
		if(rdist>len)
			return REALTOINT(rdist - len);
		
		sign = EndY - StartY > 0 ? 1 : -1;
		x = StartX;
		y = StartY + dist * sign;
		
		while(1)
		{
			atGRPX_FillCircleAA( lpCanvas, x, y, radius, Color );
			
			dist += nGap;
			rdist = INTTOREAL(dist);
			if(rdist>=len)
			{
				nInitGap = REALTOINT(rdist - len);
				break;
			}
			x = StartX;
			y = StartY + dist * sign;
		}
		
		return nInitGap;
	}
	dy = INTTOREAL(EndY - StartY);
	if(dy==0)
	{
		rdist = INTTOREAL(dist);
		len = ABS(INTTOREAL(EndX - StartX));
		if(rdist>len)
			return REALTOINT(rdist - len);
		
		sign = EndX - StartX > 0 ? 1 : -1;
		x = StartX + dist * sign;
		y = StartY;
		
		while(1)
		{
			atGRPX_FillCircleAA( lpCanvas, x, y, radius, Color );
			
			dist += nGap;
			rdist = INTTOREAL(dist);
			if(rdist>=len)
			{
				nInitGap = REALTOINT(rdist - len);
				break;
			}
			x = StartX + dist * sign;
			y = StartY;
		}
		
		return nInitGap;
	}
	theta = ATAN2(dx, dy);
	cosval = COS(theta);
	sinval = SIN(theta);
	rdist = INTTOREAL(dist);
	len = REALDIV(dx, cosval);
	if(rdist>len)
		return REALTOINT(rdist - len);
	
	x = StartX + REALTOINT(dist * cosval);
	y = StartY + REALTOINT(dist * sinval);
	
	while(1)
	{
		atGRPX_FillCircleAA( lpCanvas, x, y, radius, Color );
		
		dist += nGap;
		rdist = INTTOREAL(dist);
		if(rdist>=len)
		{
			nInitGap = REALTOINT(rdist - len);
			break;
		}
		x = StartX + REALTOINT(dist * cosval);
		y = StartY + REALTOINT(dist * sinval);
	}
	
	return nInitGap;
}

/*******************
  다각형 라인을 그린다. Kevin
********************/
static void _atGRPX_DrawPolyLineWidthDash( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth,
										  atINT nDashLen, atINT nGap )
{
	int i, init_gapdash;

	init_gapdash = nGap / 2;
	for(i=0; i<nCount-1; ++i)
		init_gapdash = _atGRPX_DrawLineWidthDash(lpCanvas, lpPtrList[i].nX, lpPtrList[i].nY, lpPtrList[i+1].nX, lpPtrList[i+1].nY, Color, nWidth, atFALSE, nDashLen, nGap, init_gapdash);
	if(bClosed)
		_atGRPX_DrawLineWidthDash(lpCanvas, lpPtrList[nCount-1].nX, lpPtrList[nCount-1].nY, lpPtrList[0].nX, lpPtrList[0].nY, Color, nWidth, atFALSE, nDashLen, nGap, init_gapdash);
}

/*******************
  다각형 라인을 그린다. Kevin
********************/
static void _atGRPX_DrawPolyLineWidthDashAA( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth,
										  atINT nDashLen, atINT nGap )
{
	int i, init_gapdash;
	
	init_gapdash = nGap / 2;
	for(i=0; i<nCount-1; ++i)
		init_gapdash = _atGRPX_DrawLineWidthDash(lpCanvas, lpPtrList[i].nX, lpPtrList[i].nY, lpPtrList[i+1].nX, lpPtrList[i+1].nY, Color, nWidth, atTRUE, nDashLen, nGap, init_gapdash);
	if(bClosed)
		_atGRPX_DrawLineWidthDash(lpCanvas, lpPtrList[nCount-1].nX, lpPtrList[nCount-1].nY, lpPtrList[0].nX, lpPtrList[0].nY, Color, nWidth, atTRUE, nDashLen, nGap, init_gapdash);
}

/*******************
  다각형 라인을 그린다. Kevin
********************/
static void _atGRPX_DrawPolyLineWidthDot( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth, atINT nGap )
{
	int i, init_gap;

	init_gap = 0;
	for(i=0; i<nCount-1; ++i)
		init_gap = _atGRPX_DrawLineWidthDot(lpCanvas, lpPtrList[i].nX, lpPtrList[i].nY, lpPtrList[i+1].nX, lpPtrList[i+1].nY, Color, nWidth, nGap, init_gap);
	if(bClosed)
		_atGRPX_DrawLineWidthDot(lpCanvas, lpPtrList[nCount-1].nX, lpPtrList[nCount-1].nY, lpPtrList[0].nX, lpPtrList[0].nY, Color, nWidth, nGap, init_gap);
}

/*******************
  다각형 라인을 그린다. Kevin
********************/
static void _atGRPX_DrawPolyLineWidthDotAA( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth, atINT nGap )
{
	int i, init_gap;
	
	init_gap = 0;
	for(i=0; i<nCount-1; ++i)
		init_gap = _atGRPX_DrawLineWidthDotAA(lpCanvas, lpPtrList[i].nX, lpPtrList[i].nY, lpPtrList[i+1].nX, lpPtrList[i+1].nY, Color, nWidth, nGap, init_gap);
	if(bClosed)
		_atGRPX_DrawLineWidthDotAA(lpCanvas, lpPtrList[nCount-1].nX, lpPtrList[nCount-1].nY, lpPtrList[0].nX, lpPtrList[0].nY, Color, nWidth, nGap, init_gap);
}

/*******************
  다각형 라인을 그린다.
********************/
void atGRPX_DrawPolyLineWidth( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth )
{
    if( nWidth <= 0 ) return;
    if( nWidth == 1 ){
	    atGRPX_DrawPolyLine( lpCanvas, lpPtrList, nCount, bClosed, Color );
        return;
    }

//     _atGRPX_DrawPolyLineWidth0( lpCanvas, lpPtrList, nCount, bClosed, Color, nWidth, atGRPX_DrawPixel, atGRPX_DrawLineWidthExt, atFALSE, atNULL, atNULL );
    _atGRPX_DrawPolyLineWidth1( lpCanvas, lpPtrList, nCount, bClosed, Color, nWidth, atGRPX_DrawPixel, atFALSE, atNULL, atNULL );
}


/*******************
  다각형 라인을 그린다.
  (Anti-Aliasing)
********************/
void atGRPX_DrawPolyLineWidthAA( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth )
{
    if( nWidth <= 0 ) return;
    if( nWidth == 1 ){
	    atGRPX_DrawPolyLineAA( lpCanvas, lpPtrList, nCount, bClosed, Color );
        return;
    }

//     _atGRPX_DrawPolyLineWidth0( lpCanvas, lpPtrList, nCount, bClosed, Color, nWidth, atGRPX_DrawPixel, atGRPX_DrawLineWidthExtAA, atTRUE, atNULL, atNULL );
    _atGRPX_DrawPolyLineWidth1( lpCanvas, lpPtrList, nCount, bClosed, Color, nWidth, atGRPX_DrawPixel, atTRUE, atNULL, atNULL );
}


/*******************
  Aliased -------------- 그린다.
********************/
void atGRPX_DrawPolyLineWidthDash( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth, atINT nDashLen, atINT nGap )
{
    if( nWidth <= 0 ) return;
    if( nWidth == 1 ){
	    atGRPX_DrawPolyLineDash( lpCanvas, lpPtrList, nCount, bClosed, Color, nDashLen, nGap );
        return;
    }

    _atGRPX_DrawPolyLineWidthDash( lpCanvas, lpPtrList, nCount, bClosed, Color, nWidth, nDashLen, nGap );
}


/*******************
  Anti-aliased ------------- 그린다.
********************/
void atGRPX_DrawPolyLineWidthDashAA( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth, atINT nDashLen, atINT nGap )
{
    if( nWidth <= 0 ) return;
    if( nWidth == 1 ){
	    atGRPX_DrawPolyLineDashAA( lpCanvas, lpPtrList, nCount, bClosed, Color, nDashLen, nGap );
        return;
    }

    _atGRPX_DrawPolyLineWidthDashAA( lpCanvas, lpPtrList, nCount, bClosed, Color, nWidth, nDashLen, nGap );
}


/*******************
  Aliased .............. 그린다.
********************/
void atGRPX_DrawPolyLineWidthDot( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth, atINT nGap )
{
    if( nWidth <= 0 ) return;
    if( nWidth == 1 ){
		atGRPX_DrawPolyLineDot( lpCanvas, lpPtrList, nCount, bClosed, Color, nGap );
        return;
    }
	
    _atGRPX_DrawPolyLineWidthDot( lpCanvas, lpPtrList, nCount, bClosed, Color, nWidth, nGap );
}


/*******************
  Anti-aliased ............. 그린다.
********************/
void atGRPX_DrawPolyLineWidthDotAA( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth, atINT nGap )
{
    if( nWidth <= 0 ) return;
    if( nWidth == 1 ){
		atGRPX_DrawPolyLineDotAA( lpCanvas, lpPtrList, nCount, bClosed, Color, nGap );
        return;
    }
	
    _atGRPX_DrawPolyLineWidthDotAA( lpCanvas, lpPtrList, nCount, bClosed, Color, nWidth, nGap );
}


/*******************
  다각형 라인을 그린다.
********************/
void atGRPX_DrawPolyLineWidthEx( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth, atLPGRPX_FUNC_COORD_CONV_REAL lpCnvFunc, atLPVOID lpParam )
{
    if( nWidth <= 0 ) return;
    _atGRPX_DrawPolyLineWidth0( lpCanvas, lpPtrList, nCount, bClosed, Color, nWidth, atGRPX_DrawPixel, atGRPX_DrawLineWidthExt, atFALSE, lpCnvFunc, lpParam );
}


/*******************
  다각형 라인을 그린다.
  (Anti-Aliasing)
********************/
void atGRPX_DrawPolyLineWidthExAA( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth, atLPGRPX_FUNC_COORD_CONV_REAL lpCnvFunc, atLPVOID lpParam )
{
    if( nWidth <= 0 ) return;
    _atGRPX_DrawPolyLineWidth0( lpCanvas, lpPtrList, nCount, bClosed, Color, nWidth, atGRPX_DrawPixel, atGRPX_DrawLineWidthExtAA, atTRUE, lpCnvFunc, lpParam );
}


/*******************
  외각선 다각형 라인을 그린다.
********************/
void atGRPX_DrawPolyLineWidthWithEdge( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth, atINT nEdgeWidth )
{
/*
    if( nWidth <= 0 ) return;
    if( nWidth == 1 ){
	    atGRPX_DrawPolyLineAA( lpCanvas, lpPtrList, nCount, bClosed, Color );
        return;
    }

    _atGRPX_DrawPolyLine0( lpCanvas, lpPtrList, nCount, bClosed, Color, nWidth, atGRPX_DrawPixel, atGRPX_DrawLineWidthExtAA, atTRUE );
*/    
}


/*******************
  외각선 다각형 라인을 그린다.
  (Anti-Aliasing)
********************/
void atGRPX_DrawPolyLineWidthWithEdgeAA( atGRPX_CANVAS *lpCanvas, atPOINT *lpPtrList, atINT nCount, atBOOL bClosed, atGRPX_COLOR Color, atINT nWidth, atINT nEdgeWidth )
{
/*
    if( nWidth <= 0 ) return;
    if( nWidth == 1 ){
	    atGRPX_DrawPolyLineAA( lpCanvas, lpPtrList, nCount, bClosed, Color );
        return;
    }

    _atGRPX_DrawPolyLine0( lpCanvas, lpPtrList, nCount, bClosed, Color, nWidth, atGRPX_DrawPixel, atGRPX_DrawLineWidthExtAA, atTRUE );
*/    
}

