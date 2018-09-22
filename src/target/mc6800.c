/* ===========================================================================
 * uz80as, an assembler for the Zilog Z80 and several other microprocessors.
 *
 * Motorola 6800.
 * ===========================================================================
 */

#include "pp.h"
#include "err.h"
#include "options.h"
#include "uz80as.h"
#include <stddef.h>

/* pat:
 *	a: expr
 * 	b: NEGA,COMA,LSRA,RORA,ASRA,
 *	   ROLA,DECA,INCA,TSTA,CLRA
 * 	c: NEGB,COMB,LSRB,RORB,ASRB,
 *	   ROLB,DECB,INCB,TSTB,CLRB
 * 	d: NEG,COM,LSR,ROR,ASR,
 *	   ROL,DEC,INC,TST,JMP,CLR
 *	e: SUBA,CMPA,SBCA,ANDA,BITA,LDAA,
 * 	   EORA,ADCA,ORAA,ADDA
 *	f: SUBB,CMPB,SBCB,ANDB,BITB,LDAB,
 * 	   EORB,ADCB,ORAB,ADDB
 * 	g: INX,DEX,CLV,SEV,CLC,SEC,CLI,SEI
 *      h: BRA,BHI,BLS,BCC,BCS,BNE,BEQ,
 *	   BVC,BVS,BPL,BMI,BGE,BLT,BGT,BLE
 *	i: TSX,INS,PULA,PULB,DES,TXS,PSHA,
 *	   PSHB,RTS,RTI,WAI,SWI
 *
 * gen:
 * 	.: output lastbyte
 * 	b: (op << 3) | lastbyte
 * 	c: op | lastbyte
 * 	d: lastbyte = op as 8 bit value
 * 	e: output op as word (no '.' should follow)
 * 	f: ouput op as big endian word (no '.' should follow)
 * 	g: if op<=$ff output lastbyte and output op as byte
 * 	   else output (lastbyte | 0x20) and output op as big endian word
 * 	   (no '.' should follow)
 * 	h: relative - 2
 */

static const struct matchtab s_matchtab_mc6800[] = {
	{ "NOP", "01.", 1, 0 },
	{ "TAP", "06.", 1, 0 },
	{ "TPA", "07.", 1, 0 },
	{ "g", "08c0.", 1, 0 },
	{ "SBA", "10.", 1, 0 },
	{ "CBA", "11.", 1, 0 },
	{ "TAB", "16.", 1, 0 },
	{ "TBA", "17.", 1, 0 },
	{ "DAA", "19.", 1, 0 },
	{ "ABA", "1B.", 1, 0 },
	{ "i", "30c0.", 1, 0 }, 
	{ "h a", "20c0.h1.", 1, 0, "r8" },
	{ "b", "40c0.", 1, 0 },
	{ "c", "50c0.", 1, 0 },
	{ "d a,X", "60c0.d1.", 1, 0, "e8" },
	{ "d a", "70c0.f1", 1, 0, "e16" },
	{ "e #a", "80c0.d1.", 1, 0, "e8" },
	{ "f #a", "C0c0.d1.", 1, 0, "e8" },
	{ "e >a", "B0c0.f1", 1, 0 },
	{ "f >a", "F0c0.f1", 1, 0 },
	{ "e a,X", "A0c0.d1.", 1, 0, "e8" },
	{ "f a,X", "E0c0.d1.", 1, 0, "e8" },
	{ "e a", "90c0g1", 1, 0 },
	{ "f a", "D0c0g1", 1, 0 },
	{ "STAA >a", "B7.f0", 1, 0 },
	{ "STAA a,X", "A7.d0.", 1, 0, "e8" },
	{ "STAA a", "97g0", 1, 0 },
	{ "STAB >a", "F7.f0", 1, 0 },
	{ "STAB a,X", "E7.d0.", 1, 0, "e8" },
	{ "STAB a", "D7g0", 1, 0 },
	{ "CPX #a", "8C.f0", 1, 0, "e16" },
	{ "CPX >a", "BC.f0", 1, 0 },
	{ "CPX a,X", "AC.d0.", 1, 0, "e8" },
	{ "CPX a", "9Cg0", 1, 0 },
	{ "LDS #a", "8E.f0", 1, 0, "e16" },
	{ "LDS >a", "BE.f0", 1, 0 },
	{ "LDS a,X", "AE.d0.", 1, 0, "e8" },
	{ "LDS a", "9Eg0", 1, 0 },
	{ "STS >a", "BF.f0", 1, 0 },
	{ "STS a,X", "AF.d0.", 1, 0, "e8" },
	{ "STS a", "9Fg0", 1, 0 },
	{ "LDX #a", "CE.f0", 1, 0, "e16" },
	{ "LDX >a", "FE.f0", 1, 0 },
	{ "LDX a,X", "EE.d0.", 1, 0, "e8" },
	{ "LDX a", "DEg0", 1, 0 },
	{ "STX >a", "FF.f0", 1, 0 },
	{ "STX a,X", "EF.d0.", 1, 0, "e8" },
	{ "STX a", "DFg0", 1, 0 },
	{ "BSR a", "8D.h0.", 1, 0, "r8" },
	{ "JSR a,X", "AD.d0.", 1, 0, "e8" },
	{ "JSR a", "BD.f0", 1, 0, "e16" },
	{ NULL, NULL },
};

