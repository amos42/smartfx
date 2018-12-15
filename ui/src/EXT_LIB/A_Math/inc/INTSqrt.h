/*#############################################################################
-------------------------------------------------------------------------------

                                   INTSqrt.h
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules : A_VM
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Integer square root header definition

-------------------------------------------------------------------------------
#############################################################################*/
#ifndef __INTSQRT_H_
#define __INTSQRT_H_


#include "AT_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


unsigned long   IntSqrt( unsigned long v );

#ifdef __DEF_BASE_TYPE_INT64_
unsigned long   IntSqrt64( atINT64 v );
#endif


#ifdef __cplusplus
}
#endif


#endif

