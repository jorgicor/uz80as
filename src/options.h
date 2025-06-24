/* ===========================================================================
 * uz80as, an assembler for the Zilog Z80 and several other microprocessors.
 *
 * Global options, normally coming from the command line.
 * ===========================================================================
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#define OUTPUT_FORMAT_BINARY  0
#define OUTPUT_FORMAT_IHEX    1
#define OUTPUT_FORMAT_SREC    2

/* Predefined macro at the command line. */
struct predef {
	struct predef *next;
	const char *name;
};

extern int s_hex_output;       /* Output Intel HEX format */
extern char *s_hexfname;       /* HEX output filename */
extern int s_output_format;    /* Output format selection */
extern char* s_outfname;       /* Output filename */
extern const char *s_asmfname;
extern const char *s_objfname;
extern const char *s_lstfname;
extern const char *s_expfname;
extern const char *s_target_id;
extern int s_force_export;
extern int s_listing;
extern int s_extended_op;
extern int s_undocumented_op;
extern int s_mem_fillval;
extern struct predef *s_predefs;

void predefine(const char *name);

#endif
