#ifndef __SA_TOUCH_H_
#define __SA_TOUCH_H_


#include "SA_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
	SYS_MSG_TOUCHDOWN,
	SYS_MSG_TOUCHMOVE,
	SYS_MSG_TOUCHUP,
	SYS_MSG_LONGCLICK
} SYS_MSG_TOUCH;

typedef int (SYS_FUNC_TOUCHNOTIFY)( int msg, long lParam1, long lParam2 );


atBOOL 	SYSAPI_InitTouchDev( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
void 	SYSAPI_FinalTouchDev(void);

void	SYSAPI_SetTouchNotifyFunc( SYS_FUNC_TOUCHNOTIFY *fnInputFunc );

atBOOL	SYSAPI_GetMousePos( int *x, int *y );

int		SYSAPI_ConvOEMPtr2VPtr( int *x, int *y );
int		SYSAPI_ConvVPtr2OemPtr( int *x, int *y );



#ifdef __cplusplus
}
#endif


#endif 
