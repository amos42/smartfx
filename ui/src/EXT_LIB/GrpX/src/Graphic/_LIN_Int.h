/*###########################################################################
-----------------------------------------------------------------------------

                                LIN_Int.h
                         (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : 선 그리기 위한 매크로들의 모임.

-----------------------------------------------------------------------------
###########################################################################*/
#ifndef ___LIN_INT_H_
#define ___LIN_INT_H_


#include "AT_Types.h"


#define CS_CODE_NONE		(0x00)
// CS_CODE (CCW)
#define CS_CODE_MASK        (0x0F)
#define CS_CODE_TOP		    (0x01)
#define CS_CODE_LEFT		(0x02)
#define CS_CODE_BOTTOM	    (0x04)
#define CS_CODE_RIGHT	    (0x08)
// CS_CLIPPED_CODE
#define CS_CODE_CLIPPED_MASK    (0xF0)
#define CS_CODE_CLIPPED_TOP	    (0x10)
#define CS_CODE_CLIPPED_LEFT	(0x20)
#define CS_CODE_CLIPPED_BOTTOM	(0x40)
#define CS_CODE_CLIPPED_RIGHT	(0x80)

// input : StartX, StartY, EndX, EndY, clip_StartX, clip_StartY, clip_EndX, clip_EndY
// output : code1, code2
#define MAKE_CS_CODE( code, x, y, clip_StartX, clip_StartY, clip_EndX, clip_EndY ) \
	{ \
		code = CS_CODE_NONE;									\
		if( y < clip_StartY ) code |= CS_CODE_TOP;		\
		if( y > clip_EndY   ) code |= CS_CODE_BOTTOM;	\
		if( x < clip_StartX ) code |= CS_CODE_LEFT;		\
		if( x > clip_EndX   ) code |= CS_CODE_RIGHT;		\
	}

#define MAKE_CS_CODE_2( code1, code2, StartX, StartY, EndX, EndY, clip_StartX, clip_StartY, clip_EndX, clip_EndY ) \
	{ \
        MAKE_CS_CODE( code1, StartX, StartY, clip_StartX, clip_StartY, clip_EndX, clip_EndY ); \
        MAKE_CS_CODE( code2, EndX, EndY, clip_StartX, clip_StartY, clip_EndX, clip_EndY ); \
	}


#define GET_NEXT_CS_CODE(a, is_ccw)	( (is_ccw)? (((a)==CS_CODEMASK_RIGHT)?CS_CODEMASK_TOP:(a)<<1)\
							    	           :(((a)==CS_CODEMASK_TOP)?CS_CODEMASK_RIGHT:(a)>>1)


#endif
