/**
  @file WinObj.c
  @brief WinObj에 관한 API
  @remark 주로 Window, Control 등으로 상속 받아 내부용으로 사용한다.
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see WinObj.h
*/
#include "AT_Types.h"
#include "UI_MNG.h"
#include "WinObj.h"


#define atOBJSOFTKEYFLAG_ACTIVATE	(0x8000)
#define atOBJSOFTKEYFLAG_LEFT  		(1 << 2)
#define atOBJSOFTKEYFLAG_CENTER		(1 << 1)
#define atOBJSOFTKEYFLAG_RIGHT		(1 << 0)

#define atOBJSOFTKEYFLAG_IDS_LEFT   (1 << 4)
#define atOBJSOFTKEYFLAG_IDS_CENTER (1 << 5)
#define atOBJSOFTKEYFLAG_IDS_RIGHT  (1 << 6)

#define atOBJSOFTKEYFLAG_LEFT_DISABLE   (1 << 7)
#define atOBJSOFTKEYFLAG_CENTER_DISABLE (1 << 8)
#define atOBJSOFTKEYFLAG_RIGHT_DISABLE  (1 << 9)



static atLPWINOBJ g_lpWinObj_PoolList[atWINOBJ_MAX_WINOBJ_CNT+1] = { atNULL, };
static int g_WinObj_CurIdx = 0;
static int g_WinObj_Count = 0;

static atHWINOBJ g_hCaptureKeyObj = atNULL;


// WinObj 포인터 풀에서 비어 있는 곳을 찾는다.
static atLPWINOBJ *find_free_winobj(void)
{
	int i;
	atLPWINOBJ *p;

	for( i = 0; i < atWINOBJ_MAX_WINOBJ_CNT; i++ ){
		p = &g_lpWinObj_PoolList[g_WinObj_CurIdx++];
		if( g_WinObj_CurIdx >= atWINOBJ_MAX_WINOBJ_CNT ) g_WinObj_CurIdx = 0;
		if( *p == atNULL ){
			return p;
		}
	}

	return atNULL;
}

/** @brief WinObj의 타이머 타입. (내부용) */
typedef struct _tagatWINOBJTIMER {
	atHTIMER  hTimer;
	atHWINOBJ hWinObj;
	atBOOL    bFocus;
	atDWORD   id;	
} atWINOBJTIMER;

static void cbWinTimer(atHTIMER hTimer, void *param)
{
	atWINOBJTIMER *tmr = (atWINOBJTIMER *)param;
	atWINOBJ_PostMessage( tmr->hWinObj, OBJMSG_TIMER, (long)hTimer, (long)tmr->id );
}


/** 
 @brief WinObj 핸들을 생성한다.
 
 WinObj 포인터로부터 핸들을 생성한다.

 @param lpWinObj [in] : WinObj 개체의 포인터
 @return WinObj 핸들
 @remark 핸들 총 갯수는 atWINOBJ_MAX_WINOBJ_CNT 갯수를 넘을 수 없다.
*/
atHWINOBJ atWINOBJ_CreateHandle( atLPWINOBJ lpWinObj )
{
	atLPWINOBJ *p;

	if( g_WinObj_Count >= atWINOBJ_MAX_WINOBJ_CNT ) return atNULL;
		
	p = find_free_winobj();
	if( p == atNULL ) return atNULL;

	*p = lpWinObj;
	g_WinObj_Count ++;

	return (atHWINOBJ)p;
}


/** 
 @brief WinObj 핸들을 소멸시킨다.
 
 WinObj 핸들을 소멸시킨다.

 @param hWinObj [in] : WinObj의 핸들
 @return 없음
*/
void atWINOBJ_ReleaseHandle( atHWINOBJ hWinObj )
{
	if( hWinObj == atNULL ) return;
	if( g_WinObj_Count <= 0 ) return;

	*((atLPWINOBJ *)hWinObj) = atNULL;
	g_WinObj_Count --;
}


/** 
 @brief WinObj 개체의 포인터를 얻는다.
 
 WinObj 개체의 포인터를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @return WinObj 개체의 포인터
*/
atLPWINOBJ atWINOBJ_GetPtr(atHWINOBJ hWinObj)
{
	if( hWinObj == atNULL ) return atNULL;
	if( ((atLPWINOBJ *)hWinObj < &g_lpWinObj_PoolList[0]) || ((atLPWINOBJ *)hWinObj > &g_lpWinObj_PoolList[atWINOBJ_MAX_WINOBJ_CNT-1]) ) return atNULL;
	return *((atLPWINOBJ *)hWinObj);
}


/** 
 @brief WinObj의 내장 Data 포인터를 얻는다.
 
 WinObj의 내장 Data 포인터를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @return WinObj의 내장 Data 포인터
*/
void *atWINOBJ_GetDataPtr(atHWINOBJ hWinObj)
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj ) return lpWinObj->lpObjData;
	else return atNULL;
}


/** 
 @brief WinObj의 ID를 얻는다.
 
 WinObj의 ID를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @return WinObj의 ID
*/
atINT atWINOBJ_GetID( atHWINOBJ hWinObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj ) return lpWinObj->nID;
	else return -1;
}


/** 
 @brief WinObj의 인스턴스를 생성한다.
 
 WinObj의 인스턴스를 생성한다.

 @param ExtObjSize [in] : WinObj의 추가적인 Data 크기
 @return WinObj의 핸들
*/
atHWINOBJ		atWINOBJ_CreateInstance( atINT ExtObjSize )
{
	atLPWINOBJ	lpWinObj;
	atHWINOBJ	hWinObj;

	lpWinObj = (atLPWINOBJ)UI_MEMALLOC(sizeof(atWINOBJ) + ExtObjSize );
	if ( lpWinObj == atNULL ) return atNULL;
	
	atUIAPI_memset( lpWinObj, 0, sizeof(atWINOBJ) + ExtObjSize );
//	atUIAPI_memset( (atBYTE *)lpWinObj + sizeof(atWINOBJ), 0, ExtObjSize );

	hWinObj = atWINOBJ_CreateHandle(lpWinObj);
	if( hWinObj == atNULL ) return atNULL;
	
	return hWinObj;
}


/** 
 @brief WinObj를 초기화시킨다.
 
 WinObj를 초기화시킨다.

 @param hWinObj [in] : WinObj의 핸들
 @param hOwnerObj [in] : 부모 WinObj의 핸들
 @param dwClassID [in] : 클래스의 ID
 @param nID [in] : WinObj의 ID
 @param szWinObjName [in] : 오브젝트 이름.
 @param dwAttr [in] : 속성. default는 atOBJATTR_NORMAL
 @param rtLayoutMargin [in] : 외각선의 여백. 이 여백을 제외하여 클라이언트의 크기가 결정된다.
 @param dwUserDataSize [in] : 사용자 추가 데이터 크기
 @param fnDefObjProc [in] : Default 이벤트 핸들러.
 @param fnObjProc [in] : 사용자 이벤트 핸들러.
 @param lpCreateParam [in] : 생성시 전달할 파라미터
 @param lpExData [in] : 생성시 참고할 추가 데이터. 주로 WinClass에서 전달.
 @return 성공하면 atTRUE, 실패하면 atFALSE
*/
atBOOL		atWINOBJ_InitWinObj( atHWINOBJ hWinObj, atHWINOBJ hOwnerObj, atDWORD dwClassID, atINT nID, atLPTSTR szWinObjName, atDWORD dwAttr,
									atMARGIN *rtLayoutMargin,
									atDWORD dwUserDataSize, atLPWINOBJPROC fnDefObjProc, atLPWINOBJPROC fnObjProc,
									void *lpCreateParam, void *lpExData )
{
	atLPWINOBJ	lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if ( lpWinObj == atNULL ) return atFALSE;

	lpWinObj->bActivate = atFALSE;
	lpWinObj->dwClassID = dwClassID;
//	lpWinObj->lpImplObj = lpWinObj;

	lpWinObj->nID		= nID;
	if( szWinObjName ){
		atUIAPI_strncpy( lpWinObj->szObjName, szWinObjName, atWINOBJ_OBJNAME_LEN );
		lpWinObj->szObjName[atWINOBJ_OBJNAME_LEN] = _ATC('\0');
	} else {
		lpWinObj->szObjName[0] = _ATC('\0');
	}
	atUIAPI_strncpy( lpWinObj->szClassName, _AT(""), atWINOBJ_CLASSNAME_LEN );

	lpWinObj->dwAttr 		= dwAttr;

	lpWinObj->hOwnerObj		= hOwnerObj;
	lpWinObj->hParentObj	= atNULL;

	lpWinObj->bEnable	= atTRUE;
	lpWinObj->bShow		= atFALSE;
	//lpWinObj->bFocus		= atFALSE;

	if( fnDefObjProc != atNULL )
		lpWinObj->fnDefObjProc = fnDefObjProc;
	else
		lpWinObj->fnDefObjProc = atWINOBJ_DefObjProc;
	lpWinObj->fnObjProc	= fnObjProc;
#ifdef __USES_NOTIFIER
	lpWinObj->fnObjNotifier	= atNULL;
#endif

	if ( dwUserDataSize > 0 ){
		if( (lpWinObj->lpObjData = (void*)UI_MEMALLOC(dwUserDataSize)) != atNULL ){
			lpWinObj->nObjDataSize = dwUserDataSize;
			atUIAPI_memset(lpWinObj->lpObjData, 0, dwUserDataSize);
		}
	} else {
		lpWinObj->nObjDataSize = 0;
		lpWinObj->lpObjData = (void*)atNULL;
	}

	lpWinObj->dwSoftkeyFlag = 0x8000;

	if( rtLayoutMargin ){
		lpWinObj->rtLayoutMargin = *rtLayoutMargin;
	} else {
		SET_MARGIN( lpWinObj->rtLayoutMargin, 0, 0, 0, 0 );
	}

	SET_RECT( lpWinObj->rect, 0, 0, 0, 0 );
	SET_MARGIN( lpWinObj->rtAnchor, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE );

	lpWinObj->hGDC = atNULL;
	
	lpWinObj->hChildObjMng = atNULL;
	lpWinObj->lpTimerList = atNULL;  // 타이머를 안 쓰는 윈도우는 메모리 절약을 위해

	lpWinObj->bMouseDown = atFALSE;
	lpWinObj->hMouseActWinObj = atNULL;
	lpWinObj->nMouseDownPosX = lpWinObj->nMouseDownPosY = 0;
	
	lpWinObj->lpCreateParam = lpCreateParam;
	lpWinObj->lpExLayoutData = lpExData;
	
	return atTRUE;
}


/** 
 @brief WinObj를 예비 초기화시킨다.
 
 WinObj를 예비 초기화시킨다.

 @param hOwnerObj [in] : 부모 WinObj의 핸들
 @param dwClassID [in] : 클래스의 ID
 @param nID [in] : WinObj의 ID
 @param szWinObjName [in] : 오브젝트 이름.
 @param dwAttr [in] : 속성. default는 atOBJATTR_NORMAL
 @param rtLayoutMargin [in] : 외각선의 여백. 이 여백을 제외하여 클라이언트의 크기가 결정된다.
 @param dwUserDataSize [in] : 사용자 추가 데이터 크기
 @param fnDefObjProc [in] : Default 이벤트 핸들러.
 @param fnObjProc [in] : 사용자 이벤트 핸들러.
 @param lpCreateParam [in] : 생성시 전달할 파라미터
 @param lpExData [in] : 생성시 참고할 추가 데이터. 주로 WinClass에서 전달.
 @param ExtObjSize [in] : 사용자 추가 데이터. 실제적으로 사용자 윈도우 등에서 사용한다.
 @return WinObj 핸들
*/
atHWINOBJ		atWINOBJ_PreCreate( atHWINOBJ hOwnerObj, atDWORD dwClassID, atINT nID, atLPTSTR szWinObjName, atDWORD dwAttr,
									atMARGIN *rtLayoutMargin,
									atDWORD dwUserDataSize, atLPWINOBJPROC fnDefObjProc, atLPWINOBJPROC fnObjProc, 
									atVOID *lpCreateParam, atVOID *lpExData,
								  	atINT ExtObjSize )
{
	atHWINOBJ	hWinObj;

	hWinObj = atWINOBJ_CreateInstance( ExtObjSize );
	if ( hWinObj == atNULL ) return atNULL;

	atWINOBJ_InitWinObj( hWinObj, hOwnerObj, dwClassID, nID, szWinObjName, dwAttr, 
						  rtLayoutMargin, 
						  dwUserDataSize, fnDefObjProc, fnObjProc,
						  lpCreateParam, lpExData );
	return hWinObj;
}


/** 
 @brief WinObj를 생성을 완료한다.
 
 WinObj를 생성을 완료한다.

 @param hWinObj [in] : WinObj의 핸들
 @param nWidth, nHeight [in] : 생성될 윈도우의 크기 
 @param rtLayoutMargin [in] : 외각선의 여백. 이 여백을 제외하여 클라이언트의 크기가 결정된다.
 @param rtAnchor [in] : Anchor를 지정한다.
 @return 성공하면 atTRUE, 실패하면 atFALSE
*/
atBOOL	atWINOBJ_PostCreate( atHWINOBJ hWinObj, atINT nWidth, atINT nHeight, atMARGIN *rtAnchor )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if ( lpWinObj == atNULL ) return atFALSE;

	lpWinObj->hGDC = atUIAPI_CreateGDC( 0, 0, nWidth, nHeight, atUIAPI_GetScreenBPP(), 0L ); 
	
	atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_INIT, (long)lpWinObj->lpCreateParam, (long)lpWinObj->lpExLayoutData );

	atWINOBJ_SetAnchor2( hWinObj, rtAnchor );
	
	atWINOBJ_SetRegion( hWinObj, 0, 0, nWidth, nHeight, atFALSE );

	return atTRUE;
}


/** 
 @brief WinObj를 생성한다.
 
 WinObj를 생성한다.

 @param hOwnerObj [in] : 부모 WinObj의 핸들
 @param dwClassID [in] : 클래스의 ID
 @param nID [in] : WinObj의 ID
 @param szWinObjName [in] : 오브젝트 이름.
 @param dwAttr [in] : 속성. default는 atOBJATTR_NORMAL
 @param nWidth, nHeight [in] : 생성될 윈도우의 크기 
 @param rtLayoutMargin [in] : 외각선의 여백. 이 여백을 제외하여 클라이언트의 크기가 결정된다.
 @param rtAnchor [in] : Anchor를 지정한다.
 @param dwUserDataSize [in] : 사용자 추가 데이터 크기
 @param fnDefObjProc [in] : Default 이벤트 핸들러.
 @param fnObjProc [in] : 사용자 이벤트 핸들러.
 @param lpCreateParam [in] : 생성시 전달할 파라미터
 @param lpExData [in] : 생성시 참고할 추가 데이터. 주로 WinClass에서 전달.
 @param ExtObjSize [in] : 사용자 추가 데이터. 실제적으로 사용자 윈도우 등에서 사용한다. 
 @return WinObj 핸들
*/
atHWINOBJ		atWINOBJ_Create( atHWINOBJ hOwnerObj, atDWORD dwClassID, atINT nID, atLPTSTR szWinObjName, atDWORD dwAttr,
									atINT nWidth, atINT nHeight, atMARGIN *rtLayoutMargin, atMARGIN *rtAnchor,
									atDWORD dwUserDataSize, atLPWINOBJPROC fnDefObjProc, atLPWINOBJPROC fnObjProc, 
									atVOID *lpCreateParam, atVOID *lpExData,
								  	atINT ExtObjSize )
{
	atHWINOBJ	hWinObj;

	hWinObj = atWINOBJ_PreCreate( hOwnerObj, dwClassID, nID, szWinObjName, dwAttr, 
						  rtLayoutMargin, 
						  dwUserDataSize, fnDefObjProc, fnObjProc,
						  lpCreateParam, lpExData,
						  ExtObjSize );
	
	if( hWinObj == atNULL ) return atNULL;
	
	atWINOBJ_PostCreate( hWinObj, nWidth, nHeight, rtAnchor );

	return hWinObj;
}


