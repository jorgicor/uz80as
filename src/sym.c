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

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "err.h"
#include "sym.h"

/*
 * Maximum number of symbols (labels) allowed.
 * Must not be more than 64K.
 */
#define NSYMS		15000

/* Closest prime to NSYMS / 4. */
#define SYMTABSZ	3739

/*
 * Nodes for the s_symtab hash table.
 * The symbol at index 0 is never used.
 */
static struct sym s_symlist[NSYMS];

/*
 * Hash table of indexes into s_symlist.
 * 0 means that the bucket is empty.
 */
static unsigned short s_symtab[SYMTABSZ];

/* Next free symbol in s_symlist. Note: 0 not used. */
static int s_nsyms = 1;

/*
 * Lookups the string in [p, q[ in s_symtab.
 * If !insert, returns the symbol or NULL if it is not in the table.
 * If insert, inserts the symbol in the table if it is not there, and
 * sets its .val to 'pc'.
 */
struct sym *lookup(const char *p, const char *q, int insert, int pc)
{
	int h, k;
	struct sym *nod;

	if (q - p > SYMLEN - 1) {
		/* Label too long, don't add. */
		eprint(_("label too long"));
		epchars(p, q);
		enl();
		newerr();
		/*
		 * This would truncate:
		 * q = p + (SYMLEN - 1);
		 */
		return NULL;
	}

	h = hash(p, q, SYMTABSZ);
	for (k = s_symtab[h]; k != 0; k = s_symlist[k].next) {
		if (scmp(p, q, s_symlist[k].name) == 0) {
			if (insert) {
				wprint("duplicate label (%s)\n",
					s_symlist[k].name);
			}
			return &s_symlist[k];
		}
	}

	if (insert) {
		if (s_nsyms == NSYMS) {
			eprint(_("maximum number of labels exceeded (%d)\n"),
				NSYMS);
			exit(EXIT_FAILURE);
		}

		nod = &s_symlist[s_nsyms];
		nod->next = s_symtab[h];
		s_symtab[h] = (unsigned short) s_nsyms;
		s_nsyms++;

		k = 0;
		while (p != q && k < SYMLEN - 1)
			nod->name[k++] = *p++;
		nod->name[k] = '\0';
		nod->val = pc;
		return nod;
	}

	return NULL;
}
