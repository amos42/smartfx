/*###########################################################################
-----------------------------------------------------------------------------

                                _PLY_func.c
                         (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : 폴리곤 Base 루틴

-----------------------------------------------------------------------------
###########################################################################*/

#include "GrpX.h"
#include "_PLY_func.h"
#include <stdlib.h>
#include <string.h>


#define EDGE_ATTR_NORMAL 0
#define EDGE_ATTR_CLIPPED_LEFT   1
#define EDGE_ATTR_CLIPPED_RIGHT 2

// 엣지 엔트리
typedef struct tagEDGE_ENTRY {
    atSHORT   active;               // 엣지 풀을 위해 존재.
    atU16        attr;               // 엣지 풀을 위해 존재.

    atLONG   y_upper;              // 다음 도달할 Y 좌표
    atREAL    x_inter;              // 현재 X 교차점
    atREAL    dx_per_scan;          // 스캔라인당 X 변화량

    struct tagEDGE_ENTRY *next;     // 연결된 다음 엣지 포인터
} EDGE_ENTRY, *LPEDGE_ENTRY;

// 스캔 진행중인 활성화된 엣지 정보
typedef struct tagACTIVE_EDGES {
    int         EdgeCount;
    EDGE_ENTRY *lpEdges;
} ACTIVE_EDGES;


// 엣지 풀로부터 엣지를 할당 받는다.
static EDGE_ENTRY *SPF_findFreeEdge( EDGE_ENTRY *edge_pool, int max_cnt )
{
    int i;
    EDGE_ENTRY *edge = atNULL, *p = edge_pool;

    for( i = 0; i < max_cnt; i++ ){
      if( p->active == atFALSE ){
        p->active = atTRUE;
        edge = p;
        break;
      }
      p++;
    }

    return edge;
}

// 엣지를 해제한다.
static void SPF_freeEdge( EDGE_ENTRY *edge )
{
    edge->active = atFALSE;
}

// 엣지를 모두 해제한다.
static void SPF_freeAllEdge( EDGE_ENTRY *edge_pool, int max_cnt )
{
    int i;
    EDGE_ENTRY *p = edge_pool;

    for( i = 0; i < max_cnt; i++ ){
      p->active = atFALSE;
      p++;
    }
}

// 외각선 하나를 추가. 추가 과정에서 X 정렬을 한다.
static int SPF_insertEdge( EDGE_ENTRY ** scan_edge, EDGE_ENTRY * new_edge )
{
    EDGE_ENTRY *p, *pre_p;

    pre_p = *scan_edge;

    // 만약 해당 엔트리가 비었으면 시작 엔트리를 교체.
    if( pre_p == atNULL ){
      new_edge->next = atNULL;
      *scan_edge = new_edge;
      return 1;
    }

    // X 좌표 기준으로 새로운 엣지가 들어갈 위치를 찾는다.
    p = pre_p;
    while( p != atNULL ){
        if (new_edge->x_inter <= p->x_inter){
            // 위치가 정확히 겹치는 점일 경우, 좌우를 판단해야 한다.
            if( (new_edge->x_inter == p->x_inter) ){
                if(new_edge->dx_per_scan > p->dx_per_scan){
                  pre_p = p;
                  p = p->next;
                }
            }
            break;
        }
        pre_p = p;
        p = p->next;
    }


    // 해당 위치에 엣지를 삽입
    if( p == *scan_edge ){
        new_edge->next = p;
        *scan_edge = new_edge;
    } else {
        new_edge->next = pre_p->next;
        pre_p->next = new_edge;
    }

    return 1;
}

// 현재 엣지 리스트에 연결된 엣지의 갯수를 얻는다.
static int SPF_getEdgeCount( EDGE_ENTRY * edge )
{
    int cnt = 0;
    
    while( edge != atNULL ){
        cnt++;
        edge = edge->next;
    }

    return cnt;
}

