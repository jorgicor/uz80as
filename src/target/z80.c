/* ===========================================================================
 * uz80as, an assembler for the Zilog Z80 and several other microprocessors.
 *
 * Zilog Z80 CPU.
 * ===========================================================================
 */

#include "pp.h"
#include "err.h"
#include "options.h"
#include "uz80as.h"
#include <stddef.h>

/* pat:
 * 	a: expr
 * 	b: B,C,D,E,H,L,A
 * 	c: IX,IY (must be followed by + or -)
 * 	d: BC,DE,HL,SP
 * 	e: IX,IY
 * 	f: BC,DE,HL,AF
 * 	g: ADD,ADC,SUB,SBC,AND,XOR,OR,CP
 * 	h: INC,DEC
 * 	i: BC,DE,IX,SP
 * 	j: BC,DE,IY,SP
 * 	k: RLC,RRC,RL,RR,SLA,SRA,SRL
 * 	l: BIT,RES,SET
 * 	m: NZ,Z,NC,C,PO,PE,P,M
 * 	n: NZ,Z,NC,C
 * 	o: *
 *      p: B,C,D,E,IXH,IXL,A
 *      q: B,C,D,E,IYH,IYL,A
 *
 * gen:
 * 	.: output lastbyte
 * 	b: (op << 3) | lastbyte
 * 	c: op | lastbyte
 * 	d: lastbyte = op as 8 bit value
 * 	e: output op as word (no '.' should follow)
 * 	f: (op << 4) | lastbyte
 * 	g: (op << 6) | lastbyte
 * 	h: *
 * 	i: relative jump to op
 * 	j: possible value to RST
 * 	k: possible value to IM
 * 	m: check arithmetic used with A register
 * 	n: check arithmetic used without A register
 */

