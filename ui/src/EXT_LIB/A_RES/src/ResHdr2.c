/*#############################################################################
-------------------------------------------------------------------------------

                                   Resource.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules : A_VM, A_Utils
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Resource Manager related function definition
 
-------------------------------------------------------------------------------
#############################################################################*/
#define __IMPLEMENT_

#include "AT_Types.h"
#include "A_RES.h"
#include "ResHdr2.h"
#include "StrFunc.h"


/***************************
  리소스를 생성한다.
****************************/
atHRESOURCE atRES_CreateResource_ver2( const atLPVOID DataPtr, atLONG DataSize, atINT SrcType, atRESFUNCS *lpFuncs )
{
	atRES_RESOURCE_INFO tRes;
	atRES_RESOURCE_INFO *pRes;

	memset( &tRes, 0, sizeof(atRES_RESOURCE_INFO) );
/*
	pRes->SourceType = atRES_SRC_NONE;
	pRes->ResourceSize = 0;
	pRes->ResourcePtr = atNULL;
	memset( pRes->ResourceFile, 0, sizeof(pRes->ResourceFile) );
	
    pRes->FileCount = 0;
	pRes->FATOffset = 0;
	pRes->BaseOffset = 0;

	pRes->lpFileHdr_Cache = atNULL;
*/
	if( lpFuncs != atNULL ) {
		tRes.tFuncs.lpData = lpFuncs->lpData;
		tRes.tFuncs.fnAllocator = lpFuncs->fnAllocator;
		tRes.tFuncs.fnDeallocator = lpFuncs->fnDeallocator;
		tRes.tFuncs.fnAnsiStrComp = (lpFuncs->fnAnsiStrComp != atNULL) ? lpFuncs->fnAnsiStrComp : _strcmp_x;
		tRes.tFuncs.fnAnsiStrNCopy = (lpFuncs->fnAnsiStrNCopy != atNULL) ? lpFuncs->fnAnsiStrNCopy : _strncpy_x;
		tRes.tFuncs.fnAnsiStrUpper = (lpFuncs->fnAnsiStrUpper != atNULL) ? lpFuncs->fnAnsiStrUpper : _strupr_x;
		tRes.tFuncs.fnStrNCopy = (lpFuncs->fnStrNCopy != atNULL) ? lpFuncs->fnStrNCopy : _tcsncpy_x;
		tRes.tFuncs.fnLoadFile = lpFuncs->fnLoadFile;
	}

	if( !atRES_SetResourceSource( (atHRESOURCE)&tRes, DataPtr, DataSize, SrcType ) ) return atNULL;

//	if( lpResourceMng == atNULL ) return atNULL;
	pRes = (atRES_RESOURCE_INFO *)atRES_AllocMem( &tRes, sizeof(atRES_RESOURCE_INFO) );
	if( pRes == atNULL ){
		return atNULL;
	}

	*pRes = tRes;

	atRES_PrepareResourceHeader( (atHRESOURCE)pRes );

	return (atHRESOURCE)pRes;
}


/***************************
리소스 정보를 얻는다.
****************************/
atBOOL atRES_GetResourceInfo_ver2(atHRESOURCE hResource, atRES_RESOURCE_INFO *lpResInfo)
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
	if (pRes == atNULL) return atFALSE;

	*lpResInfo = *pRes;

	return atTRUE;
}


/***************************
  리소스의 소스를 지정한다.
****************************/
atBOOL atRES_SetResourceSource_ver2( atHRESOURCE hResource, atLPVOID DataPtr, atLONG DataSize, atINT SrcType )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
	RESOURCE_HEADER rh;

	if( pRes == atNULL ) return atFALSE;

	memset(&rh, 0, sizeof(rh));
	//SYSAPI_DebugPrintf(_AT("atRES_SetResourceSource(dataSize=%d,srcType=%d)\n"), DataSize, SrcType);

    pRes->SourceType = SrcType;
    pRes->ResourceSize = DataSize;

    pRes->ResourcePtr = atNULL;
    memset( pRes->ResourceFile, 0, sizeof(pRes->ResourceFile) );
   
    switch( SrcType ){
      case atRES_SRC_DIRECTORY : pRes->tFuncs.fnStrNCopy( pRes->ResourceFile, (atLPTSTR)DataPtr, 256 ); break;
      case atRES_SRC_RESFILE   : pRes->tFuncs.fnStrNCopy( pRes->ResourceFile, (atLPTSTR)DataPtr, 256 ); break;
      case atRES_SRC_RESSTREAM : pRes->ResourcePtr = DataPtr; break;
    }
	
	// check-me
    if(atRES_ReadROM( hResource, &rh, 0, sizeof(RESOURCE_HEADER) ) == 0)
    {
    	//SYSAPI_DebugPrintf(_AT("atRES_ReadROM failed!!\n"));
    	return atFALSE;
    }	

	if( rh.Version == 0x01 ){
      pRes->SourceType = atRES_SRC_NONE;
   	  return atFALSE;
	}

    pRes->FileCount = rh.FileCount;
    pRes->FATOffset = sizeof(RESOURCE_HEADER);
    pRes->BaseOffset = sizeof(RESOURCE_HEADER) + (rh.FileCount * sizeof(RES_FILE_HEADER));
    pRes->lpFileHdr_Cache = atNULL;
   
    return( atTRUE );
}


/***************************
  리소스의 헤더 캐쉬를 갱신한다.
****************************/
atBOOL atRES_PrepareResourceHeader_ver2( atHRESOURCE hResource )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
	if( pRes == atNULL ) return atFALSE;

	if( pRes->lpFileHdr_Cache != atNULL ) return atFALSE;

    // 미리 FAT 테이블을 읽어 놓는다.
	pRes->lpFileHdr_Cache = (RES_FILE_HEADER *)atRES_AllocMem( hResource, sizeof(RES_FILE_HEADER) *  pRes->FileCount );
    atRES_ReadROM( hResource, pRes->lpFileHdr_Cache, sizeof(RESOURCE_HEADER), sizeof(RES_FILE_HEADER) *  pRes->FileCount );

	return atTRUE;
}


/***************************
 
****************************/
atINT  atRES_GetFileCount_ver2( atHRESOURCE hResource )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
	if( pRes == atNULL ) return 0;

    return pRes->FileCount;
}


/***************************
 
****************************/
atLONG  atRES_GetFATOffset_ver2( atHRESOURCE hResource )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
	if( pRes == atNULL ) return 0;

    return pRes->FATOffset;
}


/***************************
 
****************************/
atLONG  atRES_GetBaseOffset_ver2( atHRESOURCE hResource )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
	if( pRes == atNULL ) return 0;

    return pRes->BaseOffset;
}
