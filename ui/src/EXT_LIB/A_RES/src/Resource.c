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
#include "StrFunc.h"


/***************************
  리소스를 생성한다.
****************************/
atHRESOURCE atRES_CreateResource( const atLPVOID DataPtr, atLONG DataSize, atINT SrcType, atRESFUNCS *lpFuncs )
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
		tRes.tFuncs.fnMemSet = (lpFuncs->fnMemSet != atNULL) ? lpFuncs->fnMemSet : _memset_x;
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
  리소스를 삭제한다.
****************************/
atVOID atRES_DestroyResource( atHRESOURCE hResource )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
	if( pRes == atNULL ) return;

    if( pRes->lpFileHdr_Cache ) pRes->tFuncs.fnDeallocator( pRes->lpFileHdr_Cache );

	pRes->tFuncs.fnDeallocator( pRes );
}


/***************************
리소스 정보를 얻는다.
****************************/
atBOOL atRES_GetResourceInfo(atHRESOURCE hResource, atRES_RESOURCE_INFO *lpResInfo)
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
	if (pRes == atNULL) return atFALSE;

	*lpResInfo = *pRes;

	return atTRUE;
}


/***************************
  리소스의 할당/해제 함수를 지정한다.
****************************/
/* 중간에 allocator/deallocator가 바뀌기 때문에, 이전 할당된 것을 재할당 해야 함. 복잡해서 일단 다음에 구현할 예정.
atVOID atRES_SetAllocator( atHRESOURCE hResource, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator )
{
	atLPRESOURCEMNG lpResourceMng = atNULL;
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
	if( pRes == atNULL ) return;

	if( lpResourceMng == atNULL ) lpResourceMng = g_lpResourceMng; // 편법

	if( fnAllocator != atNULL )
		pRes->fnAllocator = fnAllocator;
	else
		pRes->fnAllocator = lpResourceMng->lstFuncs.fnAllocMem;
	if( fnDeallocator != atNULL )
		pRes->fnDeallocator = fnDeallocator;
	else
		pRes->fnDeallocator = lpResourceMng->lstFuncs.fnFreeMem;
}
*/

/***************************
  리소스의 할당/해제 함수를 얻는다.
****************************/
atVOID atRES_GetAllocator( atHRESOURCE hResource, atLPCALLBACKALLOC *lpfnAllocator, atLPCALLBACKFREE *lpfnDeallocator )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
	if( pRes == atNULL ) return;

	if( lpfnAllocator ) *lpfnAllocator = pRes->tFuncs.fnAllocator;
	if( lpfnDeallocator ) *lpfnDeallocator = pRes->tFuncs.fnDeallocator;
}


/***************************
  리소스의 메모리 할당 함수
****************************/
atLPVOID atRES_AllocMem( atHRESOURCE hResource, atLONG nSize )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
	if( pRes == atNULL ) return atNULL;

	if( pRes->tFuncs.fnAllocator == atNULL ) return atNULL;

	return pRes->tFuncs.fnAllocator( nSize );
}


/***************************
  리소스의 메모리 해제 함수
****************************/
atVOID atRES_FreeMem( atHRESOURCE hResource, atLPVOID lpPtr )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
	if( pRes == atNULL ) return;

	if( pRes->tFuncs.fnDeallocator == atNULL ) return;

	pRes->tFuncs.fnDeallocator( lpPtr );
}


