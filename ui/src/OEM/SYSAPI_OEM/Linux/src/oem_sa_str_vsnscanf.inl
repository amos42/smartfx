#include "OEM_SA_Mem.h"
#include <string.h>
#include <wchar.h>


#ifdef _TCHAR
#undef _TCHAR
#endif
#ifdef _XT
#undef _XT
#endif
#ifdef _LOCAL_FUNC_
#undef _LOCAL_FUNC_
#endif
#ifdef _GLOBAL_FUNC_
#undef _GLOBAL_FUNC_
#endif
#ifdef _LOCAL_LABEL_
#undef _LOCAL_LABEL_
#endif

#ifdef __USES_FORCE_UNICODE_
#define _TCHAR wchar_t
#define _XT(a) L##a
#define _LOCAL_FUNC_(a) _w_vsf_##a
#define _LOCAL_LABEL_(a) _w_vsf_##a
#define _GLOBAL_FUNC_(a) w##a
#else
#define _TCHAR unsigned char
#define _XT(a) a
#define _LOCAL_FUNC_(a) _vsf_##a
#define _LOCAL_LABEL_(a) _vsf_##a
#define _GLOBAL_FUNC_(a) a
#endif



// Character types
#define _ATC_UPPER          0x1     // Upper case letter
#define _ATC_LOWER          0x2     // Lower case letter
#define _ATC_DIGIT          0x4     // Digit[0-9]
#define _ATC_SPACE          0x8     // Tab, carriage return, newline, vertical tab or form feed
#define _ATC_PUNCT          0x10    // Punctuation character
#define _ATC_CONTROL        0x20    // Control character
#define _ATC_BLANK          0x40    // Space char
#define _ATC_HEX            0x80    // Hexadecimal digit

#define _AT_DBL_MAX_EXP		308
#define _AT_DBL_MIN_EXP		(-307)
#define _AT_EOF				(-1)

#define _AT_BUFSIZ		512


