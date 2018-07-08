/* ===========================================================================
 * uz80as, a macro assembler for Z80 based microprocessors.
 *
 * Include file stack.
 * ===========================================================================
 */

#ifndef INCL_H
#define INCL_H

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

struct incfile {
	struct incfile *prev;
	FILE *fin;
	int linenum;
	char *name;
};

void pushfile(const char *p, const char *q);
void popfile(void);
struct incfile *curfile(void);
int nfiles(void);

#endif
