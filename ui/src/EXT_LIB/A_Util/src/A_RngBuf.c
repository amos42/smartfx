/**************************************************************************
 * @file A_RngBuf.c
 * @author gmju
 * @date 2005-10-29 17:00:00
 * @brief 
**************************************************************************/
#include "A_RngBuf.h"
#include <string.h>


/****************************************
  주어진 메모리를 바탕으로 링버퍼를 생성한다.
  단, 주어진 메모리는 총 링버퍼 크기보다
*****************************************/
#ifdef atRINGBUF_USE_OWNMEMORY
atRINGBUF * atRINGBUF_Create( atRINGBUFFUNCS *lpFuncs, atLONG lBufSize )
#else
atBOOL  atRINGBUF_Create( atRINGBUFFUNCS *lpFuncs, atRINGBUF * This, atVOID * pBuf, atLONG lBufSize )
#endif
{
    if( pBuf == atNULL ) return atFALSE;

    This->pBuf      = pBuf;
    This->lBufSize  = lBufSize;
    This->lRingMode = 0; //atRINGMODE_WR_FULLDATA | atRINGMODE_WR_FULLDATA;
    if( lpFuncs ) This->lstFuncs  = *lpFuncs;
    atRINGBUF_Clear( This );

    return atTRUE;
}


/****************************************

*****************************************/
atBOOL  atRINGBUF_Destroy( atRINGBUF * This )
{
    if( This == atNULL ) return atFALSE;

    if( This->pDeleteFunc ) This->pDeleteFunc( This->pBuf );

    return atTRUE;
}


/****************************************

*****************************************/
atVOID  atRINGBUF_Clear( atRINGBUF * This )
{
    if( This == atNULL ) return;

    This->lStartPos = 0;
    This->lFillSize = 0;  
    This->lCurWrPos = This->lCurRdPos = 0;
}


/****************************************
  Callback 함수 지정
*****************************************/
atVOID atRINGBUF_SetCallbackFunc( atRINGBUF * This, atRINGBUF_DELETEFUNC * pDeleteFunc )
{
    if( This == atNULL ) return;
    
    This->pDeleteFunc = pDeleteFunc;
}


/****************************************

*****************************************/
atRINGMODE  atRINGBUF_SetMode( atRINGBUF * This, atRINGMODE lRingMode )
{
    atRINGMODE old;
    
    if( This == atNULL ) return 0;

    old = This->lRingMode;
    This->lRingMode  = lRingMode;
    
    return old;
}


/****************************************

*****************************************/
atRINGMODE  atRINGBUF_GetMode( atRINGBUF * This )
{
    if( This == atNULL ) return 0;

    return  This->lRingMode;
}


/****************************************

*****************************************/
atLONG atRINGBUF_GetFillSize( atRINGBUF * This )
{
    if( This == atNULL ) return 0;

    return This->lFillSize;
}


/****************************************

*****************************************/
atLONG atRINGBUF_GetLeftSize( atRINGBUF * This )
{
    if( This == atNULL ) return 0;

    return This->lBufSize - atRINGBUF_GetFillSize(This);
}


/****************************************

*****************************************/
atLONG  atRINGBUF_Drop( atRINGBUF * This, atLONG lDropSize, atRINGDROPPOS iDropPos )
{
    if( lDropSize >= This->lFillSize ){
      atRINGBUF_Clear( This );
    } else {
      if( iDropPos == atRINGDROP_START ){
        This->lStartPos += lDropSize;
        if( This->lStartPos >= This->lBufSize ) This->lStartPos -= This->lBufSize;
        This->lFillSize -= lDropSize;
        This->lCurWrPos -= lDropSize;
        if( This->lCurWrPos < 0 ) This->lCurWrPos = 0;
        This->lCurRdPos -= lDropSize;
        if( This->lCurRdPos < 0 ) This->lCurRdPos = 0;
      } else if( iDropPos == atRINGDROP_END ){
        This->lFillSize -= lDropSize;
        if( This->lCurWrPos >= This->lFillSize ) This->lCurWrPos = This->lFillSize - 1;
        if( This->lCurRdPos >= This->lFillSize ) This->lCurRdPos = This->lFillSize - 1;
      }
   }
   
   return This->lFillSize;
}


