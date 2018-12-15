/*#############################################################################
-------------------------------------------------------------------------------

                                   INTSqrt.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules : A_VM
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Integer square root function definition

-------------------------------------------------------------------------------
#############################################################################*/

#include "AT_Types.h"
#include "INTSqrt.h"



unsigned long IntSqrt( unsigned long v )
{
    signed long eax, ebx, ecx, edx;

    eax = v;
    ecx = 0;

    ebx = 0x40000000;

LP1:
    edx = eax;
    edx -= ebx;
    if( edx < 0 ) goto LP2;

    edx -= ecx;
    if( edx < 0 ) goto LP2;

    eax = edx;
    ecx >>= 1;  // 논리 쉬프트로 해야 함.
    ecx |= ebx;

    ebx >>= 2;
    if( ebx > 0 ) goto LP1;
    goto LP3;

LP2:
    ecx >>= 1;
    ebx >>= 2;
    if( ebx > 0 ) goto LP1;

LP3:
    eax = ecx;

    return( (unsigned long)eax );
}



#ifdef __DEF_BASE_TYPE_INT64_

unsigned long IntSqrt64( atINT64 v )
{
    atINT64 eax, ebx, ecx, edx;

    eax = v;
    ecx = 0;

    ebx = (atINT64)0x4000000000000000;

LP1:
    edx = eax;
    edx -= ebx;
    if( edx < 0 ) goto LP2;

    edx -= ecx;
    if( edx < 0 ) goto LP2;

    eax = edx;
    ecx >>= 1;  // 논리 쉬프트로 해야 함.
    ecx |= ebx;

    ebx >>= 2;
    if( ebx > 0 ) goto LP1;
    goto LP3;

LP2:
    ecx >>= 1;
    ebx >>= 2;
    if( ebx > 0 ) goto LP1;

LP3:
    eax = ecx;

    return( (unsigned long)eax );
}

#endif
