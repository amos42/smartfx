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
//#include <stdlib.h>
//#include <string.h>



//extern FILE_HEADER *lpFileHdr_Cache;


/****************************
  Binary Search로 해당 파일을 찾는다.
	(정방향 소트된 것 대상)
 ※ WinCE에는 이 함수가 없어서..
*****************************/
static RES_FILE_HEADER * _bsearch( const atANSI_CHAR *key, const RES_FILE_HEADER *base, atINT n_elem, atINT (*fnAnsiStrComp)( atLPCSTR, atLPCSTR ) )
{
	RES_FILE_HEADER *kmin, *probe;
    atINT i, j;

    kmin = (RES_FILE_HEADER *)base;
    while( n_elem > 0 ){
      i = n_elem >> 1; // nelem / 2
      probe = kmin + i;
      j = fnAnsiStrComp( (atLPCSTR)key, (atLPCSTR)probe->FileName );
      if( j == 0 )
        return probe;
      else if( j < 0 )
        n_elem = i;
      else {
        kmin = probe + 1;
        n_elem -= i + 1;
      }
    }

    return atNULL;
}

/****************************
  bsearch용
*****************************/
/*
static atINT cmpfunc( const void *a, const void *b )
{
    return( strncmp( (atCHAR *)a, ((FILE_HEADER *)b)->FileName, 40 ) );
}
*/


/****************************

*****************************/
atINT atRES_FindFileByOrder( atHRESOURCE hResource, atINT OrderIndex )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
    atINT   i, cnt;
    RES_FILE_HEADER *fh;
	int idx;

	if( pRes == atNULL ) return -1;

    fh = (RES_FILE_HEADER *)pRes->lpFileHdr_Cache;
    idx = -1;

    cnt = atRES_GetFileCount(hResource);
	
    for( i = 0; i < cnt; i ++ ){
      if( fh->Index == OrderIndex ){
        idx = i;
        break;
      }
      fh ++;
    }

    return idx;
}


/****************************

*****************************/
atINT atRES_FindFileByID( atHRESOURCE hResource, atDWORD ID )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
    atINT   i, cnt;
    RES_FILE_HEADER *fh;
	int idx;

	if( pRes == atNULL ) return -1;

    fh = pRes->lpFileHdr_Cache;
    idx = -1;

    cnt = atRES_GetFileCount(hResource);
	
    for( i = 0; i < cnt; i ++ ){
      if( fh->ID == ID ){
        idx= i;
        break;
      }
      fh ++;
    }

    return idx;
}


/****************************

*****************************/
atINT atRES_FindFileByName( atHRESOURCE hResource, const atLPSTR szFileName )
{
	atRES_RESOURCE_INFO *pRes = (atRES_RESOURCE_INFO *)hResource;
    atINT cnt;
	RES_FILE_HEADER *rfh, *tfh;
	atANSI_CHAR filename[256 + 1 + 3];
	
	if( pRes == atNULL ) return -1;

    cnt = atRES_GetFileCount(hResource);
	
	pRes->tFuncs.fnAnsiStrNCopy( (char*)filename, (atLPCSTR)szFileName, 40 );
	pRes->tFuncs.fnAnsiStrUpper( filename );
	
	rfh = pRes->lpFileHdr_Cache;

	tfh = _bsearch( filename, rfh, cnt, pRes->tFuncs.fnAnsiStrComp );
    if( tfh == atNULL ) return -1;

	return (tfh - rfh);// / sizeof(RES_FILE_HEADER);
}


