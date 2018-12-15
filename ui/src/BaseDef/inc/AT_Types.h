/*###########################################################################*/
/**
  @file AT_Types.h
  @brief 기본 Type 정의

  기본적으로 자주 사용되는 Type 정의 포함

  @remark 거의 모든 파일에 기본 include 된다.
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see AT_Real.h
*/
/*###########################################################################*/
#ifndef __AT_TYPES_H_
#define __AT_TYPES_H_


/* Possible Pre-define Options; */
#define __USES_UNICODE_
/* #define __X86_ */
/* #define __ARM7_ */
/* #define __ARM9_ */
/* #define __MIPS_ */


#if defined(__STDC__)
# define __C89__
# if defined(__STDC_VERSION__)
#  define __C90__
#  if (__STDC_VERSION__ >= 199409L)
#   define __C94__
#  endif
#  if (__STDC_VERSION__ >= 199901L)
#   define __C99__
#  endif
# endif
#endif


#ifdef WIN32 /* Windows */
#ifndef __X86_
#define __X86_
#endif
#endif

#if defined(_UNICODE) || defined(UNICODE) /* Windows */
#ifndef __USES_UNICODE_
#define __USES_UNICODE_
#endif
#endif

#if defined(__ARM7_) || defined(__ARM9_)
#ifndef __ARM_
#define __ARM_
#endif
#endif

/*default : __X86_*/
#if !defined(__ARM_) && !defined(__X86_)  && !defined(__MIPS_)
#ifndef __X86_
#define __X86_
#endif
#endif

/*#define COMPILER ARMCC / TCC / BC / VC*/

#if defined(__X86_) || defined(__ARM_)
#define __LITTLE_ENDIAN_
#elif defined(__MIPS_)
#define __BIG_ENDIAN_
#else
#define __LITTLE_ENDIAN_
#endif

#if defined(__ARM_)
#define PACKED __packed 
#else
#define PACKED
#endif


#include <stdint.h>

#ifdef __DEF_CHAR_STR_TYPE_
typedef short   atWCHAR_T;
#else
#include <wchar.h>
typedef wchar_t atWCHAR_T;
#endif


