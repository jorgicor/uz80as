; ===========================================================================
; uz80as, a macro assembler for Z80 based microprocessors.
; ===========================================================================

#define equ .equ
#define end .end

n:          equ 20h
nn:         equ 0584h
dddd:       equ 07h
a16:        equ $1234
hi8:	equ $ff11
port:       equ 3
imm8:       equ 56h    ;immediate data (8 bits)
offs:     equ 7
offset_neg: equ -7

; 0x

	NOP
	LD BC,nn
	LD (BC),A
	INC BC
	INC B
	DEC B
	LD B,n
	RLCA
	EX AF,AF'
	ADD HL,BC
	LD A,(BC)
	DEC BC
	INC C
	DEC C
	LD C,n
	RRCA

; 1x

loop0:	DJNZ loop0
	LD DE,nn
	LD (DE),A
	INC DE
	INC D 
	DEC D
	LD D,n
	RLA
loop1:	JR loop1
	ADD HL,DE
	LD A,(DE)
	DEC DE
	INC E
	DEC E
	LD E,n
	RRA

; 2x

loop2:	JR NZ,loop2
	LD HL,nn
	LD (nn),HL
	INC HL
	INC H
	DEC H
	LD H,n
	DAA
loop3:	JR Z,loop3
	ADD HL,HL
	LD HL,(nn)
	DEC HL
	INC L
	DEC L
	LD L,n
	CPL

; 3x

loop4:	JR NC,loop4
	LD SP,nn
	LD (nn),A
	INC SP
	INC (HL)
	DEC (HL)
	LD (HL),n
	SCF
loop5:	JR C,loop5
	ADD HL,SP
	LD A,(nn)
	DEC SP
	INC A
	DEC A
	LD A,n
	CCF

; 4x

	LD B,B
	LD B,C
	LD B,D
	LD B,E
	LD B,H
	LD B,L
	LD B,(HL)
	LD B,A
	LD C,B
	LD C,C
	LD C,D
	LD C,E
	LD C,H
	LD C,L
	LD C,(HL)
	LD C,A

; 5x

	LD D,B
	LD D,C
	LD D,D
	LD D,E
	LD D,H
	LD D,L
	LD D,(HL)
	LD D,A
	LD E,B
	LD E,C
	LD E,D
	LD E,E
	LD E,H
	LD E,L
	LD E,(HL)
	LD E,A

; 6x

	LD H,B
	LD H,C
	LD H,D
	LD H,E
	LD H,H
	LD H,L
	LD H,(HL)
	LD H,A
	LD L,B
	LD L,C
	LD L,D
	LD L,E
	LD L,H
	LD L,L
	LD L,(HL)
	LD L,A

; 7X

	LD (HL),B
	LD (HL),C
	LD (HL),D
	LD (HL),E
	LD (HL),H
	LD (HL),L
	HALT
	LD (HL),A
	LD A,B
	LD A,C
	LD A,D
	LD A,E
	LD A,H
	LD A,L
	LD A,(HL)
	LD A,A

; 8x

	ADD A,B
	ADD A,C
	ADD A,D
	ADD A,E
	ADD A,H
	ADD A,L
	ADD A,(HL)
	ADD A,A
	ADC A,B
	ADC A,C
	ADC A,D
	ADC A,E
	ADC A,H
	ADC A,L
	ADC A,(HL)
	ADC A,A

; 9x

	SUB B
	SUB C
	SUB D
	SUB E
	SUB H
	SUB L
	SUB (HL)
	SUB A
	SBC A,B
	SBC A,C
	SBC A,D
	SBC A,E
	SBC A,H
	SBC A,L
	SBC A,(HL)
	SBC A,A

; Ax

	AND B
	AND C
	AND D
	AND E
	AND H
	AND L
	AND (HL)
	AND A
	XOR B
	XOR C
	XOR D
	XOR E
	XOR H
	XOR L
	XOR (HL)
	XOR A

; Bx

	OR B
	OR C
	OR D
	OR E
	OR H
	OR L
	OR (HL)
	OR A
	CP B
	CP C
	CP D
	CP E
	CP H
	CP L
	CP (HL)
	CP A

; Cx

	RET NZ
	POP BC
	JP NZ,a16
	JP a16
	CALL NZ,a16
	PUSH BC
	ADD A,n
	RST 00h
	RET Z
	RET
	JP Z,a16
	; CB prefix
	CALL Z,a16
	CALL a16
	ADC A,n
	RST 08h

; Dx

	RET NC
	POP DE
	JP NC,a16
	OUT (n),A
	CALL NC,a16
	PUSH DE
	SUB n
	RST 10h
	RET C
	EXX
	JP C,a16
	IN A,(n)
	CALL C,a16
	; DD prefix
	SBC A,n
	RST 18h

