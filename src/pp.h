/* ===========================================================================
 * uz80as, an assembler for the Zilog Z80 and several other microprocessors.
 *
 * Preprocessor.
 * ===========================================================================
 */

#ifndef PP_H
#define PP_H

/* Max line length after macro expansion + '\0'. */
#define LINESZ		512

extern char *s_pline;
extern const char *s_pline_ep;
extern int s_pc;
extern int s_pass;
extern int s_skipon;

/* void pp_line(const char *line); */

void pp_reset(void);
void pp_predefine(const char *p);
void pp_expand(const char *line);
const char *pp_parse_ppdirec(const char *p);

#endif
