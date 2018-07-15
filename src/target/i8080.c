/* ===========================================================================
 * uz80as, an assembler for the Zilog Z80 and several other microprocessors.
 *
 * Intel 8080.
 * ===========================================================================
 */

#include "pp.h"
#include "err.h"
#include "options.h"
#include "uz80as.h"
#include <stddef.h>

/* pat:
 *	a: expr
 *	b: B,C,D,E,H,L,M,A
 *	c: B,D,H,SP
 *	d: B,D
 *	e: B,D,H,PSW
 *	f: JNZ,JZ,JNC,JC,JPO,JPE,JP,JM
 *	g: CNZ,CZ,CNC,CC,CPO,CPE,CP,CM
 *	h: RNZ,RZ,RNC,RC,RPO,RPE,RP,RM
 *	i: B,C,D,E,H,L,A
 *	j: ADD,ADC,SUB,SBB,ANA,XRA,ORA,CMP
 *	k: RLC,RRC,RAL,RAR
 *	l: ADI,ACI,SUI,SBI,ANI,XRI,ORI,CPI
 *	m: SHLD,LHLD,STA,LDA
 *	n: DI,EI
 *	o: OUT,IN
 *	p: STC,CMC
 *	q: POP,PUSH
 *	r: STAX,LDAX
 *
 * gen:
 * 	.: output lastbyte
 * 	b: (op << 3) | lastbyte
 * 	c: op | lastbyte
 * 	d: lastbyte = op as 8 bit value
 * 	e: output op as word (no '.' sould follow)
 * 	f: (op << 4) | lastbyte
 * 	g: possible value to RST
 * 	h: (op << 2) | lastbyte
 */

const struct matchtab s_matchtab_i8080[] = {
	{ "MOV M,i", "70c0." },
	{ "MOV i,M", "46b0." },
	{ "MOV i,i", "40b0c1." },
	{ "MVI b,a", "06b0.d1." },
	{ "LXI c,a", "01f0.e1" },
	{ "m a", "22b0.e1" },
	{ "r d", "02b0f1." },
	{ "XCHG", "EB." },
	{ "j b", "80b0c1." },
	{ "l a", "C6b0.d0." },
	{ "INR b", "04b0." },
	{ "DCR b", "05b0." },
	{ "INX c", "03f0." },
	{ "DCX c", "0Bf0." },
	{ "DAD c", "09f0." },
	{ "DAA", "27." },
	{ "k", "07b0." },
	{ "CMA", "2F." },
	{ "p", "37b0." },
	{ "JMP a", "C3.e0" },
	{ "f a", "C2b0.e1" },
	{ "CALL a", "CD.e0" },
	{ "g a", "C4b0.e1" },
	{ "RET", "C9." },
	{ "h", "C0b0." },
	{ "RST a", "C7g0." },
	{ "PCHL", "E9." },
	{ "q e", "C1h0f1." },
	{ "XTHL", "E3." },
	{ "SPHL", "F9." },
	{ "o a", "D3b0.d0." },
	{ "n", "F3b0." },
	{ "HLT", "76." },
	{ "NOP", "00."},
	{ NULL, NULL },
};

static const char *const bval[] = { "B", "C", "D", "E",
				    "H", "L", "M", "A", NULL };
static const char *const cval[] = { "B", "D", "H", "SP", NULL };
static const char *const dval[] = { "B", "D", NULL };
static const char *const eval[] = { "B", "D", "H", "PSW", NULL };
static const char *const fval[] = { "JNZ", "JZ", "JNC", "JC",
				    "JPO", "JPE", "JP", "JM", NULL };
static const char *const gval[] = { "CNZ", "CZ", "CNC", "CC",
				    "CPO", "CPE", "CP", "CM", NULL };
static const char *const hval[] = { "RNZ", "RZ", "RNC", "RC",
				    "RPO", "RPE", "RP", "RM", NULL };
static const char *const ival[] = { "B", "C", "D", "E",
				    "H", "L", "", "A", NULL };
static const char *const jval[] = { "ADD", "ADC", "SUB", "SBB",
				    "ANA", "XRA", "ORA", "CMP", NULL };
static const char *const kval[] = { "RLC", "RRC", "RAL", "RAR", NULL };
static const char *const lval[] = { "ADI", "ACI", "SUI", "SBI",
				    "ANI", "XRI", "ORI", "CPI", NULL };
static const char *const mval[] = { "SHLD", "LHLD", "STA", "LDA", NULL };
static const char *const nval[] = { "DI", "EI", NULL };
static const char *const oval[] = { "OUT", "IN", NULL };
static const char *const pval[] = { "STC", "CMC", NULL };
static const char *const qval[] = { "POP", "PUSH", NULL };
static const char *const rval[] = { "STAX", "LDAX", NULL };

static const char *const *const valtab[] = { 
	bval, cval, dval, eval, fval,
       	gval, hval, ival, jval, kval,
	lval, mval, nval, oval, pval,
	qval, rval
};

static int match_i8080(char c, const char *p, const char **q)
{
	int v;

	if (c <= 'r') {
		v = mreg(p, valtab[(int) (c - 'b')], q);
	} else {
		v = -1;
	}

	return v;
}

static int gen_i8080(int *eb, char p, const int *vs, int i, int savepc)
{
	int b;
       
	b = *eb;
	switch (p) {
	case 'f': b |= (vs[i] << 4); break;
	case 'g': if (s_pass > 0 && (vs[i] & ~7) != 0) {
			  eprint(_("invalid RST argument (%d)\n"),
				vs[i]);
			  eprcol(s_pline, s_pline_ep);
			  newerr();
		  }
		  b |= (vs[i] << 3);
		  break;
	case 'h': b |= (vs[i] << 2); break;
	default:
		  return -1;
	}

	*eb = b;
	return 0;
}

static int s_pat_char = 'b';
static int s_pat_index;

static void pat_char_rewind_i8080(int c)
{
	s_pat_char = c;
	s_pat_index = 0;
};

static const char *pat_next_str_i8080(void)
{
	const char *s;

	if (s_pat_char >= 'b' && s_pat_char <= 'r') {
		s = valtab[(int) (s_pat_char - 'b')][s_pat_index];
		if (s != NULL) {
			s_pat_index++;
		}
	} else {
		s = NULL;
	}

	return s;
};

const struct target s_target_i8080 = {
	"i8080",
	"Intel 8080",
	s_matchtab_i8080,
	match_i8080,
	gen_i8080,
	pat_char_rewind_i8080,
	pat_next_str_i8080
};