; Ex

	RET PO
	POP HL
	JP PO,a16
	EX (SP),HL
	CALL PO,a16
	PUSH HL
	AND n
	RST 20h
	RET PE
	JP (HL)
	JP PE,a16
	EX DE,HL
	CALL PE,a16
	; ED prefix
	XOR n
	RST 28h

; Fx

	RET P
	POP AF
	JP P,a16
	DI
	CALL P,a16
	PUSH AF
	OR n
	RST 30h
	RET M
	LD SP,HL
	JP M,a16
	EI
	CALL M,a16
	; FD prefix
	CP n
	RST 38h

; CB prefix

; 0x

	RLC B
	RLC C
	RLC D
	RLC E
	RLC H
	RLC L
	RLC (HL)
	RLC A
	RRC B
	RRC C
	RRC D
	RRC E
	RRC H
	RRC L
	RRC (HL)
	RRC A

; 1x

	RL B
	RL C
	RL D
	RL E
	RL H
	RL L
	RL (HL)
	RL A
	RR B
	RR C
	RR D
	RR E
	RR H
	RR L
	RR (HL)
	RR A

; 2x

	SLA B
	SLA C
	SLA D
	SLA E
	SLA H
	SLA L
	SLA (HL)
	SLA A
	SRA B
	SRA C
	SRA D
	SRA E
	SRA H
	SRA L
	SRA (HL)
	SRA A

; 3x

	SLL B
	SLL C
	SLL D
	SLL E
	SLL H
	SLL L
	SLL (HL)
	SLL A
	SRL B
	SRL C
	SRL D
	SRL E
	SRL H
	SRL L
	SRL (HL)
	SRL A

; 4x

	BIT 0,B
	BIT 0,C
	BIT 0,D
	BIT 0,E
	BIT 0,H
	BIT 0,L
	BIT 0,(HL)
	BIT 0,A
	BIT 1,B
	BIT 1,C
	BIT 1,D
	BIT 1,E
	BIT 1,H
	BIT 1,L
	BIT 1,(HL)
	BIT 1,A

; 5x

	BIT 2,B
	BIT 2,C
	BIT 2,D
	BIT 2,E
	BIT 2,H
	BIT 2,L
	BIT 2,(HL)
	BIT 2,A
	BIT 3,B
	BIT 3,C
	BIT 3,D
	BIT 3,E
	BIT 3,H
	BIT 3,L
	BIT 3,(HL)
	BIT 3,A

; 6x

	BIT 4,B
	BIT 4,C
	BIT 4,D
	BIT 4,E
	BIT 4,H
	BIT 4,L
	BIT 4,(HL)
	BIT 4,A
	BIT 5,B
	BIT 5,C
	BIT 5,D
	BIT 5,E
	BIT 5,H
	BIT 5,L
	BIT 5,(HL)
	BIT 5,A

; 7x

	BIT 6,B
	BIT 6,C
	BIT 6,D
	BIT 6,E
	BIT 6,H
	BIT 6,L
	BIT 6,(HL)
	BIT 6,A
	BIT 7,B
	BIT 7,C
	BIT 7,D
	BIT 7,E
	BIT 7,H
	BIT 7,L
	BIT 7,(HL)
	BIT 7,A

; 8x

	RES 0,B
	RES 0,C
	RES 0,D
	RES 0,E
	RES 0,H
	RES 0,L
	RES 0,(HL)
	RES 0,A
	RES 1,B
	RES 1,C
	RES 1,D
	RES 1,E
	RES 1,H
	RES 1,L
	RES 1,(HL)
	RES 1,A

; 9x

	RES 2,B
	RES 2,C
	RES 2,D
	RES 2,E
	RES 2,H
	RES 2,L
	RES 2,(HL)
	RES 2,A
	RES 3,B
	RES 3,C
	RES 3,D
	RES 3,E
	RES 3,H
	RES 3,L
	RES 3,(HL)
	RES 3,A

; Ax

	RES 4,B
	RES 4,C
	RES 4,D
	RES 4,E
	RES 4,H
	RES 4,L
	RES 4,(HL)
	RES 4,A
	RES 5,B
	RES 5,C
	RES 5,D
	RES 5,E
	RES 5,H
	RES 5,L
	RES 5,(HL)
	RES 5,A

; Bx

	RES 6,B
	RES 6,C
	RES 6,D
	RES 6,E
	RES 6,H
	RES 6,L
	RES 6,(HL)
	RES 6,A
	RES 7,B
	RES 7,C
	RES 7,D
	RES 7,E
	RES 7,H
	RES 7,L
	RES 7,(HL)
	RES 7,A

; Cx

	SET 0,B
	SET 0,C
	SET 0,D
	SET 0,E
	SET 0,H
	SET 0,L
	SET 0,(HL)
	SET 0,A
	SET 1,B
	SET 1,C
	SET 1,D
	SET 1,E
	SET 1,H
	SET 1,L
	SET 1,(HL)
	SET 1,A

