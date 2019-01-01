/**
  @file Control.h
  @brief Control에 관한 API
  @remark Control.c의 인터페이스
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see Control.c WinObj.h
*/
#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "UI_Types.h"
#include "WinObj.h"
//#include "CtlMng.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    CTL_PARENTTYPE_NONE,
	CTL_PARENTTYPE_WINDOW,
	CTL_PARENTTYPE_CONTROL
} CTLPARENTYPE;

/**
 @brief Control의 이벤트 메시지들.
 @see OBJMSG
*/
typedef enum {
	CTLMSG_NULL 			= OBJMSG_NULL,
	CTLMSG_DUMMY 			= OBJMSG_DUMMY,

	CTLMSG_INIT 			= OBJMSG_INIT,				/**< @see OBJMSG_INIT */
	CTLMSG_RELEASE 		= OBJMSG_RELEASE,                /**< @see OBJMSG_RELEASE */

	CTLMSG_REQ_DESTROY		= OBJMSG_REQ_DESTROY,		 /**< @see OBJMSG_REQ_DESTROY */
	CTLMSG_PREDESTROY		= OBJMSG_PREDESTROY,         /**< @see OBJMSG_PREDESTROY */
	CTLMSG_DESTROY		= OBJMSG_DESTROY,				/**< @see OBJMSG_DESTROY */

	CTLMSG_SHOW			= OBJMSG_SHOW,          		/**< @see OBJMSG_SHOW */
	CTLMSG_HIDE			= OBJMSG_HIDE,          		/**< @see OBJMSG_HIDE */
	CTLMSG_CHANGEPOS		= OBJMSG_CHANGEPOS,     	/**< @see OBJMSG_CHANGEPOS */
	CTLMSG_PRERESIZE			= OBJMSG_PRERESIZE,     /**< @see OBJMSG_PRERESIZE */
	CTLMSG_RESIZE			= OBJMSG_RESIZE,            /**< @see OBJMSG_RESIZE */
	CTLMSG_CHANGELAYOUT	= OBJMSG_CHANGELAYOUT,        	/**< @see OBJMSG_CHANGELAYOUT */

	CTLMSG_PREPAINT			= OBJMSG_PREPAINT,     		/**< @see OBJMSG_PREPAINT */
	CTLMSG_PAINTLAYOUT		= OBJMSG_PAINTLAYOUT,		/**< @see OBJMSG_PAINTLAYOUT */
	CTLMSG_PAINT			= OBJMSG_PAINT,             /**< @see OBJMSG_PAINT */
	CTLMSG_PAINTOBJS		= OBJMSG_PAINTOBJS,         /**< @see OBJMSG_PAINTOBJS */
	CTLMSG_POSTPAINT		= OBJMSG_POSTPAINT,         /**< @see OBJMSG_POSTPAINT */

	CTLMSG_SUSPEND		= OBJMSG_PAUSE,					/**< @see OBJMSG_PAUSE */
	CTLMSG_RESUME			= OBJMSG_RESUME,            /**< @see OBJMSG_RESUME */

	CTLMSG_GOTFOCUS		= OBJMSG_GOTFOCUS,				/**< @see OBJMSG_GOTFOCUS */
	CTLMSG_LOSTFOCUS		= OBJMSG_LOSTFOCUS,         /**< @see OBJMSG_LOSTFOCUS */

	CTLMSG_TIMER			= OBJMSG_TIMER,				/**< @see OBJMSG_TIMER */
	CTLMSG_COMMAND		= OBJMSG_COMMAND,				/**< @see OBJMSG_COMMAND */
	CTLMSG_COMMAND_EX	= OBJMSG_COMMAND_EX,	        /**< @see OBJMSG_COMMAND_EX */
	CTLMSG_CHANGE			= OBJMSG_CHANGE,            /**< @see OBJMSG_CHANGE */

	CTLMSG_KEYDOWN		= OBJMSG_KEYDOWN,				/**< @see OBJMSG_KEYDOWN */
	CTLMSG_KEYPRESS		= OBJMSG_KEYPRESS,              /**< @see OBJMSG_KEYPRESS */
	CTLMSG_LONGKEYDOWN	= OBJMSG_LONGKEYDOWN,           /**< @see OBJMSG_LONGKEYDOWN */
	CTLMSG_KEYUP			= OBJMSG_KEYUP,             /**< @see OBJMSG_KEYUP */
	CTLMSG_KEYCHAR		= OBJMSG_KEYCHAR,               /**< @see OBJMSG_KEYCHAR */
	CTLMSG_SYSKEYDOWN	= OBJMSG_SYSKEYDOWN,            /**< @see OBJMSG_SYSKEYDOWN */

	CTLMSG_PREMOUSEDOWN	= OBJMSG_PREMOUSEDOWN,			/**< @see OBJMSG_PREMOUSEDOWN */
	CTLMSG_MOUSEDOWN	= OBJMSG_MOUSEDOWN,             /**< @see OBJMSG_MOUSEDOWN */
	CTLMSG_MOUSEUP		= OBJMSG_MOUSEUP,               /**< @see OBJMSG_MOUSEUP */
	CTLMSG_MOUSEMOVE		= OBJMSG_MOUSEMOVE,         /**< @see OBJMSG_MOUSEMOVE */
	CTLMSG_MOUSECLICK		= OBJMSG_MOUSECLICK,        /**< @see OBJMSG_MOUSECLICK */
	CTLMSG_PREMOUSELONGCLICK	= OBJMSG_PREMOUSELONGCLICK,	/**< @see OBJMSG_PREMOUSELONGCLICK */
	CTLMSG_MOUSELONGCLICK		= OBJMSG_MOUSELONGCLICK,    /**< @see OBJMSG_MOUSELONGCLICK */

	CTLMSG_CHANGECONTROLFOCUS = OBJMSG_CHANGECHILDOBJFOCUS,           /**< @see OBJMSG_CHANGECHILDOBJFOCUS */
	CTLMSG_REQ_CHANGECONTROLFOCUS = OBJMSG_REQ_CHANGECHILDOBJFOCUS,   /**< @see OBJMSG_REQ_CHANGECHILDOBJFOCUS */

	CTLMSG_MAX,

	CTLMSG_USERDEF = OBJMSG_USERDEF						/**< @see OBJMSG_USERDEF */
} CTLMSG;


