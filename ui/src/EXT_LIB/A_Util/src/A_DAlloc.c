/*#############################################################################
-------------------------------------------------------------------------------

                                  A_Dalloc.c
                           (* =================== *)


Version				: 1.1.0
Dependent Modules	:
Author				: Ju, Gyeong-min ( Amos )
Create				: 2001/04/25		01:47:09 am
Update				: 2003/04/29		00:00:00 am
Update				: 2004/12/08		21:31:00 am		KHYEO
Update				: 2005/02/18						RYOJIN, KHYEO (속도 향상)
Update				: 2005/02/21						RYOJIN (안정화)
Update				: 2005/04/12 					khyeo (port to RMEM)
Update				: 2005/09/01					gmju (SectionMem 도입하여 수정)
Update				: 2008/07/11					gmju (entry_id를 좀 더 적용)
Update				: 2009/03/05					gmju (설정된 총 Heap 사이즈 얻는 함수 추가)
C Compiler			: ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
Assembler			: ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
Description			: Dynamic Allocation related fucntion definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "A_DAlloc.h"

#include <stdlib.h>
#include <string.h>

//=========================================
#define CHECK_ENTRY_ID          /// 메모리 무결성을 체크하기 위해... 대신 약간의 메모리와 속도를 더 소모한다.
#define FAST_ALLOC              /// 메모리를 빠르게 할당하기 위해... 특수한 경우에 따라 오히려 더 느려질 수도 있다.
//=========================================


#ifndef atNULL
#define atNULL	0
#endif

#ifndef atTRUE
#define atTRUE 1
#endif
#ifndef atFALSE
#define atFALSE 0
#endif


//	각 메모리 블럭 속성
typedef enum {
    ES_INVALID,     // 무효한 블럭 - 현재 사용 안 함.
    ES_FREED,       // 할당 안 된 블럭
    ES_ALLOCATED    // 할당된 블럭
} ENTRYSTATUS;

#define DA_HEADER_ID		(0x50414548)    /// ASCII 'H' 'E' 'A' 'P'
#define DA_SECTION_ID		(0x54434553)    /// ASCII 'S' 'E' 'C' 'T'
#ifdef CHECK_ENTRY_ID
//#define DA_ENTRY_ID		(0x52544E45)    /// ASCII 'E' 'N' 'T' 'R'
#define DA_ENTRY_ID			(0x5445)		/// ASCII 'E' 'T'
#define DA_INVALID_ENTRY_ID	(0x0000)		/// 
#endif

// Heap의 헤더
typedef struct tagHEAPHEADER {
    unsigned long header_id;
    unsigned long heap_size;
#ifdef FAST_ALLOC	
    void *	           last_access_ptr;
    char               dummy[4];
#else
    char               dummy[8];
#endif
} HEAPHEADER, * PHEAPHEADER;

// 각 Entry의 헤더
typedef struct tagENTRY {
#ifdef CHECK_ENTRY_ID
	unsigned short entry_id;
	unsigned char /*ENTRYSTATUS*/ status;
	unsigned char /*ENTRYATTR*/ attr;
#else
	unsigned short /*ENTRYSTATUS*/ status;
	unsigned short /*ENTRYATTR*/ attr;
#endif
	unsigned long size;
	struct tagENTRY *     prev;  // 할당 매커니즘 자체는 next만으로 가능하나, 메모리 단편화를 줄이기 위한 목적으로...
//	struct tagENTRY *     next;
} ENTRY, * PENTRY; // 12~16 Bytes


#define DA_ALIGN				(sizeof(atDWORD)) /// 4의 배수이어야 함.
#define DA_ALIGNMENT(a)		(((atUINTPTR)(a) + (DA_ALIGN-1)) & (~(DA_ALIGN-1)))
//#define DA_ALIGNMENT(a)			(((atUINTPTR)(a)+(DA_ALIGN-1)) / DA_ALIGN * DA_ALIGN)
#define DA_ALIGNED_SIZE(a)		DA_ALIGNMENT(a)
#define DA_HEAPHEADER_SIZE		DA_ALIGNED_SIZE( sizeof(HEAPHEADER) )
#define DA_ENTRYHEADER_SIZE		DA_ALIGNED_SIZE( sizeof(ENTRY) )
#define DA_LEFTOVER				(DA_ALIGN*2)

//#define DA_ENTRY_SIZE(ptr)   		( (ptr->next)? ((long)(ptr->next) - (long)ptr - DA_ENTRYHEADER_SIZE) : 0 )
#define DA_ENTRY_SIZE(ptr)   		((ptr)->size)
//#define DA_ENTRY_NEXT(ptr)   		(ptr->next)
#define DA_ENTRY_NEXT(ptr)   	((ENTRY*)( ((ptr)->size != 0)? (atUINTPTR)(ptr) + DA_ENTRYHEADER_SIZE + DA_ALIGNMENT((ptr)->size) : atNULL))

#define	DA_MIN(a,b)	        (((a)<=(b))?(a):(b))
#define	DA_MAX(a,b)	        (((a)>=(b))?(a):(b))


