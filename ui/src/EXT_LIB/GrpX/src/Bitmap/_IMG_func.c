/*#############################################################################
-------------------------------------------------------------------------------

                                  IMG_R.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Put image function definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "GrpX.h"
#include "A_Math.h"
#include "_IMG_func.h"


/*
static void _DrawBitmapLine2( atGRPX_PIXEL *ptr, int width, atGRPX_BITMAP *lpBitmap, atREAL x, atREAL y, atREAL dx, atREAL dy )
{
    int i;
    int xx, yy;
    atGRPX_PIXEL *img_ptr = (atGRPX_PIXEL *)lpBitmap->lpBits;
    int bmWidth = lpBitmap->nWidth;
//    int bmHeight = lpBitmap->nHeight;

    for( i = 0; i < width; i++ ){
      xx = REALTOINT(x);
      yy = REALTOINT(y);
//	  if( (x >= 0) && (x < bmWidth) && (y >= 0) && (y < bmHeight) ){ // 불안해서 넣을까 말까 고민 중.
	    *ptr = *(img_ptr + yy*bmWidth + xx);
//    }
      ptr++;
      x += dx;
      y += dy;
   }
}
*/

static void _DrawBitmapLine( atGRPX_CANVAS *lpCanvas, atINT X, atINT Y, int width, atGRPX_BITMAP *lpBitmap, int ix1, int iy1, int ix2, int iy2,
                        atLONG ExParam1,
						__IMAGECALLBACK__DRAW_PIXEL *fnDrawPixel,
						__IMAGECALLBACK__DRAW_IMAGE_LINE *fnDrawBitmapLine )
{
   int t;
   atREAL dx, dy;
   atREAL xx, yy;
 //  int bmWidth = lpBitmap->nWidth;
//   int bmHeight = lpBitmap->nHeight;
   atGRPX_PIXEL *tgt_ptr = atGRPX_GetFrameBufferPtrXY( lpCanvas, X, Y );
   atRECT *clip;

   if( width == 0 ) return;

   clip = atGRPX_GetClipRect( lpCanvas );

//   if( (Y < clip->nStartY) || (Y > clip->nEndY) ) return; // 이미 Y클리핑 처리 되어 옴
   if( (X > clip->nEndX) || ((X + width) <= clip->nStartX ) ) return;

   if( (ix1 == ix2) && (iy1 == iy2) ){
     fnDrawPixel( tgt_ptr, lpBitmap, ix1, iy1, ExParam1 );
     return;
   }

   dx = INTTOREAL(ix2 - ix1) / width;
   dy = INTTOREAL(iy2 - iy1) / width;
   xx = INTTOREAL(ix1);
   yy = INTTOREAL(iy1);

   if( (X+width) >= clip->nEndX ){
     width -= (X+width) - clip->nEndX - 1;
   }
   if( X < clip->nStartX ){
     t = clip->nStartX - X;
     xx += dx * t;
     yy += dy * t;
     tgt_ptr += t;
     width -= t;
   }

   fnDrawBitmapLine( tgt_ptr, lpBitmap, width, xx, yy, dx, dy, ExParam1 );
}


