/* ===========================================================================
 * uz80as, a macro assembler for Z80 based microprocessors.
 *
 * Assembler.
 * ===========================================================================
 */

#include <config.h>
#include "uz80as.h"
#include "options.h"
#include "utils.h"
#include "err.h"
#include "incl.h"
#include "sym.h"
#include "expr.h"
#include "exprint.h"
#include "pp.h"
#include "list.h"

#ifndef ASSERT_H
#include <assert.h>
#endif

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

static const char *d_null(const char *);
static const char *d_block(const char *);
static const char *d_byte(const char *);
static const char *d_chk(const char *);
static const char *d_codes(const char *);
static const char *d_echo(const char *);
static const char *d_eject(const char *);
static const char *d_export(const char *);
static const char *d_end(const char *);
static const char *d_equ(const char *);
static const char *d_fill(const char *);
static const char *d_list(const char *);
static const char *d_lsfirst(const char *);
static const char *d_msfirst(const char *);
static const char *d_nocodes(const char *);
static const char *d_nolist(const char *);
static const char *d_org(const char *);
static const char *d_text(const char *);
static const char *d_title(const char *);
static const char *d_word(const char *);

/* 
 * Directives.
 * This table must be sorted, to allow for binary search.
 */ 
static struct direc {
	const char *name;
	const char *(*fun)(const char *);
} s_directab[] = { 
	{ "BLOCK", d_block },
	{ "BYTE", d_byte },
	{ "CHK", d_chk },
	{ "CODES", d_codes },
	{ "DB", d_byte },
	{ "DW", d_word },
	{ "ECHO", d_echo },
	{ "EJECT", d_eject },
	{ "END", d_end },
	{ "EQU", d_equ },
	{ "EXPORT", d_export },
	{ "FILL", d_fill },
	{ "LIST", d_list },
	{ "LSFIRST", d_lsfirst },
	{ "MSFIRST", d_msfirst },
	{ "NOCODES", d_nocodes },
	{ "NOLIST", d_nolist },
	{ "NOPAGE", d_null },
	{ "ORG", d_org },
	{ "PAGE", d_null },
	{ "TEXT", d_text },
	{ "TITLE", d_title },
	{ "WORD", d_word },
};

/* matchtab.flags */
enum {
	MATCH_F_UNDOC = 1,
	MATCH_F_EXTEN = 2,
};

struct matchtab {
	const char *pat;
	const char *gen;
	unsigned char flags;
};

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
 * 	k: RLC,RRC,RL,RR,SLA,SRA,SLL,SRL
 * 	l: BIT,RES,SET
 * 	m: NZ,Z,NC,C,PO,PE,P,M
 * 	n: NZ,Z,NC,C
 * 	o: RLC,RRC,RL,RR,SLA,SRA,SWAP,SRL
 *      p: B,C,D,E,IXH,IXL,A
 *      q: B,C,D,E,IYH,IYL,A
 *
 * gen:
 * 	.: output lastbyte
 * 	b: (op << 3) | lastbyte
 * 	c: op | lastbyte
 * 	d: lastbyte = op as 8 bit value
 * 	e: output op as word (no '.' sould follow)
 * 	f: (op << 4) | lastbyte
 * 	g: (op << 6) | lastbyte
 * 	h: (op << 3) | lastbyte
 * 	i: relative jump to op
 * 	j: possible value to RST
 * 	k: possible value to IM
 * 	m: check arithmetic used with A register
 * 	n: check arithmetic used without A register
 * 	o: if op >= FF00 output last byte and then op as 8 bit value;
 * 	   else output (lastbyte | 0x0A) and output op as word
 * 	   (no '.' should follow)
 */

#if 0
Opcode  Z80             GMB
 ---------------------------------------
 08      EX   AF,AF      LD   (nn),SP
 10      DJNZ PC+dd      STOP
 22      LD   (nn),HL    LDI  (HL),A
 2A      LD   HL,(nn)    LDI  A,(HL)
 32      LD   (nn),A     LDD  (HL),A
 3A      LD   A,(nn)     LDD  A,(HL)
 D3      OUT  (n),A      -
 D9      EXX             RETI
 DB      IN   A,(n)      -
 DD      <IX>            -
 E0      RET  PO         LD   (FF00+n),A
 E2      JP   PO,nn      LD   (FF00+C),A
 E3      EX   (SP),HL    -
 E4      CALL PO,nn      -
 E8      RET  PE         ADD  SP,dd
 EA      JP   PE,nn      LD   (nn),A
 EB      EX   DE,HL      -
 EC      CALL PE,nn      -
 ED      <pref>          -
 F0      RET  P          LD   A,(FF00+n)
 F2      JP   P,nn       LD   A,(FF00+C)
 F4      CALL P,nn       -
 F8      RET  M          LD   HL,SP+dd
 FA      JP   M,nn       LD   A,(nn)
 FC      CALL M,nn       -
 FD      <IY>            -
 CB3X    SLL  r/(HL)     SWAP r/(HL) 
#endif

