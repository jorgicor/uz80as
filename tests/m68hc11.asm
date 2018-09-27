; ===========================================================================
; uz80as, an assembler for the Zilog Z80 and several other microprocessors.
;
; Motorola 68HC11.
; ===========================================================================


#define equ .equ
#define end .end

.MSFIRST

n:	equ $77
nn:	equ $1234

; 0x

	TEST ; **
	NOP
	IDIV ; **
	FDIV ; **
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
	BRSET n,n,* ; **
	BRCLR n,n,* ; **
	BSET n,n ; **
	BCLR n,n ; **
	TAB
	TBA
	; 8
	DAA
	; A
	ABA
	BSET n,X,n ; **
	BCLR n,X,n ; **
	BRSET n,X,n,* ; **
	BRCLR n,X,n,* ; **

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
	LSLA ; ** = ASLA
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
	LSLB ; ** = ASLB
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
	JMP n,X
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
	JMP nn
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
	XGDX ; **

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
	STOP ; **

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

; 18xx

	ABY ; 3A
	ADCA n,Y ; A9
	ADCB n,Y ; E9
	ADDA n,Y ; AB
	ADDB n,Y ; EB
	ADDD n,Y ; E3
	ANDA n,Y ; A4
	ANDB n,Y ; E4
	ASL n,Y ; 68
	ASR n,Y ; 67
	BCLR n,Y,n ; 1D
	BITA n,Y ; A5
	BITB n,Y ; E5
	BRCLR n,Y,n,* ; 1F
	BRSET n,Y,n,* ; 1E
	BSET n,Y,n ; 1C
	CLR n,Y ; 6F
	CMPA n,Y ; A1
	CMPB n,Y ; E1
	COM n,Y ; 63
	CPY #n ; 8C
	CPY n ; 9C
	CPY nn ; BC
	CPY n,Y ; AC
	DEC n,Y ; 6A
	DEY ; 09
	EORA n,Y ; A8
	EORB n,Y ; E8
	INC n,Y ; 6C
	INY ; 08
	JMP n,Y ; 6E
	JSR n,Y ; AD
	LDAA n,Y ; A6
	LDAB n,Y ; E6
	LDD n,Y ; EC
	LDS n,Y ; AE
	LDY #n ; CE
	LDY n ; DE
	LDY nn ; FE
	LDY n,Y ; EE
	LSL n,Y ; 68
	LSR n,Y ; 64
	NEG n,Y ; 60
	ORAA n,Y ; AA
	ORAB n,Y ; EA
	PSHY ; 3C
	PULY ; 38
	ROL n,Y ; 69
	ROR n,Y ; 66
	SBCA n,Y ; A2
	SBCB n,Y ; E2
	STAA n,Y ; A7
	STAB n,Y ; E7
	STD n,Y ; ED
	STS n,Y ; AF
	STY n ; DF
	STY nn ; FF
	STY >n ; FF
	STY n,Y ; EF
	SUBA n,Y ; A0
	SUBB n,Y ; E0
	SUBD n,Y ; A3
	TST n,Y ; 6D
	TSY ; 30
	TYS ; 35
	XGDY ; 8F

; 1Axx

	CPD #nn ; 83
	CPD n ; 93
	CPD nn ; B3
	CPD n,X ; A3
	CPY n,X ; AC
	LDY n,X ; EE
	STY n,X ; EF

; CDxx

	CPD n,Y ; A3
	CPX n,Y ; AC
	LDX n,Y ; EE
	STX n,Y ; EF

#ifdef UZ80AS

; 1Axx

	CPD >n ; B3

#endif

	end
