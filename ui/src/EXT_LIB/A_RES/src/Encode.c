/*#############################################################################
-------------------------------------------------------------------------------

                                  Encode.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules : A_VM, A_Utils
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Encode related function definition

-------------------------------------------------------------------------------
#############################################################################*/
#define __IMPLEMENT_

#include "AT_Types.h"
#include "A_RES.h"
#include <string.h>


#define BITS_PER_PIXEL  8
#define CLEAR_CODE      (1 << BITS_PER_PIXEL)
#define EOF_CODE        (CLEAR_CODE + 1)


static char *OutBuff;
static short Suffix[4097], Prefix[4097], Next[4097];
static short CodeSize, MaxCode, FreeCode;
static atULONG BytePos, BitPos;


//#ifndef __BIG_ENDIAN_
union XXX {
  atBYTE  b[4];
  atDWORD d;
}; 
//#endif

static void OutCode( int code )
{
//#ifndef __BIG_ENDIAN_
    atBYTE *p;
    union XXX x;
//#endif

#ifdef __BIG_ENDIAN_
//    *(long *)(OutBuff + BytePos) += ((long)code << BitPos);
    p = (atBYTE *)OutBuff + BytePos;
    x.b[3] = p[0];
    x.b[2] = p[1];
    x.b[1] = p[2];
    x.b[0] = p[3];
    x.d += ((unsigned long)code << BitPos);
    p[0] = x.b[3];
    p[1] = x.b[2];
    p[2] = x.b[1];
    p[3] = x.b[0];
#else
    p = (atBYTE *)OutBuff + BytePos;
    x.b[0] = p[0];
    x.b[1] = p[1];
    x.b[2] = p[2];
    x.b[3] = p[3];
    x.d += ((unsigned long)code << BitPos);
    p[0] = x.b[0];
    p[1] = x.b[1];
    p[2] = x.b[2];
    p[3] = x.b[3];
#endif

    BitPos  += CodeSize;
    BytePos += BitPos >> 3;    /* BytePos += BitPos / 8 */
    BitPos  &= 0x07;           /* BitPos  %= 8;         */
}


static void Initialize( void )
{
    int i;

    FreeCode = CLEAR_CODE + 2;
    MaxCode  = CLEAR_CODE * 2;

    for( i = 0; i < 4097; i ++ )
      Prefix[i] = Suffix[i] = Next[i] = -1;

    OutCode( CLEAR_CODE );

    CodeSize = BITS_PER_PIXEL + 1;
}


static void Append( int pre, int suf )
{
    int i;

    if( Suffix[ suf ] == -1 )
      Suffix[ suf ] = FreeCode;
    else{
      suf = Suffix[ suf ];
      while( (i = Next[ suf ]) != -1 ) suf = i;
      Next[ suf ] = FreeCode;
    }

    Prefix[ FreeCode ] = pre;
}


/*********************
   이미지를 압축한다.
**********************/
atULONG atRES_Encode( atCHAR *DesData, atCHAR *SrcData, atULONG DataSize )
{
    short i, j;
    atCHAR *DataEndPtr;
    atWORD CurCode = 0;

//    OutBuff = (char *)ALLOCMEM( DataSize + 10 );
    OutBuff = (char *)DesData;
    memset( OutBuff, 0, DataSize );

    DataEndPtr = SrcData + DataSize - 1;

    BitPos = 0;
    BytePos = 0;

    CodeSize = BITS_PER_PIXEL + 1;
    Initialize();

    i = (short)*(atBYTE *)SrcData++;

    while( SrcData <= DataEndPtr ){
      j = i;

      if( Prefix[i] == -1 ){
         Prefix[i] = CurCode = i;
         i = (short)*(atBYTE *)SrcData++;
      } else {
        while( j != -1 ){
          CurCode = j;
          j = Suffix[ CurCode ];
          i = (short)*(atBYTE *)SrcData++;
          while( j != -1 && i != Prefix[j] ) j = Next[j];
          if( SrcData > DataEndPtr ) break;
        }
      }

      if( BytePos >= DataSize ){
        BytePos = 0;
        break;
      }

      OutCode( CurCode );

      if( SrcData <= DataEndPtr ){
        Append( i, CurCode );

        if( FreeCode++ >= MaxCode ){
          MaxCode <<= 1;
          if( CodeSize < 12 )
            CodeSize++;
          else
            Initialize();
        }
      } else {
        OutCode( i );
      }
    }

    if( BytePos > 0 ){
      OutCode( EOF_CODE );
      if( BitPos > 0 ) BytePos ++;
      if( BytePos < DataSize ){
//        *DesData = (CHAR *)ALLOCMEM( BytePos );
//        memcpy( *DesData, OutBuff, BytePos );
      } else {
        BytePos = 0;
      }
    }

//    FREEMEM( OutBuff );

    return( BytePos );
}