; Dx

	SET 2,B
	SET 2,C
	SET 2,D
	SET 2,E
	SET 2,H
	SET 2,L
	SET 2,(HL)
	SET 2,A
	SET 3,B
	SET 3,C
	SET 3,D
	SET 3,E
	SET 3,H
	SET 3,L
	SET 3,(HL)
	SET 3,A

; Ex

	SET 4,B
	SET 4,C
	SET 4,D
	SET 4,E
	SET 4,H
	SET 4,L
	SET 4,(HL)
	SET 4,A
	SET 5,B
	SET 5,C
	SET 5,D
	SET 5,E
	SET 5,H
	SET 5,L
	SET 5,(HL)
	SET 5,A

; Fx

	SET 6,B
	SET 6,C
	SET 6,D
	SET 6,E
	SET 6,H
	SET 6,L
	SET 6,(HL)
	SET 6,A
	SET 7,B
	SET 7,C
	SET 7,D
	SET 7,E
	SET 7,H
	SET 7,L
	SET 7,(HL)
	SET 7,A

; ED prefix

; 4x

	IN B,(C)
	OUT (C),B
	SBC HL,BC
	LD (nn),BC
	NEG
	RETN
	IM 0
	LD I,A
	IN C,(C)
	OUT (C),C
	ADC HL,BC
	LD BC,(nn)
	; NEG
	RETI
	; IM 0/1
	LD R,A

; 5x

	IN D,(C)
	OUT (C),D
	SBC HL,DE
	LD (nn),DE
	; NEG
	; RETN
	IM 1
	LD A,I
	IN E,(C)
	OUT (C),E
	ADC HL,DE
	LD DE,(nn)
	; NEG
	; RETN
	IM 2
	LD A,R

; 6x

	IN H,(C)
	OUT (C),H
	SBC HL,HL
	; LD (nn),HL
	; NEG
	; RETN
	; IM 0
	RRD
	IN L,(C)
	OUT (C),L
	ADC HL,HL
	; LD HL,(nn)
	; NEG
	; RETN
	; IM 0/1
	RLD

; 7x

	IN (C)
	OUT (C),0
	SBC HL,SP
	LD (nn),SP
	; NEG
	; RETN
	; IM 1
	;
	IN A,(C)
	OUT (C),A
	ADC HL,SP
	LD SP,(nn)
	; NEG
	; RETN
	; IM 2
	;

; Ax

	LDI
	CPI
	INI
	OUTI
	; A4-A7
	LDD
	CPD
	IND
	OUTD
	; AC-AF

; Bx

	LDIR
	CPIR
	INIR
	OTIR
	; B4-B7
	LDDR
	CPDR
	INDR
	OTDR
	; BC-BF

; DD prefix

; 0x

	; 00-08
	ADD IX,BC
	; 0A-0F

; 1x

	; 10-18
	ADD IX,DE
	; 1A-1F

; 2x

	; 20
	LD IX,nn
	LD (nn),IX
	INC IX
	INC IXH
	DEC IXH
	LD IXH,n
	; 27
	; 28
	ADD IX,IX
	LD IX,(nn)
	DEC IX
	INC IXL
	DEC IXL
	LD IXL,n
	; 2F

; 3x

	; 30-33
	INC (IX+offs)
	DEC (IX+offs)
	LD (IX+offs),n
	; 37
	; 38
	ADD IX,SP
	; 3A-3F

; 4x

	; 40-43
	LD B,IXH
	LD B,IXL
	LD B,(IX+offs)
	; 47-4B
	LD C,IXH
	LD C,IXL
	LD C,(IX+offs)
	; 4F

; 5x

	; 50-53
	LD D,IXH
	LD D,IXL
	LD D,(IX+offs)
	; 57-5B
	LD E,IXH
	LD E,IXL
	LD E,(IX+offs)
	; 5F

; 6x

	LD IXH,B	; 60
	LD IXH,C	; 61
	LD IXH,D	; 62
	LD IXH,E	; 63
	LD IXH,IXH	; 64
	LD IXH,IXL	; 65
	LD H,(IX+offs)
	LD IXH,A	; 67
	LD IXL,B	; 68
	LD IXL,C	; 69
	LD IXL,D	; 6A
	LD IXL,E	; 6B
	LD IXL,IXH	; 6C
	LD IXL,IXL	; 6D
	LD L,(IX+offs)
	LD IXL,A	; 6F

; 7x

	LD (IX+offs),B
	LD (IX+offs),C
	LD (IX+offs),D
	LD (IX+offs),E
	LD (IX+offs),H
	LD (IX+offs),L
	; 76
	LD (IX+offs),A
	; 78-7B
	LD A,IXH
	LD A,IXL
	LD A,(IX+offs)
	; 7F