typedef atHWINOBJ atHCONTROL;		/**< Control 핸들 */
typedef atHANDLE	atHCTLTIMER;	/**< Control Timer 핸들 */


typedef atWINOBJPROC atCTRLPROC;	/**< Control의 핸들러 타입 */
typedef atCTRLPROC* atLPCTRLPROC;	/**< Control의 핸들러 포인터 타입 */


/** @brief Control의 컬러 셋 지정 플랫 */
#define atCONTROL_COLORFLAG_NORMAL_FORE_COLOR 	(0x1001)
#define atCONTROL_COLORFLAG_NORMAL_TEXT_COLOR	(0x1002)
#define atCONTROL_COLORFLAG_NORMAL_BACK_COLOR	(0x1004)
#define atCONTROL_COLORFLAG_NORMAL_ALL			(atCONTROL_COLORFLAG_NORMAL_FORE_COLOR | atCONTROL_COLORFLAG_NORMAL_TEXT_COLOR | atCONTROL_COLORFLAG_NORMAL_BACK_COLOR)
#define atCONTROL_COLORFLAG_SELECT_FORE_COLOR 	(0x2001)
#define atCONTROL_COLORFLAG_SELECT_TEXT_COLOR	(0x2002)
#define atCONTROL_COLORFLAG_SELECT_BACK_COLOR	(0x2004)
#define atCONTROL_COLORFLAG_SELECT_ALL			(atCONTROL_COLORFLAG_SELECT_FORE_COLOR | atCONTROL_COLORFLAG_SELECT_TEXT_COLOR | atCONTROL_COLORFLAG_SELECT_BACK_COLOR)
#define atCONTROL_COLORFLAG_DISABLE_FORE_COLOR 	(0x4001)
#define atCONTROL_COLORFLAG_DISABLE_TEXT_COLOR	(0x4002)
#define atCONTROL_COLORFLAG_DISABLE_BACK_COLOR	(0x4004)
#define atCONTROL_COLORFLAG_DISABLE_ALL			(atCONTROL_COLORFLAG_DISABLE_FORE_COLOR | atCONTROL_COLORFLAG_DISABLE_TEXT_COLOR | atCONTROL_COLORFLAG_DISABLE_BACK_COLOR)
#define atCONTROL_COLORFLAG_ALL					(atCONTROL_COLORFLAG_NORMAL_ALL | atCONTROL_COLORFLAG_SELECT_ALL | atCONTROL_COLORFLAG_DISABLE_ALL)

