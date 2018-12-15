/*#############################################################################
-------------------------------------------------------------------------------

                                  A_Res.h
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules : A_VM, A_Utils
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Resource related function definition

-------------------------------------------------------------------------------
#############################################################################*/
#ifndef __A_RES_H_
#define __A_RES_H_


#include "AT_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atHANDLE atHRESOURCE;


// Resource Source
#define atRES_SRC_NONE              0       // Not Support
#define atRES_SRC_DIRECTORY         1       // Not Support
#define atRES_SRC_RESFILE           2
#define atRES_SRC_RESSTREAM         3
#define atRES_SRC_USERDEFINE        0x1000  // Not Support

// Resource Compress Type
#define atRES_COMPRESS_NONE         0
#define atRES_COMPRESS_LZW          1
#define atRES_COMPRESS_RLE          2       // Not Support
#define atRES_COMPRESS_JPEG         3       // Not Support


/*
*/
typedef struct _tagatRESFUNCS {
    atLPVOID                lpData;
    atLPCALLBACKALLOC       fnAllocator;
    atLPCALLBACKFREE        fnDeallocator;
    atVOID                  (*fnMemSet)( atLPVOID, atCHAR, atLONG );
    atINT                   (*fnAnsiStrComp)( atLPSTR, atLPCSTR );
    atLPSTR                 (*fnAnsiStrNCopy)( atLPSTR, atLPCSTR, atLONG );
    atLPSTR                 (*fnAnsiStrUpper)( atLPSTR );
    atLPTSTR                (*fnStrNCopy)( atLPTSTR, atLPCTSTR, atLONG );
    atLONG                  (*fnLoadFile)( atLPTSTR, atLPVOID, atLONG, atLONG );
} atRESFUNCS;

typedef struct {
    atDWORD   	ResourceID;
    atINT     	SourceType;
    atLONG    	ResourceSize;
    atCHAR    *	ResourcePtr;
    atTCHAR   	ResourceFile[256];
    atLONG    	FileCount;
    atLONG    	FATOffset;
    atLONG    	BaseOffset;
	atRESFUNCS  tFuncs;
    void      * lpFileHdr_Cache;
} atRES_RESOURCE_INFO;

typedef struct {
    atDWORD   ID;
//    atTCHAR   FileName[40];       // 파일 명                
    atANSI_CHAR	FileName[40];       // 파일 명 (ANSI 문자열만 허용)                
    atLONG    	FileSize;           // 압축하기 전의 사이즈   
    atLONG    	Index;              // Resource Index         
    atLONG    	Offset;             // 헤더를 포함한 오프셋         
	atBYTE      CompressMethod;     // 압축 방식 (atRES_COMPRESS_NONE | atRES_COMPRESS_LZW | ...)
	atBYTE     	Dummy1;             // 예비              
    atBYTE     	Dummy2;             // 예비              
    atBYTE     	Dummy3;             // 예비              
    atLONG    	CompressedSize;     // 파일 사이즈            
} atRES_FILE_INFO;

#ifdef __IMPLEMENT_

// Resource Attribute
#define atRES_ATTR_NONE             0x0000
#define atRES_ATTR_REORDERED        0x0001
#define atRES_ATTR_EMB_NAME_INDEX   0x0002
#define atRES_ATTR_EMB_ORDER_INDEX  0x0003

typedef struct {
    atUCHAR     	Header[20];         // Resource Header 
    atULONG     	Version;            // Resource 버전
    atULONG     	FileCount;          // 총 파일 갯수
    atULONG     	ResourceSize;       // 리소스의 크기
    atULONG     	Attr;				// 리소스의 속성
    atULONG     	IndexerCount;		// 포함한 인덱스의 갯수
    atBYTE      	Reserved[64-(20+4+4+4+4+4)];
} RESOURCE_HEADER;

typedef struct {
    atUCHAR     	TableName[8];       // Table Name
    atULONG     	TableSize;          // Table의 크기
    atBYTE      	Reserved[32-(8+4)];
} RESOURCE_INDEXTBL_HEADER;

