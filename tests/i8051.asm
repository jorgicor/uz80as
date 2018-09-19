; ===========================================================================
; uz80as, an assembler for the Zilog Z80 and several other microprocessors.
;
; Intel 8051.
; ===========================================================================


#define equ .equ
#define end .end

n:	equ 77
da: 	equ 66
ba:	equ 55

; 0x

	NOP
	AJMP $077
	LJMP $1234
	RR A
	INC A
	INC da
	INC @R0
	INC @R1
	INC R0
	INC R1
	INC R2
	INC R3
	INC R4
	INC R5
	INC R6
	INC R7

; 1x

	JBC ba,*
	ACALL $077
	LCALL $1234
	RRC A
	DEC A
	DEC da
	DEC @R0
	DEC @R1
	DEC R0
	DEC R1
	DEC R2
	DEC R3
	DEC R4
	DEC R5
	DEC R6
	DEC R7

; 2x

	JB ba,*
	AJMP $177
	RET
	RL A
	ADD A,#n
	ADD A,da
	ADD A,@R0
	ADD A,@R1
	ADD A,R0
	ADD A,R1
	ADD A,R2
	ADD A,R3
	ADD A,R4
	ADD A,R5
	ADD A,R6
	ADD A,R7

; 3x

	JNB ba,*
	ACALL $177
	RETI
	RLC A
	ADDC A,#n
	ADDC A,da
	ADDC A,@R0
	ADDC A,@R1
	ADDC A,R0
	ADDC A,R1
	ADDC A,R2
	ADDC A,R3
	ADDC A,R4
	ADDC A,R5
	ADDC A,R6
	ADDC A,R7

; 4x

	JC *
	AJMP $277
	ORL da,A
	ORL da,#n
	ORL A,#n
	ORL A,da
	ORL A,@R0
	ORL A,@R1
	ORL A,R0
	ORL A,R1
	ORL A,R2
	ORL A,R3
	ORL A,R4
	ORL A,R5
	ORL A,R6
	ORL A,R7

; 5x

	JNC *
	ACALL $277
	ANL da,A
	ANL da,#n
	ANL A,#n
	ANL A,da
	ANL A,@R0
	ANL A,@R1
	ANL A,R0
	ANL A,R1
	ANL A,R2
	ANL A,R3
	ANL A,R4
	ANL A,R5
	ANL A,R6
	ANL A,R7

; 6x

	JZ *
	AJMP $377
	XRL da,A
	XRL da,#n
	XRL A,#n
	XRL A,da
	XRL A,@R0
	XRL A,@R1
	XRL A,R0
	XRL A,R1
	XRL A,R2
	XRL A,R3
	XRL A,R4
	XRL A,R5
	XRL A,R6
	XRL A,R7

; 7x

	JNZ *
	ACALL $377
	ORL C,ba
	JMP @A+DPTR
	MOV A,#n
	MOV da,#n
	MOV @R0,#n
	MOV @R1,#n
	MOV R0,#n
	MOV R1,#n
	MOV R2,#n
	MOV R3,#n
	MOV R4,#n
	MOV R5,#n
	MOV R6,#n
	MOV R7,#n

; 8x

	SJMP *
	AJMP $477
	ANL C,ba
	MOVC A,@A+PC
	DIV AB
	MOV da,da
	MOV da,@R0
	MOV da,@R1
	MOV da,R0
	MOV da,R1
	MOV da,R2
	MOV da,R3
	MOV da,R4
	MOV da,R5
	MOV da,R6
	MOV da,R7

; 9x

	MOV DPTR,#$1234
	ACALL $477
	MOV ba,C
	MOVC A,@A+DPTR
	SUBB A,#n
	SUBB A,da
	SUBB A,@R0
	SUBB A,@R1
	SUBB A,R0
	SUBB A,R1
	SUBB A,R2
	SUBB A,R3
	SUBB A,R4
	SUBB A,R5
	SUBB A,R6
	SUBB A,R7

; Ax

	ORL C,/ba
	AJMP $577
	MOV C,ba
	INC DPTR
	MUL AB
	; 5
	MOV @R0,da
	MOV @R1,da
	MOV R0,da
	MOV R1,da
	MOV R2,da
	MOV R3,da
	MOV R4,da
	MOV R5,da
	MOV R6,da
	MOV R7,da

; Bx

	ANL C,/ba
	ACALL $577
	CPL ba
	CPL C
	CJNE A,#n,*
	CJNE A,da,*
	CJNE @R0,#n,*
	CJNE @R1,#n,*
	CJNE R0,#n,*
	CJNE R1,#n,*
	CJNE R2,#n,*
	CJNE R3,#n,*
	CJNE R4,#n,*
	CJNE R5,#n,*
	CJNE R6,#n,*
	CJNE R7,#n,*

; Cx

	PUSH da
	AJMP $677
	CLR ba
	CLR C
	SWAP A
	XCH A,da
	XCH A,@R0
	XCH A,@R1
	XCH A,R0
	XCH A,R1
	XCH A,R2
	XCH A,R3
	XCH A,R4
	XCH A,R5
	XCH A,R6
	XCH A,R7

; Dx

	POP da
	ACALL $677
	SETB ba
	SETB C
	DA A
	DJNZ da,*
	XCHD A,@R0
	XCHD A,@R1
	DJNZ R0,*
	DJNZ R1,*
	DJNZ R2,*
	DJNZ R3,*
	DJNZ R4,*
	DJNZ R5,*
	DJNZ R6,*
	DJNZ R7,*

; Ex

	MOVX A,@DPTR
	AJMP $777
	MOVX A,@R0
	MOVX A,@R1
	CLR A
	MOV A,da
	MOV A,@R0
	MOV A,@R1
	MOV A,R0
	MOV A,R1
	MOV A,R2
	MOV A,R3
	MOV A,R4
	MOV A,R5
	MOV A,R6
	MOV A,R7

; Fx

	MOVX @DPTR,A
	ACALL $777
	MOVX @R0,A
	MOVX @R1,A
	CPL A
	MOV da,A
	MOV @R0,A
	MOV @R1,A
	MOV R0,A
	MOV R1,A
	MOV R2,A
	MOV R3,A
	MOV R4,A
	MOV R5,A
	MOV R6,A
	MOV R7,A

	.DW $1234

	end
