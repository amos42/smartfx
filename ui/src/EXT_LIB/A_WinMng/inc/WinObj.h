/**
  @file WinObj.h
  @brief WinObj에 관한 API
  @remark WinObj에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see WinObj.c
*/
#ifndef __WINOBJ_H_
#define __WINOBJ_H_

#include "UI_Types.h"
#include "UI_Event.h"
#include "GDC.h"
#include "UI_API/UI_API.h"
#include "A_UI_MSG.h"


#ifdef __cplusplus
extern "C" {
#endif

//#define __USES_NOTIFIER

#define	atWINOBJ_MAX_WINOBJ_CNT 	256				/**< WinObj의 최대 갯수 */
#define	atWINOBJ_OBJNAME_LEN		32				/**< WinObj 이름의 최대 길이 */
#define	atWINOBJ_CLASSNAME_LEN	32				/**< WinObj 클래스 이름의 최대 길이 */
#ifndef atSOFTKEY_TEXT_LEN
#define	atSOFTKEY_TEXT_LEN	32					/**< SoftKey 텍스트의 최대 길이 */
#endif


/**
 @brief WinObj 이벤트 메시지 
 @see atUIMSG
*/
typedef enum {
	OBJMSG_NULL         = atNULL,	
	OBJMSG_DUMMY        = atUIMSG_DUMMY,
		
	OBJMSG_INIT         = atUIMSG_INIT,					/**< @see atUIMSG_INIT */
	OBJMSG_RELEASE      = atUIMSG_RELEASE,			/**< @see atUIMSG_RELEASE */

	OBJMSG_REQ_DESTROY  = atUIMSG_REQ_DESTROY,	/**< @see atUIMSG_REQ_DESTROY */
	OBJMSG_PREDESTROY   = atUIMSG_PREDESTROY,		/**< @see atUIMSG_PREDESTROY */
	OBJMSG_DESTROY      = atUIMSG_DESTROY,			/**< @see atUIMSG_DESTROY */
	
	OBJMSG_REGIST     = atUIMSG_REGIST,				/**< @see atUIMSG_REGIST */
	OBJMSG_SHOW         = atUIMSG_SHOW,				/**< @see atUIMSG_SHOW */
	OBJMSG_HIDE         = atUIMSG_HIDE,				/**< @see atUIMSG_HIDE */
	OBJMSG_CHANGEPOS    = atUIMSG_CHANGEPOS,		/**< @see atUIMSG_CHANGEPOS */
	OBJMSG_PRERESIZE       = atUIMSG_PRERESIZE,		/**< @see atUIMSG_PRERESIZE */
	OBJMSG_RESIZE       = atUIMSG_RESIZE,				/**< @see atUIMSG_RESIZE */
	OBJMSG_CHANGELAYOUT = atUIMSG_CHANGELAYOUT,	/**< @see atUIMSG_CHANGELAYOUT */

	OBJMSG_PREPAINT     = atUIMSG_PREPAINT,			/**< @see atUIMSG_PREPAINT */
	OBJMSG_PAINTLAYOUT	= atUIMSG_PAINTLAYOUT,	/**< @see atUIMSG_PAINTLAYOUT */
	OBJMSG_PAINT        = atUIMSG_PAINT,				/**< @see atUIMSG_PAINT */
	OBJMSG_PAINTCHILD   = atUIMSG_PAINTCHILD,				/**< @see atUIMSG_PAINT */
	OBJMSG_PAINTOBJS	= atUIMSG_PAINTOBJS,			/**< @see atUIMSG_PAINTOBJS */
	OBJMSG_POSTPAINT    = atUIMSG_POSTPAINT,		/**< @see atUIMSG_POSTPAINT */

	OBJMSG_PAUSE        = atUIMSG_PAUSE,				/**< @see atUIMSG_PAUSE */
	OBJMSG_RESUME       = atUIMSG_RESUME,			/**< @see atUIMSG_RESUME */

	OBJMSG_GOTFOCUS     = atUIMSG_GOTFOCUS,		/**< @see atUIMSG_GOTFOCUS */
	OBJMSG_LOSTFOCUS    = atUIMSG_LOSTFOCUS		/**< @see atUIMSG_LOSTFOCUS */,

	OBJMSG_TIMER        = atUIMSG_TIMER,				/**< @see atUIMSG_TIMER */
	OBJMSG_SYSNOTIFY    = atUIMSG_SYSNOTIFY,		/**< @see atUIMSG_SYSNOTIFY */
	OBJMSG_COMMAND      = atUIMSG_COMMAND,		/**< @see atUIMSG_COMMAND */
	OBJMSG_COMMAND_EX      = atUIMSG_COMMAND_EX,	/**< @see atUIMSG_COMMAND_EX */
	OBJMSG_CHANGE       = atUIMSG_CHANGE,			/**< @see atUIMSG_CHANGE */

	OBJMSG_KEYDOWN      = atUIMSG_KEYDOWN,			/**< @see atUIMSG_KEYDOWN */
	OBJMSG_KEYPRESS     = atUIMSG_KEYPRESS,			/**< @see atUIMSG_KEYPRESS */
	OBJMSG_LONGKEYDOWN  = atUIMSG_LONGKEYDOWN,	/**< @see atUIMSG_LONGKEYDOWN */
	OBJMSG_KEYUP        = atUIMSG_KEYUP,				/**< @see atUIMSG_KEYUP */
	OBJMSG_KEYCHAR      = atUIMSG_KEYCHAR,			/**< @see atUIMSG_KEYCHAR */
	OBJMSG_SYSKEYDOWN   = atUIMSG_SYSKEYDOWN,	/**< @see atUIMSG_SYSKEYDOWN */

	OBJMSG_PREMOUSEDOWN    = atUIMSG_PREMOUSEDOWN,	/**< @see atUIMSG_PREMOUSEDOWN */
	OBJMSG_MOUSEDOWN    = atUIMSG_MOUSEDOWN,	/**< @see atUIMSG_MOUSEDOWN */
	OBJMSG_MOUSEUP      = atUIMSG_MOUSEUP,			/**< @see atUIMSG_MOUSEUP */
	OBJMSG_MOUSEMOVE    = atUIMSG_MOUSEMOVE,		/**< @see atUIMSG_MOUSEMOVE */
	OBJMSG_MOUSECLICK    = atUIMSG_MOUSECLICK,		/**< @see atUIMSG_MOUSECLICK */
	OBJMSG_PREMOUSELONGCLICK = atUIMSG_PREMOUSELONGCLICK,	/**< @see atUIMSG_PREMOUSELONGCLICK */
	OBJMSG_MOUSELONGCLICK    = atUIMSG_MOUSELONGCLICK,	/**< @see atUIMSG_MOUSELONGCLICK */

	OBJMSG_CHANGECHILDOBJFOCUS,					/**< Child 개체의 포커스가 변경됨  */
	OBJMSG_REQ_CHANGECHILDOBJFOCUS,				/**< Child 개체의 포커스의 변경을 요구함 */

	OBJMSG_MAX,
	
	OBJMSG_USERDEF      = atUIMSG_USERDEF			/**< @see atUIMSG_USERDEF */
} OBJMSG;


#define atOBJATTR_NONE  			(0x00000000)			/**< 속성 없음 */
#define atOBJATTR_MODAL  	       	(0x80000000)			/**< Modal 속성  */
#define atOBJATTR_STAYONTOP  	       (0x40000000)			/**< Stay On Top 속성  */
#define atOBJATTR_FOCUSABLE  		(0x20000000)			/**< Focus 갖는 것 가능 속성  */
#define atOBJATTR_CHILD  			(0x10000000)			/**< Child 개체 속성 */
#define atOBJATTR_USEANCHOR		(0x08000000)			/**< Anchor 사용 속성 */
#define atOBJATTR_TRANSPARENT  	(0x00008000)			/**< 투명 개체 속성 */
#define atOBJATTR_EVENTTARGET       	(0x00004000)			/**< 마우스 이벤트의 대상 여부. */

/** 기본 생성시 지정되는 속성 */
#define atOBJATTR_NORMAL  			(atOBJATTR_NONE | atOBJATTR_FOCUSABLE | atOBJATTR_EVENTTARGET)

//#define atCOORD_STICKER				(0x80000000)
//#define atCOORD_ALIGN				(0x40000000)

#define atANCHOR_IGNORE			(0x40000000)			/**< Anchor 무시. */


typedef atHANDLE atHWINOBJ;							/**< WinObj 핸들 */
typedef atHANDLE	 atHWINOBJTIMER;						/**< WinObj Timer 핸들 */

typedef int (atWINOBJPROC)( atHWINOBJ hWinObj, atINT nMsg, atLONG lParam1, atLONG lParam2 );	/**< WinObj의 핸들러 타입 */
typedef atWINOBJPROC* atLPWINOBJPROC;												/**< WinObj의 핸들러 포인터 타입 */

#ifdef __USES_NOTIFIER
typedef void (atWINOBJNOTIFIER)( atHWINOBJ hWinObj, atINT nMsg, atLONG lParam1, atLONG lParam2, atLPVOID lpNotifyData, atINT nRetCode );
typedef atWINOBJPROC* atLPWINOBJNOTIFIER;												/**< WinObj의 Notifier 포인터 타입 */
#endif


#define _TO_WINOBJ(a) 	((atHWINOBJ)(a))


/** 
 @brief WinObj 개체

 WinObj의 개체 구조체
 
 @remark WINOBJ를 상속 받을 개체는 반드시 개체의 맨 앞 필드에 WINOBJ 개체를 위치시키야 한다. (C++의 형태를 흉내) 
*/
typedef struct _tagatWINOBJ {
	atBOOL				bActivate;				/**< WinObj가 활성화 되었는가 */
	atDWORD				dwClassID;				/**< 클래스 ID */
	
	atHWINOBJ			hOwnerObj;				/**< 여기서 지정한 개체가 소멸할 때, 함께 소멸한다. */
	atHWINOBJ			hParentObj;				/**< 포함되어 표시될 부모 개체 */
	atINT				nID;					/**< ID. Unique하지 않다. */
	atDWORD				dwAttr;					/**< 속성 */
	atRECT				rect;					/**< 표시될 영역 */
	
	atBOOL				bEnable;				/**< enable or disable flag */
	//atBOOL			bFocus;					/**< 현재 포커스를 가졌는가 */
	atBOOL				bShow;					/**< 화면에 Show, Hide 설정 불변수 */
	
	atLONG				nObjDataSize;			/**< 데이터의 크기 */
	atLPVOID			lpObjData;				/**< 데이터 포인터 */
	atLPWINOBJPROC		fnDefObjProc;			/**< Defaut 핸들러 */
	atLPWINOBJPROC		fnObjProc;				/**< WinObj 핸들러 */

#ifdef __USES_NOTIFIER
	atLPWINOBJNOTIFIER	fnObjNotifier;			/**< WinObj의 notifier */
	atLPVOID			lpObjNotifyData;		/**< 데이터 포인터 */
#endif

	atTCHAR				szObjName[atWINOBJ_OBJNAME_LEN+1];		/**< 이름 */
	atTCHAR				szClassName[atWINOBJ_CLASSNAME_LEN+1];	/**< 클래스 이름 */

	atDWORD			dwSoftkeyFlag;							/**< 유효한 소프트키 플랫 */
	atTCHAR				szSoftKey[3][atSOFTKEY_TEXT_LEN+1];		/**< 소프트키 텍스트 */
	atLONG				nSoftKeyIDS[3];							/**< 소프트키의 IDS */

	atHGDC				hGDC;					/**< GDC 핸들 */

	atMARGIN			rtLayoutMargin;			/**< 클라이언트와의 여백 */
	atMARGIN			rtAnchor;				/**< Ahcor */

//	atHWINOBJMNG		hWinObjMng;
//	atHCONTAINER		hWinObjMng;
	atHANDLE			hChildObjMng;			/**< Child 개체의 매니저 */

	atUILIST   			lpTimerList;				/**< 내장 타이머 리스트 */
	
	void *				lpCreateParam;			/**< 생성시 사용자가 내정한 파라미터 */
	void *				lpExLayoutData;			/**< 레이아웃을 위해 참고로 사용될 데이터 */

	atHWINOBJ			hMouseActWinObj;		/**< 현재 동작 중인 마우스 이벤트의 대상 Child 개체 (Real Time) */
	atBOOL				bMouseDown;			/**< 마우스 버튼이 눌렸는가. (Real Time) */
	atINT				nMouseDownPosX, nMouseDownPosY;		/**< 마우스 버튼 눌렸을 때의 좌표 (Real Time) */

	atLONG				Tag;					/**< Dummy 용도 */
} atWINOBJ, *atLPWINOBJ;

/** 
 @brief WinObj 컨테이너 개체

 WinObj의 집합을 저장하는 구조체
 
 @remark 현재 사용되지 않음.
*/
typedef struct _tagatOBJCONTAINER {
	atHWINOBJ			hParentObj;
	atUILIST			lstObjs;
} atOBJCONTAINER, *atLPOBJCONTAINER;


atHWINOBJ	atWINOBJ_CreateHandle( atLPWINOBJ lpWinObj );
void			atWINOBJ_ReleaseHandle( atHWINOBJ hWinObj );

atLPWINOBJ	atWINOBJ_GetPtr( atHWINOBJ hWinObj );
void 	  * 	atWINOBJ_GetDataPtr( atHWINOBJ hWinObj );
atINT		atWINOBJ_GetID( atHWINOBJ hWinObj );

atLPWINOBJPROC	atWINOBJ_GetObjProc( atHWINOBJ hWinObj );
atLPWINOBJPROC	atWINOBJ_SetObjProc( atHWINOBJ hWinObj, atLPWINOBJPROC lpProc );

#ifdef __USES_NOTIFIER
atLPWINOBJNOTIFIER	atWINOBJ_GetObjNotifier( atHWINOBJ hWinObj );
atLPVOID			atWINOBJ_GetObjNotifyData( atHWINOBJ hWinObj );
atLPWINOBJNOTIFIER	atWINOBJ_SetObjNotifier( atHWINOBJ hWinObj, atLPWINOBJNOTIFIER lpNotifier, atLPVOID lpNotifyData );
#endif

int 			atWINOBJ_CallObjProc( atLPWINOBJPROC lpProc, atHWINOBJ hWinObj, int nMsg, long lParam1, long lParam2 );
int 			atWINOBJ_ProcessObjProc( atHWINOBJ hWinObj, int nMsg, long lParam1, long lParam2 );
int 			atWINOBJ_DefObjProc( atHWINOBJ hWinObj, int nMsg, long lParam1, long lParam2 );

atBOOL		atWINOBJ_InitWinObj( atHWINOBJ hWinObj, atHWINOBJ hOwnerObj, atDWORD dwClassID, atINT nID, atLPTSTR szWinObjName, atDWORD dwAttr,
									atMARGIN *rtLayoutMargin,
									atDWORD dwUserDataSize, atLPWINOBJPROC fnDefObjProc, atLPWINOBJPROC fnObjProc,
									void *lpCreateParam, void *lpExData );
atHWINOBJ	atWINOBJ_PreCreate( atHWINOBJ hOwnerObj, atDWORD dwClassID, atINT nID, atLPTSTR szWinObjName, atDWORD dwAttr,
									atMARGIN *rtLayoutMargin,
									atDWORD dwUserDataSize, atLPWINOBJPROC fnDefObjProc, atLPWINOBJPROC fnObjProc, 
									atVOID *lpCreateParam, atVOID *lpExData,
								  	atINT ExtObjSize );
atBOOL		atWINOBJ_PostCreate( atHWINOBJ hWinObj, atINT nWidth, atINT nHeight, atMARGIN *rtAnchor );
atHWINOBJ	atWINOBJ_Create( atHWINOBJ hOwnerObj, atDWORD dwClassID, atINT nID, atLPTSTR szWinObjName, atDWORD dwAttr,
								atINT nWidth, atINT nHeight, atMARGIN *rtLayoutMargin, atMARGIN *rtAnchor,
								atDWORD dwUserDataSize, atLPWINOBJPROC fnDefObjProc, atLPWINOBJPROC fnObjProc, 
								atVOID *lpCreateParam, atVOID *lpExData,
							  	atINT ExtObjSize );
void 		atWINOBJ_Destroy0( atLPWINOBJ lpWinObj );
void 		atWINOBJ_Destroy( atHWINOBJ hWinObj );
atBOOL		atWINOBJ_IsValidate( atHWINOBJ hWinObj );

void			atWINOBJ_Activate( atHWINOBJ hWinObj );
void			atWINOBJ_Deactivate( atHWINOBJ hWinObj );
atBOOL		atWINOBJ_IsActivate( atHWINOBJ hWinObj );

atDWORD       atWINOBJ_SetAttr( atHWINOBJ hWinObj, atDWORD dwAttr );
atDWORD       atWINOBJ_GetAttr( atHWINOBJ hWinObj );
atDWORD       atWINOBJ_ChangeAttr( atHWINOBJ hWinObj, atDWORD dwAddAttr, atDWORD dwRemoveAttr );

atDWORD	atWINOBJ_GetClassID( atHWINOBJ hWinObj );
atHWINOBJ 	atWINOBJ_GetOwnerObj(atHWINOBJ hWinObj);
atHWINOBJ 	atWINOBJ_SetOwnerObj(atHWINOBJ hWinObj, atHWINOBJ hOwnerObj);
atHWINOBJ	atWINOBJ_GetParentObj( atHWINOBJ hWinObj );
atHWINOBJ	atWINOBJ_SetParentObj( atHWINOBJ hWinObj, atHWINOBJ hParentObj );

atLPTSTR 	atWINOBJ_GetName( atHWINOBJ hWinObj );
atLPTSTR 	atWINOBJ_GetClassName( atHWINOBJ hWinObj );

atINT	atWINOBJ_SendMessage( atHWINOBJ hWinObj, atINT nMsg, atLONG lParam1, atLONG lParam2 );
atINT	atWINOBJ_PostMessage( atHWINOBJ hWinObj, atINT nMsg, atLONG lParam1, atLONG lParam2 );
atINT	atWINOBJ_PostMessageToParent( atHWINOBJ hWinObj, atINT nMsg, atLONG lParam1, atLONG lParam2 );

void		atWINOBJ_SetVisible(atHWINOBJ hWinObj, atBOOL bVisible );
atBOOL		atWINOBJ_IsVisible( atHWINOBJ hWinObj );
void 		atWINOBJ_Enabled( atHWINOBJ hWinObj, atBOOL bEnable );
atBOOL 		atWINOBJ_IsEnable( atHWINOBJ hWinObj );
//atBOOL		atWINOBJ_SetFocus( atHWINOBJ hWinObj, atBOOL bFocus );
//atBOOL 		atWINOBJ_IsFocus( atHWINOBJ hWinObj );
atBOOL		atWINOBJ_IsFocusable( atHWINOBJ hWinObj );
void       		atWINOBJ_UseAnchor( atHWINOBJ hWinObj, atBOOL bUseAnchor );

void 		atWINOBJ_SetPosition( atHWINOBJ hWinObj, atINT nX, atINT nY, atBOOL bForceChange );
void 		atWINOBJ_GetPosition( atHWINOBJ hWinObj, atINT *pnX, atINT *pnY );
void 		atWINOBJ_SetRect( atHWINOBJ hWinObj, int nStartX, int nStartY, int nEndX, int nEndY, atBOOL bForceChange );
void 		atWINOBJ_SetRegion(atHWINOBJ hWinObj, int nX, int nY, int nWidth, int nHeight, atBOOL bForceChange );
void 		atWINOBJ_GetRect( atHWINOBJ hWinObj, atRECT * pRect );
void 		atWINOBJ_GetRegion(atHWINOBJ hWinObj, atREGION * pRegion);
void 		atWINOBJ_GetLayoutMargin( atHWINOBJ hWinObj, atMARGIN * lpMargin );
void 		atWINOBJ_SetLayoutMargin( atHWINOBJ hWinObj, int nTop, int nBottom, int nLeft, int nRight );
void			atWINOBJ_SetLayoutMargin2( atHWINOBJ hWinObj, atMARGIN *lpMargin );
void 		atWINOBJ_GetClientRect( atHWINOBJ hWinObj, atRECT * pRect );
void 		atWINOBJ_GetClientRegion( atHWINOBJ hWinObj, atREGION * pRegion );

void			atWINOBJ_ConvClientScreenPos( atHWINOBJ hWinObj, atINT *lpOX, atINT *lpOY, atINT nX, atINT nY );
void 		atWINOBJ_ConvScreenPos( atHWINOBJ hWinObj, atINT *lpOX, atINT *lpOY, atINT nX, atINT nY );
atBOOL 		atWINOBJ_ConvClientScreenRect( atHWINOBJ hWinObj, atRECT *lpDesRect, atRECT *lpSrcRect );
atBOOL		atWINOBJ_ConvScreenRect( atHWINOBJ hWinObj, atRECT *lpDesRect, atRECT *lpSrcRect );
atBOOL 		atWINOBJ_ConvClientScreenRegion( atHWINOBJ hWinObj, atREGION *lpDesRegion, atREGION *lpSrcRegion );
atBOOL		atWINOBJ_ConvScreenRegion( atHWINOBJ hWinObj, atREGION *lpDesRegion, atREGION *lpSrcRegion );
atBOOL 		atWINOBJ_ConvScreenValidateClientRect( atHWINOBJ hWinObj, atRECT *lpDesRect, atRECT *lpSrcRect );
atBOOL 		atWINOBJ_ConvScreenValidateClientRegion( atHWINOBJ hWinObj, atREGION *lpDesRegion, atREGION *lpSrcRegion );
atBOOL		atWINOBJ_ConvScreenValidateRect( atHWINOBJ hWinObj, atRECT *lpDesRect, atRECT *lpSrcRect );
atBOOL		atWINOBJ_ConvScreenValidateRegion( atHWINOBJ hWinObj, atREGION *lpDesRegion, atREGION *lpSrcRegion );

atBOOL 		atWINOBJ_GetScreenRect( atHWINOBJ hWinObj, atRECT *lpRect );
atBOOL 		atWINOBJ_GetScreenRegion( atHWINOBJ hWinObj, atREGION *lpRegion );

void			atWINOBJ_GetClientAdjustSize( atHWINOBJ hWinObj, atSIZE *lpWinSize, atSIZE *lpClientSize );

void 		atWINOBJ_SetAnchor( atHWINOBJ hWinObj, int nTop, int nBottom, int nLeft, int nRight );
void			atWINOBJ_SetAnchor2( atHWINOBJ hWinObj, atMARGIN *lprtAnchor );

atLPVOID 	atWINOBJ_GetCreateParam(atHWINOBJ hWinObj);

void			atWINOBJ_ActivateSoftKey( atHWINOBJ hWinObj, atBOOL bActivate );
void 		atWINOBJ_SetSoftKey( atHWINOBJ hWinObj, atLPTSTR szLeft, atLPTSTR szCenter, atLPTSTR szRight );
void 		atWINOBJ_SetSoftKeyEnabled( atHWINOBJ hWinObj, atBOOL bLeft, atBOOL bCenter, atBOOL bRight );
void 		atWINOBJ_GetSoftKeyEnabled( atHWINOBJ hWinObj, atBOOL *pbLeft, atBOOL *pbCenter, atBOOL *pbRight );
void			atWINOBJ_SetSoftKeyIDS( atHWINOBJ hWinObj, atLONG leftIds, atLONG centerIds, atLONG rightIds);
void 		atWINOBJ_RealizeSoftKey( atHWINOBJ hWinObj );

atHGDC		atWINOBJ_GetGDC( atHWINOBJ hWinObj );
void		atWINOBJ_Refresh( atHWINOBJ hWinObj );
void		atWINOBJ_RefreshClient( atHWINOBJ hWinObj );
void 		atWINOBJ_Draw( atHWINOBJ hWinObj, atBOOL bFocused );
void 		atWINOBJ_DrawRegion( atHWINOBJ hWinObj, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atBOOL bFocused, atBOOL bFlush );


atBOOL atWINOBJ_AddChildWinObj( atHWINOBJ hWinObj, atHWINOBJ hChildWinObj );
void atWINOBJ_RemoveChildWinObj( atHWINOBJ hWinObj, atHWINOBJ hChildWinObj );
atINT atWINOBJ_GetChildWinObjCount( atHWINOBJ hWinObj );
atHWINOBJ atWINOBJ_GetCurChildWinObj( atHWINOBJ hWinObj );
atHWINOBJ atWINOBJ_FindChildWinObjByID( atHWINOBJ hWinObj, atINT nWinObjID );
atHWINOBJ atWINOBJ_FindChildWinObjByName( atHWINOBJ hWinObj, atLPTSTR szWinObjName );
atBOOL atWINOBJ_SetCurChildWinObj(atHWINOBJ hWinObj, atHWINOBJ hChildWinObj);
atHWINOBJ atWINOBJ_GetNextFocusableChildWinObj(atHWINOBJ hWinObj, atHWINOBJ hChildWinObj);
void atWINOBJ_ShowChildWinObj( atHWINOBJ hWinObj, atHWINOBJ hChildWinObj, atBOOL bShow );
atHWINOBJ atWINOBJ_SetTopChildWinObj( atHWINOBJ hWinObj, atHWINOBJ hChildWinObj );
atHWINOBJ atWINOBJ_SetBottomChildWinObj( atHWINOBJ hWinObj, atHWINOBJ hChildWinObj );
void atWINOBJ_PostMessageAllChildWinObjs( atHWINOBJ hWinObj, int nMsg, long lParam1, long lParam2 );
void atWINOBJ_RemoveAllChildWinObjs( atHWINOBJ hWinObj );
int atWINOBJ_DrawAllChildWinObjs( atHWINOBJ hWinObj, atBOOL bFocused );
void atWINOBJ_AdjustAllChildWinObjs( atHWINOBJ hWinObj );

atHWINOBJTIMER atWINOBJ_AddTimer( atHWINOBJ hWinObj, int id, long interval, atBOOL IsRepeat, atBOOL IsEnable, atBOOL IsFocus );
atHWINOBJTIMER atWINOBJ_GetTimer( atHWINOBJ hWinObj, int id );
void atWINOBJ_SetTimerInterval( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer, atLONG nInterval );
void atWINOBJ_EnableTimer( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer, atBOOL IsEnable );
void atWINOBJ_ResetTimer( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer );
void atWINOBJ_RemoveTimer( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer );
void atWINOBJ_SuspendTimer( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer );
void atWINOBJ_ResumeTimer( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer );
void atWINOBJ_EnableAllTimers( atHWINOBJ hWinObj, atBOOL IsEnable );
void atWINOBJ_RemoveAllTimers( atHWINOBJ hWinObj );
void atWINOBJ_SuspendAllTimers( atHWINOBJ hWinObj, atBOOL bFocusedSuspend );
void atWINOBJ_ResumeAllTimers( atHWINOBJ hWinObj );

atBOOL	atWINOBJ_IsEmbededEventTarget( atHWINOBJ hWinObj, int nX, int nY );


#ifdef __cplusplus
}
#endif


#endif // __WINOBJ_H_

