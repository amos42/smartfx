/**
  @file WinObj.c
  @brief WinObj�� ���� API
  @remark �ַ� Window, Control ������ ��� �޾� ���ο����� ����Ѵ�.
  @author Amos
  @date 2010-04-19 ���� 11:41:00
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


// WinObj ������ Ǯ���� ��� �ִ� ���� ã�´�.
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

/** @brief WinObj�� Ÿ�̸� Ÿ��. (���ο�) */
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
 @brief WinObj �ڵ��� �����Ѵ�.
 
 WinObj �����ͷκ��� �ڵ��� �����Ѵ�.

 @param lpWinObj [in] : WinObj ��ü�� ������
 @return WinObj �ڵ�
 @remark �ڵ� �� ������ atWINOBJ_MAX_WINOBJ_CNT ������ ���� �� ����.
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
 @brief WinObj �ڵ��� �Ҹ��Ų��.
 
 WinObj �ڵ��� �Ҹ��Ų��.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ����
*/
void atWINOBJ_ReleaseHandle( atHWINOBJ hWinObj )
{
	if( hWinObj == atNULL ) return;
	if( g_WinObj_Count <= 0 ) return;

	*((atLPWINOBJ *)hWinObj) = atNULL;
	g_WinObj_Count --;
}


/** 
 @brief WinObj ��ü�� �����͸� ��´�.
 
 WinObj ��ü�� �����͸� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return WinObj ��ü�� ������
*/
atLPWINOBJ atWINOBJ_GetPtr(atHWINOBJ hWinObj)
{
	if( hWinObj == atNULL ) return atNULL;
	if( ((atLPWINOBJ *)hWinObj < &g_lpWinObj_PoolList[0]) || ((atLPWINOBJ *)hWinObj > &g_lpWinObj_PoolList[atWINOBJ_MAX_WINOBJ_CNT-1]) ) return atNULL;
	return *((atLPWINOBJ *)hWinObj);
}


/** 
 @brief WinObj�� ���� Data �����͸� ��´�.
 
 WinObj�� ���� Data �����͸� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return WinObj�� ���� Data ������
*/
void *atWINOBJ_GetDataPtr(atHWINOBJ hWinObj)
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj ) return lpWinObj->lpObjData;
	else return atNULL;
}


/** 
 @brief WinObj�� ID�� ��´�.
 
 WinObj�� ID�� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return WinObj�� ID
*/
atINT atWINOBJ_GetID( atHWINOBJ hWinObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj ) return lpWinObj->nID;
	else return -1;
}


/** 
 @brief WinObj�� �ν��Ͻ��� �����Ѵ�.
 
 WinObj�� �ν��Ͻ��� �����Ѵ�.

 @param ExtObjSize [in] : WinObj�� �߰����� Data ũ��
 @return WinObj�� �ڵ�
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
 @brief WinObj�� �ʱ�ȭ��Ų��.
 
 WinObj�� �ʱ�ȭ��Ų��.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param hOwnerObj [in] : �θ� WinObj�� �ڵ�
 @param dwClassID [in] : Ŭ������ ID
 @param nID [in] : WinObj�� ID
 @param szWinObjName [in] : ������Ʈ �̸�.
 @param dwAttr [in] : �Ӽ�. default�� atOBJATTR_NORMAL
 @param rtLayoutMargin [in] : �ܰ����� ����. �� ������ �����Ͽ� Ŭ���̾�Ʈ�� ũ�Ⱑ �����ȴ�.
 @param dwUserDataSize [in] : ����� �߰� ������ ũ��
 @param fnDefObjProc [in] : Default �̺�Ʈ �ڵ鷯.
 @param fnObjProc [in] : ����� �̺�Ʈ �ڵ鷯.
 @param lpCreateParam [in] : ������ ������ �Ķ����
 @param lpExData [in] : ������ ������ �߰� ������. �ַ� WinClass���� ����.
 @return �����ϸ� atTRUE, �����ϸ� atFALSE
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
	lpWinObj->lpTimerList = atNULL;  // Ÿ�̸Ӹ� �� ���� ������� �޸� ������ ����

	lpWinObj->bMouseDown = atFALSE;
	lpWinObj->hMouseActWinObj = atNULL;
	lpWinObj->nMouseDownPosX = lpWinObj->nMouseDownPosY = 0;
	
	lpWinObj->lpCreateParam = lpCreateParam;
	lpWinObj->lpExLayoutData = lpExData;
	
	return atTRUE;
}


/** 
 @brief WinObj�� ���� �ʱ�ȭ��Ų��.
 
 WinObj�� ���� �ʱ�ȭ��Ų��.

 @param hOwnerObj [in] : �θ� WinObj�� �ڵ�
 @param dwClassID [in] : Ŭ������ ID
 @param nID [in] : WinObj�� ID
 @param szWinObjName [in] : ������Ʈ �̸�.
 @param dwAttr [in] : �Ӽ�. default�� atOBJATTR_NORMAL
 @param rtLayoutMargin [in] : �ܰ����� ����. �� ������ �����Ͽ� Ŭ���̾�Ʈ�� ũ�Ⱑ �����ȴ�.
 @param dwUserDataSize [in] : ����� �߰� ������ ũ��
 @param fnDefObjProc [in] : Default �̺�Ʈ �ڵ鷯.
 @param fnObjProc [in] : ����� �̺�Ʈ �ڵ鷯.
 @param lpCreateParam [in] : ������ ������ �Ķ����
 @param lpExData [in] : ������ ������ �߰� ������. �ַ� WinClass���� ����.
 @param ExtObjSize [in] : ����� �߰� ������. ���������� ����� ������ ��� ����Ѵ�.
 @return WinObj �ڵ�
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
 @brief WinObj�� ������ �Ϸ��Ѵ�.
 
 WinObj�� ������ �Ϸ��Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param nWidth, nHeight [in] : ������ �������� ũ�� 
 @param rtLayoutMargin [in] : �ܰ����� ����. �� ������ �����Ͽ� Ŭ���̾�Ʈ�� ũ�Ⱑ �����ȴ�.
 @param rtAnchor [in] : Anchor�� �����Ѵ�.
 @return �����ϸ� atTRUE, �����ϸ� atFALSE
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
 @brief WinObj�� �����Ѵ�.
 
 WinObj�� �����Ѵ�.

 @param hOwnerObj [in] : �θ� WinObj�� �ڵ�
 @param dwClassID [in] : Ŭ������ ID
 @param nID [in] : WinObj�� ID
 @param szWinObjName [in] : ������Ʈ �̸�.
 @param dwAttr [in] : �Ӽ�. default�� atOBJATTR_NORMAL
 @param nWidth, nHeight [in] : ������ �������� ũ�� 
 @param rtLayoutMargin [in] : �ܰ����� ����. �� ������ �����Ͽ� Ŭ���̾�Ʈ�� ũ�Ⱑ �����ȴ�.
 @param rtAnchor [in] : Anchor�� �����Ѵ�.
 @param dwUserDataSize [in] : ����� �߰� ������ ũ��
 @param fnDefObjProc [in] : Default �̺�Ʈ �ڵ鷯.
 @param fnObjProc [in] : ����� �̺�Ʈ �ڵ鷯.
 @param lpCreateParam [in] : ������ ������ �Ķ����
 @param lpExData [in] : ������ ������ �߰� ������. �ַ� WinClass���� ����.
 @param ExtObjSize [in] : ����� �߰� ������. ���������� ����� ������ ��� ����Ѵ�. 
 @return WinObj �ڵ�
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
 @brief WinObj�� �Ҹ��Ų��.
 
 WinObj�� �Ҹ��Ų��.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ����
 @remark ���� ȣ���\n
 		 ����ڴ� �� �Լ��� ���� ȣ���ϱ� ���ٴ�, OBJMSG_REQ_DESTROY�� ������ �Ҹ��Ű�� ���� ����.
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
 @brief WinObj�� ��ȿ������ üũ.
 
 WinObj�� ��ȿ������ üũ.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ��ȿ�ϸ� atTRUE, ��ȿ�ϸ� atFALSE
*/
atBOOL	atWINOBJ_IsValidate( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atFALSE;
		
	return atTRUE;
}