/* The gameboy cpu Sharp LR35902 */
const struct matchtab s_matchtab_gbz80[] =
{
	{ "LD b,b", "40b0c1." },
	{ "LD b,(HL)", "46b0." },
	{ "LD A,(C)", "F2." }, // * LD A,(FF00+C)
	{ "LD A,(BC)", "0A." },
	{ "LD A,(DE)", "1A." },
	{ "LD A,(HLI)", "2A." }, // *
	{ "LD A,(HLD)", "3A." }, // *
	{ "LD A,(a)", "F0o0" }, // * LD A,(nn) & LD A,(FF00+n)
	{ "LD b,a", "06b0.d1." },
	{ "LD SP,HL", "F9." },
	{ "LDHL SP,a", "F8.d0." }, // * LD HL,SP+n
	{ "LD d,a", "01f0.e1" },
	{ "LD (C),A", "E2." }, // * LD (FF00+C),A
	{ "LD (HL),b", "70c0." },
	{ "LD (HL),a", "36.d0." },
	{ "LD (HLI),A", "22." }, // *
	{ "LD (HLD),A", "32." }, // *
	{ "LD (BC),A", "02." },
	{ "LD (DE),A", "12." },
	{ "LD (a),A", "E0o0" }, // * LD (nn),A & LD (FF00+n),A
	{ "LD (a),SP", "08.e0" }, // *
	{ "LDH A,(a)", "F0.d0." }, // * LD A,(FF00+n)
	{ "LDH (a),A", "E0.d0." }, // * LD (FF00+n),A
	{ "PUSH f", "C5f0." },
	{ "POP f", "C1f0." },
	{ "ADD HL,d", "09f0." },
	{ "ADD SP,a", "E8.d0." }, // * 
	{ "g A,b", "m080b0c1." },
	{ "g A,(HL)", "m086b0." },
	{ "g A,a", "m0C6b0.d1." },
	{ "g b", "n080b0c1." },
	{ "g (HL)", "n086b0." },
	{ "g a", "n0C6b0.d1." },
	{ "h b", "04b1c0." },
	{ "h (HL)", "34c0." },
	{ "INC d", "03f0." },
	{ "DEC d", "0Bf0." },
	{ "DAA", "27." },
	{ "CPL", "2F." },
	{ "CCF", "3F." },
	{ "SCF", "37." },
	{ "NOP", "00." },
	{ "HALT", "76." },
	{ "DI", "F3." },
	{ "EI", "FB." },
	{ "RLCA", "07." },
	{ "RLA", "17." },
	{ "RRCA", "0F." },
	{ "RRA", "1F." },
	{ "o b", "CB.00b0c1." },
	{ "o (HL)", "CB.06b0." },
	{ "l a,b", "CB.00g0b1c2." },
	{ "l a,(HL)", "CB.06g0b1." },
	{ "JP (HL)", "E9." },
	{ "JP n,a", "C2b0.e1" }, // *
	{ "JP a", "C3.e0" },
	{ "JR n,a", "20h0.i1." },
	{ "JR a", "18.i0." },
	{ "STOP", "10.00." }, // *
	{ "CALL n,a", "C4b0.e1" }, // *
	{ "CALL a", "CD.e0" },
	{ "RETI", "D9." }, // *
	{ "RET n", "C0b0." },
	{ "RET", "C9." },
	{ "RST a", "C7j0." },
	{ NULL, NULL },
};

