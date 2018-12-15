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
 Description       : 3x3 Matrix related function definition

-------------------------------------------------------------------------------
#############################################################################*/
#ifndef __MATRIX3_H_
#define __MATRIX3_H_


#include "AT_Real.h"

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif
#if defined(__GNUC__) || defined(__arm)
#pragma anon_unions
#endif

#ifdef __cplusplus
extern "C" {
#endif
    

typedef /*PACKED*/ struct tagatMATRIX3 {
    union {
        struct tagT {
            atREAL    _11, _12, _13;
            atREAL    _21, _22, _23;
            atREAL    _31, _32, _33;
        };
        atREAL m[3][3];
    };
} atMATRIX3, *atLPMATRIX3;


void        atMATRIX3_Set( atMATRIX3 *m, atREAL _11, atREAL _12, atREAL _13,
                                         atREAL _21, atREAL _22, atREAL _23,
                                         atREAL _31, atREAL _32, atREAL _33 );
void        atMATRIX3_Identity( atMATRIX3 *m );
void        atMATRIX3_Add( atMATRIX3 *q, atMATRIX3 *a, atMATRIX3 *b );
void        atMATRIX3_Multiply( atMATRIX3 *q, atMATRIX3 *a, atMATRIX3 *b );
atREAL      atMATRIX3_Determinant( atMATRIX3 *q );
void        atMATRIX3_Transpose( atMATRIX3 *q, atMATRIX3 *a );
atBOOL      atMATRIX3_Invert( atMATRIX3 *q, atMATRIX3 *a );


#ifdef __cplusplus
};
#endif


#endif

