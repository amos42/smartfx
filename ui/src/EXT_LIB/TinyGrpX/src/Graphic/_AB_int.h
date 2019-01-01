#ifndef ___AB_INT_H_
#define ___AB_INT_H_


#include "GrpX.h"


//                rr, gg, bb - 임시 변수
#define __CNV_ALPHA_PIXEL_R(cc, rr,gg,bb, weight) \
	{ \
	rr = GET_RED(cc); gg = GET_GREEN(cc); bb = GET_BLUE(cc); \
	rr = (atUCHAR)( REALTOINT( (rr) * (weight) ) ); \
	gg = (atUCHAR)( REALTOINT( (gg) * (weight) ) ); \
	bb = (atUCHAR)( REALTOINT( (bb) * (weight) ) ); \
	(cc) = MAKERGB(rr,gg,bb); \
	}

// __ALPHA_PIXEL( atGRPX_PIXEL *ptr, int r0, int g0, int b0, atREAL weight, atREAL weight2 )
//                cc, rr, gg, bb - 임시 변수
/*	
#define __ALPHA_PIXEL_R(ptr, r0,g0,b0, cc,rr,gg,bb, weight,weight2) \
	{ \
	cc = *(ptr); \
	rr = GET_RED(cc); gg = GET_GREEN(cc); bb = GET_BLUE(cc); \
	rr = ( REALTOINT( (r0) * (weight) + (rr) * (weight2) ) ); \
	if( rr > 255 ) rr = 255; \
	gg =( REALTOINT( (g0) * (weight) + (gg) * (weight2) ) ); \
	if( gg > 255 ) gg = 255; \
	bb =( REALTOINT( (b0) * (weight) + (bb) * (weight2) ) ); \
	if( bb > 255 ) bb = 255; \
	*(ptr) = MAKERGB(rr,gg,bb); \
	}
*/
#define __ALPHA_PIXEL_R(ptr, r0,g0,b0, cc,rr,gg,bb, weight,weight2) \
	{ \
	cc = *(ptr); \
	rr = GET_RED(cc); gg = GET_GREEN(cc); bb = GET_BLUE(cc); \
	rr = (atUCHAR)( REALTOINT( (r0) * (weight) + (rr) * (weight2) ) ); \
	gg = (atUCHAR)( REALTOINT( (g0) * (weight) + (gg) * (weight2) ) ); \
	bb = (atUCHAR)( REALTOINT( (b0) * (weight) + (bb) * (weight2) ) ); \
	*(ptr) = MAKERGB(rr,gg,bb); \
	}

// __ALPHA_PIXEL( atGRPX_PIXEL *ptr, int r0, int g0, int b0, atINT weight, atINT weight2 )
//                cc, rr, gg, bb - 임시 변수
/*
#define __ALPHA_PIXEL_I(ptr, r0,g0,b0, cc,rr,gg,bb, weight,weight2) \
	{ \
	cc = *(ptr); \
	rr = GET_RED(cc); gg = GET_GREEN(cc); bb = GET_BLUE(cc); \
	rr = (atUCHAR)( ( (r0) * (weight) + (rr) * (weight2) ) >> 8 ); \
	gg = (atUCHAR)( ( (g0) * (weight) + (gg) * (weight2) ) >> 8 ); \
	bb = (atUCHAR)( ( (b0) * (weight) + (bb) * (weight2) ) >> 8 ); \
	*(ptr) = MAKERGB(rr,gg,bb); \
	}
*/	
#define __ALPHA_PIXEL_I(ptr, r0,g0,b0, cc,rr,gg,bb, weight,weight2) \
	{ \
	cc = *(ptr); \
	rr = GET_RED(cc); gg = GET_GREEN(cc); bb = GET_BLUE(cc); \
	rr = (atUCHAR)( (((r0 - rr) * (weight))>>8) + (rr)); \
	gg = (atUCHAR)( (((g0 - gg) * (weight))>>8) + (gg)); \
	bb = (atUCHAR)( (((b0 - bb) * (weight))>>8) + (bb)); \
	*(ptr) = MAKERGB(rr,gg,bb); \
	}
// 알파값을 찍을 때 배경의 값이 아닌 입력되는 바탕색과 계산하여 찍는다.
#define __ALPHA_PIXEL_I_2(ptr, r0,g0,b0,rr,gg,bb, weight) \
	{ \
	rr = (atUCHAR)( (((r0 - rr) * (weight))>>8) + (rr)); \
	gg = (atUCHAR)( (((g0 - gg) * (weight))>>8) + (gg)); \
	bb = (atUCHAR)( (((b0 - bb) * (weight))>>8) + (bb)); \
	*(ptr) = MAKERGB(rr,gg,bb); \
	}