typedef struct {
    atLONG      	Index;              // Resource Index         
    atANSI_CHAR     FileName[40];       // 파일 명 (ANSI 문자열만 허용)
    atULONG     	Offset;             // 헤더를 제외한 옵셋     
    atBYTE      	CompressMethod;     // 압축 방식 (atRES_COMPRESS_NONE | atRES_COMPRESS_LZW | ...)
    atBYTE      	Dummy1;             // 예비              
    atBYTE      	Dummy2;             // 예비              
    atBYTE      	Dummy3;             // 예비              
    atULONG     	FileSize;           // 파일 사이즈            
    atULONG     	FileOrgSize;        // 압축하기 전의 사이즈   
    atDWORD     	ID;
//    atBYTE    	Reserved[64 - (4+40+4+4+4+4+4)];  // 64를 채워서 뺐음.
} RES_FILE_HEADER;

#endif  // __IMPLEMENT_


typedef struct {
    atLONG                  nResSize;
    atLPVOID                lpResData;

    atLPVOID                lpDeallocData;
    atLPCALLBACKFREE        fnDeallocator;
} atRESOURCE;


//extern atRES_RESOURCE_INFO g_ResourceInfo;

atULONG        atRES_Encode( atCHAR *DesData, atCHAR *SrcData, atULONG DataSize );
atBOOL         atRES_Decode( atCHAR *DesData, atCHAR *SrcData, atULONG DataSize );

atHRESOURCE    atRES_CreateResource(  const atLPVOID DataPtr, atLONG DataSize, atINT SrcType, atRESFUNCS *lpFuncs );
atVOID         atRES_DestroyResource( atHRESOURCE hResource );
atBOOL         atRES_GetResourceInfo( atHRESOURCE hResource, atRES_RESOURCE_INFO *lpResInfo );
//atVOID         atRES_SetAllocator( atHRESOURCE hResource, atLPCALLBACKALLOC fnAllocator, atLPCALLBACKFREE fnDeallocator );
atVOID         atRES_GetAllocator( atHRESOURCE hResource, atLPCALLBACKALLOC *lpfnAllocator, atLPCALLBACKFREE *lpfnDeallocator );
atLPVOID       atRES_AllocMem( atHRESOURCE hResource, atLONG nSize );
atVOID         atRES_FreeMem( atHRESOURCE hResource, atLPVOID lpPtr );

atBOOL         atRES_SetResourceSource( atHRESOURCE hResource, atLPVOID DataPtr, atLONG DataSize, atINT SrcType );
atBOOL         atRES_PrepareResourceHeader( atHRESOURCE hResource );
atLONG         atRES_ReadROM( atHRESOURCE hResource, atLPVOID DesPtr, atLONG Addr, atLONG Size );

atINT          atRES_GetFileCount( atHRESOURCE hResource );
atLONG         atRES_GetFATOffset( atHRESOURCE hResource );
atLONG         atRES_GetBaseOffset( atHRESOURCE hResource );
atBOOL         atRES_GetFileInfo( atHRESOURCE hResource, atINT Index, atRES_FILE_INFO *lpFileInfo );

atINT          atRES_FindFileByOrder( atHRESOURCE hResource, atINT OrderIndex );
atINT          atRES_FindFileByID( atHRESOURCE hResource, atDWORD ID );
atINT          atRES_FindFileByName( atHRESOURCE hResource, const atLPSTR szFileName );

atULONG        atRES_LoadResource( atHRESOURCE hResource, atINT Index, atRESOURCE *lpResData );
atULONG        atRES_LoadResourceEx( atHRESOURCE hResource, atINT Index, atRESOURCE *lpResData, atBOOL bForceAlloc );
atULONG        atRES_LoadResourceByOrder( atHRESOURCE hResource, atINT OrderIndex, atRESOURCE *lpResData );
atULONG        atRES_LoadResourceByOrderEx( atHRESOURCE hResource, atINT OrderIndex, atRESOURCE *lpResData, atBOOL bForceAlloc );
atULONG        atRES_LoadResourceByID( atHRESOURCE hResource, atDWORD ID, atRESOURCE *lpResData );
atULONG        atRES_LoadResourceByIDEx( atHRESOURCE hResource, atDWORD ID, atRESOURCE *lpResData, atBOOL bForceAlloc );
atULONG        atRES_LoadResourceByName( atHRESOURCE hResource, const atLPSTR szFileName, atRESOURCE *lpResData );
atULONG        atRES_LoadResourceByNameEx( atHRESOURCE hResource, const atLPSTR szFileName, atRESOURCE *lpResData, atBOOL bForceAlloc );

void           atRES_FreeResource( atRESOURCE *lpResData );


#ifdef __cplusplus
}
#endif


#endif