/* Original Zilog Z80 */
const struct matchtab s_matchtab_z80[] =
{
	{ "LD b,b", "40b0c1." },
	{ "LD p,p", "DD.40b0c1.", MATCH_F_UNDOC },
	{ "LD q,q", "FD.40b0c1.", MATCH_F_UNDOC },
	{ "LD b,(HL)", "46b0." },
	{ "LD b,(ca)", "d1.46b0.d2." },
	{ "LD A,I", "ED.57." },
	{ "LD A,R", "ED.5F." },
	{ "LD A,(BC)", "0A." },
	{ "LD A,(DE)", "1A." },
	{ "LD A,(a)", "3A.e0" },
	{ "LD b,a", "06b0.d1." },
	{ "LD p,a", "DD.06b0.d1.", MATCH_F_UNDOC },
	{ "LD q,a", "FD.06b0.d1.", MATCH_F_UNDOC },
	{ "LD I,A", "ED.47." },
	{ "LD R,A", "ED.4F." },
	{ "LD SP,HL", "F9." },
	{ "LD SP,e", "d0.F9." },
	{ "LD HL,(a)", "2A.e0" },
	{ "LD d,(a)", "ED.4Bf0.e1" },
	{ "LD d,a", "01f0.e1" },
	{ "LD e,(a)", "d0.2A.e1" },
	{ "LD e,a", "d0.21.e1" },
	{ "LD (HL),b", "70c0." },
	{ "LD (HL),a", "36.d0." },
	{ "LD (BC),A", "02." },
	{ "LD (DE),A", "12." },
	{ "LD (ca),b", "d0.70c2.d1." },
	{ "LD (ca),a", "d0.36.d1.d2." },
	{ "LD (a),A", "32.e0" },
	{ "LD (a),HL", "22.e0" },
	{ "LD (a),d", "ED.43f1.e0" },
	{ "LD (a),e", "d1.22.e0" },
	{ "PUSH f", "C5f0." },
	{ "PUSH e", "d0.E5." },
	{ "POP f", "C1f0." },
	{ "POP e", "d0.E1." },
	{ "EX DE,HL", "EB." },
	{ "EX AF,AF'", "08." },
	{ "EX (SP),HL", "E3." },
	{ "EX (SP),e", "d0.E3." },
	{ "EXX", "D9." },
	{ "LDI", "ED.A0." },
	{ "LDIR", "ED.B0." },
	{ "LDD", "ED.A8." },
	{ "LDDR", "ED.B8." },
	{ "CPI", "ED.A1." },
	{ "CPIR", "ED.B1." },
	{ "CPD", "ED.A9." },
	{ "CPDR", "ED.B9." },
	{ "ADD HL,d", "09f0." },
	{ "ADD IX,i", "DD.09f0." },
	{ "ADD IY,j", "FD.09f0." },
	{ "ADC HL,d", "ED.4Af0." },
	{ "SBC HL,d", "ED.42f0." },
	{ "g A,b", "m080b0c1." },
	{ "g A,p", "DD.m080b0c1.", MATCH_F_UNDOC },
	{ "g A,q", "FD.m080b0c1.", MATCH_F_UNDOC },
	{ "g A,(HL)", "m086b0." },
	{ "g A,(ca)", "m0d1.86b0.d2." },
	{ "g A,a", "m0C6b0.d1." },
	{ "g b", "n080b0c1." },
	{ "g p", "DD.n080b0c1.", MATCH_F_UNDOC },
	{ "g q", "FD.n080b0c1.", MATCH_F_UNDOC },
	{ "g (HL)", "n086b0." },
	{ "g (ca)", "n0d1.86b0.d2." },
	{ "g a", "n0C6b0.d1." },
	{ "h b", "04b1c0." },
	{ "h p", "DD.04b1c0.", MATCH_F_UNDOC },
	{ "h q", "FD.04b1c0.", MATCH_F_UNDOC },
	{ "h (HL)", "34c0." },
	{ "h (ca)", "d1.34c0.d2." },
	{ "INC d", "03f0." },
	{ "INC e", "d0.23." },
	{ "DEC d", "0Bf0." },
	{ "DEC e", "d0.2B." },
	{ "DAA", "27." },
	{ "CPL", "2F." },
	{ "NEG", "ED.44." },
	{ "CCF", "3F." },
	{ "SCF", "37." },
	{ "NOP", "00." },
	{ "HALT", "76." },
	{ "DI", "F3." },
	{ "EI", "FB." },
	{ "IM a", "ED.k0." },
	{ "RLCA", "07." },
	{ "RLA", "17." },
	{ "RRCA", "0F." },
	{ "RRA", "1F." },
	{ "SLL b", "CB.30c0.", MATCH_F_UNDOC },
	{ "SLL (HL)", "CB.36.", MATCH_F_UNDOC },
	{ "SLL (ca)", "d0.CB.d1.36.", MATCH_F_UNDOC },
	{ "k b", "CB.00b0c1." },
	{ "k (HL)", "CB.06b0." },
	{ "k (ca)", "d1.CB.d2.06b0." },
	{ "k (ca),b", "d1.CB.d2.00b0c3.", MATCH_F_UNDOC },
	{ "RLD", "ED.6F." },
	{ "RRD", "ED.67." },
	{ "l a,b", "CB.00g0b1c2." },
	{ "l a,(HL)", "CB.06g0b1." },
	{ "l a,(ca)", "d2.CB.d3.06g0b1." },
	{ "RES a,(ca),b", "d1.CB.d2.80b0c3.", MATCH_F_UNDOC },
	{ "SET a,(ca),b", "d1.CB.d2.C0b0c3.", MATCH_F_UNDOC },
	{ "JP (HL)", "E9." },
	{ "JP (e)", "d0.E9." },
	{ "JP m,a", "C2b0.e1" },
	{ "JP a", "C3.e0" },
	{ "JR n,a", "20h0.i1." },
	{ "JR a", "18.i0." },
	{ "DJNZ a", "10.i0." },
	{ "CALL m,a", "C4b0.e1" },
	{ "CALL a", "CD.e0" },
	{ "RETI", "ED.4D." },
	{ "RETN", "ED.45." },
	{ "RET m", "C0b0." },
	{ "RET", "C9." },
	{ "RST a", "C7j0." },
	{ "IN b,(C)", "ED.40b0." },
	{ "IN A,(a)", "DB.d0." },
	{ "IN F,(a)", "ED.70." },
	{ "IN (C)", "ED.70.", MATCH_F_UNDOC },
	{ "INI", "ED.A2." },
	{ "INIR", "ED.B2." },
	{ "IND", "ED.AA." },
	{ "INDR", "ED.BA." },
	{ "OUT (C),0", "ED.71.", MATCH_F_UNDOC },
	{ "OUT (C),b", "ED.41b0." },
	{ "OUT (a),A", "D3.d0." },
	{ "OUTI", "ED.A3." },
	{ "OTIR", "ED.B3." },
	{ "OUTD", "ED.AB." },
	{ "OTDR", "ED.BB." },
	{ NULL, NULL },
};

