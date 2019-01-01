#include "_ClipPoly_func.h"
#include <string.h>

typedef struct tagEDGEPROP {
	atBYTE Edge;
	atINT Max;
	atINT Count;
	atDWORD State;
} EDGEPROP;

typedef struct tagEDGEPROP_R {
	atBYTE Edge;
	atREAL Max;
	atINT Count;
	atDWORD State;
} EDGEPROP_R;

#define CLOSED		0x0001		// 폴리라인 폐합여부
#define CLIPLEFT	0x0400
#define CLIPTOP		0x0800
#define CLIPRIGHT	0x1000
#define CLIPBOTTOM	0x2000

atINT ClipEdge(atRECT *pClip, EDGEPROP* pEdge, atPOINT *pIn, atPOINT *pOut)
{
	atINT i, iCount, iStartX, iStartY, iEndX, iEndY, iResult;
	atBOOL b1, b2, b3, b4, bClipStart, bClipEnd;

	iCount = pEdge->Count;
	if(pEdge->State & CLOSED)
		pIn[iCount] = pIn[0];
	else
		pIn[iCount] = pIn[iCount-1];
	
	iStartX = pIn[0].nX;
	iStartY = pIn[0].nY;
	
	b1 = iStartY < 0;
	b2 = iStartY > pClip->nEndY;
	b3 = iStartX < 0;
	b4 = iStartX > pClip->nEndX;
	bClipStart = (b1 | (b2<<1) | (b3<<2) | (b4<<3)) & pEdge->Edge;
	
	iResult = 0;
	for(i=0; i<iCount; i++)
	{
		iEndX = pIn[i+1].nX;
		iEndY = pIn[i+1].nY;

		if(iStartY==iEndY || iStartX==iEndX) continue;
		
		b1 = iEndY < 0;
		b2 = iEndY > pClip->nEndY;
		b3 = iEndX < 0;
		b4 = iEndX > pClip->nEndX;
		bClipEnd = (b1 | (b2<<1) | (b3<<2) | (b4<<3)) & pEdge->Edge;
		
		if(!bClipStart)			// the point is visible
		{
			pOut[iResult].nX = iStartX;
			pOut[iResult].nY = iStartY;
			iResult++;
		}
		
		if(bClipStart^bClipEnd)	// Line crosses edge
		{
			if(pEdge->Edge & 3)
			{
				pOut[iResult].nX = iEndX + (pEdge->Max - iEndY) * (iStartX - iEndX) / (iStartY - iEndY);
				pOut[iResult].nY = pEdge->Max;
			}
			else
			{
				pOut[iResult].nX = pEdge->Max;
				pOut[iResult].nY = iEndY + (pEdge->Max - iEndX) * (iStartY - iEndY) / (iStartX - iEndX);
			}
			iResult++;
		}
		
		bClipStart = bClipEnd;
		iStartX = iEndX;
		iStartY = iEndY;
	}
	
	memcpy(pIn, pOut, iResult*sizeof(atPOINT));
	
	return iResult;
}

atINT __atGRPX_ClipPoly(atGRPX_CANVAS *lpCanvas, atPOINT *pVertex, atINT nCount, atBOOL bClosed, atPOINT *pOut)
{
	static atPOINT ptIn[MAX_VERTEX];
	atRECT *pClip;
	atINT i, iMinX, iMaxX, iMinY, iMaxY, iPenWidth;
	EDGEPROP Edge;

	if(nCount<2)
		return nCount;

    iMaxY = iMinY = pVertex[0].nY;
    iMaxX = iMinX = pVertex[0].nX;
	
    // 가장 작은 y값과 인덱스를 얻는다.
	for( i = 1; i < nCount; i++ ){
		if( pVertex[i].nY < iMinY )	iMinY = pVertex[i].nY;
		if( pVertex[i].nY > iMaxY ) iMaxY = pVertex[i].nY;
		if( pVertex[i].nX < iMinX ) iMinX = pVertex[i].nX;
		if( pVertex[i].nX > iMaxX ) iMaxX = pVertex[i].nX;
	}
	
	pClip = atGRPX_GetClipRect(lpCanvas);

    // 완전히 벗어나면 종료
    if( (iMinY > pClip->nEndY) || (iMaxY < pClip->nStartY) )
		return 0;
    if( (iMinX > pClip->nEndX) || (iMaxX < pClip->nStartX) )
		return 0;

	memcpy(ptIn, pVertex, nCount*sizeof(atPOINT));

	iPenWidth = 0;
	
	Edge.State = bClosed ? CLOSED : 0;

	// Clip to Top edge
	if(nCount>1)
	{
		Edge.Edge = 1;
		Edge.Max = -iPenWidth;
		Edge.Count = nCount;
		nCount = ClipEdge(pClip, &Edge, ptIn, pOut);
	}
	else
		return nCount;

	// Clip to Right Edge
	if(nCount>1)
	{
		Edge.Edge = 8;
		Edge.Max = pClip->nEndX+iPenWidth;
		Edge.Count = nCount;
		nCount = ClipEdge(pClip, &Edge, ptIn, pOut);
	}
	else
		return nCount;

	// Clip to Bottom edge
	if(nCount>1)
	{
		Edge.Edge = 2;
		Edge.Max = pClip->nEndY+iPenWidth;
		Edge.Count = nCount;
		nCount = ClipEdge(pClip, &Edge, ptIn, pOut);
	}
	else
		return nCount;

	// Clip to Left Edge
	if(nCount>1)
	{
		Edge.Edge = 4;
		Edge.Max = -iPenWidth;
		Edge.Count = nCount;
		nCount = ClipEdge(pClip, &Edge, ptIn, pOut);
	}

	return nCount;
}

