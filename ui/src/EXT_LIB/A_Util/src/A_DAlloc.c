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
Update				: 2005/02/18						RYOJIN, KHYEO (�ӵ� ���)
Update				: 2005/02/21						RYOJIN (����ȭ)
Update				: 2005/04/12 					khyeo (port to RMEM)
Update				: 2005/09/01					gmju (SectionMem �����Ͽ� ����)
Update				: 2008/07/11					gmju (entry_id�� �� �� ����)
Update				: 2009/03/05					gmju (������ �� Heap ������ ��� �Լ� �߰�)
C Compiler			: ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
Assembler			: ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
Description			: Dynamic Allocation related fucntion definition

-------------------------------------------------------------------------------
#############################################################################*/
#include "A_DAlloc.h"

#include <stdlib.h>
#include <string.h>

//=========================================
#define CHECK_ENTRY_ID          /// �޸� ���Ἲ�� üũ�ϱ� ����... ��� �ణ�� �޸𸮿� �ӵ��� �� �Ҹ��Ѵ�.
#define FAST_ALLOC              /// �޸𸮸� ������ �Ҵ��ϱ� ����... Ư���� ��쿡 ���� ������ �� ������ ���� �ִ�.
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


//	�� �޸� �� �Ӽ�
typedef enum {
    ES_INVALID,     // ��ȿ�� �� - ���� ��� �� ��.
    ES_FREED,       // �Ҵ� �� �� ��
    ES_ALLOCATED    // �Ҵ�� ��
} ENTRYSTATUS;

#define DA_HEADER_ID		(0x50414548)    /// ASCII 'H' 'E' 'A' 'P'
#define DA_SECTION_ID		(0x54434553)    /// ASCII 'S' 'E' 'C' 'T'
#ifdef CHECK_ENTRY_ID
//#define DA_ENTRY_ID		(0x52544E45)    /// ASCII 'E' 'N' 'T' 'R'
#define DA_ENTRY_ID			(0x5445)		/// ASCII 'E' 'T'
#define DA_INVALID_ENTRY_ID	(0x0000)		/// 
#endif

// Heap�� ���
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

// �� Entry�� ���
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
	struct tagENTRY *     prev;  // �Ҵ� ��Ŀ���� ��ü�� next������ �����ϳ�, �޸� ����ȭ�� ���̱� ���� ��������...
//	struct tagENTRY *     next;
} ENTRY, * PENTRY; // 12~16 Bytes


#define DA_ALIGN				(sizeof(atDWORD)) /// 4�� ����̾�� ��.
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


// �Ҵ���� ���� ���� ã�´�. ���ο�
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


// �Ҵ���� ���� ���� ���ĵ� ������� ã�´�. ���ο�
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
				    //size�� entry�� ���ġ ����.
				}
			}
		}

		entry = DA_ENTRY_NEXT(entry);
	}

	return( entry );
}


// �Ҵ���� ���� ���� �޸� ���������� ã�´�. (���ο�)
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


