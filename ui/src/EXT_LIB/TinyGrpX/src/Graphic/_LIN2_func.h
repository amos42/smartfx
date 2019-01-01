#ifndef ___LIN2_FUNC_H_
#define ___LIN2_FUNC_H_


#include "TinyGrpX.h"


#ifdef __cplusplus
extern "C" {
#endif
	
	atVOID __atTGRPX_DrawLine1( atTGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY,	atTGRPX_COLOR Color );
	atVOID __atTGRPX_DrawLine1_REAL( atTGRPX_CANVAS *lpCanvas, atREAL StartX, atREAL StartY, atREAL EndX, atREAL EndY, atTGRPX_COLOR Color );
	
#ifdef __cplusplus
}
#endif


#endif
