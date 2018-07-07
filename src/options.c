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

#include <stddef.h>
#include <stdio.h>
#include "err.h"
#include "options.h"

const char *s_asmfname;	/* Name of source file. */
const char *s_objfname;	/* Name of generated binary file. */
const char *s_lstfname;	/* Name of listing file. */
const char *s_cpuname = "z80"; /* Name of cpu */
int s_listing = 1;	/* If we generate the listing file or not. */
int s_extended_iset = 0;	/* Allow extended instruction set. */
int s_mem_fillval = 0;	/* Default value to fill the 64K memory. */

/* Command line macro definitions. */
struct predef *s_predefs;

/* Predefine a macro in the command line that must persist between passes. */
void predefine(const char *text)
{
	struct predef *pdef;

	pdef = emalloc(sizeof(*pdef));
	pdef->name = text;
	pdef->next = s_predefs;
	s_predefs = pdef;
}
