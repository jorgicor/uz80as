/* ===========================================================================
 * Preprocessor.
 *
 * Copyright (C) 2023 Jorge Giner Cordero
 * This file is part of uz80as, an assembler for the Zilog Z80 and several
 * other microprocessors.
 * ===========================================================================
 */

#include <config.h>
#include "pp.h"
#include "utils.h"
#include "err.h"
#include "incl.h"
#include "expr.h"
#include "exprint.h"

#ifndef CTYPE_H
#include <ctype.h>
#endif

#ifndef STDIO_H
#include <stdio.h>
#endif

#ifndef STDLIB_H
#include <stdlib.h>
#endif

#ifndef STRING_H
#include <string.h>
#endif

/* Max number of macros. */
#define NMACROS		1000

/* Closest prime to NMACROS / 4. */
#define MACTABSZ	241

/* Max number of macro arguments. */
#define	NPARAMS		10

#define DEFINESTR	"DEFINE"
#define DEFCONTSTR	"DEFCONT"
#define INCLUDESTR	"INCLUDE"
#define IFSTR		"IF"
#define IFDEFSTR	"IFDEF"
#define IFNDEFSTR	"IFNDEF"
#define ENDIFSTR	"ENDIF"
#define ELSESTR		"ELSE"

/*
 * Macro.
 *
 * For example, the macro:
 *
 * #define SUM(a,b)	(a+b)
 *
 * is:
 *
 * name = SUM
 * pars = a\0b\0
 * ppars[0] points to &pars[0], that is to "a"
 * ppars[1] points to &pars[2], that is to "b"
 * npars = 2
 * text is "(a+b)"
 */
struct macro {
	struct macro *next;	/* Next in hash chain. */
	char *name;		/* Identifier. */
	char *pars;		/* String with params separated by '\0'. */
	char *text;		/* Text to expand. */
	char *ppars[NPARAMS];	/* Pointers to the beginning of each param. */
	int npars;		/* Valid number of params in ppars. */
};

/* Hash table of preprocessor symbols. */
static struct macro *s_mactab[MACTABSZ];

/* Preprocessing line buffers. */
static char s_ppbuf[2][LINESZ];

/* If we are discarding lines; if not 0, level of if. */
int s_skipon;

/* Number of nested #if or #ifdef or #ifndef. */
static int s_nifs;

/* Last defined macro. */
static struct macro *s_lastmac;

/* Number of macros in table. */
static int s_nmacs;

/* The preprocessed line, points to one of s_ppbuf. */
char *s_pline;

/* Pointer in s_pline for error reporting. */
const char *s_pline_ep;

/* Current program counter. */
int s_pc;

/* Current pass. */
int s_pass;

/* Only valid while in the call to pp_line(). */
static const char *s_line; 	/* original line */
static const char *s_line_ep;	/* pointer inside s_line for error reporting */

/*
 * Copy [p, q[ to [dp, dq[.
 */
static char *copypp(char *dp, char *dq, const char *p, const char *q)
{
	while (dp < dq && p < q)
		*dp++ = *p++;
	return dp;
}

/* Skip characters until endline. */
static const char *syncfull(const char *p)
{
	while (*p != '\0')
		p++;
	return p;
}

/*
 * Find the 'argnum' argument in 'args' and return a pointer to it.
 *
 * 'args' is a list of arguments "([id [,id]*).
 * 'argnum' is the argument number to find.
 *
 * Return not found.
 */
static const char *findarg(const char *args, int argnum)
{
	if (*args == '(') {
		do {
			args++;
			if (argnum == 0)
				return args;
			argnum--;
			while (*args != '\0' && *args != ','
				&& *args != ')')
			{
				args++;
			}
		} while (*args == ',');
	}
	return NULL;
}

/*
 * Find the 'argnum' argument in 'args' and copy it to [dp, dq[.
 *
 * 'args' points to a list of arguments "([id [,id]*).
 * 'argnum' is the argument number to copy.
 *
 * Return the new 'dp' after copying.
 */
