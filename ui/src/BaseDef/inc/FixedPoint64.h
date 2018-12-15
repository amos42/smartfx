// FixedPoint64.h
//
/////////////////////////////////////////////////////////////////////////////

// 44.20 고정 소수점 변환기

#ifndef FIXEDPOINT64_H
#define FIXEDPOINT64_H

#include "AT_Types.h"

//#define USE_FIXEDPOINT64

#ifdef USE_FIXEDPOINT64

#error aaa
typedef atINT64				fixed;

#define FIXED_SHIFT			20
#define FIXED_SCALE			1048576.0f	// 2^FIXED_SHIFT

#define FP_HALF_ONE			524288
#define FP_ONE				1048576
#define FP_HALF_THREE		1572864
#define FP_TWO				2097152
#define FP_EPSILON			1

#define FP_PI				3294198				// FLOAT2FP(3.14159265f)	// PI
#define FP_TWO_PI			6588397				// FLOAT2FP(6.28318531f)	// 2*PI
#define FP_HALF_PI			1647099				// FLOAT2FP(1.57079633f)	// PI/2

#define FPDEG2RAD(d)		(FPMUL(d, 18301))	// FLOAT2FP(PI / 180.0f) = 18301

#define INT2FP(x)			((fixed)((atINT64)(x) << FIXED_SHIFT))
#define FP2INT(x)			((int)((fixed)(x) >> FIXED_SHIFT))
#define FLOAT2FP(x)			((fixed)((float)(x) * FIXED_SCALE))
#define FP2FLOAT(x)			((float)((fixed)(x) / FIXED_SCALE))

#define FPMUL(a, b)			((fixed)((((atINT64)(a)) * ((atINT64)(b))) >> FIXED_SHIFT))
#define FPDIV(a, b)			((fixed)((((atINT64)(a)) << FIXED_SHIFT) / (atINT64)(b)))
#define FPMULDIV(a, b, c)	((fixed)(((atINT64)(a)) * ((atINT64)(b)) / (atINT64)(c)))

#define FPHALF(x)			((fixed)(x) >> 1)
#define FPSQR(x)			(FPMUL(x, x))

#define FIXED_MASK			((1 << FIXED_SHIFT) - 1)
#define FPFIXEDK(x)			((fixed)((x) & FIXED_MASK))
#define FPINTK(x)			((fixed)((x) & (~FIXED_MASK)))
#define FP2INT_R(x)			FP2INT((x) + (FIXED_MASK/2 - 1))

#define SC_ALPHA			1335088		//83443
#define SC_BETA				424972		//26561
#define SC_GAMMA			235930		//14746

#define AS1					19648		//1228
#define AS2					77856		//4866
#define AS3					222416		//13901
#define AS4					1647024		//102939
#define AT1					823550		// PI/4
#define	AT2					2470649		// PI/4*3

#else

#include <math.h>

typedef double				fixed;

#define FP_HALF_ONE			0.5f
#define FP_ONE				1.0f
#define FP_HALF_THREE		1.5f
#define FP_TWO				2.0f
#define FP_EPSILON			0.000001f

#define FP_PI				FLOAT2FP(3.14159265f)	// PI
#define FP_TWO_PI			FLOAT2FP(6.28318531f)	// 2*PI
#define FP_HALF_PI			FLOAT2FP(1.57079633f)	// PI/2

#define FPDEG2RAD(d)		(FPMUL(d, FLOAT2FP(FP_PI / 180.0f)))

#define INT2FP(x)			((fixed)(x))
#define FP2INT(x)			((int)(floor(x)))
#define FLOAT2FP(x)			((fixed)(x))
#define FP2FLOAT(x)			((float)((fixed)(x)))

#define FPMUL(a, b)			((fixed)((fixed)(a) * (fixed)(b)))
#define FPDIV(a, b)			((fixed)((fixed)(a) / (fixed)(b)))
#define FPMULDIV(a, b, c)	((fixed)((fixed)(a) * (fixed)(b) / (fixed)(c)))

#define FPHALF(x)			((fixed)(x) * 0.5f)
#define FPSQR(x)			((fixed)(x) * (fixed)(x))

#define FPFIXEDK(x)			((fixed)(x) - (int)(x))
#define FPINTK(x)			((int)(x))
#define FP2INT_R(x)			((int)floor((fixed)(x) + 0.5f))

#define SC_ALPHA			FLOAT2FP(1.27323954f)	//83443
#define SC_BETA				FLOAT2FP(0.405284735f)	//26561
#define SC_GAMMA			FLOAT2FP(0.225f)		//14746

