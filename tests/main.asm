; ===========================================================================
; uz80as, a macro assembler for Z80 based microprocessors.
; ===========================================================================

; All this file must equal the TASM output

	.org 0

; ----------------
; Test expressions
; ----------------

; This additionally tests a db that must span 3 lines in the lst file.

	.db 0, 127, 128, 255
	.db 256,-1,-127,-128
	.db -255, +0, +255
	.db 1+256, +1+255, -1+256, +1-256
	.db 1-256, -1-256
	.db 2*3, 2*(-3), -2*3, -2*(-3)

; tasm fails for these

;	.db 2*-3, -2*-3

	.db +-1, --1, ---1, +--1

; tasm fails for this

;	.db -+1
; 	.db 1/0

	.db 5+7-3*5/2
	.db 0/256, 0*256, 0*0, 255/-1
	.db 0%5, 256%8

	.db 5==5, 5!=5, 5==6, 5!=6, 5=5, 5=6
	.db 5>5, 5>6, 6>5
	.db 5<5, 5<6, 6<5
	.db 5>=5, 5>=6, 6>=5
	.db 5<=5, 5<=6, 6<=5
	.db 5==-5, -5==5, -5==-5
	.db 5=-5, -5=5, -5=-5
	.db 5!=-5, -5!=5, -5!=-5
	.db 5>-5, -5>5, -5>-5
	.db 5<-5, -5<5, -5<-5
	.db 5<=-5, -5<=5, -5<=-5
	.db 5>=-5, -5>=5, -5>=-5
	.db 5&5, 5&6, 5|5, 5|6, 5^5, 5^6
	.db 255>>1, 255>>7, 255>>8, 256>>8
	.db 1<<1, 1<<7, 1<<8, 255<<8
	.db 1>>0, 1<<0, 1>>-1, 1<<-1
	.db ~0, ~1, ~255, ~256
	.db 1+(1+(1+(1+(1+(1+(1+(1+(1+(1)))))))))
	.db 1-(1+(1-(1+(1-(1+(1-(1+(1-(1)))))))))

; More shifting.

	;       1      30      31      0       1
	.db -1>>1, -1>>30, -1>>31, -1>>32, -1>>33

	;              1             30             31
	.db $7FFFFFFF>>1, $7FFFFFFF>>30, $7FFFFFFF>>31

	; These two give $FF for TASM on Linux 64 bits (long is 64 bits)
	; They must be 0 since we say that we use 32 bit integers.
        ;              0              1
	; .db $7FFFFFFF>>32, $7FFFFFFF>>33

	;       31      2        0        0        31
	.db -1>>-1, -1>>-30, -1>>-31, -1>>-32, -1>>-33

	;              31             31
	.db $7FFFFFFF>>-1, $7FFFFFFF>>-33

	; These two give $FF for TASM on Linux 64 bits (long is 64 bits)
	; They must be 0 since we say that we use 32 bit integers.
	;              2               0                   0
	; .db $7FFFFFFF>>-30, $7FFFFFFF>>-31, $7FFFFFFF>>-32

	.db -1>>0, -1<<0, 1>>0, 1<<0

; Prefixes and suffixes

	.db %0, 0b, 0B
	.db %1, 1b, 1B
	.db %00000000, 00000000b, 00000000B
	.db %11111111, 11111111b, 11111111B
	.db %111111111, 111111111b, 111111111B
	.db %10101010, 10101010b, 10101010B

	.db @0, 0o, 0O
	.db @1, 1o, 1O
	.db @377, 377o, 377O
	.db @477, 477o, 477O
	.db @142, 142o, 142O

	.db 0d, 0D, 1d, 1D
	.db 255d, 255D, 256d, 256D
	.db -1d, -1D, -128d, -128D

	.db $0, 0h, 0H
	.db $1, 1h, 1H
	.db $ff, $FF, 0ffh, 0FFH
	.db 0ffH, 0FFh, $100
	.db 100h, 100H
	.db 1eH, 0E1h

	.db 3%2, 3%%10
	.db 255-$, 255-$ff

; DW, WORD

	.dw 0, 255, -255, 256
	.dw 127, 128, -127, -128
	.dw 65535, 65536, -1, -65536
	.dw -65537, 32767, 32768, -32767
	.word -32768
	.dw '0', '9'-'0', 'z'-'a', 'Z'-'A'

; BLOCK

	.block -1
	.block 1
	.block 1
	.db 2
	.block -2
	.block 2
	.ds 2	; ds is equal to block

; BYTE

	.byte 0, 1, 255, 256
	.byte -1, -128

; CODES, NOCODES

	.nocodes

; Printed as is --------

; Comment column 0

	.db 0, 1, 2, 3

; ----------------------

	.codes

; Printed after line number, etc.

; ECHO accepted.

	.echo "This is a string "
	.echo 127+1
	.echo " concatenated.\n"

; FILL .

	.fill 0, 1
	.fill 0, -1
	.fill 0
	.fill 1, 1
	.fill 1, -1
	.fill 1
	.fill 2
	.fill 3
	.fill 4
	.fill 5
	.fill 6
	.fill 7
	.fill 8
	.fill 9

