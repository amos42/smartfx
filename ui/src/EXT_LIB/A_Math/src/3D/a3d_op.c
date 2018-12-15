/*###########################################################################
-----------------------------------------------------------------------------

                                a3d_op.c
                         (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : a3d operation related function definition
 
-----------------------------------------------------------------------------
###########################################################################*/

#include "AT_Real.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "TriFunc.h"
#include "a3d_op.h"
#include <stdio.h>


/********************************************
  좌표와 행렬을 곱해서 변환된 좌표를 얻는다.
  단, 동치 좌표계는 지원하지 않는다.
*********************************************/
void atA3DOP_Multiply( atCOORD4 *d, atMATRIX4 *m, atCOORD4 *s )
{
    atREAL x, y, z;
#ifdef USE_HOMOGENIZE
    atREAL w;
#endif    

    x = s->x;
    y = s->y;
    z = s->z;

#ifndef USE_HOMOGENIZE
    d->x = REALMUL(m->_11, x) + REALMUL(m->_12, y) + REALMUL(m->_13, z) + m->_14;
    d->y = REALMUL(m->_21, x) + REALMUL(m->_22, y) + REALMUL(m->_23, z) + m->_24;
    d->z = REALMUL(m->_31, x) + REALMUL(m->_32, y) + REALMUL(m->_33, z) + m->_34;
#else
    w = s->w;

    d->x = REALMUL(m->_11, x) + REALMUL(m->_12, y) + REALMUL(m->_13, z) + REALMUL(m->_14, w);
    d->y = REALMUL(m->_21, x) + REALMUL(m->_22, y) + REALMUL(m->_23, z) + REALMUL(m->_24, w);
    d->z = REALMUL(m->_31, x) + REALMUL(m->_32, y) + REALMUL(m->_33, z) + REALMUL(m->_34, w);
    
// 동치 좌표계를 1로 유지할 것인가, 그냥 둘 것인가?
#if 0    
    d->w = REALMUL(m->_41, w) + REALMUL(m->_42, w) + REALMUL(m->_43, w) + REALMUL(m->_44, w);
#else    
    w    = REALMUL(m->_41, w) + REALMUL(m->_42, w) + REALMUL(m->_43, w) + REALMUL(m->_44, w);
    //동치 좌표의 W를 무조건 1로 만든다.
    if( w != INTTOREAL(1) ){
      d->x = REALDIV( d->x, w );
      d->y = REALDIV( d->y, w );
      d->z = REALDIV( d->z, w );
    }
    d->w = INTTOREAL(1);
#endif
#endif
}



void atA3DOP_SetTranslate( atMATRIX4 *m, atREAL tx, atREAL ty, atREAL tz )
{
	atMATRIX4_Identity( m );

	m->_14 = tx;
	m->_24 = ty;
	m->_34 = tz;
}



void atA3DOP_SetScale( atMATRIX4 *m, atREAL sx, atREAL sy, atREAL sz )
{
	atMATRIX4_Identity( m );

	m->_11 = sx;
	m->_22 = sy;
	m->_33 = sz;
}


void atA3DOP_SetTranslate2( atMATRIX4 *m, atVECTOR3 *v )
{
	atA3DOP_SetTranslate( m, v->i, v->j, v->k );
}



void atA3DOP_SetScale2( atMATRIX4 *m, atVECTOR3 *v )
{
	atA3DOP_SetScale( m, v->i, v->j, v->k );
}



// 회전값을 벡터와 곱해서 행렬에 저장한다.
void atA3DOP_SetRotateX( atMATRIX4 *m, atREAL angle )
{
    atREAL c, s;

    if( angle < 0 ) angle += MAX_TRI_ANGLE;

    c = COS(angle);
    s = SIN(angle);

	m->_11 = INTTOREAL(1);
    m->_12 = 0;
    m->_13 = 0;
    m->_14 = 0;

    m->_21 = 0;
    m->_22 = c;
    m->_23 = -s;
    m->_24 = 0;

    m->_31 = 0;
    m->_32 = s;
    m->_33 = c;
    m->_34 = 0;

	m->_41 = 0;
	m->_42 = 0;
	m->_43 = 0;
	m->_44 = INTTOREAL(1);
}