const struct matchtab *s_matchtab = s_matchtab_z80;

const char *const bval[] = { "B", "C", "D", "E", "H", "L", "", "A", NULL };
const char *const cval[] = { "IX", "IY", NULL };
const char *const dval[] = { "BC", "DE", "HL", "SP", NULL };
const char *const fval[] = { "BC", "DE", "HL", "AF", NULL };
const char *const gval[] = { "ADD", "ADC", "SUB", "SBC", "AND",
			     "XOR", "OR", "CP", NULL };
const char *const hval[] = { "INC", "DEC", NULL };
const char *const ival[] = { "BC", "DE", "IX", "SP", NULL };
const char *const jval[] = { "BC", "DE", "IY", "SP", NULL };
const char *const kval[] = { "RLC", "RRC", "RL", "RR", "SLA",
			     "SRA", "SLL", "SRL", NULL };
const char *const lval[] = { "", "BIT", "RES", "SET", NULL };
const char *const mval[] = { "NZ", "Z", "NC", "C", "PO",
			     "PE", "P", "M", NULL };
const char *const nval[] = { "NZ", "Z", "NC", "C", NULL };
const char *const oval[] = { "RLC", "RRC", "RL", "RR", "SLA",
			     "SRA", "SWAP", "SRL", NULL };
const char *const pval[] = { "B", "C", "D", "E", "IXH", "IXL", "", "A", NULL };
const char *const qval[] = { "B", "C", "D", "E", "IYH", "IYL", "", "A", NULL };

const char *const *const valtab[] = { 
	bval, cval, dval, dval, fval,
       	gval, hval, ival, jval, kval,
       	lval, mval, nval, oval, pval,
	qval
};

/* The z80 addressable memory. The object code. */
static unsigned char s_mem[64 * 1024];

/* Program counter min and max ([s_minpc, s_maxpc[). */
static int s_minpc, s_maxpc;

/* Original input line. */
static char s_line[LINESZ];

/* Label defined on this line. */
static struct sym *s_lastsym;

/* Output words the most significant byte first */
static int s_msbword;

/* If we have seen the .END directive. */
static int s_end_seen;

/* We have issued the error of generating things after an .END. */
static int s_gen_after_end;

/* The empty line, to pass to listing, for compatibility with TASM. */
static const char *s_empty_line = "";

/* Pointer in s_pline for error reporting. */
static const char *s_pline_ep;

/* We skip characters until endline or backslash or comment. */
static const char *sync(const char *p)
{
	while (*p != '\0' && *p != '\\' && *p != ';')
		p++;
	return p;
}

/* 
 * Generates a byte to the output and updates s_pc, s_minpc and s_maxpc.
 * Will issue a fatal error if we write beyong 64k.
 */
static void genb(int b, const char *ep)
{
	if (s_pass == 0 && s_end_seen && !s_gen_after_end) {
		s_gen_after_end = 1;
		eprint(_("generating code after .END\n"));
		eprcol(s_pline, ep);
		newerr();
	}
	if (s_minpc < 0)
		s_minpc = s_pc;
       	if (s_pc >= 65536) {
		eprint(_("generating code beyond address 65535\n"));
		eprcol(s_pline, ep);
		exit(EXIT_FAILURE);
	}
	s_mem[s_pc] = (unsigned char) b;
	if (s_pass == 1)
		list_genb(b);
	if (s_pc < s_minpc)
		s_minpc = s_pc;
	s_pc++;
	if (s_pc > s_maxpc)
		s_maxpc = s_pc;
}

/* 
 * Generate 'n' as a 16 bit word, little endian or big endian depending on
 * s_msbword.
 */
static void genw(int n, const char *ep)
{
	if (s_msbword)
		genb(n >> 8, ep);
	genb(n, ep);
	if (!s_msbword)
		genb(n >> 8, ep);
}

/* 
 * We have matched an instruction in the table.
 * Generate the machine code for the instruction using the generation
 * pattern 'p. 'vs are the arguments generated during the matching process.
 */
static void gen(const char *p, const int *vs)
{
	int w, b, i, savepc;

	savepc = s_pc;
	b = 0;
loop:
	i = hexvalu(*p);
	if (i >= 0) {
		p++;
		b = (i << 4) | hexval(*p);
	} else if (*p == '.') {
		genb(b, s_pline_ep);
		b = 0;
	} else if (*p == '\0') {
		return;
	} else {
		i = *(p + 1) - '0';
		switch (*p++) {
		case 'b': b |= ((vs[i] & 7) << 3); break;
		case 'c': b |= (vs[i] & 7); break;
		case 'd': b = vs[i]; break;
		case 'e': genb(vs[i] & 0xff, s_pline_ep);
			  genb(vs[i] >> 8, s_pline_ep);
			  break;
		case 'f': b |= ((vs[i] & 3) << 4); break;
		case 'g': b |= ((vs[i] & 3) << 6); break;
		case 'h': b |= ((vs[i] & 3) << 3); break;
		case 'i': b = (vs[i] - savepc - 2); break;
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
					eprint(_("unofficial instruction\n"));
					eprcol(s_pline, s_pline_ep);
					newerr();
				  }
			  }
			  break;
		case 'n': if (s_pass == 0 && !s_extended_op) {
				  if (vs[i] == 0 || vs[i] == 1 || vs[i] == 3) {
					eprint(_("unofficial instruction\n"));
					eprcol(s_pline, s_pline_ep);
					newerr();
				  }
			  }
			  break;
		case 'o': w = vs[i] & 0xffff;
			  if (w >= 0xff00) {
				genb(b, s_pline_ep);
				b = 0;
			  	genb(w & 0xff, s_pline_ep);
			  } else {
				b |= 0x0A; 
				genb(b, s_pline_ep);
				b = 0;
			  	genb(w & 0xff, s_pline_ep);
			  	genb(w >> 8, s_pline_ep);
			  }
			  break;
		}
	}
	p++;
	goto loop;
}

