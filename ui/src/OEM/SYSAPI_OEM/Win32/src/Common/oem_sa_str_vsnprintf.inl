#include "SYSAPI.h"
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
#ifdef _LPTSTR
#undef _LPTSTR
#endif
#ifdef _LPCTSTR
#undef _LPCTSTR
#endif

#ifdef __USES_FORCE_UNICODE_
#define _TCHAR wchar_t
#define _XT(a) L##a
#define _LOCAL_FUNC_(a) _w_vspf_##a
#define _LOCAL_LABEL_(a) _w_vspf##a
#define _GLOBAL_FUNC_(a) w_##a
#else
#define _TCHAR unsigned char
#define _XT(a) a
#define _LOCAL_FUNC_(a) _vspf_##a
#define _LOCAL_LABEL_(a) _vspf_##a
#define _GLOBAL_FUNC_(a) a
#endif

#define _LPTSTR  _TCHAR *
#define _LPCTSTR const _LPTSTR


#define _AT_ZEROPAD		1		// Pad with zero
#define _AT_SIGN		2		// Unsigned/signed long
#define _AT_PLUS		4		// Show plus
#define _AT_SPACE		8		// Space if plus
#define _AT_LEFT		16		// Left justified
#define _AT_SPECIAL		32		// 0x
#define _AT_LARGE		64		// Use 'ABCDEF' instead of 'abcdef'
#define _AT_SKIP		128		// Use 'ABCDEF' instead of 'abcdef'

#define _AT_is_digit(c)		((c) >= '0' && (c) <= '9')

#define	_AT_CVTBUFSIZE			352		// (309 + 43)


static atDOUBLE _LOCAL_FUNC_(_modf)(atDOUBLE value, atDOUBLE *iptr)
{
	*iptr = (atDOUBLE)(int)value;
	return value - (atDOUBLE)*iptr;
}

static _TCHAR * _LOCAL_FUNC_(_cvt)(atDOUBLE arg, int ndigits, int *decpt, int *sign, _TCHAR *buf, int eflag)
{
	_TCHAR *p, *p1;
	int r2;
	atDOUBLE fi, fj;
	
	if(ndigits < 0) ndigits = 0;
	if(ndigits >= _AT_CVTBUFSIZE - 1) ndigits = _AT_CVTBUFSIZE - 2;
  
	r2 = 0;
	*sign = 0;
	p = &buf[0];
	if(arg < 0) {
		*sign = 1;
		arg = -arg;
	}
	
	arg = _LOCAL_FUNC_(_modf)(arg, &fi);
	p1 = &buf[_AT_CVTBUFSIZE];

	if(fi != 0) {
		p1 = &buf[_AT_CVTBUFSIZE];
		
		while(fi != 0) {
			fj = _LOCAL_FUNC_(_modf)(fi / 10, &fi);
			*--p1 = (int)((fj + .03) * 10) + _XT('0');
			r2++;
		}
		
		while(p1 < &buf[_AT_CVTBUFSIZE]) *p++ = *p1++;
	} else if(arg > 0) {
		while((fj = arg * 10) < 1) {
			arg = fj;
			r2--;
		}
	}

	p1 = &buf[ndigits];
	if(eflag == 0) p1 += r2;

	*decpt = r2;
	if(p1 < &buf[0]) {
		buf[0] = '\0';
		return buf;
	}

	while(p <= p1 && p < &buf[_AT_CVTBUFSIZE]) {
		arg *= 10;
		arg = _LOCAL_FUNC_(_modf)(arg, &fj);
		*p++ = (int) fj + _XT('0');
	}

	if(p1 >= &buf[_AT_CVTBUFSIZE]) {
		buf[_AT_CVTBUFSIZE - 1] = _XT('\0');
		return buf;
	}

	p = p1;
	*p1 += 5;
	while(*p1 > _XT('9')) {
		*p1 = _XT('0');
		if(p1 > buf)
			++*--p1;
		else {
			*p1 = _XT('1');
			(*decpt)++;
			if(eflag == 0) {
				if(p > buf) *p = _XT('0');

		        p++;
			}
		}
	}

	*p = _XT('\0');
	return buf;
}

static _TCHAR * _LOCAL_FUNC_(_ecvtbuf)(atDOUBLE arg, int ndigits, int *decpt, int *sign, _TCHAR *buf)
{
	return _LOCAL_FUNC_(_cvt)(arg, ndigits, decpt, sign, buf, 1);
}

