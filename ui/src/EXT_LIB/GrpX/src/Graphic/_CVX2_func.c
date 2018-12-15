#include "_CVX2_func.h"
#include "_ClipPoly_func.h"
#include "_LIN2_func.h"
#include <string.h>

/* Describes the beginning and ending X coordinates of a single horizontal line */
struct HLine {
	atINT XStart;	/* X coordinate of leftmost pixel in line */
	atINT XEnd;		/* X coordinate of rightmost pixel in line */
};

/* Describes a Length-long series of horizontal lines, all assumed to be on contiguous scan lines starting at YStart and proceeding
downward (used to describe a scan-converted polygon to the low-level hardware-dependent drawing code) */
struct HLineList {
	atINT Length;				/* # of horizontal lines */
	atINT YStart;				/* Y coordinate of topmost line */
	// 800X480! 해상도를 키우려면 알맞은 크기로 늘려야 함!
	struct HLine HLinePtr[480];	/* pointer to list of horz lines */
};

/* Advances the index by one vertex forward through the vertex list, wrapping at the end of the list */
#define INDEX_FORWARD(Index, Count)				\
	Index = (Index + 1) % Count;

/* Advances the index by one vertex backward through the vertex list, wrapping at the start of the list */
#define INDEX_BACKWARD(Index, Count)			\
	Index = (Index - 1 + Count) % Count;

/* Advances the index by one vertex either forward or backward through the vertex list, wrapping at either end of the list */
#define INDEX_MOVE(Index, Direction, Count)		\
	if (Direction > 0)							\
		Index = (Index + 1) % Count;			\
	else										\
		Index = (Index - 1 + Count) % Count;

void DrawHorizontalLineList(atGRPX_CANVAS *lpCanvas, struct HLineList * HLineListPtr, atGRPX_COLOR clrColor)
{
	struct HLine *HLinePtr;
	atINT Length, Width;
	atWORD *ScreenPtr;
	atGRPX_PIXEL *P, Pix;

	/* Point to the start of the first scan line on which to draw */
	ScreenPtr = (atWORD*)lpCanvas->lpVideoPtr + HLineListPtr->YStart * lpCanvas->nVMemWidth;

	/* Point to the XStart/XEnd descriptor for the first (top) horizontal line */
	HLinePtr = HLineListPtr->HLinePtr;
	/* Draw each horizontal line in turn, starting with the top one and advancing one line each time */
	Length = HLineListPtr->Length;
	Pix = COLOR_TO_PIXEL(clrColor);
	while (Length-- > 0)
	{
		/* Draw the whole horizontal line if it has a positive width */
		if ((Width = HLinePtr->XEnd - HLinePtr->XStart + 1) > 0)
		{
			P = ScreenPtr + HLinePtr->XStart;
			while(Width-- > 0)
				*P++ = Pix;
		}
		HLinePtr++;							/* point to next scan line X info */
		ScreenPtr += lpCanvas->nVMemWidth;	/* point to next scan line start */
	}
}