// 회전값을 벡터와 곱해서 행렬에 저장한다.
void atA3DOP_SetRotateY( atMATRIX4 *m, atREAL angle )
{
    atREAL c, s;

    if( angle < 0 ) angle += MAX_TRI_ANGLE;

    c = COS(angle);
    s = SIN(angle);

	m->_11 = c;
    m->_12 = 0;
    m->_13 = s;
    m->_14 = 0;

    m->_21 = 0;
    m->_22 = INTTOREAL(1);
    m->_23 = 0;
    m->_24 = 0;

    m->_31 = -s;
    m->_32 = 0;
    m->_33 = c;
    m->_34 = 0;

	m->_41 = 0;
	m->_42 = 0;
	m->_43 = 0;
	m->_44 = INTTOREAL(1);
}



// 회전값을 벡터와 곱해서 행렬에 저장한다.
void atA3DOP_SetRotateZ( atMATRIX4 *m, atREAL angle )
{
    atREAL c, s;

    if( angle < 0 ) angle += MAX_TRI_ANGLE;

    c = COS(angle);
    s = SIN(angle);

	m->_11 = c;
    m->_12 = -s;
    m->_13 = 0;
    m->_14 = 0;

    m->_21 = s;
    m->_22 = c;
    m->_23 = 0;
    m->_24 = 0;

    m->_31 = 0;
    m->_32 = 0;
    m->_33 = INTTOREAL(1);
    m->_34 = 0;

	m->_41 = 0;
	m->_42 = 0;
	m->_43 = 0;
	m->_44 = INTTOREAL(1);
}




#if 0

// 사원수 관련 함수들...

//-----------------------------------------------------------------------------
// Name: D3DMath_QuaternionFromRotation()
// Desc: Converts a normalized axis and angle to a unit quaternion.
//-----------------------------------------------------------------------------
atVOID atD3DMath_QuaternionFromRotation( atREAL *x, atREAL *y, atREAL *z, atREAL *w,
                                     atVECTOR3 *v, atREAL fTheta )
{
    REAL sinValue = (REAL)sin(fTheta/2);
    *x = sinValue * v->I;
    *y = sinValue * v->J;
    *z = sinValue * v->K;
    *w = (REAL)cos(fTheta/2);
}



//-----------------------------------------------------------------------------
// Name: D3DMath_RotationFromQuaternion()
// Desc: Converts a normalized axis and angle to a unit quaternion.
//-----------------------------------------------------------------------------
atVOID atD3DMath_RotationFromQuaternion( atVECTOR3 *v, atREAL *fTheta,
                                     atREAL x, atREAL y, atREAL z, atREAL w )

{
    //fTheta = (REAL)( acos(w) * 2 );
    //v->I    = (REAL)( x / sin(fTheta/2) );
    //v->J    = (REAL)( y / sin(fTheta/2) );
    //v->K    = (REAL)( z / sin(fTheta/2) );

	atREAL sinValue = REALDIV( INTTOREAL(1), SIN(fTheta / 2) );
    fTheta = (REAL)( acos(w) * 2 );
    v->I    = (REAL)( x * sinValue );
    v->J    = (REAL)( y * sinValue );
    v->K    = (REAL)( z * sinValue );
}


//-----------------------------------------------------------------------------
// Name: D3DMath_QuaternionFromAngles()
// Desc: Converts euler angles to a unit quaternion.
//-----------------------------------------------------------------------------
atVOID D3DMath_QuaternionFromAngles( atREAL *x, atREAL *y, atREAL *z, atREAL *w,
                                   atREAL fYaw, atREAL fPitch, atREAL fRoll )