static const char *const bval[] = {
"NEGA", "", "", "COMA", "LSRA", "", "RORA", "ASRA",
"ASLA", "ROLA", "DECA", "", "INCA", "TSTA", "", "CLRA",
NULL };

static const char *const cval[] = {
"NEGB", "", "", "COMB", "LSRB", "", "RORB", "ASRB",
"ASLB", "ROLB", "DECB", "", "INCB", "TSTB", "", "CLRB",
NULL };

static const char *const dval[] = {
"NEG", "", "", "COM", "LSR", "", "ROR", "ASR",
"ASL", "ROL", "DEC", "", "INC", "TST", "JMP", "CLR",
NULL };

static const char *const eval[] = {
"SUBA", "CMPA", "SBCA", "", "ANDA", "BITA", "LDAA", "",
"EORA", "ADCA", "ORAA", "ADDA",
NULL };

static const char *const fval[] = {
"SUBB", "CMPB", "SBCB", "", "ANDB", "BITB", "LDAB", "",
"EORB", "ADCB", "ORAB", "ADDB",
NULL };

static const char *const gval[] = {
"INX", "DEX", "CLV", "SEV", "CLC", "SEC", "CLI", "SEI",
NULL };

static const char *const hval[] = {
"BRA", "", "BHI", "BLS", "BCC", "BCS", "BNE", "BEQ",
"BVC", "BVS", "BPL", "BMI", "BGE", "BLT", "BGT", "BLE",
NULL };

static const char *const ival[] = {
"TSX", "INS", "PULA", "PULB", "DES", "TXS", "PSHA",
"PSHB", "", "RTS", "", "RTI", "", "", "WAI", "SWI",
NULL };

static const char *const *const valtab[] = { 
	bval, cval, dval, eval, fval,
       	gval, hval, ival
};

static int match_mc6800(char c, const char *p, const char **q)
{
	int v;

	if (c <= 'i') {
		v = mreg(p, valtab[(int) (c - 'b')], q);
	} else {
		v = -1;
	}

	return v;
}

static int gen_mc6800(int *eb, char p, const int *vs, int i, int savepc)
{
	int b;
       
	b = *eb;
	switch (p) {
	case 'f': genb(vs[i] >> 8, s_pline_ep);
		  genb(vs[i], s_pline_ep);
		  break;
	case 'g': if (vs[i] <= 255) {
			  genb(b, s_pline_ep);
			  genb(vs[i], s_pline_ep);
		  } else {
			  genb(b | 0x20, s_pline_ep);
			  genb(vs[i] >> 8, s_pline_ep);
			  genb(vs[i], s_pline_ep);
		  }
		  break;
	case 'h': b = (vs[i] - savepc - 2);
		  break;
	default:
		  return -1;
	}

	*eb = b;
	return 0;
}

static int s_pat_char = 'b';
static int s_pat_index;

static void pat_char_rewind_mc6800(int c)
{
	s_pat_char = c;
	s_pat_index = 0;
};

static const char *pat_next_str_mc6800(void)
{
	const char *s;

	if (s_pat_char >= 'b' && s_pat_char <= 'i') {
		s = valtab[(int) (s_pat_char - 'b')][s_pat_index];
		if (s != NULL) {
			s_pat_index++;
		}
	} else {
		s = NULL;
	}

	return s;
};

const struct target s_target_mc6800 = {
	.id = "mc6800",
	.descr = "Motorola 6800",
	.matcht = s_matchtab_mc6800,
	.matchf = match_mc6800,
	.genf = gen_mc6800,
	.pat_char_rewind = pat_char_rewind_mc6800,
	.pat_next_str = pat_next_str_mc6800,
	.mask = 1
};

