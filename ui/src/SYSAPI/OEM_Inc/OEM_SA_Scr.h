#ifndef __OEM_SA_SCR_H_
#define __OEM_SA_SCR_H_


#include "SA_Types.h"
#include "SA_Scr.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atDWORD		FN_OEM_SYSAPI_GetDisplayCaps( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2, SYS_DISPLAY_CAPS *lpScrCaps );
typedef atBOOL		FN_OEM_SYSAPI_InitDisplay( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef atVOID		FN_OEM_SYSAPI_FinalDisplay( NOARGS );

typedef atBOOL 		FN_OEM_SYSAPI_SetDisplayResolution( atINT nWidth, atINT nHeight, atINT nFrameWidth );

typedef atINT 		FN_OEM_SYSAPI_GetScreenWidth( NOARGS );
typedef atINT 		FN_OEM_SYSAPI_GetScreenHeight( NOARGS );
typedef atINT 		FN_OEM_SYSAPI_GetScreenBPP( NOARGS );

typedef atINT       FN_OEM_SYSAPI_GetFrameVMemWidth( NOARGS );
typedef atLPVOID   	FN_OEM_SYSAPI_GetFrameBufferPtr( NOARGS );

typedef atVOID 	 	FN_OEM_SYSAPI_FlushScreenRgn( atINT nX, atINT nY, atINT nWidth, atINT nHeight );

typedef atULONG 	FN_OEM_SYSAPI_MakeColor(atBYTE r, atBYTE g, atBYTE b);
typedef atBYTE      FN_OEM_SYSAPI_GetRValue(atULONG nColor);
typedef atBYTE      FN_OEM_SYSAPI_GetGValue(atULONG nColor);
typedef atBYTE      FN_OEM_SYSAPI_GetBValue(atULONG nColor);

typedef atBOOL 		FN_OEM_SYSAPI_BeginDraw( NOARGS );
typedef atVOID 		FN_OEM_SYSAPI_EndDraw( NOARGS );

typedef atLONG		FN_OEM_SYSAPI_ExtGraphicMethod( atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 );


typedef struct tagOEM_SYSAPI_SCR_T {
	FN_OEM_SYSAPI_GetDisplayCaps			*fnGetDisplayCaps;
	FN_OEM_SYSAPI_InitDisplay				*fnInitDisplay;
	FN_OEM_SYSAPI_FinalDisplay				*fnFinalDisplay;

	FN_OEM_SYSAPI_SetDisplayResolution		*fnSetDisplayResolution;

	FN_OEM_SYSAPI_GetScreenWidth			*fnGetScreenWidth;
	FN_OEM_SYSAPI_GetScreenHeight			*fnGetScreenHeight;
	FN_OEM_SYSAPI_GetScreenBPP				*fnGetScreenBPP;

	FN_OEM_SYSAPI_GetFrameVMemWidth			*fnGetFrameVMemWidth;
	FN_OEM_SYSAPI_GetFrameBufferPtr			*fnGetFrameBufferPtr;

	FN_OEM_SYSAPI_FlushScreenRgn			*fnFlushScreenRgn;

	FN_OEM_SYSAPI_MakeColor					*fnMakeColor;
	FN_OEM_SYSAPI_GetRValue					*fnGetRValue;
	FN_OEM_SYSAPI_GetGValue					*fnGetGValue;
	FN_OEM_SYSAPI_GetBValue					*fnGetBValue;

	FN_OEM_SYSAPI_BeginDraw					*fnBeginDraw;
	FN_OEM_SYSAPI_EndDraw					*fnEndDraw;

	FN_OEM_SYSAPI_ExtGraphicMethod			*fnExtGraphicMethod;
} OEM_SYSAPI_SCR_T;


atVOID	OEM_SYSAPI_SCR_SetOemFn( OEM_SYSAPI_SCR_T *fnOem );
atVOID	OEM_SYSAPI_SCR_GetOemFn( OEM_SYSAPI_SCR_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif 

