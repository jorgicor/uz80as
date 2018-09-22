; ===========================================================================
; uz80as, an assembler for the Zilog Z80 and several other microprocessors.
;
; Motorola 6800.
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
	; 4
	; 5
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
	; 1
	BHI *
	BLS *
	BCC *
	BCS *
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
	; 8
	RTS
	; A
	RTI
	; C
	; D
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
	; 3
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
	; 3
	ANDA n
	BITA n
	LDAA n
	STAA n
	EORA n
	ADCA n
	ORAA n
	ADDA n
	CPX n
	; D
	LDS n
	STS n

; Ax

	SUBA n,X
	CMPA n,X
	SBCA n,X
	; 3
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
	; 3
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
	; 3
	ANDB #n
	BITB #n
	LDAB #n
	; 7
	EORB #n
	ADCB #n
	ORAB #n
	ADDB #n
	; C
	; D
	LDX #nn
	; F

; Dx

	SUBB n
	CMPB n
	SBCB n
	; 3
	ANDB n
	BITB n
	LDAB n
	STAB n
	EORB n
	ADCB n
	ORAB n
	ADDB n
	; C
	; D
	LDX n
	STX n

; Ex

	SUBB n,X
	CMPB n,X
	SBCB n,X
	; 3
	ANDB n,X
	BITB n,X
	LDAB n,X
	STAB n,X
	EORB n,X
	ADCB n,X
	ORAB n,X
	ADDB n,X
	; C
	; D
	LDX n,X
	STX n,X


; Fx

	SUBB nn
	CMPB nn
	SBCB nn
	; 3
	ANDB nn
	BITB nn
	LDAB nn
	STAB nn
	EORB nn
	ADCB nn
	ORAB nn
	ADDB nn
	; C
	; D
	LDX nn
	STX nn

#ifdef UZ80AS

; Bx >

	SUBA >n
	CMPA >n
	SBCA >n
	; 3
	ANDA >n
	BITA >n
	LDAA >n
	STAA >n
	EORA >n
	ADCA >n
	ORAA >n
	ADDA >n
	CPX >n
	JSR nn
	LDS >n
	STS >n

; Fx >

	SUBB >n
	CMPB >n
	SBCB >n
	; 3
	ANDB >n
	BITB >n
	LDAB >n
	STAB >n
	EORB >n
	ADCB >n
	ORAB >n
	ADDB >n
	; C
	; D
	LDX >n
	STX >n

#endif

	.DW $1234

	end
