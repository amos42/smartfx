/**
  @file UI_Desk.h
  @brief 엔진의 기본 데스크탑 화면
  @remark 엔진의 기본 데스크탑 화면
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Desk.c
*/
#ifndef __UI_DESK_H_
#define __UI_DESK_H_


#include "UI_Types.h"
#include "UI_API/UI_API.h"
#include "SoftKey.h"
#include "AnnBar.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	atDESKTOP_AREA_ALL,
	atDESKTOP_AREA_CLIENT,
	atDESKTOP_AREA_ANNBAR,
	atDESKTOP_AREA_SOFTKEY,
	atDESKTOP_AREA_EMBEDED_ANNBAR,
	atDESKTOP_AREA_EMBEDED_SOFTKEY
} atDESKTOP_AREA;

atVOID		atDESKTOP_InitDesktop( atDWORD dwAttr );
atVOID		atDESKTOP_FinalDesktop( NOARGS );
atVOID		atDESKTOP_SetDesktopArea( atINT nX, atINT nY, atINT nWidth, atINT nHeight );
atVOID		atDESKTOP_RecalcDesktopArea( NOARGS );
atVOID 		atDESKTOP_GetDesktopArea( atINT nAreaType, atREGION *lpRegion );
atVOID 		atDESKTOP_SetDesktopColor( atCOLOR clBgColor );
atVOID		atDESKTOP_SetDesktopBitmap( atHBITMAP hBitmap );
atVOID		atDESKTOP_FlushDesktopArea( atINT nAreaType );
atVOID		atDESKTOP_DrawDesktopBackground( NOARGS );
atVOID		atDESKTOP_DrawDesktop( NOARGS );


atVOID		atDESKTOP_Refresh( atINT nAreaType );


#ifdef __cplusplus
}
#endif


#endif
