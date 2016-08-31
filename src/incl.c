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

/* Includes, file stack */

#include <assert.h>
// #include <errno.h>
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include "utils.h"
#include "err.h"
#include "incl.h"

/* Max number of nested included files. */
#define NFILES		128

/* Number of nested files. */
static int s_nfiles;

/* Current file. */
static struct incfile *s_curfile;

/* Get the current file. Never returns NULL. */
struct incfile *curfile(void)
{
	assert(s_curfile != NULL);
	return s_curfile;
}

/* The number of nested files. 0 means no file loaded. */
int nfiles(void)
{
	return s_nfiles;
}

/* Leave the current included file. */
void popfile(void)
{
	struct incfile *ifile;

	assert(s_curfile != NULL);
	fclose(s_curfile->fin);
	ifile = s_curfile;
	s_curfile = ifile->prev;
	free(ifile);
	s_nfiles--;
}

/* Include a file whose name is [p, q[. */
void pushfile(const char *p, const char *q)
{
	struct incfile *ifile;

	if (s_nfiles == NFILES) {
		eprint(_("maximum number of nested includes exceeded (%d)\n"),
			NFILES);
		exit(EXIT_FAILURE);
	}

	ifile = emalloc((sizeof *ifile) + (q - p) + 1);
	ifile->name = (char *) ((unsigned char *) ifile + sizeof *ifile);
	copychars(ifile->name, p, q);

	ifile->fin = efopen(ifile->name, "r");
	ifile->linenum = 0;
	ifile->prev = s_curfile;
	s_curfile = ifile;
	s_nfiles++;
}
