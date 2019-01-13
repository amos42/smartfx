/**
  @file UI_Entry.h
  @brief UI엔진의 Entry Pointer
  @remark UI엔진의 Entry Pointer
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Entry.c
*/
#ifndef __UI_ENTRY_H_
#define __UI_ENTRY_H_


#ifndef __AT_TYPES_H_
#include "AT_Types.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atBOOL	atUIENG_Initialize( atUIAPI_INIT_DATA *lpInitData );
void   	atUIENG_Finalize( void );
atBOOL 	atUIENG_OnKeyDown(int nKeyCode);
atBOOL 	atUIENG_OnLongKeyDown(int nKeyCode);
atBOOL 	atUIENG_OnKeyUp(int nKeyCode);
atBOOL 	atUIENG_OnKeyChar(int nCharCode);
atBOOL 	atUIENG_OnSysKeyDown(int nVKeyCode);
void 	atUIENG_OnPaint(void);
void 	atUIENG_OnResize( int width, int height );
void 	atUIENG_OnMouseDown( int btn, int x, int y );
void 	atUIENG_OnMouseMove( int btn, int x, int y );
void 	atUIENG_OnMouseUp( int btn, int x, int y );
void		atUIENG_OnMouseLongClick( int btn, int x, int y );
void 	atUIENG_OnPause(void);
void 	atUIENG_OnResume(void);
void 	atUIENG_OnSystemNotify( long nParam1, long nParam2 );


#ifdef __cplusplus
}
#endif


#endif

