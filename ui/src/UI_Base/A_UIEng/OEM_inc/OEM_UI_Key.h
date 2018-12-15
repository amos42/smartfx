/**
  @file OEM_UI_Key.h
  @brief Key, Mouse 등의 입력에 관한 포팅 OEM API 선언
  @remark Key, Mouse 등의 입력에 관한 포팅 OEM API 선언
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Key.h
*/
#ifndef __OEM_UI_KEY_H_
#define __OEM_UI_KEY_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef int		FN_OEM_UI_InitInputDev( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef void	FN_OEM_UI_FinalInputDev(void);

typedef atBOOL	FN_OEM_UI_GetMousePos( int *x, int *y );

typedef int		FN_OEM_UI_ConvOEMKey2VKey( int OEMKey );
typedef int		FN_OEM_UI_ConvVKey2OEMKey( int VKey );
typedef int		FN_OEM_UI_ConvOEMPtr2VPtr( int *x, int *y );
typedef int		FN_OEM_UI_ConvVPtr2OemPtr( int *x, int *y );


typedef struct tagOEM_UI_KEY {
	FN_OEM_UI_InitInputDev		*fnInitInputDev;
	FN_OEM_UI_FinalInputDev		*fnFinalInputDev;

	FN_OEM_UI_GetMousePos		*fnGetMousePos;

	FN_OEM_UI_ConvOEMKey2VKey	*fnConvOEMKey2VKey;
	FN_OEM_UI_ConvVKey2OEMKey	*fnConvVKey2OEMKey;
	FN_OEM_UI_ConvOEMPtr2VPtr	*fnConvOEMPtr2VPtr;
	FN_OEM_UI_ConvVPtr2OemPtr	*fnConvVPtr2OemPtr;
} OEM_UI_KEY_T;


atVOID	OEM_UI_Key_SetOemFn( OEM_UI_KEY_T *fnOem );
atVOID	OEM_UI_Key_GetOemFn( OEM_UI_KEY_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif //  _KEYPAD_H_INCLUDED