static _TCHAR * _LOCAL_FUNC_(_fcvtbuf)(atDOUBLE arg, int ndigits, int *decpt, int *sign, _TCHAR *buf)
{
	return _LOCAL_FUNC_(_cvt)(arg, ndigits, decpt, sign, buf, 0);
}


static const _TCHAR *_LOCAL_LABEL_(g_pdigits) = (const _TCHAR *)_XT("0123456789abcdefghijklmnopqrstuvwxyz");
static const _TCHAR *_LOCAL_LABEL_(g_pupper_digits) = (const _TCHAR *)_XT("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");

static void _LOCAL_FUNC_(_memcpy)(void *d, void *s, size_t cnt)
{
	char *ss = (char *)s;
	char *tt = (char *)d;
	while(cnt--) *tt++ = *ss++;
}

static size_t _LOCAL_FUNC_(_strlen)(const _TCHAR *s)
{
	const _TCHAR *sc;
	for(sc = s; *sc != _XT('\0'); ++sc);
	return sc - s;
}

static size_t _LOCAL_FUNC_(_strnlen)(const _TCHAR *s, size_t count)
{
	const _TCHAR *sc;
	for(sc = s; *sc != _XT('\0') && count--; ++sc);
	return sc - s;
}

static atINT _LOCAL_FUNC_(_skip_atoi)(const _TCHAR **s)
{
	int i = 0;
	while(_AT_is_digit(**s)) i = i*10 + *((*s)++) - _XT('0');
	return i;
}

static _TCHAR * _LOCAL_FUNC_(_number)(_TCHAR *str, long num, int base, int size, int precision, int type)
{
	_TCHAR c, sign, tmp[66] = {0, };
	_TCHAR *dig = (_TCHAR *)_LOCAL_LABEL_(g_pdigits);
	int i;

	if(type & _AT_LARGE)		dig = (_TCHAR *)_LOCAL_LABEL_(g_pupper_digits);
	if(type & _AT_LEFT)			type &= ~_AT_ZEROPAD;
	if(base < 2 || base > 36)	return 0;
  
	c = (type & _AT_ZEROPAD) ? _XT('0') : _XT(' ');
	sign = 0;
	
	if(type & _AT_SIGN) {
		if(num < 0) {
			sign = _XT('-');
			num = -num;
			size--;
		} else if(type & _AT_PLUS) {
			sign = _XT('+');
			size--;
		} else if(type & _AT_SPACE) {
			sign = _XT(' ');
			size--;
		}
	}

	if(type & _AT_SPECIAL) {
		if(base == 16)
			size -= 2;
		else if(base == 8)
			size--;
	}

	i = 0;
	if(num == 0) {
		if(!(type & _AT_SKIP))	tmp[i++] = _XT('0');
		else					tmp[i] = _XT('\0');
	} else {
		while(num != 0) {
			tmp[i++] = dig[((unsigned long) num) % (unsigned) base];
			num = ((unsigned long) num) / (unsigned) base;
		}
	}

	if(i > precision) precision = i;
	
	size -= precision;
	
	if(!(type & (_AT_ZEROPAD | _AT_LEFT))) while(size-- > 0) *str++ = _XT(' ');
	if(sign) *str++ = sign;
  
	if(type & _AT_SPECIAL) {
		if(base == 8)
			*str++ = _XT('0');
		else if(base == 16) {
			*str++ = _XT('0');
			*str++ = _LOCAL_LABEL_(g_pdigits)[33];
		}
	}

	if(!(type & _AT_LEFT)) while(size-- > 0) *str++ = c;
	while(i < precision--) *str++ = _XT('0');
	while(i-- > 0) *str++ = tmp[i];
	while(size-- > 0) *str++ = _XT(' ');

	return str;
}

static _TCHAR * _LOCAL_FUNC_(_eaddr)(_TCHAR *str, _TCHAR *addr, int size, int precision, int type)
{
	_TCHAR tmp[24] = {0, };
	_TCHAR *dig = (_TCHAR *)_LOCAL_LABEL_(g_pdigits);
	int i, len, addrlen;;

	if(type & _AT_LARGE)  dig = (_TCHAR *)_LOCAL_LABEL_(g_pupper_digits);
	len = 0;
	addrlen = _LOCAL_FUNC_(_strlen)(addr);
	for(i = 0; i < addrlen; i++) {
		if(i != 0) tmp[len++] = _XT(':');
		
		tmp[len++] = dig[addr[i] >> 4];
		tmp[len++] = dig[addr[i] & 0x0F];
	}

	if(!(type & _AT_LEFT)) while(len < size--) *str++ = _XT(' ');
	for(i = 0; i < len; ++i) *str++ = tmp[i];
	while(len < size--) *str++ = _XT(' ');

	return str;
}

