#include "pp.h"
#include "err.h"
#include "options.h"
#include "uz80as.h"
#include <stddef.h>
#include <assert.h>

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

/* pat:
 * 	a: expr
 * 	b: B,C,D,E,H,L,A
 * 	c: *
 * 	d: BC,DE,HL,SP
 * 	e: *
 * 	f: BC,DE,HL,AF
 * 	g: ADD,ADC,SUB,SBC,AND,XOR,OR,CP
 * 	h: INC,DEC
 * 	i: *
 * 	j: *
 * 	k: *
 * 	l: BIT,RES,SET
 * 	m: *
 * 	n: NZ,Z,NC,C
 * 	o: RLC,RRC,RL,RR,SLA,SRA,SWAP,SRL
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

const struct matchtab s_matchtab_gbcpu[] = {
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

static const char *const bval[] = { "B", "C", "D", "E",
				    "H", "L", "", "A", NULL };
static const char *const dval[] = { "BC", "DE", "HL", "SP", NULL };
static const char *const fval[] = { "BC", "DE", "HL", "AF", NULL };
static const char *const gval[] = { "ADD", "ADC", "SUB", "SBC",
				    "AND", "XOR", "OR", "CP", NULL };
static const char *const hval[] = { "INC", "DEC", NULL };
static const char *const lval[] = { "", "BIT", "RES", "SET", NULL };
static const char *const nval[] = { "NZ", "Z", "NC", "C", NULL };
static const char *const oval[] = { "RLC", "RRC", "RL", "RR",
				    "SLA", "SRA", "SWAP", "SRL", NULL };
static const char *const nullv[] = { NULL };

static const char *const *const valtab[] = { 
	bval, nullv, dval, nullv, fval,
       	gval, hval, nullv, nullv, nullv,
       	lval, nullv, nval, oval
};

static int match_gbcpu(char c, const char *p, const char **q)
{
	int v;

	switch (c) {
	case 'b':
	case 'd':
	case 'f':
	case 'g':
	case 'h':
	case 'l':
	case 'n':
	case 'o':
		v = mreg(p, valtab[(int) (c - 'b')], q);
		break;
	default:
		v = -1;
	}

	return v;
}

static int gen_gbcpu(int *eb, char p, const int *vs, int i, int savepc)
{
	int w, b;
       
	b = *eb;
	switch (p) {
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
	default:
		  return -1;
	}

	*eb = b;
	return 0;
}

const struct target s_target_gbcpu = {
	"gbcpu",
	"Sharp LR35902 (Nintendo Gameboy CPU)",
	s_matchtab_gbcpu,
	match_gbcpu,
	gen_gbcpu
};
