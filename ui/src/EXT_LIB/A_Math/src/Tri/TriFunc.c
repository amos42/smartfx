/*###########################################################################
-----------------------------------------------------------------------------

                                TriFunc.c
                         (* =================== *)


 Version           : 1.0.0
 Dependent Modules : 
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : Pre-calculated table definition
 
-----------------------------------------------------------------------------
###########################################################################*/

#define __GRPX_IMPLEMENT_

//#define FIXED_RADIX_SIZE (16)
//#define FIXED_RADIX_SIZE (8)

#include "AT_Types.h"
#include "TriFunc.h"


// 이 함수를 이용해 삼각함수 테이블을 만들려면...
// 아래 __MAKE_TBL_의 커맨트를 푼다.
//#define __MAKE_TBL_


//#define PI (3.1415926536)
#define PI (3.14159265358979323846)


#ifdef __FIXEDPTR_
                  
#if FIXED_RADIX_SIZE == (16)
#if   TRI_1DEG_RATIO == (10)
#define TRI_TBL_FILE "_TriTbl_0p1_r16.h"
#elif TRI_1DEG_RATIO == (5)
#define TRI_TBL_FILE "_TriTbl_0p2_r16.h"
#elif TRI_1DEG_RATIO == (2)
#define TRI_TBL_FILE "_TriTbl_0p5_r16.h"
#elif TRI_1DEG_RATIO == (1)
#define TRI_TBL_FILE "_TriTbl_1_r16.h"
//#elif TRI_1DEG_RATIO == (1/10f)
//#define TRI_TBL_FILE "_TriTbl_10_r16.h"
#else
#error "(Error) TRI_DEG_RATIO에 해당하는 테이블이 없습니다."
#endif
#elif FIXED_RADIX_SIZE == (8)
#if   TRI_1DEG_RATIO == (10)
#define TRI_TBL_FILE "_TriTbl_0p1_r8.h"
#elif TRI_1DEG_RATIO == (5)
#define TRI_TBL_FILE "_TriTbl_0p2_r8.h"
#elif TRI_1DEG_RATIO == (2)
#define TRI_TBL_FILE "_TriTbl_0p5_r8.h"
#elif TRI_1DEG_RATIO == (1)
#define TRI_TBL_FILE "_TriTbl_1_r8.h"
//#elif TRI_1DEG_RATIO == (1/10f)
//#define TRI_TBL_FILE "_TriTbl_10_r16.h"
#else
#error "(Error) TRI_DEG_RATIO에 해당하는 테이블이 없습니다."
#endif
#else
#error "(Error) FIXED_RADIX_SIZE에 해당하는 테이블이 없습니다."
#endif


#if !defined(__MAKE_TBL_)

#include TRI_TBL_FILE

#else

atREAL SinTable[ 90 * TRI_1DEG_RATIO + 1 ];
atREAL TanTable[ 90 * TRI_1DEG_RATIO + 1 ];

#endif


void InitTriangleFunc( void )
{
}


#else


#include <math.h>


atREAL SinTable[ TRI_1DEG_RATIO * 90 + 1 ];
//atREAL CosTable[ TRI_1DEG_RATIO * 90 + 1 ];
atREAL TanTable[ TRI_1DEG_RATIO * 90 + 1 ];



void InitTriangleFunc( void )
{
#ifdef __USE_TRI_TABLE_
    int i;
    double angle;

    for( i = 0; i <= TRI_1DEG_RATIO * 90; i ++ ){
      angle = (double)i * PI / (TRI_1DEG_RATIO * 180);

      SinTable[i] = (atREAL)( sin( angle ) * INTTOREAL(1) );
      CosTable[i] = (atREAL)( cos( angle ) * INTTOREAL(1) );
      TanTable[i] = (atREAL)( tan( angle ) * INTTOREAL(1) );
    }
#endif    
}


#endif


#ifdef __USE_TRI_TABLE_

// 단, 각도는 플러스 값이어야 한다.
#define SIN_P(x) (SinTable[ REALTOINT((atREAL)(x) * TRI_1DEG_RATIO) ])
//#define COS_P(x) (CosTable[ REALTOINT((atREAL)(x) * TRI_1DEG_RATIO) ]) // SIN 함수로 COS 함수 대체 가능
#define TAN_P(x) (TanTable[ REALTOINT((atREAL)(x) * TRI_1DEG_RATIO) ])


