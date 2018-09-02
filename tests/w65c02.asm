; ===========================================================================
; uz80as, an assembler for the Zilog Z80 and several other microprocessors.
;
; Western Design W65C02.
; ===========================================================================


#define equ .equ
#define end .end

zp:	    equ $77
a16:        equ $1234
n:          equ 56h

; 0x

	BRK
	ORA (zp,X)
	; 2
	; 3
	TSB zp ; *
	ORA zp
	ASL zp
	; 7
	PHP
	ORA #n
	ASL A
	; B
	TSB a16 ; *
	ORA a16
	ASL a16
	; F

; 1x

b10:	BPL b10
	ORA (zp),Y
	ORA (zp) ; *
	; 3
	TRB zp ; *
	ORA zp,X
	ASL zp,X
	; 7
	CLC
	ORA a16,Y
	INC A ; *
	; B
	TRB a16 ; *
	ORA a16,X
	ASL a16,X
	; F
	
; 2x

	JSR a16
	AND (zp,X)
	; 2
	; 3
	BIT zp
	AND zp
	ROL zp
	; *
	PLP
	AND #n
	ROL A
	; B
	BIT a16
	AND a16
	ROL a16
	; F

; 3x

b30:	BMI b30
	AND (zp),Y
	AND (zp) ; *
	; 3
	BIT zp,X ; *
	AND zp,X
	ROL zp,X
	; 7
	SEC
	AND a16,Y
	DEC A ; *
	; B
	BIT a16,X ; *
	AND a16,X
	ROL a16,X
	; F

; 4x

	RTI
	EOR (zp,X)
	; 2
	; 3
	; 4
	EOR zp
	LSR zp
	; 7
	PHA
	EOR #n
	LSR A
	; B
	JMP a16
	EOR a16
	LSR a16
	; F

; 5x

b50:	BVC b50
	EOR (zp),Y
	EOR (zp) ; *
	; 3
	; 4
	EOR zp,X
	LSR zp,X
	; 7
	CLI
	EOR a16,Y
	PHY ; *
	; B
	; C
	EOR a16,X
	LSR a16,X
	; F

; 6x

	RTS
	ADC (zp,X)
	; 2
	; 3
	STZ zp ; *
	ADC zp
	ROR zp
	; 7
	PLA
	ADC #n
	ROR A
	; B
	JMP (a16)
	ADC a16
	ROR a16
	; F

; 7x

b70:	BVS b70
	ADC (zp),Y
	ADC (zp) ; *
	; 3
	STZ zp,X ; *
	ADC zp,X
	ROR zp,X
	; 7
	SEI
	ADC a16,Y
	PLY ; *
	; B
	JMP (a16,X) ; *
	ADC a16,X
	ROR a16,X
	; *

; 8x

b80:	BRA b80 ; *
	STA (zp,X)
	; 2
	; 3
	STY zp
	STA zp
	STX zp
	; 7
	DEY
	BIT #n ; *
	TXA
	; B
	STY a16
	STA a16
	STX a16
	; F

; 9x

b90:	BCC b90
	STA (zp),Y
	STA (zp) ; *
	; 3
	STY zp,X
	STA zp,X
	STX zp,Y
	; 7
	TYA
	STA a16,Y
	TXS
	; B
	; C
	STA a16,X
	STZ a16,X ; *
	; F

; Ax

	LDY #n
	LDA (zp,X)
	LDX #n
	; 3
	LDY zp
	LDA zp
	LDX zp
	; 7
	TAY
	LDA #n
	TAX
	; B
	LDY a16
	LDA a16
	LDX a16
	; *

; Bx

bB0:	BCS bB0
	LDA (zp),Y
	LDA (zp) ; *
	; 3
	LDY zp,X
	LDA zp,X
	LDX zp,Y
	; 7
	CLV
	LDA a16,Y
	TSX
	; B
	LDY a16,X
	LDA a16,X
	LDX a16,Y
	; *

; Cx

	CPY #n
	CMP (zp,X)
	; 2
	; 3
	CPY zp
	CMP zp
	DEC zp
	; 7
	INY
	CMP #n
	DEX
	; B
	CPY a16
	CMP a16
	DEC a16
	; *

; Dx

bD0:	BNE bD0
	CMP (zp),Y
	CMP (zp) ; *
	; 3
	; 4
	CMP zp,X
	DEC zp,X
	; 7
	CLD
	CMP a16,Y
	PHX ; *
	; B 
	; C
	CMP a16,X
	DEC a16,X
	; *

; Ex

	CPX #n
	SBC (zp,X)
	; 2
	; 3
	CPX zp
	SBC zp
	INC zp
	; 7
	INX
	SBC #n
	NOP
	; B
	CPX a16
	SBC a16
	INC a16
	; *

; Fx

bF0:	BEQ bF0
	SBC (zp),Y
	SBC (zp) ; *
	; 3
	; 4
	SBC zp,X
	INC zp,X
	; 7
	SED
	SBC a16,Y
	PLX ; *
	; B
	; C
	SBC a16,X
	INC a16,X
	; *

	end