/** 
 @brief WinObj�� Ȱ��ȭ��Ų��.
 
 WinObj�� Ȱ��ȭ��Ų��.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ����
*/
void	 atWINOBJ_Activate( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return;

	lpWinObj->bActivate = atTRUE;
}


/** 
 @brief WinObj�� ��Ȱ��ȭ��Ų��.
 
 WinObj�� ��Ȱ��ȭ��Ų��.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ����
*/
void	 atWINOBJ_Deactivate( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return;

	lpWinObj->bActivate = atFALSE;
}


/** 
 @brief WinObj�� Ȱ��ȭ �������� üũ�Ѵ�.
 
 WinObj�� Ȱ��ȭ �������� üũ�Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return Ȱ�� ���¸� atTRUE, ��Ȱ�� ���¸� atFALSE
*/
atBOOL	atWINOBJ_IsActivate( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atFALSE;

	return lpWinObj->bActivate;
}


/** 
 @brief WinObj�� �̸��� ��´�.
 
 WinObj�� �̸��� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return WinObj �̸��� String ������
*/
atLPTSTR atWINOBJ_GetName(atHWINDOW hWinObj)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	
	if( lpWinObj == atNULL ) return _AT("");
	
	return lpWinObj->szObjName;
}


/** 
 @brief WinObj�� Ŭ���� �̸��� ��´�.
 
 WinObj�� �̸��� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return WinObj Ŭ���� �̸��� String ������
*/
atLPTSTR atWINOBJ_GetClassName(atHWINDOW hWinObj)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	
	if( lpWinObj == atNULL ) return _AT("");
	
	return lpWinObj->szClassName;
}


/** 
 @brief WinObj�� Attribute�� �����Ѵ�.
 
 WinObj�� Attribute�� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ���� ������ Attribute
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
 @brief WinObj�� Attribute�� ��´�.
 
 WinObj�� Attribute�� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ������ Attribute
*/
atDWORD atWINOBJ_GetAttr( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return 0;

	return lpWinObj->dwAttr;
}


/** 
 @brief WinObj�� Attribute�� �ٲ۴�.
 
 WinObj�� Attribute�� �ٲ۴�.\n
 �߰��� �Ӽ��� ������ �Ӽ��� ���� ���� ������ �� �ִ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param dwAddAttr [in] : �߰��� �Ӽ�
 @param dwRemoveAttr [in] : ������ �Ӽ�
 @return ���� ������ Attribute
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
 @brief WinObj�� �׸���.
 
 WinObj�� �׸���.\n
 OBJMSG_PREPAINT, OBJMSG_PAINTLAYOUT, OBJMSG_PAINT, OBJMSG_PAINTOBJS, OBJMSG_POSTPAINT\n
 �̺�Ʈ�� ���ʷ� �߻��Ѵ�.\n
 ���� SoftKey���� �׸��� �ȴ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param bFocused [in] : Focused
 @return ����
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
 @brief WinObj�� �Ϻθ� �׸���.
 
 WinObj�� �Ϻθ� �׸���.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param nX, nY [in] : �׸� WinObj�� ���� ��ġ
 @param nWidth, nHeight [in] : �׸� WinObj�� ����
 @param bFlush [in] : ȭ�鿡 Flush�� ���ΰ� ����
 @return ����
 @remark ���� �����Ǿ� ���� ����. atWINOBJ_Draw�� �����ϰ� ����.
*/
void atWINOBJ_DrawRegion( atHWINOBJ hWinObj, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atBOOL bFocused, atBOOL bFlush )
{
	atWINOBJ_Draw( hWinObj, bFocused ); // ������ �ӽ÷�...
}


/** 
 @brief WinObj�� ȭ�鿡 �����Ѵ�.
 
 WinObj�� ȭ�� ���� ���μ����� ��Ͽ� �߰��Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ����
 @remark ȭ�� ������ �񵿱������� �̷������ ������, ������ ȭ�鿡 ���ŵǴ� Ÿ�̹��� ��Ȯ�� ������ �����.
*/
void atWINOBJ_Refresh( atHWINOBJ hWinObj )
{
	atREGION area;
	atWINOBJ_GetRegion( hWinObj, &area );
	area.nX = area.nY = 0;
	if( !atWINOBJ_ConvScreenValidateRegion( hWinObj, &area, &area ) ) return;
	atUIAPI_AddInvalideArea( &area );

	// �̰� �־�� �� �� ���� �� �� ���... �̰� ������ �Ź� ȭ���� ��ü �����Ѵ�. ������ ���� ���� ���� �� ������...	
	atANNBAR_RefreshAnnBar();
	atSOFTKEY_RefreshSoftKey();
}


/** 
 @brief WinObj�� Ŭ���̾�Ʈ ������ ȭ�鿡 �����Ѵ�.
 
 WinObj�� Ŭ���̾�Ʈ ������ ȭ�� ���� ���μ����� ��Ͽ� �߰��Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ����
 @remark ȭ�� ������ �񵿱������� �̷������ ������, ������ ȭ�鿡 ���ŵǴ� Ÿ�̹��� ��Ȯ�� ������ �����.
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
 @brief ������ ������ Parameter�� ��´�.
 
 ������ ������ Parameter�� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return Parameter�� ������
*/
atLPVOID atWINOBJ_GetCreateParam(atHWINOBJ hWinObj)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atNULL;

	return lpWinObj->lpCreateParam;
}


/** 
 @brief WinObj�� ClassID�� ��´�.
 
 WinObj�� ClassID�� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ClassID
*/
atDWORD atWINOBJ_GetClassID( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return 0;

	return lpWinObj->dwClassID;
}


/** 
 @brief WinObj�� Parent ��ü�� �ڵ��� ��´�.
 
 WinObj�� Parent ��ü�� �ڵ��� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return Parent��ü�� �ڵ�
*/
atHWINOBJ atWINOBJ_GetParentObj(atHWINOBJ hWinObj)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atNULL;

	return lpWinObj->hParentObj;
}


