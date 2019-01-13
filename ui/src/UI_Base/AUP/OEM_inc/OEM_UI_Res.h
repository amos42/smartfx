/**
  @file OEM_UI_Res.h
  @brief Resource에 관한 포팅 OEM API 선언
  @remark Resource에 관한 포팅 OEM API 선언
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Res.h
*/
#ifndef __OEM_UI_RES_H_
#define __OEM_UI_RES_H_

#include "UI_Types.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef void * OEM_RESSRC;
typedef void * OEM_RES;


#define atOEM_UI_RES_SRC_NONE           0
#define atOEM_UI_RES_SRC_DIRECTORY      1
#define atOEM_UI_RES_SRC_RESFILE        2
#define atOEM_UI_RES_SRC_RESSTREAM      3


typedef atBOOL		FN_OEM_UI_InitRes( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef atVOID		FN_OEM_UI_FinalRes( NOARGS );

typedef OEM_RESSRC	FN_OEM_UI_CreateResSource( atLPTSTR szIDStr, atINT type, const atLPVOID ptr, atLONG size, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator );
typedef atBOOL		FN_OEM_UI_RemoveResSource( OEM_RESSRC hOemResSrc );
typedef atBOOL		FN_OEM_UI_GetResSourceInfo(OEM_RESSRC hOemResSrc, atUIAPI_RESSRC_INFO *lpResSrcInfo);

typedef atBOOL		FN_OEM_UI_SetResAllocator( OEM_RESSRC hOemResSrc, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator );
typedef atBOOL		FN_OEM_UI_GetResAllocator( OEM_RESSRC hOemResSrc, atLPCALLBACKALLOC *lpfnAllocator, atLPCALLBACKFREE *lpfnDeallocator );

typedef atBOOL 		FN_OEM_UI_IsExistRes( OEM_RESSRC hOemResSrc, atLPTSTR szFileName );
typedef atLONG		FN_OEM_UI_GetResInfo( OEM_RESSRC hOemResSrc, atLPTSTR szFileName, atUIAPI_RES_INFO *lpResInfo );

typedef OEM_RES		FN_OEM_UI_LoadRes( OEM_RESSRC hOemResSrc, atLPTSTR szFileName );
typedef atBOOL 		FN_OEM_UI_GetResData( OEM_RES hOemRes, atLPVOID *lppPtr, atLONG *lpSize );
typedef atVOID 		FN_OEM_UI_FreeRes( OEM_RES hOemRes );


typedef struct tagOEM_UI_RES {
	FN_OEM_UI_InitRes			*fnInitRes;
	FN_OEM_UI_FinalRes			*fnFinalRes;

	FN_OEM_UI_CreateResSource	*fnCreateResSource;
	FN_OEM_UI_RemoveResSource	*fnRemoveResSource;
	FN_OEM_UI_GetResSourceInfo	*fnGetResSourceInfo;

	FN_OEM_UI_SetResAllocator	*fnSetResAllocator;
	FN_OEM_UI_GetResAllocator	*fnGetResAllocator;

	FN_OEM_UI_IsExistRes		*fnIsExistRes;
	FN_OEM_UI_GetResInfo		*fnGetResInfo;

	FN_OEM_UI_LoadRes			*fnLoadRes;
	FN_OEM_UI_GetResData		*fnGetResData;
	FN_OEM_UI_FreeRes			*fnFreeRes;
} OEM_UI_RES_T;


atVOID	OEM_UI_Res_SetOemFn( OEM_UI_RES_T *fnOem );
atVOID	OEM_UI_Res_GetOemFn( OEM_UI_RES_T *fnOem );


#ifdef __cplusplus
}
#endif

#endif

