; ===========================================================================
; uz80as, an assembler for the Zilog Z80 and several other microprocessors.
;
; RCA 1802.
; ===========================================================================

#define equ .equ
#define end .end

.MSFIRST

n:          equ 20h
nn:         equ 0584h
dddd:       equ 07h
a16:        equ $1234
hi8:        equ $ff11
port:       equ 3
imm8:       equ 56h    ;immediate data (8 bits)
offset:     equ 7
offset_neg: equ -7

; 0x

	IDL ; LDN 0
	LDN 1
	LDN 2
	LDN 3
	LDN 4
	LDN 5
	LDN 6
	LDN 7
	LDN 8
	LDN 9
	LDN $A
	LDN $B
	LDN $C
	LDN $D
	LDN $E
	LDN $F

; 1x

	INC 0
	INC 1
	INC 2
	INC 3
	INC 4
	INC 5
	INC 6
	INC 7
	INC 8
	INC 9
	INC $A
	INC $B
	INC $C
	INC $D
	INC $E
	INC $F

; 2x

	DEC 0
	DEC 1
	DEC 2
	DEC 3
	DEC 4
	DEC 5
	DEC 6
	DEC 7
	DEC 8
	DEC 9
	DEC $A
	DEC $B
	DEC $C
	DEC $D
	DEC $E
	DEC $F

; 3x

	BR a16
	BQ a16
	BZ a16
	BDF a16
	BPZ a16 ; *
	BGE a16 ; *
	B1 a16
	B2 a16
	B3 a16
	B4 a16
	SKP	; *
	NBR a16
	BNQ a16
	BNZ a16
	BNF a16
	BM a16	; *
	BL a16	; *
	BN1 a16
	BN2 a16
	BN3 a16
	BN4 a16

; 4x

	LDA 0
	LDA 1
	LDA 2
	LDA 3
	LDA 4
	LDA 5
	LDA 6
	LDA 7
	LDA 8
	LDA 9
	LDA $A
	LDA $B
	LDA $C
	LDA $D
	LDA $E
	LDA $F

; 5x

	STR 0
	STR 1
	STR 2
	STR 3
	STR 4
	STR 5
	STR 6
	STR 7
	STR 8
	STR 9
	STR $A
	STR $B
	STR $C
	STR $D
	STR $E
	STR $F

; 6x

	IRX
	OUT 1
	OUT 2
	OUT 3
	OUT 4
	OUT 5
	OUT 6
	OUT 7
	;
	INP 1
	INP 2
	INP 3
	INP 4
	INP 5
	INP 6
	INP 7

; 7x

	RET
	DIS
	LDXA
	STXD
	ADC
	SDB
	SHRC
	RSHR	; *
	SMB
	SAV
	MARK
	REQ
	SEQ
	ADCI n	
	SDBI n
	SHLC
	RSHL
	SMBI n

; 8x

	GLO 0
	GLO 1
	GLO 2
	GLO 3
	GLO 4
	GLO 5
	GLO 6
	GLO 7
	GLO 8
	GLO 9
	GLO $A
	GLO $B
	GLO $C
	GLO $D
	GLO $E
	GLO $F

; 9x

	GHI 0
	GHI 1
	GHI 2
	GHI 3
	GHI 4
	GHI 5
	GHI 6
	GHI 7
	GHI 8
	GHI 9
	GHI $A
	GHI $B
	GHI $C
	GHI $D
	GHI $E
	GHI $F

; Ax

	PLO 0
	PLO 1
	PLO 2
	PLO 3
	PLO 4
	PLO 5
	PLO 6
	PLO 7
	PLO 8
	PLO 9
	PLO $A
	PLO $B
	PLO $C
	PLO $D
	PLO $E
	PLO $F

; Bx

	PHI 0
	PHI 1
	PHI 2
	PHI 3
	PHI 4
	PHI 5
	PHI 6
	PHI 7
	PHI 8
	PHI 9
	PHI $A
	PHI $B
	PHI $C
	PHI $D
	PHI $E
	PHI $F

; Cx

	LBR a16
	LBQ a16
	LBZ a16
	LBDF a16
	NOP
	LSNQ
	LSNZ
	LSNF
	LSKP		; *
	NLBR a16
	LBNQ a16
	LBNZ a16
	LBNF a16
	LSIE
	LSQ
	LSZ
	LSDF

; Dx

	SEP 0
	SEP 1
	SEP 2
	SEP 3
	SEP 4
	SEP 5
	SEP 6
	SEP 7
	SEP 8
	SEP 9
	SEP $A
	SEP $B
	SEP $C
	SEP $D
	SEP $E
	SEP $F

; Ex

	SEX 0
	SEX 1
	SEX 2
	SEX 3
	SEX 4
	SEX 5
	SEX 6
	SEX 7
	SEX 8
	SEX 9
	SEX $A
	SEX $B
	SEX $C
	SEX $D
	SEX $E
	SEX $F

; Fx

	LDX
	OR
	AND
	XOR
	ADD
	SD
	SHR
	SM
	LDI n
	ORI n
	ANI n
	XRI n
	ADI n
	SDI n
	SHL
	SMI n

	end