void atWINOBJ_Destroy0(atLPWINOBJ lpWinObj)
{
	if ( lpWinObj == atNULL ) return;
	
//	atWINOBJ_ProcessObjProc(hWinObj, OBJMSG_RELEASE, 0, 0);

	if( lpWinObj->hGDC ){
	    atUIAPI_DestroyGDC( lpWinObj->hGDC );
		lpWinObj->hGDC = atNULL;
	}
	
	if( lpWinObj->lpObjData ){
		UI_MEMFREE(lpWinObj->lpObjData);
		lpWinObj->lpObjData = atNULL;
	}

	UI_MEMFREE(lpWinObj);
}


/** 
 @brief WinObj를 소멸시킨다.
 
 WinObj를 소멸시킨다.

 @param hWinObj [in] : WinObj의 핸들
 @return 없음
 @remark 내부 호출용\n
 		 사용자는 이 함수를 직접 호출하기 보다는, OBJMSG_REQ_DESTROY를 보내서 소멸시키는 것이 좋다.
*/
void atWINOBJ_Destroy(atHWINOBJ hWinObj)
{
	atREGION rgn;
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if ( lpWinObj == atNULL ) return;

	atWINOBJ_GetRegion( hWinObj, &rgn );
	rgn.nX = rgn.nY = 0;
	atWINOBJ_ConvScreenRegion( hWinObj, &rgn, &rgn );
	
	lpWinObj->bActivate = atFALSE;
	
	atWINOBJ_RemoveAllTimers(hWinObj);
	atWINOBJ_RemoveAllChildWinObjs(hWinObj);

//	atWINOBJ_ProcessObjProc(hWinObj, OBJMSG_RELEASE, 0, 0);
	
	atWINOBJ_ReleaseHandle( hWinObj );
	
	atWINOBJ_Destroy0( lpWinObj );

	atUIAPI_AddInvalideArea( &rgn );
}


/** 
 @brief WinObj가 유효한지를 체크.
 
 WinObj가 유효한지를 체크.

 @param hWinObj [in] : WinObj의 핸들
 @return 유효하면 atTRUE, 무효하면 atFALSE
*/
atBOOL	atWINOBJ_IsValidate( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atFALSE;
		
	return atTRUE;
}


/** 
 @brief WinObj를 활성화시킨다.
 
 WinObj를 활성화시킨다.

 @param hWinObj [in] : WinObj의 핸들
 @return 없음
*/
void	 atWINOBJ_Activate( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return;

	lpWinObj->bActivate = atTRUE;
}


/** 
 @brief WinObj를 비활성화시킨다.
 
 WinObj를 비활성화시킨다.

 @param hWinObj [in] : WinObj의 핸들
 @return 없음
*/
void	 atWINOBJ_Deactivate( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return;

	lpWinObj->bActivate = atFALSE;
}


/** 
 @brief WinObj가 활성화 상태인지 체크한다.
 
 WinObj가 활성화 상태인지 체크한다.

 @param hWinObj [in] : WinObj의 핸들
 @return 활성 상태면 atTRUE, 비활성 상태면 atFALSE
*/
atBOOL	atWINOBJ_IsActivate( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atFALSE;

	return lpWinObj->bActivate;
}


/** 
 @brief WinObj의 이름을 얻는다.
 
 WinObj의 이름을 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @return WinObj 이름의 String 포인터
*/
atLPTSTR atWINOBJ_GetName(atHWINDOW hWinObj)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	
	if( lpWinObj == atNULL ) return _AT("");
	
	return lpWinObj->szObjName;
}


/** 
 @brief WinObj의 클래스 이름을 얻는다.
 
 WinObj의 이름을 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @return WinObj 클래스 이름의 String 포인터
*/
atLPTSTR atWINOBJ_GetClassName(atHWINDOW hWinObj)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	
	if( lpWinObj == atNULL ) return _AT("");
	
	return lpWinObj->szClassName;
}


/** 
 @brief WinObj의 Attribute를 설정한다.
 
 WinObj의 Attribute를 설정한다.

 @param hWinObj [in] : WinObj의 핸들
 @return 설정 이전의 Attribute
*/
atDWORD atWINOBJ_SetAttr( atHWINOBJ hWinObj, atDWORD dwAttr )
{
	atUINT oldattr;
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return 0;

	oldattr = lpWinObj->dwAttr;
	lpWinObj->dwAttr = dwAttr;

	return oldattr;
}


/** 
 @brief WinObj의 Attribute를 얻는다.
 
 WinObj의 Attribute를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @return 현재의 Attribute
*/
atDWORD atWINOBJ_GetAttr( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return 0;

	return lpWinObj->dwAttr;
}


/** 
 @brief WinObj의 Attribute를 바꾼다.
 
 WinObj의 Attribute를 바꾼다.\n
 추가할 속성과 제거할 속성을 각각 따로 지정할 수 있다.

 @param hWinObj [in] : WinObj의 핸들
 @param dwAddAttr [in] : 추가할 속성
 @param dwRemoveAttr [in] : 제거할 속성
 @return 설정 이전의 Attribute
*/
atDWORD atWINOBJ_ChangeAttr( atHWINOBJ hWinObj, atDWORD dwAddAttr, atDWORD dwRemoveAttr )
{
	atUINT oldattr;
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return 0;

	oldattr = lpWinObj->dwAttr;

	oldattr |= dwAddAttr;
	oldattr &= ~dwRemoveAttr;
	
	lpWinObj->dwAttr = oldattr;

	return oldattr;
}


/** 
 @brief WinObj를 그린다.
 
 WinObj를 그린다.\n
 OBJMSG_PREPAINT, OBJMSG_PAINTLAYOUT, OBJMSG_PAINT, OBJMSG_PAINTOBJS, OBJMSG_POSTPAINT\n
 이벤트가 차례로 발생한다.\n
 이후 SoftKey까지 그리게 된다.

 @param hWinObj [in] : WinObj의 핸들
 @param bFocused [in] : Focused
 @return 없음
*/
void atWINOBJ_Draw(atHWINOBJ hWinObj, atBOOL bFocused)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return;

	if( !atWINOBJ_IsVisible(hWinObj) ) return;

//	if ( lpWinObj->bShow ){
		atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_PREPAINT, atFALSE, (long)bFocused );
		atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_PAINTLAYOUT, atFALSE, (long)bFocused );
		atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_PAINT, atFALSE, (long)bFocused );
		atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_PAINTOBJS, atFALSE, (long)bFocused );
		atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_POSTPAINT, 0, (long)bFocused );
		atSOFTKEY_DrawSoftKey();
//	}
}


/** 
 @brief WinObj를 일부만 그린다.
 
 WinObj를 일부만 그린다.

 @param hWinObj [in] : WinObj의 핸들
 @param nX, nY [in] : 그릴 WinObj의 시작 위치
 @param nWidth, nHeight [in] : 그릴 WinObj의 영역
 @param bFlush [in] : 화면에 Flush할 것인가 여부
 @return 없음
 @remark 현재 구현되어 있지 않음. atWINOBJ_Draw와 동일하게 동작.
*/
void atWINOBJ_DrawRegion( atHWINOBJ hWinObj, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atBOOL bFocused, atBOOL bFlush )
{
	atWINOBJ_Draw( hWinObj, bFocused ); // 아직은 임시로...
}


/** 
 @brief WinObj를 화면에 갱신한다.
 
 WinObj를 화면 갱신 프로세스의 목록에 추가한다.

 @param hWinObj [in] : WinObj의 핸들
 @return 없음
 @remark 화면 갱신은 비동기적으로 이루어지기 때문에, 실제로 화면에 갱신되는 타이밍은 정확히 예측이 힘들다.
*/
void atWINOBJ_Refresh( atHWINOBJ hWinObj )
{
	atREGION area;
	atWINOBJ_GetRegion( hWinObj, &area );
	area.nX = area.nY = 0;
	if( !atWINOBJ_ConvScreenValidateRegion( hWinObj, &area, &area ) ) return;
	atUIAPI_AddInvalideArea( &area );

	// 이걸 넣어야 할 지 빼야 할 지 고민... 이것 때문에 매번 화면을 전체 갱신한다. 별도로 빼는 것이 맞을 것 같은데...	
	atANNBAR_RefreshAnnBar();
	atSOFTKEY_RefreshSoftKey();
}


/** 
 @brief WinObj의 클라이언트 영역을 화면에 갱신한다.
 
 WinObj의 클라이언트 영역을 화면 갱신 프로세스의 목록에 추가한다.

 @param hWinObj [in] : WinObj의 핸들
 @return 없음
 @remark 화면 갱신은 비동기적으로 이루어지기 때문에, 실제로 화면에 갱신되는 타이밍은 정확히 예측이 힘들다.
*/
void atWINOBJ_RefreshClient( atHWINOBJ hWinObj )
{
//	atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_REFRESH, bFlush, 0 );
	atREGION area;
	atWINOBJ_GetClientRegion( hWinObj, &area );
	area.nX = area.nY = 0;
	atWINOBJ_ConvClientScreenRegion( hWinObj, &area, &area );
	atUIAPI_AddInvalideArea( &area );
}


/** 
 @brief 생성시 지정한 Parameter를 얻는다.
 
 생성시 지정한 Parameter를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @return Parameter의 포인터
*/
atLPVOID atWINOBJ_GetCreateParam(atHWINOBJ hWinObj)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atNULL;

	return lpWinObj->lpCreateParam;
}


/** 
 @brief WinObj의 ClassID를 얻는다.
 
 WinObj의 ClassID를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @return ClassID
*/
atDWORD atWINOBJ_GetClassID( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return 0;

	return lpWinObj->dwClassID;
}


/** 
 @brief WinObj의 Parent 개체의 핸들을 얻는다.
 
 WinObj의 Parent 개체의 핸들을 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @return Parent개체의 핸들
*/
atHWINOBJ atWINOBJ_GetParentObj(atHWINOBJ hWinObj)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atNULL;

	return lpWinObj->hParentObj;
}


/** 
 @brief WinObj의 Parent 개체를 지정한다.
 
 WinObj의 Parent 개체를 지정한다.

 @param hWinObj [in] : WinObj의 핸들
 @param hParentObj [in] : Parent의 핸들
 @return 예전 Parent개체의 핸들
*/
atHWINOBJ atWINOBJ_SetParentObj(atHWINOBJ hWinObj, atHWINOBJ hParentObj)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atHWINOBJ old_obj;

	if( lpWinObj == atNULL ) return atNULL;

	old_obj = lpWinObj->hParentObj;
	lpWinObj->hParentObj = hParentObj;

	return old_obj;
}


/** 
 @brief WinObj의 Owner 개체의 핸들을 얻는다.
 
 WinObj의 Owner 개체의 핸들을 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @return Owner 개체의 핸들
*/
atHWINOBJ atWINOBJ_GetOwnerObj(atHWINOBJ hWinObj)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atNULL;

	return lpWinObj->hOwnerObj;
}


/** 
 @brief WinObj의 Owner 개체를 지정한다.
 
 WinObj의 Owner 개체를 지정한다.

 @param hWinObj [in] : WinObj의 핸들
 @param hParentObj [in] : Owner 핸들
 @return 예전 Owner 핸들
*/
atHWINOBJ atWINOBJ_SetOwnerObj(atHWINOBJ hWinObj, atHWINOBJ hOwnerObj)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atHWINOBJ old_obj;

	if( lpWinObj == atNULL ) return atNULL;

	old_obj = lpWinObj->hOwnerObj;
	lpWinObj->hOwnerObj = hOwnerObj;

	return old_obj;
}


/** 
 @brief WinObj에 메시지를 보낸다.
 
 WinObj에 메시지를 보낸다.

 @param hWinObj [in] : WinObj의 핸들
 @param nMsg [in] : 전송할 메시지
 @param lParam1, lParam2 [in] : 메시지의 파라미터
 @return 메시지 핸들러의 결과값
 @remark 메시지 처리가 핸들러의 직접적인 Call을 통해 즉각적으로 일어난다.
*/
atINT	atWINOBJ_SendMessage( atHWINOBJ hWinObj, atINT nMsg, atLONG lParam1, atLONG lParam2 )
{
/*
	atLPWINDOW lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return -1;

	return atAPP_SendMessage( lpWinObj->hApp, hWinObj, msg, param1, param2 );
*/
	return atWINOBJ_ProcessObjProc( hWinObj, nMsg, lParam1, lParam2 );
}


/** 
 @brief WinObj에 메시지를 보낸다.
 
 WinObj에 메시지를 보낸다.

 @param hWinObj [in] : WinObj의 핸들
 @param nMsg [in] : 전송할 메시지
 @param lParam1, lParam2 [in] : 메시지의 파라미터
 @return 메시지 처리 요청 결과.
 @remark 메시지 처리가 메시지 Queue를 통해 일어나기 때문에, 비동기적으로 일어난다. \n
 		하지만, 실제로 WinObj의 경우, 현재 Queue를 지원하지 않는다.\n
 		때문에 이를 상속 받은 Window의 경우엔, 별도의 Queue를 사용하여 이를 구현해야 한다.
*/
atINT	atWINOBJ_PostMessage( atHWINOBJ hWinObj, atINT nMsg, atLONG lParam1, atLONG lParam2 )
{
/*
	atLPWINDOW lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return -1;

	return atAPP_PostMessage( lpWinObj->hApp, hWinObj, nMsg, param1, param2 );
*/
	return atWINOBJ_ProcessObjProc( hWinObj, nMsg, lParam1, lParam2 );
}


/** 
 @brief WinObj의 Parent 개체에게 메시지를 보낸다.
 
 WinObj의 Parent 개체에게 메시지를 보낸다.

 @param hWinObj [in] : WinObj의 핸들
 @param nMsg [in] : 전송할 메시지
 @param lParam1, lParam2 [in] : 메시지의 파라미터
 @return 메시지 처리 요청 결과.
 @remark 메시지 처리가 메시지 Queue를 통해 일어나기 때문에, 비동기적으로 일어난다.
*/
atINT	atWINOBJ_PostMessageToParent( atHWINOBJ hWinObj, atINT nMsg, atLONG lParam1, atLONG lParam2 )
{
	atHWINOBJ hParentObj = atWINOBJ_GetParentObj(hWinObj);
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr( hParentObj );
	if( lpWinObj == atNULL ) return -1;
	
	return atWINOBJ_PostMessage( hParentObj, nMsg, lParam1, lParam2 );
}