/** 
 @brief WinObj�� Parent ��ü�� �����Ѵ�.
 
 WinObj�� Parent ��ü�� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param hParentObj [in] : Parent�� �ڵ�
 @return ���� Parent��ü�� �ڵ�
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
 @brief WinObj�� Owner ��ü�� �ڵ��� ��´�.
 
 WinObj�� Owner ��ü�� �ڵ��� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return Owner ��ü�� �ڵ�
*/
atHWINOBJ atWINOBJ_GetOwnerObj(atHWINOBJ hWinObj)
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atNULL;

	return lpWinObj->hOwnerObj;
}


/** 
 @brief WinObj�� Owner ��ü�� �����Ѵ�.
 
 WinObj�� Owner ��ü�� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param hParentObj [in] : Owner �ڵ�
 @return ���� Owner �ڵ�
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
 @brief WinObj�� �޽����� ������.
 
 WinObj�� �޽����� ������.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param nMsg [in] : ������ �޽���
 @param lParam1, lParam2 [in] : �޽����� �Ķ����
 @return �޽��� �ڵ鷯�� �����
 @remark �޽��� ó���� �ڵ鷯�� �������� Call�� ���� �ﰢ������ �Ͼ��.
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
 @brief WinObj�� �޽����� ������.
 
 WinObj�� �޽����� ������.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param nMsg [in] : ������ �޽���
 @param lParam1, lParam2 [in] : �޽����� �Ķ����
 @return �޽��� ó�� ��û ���.
 @remark �޽��� ó���� �޽��� Queue�� ���� �Ͼ�� ������, �񵿱������� �Ͼ��. \n
 		������, ������ WinObj�� ���, ���� Queue�� �������� �ʴ´�.\n
 		������ �̸� ��� ���� Window�� ��쿣, ������ Queue�� ����Ͽ� �̸� �����ؾ� �Ѵ�.
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
 @brief WinObj�� Parent ��ü���� �޽����� ������.
 
 WinObj�� Parent ��ü���� �޽����� ������.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param nMsg [in] : ������ �޽���
 @param lParam1, lParam2 [in] : �޽����� �Ķ����
 @return �޽��� ó�� ��û ���.
 @remark �޽��� ó���� �޽��� Queue�� ���� �Ͼ�� ������, �񵿱������� �Ͼ��.
*/
atINT	atWINOBJ_PostMessageToParent( atHWINOBJ hWinObj, atINT nMsg, atLONG lParam1, atLONG lParam2 )
{
	atHWINOBJ hParentObj = atWINOBJ_GetParentObj(hWinObj);
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr( hParentObj );
	if( lpWinObj == atNULL ) return -1;
	
	return atWINOBJ_PostMessage( hParentObj, nMsg, lParam1, lParam2 );
}


/** 
 @brief WinObj�� ��ġ�� �����Ѵ�.
 
 WinObj�� ��ġ�� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param nX, nY [in] : �̵��� ��ġ
 @parem bForceChange [in] : Anchor�� ������ ���θ� ����.
 @return ����
 @remark OBJMSG_CHANGEPOS �̺�Ʈ�� �߻��Ѵ�.
*/
void atWINOBJ_SetPosition( atHWINOBJ hWinObj, atINT nX, atINT nY, atBOOL bForceChange )
{
	int w, h;
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if ( lpWinObj == atNULL ) return;

	if( (lpWinObj->rect.nStartX == nX) && (lpWinObj->rect.nStartY == nY) ) return;

	// SetPosition �ϸ� Anchor�� �ϴ� �ٲ��.
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
 @brief ������ WinObj ��ġ�� ���Ѵ�.
 
 ������ WinObj ��ġ�� ���Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param pnX, pnY [out] : WinObj�� ��ġ��
 @return ����
*/
void atWINOBJ_GetPosition( atHWINOBJ hWinObj, atINT *pnX, atINT *pnY )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if ( lpWinObj == atNULL ) return;

	*pnX = lpWinObj->rect.nStartX;
	*pnY = lpWinObj->rect.nStartY;
}


/** 
 @brief WinObj�� ������ �����Ѵ�.
 
 WinObj�� ������ �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param nStartX, nStartY [in] : ������ ����
 @param nEndX, nEndY [in] : ������ ��
 @parem bForceChange [in] : Anchor�� ������ ���θ� ����.
 @return ����
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
 @brief WinObj�� ������ �����Ѵ�.
 
 WinObj�� ������ �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param nX, nY [in] : ������ ����
 @param nWidth, nHeight [in] : ������ ũ��
 @parem bForceChange [in] : Anchor�� ������ ���θ� ����.
 @return ����
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
 @brief WinObj�� ������ ��´�.
 
 WinObj�� ������ ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param pRect [out] : ���� WinObj�� ����
 @return ����
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
 @brief WinObj�� ������ ��´�.
 
 WinObj�� ������ ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param pRegion [out] : ���� WinObj�� ����
 @return ����
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
 @brief ���̾ƿ��� ������ ��´�.
 
 ���̾ƿ��� ������ ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpMargin [out] : ���̾ƿ��� ����
 @return ����
 @remark ���̾ƿ��� ������ ũ��� Ŭ���̾�Ʈ ������ �������� ������ �ش�.
*/
void atWINOBJ_GetLayoutMargin( atHWINOBJ hWinObj, atMARGIN * lpMargin )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if ( lpWinObj == atNULL ) return;
	
	if( lpMargin ) *lpMargin = lpWinObj->rtLayoutMargin;
}


/** 
 @brief ���̾ƿ��� ������ �����Ѵ�.
 
 ���̾ƿ��� ������ �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param nTop [in] : ���̾ƿ��� ���� ����
 @param nBottom [in] : ���̾ƿ��� �Ʒ��� ����
 @param nLeft [in] : ���̾ƿ��� ���� ����
 @param nRight [in] : ���̾ƿ��� ������ ����
 @return ����
 @remark ���̾ƿ��� ������ ũ��� Ŭ���̾�Ʈ ������ �������� ������ �ش�.
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
 @brief ���̾ƿ��� ������ �����Ѵ�.
 
 ���̾ƿ��� ������ �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpMargin [in] : ���̾ƿ��� ����
 @return ����
 @remark ���̾ƿ��� ������ ũ��� Ŭ���̾�Ʈ ������ �������� ������ �ش�.
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
 @brief Ŭ���̾�Ʈ�� ������ ��´�.
 
 Ŭ���̾�Ʈ�� ������ ��´�.\n
 ������ ����ڰ� �����ΰ��� �׸� ��, Ŭ���̾�Ʈ ������ �ַ� ������� �Ѵ�.\n
 Ŭ���̾�Ʈ ������ ������ �׻� 0, 0�̴�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param pRect [out] : Ŭ���̾�Ʈ ����
 @return ����
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
 @brief Ŭ���̾�Ʈ�� ������ ��´�.
 
 Ŭ���̾�Ʈ�� ������ ��´�.\n
 ������ ����ڰ� �����ΰ��� �׸� ��, Ŭ���̾�Ʈ ������ �ַ� ������� �Ѵ�.\n
 Ŭ���̾�Ʈ ������ ������ �׻� 0, 0�̴�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param pRegion [out] : Ŭ���̾�Ʈ ����
 @return ����
*/
void atWINOBJ_GetClientRegion( atHWINOBJ hWinObj, atREGION * pRegion )
{
	atRECT rect;
	atWINOBJ_GetClientRect( hWinObj, &rect );
	RECT2REGION( *pRegion, rect );
}


