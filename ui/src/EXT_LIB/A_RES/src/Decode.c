/*#############################################################################
-------------------------------------------------------------------------------

                                  Decode.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules : A_VM, A_Utils
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Decode related function definition

-------------------------------------------------------------------------------
#############################################################################*/
#define __IMPLEMENT_

#include "AT_Types.h"
#include "A_RES.h"
#include <string.h>


#define BITS_PER_PIXEL  (8)
#define CLEAR_CODE      ((short)(1 << BITS_PER_PIXEL))
#define EOF_CODE        (CLEAR_CODE + 1)
#define BIT_MASK        (CLEAR_CODE - 1)


static char *InBuff;

static short Suffix[4096], Prefix[4096], OutCode[2048];
static short CodeSize, ReadMask;
static int BitPos;

union XXX {
  atBYTE  b[4];
  atDWORD d;
}; 

static short ReadCode(void)
{
    short i;
    long RawCode;
    union XXX x;

//    RawCode = *(long *)InBuff; // Word Align이 안 된 경우, ARM과 같은 프로세서에선 Panic 된다. 
//    RawCode = (DWORD)InBuff[0] << 24 | (DWORD)InBuff[1] << 16 | 
//              (DWORD)InBuff[2] << 8  | (DWORD)InBuff[3];
#ifdef __BIG_ENDIAN_
    x.b[3] = InBuff[0];
    x.b[2] = InBuff[1];
    x.b[1] = InBuff[2];
    x.b[0] = InBuff[3];
#else
    x.b[0] = InBuff[0];
    x.b[1] = InBuff[1];
    x.b[2] = InBuff[2];
    x.b[3] = InBuff[3];
#endif
    RawCode = (long)x.d;

    i = (short)(RawCode >> BitPos) & ReadMask;

    BitPos += CodeSize;
    InBuff += BitPos >> 3;    /* InBuff += BitPos / 8 */
    BitPos &= 0x07;           /* BitPos %= 8;         */

    return( i );
}


/***********************
  압축된 이미지를 푼다.
************************/
atBOOL atRES_Decode( atCHAR *DesData, atCHAR *SrcData, atULONG DataSize )
{
    short i, OutCount;
    short Code, InCode, CurCode, OldCode, FinChar;
    short MaxCode, FirstFree, FreeCode;
    atBYTE *DataPtr, *DataEndPtr;

    if( DataSize == 0 ) return( atFALSE );

    InBuff = (char*)SrcData;
//    *(atBYTE **)DesData = DataPtr = (atBYTE *)ALLOCMEM( DataSize );
    
    DataPtr = (atBYTE *)DesData;
    if( DataPtr == atNULL ) return( atFALSE );
    memset( DataPtr, 0, DataSize );

    DataEndPtr = DataPtr + DataSize - 1;


    /* Initialize */
    OldCode   = FinChar = 0;
    FirstFree = FreeCode  = CLEAR_CODE + 2;
    MaxCode   = CLEAR_CODE * 2;
    ReadMask  = MaxCode - 1;
    OutCount  = 0;

    CodeSize  = BITS_PER_PIXEL + 1;


    BitPos = 0;
    Code = ReadCode();

    while( DataPtr <= DataEndPtr && Code != EOF_CODE ){
      if( Code == CLEAR_CODE ){
         CodeSize = BITS_PER_PIXEL + 1;
         MaxCode = (1 << CodeSize);
         ReadMask = MaxCode - 1;
         FreeCode = FirstFree;
         OldCode = ReadCode();
         FinChar = OldCode & BIT_MASK;
         *DataPtr++ = (atBYTE)FinChar;
      } else {
         CurCode = InCode = Code;
         if( CurCode >= FreeCode ){
           CurCode = OldCode;
           OutCode[ OutCount++ ] = FinChar;
         }

         while( CurCode > BIT_MASK ){
           if( OutCount > 2047 ) return( atFALSE );
           OutCode[ OutCount++ ] = Suffix[ CurCode ];
           CurCode = Prefix[ CurCode ];
         }
         FinChar = CurCode & BIT_MASK;
         OutCode[OutCount++] = FinChar;

         for( i = OutCount-1; i >= 0; i-- ){
           *DataPtr++ = (atBYTE)OutCode[i];
           if( DataPtr > DataEndPtr ) break;
         }

         OutCount = 0;

         Prefix[ FreeCode ] = OldCode;
         Suffix[ FreeCode ] = FinChar;
         OldCode = InCode;

         FreeCode ++;
         if( FreeCode >= MaxCode ){
           if( CodeSize < 12 ){
              CodeSize++;
              MaxCode *= 2;
              ReadMask = MaxCode - 1;
           }
         }
      }

      Code = ReadCode();
    }
    
    return( atTRUE );
}

