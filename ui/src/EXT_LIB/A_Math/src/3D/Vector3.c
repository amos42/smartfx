/*#############################################################################
-------------------------------------------------------------------------------

                                   Vector3.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules : A_VM
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : 3-dimesion Vector related function definition

-------------------------------------------------------------------------------
#############################################################################*/

#include "Vector3.h"
#include "INTSqrt.h"


/**************************************
  3차원 벡터를 세트한다.
***************************************/
void atVECTOR3_Set( atVECTOR3 *t, atREAL i, atREAL j, atREAL k )
{
    t->i = i;
    t->j = j;
    t->k = k;
}


/**************************************
  3차원 벡터의 역벡터를 구한다.
***************************************/
void atVECTOR3_Inverse( atVECTOR3 *t )
{
    t->i = -t->i;
    t->j = -t->j;
    t->k = -t->k;
}


/**************************************
  3차원 벡터를 서로 더한다.
***************************************/
void atVECTOR3_Add( atVECTOR3 *t, atVECTOR3 *u, atVECTOR3 *v )
{
    t->i = u->i + v->i;
    t->j = u->j + v->j;
    t->k = u->k + v->k;
}


/**************************************
  3차원 벡터를 서로 뺀다.
***************************************/
void atVECTOR3_Subtract( atVECTOR3 *t, atVECTOR3 *u, atVECTOR3 *v )
{
    t->i = u->i - v->i;
    t->j = u->j - v->j;
    t->k = u->k - v->k;
}


/**************************************
  3차원 벡터 간의 내적을 구한다.
***************************************/
atREAL atVECTOR3_DotProduct( atVECTOR3 *u, atVECTOR3 *v )
{
    return( REALMUL( u->i, v->i ) + REALMUL( u->j, v->j ) + REALMUL( u->k, v->k ) );
}


/**************************************
  3차원 벡터 간의 외적을 구한다.
***************************************/
void atVECTOR3_CrossProduct( atVECTOR3 *t, atVECTOR3 *u, atVECTOR3 *v )
{
    atVECTOR3 vv;
    atVECTOR3 *pbk = atNULL;

    if( (t == u) || (t == v) ){
      pbk = t;
      t = &vv;
    }

    t->i = REALMUL( u->j, v->k ) - REALMUL( u->k, v->j );
    t->j = REALMUL( u->k, v->i ) - REALMUL( u->i, v->k );
    t->k = REALMUL( u->i, v->j ) - REALMUL( u->j, v->i );

    if( pbk ){
      *pbk = vv;
    }
}


/**************************************
  3차원 벡터의 크기를 얻는다.
***************************************/
atREAL atVECTOR3_GetNorm( atVECTOR3 *v )
{
    return REALSQRT( REALMUL(v->i, v->i) + REALMUL(v->j, v->j) + REALMUL(v->k, v->k) );
}


#ifdef __FIXEDPTR_ 
#define REALMUL64( a, b )  ((atINT64)(  (  ((atINT64)(a))*(atREAL)(b)) >> FIXED_RADIX_SIZE  )   )
#define REALSQRT64(a)      ((atINT64)(IntSqrt64((atINT64)(a)) << (FIXED_RADIX_SIZE/2)))
/**************************************
  3차원 벡터의 크기를 얻는다.
***************************************/
atREAL atVECTOR3_GetNorm64( atVECTOR3 *v )
{
    atINT64 a, b, c;
    a = REALMUL64(v->i, v->i);
    b = REALMUL64(v->j, v->j);
    c = REALMUL64(v->k, v->k);
    
    return (atREAL)REALSQRT64( a + b + c );
}
#undef REALSQRT64
#undef REALMUL64
#endif


/**************************************
  3차원 벡터를 정규화시킨다.
***************************************/
void atVECTOR3_Normalize( atVECTOR3 *v )
{
    atREAL a;

    a = atVECTOR3_GetNorm( v );

    if( a == INTTOREAL(0) ){
      v->i = v->j = v->k = 0;
    } else if( a != INTTOREAL(1) ){
/*        
      v->i = REALDIV( v->i, a );
      v->j = REALDIV( v->j, a );
      v->k = REALDIV( v->k, a );
*/      
      // 곱셈보다 나눗셈이 더 느리기 때문에...
      a = REALDIV( INTTOREAL(1), a );
      v->i = REALMUL( v->i, a );
      v->j = REALMUL( v->j, a );
      v->k = REALMUL( v->k, a );
    }
}


#ifdef __FIXEDPTR_ 
/**************************************
  3차원 벡터를 정규화시킨다.
***************************************/
void atVECTOR3_Normalize64( atVECTOR3 *v )
{
    atREAL a;

    a = atVECTOR3_GetNorm64( v );

    if( a == INTTOREAL(0) ){
      v->i = v->j = v->k = 0;
    } else if( a != INTTOREAL(1) ){
      // 곱셈보다 나눗셈이 더 느리기 때문에...
      a = REALDIV( INTTOREAL(1), a );
      v->i = REALMUL( v->i, a );
      v->j = REALMUL( v->j, a );
      v->k = REALMUL( v->k, a );
    }
}
#endif


/**************************************
  3차원 벡터에 스칼라 곱을 한다.
***************************************/
void atVECTOR3_ScalarProduct( atVECTOR3 *t, atVECTOR3 *v, atREAL scalar )
{
    t->i = REALMUL( v->i, scalar );
    t->j = REALMUL( v->j, scalar );
    t->k = REALMUL( v->k, scalar );
}


/**************************************
  3차원 벡터에 스칼라(정수) 곱을 한다.
***************************************/
void atVECTOR3_ScalarProduct_i( atVECTOR3 *t, atVECTOR3 *v, atINT i_scalar )
{
    t->i = v->i * i_scalar;
    t->j = v->j * i_scalar;
    t->k = v->k * i_scalar;
}