; 8x

	; 80-83
	ADD A,IXH
	ADD A,IXL
	ADD A,(IX+offs)
	; 86-8B
	ADC A,IXH
	ADC A,IXL
	ADC A,(IX+offs)
	; 8F

; 9x

	; 90-93
	SUB IXH
	SUB IXL
	SUB (IX+offs)
	; 96-9B
	SBC A,IXH
	SBC A,IXL
	SBC A,(IX+offs)
	; 9F

; Ax

	; A0-A3
	AND IXH
	AND IXL
	AND (IX+offs)
	; A6-AB
	XOR IXH
	XOR IXL
	XOR (IX+offs)
	; AF

; Bx

	; B0-B3
	OR IXH
	OR IXL
	OR (IX+offs)
	; B6-BB
	CP IXH
	CP IXL
	CP (IX+offs)
	; BF

; Cx	

	; DDCB prefix

; Dx
; Ex

	; E0
	POP IX
	; E2
	EX (SP),IX
	; E4
	PUSH IX
	; E6-E8
	JP (IX)
	; EA-EF

; Fx

	; F0-F8
	LD SP,IX
	; FA-FF

; FD prefix

; 0x

	; 00-08
	ADD IY,BC
	; 0A-0F

; 1x

	; 10-18
	ADD IY,DE
	; 1A-1F

; 2x

	; 20
	LD IY,nn
	LD (nn),IY
	INC IY
	INC IYH
	DEC IYH
	LD IYH,n
	; 27
	; 28
	ADD IY,IY
	LD IY,(nn)
	DEC IY
	INC IYL
	DEC IYL
	LD IYL,n
	; 2F

; 3x

	; 30-33
	INC (IY+offs)
	DEC (IY+offs)
	LD (IY+offs),n
	; 37
	; 38
	ADD IY,SP
	; 3A-3F

; 4x

	; 40-43
	LD B,IYH
	LD B,IYL
	LD B,(IY+offs)
	; 47-4B
	LD C,IYH
	LD C,IYL
	LD C,(IY+offs)
	; 4F

; 5x

	; 50-53
	LD D,IYH
	LD D,IYL
	LD D,(IY+offs)
	; 57-5B
	LD E,IYH
	LD E,IYL
	LD E,(IY+offs)
	; 5F

; 6x

	LD IYH,B	; 60
	LD IYH,C	; 61
	LD IYH,D	; 62
	LD IYH,E	; 63
	LD IYH,IYH	; 64
	LD IYH,IYL	; 65
	LD H,(IY+offs)
	LD IYH,A	; 67
	LD IYL,B	; 68
	LD IYL,C	; 69
	LD IYL,D	; 6A
	LD IYL,E	; 6B
	LD IYL,IYH	; 6C
	LD IYL,IYL	; 6D
	LD L,(IY+offs)
	LD IYL,A	; 6F

; 7x

	LD (IY+offs),B
	LD (IY+offs),C
	LD (IY+offs),D
	LD (IY+offs),E
	LD (IY+offs),H
	LD (IY+offs),L
	; 76
	LD (IY+offs),A
	; 78-7B
	LD A,IYH
	LD A,IYL
	LD A,(IY+offs)
	; 7F

; 8x

	; 80-83
	ADD A,IYH
	ADD A,IYL
	ADD A,(IY+offs)
	; 86-8B
	ADC A,IYH
	ADC A,IYL
	ADC A,(IY+offs)
	; 8F

; 9x

	; 90-93
	SUB IYH
	SUB IYL
	SUB (IY+offs)
	; 96-9B
	SBC A,IYH
	SBC A,IYL
	SBC A,(IY+offs)
	; 9F

; Ax

	; A0-A3
	AND IYH
	AND IYL
	AND (IY+offs)
	; A6-AB
	XOR IYH
	XOR IYL
	XOR (IY+offs)
	; AF

; Bx

	; B0-B3
	OR IYH
	OR IYL
	OR (IY+offs)
	; B6-BB
	CP IYH
	CP IYL
	CP (IY+offs)
	; BF

; Cx	

	; FDCB prefix

; Dx
; Ex

	; E0
	POP IY
	; E2
	EX (SP),IY
	; E4
	PUSH IY
	; E6-E8
	JP (IY)
	; EA-EF

; Fx

	; F0-F8
	LD SP,IY
	; FA-FF

; DDCB prefix

; 0x

	RLC (IX+offs),B
	RLC (IX+offs),C
	RLC (IX+offs),D
	RLC (IX+offs),E
	RLC (IX+offs),H
	RLC (IX+offs),L
	RLC (IX+offs)		; 06
	RLC (IX+offs),A
	RRC (IX+offs),B
	RRC (IX+offs),C
	RRC (IX+offs),D
	RRC (IX+offs),E
	RRC (IX+offs),H
	RRC (IX+offs),L
	RRC (IX+offs)		; 0E
	RRC (IX+offs),A