/*
#define __ALPHA_PIXEL_I(ptr, r0,g0,b0, cc,rr,gg,bb, weight,weight2) \
	{ \
	cc = *(ptr); \
	rr = GET_RED(cc); gg = GET_GREEN(cc); bb = GET_BLUE(cc); \
	rr = (atUCHAR)( ( (r0) * (weight) + (rr) * (weight2) ) >> 8 ); \
	gg = (atUCHAR)( ( (g0) * (weight) + (gg) * (weight2) ) >> 8 ); \
	bb = (atUCHAR)( ( (b0) * (weight) + (bb) * (weight2) ) >> 8 ); \
	*(ptr) = MAKERGB(rr,gg,bb); \
	}
*/
// 50% 블랜딩 적용된 Pixel 찍는 루틴
#if atGRPX_PIXEL_BPP == 16
#define __ALPHA_PIXEL_50P_CNV(cl)	(((cl) & 0xF7DE) >> 1)
#define __ALPHA_PIXEL_50P(ptr, cl) \
	{ \
		*(atGRPX_PIXEL*)(ptr) = __ALPHA_PIXEL_50P_CNV(*(atGRPX_PIXEL*)(ptr)) + __ALPHA_PIXEL_50P_CNV((cl)); \
	}
#define __ALPHA_PIXEL_50P_2(ptr, cl2) \
	{ \
		*(atGRPX_PIXEL*)(ptr) = __ALPHA_PIXEL_50P_CNV(*(atGRPX_PIXEL*)(ptr)) + (cl2); \
	}
#define __ALPHA_PIXEL_50P_3(ptr, r, g, b) \
	{ \
		*(atGRPX_PIXEL*)(ptr) = __ALPHA_PIXEL_50P_CNV(*(atGRPX_PIXEL*)(ptr)) + __ALPHA_PIXEL_50P_CNV(MAKERGB((r),(g),(b))); \
	}
/*
#define __ALPHA_PIXEL_50P_4(ptr, cl) \
	{ \
		*(atGRPX_PIXEL*)(ptr) = ((*(atGRPX_PIXEL*)(ptr))&cl)+((((*(atGRPX_PIXEL*)(ptr))^cl)&0xf7de)>>1); \
	}
#define __ALPHA_PIXEL_25P(ptr, cl) \
	{ \
		*(atGRPX_PIXEL*)(ptr) = __ALPHA_PIXEL_50P_CNV(*(atGRPX_PIXEL*)(ptr)) + __ALPHA_PIXEL_50P_CNV((cl)); \
	}
#define __ALPHA_PIXEL_25P_2(ptr, cl2) \
	{ \
		*(atGRPX_PIXEL*)(ptr) = __ALPHA_PIXEL_50P_CNV(*(atGRPX_PIXEL*)(ptr)) + (cl2); \
	}
#define __ALPHA_PIXEL_25P_3(ptr, r, g, b) \
	{ \
		*(atGRPX_PIXEL*)(ptr) = __ALPHA_PIXEL_50P_CNV(*(atGRPX_PIXEL*)(ptr)) + __ALPHA_PIXEL_50P_CNV(MAKERGB((r),(g),(b))); \
	}
*/
#elif atGRPX_PIXEL_BPP == 24
#define __ALPHA_PIXEL_50P_CNV(cl)	(((cl) & 0xFE) >> 1)
#define __ALPHA_PIXEL_50P_CNV_2(cl)	(((cl) & 0xFEFEFE) >> 1)
#define __ALPHA_PIXEL_50P(ptr, cl) \
	{ \
		*(atBYTE)(ptr)   = __ALPHA_PIXEL_50P_CNV(*(atBYTE*)(ptr))     + __ALPHA_PIXEL_50P_CNV((cl) >> 16); \
		*(atBYTE)(ptr+1) = __ALPHA_PIXEL_50P_CNV(*((atBYTE*)(ptr)+1)) + __ALPHA_PIXEL_50P_CNV((cl) >> 8); \
		*(atBYTE)(ptr+2) = __ALPHA_PIXEL_50P_CNV(*((atBYTE*)(ptr)+2)) + __ALPHA_PIXEL_50P_CNV((cl)); \
	}
#define __ALPHA_PIXEL_50P_2(ptr, cl2) \
	{ \
		*(atBYTE)(ptr)   = __ALPHA_PIXEL_50P_CNV(*(atGRPX_PIXEL*)(ptr))     + ((cl2) >> 16); \
		*(atBYTE)(ptr+1) = __ALPHA_PIXEL_50P_CNV(*((atGRPX_PIXEL*)(ptr)+1)) + ((cl2) >> 8); \
		*(atBYTE)(ptr+2) = __ALPHA_PIXEL_50P_CNV(*((atGRPX_PIXEL*)(ptr)+2)) +  (cl2); \
	}
#define __ALPHA_PIXEL_50P_3(ptr, r, g, b) \
	{ \
		*(atBYTE)(ptr)   = __ALPHA_PIXEL_50P_CNV(*(atGRPX_PIXEL*)(ptr))     + (r); \
		*(atBYTE)(ptr+1) = __ALPHA_PIXEL_50P_CNV(*((atGRPX_PIXEL*)(ptr)+1)) + (g); \
		*(atBYTE)(ptr+2) = __ALPHA_PIXEL_50P_CNV(*((atGRPX_PIXEL*)(ptr)+2)) + (b); \
	}
#else



#endif


#endif