static _TCHAR * _LOCAL_FUNC_(_iaddr)(_TCHAR *str, _TCHAR *addr, int size, int precision, int type)
{
	_TCHAR tmp[24] = {0, };
	int i, n, len, addrlen;

	len = 0;
	addrlen = _LOCAL_FUNC_(_strlen)((const _TCHAR*)addr);
	for(i = 0; i < addrlen; i++) {
		if(i != 0) tmp[len++] = _XT('.');
		n = addr[i];
		
		if(n == 0)
			tmp[len++] = _LOCAL_LABEL_(g_pdigits)[0];
		else {
			if(n >= 100) {
				tmp[len++] = _LOCAL_LABEL_(g_pdigits)[n / 100];
				n = n % 100;
				tmp[len++] = _LOCAL_LABEL_(g_pdigits)[n / 10];
				n = n % 10;
			} else if(n >= 10) {
				tmp[len++] = _LOCAL_LABEL_(g_pdigits)[n / 10];
				n = n % 10;
			}
			
			tmp[len++] = _LOCAL_LABEL_(g_pdigits)[n];
		}
	}
	
	if(!(type & _AT_LEFT)) while(len < size--) *str++ = _XT(' ');
	for(i = 0; i < len; ++i) *str++ = tmp[i];
	while(len < size--) *str++ = _XT(' ');

	return str;
}

////////////////////////////////////////////////////////////////////////////
#ifndef _AT_NOFLOAT
////////////////////////////////////////////////////////////////////////////

static atVOID _LOCAL_FUNC_(_cfltcvt)(atDOUBLE value, _TCHAR *buffer, _TCHAR fmt, int precision)
{
	_TCHAR *digits = NULL;
	_TCHAR cvtbuf[_AT_CVTBUFSIZE + 1] = {0, };
	int decpt, sign, exp, pos;
	int capexp = 0;
	int magnitude;

	if(fmt == _XT('G') || fmt == _XT('E')) {
		capexp = 1;
		fmt += _XT('a') - _XT('A');
	}
	
	if(fmt == _XT('g')) {
		digits = _LOCAL_FUNC_(_ecvtbuf)(value, precision, &decpt, &sign, cvtbuf);
		magnitude = decpt - 1;
		if(magnitude < -4  ||  magnitude > precision - 1) {
			fmt = _XT('e');
			precision -= 1;
		} else {
			fmt = _XT('f');
			precision -= decpt;
		}
	}

	if(fmt == _XT('e')) {
		digits = _LOCAL_FUNC_(_ecvtbuf)(value, precision + 1, &decpt, &sign, cvtbuf);
		if(sign) *buffer++ = _XT('-');
		
		*buffer++ = *digits;
		if(precision > 0) *buffer++ = _XT('.');

#ifdef __USES_FORCE_UNICODE_
		_LOCAL_FUNC_(_memcpy)(buffer, digits + 1, (precision << 1));
#else
		_LOCAL_FUNC_(_memcpy)(buffer, digits + 1, precision);
#endif
		buffer += precision;
		*buffer++ = capexp ? _XT('E') : _XT('e');

		if(decpt == 0) {
			if(value == 0.0)	exp = 0;
			else				exp = -1;
		} else					exp = decpt - 1;

		if(exp < 0) {
			*buffer++ = _XT('-');
			exp = -exp;
		} else {
			*buffer++ = _XT('+');
		}

		buffer[2] = (exp % 10) + _XT('0');
		exp = exp / 10;
		buffer[1] = (exp % 10) + _XT('0');
		exp = exp / 10;
		buffer[0] = (exp % 10) + _XT('0');
		buffer += 3;
	} else if(fmt == _XT('f')) {
		digits = _LOCAL_FUNC_(_fcvtbuf)(value, precision, &decpt, &sign, cvtbuf);
		if(sign) *buffer++ = _XT('-');
		if(*digits) {
			if(decpt <= 0) {
				*buffer++ = _XT('0');
				*buffer++ = _XT('.');

				for(pos = 0; pos < -decpt; pos++) *buffer++ = _XT('0');
				while(*digits) *buffer++ = *digits++;
			} else {
				pos = 0;
				while(*digits) {
					if(pos++ == decpt) *buffer++ = _XT('.');
					*buffer++ = *digits++;
				}
			}
		} else {
			*buffer++ = _XT('0');
			if(precision > 0) {
				*buffer++ = _XT('.');
				for(pos = 0; pos < precision; pos++) *buffer++ = _XT('0');
			}
		}
	}
	
	*buffer = _XT('\0');
}