// 외각선 정보에 Y 좌표와 기울기를 세트한다.
// 클리핑을 적용하여 유효한 외각선만 포함하며, 이 경우 클리핑 적용된 보정된 좌표로 세트한다.
static void SPF_makeEdgeRec( EDGE_ENTRY *scan_edges[], EDGE_ENTRY * edge, atPOINT *lower, atPOINT *upper, int yComp, atRECT *clip, atDWORD dwAttr )
{
    atLONG lower_y = lower->nY;

    // 속성을 세트한다. (클리핑 된 엣지인가)
    edge->attr = (atU16)dwAttr;
	
    // 기울기를 세트한다.
    edge->dx_per_scan = INTTOREAL(upper->nX - lower->nX) / (upper->nY - lower_y);

    // 시작 Y 좌표만 클리핑 한다.
    if( lower_y >= clip->nStartY )
      edge->x_inter = INTTOREAL(lower->nX);
    else {
      edge->x_inter = INTTOREAL(lower->nX) + edge->dx_per_scan * (clip->nStartY - lower_y);
      lower_y = clip->nStartY;
    }

    // 기준 Y 값보다 작으면 끝나는 좌표를 한칸 작게.
//    if (upper->nY < yComp)
//        edge->y_upper = upper->nY - 1;
//    else
//        edge->y_upper = upper->nY;
edge->y_upper = upper->nY - 1;  // 홀 폴리곤에 수평선 있을 시 한줄 빼먹는 버그. 원인 파악을 위해 임시로 무조건 -1 해서 넣는다.

    // 뒤에 이어지는 엣지 없음.
    edge->next = atNULL;

    // 시작 스캔 라인 리스트에 엣지를 추가
    SPF_insertEdge( &scan_edges[lower_y], edge );
}

// 외각선 정보에 Y 좌표와 기울기를 세트한다.
// 클리핑을 적용하여 유효한 외각선만 포함하며, 이 경우 클리핑 적용된 보정된 좌표로 세트한다.
static void SPF_makeEdgeRec_REAL( EDGE_ENTRY *scan_edges[], EDGE_ENTRY * edge, atRPOINT *lower, atRPOINT *upper, atREAL yComp, atRECT *clip, atDWORD dwAttr )
{
    atREAL lower_y = lower->rY;

    // 속성을 세트한다. (클리핑 된 엣지인가)
    edge->attr = (atU16)dwAttr;
	
    // 기울기를 세트한다.
    edge->dx_per_scan = REALDIV( (upper->rX - lower->rX), (upper->rY - lower_y) );

    // 시작 Y 좌표만 클리핑 한다.
    if( REALTOINT(lower_y) >= clip->nStartY )
      edge->x_inter = lower->rX;
    else {
      edge->x_inter = lower->rX + REALMUL( edge->dx_per_scan, (INTTOREAL(clip->nStartY) - lower_y) );
      lower_y = INTTOREAL(clip->nStartY);
    }

    // 기준 Y 값보다 작으면 끝나는 좌표를 한칸 작게.
//    if (upper->rY < yComp)
//        edge->y_upper = REALTOINT(upper->rY);
//    else
//        edge->y_upper = REALTOINT_U(upper->rY);
edge->y_upper = REALTOINT(upper->rY)-1;  // 홀 폴리곤에 수평선 있을 시 한줄 빼먹는 버그. 원인 파악을 위해 임시로 무조건 -1 해서 넣는다.

    // 뒤에 이어지는 엣지 없음.
    edge->next = atNULL;

    // 시작 스캔 라인 리스트에 엣지를 추가
    SPF_insertEdge( &scan_edges[REALTOINT(lower_y)], edge );
}

// 주어진 인덱스의 바로 다음 Y 좌표를 찾아낸다.
static int SPF_FindNextY( int k, int cnt, atPOINT * pts )
{
    int j = 0;
	
    if ((k+1) > (cnt-1))
        j = 0;
    else
        j = k + 1;
    while (pts[k].nY == pts[j].nY)
        if ((j+1) > (cnt-1))
            j = 0;
        else
            j++;

//    return (pts[j].nY);
    return j;
}

// 주어진 인덱스의 바로 다음 Y 좌표를 찾아낸다.
static atREAL SPF_FindNextY_REAL( int k, int cnt, atRPOINT * pts )
{
    int j;
    if ((k+1) > (cnt-1))
        j = 0;
    else
        j = k + 1;
    while (pts[k].rY <= pts[j].rY)
        if ((j+1) > (cnt-1))
            j = 0;
        else
            j++;

    return (pts[j].rY);
}

