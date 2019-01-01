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
 Description       : ������ Base ��ƾ

-----------------------------------------------------------------------------
###########################################################################*/

#include "GrpX.h"
#include "_PLY_func.h"
#include <stdlib.h>
#include <string.h>


#define EDGE_ATTR_NORMAL 0
#define EDGE_ATTR_CLIPPED_LEFT   1
#define EDGE_ATTR_CLIPPED_RIGHT 2

// ���� ��Ʈ��
typedef struct tagEDGE_ENTRY {
    atSHORT   active;               // ���� Ǯ�� ���� ����.
    atU16        attr;               // ���� Ǯ�� ���� ����.

    atLONG   y_upper;              // ���� ������ Y ��ǥ
    atREAL    x_inter;              // ���� X ������
    atREAL    dx_per_scan;          // ��ĵ���δ� X ��ȭ��

    struct tagEDGE_ENTRY *next;     // ����� ���� ���� ������
} EDGE_ENTRY, *LPEDGE_ENTRY;

// ��ĵ �������� Ȱ��ȭ�� ���� ����
typedef struct tagACTIVE_EDGES {
    int         EdgeCount;
    EDGE_ENTRY *lpEdges;
} ACTIVE_EDGES;


// ���� Ǯ�κ��� ������ �Ҵ� �޴´�.
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

// ������ �����Ѵ�.
static void SPF_freeEdge( EDGE_ENTRY *edge )
{
    edge->active = atFALSE;
}

// ������ ��� �����Ѵ�.
static void SPF_freeAllEdge( EDGE_ENTRY *edge_pool, int max_cnt )
{
    int i;
    EDGE_ENTRY *p = edge_pool;

    for( i = 0; i < max_cnt; i++ ){
      p->active = atFALSE;
      p++;
    }
}

