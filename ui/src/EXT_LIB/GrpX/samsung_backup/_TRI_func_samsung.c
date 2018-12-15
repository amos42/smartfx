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

#include "GrpX.h"
#include "GrpX_samsung.h"
#include "_TRI_Int.h"
#include "_TRI_Int_samsung.h"
#include "_TRI_func.h"
#include "_TRI_func_samsung.h"
#include "AT_Real.h"
#include "math.h" //for sqrt
#include "_BRE_func_samsung.h"

#define MAX_EDGEBUFFER_SIZE	480

atLONG g_edgelist[MAX_EDGEBUFFER_SIZE][2] = {0};

#define SET_BRESENHAM_TRI_LINE_VAR( StartX, StartY, EndX, EndY, dx, dy, du, dv, u, v, drawu_s, drawv_s, uincr, vincr, inverse, isOdd, uend, d, incrS, incrD, twovdu, invD, invD2du, edgeflag) \
{																											\
	dx = EndX - StartX;																						\
	dy = EndY - StartY;																						\
																											\
	if(ABS(dx) > ABS(dy))																					\
	{																										\
		du = ABS(dx);																						\
		dv = ABS(dy);																						\
		u = EndX;																							\
		v = EndY;																							\
		drawu_s = StartX;																					\
		drawv_s = StartY;																					\
		uincr = 1;																							\
		vincr = 1;																							\
		if (dx < 0)																							\
		{																									\
			uincr = -uincr;																					\
		}																									\
		if (dy < 0)																							\
		{																									\
			vincr = -vincr;																					\
		}																									\
		inverse = atFALSE;																					\
    }																										\
    else																									\
    {																										\
		du = ABS(dy);																						\
		dv = ABS(dx);																						\
		u = EndY;																							\
		v = EndX;																							\
		drawu_s = StartY;																					\
		drawv_s = StartX;																					\
		uincr = 1;																							\
		vincr = 1;																							\
		if (dy < 0)																							\
		{																									\
			uincr = -uincr;																					\
		}																									\
		if (dx < 0)																							\
		{																									\
			vincr = -vincr;																					\
		}																									\
		inverse = atTRUE;																					\
    }																										\
																											\
	if(du % 2)																								\
	{																										\
		isOdd = atTRUE;																						\
	}																										\
	else																									\
	{																										\
		isOdd = atFALSE;																					\
	}																										\
																											\
	uend = u + du;																							\
    d = (2 * dv) - du;																						\
    incrS = 2 * dv;																							\
    incrD = 2 * (dv - du);																					\
    twovdu = 0;																								\
																											\
	invD = REALDIV(INTTOREAL(1),INTTOREAL(2 * IntSqrt((unsigned long)(du*du) + (unsigned long)(dv*dv))));	\
	invD2du = REALMUL(INTTOREAL(2),REALMUL(INTTOREAL(du),invD));											\
																											\
	if((uincr + vincr) == 0)																				\
	{																										\
		edgeflag = atTRUE;																					\
	}																										\
	else																									\
	{																										\
		edgeflag = atFALSE;																					\
	}																										\
}

#define SET_BRESENHAM_TRI_MIR_LINE_VAR( StartX, StartY, EndX, EndY, dx, dy, du, dv, u, v, drawu_s, drawv_s, drawu_e, drawv_e, uincr, vincr, inverse, isOdd, uend, d, incrS, incrD, twovdu, invD, invD2du, edgeflag) \
{																											\
	dx = EndX - StartX;																						\
	dy = EndY - StartY;																						\
																											\
	if(ABS(dx) > ABS(dy))																					\
	{																										\
		du = ABS(dx);																						\
		dv = ABS(dy);																						\
		u = EndX;																							\
		v = EndY;																							\
		drawu_s = StartX;																					\
		drawv_s = StartY;																					\
		drawu_e = EndX;																						\
		drawv_e = EndY;																						\
		uincr = 1;																							\
		vincr = 1;																							\
		if (dx < 0)																							\
		{																									\
			uincr = -uincr;																					\
		}																									\
		if (dy < 0)																							\
		{																									\
			vincr = -vincr;																					\
		}																									\
		inverse = atFALSE;																					\
	}																										\
	else																									\
    {																										\
		du = ABS(dy);																						\
		dv = ABS(dx);																						\
		u = EndY;																							\
		v = EndX;																							\
		drawu_s = StartY;																					\
		drawv_s = StartX;																					\
		drawu_e = EndY;																						\
		drawv_e = EndX;																						\
		uincr = 1;																							\
		vincr = 1;																							\
		if (dy < 0)																							\
		{																									\
			uincr = -uincr;																					\
		}																									\
		if (dx < 0)																							\
		{																									\
			vincr = -vincr;																					\
		}																									\
		inverse = atTRUE;																					\
    }																										\
																											\
	if(du % 2)																								\
	{																										\
		isOdd = atTRUE;																						\
	}																										\
	else																									\
	{																										\
		isOdd = atFALSE;																					\
	}																										\
																											\
	uend = u + (du>>1) +1;																							\
    d = (2 * dv) - du;																						\
    incrS = 2 * dv;																							\
    incrD = 2 * (dv - du);																					\
    twovdu = 0;																								\
																											\
	invD = REALDIV(INTTOREAL(1),INTTOREAL(2 * IntSqrt((unsigned long)(du*du) + (unsigned long)(dv*dv))));	\
	invD2du = REALMUL(INTTOREAL(2),REALMUL(INTTOREAL(du),invD));											\
																											\
	if((uincr + vincr) == 0)																				\
	{																										\
		edgeflag = atTRUE;																					\
	}																										\
	else																									\
	{																										\
		edgeflag = atFALSE;																					\
	}																										\
}



#define CAL_NODRAW_LINE_MID(y, draw_x, draw_y, dir, invD, twovdu, invD2du, diff_distance, alpha_var_1, alpha_var_2, inverse, lpCanvas, drawv_s, drawu_s, vincr, uincr, clip, Color, d, du, incrS, incrD, v, u, uend)\
{																											\
	if(inverse)																								\
	{																										\
				draw_x = drawv_s;																			\
				draw_y = drawu_s;																			\
	}																										\
	else																									\
	{																										\
				draw_x = drawu_s;																			\
				draw_y = drawv_s;																			\
	}																										\
	do																										\
	{																										\
		if (d < 0)																							\
		{																									\
			/* choose straight (u direction) */																\
			twovdu = d + du;																				\
			d = d + incrS;																					\
		}																									\
		else																								\
		{																									\
			/* choose diagonal (u+v direction) */															\
			twovdu = d - du;																				\
			d = d + incrD;																					\
			v = v+1;																						\
			drawv_s = drawv_s + vincr;																		\
		}																									\
		u = u+1;																							\
		drawu_s = drawu_s + uincr;																			\
																											\
		if(inverse)																							\
		{																									\
			if(dir)																							\
			{																								\
				draw_y = drawu_s;																			\
				if(drawv_s <= draw_x && draw_y <= y)															\
				{																							\
					draw_x = drawv_s;																		\
				}																							\
			}																								\
			else																							\
			{																								\
				draw_y = drawu_s;																			\
				if(drawv_s >= draw_x && draw_y <= y)															\
				{																							\
					draw_x = drawv_s;																		\
				}																							\
			}																								\
		}																									\
		else																								\
		{																									\
			if(dir)																							\
			{																								\
				draw_y = drawv_s;																			\
				if(drawu_s <= draw_x && draw_y <= y)															\
				{																							\
					draw_x = drawu_s;																		\
				}																							\
			}																								\
			else																							\
			{																								\
				draw_y = drawv_s;																			\
				if(drawu_s >= draw_x && draw_y <= y)															\
				{																							\
					draw_x = drawu_s;																		\
				}																							\
			}																								\
		}																									\
	} while (draw_y <= y && u < uend);																		\
}

// edgelist는 y축에 따른 x좌표를 저장할 배열.
// dir은 좌측 우측라인 구분 인자.
#define CAL_NODRAW_MIR_LINE_MID(edgelist, y_s, y_e, dir, invD, twovdu, invD2du, diff_distance, alpha_var_1, alpha_var_2, inverse, lpCanvas, drawv_s, drawu_s, drawv_e, drawu_e, isOdd, vincr, uincr, clip, Color, d, du, incrS, incrD, v, u, uend)\
{																											\
	if(inverse)																								\
	{																										\
				edgelist[drawu_s][dir] = drawv_s;															\
				edgelist[drawu_e][dir] = drawv_e;															\
				y_s = drawu_s;																				\
				y_e = drawu_e;																				\
	}																										\
	else																									\
	{																										\
				edgelist[drawv_s][dir] = drawu_s;															\
				edgelist[drawv_e][dir] = drawu_e;															\
				y_s = drawv_s;																				\
				y_e = drawv_e;																				\
	}																										\
	do																										\
	{																										\
		if (d < 0)																							\
		{																									\
			/* choose straight (u direction) */																\
			twovdu = d + du;																				\
			d = d + incrS;																					\
		}																									\
		else																								\
		{																									\
			/* choose diagonal (u+v direction) */															\
			twovdu = d - du;																				\
			d = d + incrD;																					\
			v = v+1;																						\
			drawv_s = drawv_s + vincr;																		\
			drawv_e = drawv_e - vincr;																		\
		}																									\
		u = u+1;																							\
		drawu_s = drawu_s + uincr;																			\
		drawu_e = drawu_e - uincr;																			\
																											\
																											\
		if(inverse)																							\
		{																									\
			if(drawu_e < drawu_s)																				\
			continue;																							\
			if(dir)																							\
			{																								\
				if(drawu_s != y_s)																			\
				{																							\
					if(drawu_e == drawu_s)																	\
					{																						\
						if(drawv_s > drawv_e)																\
						{																					\
							edgelist[drawu_s][dir] = drawv_s;												\
						}																					\
						else																				\
						{																					\
							edgelist[drawu_s][dir] = drawv_e;												\
						}																					\
					}																						\
					else																					\
					{																						\
						edgelist[drawu_s][dir] = drawv_s;													\
						edgelist[drawu_e][dir] = drawv_e;													\
					}																						\
				}																							\
				else																						\
				{																							\
					if(edgelist[drawu_s][dir] > drawv_s)													\
					{																						\
						edgelist[drawu_s][dir] = drawv_s;													\
					}																						\
					if(edgelist[drawu_e][dir] > drawv_e)													\
					{																						\
						edgelist[drawu_e][dir] = drawv_e;													\
					}																						\
				}																							\
																											\
				y_s = drawu_s;																				\
				y_e = drawu_e;																				\
			}																								\
			else																							\
			{																								\
				if(drawu_s != y_s)																			\
				{																							\
					if(drawu_e == drawu_s)																	\
					{																						\
						if(drawv_s < drawv_e)																\
						{																					\
							edgelist[drawu_s][dir] = drawv_s;												\
						}																					\
						else																				\
						{																					\
							edgelist[drawu_s][dir] = drawv_e;												\
						}																					\
					}																						\
					else																					\
					{																						\
						edgelist[drawu_s][dir] = drawv_s;													\
						edgelist[drawu_e][dir] = drawv_e;													\
					}																						\
				}																							\
				else																						\
				{																							\
					if(edgelist[drawu_s][dir] < drawv_s)													\
					{																						\
						edgelist[drawu_s][dir] = drawv_s;													\
					}																						\
					if(edgelist[drawu_e][dir] < drawv_e)													\
					{																						\
						edgelist[drawu_e][dir] = drawv_e;													\
					}																						\
				}																							\
																											\
				y_s = drawu_s;																				\
				y_e = drawu_e;																				\
			}																								\
		}																									\
		else																								\
		{																									\
			if(drawv_e < drawv_s)																				\
			continue;																							\
			if(dir)																							\
			{																								\
				if(drawv_s != y_s)																			\
				{																							\
					if(drawv_e == drawv_s)																	\
					{																						\
						if(drawu_s > drawu_e)																\
						{																					\
							edgelist[drawv_s][dir] = drawu_s;												\
						}																					\
						else																				\
						{																					\
							edgelist[drawv_s][dir] = drawu_e;												\
						}																					\
					}																						\
					else																					\
					{																						\
						edgelist[drawv_s][dir] = drawu_s;													\
						edgelist[drawv_e][dir] = drawu_e;													\
					}																						\
				}																							\
				else																						\
				{																							\
					if(edgelist[drawv_s][dir] > drawu_s)													\
					{																						\
						edgelist[drawv_s][dir] = drawu_s;													\
					}																						\
					if(edgelist[drawv_e][dir] > drawu_e)								\
					{																						\
						edgelist[drawv_e][dir] = drawu_e;													\
					}																						\
				}																							\
																											\
				y_s = drawv_s;																				\
				y_e = drawv_e;																				\
			}																								\
			else																							\
			{																								\
				if(drawv_s != y_s)																			\
				{																							\
					if(drawv_e == drawv_s)																	\
					{																						\
						if(drawu_s < drawu_e)																\
						{																					\
							edgelist[drawv_s][dir] = drawu_s;												\
						}																					\
						else																				\
						{																					\
							edgelist[drawv_s][dir] = drawu_e;												\
						}																					\
					}																						\
					else																					\
					{																						\
						edgelist[drawv_s][dir] = drawu_s;													\
						edgelist[drawv_e][dir] = drawu_e;													\
					}																						\
				}																							\
				else																						\
				{																							\
					if(edgelist[drawv_s][dir] < drawu_s)								\
					{																						\
						edgelist[drawv_s][dir] = drawu_s;													\
					}																						\
					if(edgelist[drawv_e][dir] < drawu_e)								\
					{																						\
						edgelist[drawv_e][dir] = drawu_e;													\
					}																						\
				}																							\
																											\
				y_s = drawv_s;																				\
				y_e = drawv_e;																				\
			}																								\
		}																									\
	} while (u < uend);																					\
}


