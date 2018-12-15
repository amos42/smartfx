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
#include <math.h>
#include "GrpX.h"
#include "GrpX_samsung.h"
#include "_LIN_func.h"
#include <string.h>
#include "TriFunc.h"
//samsung
#include "A_Math.h"
#include "_BRE_func_samsung.h"


typedef void _FUNC_DRAW_PIXEL( atGRPX_CANVAS *, atINT, atINT, atGRPX_COLOR );
typedef void _FUNC_DRAW_LINE( atGRPX_CANVAS *, atINT, atINT, atINT, atINT, atGRPX_COLOR, atINT, atINT );

#define FIXEDEPSILON	6553

void GetInnerPoints(atLPOINT a_ptInput0, atLPOINT a_ptInput1, atINT nWidth, atLPOINT* a_ptOutput0, atLPOINT* a_ptOutput1)
{
	atINT i = 0;
	atREAL fM[3] = {0, 0, 0};
	atREAL fDX[3] = {0, 0, 0};
	atREAL fDY[3] = {0, 0, 0};
	
	atINT nDeltaNS = 0;
	atINT nDeltaEW = 0;
	
	*a_ptOutput0 = a_ptInput0;
	*a_ptOutput1 = a_ptInput1;
	
	fDX[0] = (a_ptInput1.nX - a_ptInput0.nX);
	fDY[0] = (a_ptInput1.nY - a_ptInput0.nY);
	
	if(ABS(fDX[0]) < FIXEDEPSILON) nDeltaEW = 0;
	else if(fDX[0] < 0) nDeltaEW--;
	else if(0 < fDX[0]) nDeltaEW++;
	
	if(ABS(fDY[0]) < FIXEDEPSILON) nDeltaNS = 0;
	else if(fDY[0] < 0) nDeltaNS--;
	else if(0 < fDY[0]) nDeltaNS++;
	
	if(ABS(fDX[0]) < FIXEDEPSILON) fM[0] = 0;
	else fM[0] = REALDIV(fDY[0], fDX[0]);
	
	for(i = 0; i < nWidth; i++)
	{
		if(nDeltaEW == 0 && nDeltaNS == 0)
		{
			return;
		}
		else if(nDeltaEW != 0 && nDeltaNS == 0)
		{
			a_ptOutput0->nX += INTTOREAL(nDeltaEW);
			a_ptOutput1->nX -= INTTOREAL(nDeltaEW);
		}
		else if(nDeltaEW == 0 && nDeltaNS != 0)
		{
			a_ptOutput0->nY += INTTOREAL(nDeltaNS);
			a_ptOutput1->nY -= INTTOREAL(nDeltaNS);
		}
		else if(nDeltaEW != 0 && nDeltaNS != 0)
		{
			fDX[1] = (a_ptInput1.nX - ((a_ptOutput0->nX) + INTTOREAL(nDeltaEW)));
			fDY[1] = (a_ptInput1.nY - (a_ptOutput0->nY));
			if(ABS(fDX[1]) < FIXEDEPSILON) fM[1] = 0;
			else fM[1] = REALDIV(fDY[1], fDX[1]);
			
			fDX[2] = (a_ptInput1.nX - (a_ptOutput0->nX));
			fDY[2] = (a_ptInput1.nY - ((a_ptOutput0->nY) + INTTOREAL(nDeltaNS)));
			if(ABS(fDX[2]) < FIXEDEPSILON) fM[2] = 0;
			else fM[2] = REALDIV(fDY[2], fDX[2]);
			
			if(ABS(fM[1] - fM[0]) < ABS(fM[2] - fM[0]))
			{
				a_ptOutput0->nX += INTTOREAL(nDeltaEW);
				a_ptOutput1->nX -= INTTOREAL(nDeltaEW);
			}
			else
			{
				a_ptOutput0->nY += INTTOREAL(nDeltaNS);
				a_ptOutput1->nY -= INTTOREAL(nDeltaNS);
			}
		}
	}
}

void GetCapPoints2(atPOINT ptCenter, atPOINT ptAnchor, atPOINT ptLeft, atPOINT ptRight, atPOINT a_ptOut[])
{
	atREAL fAngle[3]; // clockwise angles
	atLONG nDX = ptCenter.nX - ptAnchor.nX;
	atLONG nDY = ptCenter.nY - ptAnchor.nY;
	
	// RC: Re-Calculated, Round-Cap
	atREAL fRCCenterX = REALDIV(ptLeft.nX + ptRight.nX, 2);
	atREAL fRCCenterY = REALDIV(ptLeft.nY + ptRight.nY, 2);
	atREAL fDXLTOC = INTTOREAL(ptLeft.nX) - fRCCenterX;
	atREAL fDYLTOC = INTTOREAL(ptLeft.nY) - fRCCenterY;
	atREAL fRadius = FLOATTOREAL(sqrt(REALTOFLOAT(REALMUL(fDXLTOC, fDXLTOC) + REALMUL(fDYLTOC, fDYLTOC)))-1);
	atINT i = 0;
	
	if(nDX == 0 && nDY == 0)
	{
		return;
	}	
	else if(nDX == 0)
	{ 
		fAngle[1] = (nDY > 0) ? INTTOREAL(90) : INTTOREAL(270);
	}
	else if(nDY == 0)
	{
		fAngle[1] = (nDX >= 0) ? INTTOREAL(0) : INTTOREAL(180);
	}
	else
	{
		fAngle[1] = ATAN2(nDX, nDY); // anchor에서 center로 가는 벡터의 각도 (기준 각도)
	}
	
	fAngle[0] = fAngle[1] - INTTOREAL(45); // 기준 각도에서 45도를 뺀 각도
	fAngle[2] = fAngle[1] + INTTOREAL(45); // 기준 각도에서 45도를 더한 각도
	
	
	for(i = 0; i < 3; i++)
	{
//		a_ptOut[i].nX = REALTOINT(fRCCenterX + REALMUL(COS(fAngle[i]), fRadius) + FLOATTOREAL(0.5));
//		a_ptOut[i].nY = REALTOINT(fRCCenterY + REALMUL(SIN(fAngle[i]), fRadius) + FLOATTOREAL(0.5));
		a_ptOut[i].nX = REALTOINT(fRCCenterX + REALMUL(COS(fAngle[i]), REALMUL(fRadius, 78643)) + FLOATTOREAL(0.5));
		a_ptOut[i].nY = REALTOINT(fRCCenterY + REALMUL(SIN(fAngle[i]), REALMUL(fRadius, 78643)) + FLOATTOREAL(0.5));
	}
}