// 폴리곤 포인트 리스트를 바탕으로, 외각선 리스트를 생성해 낸다.
// (Hole을 포함한 Poly-Polygon의 경우에도 이를 반복 호출해 주기만 하면 된다.)
static void SPF_buildEdgeList( EDGE_ENTRY * edge_pool, int max_cnt, EDGE_ENTRY * scan_edges[], atPOINT pts[], int cnt, atRECT *clip, int *pmin_x, int *pmax_x, int *pmin_y, int *pmax_y, atLPGRPX_FUNC_COORD_CONV lpCnvFunc, atLPVOID lpParam )
{
    EDGE_ENTRY * edge;
    atPOINT v1, v2, v3;
    int i, j, yPrev;
    atBOOL isAdd;
    int min_x, max_x, min_y, max_y;
    atDWORD dwClipped;

    // 만약 끝의 점이 처음 점과 일치하면 제외
    if( (pts[0].nX == pts[cnt-1].nX) && (pts[0].nY == pts[cnt-1].nY) ) cnt--;

    if( lpCnvFunc ){
      	lpCnvFunc( lpParam, &v3.nX, &v3.nY, pts[cnt-2].nX, pts[cnt-2].nY );
	yPrev = v3.nY;
    } else
    	yPrev = pts[cnt - 2].nY;

    if( lpCnvFunc )
      	lpCnvFunc( lpParam, &v1.nX, &v1.nY, pts[cnt-1].nX, pts[cnt-1].nY );
    else
      v1 = pts[cnt-1];

	min_x = *pmin_x;
	min_y = *pmin_y;
	max_x = *pmax_x;
	max_y = *pmax_y;

    for( i = 0; i < cnt; i++ ){
	 if( lpCnvFunc )
        	lpCnvFunc( lpParam, &v2.nX, &v2.nY, pts[i].nX, pts[i].nY );
        else
        	v2 = pts[i];
	// 점의 위치가 같으면 무시
	if( (v1.nX == v2.nX) && (v1.nY == v2.nY) ) continue;

        // 클리핑 영역을 완전히 벗어난 외각선은 아예 제외. (Y측 클리핑만)
        isAdd = atTRUE;
	 dwClipped = 0;
        if( (v1.nY < clip->nStartY) && (v2.nY < clip->nStartY) ) isAdd = atFALSE;
        else if( (v1.nY > clip->nEndY) && (v2.nY > clip->nEndY) ) isAdd = atFALSE;
/*
        // X클리핑 영역을 완전히 벗어난 외각선은 외각선에 붙도록
        if( (v1.nX < clip->nStartX) && (v2.nX < clip->nStartX) ){
		v1.nX = v2.nX = clip->nStartX;
		dwClipped = EDGE_ATTR_CLIPPED_RIGHT;
        } else if( (v1.nX > clip->nEndX) && (v2.nX > clip->nEndX) ){
		v1.nX = v2.nX = clip->nEndX;
		dwClipped = EDGE_ATTR_CLIPPED_LEFT;
        }
*/

        if( isAdd ){
           if (v1.nY != v2.nY) { // 수평 위치가 같은 것은 제외.
              edge = SPF_findFreeEdge(edge_pool, max_cnt);
			  if( edge == atNULL ) break;

              if (v1.nY < v2.nY){
                  // 상승 엣지
                  j = SPF_FindNextY(i, cnt, pts);
		    if( lpCnvFunc ){
	        	lpCnvFunc( lpParam, &v3.nX, &v3.nY, pts[j].nX, pts[j].nY );
			yPrev = v3.nY;
	           } else
			yPrev = pts[j].nY;
                  SPF_makeEdgeRec ( scan_edges, edge, &v1, &v2, yPrev, clip, dwClipped );
              } else {
                  // 하강 엣지
                  SPF_makeEdgeRec ( scan_edges, edge, &v2, &v1, yPrev, clip, dwClipped );
              }

              // 최대 최소값 갱신
              if( v2.nX < min_x ) min_x = v2.nX;
              if( v2.nX > max_x ) max_x = v2.nX;
              if( v2.nY < min_y ) min_y = v2.nY;
              if( v2.nY > max_y ) max_y = v2.nY;
           }
        }

        yPrev = v1.nY;
        v1 = v2;
    }

    if( pmin_x ) *pmin_x = min_x;
    if( pmax_x ) *pmax_x = max_x;
    if( pmin_y ) *pmin_y = min_y;
    if( pmax_y ) *pmax_y = max_y;
}