static char *copyarg(char *dp, char *dq, const char *args, int argnum)
{
	const char *p;

	p = findarg(args, argnum);
	if (p == NULL)
		return dp;

	while (dp < dq && *p != '\0' && *p != ',' && *p != ')')
		*dp++ = *p++;
	return dp;
}

/*
 * Sees if [idp, idq[ is a parameter of the macro 'pps'.
 * If it is, return the number of parameter.
 * Else return -1.
 */
static int findparam(const char *idp, const char *idq, struct macro *pps)
{
	int i;
	const char *p, *r;

	for (i = 0; i < pps->npars; i++) {
		p = pps->ppars[i];
		r = idp;
		while (*p != '\0' && r < idq && *p == *r) {
			p++;
			r++;
		}
		if (*p == '\0' && r == idq)
			return i;
	}
	return -1;
}

/*
 * Lookup the string in [p, q[ in 's_mactab'.
 * Return the symbol or NULL if it is not in the table.
 */
static struct macro *pplookup(const char *p, const char *q)
{
	int h;
	struct macro *nod;

	h = hash(p, q, MACTABSZ);
	for (nod = s_mactab[h]; nod != NULL; nod = nod->next)
		if (scmp(p, q, nod->name) == 0)
			return nod;
	
	return nod;
}

static int isargc(char c)
{
	return (c == '_') || isalnum(c);
}

/*
 * Expand macro in [dp, dq[.
 *
 * 'pps' is the macro to expand.
 * 'args' points to the start of the arguments to substitute, if any.
 *
 * Return new dp.
 */
static char *expandid(char *dp, char *dq, struct macro *pps, const char *args)
{
	const char *p, *q;
	int argnum;

	p = pps->text;
	while (*p != '\0' && dp < dq) {
		if (isidc0(*p)) {
			for (q = p; isargc(*q); q++)
				;
			argnum = findparam(p, q, pps);
			if (argnum >= 0)
				dp = copyarg(dp, dq, args, argnum);
			else
				dp = copypp(dp, dq, p, q);
			p = q;
		} else {
			*dp++ = *p++;
		}
	}
	return dp;
}

/* 
 * If 'p' points the the start of an argument list, that is, '(',
 * point to one character past the first ')' after 'p'.
 * Else return 'p'.
 */
static const char *skipargs(const char *p)
{
	if (*p == '(') {
		while (*p != '\0' && *p != ')')
			p++;
		if (*p == ')')
			p++;
	}
	return p;
}

/*
 * Expand macros found in p (null terminated) into [dp, dq[.
 * dq must be writable to put a final '\0'.
 */
static int expand_line(char *dp, char *dq, const char *p, int expand)
{
	// char *op;
	int expanded, validid;
	const char *s;
	struct macro *nod;

	/*
	op = dp;
	while (dp < dq && isspace(*p)) {
		*dp++ = *p++;
	}
	expand = *p != '#';
	if (expand && s_skipon) {
		*op = '\0';
		return 0;
	}
	*/

	expanded = 0;
	validid = 1;
	while (dp < dq && *p != '\0' && *p != ';') {
		if (*p == '\'' && *(p + 1) != '\0' && *(p + 2) == '\'') {
			/* characters */
			dp = copypp(dp, dq, p, p + 3);
			p += 3;
			validid = 1;
		} else if (*p == '\"') {
			/* strings */
			s = p;
			p++;
			while (*p != '\0' && *p != '\"')
				p++;
			if (*p == '\"')
				p++;
			dp = copypp(dp, dq, s, p);
			validid = 1;
		} else if (isidc0(*p)) {
			s = p;
			while (isidc(*p))
				p++;
			if (validid && expand) {
				nod = pplookup(s, p);
				if (nod != NULL) {
					dp = expandid(dp, dq, nod, p);
					expanded = 1;
					p = skipargs(p);
				} else {
					dp = copypp(dp, dq, s, p);
				}
			} else {
				dp = copypp(dp, dq, s, p);
			}
			validid = 1;
		} else {
			validid = *p != '.' && *p != '#' && !isalnum(*p);
			*dp++ = *p++;
		}
	}
	*dp = '\0';
	return expanded;
}