atINT GetJointAngle(atPOINT* ptIn_0, atPOINT* ptIn_1, atPOINT* ptIn_2) // ptIn[0]: Start Point, ptIn[1]: Center Point, ptIn[2]: End Point
{ // -> 1백만번 수행 시 0.5초 (CPU: 2.4GHz AMD Dual Core AMD Opteron 280 * 2, RAM: 4GB)
	atINT nAngle = 0;
	atREAL fAngle[3];
	atREAL fDX[2];
	atREAL fDY[2];
	atINT i = 0;

	fDX[0] = INTTOREAL(ptIn_0->nX) - INTTOREAL(ptIn_1->nX);
	fDY[0] = INTTOREAL(ptIn_0->nY) - INTTOREAL(ptIn_1->nY);
	fDX[1] = INTTOREAL(ptIn_2->nX) - INTTOREAL(ptIn_1->nX);
	fDY[1] = INTTOREAL(ptIn_2->nY) - INTTOREAL(ptIn_1->nY);

	for(i = 0; i < 2; i++)
		{
		if(fDX[i] == 0 && fDY[i] == 0)
			{
			return 0;
			}
		else if(fDX[i] == 0)
			{
			fAngle[i] = (fDY[i] >= 0) ? INTTOREAL(90) : INTTOREAL(270);
			}
		else if(fDY[i] == 0)
			{
			fAngle[i] = (fDX[i] >= 0) ? INTTOREAL(0) : INTTOREAL(180);
			}
		else
			{
			fAngle[i] = ATAN2(fDX[i], fDY[i]);
			}
		}

	fAngle[2] = fAngle[1] - fAngle[0];
	if(fAngle[2] < INTTOREAL(0))
		fAngle[2] += INTTOREAL(360);
	else if(fAngle[2] > INTTOREAL(360))
		fAngle[2] -= INTTOREAL(360);

	nAngle = REALTOINT(fAngle[2]);
	return nAngle;
}

atVOID GetEdgePoints(atRPOINT ptIn[], atRPOINT ptOut[])
{
	ptOut[0].rX = (ptIn[3].rX + ptIn[4].rX)/2;
	ptOut[0].rY = (ptIn[3].rY + ptIn[4].rY)/2;
	ptOut[1].rX = (ptIn[2].rX + ptIn[5].rX)/2;
	ptOut[1].rY = (ptIn[2].rY + ptIn[5].rY)/2;
}

void GetCapPoints(atPOINT a_ptCenter, atPOINT a_ptAnchor, int nRadius, atPOINT a_ptOut[])
{
	atREAL fAngle[5]; // clockwise angles
	atREAL fDX = INTTOREAL(a_ptCenter.nX) - INTTOREAL(a_ptAnchor.nX);
	atREAL fDY = INTTOREAL(a_ptCenter.nY) - INTTOREAL(a_ptAnchor.nY);
	atINT i = 0;
	
	if((a_ptCenter.nX == a_ptAnchor.nX) && (a_ptCenter.nY == a_ptAnchor.nY)) 
		{
		for(i = 0; i < 5; i++)
			{
			a_ptOut[i].nX = a_ptCenter.nX;
			a_ptOut[i].nY = a_ptCenter.nY;
			}
		return;
		}
	
	if(fDY == INTTOREAL(0))
	{
		fAngle[2] = (fDX >= INTTOREAL(0)) ? INTTOREAL(0) : INTTOREAL(180);
	}
	else if(fDX == INTTOREAL(0))
	{ 
		fAngle[2] = (fDY > INTTOREAL(0)) ? INTTOREAL(90) : INTTOREAL(270);
	}
	else
	{
		fAngle[2] = ATAN2(fDX, fDY); // anchor에서 center로 가는 벡터의 각도 (기준 각도)
	}
	
	fAngle[0] = fAngle[2] - INTTOREAL(90); // 기준 각도에서 90도를 뺀 각도
	fAngle[1] = fAngle[2] - INTTOREAL(45); // 기준 각도에서 45도를 뺀 각도
	fAngle[3] = fAngle[2] + INTTOREAL(45); // 기준 각도에서 45도를 더한 각도
	fAngle[4] = fAngle[2] + INTTOREAL(90); // 기준 각도에서 90도를 더한 각도
	
	for(i = 0; i < 5; i++)
	{
		a_ptOut[i].nX = a_ptCenter.nX + REALTOINT(COS(fAngle[i]) * (nRadius-1) + FLOATTOREAL(0.5));
		a_ptOut[i].nY = a_ptCenter.nY + REALTOINT(SIN(fAngle[i]) * (nRadius-1) + FLOATTOREAL(0.5));
	}
}

//ccs
// 여기 까지 작업함.
atREAL GetPolygonFromWideLine(atREAL kiL, atREAL kiT, atREAL kiR, atREAL kiB, int nWidth, atRPOINT pkPtOut[])
{
	atRPOINT ptLT;
	atRPOINT ptRB;
	atLONG nRadius = 0;
	atREAL fAngle[3]; // clockwise angles
	atLONG nDX = 0;
	atLONG nDY = 0;
	atPOINT ptVector[2];
	atINT i = 0;
	
	atREAL t1 = 0;
	atINT t2 = 0;
	
	if(nWidth <= 1)
	{
		pkPtOut[0].rX = kiL;
		pkPtOut[0].rY = kiT;
		pkPtOut[1].rX = kiL;
		pkPtOut[1].rY = kiT;
		pkPtOut[2].rX = kiR;
		pkPtOut[2].rY = kiB;
		pkPtOut[3].rX = kiR;
		pkPtOut[3].rY = kiB;
		return INTTOREAL(360);
	}
	
	ptLT.rX = REALTOINT(kiL);
	ptLT.rY = REALTOINT(kiT);
	ptRB.rX = REALTOINT(kiR);
	ptRB.rY = REALTOINT(kiB);
	nDX = ptRB.rX - ptLT.rX;
	nDY = ptRB.rY - ptLT.rY;
	
	if(nDX == INTTOREAL(0) && nDY == INTTOREAL(0))
	{
		return INTTOREAL(360);
	}
	else if(nDX == INTTOREAL(0))
	{ 
		fAngle[2] = (nDY > INTTOREAL(0)) ? INTTOREAL(90) : INTTOREAL(270);
	}
	else if(nDY == INTTOREAL(0))
	{
		fAngle[2] = (nDX >= INTTOREAL(0)) ? INTTOREAL(0) : INTTOREAL(180);
	}	
	else
	{
		fAngle[2] = ATAN2(nDX, nDY); // RB에서 LT으로 가는 벡터의 각도 (기준 각도)
	}
	
	fAngle[0] = fAngle[2] + INTTOREAL(90); // 기준 각도에서 90도를 더한 각도
	fAngle[1] = fAngle[2] - INTTOREAL(90); // 기준 각도에서 90도를 뺀 각도
	
	if(REALTOINT(fAngle[0]) <= 45 || REALTOINT(fAngle[0]) > 270)
	{
		nRadius = nWidth/2;
		ptVector[0].nX = REALTOINT(COS(fAngle[0]) * nRadius + FLOATTOREAL(0.5));
		ptVector[0].nY = REALTOINT(SIN(fAngle[0]) * nRadius + FLOATTOREAL(0.5));
		if(nWidth%2 == 0) nRadius--;
		ptVector[1].nX = REALTOINT(COS(fAngle[1]) * nRadius + FLOATTOREAL(0.5));
		ptVector[1].nY = REALTOINT(SIN(fAngle[1]) * nRadius + FLOATTOREAL(0.5));
	}
	else
	{
		nRadius = nWidth/2;
		ptVector[1].nX = REALTOINT(COS(fAngle[1]) * nRadius + FLOATTOREAL(0.5));
		ptVector[1].nY = REALTOINT(SIN(fAngle[1]) * nRadius + FLOATTOREAL(0.5));
		if(nWidth%2 == 0) nRadius--;
		ptVector[0].nX = REALTOINT(COS(fAngle[0]) * nRadius + FLOATTOREAL(0.5));
		ptVector[0].nY = REALTOINT(SIN(fAngle[0]) * nRadius + FLOATTOREAL(0.5));
	}
	
	pkPtOut[0].rX = INTTOREAL(ptLT.rX + ptVector[0].nX);
	pkPtOut[0].rY = INTTOREAL(ptLT.rY + ptVector[0].nY);
	pkPtOut[1].rX = INTTOREAL(ptLT.rX + ptVector[1].nX);
	pkPtOut[1].rY = INTTOREAL(ptLT.rY + ptVector[1].nY);
	pkPtOut[2].rX = INTTOREAL(ptRB.rX + ptVector[1].nX);
	pkPtOut[2].rY = INTTOREAL(ptRB.rY + ptVector[1].nY);
	pkPtOut[3].rX = INTTOREAL(ptRB.rX + ptVector[0].nX);
	pkPtOut[3].rY = INTTOREAL(ptRB.rY + ptVector[0].nY);
	
	return INTTOREAL(fAngle[2]);
}