/** 
 @brief WinObj의 위치를 지정한다.
 
 WinObj의 위치를 지정한다.

 @param hWinObj [in] : WinObj의 핸들
 @param nX, nY [in] : 이동할 위치
 @parem bForceChange [in] : Anchor를 없앨지 여부를 결정.
 @return 없음
 @remark OBJMSG_CHANGEPOS 이벤트가 발생한다.
*/
void atWINOBJ_SetPosition( atHWINOBJ hWinObj, atINT nX, atINT nY, atBOOL bForceChange )
{
	int w, h;
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if ( lpWinObj == atNULL ) return;

	if( (lpWinObj->rect.nStartX == nX) && (lpWinObj->rect.nStartY == nY) ) return;

	// SetPosition 하면 Anchor도 일단 바뀐다.
//	if( bForceChange ){
//		lpWinObj->rtAnchor.nLeft = nX;
//		lpWinObj->rtAnchor.nTop = nY;
//	}
	
	w = RECT_WIDTH(lpWinObj->rect);
	h = RECT_HEIGHT(lpWinObj->rect);
	lpWinObj->rect.nStartX = nX;
	lpWinObj->rect.nStartY = nY;
	lpWinObj->rect.nEndX = nX + w - 1;
	lpWinObj->rect.nEndY = nY + h - 1;
	
	if( bForceChange ){
//		SET_MARGIN( lpWinObj->rtAnchor, nY, atANCHOR_IGNORE, nX, atANCHOR_IGNORE );
		lpWinObj->dwAttr &= ~atOBJATTR_USEANCHOR;
	}
		
//	if( lpWinObj->bActivate ){
		atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_CHANGEPOS, 0, 0 );
//	}
}


/** 
 @brief 현재의 WinObj 위치를 구한다.
 
 현재의 WinObj 위치를 구한다.

 @param hWinObj [in] : WinObj의 핸들
 @param pnX, pnY [out] : WinObj의 위치값
 @return 없음
*/
void atWINOBJ_GetPosition( atHWINOBJ hWinObj, atINT *pnX, atINT *pnY )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if ( lpWinObj == atNULL ) return;

	*pnX = lpWinObj->rect.nStartX;
	*pnY = lpWinObj->rect.nStartY;
}


/** 
 @brief WinObj의 영역을 설정한다.
 
 WinObj의 영역을 설정한다.

 @param hWinObj [in] : WinObj의 핸들
 @param nStartX, nStartY [in] : 영역의 시작
 @param nEndX, nEndY [in] : 영역의 끝
 @parem bForceChange [in] : Anchor를 없앨지 여부를 결정.
 @return 없음
*/
void atWINOBJ_SetRect(atHWINOBJ hWinObj, int nStartX, int nStartY, int nEndX, int nEndY, atBOOL bForceChange )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atBOOL chg_pos, chg_size;

	chg_pos = ( (lpWinObj->rect.nStartX != nStartX) || (lpWinObj->rect.nStartY != nStartY) );
	chg_size = ( (RECT_WIDTH(lpWinObj->rect) != (nEndX-nStartX+1)) || (RECT_HEIGHT(lpWinObj->rect) != (nEndY-nStartY+1)) );
	
	if( chg_pos || chg_size ){
		lpWinObj->rect.nStartX = nStartX;
		lpWinObj->rect.nStartY = nStartY;
		lpWinObj->rect.nEndX = nEndX;
		lpWinObj->rect.nEndY = nEndY;

		if( bForceChange ){
			lpWinObj->dwAttr &= ~atOBJATTR_USEANCHOR;
		}
		
//		if( lpWinObj->bActivate ){
			if( chg_pos ) atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_CHANGEPOS, 0, 0 );
			if( chg_size ){
				atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_PRERESIZE, 0, 0 );
				atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_RESIZE, 0, 0 );
				atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_CHANGELAYOUT, 0, 0 );	
			}
//		}
	}
}


/** 
 @brief WinObj의 영역을 설정한다.
 
 WinObj의 영역을 설정한다.

 @param hWinObj [in] : WinObj의 핸들
 @param nX, nY [in] : 영역의 시작
 @param nWidth, nHeight [in] : 영역의 크기
 @parem bForceChange [in] : Anchor를 없앨지 여부를 결정.
 @return 없음
*/
void atWINOBJ_SetRegion(atHWINOBJ hWinObj, int nX, int nY, int nWidth, int nHeight, atBOOL bForceChange )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atBOOL chg_pos, chg_size;

	chg_pos = ( (lpWinObj->rect.nStartX != nX) || (lpWinObj->rect.nStartY != nY) );
	chg_size = ( (RECT_WIDTH(lpWinObj->rect) != nWidth) || (RECT_HEIGHT(lpWinObj->rect) != nHeight) );
	
	if( chg_pos || chg_size ){
		lpWinObj->rect.nStartX = nX;
		lpWinObj->rect.nStartY = nY;
		lpWinObj->rect.nEndX = nX + nWidth - 1;
		lpWinObj->rect.nEndY = nY + nHeight - 1;

		if( bForceChange ){
			lpWinObj->dwAttr &= ~atOBJATTR_USEANCHOR;
		}

//		if( lpWinObj->bActivate ){
			if( chg_pos ) atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_CHANGEPOS, 0, 0 );
			if( chg_size ){
				atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_PRERESIZE, 0, 0 );
				atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_RESIZE, 0, 0 );
				atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_CHANGELAYOUT, 0, 0 );	
			}
//		}
	}
}


/** 
 @brief WinObj의 영역을 얻는다.
 
 WinObj의 영역을 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @param pRect [out] : 현재 WinObj의 영역
 @return 없음
*/
void atWINOBJ_GetRect(atHWINOBJ hWinObj, atRECT * pRect)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj ){
		atUIAPI_memcpy(pRect, &lpWinObj->rect, sizeof(atRECT));
	} else {
		atUIAPI_memset(pRect, 0, sizeof(atRECT));
	}
}


/** 
 @brief WinObj의 영역을 얻는다.
 
 WinObj의 영역을 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @param pRegion [out] : 현재 WinObj의 영역
 @return 없음
*/
void atWINOBJ_GetRegion(atHWINOBJ hWinObj, atREGION * pRegion)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj ){
		pRegion->nX = lpWinObj->rect.nStartX;
		pRegion->nY = lpWinObj->rect.nStartY;
		pRegion->nWidth = RECT_WIDTH(lpWinObj->rect);
		pRegion->nHeight = RECT_HEIGHT(lpWinObj->rect);
	} else {
		atUIAPI_memset(pRegion, 0, sizeof(atRECT));
	}
}


/** 
 @brief 레이아웃의 여백을 얻는다.
 
 레이아웃의 여백을 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @param lpMargin [out] : 레이아웃의 여백
 @return 없음
 @remark 레이아웃의 여백의 크기는 클라이언트 영역에 직접적인 영향을 준다.
*/
void atWINOBJ_GetLayoutMargin( atHWINOBJ hWinObj, atMARGIN * lpMargin )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if ( lpWinObj == atNULL ) return;
	
	if( lpMargin ) *lpMargin = lpWinObj->rtLayoutMargin;
}


/** 
 @brief 레이아웃의 여백을 지정한다.
 
 레이아웃의 여백을 지정한다.

 @param hWinObj [in] : WinObj의 핸들
 @param nTop [in] : 레이아웃의 윗쪽 여백
 @param nBottom [in] : 레이아웃의 아랫쪽 여백
 @param nLeft [in] : 레이아웃의 왼쪽 여백
 @param nRight [in] : 레이아웃의 오른쪽 여백
 @return 없음
 @remark 레이아웃의 여백의 크기는 클라이언트 영역에 직접적인 영향을 준다.
*/
void atWINOBJ_SetLayoutMargin( atHWINOBJ hWinObj, int nTop, int nBottom, int nLeft, int nRight )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if ( lpWinObj == atNULL ) return;
	
	if( nTop > 0 ) lpWinObj->rtLayoutMargin.nTop = nTop;
	if( nBottom > 0 ) lpWinObj->rtLayoutMargin.nBottom = nBottom;
	if( nRight > 0 ) lpWinObj->rtLayoutMargin.nRight = nRight;
	if( nLeft > 0 ) lpWinObj->rtLayoutMargin.nLeft = nLeft;

	atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_CHANGELAYOUT, 0, 0 );	
}


/** 
 @brief 레이아웃의 여백을 지정한다.
 
 레이아웃의 여백을 지정한다.

 @param hWinObj [in] : WinObj의 핸들
 @param lpMargin [in] : 레이아웃의 여백
 @return 없음
 @remark 레이아웃의 여백의 크기는 클라이언트 영역에 직접적인 영향을 준다.
*/
void atWINOBJ_SetLayoutMargin2( atHWINOBJ hWinObj, atMARGIN *lpMargin )
{
	if( lpMargin == atNULL ){
		atWINOBJ_SetLayoutMargin( hWinObj, 0, 0, 0, 0 );
	} else {
		atWINOBJ_SetLayoutMargin( hWinObj, lpMargin->nTop, lpMargin->nBottom, lpMargin->nLeft, lpMargin->nRight );
	}
}


/** 
 @brief 클라이언트의 영역을 얻는다.
 
 클라이언트의 영역을 얻는다.\n
 실제로 사용자가 무엇인가를 그릴 땐, 클라이언트 영역을 주로 대상으로 한다.\n
 클라이언트 영역의 시작은 항상 0, 0이다.

 @param hWinObj [in] : WinObj의 핸들
 @param pRect [out] : 클라이언트 영역
 @return 없음
*/
void atWINOBJ_GetClientRect( atHWINOBJ hWinObj, atRECT * pRect )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj ){
		pRect->nStartX 	= 0;
		pRect->nStartY 	= 0;
		pRect->nEndX 	= RECT_WIDTH(lpWinObj->rect) - 1 - (lpWinObj->rtLayoutMargin.nLeft + lpWinObj->rtLayoutMargin.nRight);
		pRect->nEndY 	= RECT_HEIGHT(lpWinObj->rect) - 1 - (lpWinObj->rtLayoutMargin.nBottom + lpWinObj->rtLayoutMargin.nTop);
	} else {
		SET_RECT( *pRect, 0, 0, 0, 0 );
	}
}


/** 
 @brief 클라이언트의 영역을 얻는다.
 
 클라이언트의 영역을 얻는다.\n
 실제로 사용자가 무엇인가를 그릴 땐, 클라이언트 영역을 주로 대상으로 한다.\n
 클라이언트 영역의 시작은 항상 0, 0이다.

 @param hWinObj [in] : WinObj의 핸들
 @param pRegion [out] : 클라이언트 영역
 @return 없음
*/
void atWINOBJ_GetClientRegion( atHWINOBJ hWinObj, atREGION * pRegion )
{
	atRECT rect;
	atWINOBJ_GetClientRect( hWinObj, &rect );
	RECT2REGION( *pRegion, rect );
}


/** 
 @brief WinObj의 클라이언트상의 지정된 영역을 화면상의 유효한 영역으로 변환한다.
 
 WinObj의 클라이언트상의 지정된 영역을 화면상의 유효한 영역으로 변환한다.\n
 만약 화면에 보이지 않는 영역이라면 무효한 영역으로 간주한다.

 @param hWinObj [in] : WinObj의 핸들
 @param lpOX, lpOY [out] : 화면상의 영역
 @param nX, nY [in] : 클라이언트상의 영역
 @return 유효한 크기를 얻으면 atTRUE, 아니면 atFALSE
*/
void atWINOBJ_ConvScreenPos( atHWINOBJ hWinObj, atINT *lpOX, atINT *lpOY, atINT nX, atINT nY )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	int x, y;

	if( lpWinObj /*&& (lpWinObj->dwAttr & atOBJATTR_CHILD)*/ ){
		x = nX + lpWinObj->rect.nStartX;
		y = nY + lpWinObj->rect.nStartY;
		if( (lpWinObj->dwAttr & atOBJATTR_CHILD) &&  (lpWinObj->hParentObj != atNULL) ){
			atWINOBJ_ConvClientScreenPos( lpWinObj->hParentObj, &x, &y, x, y );
		}
		*lpOX = x;
		*lpOY = y;
	} else {
		*lpOX = nX;
		*lpOY = nY;
	}
}


/** 
 @brief WinObj의 클라이언트상의 지정된 영역을 화면상의 유효한 영역으로 변환한다.
 
 WinObj의 클라이언트상의 지정된 영역을 화면상의 유효한 영역으로 변환한다.\n
 만약 화면에 보이지 않는 영역이라면 무효한 영역으로 간주한다.

 @param hWinObj [in] : WinObj의 핸들
 @param lpOX, lpOY [out] : 화면상의 영역
 @param nX, nY [in] : 클라이언트상의 영역
 @return 유효한 크기를 얻으면 atTRUE, 아니면 atFALSE
*/
void atWINOBJ_ConvClientScreenPos( atHWINOBJ hWinObj, atINT *lpOX, atINT *lpOY, atINT nX, atINT nY )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	int x, y;

	if( lpWinObj /*&& (lpWinObj->dwAttr & atOBJATTR_CHILD)*/ ){
		x = nX + lpWinObj->rect.nStartX + lpWinObj->rtLayoutMargin.nLeft;
		y = nY + lpWinObj->rect.nStartY + lpWinObj->rtLayoutMargin.nTop;
		if( (lpWinObj->dwAttr & atOBJATTR_CHILD) &&  (lpWinObj->hParentObj != atNULL) ){
//		if( lpWinObj->dwAttr & atOBJATTR_CHILD ){
			atWINOBJ_ConvClientScreenPos( lpWinObj->hParentObj, &x, &y, x, y );
		}
		*lpOX = x;
		*lpOY = y;
	} else {
		*lpOX = nX;
		*lpOY = nY;
	}
}


/** 
 @brief WinObj의 클라이언트상의 지정된 영역을 화면상의 유효한 영역으로 변환한다.
 
 WinObj의 클라이언트상의 지정된 영역을 화면상의 유효한 영역으로 변환한다.\n
 만약 화면에 보이지 않는 영역이라면 무효한 영역으로 간주한다.

 @param hWinObj [in] : WinObj의 핸들
 @param lpDesRegion [out] : 화면상의 영역
 @param lpSrcRegion [in] : 클라이언트상의 영역
 @return 유효한 크기를 얻으면 atTRUE, 아니면 atFALSE
*/
atBOOL atWINOBJ_ConvScreenRegion( atHWINOBJ hWinObj, atREGION *lpDesRegion, atREGION *lpSrcRegion )
{
	int x, y;
	atRECT rect;
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atFALSE;
	
	atWINOBJ_ConvScreenPos( hWinObj, &x, &y, lpSrcRegion->nX, lpSrcRegion->nY );

	REGION2RECT( rect, *lpSrcRegion );
	atUIAPI_MoveRect( &rect, &rect, x, y );
	
	RECT2REGION( *lpDesRegion, rect );

	return atTRUE;
}


