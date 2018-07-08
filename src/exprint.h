/* ===========================================================================
 * uz80as, a macro assembler for Z80 based microprocessors.
 *
 * Expression error reporting.
 * ===========================================================================
 */

#ifndef EXPRINT_H
#define EXPRINT_H

#ifndef EXPR_H
#include "expr.h"
#endif

void exprint(enum expr_ecode ecode, const char *pline, const char *ep);

#endif
