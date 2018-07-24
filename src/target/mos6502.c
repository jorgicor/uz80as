/* ===========================================================================
 * uz80as, an assembler for the Zilog Z80 and several other microprocessors.
 *
 * MOS Technology 6502.
 * ===========================================================================
 */

#include "pp.h"
#include "err.h"
#include "options.h"
#include "uz80as.h"
#include <stddef.h>

/* pat:
 *	a: expr
 *	b: ORA,AND,EOR,ADC,LDA,CMP,SBC
 *	c: ORA,AND,EOR,ADC,STA,LDA,CMP,SBC
 *	d: PHP,CLC,PLP,SEC,PHA,CLI,PLA,SEI,
 *	   DEY,TYA,TAY,CLV,INY,CLD,INX,SED
 *	e: ASL,ROL,LSR,ROR
 *	f: DEC, INC
 *	g: BPL,BMI,BVC,BVS,BCC,BCS,BNE,BEQ
 *	h: TXA,TXS,TAX,TSX,DEX,NOP
 *	i: CPY,CPX
 *
 * gen:
 * 	.: output lastbyte
 * 	b: (op << 3) | lastbyte
 * 	c: op | lastbyte
 * 	d: lastbyte = op as 8 bit value
 * 	e: output op as word (no '.' should follow)
 * 	f: (op << 5) | lastbyte
 * 	g: if op <= $FF output last byte and then op as 8 bit value;
 * 	   else output (lastbyte | 0x08) and output op as word
 * 	   (no '.' should follow)
 * 	h: (op << 4) | lastbyte
 * 	i: relative jump to op
 */

const struct matchtab s_matchtab_mos6502[] = {
	{ "BRK", "00.", 1, 0 },
	{ "JSR a", "20.e0", 1, 0 },
	{ "RTI", "40.", 1, 0 },
	{ "RTS", "60.", 1, 0 },
	{ "h", "8Ah0.", 1, 0 },
	{ "d", "08h0.", 1, 0 },
	{ "c #a", "09f0.d1.", 1, 0 },
	{ "b (a,X)", "01f0.d1.", 1, 0 },
	{ "b (a),Y", "11f0.d1.", 1, 0 },
	{ "b a", "05f0g1", 1, 0 },
	{ "b a,X", "15f0g1", 1, 0 },
	{ "b a,Y", "19f0.e1", 1, 0 },
	{ "e A", "0Af0.", 1, 0 },
	{ "e a", "06f0g1", 1, 0 },
	{ "e a,X", "16f0g1", 1, 0 },
	{ "STX a", "86g0", 1, 0 },
	{ "STX a,Y", "96.d0.", 1, 0 },
	{ "LDX #a", "A2.d0.", 1, 0 },
	{ "LDX a", "A6g0", 1, 0 },
	{ "LDX a,Y", "B6g0", 1, 0 },
	{ "f a", "C6f0g1", 1, 0 },
	{ "f a,X", "D6f0g1", 1, 0 },
	{ "g a", "10f0.i1.", 1, 0 },
	{ "BIT a", "24g0", 1, 0 },
	{ "JMP (a)", "6C.e0", 1, 0 },
	{ "JMP a", "4C.e0", 1, 0 },
	{ "STY a", "84g0", 1, 0 },
	{ "STY a,X", "94.d0.", 1, 0 },
	{ "LDY #a", "A0.d0.", 1, 0 },
	{ "LDY a", "A4g0", 1, 0 },
	{ "LDY a,X", "B4g0", 1, 0 },
	{ "i #a", "C0f0.d1.", 1, 0 },
	{ "i a", "C4f0g1", 1, 0 },
	{ NULL, NULL },
};

static const char *const bval[] = {
	"ORA", "AND", "EOR", "ADC",
	"STA", "LDA", "CMP", "SBC",
       	NULL
};

static const char *const cval[] = {
	"ORA", "AND", "EOR", "ADC",
	"", "LDA", "CMP", "SBC", NULL
};

static const char *const dval[] = {
       	"PHP", "CLC", "PLP", "SEC",
	"PHA", "CLI", "PLA", "SEI",
	"DEY", "TYA", "TAY", "CLV",
	"INY", "CLD", "INX", "SED",
	NULL
};


static const char *const eval[] = {
	"ASL", "ROL", "LSR", "ROR",
	NULL
};

static const char *const fval[] = {
	"DEC", "INC",
	NULL
};

static const char *const gval[] = {
	"BPL", "BMI", "BVC", "BVS",
	"BCC", "BCS", "BNE", "BEQ",
	NULL
};

static const char *const hval[] = {
	"TXA", "TXS", "TAX", "TSX",
	"DEX", "", "NOP",
	NULL
};

static const char *const ival[] = {
	"CPY", "CPX",
	NULL
};

static const char *const *const valtab[] = { 
	bval, cval, dval, eval, fval,
	gval, hval, ival
};

static int match_mos6502(char c, const char *p, const char **q)
{
	int v;

	if (c <= 'i') {
		v = mreg(p, valtab[(int) (c - 'b')], q);
	} else {
		v = -1;
	}

	return v;
}

static int gen_mos6502(int *eb, char p, const int *vs, int i, int savepc)
{
	int b, w;
       
	b = *eb;
	switch (p) {
	case 'f': b |= (vs[i] << 5); break;
	case 'g': w = vs[i] & 0xffff;
		  if (w <= 0xff) {
			genb(b, s_pline_ep);
			b = 0;
			genb(w & 0xff, s_pline_ep);
		  } else {
			b |= 0x08; 
			genb(b, s_pline_ep);
			b = 0;
			genb(w & 0xff, s_pline_ep);
			genb(w >> 8, s_pline_ep);
		  }
		  break;
	case 'h': b |= (vs[i] << 4); break;
	case 'i': b = (vs[i] - savepc - 2); break;
	default:
		  return -1;
	}

	*eb = b;
	return 0;
}

static int s_pat_char = 'b';
static int s_pat_index;

static void pat_char_rewind_mos6502(int c)
{
	s_pat_char = c;
	s_pat_index = 0;
};

static const char *pat_next_str_mos6502(void)
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

const struct target s_target_mos6502 = {
	.id = "mos6502",
	.descr = "MOS Technology 6502",
	.matcht = s_matchtab_mos6502,
	.matchf = match_mos6502,
	.genf = gen_mos6502,
	.pat_char_rewind = pat_char_rewind_mos6502,
	.pat_next_str = pat_next_str_mos6502,
	.mask = 1
};
