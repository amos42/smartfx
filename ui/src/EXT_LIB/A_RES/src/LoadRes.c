/*#############################################################################
-------------------------------------------------------------------------------

                                  LoadRes.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules : A_VM, A_Utils
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Resource Loader related function definition
 
-------------------------------------------------------------------------------
#############################################################################*/
#define __IMPLEMENT_

#include "AT_Types.h"
#include "A_RES.h"


/*
//  8비트 데이터를 읽는다.
static atU8 Get8bits( void *Data )
{
    return( *(atU8 *)Data );
}

//  16비트 데이터를 읽는다.
static atU16 Get16bits( void *Data )
{
	atU8 v1, v2;
	
	v1 = *(atU8 *)Data;
	v2 = *(((atU8 *)Data)+1);
	
    return( ((atU16)v1 << 8) | v2 );
}

//  32비트 데이터를 읽는다.
static atU32 Get32bits( void *Data )
{
	atU8 v1, v2, v3, v4;
	
	v1 = *(atU8 *)Data;
	v2 = *(((atU8 *)Data)+1);
	v3 = *(((atU8 *)Data)+2);
	v4 = *(((atU8 *)Data)+3);
	
    return( ((atU32)v1 << 24) | ((atU32)v2 << 16) | ((atU32)v3 << 8) | v4 );
}
*/


/*****************************
  인덱스로 리소스를 로드한다.
*******************************/
atULONG atRES_LoadResourceEx( atHRESOURCE hResource, atINT Index, atRESOURCE *lpResData, atBOOL bForceAlloc )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
    atCHAR *tp, *tp2;
	atRES_FILE_INFO fileinfo;
	atBOOL r;
	int size;

	if( pRes == atNULL ) return 0;

    lpResData->lpResData = atNULL;
    lpResData->nResSize = 0;

    if( pRes->SourceType == atRES_SRC_DIRECTORY ){
		// Not Support
    } else if( pRes->SourceType == atRES_SRC_RESFILE ){
        if( pRes->ResourceFile[0] == '\0' ) return( 0 );  
		r = atRES_GetFileInfo( hResource, Index, &fileinfo );
		if( r == atFALSE ) return 0;

		tp = (atCHAR *)pRes->tFuncs.fnAllocator( fileinfo.CompressedSize + 4); // alloc을 4byte이상 하도록 한다.
		if( tp == atNULL ) return( -3 );
		atRES_ReadROM( hResource, tp, fileinfo.Offset, fileinfo.CompressedSize );
		
        if( fileinfo.CompressMethod == atRES_COMPRESS_LZW ){
		  tp2 = (atCHAR *)pRes->tFuncs.fnAllocator( fileinfo.FileSize + 4); // alloc을 4byte이상 하도록 한다.
          if( tp2 == atNULL ) return( -3 );
          atRES_Decode( tp2, tp, fileinfo.FileSize );
          lpResData->lpResData = tp2;
		  lpResData->nResSize = fileinfo.FileSize;
		  pRes->tFuncs.fnDeallocator( tp );
        } else {
          lpResData->lpResData = tp;
		  lpResData->nResSize = fileinfo.FileSize;
        }
    
		lpResData->nResSize = fileinfo.FileSize;
		lpResData->fnDeallocator = pRes->tFuncs.fnDeallocator;

        size = fileinfo.FileSize;    

		return( size );
    } else if( pRes->SourceType == atRES_SRC_RESSTREAM ){
        if( pRes->ResourcePtr == atNULL ) return( 0 );  
		r = atRES_GetFileInfo( hResource, Index, &fileinfo );
		if( r == atFALSE ) return 0;

        if( fileinfo.CompressMethod == atRES_COMPRESS_LZW ){
		  tp = (atCHAR *)pRes->tFuncs.fnAllocator( fileinfo.FileSize + 4); // alloc을 4byte이상 하도록 한다.
          if( tp == atNULL ) return( -3 );
          atRES_Decode( tp, pRes->ResourcePtr + fileinfo.Offset, fileinfo.FileSize );
          lpResData->lpResData = tp;
		  lpResData->nResSize = fileinfo.FileSize;
		  lpResData->lpDeallocData = pRes->tFuncs.lpData;
		  lpResData->fnDeallocator = pRes->tFuncs.fnDeallocator;
        } else {
		  if(bForceAlloc && (pRes->tFuncs.fnAllocator != atNULL)){
			tp = (atCHAR *)pRes->tFuncs.fnAllocator( fileinfo.FileSize + 4); // alloc을 4byte이상 하도록 한다.
			if( tp == atNULL ) return( -3 );
			lpResData->lpResData = tp;
			lpResData->nResSize = fileinfo.FileSize;
		    lpResData->lpDeallocData = pRes->tFuncs.lpData;
			lpResData->fnDeallocator = pRes->tFuncs.fnDeallocator;
		  } else {
			lpResData->lpResData = (atLPVOID)((atINTPTR)pRes->ResourcePtr + fileinfo.Offset);
			lpResData->nResSize = fileinfo.FileSize;
			lpResData->fnDeallocator = atNULL;
		  }
        }
    
        size = fileinfo.FileSize;

        return( size );
    }

    return( 0 );
}


/*****************************
  인덱스로 리소스를 로드한다.
*******************************/
atULONG atRES_LoadResource( atHRESOURCE hResource, atINT Index, atRESOURCE *lpResData )
{
	return atRES_LoadResourceEx( hResource, Index, lpResData, atFALSE );
}