atREAL fast_sin( atREAL deg )
{
    atREAL v;

	deg %= MAX_TRI_ANGLE;
    if( deg < 0 ) deg += MAX_TRI_ANGLE;

	if( deg <= INTTOREAL(90) ){
      v = SIN_P( deg );
	} else if( deg <= INTTOREAL(180) ){
      v = SIN_P( INTTOREAL(180) - deg );
	} else if( deg <= INTTOREAL(270) ){
      v = -SIN_P( deg - INTTOREAL(180) );
	} else {
      v = -SIN_P( INTTOREAL(360) - deg );
	}

    return( v );
}


atREAL fast_cos( atREAL deg )
{
    atREAL v;

	v = fast_sin( deg + INTTOREAL(90) );    

    return( v );
}

atREAL fast_tan( atREAL deg )
{
    atREAL v;

	deg %= MAX_TRI_ANGLE;
    if( deg < 0 ) deg += MAX_TRI_ANGLE;

	if( deg <= INTTOREAL(90) ){
	  v = TAN_P( deg );
	} else {
      v = -TAN_P( INTTOREAL(90) - deg );
	}

    return( v );
}

#else

#include <math.h>

atREAL fast_sin( atREAL deg )
{
    return sin( deg * PI / 180 );
}


atREAL fast_cos( atREAL deg )
{
    return cos( deg * PI / 180 );
}

atREAL fast_tan( atREAL deg )
{
    return tan( deg * PI / 180 );
}


#endif


//==============================================

#if defined(__MAKE_TBL_)

#include <math.h>
#include <stdio.h>

int main(void)
{
    int i;
    double angle, v;
    atREAL vv;
#ifdef __FIXEDPTR_      
    const char *fmt = "%9ld,";
#else
    const char *fmt = "%f,";
#endif
    FILE *fp;
    
    fp = fopen( TRI_TBL_FILE, "wt" );
    if( fp == NULL ) return( 1 );

    fprintf( fp, "\n//#define TRI_1DEG_RATIO      %5.2f\n\n\n", (float)1 / TRI_1DEG_RATIO );
    
    fprintf( fp, "const atREAL SinTable[ TRI_1DEG_RATIO * 90 + 1 ] = {" );
    for( i = 0; i <= TRI_1DEG_RATIO * 90; i ++ ){
      angle = (double)i * PI / (TRI_1DEG_RATIO * 180);
      v = sin( angle );
      vv = ((v > 0x7fff) || (v < -0xffff) )? 0x7fffffff : (atREAL)(v * INTTOREAL(1));
      if( (i % 10) == 0 ) fprintf( fp, "\n   " );
      fprintf( fp, fmt, vv );
    }
    fprintf( fp, " // 90\n};\n\n" );

/*      
    fprintf( fp, "const atREAL CosTable[ TRI_1DEG_RATIO * 90 + 1 ] = {" );
    for( i = 0; i <= TRI_1DEG_RATIO * 90; i ++ ){
      angle = (double)i * PI / (TRI_1DEG_RATIO * 180);
      v = cos( angle );
      vv = ((v > 0x7fff) || (v < -0xffff) )? 0x7fffffff : (atREAL)(v * INTTOREAL(1));
      if( (i % 10) == 0 ) fprintf( fp, "\n   " );
      fprintf( fp, fmt, vv );
    }
    fprintf( fp, "  // 180\n};\n\n" );
*/
      
    fprintf( fp, "const atREAL TanTable[ TRI_1DEG_RATIO * 90 + 1 ] = {" );
    for( i = 0; i <= TRI_1DEG_RATIO * 90; i ++ ){
      angle = (double)i * PI / (TRI_1DEG_RATIO * 180);
      v = tan( angle );
      vv = ((v > 0x7fff) || (v < -0xffff) )? 0x7fffffff : (atREAL)(v * INTTOREAL(1));
      if( (i % 10) == 0 ) fprintf( fp, "\n   " );
      fprintf( fp, fmt, vv );
    }
    fprintf( fp, "  // 90\n};\n\n" );

    fclose( fp );
    
    return( 0 );
}

#endif