/*
 * Tries to match *p with any of the strings in list.
 * If matched, returns the index in list and r points to the position
 * in p past the matched string.
 */
static int mreg(const char *p, const char *const list[], const char **r)
{
	const char *s;
	const char *q;
	int i;

	i = 0;
	while ((s = list[i++]) != NULL) {
		if (*s == '\0')
			continue;
		q = p;
		while (toupper(*q++) == *s++) {
			if (*s == '\0') {
				*r = q;
				return i - 1;
			}
		}
	}
	return -1;
}

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

static int isoctal(int c)
{
	return c >= '0' && c <= '7';
}

/*
 * Read an octal of 3 digits, being the maximum value 377 (255 decimal);
 * Return -1 if there is an error in the syntax.
 */
static int readoctal(const char *p)
{
	int n;
	const char *q;

	if (*p >= '0' && *p <= '3' && isoctal(*(p + 1)) && isoctal(*(p + 2))) {
		n = 0;
		q = p + 3;
		while (p < q) {
			n *= 8;
			n += (*p - '0');
			p++;
		}
		return n;
	}

	return -1;
}

enum strmode {
	STRMODE_ECHO,
	STRMODE_NULL,
	STRMODE_BYTE,
	STRMODE_WORD
};

/* 
 * Generate the string bytes until double quote or null char.
 * Return a pointer to the ending double quote character or '\0'.
 * 'p must point to the starting double quote.
 * If mode:
 * 	STRMODE_ECHO only echo to stderr the characters.
 * 	STRMODE_NULL only parses the string.
 * 	STRMODE_BYTE generate the characters in the binary file as bytes.
 * 	STRMODE_WORD generate the characters in the binary file as words.
 */
static const char *genstr(const char *p, enum strmode mode)
{
	int c;

	for (p = p + 1; *p != '\0' && *p != '\"'; p++) {
		c = *p;
		if (c == '\\') {
			p++;
			switch (*p) {
			case 'n': c = '\n'; break;
			case 'r': c = '\r'; break;
			case 'b': c = '\b'; break;
			case 't': c = '\t'; break;
			case 'f': c = '\f'; break;
			case '\\': c = '\\'; break;
			case '\"': c = '\"'; break;
			default:
				c = readoctal(p);
				if (c < 0) {
					eprint(_("bad character escape "
						 "sequence\n"));
					eprcol(s_pline, p - 1);
					newerr();
					p--;
				} else {
					p += 2;
				}
			}
		}
		switch (mode) {
		case STRMODE_ECHO: fputc(c, stderr); break;
		case STRMODE_NULL: break;
		case STRMODE_BYTE: genb(c, p); break;
		case STRMODE_WORD: genw(c, p); break;
		}
	}

	return p;
}

/* Match an instruction.
 * If no match returns NULL; else returns one past end of match.
 * p should point to no whitespace.
 */
static const char *match(const char *p)
{
	const char *s, *pp, *q;
	int v, n, vi, linepc;
	int vs[4];

	assert(!isspace(*p));

	linepc = s_pc;
	pp = p;
	n = -1;
next:
	n++;
	s = s_matchtab[n].pat;
	if (s == NULL) {
		return NULL;
	} else if (!s_undocumented_op
	           && (s_matchtab[n].flags & MATCH_F_UNDOC))
	{
		goto next;
	}
	p = pp;
	vi = 0;
loop:
	if (*s == '\0') {
		p = skipws(p);
		if (*p != ';' && *p != '\0' && *p != '\\')
			goto next;
		else
			goto found;
	} else if (*s == ' ') {
		if (!isspace(*p))
			goto next;
		p = skipws(p);
	} else if ((*s == ',' || *s == '(' || *s == ')') && isspace(*p)) {
		p = skipws(p);
		if (*s != *p)
			goto next;
		p = skipws(p + 1);
	} else if (*s == 'a') {
		p = expr(p, &v, linepc, s_pass == 0, NULL, NULL);
		if (p == NULL)
			return NULL;
		vs[vi++] = v;
	} else if (*s == 'c' || *s == 'e') {
		v = indval(p, *s == 'c', &q);
		goto reg;
	} else if (*s >= 'b' && *s <= 'q') {
		v = mreg(p, valtab[(int) (*s - 'b')], &q);
		goto reg;
	} else if (*p == *s && *p == ',') {
		p = skipws(p + 1);
	} else if (toupper(*p) == *s) {
		p++;
	} else {
		goto next;
	}
freg:
	s++;
	goto loop;
reg:
	if (v < 0) {
		goto next;
	} else {
		vs[vi++] = v;
		p = q;
	}
	goto freg;
found:
	// printf("%s\n", s_matchtab[n].pat);
	gen(s_matchtab[n].gen, vs);
	return p;
}