static const struct matchtab s_matchtab_z80[] = {
	{ "LD b,b", "40b0c1.", 3, 0 },
	{ "LD p,p", "DD.40b0c1.", 1, 1 },
	{ "LD q,q", "FD.40b0c1.", 1, 1 },
	{ "LD b,(HL)", "46b0.", 3, 0 },
	{ "LD b,(ca)", "d1.46b0.d2.", 3, 0, "ii" },
	{ "LD A,I", "ED.57.", 3, 0 },
	{ "LD A,R", "ED.5F.", 3, 0 },
	{ "LD A,(BC)", "0A.", 3, 0 },
	{ "LD A,(DE)", "1A.", 3, 0 },
	{ "LD A,(a)", "3A.e0", 3, 0 },
	{ "LD b,a", "06b0.d1.", 3, 0, "e8" },
	{ "LD p,a", "DD.06b0.d1.", 1, 1, "e8" },
	{ "LD q,a", "FD.06b0.d1.", 1, 1, "e8" },
	{ "LD I,A", "ED.47.", 3, 0 },
	{ "LD R,A", "ED.4F.", 3, 0 },
	{ "LD SP,HL", "F9.", 3, 0 },
	{ "LD SP,e", "d0.F9.", 3, 0 },
	{ "LD HL,(a)", "2A.e0", 3, 0 },
	{ "LD d,(a)", "ED.4Bf0.e1", 3, 0 },
	{ "LD d,a", "01f0.e1", 3, 0 },
	{ "LD e,(a)", "d0.2A.e1", 3, 0 },
	{ "LD e,a", "d0.21.e1", 3, 0 },
	{ "LD (HL),b", "70c0.", 3, 0 },
	{ "LD (HL),a", "36.d0.", 3, 0, "e8" },
	{ "LD (BC),A", "02.", 3, 0 },
	{ "LD (DE),A", "12.", 3, 0 },
	{ "LD (ca),b", "d0.70c2.d1.", 3, 0, "ii" },
	{ "LD (ca),a", "d0.36.d1.d2.", 3, 0, "iie8" },
	{ "LD (a),A", "32.e0", 3, 0 },
	{ "LD (a),HL", "22.e0", 3, 0 },
	{ "LD (a),d", "ED.43f1.e0", 3, 0 },
	{ "LD (a),e", "d1.22.e0", 3, 0 },
	{ "PUSH f", "C5f0.", 3, 0 },
	{ "PUSH e", "d0.E5.", 3, 0 },
	{ "POP f", "C1f0.", 3, 0 },
	{ "POP e", "d0.E1.", 3, 0 },
	{ "EX DE,HL", "EB.", 3, 0 },
	{ "EX AF,AF'", "08.", 3, 0 },
	{ "EX (SP),HL", "E3.", 3, 0 },
	{ "EX (SP),e", "d0.E3.", 3, 0 },
	{ "EXX", "D9.", 3, 0 },
	{ "LDI", "ED.A0.", 3, 0 },
	{ "LDIR", "ED.B0.", 3, 0 },
	{ "LDD", "ED.A8.", 3, 0 },
	{ "LDDR", "ED.B8.", 3, 0 },
	{ "CPI", "ED.A1.", 3, 0 },
	{ "CPIR", "ED.B1.", 3, 0 },
	{ "CPD", "ED.A9.", 3, 0 },
	{ "CPDR", "ED.B9.", 3, 0 },
	{ "ADD HL,d", "09f0.", 3, 0 },
	{ "ADD IX,i", "DD.09f0.", 3, 0 },
	{ "ADD IY,j", "FD.09f0.", 3, 0 },
	{ "ADC HL,d", "ED.4Af0.", 3, 0 },
	{ "SBC HL,d", "ED.42f0.", 3, 0 },
	{ "g A,b", "m080b0c1.", 3, 0 },
	{ "g A,p", "DD.m080b0c1.", 1, 1 },
	{ "g A,q", "FD.m080b0c1.", 1, 1 },
	{ "g A,(HL)", "m086b0.", 3, 0 },
	{ "g A,(ca)", "m0d1.86b0.d2.", 3, 0, "ii" },
	{ "g A,a", "m0C6b0.d1.", 3, 0, "e8" },
	{ "g b", "n080b0c1.", 3, 0 },
	{ "g p", "DD.n080b0c1.", 1, 1 },
	{ "g q", "FD.n080b0c1.", 1, 1 },
	{ "g (HL)", "n086b0.", 3, 0 },
	{ "g (ca)", "n0d1.86b0.d2.", 3, 0, "ii" },
	{ "g a", "n0C6b0.d1.", 3, 0, "e8" },
	{ "h b", "04b1c0.", 3, 0 },
	{ "h p", "DD.04b1c0.", 1, 1 },
	{ "h q", "FD.04b1c0.", 1, 1 },
	{ "h (HL)", "34c0.", 3, 0 },
	{ "h (ca)", "d1.34c0.d2.", 3, 0, "ii" },
	{ "INC d", "03f0.", 3, 0 },
	{ "INC e", "d0.23.", 3, 0 },
	{ "DEC d", "0Bf0.", 3, 0 },
	{ "DEC e", "d0.2B.", 3, 0 },
	{ "DAA", "27.", 3, 0 },
	{ "CPL", "2F.", 3, 0 },
	{ "NEG", "ED.44.", 3, 0 },
	{ "CCF", "3F.", 3, 0 },
	{ "SCF", "37.", 3, 0 },
	{ "NOP", "00.", 3, 0 },
	{ "HALT", "76.", 3, 0 },
	{ "DI", "F3.", 3, 0 },
	{ "EI", "FB.", 3, 0 },
	{ "IM a", "ED.k0.", 3, 0, "tt" },
	{ "RLCA", "07.", 3, 0 },
	{ "RLA", "17.", 3, 0 },
	{ "RRCA", "0F.", 3, 0 },
	{ "RRA", "1F.", 3, 0 },
	{ "SLL b", "CB.30c0.", 1, 1 },
	{ "SLL (HL)", "CB.36.", 1, 1 },
	{ "SLL (ca)", "d0.CB.d1.36.", 1, 1, "ii" },
	{ "SLL (ca),b", "d0.CB.d1.30c2.", 1, 1, "ii" },
	{ "k b", "CB.00b0c1.", 3, 0 },
	{ "k (HL)", "CB.06b0.", 3, 0 },
	{ "k (ca)", "d1.CB.d2.06b0.", 3, 0, "ii" },
	{ "k (ca),b", "d1.CB.d2.00b0c3.", 1, 1, "ii" },
	{ "RLD", "ED.6F.", 3, 0 },
	{ "RRD", "ED.67.", 3, 0 },
	{ "l a,b", "CB.00g0b1c2.", 3, 0, "b3" },
	{ "l a,(HL)", "CB.06g0b1.", 3, 0, "b3" },
	{ "l a,(ca)", "d2.CB.d3.06g0b1.", 3, 0, "b3ii" },
	{ "RES a,(ca),b", "d1.CB.d2.80b0c3.", 1, 1, "b3ii" },
	{ "SET a,(ca),b", "d1.CB.d2.C0b0c3.", 1, 1, "b3ii" },
	{ "JP (HL)", "E9.", 3, 0 },
	{ "JP (e)", "d0.E9.", 3, 0 },
	{ "JP m,a", "C2b0.e1", 3, 0 },
	{ "JP a", "C3.e0", 3, 0 },
	{ "JR n,a", "20b0.i1.", 3, 0, "r8" },
	{ "JR a", "18.i0.", 3, 0, "r8" },
	{ "DJNZ a", "10.i0.", 3, 0, "r8" },
	{ "CALL m,a", "C4b0.e1", 3, 0 },
	{ "CALL a", "CD.e0", 3, 0 },
	{ "RETI", "ED.4D.", 3, 0 },
	{ "RETN", "ED.45.", 3, 0 },
	{ "RET m", "C0b0.", 3, 0 },
	{ "RET", "C9.", 3, 0 },
	{ "RST a", "C7j0.", 3, 0, "ss" },
	{ "IN b,(C)", "ED.40b0.", 3, 0 },
	{ "IN A,(a)", "DB.d0.", 3, 0, "e8" },
	{ "IN F,(a)", "ED.70.", 3, 0 },
	{ "IN (C)", "ED.70.", 1, 1 },
	{ "INI", "ED.A2.", 3, 0 },
	{ "INIR", "ED.B2.", 3, 0 },
	{ "IND", "ED.AA.", 3, 0 },
	{ "INDR", "ED.BA.", 3, 0 },
	{ "OUT (C),0", "ED.71.", 1, 1 },
	{ "OUT (C),b", "ED.41b0.", 3, 0 },
	{ "OUT (a),A", "D3.d0.", 3, 0, "e8" },
	{ "OUTI", "ED.A3.", 3, 0 },
	{ "OTIR", "ED.B3.", 3, 0 },
	{ "OUTD", "ED.AB.", 3, 0 },
	{ "OTDR", "ED.BB.", 3, 0 },
	/* hd64180 added instructions */
	{ "IN0 b,(a)", "ED.00b0.d1.", 2, 0, "e8" }, 
	{ "OUT0 (a),b", "ED.01b1.d0.", 2, 0, "e8" },
	{ "OTDM", "ED.8B.", 2, 0 },
	{ "OTDMR", "ED.9B.", 2, 0 },
	{ "OTIM", "ED.83.", 2, 0 },
	{ "OTIMR", "ED.93.", 2, 0 },
	{ "MLT d", "ED.4Cf0.", 2, 0 },
	{ "SLP", "ED.76.", 2, 0 },
	{ "TST b", "ED.04b0.", 2, 0 },
	{ "TST (HL)", "ED.34.", 2, 0 },
	{ "TST a", "ED.64.d0.", 2, 0, "e8" },
	{ "TSTIO a", "ED.74.d0.", 2, 0, "e8" },
	{ NULL, NULL },
};

