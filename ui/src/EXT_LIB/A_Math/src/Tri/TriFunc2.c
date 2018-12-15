/*###########################################################################
-----------------------------------------------------------------------------

                                TriFunc2.h
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
#include "AT_Types.h"
#include "TriFunc.h"


#define PI_F  3.14159265358979323846
#define PI_R  FLOATTOREAL(PI_F)


#if defined(__USE_TRI_TABLE_)

/*
// 삼각함수용으로 최적화된 Binary Search 루틴.
// (정방향 소트된 것 대상)
// 정확히 일치하지 않더라도 중간 인덱스를 리턴해야 한다.
static int _bsearch( atREAL key, const atREAL *base, int nelem )
{
    atREAL *kmin, *probe;
    atREAL *probe0, min = 0x7FFFFFFF;
    int i, j;

    kmin = (atREAL *)base;
    while( nelem > 0 ){
      i = nelem >> 1; // nelem / 2
      probe = kmin + i;
      j = key - *probe;
      if( j == 0 )
        return( (probe - base) );
      else {
        if( ABS(j) < min ){
          min = ABS(j);
          probe0 = probe;
        }
        if( j < 0 )
          nelem = i;
        else {
          kmin = probe + 1;
          nelem -= i + 1;
        }
      }
    }

    return( (probe0 - base) );
}
*/


/*
// 삼각함수용으로 최적화된 Binary Search 루틴.
// (역방향 소트된 것 대상. 속도를 위해 일부러 따로 만듦)
// 정확히 일치하지 않더라도 중간 인덱스를 리턴해야 한다.
static int _bsearch2( atREAL key, const atREAL *base, int nelem )
{
    atREAL *kmin, *probe;
    atREAL *probe0, min = 0x7FFFFFFF;
    int i, j;

    kmin = (atREAL *)base;
    while( nelem > 0 ){
      i = nelem >> 1; // nelem / 2
      probe = kmin + i;
      j = *probe - key;
      if( j == 0 )
        return( (probe - base) );
      else {
        if( ABS(j) < min ){
          min = ABS(j);
          probe0 = probe;
        }
        if( j < 0 )
          nelem = i;
        else {
          kmin = probe + 1;
          nelem -= i + 1;
        }
      }
    }

    return( (probe0 - base) );
}
*/


