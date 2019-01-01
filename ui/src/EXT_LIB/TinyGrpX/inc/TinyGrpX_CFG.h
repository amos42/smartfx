/*#############################################################################
-------------------------------------------------------------------------------

                                  GrpX_CFG.h
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : GrpX - 2D Graphics function pool

-------------------------------------------------------------------------------
#############################################################################*/
#ifndef __TINYGRPX_CFG_H_
#define __TINYGRPX_CFG_H_


#ifndef __TINYGRPX_USER_CFG_


#define atTGRPX_PIXEL_BPP           (16)
#define atTGRPX_ALPHA_BPP           (8)

#define __FIXEDPTR_
#define FIXED_RADIX_SIZE    (8)

#define atTGRPX_DEF_MAX_CANVAS_COUNT	(256)

#define atTGRPX_DEF_FONT_NAME       _AT("System")
#define atTGRPX_DEF_FONT_SIZE		(16)

#define atTGRPX_USES_ALPHABLEND

#define atTGRPX_USES_POLYGON_CACHE
#define atTGRPX_BASE_POLYGON_ENTRY   (1000)
#define atTGRPX_MAX_POLYGON_ENTRY   (1000)
#ifdef atTGRPX_USES_POLYGON_CACHE
#define atTGRPX_AUTOINC_POLYGON_ENTRY
#endif

#endif


#endif
