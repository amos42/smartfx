#ifndef __BASEDEF_H_
#define __BASEDEF_H_
/*###########################################################################
-----------------------------------------------------------------------------

                                basedef.h
                         (* =================== *)

 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : base definition header 
 
-----------------------------------------------------------------------------
###########################################################################*/

#ifdef __cplusplus
  extern "C" {
#endif


/* 불린 타입 상수 */
#ifndef __BOOL_CONST_
#define __BOOL_CONST_
#define  TRUE            1
#define  FALSE           0
#endif


/* 기본 데이터 형 */
#ifndef __BASE_TYPE_
#define __BASE_TYPE_
typedef  char            CHAR;
typedef  int             INT;
typedef  long            LONG;
typedef  unsigned char   BYTE;
typedef  unsigned short  WORD;
typedef  unsigned long   DWORD;
#endif

/* 확장된 데이터 형 */
#ifndef __EXT_TYPE_
#define __EXT_TYPE_
typedef  INT             BOOL;
typedef  unsigned int    UINT;
typedef  unsigned long   ULONG;
#endif

/* 사이즈 규정 데이터 형 */
#ifndef __SIZED_TYPE_
#define __SIZED_TYPE_
typedef  unsigned char   U8;
typedef  signed char     S8;
typedef  unsigned short  U16;
typedef  signed short    S16;
typedef  unsigned long   U32;
typedef  signed long     S32;
#endif


/* NULL */
#ifndef NULL
#define  NULL  ((void *)0)
#endif


/* 기본 콜백 */
#ifndef __BASE_CALLBACK_
#define __BASE_CALLBACK_
typedef  void (CALLBACKBYVOID)( void );
typedef  CALLBACKBYVOID * LPCALLBACKBYVOID;
typedef  void (CALLBACKBYPTR)( void * );
typedef  CALLBACKBYPTR * LPCALLBACKBYPTR;
typedef  void (CALLBACKBYINT)( int );
typedef  CALLBACKBYINT * LPCALLBACKBYINT;
#endif


/* MAX 함수 */
#ifndef MAX
#define MAX(a,b) (((a)>=(b))?(a):(b))
#endif


#ifdef __cplusplus
}
#endif


#endif