// 할당되지 않은 블럭을 찾는다. 내부용
static PENTRY DA_findFree( PENTRY entry, unsigned long aligned_size )
{
	aligned_size += DA_ENTRYHEADER_SIZE;
	
 	while( entry != atNULL ){
#ifdef CHECK_ENTRY_ID
        if( entry->entry_id != DA_ENTRY_ID ){
	        return atNULL;
        }		
#endif
		if( entry->status == ES_FREED ){
			if( aligned_size <= DA_ENTRY_SIZE(entry) ){
				return( entry );
			}
		}
		
		entry = DA_ENTRY_NEXT( entry );
	}

	return( atNULL );
}


// 할당되지 않은 블럭을 정렬된 사이즈로 찾는다. 내부용
static PENTRY DA_findAlignedFree( PENTRY entry, unsigned long aligned_size, unsigned long align )
{
	unsigned long entryTarget;

 	while( entry != atNULL ){
 		if( entry->status == ES_FREED ){
			if( !(DA_ENTRY_SIZE(entry) < (aligned_size + DA_ENTRYHEADER_SIZE)) ){
				entryTarget = (((atUINTPTR)entry + align - 1) / align) * align;
				
				if( (entryTarget - (atUINTPTR)entry ) < (DA_ENTRYHEADER_SIZE*2) ){
					entryTarget = entryTarget + align;
				}
				
				if( DA_ENTRY_SIZE(entry) >= (entryTarget - (int)(atINTPTR)entry + aligned_size + DA_ENTRYHEADER_SIZE) ){				
					break;
				} else {
				    //size가 entry에 충분치 않음.
				}
			}
		}

		entry = DA_ENTRY_NEXT(entry);
	}

	return( entry );
}


// 할당되지 않은 블럭을 메모리 끝에서부터 찾는다. (내부용)
static PENTRY DA_findFreeLast( PENTRY entry, unsigned long aligned_size )
{
	aligned_size += DA_ENTRYHEADER_SIZE;
	
	while( entry != atNULL ){
 		if( entry->status == ES_FREED ){
			if( aligned_size <= DA_ENTRY_SIZE(entry) ){
				return( entry );
			}
		}
		
		entry = entry->prev;
	}

	return( atNULL );
}


// 지정 주소를 기준으로, 메모리를 할당된 블럭으로 잘라낸다. (내부용)
static PENTRY DA_split( PENTRY entry, unsigned long aligned_size )
{
	PENTRY newb, next_entry;

//	if ( !(DA_ENTRY_SIZE(entry) < (aligned_size + DA_ENTRYHEADER_SIZE)) ){ // 이미 이전에 조건판단을 했으므로...

	// 만약 블럭의 크기가 할당하고자 하는 크기에 빠듯하면 그냥 속성만 바꾼다.
	if( DA_ENTRY_SIZE(entry) <= DA_ENTRYHEADER_SIZE + aligned_size ){
		entry->status = ES_ALLOCATED;
		return entry;
	}
		
		newb				= (PENTRY)((atUINTPTR)entry + DA_ENTRYHEADER_SIZE + aligned_size);
#ifdef CHECK_ENTRY_ID	
	    newb->entry_id	    = DA_ENTRY_ID; //khyeo 2004.12.10 entry ID 추가
#endif
//		newb->next			= entry->next;
		newb->size			= entry->size - DA_ENTRYHEADER_SIZE - aligned_size;
		newb->prev			= entry;
		newb->status		= ES_FREED;
		newb->attr			= 0x00;

		next_entry = DA_ENTRY_NEXT(entry);
        	if( next_entry != atNULL )
			next_entry->prev	= newb;
		
    	entry->status		= ES_ALLOCATED;
    	entry->attr			= 0x00;
	entry->size 			= aligned_size;

		return newb;
//	}

	return atNULL;
}


// 지정 주소를 기준으로, 다음 블럭의 메모리를 무조건 합친다. (내부용)
static void DA_merge_next( PENTRY entry )
{
	PENTRY entry_nxt, entry_next_next;

    entry_nxt = DA_ENTRY_NEXT(entry);
    if( (entry_nxt != atNULL) ){
		entry->size += DA_ENTRYHEADER_SIZE + entry_nxt->size;
		if( DA_ENTRY_SIZE(entry_nxt) > 0 ){
			entry_next_next = DA_ENTRY_NEXT(entry_nxt);
			entry_next_next->prev = entry;
		}
#ifdef CHECK_ENTRY_ID	
	    entry_nxt->entry_id   = DA_INVALID_ENTRY_ID;
#endif
	}
}


// 지정 주소를 기준으로, 이전 블럭의 메모리를 무조건 합친다. (내부용)
static void DA_merge_prev( PENTRY entry )
{
	PENTRY entry_prv, entry_nxt;

    entry_prv = entry->prev;
    if( (entry_prv != atNULL) ){
		entry_prv->size += DA_ENTRYHEADER_SIZE + entry->size;
		entry_nxt = DA_ENTRY_NEXT(entry);
		if( entry_nxt )  entry_nxt->prev = entry_prv;
#ifdef CHECK_ENTRY_ID	
	    entry->entry_id   = DA_INVALID_ENTRY_ID;
#endif
	}
}