static unsigned short _LOCAL_LABEL_(__ctype)[257] = 
{
  0,                      // -1 EOF
  _ATC_CONTROL,               // 00 (NUL)
  _ATC_CONTROL,               // 01 (SOH)
  _ATC_CONTROL,               // 02 (STX)
  _ATC_CONTROL,               // 03 (ETX)
  _ATC_CONTROL,               // 04 (EOT)
  _ATC_CONTROL,               // 05 (ENQ)
  _ATC_CONTROL,               // 06 (ACK)
  _ATC_CONTROL,               // 07 (BEL)
  _ATC_CONTROL,               // 08 (BS)
  _ATC_SPACE+_ATC_CONTROL,     // 09 (HT)
  _ATC_SPACE+_ATC_CONTROL,     // 0A (LF)
  _ATC_SPACE+_ATC_CONTROL,     // 0B (VT)
  _ATC_SPACE+_ATC_CONTROL,     // 0C (FF)
  _ATC_SPACE+_ATC_CONTROL,     // 0D (CR)
  _ATC_CONTROL,               // 0E (SI)
  _ATC_CONTROL,               // 0F (SO)
  _ATC_CONTROL,               // 10 (DLE)
  _ATC_CONTROL,               // 11 (DC1)
  _ATC_CONTROL,               // 12 (DC2)
  _ATC_CONTROL,               // 13 (DC3)
  _ATC_CONTROL,               // 14 (DC4)
  _ATC_CONTROL,               // 15 (NAK)
  _ATC_CONTROL,               // 16 (SYN)
  _ATC_CONTROL,               // 17 (ETB)
  _ATC_CONTROL,               // 18 (CAN)
  _ATC_CONTROL,               // 19 (EM)
  _ATC_CONTROL,               // 1A (SUB)
  _ATC_CONTROL,               // 1B (ESC)
  _ATC_CONTROL,               // 1C (FS)
  _ATC_CONTROL,               // 1D (GS)
  _ATC_CONTROL,               // 1E (RS)
  _ATC_CONTROL,               // 1F (US)
  _ATC_SPACE+_ATC_BLANK,          // 20 SPACE
  _ATC_PUNCT,                 // 21 !
  _ATC_PUNCT,                 // 22 "
  _ATC_PUNCT,                 // 23 #
  _ATC_PUNCT,                 // 24 $
  _ATC_PUNCT,                 // 25 %
  _ATC_PUNCT,                 // 26 &
  _ATC_PUNCT,                 // 27 '
  _ATC_PUNCT,                 // 28 (
  _ATC_PUNCT,                 // 29 )
  _ATC_PUNCT,                 // 2A *
  _ATC_PUNCT,                 // 2B +
  _ATC_PUNCT,                 // 2C ,
  _ATC_PUNCT,                 // 2D -
  _ATC_PUNCT,                 // 2E .
  _ATC_PUNCT,                 // 2F /
  _ATC_DIGIT+_ATC_HEX,            // 30 0
  _ATC_DIGIT+_ATC_HEX,            // 31 1
  _ATC_DIGIT+_ATC_HEX,            // 32 2
  _ATC_DIGIT+_ATC_HEX,            // 33 3
  _ATC_DIGIT+_ATC_HEX,            // 34 4
  _ATC_DIGIT+_ATC_HEX,            // 35 5
  _ATC_DIGIT+_ATC_HEX,            // 36 6
  _ATC_DIGIT+_ATC_HEX,            // 37 7
  _ATC_DIGIT+_ATC_HEX,            // 38 8
  _ATC_DIGIT+_ATC_HEX,            // 39 9
  _ATC_PUNCT,                 // 3A :
  _ATC_PUNCT,                 // 3B ;
  _ATC_PUNCT,                 // 3C <
  _ATC_PUNCT,                 // 3D =
  _ATC_PUNCT,                 // 3E >
  _ATC_PUNCT,                 // 3F ?
  _ATC_PUNCT,                 // 40 @
  _ATC_UPPER+_ATC_HEX,            // 41 A
  _ATC_UPPER+_ATC_HEX,            // 42 B
  _ATC_UPPER+_ATC_HEX,            // 43 C
  _ATC_UPPER+_ATC_HEX,            // 44 D
  _ATC_UPPER+_ATC_HEX,            // 45 E
  _ATC_UPPER+_ATC_HEX,            // 46 F
  _ATC_UPPER,                 // 47 G
  _ATC_UPPER,                 // 48 H
  _ATC_UPPER,                 // 49 I
  _ATC_UPPER,                 // 4A J
  _ATC_UPPER,                 // 4B K
  _ATC_UPPER,                 // 4C L
  _ATC_UPPER,                 // 4D M
  _ATC_UPPER,                 // 4E N
  _ATC_UPPER,                 // 4F O
  _ATC_UPPER,                 // 50 P
  _ATC_UPPER,                 // 51 Q
  _ATC_UPPER,                 // 52 R
  _ATC_UPPER,                 // 53 S
  _ATC_UPPER,                 // 54 T
  _ATC_UPPER,                 // 55 U
  _ATC_UPPER,                 // 56 V
  _ATC_UPPER,                 // 57 W
  _ATC_UPPER,                 // 58 X
  _ATC_UPPER,                 // 59 Y
  _ATC_UPPER,                 // 5A Z
  _ATC_PUNCT,                 // 5B [
  _ATC_PUNCT,                 // 5C \ 
  _ATC_PUNCT,                 // 5D ]
  _ATC_PUNCT,                 // 5E ^
  _ATC_PUNCT,                 // 5F _
  _ATC_PUNCT,                 // 60 `
  _ATC_LOWER+_ATC_HEX,            // 61 a
  _ATC_LOWER+_ATC_HEX,            // 62 b
  _ATC_LOWER+_ATC_HEX,            // 63 c
  _ATC_LOWER+_ATC_HEX,            // 64 d
  _ATC_LOWER+_ATC_HEX,            // 65 e
  _ATC_LOWER+_ATC_HEX,            // 66 f
  _ATC_LOWER,                 // 67 g
  _ATC_LOWER,                 // 68 h
  _ATC_LOWER,                 // 69 i
  _ATC_LOWER,                 // 6A j
  _ATC_LOWER,                 // 6B k
  _ATC_LOWER,                 // 6C l
  _ATC_LOWER,                 // 6D m
  _ATC_LOWER,                 // 6E n
  _ATC_LOWER,                 // 6F o
  _ATC_LOWER,                 // 70 p
  _ATC_LOWER,                 // 71 q
  _ATC_LOWER,                 // 72 r
  _ATC_LOWER,                 // 73 s
  _ATC_LOWER,                 // 74 t
  _ATC_LOWER,                 // 75 u
  _ATC_LOWER,                 // 76 v
  _ATC_LOWER,                 // 77 w
  _ATC_LOWER,                 // 78 x
  _ATC_LOWER,                 // 79 y
  _ATC_LOWER,                 // 7A z
  _ATC_PUNCT,                 // 7B {
  _ATC_PUNCT,                 // 7C |
  _ATC_PUNCT,                 // 7D }
  _ATC_PUNCT,                 // 7E ~
  _ATC_CONTROL,               // 7F (DEL)
  // and the rest are 0...
};

