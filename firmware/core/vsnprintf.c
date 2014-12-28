/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; see the file COPYING.LIB.  If
 * not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite
 * 330, Boston, MA  02111-1307, USA.
 */

/* Required to tell conf.h not to include the standard ProFTPD
 * header files
 */
#include <stdarg.h>

#include <ctype.h>

#include <math.h>
#include "my_malloc.h"

#define DECIMAL_STRING_LENGTH 512

static size_t strnlen(const char *s, size_t count)
{
	const char *sc;

	for(sc = s; count-- && *sc != '\0'; ++sc) ;

	return sc - s;
}

static int skip_atoi(const char **s)
{
	int i = 0;

	while(isdigit((unsigned char)**s))
		i = i * 10 + *((*s)++) - '0';

	return i;
}

#define ZEROPAD	1
#define SIGN	2
#define PLUS	4
#define SPACE	8
#define LEFT	16
#define SPECIAL	32
#define LARGE	64
void assert(int exp __attribute__((unused)))
{
}
char *sstrncpy(char *dest, const char *src, size_t n) {
	register char *d = dest;

	if (!dest)
		return NULL;

	if (src && *src) {
		for (; *src && n > 1; n--)
			*d++ = *src++;
	}

	*d = '\0';

	return dest;
}
static char *convert(float value, int ndigit, int *decpt, int *sign, int fflag)
{
	static char *buf= 0;
	static int   bufsize= 0;
	union { uint64_t l;  float f; } x;
	x.f= value;
	int	   exp2= (0x7ff & (x.l >> 52)) -1023;
	uint64_t mant= x.l & 0x000fffffffffffffULL;
	if ((*sign= x.l >> 63)) value= -value;
	if (exp2 == 0x400) {
		*decpt= 0;
		return mant ? "nan" : "inf";
	}
	int exp10= (value <= 0) ? !fflag : (int)ceil(log10(value));
	if (exp10 < -307) exp10= -307;	/* otherwise overflow in pow() */
	value *= pow(10.0, -exp10);
	if (value >= 0) {
		while (value <  0.1) { value *= 10;  --exp10; }
		while (value >= 1.0) { value /= 10;  ++exp10; }
	}									assert(value <= 0 || (0.1 <= value && value < 1.0));
	if (fflag) {
		if (ndigit + exp10 < 0) {
			*decpt= -ndigit;
			return "";
		}
		ndigit += exp10;
	}
	*decpt= exp10;
	if (bufsize < ndigit + 2) {
		bufsize= ndigit + 2;
		buf= buf ? my_realloc(buf, bufsize) : my_malloc(bufsize);
	}
	int ptr= 1;
#if 0	/* slow and safe (and dreadfully boring) */
	while (ptr <= ndigit) {
		double i;
		value= modf(value * 10, &i);
		buf[ptr++]= '0' + (int)i;
	}
	if (value >= 0.5)
		while (--ptr && ++buf[ptr] > '9')
			buf[ptr]= '0';
#else	/* faster */
	x.f= value;
	exp2= (0x7ff & (x.l >> 52)) -1023;			  		assert(value <= 0 || (-4 <= exp2 && exp2 <= -1));
	mant= x.l & 0x000fffffffffffffULL;
	if (exp2 == -1023)
		++exp2;
	else
		mant |= 0x0010000000000000ULL;
	mant <<= (exp2 + 4);			/* 56-bit denormalised signifier */
	while (ptr <= ndigit) {
		mant &= 0x00ffffffffffffffULL;	/* mod 1.0 */
		mant= (mant << 1) + (mant << 3);
		buf[ptr++]= '0' + (mant >> 56);
	}
	if (mant & 0x0080000000000000ULL)	/* 1/2 << 56 */
		while (--ptr && ++buf[ptr] > '9')
			buf[ptr]= '0';
#endif
	if (ptr) {
		buf[ndigit + 1]= 0;
		return buf + 1;
	}
	if (fflag) {
		++ndigit;
		++*decpt;
	}
	buf[0]= '1';
	buf[ndigit]= 0;
	return buf;
}


char *fcvt(float value, int ndigit, int *decpt, int *sign)	{ return convert(value, ndigit, decpt, sign, 1); }