static void _Line_Copy( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, int width, int type1, int type2, atREAL v1, atREAL v2,
                        atLONG ExParam1,
						__IMAGECALLBACK__DRAW_PIXEL *fnDrawPixel,
						__IMAGECALLBACK__DRAW_IMAGE_LINE *fnDrawBitmapLine )
{
	int bmWidth = lpBitmap->nWidth;
	int bmHeight = lpBitmap->nHeight;
	int x1, y1, x2, y2;

	switch( type1 ){
		case 0 :
			switch( type2 ){
				case 0 : x1 = 0; y1 = REALTOINT(v1*bmHeight); x2 = REALTOINT(v2*bmWidth); y2 = 0; break;
				case 1 : x1 = 0; y1 = REALTOINT(v1*bmHeight); x2 = bmWidth-1; y2 = REALTOINT(v2*bmHeight); break;
				case 2 : x1 = REALTOINT(v1*bmWidth); y1 = bmHeight-1; x2 = REALTOINT(v2*bmWidth); y2 = 0; break;
				case 3 : x1 = REALTOINT(v1*bmWidth); y1 = bmHeight-1; x2 = bmWidth-1; y2 = REALTOINT(v2*bmHeight); break;
			}
			break;
		case 1 :
			switch( type2 ){
				case 0 : x1 = bmWidth-1-REALTOINT(v1*bmWidth); y1 = 0; x2 = bmWidth-1; y2 = REALTOINT(v2*bmHeight); break;
				case 1 : x1 = bmWidth-1-REALTOINT(v1*bmWidth); y1 = 0; x2 = bmWidth-1-REALTOINT(v2*bmWidth); y2 = bmHeight-1; break;
				case 2 : x1 = 0; y1 = REALTOINT(v1*bmHeight); x2 = bmWidth-1; y2 = REALTOINT(v2*bmHeight); break;
				case 3 : x1 = 0; y1 = REALTOINT(v1*bmHeight); x2 = bmWidth-1-REALTOINT(v2*bmWidth); y2 = bmHeight-1; break;
			}
			break;
		case 2 :
			switch( type2 ){
				case 0 : x1 = REALTOINT(v1*bmWidth); y1 = bmHeight-1; x2 = 0; y2 = bmHeight-1-REALTOINT(v2*bmHeight); break;
				case 1 : x1 = REALTOINT(v1*bmWidth); y1 = bmHeight-1; x2 = REALTOINT(v2*bmWidth); y2 = 0; break;
				case 2 : x1 = bmWidth-1; y1 = bmHeight-1-REALTOINT(v1*bmHeight); x2 = 0; y2 = bmHeight-1-REALTOINT(v2*bmHeight); break;
				case 3 : x1 = bmWidth-1; y1 = bmHeight-1-REALTOINT(v1*bmHeight); x2 = REALTOINT(v2*bmWidth); y2 = 0; break;
			}
			break;
		case 3 :
			switch( type2 ){
				case 0 : x1 = bmWidth-1; y1 = bmHeight-1-REALTOINT(v1*bmHeight); x2 = bmWidth-1-REALTOINT(v2*bmWidth); y2 = bmHeight-1; break;
				case 1 : x1 = bmWidth-1; y1 = bmHeight-1-REALTOINT(v1*bmHeight); x2 = 0; y2 = bmHeight-1-REALTOINT(v2*bmHeight); break;
				case 2 : x1 = bmWidth-1-REALTOINT(v1*bmWidth); y1 = 0; x2 = bmWidth-1-REALTOINT(v2*bmWidth); y2 = bmHeight-1; break;
				case 3 : x1 = bmWidth-1-REALTOINT(v1*bmWidth); y1 = 0; x2 = 0; y2 = bmHeight-1-REALTOINT(v2*bmHeight); break;
			}
			break;
	}
	
	_DrawBitmapLine( lpCanvas, X, Y, width, lpBitmap, x1, y1, x2, y2, ExParam1, fnDrawPixel, fnDrawBitmapLine );
}