/*
 * Code for approximation of cos, sin, tan and inv sin, etc.
 * Surprisingly accurate and very usable.
 *
 * Domain:
 * Sin/Cos    [0, pi/2]
 * Tan        [0,pi/4]
 * InvSin/Cos [0, 1]
 * InvTan     [-1, 1]

float fast_sin(const float val)
{
    float fASqr = val*val;
    float fResult = -2.39e-08f;
    fResult *= fASqr;
    fResult += 2.7526e-06f;
    fResult *= fASqr;
    fResult -= 1.98409e-04f;
    fResult *= fASqr;
    fResult += 8.3333315e-03f;
    fResult *= fASqr;
    fResult -= 1.666666664e-01f;
    fResult *= fASqr;
    fResult += 1.0f;
    fResult *= val;

    return fResult;
}

float fast_cos(const float val)
{
    float fASqr = val*val;
    float fResult = -2.605e-07f;
    fResult *= fASqr;
    fResult += 2.47609e-05f;
    fResult *= fASqr;
    fResult -= 1.3888397e-03f;
    fResult *= fASqr;
    fResult += 4.16666418e-02f;
    fResult *= fASqr;
    fResult -= 4.999999963e-01f;
    fResult *= fASqr;
    fResult += 1.0f;

    return fResult;  
}

float fast_tan(const float val)
{
    float fASqr = val*val;
    float fResult = 9.5168091e-03f;
    fResult *= fASqr;
    fResult += 2.900525e-03f;
    fResult *= fASqr;
    fResult += 2.45650893e-02f;
    fResult *= fASqr;
    fResult += 5.33740603e-02f;
    fResult *= fASqr;
    fResult += 1.333923995e-01f;
    fResult *= fASqr;
    fResult += 3.333314036e-01f;
    fResult *= fASqr;
    fResult += 1.0f;
    fResult *= val;

    return fResult;

}

float fast_asin(float val)
{
    float fRoot = sqrt(1.0f-val);
    float fResult = -0.0187293f;
    fResult *= val;
    fResult += 0.0742610f;
    fResult *= val;
    fResult -= 0.2121144f;
    fResult *= val;
    fResult += 1.5707288f;
    fResult = SGD_PI_2 - fRoot*fResult;

    return fResult;
}

float fast_acos(float val)
{
    float fRoot = sqrt(1.0f-val);
    float fResult = -0.0187293f;
    fResult *= val;
    fResult += 0.0742610f;
    fResult *= val;
    fResult -= 0.2121144f;
    fResult *= val;
    fResult += 1.5707288f;
    fResult *= fRoot;

    return fResult;
}

float fast_atan(float val)
{
    float fVSqr = val*val;
    float fResult = 0.0028662257f;
    fResult *= fVSqr;
    fResult -= 0.0161657367f;
    fResult *= fVSqr;
    fResult += 0.0429096138f;
    fResult *= fVSqr;
    fResult -= 0.0752896400f;
    fResult *= fVSqr;
    fResult += 0.1065626393f;
    fResult *= fVSqr;
    fResult -= 0.1420889944f;
    fResult *= fVSqr;
    fResult += 0.1999355085f;
    fResult *= fVSqr;
    fResult -= 0.3333314528f;
    fResult *= fVSqr;
    fResult += 1.0f;
    fResult *= val;

    return fResult;
}
*/


/*
atREAL fast_asin( atREAL value )
{
    int i;

    // Sin은 0~90도까지 소트되어 있으므로...
	if( value >= 0 ){
		i = _bsearch( value, &SinTable[0], 90*TRI_1DEG_RATIO+1 );
	} else {
		i = -_bsearch( -value, &SinTable[0], 90*TRI_1DEG_RATIO+1 );
	}
    
    return( INTTOREAL(i) / TRI_1DEG_RATIO );
}

atREAL fast_acos( atREAL value )
{
    int i;

    // Sin은 0~90도까지 소트되어 있고, Cos과 90도 차이 나므로...
	if( value > 0 ){
		i = 90*TRI_1DEG_RATIO - _bsearch( value, &SinTable[0], 90*TRI_1DEG_RATIO+1 );
	} else {
		i = _bsearch( -value, &SinTable[0], 90*TRI_1DEG_RATIO+1 ) - 90*TRI_1DEG_RATIO;
	}

    return( INTTOREAL(i) / TRI_1DEG_RATIO );
}

atREAL fast_atan( atREAL value )
{
    int i;         

	if( value == 0 ) return INTTOREAL(0);

    // Tan은 0~90도까지 소트되어 있으므로...
	if( value >= 0 ){
		i = _bsearch( value, &TanTable[0], (90)*TRI_1DEG_RATIO+1 );
	} else {
		i = 180*TRI_1DEG_RATIO - _bsearch( -value, &TanTable[0], (90)*TRI_1DEG_RATIO+1 );
	}

    return( INTTOREAL(i) / TRI_1DEG_RATIO ); 
}

atREAL fast_atan2( atREAL dx, atREAL dy )
{
	atREAL r;
	
	if( dy == INTTOREAL(0) ){
	  return (dx >= INTTOREAL(0))? INTTOREAL(0) : INTTOREAL(180);
	} else if( dx == INTTOREAL(0) ){ 
	  return (dy > INTTOREAL(0))? INTTOREAL(90) : INTTOREAL(270);
	}
	
       r = ATAN( REALDIV(dy, dx) );
	if( dy < 0 ){
		r += INTTOREAL(180);
	}
	return r;
}
*/