/*
 * Expand macros found in 'p' (null terminated).
 * Return a pointer to an internal preprocessed line (null terminated).
 */
static char *expand_line0(const char *p)
{
	int iter, expanded, expand;
	const char *pp;
	char *np, *nq, *op;

	iter = 0;
	np = &s_ppbuf[iter & 1][0];
	nq = &s_ppbuf[iter & 1][LINESZ - 1];

	/* Dont expand if '# starts line. */
	pp = p;
	while (isspace(*pp))
		pp++;
	expand = *pp != '#';
	if (expand && s_skipon) {
		*np = '\0';
		return np;
	}

	expanded = expand_line(np, nq, p, expand);
	/* TODO: recursive macro expansion limit */
	while (expanded && iter < 5) {
		op = np;
		iter++;
		np = &s_ppbuf[iter & 1][0];
		nq = &s_ppbuf[iter & 1][LINESZ - 1];
		expanded = expand_line(np, nq, op, expand);
	}
	return np;
}

/* Expand all macros from s_line into s_pline and skips comments.
 * s_pline does not contain comments.
 */
void pp_expand(const char *line)
{
	s_line = line;
	s_line_ep = line;
	s_pline = expand_line0(line);
}

/*
 * Check if 'p' starts with the preprocessor directive 'ucq', that must be in
 * upper case.
 * 'p' can have any case.
 * After the preprocessor directive must be a space or '\0'.
 * Return 1 if all the above is true. 0 otherwise.
 */
static int isppid(const char *p, const char *ucq)
{
	while (*p != '\0' && *ucq != '\0' && toupper(*p) == *ucq) {
		p++;
		ucq++;
	}
	return (*ucq == '\0') && (*p == '\0' || *p == '\\' || isspace(*p));
}

/*
 * Define a macro.
 *
 * [idp, idq[ is the macro id.
 * [ap, aq[ is the macro argument list. If ap == aq there are no arguments.
 * [tp, tq[ is the macro text.
 */ 
static void define(const char *idp, const char *idq,
		   const char *tp, const char *tq,
		   const char *ap, const char *aq)
{
	static const char *zero = "0";

	int h;
	char *p;
	struct macro *nod;

	h = hash(idp, idq, MACTABSZ);
	for (nod = s_mactab[h]; nod != NULL; nod = nod->next) {
		if (scmp(idp, idq, nod->name) == 0) {
			/* Already defined. */
			return;
		}
	}

	s_nmacs++;
	if (s_nmacs >= NMACROS) {
		eprint(_("maximum number of macros exceeded (%d)\n"), NMACROS);
		exit(EXIT_FAILURE);
	}

	if (tp == tq && idp == idq) {
		/* Empty text on macro translates to '0'. */
		tp = zero;
		tq = tp + 1;
	}

	nod = emalloc((sizeof *nod) + (idq - idp) + (aq - ap) + 2);
	nod->text = emalloc(tq - tp + 1);

	nod->name = (char *) ((unsigned char *) nod + (sizeof *nod));
	nod->pars = nod->name + (idq - idp + 1);

	copychars(nod->name, idp, idq);
	copychars(nod->text, tp, tq);
	copychars(nod->pars, ap, aq);

	/* printf("DEF %s(%s) %s\n", nod->name, nod->pars, nod->text); */

	/* We don't check whether the arguments are different. */

	/*
	 * Make ppars point to each argument and null terminate each one.
	 * Count the number of arguments.
	 */
	nod->npars = 0;
	p = nod->pars;
	while (*p != '\0') {
		nod->ppars[nod->npars++] = p;
		while (*p != '\0' && *p != ',')
			p++;
		if (*p == ',')
			*p++ = '\0';
	}

	nod->next = s_mactab[h];
	s_mactab[h] = nod;
	s_lastmac = nod;
}

/* Add the text [p, q[ to the last macro text.  */
static void defcont(const char *p, const char *q)
{
	size_t len;

	len = strlen(s_lastmac->text);
	s_lastmac->text = erealloc(s_lastmac->text, (q - p) + len + 1);
	copychars(s_lastmac->text + len, p, q);
}