; 1x

	RL (IX+offs),B
	RL (IX+offs),C
	RL (IX+offs),D
	RL (IX+offs),E
	RL (IX+offs),H
	RL (IX+offs),L
	RL (IX+offs)		; 16
	RL (IX+offs),A
	RR (IX+offs),B
	RR (IX+offs),C
	RR (IX+offs),D
	RR (IX+offs),E
	RR (IX+offs),H
	RR (IX+offs),L
	RR (IX+offs)		; 1E
	RR (IX+offs),A

; 2x

	SLA (IX+offs),B
	SLA (IX+offs),C
	SLA (IX+offs),D
	SLA (IX+offs),E
	SLA (IX+offs),H
	SLA (IX+offs),L
	SLA (IX+offs)		; 26
	SLA (IX+offs),A
	SRA (IX+offs),B
	SRA (IX+offs),C
	SRA (IX+offs),D
	SRA (IX+offs),E
	SRA (IX+offs),H
	SRA (IX+offs),L
	SRA (IX+offs)		; 2E
	SRA (IX+offs),A

; 3x

	SLL (IX+offs),B
	SLL (IX+offs),C
	SLL (IX+offs),D
	SLL (IX+offs),E
	SLL (IX+offs),H
	SLL (IX+offs),L
	SLL (IX+offs)		; 36
	SLL (IX+offs),A
	SRL (IX+offs),B
	SRL (IX+offs),C
	SRL (IX+offs),D
	SRL (IX+offs),E
	SRL (IX+offs),H
	SRL (IX+offs),L
	SRL (IX+offs)		; 3E
	SRL (IX+offs),A

; 4x

	; BIT 0,(IX+offs)
	; BIT 0,(IX+offs)
	; BIT 0,(IX+offs)
	; BIT 0,(IX+offs)
	; BIT 0,(IX+offs)
	; BIT 0,(IX+offs)
	BIT 0,(IX+offs)		; 46
	; BIT 0,(IX+offs)
	; BIT 1,(IX+offs)
	; BIT 1,(IX+offs)
	; BIT 1,(IX+offs)
	; BIT 1,(IX+offs)
	; BIT 1,(IX+offs)
	; BIT 1,(IX+offs)
	BIT 1,(IX+offs)		; 4E
	; BIT 1,(IX+offs)

; 5x

	; BIT 2,(IX+offs)
	; BIT 2,(IX+offs)
	; BIT 2,(IX+offs)
	; BIT 2,(IX+offs)
	; BIT 2,(IX+offs)
	; BIT 2,(IX+offs)
	BIT 2,(IX+offs)		; 56
	; BIT 2,(IX+offs)
	; BIT 3,(IX+offs)
	; BIT 3,(IX+offs)
	; BIT 3,(IX+offs)
	; BIT 3,(IX+offs)
	; BIT 3,(IX+offs)
	; BIT 3,(IX+offs)
	BIT 3,(IX+offs)		; 5E
	; BIT 3,(IX+offs)

; 6x

	; BIT 4,(IX+offs)
	; BIT 4,(IX+offs)
	; BIT 4,(IX+offs)
	; BIT 4,(IX+offs)
	; BIT 4,(IX+offs)
	; BIT 4,(IX+offs)
	BIT 4,(IX+offs)		; 66
	; BIT 4,(IX+offs)
	; BIT 5,(IX+offs)
	; BIT 5,(IX+offs)
	; BIT 5,(IX+offs)
	; BIT 5,(IX+offs)
	; BIT 5,(IX+offs)
	; BIT 5,(IX+offs)
	BIT 5,(IX+offs)		; 6E
	; BIT 5,(IX+offs)

; 7x

	; BIT 6,(IX+offs)
	; BIT 6,(IX+offs)
	; BIT 6,(IX+offs)
	; BIT 6,(IX+offs)
	; BIT 6,(IX+offs)
	; BIT 6,(IX+offs)
	BIT 6,(IX+offs)		; 76
	; BIT 6,(IX+offs)
	; BIT 7,(IX+offs)
	; BIT 7,(IX+offs)
	; BIT 7,(IX+offs)
	; BIT 7,(IX+offs)
	; BIT 7,(IX+offs)
	; BIT 7,(IX+offs)
	BIT 7,(IX+offs)		; 7E
	; BIT 7,(IX+offs)

; 8x

	RES 0,(IX+offs),B
	RES 0,(IX+offs),C
	RES 0,(IX+offs),D
	RES 0,(IX+offs),E
	RES 0,(IX+offs),H
	RES 0,(IX+offs),L
	RES 0,(IX+offs)		; 86
	RES 0,(IX+offs),A
	RES 1,(IX+offs),B
	RES 1,(IX+offs),C
	RES 1,(IX+offs),D
	RES 1,(IX+offs),E
	RES 1,(IX+offs),H
	RES 1,(IX+offs),L
	RES 1,(IX+offs)		; 8E
	RES 1,(IX+offs),A