/** 
 @brief WinObj의 클라이언트상의 지정된 영역을 화면상의 유효한 영역으로 변환한다.
 
 WinObj의 클라이언트상의 지정된 영역을 화면상의 유효한 영역으로 변환한다.\n
 만약 화면에 보이지 않는 영역이라면 무효한 영역으로 간주한다.

 @param hWinObj [in] : WinObj의 핸들
 @param lpDesRegion [out] : 화면상의 영역
 @param lpSrcRegion [in] : 클라이언트상의 영역
 @return 유효한 크기를 얻으면 atTRUE, 아니면 atFALSE
*/
atBOOL atWINOBJ_ConvScreenValidateClientRect( atHWINOBJ hWinObj, atRECT *lpDesRect, atRECT *lpSrcRect )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atRECT rect, validerect;
//	atREGION parentarea;
	atBOOL ret;

	if( lpWinObj ){
		if( !lpWinObj->bActivate ) return atFALSE;
		if( !lpWinObj->bShow ) return atFALSE;
	}

	if( lpWinObj /*&& (lpWinObj->dwAttr & atOBJATTR_CHILD)*/ ){
//		SET_RECT( validerect, lpWinObj->rtLayoutMargin.nLeft, lpWinObj->rtLayoutMargin.nTop,
//							RECT_WIDTH(lpWinObj->rect)-1-lpWinObj->rtLayoutMargin.nRight, 
//							RECT_HEIGHT(lpWinObj->rect)-1-lpWinObj->rtLayoutMargin.nBottom );
		SET_RECT( validerect, 0, 0,
							RECT_WIDTH(lpWinObj->rect)-1-(lpWinObj->rtLayoutMargin.nLeft+lpWinObj->rtLayoutMargin.nRight), 
							RECT_HEIGHT(lpWinObj->rect)-1-(lpWinObj->rtLayoutMargin.nTop+lpWinObj->rtLayoutMargin.nBottom) );
		ret = atUIAPI_IntersectRect( &rect, lpSrcRect, &validerect );
		if( !ret ) return atFALSE;

		atUIAPI_MoveRect( &rect, &rect, lpWinObj->rect.nStartX + lpWinObj->rtLayoutMargin.nLeft, lpWinObj->rect.nStartY + lpWinObj->rtLayoutMargin.nTop );
		
		if( (lpWinObj->dwAttr & atOBJATTR_CHILD) &&  (lpWinObj->hParentObj != atNULL) ){
			ret = atWINOBJ_ConvScreenValidateClientRect( lpWinObj->hParentObj, &rect, &rect );
		} else {
			ret = atWINOBJ_ConvScreenValidateClientRect( atNULL, &rect, &rect );
		}
		if( !ret ) return atFALSE;

		*lpDesRect = rect;
	} else {
		atREGION rgn;
		atDESKTOP_GetDesktopArea( atDESKTOP_AREA_CLIENT,  &rgn );
		REGION2RECT( validerect, rgn );
		
		ret = atUIAPI_IsIntersectRect( lpSrcRect, &validerect );
		if( !ret ) return atFALSE;
		
		*lpDesRect = *lpSrcRect;
	}

	return atTRUE;
}


/** 
 @brief WinObj의 클라이언트상의 지정된 영역을 화면상의 유효한 영역으로 변환한다.
 
 WinObj의 클라이언트상의 지정된 영역을 화면상의 유효한 영역으로 변환한다.\n
 만약 화면에 보이지 않는 영역이라면 무효한 영역으로 간주한다.

 @param hWinObj [in] : WinObj의 핸들
 @param lpDesRegion [out] : 화면상의 영역
 @param lpSrcRegion [in] : 클라이언트상의 영역
 @return 유효한 크기를 얻으면 atTRUE, 아니면 atFALSE
*/
atBOOL atWINOBJ_ConvScreenValidateRect( atHWINOBJ hWinObj, atRECT *lpDesRect, atRECT *lpSrcRect )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atRECT rect, validerect;
	atBOOL ret;

	if( lpWinObj ){
		if( !lpWinObj->bActivate ) return atFALSE;
		if( !lpWinObj->bShow ) return atFALSE;
	}

	if( lpWinObj /*&& (lpWinObj->dwAttr & atOBJATTR_CHILD)*/ ){
		SET_RECT( validerect, 0, 0,	RECT_WIDTH(lpWinObj->rect)-1, RECT_HEIGHT(lpWinObj->rect)-1 );
		ret = atUIAPI_IntersectRect( &rect, lpSrcRect, &validerect );
		if( !ret ) return atFALSE;

		atUIAPI_MoveRect( &rect, &rect, lpWinObj->rect.nStartX, lpWinObj->rect.nStartY );
		
		if( (lpWinObj->dwAttr & atOBJATTR_CHILD) ){ 
			if( lpWinObj->hParentObj == atNULL ) return atFALSE;
			ret = atWINOBJ_ConvScreenValidateClientRect( lpWinObj->hParentObj, &rect, &rect );
		} else {
			ret = atWINOBJ_ConvScreenValidateClientRect( atNULL, &rect, &rect );
		}
		if( !ret ) return atFALSE;

		*lpDesRect = rect;
	} else {
		atREGION rgn;
		atDESKTOP_GetDesktopArea( atDESKTOP_AREA_CLIENT,  &rgn );
		REGION2RECT( validerect, rgn );
		
		ret = atUIAPI_IsIntersectRect( lpSrcRect, &validerect );
		if( !ret ) return atFALSE;
		
		*lpDesRect = *lpSrcRect;
	}

	return atTRUE;
}


/** 
 @brief WinObj의 클라이언트상의 지정된 영역을 화면상의 유효한 영역으로 변환한다.
 
 WinObj의 클라이언트상의 지정된 영역을 화면상의 유효한 영역으로 변환한다.\n
 만약 화면에 보이지 않는 영역이라면 무효한 영역으로 간주한다.

 @param hWinObj [in] : WinObj의 핸들
 @param lpDesRegion [out] : 화면상의 영역
 @param lpSrcRegion [in] : 클라이언트상의 영역
 @return 유효한 크기를 얻으면 atTRUE, 아니면 atFALSE
*/
atBOOL atWINOBJ_ConvScreenValidateClientRegion( atHWINOBJ hWinObj, atREGION *lpDesRegion, atREGION *lpSrcRegion )
{
	atRECT rect;
	atBOOL ret;

	REGION2RECT( rect, *lpSrcRegion );
	ret = atWINOBJ_ConvScreenValidateClientRect( hWinObj, &rect, &rect );
	if( !ret ) return atFALSE;
	RECT2REGION( *lpDesRegion, rect );
	return atTRUE;
}


/** 
 @brief WinObj의 클라이언트상의 지정된 영역을 화면상의 유효한 영역으로 변환한다.
 
 WinObj의 클라이언트상의 지정된 영역을 화면상의 유효한 영역으로 변환한다.\n
 만약 화면에 보이지 않는 영역이라면 무효한 영역으로 간주한다.

 @param hWinObj [in] : WinObj의 핸들
 @param lpDesRegion [out] : 화면상의 영역
 @param lpSrcRegion [in] : 클라이언트상의 영역
 @return 유효한 크기를 얻으면 atTRUE, 아니면 atFALSE
*/
atBOOL atWINOBJ_ConvScreenValidateRegion( atHWINOBJ hWinObj, atREGION *lpDesRegion, atREGION *lpSrcRegion )
{
	atRECT rect;
	atBOOL ret;

	REGION2RECT( rect, *lpSrcRegion );
	ret = atWINOBJ_ConvScreenValidateRect( hWinObj, &rect, &rect );
	if( !ret ) return atFALSE;
	RECT2REGION( *lpDesRegion, rect );
	return atTRUE;
}


/** 
 @brief WinObj의 클라이언트상의 지정된 영역을 화면상의 영역으로 변환한다.
 
 WinObj의 클라이언트상의 지정된 영역을 화면상의 영역으로 변환한다.

 @param hWinObj [in] : WinObj의 핸들
 @param lpDesRegion [out] : 화면상의 영역
 @param lpSrcRegion [in] : 클라이언트상의 영역
 @return 유효한 크기를 얻으면 atTRUE, 아니면 atFALSE
*/
atBOOL atWINOBJ_ConvClientScreenRegion( atHWINOBJ hWinObj, atREGION *lpDesRegion, atREGION *lpSrcRegion )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atFALSE;
	
	atWINOBJ_ConvClientScreenPos( hWinObj, (int *)&lpDesRegion->nX, (int *)&lpDesRegion->nY, lpSrcRegion->nX, lpSrcRegion->nY );

	lpDesRegion->nWidth = lpSrcRegion->nWidth;
	lpDesRegion->nHeight = lpSrcRegion->nHeight;

	return atTRUE;
}


/** 
 @brief WinObj상의 지정된 영역을 화면상의 영역으로 변환한다.
 
 WinObj상의 지정된 영역을 화면상의 영역으로 변환한다.

 @param hWinObj [in] : WinObj의 핸들
 @param lpDesRect [out] : 화면상의 영역
 @param lpSrcRect [in] : 클라이언트상의 영역
 @return 유효한 크기를 얻으면 atTRUE, 아니면 atFALSE
*/
atBOOL atWINOBJ_ConvScreenRect( atHWINOBJ hWinObj, atRECT *lpDesRect, atRECT *lpSrcRect )
{
	atREGION rgn;
	atBOOL ret;

	RECT2REGION( rgn, *lpSrcRect );

	ret = atWINOBJ_ConvScreenRegion( hWinObj, &rgn, &rgn );
	if( !ret ) return atFALSE;

	REGION2RECT( *lpDesRect, rgn );	

	return atTRUE;
}


/** 
 @brief WinObj의 클라이언트상의 지정된 영역을 화면상의 영역으로 변환한다.
 
 WinObj의 클라이언트상의 지정된 영역을 화면상의 영역으로 변환한다.

 @param hWinObj [in] : WinObj의 핸들
 @param lpDesRect [out] : 화면상의 영역
 @param lpSrcRect [in] : 클라이언트상의 영역
 @return 유효한 크기를 얻으면 atTRUE, 아니면 atFALSE
*/
atBOOL atWINOBJ_ConvClientScreenRect( atHWINOBJ hWinObj, atRECT *lpDesRect, atRECT *lpSrcRect )
{
	atREGION rgn;
	atBOOL ret;

	RECT2REGION( rgn, *lpSrcRect );

	ret = atWINOBJ_ConvClientScreenRegion( hWinObj, &rgn, &rgn );
	if( !ret ) return atFALSE;

	REGION2RECT( *lpDesRect, rgn );	

	return atTRUE;
}


/** 
 @brief 화면상의 WinObj 영역을 얻는다.
 
 화면상의 WinObj 영역을 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @param lpRect [out] : 화면상의 WinObj 영역
 @return 유효한 크기를 얻으면 atTRUE, 아니면 atFALSE
*/
atBOOL atWINOBJ_GetScreenRect( atHWINOBJ hWinObj, atRECT *lpRect )
{
	atWINOBJ_GetRect( hWinObj, lpRect );
	atUIAPI_MoveRect( lpRect, lpRect, -lpRect->nStartX, -lpRect->nStartY );
	return atWINOBJ_ConvScreenRect( hWinObj, lpRect, lpRect );
}


/** 
 @brief 화면상의 WinObj 영역을 얻는다.
 
 화면상의 WinObj 영역을 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @param lpRegion [out] : 화면상의 WinObj 영역
 @return 유효한 크기를 얻으면 atTRUE, 아니면 atFALSE
*/
atBOOL atWINOBJ_GetScreenRegion( atHWINOBJ hWinObj, atREGION *lpRegion )
{
	atWINOBJ_GetRegion( hWinObj, lpRegion );
	lpRegion->nX = lpRegion->nY = 0;
	return atWINOBJ_ConvScreenRegion( hWinObj, lpRegion, lpRegion );
}


/** 
 @brief 지정된 클라이언트 크기를 적용하여 계산된 WinObj의 크기를 얻는다.
 
 지정된 클라이언트 크기를 적용하여 계산된 WinObj의 크기를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @param lpWinSize [out] : 적용된 윈도우 크기
 @param lpClientSize [in] : 적용하고자 하는 클라이언트 크기
 @return 없음
*/
void	atWINOBJ_GetClientAdjustSize( atHWINOBJ hWinObj, atSIZE *lpWinSize, atSIZE *lpClientSize )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;
	if( (lpWinSize == atNULL) || (lpClientSize == atNULL) ) return;

	lpWinSize->nWidth = lpClientSize->nWidth + lpWinObj->rtLayoutMargin.nLeft + lpWinObj->rtLayoutMargin.nRight;
	lpWinSize->nHeight = lpClientSize->nHeight + lpWinObj->rtLayoutMargin.nTop + lpWinObj->rtLayoutMargin.nBottom;
}


/** 
 @brief WinObj의 Anchor를 설정한다.
 
 WinObj의 Anchor를 설정한다.\n
 Anchor란, 개체가 부모 개체에 대해 유지를 강제하는 여백의 크기를 의미한다.\n
 Anchor를 통해, 부모 개체가 레이아웃이 변경될 때마다 자동으로 자식 개체의 레이아웃을 자동으로 함께 변경시키는 기능을 갖도록 만들어 준다.
 Anchor를 무시할 땐, atANCHOR_IGNORE 값을 파라미터로 사용한다.

 @param hWinObj [in] : WinObj의 핸들
 @param nTop [in] : 윗쪽 Anchor 값. 무시할 땐 atANCHOR_IGNORE
 @param nBottom [in] : 아랫쪽 Anchor 값. 무시할 땐 atANCHOR_IGNORE
 @param nLeft [in] : 왼쪽 Anchor 값. 무시할 땐 atANCHOR_IGNORE
 @param nRight [in] : 오른쪽 Anchor 값. 무시할 땐 atANCHOR_IGNORE
 @return 없음
 @remark Top/Bottom, Left/Right 등, 양편을 모두 atANCHOR_IGNORE로 설정하면 크기 변경 없이 위치만 중앙으로 이동하는 효과를 갖는다.
*/
void	atWINOBJ_SetAnchor( atHWINOBJ hWinObj, int nTop, int nBottom, int nLeft, int nRight )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;

	lpWinObj->dwAttr |= atOBJATTR_USEANCHOR;
	SET_MARGIN( lpWinObj->rtAnchor, nTop, nBottom, nLeft, nRight );

//	atWINOBJ_PostMessageToParent( hWinObj, OBJMSG_CHANGECHILDLAYOUT, lpWinObj->nID, (long)hWinObj );
}


/** 
 @brief WinObj의 Anchor를 설정한다.
 
 WinObj의 Anchor를 설정한다.

 @param hWinObj [in] : WinObj의 핸들
 @param lprtAnchor [in] : Anchor 값
 @return 없음
 @see atWINOBJ_SetAnchor
*/
void	atWINOBJ_SetAnchor2( atHWINOBJ hWinObj, atMARGIN *lprtAnchor )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;

	if( lprtAnchor != atNULL ){
		atWINOBJ_SetAnchor( hWinObj, lprtAnchor->nTop, lprtAnchor->nBottom, lprtAnchor->nLeft, lprtAnchor->nRight );
	} else {
		atWINOBJ_SetAnchor( hWinObj, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE, atANCHOR_IGNORE );
		lpWinObj->dwAttr &= ~atOBJATTR_USEANCHOR;
	}
}


/** 
 @brief WinObj의 SoftKey를 활성화시킨다.
 
 WinObj의 SoftKey를 활성화시킨다.

 @param hWinObj [in] : WinObj의 핸들
 @param bActivate [in] : atTRUE = SoftKey 활성화\n
						 atFALSE = SoftKey 비활성화
 @return 없음
 @remark 만약 포커스를 가진 WinObj라면 현 개체의 소프트키를 실제 소프트키에 적용까지 시켜 준다.
*/
void atWINOBJ_ActivateSoftKey( atHWINOBJ hWinObj, atBOOL bActivate )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( bActivate == (lpWinObj->dwSoftkeyFlag & atOBJSOFTKEYFLAG_ACTIVATE) ) return;

	if( bActivate ){
		lpWinObj->dwSoftkeyFlag |= atOBJSOFTKEYFLAG_ACTIVATE;
		//if( atWINOBJ_IsFocus(hWinObj) )
			atWINOBJ_RealizeSoftKey( hWinObj );
	} else {
		lpWinObj->dwSoftkeyFlag &= ~atOBJSOFTKEYFLAG_ACTIVATE;
	}
}


