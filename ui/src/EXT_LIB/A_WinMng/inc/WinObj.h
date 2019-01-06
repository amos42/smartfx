/**
  @file WinObj.h
  @brief WinObj�� ���� API
  @remark WinObj�� ���� API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
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

#define	atWINOBJ_MAX_WINOBJ_CNT 	256				/**< WinObj�� �ִ� ���� */
#define	atWINOBJ_OBJNAME_LEN		32				/**< WinObj �̸��� �ִ� ���� */
#define	atWINOBJ_CLASSNAME_LEN	32				/**< WinObj Ŭ���� �̸��� �ִ� ���� */
#ifndef atSOFTKEY_TEXT_LEN
#define	atSOFTKEY_TEXT_LEN	32					/**< SoftKey �ؽ�Ʈ�� �ִ� ���� */
#endif


/**
 @brief WinObj �̺�Ʈ �޽��� 
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

	OBJMSG_CHANGECHILDOBJFOCUS,					/**< Child ��ü�� ��Ŀ���� �����  */
	OBJMSG_REQ_CHANGECHILDOBJFOCUS,				/**< Child ��ü�� ��Ŀ���� ������ �䱸�� */

	OBJMSG_MAX,
	
	OBJMSG_USERDEF      = atUIMSG_USERDEF			/**< @see atUIMSG_USERDEF */
} OBJMSG;


#define atOBJATTR_NONE  			(0x00000000)			/**< �Ӽ� ���� */
#define atOBJATTR_MODAL  	       	(0x80000000)			/**< Modal �Ӽ�  */
#define atOBJATTR_STAYONTOP  	       (0x40000000)			/**< Stay On Top �Ӽ�  */
#define atOBJATTR_FOCUSABLE  		(0x20000000)			/**< Focus ���� �� ���� �Ӽ�  */
#define atOBJATTR_CHILD  			(0x10000000)			/**< Child ��ü �Ӽ� */
#define atOBJATTR_USEANCHOR		(0x08000000)			/**< Anchor ��� �Ӽ� */
#define atOBJATTR_TRANSPARENT  	(0x00008000)			/**< ���� ��ü �Ӽ� */
#define atOBJATTR_EVENTTARGET       	(0x00004000)			/**< ���콺 �̺�Ʈ�� ��� ����. */

/** �⺻ ������ �����Ǵ� �Ӽ� */
#define atOBJATTR_NORMAL  			(atOBJATTR_NONE | atOBJATTR_FOCUSABLE | atOBJATTR_EVENTTARGET)

//#define atCOORD_STICKER				(0x80000000)
//#define atCOORD_ALIGN				(0x40000000)

#define atANCHOR_IGNORE			(0x40000000)			/**< Anchor ����. */


typedef atHANDLE atHWINOBJ;							/**< WinObj �ڵ� */
typedef atHANDLE	 atHWINOBJTIMER;						/**< WinObj Timer �ڵ� */

typedef int (atWINOBJPROC)( atHWINOBJ hWinObj, atINT nMsg, atLONG lParam1, atLONG lParam2 );	/**< WinObj�� �ڵ鷯 Ÿ�� */
typedef atWINOBJPROC* atLPWINOBJPROC;												/**< WinObj�� �ڵ鷯 ������ Ÿ�� */

#ifdef __USES_NOTIFIER
typedef void (atWINOBJNOTIFIER)( atHWINOBJ hWinObj, atINT nMsg, atLONG lParam1, atLONG lParam2, atLPVOID lpNotifyData, atINT nRetCode );
typedef atWINOBJPROC* atLPWINOBJNOTIFIER;												/**< WinObj�� Notifier ������ Ÿ�� */
#endif


#define _TO_WINOBJ(a) 	((atHWINOBJ)(a))


/** 
 @brief WinObj ��ü

 WinObj�� ��ü ����ü
 
 @remark WINOBJ�� ��� ���� ��ü�� �ݵ�� ��ü�� �� �� �ʵ忡 WINOBJ ��ü�� ��ġ��Ű�� �Ѵ�. (C++�� ���¸� �䳻) 
*/
typedef struct _tagatWINOBJ {
	atBOOL				bActivate;				/**< WinObj�� Ȱ��ȭ �Ǿ��°� */
	atDWORD				dwClassID;				/**< Ŭ���� ID */
	
	atHWINOBJ			hOwnerObj;				/**< ���⼭ ������ ��ü�� �Ҹ��� ��, �Բ� �Ҹ��Ѵ�. */
	atHWINOBJ			hParentObj;				/**< ���ԵǾ� ǥ�õ� �θ� ��ü */
	atINT				nID;					/**< ID. Unique���� �ʴ�. */
	atDWORD				dwAttr;					/**< �Ӽ� */
	atRECT				rect;					/**< ǥ�õ� ���� */
	
	atBOOL				bEnable;				/**< enable or disable flag */
	//atBOOL			bFocus;					/**< ���� ��Ŀ���� �����°� */
	atBOOL				bShow;					/**< ȭ�鿡 Show, Hide ���� �Һ��� */
	
	atLONG				nObjDataSize;			/**< �������� ũ�� */
	atLPVOID			lpObjData;				/**< ������ ������ */
	atLPWINOBJPROC		fnDefObjProc;			/**< Defaut �ڵ鷯 */
	atLPWINOBJPROC		fnObjProc;				/**< WinObj �ڵ鷯 */

#ifdef __USES_NOTIFIER
	atLPWINOBJNOTIFIER	fnObjNotifier;			/**< WinObj�� notifier */
	atLPVOID			lpObjNotifyData;		/**< ������ ������ */
#endif

	atTCHAR				szObjName[atWINOBJ_OBJNAME_LEN+1];		/**< �̸� */
	atTCHAR				szClassName[atWINOBJ_CLASSNAME_LEN+1];	/**< Ŭ���� �̸� */

	atDWORD			dwSoftkeyFlag;							/**< ��ȿ�� ����ƮŰ �÷� */
	atTCHAR				szSoftKey[3][atSOFTKEY_TEXT_LEN+1];		/**< ����ƮŰ �ؽ�Ʈ */
	atLONG				nSoftKeyIDS[3];							/**< ����ƮŰ�� IDS */

	atHGDC				hGDC;					/**< GDC �ڵ� */

	atMARGIN			rtLayoutMargin;			/**< Ŭ���̾�Ʈ���� ���� */
	atMARGIN			rtAnchor;				/**< Ahcor */

//	atHWINOBJMNG		hWinObjMng;
//	atHCONTAINER		hWinObjMng;
	atHANDLE			hChildObjMng;			/**< Child ��ü�� �Ŵ��� */

	atUILIST   			lpTimerList;				/**< ���� Ÿ�̸� ����Ʈ */
	
	void *				lpCreateParam;			/**< ������ ����ڰ� ������ �Ķ���� */
	void *				lpExLayoutData;			/**< ���̾ƿ��� ���� ����� ���� ������ */

	atHWINOBJ			hMouseActWinObj;		/**< ���� ���� ���� ���콺 �̺�Ʈ�� ��� Child ��ü (Real Time) */
	atBOOL				bMouseDown;			/**< ���콺 ��ư�� ���ȴ°�. (Real Time) */
	atINT				nMouseDownPosX, nMouseDownPosY;		/**< ���콺 ��ư ������ ���� ��ǥ (Real Time) */

	atLONG				Tag;					/**< Dummy �뵵 */
} atWINOBJ, *atLPWINOBJ;

/** 
 @brief WinObj �����̳� ��ü

 WinObj�� ������ �����ϴ� ����ü
 
 @remark ���� ������ ����.
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