#define AS1			FLOATTOREAL(-0.0187293f);
#define AS2			FLOATTOREAL(0.0742610f); 
#define AS3			FLOATTOREAL(0.2121144f); 
#define AS4			FLOATTOREAL(1.5707288f);

atREAL fast_asin( atREAL value )
{
	atREAL fRoot, result;
	atREAL angle;
	int sign = (value >= 0)? 1 : -1;

	if( value < 0 ) value = -value;

	fRoot = REALSQRT( INTTOREAL(1) - value );
	result = AS1;

	result = REALMUL(result, value);
	result += AS2;
	result = REALMUL(result, value);
	result -= AS3;
	result = REALMUL(result, value);
	result += AS4;
	result = REALHALF(PI_R) - (REALMUL(fRoot, result));

	angle = (sign >= 0)? result : -result;

	return REALDIV( angle * 180, PI_R );
}


atREAL fast_acos( atREAL value )
{
	atREAL fRoot, result;
	atREAL angle;
	int sign = (value >= 0)? 1 : -1;

	if( value < 0 ) value = -value;

	fRoot = REALSQRT( INTTOREAL(1) - value );
	result = AS1;

	result = REALMUL(result, value);
	result += AS2;
	result = REALMUL(result, value);
	result -= AS3;
	result = REALMUL(result, value);
	result += AS4;
	result = REALMUL(fRoot, result);

	angle = (sign >= 0)? result : (REALHALF(PI_R) + result);

	return REALDIV( angle * 180, PI_R );
}



/*
#define EPSILON		1							// 1E-10
#define AT1			FLOATTOREAL(PI_F / 4)		//  51472		// PI/4
#define	AT2			FLOATTOREAL(PI_F * 3 / 4)   // 154416		// PI/4*3

atREAL fast_atan2( atREAL x, atREAL y )
{
	atREAL angle;
	atREAL abs_y = ABS(y) + EPSILON;	// kludge to prevent 0/0 condition

	if(x >= 0)
		angle = AT1 - REALMUL(AT1, REALDIV(x - abs_y, x + abs_y));
	else
		angle = AT2 - REALMUL(AT1, REALDIV(x + abs_y, abs_y - x));
	
	angle = REALDIV( angle * 180, PI_R );

	return (y < 0)? -angle : angle;
}
*/
#define _EPSILON  1	// 1E-10

atREAL fast_atan2( atREAL x, atREAL y )
{
	atREAL angle;
	atREAL abs_y = ABS(y) + _EPSILON;	// kludge to prevent 0/0 condition

	if(x >= 0)
		angle = INTTOREAL(1) - REALDIV(x - abs_y, x + abs_y);
	else
		angle = INTTOREAL(3) - REALDIV(x + abs_y, abs_y - x);
	
	angle = REALQUART( angle * 180 );

	return (y >= 0)? angle : -angle;
}

atREAL fast_atan( atREAL value )
{
    return fast_atan2( 1, value ); 
}

#else

#include <math.h>

atREAL fast_asin( atREAL value )
{
    return asin( value ) * 180 / PI_F;
}


atREAL fast_acos( atREAL value )
{
    return acos( value ) * 180 / PI_F;
}


atREAL fast_atan( atREAL value )
{
    atREAL v = atan( value ) * 180 / PI_F;
    if( v < 0 ) v += INTTOREAL(360);
    return v;
}

atREAL fast_atan2( atREAL dx, atREAL dy )
{
	atREAL r;
	
	if( dy == INTTOREAL(0) ){
	  return (dx >= INTTOREAL(0))? INTTOREAL(0) : INTTOREAL(180);
	} else if( dx == INTTOREAL(0) ){ 
	  return (dy > INTTOREAL(0))? INTTOREAL(90) : INTTOREAL(270);
	}
	
       r = ATAN( REALDIV(dy, dx) );
	if( dx < 0 ){
		r += INTTOREAL(180);
	}
	if( r > INTTOREAL(360) ) r -= INTTOREAL(360);
	
	return r;
}


#endif