/** 
 @brief WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� ��ȿ�� �������� ��ȯ�Ѵ�.
 
 WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� ��ȿ�� �������� ��ȯ�Ѵ�.\n
 ���� ȭ�鿡 ������ �ʴ� �����̶�� ��ȿ�� �������� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpOX, lpOY [out] : ȭ����� ����
 @param nX, nY [in] : Ŭ���̾�Ʈ���� ����
 @return ��ȿ�� ũ�⸦ ������ atTRUE, �ƴϸ� atFALSE
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
 @brief WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� ��ȿ�� �������� ��ȯ�Ѵ�.
 
 WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� ��ȿ�� �������� ��ȯ�Ѵ�.\n
 ���� ȭ�鿡 ������ �ʴ� �����̶�� ��ȿ�� �������� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpOX, lpOY [out] : ȭ����� ����
 @param nX, nY [in] : Ŭ���̾�Ʈ���� ����
 @return ��ȿ�� ũ�⸦ ������ atTRUE, �ƴϸ� atFALSE
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
 @brief WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� ��ȿ�� �������� ��ȯ�Ѵ�.
 
 WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� ��ȿ�� �������� ��ȯ�Ѵ�.\n
 ���� ȭ�鿡 ������ �ʴ� �����̶�� ��ȿ�� �������� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpDesRegion [out] : ȭ����� ����
 @param lpSrcRegion [in] : Ŭ���̾�Ʈ���� ����
 @return ��ȿ�� ũ�⸦ ������ atTRUE, �ƴϸ� atFALSE
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
 @brief WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� ��ȿ�� �������� ��ȯ�Ѵ�.
 
 WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� ��ȿ�� �������� ��ȯ�Ѵ�.\n
 ���� ȭ�鿡 ������ �ʴ� �����̶�� ��ȿ�� �������� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpDesRegion [out] : ȭ����� ����
 @param lpSrcRegion [in] : Ŭ���̾�Ʈ���� ����
 @return ��ȿ�� ũ�⸦ ������ atTRUE, �ƴϸ� atFALSE
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
 @brief WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� ��ȿ�� �������� ��ȯ�Ѵ�.
 
 WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� ��ȿ�� �������� ��ȯ�Ѵ�.\n
 ���� ȭ�鿡 ������ �ʴ� �����̶�� ��ȿ�� �������� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpDesRegion [out] : ȭ����� ����
 @param lpSrcRegion [in] : Ŭ���̾�Ʈ���� ����
 @return ��ȿ�� ũ�⸦ ������ atTRUE, �ƴϸ� atFALSE
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
 @brief WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� ��ȿ�� �������� ��ȯ�Ѵ�.
 
 WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� ��ȿ�� �������� ��ȯ�Ѵ�.\n
 ���� ȭ�鿡 ������ �ʴ� �����̶�� ��ȿ�� �������� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpDesRegion [out] : ȭ����� ����
 @param lpSrcRegion [in] : Ŭ���̾�Ʈ���� ����
 @return ��ȿ�� ũ�⸦ ������ atTRUE, �ƴϸ� atFALSE
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
 @brief WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� ��ȿ�� �������� ��ȯ�Ѵ�.
 
 WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� ��ȿ�� �������� ��ȯ�Ѵ�.\n
 ���� ȭ�鿡 ������ �ʴ� �����̶�� ��ȿ�� �������� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpDesRegion [out] : ȭ����� ����
 @param lpSrcRegion [in] : Ŭ���̾�Ʈ���� ����
 @return ��ȿ�� ũ�⸦ ������ atTRUE, �ƴϸ� atFALSE
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
 @brief WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� �������� ��ȯ�Ѵ�.
 
 WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� �������� ��ȯ�Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpDesRegion [out] : ȭ����� ����
 @param lpSrcRegion [in] : Ŭ���̾�Ʈ���� ����
 @return ��ȿ�� ũ�⸦ ������ atTRUE, �ƴϸ� atFALSE
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
 @brief WinObj���� ������ ������ ȭ����� �������� ��ȯ�Ѵ�.
 
 WinObj���� ������ ������ ȭ����� �������� ��ȯ�Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpDesRect [out] : ȭ����� ����
 @param lpSrcRect [in] : Ŭ���̾�Ʈ���� ����
 @return ��ȿ�� ũ�⸦ ������ atTRUE, �ƴϸ� atFALSE
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
 @brief WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� �������� ��ȯ�Ѵ�.
 
 WinObj�� Ŭ���̾�Ʈ���� ������ ������ ȭ����� �������� ��ȯ�Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpDesRect [out] : ȭ����� ����
 @param lpSrcRect [in] : Ŭ���̾�Ʈ���� ����
 @return ��ȿ�� ũ�⸦ ������ atTRUE, �ƴϸ� atFALSE
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
 @brief ȭ����� WinObj ������ ��´�.
 
 ȭ����� WinObj ������ ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpRect [out] : ȭ����� WinObj ����
 @return ��ȿ�� ũ�⸦ ������ atTRUE, �ƴϸ� atFALSE
*/
atBOOL atWINOBJ_GetScreenRect( atHWINOBJ hWinObj, atRECT *lpRect )
{
	atWINOBJ_GetRect( hWinObj, lpRect );
	atUIAPI_MoveRect( lpRect, lpRect, -lpRect->nStartX, -lpRect->nStartY );
	return atWINOBJ_ConvScreenRect( hWinObj, lpRect, lpRect );
}


/** 
 @brief ȭ����� WinObj ������ ��´�.
 
 ȭ����� WinObj ������ ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpRegion [out] : ȭ����� WinObj ����
 @return ��ȿ�� ũ�⸦ ������ atTRUE, �ƴϸ� atFALSE
*/
atBOOL atWINOBJ_GetScreenRegion( atHWINOBJ hWinObj, atREGION *lpRegion )
{
	atWINOBJ_GetRegion( hWinObj, lpRegion );
	lpRegion->nX = lpRegion->nY = 0;
	return atWINOBJ_ConvScreenRegion( hWinObj, lpRegion, lpRegion );
}