{
    REAL fSinYaw   = SIN(fYaw/2);
    REAL fSinPitch = SIN(fPitch/2);
    REAL fSinRoll  = SIN(fRoll/2);
    REAL fCosYaw   = SIN(fYaw/2);
    REAL fCosPitch = SIN(fPitch/2);
    REAL fCosRoll  = SIN(fRoll/2);

    x = fSinRoll * fCosPitch * fCosYaw - fCosRoll * fSinPitch * fSinYaw;
    y = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
    z = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
    w = fCosRoll * fCosPitch * fCosYaw + fSinRoll * fSinPitch * fSinYaw;
}



//-----------------------------------------------------------------------------
// Name: D3DMath_atMATRIX4FromQuaternion()
// Desc: Converts a unit quaternion into a rotation atMATRIX4.
//-----------------------------------------------------------------------------
atVOID D3DMath_atMATRIX4FromQuaternion( atMATRIX4 *mat, atREAL x, atREAL y, atREAL z, atREAL w )
{
    atREAL xx = x*x; atREAL yy = y*y; atREAL zz = z*z;
    atREAL xy = x*y; atREAL xz = x*z; atREAL yz = y*z;
    atREAL wx = w*x; atREAL wy = w*y; atREAL wz = w*z;

    mat->_11 = 1 - 2 * ( yy + zz );
    mat->_12 =     2 * ( xy - wz );
    mat->_13 =     2 * ( xz + wy );

    mat->_21 =     2 * ( xy + wz );
    mat->_22 = 1 - 2 * ( xx + zz );
    mat->_23 =     2 * ( yz - wx );

    mat->_31 =     2 * ( xz - wy );
    mat->_32 =     2 * ( yz + wx );
    mat->_33 = 1 - 2 * ( xx + yy );

    mat->_14 = mat->_24 = mat->_34 = 0.0f;
    mat->_41 = mat->_42 = mat->_43 = 0.0f;
    mat->_44 = 1.0f;
}


//-----------------------------------------------------------------------------
// Name: D3DMath_QuaternionFromatMATRIX4()
// Desc: Converts a rotation atMATRIX4 into a unit quaternion.
//-----------------------------------------------------------------------------
VOID D3DMath_QuaternionFromatMATRIX4( REAL *x, REAL *y, REAL *z, REAL& w, atMATRIX4 *mat )
{
    if( mat->_11 + mat->_22 + mat->_33 > 0.0f )
    {
        REAL s = (REAL)sqrt( mat->_11 + mat->_22 + mat->_33 + mat->_44 );

        *x = (mat->_23-mat->_32) / (2*s);
        *y = (mat->_31-mat->_13) / (2*s);
        *z = (mat->_12-mat->_21) / (2*s);
        *w = 0.5f * s;
    } else {


    }
    REAL xx = x*x; REAL yy = y*y; REAL zz = z*z;
    REAL xy = x*y; REAL xz = x*z; REAL yz = y*z;
    REAL wx = w*x; REAL wy = w*y; REAL wz = w*z;

    mat->_11 = 1 - 2 * ( yy + zz );
    mat->_12 =     2 * ( xy - wz );
    mat->_13 =     2 * ( xz + wy );

    mat->_21 =     2 * ( xy + wz );
    mat->_22 = 1 - 2 * ( xx + zz );
    mat->_23 =     2 * ( yz - wx );

    mat->_31 =     2 * ( xz - wy );
    mat->_32 =     2 * ( yz + wx );
    mat->_33 = 1 - 2 * ( xx + yy );

    mat->_14 = mat->_24 = mat->_34 = 0.0f;
    mat->_41 = mat->_42 = mat->_43 = 0.0f;
    mat->_44 = 1.0f;
}


