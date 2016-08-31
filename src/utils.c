/*
Copyright (c) 2016 Jorge Giner Cordero

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <ctype.h>
#include <limits.h>
#include "utils.h"

/*
 * Copy [p, q[ to dst and null terminate dst.
 */
void copychars(char *dst, const char *p, const char *q)
{
	while (p != q)
		*dst++ = *p++;
	*dst = '\0';
}

/* Skip space. */
const char *skipws(const char *p)
{
	while (isspace(*p))
		p++;
	return p;
}

/* Return 1 if *p is a valid start character for an identifier. */
int isidc0(char c)
{
	return (c == '_') || isalpha(c);
}

/* 
 * Return 1 if *p is a valid character for an identifier.
 * Don't use for the first character.
 */
int isidc(char c)
{
	return (c == '_') || (c == '.') || isalnum(c);
}

/* Hash the string in [p, q[ to give a bucket in symtab. */
int hash(const char *p, const char *q, unsigned int tabsz)
{
	unsigned int h;

	h = 0;
	while (p != q) {
		h = 31 * h + (unsigned char) *p;
		p++;
	}

	return h % tabsz;
}

/* 
 * Compare the string in [p, q[ with the null-terminated string s.
 * Return 0 if equal.
 */
int scmp(const char *p, const char *q, const char *s)
{
	while (p < q) {
		if (*p == *s) {
			p++;
			s++;
		} else if (*s == '\0') {
			return 1;
		} else if (*p < *s) {
			return -1;
		} else {
			return 1;
		}
	}

	if (*s == '\0')
		return 0;
	else
		return -1;
}
/*
 * Given a hexadecimal character (in upper case), returns its integer value.
 * Returns -1 if c is not a hexadecimal character.
 */
int hexvalu(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'A' && c <= 'F')
		return (c - 'A') + 10;
	else
		return -1;
}

/*
 * Given a hexadecimal character, returns its integer value.
 * Returns -1 if c is not a hexadecimal character.
 */
int hexval(char c)
{
	if (c >= 'a' && c <= 'f')
		return (c - 'a') + 10;
	else
		return hexvalu(c);
}

int int_precission(void)
{
	static int bits = 0;
	unsigned int i;

	if (bits > 0)
		return bits;

	i = INT_MAX;
	bits = 0;
	while (i) {
		bits++;
		i >>= 1;
	}
	return bits;
}