#define AS1					FLOAT2FP(0.018738f)		//1228
#define AS2					FLOAT2FP(0.074249f)		//4866
#define AS3					FLOAT2FP(0.212112f)		//13901
#define AS4					FLOAT2FP(1.570724f)		//102939
#define AT1					FLOAT2FP(0.78539816f)	// PI/4
#define	AT2					FLOAT2FP(2.35619449f)	// PI/4*3

#define FPSQRT(x)           sqrt(x * x)

#endif

#ifndef ABS
#define ABS(a)				(((a)>=(0))?(a):-(a))
#endif

#define UNPACK16_R(C)		((atBYTE)(((C) >> 8) & 0xF8))
#define UNPACK16_G(C)		((atBYTE)(((C) >> 3) & 0xFC))
#define UNPACK16_B(C)		((atBYTE)(((C) << 3) & 0xF8))
#define PACK16(R, G, B)		((atWORD)(((((atWORD)((R) & 0xF8)) << 8)) | ((((atWORD)((G) & 0xFC)) << 3)) | (((atWORD)((B) & 0xF8)) >> 3)))

#define UNPACK24_A(C)		(int)(((C)>>24)&0xFF)
#define UNPACK24_R(C)		(int)(((C)>>16)&0xFF)
#define UNPACK24_G(C)		(int)(((C)>>8)&0xFF)
#define UNPACK24_B(C)		(int)((C)&0xFF)
#define PACK24(R, G, B, A)	(((A)<<24) | ((R)<<16) | ((G)<<8) | (B))


#ifdef USE_FIXEDPOINT64
inline float fsqrt(float x)
{
	int i = *(int*)&x;
	i = 0x5f3759df - (i>>1);
	float y = *(float*)&i;
	
	float xhalf = 0.5f*x;
	y = y*(1.5f - xhalf*y*y);
	
	return x * y;
}
#else
inline fixed fsqrt(fixed x)
{
	return sqrt(x);
}
#endif

#if 0
inline fixed FPSQRT(fixed x)
{
	float fx = FP2FLOAT(x);
	int i = *(int*)&fx;
	i = 0x5f3759df - (i>>1);
	fixed y = FLOAT2FP(*(float*)&i);
	
	y = FPMUL(y, FP_HALF_THREE - FPMUL(FPHALF(x), FPSQR(y)));
	
	return FPMUL(x, y);
}

// fast sine function
inline fixed FPSIN(fixed x)
{
	if (x < -FP_PI)
		x += FP_TWO_PI;
	else if (x > FP_PI)
		x -= FP_TWO_PI;

	fixed xsin(0);
	//compute sine
	if (x < 0)
	{
		xsin = FPMUL(SC_ALPHA, x) + FPMUL(SC_BETA, FPSQR(x));
		
		if(xsin<0)
			xsin = FPMUL(SC_GAMMA, FPMUL(xsin, -xsin) - xsin) + xsin;
		else
			xsin = FPMUL(SC_GAMMA, FPMUL(xsin, xsin) - xsin) + xsin;
	}
	else
	{
		xsin = FPMUL(SC_ALPHA, x) - FPMUL(SC_BETA, FPSQR(x));
		
		if(xsin<0)
			xsin = FPMUL(SC_GAMMA, FPMUL(xsin, -xsin) - xsin) + xsin;
		else
			xsin = FPMUL(SC_GAMMA, FPMUL(xsin, xsin) - xsin) + xsin;
	}

	return xsin;
}

// fast cosine function
inline fixed FPCOS(fixed x)
{
	if (x < -FP_PI)
		x += FP_TWO_PI;
	else if (x > FP_PI)
		x -= FP_TWO_PI;

	x += FP_HALF_PI;
	if(x > FP_PI)
		x -= FP_TWO_PI;

	fixed xcos(0);
	//compute sine
	if (x < 0)
	{
		xcos = FPMUL(SC_ALPHA, x) + FPMUL(SC_BETA, FPSQR(x));
		
		if(xcos<0)
			xcos = FPMUL(SC_GAMMA, FPMUL(xcos, -xcos) - xcos) + xcos;
		else
			xcos = FPMUL(SC_GAMMA, FPMUL(xcos, xcos) - xcos) + xcos;
	}
	else
	{
		xcos = FPMUL(SC_ALPHA, x) - FPMUL(SC_BETA, FPSQR(x));
		
		if(xcos<0)
			xcos = FPMUL(SC_GAMMA, FPMUL(xcos, -xcos) - xcos) + xcos;
		else
			xcos = FPMUL(SC_GAMMA, FPMUL(xcos, xcos) - xcos) + xcos;
	}
	
	return xcos;
}