// 해당 포인트 전후로 메모리 단편화를 제거한다. (내부용)
static PENTRY DA_compact( PENTRY entry )
{
	PENTRY entry_prv, entry_nxt, last_entry;

 	if( entry == atNULL ) return atNULL;
	if( entry->status != ES_FREED ) return atNULL;

	while( (entry_nxt = DA_ENTRY_NEXT(entry)) && (entry_nxt->status == ES_FREED) ){
	    DA_merge_next( entry );
	}
	last_entry = entry;
	
	entry_prv = entry->prev;
 	while( (entry_prv != atNULL) && (entry_prv->status == ES_FREED) ){
	    DA_merge_next( entry_prv );
		last_entry = entry_prv;
		entry_prv = entry_prv->prev;
	}

	return last_entry;
}


// 메모리가 무결한지 체크한다. (내부용)
// 0  : 정상
// -1 : 시작 포인트가 NULL
// -2 : 시작 포인트가 시작이 아니다. (이전 방향을 가리키는 포인터가 NULL이 아니다.)
// -3 : 엔트리 블럭이 깨졌다. (Entry ID가 일치하지 않는다.)
// -4 : 유효하지 않은 속성이다. (할당, 해제, 어디에도 해당 안 된다.)
// -5 : 이전 포인터와의 정보가 서로 일치하지 않는다.
// -6 : 이후 포인터와의 정보가 서로 일치하지 않는다.
// -7 : 메모리 전체 블럭의 끝과, 마지막 블럭의 끝의 정보가 일치하지 않는다.
static int DA_validate( PENTRY start_entry, PENTRY last_entry )
{
	PENTRY entry, entry_nxt, old_entry = atNULL;

    if( start_entry == atNULL ) return -1;
    if( start_entry->prev != atNULL ) return -2;

	entry = start_entry;
	while( (entry != atNULL) && (entry >= start_entry) && (entry <= last_entry) ){
#ifdef CHECK_ENTRY_ID
        if( entry->entry_id != DA_ENTRY_ID ){
	        return -3;
        }
#endif

		if( /*(entry->status != ES_ALLOCATED) &&*/ (entry->status != ES_FREED) && (entry->status != ES_ALLOCATED) ){
	        return -4;
		}

/*
        if( entry->prev != atNULL ){
            if( DA_ENTRY_NEXT(entry->prev) != entry ){
	        	return -5;
            }
        }
*/

	 entry_nxt = DA_ENTRY_NEXT(entry);
        
        if( entry_nxt != atNULL ){
			long sz = DA_ENTRY_SIZE(entry);
			if( sz < 0 ){
	        	return -6;
			}
			if( (entry->status == ES_ALLOCATED) && (sz <= 0) ){
	        	return -6;
			}
/*
            if( entry_nxt->prev != entry ){
	        	return -6;
            }
*/
        }

		old_entry = entry;
		entry = entry_nxt;
	}

	if( old_entry != last_entry )
	    return -7;

	return 0;
}


/**
* 힙을 지정하고 초기화한다.
* @param pHeapStartAddr	힙의 포인터
* @param lSize				힙의 크기
* @return 결과값
*/
atBOOL atDALLOC_InitHeap( atVOID *pHeapStartAddr, atLONG lSize )
{
	if( pHeapStartAddr == atNULL ) return atFALSE;

    ((PHEAPHEADER)pHeapStartAddr)->header_id = DA_HEADER_ID;
    ((PHEAPHEADER)pHeapStartAddr)->heap_size = lSize;// - DA_HEAPHEADER_SIZE - DA_ENTRYHEADER_SIZE * 2;

    atDALLOC_ResetHeap( pHeapStartAddr );

	return( atTRUE );
}


/****************************************
  현재 힙을 초기화한다.
*****************************************/
atVOID atDALLOC_ResetHeap( atVOID *pHeapStartAddr )
{
	PENTRY entry, entry2;
	int size;

	if( pHeapStartAddr == atNULL )	return;
 
#ifdef FAST_ALLOC	
	((PHEAPHEADER)pHeapStartAddr)->last_access_ptr = atNULL;
#endif

	entry			= (PENTRY)((atUINTPTR)pHeapStartAddr + DA_HEAPHEADER_SIZE);
	size            = ((PHEAPHEADER)pHeapStartAddr)->heap_size - DA_HEAPHEADER_SIZE - DA_ENTRYHEADER_SIZE*2;

//	entry->next		= (PENTRY)((atUINTPTR)entry + size + DA_ENTRYHEADER_SIZE);
	entry->size		= size;
	entry->prev		= atNULL;
	entry->status	= ES_FREED;
	entry->attr		= 0x00;
#ifdef CHECK_ENTRY_ID	
	entry->entry_id	= DA_ENTRY_ID; //khyeo 2004.12.10 entry ID 추가
#endif

	entry2		= DA_ENTRY_NEXT(entry);
	entry2->prev	= entry;
	entry2->size	= 0;
	entry2->status	= ES_ALLOCATED;
	entry2->attr	= 0x00;
#ifdef CHECK_ENTRY_ID	
	entry2->entry_id= DA_ENTRY_ID; //khyeo 2004.12.10 entry ID 추가
#endif
}


