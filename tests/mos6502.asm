; ===========================================================================
; uz80as, an assembler for the Zilog Z80 and several other microprocessors.
;
; MOS Technology 6502.
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
	; 4
	ORA zp
	ASL zp
	; 7
	PHP
	ORA #n
	ASL A
	; B
	; C
	ORA a16
	ASL a16
	; F

; 1x

	BPL *
	ORA (zp),Y
	; 2
	; 3
	; 4
	ORA zp,X
	ASL zp,X
	; 7
	CLC
	ORA a16,Y
	; A
	; B
	; C
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
	; 7
	PLP
	AND #n
	ROL A
	; B
	BIT a16
	AND a16
	ROL a16
	; F

; 3x

	BMI *
	AND (zp),Y
	; 2
	; 3
	; 4
	AND zp,X
	ROL zp,X
	; 7
	SEC
	AND a16,Y
	; A
	; B
	; C
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

	BVC *
	EOR (zp),Y
	; 2
	; 3
	; 4
	EOR zp,X
	LSR zp,X
	; 7
	CLI
	EOR a16,Y
	; A
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
	; 4
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

	BVS *
	ADC (zp),Y
	; 2
	; 3
	; 4
	ADC zp,X
	ROR zp,X
	; 7
	SEI
	ADC a16,Y
	; A
	; B
	; C
	ADC a16,X
	ROR a16,X
	; F

; 8x

	; 0
	STA (zp,X)
	; 2
	; 3
	STY zp
	STA zp
	STX zp
	; 7
	DEY
	; 9
	TXA
	; B
	STY a16
	STA a16
	STX a16
	; F

; 9x

	BCC *
	STA (zp),Y
	; 2
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
	; E
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
	; F

; Bx

	BCS *
	LDA (zp),Y
	; 2
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
	; F

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
	; F

; Dx

	BNE *
	CMP (zp),Y
	; 2
	; 3
	; 4
	CMP zp,X
	DEC zp,X
	; 7
	CLD
	CMP a16,Y
	; A
	; B
	; C
	CMP a16,X
	DEC a16,X
	; F

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
	; F

; Fx

	BEQ *
	SBC (zp),Y
	; 2
	; 3
	; 4
	SBC zp,X
	INC zp,X
	; 7
	SED
	SBC a16,Y
	; A
	; B
	; C
	SBC a16,X
	INC a16,X
	; F

; Forcing 16 bits
; 0x

	; 1
	; 2
	; 2
	; 3
	; 4
	; 5
	; 6
	; 7
	; 8
	; 9
	; A
	; B
	; C
	ORA >zp
	ASL >zp
	; F

; 1x

	; 0
	; 1
	; 2
	; 3
	; 4
	; 5
	; 6
	; 7
	; 8
	; 9
	; A
	; B
	; C
	ORA >zp,X
	ASL >zp,X
	; F
	
; 2x

	; 0
	; 1
	; 2
	; 3
	; 4
	; 5
	; 6
	; 7
	; 8
	; 9
	; A
	; B
	BIT >zp
	AND >zp
	ROL >zp
	; F

; 3x

	; 0
	; 1
	; 2
	; 3
	; 4
	; 5
	; 6
	; 7
	; 8
	; 9
	; A
	; B
	; C
	AND >zp,X
	ROL >zp,X
	; F

; 4x

	; 1
	; 2
	; 2
	; 3
	; 4
	; 5
	; 6
	; 7
	; 8
	; 9
	; A
	; B
	; C
	EOR >zp
	LSR >zp
	; F

; 5x

	; 1
	; 2
	; 2
	; 3
	; 4
	; 5
	; 6
	; 7
	; 8
	; 9
	; A
	; B
	; C
	EOR >zp,X
	LSR >zp,X
	; F

; 6x

	; 1
	; 2
	; 2
	; 3
	; 4
	; 5
	; 6
	; 7
	; 8
	; 9
	; A
	; B
	; C
	ADC >zp
	ROR >zp
	; F

; 7x

	; 0
	; 1
	; 2
	; 3
	; 4
	; 5
	; 6
	; 7
	; 8
	; 9
	; A
	; B
	; C
	ADC >zp,X
	ROR >zp,X
	; F

; 8x

	; 0
	; 1
	; 2
	; 3
	; 4
	; 5
	; 6
	; 7
	; 8
	; 9
	; A
	; B
	STY >zp
	STA >zp
	STX >zp
	; F

; 9x

	; 0
	; 1
	; 2
	; 3
	; 4
	; 5
	; 6
	; 7
	; 8
	; 9
	; A
	; B
	; C
	STA >zp,X
	; E
	; F

; Ax

	; 0
	; 1 
	; 2
	; 3
	; 4
	; 5
	; 6
	; 7
	; 8
	; 9
	; A
	; B
	LDY >zp
	LDA >zp
	LDX >zp
	; F

; Bx

	; 0
	; 1
	; 2
	; 3
	; 4
	; 5
	; 6
	; 7
	; 8
	; 9
	; A
	; B
	LDY >zp,X
	LDA >zp,X
	LDX >zp,Y
	; F

; Cx

	; 0
	; 1
	; 2
	; 3
	; 4
	; 5
	; 6
	; 7
	; 8
	; 9
	; A
	; B
	CPY >zp
	CMP >zp
	DEC >zp
	; F

; Dx

	; 0
	; 1
	; 2
	; 3
	; 4
	; 5
	; 6
	; 7
	; 8
	; 9
	; A
	; B
	; C
	CMP >zp,X
	DEC >zp,X
	; F

; Ex

	; 0
	; 1
	; 2
	; 3
	; 4
	; 5
	; 6
	; 7
	; 8
	; 9
	; A
	; B
	CPX >zp
	SBC >zp
	INC >zp
	; F

; Fx

	; 0
	; 1
	; 2
	; 3
	; 4
	; 5
	; 6
	; 7
	; 8
	; 9
	; A
	; B
	; C
	SBC >zp,X
	INC >zp,X
	; F

	end
