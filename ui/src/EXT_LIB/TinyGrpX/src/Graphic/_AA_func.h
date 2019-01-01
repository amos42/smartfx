/*#############################################################################
-------------------------------------------------------------------------------

                                  Line.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Line function definition

-------------------------------------------------------------------------------
#############################################################################*/

#ifndef ___AA_FUNC_H_
#define ___AA_FUNC_H_


#include "GrpX.h"


#ifdef __cplusplus
extern "C" {
#endif


void __AA_DrawPixelAA_One( atGRPX_PIXEL *pBuf, atREAL rDX, atREAL rDY, int ColorR, int ColorG, int ColorB );
void __AA_DrawPixelAA( atGRPX_PIXEL *pBuf, atINT nVMemWidth, atINT nX, atINT nY, atREAL rDX, atREAL rDY, int ColorR, int ColorG, int ColorB );

void __AA_DrawPixelRealAA_One( atGRPX_PIXEL *pBuf, atREAL rX, atREAL rY, int ColorR, int ColorG, int ColorB );

void __AA_DrawPixelAA_H( atGRPX_PIXEL *pBuf, atREAL rDX, int ColorR, int ColorG, int ColorB, atINT x, atRECT *clip );
void __AA_DrawPixelAA_H_One( atGRPX_PIXEL *pBuf, atREAL rDX, int ColorR, int ColorG, int ColorB );
void __AA_DrawPixelAA_V( atGRPX_PIXEL *pBuf, atINT nVMemWidth, atREAL rDY, int ColorR, int ColorG, int ColorB, atINT y, atRECT *clip );
void __AA_DrawPixelAA_V_One( atGRPX_PIXEL *pBuf, atREAL rDY, int ColorR, int ColorG, int ColorB );

void __AA_DrawPixelRealAA( atGRPX_PIXEL *pBuf, atINT nVMemWidth, atREAL rX, atREAL rY, int ColorR, int ColorG, int ColorB );
void __AA_DrawPixelRealAA_H( atGRPX_PIXEL *pBuf, atREAL rX, int ColorR, int ColorG, int ColorB );
void __AA_DrawPixelRealAA_V( atGRPX_PIXEL *pBuf, atINT nVMemWidth, atREAL rY, int ColorR, int ColorG, int ColorB );




void __atGRPX_DrawPixelRealAA( atGRPX_CANVAS *lpCanvas, atREAL rX, atREAL rY, int ColorR, int ColorG, int ColorB );
void __atGRPX_DrawPixelRealAA_H( atGRPX_CANVAS *lpCanvas, atREAL rX, atINT nY, int ColorR, int ColorG, int ColorB );
void __atGRPX_DrawPixelRealAA_V( atGRPX_CANVAS *lpCanvas, atINT nX, atREAL rY, int ColorR, int ColorG, int ColorB );


void AACopyPixel_i_2( atGRPX_CANVAS *lpCanvas, int x, int y, atUINT dx, atUINT dy, int ColorR, int ColorG, int ColorB );
void AACopyPixelV_i_2( atGRPX_CANVAS *lpCanvas, int x, int y, atUINT dx, atUINT dy, int ColorR, int ColorG, int ColorB );
void AACopyPixelH_i_2( atGRPX_CANVAS *lpCanvas, int x, int y, atUINT dx, atUINT dy, int ColorR, int ColorG, int ColorB );
//void AAACopyPixel_i_2( atCANVAS *lpCanvas, int x, int y, UINT dx, UINT dy, int ColorR, int ColorG, int ColorB, int alpha )


#define __AA_ADJ_NONE 0
#define __AA_ADJ_LEFT 1
#define __AA_ADJ_RIGHT 2
#define __AA_ADJ_BOTH (__AA_ADJ_LEFT | __AA_ADJ_RIGHT)


void __AA_DrawHorizeLine( atGRPX_CANVAS *lpCanvas, atINT scan_y, atREAL lx, atREAL ly, atREAL rx, atREAL ry, atREAL ldxdy, atREAL rdxdy, atGRPX_PIXEL color, atREAL init_alpha, atDWORD dwAdjEdge );



#ifdef __cplusplus
};
#endif


#endif
