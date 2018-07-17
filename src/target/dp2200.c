/* ===========================================================================
 * uz80as, an assembler for the Zilog Z80 and several other microprocessors.
 *
 * Datapoint 2200.
 * ===========================================================================
 */

#include "pp.h"
#include "err.h"
#include "options.h"
#include "uz80as.h"
#include <stddef.h>

/* pat:
 *	a: expr
 *	b: ADA,ADB,ADC,ADD,ADH,ADL,ADM,
 *	   ACA,ACB,ACC,ACD,ACH,ACL,ACM,
 *	   SUA,SUB,SUC,SUD,SUH,SUL,SUM,
 *	   SBA,SBB,SBC,SBD,SBH,SBL,SBM,
 *	   NDA,NDB,NDC,NDD,NDH,NDL,NDM,
 *	   XRA,XRB,XRC,XRD,XRH,XRL,XRM,
 *	   ORA,ORB,ORC,ORD,ORH,ORL,ORM,
 *	   CPA,CPB,CPC,CPD,CPH,CPL,CPM
 *	c: NOP,LAB,LAC,LAD,LAE,LAH,LAL,LAM,
 *	   LBA,LBC,LBD,LBE,LBH,LBL,LBM,
 *	   LCA,LCB,LCD,LCE,LCH,LCL,LCM,
 *	   LDA,LDB,LDC,LDE,LDH,LDL,LDM,
 *	   LEA,LEB,LEC,LED,LEH,LEL,LEM,
 *	   LHA,LHB,LHC,LHD,LHE,LHL,LHM,
 *	   LLA,LLB,LLC,LLD,LLE,LLH,LLM,
 *	   LMA,LMB,LMC,LMD,LME,LMH,LML
 *	d: ADR,STATUS,DATA,WRITE,COM1,COM2,COM3,COM4
 *	   BEEP,CLICK,DECK1,DECK2,
 *	   RBK,WBK,BSP,SF,SB,REWND,TSTOP 
 *	e: RFC,RFS,RTC,RTS
 *	f: RFZ,RFP,RTZ,RTP
 *	g: AD,SU,ND,OR
 *	h: LA,LC,LE,LL
 *	i: AC,SB,XR,CP
 *	j: LB,LD,LH
 *	k: JFC,JFS,JTC,JTS
 *	l: CFC,CFS,CTC,CTS
 *	m: JFZ,JFP,JTZ,JTP
 *	n: CFZ,CFP,CTZ,CTP
 *
 * gen:
 * 	.: output lastbyte
 * 	b: (op << 3) | lastbyte
 * 	c: op | lastbyte
 * 	d: lastbyte = op as 8 bit value
 * 	e: output op as word (no '.' sould follow)
 * 	f: (op << 1) + lastbyte
 * 	g: (op << 4) | lastbyte
 */

const struct matchtab s_matchtab_dp2200[] = {
	{ "HALT", "00." },
	{ "SLC", "02." },
	{ "SRC", "0A." },
	{ "RETURN", "07." },
	{ "INPUT", "41." },
	{ "b", "80c0." },
	{ "c", "C0c0." },
	{ "EX d", "51f0." },
	{ "e", "03g0." },
	{ "f", "0Bg0." },
	{ "g a", "04g0.d1." },
	{ "h a", "06g0.d1." },
	{ "i a", "0Cg0.d1." },
	{ "j a", "0Eg0.d1." },
	{ "k a", "40g0.e1" },
	{ "l a", "42g0.e1" },
	{ "m a", "48g0.e1" },
	{ "n a", "4Ag0.e1" },
	{ "JMP a", "44.e0" },
	{ "CALL a", "46.e0" },
	// { "BETA", "10." },
	// { "DI", "20." },
	// { "POP", "30." },
	// { "ALPHA", "18." },
	// { "EI", "28." },
	// { "PUSH", "38." },
	{ NULL, NULL },
};

