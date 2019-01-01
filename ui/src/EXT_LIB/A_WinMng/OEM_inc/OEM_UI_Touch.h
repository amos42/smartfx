/**
  @file OEM_UI_Touch.h
  @brief Touch, Mouse 등의 입력에 관한 포팅 OEM API 선언
  @remark Touch, Mouse 등의 입력에 관한 포팅 OEM API 선언
  @author Amos
  @date 2010-05-07 오전 18:15:00
  @see UI_Touch.h
*/
#ifndef __OEM_UI_TOUCH_H_
#define __OEM_UI_TOUCH_H_


#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef int		FN_OEM_UI_InitTouchDev( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef void	FN_OEM_UI_FinalTouchDev(void);

typedef atBOOL	FN_OEM_UI_GetMousePos( int *x, int *y );

typedef int		FN_OEM_UI_ConvOEMPtr2VPtr( int *x, int *y );
typedef int		FN_OEM_UI_ConvVPtr2OemPtr( int *x, int *y );


typedef struct tagOEM_UI_TOUCH {
	FN_OEM_UI_InitTouchDev		*fnInitTouchDev;
	FN_OEM_UI_FinalTouchDev		*fnFinalTouchDev;

	FN_OEM_UI_GetMousePos		*fnGetMousePos;

	FN_OEM_UI_ConvOEMPtr2VPtr	*fnConvOEMPtr2VPtr;
	FN_OEM_UI_ConvVPtr2OemPtr	*fnConvVPtr2OemPtr;
} OEM_UI_TOUCH_T;


atVOID	OEM_UI_Touch_SetOemFn( OEM_UI_TOUCH_T *fnOem );
atVOID	OEM_UI_Touch_GetOemFn( OEM_UI_TOUCH_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif 