/** 
 @brief ������ Ŭ���̾�Ʈ ũ�⸦ �����Ͽ� ���� WinObj�� ũ�⸦ ��´�.
 
 ������ Ŭ���̾�Ʈ ũ�⸦ �����Ͽ� ���� WinObj�� ũ�⸦ ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpWinSize [out] : ����� ������ ũ��
 @param lpClientSize [in] : �����ϰ��� �ϴ� Ŭ���̾�Ʈ ũ��
 @return ����
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
 @brief WinObj�� Anchor�� �����Ѵ�.
 
 WinObj�� Anchor�� �����Ѵ�.\n
 Anchor��, ��ü�� �θ� ��ü�� ���� ������ �����ϴ� ������ ũ�⸦ �ǹ��Ѵ�.\n
 Anchor�� ����, �θ� ��ü�� ���̾ƿ��� ����� ������ �ڵ����� �ڽ� ��ü�� ���̾ƿ��� �ڵ����� �Բ� �����Ű�� ����� ������ ����� �ش�.
 Anchor�� ������ ��, atANCHOR_IGNORE ���� �Ķ���ͷ� ����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param nTop [in] : ���� Anchor ��. ������ �� atANCHOR_IGNORE
 @param nBottom [in] : �Ʒ��� Anchor ��. ������ �� atANCHOR_IGNORE
 @param nLeft [in] : ���� Anchor ��. ������ �� atANCHOR_IGNORE
 @param nRight [in] : ������ Anchor ��. ������ �� atANCHOR_IGNORE
 @return ����
 @remark Top/Bottom, Left/Right ��, ������ ��� atANCHOR_IGNORE�� �����ϸ� ũ�� ���� ���� ��ġ�� �߾����� �̵��ϴ� ȿ���� ���´�.
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
 @brief WinObj�� Anchor�� �����Ѵ�.
 
 WinObj�� Anchor�� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lprtAnchor [in] : Anchor ��
 @return ����
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
 @brief WinObj�� SoftKey�� Ȱ��ȭ��Ų��.
 
 WinObj�� SoftKey�� Ȱ��ȭ��Ų��.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param bActivate [in] : atTRUE = SoftKey Ȱ��ȭ\n
						 atFALSE = SoftKey ��Ȱ��ȭ
 @return ����
 @remark ���� ��Ŀ���� ���� WinObj��� �� ��ü�� ����ƮŰ�� ���� ����ƮŰ�� ������� ���� �ش�.
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
 @brief WinObj�� SoftKey�� �����Ѵ�.
 
 WinObj�� SoftKey�� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param szLeft [in] : ���� SoftKey ���ڿ�
 @param szCenter [in] : ��� SoftKey ���ڿ�
 @param szRight [in] : ������ SoftKey ���ڿ�
 @return ����
 @remark ���� ��Ŀ���� ���� WinObj��� �� ��ü�� ����ƮŰ�� ���� ����ƮŰ�� ������� ���� �ش�.
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
 @brief WinObj�� SoftKey�� Enable/Disable ���¸� �����Ѵ�.
 
 WinObj�� SoftKey�� Enable/Disable ���¸� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param bLeft [in] : ���� SoftKey Enable/Disable
 @param bCenter [in] : ��� SoftKey Enable/Disable
 @param bRight [in] : ������ SoftKey Enable/Disable
 @return ����
 @remark ���� ��Ŀ���� ���� WinObj��� �� ��ü�� ����ƮŰ�� ���� ����ƮŰ�� ������� ���� �ش�.
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
 @brief WinObj�� SoftKey�� Enable/Disable ���¸� ��´�.
 
 WinObj�� SoftKey�� Enable/Disable ���¸� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param pbLeft [in] : ���� SoftKey Enable/Disable ���� ���� ������
 @param pbCenter [in] : ��� SoftKey Enable/Disable ���� ���� ������
 @param pbRight [in] : ������ SoftKey Enable/Disable ���� ���� ������
 @return ����
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
 @brief WinObj�� SoftKeyd IDS�� �����Ѵ�.
 
 WinObj�� SoftKey IDS�� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param szLeft [in] : ���� SoftKey IDS
 @param szCenter [in] : ��� SoftKey IDS
 @param szRight [in] : ������ SoftKey IDS
 @return ����
 @remark ���� ��Ŀ���� ���� WinObj��� �� ��ü�� ����ƮŰ�� ���� ����ƮŰ�� ������� ���� �ش�.
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
 @brief WinObj�� SoftKey�� ���� SoftKey�� �����Ų��.
 
 WinObj�� SoftKey�� ���� SoftKey�� �����Ų��.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ����
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
 @brief WinObj�� ȭ�鿡 ǥ���ϰų� �����.
 
 WinObj�� ȭ�鿡 ǥ���ϰų� �����.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param bShow [in] : atTRUE = Show \n
					 atFALSE = Hide
 @return ����
 @remark �ַ� ���ο����� ���Ǹ�, ����ڰ� ȣ��ÿ� �ش� ��ü�� �θ��� atWINOBJ_ShowChildWinObj�� �̿��Ѵ�.\n
		 OBJMSG_SHOW / OBJMSG_HIDE�� �߻���Ų��.
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
 @brief WinObj�� ȭ�鿡 ǥ�õǴ°� ���θ� �˾Ƴ���.
 
 WinObj�� ȭ�鿡 ǥ�õǴ°� ���θ� �˾Ƴ���.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return Show ���¸� atTRUE��, Hide ���¸� atFALSE
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
 @brief WinObj�� �̺�Ʈ�� ���� �� �ִ��� ���θ� �����Ѵ�.
 
 WinObj�� �̺�Ʈ�� ���� �� �ִ��� ���θ� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param bEnable [in] : atTRUE = Enable\n
					   atFALSE = Disable
 @return ����
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
 @brief WinObj�� �̺�Ʈ�� ���� �� �ִ� ���θ� �˾Ƴ���.
 
 WinObj�� �̺�Ʈ�� ���� �� �ִ� ���θ� �˾Ƴ���.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return Enable ���¸� atTRUE, Disable ���¸� atFALSE
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
 @brief WinObj�� ��Ŀ���� ������ �ش�.
 
 WinObj�� ��Ŀ���� ������ �ش�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param bFocus [in] : ��Ŀ��
 @return ����
 @remark �ַ� ���ο����� ���Ǹ�, ����ڰ� ȣ��ÿ� �ش� ��ü�� �θ��� atWINOBJ_SetChildWinObjFocus�� �̿��ؼ� ��Ŀ���� �̵���Ų��.\n
		 OBJMSG_GOTFOCUS / OBJMSG_LOSTFOCUS�� �߻���Ų��.
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
 @brief ��Ŀ���� ���� WinObj�ΰ� ���θ� ��´�.
 
 ��Ŀ���� ���� WinObj�ΰ� ���θ� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ���� ��Ŀ���� ������ ������ atTRUE, �ƴϸ� atFALSE
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
 @brief WinObj�� ��Ŀ���� ���� �� �ִ��� ���θ� ��´�.
 
 WinObj�� ��Ŀ���� ���� �� �ִ��� ���θ� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ��Ŀ���� ���� �� �ִ� ��ü�� atTREU, �ƴϸ� atFALSE
*/
atBOOL atWINOBJ_IsFocusable( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atFALSE;
	
	return lpWinObj->dwAttr & atOBJATTR_FOCUSABLE;
}