; LIST, NOLIST

	.nolist

; This does not appear in the listing.

	.list

; This appears.

; LSFIRST, MSFIRST

	.word $1234
	.msfirst
	.word $1234
	.lsfirst
	.word $1234

; ORG simple.

	.org $
	.org $+1
	.org $-1
	.org *+1
	.org *-1

; ORG forms.

*=$+1
$=*+1
	*=*+1
	$=$+1
	$= $+1
	*= *+1

	.db 128

; Testing * (current program counter) and multiply operator

	.db **2

; Not correctly parsed by TASM.

;	.db ***
;	.db 2**

; Test label in org: must take value after the org.

orglab
orglab1	.org $+127
orglab2	.dw orglab1
orglab3
orglab4 .org $+1
	.dw orglab
	.dw orglab3
	.dw orglab4

; EQU

label1		.equ 1
label2:		.equ 2
label3_		.equ 3
label4_:	.equ 4
label5		=5
label6:		=6
label7_		=7
label8_:	=8
label9.		=9
labela.:	=10
labelb		= 11

; PAGE/NOPAGE

; TEXT

	.text "\n\r\b\t\f\\\""
	.text "azAZ09"
	.text "\000\377\135"

; DB, DW with text.

	
	.db "\n\r\b\t\f\\\""
	.db "azAZ09"
	.db "\000\377\135"
	.dw "\n\r\b\t\f\\\""
	.dw "azAZ09"
	.dw "\000\135"
	.db "Hi", 1, 2, "Bye"
	.dw "Hi", 1, 2, "Bye"

; This gives a different result in TASM.

;	.dw "\377"

; Trailing commas.

	.db 1, 2,
	.db "ab", "cd",
	.db 1, 2 ; comment
	.db "ab", "cd", ;comment

; Mixed case directives.

	.dB 0
	.Db 1

; Directives in column 0.

.db 2
.dB 3
.DB 4

; Label names.

_
L
_L9._
LL9._
la9._
l

; Line configurations.

; comment
	; comment
;
	;
L10
L11	.db 0
L12	;comment
L13.db	.db 0
L14
L15	.db 0	; comment
L16	.db 0;comment
P10:
P11:	.db 0
P12:	;comment
P13.db:	.db 0
P14:
P15:	.db 0	; comment
P16:	.db 0;comment

; This should fail if uncommented.

;label12;comment

; Multiline scapes

	.DB 5\ .DB 6\.DB 7
ML1	\ML2 .DB 1

; Preprocessor

#define Mm9._
#define _Mm9._
	#define MAC
; #define MOC
#define ONE	1
#define TWO	(1+1)

; TASM allows for this
; #define BAD%	3
;	.db BAD%

#define SUM1(n)	(n+ONE)

	.db SUM1(5)

#define MUL(a,b)	a * b

	.db MUL(5,6)

#define VOID(a,b)

VOID
VOID(5)
VOID(5,6)

#define	ARG1(a,b,c)	a

	.db ARG1(7)

#define	ARG2(a,b,c)	b

	.db ARG2(1,8)

; TASM doesn't allow _a, _7 _. for arguments
; #define ALLOW(a,ab,_a,_7,_.)	a+ab+_a+_7+_.
;	.db ALLOW(1,2,3,4,5)

#define ALLOW(a,a7,a7b7)	a+a7+a7b7

	.db ALLOW(1,2,3)
	.db ALLOW( 1 , 2 , 3 )

#define CONT	5+
#defcont	6+
#defcont	7

	.db CONT

#define	MULTILINE	.DB 1\ .DB 2\
#defcont		.DB 3\
#defcont		.DB 4

	MULTILINE

#define INCL

#ifdef INCL
	.db 1
#endif
#ifndef INCL
	.db 2
#endif
#ifdef INCL
	.db 8
#else
	.db 16
#endif
#ifndef INCL
	.db 32
#else
	.db 64
#endif

#ifdef NINCL
	.db 1
#endif
#ifndef NINCL
	.db 2
#endif
#ifdef NINCL
	.db 8
#else
	.db 16
#endif
#ifndef NINCL
	.db 32
#else
	.db 64
#endif

#ifdef INCL
	#ifdef NINCL
		.db 1
	#else
		.db 2
	#endif
	#ifndef NINCL
		.db 4
	#else
		.db 8
	#endif
#else
	#ifdef NINCL
		.db 16
	#else
		.db 32
	#endif
	#ifndef NINCL
		.db 64
	#else
		.db 128
	#endif
#endif

#ifndef INCL
	#ifdef NINCL
		.db 1
	#else
		.db 2
	#endif
	#ifndef NINCL
		.db 4
	#else
		.db 8
	#endif
#else
	#ifdef NINCL
		.db 16
	#else
		.db 32
	#endif
	#ifndef NINCL
		.db 64
	#else
		.db 128
	#endif
#endif

prgend	.end	prgend