static const char *const bval[] = {
"ADA", "ADB", "ADC", "ADD", "ADE", "ADH", "ADL", "ADM",
"ACA", "ACB", "ACC", "ACD", "ACE", "ACH", "ACL", "ACM",
"SUA", "SUB", "SUC", "SUD", "SUE", "SUH", "SUL", "SUM",
"SBA", "SBB", "SBC", "SBD", "SBE", "SBH", "SBL", "SBM",
"NDA", "NDB", "NDC", "NDD", "NDE", "NDH", "NDL", "NDM",
"XRA", "XRB", "XRC", "XRD", "XRE", "XRH", "XRL", "XRM",
"ORA", "ORB", "ORC", "ORD", "ORE", "ORH", "ORL", "ORM",
"CPA", "CPB", "CPC", "CPD", "CPE", "CPH", "CPL", "CPM",
NULL };

static const char *const cval[] = {
"NOP", "LAB", "LAC", "LAD", "LAE", "LAH", "LAL", "LAM",
"LBA", "",    "LBC", "LBD", "LBE", "LBH", "LBL", "LBM",
"LCA", "LCB", "",    "LCD", "LCE", "LCH", "LCL", "LCM",
"LDA", "LDB", "LDC", "",    "LDE", "LDH", "LDL", "LDM",
"LEA", "LEB", "LEC", "LED", "",    "LEH", "LEL", "LEM",
"LHA", "LHB", "LHC", "LHD", "LHE", "",    "LHL", "LHM",
"LLA", "LLB", "LLC", "LLD", "LLE", "LLH", "",    "LLM",
"LMA", "LMB", "LMC", "LMD", "LME", "LMH", "LML",
NULL };

static const char *const dval[] = {
"ADR", "STATUS", "DATA", "WRITE", "COM1", "COM2",  "COM3",  "COM4",
"",    "",       "",     "",      "BEEP", "CLICK", "DECK1", "DECK2", 
"RBK", "WBK",    "",     "BSP",   "SF",   "SB",    "REWND", "TSTOP", 
NULL };

static const char *const eval[] = { "RFC", "RFS", "RTC", "RTS", NULL };
static const char *const fval[] = { "RFZ", "RFP", "RTZ", "RTP", NULL };
static const char *const gval[] = { "AD", "SU", "ND", "OR", NULL };
static const char *const hval[] = { "LA", "LC", "LE", "LL", NULL };
static const char *const ival[] = { "AC", "SB", "XR", "CP", NULL };
static const char *const jval[] = { "LB", "LD", "LH", NULL };
static const char *const kval[] = { "JFC", "JFS", "JTC", "JTS", NULL };
static const char *const lval[] = { "CFC", "CFS", "CTC", "CTS", NULL };
static const char *const mval[] = { "JFZ", "JFP", "JTZ", "JTP", NULL };
static const char *const nval[] = { "CFZ", "CFP", "CTZ", "CTP", NULL };
				  
static const char *const *const valtab[] = { 
	bval, cval, dval, eval, fval,
       	gval, hval, ival, jval, kval,
	lval, mval, nval
};

static int match_dp2200(char c, const char *p, const char **q)
{
	int v;

	if (c <= 'n') {
		v = mreg(p, valtab[(int) (c - 'b')], q);
	} else {
		v = -1;
	}

	return v;
}

static int gen_dp2200(int *eb, char p, const int *vs, int i, int savepc)
{
	int b;
       
	b = *eb;
	switch (p) {
	case 'f': b += (vs[i] << 1); break;
	case 'g': b |= (vs[i] << 4); break;
	default:
		  return -1;
	}

	*eb = b;
	return 0;
}

static int s_pat_char = 'b';
static int s_pat_index;

static void pat_char_rewind_dp2200(int c)
{
	s_pat_char = c;
	s_pat_index = 0;
};

static const char *pat_next_str_dp2200(void)
{
	const char *s;

	if (s_pat_char >= 'b' && s_pat_char <= 'n') {
		s = valtab[(int) (s_pat_char - 'b')][s_pat_index];
		if (s != NULL) {
			s_pat_index++;
		}
	} else {
		s = NULL;
	}

	return s;
};

const struct target s_target_dp2200 = {
	"dp2200",
	"Datapoint 2200",
	s_matchtab_dp2200,
	match_dp2200,
	gen_dp2200,
	pat_char_rewind_dp2200,
	pat_next_str_dp2200
};
