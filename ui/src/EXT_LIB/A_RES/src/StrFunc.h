/*#############################################################################
-------------------------------------------------------------------------------

                                  StrFunc.h
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules : A_VM, A_Utils
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Resource related function definition

-------------------------------------------------------------------------------
#############################################################################*/
#ifndef __STRFUNC_H_
#define __STRFUNC_H_


#include "AT_Types.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __IMPLEMENT_

atVOID    _memset_x( atLPVOID des, atCHAR ch, atLONG len );
atLPTSTR  _tcsncpy_x( atLPTSTR des, atLPCTSTR src, atLONG len );
atLPSTR   _strupr_x( atLPSTR str );
atLPSTR   _strncpy_x( atLPSTR des, atLPCSTR src, atLONG len );
atINT     _strcmp_x( atLPCSTR str1, atLPCSTR str2 );

#endif


#ifdef __cplusplus
}
#endif


#endif