#ifdef __cplusplus
extern "C" {
#endif


/* 기본 데이터 형 */
#ifndef __DEF_BASE_TYPE_
#define __DEF_BASE_TYPE_
typedef  char            atCHAR;
typedef  int             atINT;
typedef  short           atSHORT;
typedef  long            atLONG;
typedef  float           atFLOAT;
typedef  double          atDOUBLE;
typedef  void            atVOID;	
#endif


/* 기본 데이터 형 (INT64) */
#ifndef __DEF_BASE_TYPE_INT64_
#define __DEF_BASE_TYPE_INT64_
#if		defined(_LINUX_)
typedef  long long  atINT64;
#elif   defined(__X86_)
typedef  __int64    atINT64;
#elif defined(__ARM_)
typedef  long long  atINT64;
#elif defined(__MIPS_)
typedef  long long  atINT64;
#else
#undef __BASE_TYPE_INT64_
#endif
#endif


/* 기본 데이터 형 2 */
#ifndef __DEF_BASE2_TYPE_
#define __DEF_BASE2_TYPE_
typedef  signed char     atSCHAR;
typedef  signed int      atSINT;
typedef  signed long     atSLONG;
typedef  unsigned char   atUCHAR;
typedef  unsigned short  atUSHORT;
typedef  unsigned int    atUINT;
typedef  unsigned long   atULONG;
#endif


/* 기본 데이터 형 3 */
#ifndef __DEF_BASE3_TYPE_
#define __DEF_BASE3_TYPE_
typedef  unsigned char   atBYTE;
typedef  unsigned short  atWORD;
typedef  unsigned long   atDWORD;
#endif


/* 확장된 데이터 형 */
#ifndef __DEF_EXT_TYPE_
#define __DEF_EXT_TYPE_
typedef  atINT           atBOOL;
#endif

/* 자주 쓰는 포인터 형 */
#ifndef __DEF_PTR_TYPE_
#define __DEF_PTR_TYPE_
typedef  atVOID *        atLPVOID;
typedef  atBYTE *        atLPBYTE;
#if defined(__C99__x)
typedef  intptr_t        atINTPTR;
typedef  uintptr_t       atUINTPTR;
#else
typedef  atLONG          atINTPTR;
typedef  atULONG         atUINTPTR;
#endif
#endif

/* 사이즈 규정 데이터 형 */
#ifndef __DEF_SIZED_TYPE_
#define __DEF_SIZED_TYPE_
typedef  unsigned char   atU8;
typedef  signed char     atS8;
typedef  unsigned short  atU16;
typedef  signed short    atS16;
typedef  unsigned long   atU32;
typedef  signed long     atS32;
typedef  atS8		 atINT8;
typedef  atU8	 	 atUINT8;
typedef  atS16		 atINT16;
typedef  atU16		 atUINT16;
typedef  atS32		 atINT32;
typedef  atU32		 atUINT32;
#endif


/* 문자, 문자열 타입 */
#ifndef __DEF_CHAR_STR_TYPE_
#define __DEF_CHAR_STR_TYPE_
typedef atWCHAR_T	   atWCHAR;
	
typedef atCHAR		   atANSI_CHAR;
typedef atSHORT 	   atUCS2_CHAR;
typedef atLONG 		   atUCS4_CHAR;
typedef atWCHAR 	   atUCS_CHAR;

#ifdef __USES_UNICODE_
typedef  atUCS_CHAR      atTCHAR;
#define  _ATC(a)         L##a
#define  _AT(a)          L##a
#else
typedef  atANSI_CHAR     atTCHAR;
#define  _ATC(a)         a
#define  _AT(a)          a
#endif

typedef  atANSI_CHAR *     	   atLPSTR;
typedef  atUCS_CHAR *      	   atLPWSTR;
typedef  atTCHAR *       	   atLPTSTR;

typedef  const atLPSTR		   atLPCSTR;
typedef  const atLPWSTR        atLPCWSTR;
typedef  const atLPTSTR        atLPCTSTR;
#endif

	
/* 결과 타입 */
#ifndef __DEF_RESULT_TYPE_
#define __DEF_RESULT_TYPE_
typedef char             atBRESULT;
typedef short            atSRESULT;
typedef long             atLRESULT;
typedef unsigned         atURESULT;
#endif


/* 상태 타입 */
#ifndef __DEF_STATUS_TYPE_
#define __DEF_STATUS_TYPE_
typedef char             atBSTATUS;
typedef short            atSSTATUS;
typedef long             atLSTATUS;
typedef unsigned         atUSTATUS;
#endif


/* 핸들 타입 */
#ifndef __DEF_HANDLE_TYPE_
#define __DEF_HANDLE_TYPE_
typedef void *           atHANDLE;
#endif


/* 파라미터 IN/OUT */
#ifndef __DEF_PARAM_INOUT_
#define __DEF_PARAM_INOUT_
#ifndef _IN_
#define _IN_
#endif
#ifndef _OUT_
#define _OUT_
#endif
#ifndef _IN_OUT_
#define _IN_OUT_
#endif
#endif


/* 불린 타입 상수 */
#ifndef __DEF_BOOL_CONST_
#define __DEF_BOOL_CONST_
#define  atTRUE          (1)
#define  atFALSE         (0)
#endif


/* Enable/Disable */
#ifndef __DEF_ABLE_CONST_
#define __DEF_ABLE_CONST_
#define atENABLE         (1)
#define atDISABLE        (0)
#endif


/* NULL */
#ifndef atNULL
/*#define  atNULL          ((void *)0)*/
#define  atNULL          (0)
#endif


/* LOCAL */
#ifndef atLOCAL
#define atLOCAL          static
#endif

/* NOARGS */
#ifndef NOARGS
#define NOARGS          void
#endif


/* 위치, 크기, 영역 타입 */
#ifndef __DEF_GEOMETRIC_TYPE_
#define __DEF_GEOMETRIC_TYPE_

typedef struct tagatSPOINT {
    atSHORT    nX, nY;
} atSPOINT;

typedef struct tagatLPOINT {
    atLONG     nX, nY;
} atLPOINT;

typedef struct tagatSSIZE {
    atSHORT    nWidth, nHeight;
} atSSIZE;

typedef struct tagatLSIZE {
    atLONG     nWidth, nHeight;
} atLSIZE;

typedef struct tagatSRECT {
    atSHORT    nStartX, nStartY;
    atSHORT    nEndX, nEndY;
} atSRECT;

typedef struct tagatLRECT {
    atLONG     nStartX, nStartY;
    atLONG     nEndX, nEndY;
} atLRECT;

typedef struct tagatSRECT2 {
    atSHORT    nLeft, nTop;
    atSHORT    nRight, nBottom;
} atSRECT2;

typedef struct tagatLRECT2 {
    atLONG     nLeft, nTop;
    atLONG     nRight, nBottom;
} atLRECT2;

typedef struct tagatSREGION {
    atSHORT    nX, nY;
    atSHORT    nWidth, nHeight;
} atSREGION;

typedef struct tagatLREGION {
    atLONG     nX, nY;
    atLONG     nWidth, nHeight;
} atLREGION;

typedef atLPOINT    atPOINT;
typedef atLSIZE     atSIZE;
typedef atLRECT     atRECT;
typedef atLRECT2    atRECT2;
typedef atLREGION   atREGION;

#ifndef SET_POINT
#define SET_POINT(pt,x,y)			{(pt).nX = (x); (pt).nY = (y);}
#endif
#ifndef SET_SIZE 
#define SET_SIZE(sz,w,h)			{(sz).nWidth = (w); (sz).nHeight = (h);}
#endif
#ifndef SET_RECT 
#define SET_RECT(rt, sx,sy,ex,ey) 	{(rt).nStartX = (sx); (rt).nStartY = (sy); (rt).nEndX = (ex); (rt).nEndY = (ey);}
#endif
#ifndef SET_RECT2
#define SET_RECT2(rt, l,t,r,b) 		{(rt).nLeft = (l); (rt).nTop = (t); (rt).nRight = (r); (rt).nBottom = (b);}
#endif
#ifndef SET_REGION
#define SET_REGION(rg, x,y,w,h) 	{(rg).nX = (x); (rg).nY = (y); (rg).nWidth = (w); (rg).nHeight = (h);}
#endif

#ifndef PT_IN_REGION
#define PT_IN_REGION(rgn, x, y) 	(x >= rgn.nX && x <= rgn.nX+rgn.nWidth-1 && y >= rgn.nY && y <= rgn.nY+rgn.nHeight-1)
#endif
	
#ifndef RECT_WIDTH
#define RECT_WIDTH(a) ABS(((a).nEndX - (a).nStartX) + 1)
#endif
#ifndef RECT_HEIGHT
#define RECT_HEIGHT(a) ABS(((a).nEndY - (a).nStartY) + 1)
#endif
#ifndef RECT2_WIDTH
#define RECT2_WIDTH(a) ABS(((a).nRight - (a).nLeft) + 1)
#endif
#ifndef RECT2_HEIGHT
#define RECT2_HEIGHT(a) ABS(((a).nBottom - (a).nTop) + 1)
#endif

#ifndef REGION2RECT
#define REGION2RECT(a,b) SET_RECT((a), (b).nX, (b).nY, (b).nX+(b).nWidth-1, (b).nY+(b).nHeight-1)
#endif
#ifndef RECT2REGION
#define RECT2REGION(a,b) SET_REGION((a), (b).nStartX, (b).nStartY, RECT_WIDTH(b), RECT_HEIGHT(b))
#endif

#endif

/* 시간, 날짜 */
#ifndef __TIME_DATE_TYPE_
#define __TIME_DATE_TYPE_
typedef struct _tagatDATETIME {
    atSHORT	nYear;
    atSHORT	nMonth;
    atSHORT	nDay;
    atSHORT	nHour;
    atSHORT	nMinute;
    atSHORT	nSec;  
} atDATETIME;
#endif

/* 기본 콜백 */
#ifndef __DEF_BASE_CALLBACK_
#define __DEF_BASE_CALLBACK_
typedef  atINT (atCALLBACKBYVOID)( NOARGS );
typedef  atCALLBACKBYVOID * atLPCALLBACKBYVOID;
typedef  atINT (atCALLBACKBYPTR)( atVOID * );
typedef  atCALLBACKBYPTR * atLPCALLBACKBYPTR;
typedef  atINT (atCALLBACKBYINT)( atINT );
typedef  atCALLBACKBYINT * atLPCALLBACKBYINT;
#endif

/* Malloc/Free 콜백 */
#ifndef __DEF_MALLOC_CALLBACK_
#define __DEF_MALLOC_CALLBACK_
typedef  atLPVOID (atCALLBACKALLOC)( atLONG );
typedef  atCALLBACKALLOC * atLPCALLBACKALLOC;
typedef  atVOID (atCALLBACKFREE)( atLPVOID );
typedef  atCALLBACKFREE * atLPCALLBACKFREE;
typedef  atVOID (atCALLBACKREALLOC)( atLPVOID, atLONG );
typedef  atCALLBACKREALLOC * atLPCALLBACKREALLOC;

#define atCALLFUNC(a)      (a)
#define atSAFECALLFUNC(a)  ((a)==atNULL)? atNULL : (a)
#endif


/* MIN/MAX 함수 */
#ifndef __MACROS_H  // coachfw\c10p\os\include

#ifndef MIN
#define MIN(a,b) (((a)<=(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>=(b))?(a):(b))
#endif
#endif // !_TX_

/* ABS 함수 */
#ifndef ABS
#define ABS(a) (((a)>=(0))?(a):-(a))
#endif

/* SWAP 함수 */
#ifndef SWAP
#define SWAP(a,b,t) {(t)=(a);(a)=(b);(b)=(t);}
#endif

/* COUNTOF 함수 */
#ifndef COUNTOF
#define COUNTOF(a)       (  sizeof(a)/sizeof(a[0]) )
#endif


#ifdef __cplusplus
}
#endif


#endif