/**
 @brief Control의 컬러 셋
*/
typedef struct _tagatCONTROL_COLOR {
	atCOLOR 			clNormalFrColor, clNormalTxtColor, clNormalBgColor;		/**< 일반적인 컬러 */
	atCOLOR 			clFocusedFrColor, clFocusedTxtColor, clFocusedBgColor;	/**< 포커스를 가질 경우 컬러 */
	atCOLOR 			clDisableFrColor, clDisableTxtColor, clDisableBgColor;	/**< Disable 상태의 컬러 */
} atCONTROL_COLOR;

#define atCS_TRANSPARENT	0x0800
#define atCS_DRAW_BORDER	0x0400

/**
 @brief Control 개체

 Control의 개체 구조체

 @remark WINOBJ를 상속 받았기에, 맨 앞 필드에 WINOBJ 개체가 위치한다. (C++의 형태를 흉내)
*/
typedef struct _tagatCONTROL {
	atWINOBJ			Base;			/**< Window, Control 등의 기본 속성. 반드시 맨 앞에 있어야 한다. */

	atCONTROL_COLOR		tControlColor;		/**< 컬러 세트 */
	atHFONT				hFont;			/**< 내장 폰트 */

	atUILIST			lstProperties;
	atUILIST			lstMethods;

	void 			   *lpUserData;		/**< 사용자 데이터 */
} atCONTROL, *atLPCONTROL;


atHCONTROL		atCONTROL_Create( int nID, int nX, int nY, int nWidth, int nHeight, int attr, atDWORD dwUserDataSize, atLPCTRLPROC fnCtlProc, void *lpCreateParam );
atHCONTROL		atCONTROL_CreateWithAnchor( int nID, int nWidth, int nHeight, atMARGIN *rtAnchor, int attr, atDWORD dwUserDataSize, atLPCTRLPROC fnCtlProc, void *lpCreateParam );
void            atCONTROL_Destroy(atHCONTROL hControl);
atBOOL 		atCONTROL_GetControlColor( atHCONTROL hControl, atCONTROL_COLOR *lpCtlColor );
atBOOL 		atCONTROL_SetControlColor( atHCONTROL hControl, atCONTROL_COLOR *lpCtlColor );
atBOOL		atCONTROL_SetControlColorEx( atHCONTROL hControl, atDWORD dwChangeFlag, atCONTROL_COLOR *lpCtlColor );
atINT		atCONTROL_PostMessageToParent( atHCONTROL hControl, int msg, long param1, long param2 );
atHFONT         atCONTROL_SetFont(atHCONTROL hControl, atHFONT hFont);
atHFONT         atCONTROL_GetFont(atHCONTROL hControl);
atHGDC 			atCONTROL_GetGDC(atHCONTROL hControl);
//void            atCONTROL_SetFocusable( atHCONTROL hControl, atBOOL bFocusable);
void		atCONTROL_PrepareGDC( atHCONTROL hControl, atHGDC hCanvas, atBOOL bFocused );

void 		atCONTROL_GetPosition( atHCONTROL hControl, int *nX, int *nY );
void			atCONTROL_SetSize( atHCONTROL hControl, int nWidth, int nHeight );
void 		atCONTROL_IncSize( atHCONTROL hControl, int ndX, int ndY );

int             atCONTROL_DefControlProc( atHCONTROL hControl, int nMsg, long lParam1, long lParam2 );
void            atCONTROL_SetUserDataPtr( atHCONTROL hControl, void *lpUserData );
void *          atCONTROL_GetUserDataPtr( atHCONTROL hControl );


#ifdef __cplusplus
}
#endif


#include "Control.inl"


#endif // __CONTROL_H_

