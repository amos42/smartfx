/*###########################################################################
-----------------------------------------------------------------------------

                                A_Utils
                       (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : {idx - value} 노드를 정렬한다.
 
-----------------------------------------------------------------------------
###########################################################################*/
#ifndef __NODESORT_H_
#define __NODESORT_H_


#include "AT_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SORT_ASC  0
#define SORT_DESC 1


typedef struct tagSORTNODE {
    long  idx;
    long  value;
} atSORTNODE;

typedef struct tagaSORTNODE_F {
    long    idx;
    float   value;
} atSORTNODE_F;


void  NodeSort( atSORTNODE *nodelist, int count, int opt );
void  NodeSort_F( atSORTNODE_F *nodelist, int count, int opt );  // No Implement


#ifdef __cplusplus
}
#endif


#endif