/*****************************
  이미지를 회전하여 출력한다.
******************************/
void __atGRPX_DrawBitmapRotate0( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, atREAL Angle,
                        atLONG ExParam1,
						__IMAGECALLBACK__DRAW_PIXEL *fnDrawPixel,
						__IMAGECALLBACK__DRAW_IMAGE_LINE *fnDrawBitmapLine )
{
	int i, j;
    atPOINT p[4], *tp[4];
    short HalfH,HalfW;
	int idx[4][2];
	int t;
	int pt1_y, pt2_y, pt3_y, pt4_y;
    int sy, ey;
	int type1, type2;
    atRECT *clip;
    int cen_x, cen_y;

    int ldx, ldy, rdx, rdy;
    atREAL ldxdy, rdxdy;
    atREAL lx, rx;

    int ly, ry;
    int lty, rty;
	atREAL lv, rv;
	atREAL ldv, rdv;

    if( (lpBitmap == atNULL) ) return;
	
    // 회전시킨 좌표를 얻는다.
    cen_x = -lpBitmap->nLogX;
    cen_y = -lpBitmap->nLogY;
	HalfW = -cen_x;
	HalfH = -cen_y;
	p[0].nX = X + REALTOINT((HalfW*COS(Angle))-(HalfH*SIN(Angle)));
	p[0].nY = Y + REALTOINT((HalfW*SIN(Angle))+(HalfH*COS(Angle)));
	HalfW = lpBitmap->nWidth - cen_x;
	HalfH = -cen_y;
	p[1].nX = X + REALTOINT((HalfW*COS(Angle))-(HalfH*SIN(Angle)));
	p[1].nY = Y + REALTOINT((HalfW*SIN(Angle))+(HalfH*COS(Angle)));
	HalfW = -cen_x;
	HalfH = lpBitmap->nHeight - cen_y;
	p[2].nX = X + REALTOINT((HalfW*COS(Angle))-(HalfH*SIN(Angle)));
	p[2].nY = Y + REALTOINT((HalfW*SIN(Angle))+(HalfH*COS(Angle)));
	HalfW = lpBitmap->nWidth - cen_x;
	HalfH = lpBitmap->nHeight - cen_y;
	p[3].nX = X + REALTOINT((HalfW*COS(Angle))-(HalfH*SIN(Angle)));
	p[3].nY = Y + REALTOINT((HalfW*SIN(Angle))+(HalfH*COS(Angle)));

	// 각 점을 Y좌표로 소트한다.
	for( i = 0; i < 4; i++ ){
	  idx[i][0] = i;
	  idx[i][1] = p[i].nY;
	}
	for( i = 0; i < 4-1; i++ ){
	  for( j = i+1; j < 4; j++ ){
		if( idx[i][1] > idx[j][1] ){
		  SWAP( idx[i][0], idx[j][0], t );
		  SWAP( idx[i][1], idx[j][1], t );
		}
	  }
	}

	// 꼭지점 Y 좌표를 중심으로 점의 위치를 재편성한다.
	/*
      P1(X1,Y1)    P2(X2,Y2)
          #            #
          #            #
      P3(X3,Y3)    P4(X4,Y4)
	*/
	type1 = idx[0][0];
	if( type1 == 0 ){
		tp[0] = &p[0]; tp[1] = &p[1]; tp[2] = &p[2]; tp[3] = &p[3];
	} else if( type1 == 1 ){
		tp[0] = &p[1]; tp[1] = &p[3]; tp[2] = &p[0]; tp[3] = &p[2];
	} else if( type1 == 2 ){
		tp[0] = &p[2]; tp[1] = &p[0]; tp[2] = &p[3]; tp[3] = &p[1];
	} else if( type1 == 3 ){
		tp[0] = &p[3]; tp[1] = &p[2]; tp[2] = &p[1]; tp[3] = &p[0];
	}

    clip = atGRPX_GetClipRect( lpCanvas );

    // 완전히 벗어나면 종료
    if( (tp[3]->nY < clip->nStartY) || (tp[0]->nY > clip->nEndY) ) return;
    if( (tp[1]->nX < clip->nStartX) || (tp[2]->nX > clip->nEndX) ) return;

	// 스캔라인 Y 기준점
	pt1_y = tp[0]->nY;
	pt2_y = MIN( tp[1]->nY, tp[2]->nY );
	pt3_y = MAX( tp[1]->nY, tp[2]->nY );
	pt4_y = tp[3]->nY;

	// 꼭지점에서 중간 첫번째 지점까지 그린다.
	if( pt1_y != pt2_y ){
		lty = tp[2]->nY - pt1_y;
		rty = tp[1]->nY - pt1_y;
		ly = ry = 0;

		rdx = tp[1]->nX - tp[0]->nX;
		rdy = tp[1]->nY - tp[0]->nY;
		ldx = tp[2]->nX - tp[0]->nX;
		ldy = tp[2]->nY - tp[0]->nY;

		lx = rx = INTTOREAL(tp[0]->nX);

		if( ldy == 0 || rdy == 0 ){

		} else {
          ldxdy = INTTOREAL(ldx) / ldy;
          rdxdy = INTTOREAL(rdx) / rdy;

          // Y 클리핑 처리
          sy = pt1_y; ey = pt2_y - 1;
          if( pt2_y > clip->nEndY ){
            ey = clip->nEndY;
          }
          if( pt1_y < clip->nStartY ){
            t = MIN( clip->nStartY, pt2_y );
            sy = t;
            t -= pt1_y;
		    lx += ldxdy * t; rx += rdxdy * t;
            ly += t; ry += t;
          }

		  ldv = REAL_ONE / lty;
		  rdv = REAL_ONE / rty;
		  lv = ldv * ly;
		  rv = rdv * ry;

		  for( i = sy; i <= ey; i++ ){
		    _Line_Copy( lpCanvas, lpBitmap, REALTOINT(lx), i, REALTOINT(rx - lx) + 1,
                                           type1, 0, lv, rv,
										   ExParam1,
                                           fnDrawPixel, fnDrawBitmapLine );
		    lx += ldxdy; rx += rdxdy;
            ly ++; ry ++;
			lv += ldv; rv += rdv;
          }
		}
    } else {
		lty = tp[2]->nY - pt1_y;
		rty = tp[3]->nY - pt1_y;
		ly = ry = 0;
		lx = INTTOREAL(tp[0]->nX);
        rx = INTTOREAL(tp[1]->nX);
		if( tp[1]->nY < tp[2]->nY ){
			rdx = 0;
			rdy = tp[3]->nY - tp[1]->nY;
			ldx = 0;
			ldy = tp[2]->nY - tp[0]->nY;
        } else {
			rdx = 0;
			rdy = tp[1]->nY - tp[0]->nY;
			ldx = 0;
			ldy = tp[3]->nY - tp[2]->nY;
        }
    }

	// 중간 첫번째 지점부터 중간 두번째 지점까지 그린다.
	if( pt2_y != pt3_y ){
		if( tp[1]->nY < tp[2]->nY ){
			rty = pt4_y - pt2_y;
			ry = 0;

			rdx = tp[3]->nX - tp[1]->nX;
			rdy = tp[3]->nY - tp[1]->nY;
			rx = INTTOREAL(tp[1]->nX);
			type2 = 1;
		} else {
			lty = pt4_y - pt2_y;
			ly = 0;

			ldx = tp[3]->nX - tp[2]->nX;
			ldy = tp[3]->nY - tp[2]->nY;
			lx = INTTOREAL(tp[2]->nX);
			type2 = 2;
		}

		if( ldy == 0 || rdy == 0 ){

		} else {
          ldxdy = INTTOREAL(ldx) / ldy;
          rdxdy = INTTOREAL(rdx) / rdy;

          // Y 클리핑 처리
          sy = pt2_y; ey = pt3_y - 1;
          if( pt3_y == pt4_y ) ey++; // 만약 끝라인이 같으면 중간에 끝라인까지 처리 
          if( pt3_y > clip->nEndY ){
            ey = clip->nEndY;
          }
          if( pt2_y < clip->nStartY ){
            t = MIN( clip->nStartY, pt3_y );
            sy = t;
            t -= pt2_y;
		    lx += ldxdy * t; rx += rdxdy * t;
            ly += t; ry += t;
          }

		  ldv = REAL_ONE / lty;
		  rdv = REAL_ONE / rty;
		  lv = ldv * ly;
		  rv = rdv * ry;

		  for( i = sy; i <= ey; i++ ){
			_Line_Copy( lpCanvas, lpBitmap, REALTOINT(lx), i, REALTOINT(rx - lx) + 1,
                                           type1, type2, lv, rv,
                                           ExParam1,
                                           fnDrawPixel, fnDrawBitmapLine );
			lx += ldxdy; rx += rdxdy;
            ly ++; ry ++;
			lv += ldv; rv += rdv;
		  }
		}
	}

	// 중간 두번째 지점부터 마지막 꼭지점까지 그린다.
	if( pt3_y != pt4_y ){
		if( tp[2]->nY > tp[1]->nY ){
			lty = pt4_y - pt3_y;
			ly = 0;

			ldx = tp[3]->nX - tp[2]->nX;
			ldy = tp[3]->nY - tp[2]->nY;
			lx = INTTOREAL(tp[2]->nX);
		} else {
			rty = pt4_y - pt3_y;
			ry = 0;

			rdx = tp[3]->nX - tp[1]->nX;
			rdy = tp[3]->nY - tp[1]->nY;
			rx = INTTOREAL(tp[1]->nX);
		}

		if( ldy == 0 || rdy == 0 ){

		} else {
          ldxdy = INTTOREAL(ldx) / ldy;
          rdxdy = INTTOREAL(rdx) / rdy;

          // Y 클리핑 처리
          sy = pt3_y; ey = pt4_y;
          if( pt4_y > clip->nEndY ){
            ey = clip->nEndY;
          }
          if( pt3_y < clip->nStartY ){
            t = MIN( clip->nStartY, pt4_y );
            sy = t;
            t -= pt2_y;
		    lx += ldxdy * t; rx += rdxdy * t;
            ly += t; ry += t;
          }

		  ldv = REAL_ONE / lty;
		  rdv = REAL_ONE / rty;
		  lv = ldv * ly;
		  rv = rdv * ry;

		  for( i = sy; i <= ey; i++ ){
			_Line_Copy( lpCanvas, lpBitmap, REALTOINT(lx), i, REALTOINT(rx - lx) + 1,
                                           type1, 3, lv, rv,
                                           ExParam1,
                                           fnDrawPixel, fnDrawBitmapLine );
			lx += ldxdy; rx += rdxdy;
            ly++; ry++;
			lv += ldv; rv += rdv;
		  }
		}
	}
}



