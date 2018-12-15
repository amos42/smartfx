/*###########################################################################*/
/**
  @file AT_Real.h
  @brief Real Ÿ�� ���� ����

  Fixed Ptr�� ������ �Ǽ� ������ ������ ���

  @remark Fixed Ptr�� ���, �ִ밪�� Boundary�� �۱� ������
          ���� ����� �׻� Overflow�� �����ؾ� �Ѵ�.
          Overflow�� �ǽɵ� ������ INT64�� ����Ѵ�.
		  ���� float Ÿ���� REAL�� �ϰ� �ʹٸ�
		  __FIXEDPTR_ define�� comment ó��
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see AT_Types.h
*/
/*###########################################################################*/
#ifndef __AT_REAL_H_
#define __AT_REAL_H_


#ifndef __REAL_TYPE_
#define __REAL_TYPE_


//=======================================
// ���� float Ÿ���� REAL�� �ϰ� �ʹٸ�
// ���� ������ comment ó��
#ifndef __FIXEDPTR_
#define __FIXEDPTR_
#endif
//=======================================

#include "AT_Types.h"


typedef signed long        FIXEDPTR_TYPE;
#ifndef FIXED_RADIX_SIZE
#define FIXED_RADIX_SIZE   (16)
#endif

#define FIXEDREALMASK     ((1 << FIXED_RADIX_SIZE)-1)
#define FIXEDREALK(a)     ((FIXEDPTR_TYPE)((a) & FIXEDREALMASK))
#define FIXEDINTK(a)      ((FIXEDPTR_TYPE)((a) & (~FIXEDREALMASK)))

#define INTTOFIXED(a)     ((FIXEDPTR_TYPE)((a) << FIXED_RADIX_SIZE))
#define FIXEDTOINT(a)     ((atINT)((atSLONG)(a) >> FIXED_RADIX_SIZE))
#define FLOATTOFIXED(a)   ((FIXEDPTR_TYPE)((atFLOAT)(a) * ((1 << FIXED_RADIX_SIZE) * 1.0)))
#define FIXEDTOFLOAT(a)   ((atFLOAT)(((atSLONG)(a)) / ((1 << FIXED_RADIX_SIZE) * 1.0)))


#ifdef __FIXEDPTR_	/// REAL == signed long

typedef FIXEDPTR_TYPE    atREAL;

#define REAL_ONE		 65536

#define INTTOREAL(a)     INTTOFIXED(a)
#define REALTOINT(a)     FIXEDTOINT(a)
#define REALTOINT_R(a)   FIXEDTOINT((a)+(FIXEDREALMASK/2-1))
#define REALTOINT_U(a)   FIXEDTOINT((a)+(FIXEDREALMASK))
#define FLOATTOREAL(a)   FLOATTOFIXED(a)
#define REALTOFLOAT(a)   FIXEDTOFLOAT(a)

#define REALREALK(a)     FIXEDREALK(a)
#define REALINTK(a)      FIXEDINTK(a)

extern
#ifdef __cplusplus
"C" 
#endif
unsigned long IntSqrt( unsigned long v );
#define REALSQRT(a)      ((atREAL)(IntSqrt(a) << (FIXED_RADIX_SIZE/2)))

#define REALMUL( a, b )  ((atREAL)(  (  ((atINT64)(a))*(atREAL)(b)) >> FIXED_RADIX_SIZE  )   )
#define REALDIV( a, b )  ((atREAL)(  (  ((atINT64)(a)) << FIXED_RADIX_SIZE   ) / (atREAL)(b) ) )
#define REALMULDIV( a, b, c )  ((atREAL)(  ((atINT64)(a))*(atREAL)(b)/(atREAL)(c) ))
#define REALDIVMUL( a, b, c )  ((atREAL)(  ((atINT64)(a))/(atREAL)(b)*(atREAL)(c) ))
#define REALHALF( a )	 	((atREAL)(a) >> 1)
#define REALQUART( a )	 ((atREAL)(a) >> 2)

#define FIXEDTOREAL(a)   ((atREAL)(a))
#define REALTOFIXED(a)   ((atSLONG)(a))

#else  /// REAL == float

typedef double           atREAL;

#define REAL_ONE		 1

#define INTTOREAL(a)     ((atREAL)(a))
#define REALTOINT(a)     ((atINT)floor((atREAL)(a)))
#define REALTOINT_R(a)   ((atINT)floor((atREAL)(a) + 0.5f))
#define REALTOINT_U(a)   ((atINT)ceil((atREAL)(a)))
#define FLOATTOREAL(a)   ((atREAL)((atFLOAT)(a)))
#define REALTOFLOAT(a)   ((atFLOAT)((atREAL)(a)))

#define REALREALK(a)     ((a)-(atINT)(a))
#define REALINTK(a)      ((atINT)(a))

#include <math.h>
#define REALSQRT(a)      ((atREAL)(sqrt(a)))

#define REALMUL( a, b )  ((atREAL)( (a) * (b) ))
#define REALDIV( a, b )  ((atREAL)( (a) / (b) ))
#define REALMULDIV( a, b, c )  ((atREAL)(  (a)*(b)/(c)  )
#define REALDIVMUL( a, b, c )  ((atREAL)(  (a)/(b)*(c)  )
#define REALHALF( a )	 	((atREAL)(a) / 2)
#define REALQUART( a ) 	((atREAL)(a) / 4)

#define FIXEDTOREAL(a)   FIXEDTOFLOAT(a)
#define REALTOFIXED(a)   FLOATTOFIXED(a)

#endif  // __FIXEDPTR_


#endif // __REAL_TYPE_


#endif // __AT_REAL_H_