void atGRPX_DrawRoundCap(atGRPX_CANVAS *lpCanvas, atPOINT* Pt, atPOINT* Pt1, atINT width, atGRPX_COLOR Color)
{
	atPOINT Outpt[5];
	atPOINT Inpt[2];
	
	Inpt[0].nX = Pt->nX;
	Inpt[0].nY = Pt->nY;
	Inpt[1].nX = Pt1->nX;
	Inpt[1].nY = Pt1->nY;
	
	//Get Draw Point
	GetCapPoints(Inpt[0], Inpt[1], width/2, Outpt);
	
	//DrawTriangle
	atGRPX_FillTriangle( lpCanvas,	Pt->nX, Pt->nY,
		Outpt[0].nX, Outpt[0].nY,
		Outpt[1].nX, Outpt[1].nY, Color );
	atGRPX_FillTriangle( lpCanvas,	Pt->nX, Pt->nY,
		Outpt[1].nX, Outpt[1].nY,
		Outpt[2].nX, Outpt[2].nY, Color);
	atGRPX_FillTriangle( lpCanvas,	Pt->nX, Pt->nY,
		Outpt[2].nX, Outpt[2].nY,
		Outpt[3].nX, Outpt[3].nY, Color);
	atGRPX_FillTriangle( lpCanvas,	Pt->nX, Pt->nY,
		Outpt[3].nX, Outpt[3].nY,
		Outpt[4].nX, Outpt[4].nY, Color );
}

void atGRPX_DrawRoundCap2(atGRPX_CANVAS *lpCanvas, atPOINT ptCenter, atPOINT ptAnchor, atPOINT ptLeft, atPOINT ptRight, atGRPX_COLOR Color, atINT nWidth)
{
 	atPOINT a_ptOutput[3];
 
	if((ptCenter.nX == ptAnchor.nX) && (ptCenter.nY == ptAnchor.nY))
	{
		return;
	}
	// Get Draw Point
 	GetCapPoints2(ptCenter, ptAnchor, ptLeft, ptRight, a_ptOutput);

	// Draw Triangle

	if(nWidth > 5)
	{
		atGRPX_FillRoundCapTriangleAA( lpCanvas,	ptCenter.nX, ptCenter.nY,
			ptLeft.nX, ptLeft.nY,
			a_ptOutput[0].nX, a_ptOutput[0].nY, Color);
		atGRPX_FillRoundCapTriangleAA( lpCanvas,	ptCenter.nX, ptCenter.nY,
			a_ptOutput[0].nX, a_ptOutput[0].nY,
			a_ptOutput[1].nX, a_ptOutput[1].nY, Color);
		atGRPX_FillRoundCapTriangleAA( lpCanvas,	ptCenter.nX, ptCenter.nY,
			a_ptOutput[1].nX, a_ptOutput[1].nY,
			a_ptOutput[2].nX, a_ptOutput[2].nY, Color);
		atGRPX_FillRoundCapTriangleAA( lpCanvas,	ptCenter.nX, ptCenter.nY,
			a_ptOutput[2].nX, a_ptOutput[2].nY,
			ptRight.nX, ptRight.nY, Color );

		atGRPX_DrawLineAA(lpCanvas, ptLeft.nX, ptLeft.nY, a_ptOutput[0].nX, a_ptOutput[0].nY, Color);
		atGRPX_DrawLineAA(lpCanvas, a_ptOutput[0].nX, a_ptOutput[0].nY, a_ptOutput[1].nX, a_ptOutput[1].nY, Color);
		atGRPX_DrawLineAA(lpCanvas, a_ptOutput[1].nX, a_ptOutput[1].nY, a_ptOutput[2].nX, a_ptOutput[2].nY, Color);
		atGRPX_DrawLineAA(lpCanvas, a_ptOutput[2].nX, a_ptOutput[2].nY, ptRight.nX, ptRight.nY, Color);
	}	
}

void atGRPX_DrawRoundCap3(atGRPX_CANVAS *lpCanvas, atPOINT* ptCenter, atINT nWidth, atGRPX_COLOR Color, atRECT* clip)
{
	atPOINT ptTL; //TopLeft
	atPOINT ptBR; //BottomLeft
	
	atINT radius;
	atBOOL isIn; // is in clip rect?
	
	radius = nWidth / 2;
	
	ptTL.nX = ptCenter->nX - radius;
	ptTL.nY = ptCenter->nY - radius;
	ptBR.nX = ptCenter->nX + radius;
	ptBR.nY = ptCenter->nY + radius;
	
	//클리핑 영역 안인지 확인
	if(ptTL.nX > clip->nEndX || ptTL.nY > clip->nEndY || ptBR.nX < clip->nStartX || ptBR.nY < clip->nStartY)
	{
		return;
	}
	
	if(ptTL.nX < clip->nStartX || ptTL.nY < clip->nStartY || ptBR.nX > clip->nEndX || ptBR.nY > clip->nEndY)
	{
		isIn = atFALSE;
	}
	else
	{
		isIn = atTRUE;
	}
	
	if(isIn)
	{
		atGRPX_DrawCircleTableMem(lpCanvas, ptTL, nWidth, Color);
	}
	else
	{
		atGRPX_DrawCircleTablePt(lpCanvas, ptTL, nWidth, Color, clip);
	}
	
	
	//클리핑 영역 안인 경우(메모리 접근)
	
	//밖인 경우(좌표 접근)
}

