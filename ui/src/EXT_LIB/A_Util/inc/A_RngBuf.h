#ifndef __A_RNGBUF_H_
#define __A_RNGBUF_H_


#include "AT_Types.h"


//#define atRINGBUF_USE_OWNMEMORY   /// 자체 할당 메모리 사용 여부
//#define atRINGBUF_USE_SECTIONMEM  /// Section 메모리 사용 여부


#ifdef __cplusplus
extern "C" {
#endif 


typedef struct _tagatRINGBUFFUNCS {
	atLPVOID			lpData;
	atLPCALLBACKALLOC   fnAllocMem;
	atLPCALLBACKFREE    fnFreeMem;
} atRINGBUFFUNCS;


typedef atINT    (atRINGBUF_DELETEFUNC)( atVOID * pPtr );


typedef atLONG atRINGMODE;

#define atRINGMODE_WR_MASK              0x0000FFFF
//#define atRINGMODE_WR_FULLDATA          0x00000000
#define atRINGMODE_WR_DROPWRITE         0x00000001
#define atRINGMODE_WR_ALLOWPARTICLE     0x00000002
//#define atRINGMODE_WR_NOINC             0x00000004

#define atRINGMODE_RD_MASK              0xFFFF0000
//#define atRINGMODE_RD_FULLDATA          0x00000000
//#define atRINGMODE_RD_DROPREAD          0x00010000
#define atRINGMODE_RD_ALLOWPARTICLE     0x00020000
#define atRINGMODE_RD_NOINC             0x00040000


typedef enum {
     atRINGSEEK_START = 0x00,
     atRINGSEEK_CURRENT,
     atRINGSEEK_END,
     atRINGSEEK_WR_START = atRINGSEEK_START,
     atRINGSEEK_WR_CURRENT,
     atRINGSEEK_WR_END,
     atRINGSEEK_RD_START = atRINGSEEK_START + 0x10,
     atRINGSEEK_RD_CURRENT,
     atRINGSEEK_RD_END
} atRINGSEEKMODE;


typedef enum {
     atRINGDROP_START,
     atRINGDROP_END
} atRINGDROPPOS;


typedef struct tagatRINGBUF {
	  atRINGBUFFUNCS		 lstFuncs;
      atVOID *               pBuf;
      atLONG                 lBufSize;
      atLONG                 lCurRdPos;
      atLONG                 lCurWrPos;
      atLONG                 lStartPos;
      atLONG                 lFillSize;
      atINT                  lRingMode;
      atRINGBUF_DELETEFUNC * pDeleteFunc;
} atRINGBUF;


#ifdef atRINGBUF_USE_OWNMEMORY
atRINGBUF * atRINGBUF_Create( atRINGBUFFUNCS *lpFuncs, atLONG lBufSize );
#else
atBOOL  atRINGBUF_Create( atRINGBUFFUNCS *lpFuncs, atRINGBUF * This, atVOID * pBuf, atLONG lBufSize );
#endif
atBOOL      atRINGBUF_Destroy( atRINGBUF * This );
atVOID      atRINGBUF_Clear( atRINGBUF * This );
atVOID      atRINGBUF_SetCallbackFunc( atRINGBUF * This, atRINGBUF_DELETEFUNC *DeleteFunc );
atRINGMODE  atRINGBUF_SetMode( atRINGBUF * This, atRINGMODE lRingMode );
atRINGMODE  atRINGBUF_GetMode( atRINGBUF * This );
atLONG      atRINGBUF_GetFillSize( atRINGBUF * This );
atLONG      atRINGBUF_GetLeftSize( atRINGBUF * This );
atLONG      atRINGBUF_Drop( atRINGBUF * This, atLONG lDropSize, atRINGDROPPOS iDropPos );
atLONG      atRINGBUF_Seek( atRINGBUF * This, atLONG lPos, atRINGSEEKMODE iSeekMode );
atLONG      atRINGBUF_Read( atRINGBUF * This, void * pDst, atLONG lSize );
atLONG      atRINGBUF_Write( atRINGBUF * This, void * pSrc, atLONG lSize );
atVOID *   atRINGBUF_GetReaderblePtr( atRINGBUF * This, atLONG lSize, atBOOL bIsMove );



#ifdef __cplusplus
};
#endif


#endif /* __A_RNGBUF_H_ */
