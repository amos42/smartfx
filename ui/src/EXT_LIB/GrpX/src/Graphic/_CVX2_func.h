#ifndef ___CVX2_FUNC_H_
#define ___CVX2_FUNC_H_


#include "GrpX.h"
#include "AT_Real.h"

#ifdef __cplusplus
extern "C" {
#endif

atINT __atGRPX_FillConvexPolygon1(atGRPX_CANVAS *lpCanvas, atPOINT *pVertex, atINT nCount, atGRPX_COLOR clrColor, atBOOL bAA, atBOOL bCliped);
atINT __atGRPX_FillConvexPolygon1_REAL(atGRPX_CANVAS *lpCanvas, atRPOINT *pVertex, atINT nCount, atGRPX_COLOR clrColor, atBOOL bAA, atBOOL bCliped);

#ifdef __cplusplus
}
#endif


#endif