/****************************************

*****************************************/
static atLONG  __seek( atRINGBUF * This, atLONG lPos, atRINGSEEKMODE iSeekMode, atLONG curpos )
{
    long fill = atRINGBUF_GetFillSize(This);
    
    if( iSeekMode == atRINGSEEK_START ){
        curpos = lPos;
    } else if( iSeekMode == atRINGSEEK_END ){
        curpos = fill + lPos;
    } else if( iSeekMode == atRINGSEEK_CURRENT ){
        curpos += lPos;
    }
    if( curpos < 0 ) curpos = 0;
    if( curpos > fill ) curpos = fill;
    
    return curpos;
}
atLONG  atRINGBUF_Seek( atRINGBUF * This, atLONG lPos, atRINGSEEKMODE iSeekMode )
{
    long curpos;
    
    if( iSeekMode >= atRINGSEEK_RD_START ){
        curpos = This->lCurRdPos;
        curpos = __seek( This, lPos, (atRINGSEEKMODE)((int)iSeekMode - (int)atRINGSEEK_RD_START), curpos );
        This->lCurRdPos = curpos;
    } else {
        curpos = This->lCurWrPos;
        curpos = __seek( This, lPos, iSeekMode, curpos );
        This->lCurWrPos = curpos;
    }
    
    return curpos;
}


/****************************************

*****************************************/
// 버퍼에서 데이터를 읽는다. 단, 크기는 들어 있는 데이터 크기를 초과하지 않는다.
static void __read( atRINGBUF * This, char *ptr, int size )
{
    char *p = This->pBuf;
    int bsize = This->lBufSize;
    int st = This->lStartPos;
    int cp = This->lCurRdPos;
    int sz2;

    cp = st + cp;
    if( cp > bsize ) cp -= bsize;
    
    if( cp + size > bsize ){
        sz2 = bsize - cp;
        memcpy( ptr, p+cp, sz2 );
        ptr += sz2;
        size -= sz2;
        cp = 0;
//        This->lCurRdPos += sz2;
    }
    
    memcpy( ptr, p+cp, size );

//    This->lCurRdPos += size;
}

atLONG atRINGBUF_Read( atRINGBUF * This, atVOID * pDst, atLONG lSize )
{
    long fill = atRINGBUF_GetFillSize(This);

    fill -= This->lCurRdPos;
    if( fill <= 0 ) return 0;

    if( lSize <= fill ){
        __read( This, pDst, lSize );
        if( !(This->lRingMode & atRINGMODE_RD_NOINC) ) This->lCurRdPos += lSize;
        return lSize;
    } else {
//        if( (This->lRingMode & atRINGMODE_RD_MASK) == atRINGMODE_RD_ALLOWPARTICLE ){
        if( This->lRingMode & atRINGMODE_RD_ALLOWPARTICLE ){
            __read( This, pDst, fill );
            if( !(This->lRingMode & atRINGMODE_RD_NOINC) ) This->lCurRdPos += fill;
            return fill;
        } else {
            return 0;
        }
    }
}


/****************************************

*****************************************/
atVOID *  atRINGBUF_GetReaderblePtr( atRINGBUF * This, atLONG lSize, atBOOL bIsMove )
{    
    long fill = This->lFillSize;
    char *p = This->pBuf;
    int bsize = This->lBufSize;
    int st = This->lStartPos;
    int cp = This->lCurRdPos;

    if( lSize > fill ) return atNULL;
    
    cp += st;
    if( cp > bsize ) cp -= bsize;
    fill += st;
    if( fill > bsize ) fill -= bsize;

    if( cp + lSize > bsize ) return atNULL;
    if( cp <= fill ){
        if( cp + lSize > fill ) return atNULL;
    }
 
    if( bIsMove ) This->lCurRdPos += lSize;
	
    return p + cp;
}


/****************************************

*****************************************/
// 버퍼에 데이터를 쓴다. 단, 크기는 가용 데이터 크기를 초과하지 않는다.
static void __write( atRINGBUF * This, char *ptr, int size )
{
    char *p = This->pBuf;
    int bsize = This->lBufSize;
    int st = This->lStartPos;
    int cp = This->lCurWrPos;
    int sz2;

    cp = st + cp;
    if( cp > bsize ) cp -= bsize;
 
    if( cp + size > bsize ){
        sz2 = bsize - cp;
        memcpy( p+cp, ptr, sz2 );
        ptr += sz2;
        size -= sz2;
        cp = 0;
        This->lCurWrPos += sz2;
    }
    
    memcpy( p+cp, ptr, size );

    This->lCurWrPos += size;
    if( This->lCurWrPos > This->lFillSize ) This->lFillSize = This->lCurWrPos;
}

atLONG atRINGBUF_Write( atRINGBUF * This, atVOID * pSrc, atLONG lSize )
{
    long left = This->lBufSize - This->lCurWrPos;
    
    if( lSize <= left ){
        __write( This, pSrc, lSize );
        return lSize;
    } else {
        if( (This->lRingMode & atRINGMODE_WR_MASK) == atRINGMODE_WR_ALLOWPARTICLE ){
            if( left <= 0 ) return 0;
            __write( This, pSrc, left );
            return left;
        } else if( (This->lRingMode & atRINGMODE_WR_MASK) == atRINGMODE_WR_DROPWRITE ) {
            if( lSize <= This->lBufSize ){
              atRINGBUF_Drop( This, lSize-left, atRINGDROP_START );
              __write( This, pSrc, lSize );
              return lSize;
            }
        }
    }
    
    return 0;
}





