; 9x

	RES 2,(IX+offs),B
	RES 2,(IX+offs),C
	RES 2,(IX+offs),D
	RES 2,(IX+offs),E
	RES 2,(IX+offs),H
	RES 2,(IX+offs),L
	RES 2,(IX+offs)		; 96
	RES 2,(IX+offs),A
	RES 3,(IX+offs),B
	RES 3,(IX+offs),C
	RES 3,(IX+offs),D
	RES 3,(IX+offs),E
	RES 3,(IX+offs),H
	RES 3,(IX+offs),L
	RES 3,(IX+offs)		; 9E
	RES 3,(IX+offs),A

; Ax

	RES 4,(IX+offs),B
	RES 4,(IX+offs),C
	RES 4,(IX+offs),D
	RES 4,(IX+offs),E
	RES 4,(IX+offs),H
	RES 4,(IX+offs),L
	RES 4,(IX+offs)		; A6
	RES 4,(IX+offs),A
	RES 5,(IX+offs),B
	RES 5,(IX+offs),C
	RES 5,(IX+offs),D
	RES 5,(IX+offs),E
	RES 5,(IX+offs),H
	RES 5,(IX+offs),L
	RES 5,(IX+offs)		; AE
	RES 5,(IX+offs),A

; Bx

	RES 6,(IX+offs),B
	RES 6,(IX+offs),C
	RES 6,(IX+offs),D
	RES 6,(IX+offs),E
	RES 6,(IX+offs),H
	RES 6,(IX+offs),L
	RES 6,(IX+offs)		; B6
	RES 6,(IX+offs),A
	RES 7,(IX+offs),B
	RES 7,(IX+offs),C
	RES 7,(IX+offs),D
	RES 7,(IX+offs),E
	RES 7,(IX+offs),H
	RES 7,(IX+offs),L
	RES 7,(IX+offs)		; BE
	RES 7,(IX+offs),A

; Cx

	SET 0,(IX+offs),B
	SET 0,(IX+offs),C
	SET 0,(IX+offs),D
	SET 0,(IX+offs),E
	SET 0,(IX+offs),H
	SET 0,(IX+offs),L
	SET 0,(IX+offs)		; C6
	SET 0,(IX+offs),A
	SET 1,(IX+offs),B
	SET 1,(IX+offs),C
	SET 1,(IX+offs),D
	SET 1,(IX+offs),E
	SET 1,(IX+offs),H
	SET 1,(IX+offs),L
	SET 1,(IX+offs)		; CE
	SET 1,(IX+offs),A

; Dx

	SET 2,(IX+offs),B
	SET 2,(IX+offs),C
	SET 2,(IX+offs),D
	SET 2,(IX+offs),E
	SET 2,(IX+offs),H
	SET 2,(IX+offs),L
	SET 2,(IX+offs)		; D6
	SET 2,(IX+offs),A
	SET 3,(IX+offs),B
	SET 3,(IX+offs),C
	SET 3,(IX+offs),D
	SET 3,(IX+offs),E
	SET 3,(IX+offs),H
	SET 3,(IX+offs),L
	SET 3,(IX+offs)		; DE
	SET 3,(IX+offs),A

; Ex

	SET 4,(IX+offs),B
	SET 4,(IX+offs),C
	SET 4,(IX+offs),D
	SET 4,(IX+offs),E
	SET 4,(IX+offs),H
	SET 4,(IX+offs),L
	SET 4,(IX+offs)		; E6
	SET 4,(IX+offs),A
	SET 5,(IX+offs),B
	SET 5,(IX+offs),C
	SET 5,(IX+offs),D
	SET 5,(IX+offs),E
	SET 5,(IX+offs),H
	SET 5,(IX+offs),L
	SET 5,(IX+offs)		; EE
	SET 5,(IX+offs),A

; Fx

	SET 6,(IX+offs),B
	SET 6,(IX+offs),C
	SET 6,(IX+offs),D
	SET 6,(IX+offs),E
	SET 6,(IX+offs),H
	SET 6,(IX+offs),L
	SET 6,(IX+offs)		; F6
	SET 6,(IX+offs),A
	SET 7,(IX+offs),B
	SET 7,(IX+offs),C
	SET 7,(IX+offs),D
	SET 7,(IX+offs),E
	SET 7,(IX+offs),H
	SET 7,(IX+offs),L
	SET 7,(IX+offs)		; FE
	SET 7,(IX+offs),A

; FDCB prefix