/****************************************
  
*****************************************/
atBOOL atDALLOC_CheckValidateHeap( atVOID *pHeapStartAddr )
{
	PENTRY entry, last_entry;
	int size;

	if( pHeapStartAddr == atNULL )	return atFALSE;

	entry 	= (PENTRY)((atUINTPTR)pHeapStartAddr + DA_HEAPHEADER_SIZE);
	size            = ((PHEAPHEADER)pHeapStartAddr)->heap_size - DA_HEAPHEADER_SIZE - DA_ENTRYHEADER_SIZE*2;
	last_entry		= (PENTRY)((atUINTPTR)entry + size + DA_ENTRYHEADER_SIZE);

    return (atBOOL)(DA_validate( entry, last_entry ) == 0);
}


/****************************************
  
*****************************************/
atBOOL atDALLOC_CheckValidatePtr( atVOID *pHeapStartAddr, atVOID *pPtr )
{
	// 유효한 포인터인지 확인을 위해 처음부터 메모리를 뒤진다.
	PENTRY at = (PENTRY)((atUINTPTR)pHeapStartAddr + DA_HEAPHEADER_SIZE);
	PENTRY entry = (PENTRY)((char *)pPtr - DA_ENTRYHEADER_SIZE);

    if( DA_validate( at, DA_ENTRY_NEXT(at) ) < 0 ) return atFALSE;

	while( at != atNULL ){
	    if( at == entry ) break;
	    at = DA_ENTRY_NEXT(at);
	}
	return ( at != atNULL )? atTRUE : atFALSE;
}


/****************************************
  
*****************************************/
atLONG atDALLOC_GetTotalAllocSize( atVOID *pHeapStartAddr )
{
	PENTRY entry;
    long sum = 0;
    
	entry 	= (PENTRY)((atUINTPTR)pHeapStartAddr + DA_HEAPHEADER_SIZE);

    while( entry ){
	if( (entry->status == ES_ALLOCATED) && (DA_ENTRY_NEXT(entry) != atNULL) ){
        	sum += DA_ENTRY_SIZE(entry);
	}
        entry = DA_ENTRY_NEXT(entry);
    }
    
    return sum;
}


/****************************************
  전체 메모리 단편화를 제거한다.
*****************************************/
void atDALLOC_Compact( atVOID *pHeapStartAddr )
{
	PENTRY entry;

	if( pHeapStartAddr == atNULL ) return;

	entry	= (PENTRY)((atUINTPTR)pHeapStartAddr + DA_HEAPHEADER_SIZE);

	DA_compact( entry );
}


/****************************************
  메모리를 할당한다.
*****************************************/
atVOID * atDALLOC_AllocMem( atVOID *pHeapStartAddr, atLONG lSize )
{
    int size;
	PENTRY		entry, entry2;
	char *r;

	if( pHeapStartAddr == atNULL ) return atNULL;
	if( lSize <= 0 ) return atNULL;
	
#ifdef FAST_ALLOC	
	entry		= (PENTRY)(((PHEAPHEADER)pHeapStartAddr)->last_access_ptr);
#else
	entry		= (PENTRY)((char *)pHeapStartAddr + DA_HEAPHEADER_SIZE);
#endif
	size		= DA_ALIGNED_SIZE(lSize);

	// try to find a free piece of memory
	entry2	= DA_findFree(entry, size);
	if( entry2 == atNULL ){
#ifdef FAST_ALLOC	
		entry		= (PENTRY)((char *)pHeapStartAddr + DA_HEAPHEADER_SIZE);
		entry2	= DA_findFree(entry, size);
		if( entry2 == atNULL ){
#endif
		// compact and try again
		atDALLOC_Compact( pHeapStartAddr );

		entry2 = DA_findFree( entry, size );
		if( entry2 == atNULL ){
			return atNULL;
		}
#ifdef FAST_ALLOC	
		}
#endif
	}

	DA_split( entry2, size );

//if( !atDALLOC_CheckValidateHeap(pHeapStartAddr) ) return atNULL;

	r = (char *)entry2 + DA_ENTRYHEADER_SIZE;

	return( (void *)r );
}


