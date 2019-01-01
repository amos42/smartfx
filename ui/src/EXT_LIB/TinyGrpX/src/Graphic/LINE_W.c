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
#include "_LIN_int.h"
#include "_LIN_func.h"
#include "_OL_func.h"
#include "A_Math.h"


/*******************************
  양 끝 점을 잇는 선을 그린다.
  (선 두께 있는 버전)
********************************/
void atGRPX_DrawLineWidthExt( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth, atINT ExtType )
{
	atREAL iL, iT, iR, iB;
	atRPOINT kPtOut[5];
	atINT radius;

	if( nWidth <= 0 ) return;
	if( nWidth == 1 ){
	  atGRPX_DrawLine( lpCanvas, StartX, StartY, EndX, EndY, Color );
	  return;
	}

	if(nWidth%2==1)
		radius = nWidth / 2 + 1;
	else
		radius = nWidth / 2;

	if( (StartX) == EndX && (StartY == EndY) ){
//	  int hw1 = -(nWidth>>1), hw2 = +(nWidth>>1);
//	  if( !(nWidth & 0x1) ) hw1++;
//	  atGRPX_FillRect( lpCanvas, StartX-hw1, StartY-hw1, StartX+hw2, StartY+hw2, Color );
	  atGRPX_FillCircle( lpCanvas, StartX, StartY, radius, Color );
	  return;
	}

	iL = INTTOREAL(StartX);
	iT = INTTOREAL(StartY);
	iR = INTTOREAL(EndX);
	iB = INTTOREAL(EndY);

	if( nWidth > 1 ){
		__atGRPX_GetOutlinePtrs_REAL( iL, iT, iR, iB, INTTOREAL(nWidth), kPtOut,
                                   ExtType & atGRPX_LINEEXT_START_EXTEND,
                                   ExtType & atGRPX_LINEEXT_END_EXTEND );
/*		
		atGRPX_FillQuadrangle( lpCanvas, REALTOINT(kPtOut[0].nX), REALTOINT(kPtOut[0].nY),
									   REALTOINT(kPtOut[1].nX), REALTOINT(kPtOut[1].nY),
									   REALTOINT(kPtOut[2].nX), REALTOINT(kPtOut[2].nY),
									   REALTOINT(kPtOut[3].nX), REALTOINT(kPtOut[3].nY), Color );
*/									   
		atGRPX_FillQuadrangle_REAL( lpCanvas, kPtOut[0].rX, kPtOut[0].rY,
									   kPtOut[1].rX, kPtOut[1].rY,
									   kPtOut[2].rX, kPtOut[2].rY,
									   kPtOut[3].rX, kPtOut[3].rY, Color );
              if( ExtType & atGRPX_LINEEXT_START_ROUNDCAP )
    			atGRPX_FillCircle( lpCanvas, StartX, StartY, radius, Color );
              if( ExtType & atGRPX_LINEEXT_END_ROUNDCAP )
    			atGRPX_FillCircle( lpCanvas, EndX, EndY, radius, Color );
	}
}


/*******************************
  양 끝 점을 잇는 선을 그린다.
  (선 두께 있는 버전)
  (AntiAliasing 적용 버전)
********************************/
void atGRPX_DrawLineWidthExtAA( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth, atINT ExtType )
{
	atREAL iL, iT, iR, iB;
	atRPOINT kPtOut[5];
	atINT radius;

	if( nWidth <= 0 ) return;
	if( nWidth == 1 ){
	  atGRPX_DrawLineAA( lpCanvas, StartX, StartY, EndX, EndY, Color );
	  return;
	}
	
	if(nWidth%2==1)
		radius = nWidth / 2 + 1;
	else
		radius = nWidth / 2;

	if( (StartX) == EndX && (StartY == EndY) ){
//	  int hw1 = -(nWidth>>1), hw2 = +(nWidth>>1);
//	  if( !(nWidth & 0x1) ) hw1++;
//	  atGRPX_FillRect( lpCanvas, StartX-hw1, StartY-hw1, StartX+hw2, StartY+hw2, Color );
//	  atGRPX_FillCircleAA( lpCanvas, StartX, StartY, nWidth>>1, Color );
	  atGRPX_FillCircleAA( lpCanvas, StartX, StartY, radius, Color );
	  return;
	}

	iL = INTTOREAL(StartX);
	iT = INTTOREAL(StartY);
	iR = INTTOREAL(EndX);
	iB = INTTOREAL(EndY);

	if( nWidth > 1 ){
		__atGRPX_GetOutlinePtrs_REAL( iL, iT, iR, iB, INTTOREAL(nWidth), kPtOut,
                                   ExtType & atGRPX_LINEEXT_START_EXTEND,
                                   ExtType & atGRPX_LINEEXT_END_EXTEND );
/*
		atGRPX_FillQuadrangleAA( lpCanvas, REALTOINT(kPtOut[0].rX), REALTOINT(kPtOut[0].rY),
									   REALTOINT(kPtOut[1].rX), REALTOINT(kPtOut[1].rY),
									   REALTOINT(kPtOut[2].rX), REALTOINT(kPtOut[2].rY),
									   REALTOINT(kPtOut[3].rX), REALTOINT(kPtOut[3].rY), Color );
*/									   
		atGRPX_FillQuadrangleAA_REAL( lpCanvas, kPtOut[0].rX, kPtOut[0].rY,
									   kPtOut[1].rX, kPtOut[1].rY,
									   kPtOut[2].rX, kPtOut[2].rY,
									   kPtOut[3].rX, kPtOut[3].rY, Color );
              if( ExtType & atGRPX_LINEEXT_START_ROUNDCAP )
    			atGRPX_FillCircleAA( lpCanvas, StartX, StartY, radius, Color );
              if( ExtType & atGRPX_LINEEXT_END_ROUNDCAP )
    			atGRPX_FillCircleAA( lpCanvas, EndX, EndY, radius, Color );
	}
}


