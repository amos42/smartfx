/**
  @file Window.h
  @brief Window�� ���� API
  @remark Window�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see Window.inl Window.c WinObj.h Control.h
*/
#ifndef __WINDOW_H_
#define __WINDOW_H_

#include "UI_Types.h"
#include "WinObj.h"
#include "Control.h"
#include "CtlMng.h"
#include "UI_API/UI_API.h"
#include "UI_Util.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 @brief Window�� �̺�Ʈ �޽�����.
 @see OBJMSG
*/
typedef enum {
	WNDMSG_NULL 			= OBJMSG_NULL,
	WNDMSG_DUMMY 			= OBJMSG_DUMMY,

	WNDMSG_INIT 			= OBJMSG_INIT,				/**< @see OBJMSG_INIT */
	WNDMSG_RELEASE 			= OBJMSG_RELEASE,            /**< @see OBJMSG_RELEASE */

	WNDMSG_REQ_DESTROY		= OBJMSG_REQ_DESTROY,		/**< @see OBJMSG_REQ_DESTROY */
	WNDMSG_PREDESTROY		= OBJMSG_PREDESTROY,        /**< @see OBJMSG_PREDESTROY */
	WNDMSG_DESTROY		= OBJMSG_DESTROY,               /**< @see OBJMSG_DESTROY */

	WNDMSG_REGIST			= OBJMSG_REGIST,            /**< @see OBJMSG_REGIST */
	WNDMSG_SHOW				= OBJMSG_SHOW,				/**< @see OBJMSG_SHOW */
	WNDMSG_HIDE				= OBJMSG_HIDE,              /**< @see OBJMSG_HIDE */
	WNDMSG_CHANGEPOS		= OBJMSG_CHANGEPOS,         /**< @see OBJMSG_CHANGEPOS */
	WNDMSG_PRERESIZE			= OBJMSG_PRERESIZE,     /**< @see OBJMSG_PRERESIZE */
	WNDMSG_RESIZE			= OBJMSG_RESIZE,            /**< @see OBJMSG_RESIZE */
	WNDMSG_CHANGELAYOUT		= OBJMSG_CHANGELAYOUT,      /**< @see OBJMSG_CHANGELAYOUT */

	WNDMSG_PREPAINT			= OBJMSG_PREPAINT,			/**< @see OBJMSG_PREPAINT */
	WNDMSG_PAINTLAYOUT		= OBJMSG_PAINTLAYOUT,       /**< @see OBJMSG_PAINTLAYOUT */
	WNDMSG_PAINT			= OBJMSG_PAINT,             /**< @see OBJMSG_PAINT */
	WNDMSG_PAINTOBJS		= OBJMSG_PAINTOBJS,         /**< @see OBJMSG_PAINTOBJS */
	WNDMSG_POSTPAINT		= OBJMSG_POSTPAINT,         /**< @see OBJMSG_POSTPAINT */

	WNDMSG_PAUSE			= OBJMSG_PAUSE,				/**< @see OBJMSG_PAUSE */
	WNDMSG_RESUME			= OBJMSG_RESUME,            /**< @see OBJMSG_RESUME */

	WNDMSG_GOTFOCUS			= OBJMSG_GOTFOCUS,			/**< @see OBJMSG_GOTFOCUS */
	WNDMSG_LOSTFOCUS		= OBJMSG_LOSTFOCUS,         /**< @see OBJMSG_LOSTFOCUS */

	WNDMSG_TIMER			= OBJMSG_TIMER,				/**< @see OBJMSG_TIMER */
	WNDMSG_SYSNOTIFY		= OBJMSG_SYSNOTIFY,         /**< @see OBJMSG_SYSNOTIFY */
	WNDMSG_COMMAND			= OBJMSG_COMMAND,           /**< @see OBJMSG_COMMAND */
	WNDMSG_COMMAND_EX		= OBJMSG_COMMAND_EX,        /**< @see OBJMSG_COMMAND_EX */
	WNDMSG_CHANGE			= OBJMSG_CHANGE,            /**< @see OBJMSG_CHANGE */

	WNDMSG_KEYDOWN			= OBJMSG_KEYDOWN,			/**< @see OBJMSG_KEYDOWN */
	WNDMSG_KEYPRESS			= OBJMSG_KEYPRESS,          /**< @see OBJMSG_KEYPRESS */
	WNDMSG_LONGKEYDOWN		= OBJMSG_LONGKEYDOWN,       /**< @see OBJMSG_LONGKEYDOWN */
	WNDMSG_KEYUP			= OBJMSG_KEYUP,             /**< @see OBJMSG_KEYUP */
	WNDMSG_KEYCHAR		= OBJMSG_KEYCHAR,               /**< @see OBJMSG_KEYCHAR */
	WNDMSG_SYSKEYDOWN	= OBJMSG_SYSKEYDOWN,            /**< @see OBJMSG_SYSKEYDOWN */

	WNDMSG_PREMOUSEDOWN		= OBJMSG_PREMOUSEDOWN,		/**< @see OBJMSG_PREMOUSEDOWN */
	WNDMSG_MOUSEDOWN		= OBJMSG_MOUSEDOWN,         /**< @see OBJMSG_MOUSEDOWN */
	WNDMSG_MOUSEUP			= OBJMSG_MOUSEUP,           /**< @see OBJMSG_MOUSEUP */
	WNDMSG_MOUSEMOVE		= OBJMSG_MOUSEMOVE,         /**< @see OBJMSG_MOUSEMOVE */
	WNDMSG_MOUSECLICK		= OBJMSG_MOUSECLICK,        /**< @see OBJMSG_MOUSECLICK */
	WNDMSG_PREMOUSELONGCLICK	= OBJMSG_PREMOUSELONGCLICK,	/**< @see OBJMSG_PREMOUSELONGCLICK */
	WNDMSG_MOUSELONGCLICK	= OBJMSG_MOUSELONGCLICK,        /**< @see OBJMSG_MOUSELONGCLICK */

	WNDMSG_CHANGECONTROLFOCUS = OBJMSG_CHANGECHILDOBJFOCUS,				/**< @see OBJMSG_CHANGECHILDOBJFOCUS */
	WNDMSG_REQ_CHANGECONTROLFOCUS = OBJMSG_REQ_CHANGECHILDOBJFOCUS,     /**< @see OBJMSG_REQ_CHANGECHILDOBJFOCUS */

	WNDMSG_MAX,

	WNDMSG_USERDEF = OBJMSG_USERDEF						/**< @see OBJMSG_USERDEF */
} WNDMSG;