/********************
  이미지를 출력한다.
*********************/
void __atGRPX_DrawBitmapPart0( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y,
                           atINT ImageX, atINT ImageY, atINT ImageWidth, atINT ImageHeight,
                           atLONG ExParam1,
						   __IMAGECALLBACK__DRAW_IMAGE_PART *fnDrawBitmap )
{
    int t;
    atRECT *clip;

    if( (ImageWidth == 0) || (ImageHeight == 0) ) return;
    if( (ImageX >= lpBitmap->nWidth) || (ImageY >= lpBitmap->nHeight) ) return;
    if( (ImageX + ImageWidth > lpBitmap->nWidth) ){
      ImageWidth = lpBitmap->nWidth - ImageX;
    }
    if( (ImageY + ImageHeight > lpBitmap->nHeight) ){
      ImageHeight = lpBitmap->nHeight - ImageY;
    }
    if( (ImageWidth == 0) || (ImageHeight == 0) ) return;
    if( ImageX < 0 ){
      ImageWidth += ImageX;
      ImageX = 0;
    }
    if( ImageY < 0 ){
      ImageHeight += ImageY;
      ImageY = 0;
    }

    X += lpBitmap->nLogX;
    Y += lpBitmap->nLogY;

    clip = atGRPX_GetClipRect(lpCanvas);

    if( Y > clip->nEndY ) return;
    if( Y < clip->nStartY ){
      ImageHeight -= clip->nStartY - Y;
      ImageY += clip->nStartY - Y;
      Y = clip->nStartY;
    }

    t = Y + ImageHeight - 1;
    if( t < clip->nStartY ) return;
    if( t > clip->nEndY ){
      ImageHeight -= t - clip->nEndY;
    }

    if( X > clip->nEndX ) return;
    if( X < clip->nStartX ){
      ImageWidth -= clip->nStartX - X;
      ImageX += clip->nStartX - X;
      X = clip->nStartX;
    }
    t = X + ImageWidth - 1;
    if( t < clip->nStartX ) return;
    if( t > clip->nEndX ){
      ImageWidth -= t - clip->nEndX;
    }

    fnDrawBitmap( lpCanvas, lpBitmap, X, Y, ImageX, ImageY, ImageWidth, ImageHeight, ExParam1 );
}