// In : 삼각형
atVOID	atGRPX_DrawPolyBreLineWidthAAIn(atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color, atGRPX_COLOR OutColor, atINT nWidth)
{
	atINT i = 0;
	atPOINT *lpPtr, *lpPtr2, *lpPtr3;
	atREAL iL, iT, iR, iB;
	atRPOINT kPtIn1[4];  // 임시 포인트
	atRPOINT kPtIn2[4]; //임시 포인트
	atRPOINT kPtIn[8];  //입력 포인트
	atRPOINT kPtOut[2]; //계산된 코너 포인트
	atINT jointAngle;
	atPOINT ptLeft; //For Round Cap
	atPOINT ptRight;
	
	//temp
	atRECT *clip = atGRPX_GetClipRect( lpCanvas );

	if( lpPtrList == atNULL ) return;
	if( nCount <= 0 ) return;	
	
	lpPtr = lpPtrList;
	
	if( nCount == 1 )
	{
		atGRPX_DrawPixel( lpCanvas, lpPtr->nX, lpPtr->nY, Color );
		return;
	}

	lpPtr2 = lpPtr + 1;
	
	if( nCount == 2)
	{
		iL = INTTOREAL(lpPtr->nX);
		iT = INTTOREAL(lpPtr->nY);
		iR = INTTOREAL(lpPtr2->nX);
		iB = INTTOREAL(lpPtr2->nY);

		if(iL == iR && iT == iB)
		{
			atGRPX_DrawPixel( lpCanvas, lpPtr->nX, lpPtr->nY, Color );
			return;
		}
		GetPolygonFromWideLine( iL, iT, iR, iB, nWidth, kPtIn1 );
		atGRPX_DrawBreLineWidthAAIn( lpCanvas, kPtIn1[0], kPtIn1[1], kPtIn1[2], kPtIn1[3], Color, OutColor, nWidth );

		ptLeft.nX = REALTOINT(kPtIn1[0].rX);
		ptLeft.nY = REALTOINT(kPtIn1[0].rY);
		ptRight.nX = REALTOINT(kPtIn1[1].rX);
		ptRight.nY = REALTOINT(kPtIn1[1].rY);
		atGRPX_DrawRoundCap2( lpCanvas, *lpPtr, *lpPtr2, ptLeft, ptRight, Color, nWidth);
		ptLeft.nX = REALTOINT(kPtIn1[2].rX);
		ptLeft.nY = REALTOINT(kPtIn1[2].rY);
		ptRight.nX = REALTOINT(kPtIn1[3].rX);
		ptRight.nY = REALTOINT(kPtIn1[3].rY);
		atGRPX_DrawRoundCap2( lpCanvas, *lpPtr2, *lpPtr, ptLeft, ptRight, Color, nWidth);
		return;
	}

	lpPtr3 = lpPtr2 + 1;

	
	iL = INTTOREAL(lpPtr->nX);
	iT = INTTOREAL(lpPtr->nY);
	iR = INTTOREAL(lpPtr2->nX);
	iB = INTTOREAL(lpPtr2->nY);

	while(iL == iR && iT == iB)
	{
		lpPtr = lpPtr2;	
		lpPtr2 = lpPtr3; 
		lpPtr3++;
		iL = INTTOREAL(lpPtr->nX);
		iT = INTTOREAL(lpPtr->nY);
		iR = INTTOREAL(lpPtr2->nX);
		iB = INTTOREAL(lpPtr2->nY);
		i++;
		if(i == nCount-1)return;
	}

	GetPolygonFromWideLine( iL, iT, iR, iB, nWidth, kPtIn1 );
	
	kPtOut[0].rX = kPtIn[4].rX = kPtIn1[0].rX;
	kPtOut[0].rY = kPtIn[4].rY = kPtIn1[0].rY;
	kPtOut[1].rX = kPtIn[5].rX = kPtIn1[1].rX;
	kPtOut[1].rY = kPtIn[5].rY = kPtIn1[1].rY;
	kPtIn[6].rX = kPtIn1[2].rX;
	kPtIn[6].rY = kPtIn1[2].rY;
	kPtIn[7].rX = kPtIn1[3].rX;
	kPtIn[7].rY = kPtIn1[3].rY;
	
	ptLeft.nX = REALTOINT(kPtIn1[0].rX);
	ptLeft.nY = REALTOINT(kPtIn1[0].rY);
	ptRight.nX = REALTOINT(kPtIn1[1].rX);
	ptRight.nY = REALTOINT(kPtIn1[1].rY);
	atGRPX_DrawRoundCap2( lpCanvas, *lpPtr, *lpPtr2, ptLeft, ptRight, Color, nWidth);
	
	for( ; i < nCount-2; i++ )
	{ 
		
		kPtIn[0].rX = kPtIn[4].rX;
		kPtIn[0].rY = kPtIn[4].rY;
		kPtIn[1].rX = kPtIn[5].rX;
		kPtIn[1].rY = kPtIn[5].rY;
		kPtIn[2].rX = kPtIn[6].rX;
		kPtIn[2].rY = kPtIn[6].rY;
		kPtIn[3].rX = kPtIn[7].rX;
		kPtIn[3].rY = kPtIn[7].rY;
		
		iL = INTTOREAL(lpPtr2->nX);
		iT = INTTOREAL(lpPtr2->nY);
		iR = INTTOREAL(lpPtr3->nX);
		iB = INTTOREAL(lpPtr3->nY);

		if(iL == iR && iT == iB)
		{
			lpPtr = lpPtr2;	
			lpPtr2 = lpPtr3; 
			lpPtr3++;
			continue;
		}
		
		GetPolygonFromWideLine( iL, iT, iR, iB, nWidth, kPtIn2 );
		
		kPtIn[4].rX = kPtIn2[0].rX;
		kPtIn[4].rY = kPtIn2[0].rY;
		kPtIn[5].rX = kPtIn2[1].rX;
		kPtIn[5].rY = kPtIn2[1].rY;
		kPtIn[6].rX = kPtIn2[2].rX;
		kPtIn[6].rY = kPtIn2[2].rY;
		kPtIn[7].rX = kPtIn2[3].rX;
		kPtIn[7].rY = kPtIn2[3].rY;
		
		//점을 구한다.
		jointAngle = GetJointAngle(lpPtr, lpPtr2, lpPtr3);
		if(jointAngle > 135 && jointAngle < 225 )
		{
			// 90~270도
			GetEdgePoints(kPtIn, kPtOut);
			
			//외각선
			atGRPX_DrawBreLineWidthAAIn( lpCanvas, kPtIn[0], kPtIn[1], kPtOut[1], kPtOut[0], Color, OutColor, nWidth );
			
			// 새로운 중간 값을 다음에 사용을 위해 저장
			kPtIn[4].rX = kPtOut[0].rX;
			kPtIn[4].rY = kPtOut[0].rY;
			kPtIn[5].rX = kPtOut[1].rX;
			kPtIn[5].rY = kPtOut[1].rY;
		}
		else
		{
			//외각선
			atGRPX_DrawBreLineWidthAAIn( lpCanvas, kPtIn[0], kPtIn[1], kPtIn[2], kPtIn[3], Color, OutColor, nWidth );		
			
			// Draw RoundCap
			ptLeft.nX = REALTOINT(kPtIn[2].rX);
			ptLeft.nY = REALTOINT(kPtIn[2].rY);
			ptRight.nX = REALTOINT(kPtIn[3].rX);
			ptRight.nY = REALTOINT(kPtIn[3].rY);
			atGRPX_DrawRoundCap2( lpCanvas, *lpPtr2, *lpPtr, ptLeft, ptRight, Color, nWidth);
			
			
			ptLeft.nX = REALTOINT(kPtIn[4].rX);
			ptLeft.nY = REALTOINT(kPtIn[4].rY);
			ptRight.nX = REALTOINT(kPtIn[5].rX);
			ptRight.nY = REALTOINT(kPtIn[5].rY);
			atGRPX_DrawRoundCap2( lpCanvas, *lpPtr2, *lpPtr3, ptLeft, ptRight, Color, nWidth);
		}
		
		//다음에 사용할 좌표를 저장.
		lpPtr = lpPtr2;
		lpPtr2 = lpPtr3;
		lpPtr3++;
	}
	
	// 마지막 삼각형
	atGRPX_DrawBreLineWidthAAIn( lpCanvas, kPtIn[4], kPtIn[5], kPtIn[6], kPtIn[7], Color, OutColor, nWidth );
	ptLeft.nX = REALTOINT(kPtIn[6].rX);
	ptLeft.nY = REALTOINT(kPtIn[6].rY);
	ptRight.nX = REALTOINT(kPtIn[7].rX);
	ptRight.nY = REALTOINT(kPtIn[7].rY);
	atGRPX_DrawRoundCap2( lpCanvas, *lpPtr2, *lpPtr, ptLeft, ptRight, Color, nWidth);
}