// 폴리곤 포인트 리스트를 바탕으로, 외각선 리스트를 생성해 낸다.
// (Hole을 포함한 Poly-Polygon의 경우에도 이를 반복 호출해 주기만 하면 된다.)
static void SPF_buildEdgeList_REAL( EDGE_ENTRY * edge_pool, int max_cnt, EDGE_ENTRY * scan_edges[], atRPOINT pts[], int cnt, atRECT *clip, atREAL *pmin_x, atREAL *pmax_x, atREAL *pmin_y, atREAL *pmax_y )
{
    EDGE_ENTRY * edge;
    atRPOINT v1, v2;
    int i, yPrev;
    atBOOL isAdd;
    atREAL min_x, max_x, min_y, max_y;
    atDWORD dwClipped;

    // 만약 끝의 점이 처음 점과 일치하면 제외
    if( (pts[0].rX == pts[cnt-1].rX) && (pts[0].rY == pts[cnt-1].rY) ) cnt--;

    yPrev = pts[cnt - 2].rY;

    v1 = pts[cnt-1];

	min_x = *pmin_x;
	min_y = *pmin_y;
	max_x = *pmax_x;
	max_y = *pmax_y;

    for( i = 0; i < cnt; i++ ){
        v2 = pts[i];
	// 점의 위치가 같으면 무시
	if( (v1.rX == v2.rX) && (v1.rY == v2.rY) ) continue;

        // 클리핑 영역을 완전히 벗어난 외각선은 아예 제외. (Y측 클리핑만)
        isAdd = atTRUE;
	dwClipped = 0;
        if( (REALTOINT(v1.rY) < clip->nStartY) && (REALTOINT(v2.rY) < clip->nStartY) ) isAdd = atFALSE;
        else if( (REALTOINT(v1.rY) > clip->nEndY) && (REALTOINT(v2.rY) > clip->nEndY) ) isAdd = atFALSE;
/*
        // X클리핑 영역을 완전히 벗어난 외각선은 외각선에 붙도록
        if( (REALTOINT(v1.rX) < clip->nStartX) && (REALTOINT(v2.rX) < clip->nStartX) ){
		v1.rX = v2.rX = INTTOREAL(clip->nStartX);
		dwClipped = EDGE_ATTR_CLIPPED_RIGHT;
        } else if( (REALTOINT(v1.rX) > clip->nEndX) && (REALTOINT(v2.rX) > clip->nEndX) ){
		v1.rX = v2.rX = INTTOREAL(clip->nEndX);
		dwClipped = EDGE_ATTR_CLIPPED_LEFT;
        }
*/

        if( isAdd ){
           if (v1.rY != v2.rY) { // 수평 위치가 같은 것은 제외.
              edge = SPF_findFreeEdge(edge_pool, max_cnt);
			  if( edge == atNULL ) break;

              if (v1.rY < v2.rY){
                  // 상승 엣지
                  SPF_makeEdgeRec_REAL ( scan_edges, edge, &v1, &v2, SPF_FindNextY_REAL(i, cnt, pts), clip, dwClipped );
              } else {
                  // 하강 엣지
                  SPF_makeEdgeRec_REAL ( scan_edges, edge, &v2, &v1, yPrev, clip, dwClipped );
              }

              // 최대 최소값 갱신
              if( v2.rX < min_x ) min_x = v2.rX;
              if( v2.rX > max_x ) max_x = v2.rX;
              if( v2.rY < min_y ) min_y = v2.rY;
              if( v2.rY > max_y ) max_y = v2.rY;
           }
        }

        yPrev = v1.rY;
        v1 = v2;
    }

    if( pmin_x ) *pmin_x = min_x;
    if( pmax_x ) *pmax_x = max_x;
    if( pmin_y ) *pmin_y = min_y;
    if( pmax_y ) *pmax_y = max_y;
}