/** 
 @brief WinObj�� ��Ŀ���� ���� �� �ִ°� ���°� ���θ� �����Ѵ�.
 
 WinObj�� ��Ŀ���� ���� �� �ִ°� ���°� ���θ� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param bFocusable [in] : ��Ŀ���� ���� �� �ִ°� ���°� ����
 @return ����
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

		// ��Ŀ���� ���� obj�� focusable �Ӽ��� ���ֹ����� ��쿣 next sibliling obj�� ��Ŀ���� �ű��.
		if( !bFocusable /* && lpWinObj->bFocus*/ ){
			atHWINOBJ hParentObj = atWINOBJ_GetParentObj(hWinObj);
			if( hParentObj != atNULL ){
				if(atWINOBJ_GetCurChildWinObj( hParentObj ) == hWinObj) {
					atHWINOBJ hNextObj = atWINOBJ_GetNextFocusableChildWinObj( hParentObj, hWinObj );
					//if(hNextObj != atNULL) {
						atWINOBJ_SetCurChildWinObj(hParentObj, hNextObj);

						//if( atWINOBJ_GetClassID(hParentObj) == 0x0001 ){ // �θ� ������ Ÿ���� ����
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
 @brief Anchor�� ��� ���θ� �����Ѵ�.
 
 Anchor�� ��� ���θ� �����Ѵ�.\n
 �⺻������ �ʱ� �����ÿ��� Anchor�� ��� �����ϵ��� ����������,\n
 atWINOBJ_SetPosition�̳� atWINOBJ_SetRect, atWINOBJ_SetRegion�� ���� �ڵ����� ������ ���� �ִ�.\n
 �� ���, �ٽ� Anchor�� Ȱ��ȭ��Ű���� �Ѵٸ� �� �Լ��� �Ἥ UseAnchor �Ӽ��� ����� �Ѵ�.


 @param hWinObj [in] : WinObj�� �ڵ�
 @return ����
 @remark �⺻������ �ʱ� �����ÿ��� Anchor�� ��� �����ϵ��� ����������, 
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
 @brief WinObj�� �ڵ鷯�� ��´�.
 
 WinObj�� �ڵ鷯�� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return WinObj�� �ڵ鷯
*/
atLPWINOBJPROC	atWINOBJ_GetObjProc( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atNULL;
	
	return lpWinObj->fnObjProc;
}


/** 
 @brief WinObj�� �ڵ鷯�� �缳���Ѵ�.
 
 WinObj�� �ڵ鷯�� �缳���Ѵ�.\n
 �ַ� ����Ŭ������ ���� ����ڰ� �ڵ鷯�� ������ �� �� ����.\n
 �� ���, ��ȯ�� ������ �ڵ鷯�� ����ڰ� �����Ͽ� ������ �� �ڵ鷯 �ӿ��� atWINOBJ_CallObjProc�� �̿��� ȣ���� ��� �Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpProc [in] : WinObj�� �ڵ鷯
 @return ���� ������ �ڵ鷯
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
 @brief WinObj�� Notify �ڵ鷯�� ��´�.
 
 WinObj�� Notify �ڵ鷯�� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return WinObj�� Notify �ڵ鷯
*/
atLPWINOBJNOTIFIER	atWINOBJ_GetObjNotifier( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atNULL;
	
	return lpWinObj->fnObjNotifier;
}


/** 
 @brief WinObj�� Notify Data�� ��´�.
 
 WinObj�� Notify Data�� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return WinObj�� Notify Data ������
*/
atLPVOID	atWINOBJ_GetObjNotifyData( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);

	if( lpWinObj == atNULL ) return atNULL;
	
	return lpWinObj->lpObjNotifyData;
}


/** 
 @brief WinObj�� Notify �ڵ鷯�� �缳���Ѵ�.
 
 WinObj�� Notify �ڵ鷯�� �缳���Ѵ�.\n

 @param hWinObj [in] : WinObj�� �ڵ�
 @param lpNotifier [in] : WinObj�� Notify �ڵ鷯
 @param lpNotifyData [in] : WinObj�� Notify ������
 @return ���� ������ �ڵ鷯
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
 @brief WinObj�� �ڵ鷯�� ���� ȣ���Ѵ�.
 
 WinObj�� �ڵ鷯�� ���� ȣ���Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param nMsg [in] : ������ �޽���
 @param lParam1, lParam2 [in] : �޽����� �Ķ����
 @return �޽��� �ڵ鷯�� �����
 @remark �ڵ鷯�� ����Ŭ������ ���� ����. ������ ���ο����� ���δ�. \n
		 ����ڰ� ���� �޽��� �ڵ鷯�� ȣ���� ���� atWINOBJ_ProcessObjProc�� ����Ѵ�.
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
 @brief WinObj�� �޽��� ���μ����� �����Ѵ�.
 
 WinObj�� �޽��� ���μ����� �����Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param nMsg [in] : ������ �޽���
 @param lParam1, lParam2 [in] : �޽����� �Ķ����
 @return �޽��� �ڵ鷯�� �����
 @remark �ַ� ���ο����� ���δ�.
*/
int atWINOBJ_ProcessObjProc( atHWINOBJ hWinObj, int nMsg, long lParam1, long lParam2 )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	int r = 0;

	if( lpWinObj == atNULL ) return -1;
	
	if( lpWinObj->fnObjProc ) r = lpWinObj->fnObjProc( hWinObj, nMsg, lParam1, lParam2 );
	if( r == 0 ){
	  // ���� ó���� �����츦 �ı��ߴٸ�, ���� ó���� ���� �ʴ´�.
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
 @brief WinObj�� GDC�� ��´�.
 
 WinObj�� GDC(Graphic Device Context)�� ��´�.\n
 �⺻������ WinObj�� GDC�� �Ѱ��� ���� �ȴ�.\n
 �� �Լ��� ȣ���԰� ���ÿ� �ǽð����� GDC�� ������ ���� �ȴ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return WinObj�� GDC �ڵ�
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
 @brief WinObj�� Child WinObj�� �߰��Ѵ�.
 
 WinObj�� Child WinObj�� �߰��Ѵ�.\n

 @param hWinObj [in] : WinObj�� �ڵ�
 @param hChildWinObj [in] : Child WinObj�� �ڵ�
 @return ����
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
 @brief Child ��ü�� �Ҹ�
 
 Child ��ü�� �Ҹ�

 @param hWinObj [in] : WinObj�� �ڵ�
 @param hChildWinObj [in] : Child WinObj�� �ڵ�
 @return ����
*/
void atWINOBJ_RemoveChildWinObj( atHWINOBJ hWinObj, atHWINOBJ hChildWinObj )
{
	atLPWINOBJ lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;
	
	atWINOBJMNG_DestroyWinObj( lpWinObj->hChildObjMng, hWinObj );
}


/** 
 @brief ����� Child ��ü�� ������ ���Ѵ�.
 
 ����� Child ��ü�� ������ ���Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ����� Child ��ü�� ����
*/
atINT atWINOBJ_GetChildWinObjCount( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return 0;
	
	return atWINOBJMNG_GetWinObjCount( lpWinObj->hChildObjMng );
}


/** 
 @brief ���� ��Ŀ���� ���� Child ��ü�� ��´�.
 
 ���� ��Ŀ���� ���� Child ��ü�� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ��Ŀ���� ���� Child ��ü�� �ڵ�
*/
atHWINOBJ atWINOBJ_GetCurChildWinObj( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	return atWINOBJMNG_GetCurWinObj(lpWinObj->hChildObjMng);
}


/** 
 @brief ID�� ��ġ�ϴ� Child��ü�� ��´�.
 
 ID�� ��ġ�ϴ� Child��ü�� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param nWinObjID [in] : Child ��ü�� ID
 @return ID�� ��ġ�ϴ� Child��ü�� �ڵ�
 @remark ID�� Unique���� �ʱ� ������, �ߺ��� ID�� ���� ��� Ư���� ��ü�� ������ �� �ִ�.
*/
atHWINOBJ atWINOBJ_FindChildWinObjByID( atHWINOBJ hWinObj, atINT nWinObjID )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	return atWINOBJMNG_FindWinObjByID(lpWinObj->hChildObjMng, nWinObjID);
}


/** 
 @brief �ش� �̸��� Child��ü�� ��´�.
 
 �ش� �̸��� Child��ü�� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param szWinObjName [in] : Child ��ü�� �̸�
 @return �ش� �̸��� Child��ü�� �ڵ�
 @remark �̸��� Unique���� �ʱ� ������, �ߺ��� �̸��� ���� ��� Ư���� ��ü�� ������ �� �ִ�.
*/
atHWINOBJ atWINOBJ_FindChildWinObjByName( atHWINOBJ hWinObj, atLPTSTR szWinObjName )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	return atWINOBJMNG_FindWinObjByName(lpWinObj->hChildObjMng, szWinObjName);
}


/** 
 @brief Child ��ü�� ��Ŀ���� �ش�.
 
 Child ��ü�� ��Ŀ���� �ش�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param hChildWinObj [in] : Child WinObj�� �ڵ�
 @return ������ atTRUE, ���н� atFALSE
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
 @brief ��Ŀ���� ���� �� �ִ� ���� Child ��ü�� ��´�.
 
 ��Ŀ���� ���� �� �ִ� ���� Child ��ü�� ��´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param hChildWinObj [in] : Child WinObj�� �ڵ�
 @return ������ Next Child WinObj�� �ڵ�, ���н� atNULL
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
 @brief Child ��ü�� ȭ�鿡 ǥ���ϰų� �����.
 
 Child ��ü�� ȭ�鿡 ǥ���ϰų� �����.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param hChildWinObj [in] : Child WinObj�� �ڵ�
 @param bShow [in] : Show / Hide
 @return ����
*/
void atWINOBJ_ShowChildWinObj( atHWINOBJ hWinObj, atHWINOBJ hChildWinObj, atBOOL bShow )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;
	
	atWINOBJMNG_ShowWinObj( lpWinObj->hChildObjMng, hChildWinObj, bShow );
}
 

