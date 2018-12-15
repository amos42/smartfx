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

#include "AT_Types.h"
#include "NodeSort.h"



static int partition_a( atSORTNODE *A, int p, int r, int opt )
{
    int i, j, tt;
    long x;
    atSORTNODE dummy;

    i = p - 1;
    j = r + 1;
    tt = 1;

    x = A[p].value;
    while(tt){
      if( opt == SORT_ASC ){
        do { j --; } while( A[j].value < x );
        do { i ++; } while( A[i].value > x );
      } else {
        do { j --; } while( A[j].value > x );
        do { i ++; } while( A[i].value < x );
      }

      if( i < j ){
        SWAP( A[i], A[j], dummy );
      } else {
        tt = 0;
      }
    }

    return j;
}

static void quicksort_a( atSORTNODE *A, int p, int r, int opt )
{
    int q;
    if( p < r ){
      q = partition_a( A, p, r, opt );
      quicksort_a( A, p, q, opt );
      quicksort_a( A, q+1, r, opt );
    }
}




static int partition_f( atSORTNODE_F *A, int p, int r, int opt )
{
    int i, j, tt;
    float x;
    atSORTNODE_F dummy;

    i = p - 1;
    j = r + 1;
    tt = 1;

    x = A[p].value;
    while(tt){
      if( opt == SORT_ASC ){
        do { j --; } while( A[j].value < x );
        do { i ++; } while( A[i].value > x );
      } else {
        do { j --; } while( A[j].value > x );
        do { i ++; } while( A[i].value < x );
      }

      if( i < j ){
        SWAP( A[i], A[j], dummy );
      } else {
        tt = 0;
      }
    }

    return j;
}

static void quicksort_f( atSORTNODE_F *A, int p, int r, int opt )
{
    int q;
    if( p < r ){
      q = partition_f( A, p, r, opt );
      quicksort_f( A, p, q, opt );
      quicksort_f( A, q+1, r, opt );
    }
}


void  NodeSort( atSORTNODE *nodelist, int count, int opt )
{
    quicksort_a( nodelist, 0, count-1, opt );
}


void  NodeSort_F( atSORTNODE_F *nodelist, int count, int opt )
{
    quicksort_f( nodelist, 0, count-1, opt );
}







/*************

static void quicksort( SORTNODE *s, int lo, int hi, int opt )
{
    int mid;
    SORTNODE partition;
    SORTNODE tmp;
    int left, right;

        if (lo >= hi) return;

        // Use median-of-three(lo, mid, hi) to pick a partition.  Also
        // swap them into relative order while we are at it.
        mid = (lo + hi) / 2;

        if( s[lo].value > s[mid].value ){
          SWAP( s[lo], s[mid], tmp );
        }

        if( s[mid].value > s[hi].value ){
          SWAP( s[mid], s[hi], tmp );

          if( s[lo].value > s[mid].value ){
            SWAP( s[li], s[mid], tmp );
          }
        }

        // Start one past lo since already handled lo.
        left = lo+1;
        // Similarly, end one before hi since already handled hi.
        right = hi-1;
        // If there are three or fewer elements, we are done.
        if (left >= right) return;

        partition = s[mid];

        while(1){
            while ( s[right].value > partition.value )
                right--;

            while (left < right &&
                   s[left].value <= partition.value )
                left++;

            if (left < right){
              SWAP( s[left], s[right], tmp );
              right--;
            } else
              break;
        }

        quicksort(s, lo, left, opt);
        quicksort(s, left+1, hi, opt);
}

***************/