// 현재 스캔라인 활성화된 엣지 리스트 추가
static void SPF_addActiveEdgeList( ACTIVE_EDGES *active, EDGE_ENTRY *edges )
{
    EDGE_ENTRY *p, *q;

    // 현재 활성화된 Edge가 없다면, 아예 통째로 리스트 갱신
    // (이미 엣지 리스트는 X 소트가 되어 있기 때문에.)
    if( active->lpEdges == atNULL  ){
      active->lpEdges = edges;
      active->EdgeCount = SPF_getEdgeCount(edges);
      return;
    }

    // 엣지 리스트의 엣지들을 X 소트시키면서 활성화된 엣지 리스트에 추가
    p = edges;
    while (p) {
        q = p->next;
        SPF_insertEdge( &active->lpEdges, p );
        active->EdgeCount++;
        p = q;
    }
}

// 활성화된 엣지를 하나 삭제. (메모리 해제에 대한 기준 아직 마련 못 했음.)
static void  SPF_deleteActiveEdge( ACTIVE_EDGES *active, EDGE_ENTRY *pre_p, EDGE_ENTRY *p )
{
    active->EdgeCount--;

    if( p == active->lpEdges ) {
      active->lpEdges = p->next;
      SPF_freeEdge( p );
      return;
    }

    pre_p->next = p->next;
    SPF_freeEdge( p );
}

// 활성화된 엣지 재정렬. (Simple(Convex, Concave) 폴리곤의 경우엔 불필요. Intersecting 폴리곤의 경우 필수)
static void SPF_resortActiveList( ACTIVE_EDGES *active )
{
    EDGE_ENTRY *q, *p;

    if( active->EdgeCount < 2 ) return;

    p = active->lpEdges;
    active->lpEdges = atNULL;
    while (p){
        q = p->next;
        SPF_insertEdge( &active->lpEdges, p );
        p = q;
    }
}

// 활성화된 엣지 업데이트. 스캔라인 증가에 따라 X 좌표 갱신 및 무효한 엣지 제거
static void SPF_updateActiveList( ACTIVE_EDGES *active, int y_scan )
{
    EDGE_ENTRY *p, *pre_p, *q;

    if( active->lpEdges == atNULL ) return;

    pre_p = active->lpEdges;
    p = pre_p;
    while(p){
        if( y_scan >= p->y_upper ){
            q = p->next;
            SPF_deleteActiveEdge( active, pre_p, p );
            p = q;
        } else {
            p->x_inter = p->x_inter + p->dx_per_scan;
            pre_p = p;
            p = p->next;
        }
    }
}

// 현재 스캔라인 필링
static void SPF_fillScan (atGRPX_CANVAS *lpCanvas, int y_scan, ACTIVE_EDGES *active,
                atLONG ExParam1,
				__POLYCALLBACK__DRAW_LINE *fnDrawLine)
{
    EDGE_ENTRY * p1, * p2;
    atRECT *clip;
    atREAL sx, ex;

    clip = atGRPX_GetClipRect(lpCanvas);
//    if( scan < clip->nStartY ) return; // 이미 수직 클리핑이 되어 있기에 불필요.
//    if( scan > clip->nEndY ) return;   // 이미 수직 클리핑이 되어 있에 불필요.

    p1 = active->lpEdges;
    while (p1) {
        p2 = p1->next;
        if( p2 == atNULL ) break;
        // 현재 엣지의 양 교차점을 얻는다. (X 정렬되어 있는 상태)
        sx = p1->x_inter;
        ex = p2->x_inter;
//	 if( sx != ex )
	{
          // X클리핑하여 수평선 긋는 callback 함수를 호출
//          if( (ex >= INTTOREAL(clip->nStartX)) && (sx <= INTTOREAL(clip->nEndX)) ){
//             if( sx < INTTOREAL(clip->nStartX) ) sx = INTTOREAL(clip->nStartX);
//             if( ex > INTTOREAL(clip->nEndX) ) ex = INTTOREAL(clip->nEndX);
             fnDrawLine( lpCanvas, sx, ex, y_scan, p1->dx_per_scan, p2->dx_per_scan, ((atDWORD)p1->attr << 16) | (atDWORD)p2->attr, ExParam1 );
//          }
	 }
        p1 = p2->next; // 번갈아가며 찍기 때문에 한칸 건너 뛴다.
    }
}