inline fixed FPTAN(fixed x)
{
	return FPDIV(FPSIN(x), FPCOS(x));
}

#endif

inline void FPSINCOS(fixed rTheta, fixed &rSin, fixed &rCos)
{
	//always wrap input angle to -PI..PI
	if(rTheta<-FP_PI)
		rTheta += FP_TWO_PI;
	else if(rTheta>FP_PI)
		rTheta -= FP_TWO_PI;

#ifdef USE_FIXEDPOINT64
	//compute sine
	if(rTheta<0)
	{
		rSin = FPMUL(SC_ALPHA, rTheta) + FPMUL(SC_BETA, FPMUL(rTheta, rTheta));
		
		if(rSin<0)
			rSin = FPMUL(SC_GAMMA, FPMUL(rSin, -rSin) - rSin) + rSin;
		else
			rSin = FPMUL(SC_GAMMA, FPMUL(rSin, rSin) - rSin) + rSin;
	}
	else
	{
		rSin = FPMUL(SC_ALPHA, rTheta) - FPMUL(SC_BETA, FPMUL(rTheta, rTheta));
		
		if(rSin<0)
			rSin = FPMUL(SC_GAMMA, FPMUL(rSin, -rSin) - rSin) + rSin;
		else
			rSin = FPMUL(SC_GAMMA, FPMUL(rSin, rSin) - rSin) + rSin;
	}
	
	//compute cosine: sin(x + PI/2) = cos(x)
	rTheta += FP_HALF_PI;
	if(rTheta>FP_PI)
		rTheta -= FP_TWO_PI;
	
	if(rTheta<0)
	{
		rCos = FPMUL(SC_ALPHA, rTheta) + FPMUL(SC_BETA, FPMUL(rTheta, rTheta));
		
		if(rCos<0)
			rCos = FPMUL(SC_GAMMA, FPMUL(rCos, -rCos) - rCos) + rCos;
		else
			rCos = FPMUL(SC_GAMMA, FPMUL(rCos, rCos) - rCos) + rCos;
	}
	else
	{
		rCos = FPMUL(SC_ALPHA, rTheta) - FPMUL(SC_BETA, FPMUL(rTheta, rTheta));
		
		if(rCos<0)
			rCos = FPMUL(SC_GAMMA, FPMUL(rCos, -rCos) - rCos) + rCos;
		else
			rCos = FPMUL(SC_GAMMA, FPMUL(rCos, rCos) - rCos) + rCos;
	}
#else
	// fix-me
	rSin = sin(rTheta);
	rCos = cos(rTheta);
#endif
}

inline fixed FPASIN(fixed x)
{
#ifdef USE_FIXEDPOINT64
	int sign = x<0 ? -1 : 1;
	if(x<0) x = -x;
	fixed fRoot = FPSQRT(FP_ONE - x);
	fixed result = -AS1;
	result = FPMUL(result, x);
	result += AS2;
	result = FPMUL(result, x);
	result -= AS3;
	result = FPMUL(result, x);
	result += AS4;
	result = FP_HALF_PI - (FPMUL(fRoot, result));
	return result * sign;
#else
	return asin(x);
#endif
}

inline fixed FPACOS(fixed x)
{
#ifdef USE_FIXEDPOINT64
	int sign = x<0 ? -1 : 1;
	if(x<0) x = -x;
	fixed xRoot = FPSQRT(FP_ONE - x);
	fixed result = -AS1;
	result = FPMUL(result, x);
	result += AS2;
	result = FPMUL(result, x);
	result -= AS3;
	result = FPMUL(result, x);
	result += AS4;
	result = FPMUL(xRoot, result);

	fixed ret = sign<0 ? FP_HALF_PI + result : result;
	return ret;
#else
	return acos(x);
#endif
}

inline fixed FPATAN2(fixed y, fixed x)
{
#if USE_FIXEDPOINT64
	fixed angle;
	fixed abs_y = ABS(y) + FP_EPSILON;	// kludge to prevent 0/0 condition
	if (x>=0)
		angle = AT1 - FPMUL(AT1, FPDIV(x - abs_y, x + abs_y));
	else
		angle = AT2 - FPMUL(AT1, FPDIV(x + abs_y, abs_y - x));
	
	return y<0 ? -angle : angle;
#else
	return atan2(y,x);
#endif
}



#endif
