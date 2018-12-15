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
#ifndef __RESHDR1_H_
#define __RESHDR1_H_


#include "AT_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


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
} atRES_FILE_INFO_VER1;

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
} RESOURCE_HEADER_VER1;

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
} RES_FILE_HEADER_VER1;


#ifdef __cplusplus
}
#endif


#endif