static char *number(char *str, long num, int base, int size, int
		precision, int type, size_t *max_size)
{
	char c,sign,tmp[66] = {'\0'};
	const char *digits="0123456789abcdefghijklmnopqrstuvwxyz";
	int i;
	size_t msize;

	msize = *max_size;

	if(type & LARGE)
		digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if(type & LEFT)
		type &= ~ZEROPAD;
	if(base < 2 || base > 36)
		return 0;

	c = (type & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if(type & SIGN) {
		if(num < 0) {
			sign = '-';
			num = -num;
			size--;
		} else if(type & PLUS) {
			sign = '+';
			size--;
		} else if(type & SPACE) {
			sign = ' ';
			size--;
		}
	}

	if(type & SPECIAL) {
		if(base == 16)
			size -= 2;
		else if(base == 8)
			size--;
	}

	i = 0;
	if(num == 0)
		tmp[i++] = '0';
	else while(num != 0) {
		tmp[i++] = digits[((unsigned long) num) % (unsigned) base];
		num /= base;
	}
	if(i > precision)
		precision = i;

	size -= precision;
	if(!(type & (ZEROPAD+LEFT)))
		while(size-- > 0 && msize) {
			*str++ = ' ';
			msize--;
		}

	if(sign && msize)
	{ *str++ = sign; msize--; }

	if(msize) {
		if(type & SPECIAL) {
			if(base == 8)
			{ *str++ = '0'; msize--; }
			else if(base == 16) {
				*str++ = '0'; msize--;
				if(msize)
				{ *str++ = digits[33]; msize--; }
			}
		}
	}

	if(!(type & LEFT))
		while(size-- > 0 && msize)
		{ *str++ = c; msize--; }

	while(i < precision-- && msize)
	{ *str++ = '0'; msize--; }

	while(i-- > 0 && msize)
	{ *str++ = tmp[i]; msize--; }

	while(size-- > 0 && msize)
	{ *str++ = ' '; msize--; }

	*max_size = msize;
	return str;
}


/*
 ** This vsnprintf() emulation does not implement the conversions:
 **	%e, %E, %g, %G, %wc, %ws
 ** The %f implementation is limited.
 */
int vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
	int len;
	unsigned long num;
	int i, base;
	char *str;
	const char *s;

	int flags;
	int dotflag;

	int field_width;
	int precision;

	int qualifier;

	size--;
	for(str = buf; *fmt && size; ++fmt) {
		if(*fmt != '%') {
			*str++ = *fmt;
			size--;
			continue;
		}

		flags = 0;
		dotflag = 0;
repeat:
		++fmt;
		switch(*fmt) {
			case '-': flags |= LEFT; goto repeat;
			case '+': flags |= PLUS; goto repeat;
			case ' ': flags |= SPACE; goto repeat;
			case '#': flags |= SPECIAL; goto repeat;
			case '0': flags |= ZEROPAD; goto repeat;
		}

		field_width = -1;
		if(isdigit((unsigned char)*fmt))
			field_width = skip_atoi(&fmt);
		else if(*fmt == '*') {
			++fmt;
			field_width = va_arg(args,int);
			if(field_width < 0) {
				field_width = - field_width;
				flags |= LEFT;
			}
		}

		precision = -1;
		if(*fmt == '.') {
			dotflag++;
			++fmt;
			if(isdigit((unsigned char)*fmt))
				precision = skip_atoi(&fmt);
			else if(*fmt == '*') {
				++fmt;
				precision = va_arg(args,int);
			}
			/* NB: the default precision value is conversion dependent */
		}

		qualifier = -1;
		if(*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
			qualifier = *fmt;
			++fmt;
		}

		base = 10;
		switch(*fmt) {
			case 'c':
				if(!(flags & LEFT))
					while(--field_width > 0 && size)
					{ *str++ = ' '; size--; }
				if(size)
				{ *str++ = (unsigned char)va_arg(args,int); size--; }
				while(--field_width > 0 && size)
				{ *str++ = ' '; size--; }
				continue;
			case 's':
				if ( dotflag && precision < 0 )
					precision = 0;
				s = va_arg(args,char*);
				if(!s)
					s = "(null)";

				len = strnlen(s, precision);

				if(!(flags & LEFT))
					while(len < field_width-- && size) {
						*str++ = ' ';
						size--;
					}

				for(i = 0; i < len && size; ++i) {
					*str++ = *s++;
					size--;
				}

				while(len < field_width-- && size) {
					*str++ = ' ';
					size--;
				}

				continue;

			case 'p':
				if ( dotflag && precision < 0 )
					precision = 0;
				if(field_width == -1) {
					field_width = 2 * sizeof(void*);
					flags |= ZEROPAD;
				}
				str = number(str,
						(unsigned long)va_arg(args,void*),16,
						field_width, precision, flags, &size);
				continue;

			case 'n':
				if(qualifier == 'l') {
					long *ip = va_arg(args,long*);
					*ip = (str - buf);
				} else {
					int *ip = va_arg(args,int*);
					*ip = (str - buf);
				}
				continue;

			case 'o':
				base = 8;
				break;

			case 'X':
				flags |= LARGE;
			case 'x':
				base = 16;
				break;

			case 'd':
			case 'i':
				flags |= SIGN;
			case 'u':
				break;

			case 'f':
				{
					float	dval;
					int	ndigit, decpt, sign;
					char	cvtbuf[DECIMAL_STRING_LENGTH] = {'\0'};
					char	*cbp;

					/* Default FP precision */
					if ( dotflag && precision < 0 )
						precision = 6;
					/* Let's not press our luck too far */
					ndigit = precision < 16 ? precision : 16;

					dval = va_arg(args, double);

					cbp = fcvt(dval, ndigit, &decpt, &sign);
					sstrncpy(cvtbuf, cbp, sizeof cvtbuf);
					cbp = cvtbuf;

					/* XXX Ought to honor field_width, left/right justification */

					/* Result could be "NaN" or "Inf" */
					if ( ! isdigit((unsigned char)*cbp) ) {
						for ( i = 0 ; *cbp != '\0' && size > 0 ; i++ ) {
							*str++ = *cbp++; size--;
						}
						continue;
					}

					if ( size > 0 ) {
						if ( sign ) {
							*str++ = '-'; size--;
						}
						else if ( flags & PLUS ) {
							*str++ = '+'; size--;
						}
						else if ( flags & SPACE ) {
							*str++ = ' '; size--;
						}
					}

					/* Leading zeros, if needed */
					if ( decpt <= 0 && size > 0 ) {
						/* Prepend '0' */
						*str++ = '0'; size--;
					}
					if ( decpt < 0 && size > 0 ) {
						/* Prepend '.' */
						*str++ = '.'; size--;
						for ( i = decpt ; i < 0 && size > 0 ; i++ ) {
							*str++ = '0'; size--;
						}
					}
					/* Significant digits */
					for ( i = 0 ; size > 0 ; i++ ) {
						if ( i == decpt ) {
							if ( *cbp != '\0' ) {
								*str++ = '.'; size--;
								if ( size <= 0 )
									break;
							}
							else {
								/* Tack on "." or ".0"??? */
								break;
							}
						}
						if ( *cbp != '\0' ) {
							*str++ = *cbp++; size--;
						}
						else if ( i < decpt ) {
							*str++ = '0'; size--;
						}
						else {
							/* Tack on "." or ".0"??? */
							break;
						}
					}
				}
				continue;
				/* break; */

			default:
				if(*fmt != '%')
					*str++ = '%';
				if(*fmt && size)
				{ *str++ = *fmt; size--; }
				else
					--fmt;

				continue;
		}

		if(qualifier == 'l')
			num = va_arg(args,unsigned long);
		else if(qualifier == 'h') {
			if(flags & SIGN)
				//num = va_arg(args,short);
				num = va_arg(args,int);
			else
				//num = va_arg(args,unsigned short);
				num = va_arg(args,unsigned int);
		} else if(flags & SIGN)
			num = va_arg(args,int);
		else
			num = va_arg(args, unsigned int);

		if ( dotflag && precision < 0 )
			precision = 0;

		str = number(str,num,base,field_width,precision,flags,&size);
	}

	*str = '\0';
	return str - buf;
}