/***********************

************************/
void __atGRPX_DrawBitmapStretch0( atGRPX_CANVAS *lpCanvas, atGRPX_BITMAP *lpBitmap, atINT X, atINT Y, 
								  atINT tWidth, atINT tHeight,
								  atLONG ExParam1,
							      __IMAGECALLBACK__DRAW_IMAGE_STRETCH *fnDrawBitmap )
{
    atRECT *clip;
    int width, height, t;
    atREAL dx, dy;
	atINT ImgX, ImgY;

    width = lpBitmap->nWidth;
    height = lpBitmap->nHeight;

    if( width == 0 || height == 0 || tWidth == 0 || tHeight == 0 ) return;

    dx = INTTOREAL(width)  / tWidth;
    dy = INTTOREAL(height) / tHeight;

    X += lpBitmap->nLogX * tWidth  / width;
    Y += lpBitmap->nLogY * tHeight / height;

	ImgX = ImgY = 0;

    clip = atGRPX_GetClipRect(lpCanvas);

    if( Y > clip->nEndY ) return;
    if( Y < clip->nStartY ){
      tHeight -= clip->nStartY - Y;
//      ImgPtr += REALTOINT( (clip->nStartY - Y) * dy ) * width;
	  ImgY += REALTOINT( (clip->nStartY - Y) * dy );
      Y = clip->nStartY;
    }
    t = Y + tHeight - 1;
    if( t < clip->nStartY ) return;
    if( t > clip->nEndY ){
      tHeight -= t - clip->nEndY;
    }

    if( X > clip->nEndX ) return;
    if( X < clip->nStartX ){
      tWidth -= clip->nStartX - X;
//      ImgPtr += REALTOINT( (clip->nStartX - X) * dx );
	  ImgX += REALTOINT( (clip->nStartX - X) * dx );
      X = clip->nStartX;
    }
    t = X + tWidth - 1;
    if( t < clip->nStartX ) return;
    if( t > clip->nEndX ){
      tWidth -= t - clip->nEndX;
    }

    fnDrawBitmap( lpCanvas, lpBitmap, X, Y, tWidth, tHeight, ImgX, ImgY, dx, dy, ExParam1 );
}