/****************************************
  끝에서부터 역으로 메모리를 할당한다.
  주로 임시로 쓸 메모리를 할당할 때 쓴다.
*****************************************/
atVOID * atDALLOC_AllocMemFromLast( atVOID *pHeapStartAddr, atLONG lSize )
{
  int size;
	PENTRY		entry;
	unsigned long r;

	if( pHeapStartAddr == atNULL ) return atNULL;

	size		= ((atUINTPTR)(lSize + DA_ALIGN - 1)/DA_ALIGN)*DA_ALIGN + DA_LEFTOVER;
    entry       = (PENTRY)((char *)pHeapStartAddr + DA_HEAPHEADER_SIZE + size - DA_ENTRYHEADER_SIZE);
    
	// try to find a free piece of memory 
	entry = DA_findFreeLast( entry, size );
	if( entry == atNULL ){
		// compact and try again 
		atDALLOC_Compact( pHeapStartAddr );
		
		entry = DA_findFreeLast( entry, size );
		if( entry == atNULL ){
			return atNULL;
		}
	}  

	DA_split( entry, lSize );

	r = (atUINTPTR)entry + DA_ENTRYHEADER_SIZE;

	return( (void *)(atINTPTR)r );
}


/****************************************

*****************************************/
atVOID * atDALLOC_AllocAlignedMem( atVOID *pHeapStartAddr, atLONG lSize, atINT iAlign )
{
    int size;
	PENTRY		entry;
	unsigned long entryTarget;
	unsigned long r, size2;

	if( pHeapStartAddr == atNULL ){
		return( atNULL );
	}

	entry		= (PENTRY)((char *)pHeapStartAddr + DA_HEAPHEADER_SIZE);
	size		= ((atUINTPTR)(lSize + DA_ALIGN - 1)/DA_ALIGN)*DA_ALIGN + DA_LEFTOVER;

	// try to find a free piece of memory 
	entry	= DA_findAlignedFree( entry, size, iAlign );
	if( entry == atNULL ){
		// compact and try again 
		atDALLOC_Compact(pHeapStartAddr);
		
		entry = DA_findAlignedFree(pHeapStartAddr, size, iAlign);
		if (entry == atNULL){
			return(atNULL);
		}
	}
	
	entryTarget = (((atUINTPTR)entry + iAlign - 1)/iAlign)*iAlign;

	if( (entryTarget - (atUINTPTR)entry ) < (DA_ENTRYHEADER_SIZE*2) ){
		entryTarget = entryTarget + iAlign;
	}
	
	size2 = entryTarget - (atUINTPTR)entry -DA_ENTRYHEADER_SIZE; //자신의 헤더 공간을 뺌.
	DA_split( entry, size2-DA_ENTRYHEADER_SIZE ); // next 의 header를 위한 공간을 빼야함.
	DA_split( DA_ENTRY_NEXT(entry), size );
	entry->status = ES_FREED;

	r = (atUINTPTR)(DA_ENTRY_NEXT(entry)) + DA_ENTRYHEADER_SIZE;

	return( (void *)(atINTPTR)r );
}


/****************************************
  할당된 메모리를 해제한다.
*****************************************/
atBOOL atDALLOC_FreeMem( atVOID *pHeapStartAddr, atVOID *pPtr )
{
	PENTRY  entry;

	if( pHeapStartAddr == atNULL ) return atFALSE;  
	if( pPtr == atNULL ) return atFALSE; 
	
//if( !atDALLOC_CheckValidateHeap(pHeapStartAddr) ) return atFALSE;

	entry = (PENTRY)((atUINTPTR)pPtr - DA_ENTRYHEADER_SIZE);

#if 0 /// 극한의 경우에... 디버깅용으로만 사용. 무지 느림.
	{
		// 유효한 포인터인지 확인을 위해 처음부터 메모리를 뒤진다.
		PENTRY at = (PENTRY)((atUINTPTR)pHeapStartAddr + DA_HEAPHEADER_SIZE);
		while( at != atNULL ){
		    	if (at == entry) break;
		    	at = DA_ENTRY_NEXT(at);
		}
		if( at == atNULL ) return atFALSE - 2;
	}
#endif    

#ifdef CHECK_ENTRY_ID
	if( entry->entry_id != DA_ENTRY_ID ) return atFALSE - 3;
#endif
	if( entry->status != ES_ALLOCATED ) return atFALSE - 4;

	entry->status = ES_FREED;

//if( !atDALLOC_CheckValidateHeap(pHeapStartAddr) ) return atFALSE;

	entry = DA_compact( entry );

#ifdef FAST_ALLOC	
	((PHEAPHEADER)pHeapStartAddr)->last_access_ptr = (void *)entry;
#endif

//if( !atDALLOC_CheckValidateHeap(pHeapStartAddr) ) return atFALSE;

	return atTRUE;
}


/****************************************
  현재 포인터에 할당된 메모리의 크기를 구한다.
*****************************************/
atLONG atDALLOC_GetAllocSize( atVOID *pHeapStartAddrPtr, atVOID *pPtr )
{
	PENTRY  entry;

	if( pHeapStartAddrPtr == atNULL ) return 0; 
	if( pPtr == atNULL ) return 0; 

	entry		= (PENTRY)((atUINTPTR)pPtr - DA_ENTRYHEADER_SIZE);

	if( DA_ENTRY_NEXT(entry) == atNULL ) return 0;

	return( DA_ENTRY_SIZE( entry ) );
}