//-----------------------------------------------------------------------------
// Name: D3DMath_QuaternionMultiply()
// Desc: Mulitples two quaternions together as in {Q} = {A} * {B}.
//-----------------------------------------------------------------------------
VOID D3DMath_QuaternionMultiply( REAL *Qx, REAL *Qy, REAL *Qz, REAL *Qw,
                                  REAL Ax, REAL Ay, REAL Az, REAL Aw,
                                  REAL Bx, REAL By, REAL Bz, REAL Bw )
{
	REAL Dx = Bw*Ax + Bx*Aw + By*Az - Bz*Ay;
    REAL Dy = Bw*Ay + By*Aw + Bz*Ax - Bx*Az;
    REAL Dz = Bw*Az + Bz*Aw + Bx*Ay - By*Ax;
    REAL Dw = Bw*Aw - Bx*Ax - By*Ay - Bz*Az;

    *Qx = Dx; *Qy = Dy; *Qz = Dz; *Qw = Dw;
}


//-----------------------------------------------------------------------------
// Name: D3DMath_SlerpQuaternions()
// Desc: Compute a quaternion which is the spherical linear interpolation
//       between two other quaternions by dvFraction.
//-----------------------------------------------------------------------------
VOID D3DMath_QuaternionSlerp( REAL& Qx, REAL& Qy, REAL& Qz, REAL& Qw,
                              REAL Ax, REAL Ay, REAL Az, REAL Aw,
                              REAL Bx, REAL By, REAL Bz, REAL Bw,
                              REAL fAlpha )
{
    REAL fScale1;
    REAL fScale2;

    // Compute dot product, aka cos(theta):
    REAL fCosTheta = Ax*Bx + Ay*By + Az*Bz + Aw*Bw;

    if( fCosTheta < 0.0f )
    {
        // Flip start quaternion
		Ax = -Ax; Ay = -Ay; Az = -Az; Aw = -Aw;
        fCosTheta = -fCosTheta;
    }

    if( fCosTheta + 1.0f > 0.05f )
    {
        // If the quaternions are close, use linear interploation
        if( 1.0f - fCosTheta < 0.05f )
        {
            fScale1 = 1.0f - fAlpha;
            fScale2 = fAlpha;
        }
        else // Otherwise, do spherical interpolation
        {
            REAL fTheta    = (REAL)acos( fCosTheta );
            REAL fSinTheta = (REAL)sin( fTheta );

            fScale1 = (REAL)sin( fTheta * (1.0f-fAlpha) ) / fSinTheta;
            fScale2 = (REAL)sin( fTheta * fAlpha ) / fSinTheta;
        }
    }
    else
    {
        Bx = -Ay;
        By =  Ax;
        Bz = -Aw;
        Bw =  Az;

		fScale1 = (REAL)sin( g_PI_DIV_2 * (1.0f - fAlpha) );
        fScale2 = (REAL)sin( g_PI_DIV_2 * fAlpha );
    }

    Qx = fScale1 * Ax + fScale2 * Bx;
    Qy = fScale1 * Ay + fScale2 * By;
    Qz = fScale1 * Az + fScale2 * Bz;
    Qw = fScale1 * Aw + fScale2 * Bw;
}


#endif




