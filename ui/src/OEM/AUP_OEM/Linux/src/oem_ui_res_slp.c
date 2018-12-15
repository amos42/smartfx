#include "A_UIENG.h"
#include "UI_API/UI_API.h"
#include "OEM_UI_Res.h"
#include "A_Res.h"
#include <string.h>


static atRESFUNCS g_lstFuncs;


atBOOL	OEM_UI_InitRes(atLONG lSysInst, atLONG lInitData1, atLONG lInitData2)
{
	//atBOOL r = atFALSE;
	//atRESFUNCS lstFuncs;

	g_lstFuncs.lpData = atNULL;
	g_lstFuncs.fnAllocator = atUIAPI_AllocMem;
	g_lstFuncs.fnDeallocator = (atLPCALLBACKFREE)atUIAPI_FreeMem;
	g_lstFuncs.fnStrNCopy = atUIAPI_strncpy;
	g_lstFuncs.fnLoadFile = atUIAPI_LoadFile;

	{
		long *v = (long *)lInitData2;
		if(v != atNULL){
			atHRESSRC hResSrc = atUIAPI_AddResSource((atLPTSTR)lInitData1, atUI_RES_SRC_RESSTREAM, (const atLPVOID)v[0], v[1]);
			atUIAPI_SetDefaultResSource(hResSrc);
		}
	}

	return atTRUE;
}

void	OEM_UI_FinalRes(void)
{
}

OEM_RESSRC	OEM_UI_CreateResSource( atLPTSTR szIDStr, int type, void *ptr, long size, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator )
{
	atHRESOURCE hRes;

	hRes = atRES_CreateResource( ptr, size, type, &g_lstFuncs );
	if( hRes == atNULL ) return atNULL;

	return (OEM_RESSRC)hRes;
}

atBOOL	OEM_UI_RemoveResSource( OEM_RESSRC hOemResSrc )
{
	atRES_DestroyResource( hOemResSrc );
	return atTRUE;
}

atBOOL OEM_UI_GetResSourceInfo(OEM_RESSRC hOemResSrc, atUIAPI_RESSRC_INFO *lpResSrcInfo)
{
	atRES_RESOURCE_INFO resinfo;
	atBOOL ret = atRES_GetResourceInfo((atHRESOURCE)hOemResSrc, &resinfo);
	if (!ret) return atFALSE;

	if (lpResSrcInfo != atNULL) {
		lpResSrcInfo->type = resinfo.SourceType;
		lpResSrcInfo->lpPtr = resinfo.ResourcePtr;
		lpResSrcInfo->size = resinfo.ResourceSize;
	}

	return atTRUE;
}

atBOOL	OEM_UI_SetResAllocator( OEM_RESSRC hOemResSrc, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator )
{
/*
	return atRES_SetAllocator( (atHRESOURCE)hOemResSrc, fnAllocator, fnDeallocator );			
*/

	return atFALSE;
}

atBOOL	OEM_UI_GetResAllocator( OEM_RESSRC hOemResSrc, atLPCALLBACKALLOC *lpfnAllocator, atLPCALLBACKFREE *lpfnDeallocator )
{
	if( hOemResSrc == atNULL ){
		if( lpfnAllocator ) *lpfnAllocator = atNULL;
		if( lpfnDeallocator ) *lpfnDeallocator = atNULL;
		return atFALSE;
	}
	
	atRES_GetAllocator( (atHRESOURCE)hOemResSrc, lpfnAllocator, lpfnDeallocator );			

	return atTRUE;
}

atBOOL OEM_UI_IsExistRes( OEM_RESSRC hOemResSrc, atLPTSTR pFileName )
{
	atANSI_CHAR filename[128];
	atUIAPI_ToANSI( filename, pFileName, -1 );	
	return (atRES_FindFileByName( (atHRESOURCE)hOemResSrc, filename ) >= 0);
}

atLONG OEM_UI_GetResInfo(OEM_RESSRC hOemResSrc, atLPTSTR szFileName, atUIAPI_RES_INFO *lpResInfo)
{
	atANSI_CHAR filename[128];
	int idx;
	atRES_FILE_INFO fi;

	if (atUIAPI_ToANSI(filename, szFileName, -1) == 0) return 0;

	idx = atRES_FindFileByName((atHRESOURCE)hOemResSrc, filename);
	if (idx < 0) return 0;

	if (!atRES_GetFileInfo((atHRESOURCE)hOemResSrc, idx, &fi)) {
		return 0;
	}

	if (lpResInfo != atNULL) {
		lpResInfo->nStreamOffset = fi.Offset;
		lpResInfo->nFileSize = fi.FileSize;
		lpResInfo->nCompressMethod = (fi.CompressMethod == atRES_COMPRESS_LZW) ? 1 : 0;
		lpResInfo->nCompressedSize = fi.CompressedSize;
	}

	return fi.FileSize;
}

OEM_RES	OEM_UI_LoadRes( OEM_RESSRC hOemResSrc, atLPTSTR szFileName )
{
	atRESOURCE *lpRes;
	atANSI_CHAR filename[128];
	if (atUIAPI_ToANSI( filename, szFileName, -1 ) == 0) return 0;
	lpRes = atUIAPI_AllocMem( sizeof(atRESOURCE) );
	if( atRES_LoadResourceByName( (atHRESOURCE)hOemResSrc, filename, lpRes ) <= 0 ){
		atUIAPI_FreeMem( lpRes );
		return atNULL;
	}
	return (OEM_RES)lpRes;
}

atBOOL OEM_UI_GetResData( OEM_RES hOemRes, atLPVOID *lppPtr, atLONG *lpSize )
{
	if( lppPtr ) *lppPtr = ((atRESOURCE *)hOemRes)->lpResData;
	if( lpSize ) *lpSize = ((atRESOURCE *)hOemRes)->nResSize;

	return atTRUE;
}

atVOID OEM_UI_FreeRes( OEM_RES hOemRes )
{
    atRES_FreeResource( (atRESOURCE *)hOemRes );
    atUIAPI_FreeMem( hOemRes );
}