/** 
 @brief WinObj의 SoftKey를 설정한다.
 
 WinObj의 SoftKey를 설정한다.

 @param hWinObj [in] : WinObj의 핸들
 @param szLeft [in] : 왼쪽 SoftKey 문자열
 @param szCenter [in] : 가운데 SoftKey 문자열
 @param szRight [in] : 오른쪽 SoftKey 문자열
 @return 없음
 @remark 만약 포커스를 가진 WinObj라면 현 개체의 소프트키를 실제 소프트키에 적용까지 시켜 준다.
*/
void atWINOBJ_SetSoftKey( atHWINOBJ hWinObj, atLPTSTR szLeft, atLPTSTR szCenter, atLPTSTR szRight )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;

	if( szLeft ){
		lpWinObj->dwSoftkeyFlag |= atOBJSOFTKEYFLAG_LEFT;
		atUIAPI_strcpy( lpWinObj->szSoftKey[atSOFTKEY_LEFT], szLeft );
	}
	if( szCenter ){
		lpWinObj->dwSoftkeyFlag |= atOBJSOFTKEYFLAG_CENTER;
		atUIAPI_strcpy( lpWinObj->szSoftKey[atSOFTKEY_CENTER], szCenter );
	}
	if( szRight ){
		lpWinObj->dwSoftkeyFlag |= atOBJSOFTKEYFLAG_RIGHT;
		atUIAPI_strcpy( lpWinObj->szSoftKey[atSOFTKEY_RIGHT], szRight );
	}

	//if( atWINOBJ_IsFocus(hWinObj) )
	//	atWINOBJ_RealizeSoftKey( hWinObj );
}


/** 
 @brief WinObj의 SoftKey의 Enable/Disable 상태를 설정한다.
 
 WinObj의 SoftKey의 Enable/Disable 상태를 설정한다.

 @param hWinObj [in] : WinObj의 핸들
 @param bLeft [in] : 왼쪽 SoftKey Enable/Disable
 @param bCenter [in] : 가운데 SoftKey Enable/Disable
 @param bRight [in] : 오른쪽 SoftKey Enable/Disable
 @return 없음
 @remark 만약 포커스를 가진 WinObj라면 현 개체의 소프트키를 실제 소프트키에 적용까지 시켜 준다.
*/
void atWINOBJ_SetSoftKeyEnabled( atHWINOBJ hWinObj, atBOOL bLeft, atBOOL bCenter, atBOOL bRight )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;

	if( bLeft ) 
		lpWinObj->dwSoftkeyFlag &= ~atOBJSOFTKEYFLAG_LEFT_DISABLE;
	else
		lpWinObj->dwSoftkeyFlag |= atOBJSOFTKEYFLAG_LEFT_DISABLE;
	if( bCenter ) 
		lpWinObj->dwSoftkeyFlag &= ~atOBJSOFTKEYFLAG_CENTER_DISABLE;
	else
		lpWinObj->dwSoftkeyFlag |= atOBJSOFTKEYFLAG_CENTER_DISABLE;
	if( bRight ) 
		lpWinObj->dwSoftkeyFlag &= ~atOBJSOFTKEYFLAG_RIGHT_DISABLE;
	else
		lpWinObj->dwSoftkeyFlag |= atOBJSOFTKEYFLAG_RIGHT_DISABLE;
	
	//if( atWINOBJ_IsFocus(hWinObj) )
		atWINOBJ_RealizeSoftKey( hWinObj );
}


/** 
 @brief WinObj의 SoftKey의 Enable/Disable 상태를 얻는다.
 
 WinObj의 SoftKey의 Enable/Disable 상태를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @param pbLeft [in] : 왼쪽 SoftKey Enable/Disable 값을 얻을 포인터
 @param pbCenter [in] : 가운데 SoftKey Enable/Disable 값을 얻을 포인터
 @param pbRight [in] : 오른쪽 SoftKey Enable/Disable 값을 얻을 포인터
 @return 없음
*/
void atWINOBJ_GetSoftKeyEnabled( atHWINOBJ hWinObj, atBOOL *pbLeft, atBOOL *pbCenter, atBOOL *pbRight )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;

	if( pbLeft ) *pbLeft = (lpWinObj->dwSoftkeyFlag & atOBJSOFTKEYFLAG_LEFT_DISABLE)? atFALSE : atTRUE;
	if( pbCenter ) *pbCenter = (lpWinObj->dwSoftkeyFlag & atOBJSOFTKEYFLAG_CENTER_DISABLE)? atFALSE : atTRUE;
	if( pbRight ) *pbRight = (lpWinObj->dwSoftkeyFlag & atOBJSOFTKEYFLAG_RIGHT_DISABLE)? atFALSE : atTRUE;
}


/** 
 @brief WinObj의 SoftKeyd IDS를 설정한다.
 
 WinObj의 SoftKey IDS를 설정한다.

 @param hWinObj [in] : WinObj의 핸들
 @param szLeft [in] : 왼쪽 SoftKey IDS
 @param szCenter [in] : 가운데 SoftKey IDS
 @param szRight [in] : 오른쪽 SoftKey IDS
 @return 없음
 @remark 만약 포커스를 가진 WinObj라면 현 개체의 소프트키를 실제 소프트키에 적용까지 시켜 준다.
*/
void atWINOBJ_SetSoftKeyIDS( atHWINOBJ hWinObj, atLONG leftIds, atLONG centerIds, atLONG rightIds)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;

	atUIAPI_memset(lpWinObj->szSoftKey[atSOFTKEY_LEFT  ], 0, atSOFTKEY_TEXT_LEN);
	atUIAPI_memset(lpWinObj->szSoftKey[atSOFTKEY_CENTER], 0, atSOFTKEY_TEXT_LEN);
	atUIAPI_memset(lpWinObj->szSoftKey[atSOFTKEY_RIGHT ], 0, atSOFTKEY_TEXT_LEN);

	lpWinObj->nSoftKeyIDS[atSOFTKEY_LEFT  ] = 0;
	lpWinObj->nSoftKeyIDS[atSOFTKEY_CENTER] = 0;
	lpWinObj->nSoftKeyIDS[atSOFTKEY_RIGHT ] = 0;


	if( leftIds > -1 ){
		lpWinObj->dwSoftkeyFlag |= atOBJSOFTKEYFLAG_LEFT;
		lpWinObj->dwSoftkeyFlag |= atOBJSOFTKEYFLAG_IDS_LEFT;
		lpWinObj->nSoftKeyIDS[atSOFTKEY_LEFT] = leftIds;
	}
	if( centerIds > -1 ){
		lpWinObj->dwSoftkeyFlag |= atOBJSOFTKEYFLAG_CENTER;
		lpWinObj->dwSoftkeyFlag |= atOBJSOFTKEYFLAG_IDS_CENTER;
		lpWinObj->nSoftKeyIDS[atSOFTKEY_CENTER] = centerIds;
	}
	if( rightIds > -1 ){
		lpWinObj->dwSoftkeyFlag |= atOBJSOFTKEYFLAG_RIGHT;
		lpWinObj->dwSoftkeyFlag |= atOBJSOFTKEYFLAG_IDS_RIGHT;
		lpWinObj->nSoftKeyIDS[atSOFTKEY_RIGHT] = rightIds;
	}

	//if( atWINOBJ_IsFocus(hWinObj) )
		atWINOBJ_RealizeSoftKey( hWinObj );
}


//#ifndef COUNTOF
// #define COUNTOF(OBJs)  (sizeof(OBJs)/sizeof(OBJs[0]) )
//#endif


/** 
 @brief WinObj의 SoftKey를 실제 SoftKey로 적용시킨다.
 
 WinObj의 SoftKey를 실제 SoftKey로 적용시킨다.

 @param hWinObj [in] : WinObj의 핸들
 @return 없음
*/
void atWINOBJ_RealizeSoftKey( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atLPTSTR szLeft, szCenter, szRight;
	atBOOL bLeft, bCenter, bRight;

	if( lpWinObj == atNULL ) return;
	if( !lpWinObj->bActivate ) return;
	
	if( lpWinObj->dwSoftkeyFlag & atOBJSOFTKEYFLAG_ACTIVATE ){
		if(lpWinObj->dwSoftkeyFlag & atOBJSOFTKEYFLAG_IDS_LEFT)
			atUIAPI_LoadString(lpWinObj->nSoftKeyIDS[atSOFTKEY_LEFT],  lpWinObj->szSoftKey[atSOFTKEY_LEFT],   COUNTOF(lpWinObj->szSoftKey[atSOFTKEY_LEFT]));
		if(lpWinObj->dwSoftkeyFlag & atOBJSOFTKEYFLAG_IDS_CENTER)
			atUIAPI_LoadString(lpWinObj->nSoftKeyIDS[atSOFTKEY_CENTER],lpWinObj->szSoftKey[atSOFTKEY_CENTER], COUNTOF(lpWinObj->szSoftKey[atSOFTKEY_CENTER]));
		if(lpWinObj->dwSoftkeyFlag & atOBJSOFTKEYFLAG_IDS_RIGHT)
			atUIAPI_LoadString(lpWinObj->nSoftKeyIDS[atSOFTKEY_RIGHT], lpWinObj->szSoftKey[atSOFTKEY_RIGHT], COUNTOF(lpWinObj->szSoftKey[atSOFTKEY_RIGHT]));

		szLeft   = ( lpWinObj->dwSoftkeyFlag & atOBJSOFTKEYFLAG_LEFT )  ? lpWinObj->szSoftKey[atSOFTKEY_LEFT] : atNULL;
		szCenter = ( lpWinObj->dwSoftkeyFlag & atOBJSOFTKEYFLAG_CENTER )? lpWinObj->szSoftKey[atSOFTKEY_CENTER] : atNULL;
		szRight  = ( lpWinObj->dwSoftkeyFlag & atOBJSOFTKEYFLAG_RIGHT ) ? lpWinObj->szSoftKey[atSOFTKEY_RIGHT] : atNULL;
		bLeft = ( lpWinObj->dwSoftkeyFlag & atOBJSOFTKEYFLAG_LEFT_DISABLE )  ? atFALSE : atTRUE;
		bCenter = ( lpWinObj->dwSoftkeyFlag & atOBJSOFTKEYFLAG_CENTER_DISABLE )  ? atFALSE : atTRUE;
		bRight = ( lpWinObj->dwSoftkeyFlag & atOBJSOFTKEYFLAG_RIGHT_DISABLE )  ? atFALSE : atTRUE;
		atSOFTKEY_SetSoftKeyText( szLeft, szCenter, szRight );
		atSOFTKEY_SetSoftKeyEnabled( bLeft, bCenter, bRight );
		atSOFTKEY_RefreshSoftKey();			
	}
}


/** 
 @brief WinObj를 화면에 표시하거나 감춘다.
 
 WinObj를 화면에 표시하거나 감춘다.

 @param hWinObj [in] : WinObj의 핸들
 @param bShow [in] : atTRUE = Show \n
					 atFALSE = Hide
 @return 없음
 @remark 주로 내부용으로 사용되며, 사용자가 호출시엔 해당 개체의 부모의 atWINOBJ_ShowChildWinObj를 이용한다.\n
		 OBJMSG_SHOW / OBJMSG_HIDE를 발생시킨다.
*/
void atWINOBJ_SetVisible( atHWINOBJ hWinObj, atBOOL bShow )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return;
	if( lpWinObj->bShow == bShow ) return;

	lpWinObj->bShow = bShow;

	if( bShow == atTRUE ){
		atWINOBJ_AdjustAllChildWinObjs( hWinObj );
		atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_SHOW, 0, 0 );
	} else {
		atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_HIDE, 0, 0 );
	}
}


/** 
 @brief WinObj가 화면에 표시되는가 여부를 알아낸다.
 
 WinObj가 화면에 표시되는가 여부를 알아낸다.

 @param hWinObj [in] : WinObj의 핸들
 @return Show 상태면 atTRUE를, Hide 상태면 atFALSE
*/
atBOOL atWINOBJ_IsVisible( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atFALSE;

	if( !lpWinObj->bActivate ) return atFALSE;
	if( !lpWinObj->bShow ) return atFALSE;
	if( lpWinObj->hParentObj ) return atWINOBJ_IsVisible( lpWinObj->hParentObj );

	return atTRUE;
}


/** 
 @brief WinObj가 이벤트를 받을 수 있는지 여부를 결정한다.
 
 WinObj가 이벤트를 받을 수 있는지 여부를 결정한다.

 @param hWinObj [in] : WinObj의 핸들
 @param bEnable [in] : atTRUE = Enable\n
					   atFALSE = Disable
 @return 없음
*/
void atWINOBJ_Enabled( atHWINOBJ hWinObj, atBOOL bEnable )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return;
	if( lpWinObj->bEnable == bEnable ) return;

	lpWinObj->bEnable = bEnable;

	if( bEnable == atTRUE ){
//		atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_SHOW, 0, 0 );
//		if(atWINOBJ_IsVisible(hWinObj)) atWINOBJ_Draw(hWinObj, atFALSE);
	} else {
//		atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_HIDE, 0, 0 );
	}
}


/** 
 @brief WinObj가 이벤트를 받을 수 있는 여부를 알아낸다.
 
 WinObj가 이벤트를 받을 수 있는 여부를 알아낸다.

 @param hWinObj [in] : WinObj의 핸들
 @return Enable 상태면 atTRUE, Disable 상태면 atFALSE
*/
atBOOL atWINOBJ_IsEnable( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atFALSE;

	if( !lpWinObj->bActivate ) return atFALSE;
	if( !lpWinObj->bEnable ) return atFALSE;
	if( !lpWinObj->bShow ) return atFALSE;
	if( lpWinObj->hParentObj ) return atWINOBJ_IsEnable( lpWinObj->hParentObj );

	return atTRUE;
}


/** 
 @brief WinObj에 포커스를 지정해 준다.
 
 WinObj에 포커스를 지정해 준다.

 @param hWinObj [in] : WinObj의 핸들
 @param bFocus [in] : 포커스
 @return 없음
 @remark 주로 내부용으로 사용되며, 사용자가 호출시엔 해당 개체의 부모의 atWINOBJ_SetChildWinObjFocus를 이용해서 포커스를 이동시킨다.\n
		 OBJMSG_GOTFOCUS / OBJMSG_LOSTFOCUS를 발생시킨다.
*/
/*
atBOOL atWINOBJ_SetFocus( atHWINOBJ hWinObj, atBOOL bFocus )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atFALSE;
	if( lpWinObj->bFocus == bFocus ) return bFocus;

	if( bFocus ){
		if( !atWINOBJ_IsEnable(hWinObj) ) return atFALSE;
		if( !atWINOBJ_IsFocusable(hWinObj) ) return atFALSE;
		lpWinObj->bFocus = atTRUE;
		if( atWINOBJ_IsFocus(hWinObj) )
			atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_GOTFOCUS, 0, 0 );
	} else {
		if( lpWinObj->bFocus ){
			atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_LOSTFOCUS, 0, 0 );
			lpWinObj->bFocus = atFALSE;
		}
	}

	return lpWinObj->bFocus;
}
*/

/** 
 @brief 포커스를 가진 WinObj인가 여부를 얻는다.
 
 포커스를 가진 WinObj인가 여부를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @return 현재 포커스를 가지고 있으면 atTRUE, 아니면 atFALSE
*/
/*
atBOOL atWINOBJ_IsFocus( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atFALSE;

	if( !lpWinObj->bFocus ) return atFALSE;
	if( lpWinObj->hParentObj ) return atWINOBJ_IsFocus( lpWinObj->hParentObj );

	return atTRUE;
}
*/