/*************************************
  Hole을 포함한 다각형 라인을 그린다.
**************************************/
void __atGRPX_FillPolyPolygon0( atGRPX_CANVAS *lpCanvas, atPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount,
						        atLONG ExParam1,
						        __POLYCALLBACK__DRAW_LINE *fnDrawLine,
							 atLPGRPX_FUNC_COORD_CONV lpCnvFunc, atLPVOID lpParam )
{
	int i;
    EDGE_ENTRY **scan_edges;
    EDGE_ENTRY *edge_pool;
    int idx, y_scan;
    int min_x, min_y, max_x, max_y;
    atRECT *clip;
    ACTIVE_EDGES active;

    if( lpPtrList == atNULL ) return;
	if( nPolyCount <= 0 ) return;

//	g_lpGrpX = lpCanvas->lpGrpX;

    clip = atGRPX_GetClipRect(lpCanvas);
    if( clip->nStartY > clip->nEndY ) return;

    // 클리핑 아랫쪽 끝까지 이어지는 외각선 테이블을 생성.
    // (클래핑 시작 부분까지 제거하려고 했으나, 불안한 면이 있어서 끝 부분까지만 했음.
    //  사용 빈도가 높기 때문에 Polygon 캐쉬를 이용함.)
#ifdef atGRPX_USES_POLYGON_CACHE
    {
    atLPGRPXMNG lpGrpX = lpCanvas->lpGrpX;
    int cachesize = sizeof(EDGE_ENTRY *) * lpGrpX->ScrInfo.nFrameHeight
                  + sizeof(EDGE_ENTRY) * atGRPX_MAX_POLYGON_ENTRY;
    if( (cachesize > lpGrpX->nPolygonCacheSize) && lpGrpX->lpPolygonCache ){
         atGRPX_FreeMem( lpGrpX, lpGrpX->lpPolygonCache );
	  lpGrpX->lpPolygonCache = atNULL;
	  lpGrpX->nPolygonCacheSize = 0;
    }
    if( lpGrpX->lpPolygonCache == atNULL ){
	    lpGrpX->lpPolygonCache = (void *)atGRPX_AllocMem( lpGrpX, cachesize );
	    if( lpGrpX->lpPolygonCache == atNULL ) return;
	  lpGrpX->nPolygonCacheSize = cachesize;
    }

	scan_edges = (EDGE_ENTRY **)lpGrpX->lpPolygonCache;
    memset( &scan_edges[clip->nStartY], 0, sizeof(EDGE_ENTRY*)*(clip->nEndY-clip->nStartY+1) );
    edge_pool = (EDGE_ENTRY *)((atBYTE *)lpGrpX->lpPolygonCache + sizeof(EDGE_ENTRY*)*lpGrpX->ScrInfo.nFrameHeight);
    SPF_freeAllEdge( edge_pool, atGRPX_MAX_POLYGON_ENTRY );
    }
#else
	scan_edges = (EDGE_ENTRY **)atGRPX_AllocMem( lpCanvas->lpGrpX, sizeof(EDGE_ENTRY *) * (clip->nEndY + 1) );
    memset( scan_edges, 0, sizeof(EDGE_ENTRY *) * (clip->nEndY + 1) );
    edge_pool = (EDGE_ENTRY *)atGRPX_AllocMem( lpCanvas->lpGrpX, sizeof(EDGE_ENTRY) * atGRPX_MAX_POLYGON_ENTRY );
    SPF_freeAllEdge( edge_pool, atGRPX_MAX_POLYGON_ENTRY );
#endif

    // 엣지 리스트를 생성한다.
    idx = 0;
    min_x = max_x = lpPtrList[0].nX;
    min_y = max_y = lpPtrList[0].nY;
    for( i = 0; i < nPolyCount; i++ ){
        if( nPartCount[i] >= 3 ){
            SPF_buildEdgeList( edge_pool, atGRPX_MAX_POLYGON_ENTRY, scan_edges, (atPOINT *)&lpPtrList[idx], nPartCount[i], clip, &min_x, &min_y, &min_y, &max_y, lpCnvFunc, lpParam );
        }
        idx += nPartCount[i];
    }

    // 클리핑 영역으로 스캔라인의 시작과 끝을 얻어낸다.
    if( max_y < clip->nStartY ) return;
    if( min_y > clip->nEndY ) return;
    if( min_y < clip->nStartY ) min_y = clip->nStartY;
    if( max_y > clip->nEndY ) max_y = clip->nEndY;

    // 활성화된 엣지 초기화
    active.EdgeCount = 0;
    active.lpEdges = atNULL;

    // 스캔라인을 진행한다.
    for( y_scan = min_y; y_scan <= max_y; y_scan++ ){
      // 엣지가 있는 스캔라인에 도달하면...
      if( scan_edges[y_scan] != atNULL ){
          // 활성화된 엣지에 해당 스캔라인의 엣지들을 모두 추가
          SPF_addActiveEdgeList( &active, scan_edges[y_scan] );
      }
      // 활성화된 엣지가 있다면...
      if( active.lpEdges != atNULL ) {
        // 스캔라인 fill을 한다.
        SPF_fillScan( lpCanvas, y_scan, &active, ExParam1, fnDrawLine );

        // 활성화된 엣지들을 갱신한다. (엣지 x좌표 갱신 및, 무효해진 엣지 삭제 등...)
        SPF_updateActiveList( &active, y_scan );
        // 활성화된 엣지를 대상으로 다시 소트한다.
        SPF_resortActiveList( &active ); // simple polygon일 경우엔 필요 없으나, 교차된 폴리곤은 필요
      }
    }

#ifdef atGRPX_USES_POLYGON_CACHE
//    SPF_freeAllEdge( edge_pool, atGRPX_MAX_POLYGON_ENTRY ); // 어차피 나중에 재진입시 초기화 하기 때문에 불필요.
#else
	atGRPX_FreeMem( lpCanvas->lpGrpX, edge_pool );
	atGRPX_FreeMem( lpCanvas->lpGrpX, scan_edges );
#endif
}


