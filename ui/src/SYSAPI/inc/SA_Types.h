/*#############################################################################
-------------------------------------------------------------------------------

                                  SA_Types.h
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Base Defined Header

-------------------------------------------------------------------------------
#############################################################################*/
#ifndef __SA_TYPES_H_
#define __SA_TYPES_H_


#ifndef __AT_TYPES_H_
#include "AT_Types.h"
#endif


#ifdef __cplusplus
  extern "C" {
#endif


typedef struct _tagSYS_DATETIME {
    atSHORT	nYear;
    atCHAR	nMonth;
    atCHAR	nDay;
    atCHAR	nHour;
    atCHAR	nMinute;
    atCHAR	nSec;  
    atCHAR	tDummy;
} SYS_DATETIME;

#define SET_SYS_DATETIME( tm, year, month, day, hour, minute, sec ) \
	{								\
		(tm).nYear = (year);		\
		(tm).nMonth = (month);		\
		(tm).nDay = (day);			\
		(tm).nHour = (hour);		\
		(tm).nMinute = (minute);	\
		(tm).nSec = (sec);			\
		(tm).tDummy = 0;			\
	}


//typedef atU32		atCOLOR;

/*
typedef union _tagSYS_COLOR2 {
	struct {
		atBYTE    byAlpha;
		atBYTE	byRed;
		atBYTE	byGreen;
		atBYTE	byBlue;
	} tRGB;
	atU32 nColor;
} SYS_COLOR2;
*/


#ifdef __cplusplus
}
#endif


#endif
