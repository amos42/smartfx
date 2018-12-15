/*#############################################################################
-------------------------------------------------------------------------------

                                   Vector3.h
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
#ifndef __VECTOR3_H_
#define __VECTOR3_H_


#include "AT_Types.h"
#include "AT_Real.h"


#ifdef __cplusplus
extern "C" {
#endif


// 다른 구조 내에 쓰일 때를 대비해 Packed로 한다.
typedef /*PACKED*/ struct tagatVECTOR3 {
    atREAL i, j, k;
} atVECTOR3;


void    atVECTOR3_Set( atVECTOR3 *t, atREAL i, atREAL j, atREAL k );
void    atVECTOR3_Inverse( atVECTOR3 *t );
void    atVECTOR3_Add( atVECTOR3 *t, atVECTOR3 *u, atVECTOR3 *v );
void    atVECTOR3_Subtract( atVECTOR3 *t, atVECTOR3 *u, atVECTOR3 *v );
atREAL  atVECTOR3_GetNorm( atVECTOR3 *v );
void    atVECTOR3_Normalize( atVECTOR3 *v );
#ifdef __FIXEDPTR_
atREAL  atVECTOR3_GetNorm64( atVECTOR3 *v );
void    atVECTOR3_Normalize64( atVECTOR3 *v );
#endif
atREAL  atVECTOR3_DotProduct( atVECTOR3 *u, atVECTOR3 *v );
void    atVECTOR3_CrossProduct( atVECTOR3 *t, atVECTOR3 *u, atVECTOR3 *v );
void    atVECTOR3_ScalarProduct( atVECTOR3 *t, atVECTOR3 *v, atREAL scalar );
void    atVECTOR3_ScalarProduct_i( atVECTOR3 *t, atVECTOR3 *v, atINT i_scalar );


#ifdef __cplusplus
};
#endif


#endif