void ScanEdge(int X1, int Y1, int X2, int Y2, int SetXStart, int SkipFirst, struct HLine **EdgePointPtr)
{
	atINT DeltaX, Height, Width, AdvanceAmt, ErrorTerm, i;
	atINT ErrorTermAdvance, XMajorAdvanceAmt;
	struct HLine *WorkingEdgePointPtr;

	WorkingEdgePointPtr = *EdgePointPtr; /* avoid double dereference */
	AdvanceAmt = ((DeltaX = X2 - X1) > 0) ? 1 : -1;
	/* direction in which X moves (Y2 is always > Y1, so Y always counts up) */

	if ((Height = Y2 - Y1) <= 0)  /* Y length of the edge */
		return;     /* guard against 0-length and horizontal edges */

	/* Figure out whether the edge is vertical, diagonal, X-major(mostly horizontal), or Y-major (mostly vertical) and handle appropriately */
	if ((Width = ABS(DeltaX)) == 0)
	{
		/* The edge is vertical; special-case by just storing the same X coordinate for every scan line */
		/* Scan the edge for each scan line in turn */
		for (i = Height - SkipFirst; i-- > 0; WorkingEdgePointPtr++)
		{
			/* Store the X coordinate in the appropriate edge list */
			if (SetXStart == 1)
				WorkingEdgePointPtr->XStart = X1;
			else
				WorkingEdgePointPtr->XEnd = X1;
		}
	}
	else if (Width == Height)
	{
		/* The edge is diagonal; special-case by advancing the X coordinate 1 pixel for each scan line */
		if (SkipFirst) /* skip the first point if so indicated */
			X1 += AdvanceAmt; /* move 1 pixel to the left or right */
		/* Scan the edge for each scan line in turn */
		for (i = Height - SkipFirst; i-- > 0; WorkingEdgePointPtr++)
		{
			/* Store the X coordinate in the appropriate edge list */
			if (SetXStart == 1)
				WorkingEdgePointPtr->XStart = X1;
			else
				WorkingEdgePointPtr->XEnd = X1;
			X1 += AdvanceAmt; /* move 1 pixel to the left or right */
		}
	}
	else if (Height > Width)
	{
		/* Edge is closer to vertical than horizontal (Y-major) */
		if (DeltaX >= 0)
			ErrorTerm = 0; /* initial error term going left->right */
		else
			ErrorTerm = -Height + 1;   /* going right->left */
		if (SkipFirst)
		{   /* skip the first point if so indicated */
			/* Determine whether it’s time for the X coord to advance */
			if ((ErrorTerm += Width) > 0)
			{
				X1 += AdvanceAmt; /* move 1 pixel to the left or right */
				ErrorTerm -= Height; /* advance ErrorTerm to next point */
			}
		}
		/* Scan the edge for each scan line in turn */
		for (i = Height - SkipFirst; i-- > 0; WorkingEdgePointPtr++)
		{
			/* Store the X coordinate in the appropriate edge list */
			if (SetXStart == 1)
				WorkingEdgePointPtr->XStart = X1;
			else
				WorkingEdgePointPtr->XEnd = X1;
			/* Determine whether it’s time for the X coord to advance */
			if ((ErrorTerm += Width) > 0)
			{
				X1 += AdvanceAmt; /* move 1 pixel to the left or right */
				ErrorTerm -= Height; /* advance ErrorTerm to correspond */
			}
		}
	}
	else
	{
		/* Edge is closer to horizontal than vertical (X-major) */
		/* Minimum distance to advance X each time */
		XMajorAdvanceAmt = (Width / Height) * AdvanceAmt;
		/* Error term advance for deciding when to advance X 1 extra */
		ErrorTermAdvance = Width % Height;
		if (DeltaX >= 0)
			ErrorTerm = 0; /* initial error term going left->right */
		else
			ErrorTerm = -Height + 1;   /* going right->left */
		if (SkipFirst)
		{   /* skip the first point if so indicated */
			X1 += XMajorAdvanceAmt;    /* move X minimum distance */
			/* Determine whether it’s time for X to advance one extra */
			if ((ErrorTerm += ErrorTermAdvance) > 0)
			{
				X1 += AdvanceAmt;       /* move X one more */
				ErrorTerm -= Height; /* advance ErrorTerm to correspond */
			}
		}
		/* Scan the edge for each scan line in turn */
		for (i = Height - SkipFirst; i-- > 0; WorkingEdgePointPtr++)
		{
			/* Store the X coordinate in the appropriate edge list */
			if (SetXStart == 1)
				WorkingEdgePointPtr->XStart = X1;
			else
				WorkingEdgePointPtr->XEnd = X1;
			X1 += XMajorAdvanceAmt;    /* move X minimum distance */
			/* Determine whether it’s time for X to advance one extra */
			if ((ErrorTerm += ErrorTermAdvance) > 0)
			{
				X1 += AdvanceAmt;       /* move X one more */
				ErrorTerm -= Height; /* advance ErrorTerm to correspond */
			}
		}
	}

	*EdgePointPtr = WorkingEdgePointPtr;   /* advance caller’s ptr */
}