// pointer to table for char's
static atUSHORT * _LOCAL_LABEL_(__pctype) = _LOCAL_LABEL_(__ctype) + 1; 

static size_t _LOCAL_FUNC_(_strlen)(const _TCHAR *s)
{
	const _TCHAR *sc;
	for(sc = s; *sc != _XT('\0'); ++sc);
	return sc - s;
}

static void _LOCAL_FUNC_(_strncpy)(_TCHAR *d, _TCHAR *s, size_t len)
{
	if( len <= 0 ) return;
	
	while( (*s != _XT('\0')) && len--)
		*d++ = *s++;
	*d = _XT('\0');
}

static atINT _LOCAL_FUNC_(_isdigit)(atINT c)
{
	return _LOCAL_LABEL_(__pctype)[c] & _ATC_DIGIT;
}

static atINT _LOCAL_FUNC_(_isspace)(atINT c)
{
	return _LOCAL_LABEL_(__pctype)[c] & _ATC_SPACE;
}

static atDOUBLE _LOCAL_FUNC_(_strtod)(const _TCHAR *str, _TCHAR **endptr)
{
	atDOUBLE number;
	atINT exponent;
	atINT negative;
	_TCHAR *p = (_TCHAR *) str;
	atDOUBLE p10;
	atINT n, num_digits, num_decimals;

	// Skip leading whitespace
	while( _LOCAL_FUNC_(_isspace)(*p)) p++;

	// Handle optional sign
	negative = 0;
	switch(*p) {             
		case _XT('-'): negative = 1; // Fall through to increment position
		case _XT('+'): p++;
	}
	
	number = 0.;
	exponent = 0;
	num_digits = 0;
	num_decimals = 0;

	// Process string of digits
	while( _LOCAL_FUNC_(_isdigit)(*p)) {
		number = number * 10. + (*p - '0');
		p++;
		num_digits++;
	}

	// Process decimal part
	if(*p == _XT('.')) {
		p++;
		
		while( _LOCAL_FUNC_(_isdigit)(*p)) {
			number = number * 10. + (*p - _XT('0'));
			p++;
			num_digits++;
			num_decimals++;
		}
		
		exponent -= num_decimals;
	}

	if(num_digits == 0) {
		//errno = ERANGE;
		return 0.0;
	}

	// Correct for sign
	if(negative) number = -number;

	// Process an exponent string
	if(*p == _XT('e') || *p == _XT('E')) {
		// Handle optional sign
		negative = 0;
		switch(*++p) {   
			case _XT('-'): negative = 1;   // Fall through to increment pos
			case _XT('+'): p++;
		}

		// Process string of digits
		n = 0;
		while( _LOCAL_FUNC_(_isdigit)(*p)) {   
			n = n * 10 + (*p - '0');
			p++;
		}
		
		if(negative)	exponent -= n;
		else			exponent += n;
	}
	
	if(exponent < _AT_DBL_MIN_EXP  || exponent > _AT_DBL_MAX_EXP) {
		//errno = ERANGE;
		return _AT_EOF;
	}

	// Scale the result
	p10 = 10.;
	n = exponent;
	if(n < 0) n = -n;
	while(n) {
		if(n & 1) {
			if(exponent < 0)	number /= p10;
			else				number *= p10;
		}

		n >>= 1;
		p10 *= p10;
	}


	//if(number == HUGE_VAL) errno = ERANGE;
	if(endptr) *endptr = p;
	
	return number;
}