static atVOID _LOCAL_FUNC_(_forcdecpt)(_TCHAR *buffer)
{
	while(*buffer) {
		if(*buffer == _XT('.')) return;
		if(*buffer == _XT('e') || *buffer == _XT('E')) break;
		
		buffer++;
	}

	if(*buffer) {
		int n = _LOCAL_FUNC_(_strlen)(buffer);
		while(n >= 0) {
			buffer[n + 1] = buffer[n];
			n--;
		}
	
		*buffer = _XT('.');
	} else {
		*buffer++ = _XT('.');
		*buffer = _XT('\0');
	}
}

static atVOID _LOCAL_FUNC_(_cropzeros)(_TCHAR *buffer)
{
	_TCHAR *stop;

	while(*buffer && *buffer != _XT('.')) buffer++;
	if(*buffer++) {
		while(*buffer && *buffer != _XT('e') && *buffer != _XT('E')) buffer++;
		stop = buffer--;
		while(*buffer == _XT('0')) buffer--;
		if(*buffer == _XT('.')) buffer--;
		while(*++buffer = *stop++);
	}
}

static _TCHAR * _LOCAL_FUNC_(_flt)(_TCHAR *str, atDOUBLE num, int size, int precision, _TCHAR fmt, int flags)
{
	_TCHAR tmp[80] = {0, };
	_TCHAR c, sign;
	int n, i;

	// Left align means no zero padding
	if(flags & _AT_LEFT) flags &= ~_AT_ZEROPAD;

	// Determine padding and sign char
	c = (flags & _AT_ZEROPAD) ? _XT('0') : _XT(' ');
	sign = 0;
	if(flags & _AT_SIGN) {
		if(num < 0.0) {
			sign = _XT('-');
			num = -num;
			size--;
		} else if(flags & _AT_PLUS) {
			sign = _XT('+');
			size--;
		} else if(flags & _AT_SPACE) {
			sign = _XT(' ');
			size--;
		}
	}

	// Compute the precision value
	if(precision < 0)
		precision = 6;		// Default precision: 6
	else if(precision == 0 && fmt == _XT('g'))
		precision = 1;		// ANSI specified

	// Convert floating point number to text
	_LOCAL_FUNC_(_cfltcvt)(num, tmp, fmt, precision);

	// '#' and precision == 0 means force a decimal point
	if((flags & _AT_SPECIAL) && precision == 0) _LOCAL_FUNC_(_forcdecpt)(tmp);

	// 'g' format means crop zero unless '#' given
	if(fmt == _XT('g') && !(flags & _AT_SPECIAL)) _LOCAL_FUNC_(_cropzeros)(tmp);

	n = _LOCAL_FUNC_(_strlen)(tmp);

	// Output number with alignment and padding
	size -= n;
	if(!(flags & (_AT_ZEROPAD | _AT_LEFT))) while(size-- > 0) *str++ = _XT(' ');
	if(sign) *str++ = sign;
	if(!(flags & _AT_LEFT)) while(size-- > 0) *str++ = c;
	for(i = 0; i < n; i++) *str++ = tmp[i];
	while(size-- > 0) *str++ = _XT(' ');

	return str;
}

#endif