atINT ClipEdge_REAL(atRPOINT *pClip, EDGEPROP_R* pEdge, atRPOINT *pIn, atRPOINT *pOut)
{
	atINT i, iCount, iResult;
	atREAL rStartX, rStartY, rEndX, rEndY;
	atBOOL b1, b2, b3, b4, bClipStart, bClipEnd;

	iCount = pEdge->Count;
	if(pEdge->State & CLOSED)
		pIn[iCount] = pIn[0];
	else
		pIn[iCount] = pIn[iCount-1];
	
	rStartX = pIn[0].rX;
	rStartY = pIn[0].rY;
	
	b1 = rStartY < 0;
	b2 = rStartY > pClip->rY;
	b3 = rStartX < 0;
	b4 = rStartX > pClip->rX;
	bClipStart = (b1 | (b2<<1) | (b3<<2) | (b4<<3)) & pEdge->Edge;
	
	iResult = 0;
	for(i=0; i<iCount; i++)
	{
		rEndX = pIn[i+1].rX;
		rEndY = pIn[i+1].rY;
		
		b1 = rEndY < 0;
		b2 = rEndY > pClip->rY;
		b3 = rEndX < 0;
		b4 = rEndX > pClip->rX;
		bClipEnd = (b1 | (b2<<1) | (b3<<2) | (b4<<3)) & pEdge->Edge;
		
		if(!bClipStart)			// the point is visible
		{
			pOut[iResult].rX = rStartX;
			pOut[iResult].rY = rStartY;
			iResult++;
		}
		
		if(bClipStart^bClipEnd)	// Line crosses edge
		{
			if(pEdge->Edge & 3)
			{
				pOut[iResult].rX = rEndX + REALMULDIV(pEdge->Max - rEndY, rStartX - rEndX, rStartY - rEndY);
				pOut[iResult].rY = pEdge->Max;
			}
			else
			{
				pOut[iResult].rX = pEdge->Max;
				pOut[iResult].rY = rEndY + REALMULDIV(pEdge->Max - rEndX, rStartY - rEndY, rStartX - rEndX);
			}
			iResult++;
		}
		
		bClipStart = bClipEnd;
		rStartX = rEndX;
		rStartY = rEndY;
	}
	
	memcpy(pIn, pOut, iResult*sizeof(atRPOINT));
	
	return iResult;
}

atINT __atGRPX_ClipPoly_REAL(atGRPX_CANVAS *lpCanvas, atRPOINT *pVertex, atINT nCount, atBOOL bClosed, atRPOINT *pOut)
{
	static atRPOINT ptIn[MAX_VERTEX];
	atRECT *pClip;
	atINT i;
	atREAL rMinX, rMaxX, rMinY, rMaxY;
	atRPOINT rClip;
	atREAL rPenWidth;
	EDGEPROP_R Edge;

	if(nCount<2)
		return nCount;
	
    rMaxY = rMinY = pVertex[0].rY;
    rMaxX = rMinX = pVertex[0].rX;
	
    // 가장 작은 y값과 인덱스를 얻는다.
	for( i = 1; i < nCount; i++ ){
		if( pVertex[i].rY < rMinY )	rMinY = pVertex[i].rY;
		if( pVertex[i].rY > rMaxY ) rMaxY = pVertex[i].rY;
		if( pVertex[i].rX < rMinX ) rMinX = pVertex[i].rX;
		if( pVertex[i].rX > rMaxX ) rMaxX = pVertex[i].rX;
	}
	
	pClip = atGRPX_GetClipRect(lpCanvas);
	
    // 완전히 벗어나면 종료
    if( (rMinY > INTTOREAL(pClip->nEndY)) || (rMaxY < INTTOREAL(pClip->nStartY)) ||
		(rMinX > INTTOREAL(pClip->nEndX)) || (rMaxX < INTTOREAL(pClip->nStartX)) )
		return 0;

	memcpy(ptIn, pVertex, nCount*sizeof(atPOINT));

	rClip.rX = INTTOREAL(pClip->nEndX);
	rClip.rY = INTTOREAL(pClip->nEndY);
	rPenWidth = 0;

	Edge.State = bClosed ? CLOSED : 0;

	// Clip to Top edge
	if(nCount>1)
	{
		Edge.Edge = 1;
		Edge.Max = -rPenWidth;
		Edge.Count = nCount;
		nCount = ClipEdge_REAL(&rClip, &Edge, ptIn, pOut);
	}
	else
		return nCount;

	// Clip to Right Edge
	if(nCount>1)
	{
		Edge.Edge = 8;
		Edge.Max = rClip.rX + rPenWidth;
		Edge.Count = nCount;
		nCount = ClipEdge_REAL(&rClip, &Edge, ptIn, pOut);
	}
	else
		return nCount;

	// Clip to Bottom edge
	if(nCount>1)
	{
		Edge.Edge = 2;
		Edge.Max = rClip.rY + rPenWidth;
		Edge.Count = nCount;
		nCount = ClipEdge_REAL(&rClip, &Edge, ptIn, pOut);
	}
	else
		return nCount;

	// Clip to Left Edge
	if(nCount>1)
	{
		Edge.Edge = 4;
		Edge.Max = -rPenWidth;
		Edge.Count = nCount;
		nCount = ClipEdge_REAL(&rClip, &Edge, ptIn, pOut);
	}

	return nCount;
}
