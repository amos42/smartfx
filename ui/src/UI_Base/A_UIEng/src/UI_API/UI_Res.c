/**
  @file UI_Res.c
  @brief Resource에 관한 API
  @remark Resource에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Res.h
*/
#include "UI_API/UI_API.h"
#include "OEM_UI_Res.h"


#define MAX_RESSRC_CNT 10


typedef struct _tagTResSrEntry {
	atTCHAR		szIDStr[32];
	atHRES		 hRes;
	atINT		nRefCnt;
// 	atLPCALLBACKALLOC fnAllocator;
//	atLPCALLBACKFREE fnDeallocator;
} TResSrcEntry;

static TResSrcEntry   g_ResArray[MAX_RESSRC_CNT];
static int			g_ResCount;
static atHRESSRC g_hDefResSource = atNULL;


static stringResCallbackfn g_stringResfn = atNULL;
static atINT g_bmsIndex = 0;
static atHCRSECTION g_StrCrSection;


static OEM_UI_RES_T tOemFn;

atVOID	OEM_UI_Res_SetOemFn( OEM_UI_RES_T *fnOem )
{
	tOemFn = *fnOem;
}


atVOID	OEM_UI_Res_GetOemFn( OEM_UI_RES_T *fnOem )
{
	*fnOem = tOemFn;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL	atUIAPI_InitRes( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	int i;

	g_StrCrSection = atUIAPI_CreateCriticalSection();

	for( i = 0; i < MAX_RESSRC_CNT; i++ ){
		g_ResArray[i].hRes = atNULL;
	}

	g_hDefResSource = atNULL;

    return tOemFn.fnInitRes( lSysInst, lInitData1, lInitData2 );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID	atUIAPI_FinalRes( NOARGS )
{
	int i;

	atUIAPI_DestroyCriticalSection( g_StrCrSection );
	g_StrCrSection = atNULL;
	g_hDefResSource = atNULL;

	for( i = 0; i < MAX_RESSRC_CNT; i++ ){
		if( g_ResArray[i].hRes != atNULL ){
			tOemFn.fnRemoveResSource( (OEM_RESSRC)g_ResArray[i].hRes );
			g_ResArray[i].hRes = atNULL;
			break;
		}
	}
	
    tOemFn.fnFinalRes();
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHRESSRC atUIAPI_AddResSource( atLPTSTR szIDStr, atINT type, const atLPVOID lpPtr, atLONG size )
{
    return atUIAPI_AddResSourceEx( szIDStr, type, lpPtr, size, atNULL, atNULL );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHRESSRC atUIAPI_AddResSourceEx( atLPTSTR szIDStr, atINT type, const atLPVOID lpPtr, atLONG size, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator )
{
	int i;
    atHRESSRC ressrc;

    for( i = 0; i < MAX_RESSRC_CNT; i++ ){
		if( (g_ResArray[i].hRes != atNULL) && (atUIAPI_strcmp(g_ResArray[i].szIDStr, szIDStr) == 0) ){
			g_ResArray[i].nRefCnt++;
			return g_ResArray[i].hRes;
		}
    }
	
    ressrc = (atHRESSRC)tOemFn.fnCreateResSource( szIDStr, type, lpPtr, size, fnAllocator, fnDeallocator );
    if( ressrc == atNULL ) return atNULL;

    for( i = 0; i < MAX_RESSRC_CNT; i++ ){
		if( g_ResArray[i].hRes == atNULL ){
			atUIAPI_strcpy( g_ResArray[i].szIDStr, szIDStr );
			g_ResArray[i].hRes = ressrc;
			g_ResArray[i].nRefCnt = 1;
			break;
		}
    }

    if( g_hDefResSource == atNULL ) g_hDefResSource = ressrc;
	
    return ressrc;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL	atUIAPI_RemoveResSource( atLPTSTR szIDStr )
{
	int i;

	for( i = 0; i < MAX_RESSRC_CNT; i++ ){
		if( (g_ResArray[i].hRes != atNULL) && (atUIAPI_strcmp( szIDStr, g_ResArray[i].szIDStr ) == 0) ){			
			g_ResArray[i].nRefCnt--;
			if( g_ResArray[i].nRefCnt <= 0 ){
				if( g_ResArray[i].hRes == g_hDefResSource ) g_hDefResSource = atNULL;
				tOemFn.fnRemoveResSource( (OEM_RESSRC)g_ResArray[i].hRes );
				g_ResArray[i].hRes = atNULL;
			}
			return atTRUE;
		}
	}

	return atFALSE;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHRESSRC	atUIAPI_SetDefaultResSource( atLPTSTR szIDStr )
{
	atHRESSRC hOldResSrc = g_hDefResSource;
	g_hDefResSource = atUIAPI_GetResSource( szIDStr );
	return hOldResSrc;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHRESSRC	atUIAPI_GetResSource( atLPTSTR szIDStr )
{
	int i;

	if( szIDStr == atNULL ) return atNULL;

	for( i = 0; i < MAX_RESSRC_CNT; i++ ){
		if( (g_ResArray[i].hRes != atNULL) && (atUIAPI_strcmp( szIDStr, g_ResArray[i].szIDStr ) == 0) ){			
			return g_ResArray[i].hRes;
		}
	}

	return atNULL;
}


/**
@brief

......

@param
@return 없음
*/
atBOOL atUIAPI_GetResSourceInfo( atHRESSRC hResSrc, atUIAPI_RESSRC_INFO *lpResSrcInfo )
{
	if (hResSrc == atNULL) hResSrc = g_hDefResSource;
	if (hResSrc == atNULL) return atFALSE;
	return tOemFn.fnGetResSourceInfo((OEM_RESSRC)hResSrc, lpResSrcInfo);
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL	atUIAPI_SetResAllocator( atHRESSRC hResSrc, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator )
{
    if( hResSrc == atNULL ) hResSrc = g_hDefResSource;
    if( hResSrc == atNULL ) return atFALSE;
    return tOemFn.fnSetResAllocator( (OEM_RESSRC)hResSrc, fnAllocator, fnDeallocator );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL	atUIAPI_GetResAllocator( atHRESSRC hResSrc, atLPCALLBACKALLOC *lpfnAllocator, atLPCALLBACKFREE *lpfnDeallocator )
{
    if( hResSrc == atNULL ) hResSrc = g_hDefResSource;
    if( hResSrc == atNULL ) return atFALSE;
    return tOemFn.fnGetResAllocator( (OEM_RESSRC)hResSrc, lpfnAllocator, lpfnDeallocator );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL atUIAPI_IsExistRes( atHRESSRC hResSrc, atLPTSTR szFileName )
{
    if( hResSrc == atNULL ) hResSrc = g_hDefResSource;
    if( hResSrc == atNULL ) return atFALSE;
    return tOemFn.fnIsExistRes( (OEM_RESSRC)hResSrc, szFileName );
}


/**
@brief

......

@param
@return 없음
*/
atLONG atUIAPI_GetResInfo( atHRESSRC hResSrc, atLPTSTR szFileName, atUIAPI_RES_INFO *lpResInfo )
{
	if (hResSrc == atNULL) hResSrc = g_hDefResSource;
	if (hResSrc == atNULL) return 0;
	return tOemFn.fnGetResInfo( (OEM_RESSRC)hResSrc, szFileName, lpResInfo );
}

/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atLONG atUIAPI_GetResSize( atHRESSRC hResSrc, atLPTSTR szFileName )
{
    if( hResSrc == atNULL ) hResSrc = g_hDefResSource;
    if( hResSrc == atNULL ) return 0;
    return tOemFn.fnGetResInfo( (OEM_RESSRC)hResSrc, szFileName, atNULL );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atHRES	atUIAPI_LoadRes( atHRESSRC hResSrc, atLPTSTR szFileName )
{
    if( hResSrc == atNULL ) hResSrc = g_hDefResSource;
    if( hResSrc == atNULL ) return atNULL;
    return (atHRES)tOemFn.fnLoadRes( (OEM_RESSRC)hResSrc, szFileName );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atBOOL	atUIAPI_GetResData( atHRES hRes, atLPVOID *lppPtr, atLONG *lpSize )
{
    if( hRes == atNULL ) return atFALSE;
    return tOemFn.fnGetResData( (OEM_RES)hRes, lppPtr, lpSize );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atLPVOID atUIAPI_GetResPtr( atHRES hRes )
{
    atBOOL r;
    atLPVOID lpPtr;
    atLONG size;
	
    if( hRes == atNULL ) return atNULL;
    r = tOemFn.fnGetResData( (OEM_RES)hRes, &lpPtr, &size );
    if( !r ) return atNULL;
    return lpPtr;
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_FreeRes(atHRES hRes )
{
    tOemFn.fnFreeRes( hRes );
}


/** 
 @brief 
 
 ......

 @param 
 @return 없음
*/
atVOID atUIAPI_SetStringResCallback(stringResCallbackfn fn, atINT bmsIndex)
{
	g_stringResfn = fn;
	g_bmsIndex    = bmsIndex;
}


/** 
 @brief 
 
 ......

 @param ids     [in] : String Resource ID
 @param buf     [out]: 문자열이 저장될 버퍼 
 @param bufSize [in] : buf의 크기 
 @return ids에 해당하는 문자열이 존재할 경우 atTRUE, 아니면 atFALSE
*/
atBOOL atUIAPI_LoadString(atDWORD ids, atTCHAR* buf, atLONG bufSize)
{
	if(buf && bufSize > 0) buf[0] = 0;
	if(!g_stringResfn) return atFALSE;

	return g_stringResfn(ids, buf, bufSize, g_bmsIndex);
}