static atINT _GLOBAL_FUNC_(my_vsnprintf)(_TCHAR *buf, int buflen, _LPCTSTR format, va_list args)
{
	_TCHAR *str;
	_TCHAR *s;
	_TCHAR *fmt;
	int len;
	unsigned long num;
	int i, base;
	int flags;            // Flags to number()
	int field_width;      // Width of output field
	int precision;        // Min. # of digits for integers; max number of chars for from string
	int qualifier;        // 'h', 'l', or 'L' for integer fields

	fmt = (_TCHAR *)format;

	for(str = buf; *fmt != _XT('\0'); fmt++) {
		if(*fmt != _XT('%')) {
			*str++ = *fmt;
			continue;
		}
        
		// Process flags
		flags = 0;
_AT_GOTO_REPEAT:
		fmt++;				// This also skips first '%'
		switch(*fmt) {
			case _XT('-'): flags |= _AT_LEFT; goto _AT_GOTO_REPEAT;
			case _XT('+'): flags |= _AT_PLUS; goto _AT_GOTO_REPEAT;
			case _XT(' '): flags |= _AT_SPACE; goto _AT_GOTO_REPEAT;
			case _XT('#'): flags |= _AT_SPECIAL; goto _AT_GOTO_REPEAT;
			case _XT('0'): flags |= _AT_ZEROPAD; goto _AT_GOTO_REPEAT;
		}
		
		// Get field width
		field_width = -1;
		if(_AT_is_digit(*fmt))
			field_width = _LOCAL_FUNC_(_skip_atoi)(&fmt);
		else if(*fmt == _XT('*')) {
			fmt++;
			field_width = va_arg(args, int);
			if(field_width < 0) {
				field_width = -field_width;
				flags |= _AT_LEFT;
			}
		}
		
		// Get the precision
		precision = -1;
		if(*fmt == _XT('.')) {
			++fmt;    
			if(_AT_is_digit(*fmt))
				precision = _LOCAL_FUNC_(_skip_atoi)(&fmt);
			else if(*fmt == _XT('*')) {
				++fmt;
				precision = va_arg(args, int);
			}

			if(precision < 0) precision = 0;
			if(precision == 0) flags |= _AT_SKIP;
		}
		
		// Get the conversion qualifier
		qualifier = -1;
		if(*fmt == _XT('h') || *fmt == _XT('l') || *fmt == _XT('L')) {
			qualifier = *fmt;
			fmt++;
		}

		// Default base
		base = 10;

		switch(*fmt) {
			case _XT('c'):
				if(!(flags & _AT_LEFT)) while(--field_width > 0) *str++ = _XT(' ');
				*str++ = (_TCHAR) va_arg(args, int);
				while(--field_width > 0) *str++ = _XT(' ');
				continue;
			
			case _XT('s'):
				s = va_arg(args, _TCHAR *);
				if(!s) s = (_TCHAR *)_XT("<NULL>");
				len = _LOCAL_FUNC_(_strnlen)(s, precision);
				if(!(flags & _AT_LEFT)) while(len < field_width--) *str++ = _XT(' ');
				for(i = 0; i < len; ++i) *str++ = *s++;
				while(len < field_width--) *str++ = _XT(' ');
				continue;

			case _XT('p'):
				if(field_width == -1) {
					field_width = 2 * sizeof(void *);
					flags |= _AT_ZEROPAD;
				}
				
				str = _LOCAL_FUNC_(_number)(str, (unsigned long) va_arg(args, void *), 16, field_width, precision, flags);
				continue;
				
			case _XT('n'):
				if(qualifier == _XT('l')) {
					long *ip = va_arg(args, long *);
					*ip = (str - buf);
				} else {
					int *ip = va_arg(args, int *);
					*ip = (str - buf);
				}
				continue;

			case _XT('A'):
				flags |= _AT_LARGE;
			case _XT('a'):
				if(qualifier == _XT('l')) {
					str = _LOCAL_FUNC_(_eaddr)(str, va_arg(args, _TCHAR *), field_width, precision, flags);
				} else {
					str = _LOCAL_FUNC_(_iaddr)(str, va_arg(args, _TCHAR *), field_width, precision, flags);
				}

				continue;
				
				// Integer number formats - set up the flags and "break"
				case _XT('o'):
					base = 8;
					break;
				
				case _XT('X'):
					flags |= _AT_LARGE;
				case 'x':
					base = 16;
					break;

				case _XT('d'):
				case _XT('i'):
					flags |= _AT_SIGN;
				
				case _XT('u'):
					break;
#ifndef _AT_NOFLOAT
				case _XT('E'):
				case _XT('G'):
				case _XT('e'):
				case _XT('f'):
				case _XT('g'):
					str = _LOCAL_FUNC_(_flt)(str, va_arg(args, atDOUBLE), field_width, precision, *fmt, flags | _AT_SIGN);
					continue;
#endif

				default:
					if(*fmt != _XT('%')) *str++ = _XT('%');
					if(*fmt)		*str++ = *fmt;
					else			--fmt;
					continue;
		}

		if(qualifier == _XT('l')) {				num = va_arg(args, unsigned long);
		} /*else if(qualifier == _XT('h')) { 
			if(flags & _AT_SIGN)			num = va_arg(args, short);
			else							num = va_arg(args, unsigned short);
		} */else if(flags & _AT_SIGN) {		num = va_arg(args, int);
		} else								num = va_arg(args, unsigned int);

		str = _LOCAL_FUNC_(_number)(str, num, base, field_width, precision, flags);
	}
	
	*str = _XT('\0');
	return str - buf;
}


#undef _AT_ZEROPAD
#undef _AT_SIGN
#undef _AT_PLUS
#undef _AT_SPACE
#undef _AT_LEFT
#undef _AT_SPECIAL
#undef _AT_LARGE
#undef _AT_SKIP

#undef _AT_is_digit

#undef	_AT_CVTBUFSIZE
