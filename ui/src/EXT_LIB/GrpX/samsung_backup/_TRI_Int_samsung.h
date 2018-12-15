/*###########################################################################
-----------------------------------------------------------------------------

                                rTRI_Int.h
                         (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Dependent Modules :
 Description       : 삼각형 그리기 위한 매크로들의 모임.

-----------------------------------------------------------------------------
###########################################################################*/
#ifndef ___TRI_INT_SAMSUNG_H_
#define ___TRI_INT_SAMSUNG_H_


#include "AT_Types.h"


#ifdef USE_ZBUFFER
#undef USE_ZBUFFER
#endif


#ifndef MIN3
#define MIN3(a,b,c) (MIN(MIN(a,b),c))
#endif
#ifndef MAX3
#define MAX3(a,b,c) (MAX(MAX(a,b),c))
#endif



//    int TopIndex, MiddleIndex, BottomIndex;
//    POINT3D Top, Middle, Bottom;
/*
#define SORT_TRI_VERTEXS( Top,Middle,Bottom, X1,Y1,X2,Y2,X3,Y3 )  \
    { \
		int TopIndex,MiddleIndex,BottomIndex;				\
															\
		if( (Y1 <= Y2) && (Y1 <= Y3) ){						\
			Top.nY = Y1;									\
			Top.nX = X1;									\
			TopIndex = 0;									\
		} else if( (Y2 <= Y1) && (Y2 <= Y3) ){				\
			Top.nY = Y2;									\
			Top.nX = X2;									\
			TopIndex = 1;									\
		} else {											\
			Top.nY = Y3;									\
			Top.nX = X3;									\
			TopIndex = 2;									\
		}													\
															\
		if( (Y1 > Y2) && (Y1 > Y3) ){						\
			Bottom.nY = Y1;									\
			Bottom.nX = X1;									\
			BottomIndex = 0;								\
		} else if( (Y2 > Y1) && (Y2 > Y3) ){				\
			Bottom.nY = Y2;									\
			Bottom.nX = X2;									\
			BottomIndex = 1;								\
		} else {											\
			Bottom.nY = Y3;									\
			Bottom.nX = X3;									\
			BottomIndex = 2;								\
		}													\
															\
		MiddleIndex = 3 - (TopIndex + BottomIndex);			\
		if( MiddleIndex == 0 ){								\
			Middle.nY = Y1;									\
			Middle.nX = X1;									\
		} else if( MiddleIndex == 1 ){						\
			Middle.nY = Y2;									\
			Middle.nX = X2;									\
		} else {											\
			Middle.nY = Y3;									\
			Middle.nX = X3;									\
		}													\
    }
*/
#define SORT_TRI_VERTEXS( Top,Middle,Bottom, X1,Y1,X2,Y2,X3,Y3 )  \
    {                                   \
        int TopIndex;                   \
                                        \
        if( Y1 < Y2 ){                  \
            if( Y1 < Y3 ){              \
              Top.nX = X1;              \
              Top.nY = Y1;              \
              TopIndex = 0;             \
            } else {                    \
              Top.nX = X3;              \
              Top.nY = Y3;              \
              TopIndex = 2;             \
            }                           \
        } else {                        \
            if( Y2 < Y3 ){              \
              Top.nX = X2;              \
              Top.nY = Y2;              \
              TopIndex = 1;             \
            } else {                    \
              Top.nX = X3;              \
              Top.nY = Y3;              \
              TopIndex = 2;             \
            }                           \
        }                               \
        if( TopIndex == 0 ){            \
            if( Y2 < Y3 ){              \
              Middle.nX = X2;           \
              Middle.nY = Y2;           \
              Bottom.nX = X3;           \
              Bottom.nY = Y3;           \
            } else {                    \
              Middle.nX = X3;           \
              Middle.nY = Y3;           \
              Bottom.nX = X2;           \
              Bottom.nY = Y2;           \
            }                           \
        } else if( TopIndex == 1 ){     \
            if( Y1 < Y3 ){              \
              Middle.nX = X1;           \
              Middle.nY = Y1;           \
              Bottom.nX = X3;           \
              Bottom.nY = Y3;           \
            } else {                    \
              Middle.nX = X3;           \
              Middle.nY = Y3;           \
              Bottom.nX = X1;           \
              Bottom.nY = Y1;           \
            }                           \
        } else {                        \
            if( Y1 < Y2 ){              \
              Middle.nX = X1;           \
              Middle.nY = Y1;           \
              Bottom.nX = X2;           \
              Bottom.nY = Y2;           \
            } else {                    \
              Middle.nX = X2;           \
              Middle.nY = Y2;           \
              Bottom.nX = X1;           \
              Bottom.nY = Y1;           \
            }                           \
        }                               \
    }

