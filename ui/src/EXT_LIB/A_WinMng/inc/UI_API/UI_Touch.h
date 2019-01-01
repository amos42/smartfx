/**
  @file UI_Touch.h
  @brief Touch/Mouse ��, �Է¿� ���� API
  @remark Touch/Mouse ��, �Է¿� ���� API
  @author Amos
  @date 2010-05-07 18:07:00
  @see UI_Touch.c
*/
#ifndef __UI_TOUCH_H_
#define __UI_TOUCH_H_


#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


atBOOL 	atUIAPI_InitTouchDev( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
void 	atUIAPI_FinalTouchDev(void);

atBOOL	atUIAPI_GetMousePos( int *x, int *y );

int		atUIAPI_ConvOEMPtr2VPtr( int *x, int *y );
int		atUIAPI_ConvVPtr2OemPtr( int *x, int *y );



#ifdef __cplusplus
}
#endif


#endif