/** 
 @brief Child ��ü�� ���� �������� ������ �Ѵ�.
 
 Child ��ü�� ���� �������� ������ �Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param hChildWinObj [in] : Child WinObj�� �ڵ�
 @return ������ �� �տ� ������ �� Child ��ü�� �ڵ�
*/
atHWINOBJ atWINOBJ_SetTopChildWinObj( atHWINOBJ hWinObj, atHWINOBJ hChildWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	return atWINOBJMNG_SetTopWinObj( lpWinObj->hChildObjMng, hChildWinObj);
}


/** 
 @brief Child ��ü�� ���� �������� ������.
 
 Child ��ü�� ���� �������� ������.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param hChildWinObj [in] : Child WinObj�� �ڵ�
 @return ������ �� �ڿ� ��ġ�ϴ� Child ��ü�� �ڵ�
*/
atHWINOBJ atWINOBJ_SetBottomChildWinObj( atHWINOBJ hWinObj, atHWINOBJ hChildWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return atNULL;

	return atWINOBJMNG_SetBottomWinObj( lpWinObj->hChildObjMng, hChildWinObj );
}


/** 
 @brief WinObj�� Default �޽��� �ڵ鷯
 
 WinObj�� Default �޽��� �ڵ鷯

 @param hWinObj [in] : WinObj�� �ڵ�
 @param nMsg [in] : ������ �޽���
 @param lParam1, lParam2 [in] : �޽����� �Ķ����
 @return �޽��� �ڵ鷯�� �����
 @remark WinObj�� ��ӹ��� ��ü�� �ݵ�� �� �ڵ鷯�� ���Խ��� ��� �Ѵ�.
*/
void atWINOBJ_PostMessageAllChildWinObjs( atHWINOBJ hWinObj, int nMsg, long lParam1, long lParam2 )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;

	atWINOBJMNG_SendMessageAllWinObjs( _TO_WINOBJMNG(lpWinObj->hChildObjMng), nMsg, lParam1, lParam2 );
}


/** 
 @brief WinObj�� ����� ��� Child ��ü�� �Ҹ��Ų��.
 
 WinObj�� ����� ��� Child ��ü�� �Ҹ��Ų��.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ����
*/
void atWINOBJ_RemoveAllChildWinObjs( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;
	
	atWINOBJMNG_DestroyWinObjMng( lpWinObj->hChildObjMng );
	
	lpWinObj->hChildObjMng = atNULL;
}
 

/** 
 @brief WinObj�� ����� ��� Child ��ü�� �׸���. (���ο�)
 
 WinObj�� ����� ��� Child ��ü�� �׸���. (���ο�)

 @param hWinObj [in] : WinObj�� �ڵ�
 @param bFocused [in] : �� ��ü�� focused �� ���ΰ� ���� 
 @return �׷��� ���� ��ü�� ����
 @remark ���� ȣ���.
*/
int atWINOBJ_DrawAllChildWinObjs( atHWINOBJ hWinObj, atBOOL bFocused )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return -1;

	return atWINOBJMNG_RedrawAllWinObj( lpWinObj->hChildObjMng, bFocused );
}


/** 
 @brief ����� Child ��ü�� ũ��� ��ġ�� ������
 
 ����� Child ��ü�� ũ��� ��ġ�� �����Ľ�Ų��.\n
 �̶� Anchor�� ���ȴ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ����
*/
void atWINOBJ_AdjustAllChildWinObjs( atHWINOBJ hWinObj )
{
	atLPWINOBJ		lpWinObj = atWINOBJ_GetPtr(hWinObj);
	if( lpWinObj == atNULL ) return;

	atWINOBJMNG_AdjustAllWinObjArea( lpWinObj->hChildObjMng );
}


