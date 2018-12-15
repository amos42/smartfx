#ifndef ___LIN2_FUNC_H_
#define ___LIN2_FUNC_H_


#include "GrpX.h"


#ifdef __cplusplus
extern "C" {
#endif
	
	atVOID __atGRPX_DrawLine1( atGRPX_CANVAS *lpCanvas, atINT StartX, atINT StartY, atINT EndX, atINT EndY,	atGRPX_COLOR Color );
	atVOID __atGRPX_DrawLine1_REAL( atGRPX_CANVAS *lpCanvas, atREAL StartX, atREAL StartY, atREAL EndX, atREAL EndY, atGRPX_COLOR Color );
	
#ifdef __cplusplus
}
#endif


#endif