// bCliped : 포인트 배열이 클리핑 된 배열인지 아닌지..
atINT __atGRPX_FillConvexPolygon1(atGRPX_CANVAS *lpCanvas, atPOINT *pVertex, atINT nCount, atGRPX_COLOR clrColor, atBOOL bAA, atBOOL bCliped)
{
	atINT i, MinIndexL, MaxIndex, MinIndexR, SkipFirst, Temp;
	atINT MinPoint_Y, MaxPoint_Y, TopIsFlat, LeftEdgeDir;
	atINT NextIndex, CurrentIndex, PreviousIndex;
	atINT DeltaXN, DeltaYN, DeltaXP, DeltaYP;
	static atPOINT ptOut[MAX_VERTEX];

	static struct HLineList WorkingHLineList;
	struct HLine *EdgePointPtr;
	
	if(!bCliped)
		nCount = __atGRPX_ClipPoly(lpCanvas, pVertex, nCount, atTRUE, ptOut);
	else
		memcpy(ptOut, pVertex, nCount*sizeof(atPOINT));

	/* Scan the list to find the top and bottom of the polygon */
	if (nCount < 3)
		return(1);  /* reject null polygons */

	MaxPoint_Y = MinPoint_Y = ptOut[MinIndexL = MaxIndex = 0].nY;
	for (i = 1; i < nCount; i++)
	{
		if (ptOut[i].nY < MinPoint_Y)
			MinPoint_Y = ptOut[MinIndexL = i].nY; /* new top */
		else if (ptOut[i].nY > MaxPoint_Y)
			MaxPoint_Y = ptOut[MaxIndex = i].nY; /* new bottom */
	}
	if (MinPoint_Y == MaxPoint_Y)
		return(1);  /* polygon is 0-height; avoid infinite loop below */

	/* Scan in ascending order to find the last top-edge point */
	MinIndexR = MinIndexL;
	while (ptOut[MinIndexR].nY == MinPoint_Y)
		INDEX_FORWARD(MinIndexR, nCount);
	INDEX_BACKWARD(MinIndexR, nCount); /* back up to last top-edge point */

	/* Now scan in descending order to find the first top-edge point */
	while (ptOut[MinIndexL].nY == MinPoint_Y)
		INDEX_BACKWARD(MinIndexL, nCount);
	INDEX_FORWARD(MinIndexL, nCount); /* back up to first top-edge point */

	/* Figure out which direction through the vertex list from the top vertex is the left edge and which is the right */
	LeftEdgeDir = -1; /* assume left edge runs down thru vertex list */
	if ((TopIsFlat = (ptOut[MinIndexL].nX != ptOut[MinIndexR].nX) ? 1 : 0) == 1)
	{
		/* If the top is flat, just see which of the ends is leftmost */
		if (ptOut[MinIndexL].nX > ptOut[MinIndexR].nX)
		{
			LeftEdgeDir = 1;  /* left edge runs up through vertex list */
			Temp = MinIndexL;       /* swap the indices so MinIndexL   */
			MinIndexL = MinIndexR;  /* points to the start of the left */
			MinIndexR = Temp;       /* edge, similarly for MinIndexR   */
		}
	}
	else
	{
		/* Point to the downward end of the first line of each of the two edges down from the top */
		NextIndex = MinIndexR;
		INDEX_FORWARD(NextIndex, nCount);
		PreviousIndex = MinIndexL;
		INDEX_BACKWARD(PreviousIndex, nCount);
		/* Calculate X and Y lengths from the top vertex to the end of the first line down each edge;
		use those to compare slopes and see which line is leftmost */
		DeltaXN = ptOut[NextIndex].nX - ptOut[MinIndexL].nX;
		DeltaYN = ptOut[NextIndex].nY - ptOut[MinIndexL].nY;
		DeltaXP = ptOut[PreviousIndex].nX - ptOut[MinIndexL].nX;
		DeltaYP = ptOut[PreviousIndex].nY - ptOut[MinIndexL].nY;
		if (((long)DeltaXN * DeltaYP - (long)DeltaYN * DeltaXP) < 0L)
		{
			LeftEdgeDir = 1;  /* left edge runs up through vertex list */
			Temp = MinIndexL;       /* swap the indices so MinIndexL   */
			MinIndexL = MinIndexR;  /* points to the start of the left */
			MinIndexR = Temp;       /* edge, similarly for MinIndexR   */
		}
	}

	/* Set the # of scan lines in the polygon, skipping the bottom edge and also skipping the top vertex if the top isn’t flat because
	in that case the top vertex has a right edge component, and set the top scan line to draw, which is likewise the second line of
	the polygon unless the top is flat */
	if ((WorkingHLineList.Length = MaxPoint_Y - MinPoint_Y - 1 + TopIsFlat) <= 0)
		return(1);  /* there’s nothing to draw, so we’re done */
	WorkingHLineList.YStart = MinPoint_Y + 1 - TopIsFlat;

	/* Get memory in which to store the line list we generate */
// 	if ((WorkingHLineList.HLinePtr = (struct HLine *) (atGRPX_AllocMem(lpCanvas->lpGrpX, sizeof(struct HLine) * WorkingHLineList.Length))) == NULL)
// 		return(0);  /* couldn’t get memory for the line list */
	memset(WorkingHLineList.HLinePtr, 0, sizeof(struct HLine) * WorkingHLineList.Length);

	/* Scan the left edge and store the boundary points in the list */
	/* Initial pointer for storing scan converted left-edge coords */
	EdgePointPtr = WorkingHLineList.HLinePtr;
	/* Start from the top of the left edge */
	PreviousIndex = CurrentIndex = MinIndexL;
	/* Skip the first point of the first line unless the top is flat;
	if the top isn’t flat, the top vertex is exactly on a right edge and isn’t drawn */
	SkipFirst = TopIsFlat ? 0 : 1;
	/* Scan convert each line in the left edge from top to bottom */
	do {
		INDEX_MOVE(CurrentIndex, LeftEdgeDir, nCount);
		ScanEdge(ptOut[PreviousIndex].nX, ptOut[PreviousIndex].nY,
		ptOut[CurrentIndex].nX, ptOut[CurrentIndex].nY, 1, SkipFirst, &EdgePointPtr);
		PreviousIndex = CurrentIndex;
		SkipFirst = 0; /* scan convert the first point from now on */
	} while (CurrentIndex != MaxIndex);

	/* Scan the right edge and store the boundary points in the list */
	EdgePointPtr = WorkingHLineList.HLinePtr;
	PreviousIndex = CurrentIndex = MinIndexR;
	SkipFirst = TopIsFlat ? 0 : 1;
	/* Scan convert the right edge, top to bottom. X coordinates are adjusted 1 to the left, effectively causing scan conversion of
	the nearest points to the left of but not exactly on the edge */
	do {
		INDEX_MOVE(CurrentIndex, -LeftEdgeDir, nCount);
		ScanEdge(ptOut[PreviousIndex].nX - 1, ptOut[PreviousIndex].nY,
		ptOut[CurrentIndex].nX - 1, ptOut[CurrentIndex].nY, 0, SkipFirst, &EdgePointPtr);
		PreviousIndex = CurrentIndex;
		SkipFirst = 0; /* scan convert the first point from now on */
	} while (CurrentIndex != MaxIndex);

	/* Draw the line list representing the scan converted polygon */
 	DrawHorizontalLineList(lpCanvas, &WorkingHLineList, clrColor);

	if(bAA)
	{
		for(i=0; i<nCount-1; ++i)
			__atGRPX_DrawLine1(lpCanvas, ptOut[i].nX, ptOut[i].nY, ptOut[i+1].nX, ptOut[i+1].nY, clrColor);
		__atGRPX_DrawLine1(lpCanvas, ptOut[i].nX, ptOut[i].nY, ptOut[0].nX, ptOut[0].nY, clrColor);
	}

	/* Release the line list’s memory and we’re successfully done */
// 	atGRPX_FreeMem(lpCanvas->lpGrpX, WorkingHLineList.HLinePtr);

	return(1);
}