/****************************************
  메모리를 재할당한다.
  (메모리 복사시의 오버헤드를 줄이기 위한 알고리즘 적용)
*****************************************/
atVOID * atDALLOC_ReallocMem( atVOID *pHeapStartAddr, atVOID *pPtr, atLONG lSize )
{
	void			*pPtr2;
	long	oldsize, size;//, sz2;
//	PENTRY  entry, entry2;

	if( pHeapStartAddr == atNULL ) return atNULL;

    // 현재 할당된 사이즈
    oldsize = atDALLOC_GetAllocSize( pHeapStartAddr, pPtr );
    // 예전 포인터가 mtNULL이거나 예전 할당된 사이즈가 0이면 완전히 재할당.
    if( pPtr == atNULL || oldsize <= 0 ) return atDALLOC_AllocMem( pHeapStartAddr, lSize );
    // 예전 사이즈와 현재 사이즈가 같으면 예전 포인터 그냥 리턴
    if( oldsize == lSize ) return pPtr;

// 새로 메모리를 할당하고, 메모리를 복사한 후, 원래 메모리를 지운다.
pPtr2 = atDALLOC_AllocMem( pHeapStartAddr, lSize );
if( pPtr2 == atNULL ) return atNULL;
size		= DA_MIN( oldsize, lSize );
memcpy( pPtr2, pPtr, size );
atDALLOC_FreeMem( pHeapStartAddr, pPtr );
pPtr = pPtr2;

// 문제가 있어서 임시로 막음.
/*
	entry = (PENTRY)((atUINTPTR)pPtr - DA_ENTRYHEADER_SIZE);

    if( oldsize > lSize ){
        // 현재 할당된 사이즈가 지정된 사이즈보다 클 때...
        // 할당된 크기만 줄인다.
        entry2 = DA_split( entry, lSize );
        // 그 뒷부분이 free 메모리 영역이면 합친다.
        DA_compact( entry2 );
    } else {
        entry2 = DA_ENTRY_NEXT(entry);
        sz2 = lSize - oldsize;
        // 할당된 바로 뒤의 메모리 엔트리가 비었는가?
        // 빈 곳의 크기가 필요로 하는 크기보다 큰가?        
        if( (entry2->status == ES_FREED) && ((long)DA_ENTRY_SIZE(entry2) >= sz2) ){
            // 할당된 영역만 늘인다.
            entry2 = DA_split( entry, lSize );
            // 그 뒷부분이 free 메모리 영역이면 합친다.
            DA_merge_next( entry2 );
        } else {
            // 새로 메모리를 할당하고, 메모리를 복사한 후, 원래 메모리를 지운다.
		    pPtr2 = atDALLOC_AllocMem( pHeapStartAddr, lSize );
		    if( pPtr2 == atNULL ) return atNULL;

        	size		= DA_MIN( oldsize, lSize );
        	memcpy( pPtr2, pPtr, size );
        	atDALLOC_FreeMem( pHeapStartAddr, pPtr );

        	pPtr = pPtr2;
        }
    }
*/

	return( pPtr );
}


/****************************************
  할당 가능한 최대 메모리 크기를 얻는다.
*****************************************/
atLONG atDALLOC_GetHeapSize( atVOID *pHeapStartAddr )
{
    if( pHeapStartAddr == atNULL ) return( 0 );

    return ((PHEAPHEADER)pHeapStartAddr)->heap_size;
}


/****************************************
  할당 가능한 최대 메모리 크기를 얻는다.
*****************************************/
atLONG atDALLOC_GetLeftSize( atVOID *pHeapStartAddr )
{
    int maxsize, sz;
    PENTRY atNext;
    PENTRY at;

    if( pHeapStartAddr == atNULL ) return( 0 );

	atDALLOC_Compact(pHeapStartAddr);
		
    at       = (PENTRY)((atUINTPTR)pHeapStartAddr + DA_HEAPHEADER_SIZE);

    maxsize = 0;
    while( at != atNULL ){
        atNext = DA_ENTRY_NEXT(at);
        while( (at->status == ES_FREED) && (atNext != atNULL) ){
			if (atNext->status != ES_FREED) break;
			atNext = DA_ENTRY_NEXT(atNext);
        }
        sz = DA_ENTRY_SIZE(at);
        if( sz >= maxsize ) maxsize = sz;
        at = DA_ENTRY_NEXT(at);
    }

    return( maxsize );
}


/****************************************
  남은 메모리의 총 크기를 얻는다.
*****************************************/
atLONG atDALLOC_GetTotalLeftSize( atVOID *pHeapStartAddr )
{
    int maxsize, sz;
    PENTRY at;

    if( pHeapStartAddr == atNULL ) return( 0 );
    
	atDALLOC_Compact(pHeapStartAddr);
		
    at = (PENTRY)((atUINTPTR)pHeapStartAddr + DA_HEAPHEADER_SIZE);

    maxsize = 0;
    while( at != atNULL ){
		if( (at->status == ES_FREED) && (DA_ENTRY_NEXT(at) != atNULL) ){
			sz = DA_ENTRY_SIZE(at);
			maxsize += sz;
		}
		at = DA_ENTRY_NEXT(at);
    }

    return( maxsize );
}