typedef atHWINOBJ atHWINDOW;
typedef atHANDLE	atHWINTIMER;


// event method
typedef atWINOBJPROC atWNDPROC;
typedef atWNDPROC* atLPWNDPROC;


#define	atWS_USERFILTER		0x00FFFFFF

typedef enum {
	atWS_NONE = 0,
	atWS_NONEFRAME,
	atWS_FRAMEWINDOW,
	atWS_SINGLEFRAME,
	atWS_POPUPWINDOW,
	atWS_USERDEFINE
} atWINDOWSTYLE;

#define atWS_NONECLIENT 	0x1000
#define atWS_MODAL  	 	0x8000
#define atWS_STAYONTOP  	0x4000
#define atWS_UNFOCUSGRAY  	0x2000
#define atWS_TRANSPARENT	0x0800
#define atWS_NOFOCUSABLE	0x0400
#define atWS_NOTITLE		0x0200


#define	WINDOWTITLE_MAX_LENGTH	32


/**
 @brief Window ��ü

 Window�� ��ü ����ü

 @remark WINOBJ�� ��� �޾ұ⿡, �� �� �ʵ忡 WINOBJ ��ü�� ��ġ�Ѵ�. (C++�� ���¸� �䳻)
*/
typedef struct _tagatWINDOW {
	atWINOBJ			Base;			/**< Window, Control ���� �⺻ �Ӽ�. �ݵ�� �� �տ� �־�� �Ѵ�. */

//	atHAPP			hApp;
	atHANDLE		hApp;				/**< ���ø����̼��� �ڵ� */

	int				nWindowStyle;		/**< ������ ��Ÿ�� */
	atTCHAR			szWindowTitle[WINDOWTITLE_MAX_LENGTH];	/**< ������ Ÿ��Ʋ�� */
	atDWORD         	nTitleIDS;								/**< ������ Ÿ��Ʋ IDS */

	atHGDC			hClientGDC;			/**< Ŭ���̾�Ʈ�� GDC */

	void *			lpUserData;			/**< ������ �������� �ڵ� */

	atBOOL			bActivateLayout;		/**< ���̾ƿ� ��Ʈ���� ��� �����Ѱ� (Real Time) */
	atLAYOUTBITMAP   lpLayoutBm;			/**< ���̾ƿ� ��Ʈ�� */
} atWINDOW, * atLPWINDOW;