; 0x

	RLC (IY+offs),B
	RLC (IY+offs),C
	RLC (IY+offs),D
	RLC (IY+offs),E
	RLC (IY+offs),H
	RLC (IY+offs),L
	RLC (IY+offs)		; 06
	RLC (IY+offs),A
	RRC (IY+offs),B
	RRC (IY+offs),C
	RRC (IY+offs),D
	RRC (IY+offs),E
	RRC (IY+offs),H
	RRC (IY+offs),L
	RRC (IY+offs)		; 0E
	RRC (IY+offs),A

; 1x

	RL (IY+offs),B
	RL (IY+offs),C
	RL (IY+offs),D
	RL (IY+offs),E
	RL (IY+offs),H
	RL (IY+offs),L
	RL (IY+offs)		; 16
	RL (IY+offs),A
	RR (IY+offs),B
	RR (IY+offs),C
	RR (IY+offs),D
	RR (IY+offs),E
	RR (IY+offs),H
	RR (IY+offs),L
	RR (IY+offs)		; 1E
	RR (IY+offs),A

; 2x

	SLA (IY+offs),B
	SLA (IY+offs),C
	SLA (IY+offs),D
	SLA (IY+offs),E
	SLA (IY+offs),H
	SLA (IY+offs),L
	SLA (IY+offs)		; 26
	SLA (IY+offs),A
	SRA (IY+offs),B
	SRA (IY+offs),C
	SRA (IY+offs),D
	SRA (IY+offs),E
	SRA (IY+offs),H
	SRA (IY+offs),L
	SRA (IY+offs)		; 2E
	SRA (IY+offs),A

; 3x

	SLL (IY+offs),B
	SLL (IY+offs),C
	SLL (IY+offs),D
	SLL (IY+offs),E
	SLL (IY+offs),H
	SLL (IY+offs),L
	SLL (IY+offs)		; 36
	SLL (IY+offs),A
	SRL (IY+offs),B
	SRL (IY+offs),C
	SRL (IY+offs),D
	SRL (IY+offs),E
	SRL (IY+offs),H
	SRL (IY+offs),L
	SRL (IY+offs)		; 3E
	SRL (IY+offs),A

; 4x

	; BIT 0,(IY+offs)
	; BIT 0,(IY+offs)
	; BIT 0,(IY+offs)
	; BIT 0,(IY+offs)
	; BIT 0,(IY+offs)
	; BIT 0,(IY+offs)
	BIT 0,(IY+offs)		; 46
	; BIT 0,(IY+offs)
	; BIT 1,(IY+offs)
	; BIT 1,(IY+offs)
	; BIT 1,(IY+offs)
	; BIT 1,(IY+offs)
	; BIT 1,(IY+offs)
	; BIT 1,(IY+offs)
	BIT 1,(IY+offs)		; 4E
	; BIT 1,(IY+offs)

; 5x

	; BIT 2,(IY+offs)
	; BIT 2,(IY+offs)
	; BIT 2,(IY+offs)
	; BIT 2,(IY+offs)
	; BIT 2,(IY+offs)
	; BIT 2,(IY+offs)
	BIT 2,(IY+offs)		; 56
	; BIT 2,(IY+offs)
	; BIT 3,(IY+offs)
	; BIT 3,(IY+offs)
	; BIT 3,(IY+offs)
	; BIT 3,(IY+offs)
	; BIT 3,(IY+offs)
	; BIT 3,(IY+offs)
	BIT 3,(IY+offs)		; 5E
	; BIT 3,(IY+offs)

; 6x

	; BIT 4,(IY+offs)
	; BIT 4,(IY+offs)
	; BIT 4,(IY+offs)
	; BIT 4,(IY+offs)
	; BIT 4,(IY+offs)
	; BIT 4,(IY+offs)
	BIT 4,(IY+offs)		; 66
	; BIT 4,(IY+offs)
	; BIT 5,(IY+offs)
	; BIT 5,(IY+offs)
	; BIT 5,(IY+offs)
	; BIT 5,(IY+offs)
	; BIT 5,(IY+offs)
	; BIT 5,(IY+offs)
	BIT 5,(IY+offs)		; 6E
	; BIT 5,(IY+offs)

; 7x

	; BIT 6,(IY+offs)
	; BIT 6,(IY+offs)
	; BIT 6,(IY+offs)
	; BIT 6,(IY+offs)
	; BIT 6,(IY+offs)
	; BIT 6,(IY+offs)
	BIT 6,(IY+offs)		; 76
	; BIT 6,(IY+offs)
	; BIT 7,(IY+offs)
	; BIT 7,(IY+offs)
	; BIT 7,(IY+offs)
	; BIT 7,(IY+offs)
	; BIT 7,(IY+offs)
	; BIT 7,(IY+offs)
	BIT 7,(IY+offs)		; 7E
	; BIT 7,(IY+offs)