/* 
 * If 'p' points to a valid identifier start, go to the end of the identifier.
 * Else return 'p'.
 */
static const char *getid(const char *p)
{
	if (isidc0(*p)) {
		while (isidc(*p))
			p++;
	}
	return p;
}

/* Issues error in a macro definition. */
static void macdeferr(int cmdline, const char *estr, const char *ep)
{
	if (cmdline) {
		eprint(_("error in command line macro definition\n"));
	}
	eprint(estr);
	eprcol(s_line, ep);
	if (cmdline) {
		exit(EXIT_FAILURE);
	} else {
		newerr();
	}
}

/* Parse macro definition. */
const char *pmacdef(const char *p, int cmdline)
{
	const char *q, *ap, *aq, *idp, *idq, *end;

	idp = p;
	idq = getid(idp);
	if (idq == idp) {
		macdeferr(cmdline, _("identifier excepted\n"), p);
		return syncfull(p);
	}
	p = idq;
	ap = aq = p;
	if (*p == '(') {
		p++;
		ap = p;
		while (isidc0(*p)) {
			p = getid(p);
			if (*p != ',')
				break;
			p++;
		}
		if (*p != ')') {
			macdeferr(cmdline, _("')' expected\n"), p);
			return syncfull(p);
		}
		aq = p;
		p++;
	}
	if (*p != '\0' && !isspace(*p)) {
		macdeferr(cmdline, _("space expected\n"), p);
		return syncfull(p);
	}
	p = skipws(p);
	/* go to the end */
	for (q = p; *q != '\0'; q++)
		;
	end = q;
	/* go to the first non white from the end */
	while (q > p && isspace(*(q - 1)))
		q--;
	define(idp, idq, p, q, ap, aq);
	return end;
}

/* Parse #define. */
static const char *pdefine(const char *p)
{
	p = skipws(p + sizeof(DEFINESTR) - 1);
	return pmacdef(p, 0);
}

/* Parse #defcont. */
static const char *pdefcont(const char *p)
{
	const char *q, *end;

	p = skipws(p + sizeof(DEFCONTSTR) - 1);

	/* go to the end */
	for (q = p; *q != '\0'; q++)
		;

	end = q;

	/* go to the first non white from the end */
	while (q > p && isspace(*(q - 1)))
		q--;

	if (p == q) {
		/* nothing to add */
		return end;
	}

	if (s_lastmac == NULL) {
		eprint(_("#DEFCONT without a previous #DEFINE\n"));
		eprcol(s_pline, s_pline_ep);
		newerr();
		return end;
	}

	defcont(p, q);
	return end;
}

/* Parse #include. */
static void pinclude(const char *p)
{
	const char *q;

	p = skipws(p + sizeof(INCLUDESTR) - 1);
	if (*p != '\"') {
		eprint(_("#INCLUDE expects a filename between quotes\n"));
		eprcol(s_pline, p);
		newerr();
		return;
	}
	q = ++p;
	while (*q != '\0' && *q != '\"')
		q++;
	if (*q != '\"') {
		wprint(_("no terminating quote\n"));
		eprcol(s_pline, q);
	}
	pushfile(p, q);
}

/*
 * Parse #ifdef or #ifndef.
 * 'idsz' is the length of the string 'ifdef' or 'ifndef', plus '\0'.
 * 'ifdef' must be 1 if we are #ifdef, 0 if #ifndef.
 */
static const char *pifdef(const char *p, size_t idsz, int ifdef)
{
	const char *q;
	struct macro *nod;

	s_nifs++;
	p = skipws(p + idsz - 1);
	if (s_skipon) {
		while (*p != '\0' && *p != '\\')
			p++;
		return p;
	}

	if (!isidc0(*p)) {
		s_skipon = s_nifs;
		eprint(_("identifier expected\n"));
		eprcol(s_pline, p);
		newerr();
		return NULL;
	}

	q = p;
	while (isidc(*q))
		q++;

	nod = pplookup(p, q);
	if (ifdef == (nod != NULL))
		s_skipon = 0;
	else
		s_skipon = s_nifs;

	return q;
}

