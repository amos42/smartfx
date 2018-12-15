/*#############################################################################
-------------------------------------------------------------------------------

                                   Matrix4.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules : A_VM
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : 4x4 Matrix related function definition

-------------------------------------------------------------------------------
#############################################################################*/

#include "AT_Types.h"
#include "AT_Real.h"
#include "Matrix4.h"


/********************************
  4x4 행렬의 값을 세트한다.
*********************************/
void atMATRIX4_Set( atMATRIX4 *m, atREAL _11, atREAL _12, atREAL _13, atREAL _14,
                                  atREAL _21, atREAL _22, atREAL _23, atREAL _24,
                                  atREAL _31, atREAL _32, atREAL _33, atREAL _34,
                                  atREAL _41, atREAL _42, atREAL _43, atREAL _44 )
{
    m->_11 = _11; m->_12 = _12; m->_13 = _13; m->_14 = _14;
    m->_21 = _21; m->_22 = _22; m->_23 = _23; m->_24 = _24;
    m->_31 = _31; m->_32 = _32; m->_33 = _33; m->_34 = _34;
    m->_41 = _41; m->_42 = _42; m->_43 = _43; m->_44 = _44;
}


/********************************
  4x4 행렬의 기본 행렬을 구한다.
*********************************/
void atMATRIX4_Identity( atMATRIX4 *m )
{
    m->_12 = m->_13 = m->_14 = m->_21 = m->_23 = m->_24 = INTTOREAL(0);
    m->_31 = m->_32 = m->_34 = m->_41 = m->_42 = m->_43 = INTTOREAL(0);
    m->_11 = m->_22 = m->_33 = m->_44 = INTTOREAL(1);
}


/********************************
  Desc: [Q] = [A] + [B].
  두개의 4x4 행렬을 더한다.
*********************************/
void atMATRIX4_Add( atMATRIX4 *q, atMATRIX4 *a, atMATRIX4 *b )
{
/*
    int i, j;
//    atREAL *pA = (atREAL *)a->m;
//    atREAL *pB = (atREAL *)b->m;
//    atREAL *pM = (atREAL *)q->m;
    atREAL *pA = (atREAL *)&a->_11;
    atREAL *pB = (atREAL *)&b->_11;
    atREAL *pM = (atREAL *)&q->_11;

    for( i = 0; i < 4; i++ )
      for( j = 0; j < 4; j++ )
        pM[4*i+j] = pA[4*i+j] + pB[4*i+j];
*/
    q->_11 = a->_11 + b->_11;
    q->_12 = a->_12 + b->_12;
    q->_13 = a->_13 + b->_13;
    q->_14 = a->_14 + b->_14;

    q->_21 = a->_21 + b->_21;
    q->_22 = a->_22 + b->_22;
    q->_23 = a->_23 + b->_23;
    q->_24 = a->_24 + b->_24;

    q->_31 = a->_31 + b->_31;
    q->_32 = a->_32 + b->_32;
    q->_33 = a->_33 + b->_33;
    q->_34 = a->_34 + b->_34;

    q->_41 = a->_41 + b->_41;
    q->_42 = a->_42 + b->_42;
    q->_43 = a->_43 + b->_43;
    q->_44 = a->_44 + b->_44;
}


