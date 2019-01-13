/**
  @file UI_Res.h
  @brief Resource에 관한 API
  @remark Resource에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Res.c
*/
#ifndef __UI_RES_H_
#define __UI_RES_H_

#include "UI_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atHANDLE  atHRES;
typedef atHANDLE  atHRESSRC;


#define atUI_RES_SRC_NONE           0
#define atUI_RES_SRC_DIRECTORY      1
#define atUI_RES_SRC_RESFILE        2
#define atUI_RES_SRC_RESSTREAM      3
#define atUI_RES_SRC_USERDEFINE     0x10


typedef struct tagUIAPI_RESSRC_INFO {
	atINT type;
	atLPVOID lpPtr;
	atLONG size;
} atUIAPI_RESSRC_INFO;

typedef struct tagUIAPI_RES_INFO {
	atLONG nStreamOffset;
	atLONG nFileSize;
	atINT  nCompressMethod;
	atLONG nCompressedSize;
} atUIAPI_RES_INFO;


atBOOL		atUIAPI_InitRes( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
atVOID		atUIAPI_FinalRes( NOARGS );

atHRESSRC	atUIAPI_AddResSource( atLPTSTR szIDStr, atINT type, const atLPVOID lpPtr, atLONG size );
atHRESSRC	atUIAPI_AddResSourceEx( atLPTSTR szIDStr, atINT type, const atLPVOID lpPtr, atLONG size, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator );
atBOOL		atUIAPI_RemoveResSource( atLPTSTR szIDStr );
atHRESSRC	atUIAPI_SetDefaultResSource( atLPTSTR szIDStr );
atHRESSRC	atUIAPI_GetResSource( atLPTSTR szIDStr );
atBOOL      atUIAPI_GetResSourceInfo( atHRESSRC hResSrc, atUIAPI_RESSRC_INFO *lpResSrcInfo );

atBOOL		atUIAPI_SetResAllocator( atHRESSRC hResSrc, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator );
atBOOL		atUIAPI_GetResAllocator( atHRESSRC hResSrc, atLPCALLBACKALLOC *fnAllocator, atLPCALLBACKFREE *fnDeallocator );

atBOOL 		atUIAPI_IsExistRes( atHRESSRC hResSrc, atLPTSTR szFileName );
atLONG 		atUIAPI_GetResSize( atHRESSRC hResSrc, atLPTSTR szFileName );
atLONG		atUIAPI_GetResInfo( atHRESSRC hResSrc, atLPTSTR szFileName, atUIAPI_RES_INFO *lpResInfo );

atHRES		atUIAPI_LoadRes( atHRESSRC hResSrc, atLPTSTR szFileName );
atBOOL		atUIAPI_GetResData( atHRES hRes, atLPVOID *lppPtr, atLONG *lpSize );
atLPVOID		atUIAPI_GetResPtr( atHRES hRes );
atVOID 		atUIAPI_FreeRes( atHRES hRes );



typedef atBOOL (*stringResCallbackfn)(atDWORD ids, atTCHAR* buf, atLONG bufSize, atLONG bmsIndex);

atVOID  		atUIAPI_SetStringResCallback( stringResCallbackfn, atINT bmsIndex);
atBOOL 		atUIAPI_LoadString(atDWORD ids, atTCHAR* buf, atLONG bufSize);

#ifdef __cplusplus
}
#endif

#endif