/********************************************
  특정 축으로 회전하는 행렬 생성.
*********************************************/
void atA3DOP_SetRotate( atMATRIX4 *Result, atVECTOR3 *AxisVec, atREAL Angle )
{
    atVECTOR3  Vector;
    atREAL	cos, sin;
    atREAL	a, b, c;

    if( Angle < 0 ) Angle += MAX_TRI_ANGLE;
    if( Angle >= MAX_TRI_ANGLE ) Angle -= MAX_TRI_ANGLE;

    sin = (atREAL)SIN( Angle / 2 );
    cos = (atREAL)COS( Angle / 2 );

/*
{
atMATRIX4 rx, ry, rl, rx_1, ry_1;    
atMATRIX4 tm, tm2;    
REAL d;

d = REALSQRT(x*x + y*y + z*z);
x /= d;
y /= d;
z /= d;

d = REALSQRT(y*y + z*z);

atMATRIX4_Set( &rx,
1, 0, 0, 0,
0, z/d, y/d, 0,
0, -y/d, z/d, 0,
0, 0, 0, 1 );

atMATRIX4_Set( &rx_1,
1, 0, 0, 0,
0, z/d, -y/d, 0,
0, y/d, z/d, 0,
0, 0, 0, 1 );

atMATRIX4_Set( &ry,
d, 0, x, 0,
0, 1, 0, 0,
-x, 0, d, 0,
0, 0, 0, 1 );

atMATRIX4_Set( &ry_1,
d, 0, -x, 0,
0, 1, 0, 0,
x, 0, d, 0,
0, 0, 0, 1 );

atMATRIX4_Set( &rl,
cos, sin, 0, 0,
-sin, cos, 0, 0,
0, 0, 1, 0,
0, 0, 0, 1 );

atMATRIX4_Multiply( &tm, &rx, &ry );
atMATRIX4_Multiply( &tm2, &tm, &rl );
atMATRIX4_Multiply( &tm, &tm2, &ry_1 );
atMATRIX4_Multiply( Result, &tm, &rx_1 );
return;
}
*/


    Vector = *AxisVec;

	atVECTOR3_Normalize( &Vector );

    a = REALMUL( Vector.i, sin );
    b = REALMUL( Vector.j, sin );
    c = REALMUL( Vector.k, sin );

//    atMATRIX4_Identity( Result );

/*
	Result->_11 = INTTOREAL(1) - 2 * (REALMUL(b,b) + REALMUL(c,c));
    Result->_12 =           	 2 * (REALMUL(a,b) - REALMUL(cos,c));
    Result->_13 =	             2 * (REALMUL(a,c) + REALMUL(cos,b));
    Result->_14 = 0;

    Result->_21 =	             2 * (REALMUL(a,b) + REALMUL(cos,c));
    Result->_22 = INTTOREAL(1) - 2 * (REALMUL(a,a) + REALMUL(c,c));
    Result->_23 =	             2 * (REALMUL(b,c) - REALMUL(cos,a));
    Result->_24 = 0;

    Result->_31 =	             2 * (REALMUL(a,c) - REALMUL(cos,b));
    Result->_32 =	             2 * (REALMUL(b,c) + REALMUL(cos,a));
    Result->_33 = INTTOREAL(1) - 2 * (REALMUL(a,a) + REALMUL(b,b));
    Result->_34 = 0;

	Result->_41 = 0;
	Result->_42 = 0;
    Result->_43 = 0;
    Result->_44 = INTTOREAL(1);
*/
    
	Result->_11 = INTTOREAL(1) - 2 * (REALMUL(b,b) + REALMUL(c,c));
    Result->_12 =	             2 * (REALMUL(a,b) + REALMUL(cos,c));
    Result->_13 =	             2 * (REALMUL(a,c) - REALMUL(cos,b));
	Result->_14 = 0;

    Result->_21 =           	 2 * (REALMUL(a,b) - REALMUL(cos,c));
    Result->_22 = INTTOREAL(1) - 2 * (REALMUL(a,a) + REALMUL(c,c));
    Result->_23 =	             2 * (REALMUL(b,c) + REALMUL(cos,a));
	Result->_24 = 0;

    Result->_31 =	             2 * (REALMUL(a,c) + REALMUL(cos,b));
    Result->_32 =	             2 * (REALMUL(b,c) - REALMUL(cos,a));
    Result->_33 = INTTOREAL(1) - 2 * (REALMUL(a,a) + REALMUL(b,b));
    Result->_34 = 0;

    Result->_41 = 0;
    Result->_42 = 0;
    Result->_43 = 0;
    Result->_44 = INTTOREAL(1);
}


/********************************************
  x, y, z축으로 각각 회전하는 행렬 생성
*********************************************/
void atA3DOP_SetRotate2( atMATRIX4 *Result, atREAL x_r, atREAL y_r, atREAL z_r )
{
    atMATRIX4 tm, m1, m2;
    
    atA3DOP_SetRotateX( &m1, x_r );
    atA3DOP_SetRotateY( &m2, y_r );
    atMATRIX4_Multiply( &tm, &m1, &m2 );
    
    atA3DOP_SetRotateZ( &m1, z_r );
    atMATRIX4_Multiply( Result, &tm, &m1 );
}

