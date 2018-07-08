/* ===========================================================================
 * uz80as, a macro assembler for Z80 based microprocessors.
 *
 * Symbol table for labels.
 * ===========================================================================
 */

#ifndef SYM_H
#define SYM_H

/* Max symbol length + '\0'. */
#define SYMLEN		32

struct sym {
	char name[SYMLEN];		/* null terminated string */
	int val;			/* value of symbol */
	unsigned short next;		/* index into symlist; 0 is no next */
	unsigned char isequ;		/* if val comes from EQU */
};

struct sym *lookup(const char *p, const char *q, int insert, int pc);

#endif