#define CAL_LINE_MID(y, draw_x, draw_y, dir, invD, twovdu, invD2du, diff_distance, alpha_var_1, alpha_var_2, inverse, lpCanvas, drawv_s, drawu_s, vincr, uincr, clip, Color, d, du, incrS, incrD, v, u, uend)\
{																											\
	if(inverse)																								\
	{																										\
				draw_x = drawv_s;																			\
				draw_y = drawu_s;																			\
	}																										\
	else																									\
	{																										\
				draw_x = drawu_s;																			\
				draw_y = drawv_s;																			\
	}																										\
	do																										\
	{																										\
		if(inverse)																							\
		{																									\
			atGRPX_DrawPixel(lpCanvas, drawv_s, drawu_s, Color);											\
		}																									\
		else																								\
		{																									\
			atGRPX_DrawPixel(lpCanvas, drawu_s, drawv_s, Color);											\
		}																									\
																											\
		if (d < 0)																							\
		{																									\
		/* choose straight (u direction) */																	\
		twovdu = d + du;																					\
		d = d + incrS;																						\
		}																									\
		else																								\
		{																									\
		/* choose diagonal (u+v direction) */																\
		twovdu = d - du;																					\
		d = d + incrD;																						\
		v = v+1;																							\
		drawv_s = drawv_s + vincr;																			\
		}																									\
		u = u+1;																							\
		drawu_s = drawu_s + uincr;																			\
																											\
		if(inverse)																							\
		{																									\
		if(dir)																								\
			{																								\
			draw_y = drawu_s;																				\
			if(drawv_s <= draw_x && draw_y <= y)																\
				{																							\
				draw_x = drawv_s;																			\
				}																							\
			}																								\
			else																							\
			{																								\
			draw_y = drawu_s;																				\
			if(drawv_s >= draw_x && draw_y <= y)																\
				{																							\
				draw_x = drawv_s;																			\
				}																							\
			}																								\
		}																									\
		else																								\
		{																									\
		if(dir)																								\
			{																								\
			draw_y = drawv_s;																				\
			if(drawu_s <= draw_x && draw_y <= y)																\
				{																							\
				draw_x = drawu_s;																			\
				}																							\
			}																								\
			else																							\
			{																								\
			draw_y = drawv_s;																				\
			if(drawu_s >= draw_x && draw_y <= y)																\
				{																							\
				draw_x = drawu_s;																			\
				}																							\
			}																								\
		}																									\
	} while (draw_y <= y && u < uend);																		\
}

// edgelist는 y축에 따른 x좌표를 저장할 배열.
// dir은 좌측 우측라인 구분 인자.
#define CAL_MIR_LINE_MID(edgelist, y_s, y_e, dir, invD, twovdu, invD2du, diff_distance, alpha_var_1, alpha_var_2, inverse, lpCanvas, drawv_s, drawu_s, drawv_e, drawu_e, isOdd, vincr, uincr, clip, Color, d, du, incrS, incrD, v, u, uend)\
{																											\
	if(inverse)																								\
	{																										\
				edgelist[drawu_s][dir] = drawv_s;															\
				edgelist[drawu_e][dir] = drawv_e;															\
				y_s = drawu_s;																				\
				y_e = drawu_e;																				\
	}																										\
	else																									\
	{																										\
				edgelist[drawv_s][dir] = drawu_s;															\
				edgelist[drawv_e][dir] = drawu_e;															\
				y_s = drawv_s;																				\
				y_e = drawv_e;																				\
	}																										\
	do																										\
	{																										\
		if(inverse)																							\
		{																									\
			atGRPX_DrawPixel(lpCanvas, drawv_s, drawu_s, Color);											\
			atGRPX_DrawPixel(lpCanvas, drawv_e, drawu_e, Color);											\
		}																									\
		else																								\
		{																									\
			atGRPX_DrawPixel(lpCanvas, drawu_s, drawv_s, Color);											\
			atGRPX_DrawPixel(lpCanvas, drawu_e, drawv_e, Color);											\
		}																									\
		if (d < 0)																							\
		{																									\
			/* choose straight (u direction) */																\
			twovdu = d + du;																				\
			d = d + incrS;																					\
		}																									\
		else																								\
		{																									\
			/* choose diagonal (u+v direction) */															\
			twovdu = d - du;																				\
			d = d + incrD;																					\
			v = v+1;																						\
			drawv_s = drawv_s + vincr;																		\
			drawv_e = drawv_e - vincr;																		\
		}																									\
		u = u+1;																							\
		drawu_s = drawu_s + uincr;																			\
		drawu_e = drawu_e - uincr;																			\
																											\
		if(inverse)																							\
		{																									\
			if(dir)																							\
			{																								\
				if(drawu_s != y_s)																			\
				{																							\
					edgelist[drawu_s][dir] = drawv_s;														\
					edgelist[drawu_e][dir] = drawv_e;														\
				}																							\
				else																						\
				{																							\
					if(edgelist[drawu_s][dir] > drawv_s)													\
					{																						\
						edgelist[drawu_s][dir] = drawv_s;													\
					}																						\
					if(edgelist[drawu_e][dir] > drawv_e)													\
					{																						\
						edgelist[drawu_e][dir] = drawv_e;													\
					}																						\
				}																							\
																											\
				y_s = drawu_s;																				\
				y_e = drawu_e;																				\
			}																								\
			else																							\
			{																								\
				if(drawu_s != y_s)																			\
				{																							\
					edgelist[drawu_s][dir] = drawv_s;														\
					edgelist[drawu_e][dir] = drawv_e;														\
				}																							\
				else																						\
				{																							\
					if(edgelist[drawu_s][dir] < drawv_s)													\
					{																						\
						edgelist[drawu_s][dir] = drawv_s;													\
					}																						\
					if(edgelist[drawu_e][dir] < drawv_e)													\
					{																						\
						edgelist[drawu_e][dir] = drawv_e;													\
					}																						\
				}																							\
																											\
				y_s = drawu_s;																				\
				y_e = drawu_e;																				\
			}																								\
		}																									\
		else																								\
		{																									\
			if(dir)																							\
			{																								\
				if(drawv_s != y_s)																			\
				{																							\
					edgelist[drawv_s][dir] = drawu_s;														\
					edgelist[drawv_e][dir] = drawu_e;														\
				}																							\
				else																						\
				{																							\
					if(edgelist[drawv_s][dir] > drawu_s)													\
					{																						\
						edgelist[drawv_s][dir] = drawu_s;													\
					}																						\
					if(edgelist[drawv_e][dir] > drawu_e)													\
					{																						\
						edgelist[drawv_e][dir] = drawu_e;													\
					}																						\
				}																							\
																											\
				y_s = drawv_s;																				\
				y_e = drawv_e;																				\
			}																								\
			else																							\
			{																								\
				if(drawv_s != y_s)																			\
				{																							\
					edgelist[drawv_s][dir] = drawu_s;														\
					edgelist[drawv_e][dir] = drawu_e;														\
				}																							\
				else																						\
				{																							\
					if(edgelist[drawv_s][dir] < drawu_s)													\
					{																						\
						edgelist[drawv_s][dir] = drawu_s;													\
					}																						\
					if(edgelist[drawv_e][dir] < drawu_e)													\
					{																						\
						edgelist[drawv_e][dir] = drawu_e;													\
					}																						\
				}																							\
																											\
				y_s = drawv_s;																				\
				y_e = drawv_e;																				\
			}																								\
		}																									\
	} while (u < uend);																						\
																											\
	if(isOdd)																								\
	{																										\
		if(inverse)																							\
		{																									\
			atGRPX_DrawPixel(lpCanvas, drawv_s, drawu_s, Color);											\
		}																									\
		else																								\
		{																									\
			atGRPX_DrawPixel(lpCanvas, drawu_s, drawv_s, Color);											\
		}																									\
																											\
	}																										\
	else																									\
	{																										\
		if(inverse)																							\
		{																									\
			atGRPX_DrawPixel(lpCanvas, drawv_s, drawu_s, Color);											\
			atGRPX_DrawPixel(lpCanvas, drawv_e, drawu_e, Color);											\
		}																									\
		else																								\
		{																									\
			atGRPX_DrawPixel(lpCanvas, drawu_s, drawv_s, Color);											\
			atGRPX_DrawPixel(lpCanvas, drawu_e, drawv_e, Color);											\
		}																									\
	}																										\
}