/****************************************
  현재 메모리 사용 현황을 얻는다. (Debug)
*****************************************/
atVOID atDALLOC_GetHeapInfo( atVOID *pHeapStartAddr, atVOID *lpParam, void (*pViewFunc)(atVOID *lpParam, void *addr, int pos, int size, int type) )
{
    PENTRY start, at;
	int pos, sz;

    if( pHeapStartAddr == atNULL ) return;
	if( pViewFunc == atNULL ) return;
    start = at = (PENTRY)((atUINTPTR)pHeapStartAddr + DA_HEAPHEADER_SIZE);

    while( at != atNULL ){
		sz = DA_ENTRY_SIZE(at);
		pos = ((atINTPTR)at - (atINTPTR)start);
		if( sz > 0 ) pViewFunc( lpParam, (void *)at, pos, sz+DA_ENTRYHEADER_SIZE, at->status );
		at = DA_ENTRY_NEXT(at);
    }
}
 


/*===============================================================*/


atVOID  * __g_pGlobalHeap;
atLONG    __g_lGlobalHeapSize;

atVOID           atDALLOC_InitGlobalHeap( atVOID *pHeapStartAddrPtr, atLONG lSize )
{
    atDALLOC_InitHeap( pHeapStartAddrPtr, lSize );
    __g_pGlobalHeap = pHeapStartAddrPtr;
    __g_lGlobalHeapSize = lSize;    
}

atVOID           atDALLOC_ResetGlobalHeap( NOARGS )
{
    atDALLOC_ResetHeap( __g_pGlobalHeap );
}

atBOOL           atDALLOC_CheckGlobalValidateHeap( NOARGS )
{
    return atDALLOC_CheckValidateHeap( __g_pGlobalHeap );
}

atBOOL           atDALLOC_CheckGlobalValidatePtr( atVOID *pPtr )
{
    return atDALLOC_CheckValidatePtr( __g_pGlobalHeap, pPtr );
}

atLONG           atDALLOC_GetGlobalHeapSize( NOARGS )
{
    return __g_lGlobalHeapSize;
}

atLONG           atDALLOC_GetGlobalLeftSize( NOARGS )
{
    return atDALLOC_GetLeftSize( __g_pGlobalHeap );
}

atLONG           atDALLOC_GetGlobalTotalLeftSize( NOARGS )
{
    return atDALLOC_GetTotalLeftSize( __g_pGlobalHeap );
}

atVOID           atDALLOC_CompactGlobalHeap( NOARGS )
{
    atDALLOC_Compact( __g_pGlobalHeap );
}

atVOID *atDALLOC_AllocGlobalMem( atLONG lSize )
{
    return atDALLOC_AllocMem( __g_pGlobalHeap, lSize );
}

atVOID *atDALLOC_AllocGlobalMemLast( atLONG lSize )
{
    return atDALLOC_AllocMemFromLast( __g_pGlobalHeap, lSize );
}

atBOOL atDALLOC_FreeGlobalMem( atVOID *pPtr )
{
    return atDALLOC_FreeMem( __g_pGlobalHeap, pPtr );
}

atVOID * atDALLOC_ReallocGlobalMem( atVOID *pPtr, atLONG lSize )
{
    return atDALLOC_ReallocMem( __g_pGlobalHeap, pPtr, lSize );
}

atVOID atDALLOC_GetGlobalHeapInfo( atVOID *lpParam, void (*pViewFunc)(atVOID *lpParam, void *addr, int pos, int size, int type) )
{
	atDALLOC_GetHeapInfo( __g_pGlobalHeap, lpParam, pViewFunc );
}


/*===============================================================*/


typedef struct tagatSECTIONMEM {
    atBOOL      bActivate;
    atDWORD     dwID;
    atVOID    * pHeapStartAddr;
    atLONG      lHeapSize;
    atINT       iAllocCount;
    atLONG      iTotalAllocSize;
} atSECTIONMEM;

static atSECTIONMEM g_SectionMemList[ MAX_SECTIONMEM_COUNT ];
static atINT        g_SectionMemCount;


atHSECTIONMEM atDALLOC_CreateSectionMem( atINT iAttr, atLONG lHeapSize )
{
    int i, sn;
    atSECTIONMEM *psection;
  
    sn = 0;
    for( i = 0; i < MAX_SECTIONMEM_COUNT; i ++ ){
        if( g_SectionMemList[i].bActivate == atFALSE ){
            sn = i + 1;
            break;
        }
    }
    if( sn <= 0 ) return 0;

    psection = (atSECTIONMEM *)&g_SectionMemList[ sn - 1 ];
    if( iAttr == 0 ){
        psection->pHeapStartAddr = atDALLOC_AllocGlobalMem( sizeof(atDWORD) + lHeapSize );
    } else {
        psection->pHeapStartAddr = atDALLOC_AllocGlobalMemLast( sizeof(atDWORD) + lHeapSize );
    }
    *(atDWORD *)psection->pHeapStartAddr = DA_SECTION_ID;
    atDALLOC_InitHeap( (char *)psection->pHeapStartAddr + sizeof(atDWORD), /*sizeof(atDWORD) +*/ lHeapSize );
    psection->lHeapSize = lHeapSize;

    psection->bActivate = atTRUE;

    return sn;
}


