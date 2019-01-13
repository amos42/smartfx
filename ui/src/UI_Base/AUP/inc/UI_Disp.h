/**
  @file UI_Disp.h
  @brief UI엔진의 디스플레이 프로세스
  @remark UI엔진의 디스플레이 프로세스
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Disp.c
*/
#ifndef __UI_DISP_H_
#define __UI_DISP_H_


#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif
	
#define EVENT_DISP_SCREEN_LOCKED		(1)
#define EVENT_DISP_SCREEN_UNLOCKED		(2)
#define EVENT_DISP_INVALID_ADDED		(3)

typedef void (atDISPLISTENER)(int event);
typedef atDISPLISTENER * atLPDISPLISTENER;


atBOOL  	    atUIAPI_InitDisplayProcessor(void);
atVOID  	    atUIAPI_FinalDisplayProcessor(void);

atLPDISPLISTENER atUIAPI_SetDisplayListener(atLPDISPLISTENER lpDispFunc);

atBOOL		atUIAPI_TickDispProcess(void);

atVOID		atUIAPI_LockDrawClipArea( atRECT *lpClipRect, atBOOL bForce );
atVOID		atUIAPI_UnlockDrawClipArea( NOARGS );
atVOID		atUIAPI_ResetDrawClipArea( NOARGS );

atVOID       atUIAPI_AddInvalideAreaAll( NOARGS );
atVOID       atUIAPI_AddInvalideArea( atREGION *lpRegion );
atVOID       atUIAPI_AddInvalideRect( atRECT *lpRect );

atVOID       atUIAPI_ClearInvalideArea(void);

atVOID       atUIAPI_DisplayInvalideArea(void);
atVOID 	    atUIAPI_DisplayAll(void);

atVOID       atUIAPI_DisplayForceAll(void);


#ifdef __cplusplus
}
#endif


#endif