/*******************************
  양 끝 점을 잇는 선을 그린다.
  (선 두께 있는 버전)
********************************/
void atGRPX_DrawLineWidth( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth )
{
	atGRPX_DrawLineWidthExt( lpCanvas, StartX, StartY, EndX, EndY, Color, nWidth, atGRPX_LINEEXT_NONE );
}


/*******************************
  양 끝 점을 잇는 선을 그린다.
  (선 두께 있는 버전)
  (AntiAliasing 적용 버전)
********************************/
void atGRPX_DrawLineWidthAA( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth )
{
	atGRPX_DrawLineWidthExtAA( lpCanvas, StartX, StartY, EndX, EndY, Color, nWidth, atGRPX_LINEEXT_NONE );
}


/*******************************
  ------------------- 그린다.
  (선 두께 있는 버전)
********************************/
void atGRPX_DrawLineWidthDash( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth, atINT nDashLen, atINT nGap )
{
	atINT x1, y1, x2, y2, dist, sign;
	atREAL dx, dy, theta, cosval, sinval, len;

	if(nGap < nWidth)
		nGap = nWidth;
	
	dx = INTTOREAL(EndX - StartX);
	if(dx==0)
	{
		dist = 0;
		x1 = StartX;
		y1 = StartY;

		if(EndY - StartY > 0)
			sign = 1;
		else
			sign = -1;
		len = ABS(INTTOREAL(EndY - StartY));
		
		while(1)
		{
			dist += nDashLen;
			if(INTTOREAL(dist)>=len)
			{
				x2 = EndX;
				y2 = EndY;
				atGRPX_DrawLineWidthExt( lpCanvas, x1, y1, x2, y2, Color, nWidth, atGRPX_LINEEXT_ROUNDCAP );
				break;
			}
			else
			{
				x2 = StartX;
				y2 = StartY + dist * sign;
				atGRPX_DrawLineWidthExt( lpCanvas, x1, y1, x2, y2, Color, nWidth, atGRPX_LINEEXT_ROUNDCAP );
			}
			
			dist += nGap;
			if(INTTOREAL(dist)>=len) break;
			x1 = StartX;
			y1 = StartY + dist * sign;
		}
		return;
	}
	dy = INTTOREAL(EndY - StartY);
	if(dy==0)
	{
		dist = 0;
		x1 = StartX;
		y1 = StartY;
		
		if(EndX - StartX > 0)
			sign = 1;
		else
			sign = -1;
		len = ABS(INTTOREAL(EndX - StartX));
		
		while(1)
		{
			dist += nDashLen;
			if(INTTOREAL(dist)>=len)
			{
				x2 = EndX;
				y2 = EndY;
				atGRPX_DrawLineWidthExt( lpCanvas, x1, y1, x2, y2, Color, nWidth, atGRPX_LINEEXT_ROUNDCAP );
				break;
			}
			else
			{
				x2 = StartX + dist * sign;
				y2 = StartY;
				atGRPX_DrawLineWidthExt( lpCanvas, x1, y1, x2, y2, Color, nWidth, atGRPX_LINEEXT_ROUNDCAP );
			}
			
			dist += nGap;
			if(INTTOREAL(dist)>=len) break;
			x1 = StartX + dist * sign;
			y1 = StartY;
		}
		return;
	}
	theta = ATAN2(dx, dy);
	cosval = COS(theta);
	sinval = SIN(theta);
	len = REALDIV(dx, cosval);
	
	dist = 0;
	x1 = StartX;
	y1 = StartY;
	
	while(1)
	{
		dist += nDashLen;
		if(INTTOREAL(dist)>=len)
		{
			x2 = EndX;
			y2 = EndY;
			atGRPX_DrawLineWidthExt( lpCanvas, x1, y1, x2, y2, Color, nWidth, atGRPX_LINEEXT_ROUNDCAP );
			break;
		}
		else
		{
			x2 = StartX + REALTOINT(dist * cosval);
			y2 = StartY + REALTOINT(dist * sinval);
			atGRPX_DrawLineWidthExt( lpCanvas, x1, y1, x2, y2, Color, nWidth, atGRPX_LINEEXT_ROUNDCAP );
		}
		
		dist += nGap;
		if(INTTOREAL(dist)>=len) break;
		x1 = StartX + REALTOINT(dist * cosval);
		y1 = StartY + REALTOINT(dist * sinval);
	}
}