#define CAL_LINE_AA(y, draw_x, draw_y, dir, edgeflag, invD, twovdu, invD2du, diff_distance, alpha_var_1, alpha_var_2, inverse, lpCanvas, drawv_s, drawu_s, vincr, uincr, clip, Color, d, du, incrS, incrD, v, u, uend)\
{																											\
	if(inverse)																								\
	{																										\
				draw_x = drawv_s;																			\
				draw_y = drawu_s;																			\
	}																										\
	else																									\
	{																										\
				draw_x = drawu_s;																			\
				draw_y = drawv_s;																			\
	}																										\
	do																										\
	{																										\
		diff_distance = REALMUL(invD, INTTOREAL(twovdu));													\
		alpha_var_1 = REALTOINT(REALMUL(invD2du, INTTOREAL(100)));											\
		alpha_var_2 = REALTOINT(REALMUL(diff_distance,INTTOREAL(100)));										\
																											\
		if(inverse)																							\
		{																									\
			if(edgeflag)																					\
			{																								\
				if(dir)																						\
				{																							\
					atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s - vincr, drawu_s, Color, alpha_var_1 + alpha_var_2, clip);\
				}																							\
				else																						\
				{																							\
					atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s + vincr, drawu_s, Color, alpha_var_1 - alpha_var_2, clip);	\
				}																							\
			}																								\
			else																							\
			{																								\
				if(dir)																						\
				{																							\
					atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s + vincr, drawu_s, Color, alpha_var_1 - alpha_var_2, clip);\
				}																							\
				else																						\
				{																							\
					atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s - vincr, drawu_s, Color, alpha_var_1 + alpha_var_2, clip);\
				}																							\
			}																								\
		}																									\
		else																								\
		{																									\
			if(edgeflag)																					\
			{																								\
				if(dir)																						\
				{																							\
					atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s + vincr, drawv_s, Color, alpha_var_1 - alpha_var_2, clip);\
				}																							\
				else																						\
				{																							\
					atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s - vincr, drawv_s, Color, alpha_var_1 + alpha_var_2, clip);\
				}																							\
			}																								\
			else																							\
			{																								\
				if(dir)																						\
				{																							\
					atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s + vincr, drawv_s, Color, alpha_var_1 + alpha_var_2, clip);\
				}																							\
				else																						\
				{																							\
					atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s - vincr, drawv_s, Color, alpha_var_1 - alpha_var_2, clip);\
				}																							\
			}																								\
		}																									\
																											\
		if (d < 0)																							\
		{																									\
		/* choose straight (u direction) */																	\
		twovdu = d + du;																					\
		d = d + incrS;																						\
		}																									\
		else																								\
		{																									\
		/* choose diagonal (u+v direction) */																\
		twovdu = d - du;																					\
		d = d + incrD;																						\
		v = v+1;																							\
		drawv_s = drawv_s + vincr;																			\
		}																									\
		u = u+1;																							\
		drawu_s = drawu_s + uincr;																			\
																											\
		if(inverse)																							\
		{																									\
			if(dir)																							\
			{																								\
				draw_y = drawu_s;																			\
				if(drawv_s < draw_x && draw_y <= y)															\
				{																							\
					draw_x = drawv_s;																		\
				}																							\
			}																								\
			else																							\
			{																								\
				draw_y = drawu_s;																			\
				if(drawv_s > draw_x && draw_y <= y)															\
				{																							\
					draw_x = drawv_s;																		\
				}																							\
			}																								\
		}																									\
		else																								\
		{																									\
		if(dir)																								\
			{																								\
				draw_y = drawv_s;																			\
				if(drawu_s < draw_x && draw_y <= y)															\
				{																							\
					draw_x = drawu_s;																		\
				}																							\
			}																								\
			else																							\
			{																								\
				draw_y = drawv_s;																			\
				if(drawu_s > draw_x && draw_y <= y)															\
				{																							\
					draw_x = drawu_s;																		\
				}																							\
			}																								\
		}																									\
	} while (draw_y <= y && u < uend);																		\
}







atVOID cal_MIR_LINE_MID(atLONG edgelist[MAX_EDGEBUFFER_SIZE][2], atINT y_s, atINT y_e, atSHORT dir, atLONG invD, atLONG twovdu, atLONG invD2du, atREAL diff_distance, atLONG alpha_var_1, atLONG alpha_var_2, atBOOL inverse, atGRPX_CANVAS* lpCanvas, atLONG drawv_s, atLONG drawu_s, atLONG drawv_e, atLONG drawu_e, atBOOL isOdd, atLONG vincr, atLONG uincr, atRECT* clip, atGRPX_COLOR Color, atLONG d, atLONG du, atLONG incrS, atLONG incrD, atLONG v, atLONG u, atLONG uend)
{
	do																									
	{
		// error check code 추가. 알고리즘 수정해야 함. fix-me
//		if (drawu_s < 0 || drawu_e < 0)	return;
//		if (drawu_s >= MAX_EDGEBUFFER_SIZE || drawu_e>= MAX_EDGEBUFFER_SIZE) return;

		if(inverse)																						
		{																								
			atGRPX_DrawPixel(lpCanvas, drawv_s, drawu_s, Color);										
			atGRPX_DrawPixel(lpCanvas, drawv_e, drawu_e, Color);	
			
			if(dir)																						
			{

				if(drawu_s != y_s)																		
				{								
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
						edgelist[drawu_s][dir] = drawv_s;
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
						edgelist[drawu_e][dir] = drawv_e;												
				}																					
				else																				
				{																					
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
					{	
						if(edgelist[drawu_s][dir] > drawv_s)
							edgelist[drawu_s][dir] = drawv_s;
					}																				
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
					{			
						if(edgelist[drawu_e][dir] > drawv_e)
							edgelist[drawu_e][dir] = drawv_e;											
					}																				
				}																					
				
				y_s = drawu_s;																		
				y_e = drawu_e;																		
			}																						
			else																					
			{																						
				if(drawu_s != y_s)																	
				{				
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
						edgelist[drawu_s][dir] = drawv_s;					
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
						edgelist[drawu_e][dir] = drawv_e;												
				}																					
				else																				
				{																					
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
					
					{									
						if(edgelist[drawu_s][dir] < drawv_s)
							edgelist[drawu_s][dir] = drawv_s;
					}																				
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
					{
						if(edgelist[drawu_e][dir] < drawv_e)
							edgelist[drawu_e][dir] = drawv_e;
					}																				
				}																					
				
				y_s = drawu_s;																		
				y_e = drawu_e;																		
			}		
		}																								
		else																							
		{																								
			atGRPX_DrawPixel(lpCanvas, drawu_s, drawv_s, Color);										
			atGRPX_DrawPixel(lpCanvas, drawu_e, drawv_e, Color);
		
			if(dir)																					
			{																						
				if(drawv_s != y_s)																	
				{	
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )															
						edgelist[drawv_s][dir] = drawu_s;												
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )															
						edgelist[drawv_e][dir] = drawu_e;												
				}																					
				else																				
				{																					
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )
					{									
						if(edgelist[drawv_s][dir] > drawu_s)
							edgelist[drawv_s][dir] = drawu_s;
					}																				
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )
					{		
						if(edgelist[drawv_e][dir] > drawu_e)
							edgelist[drawv_e][dir] = drawu_e;
					}																				
				}																					
				
				y_s = drawv_s;																		
				y_e = drawv_e;																		
			}																						
			else																					
			{																						
				if(drawv_s != y_s)																	
				{
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )															
						edgelist[drawv_s][dir] = drawu_s;
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )															
						edgelist[drawv_e][dir] = drawu_e;												
				}																					
				else																				
				{																					
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )
					{
						if(edgelist[drawv_s][dir] < drawu_s)											
							edgelist[drawv_s][dir] = drawu_s;											
					}																				
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )															
					{	
						if(edgelist[drawv_e][dir] < drawu_e)											
							edgelist[drawv_e][dir] = drawu_e;											
					}																				
				}																					
				y_s = drawv_s;																		
				y_e = drawv_e;																		
			}					
		}																								
		if (d < 0)																						
		{																								
			twovdu = d + du;																			
			d = d + incrS;																				
		}																								
		else																							
		{																								
			twovdu = d - du;																			
			d = d + incrD;																				
			v = v+1;																					
			drawv_s = drawv_s + vincr;																	
			drawv_e = drawv_e - vincr;																	
		}																								
		u = u+1;																						
		drawu_s = drawu_s + uincr;																		
		drawu_e = drawu_e - uincr;																			
	} while (u < uend);																				
	if(drawu_s == drawu_e)
	{
		if(inverse)																						
		{		
			atGRPX_DrawPixel(lpCanvas, drawv_s, drawu_s, Color);									
			atGRPX_DrawPixel(lpCanvas, drawv_e, drawu_e, Color);
			if(dir)																						
			{	
				if(drawu_s != y_s)																		
				{								
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
						edgelist[drawu_s][dir] = drawv_s;
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
						edgelist[drawu_e][dir] = drawv_e;												
				}																					
				else		
				{
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
					{
						if(edgelist[drawu_s][dir] > drawv_s)											
							edgelist[drawu_s][dir] = drawv_s;											
					}
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
					{
						if(edgelist[drawu_e][dir] > drawv_e)											
							edgelist[drawu_e][dir] = drawv_e;											
					}																				
				}
			}																						
			else																					
			{																						
				if(drawu_s != y_s)																	
				{				
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
						edgelist[drawu_s][dir] = drawv_s;					
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
						edgelist[drawu_e][dir] = drawv_e;												
				}																					
				else																				
				{																					
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
					{									
						if(edgelist[drawu_s][dir] < drawv_s)
							edgelist[drawu_s][dir] = drawv_s;											
					}	
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )											
					{
						if(edgelist[drawu_e][dir] < drawv_e)
							edgelist[drawu_e][dir] = drawv_e;											
					}																				
				}
			}
		}																							
		else
		//v가 y, u가 x
		{
			atGRPX_DrawPixel(lpCanvas, drawu_s, drawv_s, Color);									
			atGRPX_DrawPixel(lpCanvas, drawu_e, drawv_e, Color);
			if(dir)																					
			{
				if(drawv_s != y_s)																	
				{	
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )															
						edgelist[drawv_s][dir] = drawu_s;												
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )															
						edgelist[drawv_e][dir] = drawu_e;												
				}		
				else
				{
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )
					{
						if(edgelist[drawv_s][dir] > drawu_s)											
							edgelist[drawv_s][dir] = drawu_s;											
					}
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )
					{		
						if(edgelist[drawv_e][dir] > drawu_e)
							edgelist[drawv_e][dir] = drawu_e;											
					}																				
				}
			}																						
			else																					
			{	
				if(drawv_s != y_s)																	
				{
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )															
						edgelist[drawv_s][dir] = drawu_s;
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )															
						edgelist[drawv_e][dir] = drawu_e;												
				}	
				else
				{
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )											
					{
						if(edgelist[drawv_s][dir] < drawu_s)
							edgelist[drawv_s][dir] = drawu_s;											
					}																				
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )
					{
						if(edgelist[drawv_e][dir] < drawu_e)											
							edgelist[drawv_e][dir] = drawu_e;											
					}																				
				}
			}			
		}
	}
	
}