/** 
 @brief WinObj가 포커스를 가질 수 있는지 여부를 얻는다.
 
 WinObj가 포커스를 가질 수 있는지 여부를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @return 포커스를 가질 수 있는 개체면 atTREU, 아니면 atFALSE
*/
atBOOL atWINOBJ_IsFocusable( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atFALSE;
	
	return lpWinObj->dwAttr & atOBJATTR_FOCUSABLE;
}


/** 
 @brief WinObj가 포커스를 가질 수 있는가 없는가 여부를 결정한다.
 
 WinObj가 포커스를 가질 수 있는가 없는가 여부를 결정한다.

 @param hWinObj [in] : WinObj의 핸들
 @param bFocusable [in] : 포커스를 가질 수 있는가 없는가 여부
 @return 없음
*/
void atWINOBJ_SetFocusable( atHWINOBJ hWinObj, atBOOL bFocusable )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atDWORD attr;
	atBOOL bOldFocusable;

	if( lpWinObj == atNULL ) return;

	attr = lpWinObj->dwAttr;
	bOldFocusable = (attr & atOBJATTR_FOCUSABLE)? atTRUE : atFALSE;

	if ( bOldFocusable != bFocusable ){
		if( bFocusable ) attr |= atOBJATTR_FOCUSABLE; else attr &= ~atOBJATTR_FOCUSABLE;
		lpWinObj->dwAttr = attr;

		// 포커스를 가진 obj의 focusable 속성을 없애버렸을 경우엔 next sibliling obj로 포커스를 옮긴다.
		if( !bFocusable /* && lpWinObj->bFocus*/ ){
			atHWINOBJ hParentObj = atWINOBJ_GetParentObj(hWinObj);
			if( hParentObj != atNULL ){
				if(atWINOBJ_GetCurChildWinObj( hParentObj ) == hWinObj) {
					atHWINOBJ hNextObj = atWINOBJ_GetNextFocusableChildWinObj( hParentObj, hWinObj );
					//if(hNextObj != atNULL) {
						atWINOBJ_SetCurChildWinObj(hParentObj, hNextObj);

						//if( atWINOBJ_GetClassID(hParentObj) == 0x0001 ){ // 부모가 윈도우 타입일 때만
						//	atWINDOW_SetControlFocus( hWnd, hctl );
						//}
						//if( hControl ) atCONTROL_Refresh(hControl);
						//if( hctl ) atCONTROL_Refresh(hctl);
					//}
				}
			}				
		}
	}
}


/** 
 @brief Anchor의 사용 여부를 결정한다.
 
 Anchor의 사용 여부를 결정한다.\n
 기본적으로 초기 생성시에는 Anchor는 사용 가능하도록 설정되지만,\n
 atWINOBJ_SetPosition이나 atWINOBJ_SetRect, atWINOBJ_SetRegion에 의해 자동으로 해제될 수가 있다.\n
 이 경우, 다시 Anchor를 활성화시키고자 한다면 이 함수를 써서 UseAnchor 속성을 켜줘야 한다.


 @param hWinObj [in] : WinObj의 핸들
 @return 없음
 @remark 기본적으로 초기 생성시에는 Anchor는 사용 가능하도록 설정되지만, 
 @see atWINOBJ_SetAnchor atWINOBJ_SetPosition atWINOBJ_SetRect atWINOBJ_SetRegion
*/
void atWINOBJ_UseAnchor( atHWINOBJ hWinObj, atBOOL bUseAnchor )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return;

	if( bUseAnchor ){
		lpWinObj->dwAttr |= atOBJATTR_USEANCHOR;
	} else {
		lpWinObj->dwAttr &= ~atOBJATTR_USEANCHOR;
	}
}


/** 
 @brief WinObj의 핸들러를 얻는다.
 
 WinObj의 핸들러를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @return WinObj의 핸들러
*/
atLPWINOBJPROC	atWINOBJ_GetObjProc( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atNULL;
	
	return lpWinObj->fnObjProc;
}


/** 
 @brief WinObj에 핸들러를 재설정한다.
 
 WinObj에 핸들러를 재설정한다.\n
 주로 서브클래싱을 위해 사용자가 핸들러를 재정의 할 때 쓴다.\n
 이 경우, 반환된 원래의 핸들러는 사용자가 정의하여 지정해 준 핸들러 속에서 atWINOBJ_CallObjProc를 이용해 호출해 줘야 한다.

 @param hWinObj [in] : WinObj의 핸들
 @param lpProc [in] : WinObj의 핸들러
 @return 설정 이전의 핸들러
 @see atWINOBJ_CallObjProc
*/
atLPWINOBJPROC	atWINOBJ_SetObjProc( atHWINOBJ hWinObj, atLPWINOBJPROC lpProc )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atLPWINOBJPROC lpOldProc;

	if( lpWinObj == atNULL ) return atNULL;

	lpOldProc = lpWinObj->fnObjProc;
	lpWinObj->fnObjProc = lpProc;
	
	return lpOldProc;
}


#ifdef __USES_NOTIFIER
/** 
 @brief WinObj의 Notify 핸들러를 얻는다.
 
 WinObj의 Notify 핸들러를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @return WinObj의 Notify 핸들러
*/
atLPWINOBJNOTIFIER	atWINOBJ_GetObjNotifier( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atNULL;
	
	return lpWinObj->fnObjNotifier;
}


/** 
 @brief WinObj의 Notify Data를 얻는다.
 
 WinObj의 Notify Data를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @return WinObj의 Notify Data 포인터
*/
atLPVOID	atWINOBJ_GetObjNotifyData( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atNULL;
	
	return lpWinObj->lpObjNotifyData;
}


/** 
 @brief WinObj에 Notify 핸들러를 재설정한다.
 
 WinObj에 Notify 핸들러를 재설정한다.\n

 @param hWinObj [in] : WinObj의 핸들
 @param lpNotifier [in] : WinObj의 Notify 핸들러
 @param lpNotifyData [in] : WinObj의 Notify 데이터
 @return 설정 이전의 핸들러
 @see atWINOBJ_CallObjProc
*/
atLPWINOBJNOTIFIER	atWINOBJ_SetObjNotifier( atHWINOBJ hWinObj, atLPWINOBJNOTIFIER lpNotifier, atLPVOID lpNotifyData )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atLPWINOBJNOTIFIER lpOldNotifier;

	if( lpWinObj == atNULL ) return atNULL;

	lpOldNotifier = lpWinObj->fnObjNotifier;
	lpWinObj->fnObjNotifier = lpNotifier;
	lpWinObj->lpObjNotifyData = lpNotifyData;
	
	return lpOldNotifier;
}
#endif


/** 
 @brief WinObj의 핸들러를 직접 호출한다.
 
 WinObj의 핸들러를 직접 호출한다.

 @param hWinObj [in] : WinObj의 핸들
 @param nMsg [in] : 전송할 메시지
 @param lParam1, lParam2 [in] : 메시지의 파라미터
 @return 메시지 핸들러의 결과값
 @remark 핸들러의 서브클래싱을 위해 존재. 보통은 내부용으로 쓰인다. \n
		 사용자가 직접 메시지 핸들러를 호출할 때엔 atWINOBJ_ProcessObjProc을 사용한다.
 @see atWINOBJ_ProcessObjProc
*/
int atWINOBJ_CallObjProc( atLPWINOBJPROC lpProc, atHWINOBJ hWinObj, int nMsg, long lParam1, long lParam2 )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	int r = 0;

	if( lpWinObj == atNULL ) return -1;
	
	if( lpProc ) r = lpProc( hWinObj, nMsg, lParam1, lParam2 );

	return r;
}


/** 
 @brief WinObj의 메시지 프로세싱을 진행한다.
 
 WinObj의 메시지 프로세싱을 진행한다.

 @param hWinObj [in] : WinObj의 핸들
 @param nMsg [in] : 전송할 메시지
 @param lParam1, lParam2 [in] : 메시지의 파라미터
 @return 메시지 핸들러의 결과값
 @remark 주로 내부용으로 쓰인다.
*/
int atWINOBJ_ProcessObjProc( atHWINOBJ hWinObj, int nMsg, long lParam1, long lParam2 )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	int r = 0;

	if( lpWinObj == atNULL ) return -1;
	
	if( lpWinObj->fnObjProc ) r = lpWinObj->fnObjProc( hWinObj, nMsg, lParam1, lParam2 );
	if( r == 0 ){
	  // 이전 처리가 윈도우를 파괴했다면, 이후 처리는 하지 않는다.
	  if( atWINOBJ_GetPtr(hWinObj) )
		if( lpWinObj->fnDefObjProc ) r = lpWinObj->fnDefObjProc( hWinObj, nMsg, lParam1, lParam2 );
	  else
	    r = 1;
	}
//	if( r == 0 ) r = atWINOBJ_DefObjProc( hWinObj, nMsg, lParam1, lParam2 );

#ifdef __USES_NOTIFIER
	if( lpWinObj->fnObjNotifier ) lpWinObj->fnObjNotifier( hWinObj, nMsg, lParam1, lParam2, lpWinObj->lpObjNotifyData, r );
#endif

	return r;
}


/** 
 @brief WinObj의 GDC를 얻는다.
 
 WinObj의 GDC(Graphic Device Context)를 얻는다.\n
 기본적으로 WinObj는 GDC를 한개씩 갖게 된다.\n
 이 함수를 호출함과 동시에 실시간으로 GDC의 영역이 재계산 된다.

 @param hWinObj [in] : WinObj의 핸들
 @return WinObj의 GDC 핸들
*/
atHGDC atWINOBJ_GetGDC( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atHGDC hGDC;
	atREGION rgn, rgn2, rgn3;
	atRECT rct;
	atBOOL ret;

	if( lpWinObj == atNULL ) return atNULL;

	if( !atWINOBJ_IsActivate(hWinObj) ) return atNULL;
	if( !atWINOBJ_IsVisible(hWinObj) ) return atNULL;
	
	hGDC = lpWinObj->hGDC;

	atWINOBJ_GetRegion( hWinObj, &rgn );
	rgn.nX = rgn.nY = 0;

	ret = atWINOBJ_ConvScreenValidateRegion( hWinObj, &rgn2, &rgn );
	if( !ret ){
		if( hGDC ) atUIAPI_ActivateGDC( hGDC, atFALSE );
		return atNULL;
	}

	atWINOBJ_ConvScreenRegion( hWinObj, &rgn3, &rgn );
	if( hGDC == atNULL ){
		hGDC = atUIAPI_CreateGDC( rgn3.nX, rgn3.nY, rgn3.nWidth, rgn3.nHeight, atUIAPI_GetScreenBPP(), 0L );
		if( hGDC == atNULL ) return atNULL;
		lpWinObj->hGDC = hGDC;
	}
	
	if( hGDC == atNULL ) return atNULL;
	
	ret = atUIAPI_SetGDCArea( hGDC, &rgn3 );
	if( !ret ){
		atUIAPI_ActivateGDC( hGDC, atFALSE );
		return atNULL;
	}
	rct.nStartX = rgn2.nX - rgn3.nX;
	rct.nStartY = rgn2.nY - rgn3.nY;
	rct.nEndX = rct.nStartX + rgn2.nWidth - 1;
	rct.nEndY = rct.nStartY + rgn2.nHeight - 1;
	atUIAPI_SetClipRect( hGDC, &rct );

	return hGDC;
}


/** 
 @brief WinObj에 Child WinObj를 추가한다.
 
 WinObj에 Child WinObj를 추가한다.\n

 @param hWinObj [in] : WinObj의 핸들
 @param hChildWinObj [in] : Child WinObj의 핸들
 @return 없음
*/
atBOOL atWINOBJ_AddChildWinObj( atHWINOBJ hWinObj, atHWINOBJ hChildWinObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	
	if( (lpWinObj == atNULL) || (hChildWinObj == atNULL) ) return atFALSE;

	if( lpWinObj->hChildObjMng == atNULL ){
		lpWinObj->hChildObjMng = atWINOBJMNG_CreateWinObjMng( hWinObj, 0 );
		if( lpWinObj->hChildObjMng == atNULL ) return atFALSE;
	}

	atWINOBJMNG_RegistWinObj( lpWinObj->hChildObjMng, hChildWinObj );
	
	return atTRUE;
}


/** 
 @brief Child 개체를 소멸
 
 Child 개체를 소멸

 @param hWinObj [in] : WinObj의 핸들
 @param hChildWinObj [in] : Child WinObj의 핸들
 @return 없음
*/
void atWINOBJ_RemoveChildWinObj( atHWINOBJ hWinObj, atHWINOBJ hChildWinObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;
	
	atWINOBJMNG_DestroyWinObj( lpWinObj->hChildObjMng, hWinObj );
}


/** 
 @brief 내장된 Child 개체의 갯수를 구한다.
 
 내장된 Child 개체의 갯수를 구한다.

 @param hWinObj [in] : WinObj의 핸들
 @return 내장된 Child 개체의 갯수
*/
atINT atWINOBJ_GetChildWinObjCount( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return 0;
	
	return atWINOBJMNG_GetWinObjCount( lpWinObj->hChildObjMng );
}


/** 
 @brief 현재 포커스를 가진 Child 개체를 얻는다.
 
 현재 포커스를 가진 Child 개체를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @return 포커스를 가진 Child 개체의 핸들
*/
atHWINOBJ atWINOBJ_GetCurChildWinObj( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	return atWINOBJMNG_GetCurWinObj(lpWinObj->hChildObjMng);
}


/** 
 @brief ID와 일치하는 Child개체를 얻는다.
 
 ID와 일치하는 Child개체를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @param nWinObjID [in] : Child 개체의 ID
 @return ID와 일치하는 Child개체의 핸들
 @remark ID는 Unique하지 않기 때문에, 중복된 ID를 가질 경우 특정한 개체만 얻으낼 수 있다.
*/
atHWINOBJ atWINOBJ_FindChildWinObjByID( atHWINOBJ hWinObj, atINT nWinObjID )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	return atWINOBJMNG_FindWinObjByID(lpWinObj->hChildObjMng, nWinObjID);
}


/** 
 @brief 해당 이름의 Child개체를 얻는다.
 
 해당 이름의 Child개체를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @param szWinObjName [in] : Child 개체의 이름
 @return 해당 이름의 Child개체의 핸들
 @remark 이름은 Unique하지 않기 때문에, 중복된 이름을 가질 경우 특정한 개체만 얻으낼 수 있다.
*/
atHWINOBJ atWINOBJ_FindChildWinObjByName( atHWINOBJ hWinObj, atLPTSTR szWinObjName )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	return atWINOBJMNG_FindWinObjByName(lpWinObj->hChildObjMng, szWinObjName);
}


/** 
 @brief Child 개체에 포커스를 준다.
 
 Child 개체에 포커스를 준다.

 @param hWinObj [in] : WinObj의 핸들
 @param hChildWinObj [in] : Child WinObj의 핸들
 @return 성공시 atTRUE, 실패시 atFALSE
*/
atBOOL atWINOBJ_SetCurChildWinObj(atHWINOBJ hWinObj, atHWINOBJ hChildWinObj)
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atHWINOBJ	hOldWinObj;
	if( lpWinObj == atNULL ) return atFALSE;

	hOldWinObj = atWINOBJMNG_GetCurWinObj(lpWinObj->hChildObjMng);

	atWINOBJMNG_SetCurWinObj( lpWinObj->hChildObjMng, hChildWinObj );

	if( lpWinObj->bActivate ){
		if( hOldWinObj || atWINOBJMNG_GetCurWinObj(lpWinObj->hChildObjMng) ){
			atWINOBJ_ProcessObjProc( hWinObj, OBJMSG_CHANGECHILDOBJFOCUS, (long)atWINOBJMNG_GetCurWinObj(lpWinObj->hChildObjMng), (long)hOldWinObj ); 		
		}
	}

	return atWINOBJMNG_GetCurWinObj(lpWinObj->hChildObjMng) != atNULL;
}