atVOID	atGRPX_DrawPolyBreLineWidthAAInEdge(atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color, atGRPX_COLOR OutColor, atINT nWidth)
{
	atINT i = 0;
	atPOINT *lpPtr, *lpPtr2, *lpPtr3;
	atREAL iL, iT, iR, iB;
	atRPOINT kPtIn1[4];  // 임시 포인트
	atRPOINT kPtIn2[4]; //임시 포인트
	atRPOINT kPtIn[8];  //입력 포인트
	atRPOINT kPtOut[2]; //계산된 코너 포인트
	atINT jointAngle;
	

	if( lpPtrList == atNULL ) return;
	if( nCount <= 0 ) return;	
	
	lpPtr = lpPtrList;
	
	if( nCount == 1 )
	{
		return;
	}

	lpPtr2 = lpPtr + 1;
	
	if( nCount == 2)
	{
		iL = INTTOREAL(lpPtr->nX);
		iT = INTTOREAL(lpPtr->nY);
		iR = INTTOREAL(lpPtr2->nX);
		iB = INTTOREAL(lpPtr2->nY);

		if(iL == iR && iT == iB)
		{
			atGRPX_DrawPixel( lpCanvas, lpPtr->nX, lpPtr->nY, Color );
			return;
		}

		GetPolygonFromWideLine( iL, iT, iR, iB, nWidth, kPtIn1 );
		atGRPX_DrawBreLineWidthAAInEdge( lpCanvas, kPtIn1[0], kPtIn1[1], kPtIn1[2], kPtIn1[3], Color, OutColor, nWidth );

		return;
	}

	lpPtr3 = lpPtr2 + 1;

	
	iL = INTTOREAL(lpPtr->nX);
	iT = INTTOREAL(lpPtr->nY);
	iR = INTTOREAL(lpPtr2->nX);
	iB = INTTOREAL(lpPtr2->nY);

	while(iL == iR && iT == iB)
	{
		lpPtr = lpPtr2;	
		lpPtr2 = lpPtr3; 
		lpPtr3++;
		iL = INTTOREAL(lpPtr->nX);
		iT = INTTOREAL(lpPtr->nY);
		iR = INTTOREAL(lpPtr2->nX);
		iB = INTTOREAL(lpPtr2->nY);
		i++;
		if(i == nCount-1)return;
	}

	GetPolygonFromWideLine( iL, iT, iR, iB, nWidth, kPtIn1 );
	
	kPtOut[0].rX = kPtIn[4].rX = kPtIn1[0].rX;
	kPtOut[0].rY = kPtIn[4].rY = kPtIn1[0].rY;
	kPtOut[1].rX = kPtIn[5].rX = kPtIn1[1].rX;
	kPtOut[1].rY = kPtIn[5].rY = kPtIn1[1].rY;
	kPtIn[6].rX = kPtIn1[2].rX;
	kPtIn[6].rY = kPtIn1[2].rY;
	kPtIn[7].rX = kPtIn1[3].rX;
	kPtIn[7].rY = kPtIn1[3].rY;
	
	for( ; i < nCount-2; i++ )
	{ 
		
		kPtIn[0].rX = kPtIn[4].rX;
		kPtIn[0].rY = kPtIn[4].rY;
		kPtIn[1].rX = kPtIn[5].rX;
		kPtIn[1].rY = kPtIn[5].rY;
		kPtIn[2].rX = kPtIn[6].rX;
		kPtIn[2].rY = kPtIn[6].rY;
		kPtIn[3].rX = kPtIn[7].rX;
		kPtIn[3].rY = kPtIn[7].rY;
		
		iL = INTTOREAL(lpPtr2->nX);
		iT = INTTOREAL(lpPtr2->nY);
		iR = INTTOREAL(lpPtr3->nX);
		iB = INTTOREAL(lpPtr3->nY);

		if(iL == iR && iT == iB)
		{
			lpPtr = lpPtr2;	
			lpPtr2 = lpPtr3; 
			lpPtr3++;
			continue;
		}
		
		GetPolygonFromWideLine( iL, iT, iR, iB, nWidth, kPtIn2 );
		
		kPtIn[4].rX = kPtIn2[0].rX;
		kPtIn[4].rY = kPtIn2[0].rY;
		kPtIn[5].rX = kPtIn2[1].rX;
		kPtIn[5].rY = kPtIn2[1].rY;
		kPtIn[6].rX = kPtIn2[2].rX;
		kPtIn[6].rY = kPtIn2[2].rY;
		kPtIn[7].rX = kPtIn2[3].rX;
		kPtIn[7].rY = kPtIn2[3].rY;
		
		//점을 구한다.
		jointAngle = GetJointAngle(lpPtr, lpPtr2, lpPtr3);
		if(jointAngle > 135 && jointAngle < 225 )
		{
			// 90~270도
			GetEdgePoints(kPtIn, kPtOut);
			
			//외각선
			atGRPX_DrawBreLineWidthAAInEdge( lpCanvas, kPtIn[0], kPtIn[1], kPtOut[1], kPtOut[0], Color, OutColor, nWidth );
			
			// 새로운 중간 값을 다음에 사용을 위해 저장
			kPtIn[4].rX = kPtOut[0].rX;
			kPtIn[4].rY = kPtOut[0].rY;
			kPtIn[5].rX = kPtOut[1].rX;
			kPtIn[5].rY = kPtOut[1].rY;
		}
		else
		{
			//외각선
			atGRPX_DrawBreLineWidthAAInEdge( lpCanvas, kPtIn[0], kPtIn[1], kPtIn[2], kPtIn[3], Color, OutColor, nWidth );

		}
		
		//다음에 사용할 좌표를 저장.
		lpPtr = lpPtr2;
		lpPtr2 = lpPtr3;
		lpPtr3++;
	}

	// 마지막 삼각형

	atGRPX_DrawBreLineWidthAAInEdge( lpCanvas, kPtIn[4], kPtIn[5], kPtIn[6], kPtIn[7], Color, OutColor, nWidth );
}
// Out : 두께가 2인 라인
atVOID	atGRPX_DrawPolyBreLineWidthAAOut(atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color, atGRPX_COLOR BkColor, atINT nWidth)
{
	atINT i = 0;
	atPOINT *lpPtr, *lpPtr2, *lpPtr3;
	atREAL iL, iT, iR, iB;
	atRPOINT kPtIn1[4];  // 임시 포인트
	atRPOINT kPtIn2[4]; //임시 포인트
	atRPOINT kPtIn[8];  //입력 포인트
	atRPOINT kPtOut[2]; //계산된 코너 포인트
	atPOINT kPtConnect[4]; //Round Rect or 다음 직선과 연결하기 위해 계산된 좌표 모음.
	atLONG jointAngle;
	atPOINT ptLeft; //For Round Cap
	atPOINT ptRight;
	atPOINT ptCenter; 
	atPOINT ptAnchor;

	//for Round Rect
	atRECT *clip = atGRPX_GetClipRect( lpCanvas );	

	if( lpPtrList == atNULL ) return;
	if( nCount <= 0 ) return;	
	
	lpPtr = lpPtrList;
	
	if( nCount == 1 )
	{
		atGRPX_DrawPixel( lpCanvas, lpPtr->nX, lpPtr->nY, Color );
		return;
	}

	lpPtr2 = lpPtr + 1;
	
	if( nCount == 2)
	{
		iL = INTTOREAL(lpPtr->nX);
		iT = INTTOREAL(lpPtr->nY);
		iR = INTTOREAL(lpPtr2->nX);
		iB = INTTOREAL(lpPtr2->nY);

		if(iL == iR && iT == iB)
		{
			atGRPX_DrawPixel( lpCanvas, lpPtr->nX, lpPtr->nY, Color );
			return;
		}

		GetPolygonFromWideLine( iL, iT, iR, iB, nWidth, kPtIn1 );
		atGRPX_DrawBreLineWidthAAOut( lpCanvas, kPtIn1[0], kPtIn1[1], kPtIn1[2], kPtIn1[3], kPtConnect, Color, BkColor, nWidth );
		ptCenter.nX = lpPtr->nX;
		ptCenter.nY = lpPtr->nY;
		ptAnchor.nX = lpPtr2->nX;
		ptAnchor.nY = lpPtr2->nY;
		ptLeft.nX = (kPtConnect[0].nX);
		ptLeft.nY = (kPtConnect[0].nY);
		ptRight.nX = (kPtConnect[1].nX);
		ptRight.nY = (kPtConnect[1].nY);
		atGRPX_DrawRoundCap2( lpCanvas, ptCenter, ptAnchor, ptLeft, ptRight, Color, nWidth);
		ptCenter.nX = lpPtr2->nX;
		ptCenter.nY = lpPtr2->nY;
		ptAnchor.nX = lpPtr->nX;
		ptAnchor.nY = lpPtr->nY;
		ptLeft.nX = (kPtConnect[2].nX);
		ptLeft.nY = (kPtConnect[2].nY);
		ptRight.nX = (kPtConnect[3].nX);
		ptRight.nY = (kPtConnect[3].nY);
		atGRPX_DrawRoundCap2( lpCanvas,  ptCenter, ptAnchor, ptLeft, ptRight, Color, nWidth);
		return;
	}

	lpPtr3 = lpPtr2 + 1;

	
	iL = INTTOREAL(lpPtr->nX);
	iT = INTTOREAL(lpPtr->nY);
	iR = INTTOREAL(lpPtr2->nX);
	iB = INTTOREAL(lpPtr2->nY);

	while(iL == iR && iT == iB)
	{
		lpPtr = lpPtr2;	
		lpPtr2 = lpPtr3; 
		lpPtr3++;
		iL = INTTOREAL(lpPtr->nX);
		iT = INTTOREAL(lpPtr->nY);
		iR = INTTOREAL(lpPtr2->nX);
		iB = INTTOREAL(lpPtr2->nY);
		i++;
		if(i == nCount-1)return;
	}

	GetPolygonFromWideLine( iL, iT, iR, iB, nWidth, kPtIn1 );
	
	kPtOut[0].rX = kPtIn[4].rX = kPtIn1[0].rX;
	kPtOut[0].rY = kPtIn[4].rY = kPtIn1[0].rY;
	kPtOut[1].rX = kPtIn[5].rX = kPtIn1[1].rX;
	kPtOut[1].rY = kPtIn[5].rY = kPtIn1[1].rY;
	kPtIn[6].rX = kPtIn1[2].rX;
	kPtIn[6].rY = kPtIn1[2].rY;
	kPtIn[7].rX = kPtIn1[3].rX;
	kPtIn[7].rY = kPtIn1[3].rY;

	atGRPX_GetNextRoundPoint(kPtIn1[0], kPtIn1[1], kPtIn1[2], kPtIn1[3], kPtConnect, nWidth );
	ptCenter.nX = lpPtr->nX;
	ptCenter.nY = lpPtr->nY;
	ptAnchor.nX = lpPtr2->nX;
	ptAnchor.nY = lpPtr2->nY;
	ptLeft.nX = (kPtConnect[0].nX);
	ptLeft.nY = (kPtConnect[0].nY);
	ptRight.nX = (kPtConnect[1].nX);
	ptRight.nY = (kPtConnect[1].nY);
	atGRPX_DrawRoundCap2( lpCanvas,  ptCenter, ptAnchor, ptLeft, ptRight, Color, nWidth);
	
	for( ; i < nCount-2; i++ )
	{ 
		
		kPtIn[0].rX = kPtIn[4].rX;
		kPtIn[0].rY = kPtIn[4].rY;
		kPtIn[1].rX = kPtIn[5].rX;
		kPtIn[1].rY = kPtIn[5].rY;
		kPtIn[2].rX = kPtIn[6].rX;
		kPtIn[2].rY = kPtIn[6].rY;
		kPtIn[3].rX = kPtIn[7].rX;
		kPtIn[3].rY = kPtIn[7].rY;
		
		iL = INTTOREAL(lpPtr2->nX);
		iT = INTTOREAL(lpPtr2->nY);
		iR = INTTOREAL(lpPtr3->nX);
		iB = INTTOREAL(lpPtr3->nY);

		if(iL == iR && iT == iB)
		{			
			if(i != nCount-3)
			{
				lpPtr = lpPtr2;	
				lpPtr2 = lpPtr3; 
				lpPtr3++;
			}
			continue;
		}
		
		GetPolygonFromWideLine( iL, iT, iR, iB, nWidth, kPtIn2 );
		
		kPtIn[4].rX = kPtIn2[0].rX;
		kPtIn[4].rY = kPtIn2[0].rY;
		kPtIn[5].rX = kPtIn2[1].rX;
		kPtIn[5].rY = kPtIn2[1].rY;
		kPtIn[6].rX = kPtIn2[2].rX;
		kPtIn[6].rY = kPtIn2[2].rY;
		kPtIn[7].rX = kPtIn2[3].rX;
		kPtIn[7].rY = kPtIn2[3].rY;
		
		//점을 구한다.
		jointAngle = GetJointAngle(lpPtr, lpPtr2, lpPtr3);
		if(jointAngle > 135 && jointAngle < 225 )
		{
			// 90~270도
			GetEdgePoints(kPtIn, kPtOut);
			
			//외각선
			atGRPX_DrawBreLineWidthAAOut( lpCanvas, kPtIn[0], kPtIn[1], kPtOut[1], kPtOut[0], kPtConnect, Color, BkColor, nWidth );
			
			// 새로운 중간 값을 다음에 사용을 위해 저장
			kPtIn[4].rX = kPtOut[0].rX;
			kPtIn[4].rY = kPtOut[0].rY;
			kPtIn[5].rX = kPtOut[1].rX;
			kPtIn[5].rY = kPtOut[1].rY;
		}
		else
		{
			//외각선
			atGRPX_DrawBreLineWidthAAOut( lpCanvas, kPtIn[0], kPtIn[1], kPtIn[2], kPtIn[3], kPtConnect, Color, BkColor, nWidth );
			
			// Draw RoundCap
			ptCenter.nX = lpPtr2->nX;
			ptCenter.nY = lpPtr2->nY;
			ptAnchor.nX = lpPtr->nX;
			ptAnchor.nY = lpPtr->nY;
			ptLeft.nX = (kPtConnect[2].nX);
			ptLeft.nY = (kPtConnect[2].nY);
			ptRight.nX = (kPtConnect[3].nX);
			ptRight.nY = (kPtConnect[3].nY);
			atGRPX_DrawRoundCap2( lpCanvas,  ptCenter, ptAnchor, ptLeft, ptRight, Color, nWidth);
			
			atGRPX_GetNextRoundPoint(kPtIn[4], kPtIn[5], kPtIn[6], kPtIn[7], kPtConnect, nWidth);
			ptCenter.nX = lpPtr2->nX;
			ptCenter.nY = lpPtr2->nY;
			ptAnchor.nX = lpPtr3->nX;
			ptAnchor.nY = lpPtr3->nY;
			ptLeft.nX = (kPtConnect[0].nX);
			ptLeft.nY = (kPtConnect[0].nY);
			ptRight.nX = (kPtConnect[1].nX);
			ptRight.nY = (kPtConnect[1].nY);
 			atGRPX_DrawRoundCap2( lpCanvas,  ptCenter, ptAnchor, ptLeft, ptRight, Color, nWidth);
		}
		
		
		//다음에 사용할 좌표를 저장.
		lpPtr = lpPtr2;
		lpPtr2 = lpPtr3;
		lpPtr3++;
	}
	
	// 마지막 삼각형
	
	atGRPX_DrawBreLineWidthAAOut( lpCanvas, kPtIn[4], kPtIn[5], kPtIn[6], kPtIn[7], kPtConnect, Color, BkColor, nWidth );
	ptCenter.nX = lpPtr2->nX;
	ptCenter.nY = lpPtr2->nY;
	ptAnchor.nX = lpPtr->nX;
	ptAnchor.nY = lpPtr->nY;
	ptLeft.nX = (kPtConnect[2].nX);
	ptLeft.nY = (kPtConnect[2].nY);
	ptRight.nX = (kPtConnect[3].nX);
	ptRight.nY = (kPtConnect[3].nY);
	atGRPX_DrawRoundCap2( lpCanvas,  ptCenter, ptAnchor, ptLeft, ptRight, Color, nWidth);
}