/** 
 @brief WinObj�� ���� Ÿ�̸Ӹ� �߰��Ѵ�.
 
 WinObj�� ���� Ÿ�̸Ӹ� �߰��Ѵ�.\n
 Ÿ�̸� ���۽�, �ش� WinObj�� OBJMSG_TIMER �̺�Ʈ�� ������ �ش�.\n
 WinObj�� ������ ��, �ڵ����� �Բ� �Ҹ��Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param id [in] : Ÿ�̸��� ID. Ÿ�̸� �̺�Ʈ �߻���, ù��° �Ķ���ͷ� ���޵ȴ�.
 @param interval [in] : Ÿ�̸��� ȣ�� �ֱ�. ms ����
 @param IsRepeat [in] : �ݺ��ؼ� ȣ��Ǵ°� ����.
 @param IsEnable [in] : ������ ���ÿ� Ÿ�̸Ӱ� ������ ���ΰ� ����.
 @param IsFocus [in] : WinObj�� ��Ŀ���� ������ ���� ������ ���ΰ� ����. 
 @return �߰��� Ÿ�̸��� �ڵ�
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
 @brief ID�� �̿��� WinObj�� ����� Ÿ�̸Ӹ� ã�´�.
 
 ID�� �̿��� WinObj�� ����� Ÿ�̸Ӹ� ã�´�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param id [in] : Ÿ�̸��� ID
 @return ��ġ�ϴ� ID�� Ÿ�̸� �ڵ�
 @remark ID�� Unique���� �ʱ� ������, �ߺ��� ID�� ���� ��� Ư�� Ÿ�̸Ӹ� ã�� �� �ְ� �ȴ�.
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
 @brief ������ WinObj ������ Tick���� �ʱ�ȭ�Ѵ�.
 
 ������ WinObj ������ Tick���� �ʱ�ȭ�Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param hTimer [in] : �ʱ�ȭ��ų Ÿ�̸� �ڵ�
 @return ����
*/
void atWINOBJ_SetTimerInterval( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer, atLONG nInterval )
{
	atWINOBJTIMER *tmr = (atWINOBJTIMER *)hTimer;
	if( tmr == atNULL ) return;
	
    atUIAPI_SetTimerInterval( tmr->hTimer, nInterval );
}


/** 
 @brief ������ WinObj ������ Tick���� �ʱ�ȭ�Ѵ�.
 
 ������ WinObj ������ Tick���� �ʱ�ȭ�Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param hTimer [in] : �ʱ�ȭ��ų Ÿ�̸� �ڵ�
 @return ����
*/
void atWINOBJ_EnableTimer( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer, atBOOL IsEnable )
{
	atWINOBJTIMER *tmr = (atWINOBJTIMER *)hTimer;	
	if( tmr == atNULL ) return;
	
    atUIAPI_EnableTimer( tmr->hTimer, IsEnable );
}


/** 
 @brief ������ WinObj ������ Tick���� �ʱ�ȭ�Ѵ�.
 
 ������ WinObj ������ Tick���� �ʱ�ȭ�Ѵ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param hTimer [in] : �ʱ�ȭ��ų Ÿ�̸� �ڵ�
 @return ����
*/
void atWINOBJ_ResetTimer( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer )
{
	atWINOBJTIMER *tmr = (atWINOBJTIMER *)hTimer;	
	if( tmr == atNULL ) return;
	
    atUIAPI_ResetTimer( tmr->hTimer );
}


/** 
 @brief ������ WinObj ���� Ÿ�̸Ӹ� �Ҹ��Ų��.
 
 ������ WinObj ���� Ÿ�̸Ӹ� �Ҹ��Ų��.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param hTimer [in] : �Ҹ��ų Ÿ�̸� �ڵ�
 @return ����
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
 @brief WinObj ���� Ÿ�̸Ӹ� ��� ������Ų��.
 
 WinObj ���� Ÿ�̸Ӹ� ��� ������Ų��.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param hTimer [in] : Ÿ�̸� �ڵ�
 @return ����
*/
void atWINOBJ_SuspendTimer( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer )
{
	atWINOBJTIMER *tmr = (atWINOBJTIMER *)hTimer;	
	if( tmr == atNULL ) return;
	
    atUIAPI_SuspendTimer( tmr->hTimer );
}


/** 
 @brief WinObj ���� Ÿ�̸Ӹ� Resume��Ų��.
 
 WinObj ���� Ÿ�̸Ӹ� Resume��Ų��.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param hTimer [in] : Ÿ�̸� �ڵ�
 @return ����
*/
void atWINOBJ_ResumeTimer( atHWINOBJ hWinObj, atHWINOBJTIMER hTimer )
{
	atWINOBJTIMER *tmr = (atWINOBJTIMER *)hTimer;	
	if( tmr == atNULL ) return;
	
    atUIAPI_ResumeTimer( tmr->hTimer );
}


/** 
 @brief WinObj�� ����� ��� Ÿ�̸Ӹ� Enable/Disable��Ų��.
 
 WinObj�� ����� ��� Ÿ�̸Ӹ� Enable/Disable��Ų��.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param IsEnable [in] : Enable/Disable
 @return ����
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
 @brief WinObj�� ����� ��� Ÿ�̸Ӹ� �Ҹ��Ų��.
 
 WinObj�� ����� ��� Ÿ�̸Ӹ� �Ҹ��Ų��.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ����
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
 @brief WinObj ���� Ÿ�̸Ӹ� ��� ��� ������Ų��.
 
 WinObj ���� Ÿ�̸Ӹ� ��� ��� ������Ų��.

 @param hWinObj [in] : WinObj�� �ڵ�
 @param bFocusedSuspend [in] : ��Ŀ�� Ÿ�̸Ӹ� ������ų ������ ����
 @return ����
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
 @brief ��� ������ WinObj ���� Ÿ�̸Ӹ� ��� �����
 
 ��� ������ Ÿ�̸Ӹ� ��� �����\n
 Pause �� Ÿ�̸ӷ� �����Ǹ�, Pause ��� Enable�̾��� Ÿ�̸ӷ� ���� ���ѵȴ�.

 @param hWinObj [in] : WinObj�� �ڵ�
 @return ����
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
 @brief WinObj ������ �ش� ��ġ�� Event ����� �� Child ��ü�� �ִ°�
 
 WinObj ������ �ش� ��ġ�� Event Ÿ���� �� Child ��ü�� �ִ°�

 @param hWinObj [in] : WinObj�� �ڵ�
 @param nX, nY [in] : ��ġ
 @return Event ��� ��ü�� ������ atTRUE, ������ atFALSE
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
 @brief WinObj�� Default �޽��� �ڵ鷯
 
 WinObj�� Default �޽��� �ڵ鷯

 @param hWinObj [in] : WinObj�� �ڵ�
 @param nMsg [in] : ������ �޽���
 @param lParam1, lParam2 [in] : �޽����� �Ķ����
 @return �޽��� �ڵ鷯�� �����
 @remark WinObj�� ��ӹ��� ��ü�� �ݵ�� �� �ڵ鷯�� ���Խ��� ��� �Ѵ�.
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
			atWINOBJ_RemoveAllChildWinObjs(hWinObj); // �ϴ� ���� �Ҹ���� ���´�. �� �� ������ ����.
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
/* ����, Ű���� ���ְ� �ƴ� ��ġ ������ �������̽��� ��������� ������ ſ��, Ű���� �� ó���� �� �´� �κ��� �����. ������ �ӽ÷� �浹�Ǵ� Űó���� ��� ���� ���´�.
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
				// ��ġ �������̽� �󿡼� ���ʿ��ϱ⿡, 
				// �����ս��� ����... ���콺 Ŭ������ ���� �͵��� Move���� ����.
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

