; ===========================================================================
; uz80as, an assembler for the Zilog Z80 and several other microprocessors.
;
; Motorola 6801.
; ===========================================================================


#define equ .equ
#define end .end

.MSFIRST

n:	equ $77
nn:	equ $1234

; 0x

	; 0
	NOP
	; 2
	; 3
	LSRD ; *
	ASLD ; *
	LSLD ; * = ASLD
	TAP
	TPA
	INX
	DEX
	CLV
	SEV
	CLC
	SEC
	CLI
	SEI
	
; 1x

	SBA
	CBA
	; 2
	; 3
	; 4
	; 5
	TAB
	TBA
	; 8
	DAA
	; A
	ABA
	; C
	; D
	; E
	; F

; 2x

	BRA *
	BRN * ; *
	BHI *
	BLS *
	BCC *
	BHS * ; * = BCC
	BCS *
	BLO * ; * = BCS
	BNE *
	BEQ *
	BVC *
	BVS *
	BPL *
	BMI *
	BGE *
	BLT *
	BGT *
	BLE *

; 3x

	TSX
	INS
	PULA
	PULB
	DES
	TXS
	PSHA
	PSHB
	PULX ; *
	RTS
	ABX ; *
	RTI
	PSHX ; *
	MUL ; *
	WAI
	SWI

; 4x

	NEGA
	; 1
	; 2
	COMA
	LSRA
	; 5
	RORA
	ASRA
	ASLA
	ROLA
	DECA
	; B
	INCA
	TSTA
	; E
	CLRA

; 5x

	NEGB
	; 1
	; 2
	COMB
	LSRB
	; 5
	RORB
	ASRB
	ASLB
	ROLB
	DECB
	; B
	INCB
	TSTB
	; E
	CLRB

; 6x

	NEG n,X
	; 1
	; 2
	COM n,X
	LSR n,X
	; 5
	ROR n,X
	ASR n,X
	ASL n,X
	LSL n,X ; * = ASL
	ROL n,X
	DEC n,X
	; B
	INC n,X
	TST n,X
	; E
	CLR n,X

; 7x

	NEG nn
	; 1
	; 2
	COM nn
	LSR nn
	; 5
	ROR nn
	ASR nn
	ASL nn
	LSL nn ; * = ASL
	ROL nn
	DEC nn
	; B
	INC nn
	TST nn
	; E
	CLR nn

; 8x

	SUBA #n
	CMPA #n
	SBCA #n
	SUBD #nn ; *
	ANDA #n
	BITA #n
	LDAA #n
	; 7
	EORA #n
	ADCA #n
	ORAA #n
	ADDA #n
	CPX #nn
	BSR *
	LDS #nn
	; F

; 9x

	SUBA n
	CMPA n
	SBCA n
	SUBD n ; *
	ANDA n
	BITA n
	LDAA n
	STAA n
	EORA n
	ADCA n
	ORAA n
	ADDA n
	CPX n
	JSR n ; *
	LDS n
	STS n

; Ax

	SUBA n,X
	CMPA n,X
	SBCA n,X
	SUBD n,X ; *
	ANDA n,X
	BITA n,X
	LDAA n,X
	STAA n,X
	EORA n,X
	ADCA n,X
	ORAA n,X
	ADDA n,X
	CPX n,X
	JSR n,X
	LDS n,X
	STS n,X


; Bx

	SUBA nn
	CMPA nn
	SBCA nn
	SUBD nn ; *
	ANDA nn
	BITA nn
	LDAA nn
	STAA nn
	EORA nn
	ADCA nn
	ORAA nn
	ADDA nn
	CPX nn
	JSR nn
	LDS nn
	STS nn

; Cx

	SUBB #n
	CMPB #n
	SBCB #n
	ADDD #nn ; *
	ANDB #n
	BITB #n
	LDAB #n
	; 7
	EORB #n
	ADCB #n
	ORAB #n
	ADDB #n
	LDD #nn ; *
	; D
	LDX #nn
	; F

; Dx

	SUBB n
	CMPB n
	SBCB n
	ADDD n ; *
	ANDB n
	BITB n
	LDAB n
	STAB n
	EORB n
	ADCB n
	ORAB n
	ADDB n
	LDD n ; *
	STD n ; *
	LDX n
	STX n

; Ex

	SUBB n,X
	CMPB n,X
	SBCB n,X
	ADDD n,X ; *
	ANDB n,X
	BITB n,X
	LDAB n,X
	STAB n,X
	EORB n,X
	ADCB n,X
	ORAB n,X
	ADDB n,X
	LDD n,X ; *
	STD n,X ; *
	LDX n,X
	STX n,X


; Fx

	SUBB nn
	CMPB nn
	SBCB nn
	ADDD nn ; *
	ANDB nn
	BITB nn
	LDAB nn
	STAB nn
	EORB nn
	ADCB nn
	ORAB nn
	ADDB nn
	LDD nn ; *
	STD nn ; *
	LDX nn
	STX nn

#ifdef UZ80AS

; Bx >

	SUBA >n
	CMPA >n
	SBCA >n
	SUBD >n ; *
	ANDA >n
	BITA >n
	LDAA >n
	STAA >n
	EORA >n
	ADCA >n
	ORAA >n
	ADDA >n
	CPX >n
	JSR >n ; *
	LDS >n
	STS >n

; Fx >

	SUBB >n
	CMPB >n
	SBCB >n
	ADDD >n ; *
	ANDB >n
	BITB >n
	LDAB >n
	STAB >n
	EORB >n
	ADCB >n
	ORAB >n
	ADDB >n
	LDD >n ; *
	STD >n ; *
	LDX >n
	STX >n

#endif

	.DW $1234

	end