atVOID atGRPX_DrawPolyEdgeLine( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nCount, atGRPX_COLOR Color, atINT nWidth )
{
	atINT i = 0;
	atPOINT *lpPtr, *lpPtr2, *lpPtr3;
	atREAL iL, iT, iR, iB;
	atRPOINT kPtIn1[4];  // 임시 포인트
	atRPOINT kPtIn2[4]; //임시 포인트
	atRPOINT kPtIn[8];  //입력 포인트
	atRPOINT kPtOut[2]; //계산된 코너 포인트
	atINT jointAngle;
	

	if( lpPtrList == atNULL ) return;
	if( nCount <= 0 ) return;	
	
	lpPtr = lpPtrList;
	
	if( nCount == 1 )
	{
		atGRPX_DrawPixel( lpCanvas, lpPtr->nX, lpPtr->nY, Color );
		return;
	}

	lpPtr2 = lpPtr + 1;
	
	if( nCount == 2)
	{

		atGRPX_DrawLineWidthExtAA( lpCanvas, lpPtr->nX, lpPtr->nY, lpPtr2->nX, lpPtr2->nY, Color, nWidth,atGRPX_LINEEXT_EXTEND);
		atGRPX_DrawRoundCap( lpCanvas, lpPtr, lpPtr2, nWidth, Color);
		atGRPX_DrawRoundCap( lpCanvas, lpPtr2, lpPtr, nWidth, Color);
		
		return;
	}

	lpPtr3 = lpPtr2 + 1;

	
	iL = INTTOREAL(lpPtr->nX);
	iT = INTTOREAL(lpPtr->nY);
	iR = INTTOREAL(lpPtr2->nX);
	iB = INTTOREAL(lpPtr2->nY);

	while(iL == iR && iT == iB)
	{
		lpPtr = lpPtr2;	
		lpPtr2 = lpPtr3; 
		lpPtr3++;
		iL = INTTOREAL(lpPtr->nX);
		iT = INTTOREAL(lpPtr->nY);
		iR = INTTOREAL(lpPtr2->nX);
		iB = INTTOREAL(lpPtr2->nY);
		i++;
		if(i == nCount-1)return;
	}

	GetPolygonFromWideLine( iL, iT, iR, iB, nWidth, kPtIn1 );
	
	kPtOut[0].rX = kPtIn[4].rX = kPtIn1[0].rX;
	kPtOut[0].rY = kPtIn[4].rY = kPtIn1[0].rY;
	kPtOut[1].rX = kPtIn[5].rX = kPtIn1[1].rX;
	kPtOut[1].rY = kPtIn[5].rY = kPtIn1[1].rY;
	kPtIn[6].rX = kPtIn1[2].rX;
	kPtIn[6].rY = kPtIn1[2].rY;
	kPtIn[7].rX = kPtIn1[3].rX;
	kPtIn[7].rY = kPtIn1[3].rY;
	
	atGRPX_DrawRoundCap( lpCanvas, lpPtr, lpPtr2, nWidth, Color);
	
	for( ; i < nCount-2; i++ )
	{ 
		
		kPtIn[0].rX = kPtIn[4].rX;
		kPtIn[0].rY = kPtIn[4].rY;
		kPtIn[1].rX = kPtIn[5].rX;
		kPtIn[1].rY = kPtIn[5].rY;
		kPtIn[2].rX = kPtIn[6].rX;
		kPtIn[2].rY = kPtIn[6].rY;
		kPtIn[3].rX = kPtIn[7].rX;
		kPtIn[3].rY = kPtIn[7].rY;
		
		iL = INTTOREAL(lpPtr2->nX);
		iT = INTTOREAL(lpPtr2->nY);
		iR = INTTOREAL(lpPtr3->nX);
		iB = INTTOREAL(lpPtr3->nY);

		if(iL == iR && iT == iB)
		{
			lpPtr = lpPtr2;	
			lpPtr2 = lpPtr3; 
			lpPtr3++;
			continue;
		}

		
		
		GetPolygonFromWideLine( iL, iT, iR, iB, nWidth, kPtIn2 );
		
		kPtIn[4].rX = kPtIn2[0].rX;
		kPtIn[4].rY = kPtIn2[0].rY;
		kPtIn[5].rX = kPtIn2[1].rX;
		kPtIn[5].rY = kPtIn2[1].rY;
		kPtIn[6].rX = kPtIn2[2].rX;
		kPtIn[6].rY = kPtIn2[2].rY;
		kPtIn[7].rX = kPtIn2[3].rX;
		kPtIn[7].rY = kPtIn2[3].rY;
		
		//점을 구한다.
		jointAngle = GetJointAngle(lpPtr, lpPtr2, lpPtr3);
		if(jointAngle > 135 && jointAngle < 225 )
		{
			// 90~270도
			GetEdgePoints(kPtIn, kPtOut);
			
			//삼각형
			atGRPX_FillTriangleAA( lpCanvas, REALTOINT(kPtIn[0].rX), REALTOINT(kPtIn[0].rY),
				REALTOINT(kPtIn[1].rX), REALTOINT(kPtIn[1].rY),
				REALTOINT(kPtOut[1].rX), REALTOINT(kPtOut[1].rY), Color );
			atGRPX_FillTriangleAA( lpCanvas, REALTOINT(kPtOut[1].rX), REALTOINT(kPtOut[1].rY),
				REALTOINT(kPtOut[0].rX), REALTOINT(kPtOut[0].rY),
	            REALTOINT(kPtIn[0].rX), REALTOINT(kPtIn[0].rY), Color );
			
			// 새로운 중간 값을 다음에 사용을 위해 저장
			kPtIn[4].rX = kPtOut[0].rX;
			kPtIn[4].rY = kPtOut[0].rY;
			kPtIn[5].rX = kPtOut[1].rX;
			kPtIn[5].rY = kPtOut[1].rY;
		}
		else
		{
			atGRPX_FillTriangleAA( lpCanvas, REALTOINT(kPtIn[0].rX), REALTOINT(kPtIn[0].rY),
				REALTOINT(kPtIn[1].rX), REALTOINT(kPtIn[1].rY),
				REALTOINT(kPtIn[2].rX), REALTOINT(kPtIn[2].rY), Color );
			atGRPX_FillTriangleAA( lpCanvas, REALTOINT(kPtIn[2].rX), REALTOINT(kPtIn[2].rY),
				REALTOINT(kPtIn[3].rX), REALTOINT(kPtIn[3].rY),
	            REALTOINT(kPtIn[0].rX), REALTOINT(kPtIn[0].rY), Color );
			// Draw RoundCap
			
			atGRPX_DrawRoundCap( lpCanvas, lpPtr2, lpPtr, nWidth, Color);
			atGRPX_DrawRoundCap( lpCanvas, lpPtr2, lpPtr3, nWidth, Color);
			
		}

		
		
		//다음에 사용할 좌표를 저장.
		lpPtr = lpPtr2;
		lpPtr2 = lpPtr3;
		lpPtr3++;
	}

	// 마지막 삼각형

	atGRPX_FillTriangleAA( lpCanvas, REALTOINT(kPtIn[4].rX), REALTOINT(kPtIn[4].rY),
		REALTOINT(kPtIn[5].rX), REALTOINT(kPtIn[5].rY),
		REALTOINT(kPtIn[6].rX), REALTOINT(kPtIn[6].rY), Color );
	atGRPX_FillTriangleAA( lpCanvas, REALTOINT(kPtIn[6].rX), REALTOINT(kPtIn[6].rY),
		REALTOINT(kPtIn[7].rX), REALTOINT(kPtIn[7].rY),
		REALTOINT(kPtIn[4].rX), REALTOINT(kPtIn[4].rY), Color );
	
	atGRPX_DrawRoundCap( lpCanvas, lpPtr2, lpPtr, nWidth,  Color);	
	
}
