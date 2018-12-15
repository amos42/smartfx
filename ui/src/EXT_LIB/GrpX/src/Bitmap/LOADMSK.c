/*#############################################################################
-------------------------------------------------------------------------------

                                  LOADIMG.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Loading Image pool
-------------------------------------------------------------------------------
#############################################################################*/
#include "GrpX.h"
#include <string.h>


typedef struct {
    char    Header[20];
    long    Ver;
    long    BPP;
    short   Width, Height;
    char    Reserved[64-(20+4+4+2*2)];
} MSK_HEADER;


/**************************************
  리소스로부터 이미지를 로드한다.
  8비트 리소스로부터 16비트 이미지 생성
***************************************/
atLPBYTE  atGRPX_CreateMaskFromStream( atLPGRPXMNG lpGrpX, const atCHAR *lpStream )
{
    char *res;
    MSK_HEADER *ih;
    int sz;
    atLPBYTE ptr;

    if( lpGrpX == atNULL ) return atNULL;
    if( lpStream == atNULL ) return atNULL;
    res = (char *)lpStream;

    ih = (MSK_HEADER *)res;
    res += sizeof(MSK_HEADER);
    if( strcmp( ih->Header, "Amos Image Mask;" ) ) return atNULL;

#ifdef __BIG_ENDIAN_
    REVERSE( ih->Ver );
    REVERSE( ih->BPP );
    REVERSE( ih->Width );
    REVERSE( ih->Height );
#endif

    if( ih->Ver != 1 ) return atNULL;

    sz = ih->Width * ih->Height * ih->BPP / 8;
    ptr = (atLPBYTE)atGRPX_AllocMem( lpGrpX, sz );

//    lpGrpX->lstFuncs.fnMemCopy( ptr, res, sz );
memcpy( ptr, res, sz );

    return ptr;
}