static atDOUBLE _LOCAL_FUNC_(_atof)(const _TCHAR *str)
{
	return _LOCAL_FUNC_(_strtod)(str, NULL);
}

static atLONG _LOCAL_FUNC_(_atol)(const _TCHAR *nptr)
{
	atINT c;
	atLONG total;
	atINT sign;

	while( _LOCAL_FUNC_(_isspace)((atINT)(_TCHAR) *nptr)) ++nptr;

	c = (atINT)(_TCHAR) *nptr++;
	sign = c;
	if(c == _XT('-') || c == _XT('+')) c = (atINT)(_TCHAR) *nptr++;

	total = 0;
	while( _LOCAL_FUNC_(_isdigit)(c)) {
		total = 10 * total + (c - _XT('0'));
		c = (atINT)(_TCHAR) *nptr++;
	}

	if(sign == _XT('-'))	return -total;
	else			return total;
}

static atINT _LOCAL_FUNC_(_atoi2)(const _TCHAR *nptr)
{
  return (atINT)_LOCAL_FUNC_(_atol)(nptr);
}

atINT _GLOBAL_FUNC_(my_vsnscanf)( _TCHAR *buf, int buflen, const _TCHAR *fmt, va_list args )
{
	atFLOAT *f;
	atINT conv = 0, *i, index;
	_TCHAR *a, *fp, *tp, *sp = buf;
	_TCHAR szTemp[_AT_BUFSIZ] = {_XT('\0')};

	for( fp = (_TCHAR *)fmt, tp = (_TCHAR *)fmt; *fp != _XT('\0'); fp++ ) {
		// 미리 % 타입에 대한 접근하기 위한 temp(format) pointer
		while( *tp != _XT('%') ) tp++;
		if( *tp == _XT('%') ) ++tp;

		// 버퍼 초기화
		OEM_SYSAPI_memset(szTemp, '\0', sizeof(_TCHAR) * _AT_BUFSIZ);
		// % 타입이 'c' or 'C' 인 경우 1 char 만 buffer 에 parsing
		// 그외에는 '\0', ' ', '\n' 일때까지 buffer 에 pasring
		for( index = 0; \
			(*tp != _XT('C') && *tp != _XT('c')) ? (*sp != _XT('\0') && *sp != _XT(' ') && *sp != _XT('\n')) : (*sp != _XT('\0') && index < 1); \
			index++ ) {
				szTemp[index] = *sp++;
		}
	
		// % 타입 이외에 source pointer 증가
		if(*tp != _XT('C') && *tp != _XT('c')) {
			while( *sp == _XT(' ') || *sp == _XT('\n') ) sp++;
		}

		// format pointer check
		while( *fp != _XT('%') ) fp++;
		if( *fp == _XT('%') ) {
			switch( *++fp ) {
				case _XT('i'):
				case _XT('d'):
					i = va_arg( args, atINT * );
					*i = _LOCAL_FUNC_(_atoi2)( szTemp );
					break;
				case _XT('f'):
					f = va_arg( args, atFLOAT * );
					*f = (atFLOAT)_LOCAL_FUNC_(_atof)( szTemp );
					break;
				case _XT('S'):
				case _XT('s'):
					a = va_arg( args, _TCHAR * );
					_LOCAL_FUNC_(_strncpy)( a, szTemp, _LOCAL_FUNC_(_strlen)( szTemp ) + 1 );
					break;
				case _XT('C'):
				case _XT('c'):
					a = va_arg( args, _TCHAR * );
					*a = szTemp[0];
					break;
			}

			conv++;
			tp = fp;	// format pointer backup -> temp(format) pointer
		}
	}

	return conv;	
}


#undef _ATC_UPPER
#undef _ATC_LOWER
#undef _ATC_DIGIT
#undef _ATC_SPACE
#undef _ATC_PUNCT
#undef _ATC_CONTROL
#undef _ATC_BLANK
#undef _ATC_HEX  

#undef _AT_DBL_MAX_EXP	
#undef _AT_DBL_MIN_EXP	
#undef _AT_EOF			

#undef _AT_BUFSIZ
