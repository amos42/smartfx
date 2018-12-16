/**
  @file UI_Kern.h
  @brief UI Kernel API
  @remark UI Kernel API
  @author Amos
  @date 2010-04-19 ¿ÀÀü 11:41:00
  @see UI_Kern.c
*/
#ifndef __UI_KERN_H_
#define __UI_KERN_H_


#ifndef __AT_TYPES_H_
#include "AT_Types.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atBOOL  atKERNEL_Initialize( atUIAPI_INIT_DATA *lpInitData );
void    atKERNEL_Finalize( void );
void	atKERNEL_LockScreen(void);
void	atKERNEL_UnlockScreen(void);
void	atKERNEL_LockInput(void);
void	atKERNEL_UnlockInput(void);
//atBOOL atKERNEL_CaptureMouse( atHANDLE hTargetObj );
//void    atKERNEL_ResetKey(void);
void    atKERNEL_KeyDown(int nVKey);
void    atKERNEL_KeyPress(int nVKey);
void    atKERNEL_LongKeyDown(int nVKey);
void    atKERNEL_KeyUp(int nVKey);
void    atKERNEL_KeyChar(int nCharKey);
void    atKERNEL_SysKeyDown(int nVKey);
void    atKERNEL_Paint(void);
void    atKERNEL_Resize( int width, int height );
void    atKERNEL_MouseDown( int btn, int x, int y );
void    atKERNEL_MouseMove( int btn, int x, int y );
void    atKERNEL_MouseUp( int btn, int x, int y );
void    atKERNEL_MouseLongClick( int btn, int x, int y );
void    atKERNEL_Pause(void);
void    atKERNEL_Resume(void);
void    atKERNEL_SystemNotify( long nParam1, long nParam2 );
void	atKERNEL_Tick( long nTick );

extern atBOOL g_bIsUIEngInitialize;


#ifdef __cplusplus
}
#endif


#endif