static const char *const bval[] = { "B", "C", "D", "E",
				    "H", "L", "", "A", NULL };
static const char *const cval[] = { "IX", "IY", NULL };
static const char *const dval[] = { "BC", "DE", "HL", "SP", NULL };
static const char *const fval[] = { "BC", "DE", "HL", "AF", NULL };
static const char *const gval[] = { "ADD", "ADC", "SUB", "SBC",
				    "AND", "XOR", "OR", "CP", NULL };
static const char *const hval[] = { "INC", "DEC", NULL };
static const char *const ival[] = { "BC", "DE", "IX", "SP", NULL };
static const char *const jval[] = { "BC", "DE", "IY", "SP", NULL };
static const char *const kval[] = { "RLC", "RRC", "RL", "RR",
				    "SLA", "SRA", "", "SRL", NULL };
static const char *const lval[] = { "", "BIT", "RES", "SET", NULL };
static const char *const mval[] = { "NZ", "Z", "NC", "C",
				    "PO", "PE", "P", "M", NULL };
static const char *const nval[] = { "NZ", "Z", "NC", "C", NULL };
static const char *const pval[] = { "B", "C", "D", "E",
				    "IXH", "IXL", "", "A", NULL };
static const char *const qval[] = { "B", "C", "D", "E",
				    "IYH", "IYL", "", "A", NULL };