atVOID cal_NODRAW_MIR_LINE_MID(atLONG edgelist[MAX_EDGEBUFFER_SIZE][2], atINT y_s, atINT y_e, atSHORT dir, atLONG invD, atLONG twovdu, atLONG invD2du, atREAL diff_distance, atLONG alpha_var_1, atLONG alpha_var_2, atBOOL inverse, atGRPX_CANVAS* lpCanvas, atLONG drawv_s, atLONG drawu_s, atLONG drawv_e, atLONG drawu_e, atBOOL isOdd, atLONG vincr, atLONG uincr, atRECT* clip, atGRPX_COLOR Color, atLONG d, atLONG du, atLONG incrS, atLONG incrD, atLONG v, atLONG u, atLONG uend)
{
	do																									
	{																									
		// error check code 추가. 알고리즘 수정해야 함. fix-me
//		if (drawu_s < 0 || drawu_e < 0) return;
//		if (drawu_s >= MAX_EDGEBUFFER_SIZE || drawu_e>=MAX_EDGEBUFFER_SIZE) return;


		if(inverse)																						
		{																								
			if(dir)																						
			{																							
				if(drawu_s != y_s)																		
				{								
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
					{
#if (_IS_PC_ && defined(_DEBUG))
//						if (drawu_s < 0 || drawu_e < 0) 
//						{
//							assert(0);
//						}
//						if (drawu_s >= MAX_EDGEBUFFER_SIZE || drawu_e>=MAX_EDGEBUFFER_SIZE) 
//						{
//							assert(0);
//						}
#endif

						edgelist[drawu_s][dir] = drawv_s;
					}
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
						edgelist[drawu_e][dir] = drawv_e;												
				}																					
				else																				
				{																					
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
					{	
						if(edgelist[drawu_s][dir] > drawv_s)
							edgelist[drawu_s][dir] = drawv_s;											
					}																				
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
					{			
						if(edgelist[drawu_e][dir] > drawv_e)
							edgelist[drawu_e][dir] = drawv_e;											
					}																				
				}																					
																									
				y_s = drawu_s;																		
				y_e = drawu_e;																		
			}																						
			else																					
			{																						
				if(drawu_s != y_s)																	
				{				
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
						edgelist[drawu_s][dir] = drawv_s;					
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
						edgelist[drawu_e][dir] = drawv_e;												
				}																					
				else																				
				{																					
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
					{									
						if(edgelist[drawu_s][dir] < drawv_s)
							edgelist[drawu_s][dir] = drawv_s;											
					}																				
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
					{
						if(edgelist[drawu_e][dir] < drawv_e)
							edgelist[drawu_e][dir] = drawv_e;
					}																				
				}																					
																									
				y_s = drawu_s;																		
				y_e = drawu_e;																		
			}																						
		}																							
		else																						
		{																							
			if(dir)																					
			{																						
				if(drawv_s != y_s)																	
				{	
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )															
						edgelist[drawv_s][dir] = drawu_s;
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )															
						edgelist[drawv_e][dir] = drawu_e;
				}																					
				else																				
				{																					
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )
					{									
						if(edgelist[drawv_s][dir] > drawu_s)
							edgelist[drawv_s][dir] = drawu_s;
					}																				
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )
					{		
						if(edgelist[drawv_e][dir] > drawu_e)
							edgelist[drawv_e][dir] = drawu_e;
					}																				
				}																					
																									
				y_s = drawv_s;																		
				y_e = drawv_e;																		
			}																						
			else																					
			{																						
				if(drawv_s != y_s)																	
				{
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )															
						edgelist[drawv_s][dir] = drawu_s;
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )															
						edgelist[drawv_e][dir] = drawu_e;												
				}																					
				else																				
				{																					
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )
					{
						if(edgelist[drawv_s][dir] < drawu_s)
							edgelist[drawv_s][dir] = drawu_s;
					}
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )
					{	
						if(edgelist[drawv_e][dir] < drawu_e)
							edgelist[drawv_e][dir] = drawu_e;
					}																				
				}																					
																									
				y_s = drawv_s;																		
				y_e = drawv_e;																		
			}																						
		}

		if (d < 0)																						
		{																																								
			twovdu = d + du;																			
			d = d + incrS;																				
		}																								
		else																							
		{																																				
			twovdu = d - du;																			
			d = d + incrD;																				
			v = v+1;																					
			drawv_s = drawv_s + vincr;																	
			drawv_e = drawv_e - vincr;																	
		}																								
		u = u+1;																						
		drawu_s = drawu_s + uincr;																		
		drawu_e = drawu_e - uincr;																		
	} while (u < uend);																				

	// 한줄에 겹치는 경우
	if(drawu_s == drawu_e)
	{
		if(inverse)																						
		{			
			if(dir)																						
			{	
				if(drawu_s != y_s)																		
				{								
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
						edgelist[drawu_s][dir] = drawv_s;
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
						edgelist[drawu_e][dir] = drawv_e;												
				}																					
				else		
				{
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
					{	
						if(edgelist[drawu_s][dir] > drawv_s)
							edgelist[drawu_s][dir] = drawv_s;											
					}																				
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
					{			
						if(edgelist[drawu_e][dir] > drawv_e)											
							edgelist[drawu_e][dir] = drawv_e;											
					}																				
				}
			}																						
			else																					
			{																						
				if(drawu_s != y_s)																	
				{				
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
						edgelist[drawu_s][dir] = drawv_s;					
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
						edgelist[drawu_e][dir] = drawv_e;												
				}																					
				else																				
				{																					
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
					{									
						if(edgelist[drawu_s][dir] < drawv_s)
							edgelist[drawu_s][dir] = drawv_s;											
					}																				
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
					{
						if(edgelist[drawu_e][dir] < drawv_e)
							edgelist[drawu_e][dir] = drawv_e;
					}																				
				}
			}
		}																							
		else
		//v가 y, u가 x
		{
			if(dir)																					
			{
				if(drawv_s != y_s)																	
				{	
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )															
						edgelist[drawv_s][dir] = drawu_s;												
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )															
						edgelist[drawv_e][dir] = drawu_e;												
				}		
				else
				{
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )
					{		
						if(edgelist[drawv_s][dir] > drawu_s)
							edgelist[drawv_s][dir] = drawu_s;
					}																				
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )
					{		
						if(edgelist[drawv_e][dir] > drawu_e)
							edgelist[drawv_e][dir] = drawu_e;
					}																				
				}
			}																						
			else																					
			{	
				if(drawv_s != y_s)																	
				{
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )															
						edgelist[drawv_s][dir] = drawu_s;
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )															
						edgelist[drawv_e][dir] = drawu_e;												
				}	
				else
				{
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )
					{
						if(edgelist[drawv_s][dir] < drawu_s)
							edgelist[drawv_s][dir] = drawu_s;
					}																				
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )
					{	
						if(edgelist[drawv_e][dir] < drawu_e)
							edgelist[drawv_e][dir] = drawu_e;
					}																				
				}
			}			
		}
	}																
}

atVOID cal_MIR_LINE_AA(atLONG edgelist[MAX_EDGEBUFFER_SIZE][2], atINT y_s, atINT y_e, atSHORT dir, atLONG invD, atLONG twovdu, atLONG invD2du, atREAL diff_distance, atLONG alpha_var_1, atLONG alpha_var_2, atBOOL inverse, atGRPX_CANVAS* lpCanvas, atLONG drawv_s, atLONG drawu_s, atLONG drawv_e, atLONG drawu_e, atBOOL isOdd, atLONG vincr, atLONG uincr, atRECT* clip, atGRPX_COLOR Color, atLONG d, atLONG du, atLONG incrS, atLONG incrD, atLONG v, atLONG u, atLONG uend)
{
	do																									
	{
		// error check code 추가. 알고리즘 수정해야 함. fix-me
//		if (drawu_s < 0 || drawu_e < 0) return;
//		if (drawu_s >= MAX_EDGEBUFFER_SIZE || drawu_e>= MAX_EDGEBUFFER_SIZE) return;

		diff_distance = REALMUL(invD, INTTOREAL(twovdu));													
		alpha_var_1 = REALTOINT(REALMUL(invD2du, INTTOREAL(100)));										
		alpha_var_2 = REALTOINT(REALMUL(diff_distance,INTTOREAL(100)));										
		
		if(inverse)																						
		{	
			atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s , drawu_s, Color, alpha_var_2, clip);
			atGRPX_DrawPixelBrePixel(lpCanvas, drawv_e , drawu_e, Color, alpha_var_2, clip);
			
			if(dir)																						
			{																							
				if(drawu_s != y_s)																		
				{								
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
						edgelist[drawu_s][dir] = drawv_s;
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
						edgelist[drawu_e][dir] = drawv_e;												
				}																					
				else																				
				{																					
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
					{	
						if(edgelist[drawu_s][dir] > drawv_s)
							edgelist[drawu_s][dir] = drawv_s;
					}																				
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
					{			
						if(edgelist[drawu_e][dir] > drawv_e)
							edgelist[drawu_e][dir] = drawv_e;
					}																				
				}																					
				
				y_s = drawu_s;																		
				y_e = drawu_e;																		
			}																						
			else																					
			{																						
				if(drawu_s != y_s)																	
				{				
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
						edgelist[drawu_s][dir] = drawv_s;					
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
						edgelist[drawu_e][dir] = drawv_e;												
				}																					
				else																				
				{																					
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
					{									
						if(edgelist[drawu_s][dir] < drawv_s)
							edgelist[drawu_s][dir] = drawv_s;
					}																				
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
					{
						if(edgelist[drawu_e][dir] < drawv_e)
							edgelist[drawu_e][dir] = drawv_e;
					}																				
				}																					
				
				y_s = drawu_s;																		
				y_e = drawu_e;																		
			}		
		}																								
		else																							
		{		
			atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s , drawv_s, Color, alpha_var_2, clip);
			atGRPX_DrawPixelBrePixel(lpCanvas, drawu_e , drawv_e, Color, alpha_var_2, clip);
			
			if(dir)																					
			{																						
				if(drawv_s != y_s)																	
				{	
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )															
						edgelist[drawv_s][dir] = drawu_s;												
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )															
						edgelist[drawv_e][dir] = drawu_e;												
				}																					
				else																				
				{																					
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )
					{									
						if(edgelist[drawv_s][dir] > drawu_s)
							edgelist[drawv_s][dir] = drawu_s;
					}																				
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )
					{		
						if(edgelist[drawv_e][dir] > drawu_e)
							edgelist[drawv_e][dir] = drawu_e;
					}																				
				}																					
				
				y_s = drawv_s;																		
				y_e = drawv_e;																		
			}																						
			else																					
			{																						
				if(drawv_s != y_s)																	
				{
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )															
						edgelist[drawv_s][dir] = drawu_s;
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )															
						edgelist[drawv_e][dir] = drawu_e;												
				}																					
				else																				
				{																					
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )
					{
						if(edgelist[drawv_s][dir] < drawu_s)
							edgelist[drawv_s][dir] = drawu_s;
					}																				
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )
					{	
						if(edgelist[drawv_e][dir] < drawu_e)
							edgelist[drawv_e][dir] = drawu_e;
					}																				
				}																					
				y_s = drawv_s;																		
				y_e = drawv_e;																		
			}					
		}																								
		if (d < 0)																						
		{																								
			twovdu = d + du;																			
			d = d + incrS;																				
		}																								
		else																							
		{																								
			twovdu = d - du;																			
			d = d + incrD;																				
			v = v+1;																					
			drawv_s = drawv_s + vincr;																	
			drawv_e = drawv_e - vincr;																	
		}																								
		u = u+1;																						
		drawu_s = drawu_s + uincr;																		
		drawu_e = drawu_e - uincr;																			
	} while (u < uend);																				
	if(drawu_s == drawu_e)
	{
		if(inverse)																						
		{		
			atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s , drawu_s, Color, alpha_var_2, clip);
			atGRPX_DrawPixelBrePixel(lpCanvas, drawv_e , drawu_e, Color, alpha_var_2, clip);
			if(dir)																						
			{	
				if(drawu_s != y_s)																		
				{								
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
						edgelist[drawu_s][dir] = drawv_s;
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
						edgelist[drawu_e][dir] = drawv_e;												
				}																					
				else		
				{
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
					{	
						if(edgelist[drawu_s][dir] > drawv_s)
							edgelist[drawu_s][dir] = drawv_s;
					}																				
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
					{			
						if(edgelist[drawu_e][dir] > drawv_e)
							edgelist[drawu_e][dir] = drawv_e;
					}																				
				}
			}																						
			else																					
			{																						
				if(drawu_s != y_s)																	
				{				
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
						edgelist[drawu_s][dir] = drawv_s;					
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
						edgelist[drawu_e][dir] = drawv_e;												
				}																					
				else																				
				{																					
					if(drawu_s >= clip->nStartY && drawu_s <= clip->nEndY )
					{									
						if(edgelist[drawu_s][dir] < drawv_s)
							edgelist[drawu_s][dir] = drawv_s;
					}																				
					if(drawu_e >= clip->nStartY && drawu_e <= clip->nEndY )
					{
						if(edgelist[drawu_e][dir] < drawv_e)
							edgelist[drawu_e][dir] = drawv_e;
					}																				
				}
			}
		}																							
		else
		//v가 y, u가 x
		{
			atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s , drawv_s, Color, alpha_var_2, clip);
			atGRPX_DrawPixelBrePixel(lpCanvas, drawu_e , drawv_e, Color, alpha_var_2, clip);

			if(dir)																					
			{
				if(drawv_s != y_s)																	
				{	
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )															
						edgelist[drawv_s][dir] = drawu_s;												
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )															
						edgelist[drawv_e][dir] = drawu_e;												
				}		
				else
				{
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )
					{									
						if(edgelist[drawv_s][dir] > drawu_s)
							edgelist[drawv_s][dir] = drawu_s;
					}																				
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )
					{		
						if(edgelist[drawv_e][dir] > drawu_e)
							edgelist[drawv_e][dir] = drawu_e;
					}																				
				}
			}																						
			else																					
			{	
				if(drawv_s != y_s)																	
				{
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )															
						edgelist[drawv_s][dir] = drawu_s;
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )															
						edgelist[drawv_e][dir] = drawu_e;												
				}	
				else
				{
					if(drawv_s >= clip->nStartY && drawv_s <= clip->nEndY )
					{
						if(edgelist[drawv_s][dir] < drawu_s)
							edgelist[drawv_s][dir] = drawu_s;
					}																				
					if(drawv_e >= clip->nStartY && drawv_e <= clip->nEndY )
					{	
						if(edgelist[drawv_e][dir] < drawu_e)
							edgelist[drawv_e][dir] = drawu_e;
					}																				
				}
			}			
		}
	}
	
}






