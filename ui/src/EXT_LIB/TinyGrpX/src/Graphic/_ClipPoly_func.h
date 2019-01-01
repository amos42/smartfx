#ifndef ___CLIPPOLY_FUNC_H_
#define ___CLIPPOLY_FUNC_H_

#include "GrpX.h"

#ifdef __cplusplus
extern "C" {
#endif
	
#define MAX_VERTEX	1000

atINT __atGRPX_ClipPoly( atGRPX_CANVAS *lpCanvas, atPOINT *pVertex, atINT nCount, atBOOL bClosed, atPOINT *pOut );
atINT __atGRPX_ClipPoly_REAL( atGRPX_CANVAS *lpCanvas, atRPOINT *pVertex, atINT nCount, atBOOL bClosed, atRPOINT *pOut );

#ifdef __cplusplus
}
#endif


#endif
