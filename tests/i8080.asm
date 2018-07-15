; ===========================================================================
; uz80as, an assembler for the Zilog Z80 and several other microprocessors.
;
; Intel 8080.
; ===========================================================================


#define equ .equ
#define end .end

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

	NOP
	LXI B,nn
	STAX B
	INX B
	INR B
	DCR B
	MVI B,n
	RLC
	; NOP
	DAD B
	LDAX B
	DCX B
	INR C
	DCR C
	MVI C,n
	RRC

; 1x

	; NOP
	LXI D,nn
	STAX D
	INX D
	INR D
	DCR D
	MVI D,n
	RAL
	; NOP
	DAD D
	LDAX D
	DCX D
	INR E
	DCR E
	MVI E,n
	RAR

; 2x

	; NOP
	LXI H,nn
	SHLD nn
	INX H
	INR H
	DCR H
	MVI H,n
	DAA
	; NOP
	DAD H
	LHLD nn
	DCX H
	INR L
	DCR L
	MVI L,n
	CMA

; 3x

	; NOP
	LXI SP,nn
	STA nn
	INX SP
	INR M
	DCR M
	MVI M,n
	STC
	; NOP
	DAD SP
	LDA nn
	DCX SP
	INR A
	DCR A
	MVI A,n
	CMC

; 4x

	MOV B,B
	MOV B,C
	MOV B,D
	MOV B,E
	MOV B,H
	MOV B,L
	MOV B,M
	MOV B,A
	MOV C,B
	MOV C,C
	MOV C,D
	MOV C,E
	MOV C,H
	MOV C,L
	MOV C,M
	MOV C,A

; 5x

	MOV D,B
	MOV D,C
	MOV D,D
	MOV D,E
	MOV D,H
	MOV D,L
	MOV D,M
	MOV D,A
	MOV E,B
	MOV E,C
	MOV E,D
	MOV E,E
	MOV E,H
	MOV E,L
	MOV E,M
	MOV E,A

; 6x

	MOV H,B
	MOV H,C
	MOV H,D
	MOV H,E
	MOV H,H
	MOV H,L
	MOV H,M
	MOV H,A
	MOV L,B
	MOV L,C
	MOV L,D
	MOV L,E
	MOV L,H
	MOV L,L
	MOV L,M
	MOV L,A

; 7x

	MOV M,B
	MOV M,C
	MOV M,D
	MOV M,E
	MOV M,H
	MOV M,L
	HLT
	MOV M,A
	MOV A,B
	MOV A,C
	MOV A,D
	MOV A,E
	MOV A,H
	MOV A,L
	MOV A,M
	MOV A,A

; 8x

	ADD B
	ADD C
	ADD D
	ADD E
	ADD H
	ADD L
	ADD M
	ADD A
	ADC B
	ADC C
	ADC D
	ADC E
	ADC H
	ADC L
	ADC M
	ADC A

; 9x

	SUB B
	SUB C
	SUB D
	SUB E
	SUB H
	SUB L
	SUB M
	SUB A
	SBB B
	SBB C
	SBB D
	SBB E
	SBB H
	SBB L
	SBB M
	SBB A

; Ax

	ANA B
	ANA C
	ANA D
	ANA E
	ANA H
	ANA L
	ANA M
	ANA A
	XRA B
	XRA C
	XRA D
	XRA E
	XRA H
	XRA L
	XRA M
	XRA A

; Bx

	ORA B
	ORA C
	ORA D
	ORA E
	ORA H
	ORA L
	ORA M
	ORA A
	CMP B
	CMP C
	CMP D
	CMP E
	CMP H
	CMP L
	CMP M
	CMP A

; Cx

	RNZ
	POP B
	JNZ a16
	JMP a16
	CNZ a16
	PUSH B
	ADI n
	RST 0
	RZ
	RET
	JZ a16
	; JMP a16
	CZ a16
	CALL a16
	ACI n
	RST 1

; Dx

	RNC
	POP D
	JNC a16
	OUT n
	CNC a16
	PUSH D
	SUI n
	RST 2
	RC
	; RET
	JC a16
	IN n
	CC a16
	; CALL a16
	SBI n
	RST 3

; Ex

	RPO
	POP H
	JPO a16
	XTHL
	CPO a16
	PUSH H
	ANI n
	RST 4
	RPE
	PCHL
	JPE a16
	XCHG
	CPE a16
	; CALL a16
	XRI n
	RST 5

; Fx

	RP
	POP PSW
	JP a16
	DI
	CP a16
	PUSH PSW
	ORI n
	RST 6
	RM
	SPHL
	JM a16
	EI
	CM a16
	; CALL a16
	CPI n
	RST 7

	end