/** 
 @brief 포커스를 가질 수 있는 다음 Child 개체를 얻는다.
 
 포커스를 가질 수 있는 다음 Child 개체를 얻는다.

 @param hWinObj [in] : WinObj의 핸들
 @param hChildWinObj [in] : Child WinObj의 핸들
 @return 성공시 Next Child WinObj의 핸들, 실패시 atNULL
*/
atHWINOBJ atWINOBJ_GetNextFocusableChildWinObj(atHWINOBJ hWinObj, atHWINOBJ hChildWinObj)
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atHWINOBJ hNextObj = atNULL;
	if( lpWinObj == atNULL ) return atFALSE;

	do {
		hNextObj = atWINOBJMNG_GetNextEnabledWinObj(lpWinObj->hChildObjMng, hChildWinObj);
		if( (hNextObj != atNULL) && atWINOBJ_IsFocusable(hNextObj) ) {
			break;
		}
	} while( hNextObj != atNULL && hNextObj != hChildWinObj );

	if( (hNextObj != atNULL) && atWINOBJ_IsFocusable(hNextObj) ) {
		return hNextObj;
	} else {
		return atNULL;
	}
}


/** 
 @brief Child 개체를 화면에 표시하거나 감춘다.
 
 Child 개체를 화면에 표시하거나 감춘다.

 @param hWinObj [in] : WinObj의 핸들
 @param hChildWinObj [in] : Child WinObj의 핸들
 @param bShow [in] : Show / Hide
 @return 없음
*/
void atWINOBJ_ShowChildWinObj( atHWINOBJ hWinObj, atHWINOBJ hChildWinObj, atBOOL bShow )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;
	
	atWINOBJMNG_ShowWinObj( lpWinObj->hChildObjMng, hChildWinObj, bShow );
}
 

/** 
 @brief Child 개체를 가장 앞쪽으로 나오게 한다.
 
 Child 개체를 가장 앞쪽으로 나오게 한다.

 @param hWinObj [in] : WinObj의 핸들
 @param hChildWinObj [in] : Child WinObj의 핸들
 @return 실제로 맨 앞에 나오게 된 Child 개체의 핸들
*/
atHWINOBJ atWINOBJ_SetTopChildWinObj( atHWINOBJ hWinObj, atHWINOBJ hChildWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	return atWINOBJMNG_SetTopWinObj( lpWinObj->hChildObjMng, hChildWinObj);
}


/** 
 @brief Child 개체를 가장 뒷쪽으로 보낸다.
 
 Child 개체를 가장 뒷쪽으로 보낸다.

 @param hWinObj [in] : WinObj의 핸들
 @param hChildWinObj [in] : Child WinObj의 핸들
 @return 실제로 맨 뒤에 위치하는 Child 개체의 핸들
*/
atHWINOBJ atWINOBJ_SetBottomChildWinObj( atHWINOBJ hWinObj, atHWINOBJ hChildWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	return atWINOBJMNG_SetBottomWinObj( lpWinObj->hChildObjMng, hChildWinObj );
}


/** 
 @brief WinObj의 Default 메시지 핸들러
 
 WinObj의 Default 메시지 핸들러

 @param hWinObj [in] : WinObj의 핸들
 @param nMsg [in] : 전송할 메시지
 @param lParam1, lParam2 [in] : 메시지의 파라미터
 @return 메시지 핸들러의 결과값
 @remark WinObj를 상속받은 개체는 반드시 이 핸들러를 포함시켜 줘야 한다.
*/
void atWINOBJ_PostMessageAllChildWinObjs( atHWINOBJ hWinObj, int nMsg, long lParam1, long lParam2 )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;

	atWINOBJMNG_SendMessageAllWinObjs( _TO_WINOBJMNG(lpWinObj->hChildObjMng), nMsg, lParam1, lParam2 );
}


/** 
 @brief WinObj에 내장된 모든 Child 개체를 소멸시킨다.
 
 WinObj에 내장된 모든 Child 개체를 소멸시킨다.

 @param hWinObj [in] : WinObj의 핸들
 @return 없음
*/
void atWINOBJ_RemoveAllChildWinObjs( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;
	
	atWINOBJMNG_DestroyWinObjMng( lpWinObj->hChildObjMng );
	
	lpWinObj->hChildObjMng = atNULL;
}
 

/** 
 @brief WinObj에 내장된 모든 Child 개체를 그린다. (내부용)
 
 WinObj에 내장된 모든 Child 개체를 그린다. (내부용)

 @param hWinObj [in] : WinObj의 핸들
 @param bFocused [in] : 현 개체가 focused 된 것인가 여부 
 @return 그려진 내장 개체의 갯수
 @remark 내부 호출용.
*/
int atWINOBJ_DrawAllChildWinObjs( atHWINOBJ hWinObj, atBOOL bFocused )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return -1;

	return atWINOBJMNG_RedrawAllWinObj( lpWinObj->hChildObjMng, bFocused );
}


/** 
 @brief 내장된 Child 개체의 크기와 위치를 재정렬
 
 내장된 Child 개체의 크기와 위치를 재정렬시킨다.\n
 이때 Anchor가 사용된다.

 @param hWinObj [in] : WinObj의 핸들
 @return 없음
*/
void atWINOBJ_AdjustAllChildWinObjs( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;

	atWINOBJMNG_AdjustAllWinObjArea( lpWinObj->hChildObjMng );
}


/** 
 @brief WinObj의 내장 타이머를 추가한다.
 
 WinObj의 내장 타이머를 추가한다.\n
 타이머 동작시, 해당 WinObj에 OBJMSG_TIMER 이벤트를 전달해 준다.\n
 WinObj가 삭제될 때, 자동으로 함께 소멸한다.

 @param hWinObj [in] : WinObj의 핸들
 @param id [in] : 타이머의 ID. 타이머 이벤트 발생시, 첫번째 파라미터로 전달된다.
 @param interval [in] : 타이머의 호출 주기. ms 단위
 @param IsRepeat [in] : 반복해서 호출되는가 여부.
 @param IsEnable [in] : 생성과 동시에 타이머가 동작할 것인가 여부.
 @param IsFocus [in] : WinObj가 포커스를 가졌을 때만 동작할 것인가 여부. 
 @return 추가된 타이머의 핸들
*/
atHWINOBJTIMER atWINOBJ_AddTimer( atHWINOBJ hWinObj, int id, long interval, atBOOL IsRepeat, atBOOL IsEnable, atBOOL IsFocus )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atWINOBJTIMER *tmr;	

	tmr = (atWINOBJTIMER *)atUIAPI_AllocMem( sizeof(atWINOBJTIMER) );	
	if( tmr == atNULL ) return atNULL;

	tmr->hWinObj = hWinObj;
	tmr->id = id;
	tmr->bFocus = IsFocus;
	tmr->hTimer = atUIAPI_AddTimer( interval, cbWinTimer, (void *)tmr, IsRepeat, IsEnable );

	if( lpWinObj->lpTimerList == atNULL ){
		lpWinObj->lpTimerList = atUIAPI_CreateList();
	}
	atUIAPI_AddListTail( lpWinObj->lpTimerList, tmr );
		
    return (atHWINOBJ)tmr;
}


/** 
 @brief ID를 이용해 WinObj에 내장된 타이머를 찾는다.
 
 ID를 이용해 WinObj에 내장된 타이머를 찾는다.

 @param hWinObj [in] : WinObj의 핸들
 @param id [in] : 타이머의 ID
 @return 일치하는 ID의 타이머 핸들
 @remark ID는 Unique하지 않기 때문에, 중복된 ID를 가질 경우 특정 타이머만 찾을 수 있게 된다.
*/
atHWINOBJTIMER atWINOBJ_GetTimer( atHWINOBJ hWinObj, int id )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atHWINOBJTIMER		htmr, r_htmr;
	atWINOBJTIMER *tmr;	 
	int pos;
	
	if( lpWinObj == atNULL ) return atNULL;
	if( lpWinObj->lpTimerList == atNULL ) return atNULL;

	r_htmr = atNULL;
	
	htmr = (atHWINOBJTIMER)atUIAPI_GetListHead(lpWinObj->lpTimerList, &pos);
	while ( htmr != atNULL ){
		tmr = (atWINOBJTIMER *)htmr;
		if( tmr->id == (unsigned int)id ){
			r_htmr = htmr;
			break;
		}
		
		htmr = (atHWINOBJTIMER)atUIAPI_GetListNext(lpWinObj->lpTimerList, &pos);
	}

	return r_htmr;
}


/** 
 @brief 지정된 WinObj 내장의 Tick값을 초기화한다.
 
 지정된 WinObj 내장의 Tick값을 초기화한다.

 @param hWinObj [in] : WinObj의 핸들
 @param hTimer [in] : 초기화시킬 타이머 핸들
 @return 없음
*/
void atWINOBJ_SetTimerInterval( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer, atLONG nInterval )
{
	atWINOBJTIMER *tmr = (atWINOBJTIMER *)hTimer;
	if( tmr == atNULL ) return;
	
    atUIAPI_SetTimerInterval( tmr->hTimer, nInterval );
}


/** 
 @brief 지정된 WinObj 내장의 Tick값을 초기화한다.
 
 지정된 WinObj 내장의 Tick값을 초기화한다.

 @param hWinObj [in] : WinObj의 핸들
 @param hTimer [in] : 초기화시킬 타이머 핸들
 @return 없음
*/
void atWINOBJ_EnableTimer( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer, atBOOL IsEnable )
{
	atWINOBJTIMER *tmr = (atWINOBJTIMER *)hTimer;	
	if( tmr == atNULL ) return;
	
    atUIAPI_EnableTimer( tmr->hTimer, IsEnable );
}


/** 
 @brief 지정된 WinObj 내장의 Tick값을 초기화한다.
 
 지정된 WinObj 내장의 Tick값을 초기화한다.

 @param hWinObj [in] : WinObj의 핸들
 @param hTimer [in] : 초기화시킬 타이머 핸들
 @return 없음
*/
void atWINOBJ_ResetTimer( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer )
{
	atWINOBJTIMER *tmr = (atWINOBJTIMER *)hTimer;	
	if( tmr == atNULL ) return;
	
    atUIAPI_ResetTimer( tmr->hTimer );
}


/** 
 @brief 지정된 WinObj 내장 타이머를 소멸시킨다.
 
 지정된 WinObj 내장 타이머를 소멸시킨다.

 @param hWinObj [in] : WinObj의 핸들
 @param hTimer [in] : 소멸시킬 타이머 핸들
 @return 없음
*/
void atWINOBJ_RemoveTimer( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atWINOBJTIMER *tmr = (atWINOBJTIMER *)hTimer;	
	if( tmr == atNULL ) return;
	
    atUIAPI_RemoveTimer( tmr->hTimer );

	atUIAPI_RemoveListItem( lpWinObj->lpTimerList, hTimer, atTRUE );
}


/** 
 @brief WinObj 내장 타이머를 잠시 중지시킨다.
 
 WinObj 내장 타이머를 잠시 중지시킨다.

 @param hWinObj [in] : WinObj의 핸들
 @param hTimer [in] : 타이머 핸들
 @return 없음
*/
void atWINOBJ_SuspendTimer( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer )
{
	atWINOBJTIMER *tmr = (atWINOBJTIMER *)hTimer;	
	if( tmr == atNULL ) return;
	
    atUIAPI_SuspendTimer( tmr->hTimer );
}


/** 
 @brief WinObj 내장 타이머를 Resume시킨다.
 
 WinObj 내장 타이머를 Resume시킨다.

 @param hWinObj [in] : WinObj의 핸들
 @param hTimer [in] : 타이머 핸들
 @return 없음
*/
void atWINOBJ_ResumeTimer( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer )
{
	atWINOBJTIMER *tmr = (atWINOBJTIMER *)hTimer;	
	if( tmr == atNULL ) return;
	
    atUIAPI_ResumeTimer( tmr->hTimer );
}


/** 
 @brief WinObj에 내장된 모든 타이머를 Enable/Disable시킨다.
 
 WinObj에 내장된 모든 타이머를 Enable/Disable시킨다.

 @param hWinObj [in] : WinObj의 핸들
 @param IsEnable [in] : Enable/Disable
 @return 없음
*/
void atWINOBJ_EnableAllTimers(atHWINOBJ hWinObj, atBOOL IsEnable)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atHWINOBJTIMER		tmr;
	int pos;

	if( lpWinObj == atNULL ) return;
	if( lpWinObj->lpTimerList == atNULL ) return;

	tmr = (atHWINOBJTIMER)atUIAPI_GetListHead(lpWinObj->lpTimerList, &pos);
	while ( tmr != atNULL ){
		atWINOBJ_EnableTimer( hWinObj, tmr, IsEnable );
		tmr = (atHWINOBJTIMER)atUIAPI_GetListNext(lpWinObj->lpTimerList, &pos);
	}
}


/** 
 @brief WinObj에 내장된 모든 타이머를 소멸시킨다.
 
 WinObj에 내장된 모든 타이머를 소멸시킨다.

 @param hWinObj [in] : WinObj의 핸들
 @return 없음
*/
void atWINOBJ_RemoveAllTimers(atHWINOBJ hWinObj)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atHWINOBJTIMER		tmr;

	if( lpWinObj == atNULL ) return;
	if( lpWinObj->lpTimerList == atNULL ) return;

	tmr = (atHWINOBJTIMER)atUIAPI_GetListTail(lpWinObj->lpTimerList, atNULL);

	while ( tmr != atNULL )	{
		atWINOBJ_RemoveTimer( hWinObj, tmr );
		tmr = (atHWINOBJTIMER)atUIAPI_GetListTail(lpWinObj->lpTimerList, atNULL);
	}

	atUIAPI_DestroyList(lpWinObj->lpTimerList, atFALSE);
	lpWinObj->lpTimerList = atNULL;
}


/** 
 @brief WinObj 내장 타이머를 모두 잠시 정지시킨다.
 
 WinObj 내장 타이머를 모두 잠시 정지시킨다.

 @param hWinObj [in] : WinObj의 핸들
 @param bFocusedSuspend [in] : 포커스 타이머만 정지시킬 것인지 여부
 @return 없음
*/
void atWINOBJ_SuspendAllTimers(atHWINOBJ hWinObj, atBOOL bFocusedSuspend )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atHWINOBJTIMER		hWinTimer;
	atWINOBJTIMER 		*tmr;	
	int pos;

	if( lpWinObj == atNULL ) return;
	if( lpWinObj->lpTimerList == atNULL ) return;

	hWinTimer = (atHWINOBJTIMER)atUIAPI_GetListHead(lpWinObj->lpTimerList, &pos);
	while ( hWinTimer != atNULL ){
		if( bFocusedSuspend ){
			tmr = (atWINOBJTIMER *)hWinTimer;
			if( tmr->bFocus ) atWINOBJ_SuspendTimer( hWinObj, hWinTimer );
		} else {
			atWINOBJ_SuspendTimer( hWinObj, hWinTimer );
		}
		hWinTimer = (atHWINOBJTIMER)atUIAPI_GetListNext(lpWinObj->lpTimerList, &pos);
	}
}