static void _DrawHorizLine( atGRPX_CANVAS *lpCanvas, atREAL x1, atREAL x2, int y, atREAL dx1, atREAL dx2, 
							atLONG ExParam1,
							__FTRICALLBACK__DRAW_HLINE *fnDrawHLine )
{
	atRECT *clip;
    atREAL temp;
	atREAL cx1, cx2;

//    if( x1 == x2 ) return;

    if( x1 > x2 ){
      SWAP( x1, x2, temp );
      SWAP( dx1, dx2, temp );
    }

	clip = atGRPX_GetClipRect(lpCanvas);
    cx1 = INTTOREAL(clip->nStartX);
	cx2 = INTTOREAL(clip->nEndX);

    // 좌우 클리핑
	if( (x2 < cx1) || (x1 > cx2) ) return;
    if( x1 < cx1 ){ x1 = cx1; dx1 = INTTOREAL(0); }
    if( x2 > cx2 ){ x2 = cx2; dx2 = INTTOREAL(0); }

	fnDrawHLine( lpCanvas, x1, x2, y, dx1, dx2, ExParam1 );
}


// lpCanvas, sy, ey, lx, rx, ldx, ldy, rdx, rdy, clip, ExParam1, fnDrawHLine - 입력 파라미터
// ldxdy, rdxdy, i, t, t2 - 임시 변수
#define SPF_FILL_LINE( lpCanvas, sy, ey, lx, rx, ldx, ldy, rdx, rdy, clip, ExParam1, fnDrawHLine, ldxdy, rdxdy, i, t, t2 ) \
        ldxdy = REALDIV( ldx, ldy ); \
        rdxdy = REALDIV( rdx, rdy ); \
        if( sy <= clip->nEndY ){ \
          if( ey < clip->nStartY ){ \
            t = ey - sy + 1; \
		    lx += ldxdy * t; rx += rdxdy * t; \
          } else { \
            if( ey > clip->nEndY ){    \
              ey = clip->nEndY;           \
            }                             \
            if( sy < clip->nStartY ){  \
              t = t2 = MIN( clip->nStartY, ey ); \
              t2 -= sy;                 \
              sy = t;                     \
		      lx += ldxdy * t2; rx += rdxdy * t2; \
            }                                   \
		    for( i = sy; i <= ey; i++ ){       \
		      _DrawHorizLine( lpCanvas, lx, rx, i, ldxdy, rdxdy, ExParam1, fnDrawHLine ); \
		      lx += ldxdy; rx += rdxdy;        \
            }   \
          } \
        }

/*******************************
  속이 채워진 삼각형을 그린다.
********************************/
void __atGRPX_FillTriangle0_samsung( atGRPX_CANVAS *lpCanvas, atINT X1, atINT Y1, atINT X2, atINT Y2, atINT X3, atINT Y3, 
						atLONG ExParam1,
						__FTRICALLBACK__DRAW_HLINE *fnDrawHLine )
{
#ifdef _USE_BRESENHAM

	atLONG y;
    atPOINT Top, Middle, Bottom;
    atLONG X_TpBt, X_TpMdBt;
    atLONG mxx, mix;
	atLONG aa_edge_index = 0;

	///Bresenham 변수
	
	// 공통 변수
	atGRPX_COLOR Color = (atGRPX_COLOR)ExParam1;
	atRECT* clip;
	atLONG draw_x;
	atLONG draw_y;
	atREAL gradientTpBt;
	atREAL gradientTpMd;
	atLONG TpBtEdge = 0; // Top-Bottom edge가 삼각형의 우즉에 있는지 좌측에 있는지 정하는 변수.
	atLONG TpMdBtEdge = 1;
	
			
	// 각 Line별 변수 Top-Middle : 0, Top-Bottom : 1, Middle-Bottom : 2
	atLONG dx[3];
    atLONG dy[3];
	atLONG drawu_s[3];
	atLONG drawv_s[3];
	
	atLONG du[3];
	atLONG dv[3];
	atLONG u[3];
	atLONG v[3];
	
	atLONG uincr[3];			//Screen 좌표
	atLONG vincr[3];
	
	atLONG uend[3];
    atLONG d[3];				
    atLONG incrS[3];			
    atLONG incrD[3];			
    atLONG twovdu[3];			
	
    atREAL invD[3];			
    atREAL invD2du[3];		
	
	atBOOL inverse[3];
	
	atREAL diff_distance[3];
	
	atBOOL isOdd[3];
	atBOOL edgeflag[3];
		
	atLONG alpha_var_1[3]; // 알파값 매개 변수
	atLONG alpha_var_2[3];


	clip = atGRPX_GetClipRect( lpCanvas );
	
    // 클리핑 영역을 완전히 벗어나면 종료
	mix = MIN3(X1, X2, X3);
    if( mix > clip->nEndX ) return;
	mxx = MAX3(X1, X2, X3);
    if( mxx < clip->nStartX ) return;
	
    // Sorting Triangle Vertex
    SORT_TRI_VERTEXS( Top,Middle,Bottom, X1,Y1,X2,Y2,X3,Y3);

    // 클리핑 영역을 완전히 벗어나면 종료
    if( Top.nY > clip->nEndY ) return;
    if( Bottom.nY < clip->nStartY ) return;
	
    // 한줄짜리
    if( Top.nY == Bottom.nY ){
		atGRPX_DrawHorizLine( lpCanvas, INTTOREAL(mix), INTTOREAL(mxx), Top.nY, Color);
		return;
    }

	// 두점이 같은 경우
	if( Top.nX == Middle.nX && Top.nY == Middle.nY)
	{
		atGRPX_DrawLineAA(lpCanvas, Top.nX, Top.nY, Bottom.nX, Bottom.nY, Color);
		return;
	}

	if( Bottom.nX == Middle.nX && Bottom.nY == Middle.nY)
	{
		atGRPX_DrawLineAA(lpCanvas, Top.nX, Top.nY, Middle.nX, Middle.nY, Color);
		return;
	}


	//====== 각 선분의 각도를 측정하여 좌우를 판단(0이면 왼쪽 1이면 오른쪽) =============
	dx[2] = Bottom.nX-Top.nX;
	dy[2] = Bottom.nY-Top.nY;
	if(dy[2] != 0)
	{
		gradientTpBt = INTTOREAL(dx[2])/(dy[2]);
	}
	else
	{
		if(dx[2] > 0)
		{
			gradientTpBt = INTTOREAL(1000);
		}
		else
		{
			gradientTpBt = INTTOREAL(-1000);
		}
		
	}
	
	dx[0] = Middle.nX-Top.nX;
	dy[0] = Middle.nY-Top.nY;
	if(dy[0] != 0)
	{
		gradientTpMd = INTTOREAL(dx[0])/(dy[0]);
	}
	else
	{
		if(dx[0] > 0)
		{
			gradientTpMd = INTTOREAL(1000);
		}
		else
		{
			gradientTpMd = INTTOREAL(-1000);
		}
	}
	
	
	if(gradientTpMd > gradientTpBt)
	{
		TpBtEdge = 0;
		TpMdBtEdge = 1;
	}
	else if(gradientTpMd < gradientTpBt)
	{
		TpBtEdge = 1;
		TpMdBtEdge = 0;
	}
	else //Top과 Middle의 Y값이 같은 경우.
	{
		if(Top.nX > Middle.nX)
		{
			TpBtEdge = 1;
			TpMdBtEdge = 0;
		}
		else
		{
			TpBtEdge = 0;
			TpMdBtEdge = 1;
		}
	}

    //====== Bresenham 변수 세팅 ===========
	
	// Top - Middle
	SET_BRESENHAM_TRI_LINE_VAR( Top.nX, Top.nY, Middle.nX, Middle.nY, dx[0], dy[0], du[0], dv[0], u[0], v[0], drawu_s[0], drawv_s[0], uincr[0], vincr[0], inverse[0], isOdd[0], uend[0], d[0], incrS[0], incrD[0], twovdu[0], invD[0], invD2du[0], edgeflag[0]);
	// Top - Bottom
	SET_BRESENHAM_TRI_LINE_VAR( Top.nX, Top.nY, Bottom.nX, Bottom.nY, dx[1], dy[1], du[1], dv[1], u[1], v[1], drawu_s[1], drawv_s[1], uincr[1], vincr[1], inverse[1], isOdd[1], uend[1], d[1], incrS[1], incrD[1], twovdu[1], invD[1], invD2du[1], edgeflag[1]);
	// Middle - Bottom
	SET_BRESENHAM_TRI_LINE_VAR( Middle.nX, Middle.nY, Bottom.nX, Bottom.nY, dx[2], dy[2], du[2], dv[2], u[2], v[2], drawu_s[2], drawv_s[2], uincr[2], vincr[2], inverse[2], isOdd[2], uend[2], d[2], incrS[2], incrD[2], twovdu[2], invD[2], invD2du[2], edgeflag[2]);
    
    	
    // 처음부터 중간까지 그린다.
    if( (Top.nY <= Middle.nY) && (Middle.nY >= clip->nStartY) )
	{
        y = Top.nY;
        
        if( Middle.nY > clip->nEndY )
		{
			Middle.nY = clip->nEndY;
        }
		
		
		while( y < Middle.nY )
		{
			CAL_LINE_AA(y, draw_x, draw_y, TpMdBtEdge,  edgeflag[0], invD[0], twovdu[0], invD2du[0], diff_distance[0], alpha_var_1[0], alpha_var_2[0], inverse[0], lpCanvas, drawv_s[0], drawu_s[0], vincr[0], uincr[0], clip, Color, d[0], du[0], incrS[0], incrD[0], v[0], u[0], uend[0]);
			X_TpMdBt = draw_x;
			
			CAL_LINE_AA(y, draw_x, draw_y, TpBtEdge,  edgeflag[1], invD[1], twovdu[1], invD2du[1], diff_distance[1], alpha_var_1[1], alpha_var_2[1], inverse[1], lpCanvas, drawv_s[1], drawu_s[1], vincr[1], uincr[1], clip, Color, d[1], du[1], incrS[1], incrD[1], v[1], u[1], uend[1]);
			X_TpBt = draw_x;

			if(X_TpBt >= X_TpMdBt)
			{
				atGRPX_DrawHorizLine2(lpCanvas, X_TpMdBt, y, X_TpBt - X_TpMdBt + 1,Color, clip);
			}
			else
			{
				atGRPX_DrawHorizLine2(lpCanvas, X_TpBt, y, X_TpMdBt - X_TpBt + 1,Color, clip);
			}
		    y ++;
		}
    }
	
    if( Middle.nY >= clip->nEndY ) return;
	
    // 중간부터 끝까지 그린다.
    if( Middle.nY <= Bottom.nY )
	{
        y = Middle.nY;
		
		if( Bottom.nY > clip->nEndY )
		{
			Bottom.nY = clip->nEndY;
        }
		
        while( y <= Bottom.nY )
		{
			CAL_LINE_AA(y, draw_x, draw_y, TpMdBtEdge, edgeflag[2], invD[2], twovdu[2], invD2du[2], diff_distance[2], alpha_var_1[2], alpha_var_2[2], inverse[2], lpCanvas, drawv_s[2], drawu_s[2], vincr[2], uincr[2], clip, Color, d[2], du[2], incrS[2], incrD[2], v[2], u[2], uend[2]);
			X_TpMdBt = draw_x;
		
			CAL_LINE_AA(y, draw_x, draw_y, TpBtEdge,  edgeflag[1], invD[1], twovdu[1], invD2du[1], diff_distance[1], alpha_var_1[1], alpha_var_2[1], inverse[1], lpCanvas, drawv_s[1], drawu_s[1], vincr[1], uincr[1], clip, Color, d[1], du[1], incrS[1], incrD[1], v[1], u[1], uend[1]);
			X_TpBt = draw_x;
			
			if(X_TpBt >= X_TpMdBt)
			{
				atGRPX_DrawHorizLine2(lpCanvas, X_TpMdBt, y, X_TpBt - X_TpMdBt + 1, Color, clip);
			}
			else
			{
				atGRPX_DrawHorizLine2(lpCanvas, X_TpBt, y, X_TpMdBt - X_TpBt + 1, Color, clip);
			}			
			y ++;

        }

    }
#else
    int y, height;
    atPOINT Top, Middle, Bottom;
    atREAL X_TpBt, X_TpMdBt;
    atREAL dX_TpBt, dX_TpMd, dX_MdBt;
	atRECT *clip;
	int mxx, mix;

	clip = atGRPX_GetClipRect( lpCanvas );

    // 클리핑 영역을 완전히 벗어나면 종료
	mix = MIN3(X1, X2, X3);
    if( mix > clip->nEndX ) return;
	mxx = MAX3(X1, X2, X3);
    if( mxx < clip->nStartX ) return;

    // Sorting Triangle Vertex
    SORT_TRI_VERTEXS( Top,Middle,Bottom, X1,Y1,X2,Y2,X3,Y3 );

    // 클리핑 영역을 완전히 벗어나면 종료
    if( Top.nY > clip->nEndY ) return;
    if( Bottom.nY < clip->nStartY ) return;

    // 한줄짜리
    if( Top.nY == Bottom.nY ){
	  _DrawHorizLine( lpCanvas, INTTOREAL(mix), INTTOREAL(mxx), Top.nY, INTTOREAL(0), INTTOREAL(0), ExParam1, fnDrawHLine );
      return;
    }

    // 각각의 기울기를 구한다.
    CALC_DELTA( Top,Middle,Bottom, dX_TpBt,dX_TpMd,dX_MdBt ); 

    //======== 이제부터 본격적으로 그린다. ===========
    
    X_TpBt = X_TpMdBt = INTTOREAL(Top.nX);
//    y = Top.nY;

    // 처음부터 중간까지 그린다.
    if( (Top.nY < Middle.nY) && (Middle.nY >= clip->nStartY) ){
        y = Top.nY;
        
        if( Top.nY < clip->nStartY ){
          height = (clip->nStartY - Top.nY);
          X_TpBt   += (dX_TpBt * height);
          X_TpMdBt += (dX_TpMd * height);
          y = clip->nStartY;
        }
        if( Middle.nY > clip->nEndY ){
          Middle.nY = clip->nEndY;
        }

//        if( Middle.nY >= clip->nStartY ){
          while( y < Middle.nY ){
//            _DrawHorizLine( lpCanvas, REALTOINT(X_TpBt), REALTOINT(X_TpMdBt), y, ExParam1, fnDrawHLine );
            _DrawHorizLine( lpCanvas, X_TpBt, X_TpMdBt, y, dX_TpBt, dX_TpMd, ExParam1, fnDrawHLine );

            y ++;
            X_TpBt += dX_TpBt;
            X_TpMdBt += dX_TpMd;
          }
//        }
    }

    if( Middle.nY >= clip->nEndY ) return;

	// 만약 중간 지점이 클리핑 영역 바깥이면...
      if( Middle.nY < clip->nStartY ){ 
          height = (Middle.nY - Top.nY + 1);
          X_TpBt   += (dX_TpBt * height);
          X_TpMdBt += (dX_TpMd * height);
      }

//    X_TpBt = INTTOREAL(Top.nX) + (dX_TpBt * (Middle.nY - Top.nY + 1)); // 이미 앞서 계산이 진행되어 왔으므로...
    X_TpMdBt = INTTOREAL(Middle.nX);

    // 중간부터 끝까지 그린다.
    if( Middle.nY < Bottom.nY ){
        y = Middle.nY;
    
        if( Middle.nY < clip->nStartY ){
          height = (clip->nStartY - Middle.nY + 1);
          X_TpBt += (dX_TpBt * height);
          X_TpMdBt += (dX_MdBt * height);        
    
          y = clip->nStartY;
        }
        if( Bottom.nY > clip->nEndY ){
          Bottom.nY = clip->nEndY;
        }
    
        while( y <= Bottom.nY ){
//            _DrawHorizLine( lpCanvas, REALTOINT(X_TpBt), REALTOINT(X_TpMdBt), y, ExParam1, fnDrawHLine );
            _DrawHorizLine( lpCanvas, X_TpBt, X_TpMdBt, y, dX_TpBt, dX_MdBt, ExParam1, fnDrawHLine );
    
            X_TpBt += dX_TpBt;
            X_TpMdBt += dX_MdBt;

            y++;
        }
    }
#endif
}