static const char *d_null(const char *p)
{
	while (*p != '\0' && *p != '\\') {
		if (!isspace(*p)) {
			wprint(_("invalid characters after directive\n"));
			eprcol(s_pline, p);
			return sync(p);
		} else {
			p++;
		}
	}
	return p;
}

static const char *d_end(const char *p)
{
	enum expr_ecode ecode;
	const char *q;
	const char *ep;

	if (s_pass == 0) {
		if (s_end_seen) {
			eprint(_("duplicate .END\n"));
			eprcol(s_pline, s_pline_ep);
			newerr();
		} else {
			s_end_seen = 1;
		}
	}
		
	q = expr(p, NULL, s_pc, s_pass == 0, &ecode, &ep);
	if (q == NULL && ecode == EXPR_E_NO_EXPR) {
		return p;
	} else if (q == NULL) {
		exprint(ecode, s_pline, ep);
		newerr();
		return NULL;
	} else {
		return q;
	}
}

static const char *d_codes(const char *p)
{
	s_codes = 1;
	return p;
}

static const char *d_nocodes(const char *p)
{
	s_codes = 0;
	return p;
}

static const char *d_list(const char *p)
{
	s_list_on = 1;
	return p;
}

static const char *d_nolist(const char *p)
{
	s_list_on = 0;
	return p;
}

static const char *d_eject(const char *p)
{
	list_eject();
	return p;
}

static const char *d_echo(const char *p)
{
	int n;
	int mode;
	enum expr_ecode ecode;
	const char *ep;

	mode = (s_pass == 0) ? STRMODE_NULL : STRMODE_ECHO;
	if (*p == '\"') {
		p = genstr(p, mode);
		if (*p == '\"') {
			p++;
		} else if (s_pass == 0) {
			wprint(_("no terminating quote\n"));
			eprcol(s_pline, p);
		}
	} else if (*p != '\0') {
		p = expr(p, &n, s_pc, s_pass == 0, &ecode, &ep);
		if (p == NULL) {
			exprint(ecode, s_pline, ep);
			newerr();
			return NULL;
		}
		if (mode == STRMODE_ECHO) {
			fprintf(stderr, "%d", n);
		}
	}
	return p;
}

static const char *d_equ(const char *p)
{
	int n;
	enum expr_ecode ecode;
	const char *ep;

	p = expr(p, &n, s_pc, 0, &ecode, &ep);
	if (p == NULL) {
		exprint(ecode, s_pline, ep);
		newerr();
		return NULL;
	}

	if (s_lastsym == NULL) {
		eprint(_(".EQU without label\n"));
		eprcol(s_pline, s_pline_ep);
		newerr();
	} else {
		/* TODO: check label misalign? */
		s_lastsym->val = n;
		s_lastsym->isequ = 1;
	}
	return p;
}

static const char *d_export(const char *p)
{
	/* TODO */
	return NULL;
}

static const char *d_fill(const char *p)
{
	int n, v, er;
	const char *q;
	enum expr_ecode ecode;
	const char *ep, *eps;

	eps = p;
	er = 0;
	p = expr(p, &n, s_pc, 0, &ecode, &ep); 
	if (p == NULL) {
		exprint(ecode, s_pline, ep);
		newerr();
		return NULL;
	}

	if (n < 0) {
		eprint(_("number of positions to fill is negative (%d)\n"), n);
		eprcol(s_pline, eps);
		exit(EXIT_FAILURE);
	}

	v = 255;
	p = skipws(p);
	if (*p == ',') {
		p = skipws(p + 1);
		q = expr(p, &v, s_pc, s_pass == 0, &ecode, &ep);
		if (q == NULL) {
			er = 1;
			exprint(ecode, s_pline, ep);
			newerr();
		} else {
			p = q;
		}
	}

	while (n--)
		genb(v, eps);

	if (er)
		return NULL;
	else
		return p;
}

static const char *d_lsfirst(const char *p)
{
	s_msbword = 0;
	return p;
}

static const char *d_msfirst(const char *p)
{
	s_msbword = 1;
	return p;
}

static const char *d_org(const char *p)
{
	int n;
	enum expr_ecode ecode;
	const char *ep, *eps;

	eps = p;
	p = expr(p, &n, s_pc, 0, &ecode, &ep);
	if (p == NULL) {
		exprint(ecode, s_pline, ep);
		newerr();
		return NULL;
	}

	if (n < 0 || n > 65536) {
		eprint(_(".ORG address (%d) is not in range [0, 65536]\n"), n);
		eprcol(s_pline, eps);
		exit(EXIT_FAILURE);
	}

	s_pc = n;

	/* Change the listing PC so in orgs we print the changed PC. */
	if (s_pass > 0)
		list_setpc(s_pc);

	if (s_lastsym != NULL) {
		/* TODO: check label misalign? */
		s_lastsym->val = s_pc;
		s_lastsym->isequ = 1;
	}

	return p;
}

