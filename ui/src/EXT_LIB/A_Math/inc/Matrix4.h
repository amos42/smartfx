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
#ifndef __MATRIX4_H_
#define __MATRIX4_H_


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
    

typedef /*PACKED*/ struct tagatMATRIX4 {
    union {
        struct {
            atREAL    _11, _12, _13, _14;
            atREAL    _21, _22, _23, _24;
            atREAL    _31, _32, _33, _34;
            atREAL    _41, _42, _43, _44;
        };
        atREAL m[4][4];
    };
} atMATRIX4, *atLPMATRIX4;


void        atMATRIX4_Set( atMATRIX4 *m, atREAL _11, atREAL _12, atREAL _13, atREAL _14,
                                         atREAL _21, atREAL _22, atREAL _23, atREAL _24,
                                         atREAL _31, atREAL _32, atREAL _33, atREAL _34,
                                         atREAL _41, atREAL _42, atREAL _43, atREAL _44 );
void        atMATRIX4_Identity( atMATRIX4 *m );
void        atMATRIX4_Add( atMATRIX4 *q, atMATRIX4 *a, atMATRIX4 *b );
void        atMATRIX4_Multiply( atMATRIX4 *q, atMATRIX4 *a, atMATRIX4 *b );
atREAL      atMATRIX4_Determinant( atMATRIX4 *q );
void        atMATRIX4_Transpose( atMATRIX4 *q, atMATRIX4 *a );
atBOOL      atMATRIX4_Invert( atMATRIX4 *q, atMATRIX4 *a );


#ifdef __cplusplus
};
#endif


#endif