#if 0 // 테스트용 

#include <stdio.h>


char buf[10] = "0000000000";
atRINGBUF ring;

void print_data( char *buf, int count, int startpos, int fillsize )
{
    int i;
    int endpos;
   
    if( fillsize == 0 ) fillsize = -1;
    if( startpos + fillsize <= count ){
        endpos = startpos + fillsize - 1;
    } else {
        endpos = startpos + fillsize - count - 1;
    }
    
    for( i = 0; i < count; i ++ ){
      if( i == startpos ) printf( "[" ); else printf(" ");
      printf( "%c", buf[i] );
      if( i == endpos ) printf( "]" ); else printf(" ");
    }
    printf( "\n" );
}

void print_info( atRINGBUF *ring )
{
    int i , pos;
    printf( "StartPos = %d\n", ring->lStartPos );
    printf( "FillSize = %d\n", ring->lFillSize );
    printf( "CurWrPos = %d\n", ring->lCurWrPos );
    printf( "CurRdPos = %d\n", ring->lCurRdPos );
    pos = ring->lStartPos + ring->lCurWrPos; if( pos >= ring->lBufSize ) pos -= ring->lBufSize;
    printf( "         " ); for( i = 0; i < pos; i ++ ) printf("   "); printf(" v\n");
    printf( "* Data : " ); print_data( ring->pBuf, ring->lBufSize, ring->lStartPos, ring->lFillSize );
    pos = ring->lStartPos + ring->lCurRdPos; if( pos >= ring->lBufSize ) pos -= ring->lBufSize;
    printf( "         " ); for( i = 0; i < pos; i ++ ) printf("   "); printf(" ^\n");
    printf( "==================================\n" );
}

void write( atRINGBUF *ring, char *str, int size )
{
    int cnt;
    
    cnt = atRINGBUF_Write( ring, str, size );

    printf( "> write (%d / %d) : ", cnt, size ); print_data( str, size, -1, 0 );
    print_info( ring );
}

void read( atRINGBUF *ring, int size )
{
    int cnt;
    char str[50] = "";
//char *str;
    
    cnt = atRINGBUF_Read( ring, str, size );
//str = atRINGBUF_GetReaderblePtr( ring, size, atTRUE );
//if( str != atNULL ) cnt = size; else cnt = 0;

    printf( "> read (%d / %d) : ", cnt, size ); print_data( str, cnt, -1, 0 );
    print_info( ring );
}

void seek( atRINGBUF *ring, int pos, int mode )
{
    int cnt;
    
    cnt = atRINGBUF_Seek( ring, pos, mode );

    printf( "> seek (%s : %d)\n", (mode>=atRINGSEEK_RD_START)?"ReadPos":"WritePos", cnt );
    print_info( ring );
}

void drop( atRINGBUF *ring, int cnt, int mode )
{
    atRINGBUF_Drop( ring, cnt, mode );

    printf( "> drop (%s : %d)\n", (mode==atRINGDROP_START)?"start":"end", cnt );
    print_info( ring );
}


main()
{
    atRINGBUF_Create( &ring, buf, 10 );
    print_info( &ring );
   

    write( &ring, "012", 3 );
    write( &ring, "345", 3 );
    write( &ring, "678", 3 );
    
    seek( &ring, 0, atRINGSEEK_WR_START );    
    write( &ring, "ABC", 3 );

    drop( &ring, 3, atRINGDROP_START );

    write( &ring, "DEFG", 4 );
    write( &ring, "HIJ", 3 );
    write( &ring, "KLM", 3 );

    read( &ring, 3 );
    print_info( &ring );

//    atRINGBUF_SetMode( &ring, atRINGMODE_WR_ALLOWPARTICLE | atRINGMODE_RD_ALLOWPARTICLE );
    atRINGBUF_SetMode( &ring, atRINGMODE_WR_DROPWRITE | atRINGMODE_RD_ALLOWPARTICLE );
//    atRINGBUF_SetMode( &ring, atRINGMODE_WR_ALLOWPARTICLE| atRINGMODE_RD_ALLOWPARTICLE );

    seek( &ring, 5, atRINGSEEK_RD_START );    
    read( &ring, 4 );

    atRINGBUF_SetMode( &ring, atRINGMODE_WR_DROPWRITE | atRINGMODE_RD_ALLOWPARTICLE );
    write( &ring, "OPQRST", 5 );

    seek( &ring, 7, atRINGSEEK_RD_START );    
    read( &ring, 2 );
            
    atRINGBUF_Destroy( &ring );

    return 0;
}

#endif

