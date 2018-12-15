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
#ifndef __GRPX_CFG_H_
#define __GRPX_CFG_H_


#ifndef __GRPX_USER_CFG_


#define atGRPX_PIXEL_BPP           (16)
#define atGRPX_ALPHA_BPP           (8)

#define __FIXEDPTR_
#define FIXED_RADIX_SIZE    (8)

#define atGRPX_DEF_MAX_CANVAS_COUNT	(256)

#define atGRPX_DEF_FONT_NAME       _AT("System")
#define atGRPX_DEF_FONT_SIZE		(16)

#define atGRPX_USES_ALPHABLEND

#define atGRPX_USES_POLYGON_CACHE
#define atGRPX_BASE_POLYGON_ENTRY   (1000)
#define atGRPX_MAX_POLYGON_ENTRY   (1000)
#ifdef atGRPX_USES_POLYGON_CACHE
#define atGRPX_AUTOINC_POLYGON_ENTRY
#endif

#endif


#endif