/*************************************
  Hole을 포함한 다각형 라인을 그린다.
**************************************/
void __atGRPX_FillPolyPolygon0_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT lpPtrList[], atINT nPartCount[], atINT nPolyCount,
						        atLONG ExParam1,
						        __POLYCALLBACK__DRAW_LINE *fnDrawLine )
{
	int i;
    EDGE_ENTRY **scan_edges;
    EDGE_ENTRY *edge_pool;
    int idx, y_scan;
    atREAL min_x, min_y, max_x, max_y;
    atRECT *clip;
    ACTIVE_EDGES active;

    if( lpPtrList == atNULL ) return;
	if( nPolyCount <= 0 ) return;

    clip = atGRPX_GetClipRect(lpCanvas);
    if( clip->nStartY > clip->nEndY ) return;

    // 클리핑 아랫쪽 끝까지 이어지는 외각선 테이블을 생성.
    // (클래핑 시작 부분까지 제거하려고 했으나, 불안한 면이 있어서 끝 부분까지만 했음.
    //  사용 빈도가 높기 때문에 Polygon 캐쉬를 이용함.)
#ifdef atGRPX_USES_POLYGON_CACHE
    {
    atLPGRPXMNG lpGrpX = lpCanvas->lpGrpX;
    int cachesize = sizeof(EDGE_ENTRY *) * lpGrpX->ScrInfo.nFrameHeight
                  + sizeof(EDGE_ENTRY) * atGRPX_MAX_POLYGON_ENTRY;
    if( (cachesize > lpGrpX->nPolygonCacheSize) && lpGrpX->lpPolygonCache ){
         atGRPX_FreeMem( lpGrpX, lpGrpX->lpPolygonCache );
	  lpGrpX->lpPolygonCache = atNULL;
	  lpGrpX->nPolygonCacheSize = 0;
    }
    if( lpGrpX->lpPolygonCache == atNULL ){
	    lpGrpX->lpPolygonCache = (void *)atGRPX_AllocMem( lpGrpX, cachesize );
	    if( lpGrpX->lpPolygonCache == atNULL ) return;
	  lpGrpX->nPolygonCacheSize = cachesize;
    }

	scan_edges = (EDGE_ENTRY **)lpGrpX->lpPolygonCache;
    memset( &scan_edges[clip->nStartY], 0, sizeof(EDGE_ENTRY*)*(clip->nEndY-clip->nStartY+1) );
    edge_pool = (EDGE_ENTRY *)((atBYTE *)lpGrpX->lpPolygonCache + sizeof(EDGE_ENTRY*)*lpGrpX->ScrInfo.nFrameHeight);
    SPF_freeAllEdge( edge_pool, atGRPX_MAX_POLYGON_ENTRY );
    }
#else
    scan_edges = (EDGE_ENTRY **)atGRPX_AllocMem( lpCanvas->lpGrpX, sizeof(EDGE_ENTRY *) * (clip->nEndY + 1) );
    memset( scan_edges, 0, sizeof(EDGE_ENTRY *) * (clip->nEndY + 1) );
    edge_pool = (EDGE_ENTRY *)atGRPX_AllocMem( lpCanvas->lpGrpX, sizeof(EDGE_ENTRY) * atGRPX_MAX_POLYGON_ENTRY );
    SPF_freeAllEdge( edge_pool, atGRPX_MAX_POLYGON_ENTRY );
#endif

    // 엣지 리스트를 생성한다.
    idx = 0;
    min_x = max_x = lpPtrList[0].rX;
    min_y = max_y = lpPtrList[0].rY;
    for( i = 0; i < nPolyCount; i++ ){
        if( nPartCount[i] >= 3 ){
            SPF_buildEdgeList_REAL( edge_pool, atGRPX_MAX_POLYGON_ENTRY, scan_edges, (atRPOINT *)&lpPtrList[idx], nPartCount[i], clip, &min_x, &min_y, &min_y, &max_y );
        }
        idx += nPartCount[i];
    }

    // 클리핑 영역으로 스캔라인의 시작과 끝을 얻어낸다.
    if( REALTOINT(max_y) < clip->nStartY ) return;
    if( REALTOINT(min_y) > clip->nEndY ) return;
    if( REALTOINT(min_y) < clip->nStartY ) min_y = INTTOREAL(clip->nStartY);
    if( REALTOINT(max_y) > clip->nEndY ) max_y = INTTOREAL(clip->nEndY);

    // 활성화된 엣지 초기화
    active.EdgeCount = 0;
    active.lpEdges = atNULL;

    // 스캔라인을 진행한다.
    for( y_scan = REALTOINT(min_y); y_scan <= REALTOINT(max_y); y_scan++ ){
      // 엣지가 있는 스캔라인에 도달하면...
      if( scan_edges[y_scan] != atNULL ){
          // 활성화된 엣지에 해당 스캔라인의 엣지들을 모두 추가
          SPF_addActiveEdgeList( &active, scan_edges[y_scan] );
      }
      // 활성화된 엣지가 있다면...
      if( active.lpEdges != atNULL ) {
        // 스캔라인 fill을 한다.
        SPF_fillScan( lpCanvas, y_scan, &active, ExParam1, fnDrawLine );

        // 활성화된 엣지들을 갱신한다. (엣지 x좌표 갱신 및, 무효해진 엣지 삭제 등...)
        SPF_updateActiveList( &active, y_scan );
        // 활성화된 엣지를 대상으로 다시 소트한다.
        SPF_resortActiveList( &active ); // simple polygon일 경우엔 필요 없으나, 교차된 폴리곤은 필요
      }
    }

#ifdef atGRPX_USES_POLYGON_CACHE
//    SPF_freeAllEdge( edge_pool, atGRPX_MAX_POLYGON_ENTRY ); // 어차피 나중에 재진입시 초기화 하기 때문에 불필요.
#else
	atGRPX_FreeMem( lpCanvas->lpGrpX, edge_pool );
	atGRPX_FreeMem( lpCanvas->lpGrpX, scan_edges );
#endif
}


