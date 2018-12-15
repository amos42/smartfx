#ifndef __SA_SCR_H_
#define __SA_SCR_H_


#include "SA_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atU32		SYS_COLOR;


typedef struct tagSYS_DEVICE_CAPS {
	atDWORD	dwAttr;
	atINT		nX_DPI, nY_DPI;
} SYS_DISPLAY_CAPS;


atDWORD			SYSAPI_GetDisplayCaps( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2, SYS_DISPLAY_CAPS *lpScrCaps );
atBOOL			SYSAPI_InitDisplay( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
atVOID			SYSAPI_FinalDisplay( NOARGS );

atBOOL 			SYSAPI_SetDisplayResolution( atINT nWidth, atINT nHeight, atINT nFrameWidth );

SYS_COLOR		SYSAPI_MakeColor(atBYTE r, atBYTE g, atBYTE b);
atBYTE			SYSAPI_GetRValue(SYS_COLOR nColor);
atBYTE			SYSAPI_GetGValue(SYS_COLOR nColor);
atBYTE			SYSAPI_GetBValue(SYS_COLOR nColor);

atINT			SYSAPI_GetScreenWidth( NOARGS );
atINT			SYSAPI_GetScreenHeight( NOARGS );
atINT			SYSAPI_GetScreenBPP( NOARGS );

atINT			SYSAPI_GetFrameVMemWidth( NOARGS );
atLPVOID		SYSAPI_GetFrameBufferPtr( NOARGS );

SYS_COLOR		SYSAPI_ConvColorFrom24BPP(SYS_COLOR nColor);
SYS_COLOR		SYSAPI_ConvColorFrom24iBPP(SYS_COLOR nColor);
SYS_COLOR		SYSAPI_ConvColorFrom16BPP(SYS_COLOR nColor);
SYS_COLOR		SYSAPI_ConvColorTo24BPP(SYS_COLOR nColor);
SYS_COLOR		SYSAPI_ConvColorTo24iBPP(SYS_COLOR nColor);
SYS_COLOR		SYSAPI_ConvColorTo16BPP(SYS_COLOR nColor);

atBOOL			SYSAPI_BeginDraw( NOARGS );
atVOID			SYSAPI_EndDraw( NOARGS );

atVOID			SYSAPI_FlushScreenRgn( atINT nX, atINT nY, atINT nWidth, atINT nHeight );
atVOID			SYSAPI_FlushScreen( NOARGS );

atLONG			SYSAPI_ExtGraphicMethod( atINT func_no, atLONG param1, atLONG param2, atLONG param3, atLONG param4, atLONG param5, atLONG param6 );


#ifdef __cplusplus
}
#endif


#endif // __SA_SCR_H_