/** 
 @brief 잠시 정지된 WinObj 내장 타이머를 모두 깨운다
 
 잠시 정지된 타이머를 모두 깨운다\n
 Pause 된 타이머로 한정되며, Pause 당시 Enable이었던 타이머로 또한 제한된다.

 @param hWinObj [in] : WinObj의 핸들
 @return 없음
*/
void atWINOBJ_ResumeAllTimers(atHWINOBJ hWinObj)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atHWINOBJTIMER		hWinTimer;
	int pos;

	if( lpWinObj == atNULL ) return;
	if( lpWinObj->lpTimerList == atNULL ) return;

	hWinTimer = (atHWINOBJTIMER)atUIAPI_GetListHead(lpWinObj->lpTimerList, &pos);
	while ( hWinTimer != atNULL ){
		atWINOBJ_ResumeTimer( hWinObj, hWinTimer );
		hWinTimer = (atHWINOBJTIMER)atUIAPI_GetListNext(lpWinObj->lpTimerList, &pos);
	}
}


/** 
 @brief WinObj 내부의 해당 위치에 Event 대상이 될 Child 개체가 있는가
 
 WinObj 내부의 해당 위치에 Event 타겟이 될 Child 개체가 있는가

 @param hWinObj [in] : WinObj의 핸들
 @param nX, nY [in] : 위치
 @return Event 대상 개체가 있으면 atTRUE, 없으면 atFALSE
*/
atBOOL	atWINOBJ_IsEmbededEventTarget( atHWINOBJ hWinObj, int nX, int nY )
{
	atBOOL ret = atTRUE, ret2 = atFALSE;
	atHWINOBJ 	hChildObj;
	atREGION rgn;
	atLPWINOBJ	lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atFALSE;

	if( !lpWinObj->bShow ) return atFALSE;
	if( !lpWinObj->bEnable ) return atFALSE;
	if( !(lpWinObj->dwAttr & atOBJATTR_EVENTTARGET) ) ret = atFALSE;
	
	nX -= lpWinObj->rtLayoutMargin.nLeft;
	nY -= lpWinObj->rtLayoutMargin.nTop;
	
	hChildObj = atWINOBJMNG_FindWinObjAt(lpWinObj->hChildObjMng, nX, nY);
	if( hChildObj ){
		atWINOBJ_GetRegion( hChildObj, &rgn );
		nX -= rgn.nX; nY -= rgn.nY;
		ret2 = atWINOBJ_IsEmbededEventTarget( hChildObj, nX, nY );
	}

	return (ret2)? ret2 : ret;
}

	
/** 
 @brief WinObj의 Default 메시지 핸들러
 
 WinObj의 Default 메시지 핸들러

 @param hWinObj [in] : WinObj의 핸들
 @param nMsg [in] : 전송할 메시지
 @param lParam1, lParam2 [in] : 메시지의 파라미터
 @return 메시지 핸들러의 결과값
 @remark WinObj를 상속받은 개체는 반드시 이 핸들러를 포함시켜 줘야 한다.
*/
int atWINOBJ_DefObjProc( atHWINOBJ hWinObj, int nMsg, long lParam1, long lParam2 )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	atHWINOBJ hParentObj, hChildObj, hTargetObj;
	int x, y;
	atRECT rect;
	atREGION rgn;
	int ret = 0;
	
	switch( nMsg )
	{
		case OBJMSG_INIT:
			break;
		case OBJMSG_RELEASE:
			break;
		case OBJMSG_REGIST:
			if( lParam1 != 0 ){
				*(atLONG *)lParam1 = atTRUE;
				atWINOBJ_RealizeSoftKey( hWinObj );
			}
			break;

		case OBJMSG_REQ_DESTROY:
			atWINOBJ_ProcessObjProc(hWinObj, OBJMSG_PREDESTROY, 0, 0);
			atWINOBJ_ProcessObjProc(hWinObj, OBJMSG_DESTROY, 0, 0);
			break;
		case OBJMSG_PREDESTROY:
			atWINOBJ_RemoveAllChildWinObjs(hWinObj); // 일단 먼저 소멸시켜 놓는다. 좀 더 보강할 예정.
			atWINOBJ_ProcessObjProc(hWinObj, OBJMSG_RELEASE, 0, 0);
			break;
		case OBJMSG_DESTROY:
			atWINOBJ_Destroy( hWinObj );
			break;
			
		case OBJMSG_SHOW:
			atWINOBJ_ProcessObjProc(hWinObj, OBJMSG_CHANGELAYOUT, 0, 0);
			atWINOBJ_Refresh(hWinObj);
			break;
		case OBJMSG_HIDE:
			atWINOBJ_ProcessObjProc(hWinObj, OBJMSG_CHANGELAYOUT, 0, 0);
			atWINOBJ_Refresh(hWinObj);
			break;

		case OBJMSG_CHANGELAYOUT:
			atWINOBJ_PostMessageAllChildWinObjs( hWinObj, OBJMSG_CHANGELAYOUT, lParam1, lParam2 );
			break;
			
		case OBJMSG_GOTFOCUS:
			atWINOBJ_PostMessageAllChildWinObjs( hWinObj, OBJMSG_RESUME, 0, 0 );
			hChildObj = atWINOBJ_GetCurChildWinObj(hWinObj);
			if( hChildObj != atNULL ) atWINOBJ_PostMessage( hChildObj, OBJMSG_GOTFOCUS, lParam1, lParam2 );
			atWINOBJ_ResumeAllTimers( hWinObj );
			atWINOBJ_Refresh(hWinObj);
			atWINOBJ_RealizeSoftKey( hWinObj );
			atSOFTKEY_RefreshSoftKey();
			//atSOFTKEY_DrawSoftKey( atTRUE );
			break;
		case OBJMSG_LOSTFOCUS:
			atWINOBJ_SuspendAllTimers( hWinObj, atTRUE );
			hChildObj = atWINOBJ_GetCurChildWinObj(hWinObj);
			if( hChildObj != atNULL ) atWINOBJ_PostMessage( hChildObj, OBJMSG_LOSTFOCUS, lParam1, lParam2 );
			atWINOBJ_PostMessageAllChildWinObjs( hWinObj, OBJMSG_PAUSE, 0, 0 );
			atWINOBJ_Refresh(hWinObj);
			break;

		case OBJMSG_KEYDOWN:
/* 현재, 키보드 위주가 아닌 터치 위주의 인터페이스로 대대적으로 수정된 탓에, 키보드 쪽 처리와 안 맞는 부분이 생겼다. 때문에 임시로 충돌되는 키처리는 모두 막아 놓는다.
			hChildObj = atWINOBJ_GetCurChildWinObj(hWinObj);
			if( hChildObj && atWINOBJ_IsEnable(hChildObj) ){
				g_hCaptureKeyObj = hChildObj;
				ret = atWINOBJ_PostMessage( hChildObj, OBJMSG_KEYDOWN, lParam1, 0 );
				if( ret != 0 ) return ret;
			}  else {
				switch( lParam1 ){
					case atVKEY_LEFT:
					case atVKEY_UP:	
						atWINOBJ_PostMessageToParent( hWinObj, OBJMSG_REQ_CHANGECHILDOBJFOCUS, (long)hWinObj, 0 );
						break;
					case atVKEY_RIGHT:
					case atVKEY_DOWN:
						atWINOBJ_PostMessageToParent( hWinObj, OBJMSG_REQ_CHANGECHILDOBJFOCUS, (long)hWinObj, 1 );
						break;
					default:
						g_hCaptureKeyObj = hWinObj;
						break;
				}
			}
*/
			break;
		case OBJMSG_KEYPRESS:
			break;
		case OBJMSG_KEYUP:
			break;

		case OBJMSG_PREMOUSEDOWN:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			lpWinObj->bMouseDown = atTRUE;

			lpWinObj->hMouseActWinObj = atNULL;
			x -= lpWinObj->rtLayoutMargin.nLeft;
			y -= lpWinObj->rtLayoutMargin.nTop;
			hChildObj = atWINOBJMNG_FindWinObjAt(lpWinObj->hChildObjMng, x, y);
			if( hChildObj ){
				int x2, y2;
				atWINOBJ_GetRegion( hChildObj, &rgn );
				x2 = x - rgn.nX;
				y2 = y - rgn.nY;
				if( atWINOBJ_IsEmbededEventTarget(hChildObj, x2, y2) /*hChildObj && atWINOBJ_IsEnable(hChildObj)*/ ){
					if( atWINOBJ_IsFocusable(hChildObj) && (hChildObj != atWINOBJ_GetCurChildWinObj(hWinObj)) ){
						atWINOBJ_SetCurChildWinObj( hWinObj, hChildObj );
//						atWINOBJ_Refresh( hWinObj );
					}
					lpWinObj->hMouseActWinObj = hChildObj;
					lpWinObj->nMouseDownPosX = x2;
					lpWinObj->nMouseDownPosY = y2;
					atWINOBJ_GetRect( hChildObj, &rect );
//					ret = atWINOBJ_SendMessage( hChildObj, OBJMSG_PREMOUSEDOWN, lParam1, ((x-rect.nStartX) << 16) | (y-rect.nStartY) );
					ret = atWINOBJ_SendMessage( hChildObj, OBJMSG_PREMOUSEDOWN, lParam1, (x2 << 16) | (y2 & 0xffff) );
				}
			} 
			if( lpWinObj->hMouseActWinObj == atNULL ){
				ret = atWINOBJ_SendMessage( hWinObj, OBJMSG_MOUSEDOWN, lParam1, (x << 16) | (y & 0xffff) );
			}
			break;
		case OBJMSG_MOUSEDOWN:
			break;
		case OBJMSG_MOUSEMOVE:
			if( lpWinObj->bMouseDown ){
				x = (short)(lParam2 >> 16);
				y = (short)(lParam2 & 0xffff);
				hTargetObj = lpWinObj->hMouseActWinObj;
				if( (hTargetObj != atNULL) && (hTargetObj != hWinObj) ){
					x -= lpWinObj->rtLayoutMargin.nLeft;
					y -= lpWinObj->rtLayoutMargin.nTop;
//					x -= lpWinObj->nMouseDownPosX;
//					y -= lpWinObj->nMouseDownPosY;
					atWINOBJ_GetRect( hTargetObj, &rect );
					ret = atWINOBJ_SendMessage( hTargetObj, OBJMSG_MOUSEMOVE, lParam1, ((x-rect.nStartX) << 16) | ((y-rect.nStartY) & 0xffff) );
				} else {
				}
			} else {
				// 터치 인터페이스 상에서 불필요하기에, 
				// 퍼포먼스를 위해... 마우스 클릭하지 않은 것들은 Move에서 제외.
			}
			break;
		case OBJMSG_MOUSEUP:
			if( lpWinObj->bMouseDown ){
				x = (short)(lParam2 >> 16);
				y = (short)(lParam2 & 0xffff);
				hTargetObj = lpWinObj->hMouseActWinObj;
//				if( (hTargetObj != atNULL) && (hTargetObj != hWinObj) ){
				if( hTargetObj != atNULL ){
					x -= lpWinObj->rtLayoutMargin.nLeft;
					y -= lpWinObj->rtLayoutMargin.nTop;
					atWINOBJ_GetRect( hTargetObj, &rect );
					ret = atWINOBJ_SendMessage( hTargetObj, OBJMSG_MOUSEUP, lParam1, ((x-rect.nStartX) << 16) | ((y-rect.nStartY) & 0xffff) );
				} else {
					atUIAPI_MoveRect( &rect, &lpWinObj->rect, -lpWinObj->rect.nStartX, -lpWinObj->rect.nStartY );
					if( atUIAPI_IsPtInRect( x, y, &rect ) ){
						ret = atWINOBJ_SendMessage( hWinObj, OBJMSG_MOUSECLICK, lParam1, atWINOBJ_GetID(hTargetObj) );
					}
				}

				lpWinObj->bMouseDown = atFALSE;
			}
			lpWinObj->hMouseActWinObj = atNULL;
			break;
		case OBJMSG_PREMOUSELONGCLICK:
			x = (short)(lParam2 >> 16);
			y = (short)(lParam2 & 0xffff);
			x -= lpWinObj->rtLayoutMargin.nLeft;
			y -= lpWinObj->rtLayoutMargin.nTop;
			hTargetObj = lpWinObj->hMouseActWinObj;
			if( hTargetObj && atWINOBJ_IsEnable(hTargetObj) ){
				atWINOBJ_GetRect( hTargetObj, &rect );
				ret = atWINOBJ_SendMessage( hTargetObj, OBJMSG_PREMOUSELONGCLICK, lParam1, ((x-rect.nStartX) << 16) | ((y-rect.nStartY) & 0xffff) );
			} else {
				ret = atWINOBJ_SendMessage( hWinObj, OBJMSG_MOUSELONGCLICK, lParam1, (x << 16) | (y & 0xffff) );
			}
			break;
		case OBJMSG_MOUSELONGCLICK:
			break;
		case OBJMSG_MOUSECLICK:
			break;

		case OBJMSG_PREPAINT:
			break;
		case OBJMSG_PAINTLAYOUT:
			break;
		case OBJMSG_PAINT:
			break;
		case OBJMSG_PAINTOBJS:
			atWINOBJ_DrawAllChildWinObjs( hWinObj, (atBOOL)lParam2 );
			break;
		case OBJMSG_POSTPAINT:
			break;

//		case OBJMSG_CHANGEPOS:
//			atWINOBJ_PostMessageAllChildWinObjs( hWinObj, OBJMSG_CHANGELAYOUT, 0, 0 );
//			break;

		case OBJMSG_PRERESIZE:
			atWINOBJ_AdjustAllChildWinObjs( hWinObj );
			break;

			break;
		case OBJMSG_RESIZE:
			break;

		case OBJMSG_TIMER:
			break;

		case OBJMSG_COMMAND:
		case OBJMSG_COMMAND_EX:
		case OBJMSG_CHANGECHILDOBJFOCUS:
			atWINOBJ_PostMessageToParent( hWinObj, nMsg, lParam1, lParam2 );
			break;
			
		case OBJMSG_REQ_CHANGECHILDOBJFOCUS:
			switch( lParam2 ){
				case 0:
					hChildObj = atWINOBJMNG_GetPrevEnabledWinObj( lpWinObj->hChildObjMng, (atHWINOBJ)lParam1 );
					break;
				case 1:
					hChildObj = atWINOBJMNG_GetNextEnabledWinObj( lpWinObj->hChildObjMng, (atHWINOBJ)lParam1 );
					break;
				default :
					return 0;
			}
			if( (hChildObj != atNULL) && (hChildObj != (atHWINOBJ)lParam1) ){
				atWINOBJ_SetCurChildWinObj( hWinObj, hChildObj );
				atWINOBJ_Refresh((atHWINOBJ)lParam1);
				atWINOBJ_Refresh(hChildObj);
				g_hCaptureKeyObj = hChildObj;
				return 1;
			} else {
				atWINOBJ_PostMessageToParent( hChildObj, OBJMSG_REQ_CHANGECHILDOBJFOCUS, (long)hWinObj, lParam2 );
			}
			break;
			
		case OBJMSG_CHANGE:
			break;

		default:
			hChildObj = atWINOBJ_GetCurChildWinObj(hWinObj);
			if( hChildObj ){
				ret = atWINOBJ_SendMessage( hChildObj, nMsg, lParam1, lParam2 );
			}
			break;
	}

	return ret;
}