atVOID atDALLOC_DestroySectionMem( atHSECTIONMEM hSectionMem )
{
    atSECTIONMEM *psection;
    
    if( hSectionMem <= 0 ) return;
    
    psection = (atSECTIONMEM *)&g_SectionMemList[ hSectionMem - 1 ];

    atDALLOC_FreeGlobalMem( psection->pHeapStartAddr );
//    atDALLOC_FreeGlobalMem( psection );
    
    psection->bActivate = atFALSE;
}


atVOID *atDALLOC_AllocLocalMem( atHSECTIONMEM hSectionMem, atLONG lSize )
{
    void *ptr;
    
    atSECTIONMEM *psection;

    if( hSectionMem == 0 ){
      ptr = atDALLOC_AllocGlobalMem( lSize );
    } else {
      psection = (atSECTIONMEM *)&g_SectionMemList[ hSectionMem - 1 ];
  
      // Check Validate
//      if( *(atDWORD *)psection->pHeapStartAddr != psection->dwID ) return atNULL;

      ptr = atDALLOC_AllocMem( (char *)psection->pHeapStartAddr + sizeof(atDWORD), lSize );

      if( ptr ) psection->iAllocCount ++;
    }

    return ptr;
}


atBOOL atDALLOC_FreeLocalMem( atHSECTIONMEM hSectionMem, atVOID *pPtr )
{
	atBOOL r = atFALSE;
    atSECTIONMEM *psection;

    if( pPtr == atNULL ) return atFALSE;

    if( hSectionMem == 0 ){
      r = atDALLOC_FreeGlobalMem( pPtr );
    } else {
      psection = (atSECTIONMEM *)&g_SectionMemList[ hSectionMem - 1 ];

      // Check Validate
//      if( *(atDWORD *)psection->pHeapStartAddr != psection->dwID ) return atFALSE;

      r = atDALLOC_FreeMem( (char *)psection->pHeapStartAddr + sizeof(atDWORD), pPtr );

      psection->iAllocCount --;
    }

    return r;
}


atVOID *atDALLOC_ReallocLocalMem( atHSECTIONMEM hSectionMem, atVOID *pPtr, atLONG lSize )
{
    void *ptr;
    
    atSECTIONMEM *psection;

    if( hSectionMem == 0 ){
      ptr = atDALLOC_AllocGlobalMem( lSize );
    } else {
      psection = (atSECTIONMEM *)&g_SectionMemList[ hSectionMem - 1 ];
  
      // Check Validate
//      if( *(atDWORD *)psection->pHeapStartAddr != psection->dwID ) return atNULL;

      ptr = atDALLOC_ReallocMem( (char *)psection->pHeapStartAddr + sizeof(atDWORD), pPtr, lSize );
    }

    return ptr;
}


atLONG atDALLOC_GetLocalHeapSize( atHSECTIONMEM hSectionMem )
{
    atSECTIONMEM *psection;
    
    if( hSectionMem == 0 ){
      return atDALLOC_GetGlobalHeapSize();
    } else {
      psection = (atSECTIONMEM *)&g_SectionMemList[ hSectionMem - 1 ];
      return psection->lHeapSize;
    }
}

atLONG atDALLOC_GetLocalTotalAllocSize( atHSECTIONMEM hSectionMem )
{
    atSECTIONMEM *psection;
    
    if( hSectionMem == 0 ){
//      return atDALLOC_GetGlobalTotalAllocSize();
      return atDALLOC_GetTotalAllocSize(__g_pGlobalHeap);
    } else {
      psection = (atSECTIONMEM *)&g_SectionMemList[ hSectionMem - 1 ];

      return atDALLOC_GetTotalAllocSize( (char *)psection->pHeapStartAddr + sizeof(atDWORD) );
    }
}


atINT atDALLOC_GetLocalAllocCount( atHSECTIONMEM hSectionMem )
{
    atSECTIONMEM *psection;

    if( hSectionMem == 0 ){
      return 0;
    } else {
      psection = (atSECTIONMEM *)&g_SectionMemList[ hSectionMem - 1 ];

      return psection->iAllocCount;
    }    
}


atVOID atDALLOC_GetLocalHeapInfo( atHSECTIONMEM hSectionMem, atVOID *lpParam, void (*pViewFunc)(atVOID *lpParam, void *addr, int pos, int size, int type) )
{
    atSECTIONMEM *psection;

    if( hSectionMem == 0 ){
//	  atDALLOC_GetGlobalHeapInfo( lpParam, pViewFunc );
      return;
    } else {
      psection = (atSECTIONMEM *)&g_SectionMemList[ hSectionMem - 1 ];

      atDALLOC_GetHeapInfo( (char *)psection->pHeapStartAddr + sizeof(atDWORD), lpParam, pViewFunc );
    }    
}