void ScanEdge_REAL(atREAL X1, atREAL Y1, atREAL X2, atREAL Y2, atINT SetXStart, atINT SkipFirst, struct HLine **EdgePointPtr)
{
	atINT i;
	atREAL rHeight, rWidth, rSlope, rEx, rEy, rAx/*, rBx*/;
	atINT iAy, iBy;
	struct HLine *WorkingEdgePointPtr;
	
	WorkingEdgePointPtr = *EdgePointPtr; /* avoid double dereference */
	
	if ((rHeight = Y2 - Y1) <= 0)  /* Y length of the edge */
		return;     /* guard against 0-length and horizontal edges */

	rWidth = X2 - X1;
	rSlope = REALDIV(rWidth, rHeight);
	rEy = INTTOREAL(REALTOINT(Y1) + 1) - Y1;
	rEx = REALMUL(rSlope, rEy);

	rAx = X1 + rEx;
	iAy = REALTOINT(Y1) + 1;
	iBy = REALTOINT(Y2);

	if (SetXStart == 1)
	{
		for (i = iAy; i++ <= iBy; WorkingEdgePointPtr++)
		{
			/* Store the X coordinate in the appropriate edge list */
			WorkingEdgePointPtr->XStart = REALTOINT_U(rAx);
			rAx += rSlope;
		}
	}
	else
	{
		for (i = iAy; i++ <= iBy; WorkingEdgePointPtr++)
		{
			/* Store the X coordinate in the appropriate edge list */
			WorkingEdgePointPtr->XEnd = REALTOINT(rAx);
			rAx += rSlope;
		}
	}

	*EdgePointPtr = WorkingEdgePointPtr;   /* advance caller’s ptr */
}