/*******************************
  ------------------- 그린다.
  (선 두께 있는 버전)
  (AntiAliasing 적용 버전)
********************************/
void atGRPX_DrawLineWidthDashAA( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth, atINT nDashLen, atINT nGap )
{
	atINT x1, y1, x2, y2, dist, sign;
	atREAL dx, dy, theta, cosval, sinval, len;
	
	if(nGap < nWidth)
		nGap = nWidth;
	
	dx = INTTOREAL(EndX - StartX);
	if(dx==0)
	{
		dist = 0;
		x1 = StartX;
		y1 = StartY;

		if(EndY - StartY > 0)
			sign = 1;
		else
			sign = -1;
		len = ABS(INTTOREAL(EndY - StartY));
		
		while(1)
		{
			dist += nDashLen;
			if(INTTOREAL(dist)>=len)
			{
				x2 = EndX;
				y2 = EndY;
				atGRPX_DrawLineWidthExtAA( lpCanvas, x1, y1, x2, y2, Color, nWidth, atGRPX_LINEEXT_ROUNDCAP );
				break;
			}
			else
			{
				x2 = StartX;
				y2 = StartY + dist * sign;
				atGRPX_DrawLineWidthExtAA( lpCanvas, x1, y1, x2, y2, Color, nWidth, atGRPX_LINEEXT_ROUNDCAP );
			}
			
			dist += nGap;
			if(INTTOREAL(dist)>=len) break;
			x1 = StartX;
			y1 = StartY + dist * sign;
		}
		return;
	}
	dy = INTTOREAL(EndY - StartY);
	if(dy==0)
	{
		dist = 0;
		x1 = StartX;
		y1 = StartY;
		
		if(EndX - StartX > 0)
			sign = 1;
		else
			sign = -1;
		len = ABS(INTTOREAL(EndX - StartX));
		
		while(1)
		{
			dist += nDashLen;
			if(INTTOREAL(dist)>=len)
			{
				x2 = EndX;
				y2 = EndY;
				atGRPX_DrawLineWidthExtAA( lpCanvas, x1, y1, x2, y2, Color, nWidth, atGRPX_LINEEXT_ROUNDCAP );
				break;
			}
			else
			{
				x2 = StartX + dist * sign;
				y2 = StartY;
				atGRPX_DrawLineWidthExtAA( lpCanvas, x1, y1, x2, y2, Color, nWidth, atGRPX_LINEEXT_ROUNDCAP );
			}
			
			dist += nGap;
			if(INTTOREAL(dist)>=len) break;
			x1 = StartX + dist * sign;
			y1 = StartY;
		}
		return;
	}
	theta = ATAN2(dx, dy);
	cosval = COS(theta);
	sinval = SIN(theta);
	len = REALDIV(dx, cosval);
	
	dist = 0;
	x1 = StartX;
	y1 = StartY;
	
	while(1)
	{
		dist += nDashLen;
		if(INTTOREAL(dist)>=len)
		{
			x2 = EndX;
			y2 = EndY;
			atGRPX_DrawLineWidthExtAA( lpCanvas, x1, y1, x2, y2, Color, nWidth, atGRPX_LINEEXT_ROUNDCAP );
			break;
		}
		else
		{
			x2 = StartX + REALTOINT(dist * cosval);
			y2 = StartY + REALTOINT(dist * sinval);
			atGRPX_DrawLineWidthExtAA( lpCanvas, x1, y1, x2, y2, Color, nWidth, atGRPX_LINEEXT_ROUNDCAP );
		}
		
		dist += nGap;
		if(INTTOREAL(dist)>=len) break;
		x1 = StartX + REALTOINT(dist * cosval);
		y1 = StartY + REALTOINT(dist * sinval);
	}
}


