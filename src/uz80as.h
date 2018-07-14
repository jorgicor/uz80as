/* ===========================================================================
 * uz80as, a macro assembler for Z80 based microprocessors.
 *
 * Assembler.
 * ===========================================================================
 */

#ifndef UZ80AS_H
#define UZ80AS_H

/* matchtab.flags */
enum {
	MATCH_F_UNDOC = 1,
	MATCH_F_EXTEN = 2,
};

/* pat:
 * 	a: expr
 * 	b - z: used bu target
 *
 * gen:
 * 	.: output lastbyte
 * 	b - z: used by target
 */

struct matchtab {
	const char *pat;
	const char *gen;
	unsigned char flags;
};

struct target {
	const char *id;
	const char *descr;
	const struct matchtab *matcht;
	int (*matchf)(char c, const char *p, const char **q);
	int (*genf)(int *eb, char p, const int *vs, int i, int savepc);
};

extern const char *s_pline_ep;

void genb(int b, const char *ep);
int mreg(const char *p, const char *const list[], const char **r);

void uz80as(void);

#endif