atVOID __atGRPX_FillRoundCapTriangle0( atGRPX_CANVAS *lpCanvas, atLONG X1, atLONG Y1, atLONG X2, atLONG Y2, atLONG X3, atLONG Y3, atLONG ExParam1, __FTRICALLBACK__DRAW_HLINE *fnDrawHLine)
{
#ifdef _USE_BRESENHAM
	
	int y;
    atPOINT Top, Middle, Bottom;
    atLONG X_TpBt, X_TpMdBt;
    atLONG mxx, mix, mxy, miy;
	atINT aa_edge_index = 0;
	atGRPX_COLOR Color = (atGRPX_COLOR)ExParam1;
	
	///Bresenham 변수
	
	// 공통 변수
	atRECT* clip;
	atINT draw_x;
	atINT draw_y;
	atREAL gradientTpBt;
	atREAL gradientTpMd;
	atSHORT TpBtEdge = 0; // Top-Bottom edge가 삼각형의 우즉에 있는지 좌측에 있는지 정하는 변수.
	atSHORT TpMdBtEdge = 1;
	atINT y_up, y_down;
	
	
	// 각 Line별 변수 Top-Middle : 0, Top-Bottom : 1, Middle-Bottom : 2
	atLONG dx[3];
    atLONG dy[3];
	atLONG drawu_s[3];
	atLONG drawv_s[3];
	atLONG drawu_e[3];
	atLONG drawv_e[3];
	
	atLONG du[3];
	atLONG dv[3];
	atLONG u[3];
	atLONG v[3];
	
	atLONG uincr[3];			//Screen 좌표
	atLONG vincr[3];
	
	atLONG uend[3];
    atLONG d[3];				
    atLONG incrS[3];			
    atLONG incrD[3];			
    atLONG twovdu[3];			
	
    atLONG invD[3];			
    atLONG invD2du[3];		
	
	atBOOL inverse[3];
	
	atREAL diff_distance[3];
	
	atBOOL isOdd[3];
	atBOOL edgeflag[3]; // 좌우 픽셀 구분시 필요.
	
	atLONG alpha_var_1[3]; // 알파값 매개 변수
	atLONG alpha_var_2[3];

	clip = atGRPX_GetClipRect( lpCanvas );
	
    // 클리핑 영역을 완전히 벗어나면 종료
	mix = MIN3(X1, X2, X3);
    if( mix > clip->nEndX ) return;
	mxx = MAX3(X1, X2, X3);
    if( mxx < clip->nStartX ) return;
	miy = MIN3(Y1, Y2, Y3);
    if( miy > clip->nEndY ) return;
	mxy = MAX3(Y1, Y2, Y3);
    if( mxy < clip->nStartY ) return;
	
    // Sorting Triangle Vertex
    SORT_TRI_VERTEXS_EDGE( Top,Middle,Bottom, X1,Y1,X2,Y2,X3,Y3, aa_edge_index);

    // 클리핑 영역을 완전히 벗어나면 종료
//    if( Top.nY > clip->nEndY ) return;
//    if( Bottom.nY < clip->nStartY ) return;
	
	if(Top.nX == Bottom.nX == Middle.nX && Top.nY == Bottom.nY == Middle.nY)
	{
		atGRPX_DrawPixel(lpCanvas, Top.nX, Top.nY, Color);
	}
    // 한줄짜리
    if( Top.nY == Bottom.nY ){
		atGRPX_DrawHorizLine( lpCanvas, INTTOREAL(mix), INTTOREAL(mxx), Top.nY, Color);
		return;
    }

	// 두점이 같은 경우
	if( Top.nX == Middle.nX && Top.nY == Middle.nY)
	{
		atGRPX_DrawLineAA(lpCanvas, Top.nX, Top.nY, Bottom.nX, Bottom.nY, Color);
		return;
	}
	
	if( Bottom.nX == Middle.nX && Bottom.nY == Middle.nY)
	{
		atGRPX_DrawLineAA(lpCanvas, Top.nX, Top.nY, Middle.nX, Middle.nY, Color);
		return;
	}
	//====== 각 선분의 각도를 측정하여 좌우를 판단(0이면 왼쪽 1이면 오른쪽) =============
	dx[2] = Bottom.nX-Top.nX;
	dy[2] = Bottom.nY-Top.nY;
	if(dy[2] != 0)
	{
		gradientTpBt = INTTOREAL(dx[2])/(dy[2]);
	}
	else
	{
		if(dx[2] > 0)
		{
			gradientTpBt = INTTOREAL(1000);
		}
		else
		{
			gradientTpBt = INTTOREAL(-1000);
		}
		
	}
	
	dx[0] = Middle.nX-Top.nX;
	dy[0] = Middle.nY-Top.nY;
	if(dy[0] != 0)
	{
		gradientTpMd = INTTOREAL(dx[0])/(dy[0]);
	}
	else
	{
		if(dx[0] > 0)
		{
			gradientTpMd = INTTOREAL(1000);
		}
		else
		{
			gradientTpMd = INTTOREAL(-1000);
		}
	}
	
	
	if(gradientTpMd > gradientTpBt)
	{
		TpBtEdge = 0;
		TpMdBtEdge = 1;
	}
	else if(gradientTpMd < gradientTpBt)
	{
		TpBtEdge = 1;
		TpMdBtEdge = 0;
	}
	else //Top과 Middle의 Y값이 같은 경우.
	{
		if(Top.nX > Middle.nX)
		{
			TpBtEdge = 1;
			TpMdBtEdge = 0;
		}
		else
		{
			TpBtEdge = 0;
			TpMdBtEdge = 1;
		}
	}

    //====== Bresenham 변수 세팅 ===========
	
	// Top - Middle
	SET_BRESENHAM_TRI_MIR_LINE_VAR( Top.nX, Top.nY, Middle.nX, Middle.nY, dx[0], dy[0], du[0], dv[0], u[0], v[0], drawu_s[0], drawv_s[0], drawu_e[0], drawv_e[0], uincr[0], vincr[0], inverse[0], isOdd[0], uend[0], d[0], incrS[0], incrD[0], twovdu[0], invD[0], invD2du[0], edgeflag[0]);
	// Top - Bottom
	SET_BRESENHAM_TRI_MIR_LINE_VAR( Top.nX, Top.nY, Bottom.nX, Bottom.nY, dx[1], dy[1], du[1], dv[1], u[1], v[1], drawu_s[1], drawv_s[1], drawu_e[1], drawv_e[1], uincr[1], vincr[1], inverse[1], isOdd[1], uend[1], d[1], incrS[1], incrD[1], twovdu[1], invD[1], invD2du[1], edgeflag[1]);
	// Middle - Bottom
	SET_BRESENHAM_TRI_MIR_LINE_VAR( Middle.nX, Middle.nY, Bottom.nX, Bottom.nY, dx[2], dy[2], du[2], dv[2], u[2], v[2], drawu_s[2], drawv_s[2], drawu_e[2], drawv_e[2], uincr[2], vincr[2], inverse[2], isOdd[2], uend[2], d[2], incrS[2], incrD[2], twovdu[2], invD[2], invD2du[2], edgeflag[2]);
    
	// Top_Middle을 그린다.
	y_up = -1;
	y_down = -1;
	if(aa_edge_index == 0)
	{
		cal_MIR_LINE_AA(g_edgelist, y_up, y_down, TpMdBtEdge, invD[0], twovdu[0], invD2du[0], diff_distance[0], alpha_var_1[0], alpha_var_2[0], inverse[0], lpCanvas, drawv_s[0], drawu_s[0], drawv_e[0], drawu_e[0], isOdd[0], vincr[0], uincr[0], clip, Color, d[0], du[0], incrS[0], incrD[0], v[0], u[0], uend[0]);
	}
	else
	{
		cal_MIR_LINE_MID(g_edgelist, y_up, y_down, TpMdBtEdge, invD[0], twovdu[0], invD2du[0], diff_distance[0], alpha_var_1[0], alpha_var_2[0], inverse[0], lpCanvas, drawv_s[0], drawu_s[0], drawv_e[0], drawu_e[0], isOdd[0], vincr[0], uincr[0], clip, Color, d[0], du[0], incrS[0], incrD[0], v[0], u[0], uend[0]);
	}

	y_up = -1;
	y_down = -1;
	// Top_Bottom을 그린다.
	if(aa_edge_index == 1)
	{
		cal_MIR_LINE_AA(g_edgelist, y_up, y_down, TpBtEdge, invD[1], twovdu[1], invD2du[1], diff_distance[1], alpha_var_1[1], alpha_var_2[1], inverse[1], lpCanvas, drawv_s[1], drawu_s[1], drawv_e[1], drawu_e[1], isOdd[1], vincr[1], uincr[1], clip, Color, d[1], du[1], incrS[1], incrD[1], v[1], u[1], uend[1]);
	}
	else
	{
		cal_MIR_LINE_MID(g_edgelist, y_up, y_down, TpBtEdge, invD[1], twovdu[1], invD2du[1], diff_distance[1], alpha_var_1[1], alpha_var_2[1], inverse[1], lpCanvas, drawv_s[1], drawu_s[1], drawv_e[1], drawu_e[1], isOdd[1], vincr[1], uincr[1], clip, Color, d[1], du[1], incrS[1], incrD[1], v[1], u[1], uend[1]);
	}
	
	y_up = -1;
	y_down = -1;
	// Middle_Bottom을 그린다.
	if(aa_edge_index == 2)
	{
		cal_MIR_LINE_AA(g_edgelist, y_up, y_down, TpMdBtEdge, invD[2], twovdu[2], invD2du[2], diff_distance[2], alpha_var_1[2], alpha_var_2[2], inverse[2], lpCanvas, drawv_s[2], drawu_s[2], drawv_e[2], drawu_e[2], isOdd[2], vincr[2], uincr[2], clip, Color, d[2], du[2], incrS[2], incrD[2], v[2], u[2], uend[2]);
	}
	else
	{
		cal_MIR_LINE_MID(g_edgelist, y_up, y_down, TpMdBtEdge, invD[2], twovdu[2], invD2du[2], diff_distance[2], alpha_var_1[2], alpha_var_2[2], inverse[2], lpCanvas, drawv_s[2], drawu_s[2], drawv_e[2], drawu_e[2], isOdd[2], vincr[2], uincr[2], clip, Color, d[2], du[2], incrS[2], incrD[2], v[2], u[2], uend[2]);
	}
	
	// 삼각형을 채운다.
	y = Top.nY;

	// error check code 추가. 알고리즘 수정해야 함. fix-me
	if( y < 0 ) y = 0;
	if (y >= MAX_EDGEBUFFER_SIZE) return;
	if (Bottom.nY >= MAX_EDGEBUFFER_SIZE) Bottom.nY = MAX_EDGEBUFFER_SIZE-1;
	
	for(y; y <= Bottom.nY; y++)
	{
		atGRPX_DrawHorizLine2(lpCanvas, g_edgelist[y][0]+1, y, g_edgelist[y][1] - g_edgelist[y][0] - 1,Color, clip);
	}

#else
    int y, height;
    atSPOINT Top, Middle, Bottom;
    atREAL X_TpBt, X_TpMdBt;
    atREAL dX_TpBt, dX_TpMd, dX_MdBt;
	atREAL b_TpBt, b_TpMd, b_MdBt; // y절편 값
	atSRECT *clip;
	int mxx, mix;
	
	//test
	int dx, dy;														
	
	clip = atGRPX_GetClipRect( lpCanvas );
	
    // 클리핑 영역을 완전히 벗어나면 종료
	mix = MIN3(X1, X2, X3);
    if( mix > clip->nEndX ) return;
	mxx = MAX3(X1, X2, X3);
    if( mxx < clip->nStartX ) return;
	
    // Sorting Triangle Vertex
    SORT_TRI_VERTEXS( Top,Middle,Bottom, X1,Y1,X2,Y2,X3,Y3 );
	
    // 클리핑 영역을 완전히 벗어나면 종료
    if( Top.nY > clip->nEndY ) return;
    if( Bottom.nY < clip->nStartY ) return;
	
    // 한줄짜리
    if( Top.nY == Bottom.nY ){
		_DrawHorizLine( lpCanvas, INTTOREAL(mix), INTTOREAL(mxx), Top.nY, ExParam1, fnDrawHLine );
		return;
    }
	
    // 각각의 기울기를 구한다.
	//    CALC_DELTA( Top,Middle,Bottom, dX_TpBt,dX_TpMd,dX_MdBt ); 
	
	
	dx = Bottom.nX - Top.nX;
	if( dx > 0 ) dx++; 
	else if(dx < 0) dx--;			
	dy = Bottom.nY - Top.nY; 
	if( dy > 0 ) dy++; 
	else if(dy < 0)dy--;
	
	if(dy != 0) dX_TpBt = INTTOREAL(dx) / (dy);	
	else dX_TpBt = 0;
	
	if(dx != 0)
	{
		b_TpBt = INTTOREAL(Bottom.nY) - REALMUL(REALDIV(INTTOREAL(dy),INTTOREAL(dx)), INTTOREAL(Bottom.nX));
	}
	else
	{
		b_TpBt = INTTOREAL(Bottom.nX);
	}
	
	
	dx = Middle.nX - Top.nX; 
	if( dx > 0 ) dx++; 
	else if(dx < 0) dx--;			
	dy = Middle.nY - Top.nY; 
	if( dy > 0 ) dy++; 
	else if(dy < 0)dy--;			
	
	if(dy != 0) dX_TpMd = INTTOREAL(dx) / (dy);	
	else dX_TpMd = 0;
	
	if(dx != 0)
	{
		b_TpMd = INTTOREAL(Middle.nY) - REALMUL(REALDIV(INTTOREAL(dy),INTTOREAL(dx)), INTTOREAL(Middle.nX));
	}
	else
	{
		b_TpMd = INTTOREAL(Middle.nX);
	}
	
	
	dx = Bottom.nX - Middle.nX; 
	if( dx > 0 ) dx++; 
	else if(dx < 0)dx--;		
	dy = Bottom.nY - Middle.nY; 
	if( dy > 0 ) dy++; 
	else if(dy < 0)dy--;	
	
	if(dy != 0) dX_MdBt = INTTOREAL(dx) / (dy);	
	else dX_MdBt = 0;
	
	if(dx != 0)
	{
		b_MdBt = INTTOREAL(Bottom.nY) - REALMUL(REALDIV(INTTOREAL(dy),INTTOREAL(dx)), INTTOREAL(Bottom.nX));
	}
	else
	{
		b_MdBt = INTTOREAL(Bottom.nX);
	}
	
	
	if(ABS(REALTOINT(dX_TpBt)) > 1)
	{
		atGRPX_DrawLineAA( lpCanvas, Top.nX, Top.nY, Bottom.nX, Bottom.nY, ExParam1 );
	}
	
	if(ABS(REALTOINT(dX_TpMd)) > 1)
	{
		atGRPX_DrawLineAA( lpCanvas, Top.nX, Top.nY, Middle.nX, Middle.nY, ExParam1 );
	}
	
	if(ABS(REALTOINT(dX_MdBt)) > 1)
	{
		atGRPX_DrawLineAA( lpCanvas, Middle.nX, Middle.nY, Bottom.nX, Bottom.nY, ExParam1 );
	}
	
    //======== 이제부터 본격적으로 그린다. ===========
    
    X_TpBt = X_TpMdBt = INTTOREAL(Top.nX);
	
    // 처음부터 중간까지 그린다.
    if( (Top.nY <= Middle.nY) && (Middle.nY >= clip->nStartY) )
	{
        y = Top.nY;
        
        if( Top.nY < clip->nStartY ){
			height = (clip->nStartY - Top.nY);
			//X_TpBt   += (dX_TpBt * height); //기울기를 높이를 곱해서 y축이 0인 곳으로 이동... 하지만 이곳에 오차가 있는것 같다.
			//X_TpMdBt += (dX_TpMd * height);
			if(dX_TpBt != 0)
			{
				X_TpBt = REALMUL(REALMUL(dX_TpBt, b_TpBt),INTTOREAL(-1));
			}
			else
			{
				X_TpBt = INTTOREAL(Bottom.nX);
			}
			
			if(dX_TpMd != 0)
			{
				X_TpMdBt = REALMUL(REALMUL(dX_TpMd, b_TpMd),INTTOREAL(-1));
			}
			else
			{
				X_TpMdBt = INTTOREAL(Middle.nX);
			}
			
			y = clip->nStartY;
        }
        if( Middle.nY > clip->nEndY ){
			Middle.nY = clip->nEndY;
        }
		
		//        if( Middle.nY >= clip->nStartY ){
		while( y < Middle.nY ){
			//            _DrawHorizLine( lpCanvas, REALTOINT(X_TpBt), REALTOINT(X_TpMdBt), y, ExParam1, fnDrawHLine );
            _DrawHorizLine( lpCanvas, X_TpBt, X_TpMdBt, y, ExParam1, fnDrawHLine );
			
            y ++;
            X_TpBt += dX_TpBt;
            X_TpMdBt += dX_TpMd;
		}
		//        }
    }
	
    if( Middle.nY >= clip->nEndY ) return;
	
	//    X_TpBt = INTTOREAL(Top.nX) + (dX_TpBt * (Middle.nY - Top.nY + 1)); // 이미 앞서 계산이 진행되어 왔으므로...
    X_TpMdBt = INTTOREAL(Middle.nX);
	
    // 중간부터 끝까지 그린다.
    if( Middle.nY <= Bottom.nY ){
        y = Middle.nY;
		
        if( Middle.nY < clip->nStartY ){
			height = (clip->nStartY - Middle.nY + 1);
			//          X_TpBt += (dX_TpBt * height);
			//          X_TpMdBt += (dX_MdBt * height);        
			if(dX_TpBt != 0)
			{
				X_TpBt = REALMUL(REALMUL(dX_TpBt, b_TpBt),INTTOREAL(-1));
			}
			else
			{
				X_TpBt = INTTOREAL(Bottom.nX);
			}
			
			if(dX_MdBt != 0)
			{
				X_TpMdBt = REALMUL(REALMUL(dX_MdBt, b_MdBt),INTTOREAL(-1));
			}
			else
			{
				X_TpMdBt = INTTOREAL(Bottom.nX);
			}
			
			y = clip->nStartY;
        }
        if( Bottom.nY > clip->nEndY ){
			Bottom.nY = clip->nEndY;
        }
		
        while( y < Bottom.nY ){
			//            _DrawHorizLine( lpCanvas, REALTOINT(X_TpBt), REALTOINT(X_TpMdBt), y, ExParam1, fnDrawHLine );
            _DrawHorizLine( lpCanvas, X_TpBt, X_TpMdBt, y, ExParam1, fnDrawHLine );
			
            X_TpBt += dX_TpBt;
            X_TpMdBt += dX_MdBt;
			
            y++;
        }
		
		_DrawHorizLine( lpCanvas, REALINTK(X_TpBt), REALINTK(X_TpMdBt), y, ExParam1, fnDrawHLine );

    }
#endif
}