/* Parse #else. */
static const char *pelse(const char *p)
{
	if (s_nifs == 0) {
		eprint(_("unbalanced #ELSE\n"));
		eprcol(s_pline, s_pline_ep);
		newerr();
		return NULL;
	}

	if (s_skipon && s_nifs == s_skipon)
		s_skipon = 0;
	else if (!s_skipon)
		s_skipon = s_nifs;

	return p + sizeof(ELSESTR) - 1;
}

/* Parse #endif. */
static const char *pendif(const char *p)
{
	if (s_nifs == 0) {
		eprint(_("unbalanced #ENDIF\n"));
		eprcol(s_pline, s_pline_ep);
		newerr();
		return NULL;
	}

	if (s_skipon && s_nifs == s_skipon)
		s_skipon = 0;

	s_nifs--;
	return p + sizeof(ENDIFSTR) - 1;
}

/* Parse #if. */
static const char *pif(const char *p)
{
	int v;
	enum expr_ecode ex_ec;
	const char *ep;

	s_nifs++;
	p = skipws(p + sizeof(IFSTR) - 1);
	if (s_skipon) {
		while (*p != '\0' && *p != '\\')
			p++;
		return p;
	}

	p = expr(p, &v, s_pc, 0, &ex_ec, &ep);
	if (p == NULL) {
		v = 0;
		exprint(ex_ec, s_pline, ep);
		newerr();
	}

	if (v == 0)
		s_skipon = s_nifs;
	else
		s_skipon = 0;

	return p;
}

/*
 * Parse a # directive.
 * 'p' points to the next character after the '#'.
 */
const char *pp_parse_ppdirec(const char *p)
{
	if (isppid(p, IFDEFSTR)) {
		return pifdef(p, sizeof IFDEFSTR, 1);
	} else if (isppid(p, IFNDEFSTR)) {
		return pifdef(p, sizeof IFNDEFSTR, 0);
	} else if (isppid(p, IFSTR)) {
		pif(p);
	} else if (isppid(p, ELSESTR)) {
		return pelse(p);
	} else if (isppid(p, ENDIFSTR)) {
		return pendif(p);
	} else if (s_skipon) {
		return NULL;
	} else if (isppid(p, INCLUDESTR)) {
		pinclude(p);
	} else if (isppid(p, DEFINESTR)) {
		return pdefine(p);
	} else if (isppid(p, DEFCONTSTR)) {
		return pdefcont(p);
	} else {
		eprint(_("unknown preprocessor directive\n"));
		eprcol(s_pline, s_pline_ep);
		newerr();
	}
	return NULL;
}

/*
 * Preprocess 'line' in 's_pline'.
 * In this module, while we are preprocessing:
 * 	s_line is the original line.
 * 	s_line_ep is a pointer inside line that we keep for error reporting.
 */
/*
void pp_line(const char *line)
{
	const char *p;

	s_line = line;
	s_line_ep = line;

	p = skipws(line);
	if (*p == '#') {
		s_line_ep = p;
		parse_line(p + 1);
		s_ppbuf[0][0] = '\0';
		s_pline = &s_ppbuf[0][0];
	} else if (s_skipon) {
		s_ppbuf[0][0] = '\0';
		s_pline = &s_ppbuf[0][0];
	} else {
		s_pline = expand_line0(line);
	}
}
*/

/* Reset the module for other passes. */
void pp_reset(void)
{
	int i;
	struct macro *nod, *cur;

	s_nmacs = 0;
	s_nifs = 0;
	s_skipon = 0;
	s_lastmac = NULL;
	for (i = 0; i < MACTABSZ; i++) {
		nod = s_mactab[i];
		while (nod != NULL) {
			cur = nod;
			nod = nod->next;
			free(cur->text);
			free(cur);
		}
	}
	memset(s_mactab, 0, MACTABSZ * sizeof(s_mactab[0]));
}

void pp_predefine(const char *mactext)
{
	s_line = mactext;
	s_line_ep = mactext;
	pmacdef(mactext, 1);
	s_lastmac = NULL;
}
