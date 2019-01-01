#include "_PLY2_func.h"
#include "_LIN2_func.h"
#include "_CVX2_func.h"
#include <stdlib.h>
#include <string.h>

#define CONVEX    0
#define NONCONVEX 1
#define COMPLEX   2

#define MAX_EDGE_TABLE	10000

struct HLine {
	int XStart;
	int XEnd;
};

struct HLineList {
	int Length;
	int YStart;
	struct HLine * HLinePtr;
};

struct EdgeState {
	struct EdgeState *NextEdge;
	int X;
	int StartY;
	int WholePixelXMove;
	int XDirection;
	int ErrorTerm;
	int ErrorTermAdjUp;
	int ErrorTermAdjDown;
	int Count;
};

static struct EdgeState *g_pGETPtr, *g_pAETPtr;

static void DrawHorizontalLineSeg(atGRPX_CANVAS *lpCanvas, atINT Y, atINT LeftX, atINT RightX, atGRPX_COLOR clrColor)
{
	atINT Width;
	atGRPX_PIXEL *P, Pix;

	/* Draw each pixel in the horizontal line segment, starting with the leftmost one */
	Pix = COLOR_TO_PIXEL(clrColor);
	P = atGRPX_GetFrameBufferPtrXY( lpCanvas, LeftX, Y );
	Width = RightX - LeftX + 1;
	while(Width-- > 0)
	    *P++ = Pix;
}

static void BuildGET(atPOINT *pVertex, atINT nCount, struct EdgeState *pNextFreeEdgeStruc)
{
	int i, StartX, StartY, EndX, EndY, DeltaY, DeltaX, Width, tmp;
	struct EdgeState *pNewEdgePtr;
	struct EdgeState *pFollowingEdge, **ppFollowingEdgeLink;
	atPOINT *pPt;

	/* Scan through the vertex list and put all non-0-height edges into the GET, sorted by increasing Y start coordinate */
	pPt = pVertex;   /* point to the vertex list */
	g_pGETPtr = NULL;    /* initialize the global edge table to empty */
	for (i = 0; i < nCount; i++)
	{
		/* Calculate the edge height and width */
		StartX = pPt[i].nX;
		StartY = pPt[i].nY;
		/* The edge runs from the current point to the previous one */
		if (i == 0)
		{
			/* Wrap back around to the end of the list */
			EndX = pPt[nCount-1].nX;
			EndY = pPt[nCount-1].nY;
		}
		else
		{
			EndX = pPt[i-1].nX;
			EndY = pPt[i-1].nY;
		}
		/* Make sure the edge runs top to bottom */
		if (StartY > EndY)
		{
			SWAP(StartX, EndX, tmp);
			SWAP(StartY, EndY, tmp);
		}
		/* Skip if this can¡¯t ever be an active edge (has 0 height) */
		if ((DeltaY = EndY - StartY) != 0)
		{
			/* Allocate space for this edge¡¯s info, and fill in the
			structure */
			pNewEdgePtr = pNextFreeEdgeStruc++;
			pNewEdgePtr->XDirection =   /* direction in which X moves */
			((DeltaX = EndX - StartX) > 0) ? 1 : -1;
			Width = ABS(DeltaX);
			pNewEdgePtr->X = StartX;
			pNewEdgePtr->StartY = StartY;
			pNewEdgePtr->Count = DeltaY;
			pNewEdgePtr->ErrorTermAdjDown = DeltaY;
			if (DeltaX >= 0)  /* initial error term going L->R */
				pNewEdgePtr->ErrorTerm = 0;
			else              /* initial error term going R->L */
				pNewEdgePtr->ErrorTerm = -DeltaY + 1;
			if (DeltaY >= Width)
			{     /* Y-major edge */
				pNewEdgePtr->WholePixelXMove = 0;
				pNewEdgePtr->ErrorTermAdjUp = Width;
			}
			else
			{                   /* X-major edge */
				pNewEdgePtr->WholePixelXMove = (Width / DeltaY) * pNewEdgePtr->XDirection;
				pNewEdgePtr->ErrorTermAdjUp = Width % DeltaY;
			}
			/* Link the new edge into the GET so that the edge list is still sorted by Y coordinate, and by X coordinate for all
			edges with the same Y coordinate */
			ppFollowingEdgeLink = &g_pGETPtr;
			for (;;)
			{
				pFollowingEdge = *ppFollowingEdgeLink;
				if ((pFollowingEdge == NULL) || (pFollowingEdge->StartY > StartY) || ((pFollowingEdge->StartY == StartY) && (pFollowingEdge->X >= StartX)))
				{
					pNewEdgePtr->NextEdge = pFollowingEdge;
					*ppFollowingEdgeLink = pNewEdgePtr;
					break;
				}
				ppFollowingEdgeLink = &pFollowingEdge->NextEdge;
			}
		}
	}
}