#define SORT_TRI_VERTEXS_EDGE( Top,Middle,Bottom, X1,Y1,X2,Y2,X3,Y3,AA)\
    {                                   \
        int TopIndex;                   \
                                        \
        if( Y1 < Y2 ){                  \
            if( Y1 < Y3 ){              \
              Top.nX = X1;              \
              Top.nY = Y1;              \
              TopIndex = 0;             \
            } else {                    \
              Top.nX = X3;              \
              Top.nY = Y3;              \
              TopIndex = 2;             \
            }                           \
        } else {                        \
            if( Y2 < Y3 ){              \
              Top.nX = X2;              \
              Top.nY = Y2;              \
              TopIndex = 1;             \
            } else {                    \
              Top.nX = X3;              \
              Top.nY = Y3;              \
              TopIndex = 2;             \
            }                           \
        }                               \
        if( TopIndex == 0 ){            \
            if( Y2 < Y3 ){              \
              Middle.nX = X2;           \
              Middle.nY = Y2;           \
              Bottom.nX = X3;           \
              Bottom.nY = Y3;           \
			  AA = 2;					\
            } else {                    \
              Middle.nX = X3;           \
              Middle.nY = Y3;           \
              Bottom.nX = X2;           \
              Bottom.nY = Y2;           \
			  AA = 2;					\
            }                           \
        } else if( TopIndex == 1 ){     \
            if( Y1 < Y3 ){              \
              Middle.nX = X1;           \
              Middle.nY = Y1;           \
              Bottom.nX = X3;           \
              Bottom.nY = Y3;           \
			  AA = 1;					\
            } else {                    \
              Middle.nX = X3;           \
              Middle.nY = Y3;           \
              Bottom.nX = X1;           \
              Bottom.nY = Y1;           \
			  AA = 0;					\
            }                           \
        } else {                        \
            if( Y1 < Y2 ){              \
              Middle.nX = X1;           \
              Middle.nY = Y1;           \
              Bottom.nX = X2;           \
              Bottom.nY = Y2;           \
			  AA = 1;					\
            } else {                    \
              Middle.nX = X2;           \
              Middle.nY = Y2;           \
              Bottom.nX = X1;           \
              Bottom.nY = Y1;           \
			  AA = 0;					\
            }                           \
        }                               \
    }			
// 각각의 기울기를 구한다.
#ifdef USE_ZBUFFER

//    POINT3D Top, Middle, Bottom;
//    REAL dX_TpBt, dX_TpMd, dX_MdBt;
#define CALC_DELTA( Top,Middle,Bottom, dX_TpBt,dX_TpMd,dX_MdBt, dZ_TpBt,dZ_TpMd,dZ_MdBt )  \
    { \
		dX_TpBt = (Bottom.nY == Top.nY)?    0 : INTTOREAL(Bottom.nX - Top.nX)      / (Bottom.nY - Top.nY);      \
		dX_TpMd = (Middle.nY == Top.nY)?    0 : INTTOREAL(Middle.nX - Top.nX)      / (Middle.nY - Top.nY);      \
		dX_MdBt = (Bottom.nY == Middle.nY)? 0 : INTTOREAL(Bottom.nX - Middle.nX)   / (Bottom.nY - Middle.nY);   \
		dZ_TpBt = (Bottom.nY == Top.nY)?    0 : FIXEDTOREAL(Bottom.z - Top.z)    / (Bottom.nY - Top.nY);		\
		dZ_TpMd = (Middle.nY == Top.nY)?    0 : FIXEDTOREAL(Middle.z - Top.z)    / (Middle.nY - Top.nY);		\
		dZ_MdBt = (Bottom.nY == Middle.nY)? 0 : FIXEDTOREAL(Bottom.z - Middle.z) / (Bottom.nY - Middle.nY);		\
    }

#else

//    POINT2D Top, Middle, Bottom;
//    REAL dX_TpBt, dX_TpMd, dX_MdBt;
#define CALC_DELTA( Top,Middle,Bottom, dX_TpBt,dX_TpMd,dX_MdBt )  \
    { \
		int dx, dy;														\
		dx = Bottom.nX - Top.nX; if( dx >= 0 ) dx++; else dx--;			\
		dy = Bottom.nY - Top.nY; if( dy >= 0 ) dy++; else dy--;			\
		dX_TpBt = INTTOREAL(dx) / (dy);	\
		dx = Middle.nX - Top.nX; if( dx >= 0 ) dx++; else dx--;			\
		dy = Middle.nY - Top.nY; if( dy >= 0 ) dy++; else dy--;			\
		dX_TpMd = INTTOREAL(dx) / (dy);   \
		dx = Bottom.nX - Middle.nX; if( dx >= 0 ) dx++; else dx--;		\
		dy = Bottom.nY - Middle.nY; if( dy >= 0 ) dy++; else dy--;		\
		dX_MdBt = INTTOREAL(dx) / (dy);   \
    }

#endif



#endif