// bCliped : 포인트 배열이 클리핑 된 배열인지 아닌지..
atINT __atGRPX_FillConvexPolygon1_REAL(atGRPX_CANVAS *lpCanvas, atRPOINT *pVertex, atINT nCount, atGRPX_COLOR clrColor, atBOOL bAA, atBOOL bCliped)
{
	atINT i, MinIndexL, MaxIndex, MinIndexR, Temp, LeftEdgeDir;
	atINT NextIndex, CurrentIndex, PreviousIndex;
	atREAL MinPoint_Y, MaxPoint_Y;
	atREAL DeltaXN, DeltaYN, DeltaXP, DeltaYP;
	atINT TopIsFlat, SkipFirst;
	static atRPOINT ptOut[MAX_VERTEX];
	
	static struct HLineList WorkingHLineList;
	struct HLine *EdgePointPtr;
	
	if(!bCliped)
		nCount = __atGRPX_ClipPoly_REAL(lpCanvas, pVertex, nCount, atTRUE, ptOut);
	else
		memcpy(ptOut, pVertex, nCount*sizeof(atPOINT));

	/* Scan the list to find the top and bottom of the polygon */
	if (nCount < 3)
		return(1);  /* reject null polygons */

	MaxPoint_Y = MinPoint_Y = ptOut[MinIndexL = MaxIndex = 0].rY;
	for (i = 1; i < nCount; i++)
	{
		if (ptOut[i].rY < MinPoint_Y)
			MinPoint_Y = ptOut[MinIndexL = i].rY; /* new top */
		else if (ptOut[i].rY > MaxPoint_Y)
			MaxPoint_Y = ptOut[MaxIndex = i].rY; /* new bottom */
	}
	if (MinPoint_Y == MaxPoint_Y)
		return(1);  /* polygon is 0-height; avoid infinite loop below */

	/* Scan in ascending order to find the last top-edge point */
	MinIndexR = MinIndexL;
	while (ptOut[MinIndexR].rY == MinPoint_Y)
		INDEX_FORWARD(MinIndexR, nCount);
	INDEX_BACKWARD(MinIndexR, nCount); /* back up to last top-edge point */

	/* Now scan in descending order to find the first top-edge point */
	while (ptOut[MinIndexL].rY == MinPoint_Y)
		INDEX_BACKWARD(MinIndexL, nCount);
	INDEX_FORWARD(MinIndexL, nCount); /* back up to first top-edge point */

	/* Figure out which direction through the vertex list from the top vertex is the left edge and which is the right */
	LeftEdgeDir = -1; /* assume left edge runs down thru vertex list */
	if ((TopIsFlat = (ptOut[MinIndexL].rX != ptOut[MinIndexR].rX) ? 1 : 0) == 1)
	{
		/* If the top is flat, just see which of the ends is leftmost */
		if (ptOut[MinIndexL].rX > ptOut[MinIndexR].rX)
		{
			LeftEdgeDir = 1;  /* left edge runs up through vertex list */
			Temp = MinIndexL;       /* swap the indices so MinIndexL   */
			MinIndexL = MinIndexR;  /* points to the start of the left */
			MinIndexR = Temp;       /* edge, similarly for MinIndexR   */
		}
	}
	else
	{
		/* Point to the downward end of the first line of each of the two edges down from the top */
		NextIndex = MinIndexR;
		INDEX_FORWARD(NextIndex, nCount);
		PreviousIndex = MinIndexL;
		INDEX_BACKWARD(PreviousIndex, nCount);
		/* Calculate X and Y lengths from the top vertex to the end of the first line down each edge;
		use those to compare slopes and see which line is leftmost */
		DeltaXN = ptOut[NextIndex].rX - ptOut[MinIndexL].rX;
		DeltaYN = ptOut[NextIndex].rY - ptOut[MinIndexL].rY;
		DeltaXP = ptOut[PreviousIndex].rX - ptOut[MinIndexL].rX;
		DeltaYP = ptOut[PreviousIndex].rY - ptOut[MinIndexL].rY;
		if ((REALMUL(DeltaXN, DeltaYP) - REALMUL(DeltaYN, DeltaXP)) < 0)
		{
			LeftEdgeDir = 1;  /* left edge runs up through vertex list */
			Temp = MinIndexL;       /* swap the indices so MinIndexL   */
			MinIndexL = MinIndexR;  /* points to the start of the left */
			MinIndexR = Temp;       /* edge, similarly for MinIndexR   */
		}
	}

	/* Set the # of scan lines in the polygon, skipping the bottom edge and also skipping the top vertex if the top isn’t flat because
	in that case the top vertex has a right edge component, and set the top scan line to draw, which is likewise the second line of
	the polygon unless the top is flat */
	if ((WorkingHLineList.Length = (REALTOINT(MaxPoint_Y) - REALTOINT(MinPoint_Y)+1 - 1 + TopIsFlat)) <= 0)
		return(1);  /* there’s nothing to draw, so we’re done */
	WorkingHLineList.YStart = REALTOINT(MinPoint_Y) + 1 - TopIsFlat;

	/* Get memory in which to store the line list we generate */
// 	if ((WorkingHLineList.HLinePtr = (struct HLine *) (atGRPX_AllocMem(lpCanvas->lpGrpX, sizeof(struct HLine) * WorkingHLineList.Length))) == NULL)
// 		return(0);  /* couldn’t get memory for the line list */
	memset(WorkingHLineList.HLinePtr, 0, sizeof(struct HLine) * WorkingHLineList.Length);

	/* Scan the left edge and store the boundary points in the list */
	/* Initial pointer for storing scan converted left-edge coords */
	EdgePointPtr = WorkingHLineList.HLinePtr;
	/* Start from the top of the left edge */
	PreviousIndex = CurrentIndex = MinIndexL;
	/* Skip the first point of the first line unless the top is flat;
	if the top isn’t flat, the top vertex is exactly on a right edge and isn’t drawn */
	SkipFirst = TopIsFlat ? 0 : 1;
	/* Scan convert each line in the left edge from top to bottom */
	do {
		INDEX_MOVE(CurrentIndex, LeftEdgeDir, nCount);
		ScanEdge_REAL(ptOut[PreviousIndex].rX, ptOut[PreviousIndex].rY,
		ptOut[CurrentIndex].rX, ptOut[CurrentIndex].rY, 1, SkipFirst, &EdgePointPtr);
		PreviousIndex = CurrentIndex;
		SkipFirst = 0; /* scan convert the first point from now on */
	} while (CurrentIndex != MaxIndex);

	/* Scan the right edge and store the boundary points in the list */
	EdgePointPtr = WorkingHLineList.HLinePtr;
	PreviousIndex = CurrentIndex = MinIndexR;
	SkipFirst = TopIsFlat ? 0 : 1;
	/* Scan convert the right edge, top to bottom. X coordinates are adjusted 1 to the left, effectively causing scan conversion of
	the nearest points to the left of but not exactly on the edge */
	do {
		INDEX_MOVE(CurrentIndex, -LeftEdgeDir, nCount);
		ScanEdge_REAL(ptOut[PreviousIndex].rX/* - REAL_ONE*/, ptOut[PreviousIndex].rY,
		ptOut[CurrentIndex].rX/* - REAL_ONE*/, ptOut[CurrentIndex].rY, 0, SkipFirst, &EdgePointPtr);
		PreviousIndex = CurrentIndex;
		SkipFirst = 0; /* scan convert the first point from now on */
	} while (CurrentIndex != MaxIndex);

	/* Draw the line list representing the scan converted polygon */
	DrawHorizontalLineList(lpCanvas, &WorkingHLineList, clrColor);

	if(bAA)
	{
 		for(i=0; i<nCount-1; ++i)
 			__atGRPX_DrawLine1_REAL(lpCanvas, ptOut[i].rX, ptOut[i].rY, ptOut[i+1].rX, ptOut[i+1].rY, clrColor);
 		__atGRPX_DrawLine1_REAL(lpCanvas, ptOut[i].rX, ptOut[i].rY, ptOut[0].rX, ptOut[0].rY, clrColor);
	}

	/* Release the line list’s memory and we’re successfully done */
// 	atGRPX_FreeMem(lpCanvas->lpGrpX, WorkingHLineList.HLinePtr);

	return(1);
}