/*******************************
  ................... 그린다.
  (선 두께 있는 버전)
********************************/
void atGRPX_DrawLineWidthDot( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth, atINT nGap )
{
	atINT x, y, dist, radius, sign;
	atREAL dx, dy, theta, cosval, sinval, len;
	
	if(nWidth<=2)
		radius = nWidth;
	else
		radius = nWidth/2;
	
	if(nGap < nWidth)
		nGap = nWidth;
	
	dx = INTTOREAL(EndX - StartX);
	if(dx==0)
	{
		dist = 0;
		x = StartX;
		y = StartY;
		
		sign = EndY - StartY > 0 ? 1 : -1;
		len = ABS(INTTOREAL(EndY - StartY));
		
		while(1)
		{
			atGRPX_FillCircle( lpCanvas, x, y, radius, Color );
			
			dist += nGap;
			if(INTTOREAL(dist)>=len) return;
			x = StartX;
			y = StartY + dist * sign;
		}
	}
	dy = INTTOREAL(EndY - StartY);
	if(dy==0)
	{
		dist = 0;
		x = StartX;
		y = StartY;
		
		sign = EndX - StartX > 0 ? 1 : -1;
		len = ABS(INTTOREAL(EndX - StartX));
		
		while(1)
		{
			atGRPX_FillCircle( lpCanvas, x, y, radius, Color );
			
			dist += nGap;
			if(INTTOREAL(dist)>=len) return;
			x = StartX + dist * sign;
			y = StartY;
		}
	}
	theta = ATAN2(dx, dy);
	cosval = COS(theta);
	sinval = SIN(theta);
	len = REALDIV(dx, cosval);
	
	dist = 0;
	x = StartX;
	y = StartY;
	
	while(1)
	{
		atGRPX_FillCircle( lpCanvas, x, y, radius, Color );
		
		dist += nGap;
		if(INTTOREAL(dist)>=len) return;
		x = StartX + REALTOINT(dist * cosval);
		y = StartY + REALTOINT(dist * sinval);
	}
}


/*******************************
  ................... 그린다.
  (선 두께 있는 버전)
  (AntiAliasing 적용 버전)
********************************/
void atGRPX_DrawLineWidthDotAA( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY, atGRPX_COLOR Color, atINT nWidth, atINT nGap )
{
	atINT x, y, dist, radius, sign;
	atREAL dx, dy, theta, cosval, sinval, len;
	
	if(nWidth<=2)
		radius = nWidth;
	else
		radius = nWidth/2;
	
	if(nGap < nWidth)
		nGap = nWidth;
	
	dx = INTTOREAL(EndX - StartX);
	if(dx==0)
	{
		dist = 0;
		x = StartX;
		y = StartY;
		
		sign = EndY - StartY > 0 ? 1 : -1;
		len = ABS(INTTOREAL(EndY - StartY));
		
		while(1)
		{
			atGRPX_FillCircleAA( lpCanvas, x, y, radius, Color );
			
			dist += nGap;
			if(INTTOREAL(dist)>=len) return;
			x = StartX;
			y = StartY + dist * sign;
		}
	}
	dy = INTTOREAL(EndY - StartY);
	if(dy==0)
	{
		dist = 0;
		x = StartX;
		y = StartY;
		
		sign = EndX - StartX > 0 ? 1 : -1;
		len = ABS(INTTOREAL(EndX - StartX));
		
		while(1)
		{
			atGRPX_FillCircleAA( lpCanvas, x, y, radius, Color );
			
			dist += nGap;
			if(INTTOREAL(dist)>=len) return;
			x = StartX + dist * sign;
			y = StartY;
		}
	}
	theta = ATAN2(dx, dy);
	cosval = COS(theta);
	sinval = SIN(theta);
	len = REALDIV(dx, cosval);
	
	dist = 0;
	x = StartX;
	y = StartY;
	
	while(1)
	{
		atGRPX_FillCircleAA( lpCanvas, x, y, radius, Color );
		
		dist += nGap;
		if(INTTOREAL(dist)>=len) return;
		x = StartX + REALTOINT(dist * cosval);
		y = StartY + REALTOINT(dist * sinval);
	}
}