static const char *const nullv[] = { NULL };

static const char *const *const valtab[] = { 
	bval, cval, dval, dval, fval,
       	gval, hval, ival, jval, kval,
       	lval, mval, nval, nullv, pval,
       	qval
};

static int indval(const char *p, int disp, const char **q)
{
	int v;
	const char *r;

	v = mreg(p, cval, &r);
	if (v >= 0) {
		v = (v == 0) ? 0xDD : 0xFD;
		if (!disp || *r == '+' || *r == '-') {
			*q = r;
			return v;
		}
	}
	return -1;
}

static int match_z80(char c, const char *p, const char **q)
{
	int v;

	if (c == 'c' || c == 'e') {
		v = indval(p, c == 'c', q);
	} else if (c <= 'q') {
		v = mreg(p, valtab[(int) (c - 'b')], q);
	} else {
		v = -1;
	}

	return v;
}

static int gen_z80(int *eb, char p, const int *vs, int i, int savepc)
{
	int b;
       
	b = *eb;
	switch (p) {
	case 'f': b |= (vs[i] << 4); break;
	case 'g': b |= (vs[i] << 6); break;
	case 'i': b = (vs[i] - savepc - 2);
		  if (s_pass > 0 && (b < -128 || b > 127)) {
			  eprint(_("range of relative branch exceeded (%d)\n"),
				b);
			  eprcol(s_pline, s_pline_ep);
			  newerr();
		  }
		  break;
	case 'j': if (s_pass > 0 && (vs[i] & ~56) != 0) {
			  eprint(_("invalid RST argument (%d)\n"),
				vs[i]);
			  eprcol(s_pline, s_pline_ep);
			  newerr();
		  }
		  b |= vs[i];
		  break;
	case 'k': if (s_pass > 0 && (vs[i] < 0 || vs[i] > 2)) {
			  eprint(_("invalid IM argument (%d)\n"),
				vs[i]);
			  eprcol(s_pline, s_pline_ep);
			  newerr();
		  }
		  b = 0x46;
		  if (vs[i] == 1)
			  b = 0x56;
		  else if (vs[i] == 2)
			  b = 0x5E;
		  break;
	case 'm': if (s_pass == 0 && !s_extended_op) {
			  if (vs[i] != 0 && vs[i] != 1 && vs[i] != 3) {
				eprint(_("unofficial syntax\n"));
				eprcol(s_pline, s_pline_ep);
				newerr();
			  }
		  }
		  break;
	case 'n': if (s_pass == 0 && !s_extended_op) {
			  if (vs[i] == 0 || vs[i] == 1 || vs[i] == 3) {
				eprint(_("unofficial syntax\n"));
				eprcol(s_pline, s_pline_ep);
				newerr();
			  }
		  }
		  break;
	default:
		  return -1;
	}

	*eb = b;
	return 0;
}

static int s_pat_char = 'b';
static int s_pat_index;

static void pat_char_rewind_z80(int c)
{
	s_pat_char = c;
	s_pat_index = 0;
};

static const char *pat_next_str_z80(void)
{
	const char *s;

	if (s_pat_char >= 'b' && s_pat_char <= 'q') {
		s = valtab[(int) (s_pat_char - 'b')][s_pat_index];
		if (s != NULL) {
			s_pat_index++;
		}
	} else {
		s = NULL;
	}

	return s;
};

const struct target s_target_z80 = {
	.id = "z80",
	.descr = "Zilog Z80",
	.matcht = s_matchtab_z80,
	.matchf = match_z80,
	.genf = gen_z80,
	.pat_char_rewind = pat_char_rewind_z80,
	.pat_next_str = pat_next_str_z80,
	.mask = 1
};

const struct target s_target_hd64180 = {
	.id = "hd64180",
	.descr = "Hitachi HD64180",
	.matcht = s_matchtab_z80,
	.matchf = match_z80,
	.genf = gen_z80,
	.pat_char_rewind = pat_char_rewind_z80,
	.pat_next_str = pat_next_str_z80,
	.mask = 2
};