/********************************
  Desc: [Q] = [A] * [B].
  두개의 4x4 행렬을 곱한다.
*********************************/
void atMATRIX4_Multiply( atMATRIX4 *q, atMATRIX4 *a, atMATRIX4 *b )
{
/*
    int i, j, k;
//    atREAL *pA = (atREAL *)a->m;
//    atREAL *pB = (atREAL *)b->m;
//    atREAL *pM = (atREAL *)q->m;
    atREAL *pA = (atREAL *)&a->_11;
    atREAL *pB = (atREAL *)&b->_11;
    atREAL *pM = (atREAL *)&q->_11;

    memset( pM, 0, sizeof(atMATRIX4) );

    for( i = 0; i < 4; i++ )
      for( j = 0; j < 4; j++ )
        for( k = 0; k < 4; k++ )
          pM[4*i+j] += REALMUL( pA[4*k+j], pB[4*i+k] );
*/

    atMATRIX4 m;
    atMATRIX4 *pbk = atNULL;

    if( (q == a) || (q == b) ){
      pbk = q;
      q = &m;
    }

    q->_11 = REALMUL(a->_11, b->_11)
           + REALMUL(a->_12, b->_21)
           + REALMUL(a->_13, b->_31)
           + REALMUL(a->_14, b->_41);
    q->_12 = REALMUL(a->_11, b->_12)
           + REALMUL(a->_12, b->_22)
           + REALMUL(a->_13, b->_32)
           + REALMUL(a->_14, b->_42);
    q->_13 = REALMUL(a->_11, b->_13)
           + REALMUL(a->_12, b->_23)
           + REALMUL(a->_13, b->_33)
           + REALMUL(a->_14, b->_43);
    q->_14 = REALMUL(a->_11, b->_14)
           + REALMUL(a->_12, b->_24)
           + REALMUL(a->_13, b->_34)
           + REALMUL(a->_14, b->_44);

    q->_21 = REALMUL(a->_21, b->_11)
           + REALMUL(a->_22, b->_21)
           + REALMUL(a->_23, b->_31)
           + REALMUL(a->_24, b->_41);
    q->_22 = REALMUL(a->_21, b->_12)
           + REALMUL(a->_22, b->_22)
           + REALMUL(a->_23, b->_32)
           + REALMUL(a->_24, b->_42);
    q->_23 = REALMUL(a->_21, b->_13)
           + REALMUL(a->_22, b->_23)
           + REALMUL(a->_23, b->_33)
           + REALMUL(a->_24, b->_43);
    q->_24 = REALMUL(a->_21, b->_14)
           + REALMUL(a->_22, b->_24)
           + REALMUL(a->_23, b->_34)
           + REALMUL(a->_24, b->_44);

    q->_31 = REALMUL(a->_31, b->_11)
           + REALMUL(a->_32, b->_21)
           + REALMUL(a->_33, b->_31)
           + REALMUL(a->_34, b->_41);
    q->_32 = REALMUL(a->_31, b->_12)
           + REALMUL(a->_32, b->_22)
           + REALMUL(a->_33, b->_32)
           + REALMUL(a->_34, b->_42);
    q->_33 = REALMUL(a->_31, b->_13) 
           + REALMUL(a->_32, b->_23) 
           + REALMUL(a->_33, b->_33) 
           + REALMUL(a->_34, b->_43);
    q->_34 = REALMUL(a->_31, b->_14) 
           + REALMUL(a->_32, b->_24) 
           + REALMUL(a->_33, b->_34) 
           + REALMUL(a->_34, b->_44);

    q->_41 = REALMUL(a->_41, b->_11) 
           + REALMUL(a->_42, b->_21) 
           + REALMUL(a->_43, b->_31) 
           + REALMUL(a->_44, b->_41);
    q->_42 = REALMUL(a->_41, b->_12) 
           + REALMUL(a->_42, b->_22) 
           + REALMUL(a->_43, b->_32) 
           + REALMUL(a->_44, b->_42);
    q->_43 = REALMUL(a->_41, b->_13) 
           + REALMUL(a->_42, b->_23) 
           + REALMUL(a->_43, b->_33) 
           + REALMUL(a->_44, b->_43);
    q->_44 = REALMUL(a->_41, b->_14) 
           + REALMUL(a->_42, b->_24) 
           + REALMUL(a->_43, b->_34) 
           + REALMUL(a->_44, b->_44);

    if( pbk ){
      *pbk = m;
    }
    

/*
    atMATRIX4 m;
    atMATRIX4 *pbk = NULL;

    if( (q == a) || (q == b) ){
      pbk = q;
      q = &m;
    }

    q->_11 = REALMUL(a->_11, b->_11)
           + REALMUL(a->_21, b->_12)
           + REALMUL(a->_31, b->_13)
           + REALMUL(a->_41, b->_14);
    q->_12 = REALMUL(a->_12, b->_11)
           + REALMUL(a->_22, b->_12)
           + REALMUL(a->_32, b->_13)
           + REALMUL(a->_42, b->_14);
    q->_13 = REALMUL(a->_13, b->_11)
           + REALMUL(a->_23, b->_12)
           + REALMUL(a->_33, b->_13)
           + REALMUL(a->_43, b->_14);
    q->_14 = REALMUL(a->_14, b->_11)
           + REALMUL(a->_24, b->_12)
           + REALMUL(a->_34, b->_13)
           + REALMUL(a->_44, b->_14);
                                   
    q->_21 = REALMUL(a->_11, b->_21)
           + REALMUL(a->_21, b->_22)
           + REALMUL(a->_31, b->_23)
           + REALMUL(a->_41, b->_24);
    q->_22 = REALMUL(a->_12, b->_21)
           + REALMUL(a->_22, b->_22)
           + REALMUL(a->_32, b->_23)
           + REALMUL(a->_42, b->_24);
    q->_23 = REALMUL(a->_13, b->_21)
           + REALMUL(a->_23, b->_22)
           + REALMUL(a->_33, b->_23)
           + REALMUL(a->_43, b->_24);
    q->_24 = REALMUL(a->_14, b->_21)
           + REALMUL(a->_24, b->_22)
           + REALMUL(a->_34, b->_23)
           + REALMUL(a->_44, b->_24);
                              
    q->_31 = REALMUL(a->_11, b->_31)
           + REALMUL(a->_21, b->_32)
           + REALMUL(a->_31, b->_33)
           + REALMUL(a->_41, b->_34);
    q->_32 = REALMUL(a->_12, b->_31)
           + REALMUL(a->_22, b->_32)
           + REALMUL(a->_32, b->_33)
           + REALMUL(a->_42, b->_34);
    q->_33 = REALMUL(a->_13, b->_31) 
           + REALMUL(a->_23, b->_32) 
           + REALMUL(a->_33, b->_33) 
           + REALMUL(a->_43, b->_34);
    q->_34 = REALMUL(a->_14, b->_31) 
           + REALMUL(a->_24, b->_32) 
           + REALMUL(a->_34, b->_33) 
           + REALMUL(a->_44, b->_34);
                                   
    q->_41 = REALMUL(a->_11, b->_41) 
           + REALMUL(a->_21, b->_42) 
           + REALMUL(a->_31, b->_43) 
           + REALMUL(a->_41, b->_44);
    q->_42 = REALMUL(a->_12, b->_41) 
           + REALMUL(a->_22, b->_42) 
           + REALMUL(a->_32, b->_43) 
           + REALMUL(a->_42, b->_44);
    q->_43 = REALMUL(a->_13, b->_41) 
           + REALMUL(a->_23, b->_42) 
           + REALMUL(a->_33, b->_43) 
           + REALMUL(a->_43, b->_44);
    q->_44 = REALMUL(a->_14, b->_41) 
           + REALMUL(a->_24, b->_42) 
           + REALMUL(a->_34, b->_43) 
           + REALMUL(a->_44, b->_44);

    if( pbk ){
      *pbk = m;
    }
*/    
}