/*****************************
  RC 기술 순서 리소스를 로드한다.
*******************************/
atULONG atRES_LoadResourceByOrderEx( atHRESOURCE hResource, atINT OrderIndex, atRESOURCE *lpResData, atBOOL bForceAlloc )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
    atCHAR *tp, *tp2;
	atRES_FILE_INFO fileinfo;
	atBOOL r;
	int size;
	int idx;

	if( pRes == atNULL ) return 0;

    lpResData->lpResData = atNULL;
    lpResData->nResSize = 0;

    if( pRes->SourceType == atRES_SRC_DIRECTORY ){
		// Not Support
    } else if( pRes->SourceType == atRES_SRC_RESFILE ){
        if( pRes->ResourceFile[0] == '\0' ) return( 0 );  

		idx = atRES_FindFileByOrder( hResource, OrderIndex );
		if( idx < 0 ) return 0;

		return atRES_LoadResourceEx( hResource, idx, lpResData, bForceAlloc );
    } else if( pRes->SourceType == atRES_SRC_RESSTREAM ){
        if( pRes->ResourcePtr == atNULL ) return( 0 );  

		idx = atRES_FindFileByOrder( hResource, OrderIndex );
		if( idx < 0 ) return 0;

		return atRES_LoadResourceEx( hResource, idx, lpResData, bForceAlloc );
    }

    return( 0 );
}


atULONG atRES_LoadResourceByOrder( atHRESOURCE hResource, atINT OrderIndex, atRESOURCE *lpResData )
{
	return atRES_LoadResourceByOrderEx( hResource, OrderIndex, lpResData, atFALSE );
}


/*****************************
  ID로 리소스를 로드한다.
*******************************/
atULONG atRES_LoadResourceByIDEx( atHRESOURCE hResource, atDWORD ID, atRESOURCE *lpResData, atBOOL bForceAlloc )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
    atCHAR *tp, *tp2;
	atRES_FILE_INFO fileinfo;
	atBOOL r;
	int size;
	int idx;

	if( pRes == atNULL ) return 0;

    lpResData->lpResData = atNULL;
    lpResData->nResSize = 0;

    if( pRes->SourceType == atRES_SRC_DIRECTORY ){
		// Not Support
    } else if( pRes->SourceType == atRES_SRC_RESFILE ){
        if( pRes->ResourceFile[0] == '\0' ) return( 0 );  

		idx = atRES_FindFileByID( hResource, ID );
		if( idx < 0 ) return 0;

		return atRES_LoadResourceEx( hResource, idx, lpResData, bForceAlloc );
    } else if( pRes->SourceType == atRES_SRC_RESSTREAM ){
        if( pRes->ResourcePtr == atNULL ) return( 0 );  

		idx = atRES_FindFileByID( hResource, ID );
		if( idx < 0 ) return 0;

		return atRES_LoadResourceEx( hResource, idx, lpResData, bForceAlloc );
    }

    return 0;
}


atULONG atRES_LoadResourceByID( atHRESOURCE hResource, atDWORD ID, atRESOURCE *lpResData )
{
	return atRES_LoadResourceByIDEx( hResource, ID, lpResData, atFALSE );
}


/*********************
  파일명으로 리소스를 로드한다.
**********************/
atULONG atRES_LoadResourceByNameEx( atHRESOURCE hResource, const atLPSTR szFileName, atRESOURCE *lpResData, atBOOL bForceAlloc )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
    atCHAR *tp, *tp2;
	atRES_FILE_INFO fileinfo;
	atBOOL r;
	int size;
	int idx;

	if( pRes == atNULL ) return 0;

    lpResData->lpResData = atNULL;
    lpResData->nResSize = 0;

    if( pRes->SourceType == atRES_SRC_DIRECTORY ){
/*
		pRes->tFuncs.fnLoadFile(filename);

        *DesPtr = data;
        
        return( size );
*/
    } else if( pRes->SourceType == atRES_SRC_RESFILE ){
        if( pRes->ResourceFile[0] == '\0' ) return( 0 );  

		idx = atRES_FindFileByName( hResource, szFileName );
		if( idx < 0 ) return 0;

		return atRES_LoadResourceEx( hResource, idx, lpResData, bForceAlloc );
    } else if( pRes->SourceType == atRES_SRC_RESSTREAM ){
        if( pRes->ResourcePtr == atNULL ) return( 0 );  

		idx = atRES_FindFileByName( hResource, szFileName );
		if( idx < 0 ) return 0;

		return atRES_LoadResourceEx( hResource, idx, lpResData, bForceAlloc );
    }

    return( 0 );
}


atULONG atRES_LoadResourceByName( atHRESOURCE hResource, const atLPSTR szFileName, atRESOURCE *lpResData )
{
	return atRES_LoadResourceByNameEx( hResource, szFileName, lpResData, atFALSE );
}


/*********************
  리소스를 소멸시킨다.
**********************/
void atRES_FreeResource( atRESOURCE *lpResData )
{
	if( lpResData == atNULL ) return;
	if( lpResData->lpResData == atNULL ) return;

	if(lpResData->fnDeallocator != atNULL)
		lpResData->fnDeallocator( lpResData->lpResData );
    lpResData->lpResData = atNULL;
    lpResData->nResSize = 0;
}

