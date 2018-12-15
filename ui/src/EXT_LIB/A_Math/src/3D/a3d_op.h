/*###########################################################################
-----------------------------------------------------------------------------

                                a3d_op.h
                         (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : a3d operation Defined Header
 
-----------------------------------------------------------------------------
###########################################################################*/
#ifndef __A3D_OP_H_
#define __A3D_OP_H_


#include "AT_Types.h"
#include "AT_Real.h"
#include "Vector3.h"
#include "Matrix4.h"


//#define USE_HOMOGENIZE


#ifdef __cplusplus
extern "C" {
#endif
    

typedef struct tagatCOORD2 {
    atINT X, Y;
} atCOORD2;

typedef struct tagatCOORD4 {
    atREAL  x, y, z;
#ifdef USE_HOMOGENIZE
    atREAL  w;
#endif    
} atCOORD4;


void atA3DOP_Multiply( atCOORD4 *d, atMATRIX4 *m, atCOORD4 *s );
void atA3DOP_SetTranslate( atMATRIX4 *m, atREAL tx, atREAL ty, atREAL tz );
void atA3DOP_SetTranslate2( atMATRIX4 *m, atVECTOR3 *v );
void atA3DOP_SetScale( atMATRIX4 *m, atREAL sx, atREAL sy, atREAL sz );
void atA3DOP_SetScale2( atMATRIX4 *m, atVECTOR3 *v );
void atA3DOP_SetRotateX( atMATRIX4 *m, atREAL angle );
void atA3DOP_SetRotateY( atMATRIX4 *m, atREAL angle );
void atA3DOP_SetRotateZ( atMATRIX4 *m, atREAL angle );
void atA3DOP_SetRotate( atMATRIX4 *Result, atVECTOR3 *AxisVec, atREAL Angle );
void atA3DOP_SetRotate2( atMATRIX4 *Result, atREAL x_r, atREAL y_r, atREAL z_r );


#ifdef __cplusplus
};
#endif


#endif