atVOID	atGRPX_DrawBreTriangleEdgeAA(atGRPX_CANVAS* lpCanvas, atRPOINT Pt_0, atRPOINT Pt_1, atRPOINT Pt_2, atGRPX_COLOR Color, atGRPX_COLOR OutColor, atINT nWidth)
{
	

	int y;
    atPOINT Top, Middle, Bottom;
	atLONG X1,X2,X3,Y1,Y2,Y3;
    atLONG X_TpBt, X_TpMdBt;
    atLONG mxx, mix;
	atINT aa_edge_index = 0;

	///Bresenham 변수
	
	// 공통 변수
	atRECT* clip;
	atINT draw_x;
	atINT draw_y;
	atREAL gradientTpBt;
	atREAL gradientTpMd;
	atSHORT TpBtEdge = 0; // Top-Bottom edge가 삼각형의 우즉에 있는지 좌측에 있는지 정하는 변수.
	atSHORT TpMdBtEdge = 1;
	atINT y_up, y_down;
		
			
	// 각 Line별 변수 Top-Middle : 0, Top-Bottom : 1, Middle-Bottom : 2
	atLONG dx[3];
    atLONG dy[3];
	atLONG drawu_s[3];
	atLONG drawv_s[3];
	atLONG drawu_e[3];
	atLONG drawv_e[3];
	
	atLONG du[3];
	atLONG dv[3];
	atLONG u[3];
	atLONG v[3];
	
	atLONG uincr[3];			//Screen 좌표
	atLONG vincr[3];
	
	atLONG uend[3];
    atLONG d[3];				
    atLONG incrS[3];			
    atLONG incrD[3];			
    atLONG twovdu[3];			
	
    atLONG invD[3];			
    atLONG invD2du[3];		
	
	atBOOL inverse[3];
	
	atREAL diff_distance[3];
	
	atBOOL isOdd[3];
	atBOOL edgeflag[3]; // 좌우 픽셀 구분시 필요.
		
	atLONG alpha_var_1[3]; // 알파값 매개 변수
	atLONG alpha_var_2[3];


	X1 = REALTOINT(Pt_0.rX);
	Y1 = REALTOINT(Pt_0.rY);
	X2 = REALTOINT(Pt_1.rX);
	Y2 = REALTOINT(Pt_1.rY);
	X3 = REALTOINT(Pt_2.rX);
	Y3 = REALTOINT(Pt_2.rY);
	
	clip = atGRPX_GetClipRect( lpCanvas );

//	memset(g_edgelist, 0, sizeof(atLONG)*320*2);
	
    // 클리핑 영역을 완전히 벗어나면 종료
	mix = MIN3(X1, X2, X3);
    if( mix > clip->nEndX ) return;
	mxx = MAX3(X1, X2, X3);
    if( mxx < clip->nStartX ) return;
	
    // Sorting Triangle Vertex
    SORT_TRI_VERTEXS_EDGE( Top,Middle,Bottom, X1,Y1,X2,Y2,X3,Y3, aa_edge_index);

    // 클리핑 영역을 완전히 벗어나면 종료
    if( Top.nY > clip->nEndY ) return;
    if( Bottom.nY < clip->nStartY ) return;
	
    // 한줄짜리
    if( Top.nY == Bottom.nY ){
		atGRPX_DrawHorizLine( lpCanvas, INTTOREAL(mix), INTTOREAL(mxx), Top.nY, Color);
		return;
    }

	// 두점이 같은 경우
	if( Top.nX == Middle.nX && Top.nY == Middle.nY)
	{
		atGRPX_DrawLineAA(lpCanvas, Top.nX, Top.nY, Bottom.nX, Bottom.nY, Color);
		return;
	}
	
	if( Bottom.nX == Middle.nX && Bottom.nY == Middle.nY)
	{
		atGRPX_DrawLineAA(lpCanvas, Top.nX, Top.nY, Middle.nX, Middle.nY, Color);
		return;
	}

	//====== 각 선분의 각도를 측정하여 좌우를 판단(0이면 왼쪽 1이면 오른쪽) =============
	dx[2] = Bottom.nX-Top.nX;
	dy[2] = Bottom.nY-Top.nY;
	if(dy[2] != 0)
	{
		gradientTpBt = INTTOREAL(dx[2])/(dy[2]);
	}
	else
	{
		if(dx[2] > 0)
		{
			gradientTpBt = INTTOREAL(1000);
		}
		else
		{
			gradientTpBt = INTTOREAL(-1000);
		}
		
	}
	
	dx[0] = Middle.nX-Top.nX;
	dy[0] = Middle.nY-Top.nY;
	if(dy[0] != 0)
	{
		gradientTpMd = INTTOREAL(dx[0])/(dy[0]);
	}
	else
	{
		if(dx[0] > 0)
		{
			gradientTpMd = INTTOREAL(1000);
		}
		else
		{
			gradientTpMd = INTTOREAL(-1000);
		}
	}
	
	
	if(gradientTpMd > gradientTpBt)
	{
		TpBtEdge = 0;
		TpMdBtEdge = 1;
	}
	else if(gradientTpMd < gradientTpBt)
	{
		TpBtEdge = 1;
		TpMdBtEdge = 0;
	}
	else //Top과 Middle의 Y값이 같은 경우.
	{
		if(Top.nX > Middle.nX)
		{
			TpBtEdge = 1;
			TpMdBtEdge = 0;
		}
		else
		{
			TpBtEdge = 0;
			TpMdBtEdge = 1;
		}
	}

    //====== Bresenham 변수 세팅 ===========
	
	// Top - Middle
	SET_BRESENHAM_TRI_MIR_LINE_VAR( Top.nX, Top.nY, Middle.nX, Middle.nY, dx[0], dy[0], du[0], dv[0], u[0], v[0], drawu_s[0], drawv_s[0], drawu_e[0], drawv_e[0], uincr[0], vincr[0], inverse[0], isOdd[0], uend[0], d[0], incrS[0], incrD[0], twovdu[0], invD[0], invD2du[0], edgeflag[0]);
	// Top - Bottom
	SET_BRESENHAM_TRI_MIR_LINE_VAR( Top.nX, Top.nY, Bottom.nX, Bottom.nY, dx[1], dy[1], du[1], dv[1], u[1], v[1], drawu_s[1], drawv_s[1], drawu_e[1], drawv_e[1], uincr[1], vincr[1], inverse[1], isOdd[1], uend[1], d[1], incrS[1], incrD[1], twovdu[1], invD[1], invD2du[1], edgeflag[1]);
	// Middle - Bottom
	SET_BRESENHAM_TRI_MIR_LINE_VAR( Middle.nX, Middle.nY, Bottom.nX, Bottom.nY, dx[2], dy[2], du[2], dv[2], u[2], v[2], drawu_s[2], drawv_s[2], drawu_e[2], drawv_e[2], uincr[2], vincr[2], inverse[2], isOdd[2], uend[2], d[2], incrS[2], incrD[2], twovdu[2], invD[2], invD2du[2], edgeflag[2]);
    
	// Top_Middle을 그린다.
	if(aa_edge_index == 0)
	{
		cal_NODRAW_MIR_LINE_MID(g_edgelist, y_up, y_down, TpMdBtEdge, invD[0], twovdu[0], invD2du[0], diff_distance[0], alpha_var_1[0], alpha_var_2[0], inverse[0], lpCanvas, drawv_s[0], drawu_s[0], drawv_e[0], drawu_e[0], isOdd[0], vincr[0], uincr[0], clip, Color, d[0], du[0], incrS[0], incrD[0], v[0], u[0], uend[0]);
	}
	else
	{
		cal_MIR_LINE_MID(g_edgelist, y_up, y_down, TpMdBtEdge, invD[0], twovdu[0], invD2du[0], diff_distance[0], alpha_var_1[0], alpha_var_2[0], inverse[0], lpCanvas, drawv_s[0], drawu_s[0], drawv_e[0], drawu_e[0], isOdd[0], vincr[0], uincr[0], clip, Color, d[0], du[0], incrS[0], incrD[0], v[0], u[0], uend[0]);
	}

	// Top_Bottom을 그린다.
	if(aa_edge_index == 1)
	{
		cal_NODRAW_MIR_LINE_MID(g_edgelist, y_up, y_down, TpBtEdge, invD[1], twovdu[1], invD2du[1], diff_distance[1], alpha_var_1[1], alpha_var_2[1], inverse[1], lpCanvas, drawv_s[1], drawu_s[1], drawv_e[1], drawu_e[1], isOdd[1], vincr[1], uincr[1], clip, Color, d[1], du[1], incrS[1], incrD[1], v[1], u[1], uend[1]);
	}
	else
	{
		cal_MIR_LINE_MID(g_edgelist, y_up, y_down, TpBtEdge, invD[1], twovdu[1], invD2du[1], diff_distance[1], alpha_var_1[1], alpha_var_2[1], inverse[1], lpCanvas, drawv_s[1], drawu_s[1], drawv_e[1], drawu_e[1], isOdd[1], vincr[1], uincr[1], clip, Color, d[1], du[1], incrS[1], incrD[1], v[1], u[1], uend[1]);
	}

	// Middle_Bottom을 그린다.
	if(aa_edge_index == 2)
	{
		cal_NODRAW_MIR_LINE_MID(g_edgelist, y_up, y_down, TpMdBtEdge, invD[2], twovdu[2], invD2du[2], diff_distance[2], alpha_var_1[2], alpha_var_2[2], inverse[2], lpCanvas, drawv_s[2], drawu_s[2], drawv_e[2], drawu_e[2], isOdd[2], vincr[2], uincr[2], clip, Color, d[2], du[2], incrS[2], incrD[2], v[2], u[2], uend[2]);
	}
	else
	{
		cal_MIR_LINE_MID(g_edgelist, y_up, y_down, TpMdBtEdge, invD[2], twovdu[2], invD2du[2], diff_distance[2], alpha_var_1[2], alpha_var_2[2], inverse[2], lpCanvas, drawv_s[2], drawu_s[2], drawv_e[2], drawu_e[2], isOdd[2], vincr[2], uincr[2], clip, Color, d[2], du[2], incrS[2], incrD[2], v[2], u[2], uend[2]);
	}
	
	// 삼각형을 채운다.
	y = Top.nY;
	
	// error check code 추가. 알고리즘 수정해야 함. fix-me
	if( y < 0 ) y = 0;
	if (y >= MAX_EDGEBUFFER_SIZE)	return;
	if (Bottom.nY >= MAX_EDGEBUFFER_SIZE) Bottom.nY = MAX_EDGEBUFFER_SIZE-1;

	for(y; y <= Bottom.nY; y++)
	{
		atGRPX_DrawHorizLine2(lpCanvas, g_edgelist[y][0]+1, y, g_edgelist[y][1] - g_edgelist[y][0] - 1,Color, clip);
	}
}