/****************************************************
  전치행렬을 구한다.
*****************************************************/
void atMATRIX4_Transpose( atMATRIX4 *q, atMATRIX4 *a )
{
    atREAL tmp;
    
    if( q == a ){
      SWAP( a->_12, a->_21, tmp );
      SWAP( a->_13, a->_31, tmp );
      SWAP( a->_14, a->_41, tmp );
      SWAP( a->_23, a->_32, tmp );
      SWAP( a->_24, a->_42, tmp );
      SWAP( a->_34, a->_43, tmp );      
    } else {
      q->_11 = a->_11; q->_12 = a->_21; q->_13 = a->_31; q->_14 = a->_41;
      q->_21 = a->_12; q->_22 = a->_22; q->_23 = a->_32; q->_24 = a->_42;
      q->_31 = a->_13; q->_32 = a->_23; q->_33 = a->_33; q->_34 = a->_43;
      q->_41 = a->_14; q->_42 = a->_24; q->_43 = a->_34; q->_44 = a->_44;
    }
}


/****************************************************
  행렬식을 구한다.
*****************************************************/
atREAL atMATRIX4_Determinant( atMATRIX4 *q )
{
    return( 0 );
}


/****************************************************
  역행렬을 구한다.
  단, 반드시 4번째 Row는 [0 0 0 1]로 되어 있어야 한다.
*****************************************************/
atBOOL atMATRIX4_Invert( atMATRIX4 *q, atMATRIX4 *a )
{
    atREAL fDetInv;

    if( ABS(a->_44 - INTTOREAL(1)) > FLOATTOREAL(.001f))
        return( atFALSE );

    if( ABS(a->_14) > FLOATTOREAL(.001f) || ABS(a->_24) > FLOATTOREAL(.001f) || ABS(a->_34) > FLOATTOREAL(.001f) )
        return( atFALSE );

    fDetInv = REALDIV( INTTOREAL(1),
                       REALMUL(a->_11, REALMUL(a->_22, a->_33) - REALMUL(a->_23, a->_32) ) -
                       REALMUL(a->_12, REALMUL(a->_21, a->_33) - REALMUL(a->_23, a->_31) ) +
                       REALMUL(a->_13, REALMUL(a->_21, a->_32) - REALMUL(a->_22, a->_31) ) );

    q->_11 =  REALMUL( fDetInv, REALMUL(a->_22, a->_33) - REALMUL(a->_23, a->_32) );
    q->_12 = -REALMUL( fDetInv, REALMUL(a->_12, a->_33) - REALMUL(a->_13, a->_32) );
    q->_13 =  REALMUL( fDetInv, REALMUL(a->_12, a->_23) - REALMUL(a->_13, a->_22) );
    q->_14 = 0;

    q->_21 = -REALMUL( fDetInv, REALMUL(a->_21, a->_33) - REALMUL(a->_23, a->_31) );
    q->_22 =  REALMUL( fDetInv, REALMUL(a->_11, a->_33) - REALMUL(a->_13, a->_31) );
    q->_23 = -REALMUL( fDetInv, REALMUL(a->_11, a->_23) - REALMUL(a->_13, a->_21) );
    q->_24 = 0;

    q->_31 =  REALMUL( fDetInv, REALMUL(a->_21, a->_32) - REALMUL(a->_22, a->_31) );
    q->_32 = -REALMUL( fDetInv, REALMUL(a->_11, a->_32) - REALMUL(a->_12, a->_31) );
    q->_33 =  REALMUL( fDetInv, REALMUL(a->_11, a->_22) - REALMUL(a->_12, a->_21) );
    q->_34 = 0;

    q->_41 = -( REALMUL(a->_41, q->_11) + REALMUL(a->_42, q->_21) + REALMUL(a->_43, q->_31) );
    q->_42 = -( REALMUL(a->_41, q->_12) + REALMUL(a->_42, q->_22) + REALMUL(a->_43, q->_32) );
    q->_43 = -( REALMUL(a->_41, q->_13) + REALMUL(a->_42, q->_23) + REALMUL(a->_43, q->_33) );
    q->_44 = INTTOREAL(1);

    return( atTRUE );
}
