/*#############################################################################
-------------------------------------------------------------------------------

                                   Resource.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules : A_VM, A_Utils
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Resource Manager related function definition
 
-------------------------------------------------------------------------------
#############################################################################*/
#define __IMPLEMENT_

#include "AT_Types.h"
#include "A_RES.h"
#include "StrFunc.h"


/****************************
  배열을 채운다.
*****************************/
atVOID _memset_x( atLPVOID des, atCHAR ch, atLONG len )
{
	atCHAR *dptr = (atCHAR *)des;

	//if(len < 4) {
		while( len-- ) {
		    *dptr++ = ch;
		}
	//} else {
	//	long len2 = 4 - (len & 0x3);
	//	if(len2 > 0){
	//		len -= len2;
	//		while( len2-- ) {
	//		  *dptr++ = ch;
	//		}
	//	}
	//	len2 = len >> 2;
	//	if(len2 > 0){
	//		unsigned long *dptr2 = (unsigned long *)dptr;
	//		unsigned long v = ch;
	//		v |= (v << 24) | (v << 16) | (v << 8);
	//		while( len2-- ) {
	//		  *dptr2++ = v;
	//		}
	//		dptr = (atCHAR *)dptr2;
	//	}
	//	len &= 0x3;
	//	while( len-- > 0) {
	//	  *dptr++ = ch;
	//	}
	//}
}

/****************************
  문자열을 복사한다.
*****************************/
atLPTSTR _tcsncpy_x( atLPTSTR des, atLPCTSTR src, atLONG len )
{
	atTCHAR *dptr, *sptr, ch;
	
	dptr = (atTCHAR *)des;
	sptr = (atTCHAR *)src;
	if((dptr == atNULL) || (sptr == atNULL) || (len <= 0)) return (atTCHAR *)atNULL;
	while( ((ch = *sptr++) != _AT('\0')) && (len-- > 0)) {
	  *dptr++ = ch;
	}
	*dptr = _AT('\0');
	
	return (atTCHAR *)des;
}

/****************************
  문자열을 대문자로 바꾼다.
*****************************/
atLPSTR _strupr_x( atLPSTR str )
{
    atCHAR ch;
	int delta = (int)('a' - 'A');
    
    while( (ch = *str) != (atCHAR)'\0' ) {
      if( ch >= (atCHAR)'a' && ch <= (atCHAR)'z' ) *str = (atCHAR)(ch - delta);
      str++;
    }
    
    return( str );
}

/****************************
  문자열을 복사한다.
*****************************/
atLPSTR _strncpy_x( atLPSTR des, atLPCSTR src, atLONG len )
{
	atCHAR *dptr, *sptr, ch;
	
	dptr = (atCHAR *)des;
	sptr = (atCHAR *)src;
	if((dptr == atNULL) || (sptr == atNULL) || (len <= 0)) return (atCHAR *)atNULL;
	while( ((ch = *sptr++) != (atCHAR)'\0') && (len-- > 0)) {
	  *dptr++ = ch;
	}
	*dptr = (atCHAR)'\0' ;
	
	return (atCHAR *)des;
}

/****************************
  문자열을 비교한다.
*****************************/
atINT _strcmp_x( atLPCSTR str1, atLPCSTR str2 )
{
	atUCHAR *ptr1, *ptr2, ch1, ch2;
	
	ptr1 = (atUCHAR *)str1;
	ptr2 = (atUCHAR *)str2;
	if((ptr1 == atNULL) || (ptr2 == atNULL)) return 0;
	do {
	  ch1 = *ptr1++;
	  ch2 = *ptr2++;
	} while((ch1 == ch2) && (ch1 != (atCHAR)'\0'));
	
	return (atINT)ch1 - (atINT)ch2;
}