/***************************
  리소스의 소스를 지정한다.
****************************/
atBOOL atRES_SetResourceSource( atHRESOURCE hResource, atLPVOID DataPtr, atLONG DataSize, atINT SrcType )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
	RESOURCE_HEADER rh;

	if( pRes == atNULL ) return atFALSE;

	pRes->tFuncs.fnMemSet(&rh, 0, sizeof(rh));
	//SYSAPI_DebugPrintf(_AT("atRES_SetResourceSource(dataSize=%d,srcType=%d)\n"), DataSize, SrcType);

    pRes->SourceType = SrcType;
    pRes->ResourceSize = DataSize;

    pRes->ResourcePtr = atNULL;
    pRes->tFuncs.fnMemSet( pRes->ResourceFile, 0, sizeof(pRes->ResourceFile) );
   
    switch( SrcType ){
      case atRES_SRC_DIRECTORY : pRes->tFuncs.fnStrNCopy( pRes->ResourceFile, (atLPTSTR)DataPtr, 256 ); break;
      case atRES_SRC_RESFILE   : pRes->tFuncs.fnStrNCopy( pRes->ResourceFile, (atLPTSTR)DataPtr, 256 ); break;
      case atRES_SRC_RESSTREAM : pRes->ResourcePtr = DataPtr; break;
    }
	
    if(atRES_ReadROM( hResource, &rh, 0, sizeof(RESOURCE_HEADER) ) == 0)
    {
    	SYSAPI_DebugPrintf(_AT("atRES_ReadROM failed!!\n"));
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
atBOOL atRES_PrepareResourceHeader( atHRESOURCE hResource )
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
atINT  atRES_GetFileCount( atHRESOURCE hResource )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
	if( pRes == atNULL ) return 0;

    return pRes->FileCount;
}


/***************************
 
****************************/
atLONG  atRES_GetFATOffset( atHRESOURCE hResource )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
	if( pRes == atNULL ) return 0;

    return pRes->FATOffset;
}


/***************************
 
****************************/
atLONG  atRES_GetBaseOffset( atHRESOURCE hResource )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
	if( pRes == atNULL ) return 0;

    return pRes->BaseOffset;
}


/********************
  리소스를 읽어온다.
*********************/
atLONG atRES_ReadROM( atHRESOURCE hResource, atLPVOID DesPtr, atLONG Addr, atLONG Size )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
    int i;
    char *src, *des;
    long length;

	if( pRes == atNULL ) return 0;

    switch( pRes->SourceType ){
        case atRES_SRC_DIRECTORY : 
		    break;
        case atRES_SRC_RESFILE   : 
			// check-me
			if(pRes->tFuncs.fnLoadFile( pRes->ResourceFile, DesPtr, Size, Addr ) == 0) return 0;
		    break;
        case atRES_SRC_RESSTREAM :
			src = (char *)pRes->ResourcePtr + Addr;
			des = (char *)DesPtr;

			if( ((atDWORD)(atINTPTR)src & 0x3) == ((atDWORD)(atINTPTR)des & 0x3) ){
			  // 시작 주소가 워드 정렬 되어 있지 
			  length = sizeof(atDWORD) - (atDWORD)(atINTPTR)src & 0x3;
			  for( i = 0; i < length; i ++ )
				*des++ = *src++;
			  Size -= length;

			  length = Size >> 2;
			  for( i = 0; i < length; i ++ ){
				*(long *)des = *(long *)src;
				des += 4; src += 4;
			  }

			  length = Size & 0x3;
			  for( i = 0; i < length; i ++ )
				*des++ = *src++;
			} else {
			  for( i = 0; i < Size; i ++ )
				*des++ = *src++;
			}
			break;
	}

	return Size;
}


/****************************

*****************************/
atBOOL atRES_GetFileInfo( atHRESOURCE hResource, atINT Index, atRES_FILE_INFO *lpFileInfo )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
    atINT  cnt;
    RES_FILE_HEADER *fh, *tfh;

	if( pRes == atNULL ) return atFALSE;
	if( pRes->lpFileHdr_Cache == atNULL ) return atFALSE;

    cnt = atRES_GetFileCount(hResource);
    if( Index >= cnt ) return atFALSE;
	
    fh = pRes->lpFileHdr_Cache;
    tfh = &fh[Index];
    
    if( lpFileInfo ){
		lpFileInfo->ID = tfh->ID;
	  	pRes->tFuncs.fnAnsiStrNCopy( (char*)lpFileInfo->FileName, (atLPCSTR)tfh->FileName, 40 );
		lpFileInfo->FileSize = tfh->FileOrgSize;
		lpFileInfo->Index = tfh->Index;
		lpFileInfo->Offset = atRES_GetBaseOffset(hResource) + tfh->Offset;
		lpFileInfo->CompressMethod = tfh->CompressMethod;
		lpFileInfo->Dummy1 = 0;
		lpFileInfo->Dummy2 = 0;
		lpFileInfo->Dummy3 = 0;
		lpFileInfo->CompressedSize = tfh->FileSize;
    }

    return atTRUE;
}