// �ܰ��� �ϳ��� �߰�. �߰� �������� X ������ �Ѵ�.
static int SPF_insertEdge( EDGE_ENTRY ** scan_edge, EDGE_ENTRY * new_edge )
{
    EDGE_ENTRY *p, *pre_p;

    pre_p = *scan_edge;

    // ���� �ش� ��Ʈ���� ������� ���� ��Ʈ���� ��ü.
    if( pre_p == atNULL ){
      new_edge->next = atNULL;
      *scan_edge = new_edge;
      return 1;
    }

    // X ��ǥ �������� ���ο� ������ �� ��ġ�� ã�´�.
    p = pre_p;
    while( p != atNULL ){
        if (new_edge->x_inter <= p->x_inter){
            // ��ġ�� ��Ȯ�� ��ġ�� ���� ���, �¿츦 �Ǵ��ؾ� �Ѵ�.
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


    // �ش� ��ġ�� ������ ����
    if( p == *scan_edge ){
        new_edge->next = p;
        *scan_edge = new_edge;
    } else {
        new_edge->next = pre_p->next;
        pre_p->next = new_edge;
    }

    return 1;
}

// ���� ���� ����Ʈ�� ����� ������ ������ ��´�.
static int SPF_getEdgeCount( EDGE_ENTRY * edge )
{
    int cnt = 0;
    
    while( edge != atNULL ){
        cnt++;
        edge = edge->next;
    }

    return cnt;
}

// �ܰ��� ������ Y ��ǥ�� ���⸦ ��Ʈ�Ѵ�.
// Ŭ������ �����Ͽ� ��ȿ�� �ܰ����� �����ϸ�, �� ��� Ŭ���� ����� ������ ��ǥ�� ��Ʈ�Ѵ�.
static void SPF_makeEdgeRec( EDGE_ENTRY *scan_edges[], EDGE_ENTRY * edge, atPOINT *lower, atPOINT *upper, int yComp, atRECT *clip, atDWORD dwAttr )
{
    atLONG lower_y = lower->nY;

    // �Ӽ��� ��Ʈ�Ѵ�. (Ŭ���� �� �����ΰ�)
    edge->attr = (atU16)dwAttr;
	
    // ���⸦ ��Ʈ�Ѵ�.
    edge->dx_per_scan = INTTOREAL(upper->nX - lower->nX) / (upper->nY - lower_y);

    // ���� Y ��ǥ�� Ŭ���� �Ѵ�.
    if( lower_y >= clip->nStartY )
      edge->x_inter = INTTOREAL(lower->nX);
    else {
      edge->x_inter = INTTOREAL(lower->nX) + edge->dx_per_scan * (clip->nStartY - lower_y);
      lower_y = clip->nStartY;
    }

    // ���� Y ������ ������ ������ ��ǥ�� ��ĭ �۰�.
//    if (upper->nY < yComp)
//        edge->y_upper = upper->nY - 1;
//    else
//        edge->y_upper = upper->nY;
edge->y_upper = upper->nY - 1;  // Ȧ �����￡ ���� ���� �� ���� ���Դ� ����. ���� �ľ��� ���� �ӽ÷� ������ -1 �ؼ� �ִ´�.

    // �ڿ� �̾����� ���� ����.
    edge->next = atNULL;

    // ���� ��ĵ ���� ����Ʈ�� ������ �߰�
    SPF_insertEdge( &scan_edges[lower_y], edge );
}

// �ܰ��� ������ Y ��ǥ�� ���⸦ ��Ʈ�Ѵ�.
// Ŭ������ �����Ͽ� ��ȿ�� �ܰ����� �����ϸ�, �� ��� Ŭ���� ����� ������ ��ǥ�� ��Ʈ�Ѵ�.
static void SPF_makeEdgeRec_REAL( EDGE_ENTRY *scan_edges[], EDGE_ENTRY * edge, atRPOINT *lower, atRPOINT *upper, atREAL yComp, atRECT *clip, atDWORD dwAttr )
{
    atREAL lower_y = lower->rY;

    // �Ӽ��� ��Ʈ�Ѵ�. (Ŭ���� �� �����ΰ�)
    edge->attr = (atU16)dwAttr;
	
    // ���⸦ ��Ʈ�Ѵ�.
    edge->dx_per_scan = REALDIV( (upper->rX - lower->rX), (upper->rY - lower_y) );

    // ���� Y ��ǥ�� Ŭ���� �Ѵ�.
    if( REALTOINT(lower_y) >= clip->nStartY )
      edge->x_inter = lower->rX;
    else {
      edge->x_inter = lower->rX + REALMUL( edge->dx_per_scan, (INTTOREAL(clip->nStartY) - lower_y) );
      lower_y = INTTOREAL(clip->nStartY);
    }

    // ���� Y ������ ������ ������ ��ǥ�� ��ĭ �۰�.
//    if (upper->rY < yComp)
//        edge->y_upper = REALTOINT(upper->rY);
//    else
//        edge->y_upper = REALTOINT_U(upper->rY);
edge->y_upper = REALTOINT(upper->rY)-1;  // Ȧ �����￡ ���� ���� �� ���� ���Դ� ����. ���� �ľ��� ���� �ӽ÷� ������ -1 �ؼ� �ִ´�.

    // �ڿ� �̾����� ���� ����.
    edge->next = atNULL;

    // ���� ��ĵ ���� ����Ʈ�� ������ �߰�
    SPF_insertEdge( &scan_edges[REALTOINT(lower_y)], edge );
}

// �־��� �ε����� �ٷ� ���� Y ��ǥ�� ã�Ƴ���.
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

// �־��� �ε����� �ٷ� ���� Y ��ǥ�� ã�Ƴ���.
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

// ������ ����Ʈ ����Ʈ�� ��������, �ܰ��� ����Ʈ�� ������ ����.
// (Hole�� ������ Poly-Polygon�� ��쿡�� �̸� �ݺ� ȣ���� �ֱ⸸ �ϸ� �ȴ�.)
static void SPF_buildEdgeList( EDGE_ENTRY * edge_pool, int max_cnt, EDGE_ENTRY * scan_edges[], atPOINT pts[], int cnt, atRECT *clip, int *pmin_x, int *pmax_x, int *pmin_y, int *pmax_y, atLPGRPX_FUNC_COORD_CONV lpCnvFunc, atLPVOID lpParam )
{
    EDGE_ENTRY * edge;
    atPOINT v1, v2, v3;
    int i, j, yPrev;
    atBOOL isAdd;
    int min_x, max_x, min_y, max_y;
    atDWORD dwClipped;

    // ���� ���� ���� ó�� ���� ��ġ�ϸ� ����
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
	// ���� ��ġ�� ������ ����
	if( (v1.nX == v2.nX) && (v1.nY == v2.nY) ) continue;

        // Ŭ���� ������ ������ ��� �ܰ����� �ƿ� ����. (Y�� Ŭ���θ�)
        isAdd = atTRUE;
	 dwClipped = 0;
        if( (v1.nY < clip->nStartY) && (v2.nY < clip->nStartY) ) isAdd = atFALSE;
        else if( (v1.nY > clip->nEndY) && (v2.nY > clip->nEndY) ) isAdd = atFALSE;
/*
        // XŬ���� ������ ������ ��� �ܰ����� �ܰ����� �ٵ���
        if( (v1.nX < clip->nStartX) && (v2.nX < clip->nStartX) ){
		v1.nX = v2.nX = clip->nStartX;
		dwClipped = EDGE_ATTR_CLIPPED_RIGHT;
        } else if( (v1.nX > clip->nEndX) && (v2.nX > clip->nEndX) ){
		v1.nX = v2.nX = clip->nEndX;
		dwClipped = EDGE_ATTR_CLIPPED_LEFT;
        }
*/

        if( isAdd ){
           if (v1.nY != v2.nY) { // ���� ��ġ�� ���� ���� ����.
              edge = SPF_findFreeEdge(edge_pool, max_cnt);
			  if( edge == atNULL ) break;

              if (v1.nY < v2.nY){
                  // ��� ����
                  j = SPF_FindNextY(i, cnt, pts);
		    if( lpCnvFunc ){
	        	lpCnvFunc( lpParam, &v3.nX, &v3.nY, pts[j].nX, pts[j].nY );
			yPrev = v3.nY;
	           } else
			yPrev = pts[j].nY;
                  SPF_makeEdgeRec ( scan_edges, edge, &v1, &v2, yPrev, clip, dwClipped );
              } else {
                  // �ϰ� ����
                  SPF_makeEdgeRec ( scan_edges, edge, &v2, &v1, yPrev, clip, dwClipped );
              }

              // �ִ� �ּҰ� ����
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

// ������ ����Ʈ ����Ʈ�� ��������, �ܰ��� ����Ʈ�� ������ ����.
// (Hole�� ������ Poly-Polygon�� ��쿡�� �̸� �ݺ� ȣ���� �ֱ⸸ �ϸ� �ȴ�.)
static void SPF_buildEdgeList_REAL( EDGE_ENTRY * edge_pool, int max_cnt, EDGE_ENTRY * scan_edges[], atRPOINT pts[], int cnt, atRECT *clip, atREAL *pmin_x, atREAL *pmax_x, atREAL *pmin_y, atREAL *pmax_y )
{
    EDGE_ENTRY * edge;
    atRPOINT v1, v2;
    int i, yPrev;
    atBOOL isAdd;
    atREAL min_x, max_x, min_y, max_y;
    atDWORD dwClipped;

    // ���� ���� ���� ó�� ���� ��ġ�ϸ� ����
    if( (pts[0].rX == pts[cnt-1].rX) && (pts[0].rY == pts[cnt-1].rY) ) cnt--;

    yPrev = pts[cnt - 2].rY;

    v1 = pts[cnt-1];

	min_x = *pmin_x;
	min_y = *pmin_y;
	max_x = *pmax_x;
	max_y = *pmax_y;

    for( i = 0; i < cnt; i++ ){
        v2 = pts[i];
	// ���� ��ġ�� ������ ����
	if( (v1.rX == v2.rX) && (v1.rY == v2.rY) ) continue;

        // Ŭ���� ������ ������ ��� �ܰ����� �ƿ� ����. (Y�� Ŭ���θ�)
        isAdd = atTRUE;
	dwClipped = 0;
        if( (REALTOINT(v1.rY) < clip->nStartY) && (REALTOINT(v2.rY) < clip->nStartY) ) isAdd = atFALSE;
        else if( (REALTOINT(v1.rY) > clip->nEndY) && (REALTOINT(v2.rY) > clip->nEndY) ) isAdd = atFALSE;
/*
        // XŬ���� ������ ������ ��� �ܰ����� �ܰ����� �ٵ���
        if( (REALTOINT(v1.rX) < clip->nStartX) && (REALTOINT(v2.rX) < clip->nStartX) ){
		v1.rX = v2.rX = INTTOREAL(clip->nStartX);
		dwClipped = EDGE_ATTR_CLIPPED_RIGHT;
        } else if( (REALTOINT(v1.rX) > clip->nEndX) && (REALTOINT(v2.rX) > clip->nEndX) ){
		v1.rX = v2.rX = INTTOREAL(clip->nEndX);
		dwClipped = EDGE_ATTR_CLIPPED_LEFT;
        }
*/

        if( isAdd ){
           if (v1.rY != v2.rY) { // ���� ��ġ�� ���� ���� ����.
              edge = SPF_findFreeEdge(edge_pool, max_cnt);
			  if( edge == atNULL ) break;

              if (v1.rY < v2.rY){
                  // ��� ����
                  SPF_makeEdgeRec_REAL ( scan_edges, edge, &v1, &v2, SPF_FindNextY_REAL(i, cnt, pts), clip, dwClipped );
              } else {
                  // �ϰ� ����
                  SPF_makeEdgeRec_REAL ( scan_edges, edge, &v2, &v1, yPrev, clip, dwClipped );
              }

              // �ִ� �ּҰ� ����
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

// ���� ��ĵ���� Ȱ��ȭ�� ���� ����Ʈ �߰�
static void SPF_addActiveEdgeList( ACTIVE_EDGES *active, EDGE_ENTRY *edges )
{
    EDGE_ENTRY *p, *q;

    // ���� Ȱ��ȭ�� Edge�� ���ٸ�, �ƿ� ��°�� ����Ʈ ����
    // (�̹� ���� ����Ʈ�� X ��Ʈ�� �Ǿ� �ֱ� ������.)
    if( active->lpEdges == atNULL  ){
      active->lpEdges = edges;
      active->EdgeCount = SPF_getEdgeCount(edges);
      return;
    }

    // ���� ����Ʈ�� �������� X ��Ʈ��Ű�鼭 Ȱ��ȭ�� ���� ����Ʈ�� �߰�
    p = edges;
    while (p) {
        q = p->next;
        SPF_insertEdge( &active->lpEdges, p );
        active->EdgeCount++;
        p = q;
    }
}

// Ȱ��ȭ�� ������ �ϳ� ����. (�޸� ������ ���� ���� ���� ���� �� ����.)
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

// Ȱ��ȭ�� ���� ������. (Simple(Convex, Concave) �������� ��쿣 ���ʿ�. Intersecting �������� ��� �ʼ�)
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

// Ȱ��ȭ�� ���� ������Ʈ. ��ĵ���� ������ ���� X ��ǥ ���� �� ��ȿ�� ���� ����
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

// ���� ��ĵ���� �ʸ�
static void SPF_fillScan (atGRPX_CANVAS *lpCanvas, int y_scan, ACTIVE_EDGES *active,
                atLONG ExParam1,
				__POLYCALLBACK__DRAW_LINE *fnDrawLine)
{
    EDGE_ENTRY * p1, * p2;
    atRECT *clip;
    atREAL sx, ex;

    clip = atGRPX_GetClipRect(lpCanvas);
//    if( scan < clip->nStartY ) return; // �̹� ���� Ŭ������ �Ǿ� �ֱ⿡ ���ʿ�.
//    if( scan > clip->nEndY ) return;   // �̹� ���� Ŭ������ �Ǿ� �ֿ� ���ʿ�.

    p1 = active->lpEdges;
    while (p1) {
        p2 = p1->next;
        if( p2 == atNULL ) break;
        // ���� ������ �� �������� ��´�. (X ���ĵǾ� �ִ� ����)
        sx = p1->x_inter;
        ex = p2->x_inter;
//	 if( sx != ex )
	{
          // XŬ�����Ͽ� ���� �ߴ� callback �Լ��� ȣ��
//          if( (ex >= INTTOREAL(clip->nStartX)) && (sx <= INTTOREAL(clip->nEndX)) ){
//             if( sx < INTTOREAL(clip->nStartX) ) sx = INTTOREAL(clip->nStartX);
//             if( ex > INTTOREAL(clip->nEndX) ) ex = INTTOREAL(clip->nEndX);
             fnDrawLine( lpCanvas, sx, ex, y_scan, p1->dx_per_scan, p2->dx_per_scan, ((atDWORD)p1->attr << 16) | (atDWORD)p2->attr, ExParam1 );
//          }
	 }
        p1 = p2->next; // �����ư��� ��� ������ ��ĭ �ǳ� �ڴ�.
    }
}


/*************************************
  Hole�� ������ �ٰ��� ������ �׸���.
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

    // Ŭ���� �Ʒ��� ������ �̾����� �ܰ��� ���̺��� ����.
    // (Ŭ���� ���� �κб��� �����Ϸ��� ������, �Ҿ��� ���� �־ �� �κб����� ����.
    //  ��� �󵵰� ���� ������ Polygon ĳ���� �̿���.)
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

    // ���� ����Ʈ�� �����Ѵ�.
    idx = 0;
    min_x = max_x = lpPtrList[0].nX;
    min_y = max_y = lpPtrList[0].nY;
    for( i = 0; i < nPolyCount; i++ ){
        if( nPartCount[i] >= 3 ){
            SPF_buildEdgeList( edge_pool, atGRPX_MAX_POLYGON_ENTRY, scan_edges, (atPOINT *)&lpPtrList[idx], nPartCount[i], clip, &min_x, &min_y, &min_y, &max_y, lpCnvFunc, lpParam );
        }
        idx += nPartCount[i];
    }

    // Ŭ���� �������� ��ĵ������ ���۰� ���� ����.
    if( max_y < clip->nStartY ) return;
    if( min_y > clip->nEndY ) return;
    if( min_y < clip->nStartY ) min_y = clip->nStartY;
    if( max_y > clip->nEndY ) max_y = clip->nEndY;

    // Ȱ��ȭ�� ���� �ʱ�ȭ
    active.EdgeCount = 0;
    active.lpEdges = atNULL;

    // ��ĵ������ �����Ѵ�.
    for( y_scan = min_y; y_scan <= max_y; y_scan++ ){
      // ������ �ִ� ��ĵ���ο� �����ϸ�...
      if( scan_edges[y_scan] != atNULL ){
          // Ȱ��ȭ�� ������ �ش� ��ĵ������ �������� ��� �߰�
          SPF_addActiveEdgeList( &active, scan_edges[y_scan] );
      }
      // Ȱ��ȭ�� ������ �ִٸ�...
      if( active.lpEdges != atNULL ) {
        // ��ĵ���� fill�� �Ѵ�.
        SPF_fillScan( lpCanvas, y_scan, &active, ExParam1, fnDrawLine );

        // Ȱ��ȭ�� �������� �����Ѵ�. (���� x��ǥ ���� ��, ��ȿ���� ���� ���� ��...)
        SPF_updateActiveList( &active, y_scan );
        // Ȱ��ȭ�� ������ ������� �ٽ� ��Ʈ�Ѵ�.
        SPF_resortActiveList( &active ); // simple polygon�� ��쿣 �ʿ� ������, ������ �������� �ʿ�
      }
    }

#ifdef atGRPX_USES_POLYGON_CACHE
//    SPF_freeAllEdge( edge_pool, atGRPX_MAX_POLYGON_ENTRY ); // ������ ���߿� �����Խ� �ʱ�ȭ �ϱ� ������ ���ʿ�.
#else
	atGRPX_FreeMem( lpCanvas->lpGrpX, edge_pool );
	atGRPX_FreeMem( lpCanvas->lpGrpX, scan_edges );
#endif
}


/*************************************
  Hole�� ������ �ٰ��� ������ �׸���.
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

    // Ŭ���� �Ʒ��� ������ �̾����� �ܰ��� ���̺��� ����.
    // (Ŭ���� ���� �κб��� �����Ϸ��� ������, �Ҿ��� ���� �־ �� �κб����� ����.
    //  ��� �󵵰� ���� ������ Polygon ĳ���� �̿���.)
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

    // ���� ����Ʈ�� �����Ѵ�.
    idx = 0;
    min_x = max_x = lpPtrList[0].rX;
    min_y = max_y = lpPtrList[0].rY;
    for( i = 0; i < nPolyCount; i++ ){
        if( nPartCount[i] >= 3 ){
            SPF_buildEdgeList_REAL( edge_pool, atGRPX_MAX_POLYGON_ENTRY, scan_edges, (atRPOINT *)&lpPtrList[idx], nPartCount[i], clip, &min_x, &min_y, &min_y, &max_y );
        }
        idx += nPartCount[i];
    }

    // Ŭ���� �������� ��ĵ������ ���۰� ���� ����.
    if( REALTOINT(max_y) < clip->nStartY ) return;
    if( REALTOINT(min_y) > clip->nEndY ) return;
    if( REALTOINT(min_y) < clip->nStartY ) min_y = INTTOREAL(clip->nStartY);
    if( REALTOINT(max_y) > clip->nEndY ) max_y = INTTOREAL(clip->nEndY);

    // Ȱ��ȭ�� ���� �ʱ�ȭ
    active.EdgeCount = 0;
    active.lpEdges = atNULL;

    // ��ĵ������ �����Ѵ�.
    for( y_scan = REALTOINT(min_y); y_scan <= REALTOINT(max_y); y_scan++ ){
      // ������ �ִ� ��ĵ���ο� �����ϸ�...
      if( scan_edges[y_scan] != atNULL ){
          // Ȱ��ȭ�� ������ �ش� ��ĵ������ �������� ��� �߰�
          SPF_addActiveEdgeList( &active, scan_edges[y_scan] );
      }
      // Ȱ��ȭ�� ������ �ִٸ�...
      if( active.lpEdges != atNULL ) {
        // ��ĵ���� fill�� �Ѵ�.
        SPF_fillScan( lpCanvas, y_scan, &active, ExParam1, fnDrawLine );

        // Ȱ��ȭ�� �������� �����Ѵ�. (���� x��ǥ ���� ��, ��ȿ���� ���� ���� ��...)
        SPF_updateActiveList( &active, y_scan );
        // Ȱ��ȭ�� ������ ������� �ٽ� ��Ʈ�Ѵ�.
        SPF_resortActiveList( &active ); // simple polygon�� ��쿣 �ʿ� ������, ������ �������� �ʿ�
      }
    }

#ifdef atGRPX_USES_POLYGON_CACHE
//    SPF_freeAllEdge( edge_pool, atGRPX_MAX_POLYGON_ENTRY ); // ������ ���߿� �����Խ� �ʱ�ȭ �ϱ� ������ ���ʿ�.
#else
	atGRPX_FreeMem( lpCanvas->lpGrpX, edge_pool );
	atGRPX_FreeMem( lpCanvas->lpGrpX, scan_edges );
#endif
}


