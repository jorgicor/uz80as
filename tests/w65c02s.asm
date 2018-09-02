; ===========================================================================
; uz80as, an assembler for the Zilog Z80 and several other microprocessors.
;
; Western Design W65C02S.
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
	RMB0 zp ; *
	PHP
	ORA #n
	ASL A
	; B
	TSB a16 ; *
	ORA a16
	ASL a16
b0f:	BBR0 zp,b0f ; *

; 1x

b10:	BPL b10
	ORA (zp),Y
	ORA (zp) ; *
	; 3
	TRB zp ; *
	ORA zp,X
	ASL zp,X
	RMB1 zp ; *
	CLC
	ORA a16,Y
	INC A ; *
	; B
	TRB a16 ; *
	ORA a16,X
	ASL a16,X
b1f:	BBR1 zp,b1f ; *
	
; 2x

	JSR a16
	AND (zp,X)
	; 2
	; 3
	BIT zp
	AND zp
	ROL zp
	RMB2 zp ; *
	PLP
	AND #n
	ROL A
	; B
	BIT a16
	AND a16
	ROL a16
b2f:	BBR2 zp,b2f ; *

; 3x

b30:	BMI b30
	AND (zp),Y
	AND (zp) ; *
	; 3
	BIT zp,X ; *
	AND zp,X
	ROL zp,X
	RMB3 zp ; *
	SEC
	AND a16,Y
	DEC A ; *
	; B
	BIT a16,X ; *
	AND a16,X
	ROL a16,X
b3f:	BBR3 zp,b3f ; *

; 4x

	RTI
	EOR (zp,X)
	; 2
	; 3
	; 4
	EOR zp
	LSR zp
	RMB4 zp ; *
	PHA
	EOR #n
	LSR A
	; B
	JMP a16
	EOR a16
	LSR a16
b4f:	BBR4 zp,b4f ; *

; 5x

b50:	BVC b50
	EOR (zp),Y
	EOR (zp) ; *
	; 3
	; 4
	EOR zp,X
	LSR zp,X
	RMB5 zp ; *
	CLI
	EOR a16,Y
	PHY ; *
	; B
	; C
	EOR a16,X
	LSR a16,X
b5f:	BBR5 zp,b5f ; *

; 6x

	RTS
	ADC (zp,X)
	; 2
	; 3
	STZ zp ; *
	ADC zp
	ROR zp
	RMB6 zp ; *
	PLA
	ADC #n
	ROR A
	; B
	JMP (a16)
	ADC a16
	ROR a16
b6f:	BBR6 zp,b6f ; *

; 7x

b70:	BVS b70
	ADC (zp),Y
	ADC (zp) ; *
	; 3
	STZ zp,X ; *
	ADC zp,X
	ROR zp,X
	RMB7 zp ; *
	SEI
	ADC a16,Y
	PLY ; *
	; B
	JMP (a16,X) ; *
	ADC a16,X
	ROR a16,X
b7f:	BBR7 zp,b7f ; *

; 8x

b80:	BRA b80 ; *
	STA (zp,X)
	; 2
	; 3
	STY zp
	STA zp
	STX zp
	SMB0 zp ; *
	DEY
	BIT #n ; *
	TXA
	; B
	STY a16
	STA a16
	STX a16
b8f:	BBS0 zp,b8f ; *

; 9x

b90:	BCC b90
	STA (zp),Y
	STA (zp) ; *
	; 3
	STY zp,X
	STA zp,X
	STX zp,Y
	SMB1 zp ; *
	TYA
	STA a16,Y
	TXS
	; B
	; C
	STA a16,X
	STZ a16,X ; *
b9f:	BBS1 zp,b9f ; *

; Ax

	LDY #n
	LDA (zp,X)
	LDX #n
	; 3
	LDY zp
	LDA zp
	LDX zp
	SMB2 zp ; *
	TAY
	LDA #n
	TAX
	; B
	LDY a16
	LDA a16
	LDX a16
baf:	BBS2 zp,baf ; *

; Bx

bB0:	BCS bB0
	LDA (zp),Y
	LDA (zp) ; *
	; 3
	LDY zp,X
	LDA zp,X
	LDX zp,Y
	SMB3 zp ; *
	CLV
	LDA a16,Y
	TSX
	; B
	LDY a16,X
	LDA a16,X
	LDX a16,Y
bBF:	BBS3 zp,bBF ; *

; Cx

	CPY #n
	CMP (zp,X)
	; 2
	; 3
	CPY zp
	CMP zp
	DEC zp
	SMB4 zp ; *
	INY
	CMP #n
	DEX
	WAI ; *
	CPY a16
	CMP a16
	DEC a16
bCF:	BBS4 zp,bCF ; *

; Dx

bD0:	BNE bD0
	CMP (zp),Y
	CMP (zp) ; *
	; 3
	; 4
	CMP zp,X
	DEC zp,X
	SMB5 zp ; *
	CLD
	CMP a16,Y
	PHX ; *
	STP ; * 
	; C
	CMP a16,X
	DEC a16,X
bDF:	BBS5 zp,bDF ; *

; Ex

	CPX #n
	SBC (zp,X)
	; 2
	; 3
	CPX zp
	SBC zp
	INC zp
	SMB6 zp ; *
	INX
	SBC #n
	NOP
	; B
	CPX a16
	SBC a16
	INC a16
bEF:	BBS6 zp,bEF ; *

; Fx

bF0:	BEQ bF0
	SBC (zp),Y
	SBC (zp) ; *
	; 3
	; 4
	SBC zp,X
	INC zp,X
	SMB7 zp ; *
	SED
	SBC a16,Y
	PLX ; *
	; B
	; C
	SBC a16,X
	INC a16,X
	BBS7 zp,* ; *

	end
