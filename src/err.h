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

#ifndef ERR_H
#define ERR_H

#ifndef STDDEF_H
#define STDDEF_H
#include <stddef.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#define _(str)	(str)

extern int s_nerrors;

void newerr(void);
void eprogname(void);
void echars(const char *p, const char *q);
void epchars(const char *p, const char *q);
void eprint(const char *ecode, ...);
void wprint(const char *ecode, ...);
void eprcol(const char *line, const char *q);
void enl(void);
void *emalloc(size_t n);
void *erealloc(void *p, size_t n);
FILE *efopen(const char *fname, const char *ops);

#endif