static const char *d_lst(const char *p, int w)
{
	enum strmode mode;
	int n, linepc;
	enum expr_ecode ecode;
	const char *ep, *eps;

	if (w)
		mode = STRMODE_WORD;
	else
		mode = STRMODE_BYTE;

	linepc = s_pc;
dnlst: 
	if (*p == '\"') {
		p = genstr(p, mode);
		if (*p == '\"') {
			p++;
		} else {
			wprint(_("no terminating quote\n"));
			eprcol(s_pline, p);
		}
	} else {
		eps = p;
		p = expr(p, &n, linepc, s_pass == 0, &ecode, &ep);
		if (p == NULL) {
			exprint(ecode, s_pline, ep);
			newerr();
			return NULL;
		}
		if (w)
			genw(n, eps);
		else
			genb(n, eps);
	}
	p = skipws(p);
	if (*p == ',') {
		p++;
		p = skipws(p);
		goto dnlst;
	}
	return p;
}

static const char *d_byte(const char *p)
{
	return d_lst(p, 0);
}

static const char *d_word(const char *p)
{
	return d_lst(p, 1);
}

static const char *d_text(const char *p)
{
	if (*p == '\"') {
		p = genstr(p, STRMODE_BYTE);
		if (*p == '\"') {
			p++;
		} else {
			wprint(_("no terminating quote\n"));
			eprcol(s_pline, p);
		}
		return p;
	} else {
		eprint(_(".TEXT directive needs a quoted string argument\n"));
		eprcol(s_pline, p);
		newerr();
		return NULL;
	}
}

static const char *d_title(const char *p)
{
	return NULL;
}

static const char *d_block(const char *p)
{
	int n;
	enum expr_ecode ecode;
	const char *ep, *eps;

	eps = p;
	p = expr(p, &n, s_pc, 0, &ecode, &ep);
	if (p == NULL) {
		exprint(ecode, s_pline, ep);
		newerr();
		return NULL;
	}

	s_pc += n;
	if (s_pc < 0 || s_pc > 65536) {
		eprint(_("address (%d) set by .BLOCK is not in range "
			 "[0, 65536]\n"), s_pc);
		eprcol(s_pline, eps);
		exit(EXIT_FAILURE);
	}

	return p;
}

/* a must be < b. */
static int checksum(int a, int b)
{
	int n;

	assert(a < b);

	n = 0;
	while (a < b)
		n += s_mem[a++];

	return n;
}

static const char *d_chk(const char *p)
{
	int n;
	enum expr_ecode ecode;
	const char *ep, *eps;

	eps = p;
	p = expr(p, &n, s_pc, s_pass == 0, &ecode, &ep);
	if (p == NULL) {
		exprint(ecode, s_pline, ep);
		newerr();
		genb(0, eps);
		return NULL;
	}

	if (s_pass == 0) {
		genb(0, s_pline_ep);
	} else if (n < 0 || n >= s_pc) {
		eprint(_(".CHK address (%d) is not in range [0, %d[\n"), n,
			s_pc);
		eprcol(s_pline, eps);
		newerr();
		genb(0, eps);
	} else {
		genb(checksum(n, s_pc), eps);
	}

	return p;
}

/* Parses an internal directive (those that start with '.').
 * Returns NULL on error;
 * If no error returns position past the parsed directive and arguments. */
static const char *parse_direc(const char *cp)
{
	const char *cq, *p;
	int a, b, m;

	a = 0;
	b = NELEMS(s_directab) - 1;
	while (a <= b) {
		m = (a + b) / 2;
		cq = cp;
		p = s_directab[m].name;
		while (*p != '\0' && toupper(*cq) == *p) {
			p++;
			cq++;
		}
		if (*p == '\0' && (*cq == '\0' || isspace(*cq)))
			break;
		else if (toupper(*cq) < *p)
			b = m - 1;
		else
			a = m + 1;
	}

	if (a <= b) {
		cq = skipws(cq);
		return s_directab[m].fun(cq);
	} else {
		eprint(_("unrecognized directive\n"));
		eprcol(s_pline, s_pline_ep);
		newerr();
		return NULL;
	}
}

