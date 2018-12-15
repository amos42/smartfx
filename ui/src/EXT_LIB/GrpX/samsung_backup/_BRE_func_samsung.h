#include "GrpX.h"
#include "GrpX_samsung.h"
#include "..\Graphic\_AB_int.h"

atVOID	atGRPX_DrawPixelBre( atGRPX_PIXEL* addr, atLONG inc, atGRPX_COLOR Color, atLONG invD2du, atLONG diff_distance);
atVOID	atGRPX_DrawPixelBreMir( atGRPX_PIXEL* addr_s, atGRPX_PIXEL* addr_e, atLONG inc, atGRPX_COLOR Color, atLONG invD2du, atLONG diff_distance);
atVOID	atGRPX_DrawPixelBreH( atGRPX_CANVAS *lpCanvas, atLONG draw_x, atLONG draw_y, atLONG inc, atGRPX_COLOR Color, atLONG invD2du, atLONG diff_distance);
atVOID	atGRPX_DrawPixelBreV( atGRPX_CANVAS *lpCanvas, atLONG draw_x, atLONG draw_y, atLONG inc, atGRPX_COLOR Color, atLONG invD2du, atLONG diff_distance);
atVOID	atGRPX_DrawPixelBrePixel( atGRPX_CANVAS *lpCanvas, atLONG draw_x, atLONG draw_y, atGRPX_COLOR Color, atLONG diff_distance, atRECT* clip);
atVOID	atGRPX_DrawPixelBrePixel_2( atGRPX_CANVAS *lpCanvas, atLONG draw_x, atLONG draw_y, atGRPX_COLOR InColor, atGRPX_COLOR OutColor, atLONG diff_distance, atRECT* clip);
atVOID	atGRPX_DrawPixelBrePixelMem( atGRPX_PIXEL* addr_s, atLONG inc, atGRPX_COLOR Color, atLONG diff_distance);
atVOID	atGRPX_DrawPixelBreMirEdgeMem( atGRPX_PIXEL* addr_s, atGRPX_PIXEL* addr_e, atLONG inc, atGRPX_COLOR Color, atGRPX_COLOR BkColor, atLONG mid_alpha, atLONG out_alpha_1, atLONG out_alpha_2);
atVOID	atGRPX_DrawPixelBreEdgeMem( atGRPX_PIXEL* addr_s, atLONG inc, atGRPX_COLOR Color, atGRPX_COLOR BkColor, atLONG mid_alpha, atLONG out_alpha_1);
atVOID	atGRPX_DrawPixelBreMirEdgePt( atGRPX_CANVAS *lpCanvas, atLONG draws_x, atLONG draws_y, atLONG drawe_x, atLONG drawe_y, atLONG inc, atGRPX_COLOR Color, atLONG invD2du, atLONG diff_distance, atBOOL inverse, atLONG uincr, atLONG vincr);
atVOID	atGRPX_DrawBreLineWidthAAIn( atGRPX_CANVAS *lpCanvas, atRPOINT Pt_0, atRPOINT Pt_1, atRPOINT Pt_2, atRPOINT Pt_3, atGRPX_COLOR Color, atGRPX_COLOR OutColor, atINT nWidth );
atVOID	atGRPX_DrawBreLineWidthAAOut( atGRPX_CANVAS *lpCanvas, atRPOINT Pt_0, atRPOINT Pt_1, atRPOINT Pt_2, atRPOINT Pt_3, atPOINT pkPtOut[], atGRPX_COLOR Color, atGRPX_COLOR BkColor, atINT nWidth );
atVOID	atGRPX_DrawBreEdgeAAMem(atGRPX_CANVAS* lpCanvas, atPOINT Pt_0, atPOINT Pt_1, atPOINT* pkPtOut_0, atPOINT* pkPtOut_1, atGRPX_COLOR ExParam1, atGRPX_COLOR ExParam2);
atVOID	atGRPX_DrawBreEdgeAAPt(atGRPX_CANVAS* lpCanvas, atPOINT Pt_0, atPOINT Pt_1, atPOINT* pkPtOut_0, atPOINT* pkPtOut_1, atGRPX_COLOR Color, atGRPX_COLOR BkColor);
atVOID	atGRPX_DrawBreTriangleEdgeAA(atGRPX_CANVAS* lpCanvas, atRPOINT Pt_0, atRPOINT Pt_1, atRPOINT Pt_2, atGRPX_COLOR Color, atGRPX_COLOR OutColor, atINT nWidth);
atVOID	atGRPX_GetNextRoundPoint(atRPOINT Pt_0, atRPOINT Pt_1, atRPOINT Pt_2, atRPOINT Pt_3, atPOINT pkPtOut[], atINT nWidth);
atVOID	atGRPX_DrawBreTriangleEdgeLineAA(atGRPX_CANVAS* lpCanvas, atLONG StartX, atLONG StartY, atLONG EndX, atLONG EndY, atGRPX_COLOR Color, atGRPX_COLOR OutColor);
atVOID	atGRPX_DrawBreLineWidthAAInEdge( atGRPX_CANVAS *lpCanvas, atRPOINT Pt_0, atRPOINT Pt_1, atRPOINT Pt_2, atRPOINT Pt_3, atGRPX_COLOR Color, atGRPX_COLOR OutColor, atINT nWidth );
atVOID	atGRPX_DrawHorizLine2( atGRPX_CANVAS *lpCanvas, atLONG StartX, atLONG StartY, atINT Length, atGRPX_COLOR Color, atRECT* clip );
atVOID	atGRPX_DrawCircleTableMem(atGRPX_CANVAS *lpCanvas, atPOINT ptTL, atINT nWidth, atGRPX_COLOR Color);
atVOID	atGRPX_DrawCircleTablePt(atGRPX_CANVAS *lpCanvas, atPOINT ptTL, atINT nWidth, atGRPX_COLOR Color, atRECT* clip);