/* Sorts all edges currently in the active edge table into ascending order of current X coordinates */
static void XSortAET()
{
	struct EdgeState *pCurrentEdge, **ppCurrentEdgePtr, *pTempEdge;
	int SwapOccurred;

	/* Scan through the AET and swap any adjacent edges for which the second edge is at a lower current X coord than the first edge.
	Repeat until no further swapping is needed */
	if (g_pAETPtr != NULL)
	{
		do {
			SwapOccurred = 0;
			ppCurrentEdgePtr = &g_pAETPtr;
			while ((pCurrentEdge = *ppCurrentEdgePtr)->NextEdge != NULL)
			{
				if (pCurrentEdge->X > pCurrentEdge->NextEdge->X)
				{
					/* The second edge has a lower X than the first; swap them in the AET */
					pTempEdge = pCurrentEdge->NextEdge->NextEdge;
					*ppCurrentEdgePtr = pCurrentEdge->NextEdge;
					pCurrentEdge->NextEdge->NextEdge = pCurrentEdge;
					pCurrentEdge->NextEdge = pTempEdge;
					SwapOccurred = 1;
				}
				ppCurrentEdgePtr = &(*ppCurrentEdgePtr)->NextEdge;
			}
		} while (SwapOccurred != 0);
	}
}

/* Advances each edge in the AET by one scan line. Removes edges that have been fully scanned. */
static void AdvanceAET()
{
	struct EdgeState *pCurrentEdge, **ppCurrentEdgePtr;

	/* Count down and remove or advance each edge in the AET */
	ppCurrentEdgePtr = &g_pAETPtr;
	while ((pCurrentEdge = *ppCurrentEdgePtr) != NULL)
	{
		/* Count off one scan line for this edge */
		if ((--(pCurrentEdge->Count)) == 0)
		{
			/* This edge is finished, so remove it from the AET */
			*ppCurrentEdgePtr = pCurrentEdge->NextEdge;
		}
		else
		{
			/* Advance the edge¡¯s X coordinate by minimum move */
			pCurrentEdge->X += pCurrentEdge->WholePixelXMove;
			/* Determine whether it¡¯s time for X to advance one extra */
			if ((pCurrentEdge->ErrorTerm += pCurrentEdge->ErrorTermAdjUp) > 0)
			{
				pCurrentEdge->X += pCurrentEdge->XDirection;
				pCurrentEdge->ErrorTerm -= pCurrentEdge->ErrorTermAdjDown;
			}
			ppCurrentEdgePtr = &pCurrentEdge->NextEdge;
		}
	}
}

/* Moves all edges that start at the specified Y coordinate from the GET to the AET, maintaining the X sorting of the AET. */
static void MoveXSortedToAET(int YToMove)
{
	struct EdgeState *pAETEdge, **ppAETEdgePtr, *pTempEdge;
	int CurrentX;

	/* The GET is Y sorted. Any edges that start at the desired Y
	coordinate will be first in the GET, so we¡¯ll move edges from
	the GET to AET until the first edge left in the GET is no longer
	at the desired Y coordinate. Also, the GET is X sorted within
	each Y coordinate, so each successive edge we add to the AET is
	guaranteed to belong later in the AET than the one just added. */
	ppAETEdgePtr = &g_pAETPtr;
	while ((g_pGETPtr != NULL) && (g_pGETPtr->StartY == YToMove))
	{
		CurrentX = g_pGETPtr->X;
		/* Link the new edge into the AET so that the AET is still
		sorted by X coordinate */
		for (;;)
		{
			pAETEdge = *ppAETEdgePtr;
			if ((pAETEdge == NULL) || (pAETEdge->X >= CurrentX))
			{
				pTempEdge = g_pGETPtr->NextEdge;
				*ppAETEdgePtr = g_pGETPtr;  /* link the edge into the AET */
				g_pGETPtr->NextEdge = pAETEdge;
				ppAETEdgePtr = &g_pGETPtr->NextEdge;
				g_pGETPtr = pTempEdge;   /* unlink the edge from the GET */
				break;
			}
			else
			{
				ppAETEdgePtr = &pAETEdge->NextEdge;
			}
		}
	}
}