static void parselin(const char *cp)
{
	int col0, alloweq;
	const char *q;

	s_pline_ep = cp;
start:	s_lastsym = NULL;
	alloweq = 0;
	col0 = 1;	
loop:
	if (*cp == '\0' || *cp == ';') {
		return;
	} else if (*cp == '\\') {
		if (s_pass == 1) {
			list_endln();
			list_startln(s_empty_line, curfile()->linenum, s_pc,
				nfiles());
		}
		cp++;
		goto start;
	} else if (*cp == '.') {
		s_pline_ep = cp;
		cp++;
		q = parse_direc(cp);
		if (q == NULL) {
			cp = sync(cp);
		} else {
			cp = d_null(q);
		}
	} else if ((*cp == '$' || *cp == '*') && cp[1] == '=') {
		/* Alternative form of .ORG: *= or $= */
		cp += 2;
		q = d_org(cp);
		if (q == NULL) {
			cp = sync(cp);
		} else {
			cp = d_null(q);
		}
	} else if (*cp == '=' && alloweq) {
		/* equ */
		s_pline_ep = cp;
		cp++;
		q = d_equ(cp);
		if (q == NULL) {
			cp = sync(cp);
		} else {
			cp = d_null(q);
		}
	} else if (isidc0(*cp)) {
		if (col0 && *cp != '.') {
			/* take label */
			s_pline_ep = cp;
			q = cp;
			col0 = 0;
			while (isidc(*cp))
				cp++;
			s_lastsym = lookup(q, cp, s_pass == 0, s_pc);
			if (*cp == ':' || isspace(*cp)) {
				alloweq = 1;
				cp++;
			} else if (*cp == '=') {
				alloweq = 1;
			}
			if (s_pass == 1 && !s_lastsym->isequ &&
				s_lastsym->val != s_pc)
			{
				eprint(_("misaligned label %s\n"),
					s_lastsym->name);
				fprintf(stderr, _(" Previous value was %XH, "
					"new value %XH."), s_lastsym->val,
					s_pc);
				eprcol(s_pline, s_pline_ep);
				newerr();
			}
		} else {
			cp = skipws(cp);
			s_pline_ep = cp;
			q = match(cp);
			if (q == NULL) {
				eprint(_("syntax error\n"));
				newerr();
				cp = sync(cp);
			} else {
				cp = d_null(q);
			}
		}
	} else if (isspace(*cp)) {
		col0 = 0;
		while (isspace(*cp))
			cp++;
	} else {
		eprint(_("unexpected character (%c)\n"), *cp);
		eprcol(s_pline, cp);
		newerr();
		cp = sync(cp + 1);
	}
	goto loop;
}

/*
 * Gets a new line into 's_line from 'fin.
 * Terminates the line with '\0'.
 * Does not read more than LINESZ - 1 characters.
 * Does not add a '\n' character, thus a line of length 0 it's possible.
 * Always advances to the next line.
 * Returns -1 for EOF or the line length.
 */
static int getlin(FILE *fin)
{
	int i, c;

	c = EOF;
	i = 0;
	while (i < LINESZ - 1) {
		c = getc(fin);
		if (c == EOF || c == '\n')
			break;
		s_line[i++] = (char) c;
	}
	if (c != EOF && c != '\n') {
		wprint(_("line too long, truncated to %d characters\n"),
			LINESZ);
	}
	while (c != EOF && c != '\n')
		c = getc(fin);
	if (i == 0 && c == EOF)
		return -1;
	s_line[i] = '\0';
	return i;
}

/* Preinstall the macros defined in the command line. */
static void install_predefs(void)
{
	struct predef *pdef;

	for (pdef = s_predefs; pdef != NULL; pdef = pdef->next)
		pp_define(pdef->name);
}

/* Do a pass through the source. */
static void dopass(const char *fname)
{
	/* Fill memory with default value. */
	if ((s_pass == 0 && s_mem_fillval != 0) || s_pass > 0) {
		memset(s_mem, s_mem_fillval, sizeof(s_mem));
	}

	if (s_pass > 0) {
		pp_reset();
		list_open(s_lstfname);
		s_codes = 1;
		s_list_on = 1;
	}

	install_predefs();
	s_minpc = -1;
	s_maxpc = -1;
	s_pc = 0;
	s_lastsym = NULL;
	s_msbword = 0;

	pushfile(fname, fname + strlen(fname));
	while (nfiles() > 0) {
		curfile()->linenum++;
		if (getlin(curfile()->fin) >= 0) {
			if (s_pass == 1) {
				list_startln(s_line, curfile()->linenum, s_pc,
					nfiles());
			}
			pp_line(s_line);
			if (s_pass == 1)
				list_skip(s_skipon);
			parselin(s_pline);
			if (s_pass == 1)
				list_endln();
		} else {
			popfile();
		}
	}

	list_close();
}

/* Write the object file. */
static void output ()
{
	FILE *fout;

	fout = efopen(s_objfname, "wb");
	if (s_minpc < 0)
		s_minpc = 0;
	if (s_maxpc < 0)
		s_maxpc = 0;

	fwrite(s_mem + s_minpc, 1, s_maxpc - s_minpc, fout);
	if (ferror(fout)) {
		eprint(_("cannot write to file %s\n"), s_objfname);
		clearerr(fout);
	}

	if (fclose(fout) == EOF) {
		eprint(_("cannot close file %s\n"), s_objfname);
	}
}

/* Start the assembly using the config in options.c. */
void uz80as(void)
{
	if (strcmp(s_cpuname, "gbz80") == 0) {
		s_matchtab = s_matchtab_gbz80;
	}

	for (s_pass = 0; s_nerrors == 0 && s_pass < 2; s_pass++) {
		dopass(s_asmfname);
		if (s_pass == 0 && !s_end_seen) {
			wprint(_("no .END statement in the source\n"));
		}
		if (s_nerrors == 0) {
			printf("Pass %d completed.\n", s_pass + 1);
		}
	}

	if (s_nerrors > 0) {
		exit(EXIT_FAILURE);
	}

	output();
}
