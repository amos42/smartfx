/*###########################################################################
-----------------------------------------------------------------------------

                                TriFunc.h
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
#ifndef __TRIFUNC_H_
#define __TRIFUNC_H_


#include "AT_Real.h"


#ifdef __cplusplus
extern "C" {
#endif    


// REAL을 float 타입으로 사용하며,
// 테이블을 사용하지 않을 것이라면 아래 커맨트
#ifdef __FIXEDPTR_
#define __USE_TRI_TABLE_
#endif

#ifndef TRI_1DEG_RATIO
#define TRI_1DEG_RATIO     (10)			/// 정밀도 0.1도
//#define TRI_1DEG_RATIO     (5)			/// 정밀도 0.2도
//#define TRI_1DEG_RATIO     (2)			/// 정밀도 0.5도
//#define TRI_1DEG_RATIO     (1)			/// 정밀도 1도
//#define TRI_1DEG_RATIO     (1/10f)		/// 정밀도 10도
#endif

#define MAX_TRI_ANGLE      INTTOREAL(360)


#if !defined(__FIXEDPTR_) && !defined(__USE_TRI_TABLE_)
#include <math.h>
#define SIN(x)  sin(x)
#define COS(x)  cos(x)
#define TAN(x)  tan(x)
#define ASIN(x) asin(x)
#define ACOS(x) acos(x)
#define ATAN(x) atan(x)
#else
#define SIN(x)  fast_sin(x)
#define COS(x)  fast_cos(x)
#define TAN(x)  fast_tan(x)
#define ASIN(x) fast_asin(x)
#define ACOS(x) fast_acos(x)
#define ATAN(x) fast_atan(x)
#define ATAN2(x, y) fast_atan2(x, y)
#endif


//#define SIN(x)  fast_sin(x)
//#define COS(x)  fast_cos(x)
//#define TAN(x)  fast_tan(x)
//#define ASIN(x) fast_asin(x)
//#define ACOS(x) fast_acos(x)
//#define ATAN(x) fast_atan(x)
//#define ATAN2(x, y) fast_atan2(x, y)

#if defined(__USE_TRI_TABLE_)
#if defined(__FIXEDPTR_)
	extern const atREAL SinTable[ 90 * TRI_1DEG_RATIO + 1 ];
	extern const atREAL TanTable[ 90 * TRI_1DEG_RATIO + 1 ];
#else
	extern atREAL SinTable[ 90 * TRI_1DEG_RATIO + 1 ];
	extern atREAL TanTable[ 90 * TRI_1DEG_RATIO + 1 ];
#endif
#endif

void  InitTriangleFunc( void );

atREAL  fast_sin( atREAL deg );
atREAL  fast_cos( atREAL deg );
atREAL  fast_tan( atREAL deg );

atREAL  fast_asin( atREAL value );
atREAL  fast_acos( atREAL value );
atREAL  fast_atan( atREAL value );
atREAL  fast_atan2( atREAL dx, atREAL dy );


//입력가능 deg 범위 -360 ~ +719
#define _COS( deg )				g__cos[ (deg)<0 ? (deg+360) : ( deg>=360 ? deg-360 : deg ) ]

//입력가능 deg 活?-270 ~ +809
#define _SIN( deg )				g__cos[ (deg)<90 ? (deg+270) : ( deg>=450 ? deg-450 : deg-90 ) ]
//#define _SIN( deg )				_COS( (deg)-90 )

//입력가능 deg 범위 -45 ~ +45
#define _TAN( deg )				(deg<0) ? -g__tan[ -*deg) ] : g__tan[ deg ]


#ifdef __cplusplus
};
#endif    


#endif