/* Fills the scan line described by the current AET at the specified Y coordinate in the specified color, using the odd/even fill rule */
static void ScanOutAET(atGRPX_CANVAS *lpCanvas, int YToScan, int Color)
{
	int LeftX;
	struct EdgeState *CurrentEdge;

	/* Scan through the AET, drawing line segments as each pair of edge
	crossings is encountered. The nearest pixel on or to the right
	of left edges is drawn, and the nearest pixel to the left of but
	not on right edges is drawn */
	CurrentEdge = g_pAETPtr;
	while (CurrentEdge != NULL)
	{
		LeftX = CurrentEdge->X;
		CurrentEdge = CurrentEdge->NextEdge;
		DrawHorizontalLineSeg(lpCanvas, YToScan, LeftX, CurrentEdge->X-1, Color);
		CurrentEdge = CurrentEdge->NextEdge;
	}
}

static atBOOL IsConvexPolygon(atPOINT *pVertex, atINT nCount)
{
	atINT i, nCross;

	if (nCount < 3) return atFALSE;

	for(i=0; i<nCount-2; ++i)
	{
		nCross = pVertex[i+1].nX * pVertex[i+2].nY - pVertex[i].nY * pVertex[i+1].nX - pVertex[i].nX * pVertex[i+2].nY
			- pVertex[i+1].nY * pVertex[i+2].nX + pVertex[i].nX * pVertex[i+1].nY + pVertex[i].nY * pVertex[i+2].nX;
		if(nCross < 0) return atFALSE;
	}

	nCross = pVertex[nCount-2].nX * pVertex[nCount-1].nY - pVertex[0].nY * pVertex[nCount-2].nX - pVertex[0].nX * pVertex[nCount-1].nY
		- pVertex[nCount-2].nY * pVertex[nCount-1].nX + pVertex[0].nX * pVertex[nCount-2].nY + pVertex[0].nY * pVertex[nCount-1].nX;
	if(nCross < 0) return atFALSE;

	nCross = pVertex[nCount-1].nX * pVertex[0].nY - pVertex[1].nY * pVertex[nCount-1].nX - pVertex[1].nX * pVertex[0].nY
		- pVertex[nCount-1].nY * pVertex[0].nX + pVertex[1].nX * pVertex[nCount-1].nY + pVertex[1].nY * pVertex[0].nX;
	if(nCross < 0) return atFALSE;

	return atTRUE;
}

atINT __atGRPX_FillComplexPolygon( atGRPX_CANVAS *lpCanvas, atPOINT *pVertex, atINT nCount, atGRPX_COLOR clrColor, atBOOL bAA )
{
//	struct EdgeState *pEdgeTableBuffer;
	static struct EdgeState pEdgeTableBuffer[MAX_EDGE_TABLE];
	int i, CurrentY;

	if (IsConvexPolygon(pVertex, nCount))
		return(__atGRPX_FillConvexPolygon1(lpCanvas, pVertex, nCount, clrColor, bAA, atTRUE));

	/* It takes a minimum of 3 vertices to cause any pixels to be drawn; reject polygons that are guaranteed to be invisible */
	if (nCount < 3)
		return(1);

	/* Get enough memory to store the entire edge table */
//	if ((pEdgeTableBuffer = (struct EdgeState *) (atGRPX_AllocMem(lpCanvas->lpGrpX, sizeof(struct EdgeState) * nCount))) == NULL)
//		return(0);  /* couldn¡¯¡¯t get memory for the edge table */
	memset(pEdgeTableBuffer, 0, sizeof(struct EdgeState) * nCount);

	/* Build the global edge table */
	BuildGET(pVertex, nCount, pEdgeTableBuffer);
	/* Scan down through the polygon edges, one scan line at a time, so long as at least one edge remains in either the GET or AET */
	g_pAETPtr = NULL;    /* initialize the active edge table to empty */
	CurrentY = g_pGETPtr->StartY; /* start at the top polygon vertex */
	while ((g_pGETPtr != NULL) || (g_pAETPtr != NULL))
	{
		MoveXSortedToAET(CurrentY);  /* update AET for this scan line */
		ScanOutAET(lpCanvas, CurrentY, clrColor); /* draw this scan line from AET */
		AdvanceAET();                /* advance AET edges 1 scan line */
		XSortAET();                  /* resort on X */
		CurrentY++;                  /* advance to the next scan line */
	}
	
	if(bAA)
	{
		for(i=0; i<nCount-1; ++i)
			__atGRPX_DrawLine1(lpCanvas, pVertex[i].nX, pVertex[i].nY, pVertex[i+1].nX, pVertex[i+1].nY, clrColor);
		__atGRPX_DrawLine1(lpCanvas, pVertex[i].nX, pVertex[i].nY, pVertex[0].nX, pVertex[0].nY, clrColor);
	}

	/* Release the memory we¡¯ve allocated and we¡¯re done */
//	atGRPX_FreeMem(lpCanvas->lpGrpX, pEdgeTableBuffer);
	return(1);
}
