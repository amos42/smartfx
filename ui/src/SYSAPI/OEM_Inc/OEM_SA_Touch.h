#ifndef __OEM_SA_TOUCH_H_
#define __OEM_SA_TOUCH_H_


#include "SA_Types.h"
#include "SA_Touch.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef int		FN_OEM_SYSAPI_InitTouchDev( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef void	FN_OEM_SYSAPI_FinalTouchDev(void);

typedef void	FN_OEM_SYSAPI_SetTouchNotifyFunc( SYS_FUNC_TOUCHNOTIFY *fnInputFunc );

typedef atBOOL	FN_OEM_SYSAPI_GetMousePos( int *x, int *y );

typedef int		FN_OEM_SYSAPI_ConvOEMPtr2VPtr( int *x, int *y );
typedef int		FN_OEM_SYSAPI_ConvVPtr2OemPtr( int *x, int *y );


typedef struct tagOEM_SYSAPI_TOUCH_T {
	FN_OEM_SYSAPI_InitTouchDev			*fnInitTouchDev;
	FN_OEM_SYSAPI_FinalTouchDev			*fnFinalTouchDev;

	FN_OEM_SYSAPI_SetTouchNotifyFunc	*fnSetTouchNotifyFunc;

	FN_OEM_SYSAPI_GetMousePos			*fnGetMousePos;

	FN_OEM_SYSAPI_ConvOEMPtr2VPtr		*fnConvOEMPtr2VPtr;
	FN_OEM_SYSAPI_ConvVPtr2OemPtr		*fnConvVPtr2OemPtr;
} OEM_SYSAPI_TOUCH_T;


atVOID	OEM_SYSAPI_TOUCH_SetOemFn( OEM_SYSAPI_TOUCH_T *fnOem );
atVOID	OEM_SYSAPI_TOUCH_GetOemFn( OEM_SYSAPI_TOUCH_T *fnOem );



#ifdef __cplusplus
}
#endif


#endif 

