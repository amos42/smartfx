#ifndef __A_DALLOC_H_
#define __A_DALLOC_H_


#include "AT_Types.h"


#ifdef __cplusplus
extern "C" {
#endif
    
    
/* Dynamic Alloc Functions */
atBOOL           atDALLOC_InitHeap( atVOID *pHeapStartAddr, atLONG lSize );
atVOID           atDALLOC_ResetHeap( atVOID *pHeapStartAddr );
atBOOL           atDALLOC_CheckValidateHeap( atVOID *pHeapStartAddr );
atBOOL		 atDALLOC_CheckValidatePtr( atVOID *pHeapStartAddr, atVOID *pPtr );
atLONG		atDALLOC_GetHeapSize( atVOID *pHeapStartAddr );
atLONG           atDALLOC_GetTotalAllocSize( atVOID *pHeapStartAddr );    
atLONG           atDALLOC_GetLeftSize( atVOID *pHeapStartAddr );
atLONG           atDALLOC_GetTotalLeftSize( atVOID *pHeapStartAddr );
atVOID           atDALLOC_Compact( atVOID *pHeapStartAddr );

atVOID         * atDALLOC_AllocMem( atVOID *pHeapStartAddr, atLONG lSize );
atVOID         * atDALLOC_AllocMemFromLast( atVOID *pHeapStartAddr, atLONG lSize );
atVOID         * atDALLOC_AllocAlignedMem( atVOID *pHeapStartAddr, atLONG lSize, atINT iAlign );
atBOOL           atDALLOC_FreeMem( atVOID *pHeapStartAddr, atVOID *pPtr );
atLONG           atDALLOC_GetAllocSize( atVOID *pHeapStartAddr, atVOID *pPtr );
atVOID         * atDALLOC_ReallocMem( atVOID *pHeapStartAddr, atVOID *pPtr, atLONG lSize );

atVOID 			 atDALLOC_GetHeapInfo( atVOID *pHeapStartAddr, atVOID *lpParam, void (*pViewFunc)(void *lpParam, void *addr, int pos, int size, int type) );

/* Global Heap */
extern atVOID  * __g_pGlobalHeap;
extern atLONG    __g_lGlobalHeapSize;

atVOID           atDALLOC_InitGlobalHeap( atVOID *pGlobalHeapPtr, atLONG lSize );
atVOID           atDALLOC_ResetGlobalHeap( NOARGS );
atBOOL           atDALLOC_CheckGlobalValidateHeap( NOARGS );
atBOOL           atDALLOC_CheckGlobalValidatePtr( atVOID *pPtr );
atLONG           atDALLOC_GetGlobalHeapSize( NOARGS );
atLONG           atDALLOC_GetGlobalLeftSize( NOARGS );
atLONG           atDALLOC_GetGlobalTotalLeftSize( NOARGS );
atVOID           atDALLOC_CompactGlobalHeap( NOARGS );

atVOID         * atDALLOC_AllocGlobalMem( atLONG lSize );
atVOID         * atDALLOC_AllocGlobalMemLast( atLONG lSize );
atBOOL           atDALLOC_FreeGlobalMem( atVOID *pPtr );
atVOID 	      * atDALLOC_ReallocGlobalMem( atVOID *pPtr, atLONG lSize );

atVOID 			 atDALLOC_GetGlobalHeapInfo( atVOID *lpParam, void (*pViewFunc)(atVOID *lpParam, void *addr, int pos, int size, int type) );

/* Section & Local Heap */
#define          MAX_SECTIONMEM_COUNT 64
typedef atDWORD    atHSECTIONMEM;

atHSECTIONMEM    atDALLOC_CreateSectionMem( atINT iAttr, atLONG lHeapSize );
atVOID           atDALLOC_DestroySectionMem( atHSECTIONMEM hSectionMem );
atINT            atDALLOC_GetSectionMemCount( NOARGS );
atHSECTIONMEM    atDALLOC_GetSectionMem( atINT iSectionMemNum );
//atBOOL           atDALLOC_ActivateSectionMem( atHSECTIONMEM hSectionMem, atBOOL bActivate );

atVOID         * atDALLOC_AllocLocalMem( atHSECTIONMEM hSectionMem, atLONG lSize );
atBOOL           atDALLOC_FreeLocalMem( atHSECTIONMEM hSectionMem, atVOID *pMem );
atVOID 	      * atDALLOC_ReallocLocalMem( atHSECTIONMEM hSectionMem, atVOID *pPtr, atLONG lSize );

atLONG           atDALLOC_GetLocalHeapSize( atHSECTIONMEM hSectionMem );
atLONG           atDALLOC_GetLocalTotalAllocSize( atHSECTIONMEM hSectionMem );
atINT            atDALLOC_GetLocalAllocCount( atHSECTIONMEM hSectionMem );

atVOID 			 atDALLOC_GetLocalHeapInfo( atHSECTIONMEM hSectionMem, atVOID *lpParam, void (*pViewFunc)(atVOID *lpParam, void *addr, int pos, int size, int type) );


#ifdef __cplusplus
};
#endif
    

#endif  // __A_DALLOC_H_