; 8x

	RES 0,(IY+offs),B
	RES 0,(IY+offs),C
	RES 0,(IY+offs),D
	RES 0,(IY+offs),E
	RES 0,(IY+offs),H
	RES 0,(IY+offs),L
	RES 0,(IY+offs)		; 86
	RES 0,(IY+offs),A
	RES 1,(IY+offs),B
	RES 1,(IY+offs),C
	RES 1,(IY+offs),D
	RES 1,(IY+offs),E
	RES 1,(IY+offs),H
	RES 1,(IY+offs),L
	RES 1,(IY+offs)		; 8E
	RES 1,(IY+offs),A

; 9x

	RES 2,(IY+offs),B
	RES 2,(IY+offs),C
	RES 2,(IY+offs),D
	RES 2,(IY+offs),E
	RES 2,(IY+offs),H
	RES 2,(IY+offs),L
	RES 2,(IY+offs)		; 96
	RES 2,(IY+offs),A
	RES 3,(IY+offs),B
	RES 3,(IY+offs),C
	RES 3,(IY+offs),D
	RES 3,(IY+offs),E
	RES 3,(IY+offs),H
	RES 3,(IY+offs),L
	RES 3,(IY+offs)		; 9E
	RES 3,(IY+offs),A

; Ax

	RES 4,(IY+offs),B
	RES 4,(IY+offs),C
	RES 4,(IY+offs),D
	RES 4,(IY+offs),E
	RES 4,(IY+offs),H
	RES 4,(IY+offs),L
	RES 4,(IY+offs)		; A6
	RES 4,(IY+offs),A
	RES 5,(IY+offs),B
	RES 5,(IY+offs),C
	RES 5,(IY+offs),D
	RES 5,(IY+offs),E
	RES 5,(IY+offs),H
	RES 5,(IY+offs),L
	RES 5,(IY+offs)		; AE
	RES 5,(IY+offs),A

; Bx

	RES 6,(IY+offs),B
	RES 6,(IY+offs),C
	RES 6,(IY+offs),D
	RES 6,(IY+offs),E
	RES 6,(IY+offs),H
	RES 6,(IY+offs),L
	RES 6,(IY+offs)		; B6
	RES 6,(IY+offs),A
	RES 7,(IY+offs),B
	RES 7,(IY+offs),C
	RES 7,(IY+offs),D
	RES 7,(IY+offs),E
	RES 7,(IY+offs),H
	RES 7,(IY+offs),L
	RES 7,(IY+offs)		; BE
	RES 7,(IY+offs),A

; Cx

	SET 0,(IY+offs),B
	SET 0,(IY+offs),C
	SET 0,(IY+offs),D
	SET 0,(IY+offs),E
	SET 0,(IY+offs),H
	SET 0,(IY+offs),L
	SET 0,(IY+offs)		; C6
	SET 0,(IY+offs),A
	SET 1,(IY+offs),B
	SET 1,(IY+offs),C
	SET 1,(IY+offs),D
	SET 1,(IY+offs),E
	SET 1,(IY+offs),H
	SET 1,(IY+offs),L
	SET 1,(IY+offs)		; CE
	SET 1,(IY+offs),A

; Dx

	SET 2,(IY+offs),B
	SET 2,(IY+offs),C
	SET 2,(IY+offs),D
	SET 2,(IY+offs),E
	SET 2,(IY+offs),H
	SET 2,(IY+offs),L
	SET 2,(IY+offs)		; D6
	SET 2,(IY+offs),A
	SET 3,(IY+offs),B
	SET 3,(IY+offs),C
	SET 3,(IY+offs),D
	SET 3,(IY+offs),E
	SET 3,(IY+offs),H
	SET 3,(IY+offs),L
	SET 3,(IY+offs)		; DE
	SET 3,(IY+offs),A

; Ex

	SET 4,(IY+offs),B
	SET 4,(IY+offs),C
	SET 4,(IY+offs),D
	SET 4,(IY+offs),E
	SET 4,(IY+offs),H
	SET 4,(IY+offs),L
	SET 4,(IY+offs)		; E6
	SET 4,(IY+offs),A
	SET 5,(IY+offs),B
	SET 5,(IY+offs),C
	SET 5,(IY+offs),D
	SET 5,(IY+offs),E
	SET 5,(IY+offs),H
	SET 5,(IY+offs),L
	SET 5,(IY+offs)		; EE
	SET 5,(IY+offs),A

; Fx

	SET 6,(IY+offs),B
	SET 6,(IY+offs),C
	SET 6,(IY+offs),D
	SET 6,(IY+offs),E
	SET 6,(IY+offs),H
	SET 6,(IY+offs),L
	SET 6,(IY+offs)		; F6
	SET 6,(IY+offs),A
	SET 7,(IY+offs),B
	SET 7,(IY+offs),C
	SET 7,(IY+offs),D
	SET 7,(IY+offs),E
	SET 7,(IY+offs),H
	SET 7,(IY+offs),L
	SET 7,(IY+offs)		; FE
	SET 7,(IY+offs),A

	end