atHWINDOW		atWINDOW_CreateEx( atHWINDOW hOwnerWnd, atINT nID, int nStyle, atLPTSTR szWindowText,
											int nWidth, int nHeight, atMARGIN *lpLayoutMargin, atMARGIN *rtAnchor,
											atDWORD dwUserDataSize, atLPWNDPROC fnWndProc,
											atLAYOUTBITMAP *lpLayoutBm,
											void *lpCreateParam, void *lpExData );
atHWINDOW		atWINDOW_Create( atHWINDOW hOwnerWnd, atINT nID, int nStyle, atLPTSTR szWindowText,
											int nStartX, int nStartY, int nWidth, int nHeight,
											atDWORD dwUserDataSize, atLPWNDPROC fnWndProc,
											void *lpCreateParam, void *lpExData );
void 			atWINDOW_Destroy( atHWINDOW hWnd );

int 			atWINDOW_DefWindowProc( atHWINDOW hWnd, int nMsg, long lParam1, long lParam2 );

void			atWINDOW_SetTitleText(atHWINDOW hWnd, atLPTSTR szWindowText);
void			atWINDOW_SetTitleIDS(atHWINDOW hWnd, atDWORD ids);
atLPTSTR			atWINDOW_GetTitleText(atHWINDOW hWnd);

atHGDC 			atWINDOW_GetClientGDC(atHWINDOW hWnd);
atBOOL 			atWINDOW_GetLayoutBitmap( atHWINDOW hWnd,atLAYOUTBITMAP *lpLayoutBitmap );
atBOOL 			atWINDOW_SetLayoutBitmap( atHWINDOW hWnd,
										atHBITMAP hBmLayoutTopLeft, atHBITMAP hBmLayoutTopCenter, atHBITMAP hBmLayoutTopRight,
										atHBITMAP hBmLayoutMiddleLeft, atHBITMAP hBmLayoutMiddleCenter, atHBITMAP hBmLayoutMiddleRight,
										atHBITMAP hBmLayoutBottomLeft, atHBITMAP hBmLayoutBottomCenter, atHBITMAP hBmLayoutBottomRight );
atBOOL 			atWINDOW_SetLayoutBitmap2( atHWINDOW hWnd, atLAYOUTBITMAP *lpLayoutBitmap );

void			atWINDOW_DrawClient( atHWINDOW hWnd, atBOOL bFocused );
void 			atWINDOW_DrawLayout( atHWINDOW hWnd );

void 			atWINDOW_ScreenToClient( atHWINDOW hWnd, int *x, int *y );
void 			atWINDOW_ClientToScreen( atHWINDOW hWnd, int *x, int *y );
void 			atWINDOW_ClientToScreen2( atHWINDOW hWnd, int *x1, int *y1, int *x2, int *y2 );

atBOOL 			atWINDOW_AddControl( atHWINDOW hWnd, atHCONTROL hControl );
void 			atWINDOW_RemoveControl( atHWINDOW hWnd, atHCONTROL hControl );

int 			atWINDOW_SendMessage( atHWINDOW hWnd, int nMsg, long lParam1, long lParam2 );
int 			atWINDOW_PostMessage( atHWINDOW hWnd, int nMsg, long lParam1, long lParam2 );



#ifdef __cplusplus
}
#endif


#include "Window.inl"


#endif // _MYWINDOW_H_INCLUDED