// ���� �ּҸ� ��������, �޸𸮸� �Ҵ�� ������ �߶󳽴�. (���ο�)
static PENTRY DA_split( PENTRY entry, unsigned long aligned_size )
{
	PENTRY newb, next_entry;

//	if ( !(DA_ENTRY_SIZE(entry) < (aligned_size + DA_ENTRYHEADER_SIZE)) ){ // �̹� ������ �����Ǵ��� �����Ƿ�...

	// ���� ���� ũ�Ⱑ �Ҵ��ϰ��� �ϴ� ũ�⿡ �����ϸ� �׳� �Ӽ��� �ٲ۴�.
	if( DA_ENTRY_SIZE(entry) <= DA_ENTRYHEADER_SIZE + aligned_size ){
		entry->status = ES_ALLOCATED;
		return entry;
	}
		
		newb				= (PENTRY)((atUINTPTR)entry + DA_ENTRYHEADER_SIZE + aligned_size);
#ifdef CHECK_ENTRY_ID	
	    newb->entry_id	    = DA_ENTRY_ID; //khyeo 2004.12.10 entry ID �߰�
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


// ���� �ּҸ� ��������, ���� ���� �޸𸮸� ������ ��ģ��. (���ο�)
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


// ���� �ּҸ� ��������, ���� ���� �޸𸮸� ������ ��ģ��. (���ο�)
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


// �ش� ����Ʈ ���ķ� �޸� ����ȭ�� �����Ѵ�. (���ο�)
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


// �޸𸮰� �������� üũ�Ѵ�. (���ο�)
// 0  : ����
// -1 : ���� ����Ʈ�� NULL
// -2 : ���� ����Ʈ�� ������ �ƴϴ�. (���� ������ ����Ű�� �����Ͱ� NULL�� �ƴϴ�.)
// -3 : ��Ʈ�� ���� ������. (Entry ID�� ��ġ���� �ʴ´�.)
// -4 : ��ȿ���� ���� �Ӽ��̴�. (�Ҵ�, ����, ��𿡵� �ش� �� �ȴ�.)
// -5 : ���� �����Ϳ��� ������ ���� ��ġ���� �ʴ´�.
// -6 : ���� �����Ϳ��� ������ ���� ��ġ���� �ʴ´�.
// -7 : �޸� ��ü ���� ����, ������ ���� ���� ������ ��ġ���� �ʴ´�.
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
* ���� �����ϰ� �ʱ�ȭ�Ѵ�.
* @param pHeapStartAddr	���� ������
* @param lSize				���� ũ��
* @return �����
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
  ���� ���� �ʱ�ȭ�Ѵ�.
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
	entry->entry_id	= DA_ENTRY_ID; //khyeo 2004.12.10 entry ID �߰�
#endif

	entry2		= DA_ENTRY_NEXT(entry);
	entry2->prev	= entry;
	entry2->size	= 0;
	entry2->status	= ES_ALLOCATED;
	entry2->attr	= 0x00;
#ifdef CHECK_ENTRY_ID	
	entry2->entry_id= DA_ENTRY_ID; //khyeo 2004.12.10 entry ID �߰�
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
	// ��ȿ�� ���������� Ȯ���� ���� ó������ �޸𸮸� ������.
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
  ��ü �޸� ����ȭ�� �����Ѵ�.
*****************************************/
void atDALLOC_Compact( atVOID *pHeapStartAddr )
{
	PENTRY entry;

	if( pHeapStartAddr == atNULL ) return;

	entry	= (PENTRY)((atUINTPTR)pHeapStartAddr + DA_HEAPHEADER_SIZE);

	DA_compact( entry );
}


/****************************************
  �޸𸮸� �Ҵ��Ѵ�.
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
  ���������� ������ �޸𸮸� �Ҵ��Ѵ�.
  �ַ� �ӽ÷� �� �޸𸮸� �Ҵ��� �� ����.
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
	
	size2 = entryTarget - (atUINTPTR)entry -DA_ENTRYHEADER_SIZE; //�ڽ��� ��� ������ ��.
	DA_split( entry, size2-DA_ENTRYHEADER_SIZE ); // next �� header�� ���� ������ ������.
	DA_split( DA_ENTRY_NEXT(entry), size );
	entry->status = ES_FREED;

	r = (atUINTPTR)(DA_ENTRY_NEXT(entry)) + DA_ENTRYHEADER_SIZE;

	return( (void *)(atINTPTR)r );
}


/****************************************
  �Ҵ�� �޸𸮸� �����Ѵ�.
*****************************************/
atBOOL atDALLOC_FreeMem( atVOID *pHeapStartAddr, atVOID *pPtr )
{
	PENTRY  entry;

	if( pHeapStartAddr == atNULL ) return atFALSE;  
	if( pPtr == atNULL ) return atFALSE; 
	
//if( !atDALLOC_CheckValidateHeap(pHeapStartAddr) ) return atFALSE;

	entry = (PENTRY)((atUINTPTR)pPtr - DA_ENTRYHEADER_SIZE);

#if 0 /// ������ ��쿡... ���������θ� ���. ���� ����.
	{
		// ��ȿ�� ���������� Ȯ���� ���� ó������ �޸𸮸� ������.
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
  ���� �����Ϳ� �Ҵ�� �޸��� ũ�⸦ ���Ѵ�.
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
  �޸𸮸� ���Ҵ��Ѵ�.
  (�޸� ������� ������带 ���̱� ���� �˰��� ����)
*****************************************/
atVOID * atDALLOC_ReallocMem( atVOID *pHeapStartAddr, atVOID *pPtr, atLONG lSize )
{
	void			*pPtr2;
	long	oldsize, size;//, sz2;
//	PENTRY  entry, entry2;

	if( pHeapStartAddr == atNULL ) return atNULL;

    // ���� �Ҵ�� ������
    oldsize = atDALLOC_GetAllocSize( pHeapStartAddr, pPtr );
    // ���� �����Ͱ� mtNULL�̰ų� ���� �Ҵ�� ����� 0�̸� ������ ���Ҵ�.
    if( pPtr == atNULL || oldsize <= 0 ) return atDALLOC_AllocMem( pHeapStartAddr, lSize );
    // ���� ������� ���� ����� ������ ���� ������ �׳� ����
    if( oldsize == lSize ) return pPtr;

// ���� �޸𸮸� �Ҵ��ϰ�, �޸𸮸� ������ ��, ���� �޸𸮸� �����.
pPtr2 = atDALLOC_AllocMem( pHeapStartAddr, lSize );
if( pPtr2 == atNULL ) return atNULL;
size		= DA_MIN( oldsize, lSize );
memcpy( pPtr2, pPtr, size );
atDALLOC_FreeMem( pHeapStartAddr, pPtr );
pPtr = pPtr2;

// ������ �־ �ӽ÷� ����.
/*
	entry = (PENTRY)((atUINTPTR)pPtr - DA_ENTRYHEADER_SIZE);

    if( oldsize > lSize ){
        // ���� �Ҵ�� ����� ������ ������� Ŭ ��...
        // �Ҵ�� ũ�⸸ ���δ�.
        entry2 = DA_split( entry, lSize );
        // �� �޺κ��� free �޸� �����̸� ��ģ��.
        DA_compact( entry2 );
    } else {
        entry2 = DA_ENTRY_NEXT(entry);
        sz2 = lSize - oldsize;
        // �Ҵ�� �ٷ� ���� �޸� ��Ʈ���� ����°�?
        // �� ���� ũ�Ⱑ �ʿ�� �ϴ� ũ�⺸�� ū��?        
        if( (entry2->status == ES_FREED) && ((long)DA_ENTRY_SIZE(entry2) >= sz2) ){
            // �Ҵ�� ������ ���δ�.
            entry2 = DA_split( entry, lSize );
            // �� �޺κ��� free �޸� �����̸� ��ģ��.
            DA_merge_next( entry2 );
        } else {
            // ���� �޸𸮸� �Ҵ��ϰ�, �޸𸮸� ������ ��, ���� �޸𸮸� �����.
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
  �Ҵ� ������ �ִ� �޸� ũ�⸦ ��´�.
*****************************************/
atLONG atDALLOC_GetHeapSize( atVOID *pHeapStartAddr )
{
    if( pHeapStartAddr == atNULL ) return( 0 );

    return ((PHEAPHEADER)pHeapStartAddr)->heap_size;
}


/****************************************
  �Ҵ� ������ �ִ� �޸� ũ�⸦ ��´�.
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
  ���� �޸��� �� ũ�⸦ ��´�.
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
  ���� �޸� ��� ��Ȳ�� ��´�. (Debug)
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


